/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Reactor Synchronized Task.
 *
 *      \details  This task shall be created and triggered by User Thread. The Reactor thread will do the real
 *                functionality and provide the result back to user thread.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REACTOR_SYNC_TASK_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REACTOR_SYNC_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <condition_variable>
#include <mutex>
#include <utility>

#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/runtime.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/language/unique_function.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief   Synchronous Reactor Task.
 * \details This task might be created by any thread other than reactor thread. The creator shall then call the
 *          operator() to trigger reactor thread to finish the task. The caller can then wait until reactor thread
 *          finishes his task by calling GetResult.
 *
 * \tparam Result  Return type of the called function.
 *
 * \unit IpcBinding::IpcBindingCore::CoreUtility
 */
template <typename Result>
class ReactorSyncTask final {
 public:
  /*!
   * \brief Type alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Creates a task to synchronize a function call with the reactor thread.
   *
   * \param[in,out] reactor                  The reactor.
   * \param[in]     runtime_processing_mode  The runtime processing mode.
   * \param[in,out] callable                 The function to be executed.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Initialize all the resources of the reactor sync task.
   * - Register reactor software event that calls the task when triggered.
   * - On fail, terminate.
   * \endinternal
   */
  ReactorSyncTask(ReactorInterface& reactor, runtime_configuration::RuntimeProcessingMode const runtime_processing_mode,
                  vac::language::UniqueFunction<Result()>&& callable)
      : runtime_processing_mode_{runtime_processing_mode},
        logger_{logging::kIpcLoggerContextId, logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"ReactorSyncTask"}},
        reactor_{reactor},
        func_{std::move(callable)} {
    if (runtime_processing_mode_ == runtime_configuration::RuntimeProcessingMode::thread_driven) {
      ::amsr::core::Result<ReactorCallbackHandle> register_software_event_result{
          // VCA_IPCB_VALID_REACTOR_LIFECYCLE
          reactor_.RegisterSoftwareEvent([this](ReactorCallbackHandle, ReactorEventTypes) {
            PerformTask();  // VCA_IPCB_REACTORSYNCTASK_LAMBDA_THIS_POINTER_ACCESS

            // Notify the waiting thread about finalized task
            std::lock_guard<std::mutex> const lock_guard{sync_lock_};
            task_done_ = true;
            // VCA_IPCB_CONDITION_VARIABLE_FUNCTION_SAFE
            sync_cv_.notify_one();
          })};

      if (register_software_event_result) {
        reactor_handle_perform_task_ = register_software_event_result.Value();
      } else {
        logger_.TerminateOnViolation(
            "Failed to register reactor software event for reactor sync task.",
            [&register_software_event_result](Logger::LogStream& s) {
              s << " Detailed error: ";
              s << register_software_event_result.Error();  // VCA_IPCB_CHECKED_RESULT_ERROR_ACCESS
              s << ".";
            },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
    }
  }  // COV_IpcBinding_FunctionCoverage_TAR_39344

  /*!
   * \brief Destroy the ReactorSyncTask.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Unregister reactor software event.
   * - On fail, terminate.
   * \endinternal
   */
  ~ReactorSyncTask() noexcept {
    if (runtime_processing_mode_ == runtime_configuration::RuntimeProcessingMode::thread_driven) {
      ::amsr::core::Result<void> const unregister_result{
          reactor_.UnregisterSoftwareEvent(reactor_handle_perform_task_)};  // VCA_IPCB_VALID_REACTOR_LIFECYCLE
      if (!unregister_result) {
        logger_.TerminateOnViolation(
            "Failed to unregister reactor software event for reactor sync task.",
            [&unregister_result](Logger::LogStream& s) {
              s << " Detailed error: ";
              s << unregister_result.Error();  // VCA_IPCB_CHECKED_RESULT_ERROR_ACCESS
              s << ".";
            },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
    }
    // VCA_IPCB_STD_CONDITION_VARIABLE_DESTRUCTOR, VCA_IPCB_STD_FUNCTION_DESTRUCTOR
  }  // COV_IpcBinding_FunctionCoverage_TAR_39344

  ReactorSyncTask(ReactorSyncTask const&) noexcept = delete;
  auto operator=(ReactorSyncTask const&) noexcept -> ReactorSyncTask& = delete;
  ReactorSyncTask(ReactorSyncTask&&) noexcept = delete;
  auto operator=(ReactorSyncTask&&) noexcept -> ReactorSyncTask& = delete;

  /*!
   * \brief   Triggers the task to be executed via the reactor thread and get back its result.
   * \details The caller will be blocked until the task finishes.
   *
   * \return The result of the task, or the default result in case a timeout occurred.
   *
   * \pre         The function/task to be executed must be executed within the max. allowed timeout time 'kTimeout'.
   * \context     ANY except Reactor.
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - If processing mode is kPolling.
   *   - Acquire lock to synchronize with the runtime polling mode context.
   *   - Perform the task.
   * - else
   *   - Start a timer with duration of zero seconds to trigger execution via reactor thread as fast as possible. The
   *     real task will then be executed in the timer expired callback (HandleTimer()) in the desired reactor thread
   *     context.
   * - Wait until the real task was executed within the timer expired callback 'HandleTimer()'.
   * - In case the task execution timed out abort further execution immediately to prevent undefined behaviour.
   * \endinternal
   */
  auto operator()() -> Result {  // COV_IpcBinding_FunctionCoverage_TAR_50909
    if (runtime_processing_mode_ == runtime_configuration::RuntimeProcessingMode::polling) {
      // No reactor thread is running, we must do the job ourselves.
      ::amsr::generic::SingletonAccess<Runtime> runtime{Runtime::getSingletonInstance().GetAccess()};
      std::lock_guard<std::mutex> const lock{runtime->GetPollingModeLock()};
      PerformTask();
    } else {
      task_done_ = false;  // Reset condition_variable condition

      // Trigger reactor thread to do the work via a zero-length timer
      ::amsr::core::Result<void> const trigger_reactor_result{
          reactor_.TriggerSoftwareEvent(reactor_handle_perform_task_)};  // VCA_IPCB_VALID_REACTOR_LIFECYCLE
      if (!trigger_reactor_result) {
        logger_.TerminateOnViolation(
            "Failed to trigger reactor software event for reactor sync task.",
            [&trigger_reactor_result](Logger::LogStream& s) {
              s << " Detailed error: ";
              s << trigger_reactor_result.Error();  // VCA_IPCB_CHECKED_RESULT_ERROR_ACCESS
              s << ".";
            },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }

      // Wait until the task was scheduled and executed via the reactor
      std::unique_lock<std::mutex> sync_unique_lock{sync_lock_};
      // VCA_IPCB_INTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
      bool const wait_result{sync_cv_.wait_for(sync_unique_lock, kTimeout, [this] { return task_done_; })};

      if (!wait_result) {
        // Timeout in task execution occurred.
        // An immediate return will lead to memory corruptions and undefined behaviour in case the scheduled task gets
        // executed after this ReactorSyncTask object was already destroyed.
        // To avoid and endless wait statement we immediately terminate the whole process in this case.

        logger_.TerminateOnViolation("Failed to schedule task via reactor. A timeout occurred.",
                                     Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
    }

    return result_;
  }

 private:
  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used Socal Runtime.
   */
  using Runtime = ::amsr::socal::internal::Runtime;

  /*!
   * \brief Type alias for Reactor callback handle.
   */
  using ReactorCallbackHandle = osabstraction::io::reactor1::CallbackHandle;

  /*!
   * \brief Type alias for Reactor event types.
   */
  using ReactorEventTypes = ::osabstraction::io::reactor1::EventTypes;

  /*!
   * \brief Maximum time to wait for the sync task to be processed.
   */
  static std::chrono::seconds constexpr kTimeout{10};

  /*!
   * \brief Perform the real task.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   *
   * \internal
   * - Execute the task.
   * \endinternal
   */
  void PerformTask() { result_ = func_(); }

  /*!
   * \brief The used runtime processing mode.
   */
  runtime_configuration::RuntimeProcessingMode const runtime_processing_mode_;

  /*!
   * \brief A logger.
   */
  Logger logger_;

  /*!
   * \brief Reference to the reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief Handle of Reactor software event for performing the task.
   */
  ReactorCallbackHandle reactor_handle_perform_task_{};

  /*!
   * \brief A mutex for synchronization of application and reactor thread contexts.
   */
  std::mutex sync_lock_{};

  /*!
   * \brief Condition variable to synchronize application and reactor thread contexts.
   */
  std::condition_variable sync_cv_{};  // VCA_IPCB_CONDITION_VARIABLE_DEFAULT_CONSTRUCTOR

  /*!
   * \brief Condition to synchronize application and reactor thread contexts.
   *
   * \steady FALSE
   */
  bool task_done_{false};

  /*!
   * \brief The functionality to run (executed by reactor).
   */
  vac::language::UniqueFunction<Result()> func_;

  /*!
   * \brief The result, initialized with the error result.
   */
  Result result_{};
};

/*!
 * \brief Provides kTimeout as public constexpr.
 *
 * \tparam Result  Return type of the called function.
 */
template <typename Result>
std::chrono::seconds constexpr ReactorSyncTask<Result>::kTimeout;

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REACTOR_SYNC_TASK_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::ipc_binding_core::internal::service_discovery::ReactorSyncTask::sync_lock_
 * Used for synchronization of application and reactor thread via std::condition_variable.
 *
 * \protects ::amsr::ipc_binding_core::internal::service_discovery::ReactorSyncTask::sync_cv_
 * \usedin ::amsr::ipc_binding_core::internal::service_discovery::ReactorSyncTask::operator()
 * \usedin ::amsr::ipc_binding_core::internal::service_discovery::ReactorSyncTask::ReactorSyncTask.lambda0
 * \exclude -
 * \length LONG Execution of tasks (typically ServiceDiscovery tasks) via reactor thread. Depends on scheduling of the
 *         reactor (reactor thread).
 * \endexclusivearea
 */
// clang-format on

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_IpcBinding_FunctionCoverage_TAR_39344
//   \ACCEPT  XX
//   \REASON  Coverage gap that seems like a tooling issue. Is tracked in TAR-39344.
//
// \ID COV_IpcBinding_FunctionCoverage_TAR_50909
//   \ACCEPT  XX
//   \REASON  Coverage gap that seems like a tooling issue. Is tracked in TAR-50909.
//            It is manually verified that the ReactorSyncTask::operator() function is triggered within CT.
//
// COV_JUSTIFICATION_END

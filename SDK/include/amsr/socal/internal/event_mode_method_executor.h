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
/**        \file  event_mode_method_executor.h
 *        \brief  Concrete implementation of the method executor interface for event mode.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENT_MODE_METHOD_EXECUTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENT_MODE_METHOD_EXECUTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <deque>
#include <memory>
#include <mutex>
#include <queue>

#include "amsr/core/future.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/instance_specifier_lookup_table.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/method_executor_interface.h"
#include "amsr/socal/internal/task.h"
#include "amsr/socal/internal/thread_pools_manager_interface.h"
#include "amsr/socal/internal/types.h"
#include "ara/com/service_identifier.h"
#include "ara/com/types_common.h"
#include "ara/core/memory_resource.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Class implementing the MethodExecutorInterface for event mode.
 *
 * \unit Socal::Skeleton::MethodExecutor::EventModeMethodExecutor
 */
class EventModeMethodExecutor final : public MethodExecutorInterface {  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
 public:
  /*!
   * \brief Construct a new EventModeMethodExecutor object.
   * \param[in] offered_instances               Instances represented by the skeleton object of this method executor.
   * \param[in] service_identifier              ServiceIdentifier from the Skeleton of this method executor.
   * \param[in] thread_pool_id                  The thread pool id from the Skeleton of this method executor.
   * \param[in] thread_pools_manager_interface  Singleton Access of the ThreadPoolsManagerInterface.
   * \param[in] max_queue_size                  The max number of method requests that can be queued. Value '0'
   *                                            indicates unlimited queue size.
   * \pre -
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit EventModeMethodExecutor(
      InstanceSpecifierLookupTableEntryContainer const& offered_instances,
      ::ara::com::ServiceIdentifierType const& service_identifier, ::amsr::core::StringView thread_pool_id,
      ::amsr::generic::SingletonAccess<::amsr::socal::internal::ThreadPoolsManagerInterface>
          thread_pools_manager_interface,
      std::size_t const max_queue_size) noexcept;

  /*!
   * \brief Check consistency of MethodCallProcessingMode and ThreadPool configuration.
   * \param[in] mode                            Method call processing mode to be checked.
   * \param[in] thread_pool_id                  The thread pool id from the Skeleton of this method executor.
   * \param[in] thread_pools_manager_interface  Singleton Access of the ThreadPoolsManagerInterface.
   * \param[in] max_requests                    The max number of method requests that can be queued.
   * \return Result containing no value or an error.
   * \error ComErrc::kExceedQueueSize  If the given max_requests value exceeds the max_number_of_tasks configured for
   *                                   the threadpool.
   * \pre         For kEventSingleThread processing mode, only a single worker thread shall exist to ensure sequential
   *              processing.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  static ::amsr::core::Result<void> CheckMethodCallProcessingModeValidity(
      ::ara::com::MethodCallProcessingMode const mode,
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier thread_pool_id,
      ::amsr::generic::SingletonAccess<::amsr::socal::internal::ThreadPoolsManagerInterface>
          thread_pools_manager_interface,
      std::size_t const max_requests) noexcept;

 private:
  /*!
   * \brief Type alias for Task pointer.
   */
  using TaskUniquePtr = amsr::UniquePtr<Task>;

  /*!
   * \brief Enqueues an incoming method request with its parameters.
   * \param[in] request  This object contains all the information necessary to:
   *                     1. Call the concrete method in the frontend; the binding is aware of the method to call and
   *                        stores this information in the functor.
   *                     2. Get the return value with a given type from the future, because the return type is also
   *                        known to the backend.
   *                     3. Call the response path in the binding-related part with the information provided within this
   *                        object.
   * \return true if a request was enqueued, false if not.
   * \pre         -
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \spec requires request != nullptr; \endspec
   */
  auto HandleMethodRequest(TaskUniquePtr&& request) noexcept -> bool final;

  /*!
   * \brief Execute cleanup of thread-pool tasks which are created by the method executor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \spec requires true; \endspec
   */
  void Cleanup() noexcept final;

  /*!
   * \brief Fetches the next method call from the method queue and executes it.
   * \return Future containing a boolean value of true.
   * \pre         This function must only be called on a skeleton that uses Event-driven method call processing modes.
   * \context     Callback
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   */
  auto ExecuteNextMethodCall() noexcept -> ::amsr::core::Future<bool> final;

  /*!
   * \brief Print infos about the assigned ThreadPool to the log stream.
   * \param[in] offered_instances   Instances represented by the skeleton object of this method executor.
   * \param[in] service_identifier  ServiceIdentifier from the Skeleton of this method executor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void LogThreadPoolAssignment(InstanceSpecifierLookupTableEntryContainer const& offered_instances,
                               ::ara::com::ServiceIdentifierType const& service_identifier) noexcept;

  /*!
   * \brief Identifier of the thread pool assigned to this skeleton.
   */
  ::amsr::core::StringView thread_pool_id_;

  /*!
   * \brief Singleton access of ThreadPoolsManagerInterface.
   */
  ::amsr::generic::SingletonAccess<::amsr::socal::internal::ThreadPoolsManagerInterface> threadpools_manager_interface_;

  /*!
   * \brief Logger for this component.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "EventModeMethodExecutor"_sv};

  /*!
   * \brief Type alias for the buffer that gets filled on incoming method requests.
   */
  using TaskQueueType =
      std::queue<TaskUniquePtr, std::deque<TaskUniquePtr, ::ara::core::PolymorphicAllocator<TaskUniquePtr>>>;

  /*!
   * \brief Skeleton request buffer for all incoming method requests if kPoll is active.
   */
  TaskQueueType request_buffer_{};  // VCA_SOCAL_CALLING_STL_APIS

  /*!
   * \brief Mutex for protection of request_buffer_.
   */
  std::mutex request_buffer_lock_{};

  /*!
   * \brief The max number of method requests that can be queued.
   */
  std::size_t const max_queue_size_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENT_MODE_METHOD_EXECUTOR_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::internal::EventModeMethodExecutor::request_buffer_lock_
 * Used for protecting the request buffer for incoming method requests
 *
 * \protects ::amsr::socal::internal::EventModeMethodExecutor::request_buffer_
 * \usedin ::amsr::socal::internal::EventModeMethodExecutor::HandleMethodRequest
 * \usedin ::amsr::socal::internal::EventModeMethodExecutor::ExecuteNextMethodCall
 * \exclude All other methods of a EventModeMethodExecutor object.
 * \length SHORT adds or removes an element from the request queue
 * \endexclusivearea
 */
// clang-format on

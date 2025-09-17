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
/*!        \file   someip_binding_lite/internal/someip_binding_lite_impl.h
 *         \brief  Direct access provider to SOME/IP bindings core (implementation).
 *         \unit   SomeIpBinding::SomeIpBindingLite::SomeIpBindingLite
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_BINDING_LITE_IMPL_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_BINDING_LITE_IMPL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <chrono>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/connection_action.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/logging/string_stream.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_lite/basic_ipc_address.h"
#include "amsr/someip_binding_lite/data_types.h"
#include "amsr/someip_binding_lite/error_domain.h"
#include "amsr/someip_binding_lite/internal/data_types.h"
#include "amsr/someip_binding_lite/required_instance_id.h"
#include "amsr/someip_binding_lite/required_service_instance.h"
#include "amsr/steady_timer/timer_manager_factory.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/thread/thread.h"

namespace amsr {
namespace someip_binding_lite {
namespace internal {

/*!
 * \brief Direct access provider to SOME/IP bindings core (implementation).
 *
 * \tparam ThreadType Defaults to amsr::thread::Thread, while allowing to mock its behavior for testing.
 */
template <typename ThreadType = amsr::thread::Thread>
class SomeIpBindingLiteImpl final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] uptr_reactor               Unique pointer to an instance of a reactor.
   * \param[in] someip_binding_core   A shared pointer to an instance of SOME/IP binding core.
   * \param[in] someip_daemon_address      SomeIpDaemon BasicIpc address.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Take ownership of the injected instances.
   * - Initialize the logger with a custom prefix based on the IPC address.
   * \endinternal
   */
  SomeIpBindingLiteImpl(std::unique_ptr<internal::ReactorInterfaceType>&& uptr_reactor,
                        std::shared_ptr<internal::SomeIpBindingCoreInterfaceType>&& someip_binding_core,
                        BasicIpcAddress const someip_daemon_address) noexcept
      : atomic_thread_dispatch_{false},
        uptr_reactor_{std::move(uptr_reactor)},
        timer_manager_{},
        someip_binding_core_{std::move(someip_binding_core)},
        logger_{someip_binding_core::internal::logging::kSomeIpLoggerContextId,
                someip_binding_core::internal::logging::kSomeIpLoggerContextDescription,
                someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
                    "SomeIpBindingLiteImpl"_sv, someip_daemon_address.domain, someip_daemon_address.port)} {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    amsr::steady_timer::TimerManagerFactory const timer_manager_factory{};
    amsr::core::Result<std::shared_ptr<amsr::steady_timer::TimerManagerInterface>> timer_manager_result{
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        timer_manager_factory.CreateTimerManager(*uptr_reactor_, osabstraction::time::kClockMonotonic)};

    if (!timer_manager_result.HasValue()) {  // COV_SOMEIPBINDING_SOMEIPBINDINGLITEIMPL_TIMERMANAGER_ERROR
      logger_.LogFatalAndAbort(
          [&timer_manager_result](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Failed to create timer manager: ";
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            s << timer_manager_result.Error().Message();
          },  // COV_SOMEIPBINDING_SOMEIPBINDINGLITEIMPL_CREATION_ERROR_LOG
          static_cast<char const*>(__func__), __LINE__);
    } else if (timer_manager_result.Value() == nullptr) {  // COV_SOMEIPBINDING_SOMEIPBINDINGLITEIMPL_TIMERMANAGER_ERROR
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Osab returned an invalid timer manager!";
          },  // COV_SOMEIPBINDING_SOMEIPBINDINGLITEIMPL_CREATION_ERROR_LOG
          static_cast<char const*>(__func__), __LINE__);
    } else {
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      timer_manager_ = std::move(timer_manager_result.Value());
    }
  }

  /*!
   * \brief Destructor.
   * \pre \p Stop function shall be called to stop SOME/IP binding and the
   *      reactor thread before destroying the instance.
   * \steady FALSE
   *
   * \internal
   * - If reactor's thread is running, abort.
   * \endinternal
   */
  ~SomeIpBindingLiteImpl() noexcept {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    timer_manager_->Deinitialize();
    // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
    timer_manager_.reset();

    if (atomic_thread_dispatch_.load()) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Reactor thread shall be stopped before destruction of the instance.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }  // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_DESTRUCTOR

  SomeIpBindingLiteImpl(SomeIpBindingLiteImpl&&) = delete;
  SomeIpBindingLiteImpl() = delete;
  SomeIpBindingLiteImpl(SomeIpBindingLiteImpl const&) = delete;
  SomeIpBindingLiteImpl& operator=(SomeIpBindingLiteImpl const&) = delete;
  SomeIpBindingLiteImpl& operator=(SomeIpBindingLiteImpl&&) = delete;

  /*!
   * \brief Add service instance to the list of required instances.
   * \param[in] service_id Service ID.
   * \param[in] required_instance_id Required instance ID.
   * \param[in] major_interface_version Major interface version.
   * \param[in] minor_interface_version Minor interface version.
   *
   * \return \p amsr::core::Result with an instance of "RequiredServiceInstance" or an error.
   * \error No error is returned.
   *
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Request a required service and return a communication instance.
   * - Return an error in the case of failure.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SOMEIPBINDING_VectorC++-V3.9.2_builtin_types_false_positive
  amsr::core::Result<::amsr::someip_binding_lite::RequiredServiceInstance> CreateRequiredServiceInstance(
      ServiceId const service_id, RequiredInstanceId const& required_instance_id,
      MajorInterfaceVersion const major_interface_version,
      MinorInterfaceVersion const minor_interface_version) const noexcept {
    logger_.LogDebug(
        [&service_id, &required_instance_id, &major_interface_version,
         &minor_interface_version](ara::log::LogStream& s) noexcept -> void {
          s << "Creating required service instance";
          s << " (service ID: ";
          s << service_id;
          s << ", required instance ID: ";
          s << required_instance_id.Value();
          s << ", major interface version: ";
          s << major_interface_version;
          s << ", minor interface version: ";
          s << minor_interface_version;
          s << ").";
        },
        static_cast<char const*>(__func__), __LINE__);

    amsr::core::Result<RequiredServiceInstance> result{SomeIpBindingLiteErrc::kUnknown};

    someip_protocol::internal::InstanceId const instance_id{required_instance_id.Value()};

    ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const required_service_instance_id{
        service_id, major_interface_version, minor_interface_version, instance_id};

    someip_binding_core::internal::ServiceInformation service_information{};

    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    someip_binding_core_->EmplaceRequiredServiceInstance(required_service_instance_id, std::move(service_information));

    amsr::core::Result<::amsr::someip_binding_core::internal::RequiredServiceInstance> core_rsi_wrapper_request_result{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        someip_binding_core_->RequestRequiredServiceInstance(required_service_instance_id)};

    if (!core_rsi_wrapper_request_result.HasValue()) {
      logger_.LogFatalAndAbort(
          [&required_service_instance_id](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Unable to request required service instance from the SOME/IP binding core.";
            ::amsr::someip_binding_core::internal::logging::LogBuilder::LogRequiredServiceInstanceIdToStringStream(
                s, required_service_instance_id);
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    // Create a SOME/IP Binding Lite RSI implementation instance to wrap the requested SOME/IP Binding Core
    // RSI wrapper.
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
    std::shared_ptr<::amsr::someip_binding_lite::internal::RequiredServiceInstanceImpl> sptr_implementation{
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        std::make_shared<::amsr::someip_binding_lite::internal::RequiredServiceInstanceImpl>(
            std::move(core_rsi_wrapper_request_result.Value()), required_service_instance_id)};

    // Create a SOME/IP Binding Lite RSI wrapper class instance and move the implementation into it.
    result.EmplaceValue(std::move(sptr_implementation));

    return result;
  }

  /*!
   * \brief Start the communication.
   *
   * \return \p amsr::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeThreadCreationFailed If thread creation fails.
   * \error SomeIpBindingLiteErrc::kRuntimeResourceOutOf If out of system resources.
   * \error SomeIpBindingLiteErrc::kSystemConfigurationInvalid If system configuration is invalid.
   * \error SomeIpBindingLiteErrc::kSystemPrivilegesNoAccess If system privileges do not provide resource access.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre SOME/IP daemon shall be running and shall accept connection requests.
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Initialize the status to indicate an error.
   * - Try creating reactor's thread.
   * - If thread creation is successful,
   *   - Try to connect to the SomeIpDaemon. Abort on failure.
   *   - Update the status to indicate success.
   * - Else
   *   - Update the status with the new error.
   * - Return the status.
   * \endinternal
   */
  amsr::core::Result<void> Start() noexcept {
    amsr::core::Result<void> result{SomeIpBindingLiteErrc::kUnknown};

    atomic_thread_dispatch_.store(true);

    // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
    // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
    amsr::core::Result<ThreadType> result_thread_creation{ThreadType::Create([this]() { ThreadDispatch(); })};

    if (result_thread_creation.HasValue()) {
      logger_.LogDebug([](ara::log::LogStream& s) noexcept -> void { s << "Spawned reactor thread."; },
                       static_cast<char const*>(__func__), __LINE__);

      reactor_thread_ = std::move(result_thread_creation.Value());

      // Connection retrial is always disabled in SomeIpBindingLite due to no access to the "someip_config.json".
      constexpr bool kConnectionRetrialEnabled{false};
      constexpr std::chrono::milliseconds kConnectionRetrialIntervalMs{500};

      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      someip_binding_core::internal::ConnectionAction connect{someip_binding_core_, timer_manager_.get(),
                                                              kConnectionRetrialEnabled, kConnectionRetrialIntervalMs};

      // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
      ::amsr::core::Result<void> const connection_result{connect.TryConnect()};

      if (!connection_result.HasValue()) {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "Connection establishment between SOME/IP daemon client and SOME/IP daemon failed.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }

      result.EmplaceValue();  // Clear the error.
    } else {
      amsr::core::ErrorCode const& error_code{result_thread_creation.Error()};
      result.EmplaceError(TranslateErrorCodeThreadErrc(error_code));

      // Thread was not started, set dispatch to false.
      atomic_thread_dispatch_.store(false);

      logger_.LogError(
          [&error_code](ara::log::LogStream& s) noexcept -> void {
            s << "Failed to create a new reactor thread";
            s << " (error value: ";
            s << error_code.Value();
            s << ", error message: ";
            s << error_code.Message();
            s << ", user message: ";
            s << error_code.UserMessage();
            s << ", support data: ";
            s << error_code.SupportData();
            s << ").";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Stop the communication.
   *
   * \return \p amsr::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeThreadJoiningFailed If thread joining fails.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre \p Start function shall execute without returning an error.
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Initialize the status to indicate an error.
   * - If reactor's thread is not running,
   *   - Abort.
   * - Try joining the reactor's thread.
   * - If joining is a success,
   *   - Update the status to indicate success.
   * - Else
   *   - Abort.
   * - Return the status.
   * \endinternal
   */
  amsr::core::Result<void> Stop() noexcept {
    amsr::core::Result<void> result{SomeIpBindingLiteErrc::kUnknown};

    if (!atomic_thread_dispatch_.load()) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Cannot stop if not started; reactor thread is not running.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    atomic_thread_dispatch_.store(false);

    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    uptr_reactor_->Unblock();

    // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
    amsr::core::Result<void> const result_thread_joining{reactor_thread_.Join()};

    if (result_thread_joining.HasValue()) {
      logger_.LogDebug([](ara::log::LogStream& s) noexcept -> void { s << "Successfully joined the reactor thread."; },
                       static_cast<char const*>(__func__), __LINE__);

      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      someip_binding_core_->StopServiceDiscovery();

      result.EmplaceValue();  // Clear the error.
    } else {
      amsr::core::ErrorCode const& error_code{result_thread_joining.Error()};

      logger_.LogFatalAndAbort(
          [&error_code](::amsr::someip_binding_core::internal::logging::StringStream& s) noexcept -> void {
            s << "Failed to join the reactor thread";
            s << " (error value: ";
            s << error_code.Value();
            s << ", error message: ";
            s << error_code.Message();
            s << ", user message: ";
            s << error_code.UserMessage();
            s << ", support data: ";
            s << error_code.SupportData();
            s << ").";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

 private:
  /*!
   * \brief Handle incoming events on the reactor.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Construct an amsr::result to set the name for the reactor thread.
   * - Log whether the construction of the thread name was successful or not.
   * - Keep on handling events via the reactor's thread
   *   as long as the thread execution control flag is true.
   * \endinternal
   */
  void ThreadDispatch() const noexcept {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    using vac::container::literals::operator""_sv;
    vac::container::CStringView const thread_name_{"vCsoLite"_sv};
    // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE
    amsr::core::Result<void> const result{amsr::thread::SetName(thread_name_)};

    if (!result.HasValue()) {  // COV_THREAD_SETNAME_RESULT
      logger_.LogError(
          [](ara::log::LogStream& s) { s << "Unable to set name of reactor thread."; },  // COV_LOG_ERROR_SETNAME
          static_cast<char const*>(__func__), __LINE__);
    }

    while (atomic_thread_dispatch_.load()) {
      std::chrono::nanoseconds const timeout{std::chrono::nanoseconds::max()};
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      static_cast<void>(uptr_reactor_->HandleEvents(timeout));
    }
  }

  /*!
   * \brief Flag to represent if the reactor thread is active or not.
   */
  std::atomic_bool atomic_thread_dispatch_;

  /*!
   * \brief Unique pointer to an instance of reactor.
   */
  std::unique_ptr<internal::ReactorInterfaceType> uptr_reactor_;

  /*!
   * \brief Shared pointer to the timer manager.
   */
  std::shared_ptr<::amsr::steady_timer::TimerManagerInterface> timer_manager_;

  /*!
   * \brief Shared pointer to an instance of SOME/IP binding core.
   */
  std::shared_ptr<internal::SomeIpBindingCoreInterfaceType> someip_binding_core_;

  /*!
   * \brief Thread to handle events on the reactor.
   */
  // VCA_SOMEIPBINDING_UNKNOWN_CONSTRUCTOR_CALLED_FOR_MOCK
  ThreadType reactor_thread_{};

  /*!
   * \brief The logger.
   */
  someip_binding_core::internal::logging::AraComLogger const logger_;
};

}  // namespace internal
}  // namespace someip_binding_lite
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_THREAD_SETNAME_RESULT
//   \ACCEPT  XF
//   \REASON  Branch cannot be tested by unit test as it depends on failure of amsr::thread::SetName.
//
// \ID COV_LOG_ERROR_SETNAME
//   \ACCEPT  XX
//   \REASON  Branch cannot be tested by unit test as it depends on failure of amsr::thread::SetName.
//
// \ID COV_SOMEIPBINDING_SOMEIPBINDINGLITEIMPL_TIMERMANAGER_ERROR
//   \ACCEPT  XF
//   \REASON  Error path can not be tested because the TimerManager is instantioned through the TimerManagerFactory in
//            the constructor, mocking is not possible. Timer is not used in SomeIpBindingLite, only need to fulfill the
//            API calls.
//
// \ID COV_SOMEIPBINDING_SOMEIPBINDINGLITEIMPL_CREATION_ERROR_LOG
//   \ACCEPT  XX
//   \REASON  The logger will be called in the error path for the TimerManager creation. Therefore it can not be tested
//            related to the first justification.
// COV_JUSTIFICATION_END

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_BINDING_LITE_IMPL_H_

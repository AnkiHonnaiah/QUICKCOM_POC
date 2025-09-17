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
/**        \file  thread_driven_proxy.h
 *        \brief  Thread-Driven proxy base class of a specific service interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_THREAD_DRIVEN_PROXY_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_THREAD_DRIVEN_PROXY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>

#include "amsr/core/abort.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/lifecycle.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/methods/event_mode_future_executor.h"
#include "amsr/socal/internal/thread_driven_find_service_handler.h"
#include "amsr/socal/internal/thread_pools_manager.h"
#include "amsr/socal/polling_proxy.h"
#include "amsr/socal/proxy.h"
#include "ara/com/find_service_handle.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/types_common.h"
#include "ara/core/instance_specifier.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {

/*!
 * \brief   This class represents a specific ServiceInterface Polling Proxy.
 * \details The following APIs are not considered to be threadsafe against each other, or against any other API in this
 *          instance:
 *          - FindService(),
 *          - StartFindService(),
 *          - StopFindService()
 * \tparam ServiceInterface       The corresponding ServiceInterface.
 * \tparam ProxyBackendInterface  The proxy backend interface the proxy communicates to.
 * \tparam ProxyHandleType        The corresponding ServiceInterface HandleType.
 *
 * \unit Socal::Proxy::Proxy
 */
template <typename ServiceInterface, typename ProxyBackendInterface, typename ProxyHandleType>
class ThreadDrivenProxy : public PollingProxy<ServiceInterface, ProxyBackendInterface, ProxyHandleType> {
 private:
  /*!
   * \brief Type alias for the base HandleType class.
   */
  using BaseHandleType = ::amsr::socal::HandleType;

 public:
  /*!
   * \brief Type alias for the base Proxy type.
   */
  using BaseType = PollingProxy<ServiceInterface, ProxyBackendInterface, ProxyHandleType>;

  /*!
   * \brief Type alias for the proxy specific HandleType.
   *
   * \trace SPEC-4980259, SPEC-4980344
   */
  using HandleType = ProxyHandleType;

  /*!
   * \brief Type alias for ThreadDrivenFindServiceHandler specialization.
   */
  using ThreadDrivenFindServiceHandlerType = ::amsr::socal::internal::ThreadDrivenFindServiceHandler<ProxyHandleType>;

  /*!
   * \brief Use base class constructors.
   */
  using BaseType::PollingProxy;

  /*!
   * \brief Type alias for the FutureExecutor interface type.
   */
  using FutureExecutorInterfaceType = typename BaseType::FutureExecutorInterfaceType;

  /*!
   * \brief Type alias for the EventModeFutureExecutor type.
   */
  using EventModeFutureExecutorType = ::amsr::socal::internal::methods::EventModeFutureExecutor;

  /*!
   * \brief Delete default constructor.
   */
  ThreadDrivenProxy() noexcept = delete;

  /*!
   * \brief Delete the copy constructor.
   */
  ThreadDrivenProxy(ThreadDrivenProxy const&) noexcept = delete;

  /*!
   * \brief Delete the default copy assignment operator.
   */
  ThreadDrivenProxy& operator=(ThreadDrivenProxy const&) & noexcept = delete;

  /*!
   * \brief Delete the default move operator.
   */
  ThreadDrivenProxy(ThreadDrivenProxy&& other) noexcept = delete;

  /*!
   * \brief Delete the default move assignment operator.
   */
  ThreadDrivenProxy& operator=(ThreadDrivenProxy&&) & noexcept = delete;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  /*!
   * \brief   Start an asynchronous FindService notification about service updates.
   * \details This API will not be available if 'araComRuntimeProcessingMode' is configured as 'Polling'. Note that
   *          execution of the provided handler is done within Socal's R-Port default thread-pool context. Therefore,
   *          it is strongly advised to reduce the blocking calls from the handler as much as possible to allow the
   *          R-Port default thread-pool to process other pending tasks. Incompliance with this may have undesired
   *          consequences (e.g., delayed processing of communication requests).
   * \param[in] handler   Gets called upon detection of a matching service. The callback handler is provided with the
   *                      FindServiceHandle which can be used to trigger StopFindService() API from within the handler.
   *                      The given handler shall not be null and must be valid at least until StopFindService() is
   *                      called with the corresponding FindServiceHandle.
   *                      Any exception thrown by the callback will lead to termination through "std::terminate()".
   * \param[in] instance  The instance identifier of the required service instance that needs to be searched.
   * \return FindServiceHandle for this search/find request, which is needed to stop the service availability monitoring
   *         and related firing of the given handler.
   * \pre         The given instance identifier must be configured in the ARXML model.
   * \pre         The given instance identifier must belong to the service interface.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Validate the provided handler.
   * - Validate Socal initialization status.
   * - Resolve the provided instance identifier from the internal look-up table.
   * - If the instance identifier is not resolved:
   *   - Log a fatal message and abort the application.
   * - Else (if the instance identifier is resolved):
   *   - Create an internal handler that
   *     - Transforms the provided handles from the base into the proxy-specific HandleType.
   *     - Calls the provided handler by passing the matching service instance and handle.
   *   - Invoke the StartFindService method of ThreadDrivenFindServiceHandler.
   * - Return the FindServiceHandle for this search/find request.
   *
   * Calls "amsr::core::Abort()" if:
   * - \p Socal is not initialized.
   * - \p handler is "nullptr".
   * - \p instance cannot be resolved.
   * \endinternal
   *
   * \trace SPEC-4980368
   */
  static ::ara::com::FindServiceHandle StartFindService(::ara::com::FindServiceHandlerExt<HandleType> handler,
                                                        ::ara::com::InstanceIdentifier instance) noexcept {
    ValidateFindServiceHandler(handler);  // VCA_SOCAL_PASSING_POINTER_TO_ANOTHER_FUNCTION_FOR_VALIDATION
    // VECTOR NL AutosarC++17_10-M5.2.12: MD_Socal_AutosarC++17_10-M5.2.12_array_to_pointer_decay
    ValidateIfComponentInitialized(__func__);

    // VCA_SOCAL_CALLING_STL_APIS
    internal::InstanceSpecifierLookupTableEntryContainer const service_instances{
        // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION
        BaseType::ResolveInstanceSpecifierMapping(instance)};

    // VCA_SOCAL_CALLING_STL_APIS
    return ThreadDrivenFindServiceHandlerType::StartFindService(
        // VECTOR NC AutosarC++17_10-A5.1.5: MD_Socal_AutosarC++17_10-A5.1.5_FalsePositive
        // VCA_SOCAL_CALLING_STL_APIS
        [handler](::ara::com::ServiceHandleContainer<BaseHandleType> service_handle_container,
                  ::ara::com::FindServiceHandle find_service_handle) {
          ::ara::com::ServiceHandleContainer<HandleType> concrete_handles{};
          concrete_handles.reserve(service_handle_container.size());  // VCA_SOCAL_CALLING_STL_APIS

          // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_ACCESSING_TEMPORARY_REFERENCE_OR_ITERATOR
          static_cast<void>(std::for_each(service_handle_container.cbegin(), service_handle_container.cend(),
                                          [&concrete_handles](BaseHandleType const& service_handle) {
                                            // VCA_SOCAL_CALLING_STL_APIS
                                            concrete_handles.emplace_back(service_handle.GetRequiredInstanceId(),
                                                                          service_handle.GetProvidedInstanceId());
                                          }));

          handler(  // VCA_SOCAL_CALLING_STL_APIS
              std::move(concrete_handles),
              find_service_handle);  // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION
        },
        service_instances, BaseType::GetServiceDiscoveryInternal());
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  /*!
   * \brief   Start an asynchronous FindService notification about service updates.
   * \details This API will not be available if 'araComRuntimeProcessingMode' is configured as 'Polling'. Note that
   *          execution of the provided handler is done within Socal's R-Port default thread-pool context. Therefore,
   *          it is strongly advised to reduce the blocking calls from the handler as much as possible to allow the
   *          R-Port default thread-pool to process other pending tasks. Incompliance with this may have undesired
   *          consequences (e.g., delayed processing of communication requests).
   * \param[in] handler   Gets called upon detection of a matching service. The callback handler is provided with the
   *                      FindServiceHandle which can be used to trigger StopFindService() API from within the handler.
   *                      The given handler shall not be null and must be valid at least until StopFindService() is
   *                      called with the corresponding FindServiceHandle.
   *                      Any exception thrown by the callback will lead to termination through "std::terminate()".
   * \param[in] instance  The instance specifier of the required service interface, for which a matching service
   *                      instance should be searched.
   * \return FindServiceHandle for this search/find request, which is needed to stop the service availability monitoring
   *         and related firing of the given handler.
   * \pre         The given instance specifier must be configured in the ARXML model.
   * \pre         The given instance specifier must belong to the service interface.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Validate the provided handler.
   * - Validate Socal initialization status.
   * - Resolve the provided instance specifier from the internal look-up table.
   * - If the instance specifier is not resolved:
   *   - Log a fatal message and abort the application.
   * - Else (if the instance identifier is resolved):
   *   - Create an internal handler that
   *     - Transforms the provided handles from the base into the proxy-specific HandleType.
   *     - Calls the provided handler by passing the matching service instance and handle.
   *   - Invoke the StartFindService method of ThreadDrivenFindServiceHandler.
   * - Return the FindServiceHandle for this search/find request.
   *
   * Calls "amsr::core::Abort()" if:
   * - \p Socal is not initialized.
   * - \p handler is "nullptr".
   * - \p instance cannot be resolved.
   * \endinternal
   *
   * \trace SPEC-4980368
   */
  static ::ara::com::FindServiceHandle StartFindService(::ara::com::FindServiceHandlerExt<HandleType> handler,
                                                        ::ara::core::InstanceSpecifier instance) noexcept {
    ValidateFindServiceHandler(handler);  // VCA_SOCAL_PASSING_POINTER_TO_ANOTHER_FUNCTION_FOR_VALIDATION
    // VECTOR NL AutosarC++17_10-M5.2.12: MD_Socal_AutosarC++17_10-M5.2.12_array_to_pointer_decay
    ValidateIfComponentInitialized(__func__);

    internal::InstanceSpecifierLookupTableEntryContainer const service_instances{
        BaseType::ResolveInstanceSpecifierMapping(instance)};

    // VCA_SOCAL_CALLING_STL_APIS
    return ThreadDrivenFindServiceHandlerType::StartFindService(
        // VECTOR NC AutosarC++17_10-A5.1.5: MD_Socal_AutosarC++17_10-A5.1.5_FalsePositive
        // VCA_SOCAL_CALLING_STL_APIS
        [handler](::ara::com::ServiceHandleContainer<BaseHandleType> service_handle_container,
                  ::ara::com::FindServiceHandle find_service_handle) {
          ::ara::com::ServiceHandleContainer<HandleType> concrete_handles{};
          concrete_handles.reserve(service_handle_container.size());  // VCA_SOCAL_CALLING_STL_APIS

          // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_ACCESSING_TEMPORARY_REFERENCE_OR_ITERATOR
          static_cast<void>(std::for_each(service_handle_container.cbegin(), service_handle_container.cend(),
                                          [&concrete_handles](BaseHandleType const& service_handle) {
                                            // VCA_SOCAL_CALLING_STL_APIS
                                            concrete_handles.emplace_back(service_handle.GetRequiredInstanceId(),
                                                                          service_handle.GetProvidedInstanceId());
                                          }));

          handler(  // VCA_SOCAL_CALLING_STL_APIS
              std::move(concrete_handles),
              find_service_handle);  // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION
        },
        service_instances, BaseType::GetServiceDiscoveryInternal());
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  /*!
   * \brief   Start an asynchronous FindService notification about service updates.
   * \details This API will not be available if 'araComRuntimeProcessingMode' is configured as 'Polling'. Note that
   *          execution of the provided handler is done within Socal's R-Port default thread-pool context. Therefore,
   *          it is strongly advised to reduce the blocking calls from the handler as much as possible to allow the
   *          R-Port default thread-pool to process other pending tasks. Incompliance with this may have undesired
   *          consequences (e.g., delayed processing of communication requests).
   * \param[in] handler   Gets called upon detection of a matching service. The given handler shall not be null and
   *                      must be valid at least until StopFindService() is called with the corresponding
   *                      FindServiceHandle.
   *                      Any exception thrown by the callback will lead to termination through "std::terminate()".
   * \param[in] instance  The instance identifier of the required service instance that needs to be searched.
   * \return FindServiceHandle for this search/find request, which is needed to stop the service availability monitoring
   *         and related firing of the given handler.
   * \pre         The given instance identifier must be configured in the ARXML model.
   * \pre         The given instance identifier must belong to the service interface.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Validate the provided handler.
   * - Create a wrapper handler of type FindServiceHandlerExt which internally calls the provided handler function.
   * - Call StartFindService() overload by providing the wrapper handler and the instance identifier as arguments.
   *
   * Calls "amsr::core::Abort()" if:
   * - \p handler is "nullptr".
   * \endinternal
   *
   * \trace SPEC-4980368
   */
  static ::ara::com::FindServiceHandle StartFindService(::ara::com::FindServiceHandler<HandleType> handler,
                                                        ::ara::com::InstanceIdentifier instance) noexcept {
    ValidateFindServiceHandler(handler);  // VCA_SOCAL_PASSING_POINTER_TO_ANOTHER_FUNCTION_FOR_VALIDATION

    return StartFindService(  // VCA_SOCAL_CALLING_STL_APIS
        // VCA_SOCAL_CALLING_STL_APIS
        [handler](::ara::com::ServiceHandleContainer<HandleType> service_handle_container,
                  ::ara::com::FindServiceHandle) {
          // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT
          handler(std::move(service_handle_container));
        },
        instance);  // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  /*!
   * \brief   Start an asynchronous FindService notification about service updates.
   * \details This API will not be available if 'araComRuntimeProcessingMode' is configured as 'Polling'. Note that
   *          execution of the provided handler is done within Socal's R-Port default thread-pool context. Therefore,
   *          it is strongly advised to reduce the blocking calls from the handler as much as possible to allow the
   *          R-Port default thread-pool to process other pending tasks. Incompliance with this may have undesired
   *          consequences (e.g., delayed processing of communication requests).
   * \param[in] handler   Gets called upon detection of a matching service. The given handler shall not be null and
   *                      must be valid at least until StopFindService() is called with the corresponding
   *                      FindServiceHandle.
   *                      Any exception thrown by the callback will lead to termination through "std::terminate()".
   * \param[in] instance  The instance specifier of the required service interface, for which a matching service
   *                      instance should be searched.
   * \return FindServiceHandle for this search/find request, which is needed to stop the service availability monitoring
   *         and related firing of the given handler.
   * \pre         The given instance specifier must be configured in the ARXML model.
   * \pre         The given instance specifier must belong to the service interface.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Validate the provided handler.
   * - Create a wrapper handler of type FindServiceHandlerExt which internally calls the provided handler function.
   * - Call StartFindService() overload by providing the wrapper handler and the instance specifier as arguments.
   *
   * Calls "amsr::core::Abort()" if:
   * - \p handler is "nullptr".
   * \endinternal
   *
   * \trace SPEC-4980368
   */
  static ::ara::com::FindServiceHandle StartFindService(::ara::com::FindServiceHandler<HandleType> handler,
                                                        ::ara::core::InstanceSpecifier instance) noexcept {
    ValidateFindServiceHandler(handler);  // VCA_SOCAL_PASSING_POINTER_TO_ANOTHER_FUNCTION_FOR_VALIDATION

    return StartFindService(  // VCA_SOCAL_CALLING_STL_APIS
        // VCA_SOCAL_CALLING_STL_APIS
        [handler](::ara::com::ServiceHandleContainer<HandleType> service_handle_container,
                  ::ara::com::FindServiceHandle) {
          // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT
          handler(std::move(service_handle_container));
        },
        instance);  // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  /*!
   * \brief   To stop receiving further notifications the ServiceProxy class shall provide a StopFindService method.
   * \details This API will not be available if 'araComRuntimeProcessingMode' is configured as 'Polling'.
   *          Calling this API from an application thread (i.e., from outside the FindServiceHandler callback) may lead
   *          to a deadlock between the calling thread and the ara::com thread handling the call to FindServiceHandler,
   *          if any user mutex is locked in the calling thread, as well as in the FindServiceHandler callback.
   * \param[in] handle  The FindServiceHandle object obtained by the call to the StartFindService API. The user shall
   *                    ensure that a valid FindServiceHandle object is passed to the StopFindService API.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same FindServiceHandle instance, TRUE for different instances.
   * \reentrant   FALSE for same FindServiceHandle instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Validate Socal initialization status.
   * - Invokes the StopFindService method of ThreadDrivenFindServiceHandler.
   *
   * Calls "amsr::core::Abort()" if:
   * - Socal is not initialized.
   * \endinternal
   *
   * \trace SPEC-4980370, SPEC-5951164
   */
  static void StopFindService(::ara::com::FindServiceHandle handle) noexcept {
    // VECTOR NL AutosarC++17_10-M5.2.12: MD_Socal_AutosarC++17_10-M5.2.12_array_to_pointer_decay
    ValidateIfComponentInitialized(__func__);
    ThreadDrivenFindServiceHandlerType::StopFindService(handle);  // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOCAL_M0.1.8_VoidFunctionHasNoExternalSideEffectButTriggersAnotherOperation
  /*!
   * \brief Creates the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Create the singleton instances of Base class.
   * - Create the singleton instances of ThreadDrivenFindServiceHandler.
   * \endinternal
   */
  static void CreateSingletons() noexcept {
    BaseType::CreateSingletons();
    ThreadDrivenFindServiceHandlerType::CreateSingletons();
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOCAL_M0.1.8_VoidFunctionHasNoExternalSideEffectButTriggersAnotherOperation
  /*!
   * \brief Destroys the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Destroy the singleton instances of ThreadDrivenFindServiceHandler.
   * - Destroy the singleton instances of Base class.
   * \endinternal
   */
  static void DestroySingletons() noexcept {
    ThreadDrivenFindServiceHandlerType::DestroySingletons();
    BaseType::DestroySingletons();
  }

 protected:
  /*!
   * \brief Use default destructor.
   * \pre        -
   * \context    App
   * \threadsafe FALSE for same class instance, TRUE for different instances.
   * \reentrant  FALSE for same class instance, TRUE for different instances.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~ThreadDrivenProxy() noexcept = default;  // VCA_SOCAL_DESTRUCTOR_STATIC_MEMORY

  /*!
   * \brief   Get the future executor instance.
   * \details The returned future executor is responsible for scheduling the execution of callbacks registered in
   *          context of ara::core::Future::then() on a proxy method or field get/set requests, in event-driven mode.
   * \return The future executor instance.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   */
  std::shared_ptr<FutureExecutorInterfaceType> GetFutureExecutorInstance() const noexcept final {
    // VECTOR NC AutosarC++17_10-M9.3.1: MD_SOCAL_AutosarC++17_10-M9.3.1_nonConstPointerOrReference
    return event_mode_future_executor_;
  }

 private:
  /*!
   * \brief Validate that the component is initialized.
   * \param[in] function_name  The name of the calling function (used for logging purpose).
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If Socal is not initialized:
   *   - Log fatal message and abort the application.
   *
   * - Calls "amsr::core::Abort()" if:
   *   - Socal is not initialized.
   * \endinternal
   */
  static void ValidateIfComponentInitialized(::amsr::core::String const& function_name) noexcept {
    if (!internal::IsComInitialized()) {
      ::amsr::core::String const error_msg{"Proxy::" + function_name + ": API called before Socal is initialized."};

      LogAndAbort(error_msg.c_str());
    }
  }

  /*!
   * \brief Make sure that the provided handler to StartFindService is a valid functor object.
   * \param[in] find_service_handler  The handler functor provided to StartFindService().
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If the handler is not a valid functor (nullptr):
   *   - Log fatal message and abort the application.
   *
   * - Calls "amsr::core::Abort()" if:
   *   - 'find_service_handler' is nullptr.
   * \endinternal
   */
  static void ValidateFindServiceHandler(
      ::ara::com::FindServiceHandler<HandleType> const& find_service_handler) noexcept {
    if (find_service_handler == nullptr) {
      ::amsr::core::StringView const error_msg{
          "StartFindService() called with handler as nullptr. Please use a valid handler function."};

      LogAndAbort(error_msg);
    }
  }

  /*!
   * \brief Make sure that the provided handler to StartFindService is a valid functor object.
   * \param[in] find_service_handler  The extended version of the handler functor provided to StartFindService().
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If the handler is not a valid functor (nullptr):
   *   - Log fatal message and abort the application.
   *
   * - Calls "amsr::core::Abort()" if:
   *   - 'find_service_handler' is nullptr.
   * \endinternal
   */
  static void ValidateFindServiceHandler(
      ::ara::com::FindServiceHandlerExt<HandleType> const& find_service_handler) noexcept {
    if (find_service_handler == nullptr) {
      ::amsr::core::StringView const error_msg{
          "StartFindService() called with handler as nullptr. Please use a valid handler function."};

      LogAndAbort(error_msg);
    }
  }

  /*!
   * \brief Log the provided message and trigger an abort.
   * \param[in] log_message  The message to be logged.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Log fatal message and abort the application.
   * \endinternal
   */
  static void LogAndAbort(::amsr::core::StringView const& log_message) noexcept {
    using vac::container::literals::operator""_sv;
    ::amsr::socal::internal::logging::AraComLogger const logger{
        ::amsr::socal::internal::logging::kAraComLoggerContextId,
        ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Proxy"_sv};
    logger.LogFatalAndAbort(
        [&log_message](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
          // VCA_SOCAL_CALLING_STL_APIS
          s << log_message.ToString();
        },
        static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief Executor for callbacks registered in context of ara::core::Future::then() on a proxy method or field get/set
   *        requests, in event-driven mode.
   */
  std::shared_ptr<FutureExecutorInterfaceType> event_mode_future_executor_{
      // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
      std::make_shared<EventModeFutureExecutorType>(
          static_cast<EventModeFutureExecutorType::ThreadPoolsManagerInterfaceAccessType>(
              ::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance().GetAccess()))};
};

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_THREAD_DRIVEN_PROXY_H_

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
/**        \file  generic_proxy_event.h
 *        \brief  Generic proxy event implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_EVENTS_GENERIC_PROXY_EVENT_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_EVENTS_GENERIC_PROXY_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstddef>
#include <limits>
#include <mutex>
#include <string>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_sample_ptr.h"
#include "amsr/socal/internal/events/generic_proxy_event_backend_interface.h"
#include "amsr/socal/internal/events/proxy_event_sync_interface.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/types_common.h"
#include "ara/core/result.h"
#include "vac/language/cpp17_backport.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace socal {
namespace events {

/*!
 * \brief Generic proxy event implementation.
 */
class AMSR_LG_BETA GenericProxyEvent final : public ::amsr::socal::internal::events::ProxyEventSyncInterface {
  /*!
   * \brief Type alias for generic proxy event backend.
   */
  using GenericProxyEventBackend = ::amsr::socal::internal::events::GenericProxyEventBackendInterface;

  /*!
   * \brief Sample data containing the memory pointer and the cache pointer.
   */
  using SampleData = typename GenericProxyEventBackend::SampleData;

  /*!
   * \brief Type alias for internal LogStream.
   */
  using LogStream = ::amsr::socal::internal::logging::LogStream;

  /*!
   * \brief Wrapper-Type for the ::ara::com::EventReceiveHandler.
   */
  using ProxyEventReceiveHandler = ::vac::language::UniqueFunction<void()>;

  /*!
   * \brief Alias for a pair that temporary holds updated ProxyEventReceiveHandlers.
   */
  using ReceiveHandlerPair = std::pair<bool, ProxyEventReceiveHandler>;

  /*!
   * \brief Wrapper-Type for the ::ara::com::SubscriptionStateChangeHandler.
   */
  using ProxyEventSubscriptionHandler = ::vac::language::UniqueFunction<void(::ara::com::SubscriptionState const)>;

  /*!
   * \brief Alias for a pair that temporary holds the updated subscription state change handler.
   */
  using SubscriptionHandlerPair = std::pair<bool, ProxyEventSubscriptionHandler>;

 public:
  /*!
   * \brief Type alias for generic sample pointer.
   */
  using GenericSamplePtr = ::amsr::socal::GenericSamplePtr;

  /*!
   * \brief Type alias for subscription return type.
   */
  using SubscriptionResult = ::ara::core::Result<void>;

  /*!
   * \brief Type alias for GetNewSamples return type.
   */
  using GetNewSamplesResult = ::ara::core::Result<std::size_t>;

  /*!
   * \brief Constructs a generic proxy event.
   * \param[in] event_backend  The corresponding event backend. The given reference must be valid at least until the
   *                           lifetime of this object.
   * \param[in] service_name   The shortname path of the service interface this event belongs to.
   * \param[in] event_name     The event name configured in the model.
   * \param[in] instance_id    The instance identifier of the corresponding proxy instance.
   * \param[in] prefix         The prefix for the logger.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  GenericProxyEvent(GenericProxyEventBackend& event_backend, std::string const& service_name,
                    std::string const& event_name, ::ara::com::InstanceIdentifier const& instance_id,
                    std::string const& prefix = "GenericProxyEvent") noexcept;

  /*!
   * \brief   Destroys the generic proxy event.
   * \details The call is blocked until all the thread-pool tasks related to this event are removed/executed.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~GenericProxyEvent() noexcept;

  GenericProxyEvent(GenericProxyEvent const&) = delete;
  GenericProxyEvent(GenericProxyEvent&&) = delete;
  GenericProxyEvent& operator=(GenericProxyEvent const&) = delete;
  GenericProxyEvent& operator=(GenericProxyEvent&&) = delete;

  /*!
   * \brief Subscribes to the skeleton counterpart of this proxy-side event.
   * \param[in] max_sample_count  Size limits of this cache.
   * \return Result with no value or an error.
   * \error ara::com::ComErrc::kMaxSampleCountNotRealizable  If the given max_sample_count value is different to the
   *                                                         value given in previous subscription.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  SubscriptionResult Subscribe(std::size_t const max_sample_count) noexcept;

  /*!
   * \brief Unsubscribes from the skeleton counterpart of this proxy-side event.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void Unsubscribe() noexcept;

  /*!
   * \brief Query the number of available sample slots.
   * \return The number of free sample slots available.
   * \pre         Subscribe() has been called and Unsubscribe() has not been called.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  std::size_t GetFreeSampleCount() const noexcept;

  /*!
   * \brief Reads the samples from underlying receive buffers.
   * \tparam    F            User provided callable function with the signature void(GenericSamplePtr).
   * \param[in] f            Callable to be invoked on every newly available sample.
   * \param[in] max_samples  Maximum number of samples that can be processed within this call.
   * \return Result containing the number of successfully processed events within this call or an error.
   * \error ara::com::ComErrc::kMaxSamplesReached  If application holds all the pre-allocated samples.
   * \pre         Subscribe() has been called and Unsubscribe() has not been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Create a result with the valid event processed set to zero.
   * - If the event is subscribed,
   *   - Define a callable which,
   *     - Creates a GenericSamplePtr.
   *     - Invokes the user provided callable f.
   *   - Invoke ReadSamples on the event backend and emplace its return value into result.
   * - Otherwise:
   *   - Log a fatal message and abort the application.
   * - Return the result.
   * \endinternal
   */
  template <typename F>
  auto GetNewSamples(F&& f, std::size_t const max_samples = std::numeric_limits<std::size_t>::max()) noexcept
      -> GetNewSamplesResult {
    static_assert(vac::language::is_invocable_r<void, F, GenericSamplePtr&&>::value,
                  "socal::GenericProxyEvent::GetNewSamples - Provided callable is not valid!");
    // PTP-B-Socal-GenericProxyEvent_GetNewSamples
    GetNewSamplesResult result{GetNewSamplesResult::FromValue(0)};
    if (is_subscribed_) {
      auto callable_sample_result = [this, &f](SampleData&& sample_data) {
        ValidateSampleData(sample_data);  // VCA_SOCAL_CALLING_NON_STATIC_FUNCTION_FROM_CALLBACK_SYNCHRONOUSLY
        // VCA_SOCAL_CALLING_STL_APIS
        f(GenericSamplePtr{std::move(sample_data.memory_wrapper_ptr), std::move(sample_data.cache_ptr)});
      };
      // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      result = event_backend_.ReadSamples(max_samples, callable_sample_result);
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before subscription or after unsubscription of the event.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // PTP-E-Socal-GenericProxyEvent_GetNewSamples
    return result;
  }

  /*!
   * \brief Query current subscription state.
   * \return current state of the subscription.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ::ara::com::SubscriptionState GetSubscriptionState() const noexcept;

  /*!
   * \brief Registers an event receive handler to get notified on reception of new event samples.
   * \param[in] handler  The event receive handler that needs to be invoked on the reception of new event samples.
   *                     The given callable shall not be null and shall be valid at least until it is de-registered.
   *                     Any exception thrown by the callback will lead to a termination through "std::terminate()".
   * \return Result with no value or an error.
   * \error ComErrc::kSetHandlerFailure  Failed to register the handler.
   * \pre         Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires handler != nullptr; \endspec
   */
  ::ara::core::Result<void> SetReceiveHandler(::ara::com::EventReceiveHandler handler) noexcept;

  /*!
   * \brief Deregisters the event receive handler.
   * \return Result with no value or an error.
   * \error ComErrc::kUnsetHandlerFailure  Failed to unregister the handler.
   * \pre         Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \pre         The receive handler is registered before.
   * \pre         This API shall not be called if any user-defined mutex is locked in EventReceiveHandler and in the
   *              thread from where this API is called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::ara::core::Result<void> UnsetReceiveHandler() noexcept;

  /*!
   * \brief   Registers a subscription state change handler to get notified on event subscription state changes.
   * \details Communication Management implementation will serialize calls to the registered handler. If multiple
   *          changes of the subscription state take place during the runtime of a previous call to a handler, the
   *          Communication Management aggregates all changes to one call with the last/effective state.
   * \param[in] handler  The callback handler that needs to be invoked whenever the event subscription state changes.
   *                     The given callable shall not be null and shall be valid at least until it is de-registered.
   *                     Any exception thrown by the callback will lead to a termination through "std::terminate()".
   * \return Result with no value or an error.
   * \error ComErrc::kSetHandlerFailure  Failed to register the handler.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires handler != nullptr; \endspec
   */
  ::ara::core::Result<void> SetSubscriptionStateHandler(::ara::com::SubscriptionStateChangeHandler handler) noexcept;

  /*!
   * \brief Deregisters the subscription state change handler.
   * \return Result with no value or an error.
   * \error ComErrc::kUnsetHandlerFailure  Failed to unregister the handler.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \pre         The subscription state change handler is registered before.
   * \pre         This API shall not be called if any user-defined mutex is locked in the
   *              SubscriptionStateChangeHandler and in the thread from where this API is called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::ara::core::Result<void> UnsetSubscriptionStateHandler() noexcept;

 private:
  /*!
   * \brief Validate the sample data given by the binding.
   * \param[in] sample_data  The sample data object provided by the binding.
   * \pre     -
   * \context App
   * \steady TRUE
   */
  // VECTOR NC AutosarC++17_10-M3.2.4: MD_Socal_M3.2.4_FunctionWithoutDefinitionInLgBetaCode
  void ValidateSampleData(SampleData const& sample_data) const noexcept;

  /*!
   * \copydoc ::amsr::socal::internal::events::ProxyEventSyncInterface::HandleEventNotificationSync()
   */
  void HandleEventNotificationSync() noexcept final;

  /*!
   * \copydoc ::amsr::socal::internal::events::ProxyEventSyncInterface::HandleEventSubscriptionStateUpdateSync()
   */
  void HandleEventSubscriptionStateUpdateSync() noexcept final;

  /*!
   * \brief Boolean indicating whether the proxy event is subscribed.
   */
  std::atomic_bool is_subscribed_{false};

  /*!
   * \brief Subscribed cache size.
   */
  std::size_t subscribed_max_sample_count_{0};

  /*!
   * \brief The event backend.
   */
  GenericProxyEventBackend& event_backend_;

  /*!
   * \brief An instance of the logger.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_;

  /*!
   * \brief Event receive handler currently in use (Can be set only from the callback thread (detached threads)).
   */
  ProxyEventReceiveHandler receive_handler_{nullptr};

  /*!
   * \brief Pair containing:
   *        - First: Flag to indicate receive_handler_ changed. This flag will be set within the
   *          Set/UnsetReceiveHandler APIs and will be reset within the HandleEventNotificationSync API.
   *        - Second: Event receive handler for the next callback.
   */
  ReceiveHandlerPair new_receive_handler_pair_{false, nullptr};

  /*!
   * \brief Recursive mutex to protect concurrent modification of receive_handler_. A recursive mutex is used to avoid
   *        possible dead-locks caused by calls to other APIs by the receive handler. For example if the receive
   *        handler de-registers the handler by calling UnsetReceiveHandler().
   * \note  Violation of 'HIC++ rule 18.3.3 Do not use std::recursive_mutex' currently accepted due to ara::com design.
   */
  std::recursive_mutex receive_handler_lock_{};

  /*!
   * \brief An atomic boolean that indicates, if a receive handler is set. If there is no handler set, the value of
   *        this variable is 'false'. The variable is written by application threads or worker threads and read by the
   *        reactor thread.
   */
  std::atomic_bool is_receive_handler_set_{false};

  /*!
   * \brief Event subscription state change handler currently in use.
   */
  ProxyEventSubscriptionHandler subscription_state_change_handler_{nullptr};

  /*!
   * \brief Pair containing:
   *        - First: Flag to indicate subscription_state_change_handler_ changed. This flag will be set within the
   *          Set/UnsetSubscriptionStateHandler APIs and will be reset within the HandleEventSubscriptionStateUpdateSync
   *          API.
   *        - Second: Subscription state change handler for the next callback.
   */
  SubscriptionHandlerPair new_subscription_state_change_handler_pair_{false, nullptr};

  /*!
   * \brief Recursive mutex to protect concurrent modification of subscription_state_handler_. A recursive mutex is
   *        used to avoid possible dead-locks caused by calls to other APIs by the subscription state change handler.
   *        For example if the subscription state change handler de-registers the handler by calling
   *        UnsetSubscriptionStateHandler().
   * \note  Violation of 'HIC++ rule 18.3.3 Do not use std::recursive_mutex' currently accepted due to ara::com
   *        design.
   */
  std::recursive_mutex subscription_state_change_handler_lock_{};

  /*!
   * \brief An atomic boolean that indicates, if a subscription state handler is set. If there is no handler set, the
   *        value of this variable is 'false'. The variable is written by application threads or worker threads and
   *        read by the reactor thread.
   */
  std::atomic_bool is_subscription_state_handler_set_{false};

  /*!
   * \brief Last notified event subscription state to the user.
   */
  ::ara::com::SubscriptionState last_notified_state_{::ara::com::SubscriptionState::kNotSubscribed};
};

}  // namespace events
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_EVENTS_GENERIC_PROXY_EVENT_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::events::GenericProxyEvent::receive_handler_lock_
 * Used for protecting the concurrent modification of the current event receive_handler and for synchronizing the
 * unsubscription against the event receive handler invocation.
 *
 * \protects ::amsr::socal::events::GenericProxyEvent::new_receive_handler_pair_
 * \usedin :amsr::socal::events::GenericProxyEvent::HandleEventNotificationSync
 * \usedin :amsr::socal::events::GenericProxyEvent::SetReceiveHandler
 * \usedin :amsr::socal::events::GenericProxyEvent::UnsetReceiveHandler
 * \usedin :amsr::socal::events::GenericProxyEvent::Unsubscribe
 * \exclude All other methods of a GenericProxyEvent object.
 * \length MEDIUM Updating callback function for the event receive handler or
 *                wait for the completion of ongoing event receive handler execution, if any.
 * \endexclusivearea
 */
/*!
 * \exclusivearea ::amsr::socal::events::GenericProxyEvent::subscription_state_change_handler_lock_
 * Used for protecting the concurrent modification of the current event subscription state change handler and for
 * synchronizing the unsubscription against the subscription state change handler invocation.
 *
 * \protects ::amsr::socal::events::GenericProxyEvent::new_subscription_state_change_handler_pair_
 * \protects ::amsr::socal::events::GenericProxyEvent::last_notified_state_
 * \usedin :amsr::socal::events::GenericProxyEvent::HandleEventSubscriptionStateUpdateSync
 * \usedin :amsr::socal::events::GenericProxyEvent::SetSubscriptionStateHandler
 * \usedin :amsr::socal::events::GenericProxyEvent::UnsetSubscriptionStateHandler
 * \usedin :amsr::socal::events::GenericProxyEvent::Unsubscribe
 * \exclude All other methods of a GenericProxyEvent object.
 * \length MEDIUM updating callback function for the event subscription state change handler or
 *                wait for the completion of ongoing subscription state change handler execution, if any.
 * \endexclusivearea
 */
// clang-format on

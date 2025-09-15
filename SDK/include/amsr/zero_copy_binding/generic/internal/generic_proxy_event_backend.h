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
 *         \brief  Generic proxy event backend for the ZeroCopyBinding.
 *         \unit   ZeroCopyBinding::GenericProxy
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_PROXY_EVENT_BACKEND_H_
#define LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_PROXY_EVENT_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/events/event_subscriber_interface.h"
#include "amsr/socal/internal/events/generic_proxy_event_backend_interface.h"
#include "amsr/socal/internal/generic_memory_wrapper_interface.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/proxy_event_backend.h"
#include "ara/core/optional.h"

namespace amsr {
namespace zero_copy_binding {
namespace generic {
namespace internal {

/*!
 * \brief The ZeroCopyBinding generic proxy event backend.
 */
class AMSR_LG_BETA GenericProxyEventBackend final
    : public ::amsr::socal::internal::events::GenericProxyEventBackendInterface,
      public ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventServiceUpdateListener {
  /*!
   * \brief Predicate type to represent a generic proxy.
   */
  using GenericProxyPredicate = transformation_layer::internal::GenericProxyPredicate;

  /*!
   * \brief Type of the encapsulated proxy event backend object.
   */
  using ProxyEventBackendType = transformation_layer::internal::ProxyEventBackend<GenericProxyPredicate>;

  /*!
   * \brief Type of the memory wrapper used by the encapsulated proxy event backend's ReadSamples() method.
   */
  using MemoryWrapperProxyEventType = transformation_layer::internal::MemoryWrapperProxyEvent<GenericProxyPredicate>;

 public:
  /*!
   * \brief       Constructor of GenericProxyEventBackend.
   * \param[in]   zero_copy_binding_config_view  A view to the binding's config struct.
   *                                             It must remain valid for the object's entire lifetime.
   * \param[in]   provided_instance_id           The provided instance identifier of the skeleton counterpart.
   * \param[in]   life_cycle_manager_core        A pointer to the LifeCycleManagerCore.
   *                                             The LifeCycleManagerCore has to exist for the object's entire lifetime.
   * \param[in]   proxy_instance_id              The instance id of the corresponding proxy xf.
   * \internal steady FALSE \endinternal
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  GenericProxyEventBackend(common::internal::config::ConfigView zero_copy_binding_config_view,
                           ::amsr::socal::internal::ProvidedInstanceIdentifier provided_instance_id,
                           std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core,
                           std::size_t proxy_instance_id) noexcept;

  /*!
   * \brief       Destructor of ProxyEventBackend.
   * \context     App
   * \pre         All retrieved samples must have been returned.
   * \internal steady FALSE \endinternal
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  ~GenericProxyEventBackend() noexcept final = default;

  GenericProxyEventBackend() = delete;
  GenericProxyEventBackend(GenericProxyEventBackend const&) = delete;
  GenericProxyEventBackend(GenericProxyEventBackend&&) = delete;
  auto operator=(GenericProxyEventBackend const&) -> GenericProxyEventBackend& = delete;
  auto operator=(GenericProxyEventBackend&&) -> GenericProxyEventBackend& = delete;

  /*!
   * \brief         Subscribes to the event.
   * \param[in]     cache_size  The maximum number of cached events. This parameter is currently not supported by the
   *                            ZeroCopyBinding.
   * \pre           Event must not have already been subscribed to.
   * \internal steady FALSE \endinternal
   * \context       App
   * \threadsafe    TRUE
   * \reentrant     FALSE for same class instance, TRUE for different instances.
   * \synchronous   FALSE Subscription state is reported asynchronously.
   */
  void Subscribe(std::size_t cache_size) noexcept final;

  /*!
   * \brief         Unsubscribes from the event.
   * \pre           -
   * \internal steady FALSE \endinternal
   * \context       App
   * \threadsafe    TRUE
   * \reentrant     FALSE for same class instance, TRUE for different instances.
   * \synchronous   TRUE (TODO (jmeisinger, 08.04.2024): Report subscription state asynchronously?)
   */
  void Unsubscribe() noexcept final;

  /*!
   * \brief       Accesses the samples on the shared memory and passes them to the provided callable.
   * \details     Reads all current available samples until max_samples has been reached.
   * \param[in]   max_samples            Maximum number of samples that shall be processed within this call.
   * \param[in]   callable_sample_result Callable to be invoked per received sample.
   * \return      Result containing the number of events that have been passed to the provided callable.
   * \error       The error code ::ara::com::ComErrc::kMaxSamplesReached from the interface is currently not supported.
   * \pre         Event must be subscribed to.
   * \context     App
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE
   *              TRUE against GetFreeSampleCount.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE (However, asynchronous garbage collection is triggered.)
   */
  auto ReadSamples(std::size_t max_samples, CallableReadSamplesResult const& callable_sample_result) noexcept
      -> ReadSamplesResult final;

  /*!
   * \brief       Query current subscription state.
   * \return      Current state of the subscription.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \internal \steady TRUE \endinternal
   * \spec requires true; \endspec
   */
  auto GetSubscriptionState() const noexcept -> ::ara::com::SubscriptionState final;

  /*!
   * \brief       This function is currently not supported by the ZeroCopyBinding.
   * \return      Always the value 0 to signal that this functionality is not supported.
   * \pre         Event must be subscribed to.
   * \context     App | Callback
   * \internal steady TRUE \endinternal
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventServiceUpdateListener::OnServiceInstanceUp
   */
  void OnServiceInstanceUp() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::ProxyEventServiceUpdateListener::OnServiceInstanceDown
   */
  void OnServiceInstanceDown() noexcept final;

  /*!
   * \brief       This method is not supported for the GenericProxyEventBackend.
   * \return      This method does not return as it aborts.
   * \context     ANY
   * \pre         This function shall not be called.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  [[noreturn]] auto IsConnectedToSkeletonEvent() const noexcept -> bool final;

  /*!
   * \brief       Registers an event receive handler to get notified on the reception of new event samples.
   * \param[in]   callable  The callback handler that needs to be invoked on the reception of new event samples.
   *              The given callable shall not be null and shall be valid at least until it is de-registered.
   *              The given callable shall not be invoked prior subscription and after unsubscription.
   *              The APIs RegisterReceiveHandler/DeregisterReceiveHandler shall not be called from within the
   *              given callback.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires callable != nullptr; \endspec
   * \steady      FALSE
   */

  void RegisterReceiveHandler(CallableEventNotification callable) noexcept final;

  /*!
   * \brief       Deregisters the event receive handler.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The receive handler is registered before.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady      FALSE
   */
  void DeregisterReceiveHandler() noexcept final;

  /*!
   * \brief       Registers a subscription state change handler to get notified whenever the subscription state changes.
   * \param[in] callable  The callback handler that needs to be invoked whenever the subscription state changes.
   *                      The given callable shall not be null and shall be valid at least until it is de-registered.
   *                      The given callable shall not be invoked prior subscription and after unsubscription.
   *                      The given callback shall not be invoked with "kNotSubscribed" state.
   *                      The APIs RegisterSubscriptionStateChangeHandler/DeregisterSubscriptionStateChangeHandler shall
   *                      not be called from within the given callback.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires callable != nullptr; \endspec
   * \steady      FALSE
   */
  void RegisterSubscriptionStateChangeHandler(CallableSubscriptionStateUpdate callable) noexcept final;

  /*!
   * \brief       Deregisters the subscription state change handler.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The subscription state change handler is registered before.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady      FALSE
   */
  void DeregisterSubscriptionStateChangeHandler() noexcept final;

 private:
  /*!
   * \brief The encapsulated proxy event backend object.
   */
  ProxyEventBackendType proxy_event_backend_;

  /*!
   * \brief The Logger.
   */
  core::internal::logging::ZeroCopyLogger logger_;

  /*!
   * \brief An implementation of EventSubscriberInterface used to communicate with the ProxyEventBackend.
   */
  struct InternalEventSubscriber : public ::amsr::socal::internal::events::EventSubscriberInterface {
    /*!
     * \brief       Constructs the class InternalEventSubscriber.
     * \param[in]   logger The logger of the surrounding GenericProxyEventBackend.
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     * \vprivate    Component private
     */
    explicit InternalEventSubscriber(core::internal::logging::ZeroCopyLogger& logger) noexcept;

    /*!
     * \brief       Destroys the InternalEventSubscriber.
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     * \vprivate    Component private
     */
    ~InternalEventSubscriber() noexcept override = default;

    InternalEventSubscriber() = delete;
    InternalEventSubscriber(InternalEventSubscriber const&) = delete;
    InternalEventSubscriber(InternalEventSubscriber&&) = delete;
    auto operator=(InternalEventSubscriber const&) -> InternalEventSubscriber& = delete;
    auto operator=(InternalEventSubscriber&&) -> InternalEventSubscriber& = delete;

    /*!
     * \brief       Handles a received event notification.
     * \context     Backend
     * \threadsafe  FALSE for same EventSubscriberInterface instance, TRUE for different instances.
     * \reentrant   FALSE for same EventSubscriberInterface instance, FALSE for different instances.
     * \synchronous FALSE
     * \steady      TRUE
     */
    void HandleEventNotification() noexcept final;

    /*!
     * \brief       Handles an update of the subscription state of the event.
     * \param[in]   state  The current subscription state of this event.
     * \context     Backend | App during Unsubscription.
     * \threadsafe  FALSE for same EventSubscriberInterface instance, TRUE for different instances.
     * \reentrant   FALSE for same EventSubscriberInterface instance, FALSE for different instances.
     * \synchronous FALSE
     * \steady      TRUE
     */
    void HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState subscription_state) noexcept final;

    // All members are public, as this struct is only used internally in GenericProxyEventBackend.
    // These handlers do not need to be protected, as they can only changed in the App context.

    /*!
     * \brief A variable to store the event notification handler.
     */
    ::ara::core::Optional<CallableEventNotification> event_notification_handler_{::ara::core::nullopt};

    /*!
     * \brief A variable to store the subscription update handler.
     */
    ::ara::core::Optional<CallableSubscriptionStateUpdate> subscription_state_handler_{::ara::core::nullopt};

    /*!
     * \brief A variable to store the subscription state.
     */
    std::atomic<::ara::com::SubscriptionState> subscription_state_{::ara::com::SubscriptionState::kNotSubscribed};

    /*!
     * \brief The Logger of the wrapping GenericProxyEventBackend class.
     */
    core::internal::logging::ZeroCopyLogger& logger_;
  };

  /*!
   * \brief An object that serves as a subscriber for the Subscribe()/Unsubscribe() API of the ProxyEventBackend.
   */
  InternalEventSubscriber internal_subscriber_{logger_};
};

}  // namespace internal
}  // namespace generic
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_PROXY_EVENT_BACKEND_H_

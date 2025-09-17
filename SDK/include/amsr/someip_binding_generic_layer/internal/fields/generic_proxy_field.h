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
 *         \brief  Generic proxy field
 *         \unit   SomeIpBinding::SomeIpBindingGenericLayer::Fields::GenericProxyField
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include "amsr/core/future.h"
#include "amsr/core/optional.h"
#include "amsr/socal/generic_data_ptr.h"
#include "amsr/socal/internal/fields/generic_proxy_field_backend_interface.h"
#include "amsr/someip_binding_core/internal/events/client_event.h"
#include "amsr/someip_binding_core/internal/events/event_notification_interface.h"
#include "amsr/someip_binding_core/internal/methods/client_method.h"
#include "amsr/someip_binding_generic_layer/internal/events/generic_proxy_event_notification_handler.h"
#include "amsr/someip_binding_generic_layer/internal/generic_proxy_visible_sample_cache.h"
#include "amsr/someip_binding_generic_layer/internal/methods/generic_proxy_method.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {
namespace fields {

/*!
 * \brief GenericProxyField class.
 */
class GenericProxyField final : public amsr::socal::internal::fields::GenericProxyFieldBackendInterface,
                                public amsr::someip_binding_core::internal::events::EventNotificationInterface {
 public:
  /*!
   * \brief       Constructor.
   * \param[in]   notifier_client_event Field notifier client event.
   * \param[in]   getter_client_method  Getter method.
   * \param[in]   setter_client_method  Setter method.
   * \param[in]   allocator             Allocator for getter and setter methods.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit GenericProxyField(
      ara::core::Optional<std::unique_ptr<amsr::someip_binding_core::internal::events::ClientEvent>>
          notifier_client_event,
      ara::core::Optional<std::unique_ptr<amsr::someip_binding_core::internal::methods::ClientMethod>>
          getter_client_method,
      ara::core::Optional<std::unique_ptr<amsr::someip_binding_core::internal::methods::ClientMethod>>
          setter_client_method,
      amsr::SharedPtr<vac::memory::allocator::MemoryBufferAllocator> allocator);

  /*!
   * \brief       Destructor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~GenericProxyField() noexcept;

  GenericProxyField(GenericProxyField&&) noexcept = delete;
  GenericProxyField(GenericProxyField const&) = delete;
  auto operator=(GenericProxyField const&) -> GenericProxyField& = delete;
  auto operator=(GenericProxyField&&) noexcept -> GenericProxyField& = delete;

  /*!
   * \brief Check whether a notifier is configured.
   * \return True if a notifier is configured, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto HasNotifier() const noexcept -> bool final;

  /*!
   * \brief Check whether a getter is configured.
   * \return True if a getter is configured, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto HasGetter() const noexcept -> bool final;

  /*!
   * \brief Get the latest field value.
   * \return A future object that can be used to access the result of the Get call.
   *         The result will be updated asynchronously, either with the latest field value (in serialized format)
   *         retrieved from the remote server, or with an error.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   *                                         e.g. deserialization error or other Binding internal errors.
   * \pre         Field getter is configured.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  auto Get() noexcept -> ::amsr::core::Future<::amsr::socal::GenericDataPtr> final;

  /*!
   * \brief Check whether a setter is configured.
   * \return True if a setter is configured, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto HasSetter() const noexcept -> bool final;

  /*!
   * \brief Set the field value.
   * \param[in] value  The serialized field value to be set.
   * \return A future object that can be used to access the result of the Set call.
   *         The result will be updated asynchronously, either with the latest field value (in serialized format)
   *         returned by the remote server, or with an error.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   *                                         e.g. deserialization error or other Binding internal errors.
   * \pre         Field setter is configured.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  auto Set(::amsr::socal::GenericDataType const& value) noexcept
      -> ::amsr::core::Future<::amsr::socal::GenericDataPtr> final;

  /*!
   * \brief Subscribes to the event.
   * \param[in] cache_size  The maximum number of cached events. The binding is responsible for pre-allocating the
   *                        memory for storing at least the cache_size number of samples.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady      FALSE
   */
  void Subscribe(std::size_t cache_size) noexcept final;

  /*!
   * \brief Unsubscribes from the event.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API for this class.
   * \reentrant   FALSE
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady      FALSE
   */
  void Unsubscribe() noexcept final;

  /*!
   * \brief   Reads the serialized samples from underlying receive buffers and passes them to the provided callable.
   * \details Binding implementation should start reading all the received samples from the top of its receive buffers.
   *          Reading of samples should be continued until either:
   *          - The pre-allocated memory for storing the samples is exhausted.
   *          - Given maximum samples have been processed within this call.
   *          - There are no further new samples to read.
   * \param[in] max_samples             Maximum number of samples that can be processed within this call.
   * \param[in] callable_sample_result  Callable to be invoked with the serialized sample.
   *                                    The underlying memory pointer shall never be nullptr.
   *                                    The callable is valid only until the scope of this function call,
   *                                    so storing and invoking it at a later point will lead to undefined behavior.
   * \return Result containing the number of events that have been passed to the provided callable.
   * \error ara::com::ComErrc::kMaxSamplesReached  if all slots from the binding are used at the beginning of the call.
   * \pre         Event must be subscribed.
   * \context     App
   * \threadsafe  FALSE
   *              TRUE against GetFreeSampleCount and
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto ReadSamples(std::size_t max_samples, CallableReadSamplesResult const& callable_sample_result) noexcept
      -> ReadSamplesResult final;

  /*!
   * \brief Query current subscription state.
   * \return current state of the subscription.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto GetSubscriptionState() const noexcept -> ::ara::com::SubscriptionState final;

  /*!
   * \brief Query the number of available sample slots in the local sample cache.
   * \return The number of free samples available in the local sample cache.
   * \pre         Event must be subscribed.
   * \context     App
   * \threadsafe  TRUE
   *              TRUE against ReadSamples and
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t final;

  /*!
   * \brief Registers an receive handler to get notified on the reception of new field notifications.
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new field notifications.
   *                      The given callable shall not be null and shall be valid at least until it is de-registered.
   *                      The given callable shall not be invoked prior subscription and after unsubscription.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         Notifier is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec requires callable != nullptr; \endspec
   */
  void RegisterReceiveHandler(CallableEventNotification callable) noexcept final;

  /*!
   * \brief Deregisters the receive handler.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The receive handler is registered before.
   * \pre         Notifier is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec requires true; \endspec
   */
  void DeregisterReceiveHandler() noexcept final;

  /*!
   * \brief Registers a subscription state change handler to get notified whenever the subscription state changes.
   * \param[in] callable  The callback handler that needs to be invoked whenever the subscription state changes.
   *                      The given callable shall not be null and shall be valid at least until it is de-registered.
   *                      The given callable shall not be invoked prior subscription and after unsubscription.
   *                      The given callback shall not be invoked with "kNotSubscribed" state.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec requires callable != nullptr; \endspec
   */
  void RegisterSubscriptionStateChangeHandler(CallableSubscriptionStateUpdate callable) noexcept final;

  /*!
   * \brief Deregisters the subscription state change handler.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The subscription state change handler is registered before.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec requires true; \endspec
   */
  void DeregisterSubscriptionStateChangeHandler() noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::events::EventNotificationInterface::OnEventReceived
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  void OnEventReceived() noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::events::EventNotificationInterface::OnEventSubscriptionStateUpdate
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  void OnEventSubscriptionStateUpdate(::amsr::someip_protocol::internal::SubscriptionState state) noexcept final;

 private:
  /*!
   * \brief Invisible sample cache capacity
   */
  std::size_t invisible_sample_cache_capacity_{0U};

  /*!
   * \brief Visible sample cache unique ptr
   */
  std::shared_ptr<GenericProxyVisibleSampleCache> visible_sample_cache_{nullptr};

  /*!
   * \brief Generic proxy event notification handler to be registered in the SomeIpBindingCore.
   *        SomeIpBindingCore (ClientEvent) will hold a weak pointer to this object,
   *        in order to forward incoming event notifications.
   *        It will be registered into SomeIpBindingCore within subscribe call of this class, and
   *        deregistered during unsubscribe call.
   */
  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
  std::shared_ptr<amsr::someip_binding_generic_layer::internal::events::GenericProxyEventNotificationHandler>
      notification_handler_{
          std::make_shared<amsr::someip_binding_generic_layer::internal::events::GenericProxyEventNotificationHandler>(
              *this)};

  /*!
   * \brief Client event for fields that have a notifier.
   */
  ara::core::Optional<std::unique_ptr<amsr::someip_binding_core::internal::events::ClientEvent>> notifier_client_event_;

  /*!
   * \brief Get method for fields that have a getter.
   */
  ara::core::Optional<methods::GenericProxyMethod> getter_method_;

  /*!
   * \brief Set method for fields that have a setter.
   */
  ara::core::Optional<methods::GenericProxyMethod> setter_method_;

  /*!
   * \brief Event subscription state
   */
  ::ara::com::SubscriptionState state_;

  /*!
   * \brief Socal callback for received events.
   */
  amsr::core::Optional<CallableEventNotification> receive_handler_;

  /*!
   * \brief Socal callback for state updates.
   */
  amsr::core::Optional<CallableSubscriptionStateUpdate> subscription_state_handler_;

  /*!
   * \brief Logger for tracing and debugging.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;

  /*!
   * \brief Mutex for protection against parallel access of state update callable.
   */
  std::mutex subscribe_status_update_callable_lock_{};
};

}  // namespace fields
}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_H_

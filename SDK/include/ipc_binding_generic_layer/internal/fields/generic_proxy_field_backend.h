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
 *        \brief  Generic proxy field backend declaration.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_BACKEND_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <memory>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/fields/generic_proxy_field_backend_interface.h"
#include "amsr/unique_ptr.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_generic_layer/internal/events/generic_proxy_event_backend.h"
#include "ipc_binding_generic_layer/internal/methods/generic_proxy_method_backend_interface.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {
namespace fields {

/*!
 * \brief Generic Proxy Field backend.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyField
 */
// VECTOR Next Construct Metric-OO.WMC.One: MD_IPCBINDING_Metric-OO.WMC.One_Generic_proxy_field_backend_methods
class AMSR_LG_BETA GenericProxyFieldBackend final
    : public ::amsr::socal::internal::fields::GenericProxyFieldBackendInterface {
 public:
  /*!
   * \brief Type alias for generic event backend.
   */
  using GenericProxyEventBackendInterface =
      ::amsr::ipc_binding_generic_layer::internal::events::GenericProxyEventBackendInterface;

  /*!
   * \brief Type alias for generic method backend.
   */
  using GenericProxyMethodBackendInterface =
      ::amsr::ipc_binding_generic_layer::internal::methods::GenericProxyMethodBackendInterface;

  /*!
   * \brief Type-alias for MethodId.
   */
  using MethodId = ::amsr::ipc_binding_core::internal::ipc_protocol::MethodId;

  /*!
   * \brief Type-alias for ResponseMessage.
   */
  using ResponseMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessage;

  /*!
   * \brief Type-alias for ErrorResponseMessage.
   */
  using ErrorResponseMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessage;

  /*!
   * \brief Type-alias for ApplicationErrorMessage.
   */
  using ApplicationErrorMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessage;

  /*!
   * \brief Constructor
   *
   * \param[in] notifier  Event backend for the field notifier if there is any, nullptr otherwise.
   * \param[in] getter    Method backend for the field getter if there is any, nullptr otherwise.
   * \param[in] setter    Method backend for the field setter if there is any, nullptr otherwise.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   */
  explicit GenericProxyFieldBackend(UniquePtr<GenericProxyEventBackendInterface> notifier,
                                    UniquePtr<GenericProxyMethodBackendInterface> getter,
                                    UniquePtr<GenericProxyMethodBackendInterface> setter);

  auto operator=(GenericProxyFieldBackend&&) noexcept -> GenericProxyFieldBackend& = delete;
  GenericProxyFieldBackend(GenericProxyFieldBackend const&) noexcept = delete;
  auto operator=(GenericProxyFieldBackend const&) noexcept -> GenericProxyFieldBackend& = delete;
  GenericProxyFieldBackend(GenericProxyFieldBackend&&) noexcept = delete;

  /*!
   * \brief Subscribes to the event.
   *
   * \param[in] cache_size  The maximum number of cached events.
   *
   * \pre         HasNotifier() is true.
   * \context     App
   * \threadsafe  FALSE FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady      FALSE
   */
  void Subscribe(std::size_t cache_size) noexcept final;

  /*!
   * \brief Unsubscribes from the event.
   *
   * \pre         HasNotifier() is true.
   * \context     App
   * \threadsafe  FALSE FALSE against any other API for this class.
   * \reentrant   FALSE
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void Unsubscribe() noexcept final;

  /*!
   * \brief Reads the generic notifier samples from underlying receive buffers and passes them to the provided callable.
   *
   * \param[in] max_samples             Maximum number of samples that can be processed within this call.
   * \param[in] callable_sample_result  Callable to be invoked with the serialized sample. The underlying memory pointer
   *                                    shall never be nullptr. The callable is valid only until the scope of this
   *                                    function call, so storing and invoking it at a later point will lead to
   *                                    undefined behavior.
   *
   * \return Result containing the number of events that have been passed to the provided callable.
   *
   * \error ara::com::ComErrc::kMaxSamplesReached  If all slots from the binding are used at the beginning of the call.
   *
   * \pre         HasNotifier() is true and notifier must be subscribed.
   * \context     App
   * \threadsafe  FALSE TRUE against GetFreeSampleCount and FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ReadSamples(std::size_t max_samples, CallableReadSamplesResult const& callable_sample_result) noexcept
      -> ReadSamplesResult final;

  /*!
   * \brief Query current subscription state.
   *
   * \return Current state of the subscription.
   *
   * \pre         HasNotifier() is true.
   * \context     App
   * \threadsafe  FALSE FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetSubscriptionState() const noexcept -> ::ara::com::SubscriptionState final;

  /*!
   * \brief Query the number of available sample slots in the local sample cache.
   *
   * \return The number of free samples available in the local sample cache.
   *
   * \pre         HasNotifier() is true and notifier must be subscribed.
   * \context     App
   * \threadsafe  TRUE TRUE against ReadSamples and FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t final;

  /*!
   * \brief Registers a receive handler to get notified on the reception of new field notifications.
   *
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new field notifications. The
   *                      given callable shall not be null and shall be valid at least until it is de-registered. The
   *                      given callable shall not be invoked prior subscription and after unsubscription.
   *
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         Notifier is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires callable != nullptr; \endspec
   */
  auto RegisterReceiveHandler(CallableEventNotification callable) noexcept -> void final;

  /*!
   * \brief Deregisters the receive handler.
   *
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The receive handler is registered before.
   * \pre         Notifier is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto DeregisterReceiveHandler() noexcept -> void final;

  /*!
   * \brief Registers a subscription state change handler to get notified whenever the subscription state changes.
   *
   * \param[in] callable  The callback handler that needs to be invoked whenever the subscription state changes. The
   *                      given callable shall not be null and shall be valid at least until it is de-registered. The
   *                      given callable shall not be invoked prior subscription and after unsubscription. The given
   *                      callback shall not be invoked with "kNotSubscribed" state.
   *
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires callable != nullptr; \endspec
   */
  auto RegisterSubscriptionStateChangeHandler(CallableSubscriptionStateUpdate callable) noexcept -> void final;

  /*!
   * \brief Deregisters the subscription state change handler.
   *
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The subscription state change handler is registered before.
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto DeregisterSubscriptionStateChangeHandler() noexcept -> void final;

  /*!
   * \brief Check whether a notifier is configured.
   *
   * \return True if a notifier is configured, false otherwise.
   *
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
   *
   * \return True if a getter is configured, false otherwise.
   *
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
   *
   * \return A future object that can be used to access the result of the Get call. The result will be updated
   *         asynchronously, either with the latest field value (in serialized format) retrieved from the remote server,
   *         or with an error.
   *
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error. e.g.
   *                                         deserialization error or other Binding internal errors.
   *
   * \pre         HasGetter() is true.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  auto Get() noexcept -> ::amsr::core::Future<::amsr::socal::GenericDataPtr> final;

  /*!
   * \brief Check whether a setter is configured.
   *
   * \return True if a setter is configured, false otherwise.
   *
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
   *
   * \param[in] value  The arguments passed to the setter.
   *
   * \return A future object that can be used to access the result of the Set call. The result will be updated
   *         asynchronously, either with the latest field value (in serialized format) returned by the remote server, or
   *         with an error.
   *
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error. e.g.
   *                                         deserialization error or other Binding internal errors.
   *
   * \pre         HasSetter() is true.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  auto Set(::amsr::socal::GenericDataType const& value) noexcept
      -> ::amsr::core::Future<::amsr::socal::GenericDataPtr> final;

  ~GenericProxyFieldBackend() noexcept = default;

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief The event backend for the notifier.
   */
  UniquePtr<GenericProxyEventBackendInterface> notifier_;

  /*!
   * \brief The method backend for the getter.
   */
  UniquePtr<GenericProxyMethodBackendInterface> getter_;

  /*!
   * \brief The method backend for the setter.
   */
  UniquePtr<GenericProxyMethodBackendInterface> setter_;

  /*!
   * \brief Logger for tracing and debugging.
   */
  Logger logger_;
};
}  // namespace fields
}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_BACKEND_H_

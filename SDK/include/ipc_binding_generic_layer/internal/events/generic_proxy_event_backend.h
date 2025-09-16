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
 *        \brief  Generic proxy event backend class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_BACKEND_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <utility>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/events/generic_proxy_event_backend_interface.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/transmit_message_handler_interface.h"
#include "ipc_binding_core/internal/events/invisible_sample_cache.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_generic_layer/internal/events/generic_sample_cache.h"
#include "ipc_binding_xf_common/internal/trace/trace_event_sink_interface.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {
namespace events {

/*!
 * \brief   Interface for generic proxy event backend.
 * \details Union of the interfaces towards socal and towards IpcBinding's core.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyEvent
 */
class AMSR_LG_BETA GenericProxyEventBackendInterface
    : public ::amsr::socal::internal::events::GenericProxyEventBackendInterface,
      public ::amsr::ipc_binding_core::internal::ProxyEventBackendInterface {
 public:
  GenericProxyEventBackendInterface() noexcept = default;
  ~GenericProxyEventBackendInterface() noexcept override = default;

  auto operator=(GenericProxyEventBackendInterface&&) noexcept -> GenericProxyEventBackendInterface& = delete;
  GenericProxyEventBackendInterface(GenericProxyEventBackendInterface const&) noexcept = delete;
  auto operator=(GenericProxyEventBackendInterface const&) noexcept -> GenericProxyEventBackendInterface& = delete;
  GenericProxyEventBackendInterface(GenericProxyEventBackendInterface&&) noexcept = delete;
};

/*!
 * \brief Generic Proxy Event backend.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyEvent
 */
class AMSR_LG_BETA GenericProxyEventBackend final : public GenericProxyEventBackendInterface {
 public:
  /*!
   * \brief Type of Event identifier.
   */
  using EventId = ::amsr::ipc_binding_core::internal::ipc_protocol::EventId;

  /*!
   * \brief Type of Client identifier.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  /*!
   * \brief Type alias to use strong type.
   */
  using SizeType = std::size_t;

  /*!
   * \brief Interface type for binding-specific proxy event backend.
   */
  using ProxyEventBackendInterface = ::amsr::socal::internal::events::GenericProxyEventBackendInterface;

  /*!
   * \brief Type alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type of trace event sink.
   */
  using TraceEventSinkInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceEventSinkInterface;

  /*!
   * \brief Type alias for the InvisibleSampleCache.
   */
  using InvisibleSampleCache = ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache;

  /*!
   * \brief Type alias for the container which is used by a consumer of InvisibleSampleCache.
   */
  using SampleCacheContainer = InvisibleSampleCache::SampleCacheContainer;

  /*!
   * \brief VisibleSampleCache pointer type.
   */
  using VisibleSampleCachePtr = std::shared_ptr<GenericSampleCache>;  // IGNORE_BLACKLIST external API

  /*!
   * \brief Type alias for NotificationMessage.
   */
  using NotificationMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessage;

  /*!
   * \brief Constructor
   *
   * \param[in] provided_service_instance  Provided service instance ID.
   * \param[in] event_id                   Event ID.
   * \param[in] client_id                  Client ID.
   * \param[in] trace_sink                 Sink for tracing of events.
   * \param[in] proxy_router               Reference to the ProxyRouter.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   */
  GenericProxyEventBackend(ProvidedServiceInstanceId const& provided_service_instance, EventId const event_id,
                           ClientId const client_id,
                           std::unique_ptr<TraceEventSinkInterface> trace_sink,  // IGNORE_BLACKLIST BSOCIAL-8805
                           ipc_binding_core::internal::ProxyRouterInterface& proxy_router) noexcept;

  auto operator=(GenericProxyEventBackend&&) noexcept -> GenericProxyEventBackend& = delete;
  GenericProxyEventBackend(GenericProxyEventBackend const&) noexcept = delete;
  auto operator=(GenericProxyEventBackend const&) noexcept -> GenericProxyEventBackend& = delete;
  GenericProxyEventBackend(GenericProxyEventBackend&&) noexcept = delete;

  /*!
   * \brief Destructor
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   *
   * \spec requires true; \endspec
   */
  ~GenericProxyEventBackend() noexcept final = default;

  /*!
   * \brief Subscribes to the event.
   *
   * \param[in] cache_size  The maximum number of cached events.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous FALSE Subscription state is reported asynchronously.
   *
   * \spec requires true; \endspec
   */
  auto Subscribe(std::size_t const cache_size) noexcept -> void final;

  /*!
   * \brief Unsubscribes from the event.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE FALSE against any other API for this class.
   * \reentrant   FALSE
   * \synchronous FALSE Subscription state is reported asynchronously.
   *
   * \spec requires true; \endspec
   */
  auto Unsubscribe() noexcept -> void final;

  /*!
   * \copydoc ipc_binding_core::internal::ProxyEventBackendInterface::OnNotificationReceived
   */
  auto OnNotificationReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessage&& notification) noexcept -> void final;

  /*!
   * \copydoc ipc_binding_core::internal::ProxyEventBackendInterface::OnSubscribeAckReceived
   *
   * \spec requires true; \endspec
   */
  auto OnSubscribeAckReceived() noexcept -> void final;

  /*!
   * \copydoc ipc_binding_core::internal::ProxyEventBackendInterface::OnSubscribeNAckReceived
   *
   * \spec requires true; \endspec
   */
  auto OnSubscribeNAckReceived() noexcept -> void final;

  /*!
   * \brief Query current subscription state.
   *
   * \return Current state of the subscription.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetSubscriptionState() const noexcept -> ::ara::com::SubscriptionState final;

  /*!
   * \brief   Reads the generic event samples from underlying receive buffers and passes them to the provided callable.
   * \details Binding implementation should start reading all the received samples from the top of its receive buffers.
   *          Reading of samples should be continued until either:
   *          - The pre-allocated memory for storing the samples is exhausted.
   *          - Given maximum samples have been processed within this call.
   *          - There are no further new samples to read.
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
   * \pre         Event must be subscribed.
   * \context     App
   * \threadsafe  FALSE TRUE against GetFreeSampleCount and FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  auto ReadSamples(std::size_t const max_samples, CallableReadSamplesResult const& callable_sample_result) noexcept
      -> ReadSamplesResult final;

  /*!
   * \brief Query the number of available sample slots in the local sample cache.
   *
   * \return The number of free samples available in the local sample cache.
   *
   * \pre         Event must be subscribed.
   * \context     App
   * \threadsafe  TRUE TRUE against ReadSamples and FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t final;

  /*!
   * \brief Registers an event receive handler to get notified on the reception of new event samples.
   *
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new event samples. The given
   *                      callable shall not be null and shall be valid at least until it is de-registered. The given
   *                      callable shall not be invoked prior subscription and after unsubscription.
   *
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
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
   * \brief Deregisters the event receive handler.
   *
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The receive handler is registered before.
   * \pre         Event is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
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
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyEventBackendInterface::SetConnectionState
   */
  auto SetConnectionState(bool connection_state) noexcept -> void final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyEventBackendInterface::SetServiceState
   */
  auto SetServiceState(bool service_state) noexcept -> void final;

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for SubscribeEventMessageHeader.
   */
  using SubscribeEventMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::SubscribeEventMessageHeader;

  /*!
   * \brief Type alias for UnsubscribeEventMessageHeader.
   */
  using UnsubscribeEventMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::UnsubscribeEventMessageHeader;

  /*!
   * \brief Type alias for ConnectionProxyInterface.
   */
  using ConnectionProxyInterface = ::amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface;

  /*!
   * \brief Type alias for TransmitMessageHandlerInterface.
   */
  using TransmitMessageHandlerInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::TransmitMessageHandlerInterface;

  /*!
   * \brief   Sends out a subscribe message if the event was already subscribed.
   * \details Called from the ProxyRouter to indicate that monitored service instance is available again.
   *
   * \context     Reactor
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   */
  auto ResendSubscription() noexcept -> void;

  /*!
   * \brief Reads serialized samples from the given sample cache container, stored in free sample slot and calls the
   *        provided callback function.
   *
   * \param[in] invisible_app_cache_container  Container which has the enqueued event samples cache entries.
   * \param[in] max_samples                    Maximum number of samples to process.
   * \param[in] callable_sample_result         Callable to be invoked on sample moved to free slot.
   *
   * \return The number of event samples which were successfully moved to free sample slot.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  auto ReadSamplesInternal(SampleCacheContainer& invisible_app_cache_container, std::size_t const max_samples,
                           CallableReadSamplesResult const& callable_sample_result) const noexcept -> std::size_t;

  /*!
   * \brief Query current subscription state.
   *
   * \return Current state of the subscription.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  auto GetSubscriptionStateInternal() noexcept -> ::ara::com::SubscriptionState;

  /*!
   * \brief Set current subscription state.
   *
   * \param[in] subscribe_status  Event subscribe status.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE, FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  auto SetSubscriptionStateInternal(::ara::com::SubscriptionState subscribe_status) noexcept -> void;

  /*!
   * \brief The ProvidedServiceInstanceId.
   */
  ProvidedServiceInstanceId const provided_service_instance_;

  /*!
   * \brief Event id.
   */
  EventId const event_id_;

  /*!
   * \brief Client id.
   */
  ClientId const client_id_;

  /*!
   * \brief Maximum number of stored events in the invisible cache.
   */
  SizeType cache_capacity_{0U};

  /*!
   * \brief Event storage.
   */
  InvisibleSampleCache invisible_sample_cache_{};

  /*!
   * \brief Sink for tracing of events.
   */
  std::unique_ptr<TraceEventSinkInterface> trace_sink_;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief Pointer to the ProxyRouter.
   */
  ipc_binding_core::internal::ProxyRouterInterface& proxy_router_;

  /*!
   * \brief Logger for tracing and debugging.
   */
  Logger logger_;

  /*!
   * \brief Event cache visible to the user.
   */
  VisibleSampleCachePtr visible_sample_cache_{nullptr};

  /*!
   * \brief Mutex for protection against parallel requests.
   */
  std::mutex service_up_lock_{};

  /*!
   * \brief Holds the currently known service instance state.
   */
  bool service_state_{true};

  /*!
   * \brief Mutex for protection against parallel subscriptions/un-subscriptions.
   */
  std::mutex subscription_status_lock_{};

  /*!
   * \brief Event subscription status.
   */
  ::ara::com::SubscriptionState subscription_status_{::ara::com::SubscriptionState::kNotSubscribed};

  /*!
   * \brief Event notification callback.
   */
  CallableEventNotification event_notification_callable_{nullptr};

  /*!
   * \brief Mutex for protection against parallel access of state update callable.
   */
  std::mutex subscribe_status_update_callable_lock_{};
  /*!
   * \brief Subscription status update callback.
   */
  CallableSubscriptionStateUpdate subscription_state_update_callable_{nullptr};
};

}  // namespace events
}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_PROXY_EVENT_BACKEND_H_

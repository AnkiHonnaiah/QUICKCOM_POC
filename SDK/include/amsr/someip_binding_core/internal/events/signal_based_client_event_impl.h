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
/**       \file   signal_based_client_event_impl.h
 *        \brief  The binding client event implementation for handling SOME/IP signal based events subscription and
 *                reception.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientEvent
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_SIGNAL_BASED_CLIENT_EVENT_IMPL_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_SIGNAL_BASED_CLIENT_EVENT_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/client_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/events/event_accessor_interface.h"
#include "amsr/someip_binding_core/internal/events/event_notification_interface.h"
#include "amsr/someip_binding_core/internal/events/invisible_sample_cache.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "someip-protocol/internal/someip_posix_types.h"

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace events {

/*!
 * \brief Handler class for Signal Based events.
 */
class SignalBasedClientEventImpl final : public EventAccessorInterface {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] proxy_identity               Unique ID of the client.
   * \param[in] event_id                     The ID that identifies this event.
   * \param[in] client_transport_to_daemon   The client-transport-to-daemon.
   *
   * \context App
   * \steady  FALSE
   */
  SignalBasedClientEventImpl(::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
                             ::amsr::someip_protocol::internal::EventId event_id,
                             ClientTransportToDaemonInterface& client_transport_to_daemon) noexcept;

  /*!
   * \brief       Destructor.
   *
   * \pre         The event must be in an unsubscribed state.
   *
   * \context     App
   * \steady      FALSE
   *
   * \vprivate
   */
  ~SignalBasedClientEventImpl() noexcept final;

  SignalBasedClientEventImpl(SignalBasedClientEventImpl const&) = delete;
  SignalBasedClientEventImpl(SignalBasedClientEventImpl&&) = delete;
  SignalBasedClientEventImpl& operator=(SignalBasedClientEventImpl const&) & = delete;
  SignalBasedClientEventImpl& operator=(SignalBasedClientEventImpl&&) & = delete;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::events::ClientEventInterface::SubscribeEvent
   */
  amsr::core::Result<void> SubscribeEvent(std::size_t const cache_size,
                                          std::weak_ptr<EventNotificationInterface> handler) noexcept final;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::events::ClientEventInterface::UnsubscribeEvent
   */
  void UnsubscribeEvent() noexcept final;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::events::ClientEventInterface::ReadSamples
   */
  ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::SampleCacheContainer& ReadSamples(
      std::size_t const max_samples) noexcept final;

  /*!
   * \brief       Must never be called, SOME/IP events that cannot be handled by this class.
   *
   * \param[in]   event_message A complete SOME/IP event message.
   *
   * \context     Reactor (Event Reception)
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void OnEvent(::amsr::someip_binding_core::internal::events::EventMessage const& event_message) noexcept final;

  /*!
   * \brief     Callback function for Event reception.
   *
   * \param[in] pdu_message Pdu message including the header.
   *
   * \pre          -
   *
   * \context     Reactor
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void OnPduEvent(::amsr::someip_binding_core::internal::events::PduMessage const& pdu_message) noexcept final;

  /*!
   * \brief     Callback function for subscription state change.
   *
   * \param[in] state The updated event subscription state the payload.
   *
   * \pre          -
   *
   * \context     Reactor
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void OnSubscriptionStateChange(::amsr::someip_protocol::internal::SubscriptionState const state) noexcept final;

  /*!
   * \brief   Getter for the Event Id.
   *
   * \return  Event ID.
   *
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::EventId GetEventId() const noexcept final;

  /*!
   * \brief   Getter for the subscription call state.
   * \return  Subscription call state.
   *
   * \context     App
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady true
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool GetSubscriptionCallState() const noexcept final;

 private:
  /*!
   * \brief Unique identifier for this client
   */
  ::amsr::someip_binding_core::internal::ProxyBindingIdentity const proxy_identity_;

  /*!
   * \brief The event ID.
   */
  ::amsr::someip_protocol::internal::EventId const event_id_;

  /*!
   * \brief The client transport to daemon.
   */
  ClientTransportToDaemonInterface& client_transport_to_daemon_;

  /*!
   * \brief Lock for avoiding concurrent access to is_subscribed_ flag.
   */
  std::mutex client_event_lock_{};

  /*!
   * \brief Notification target to trigger upon event reception.
   */
  std::weak_ptr<EventNotificationInterface> notification_target_{};

  /*!
   * \brief Invisible sample cache to hold received serialized events.
   */
  std::unique_ptr<InvisibleSampleCache> invisible_sample_cache_{nullptr};

  /*!
   * \brief Flag to signal if event subscription was called.
   */
  bool subscribe_was_called_{false};

  /*!
   * \brief Logger for tracing and debugging.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription,
      ::amsr::someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "SignalBasedClientEventImpl"_sv, "EventId"_sv, event_id_)};
};
}  // namespace events
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_SIGNAL_BASED_CLIENT_EVENT_IMPL_H_

/*!
 * \exclusivearea ::amsr::someip_binding_core::internal::SignalBasedClientEventImpl::client_event_lock_
 *                Ensures consistency while read and write access to the is_subscribed_ flag.
 *
 * \protects ::amsr::someip_binding_core::internal::SignalBasedClientEventImpl::is_subscribed_
 *           is the protected resource and contains the state if an event is subscribed or not.
 *
 * \usedin SubscribeEvent(write/read), UnsubscribeEvent(write/read), ReadSamples(read), OnPduEvent(read),
 *         OnSubscriptionStateChange(read), dtor(read).
 *
 * \length MEDIUM Limited to checking the flag or changing the flag.
 *                The exclusive area starts and ends always in the smallest possible scope within the protected methods.
 *                This exclusive area covers calls to at most one method.
 * \endexclusivearea
 */

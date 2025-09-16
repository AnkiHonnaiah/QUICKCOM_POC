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
/**        \file  remote_server_interface.h
 *        \brief  Remote Server Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_daemon_core/client/client_event_dispatcher_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_connection_state_change_handler.h"
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_observer.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief A Remote Sever interface.
 */
class RemoteServerInterface : public service_discovery::state_machine::client::ServiceDiscoveryClientObserver,
                              public RemoteServerConnectionStateChangeHandler {
 public:
  /*!
   * \brief Represents a SOME/IP message.
   */
  using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief Alias for the service address type.
   */
  using ServiceAddress = amsr::someip_daemon_core::configuration::types::ServiceAddress;

  /*!
   * \brief Alias for SomeIpEventHandlerInterface shared pointer.
   */
  using SomeIpEventObserverPtr = std::shared_ptr<client::SomeIpEventHandlerInterface>;

  /*!
   * \brief Alias for PduEventHandlerInterface shared pointer.
   */
  using PduEventObserverPtr = std::shared_ptr<client::PduEventHandlerInterface>;

  /*!
   * \brief   Constructor of RemoteServerInterface.
   * \steady  FALSE
   */
  RemoteServerInterface() = default;
  /*!
   * \brief   Destructor of RemoteServerInterface.
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  virtual ~RemoteServerInterface() noexcept = default;

  RemoteServerInterface(RemoteServerInterface const&) = delete;
  RemoteServerInterface(RemoteServerInterface&&) = delete;
  RemoteServerInterface& operator=(RemoteServerInterface const&) & = delete;
  RemoteServerInterface& operator=(RemoteServerInterface&&) & = delete;

  /*!
   * \brief Triggers a SOME/IP event subscription
   *
   * \param[in] event_id  The event id.
   * \param[in] observer  A pointer to the subscribed application.
   * \param[in] client_id The client id.
   *
   * \return The state of the event subscription
   *
   * \context App
   * \steady  FALSE
   */
  virtual eventgroup::EventSubscriptionState SubscribeSomeIpEvent(
      someip_protocol::internal::EventId const event_id, SomeIpEventObserverPtr const observer,
      someip_protocol::internal::ClientId const client_id) noexcept = 0;

  /*!
   * \brief Triggers a PDU event subscription
   *
   * \param[in] event_id The event id.
   * \param[in] observer A pointer to the subscribed application.
   *
   * \return The state of the event subscription
   *
   * \context App
   * \steady  FALSE
   */
  virtual eventgroup::EventSubscriptionState SubscribePduEvent(someip_protocol::internal::EventId const event_id,
                                                               PduEventObserverPtr const observer) noexcept = 0;

  /*!
   * \brief Triggers a SOME/IP event unsubscription
   *
   * \param[in] event_id  The event id.
   * \param[in] observer A pointer to the subscribed application.
   *
   * \context App
   * \steady  FALSE
   */
  virtual void UnsubscribeSomeIpEvent(someip_protocol::internal::EventId const event_id,
                                      SomeIpEventObserverPtr const observer) noexcept = 0;

  /*!
   * \brief Triggers a PDU event unsubscription
   *
   * \param[in] event_id  The event id.
   * \param[in] observer A pointer to the subscribed application.
   *
   * \context App
   * \steady  FALSE
   */
  virtual void UnsubscribePduEvent(someip_protocol::internal::EventId const event_id,
                                   PduEventObserverPtr const observer) noexcept = 0;

  /*!
   * \brief Called when a service has been offered.
   *
   * \param[in] remote_server_address The remote server address.
   *
   * \pre -
   * \context Network, Init (if Static SD is used)
   * \steady  FALSE
   */
  virtual void OnServiceOffered(ServiceAddress const& remote_server_address) noexcept = 0;

  /*!
   * \brief Called when a service has been stopped being offered.
   *
   * \pre -
   * \context Network, Shutdown (if Static SD is used)
   * \steady  FALSE
   */
  virtual void OnServiceStopped() noexcept = 0;

  /*!
   * \brief       Forwards a local SOME/IP request message.
   * \param[in]   packet      A SOME/IP request message.
   *
   * \return void if the message was sent successfully, and error otherwise.
   *
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual ara::core::Result<void> SendMethodRequest(Packet& packet) noexcept = 0;

  /*!
   * \brief Initialize the Static Service Discovery.
   *
   * \param[in] remote_server_address       Unicast IP address and port of the remote server
   * \param[in] event_multicast_endpoint    Optional multicast endpoint for multicast event reception with Static SD
   *
   * \pre     Service discovery is disabled
   * \context Init
   * \steady  FALSE
   */
  virtual void InitializeStaticSD(
      amsr::someip_daemon_core::configuration::types::ServiceAddress const& remote_server_address,
      ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> const&
          event_multicast_endpoint) noexcept = 0;

  /*!
   * \brief Called by the packet router when a SomeIp event is received.
   *
   * \param[in] instance_id The instance id
   * \param[in] packet      A SOME/IP notification message
   *
   * \pre     -
   * \steady  TRUE
   * \context Network
   */
  virtual void OnSomeIpEvent(someip_protocol::internal::InstanceId const instance_id,
                             client::Packet const& packet) noexcept = 0;

  /*!
   * \brief Called by the packet router when an UDP event is received.
   *
   * \param[in] instance_id The instance id
   * \param[in] packet      A PDU notification message
   *
   * \pre     -
   * \steady  TRUE
   * \context Network
   */
  virtual void OnPduEvent(someip_protocol::internal::InstanceId const instance_id,
                          client::PduPacket const& packet) noexcept = 0;
};
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_INTERFACE_H_

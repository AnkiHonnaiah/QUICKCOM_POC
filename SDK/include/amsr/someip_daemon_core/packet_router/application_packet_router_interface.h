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
/**        \file  application_packet_router_interface.h
 *        \brief  Application Packet Router Interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_APPLICATION_PACKET_ROUTER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_APPLICATION_PACKET_ROUTER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/someip_daemon_core/client/event_handler_interface.h"
#include "amsr/someip_daemon_core/client/method_response_handler_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "amsr/someip_daemon_core/server/method_request_handler_interface.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {

/*!
 * \brief Type alias for a local packet sink type
 */
using LocalPacketSinkType = packet_sink::LocalPacketSink;
/*!
 * \brief Type alias for the shared pointer to LocalPacketSink.
 */
using LocalPacketSinkSharedPtr = std::shared_ptr<LocalPacketSinkType>;
/*!
 * \brief Type alias for the strong datatype SomeIpServiceInstanceIdCommunication.
 * \details Contains SomeIp Interface Version, Instance Id and Major Version.
 */
using SomeIpServiceInstanceIdCommunication = configuration::types::SomeIpServiceInstanceIdCommunication;

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief Application PacketRouter.
 */
class ApplicationPacketRouterInterface {
 public:
  /*!
   * \brief     Constructs the packet router interface.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  ApplicationPacketRouterInterface() = default;
  /*!
   * \brief     Destroys the packet router interface.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~ApplicationPacketRouterInterface() = default;

  ApplicationPacketRouterInterface(ApplicationPacketRouterInterface const &) = delete;
  ApplicationPacketRouterInterface(ApplicationPacketRouterInterface &&) = delete;
  ApplicationPacketRouterInterface &operator=(ApplicationPacketRouterInterface const &) & = delete;
  ApplicationPacketRouterInterface &operator=(ApplicationPacketRouterInterface &&) & = delete;

  /*!
   * \brief       Adds a new routing entry for providers of SOME/IP method requests.
   * \param[in]   service_instance_id SOME/IP service instance id.
   * \param[in]   request_handler     A request handler application.
   * \pre         The shared pointer to the destination packet sink shall not be a "nullptr".
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void AddRequestProviderRoute(SomeIpServiceInstanceIdCommunication const &service_instance_id,
                                       server::MethodRequestHandler &request_handler) = 0;

  /*!
   * \brief       Deletes a previously added routing entry for providers of SOME/IP method requests.
   * \param[in]   service_instance_id SOME/IP service instance id.
   * \pre         -
   * \context     App | Timer | Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void DeleteRequestProviderRoute(SomeIpServiceInstanceIdCommunication const &service_instance_id) = 0;

  /*!
   * \brief       Adds a new routing entry for consumers of SOME/IP method responses.
   * \param[in]   service_instance_id SOME/IP service instance id.
   * \param[in]   client_id   SOME/IP client id.
   * \param[in]   to          A reference to the method response handler.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void AddResponseConsumerRoute(SomeIpServiceInstanceIdCommunication const &service_instance_id,
                                        someip_protocol::internal::ClientId const client_id,
                                        client::MethodResponseHandlerInterface &to) = 0;

  /*!
   * \brief        Deletes a previously added routing entry for consumers of SOME/IP method responses.
   * \param[in]    service_instance_id SOME/IP instance ID.
   * \param[in]    client_id   SOME/IP client ID.
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual void DeleteResponseConsumerRoute(SomeIpServiceInstanceIdCommunication const &service_instance_id,
                                           someip_protocol::internal::ClientId const client_id) = 0;

  /*!
   * \brief       Forwards a SOME/IP message received from local source to remote sink.
   * \param[in]   service_instance_id SOME/IP service instance id.
   * \param[in]   packet      A SOME/IP message forwarded to the remote sink (TCP/UDP). The packet
   *                          must be valid (not a nullptr).
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void Forward(someip_protocol::internal::InstanceId const instance_id,
                       std::shared_ptr<someip_protocol::internal::SomeIpMessage> const &packet) = 0;

  /*!
   * \brief       Register a remote server.
   * \param[in]   instance_id_communication The service instance id to identify the server
   * \param[in]   remote_server  Pointer to a remote server.
   *
   * \details     Every remote server must be registered once during its initialization.
   * \pre         The remote server can not be already registered with the same instance id. The API must be
   *              called only once per instance id.
   *
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void RegisterRemoteServer(SomeIpServiceInstanceIdCommunication const &instance_id_communication,
                                    client::RemoteServerInterface &remote_server) noexcept = 0;

  /*!
   * \brief       Unregister a remote server.
   * \param[in]   instance_id_communication The service instance id to identify the server
   *
   * \details     Every remote server must be registered once during its deinitialization.
   * \pre         The remote server must be unregistered before unregistration. The API must be
   *              called only once per instance id.
   *
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void UnregisterRemoteServer(
      SomeIpServiceInstanceIdCommunication const &instance_id_communication) noexcept = 0;
};

}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_APPLICATION_PACKET_ROUTER_INTERFACE_H_

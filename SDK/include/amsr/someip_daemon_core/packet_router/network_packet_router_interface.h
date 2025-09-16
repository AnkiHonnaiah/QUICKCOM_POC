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
/**        \file  network_packet_router_interface.h
 *        \brief  Network Packet Router Interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_NETWORK_PACKET_ROUTER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_NETWORK_PACKET_ROUTER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {

/*!
 * \brief Remote source identifier.
 */
struct RemoteSourceIdentifier {
  /*!
   * \brief Remote source IP address.
   */
  someip_daemon_core::IpAddress const address_;

  /*!
   * \brief Remote source port.
   */
  amsr::net::ip::Port const port_;

  /*!
   * \brief Remote source protocol.
   */
  configuration::ConfigurationTypesAndDefs::Protocol const protocol_;
};
/*!
 * \brief Type alias for a remote packet sink type
 */
using RemotePacketSinkType = packet_sink::RemotePacketSink;
/*!
 * \brief Type alias for the shared pointer to RemotePacketSink.
 */
using RemotePacketSinkSharedPtr = std::shared_ptr<RemotePacketSinkType>;

/*!
 * \brief PacketRouter.
 */
class NetworkPacketRouterInterface {
 public:
  /*!
   * \brief     Constructs the network packet router interface.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  NetworkPacketRouterInterface() = default;
  /*!
   * \brief     Destroys the network packet router interface.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~NetworkPacketRouterInterface() = default;

  NetworkPacketRouterInterface(NetworkPacketRouterInterface const &) = delete;
  NetworkPacketRouterInterface(NetworkPacketRouterInterface &&) = delete;
  NetworkPacketRouterInterface &operator=(NetworkPacketRouterInterface const &) & = delete;
  NetworkPacketRouterInterface &operator=(NetworkPacketRouterInterface &&) & = delete;

  /*!
   * \brief       Forwards a SOME/IP message received from remote source to local sink.
   * \details     The message header is validated, and checked against access rights. Only if all checks pass, it will
   *              be forwarded to the local sink. Otherwise, error will be handled.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   packet      A SOME/IP message forwarded to the local sink (Application). The packet
   *                          must be valid (not a nullptr).
   * \param[in]   reply_to    Local packet sink to which the response (if any) will be sent.
   * \param[in]   source_id   Identifier of the remote packet source.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void OnPacketReceived(someip_protocol::internal::InstanceId const instance_id,
                                std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet,
                                RemotePacketSinkSharedPtr reply_to, RemoteSourceIdentifier const &source_id) = 0;

  /*!
   * \brief       Forwards a PDU message received from remote source to local sink.
   * \details     The message header is validated, and checked against access rights. Only if all checks pass, it will
   *              be forwarded to the local sink. Otherwise, error will be handled.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   packet      A PDU message forwarded to the local sink (Application). The packet
   *                          must be valid (not a nullptr).
   * \param[in]   source_id   Identifier of the remote packet source.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void OnPduPacketReceived(someip_protocol::internal::InstanceId const instance_id,
                                   std::shared_ptr<someip_protocol::internal::PduMessage> const &packet,
                                   RemoteSourceIdentifier const &source_id) = 0;
};

}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_NETWORK_PACKET_ROUTER_INTERFACE_H_

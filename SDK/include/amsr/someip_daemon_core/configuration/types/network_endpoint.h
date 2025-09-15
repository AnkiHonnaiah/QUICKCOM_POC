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
/**        \file  network_endpoint.h
 *        \brief  Network Endpoint
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_NETWORK_ENDPOINT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_NETWORK_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/network.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/port.h"
#include "ara/core/optional.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief Network Endpoint that defines the IP Address / port of the machine.
 */
struct NetworkEndpoint {
  /*!
   * \brief The network endpoint IP address (IPv4 or IPv6).
   */
  someip_daemon_core::IpAddress address;

  /*!
   * \brief Network object representing IP address and network mask (IPv4) or IP address and prefix length (IPv6).
   */
  ::amsr::net::ip::Network network;

  /*!
   * \brief UdpPort configuration that is used for Method and Event communication in IP-Unicast case.
   *  During SOME/IP Service Discovery: PortNumber that is sent in the SD-Offer Message to client (answer on SD-find) or
   *  clients (SD-offer).
   *  Method: This is the destination-port where the server accepts the method call messages (from the clients). This is
   *  the source-port where the server sends the method response messages (to the client).
   *  Event: This is the event source-port where the server sends the event messages to the subscribed clients in
   *  IP-Unicast case.
   */
  ::ara::core::Optional<configuration::types::Port> udp_port;

  /*!
   * \brief TcpPort configuration that is used for Method and Event communication in IP-Unicast case.
   *  During SOME/IP Service Discovery: PortNumber that is sent in the SD-Offer Message to client (answer on SD-find) or
   *  clients (SD-offer).
   *  Method: This is the destination-port where the server accepts the method call messages (from the clients). This is
   *  the source-port where the server sends the method response messages (to the client).
   *  Event: This is the event source-port where the server sends the event messages to the subscribed clients in
   *  IP-Unicast case.
   */
  ::ara::core::Optional<configuration::types::Port> tcp_port;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_NETWORK_ENDPOINT_H_

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
/**        \file  service_address.h
 *        \brief  Service Address
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SERVICE_ADDRESS_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SERVICE_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "ara/core/optional.h"
#include "port.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {
/*!
 * \brief A TCP/UDP endpoint.
 */
struct NetworkEndpointAddress {
  /*!
   * \brief An IP address of a provided service instance.
   */
  someip_daemon_core::IpAddress address;

  /*!
   * \brief TCP/UDP port number.
   */
  configuration::types::Port port;
};

/*!
 * \brief A service unicast address.
 * \details One service could be provided/consumed at TCP endpoint and/or UDP endpoint.It is possible also that no
 *  address is set (e.g. in case of multicast only communication).
 */
struct ServiceAddress {
  /*!
   * \brief A network endpoint address for TCP service.
   */
  ara::core::Optional<NetworkEndpointAddress> tcp_endpoint;
  /*!
   * \brief A network endpoint address for UDP service.
   */
  ara::core::Optional<NetworkEndpointAddress> udp_endpoint;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SERVICE_ADDRESS_H_

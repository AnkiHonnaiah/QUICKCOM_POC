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
/**        \file  ip_endpoint_option.h
 *        \brief  Data structure for IP Endpoint Service Discovery options
 *
 *      \details  This data structure contains the parameters of a Service Discovery IP endpoint option
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_IP_ENDPOINT_OPTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_IP_ENDPOINT_OPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace options {

/*!
 * \brief Represents a SOME/IP SD option.
 */
struct IpEndpointOption {
  /*!
   * \brief The endpoint IP address
   */
  amsr::net::ip::Address address;

  /*!
   * \brief The layer 4 port
   */
  amsr::net::ip::Port port{};

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Compares SOME/IP SD endpoint options.
   *
   * \param[in] other A SOME/IP SD endpoint option to compare to.
   * \return true if both entries are equal and false otherwise.
   * \pre -
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  bool operator==(IpEndpointOption const& other) const {
    bool const is_same_address{address == other.address};
    return (is_same_address) && (port.port == other.port.port);
  }
};

}  // namespace options
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_IP_ENDPOINT_OPTION_H_

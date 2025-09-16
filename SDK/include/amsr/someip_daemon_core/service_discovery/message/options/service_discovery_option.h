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
/**        \file  service_discovery_option.h
 *        \brief  SD option in wire format
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_SERVICE_DISCOVERY_OPTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_SERVICE_DISCOVERY_OPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/net/ip/address.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace options {

/*!
 * \brief SOME/IP SD endpoint option types.
 */
enum class SomeIpSdEndpointOptionType : std::uint8_t {
  /*!
   * \brief Unknown SOME/IP SD option
   */
  kUnknownOption = 0x00,
  /*!
   * \brief SOME/IP SD configuration option. Non-supported option. Upon reception, It will be dropped
   */
  kConfigurationOption = 0x01,
  /*!
   * \brief SOME/IP SD load balancing option. Non-supported option. Upon reception, It will be dropped
   */
  kLoadBalancingOption = 0x02,
  /*!
   * \brief SOME/IP SD IPv4 unicast endpoint option
   * \trace SPEC-4981548
   */
  kIPv4UnicastEndpointOption = 0x04,
  /*!
   * \brief SOME/IP SD IPv6 unicast endpoint option
   * \trace SPEC-4981555
   * \trace SPEC-10144559
   */
  kIPv6UnicastEndpointOption = 0x06,
  /*!
   * \brief SOME/IP SD IPv4 multicast endpoint option
   * \trace SPEC-4981562
   */
  kIPv4MulticastEndpointOption = 0x14,
  /*!
   * \brief SOME/IP SD IPv6 multicast endpoint option
   * \trace SPEC-4981567
   * \trace SPEC-10144574
   */
  kIPv6MulticastEndpointOption = 0x16,
  /*!
   * \brief SOME/IP SD IPv4 endpoint option. Non-supported option. Upon reception, It will be dropped
   */
  kIPv4SDEndpointOption = 0x24,
  /*!
   * \brief SOME/IP SD IPv6 endpoint option. Non-supported option. Upon reception, It will be dropped
   */
  kIPv6SDEndpointOption = 0x26
};

/*!
 * \brief SOME/IP SD endpoint option protocols
 */
enum class SomeIpSdEndpointOptionProto : std::uint8_t {
  /*!
   * \brief Unknown endpoint option protocol
   */
  kUnknown = 0x00,
  /*!
   * \brief TCP endpoint option protocol
   */
  kTcp,
  /*!
   * \brief UDP endpoint option protocol
   */
  kUdp
};

/*!
 * \brief Represents a SOME/IP SD option.
 */
struct ServiceDiscoveryOption {
  /*!
   * \brief Type of service discovery option
   */
  SomeIpSdEndpointOptionType type_{};

  /*!
   * \brief The endpoint IP address
   */
  amsr::net::ip::Address address_{};

  /*!
   * \brief The layer 4 protocol
   */
  SomeIpSdEndpointOptionProto proto_{};

  /*!
   * \brief The layer 4 port
   */
  amsr::net::ip::Port port_{};

  /*!
   * \brief Flag to indicate that this option is discardable
   */
  bool discardable{};

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Compares SOME/IP SD options.
   *
   * \param[in] other A SOME/IP SD option to compare to.
   * \return true if both options are equal and false otherwise.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  bool operator==(ServiceDiscoveryOption const& other) const {
    bool const is_same_address{address_ == other.address_};
    return (type_ == other.type_) && (is_same_address) && (proto_ == other.proto_) && (port_.port == other.port_.port);
  }
};

/*!
 * \brief A container for SD options.
 */
using ServiceDiscoveryOptionContainer = ara::core::Vector<ServiceDiscoveryOption>;

}  // namespace options
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_OPTIONS_SERVICE_DISCOVERY_OPTION_H_

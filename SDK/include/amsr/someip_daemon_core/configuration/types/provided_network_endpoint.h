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
/**        \file  provided_network_endpoint.h
 *        \brief  Represents a provided network endpoint from a server with its network configuration. Provided
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PROVIDED_NETWORK_ENDPOINT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PROVIDED_NETWORK_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "ara/core/optional.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief Provided Network Endpoint that defines the unicast endpoint of the machine and the multicast endpoint if used.
 */
class ProvidedNetworkEndpoint final {
 public:
  ProvidedNetworkEndpoint() = delete;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedNetworkEndpoint() = default;

  /*!
   * \brief Builds a provided network endpoint from the machine configuration.
   *
   * \param[in] machine_mapping A machine mapping that contains the network configuration.
   * \steady FALSE
   */
  explicit ProvidedNetworkEndpoint(
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping) noexcept;

  /*!
   * \brief Default copy constructor.
   * \steady FALSE
   */
  ProvidedNetworkEndpoint(ProvidedNetworkEndpoint const&) noexcept = default;

  ProvidedNetworkEndpoint(ProvidedNetworkEndpoint&&) noexcept = delete;
  auto operator=(ProvidedNetworkEndpoint const& other) noexcept -> ProvidedNetworkEndpoint& = delete;
  auto operator=(ProvidedNetworkEndpoint&& other) noexcept -> ProvidedNetworkEndpoint& = delete;

  /*!
   * \brief Get the local endpoint (TCP or/and UDP ports and addresses).
   * \steady FALSE
   */
  NetworkEndpoint GetLocalEndpoint() const noexcept;

  /*!
   * \brief Get the multicast endpoint.
   * \pre HasMulticastEndpoint() = true
   * \steady FALSE
   */
  NetworkEndpointAddress GetMulticastEndpoint() const noexcept;

  /*!
   * \brief Returns true if the provided network endpoint has a multicast endpoint.
   * \steady FALSE
   */
  bool HasMulticastEndpoint() const noexcept;

 private:
  /*!
   * \brief An endpoint containing the unicast information about the provided server.
   */
  NetworkEndpoint local_network_{};

  /*!
   * \brief An endpoint containing the multicast information about the provided sever.
   */
  ara::core::Optional<NetworkEndpointAddress> multicast_endpoint_{ara::core::nullopt_t()};
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PROVIDED_NETWORK_ENDPOINT_H_

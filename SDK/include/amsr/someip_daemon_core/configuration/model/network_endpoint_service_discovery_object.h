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
/*!     \file     network_endpoint_service_discovery_object.h
 *      \brief    Configuration object for 'network_endpoint_service_discovery'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_SERVICE_DISCOVERY_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_SERVICE_DISCOVERY_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a 'Network endpoint service discovery' object which is filled by a
 * parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class NetworkEndpointServiceDiscoveryConfigObject final {
 public:
  /*!
   * \brief Multicast address element.
   */
  using MulticastAddressElement = CfgElement<someip_daemon_core::IpAddress>;

  /*!
   * \brief Port element.
   */
  using PortElement = CfgElement<someip_protocol::internal::Port>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] network_endpoint_service_discovery Reference to the 'Network endpoint service discovery' structure
   * to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit NetworkEndpointServiceDiscoveryConfigObject(
      ConfigurationTypesAndDefs::NetworkEndpointServiceDiscovery& network_endpoint_service_discovery) noexcept
      : multicast_address_{network_endpoint_service_discovery.multicast_address_},
        port_{network_endpoint_service_discovery.port_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~NetworkEndpointServiceDiscoveryConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  NetworkEndpointServiceDiscoveryConfigObject(NetworkEndpointServiceDiscoveryConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  NetworkEndpointServiceDiscoveryConfigObject(NetworkEndpointServiceDiscoveryConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(NetworkEndpointServiceDiscoveryConfigObject const&) & -> NetworkEndpointServiceDiscoveryConfigObject& =
      delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(NetworkEndpointServiceDiscoveryConfigObject&&) & -> NetworkEndpointServiceDiscoveryConfigObject& =
      delete;

  /*!
   * \brief Set the Multicast address when the parser finds the Multicast address.
   * \param[in] multicast_address Multicast address.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMulticastAddress(someip_daemon_core::IpAddress const& multicast_address) noexcept {
    multicast_address_.SetElement(multicast_address);
  }

  /*!
   * \brief Getter for the Multicast address element.
   * \return Reference to the Multicast address  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  MulticastAddressElement const& GetMulticastAddress() const noexcept { return multicast_address_; }

  /*!
   * \brief Set the Port when the parser finds the Port.
   * \param[in] port Port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPort(someip_protocol::internal::Port const& port) noexcept { port_.SetElement(port); }

  /*!
   * \brief Getter for the Port element.
   * \return Reference to the Port  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PortElement const& GetPort() const noexcept { return port_; }

 private:
  /*!
   * \brief The Multicast address element.
   */
  MulticastAddressElement multicast_address_;

  /*!
   * \brief The Port element.
   */
  PortElement port_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_SERVICE_DISCOVERY_OBJECT_H_

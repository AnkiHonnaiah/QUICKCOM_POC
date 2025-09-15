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
/*!     \file     required_service_instance_provider_object.h
 *      \brief    Config object for 'required_service_instance_provider'.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_PROVIDER_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_PROVIDER_OBJECT_H_

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
 * \brief This is an intermediate representation of a 'Static sd required service instance provideer' object which is
 * filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class RequiredServiceInstanceProviderConfigObject final {
 public:
  /*!
   * \brief Address element.
   */
  using AddressElement = CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>>;

  /*!
   * \brief Udp port element.
   */
  using UdpPortElement = CfgElement<ara::core::Optional<someip_protocol::internal::Port>>;

  /*!
   * \brief Tcp port element.
   */
  using TcpPortElement = CfgElement<ara::core::Optional<someip_protocol::internal::Port>>;

  /*!
   * \brief Event multicast address element.
   */
  using EventMulticastAddressElement = CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>>;

  /*!
   * \brief Event multicast port element.
   */
  using EventMulticastPortElement = CfgElement<ara::core::Optional<someip_protocol::internal::Port>>;

  /*!
   * \brief Ctor sets the references to the elements of the structure to write to.
   * \param[in] service_instance_provider Reference to the 'Static sd provided service instances
   * subscriber' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit RequiredServiceInstanceProviderConfigObject(
      ConfigurationTypesAndDefs::ServiceProviderConfiguration& service_instance_provider) noexcept
      : address_{service_instance_provider.address_},
        udp_port_{service_instance_provider.udp_port_},
        tcp_port_{service_instance_provider.tcp_port_},
        event_multicast_address_{service_instance_provider.event_multicast_address_},
        event_multicast_port_{service_instance_provider.event_multicast_port_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstanceProviderConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstanceProviderConfigObject(RequiredServiceInstanceProviderConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstanceProviderConfigObject(RequiredServiceInstanceProviderConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(RequiredServiceInstanceProviderConfigObject const&)
      -> RequiredServiceInstanceProviderConfigObject& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(RequiredServiceInstanceProviderConfigObject&&) & -> RequiredServiceInstanceProviderConfigObject& =
      delete;

  /*!
   * \brief Set the Address when the parser finds the Address.
   * \param[in] address Address.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetIpAddress(ara::core::Optional<someip_daemon_core::IpAddress> const& address) noexcept {
    address_.SetElement(address);
  }

  /*!
   * \brief Getter for the Address element.
   * \return Reference to the Address  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  AddressElement const& GetIpAddress() const noexcept { return address_; }

  /*!
   * \brief Set the Udp port when the parser finds the Udp port.
   * \param[in] udp_port Udp port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetUdpPort(someip_protocol::internal::Port const& udp_port) noexcept { udp_port_.SetElement(udp_port); }

  /*!
   * \brief Getter for the Udp port element.
   * \return Reference to the Udp port  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  UdpPortElement const& GetUdpPort() const noexcept { return udp_port_; }

  /*!
   * \brief Set the Tcp port when the parser finds the Tcp port.
   * \param[in] tcp_port Udp port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetTcpPort(someip_protocol::internal::Port const& tcp_port) noexcept { tcp_port_.SetElement(tcp_port); }

  /*!
   * \brief Getter for the Tcp port element.
   * \return Reference to the Tcp port  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TcpPortElement const& GetTcpPort() const noexcept { return tcp_port_; }

  /*!
   * \brief Set the Event Multicast Address when the parser finds the Address.
   * \param[in] address Address.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventMulticastAddress(someip_daemon_core::IpAddress const& address) noexcept {
    event_multicast_address_.SetElement(address);
  }

  /*!
   * \brief Getter for the Event Multicast Address element.
   * \return Reference to the Event Multicast Address of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  EventMulticastAddressElement const& GetEventMulticastAddress() const noexcept { return event_multicast_address_; }

  /*!
   * \brief Set the Event Multicast port when the parser finds the Tcp port.
   * \param[in] port Udp port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventMulticastPort(someip_protocol::internal::Port const& port) noexcept {
    event_multicast_port_.SetElement(port);
  }

  /*!
   * \brief Getter for the Event Multicast port element.
   * \return Reference to the Event Multicast port of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  EventMulticastPortElement const& GetEventMulticastPort() const noexcept { return event_multicast_port_; }

 private:
  /*!
   * \brief The Address element.
   */
  AddressElement address_;

  /*!
   * \brief The Udp port element.
   */
  UdpPortElement udp_port_;

  /*!
   * \brief The Tcp port element.
   */
  TcpPortElement tcp_port_;

  /*!
   * \brief The event multicast IP address.
   */
  EventMulticastAddressElement event_multicast_address_;

  /*!
   * \brief The event multicast port.
   */
  EventMulticastPortElement event_multicast_port_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_PROVIDER_OBJECT_H_

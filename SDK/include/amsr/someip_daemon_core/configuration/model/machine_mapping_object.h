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
/*!        \file  machine_mapping_object.h
 *        \brief  Internal representation of a 'machine mapping' object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_MACHINE_MAPPING_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_MACHINE_MAPPING_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/network.h"
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a 'machine mapping' object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
struct MachineMappingConfigObject final {
  /*!
   * \brief The IP address element.
   */
  CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>> address;

  /*!
   * \brief The communication type element.
   */
  CfgElement<ConfigurationTypesAndDefs::ServiceInstanceCommunicationType> communication_type;

  /*!
   * \brief The UDP port element.
   */
  CfgElement<ara::core::Optional<someip_protocol::internal::Port>> udp_port;

  /*!
   * \brief The TCP port element.
   */
  CfgElement<ara::core::Optional<someip_protocol::internal::Port>> tcp_port;

  /*!
   * \brief The event multicast IP address element.
   */
  CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>> event_multicast_address;

  /*!
   * \brief The event multicast port element.
   */
  CfgElement<someip_protocol::internal::Port> event_multicast_port;

  /*!
   * \brief The Network mask element.
   */
  CfgElement<someip_daemon_core::IpAddress> network_mask;

  /*!
   * \brief The IP address prefix length element.
   */
  CfgElement<someip_protocol::internal::IpAddressPrefixLength> prefix_length;

  /*!
   * \brief The network object representing the IP address and network mask for IPv4
   *        and IP address and prefix length for IPv6.
   */
  CfgElement<amsr::net::ip::Network> network;

  /*!
   * \brief This value indicates whether the communication management is used, meaning sockets will be opened and
   * communication packets will be routed
   */
  CfgElement<bool> communication_enabled;

  /*!
   * \brief This flag indicates whether the dynamic service discovery is used.
   */
  CfgElement<bool> service_discovery_enabled;

  /*!
   * \brief provider of the required service.
   */
  CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::ServiceProviderConfiguration>> provider;

  /*!
   * \brief Subscribers to the provided service.
   */
  CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::ServiceSubscriberConfigurationContainer>> subscribers;

  /*!
   * \brief Provided signal serialized PDUs.
   */
  CfgElement<ConfigurationTypesAndDefs::SignalSerializedPduContainer> provided_pdus;

  /*!
   * \brief UDP collection buffer size threshold.
   */
  CfgElement<ConfigurationTypesAndDefs::UdpCollectionBufferSizeThreshold> udp_collection_buffer_size_threshold;

  /*!
   * \brief The IP address for UDP endpoint of event group.
   */
  CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>> eventgroup_udp_endpoint_address;

  /*!
   * \brief The IP address for TCP endpoint of event group.
   */
  CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>> eventgroup_tcp_endpoint_address;

  /*!
   * \brief The port for UDP endpoint of event group.
   */
  CfgElement<ara::core::Optional<someip_protocol::internal::Port>> eventgroup_udp_endpoint_port;

  /*!
   * \brief The port for TCP endpoint of event group.
   */
  CfgElement<ara::core::Optional<someip_protocol::internal::Port>> eventgroup_tcp_endpoint_port;

  /*!
   * \brief The secure id corresponding to a secure connection. This is only set for a machine mapping corresponding to
   * a required service instance.
   */
  CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::SecureIdContainer>> secure_connection_ids;
};

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Make method to create a new MachineMappingConfigObject
 * \param[in] machine_mapping A MachineMapping instance to create a new MachineMappingConfigObject for.
 * \return A new MachineMappingConfigObject for the passed MachineMapping instance.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline MachineMappingConfigObject MakeMachineMappingConfigObject(
    ConfigurationTypesAndDefs::MachineMapping &machine_mapping) noexcept {
  return {
      CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>>{machine_mapping.address_},
      CfgElement<ConfigurationTypesAndDefs::ServiceInstanceCommunicationType>{machine_mapping.communication_type_},
      CfgElement<ara::core::Optional<someip_protocol::internal::Port>>{machine_mapping.udp_port_},
      CfgElement<ara::core::Optional<someip_protocol::internal::Port>>{machine_mapping.tcp_port_},
      CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>>{machine_mapping.event_multicast_address_},
      CfgElement<someip_protocol::internal::Port>{machine_mapping.event_multicast_port_},
      CfgElement<someip_daemon_core::IpAddress>{machine_mapping.network_mask_},
      CfgElement<someip_protocol::internal::IpAddressPrefixLength>{machine_mapping.prefix_length_},
      CfgElement<amsr::net::ip::Network>{machine_mapping.network_},
      CfgElement<bool>{machine_mapping.communication_enabled_},
      CfgElement<bool>{machine_mapping.service_discovery_enabled_},
      CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::ServiceProviderConfiguration>>{
          machine_mapping.provider_},
      CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::ServiceSubscriberConfigurationContainer>>{
          machine_mapping.subscribers_},
      CfgElement<ConfigurationTypesAndDefs::SignalSerializedPduContainer>{machine_mapping.provided_pdus_},
      CfgElement<ConfigurationTypesAndDefs::UdpCollectionBufferSizeThreshold>{
          machine_mapping.udp_collection_buffer_size_threshold_},
      CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>>{machine_mapping.eventgroup_udp_endpoint_address_},
      CfgElement<ara::core::Optional<someip_daemon_core::IpAddress>>{machine_mapping.eventgroup_tcp_endpoint_address_},
      CfgElement<ara::core::Optional<someip_protocol::internal::Port>>{machine_mapping.eventgroup_udp_endpoint_port_},
      CfgElement<ara::core::Optional<someip_protocol::internal::Port>>{machine_mapping.eventgroup_tcp_endpoint_port_},
      CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::SecureIdContainer>>{
          machine_mapping.secure_connection_ids}};
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the IP address on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] address IP address.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetIpAddressOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, ara::core::Optional<someip_daemon_core::IpAddress> const &address) noexcept {
  object.address.SetElement(address);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the event group UDP IP address on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] address UDP IP address.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetEventGroupUdpIpAddressOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, ara::core::Optional<someip_daemon_core::IpAddress> const &&address) {
  object.eventgroup_udp_endpoint_address.SetElement(address);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the event group TCP IP address on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] address TCP IP address.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetEventGroupTcpIpAddressOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, ara::core::Optional<someip_daemon_core::IpAddress> const &&address) {
  object.eventgroup_tcp_endpoint_address.SetElement(address);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the event group UDP port on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] udp_port UDP port.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetEventGroupUdpPortOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, ara::core::Optional<someip_protocol::internal::Port> const &udp_port) {
  object.eventgroup_udp_endpoint_port.SetElement(udp_port);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the event group TCP port on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] tcp_port TCP port.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetEventGroupTcpPortOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, ara::core::Optional<someip_protocol::internal::Port> const &tcp_port) {
  object.eventgroup_tcp_endpoint_port.SetElement(tcp_port);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief     Set the communication type on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] communication_type Communication type.
 * \pre -
 * \context   Init
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetCommunicationTypeOnMachineMappingConfigObject(
    MachineMappingConfigObject &object,
    ConfigurationTypesAndDefs::ServiceInstanceCommunicationType const communication_type) noexcept {
  object.communication_type.SetElement(communication_type);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Sets the UDP port object on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] udp_port UDP port.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetUdpPortOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, ara::core::Optional<someip_protocol::internal::Port> const &udp_port) noexcept {
  object.udp_port.SetElement(udp_port);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the TCP port on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] tcp_port TCP port.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetTcpPortOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, ara::core::Optional<someip_protocol::internal::Port> const &tcp_port) noexcept {
  object.tcp_port.SetElement(tcp_port);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the event multicast address on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] event_multicast_address Event multicast address in standard dot-decimal or hexadecimal notation.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetEventMulticastAddressOnMachineMappingConfigObject(
    MachineMappingConfigObject &object,
    ara::core::Optional<someip_daemon_core::IpAddress> const &event_multicast_address) noexcept {
  object.event_multicast_address.SetElement(event_multicast_address);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the event multicast port on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] event_multicast_port Event multicast port.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetEventMulticastPortOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, someip_protocol::internal::Port const &event_multicast_port) noexcept {
  object.event_multicast_port.SetElement(event_multicast_port);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the Network mask on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject.
 * \param[in] network_mask Address.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetNetworkMaskOnMachineMappingConfigObject(
    MachineMappingConfigObject &object, someip_daemon_core::IpAddress const &network_mask) noexcept {
  object.network_mask.SetElement(network_mask);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the IP address prefix length on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] prefix_length Address.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetPrefixLengthOnMachineMappingConfigObject(
    MachineMappingConfigObject &object,
    someip_protocol::internal::IpAddressPrefixLength const &prefix_length) noexcept {
  object.prefix_length.SetElement(prefix_length);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Create and set the network object after IP and network mask or IP and prefix length are set on a
 * MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \internal
 * - Get the IP address and assert that it is valid.
 * - If the IP address is of IPv4 type,
 *   - Get the network mask and assert that it is valid.
 *   - Use the IP address and the network mask to create a network object.
 * - Else,
 *   - Get the prefix length and assert that it is valid.
 *   - Use the IP address and the prefix length to create a network object.
 * - Assert that the network object creation is a success.
 * - Return the network object or an error if there is any.
 * \endinternal
 * \steady FALSE
 */
static inline ara::core::Result<void> SetNetworkOnMachineMappingConfigObject(
    MachineMappingConfigObject &object) noexcept {
  using Address = someip_daemon_core::IpAddress;
  using Network = amsr::net::ip::Network;

  // Start with an error.
  ara::core::Result<void> result_to_return{osabstraction::MakeErrorCode(osabstraction::OsabErrc::kUnexpected)};

  assert(object.address.GetStatus() == amsr::someip_daemon_core::configuration::ElementStatus::kSet);

  Address const address{object.address.GetElement().value()};
  ara::core::Result<Network> network_res{ara::core::Result<Network>::FromError(osabstraction::OsabErrc::kUnexpected)};

  if (address.IsV4()) {
    assert(object.network_mask.GetStatus() == amsr::someip_daemon_core::configuration::ElementStatus::kSet);

    someip_daemon_core::IpAddress const network_mask{object.network_mask.GetElement()};
    network_res = Network::MakeNetwork(address, network_mask);
  } else {  // IPv6
    assert(object.prefix_length.GetStatus() == amsr::someip_daemon_core::configuration::ElementStatus::kSet);

    someip_protocol::internal::IpAddressPrefixLength const prefix_length{object.prefix_length.GetElement()};
    amsr::net::ip::PrefixLength const prefix_length_struct{static_cast<std::uint8_t>(prefix_length)};
    network_res = Network::MakeNetwork(address, prefix_length_struct);
  }

  if (!network_res.HasValue()) {
    result_to_return.EmplaceError(network_res.Error());
  } else {
    object.network.SetElement(network_res.Value());
    result_to_return.EmplaceValue();  // Clear the error.
  }

  return result_to_return;
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the communication flag on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject.
 * \param[in] flag A Boolean value to enable or disable the use of commmunication.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetCommunicationOnMachineMappingConfigObject(MachineMappingConfigObject &object,
                                                                bool const flag) noexcept {
  object.communication_enabled.SetElement(flag);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the service discovery flag on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject.
 * \param[in] flag A Boolean value to enable or disable the use of Service Discovery.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetServiceDiscoveryOnMachineMappingConfigObject(MachineMappingConfigObject &object,
                                                                   bool const flag) noexcept {
  object.service_discovery_enabled.SetElement(flag);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the service provider struct on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject.
 * \param[in] provider
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetProviderElementOnMachineMappingConfigObject(
    MachineMappingConfigObject &object,
    ara::core::Optional<ConfigurationTypesAndDefs::ServiceProviderConfiguration> const &&provider) noexcept {
  object.provider.SetElement(provider);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the service subscriber struct on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject.
 * \details This function shall be invoked when static service discovery is enabled.
 * \param[in] subscribers
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetSubscribersElementOnMachineMappingConfigObject(
    MachineMappingConfigObject &object,
    ara::core::Optional<ConfigurationTypesAndDefs::ServiceSubscriberConfigurationContainer> const
        &&subscribers) noexcept {
  object.subscribers.SetElement(subscribers);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set the provided signal serialized PDUs on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject.
 * \param[in] provided_pdus
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetProvidedPdusElementOnMachineMappingConfigObject(
    MachineMappingConfigObject &object,
    ConfigurationTypesAndDefs::SignalSerializedPduContainer const &provided_pdus) noexcept {
  object.provided_pdus.SetElement(provided_pdus);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set UdpCollectionBufferSizeThreshold on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] value The threshold itself.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetUdpCollectionBufferSizeThresholdOnMachineMappingConfigObject(
    MachineMappingConfigObject &object,
    ConfigurationTypesAndDefs::UdpCollectionBufferSizeThreshold const &value) noexcept {
  object.udp_collection_buffer_size_threshold.SetElement(value);
}

// VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_FalsePositive
/*!
 * \brief Set SecureConnectionsIds on a MachineMappingConfigObject.
 * \param[in, out] object A reference to a MachineMappingConfigObject
 * \param[in] value The SecureIdContainer itself.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
static inline void SetSecureConnectionIds(MachineMappingConfigObject &object,
                                          ConfigurationTypesAndDefs::SecureIdContainer const &value) noexcept {
  object.secure_connection_ids.SetElement(value);
}

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_MACHINE_MAPPING_OBJECT_H_

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
/**        \file  service_discovery_server_message_builder.h
 *        \brief  Class definition for service discovery server message builder.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_payload_builder.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_message_builder_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * A Service Discovery Server message builder.
 */
class ServiceDiscoveryServerMessageBuilder final : public ServiceDiscoveryServerMessageBuilderInterface {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] address The IP address of this server service instance.
   * \param[in] config Provided Service configuration.
   *
   * \pre The given address shall be in the configuration.
   *
   * \context Init
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ServiceDiscoveryServerMessageBuilder(someip_daemon_core::IpAddress const& address,
                                       configuration::Configuration::IpcChannel::ProvidedServiceInstance const* config);

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Destructor
   *
   * \pre          -
   * \context      Shutdown
   * \steady       FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  ~ServiceDiscoveryServerMessageBuilder() noexcept override = default;

  ServiceDiscoveryServerMessageBuilder(ServiceDiscoveryServerMessageBuilder const&) = delete;
  ServiceDiscoveryServerMessageBuilder(ServiceDiscoveryServerMessageBuilder&&) = delete;
  ServiceDiscoveryServerMessageBuilder& operator=(ServiceDiscoveryServerMessageBuilder const&) & = delete;
  ServiceDiscoveryServerMessageBuilder& operator=(ServiceDiscoveryServerMessageBuilder&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Creates SD OfferService entry
   * \return An OfferServiceEntry
   *
   * \pre          At least one of the ports (TCP/UDP) in the configuration
   *               are valid
   * \context      Timer, Network, App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \trace SPEC-4980349, SPEC-4980068
   */
  message::entries::OfferServiceEntry MakeOfferServiceEntry() const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Returns the service entry Id of this service.
   *
   * \return A service entry id.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  message::entries::ServiceEntryId GetServiceEntryId() const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Creates SD a SD StopOfferService entry.
   *
   * \return A StopOfferService entry
   *
   * \pre          At least one of the ports (TCP/UDP) in the configuration
   *               are valid
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \trace SPEC-4980069
   */
  message::entries::StopOfferServiceEntry MakeStopOfferServiceEntry() const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Creates SD entry for a positive SD entry in response to a SubscribeEventgroup request.
   *
   * \param[in] eventgroup_id  A SOME/IP eventgroup identifier.
   * \param[in] ttl            A TTL value.
   * \param[in] counter        A counter value.
   *
   * \return A entries::SubscribeEventgroupAckEntry.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   *
   * \trace SPEC-4980070
   */
  message::entries::SubscribeEventgroupAckEntry MakeSubscribeEventgroupAckEntry(
      amsr::someip_protocol::internal::EventgroupId const eventgroup_id, amsr::someip_protocol::internal::Ttl const ttl,
      amsr::someip_protocol::internal::SdEntryCounter const counter) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Creates SD entry for a negative SD entry in response to a SubscribeEventgroup request.
   *
   * \param[in] eventgroup_id  A SOME/IP eventgroup identifier.
   * \param[in] counter        A counter value.
   *
   * \return A entries::SubscribeEventgroupNAckEntry.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \trace SPEC-4980071
   */
  message::entries::SubscribeEventgroupNAckEntry MakeSubscribeEventgroupNackEntry(
      amsr::someip_protocol::internal::EventgroupId const eventgroup_id,
      amsr::someip_protocol::internal::SdEntryCounter const counter) const override;

 private:
  /*!
   * \brief The configuration of a provided service instance.
   */
  configuration::Configuration::IpcChannel::ProvidedServiceInstance const* instance_config_;

  /*!
   * \brief The service entry id of this provided service instance.
   */
  message::entries::ServiceEntryId const service_entry_id_{
      instance_config_->service_id_, instance_config_->instance_id_, instance_config_->major_version_,
      instance_config_->minor_version_};

  /*!
   * \brief The corresponding port mapping of this server service instance.
   */
  configuration::Configuration::MachineMapping const* port_mapping_config_{};

  /*!
   * \brief TCP message option type.
   */
  message::options::SomeIpSdEndpointOptionType tcp_option_type_{
      service_discovery::message::options::SomeIpSdEndpointOptionType::kUnknownOption};

  /*!
   * \brief UDP message option type.
   */
  message::options::SomeIpSdEndpointOptionType udp_option_type_{
      service_discovery::message::options::SomeIpSdEndpointOptionType::kUnknownOption};

  /*!
   * \brief Event multicast message option type.
   */
  message::options::SomeIpSdEndpointOptionType multicast_option_type_{
      service_discovery::message::options::SomeIpSdEndpointOptionType::kUnknownOption};

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"ServerMessageBuilder"}};
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_H_

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
/**        \file  eventgroup_sd_message_builder.h
 *        \brief  A class to build service discovery eventgroup messages
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_SD_MESSAGE_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_SD_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_entry.h"
#include "ara/core/map.h"
#include "ara/core/optional.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief A Event group Service Discovery message builder.
 */
class EventgroupSdMessageBuilder final {
 public:
  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

  /*!
   * \brief Constructor of EventgroupSdMessageBuilder.
   *
   * \param[in] service_deployment                 Service deployment configuration to access deployment
   *                                               id and configured events information.
   * \param[in] instance_id                        A SOME/IP service instance identifier.
   * \param[in] udp_endpoint                       The network configuration to access (optional) configured
   *                                               UDP port information.
   * \param[in] required_eventgroups               Required eventgroups timing configuration.
   * \param[in] custom_subscription_endpoints      Optional TCP/UDP endpoints. If configured, these values will be used
   *                                               in as options in the subscription message instead of the ones
   *                                               provided in udp_endpoint or dynamically assigned by the OS (in case
   *                                               of TCP).
   * \steady FALSE
   */
  EventgroupSdMessageBuilder(
      configuration::types::SomeIpServiceInterfaceDeployment const& service_deployment,
      amsr::someip_protocol::internal::InstanceId const instance_id,
      ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> const& udp_endpoint,
      RequiredEventgroupsContainer const& required_eventgroups,
      configuration::types::ServiceAddress const& custom_subscription_endpoints) noexcept;

  /*!
   * \brief Destructor of EventgroupSdMessageBuilder.
   * \steady  FALSE
   */
  ~EventgroupSdMessageBuilder() noexcept = default;

  EventgroupSdMessageBuilder(EventgroupSdMessageBuilder const&) = delete;
  EventgroupSdMessageBuilder(EventgroupSdMessageBuilder&&) = delete;
  EventgroupSdMessageBuilder& operator=(EventgroupSdMessageBuilder const&) & = delete;
  EventgroupSdMessageBuilder& operator=(EventgroupSdMessageBuilder&&) & = delete;

  /*!
   * \brief Builds a SOME/IP-SD SubscribeEventgroup entry.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return A SubscribeEventgroup entry.
   * \pre   -
   * \context App
   * \steady FALSE
   *
   * \trace SPEC-6334968
   */
  service_discovery::message::entries::SubscribeEventgroupEntry MakeSubscribeEventgroupEntry(
      someip_protocol::internal::EventgroupId const eventgroup_id) const noexcept;

  /*!
   * \brief Builds a SOME/IP-SD StopSubscribeEventgroup entry.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return A SubscribeEventgroup entry.
   * \pre   -
   * \context App
   * \steady FALSE
   *
   * \trace SPEC-4980072
   * \trace SPEC-4981625
   * \trace SPEC-4981626
   * \trace SPEC-4981694
   */
  service_discovery::message::entries::StopSubscribeEventgroupEntry MakeStopSubscribeEventgroupEntry(
      someip_protocol::internal::EventgroupId const eventgroup_id) const noexcept;

  /*!
   * \brief Registers a local TCP endpoint, used for sending subscription requests
   *
   * \param[in] local_tcp_network_endpoint The local tcp endpoint
   *
   * \pre   -
   * \context App, Netework
   * \steady FALSE
   *
   * \trace SPEC-6334968
   */
  void RegisterLocalTcpNetworkEndpoint(
      configuration::types::NetworkEndpoint const& local_tcp_network_endpoint) noexcept;

 private:
  /*!
   * \brief The service interface deployment
   * \details Used in this class to access the deployment id and the list of configured
   *          events
   */
  configuration::types::SomeIpServiceInterfaceDeployment const service_deployment_;

  /*!
   * \brief The local address (TCP and UDP) to receive event notifications
   */
  amsr::someip_daemon_core::configuration::types::ServiceAddress local_endpoints_{};

  /*!
   * \brief Required eventgroup configuration.
   */
  RequiredEventgroupsContainer const required_eventgroups_;

  /*!
   * \brief The service instance id
   */
  someip_protocol::internal::InstanceId const instance_id_;

  /*!
   * \brief   A flag for TCP custom address.
   * \details Is set to true when a TCP address is provided in custom_subscriber_endpoint_options and when set to true,
   *          the TCP address and port provided by the OS during socket creation are ignored.
   */
  bool custom_tcp_address_set_{false};
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_SD_MESSAGE_BUILDER_H_

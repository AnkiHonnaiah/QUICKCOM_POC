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
/**        \file  remote_client_integrator.h
 *        \brief  remote_client_integrator
 *
 *      \details  Combining the RemoteClientCommunication object into RemoteClient and initializing static SD
 *                RemoteClient.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_INTEGRATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_INTEGRATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/init/server_initializer.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/server/remote_client.h"
#include "amsr/someip_daemon_core/server/remote_client_communication.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

template <typename ConnectionManagerType>
class RemoteClientIntegrator final {
 public:
  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;

  /*!
   * \brief Alias for ServiceDiscoveryServer type.
   */
  using ServiceDiscoveryServer = service_discovery::state_machine::server::ServiceDiscoveryServer;

  /*!
   * \brief Type alias for the service instance communication type.
   */
  using ServiceInstanceCommunicationType = configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType;

  /*!
   * \brief Create Remote Client and Integrate with Remote Client Communication.
   *
   * \param[in] someip_service_instance_id     The SOME/IP service instance information
   *                                           (service id, instance id, and version).
   * \param[in] machine_mapping                Machine mapping for the service instance
   * \param[in] conman                         A connection manager.
   * \param[in] config                         A configuration.
   * \param[in] timer_manager                  A timer manager.
   * \param[in] reactor                        Reactor, used to trigger SW events for connection cleaup.
   * \param[in] sd_endpoint                    A pointer to service discovery endpoint.
   * \param[in] provided_service_instance_cfg  The provider service instance configuration.
   * \param[in] communication_type             The communication type.
   *
   * \return    A amsr::UniquePtr of the RemoteClient object.
   *
   * \pre       -
   * \context   Init
   * \steady    FALSE
   *
   * \internal
   * - Create a unique pointer to remote client.
   * - If the service is configured to use SD,
   *  - Assert that sd_endpoint is exist.
   *  - Create and register service discovery server in RemoteClient.
   * - If the service is configured to use communication,
   *  - Setup communication and initialize the Remote Client.
   * - Return the unique pointer to remote client.
   * \endinternal
   */
  static amsr::UniquePtr<amsr::someip_daemon_core::server::RemoteClientInterface> CreateRemoteClient(
      configuration::types::SomeIpServiceInstanceId const someip_service_instance_id,
      configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping, ConnectionManager& conman,
      configuration::Configuration const& config, amsr::steady_timer::TimerManagerInterface* timer_manager,
      osabstraction::io::reactor1::Reactor1Interface& reactor,
      someip_daemon_core::service_discovery::ServiceDiscoveryEndpoint* sd_endpoint,
      configuration::ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer::const_reference
          provided_service_instance_cfg,
      ServiceInstanceCommunicationType const communication_type) noexcept {
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
    amsr::UniquePtr<amsr::someip_daemon_core::server::RemoteClient<ConnectionManager>> remote_client{
        memory::MemoryUtilities::CreateUniquePtr<amsr::someip_daemon_core::server::RemoteClient<ConnectionManager>>()};

    // Create the service discovery server.
    std::shared_ptr<ServiceDiscoveryServer> service_discovery_server{nullptr};

    if (machine_mapping.communication_type_ !=
        configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kCommunicationOnly) {
      assert(sd_endpoint != nullptr);
      // Create and register service discovery server
      service_discovery_server =
          SetupAndRegisterSDServerInRemoteClient(someip_service_instance_id, communication_type, machine_mapping,
                                                 timer_manager, sd_endpoint, config, remote_client);
    }

    if (machine_mapping.communication_type_ !=
        configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kSdOnly) {
      SetupCommunicationAndInitializeRemoteClient(someip_service_instance_id, machine_mapping, conman, reactor, config,
                                                  service_discovery_server, remote_client,
                                                  provided_service_instance_cfg);
    }
    return std::move(remote_client);
  }

 private:
  /*!
   * \brief Create and register Service Discovery Server.
   *
   * \param[in] someip_service_instance_id  The SOME/IP service instance information
   *                                        (service id, instance id, and version).
   * \param[in] communication_type          The communication type.
   * \param[in] machine_mapping             Machine mapping for the service instance
   * \param[in] timer_manager               A timer manager.
   * \param[in] sd_endpoint                 A pointer to the service discovery endpoint.
   * \param[in] config                      The provider service instance configuration.
   * \param[in] remote_client               The RemoteClient object to register the server with.
   *
   * \return    A shared_ptr to the created Service Discovery Server.
   *
   * \pre       -
   * \context   Init
   * \steady    FALSE
   *
   * \internal
   * - Create a new Service Discovery Server instance with the specified parameters.
   * - Register the Service Discovery Server with the provided Service Discovery Endpoint.
   * - Notify the Service Discovery Server about the network being up.
   * - Register the Service Discovery Server inside the given Remote Client.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A18.1.5: MD_SomeIpDaemon_AutosarC++17_10-A18.1.5_const_ref_unique_ptr
  static std::shared_ptr<ServiceDiscoveryServer> SetupAndRegisterSDServerInRemoteClient(
      configuration::types::SomeIpServiceInstanceId const someip_service_instance_id,
      ServiceInstanceCommunicationType const communication_type,
      configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping,
      amsr::steady_timer::TimerManagerInterface* timer_manager,
      someip_daemon_core::service_discovery::ServiceDiscoveryEndpoint* sd_endpoint,
      configuration::Configuration const& config,
      amsr::UniquePtr<amsr::someip_daemon_core::server::RemoteClient<ConnectionManager>> const& remote_client) {
    someip_daemon_core::configuration::types::RequiredNetworkEndpoint const local_network_endpoint{
        configuration::util::ConfigurationTypesBuilder::BuildRequiredNetworkEndpoint(machine_mapping)};

    std::shared_ptr<ServiceDiscoveryServer> const service_discovery_server{
        someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<ServiceDiscoveryServer>(
            someip_service_instance_id.deployment_id.service_interface_id, someip_service_instance_id.instance_id,
            someip_service_instance_id.deployment_id.service_interface_version.major_version,
            someip_service_instance_id.deployment_id.service_interface_version.minor_version, communication_type,
            local_network_endpoint, timer_manager, sd_endpoint, &config)};

    typename service_discovery::ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const
        sd_endpoint_observer_key{someip_service_instance_id.deployment_id.service_interface_id,
                                 someip_service_instance_id.instance_id,
                                 someip_service_instance_id.deployment_id.service_interface_version.major_version,
                                 someip_service_instance_id.deployment_id.service_interface_version.minor_version};
    sd_endpoint->RegisterServer(sd_endpoint_observer_key, service_discovery_server);
    service_discovery_server->OnNetworkUp();

    // Register service discovery server inside the remote client.
    remote_client->RegisterServiceDiscoveryServer(service_discovery_server);

    return service_discovery_server;
  }

  /*!
   * \brief Setup communication in Remote Client and Initialize Service Discovery Server.
   *
   * \param[in] someip_service_instance_id     The SOME/IP service instance information
   *                                           (service id, instance id, and version).
   * \param[in] machine_mapping                Machine mapping for the service instance
   * \param[in] conman                         A connection manager.
   * \param[in] reactor                        Reactor, used to trigger SW events for connection cleaup.
   * \param[in] config                         A configuration.
   * \param[in] service_discovery_server       A ServiceDiscoveryServer type.
   * \param[in] remote_client                  The RemoteClient object to register the server with.
   * \param[in] provided_service_instance_cfg  The provider service instance configuration.
   *
   * \pre       -
   * \context   Init
   * \steady    FALSE
   *
   * \internal
   *  - Create a shared pointer to RemoteClientCommunication.
   *  - If SD is enabled,
   *   - Get event manager from the service discovery server and set it inside remote client communication.
   *   - Register service discovery server inside event manager.
   *  - Register service discovery server inside the remote client.
   *  - If the service is configured to use communication, call Initialize StaticSd Remote Client.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A18.1.5: MD_SomeIpDaemon_AutosarC++17_10-A18.1.5_const_ref_unique_ptr
  static void SetupCommunicationAndInitializeRemoteClient(
      configuration::types::SomeIpServiceInstanceId const someip_service_instance_id,
      configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping, ConnectionManager& conman,
      osabstraction::io::reactor1::Reactor1Interface& reactor, configuration::Configuration const& config,
      std::shared_ptr<ServiceDiscoveryServer> const& service_discovery_server,
      amsr::UniquePtr<amsr::someip_daemon_core::server::RemoteClient<ConnectionManager>> const& remote_client,
      configuration::ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer::const_reference
          provided_service_instance_cfg) {
    ara::core::Map<someip_protocol::internal::EventgroupId, configuration::types::ProvidedSomeIpEventgroup> const
        provided_eventgroup_map{configuration::util::ConfigurationTypesBuilder::BuildProvidedSomeIpEventgroups(
            someip_service_instance_id, machine_mapping, config)};

    configuration::types::ProvidedNetworkEndpoint const provided_endpoint{machine_mapping};

    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
    std::shared_ptr<RemoteClientCommunication<ConnectionManager>> const remote_client_communication{
        someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<RemoteClientCommunication<ConnectionManager>>(
            someip_service_instance_id, conman, provided_endpoint,
            config.GetEventAccumulationTimeouts(someip_service_instance_id.deployment_id.service_interface_id,
                                                someip_service_instance_id.instance_id),
            provided_eventgroup_map, reactor)};

    if (service_discovery_server != nullptr) {
      server::eventgroup::EventManager& event_manager{service_discovery_server->GetEventManager()};
      remote_client_communication->SetEventManager(&event_manager);
      event_manager.RegisterRemoteClientCommunication(remote_client_communication);
    }

    remote_client->RegisterRemoteClientCommunication(remote_client_communication);

    if (machine_mapping.communication_type_ ==
        configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kCommunicationOnly) {
      // PTP-B-SomeIpDaemon-ServerInitializer_InitializeStaticSdRemoteClient
      // Handle static provided service instances
      InitializeStaticSdRemoteClient(remote_client_communication.get(), machine_mapping, provided_service_instance_cfg);
      // PTP-E-SomeIpDaemon-ServerInitializer_InitializeStaticSdRemoteClient
    }
  }

  /*!
   * \brief       InitializeStaticSdRemoteClient.
   * \param[in]   remote_client_communication   The Server Event Handler Interface.
   * \param[in]   machine_mapping               The machine mapping.
   * \param[in]   provided_service_instance     The provider service instance configuration.
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - If the subscriber are configured, then register the static eventgroup subscriptions.
   * \endinternal
   */
  static void InitializeStaticSdRemoteClient(
      server::ServerEventHandlerInterface* remote_client_communication,
      configuration::ConfigurationTypesAndDefs::MachineMappingContainer::const_reference machine_mapping,
      configuration::ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer::const_reference
          provided_service_instance) noexcept {
    // If configured, register static eventgroup subscriptions
    if (machine_mapping.subscribers_.has_value()) {
      RegisterStaticEventgroupSubscriptions(remote_client_communication, machine_mapping.subscribers_.value(),
                                            provided_service_instance);
    }
  }

  /*!
   * \brief       Register static eventgroup subscriptions.
   *
   * \param[in]   remote_client_communication   The Server Event Handler Interface.
   * \param[in]   static_subscribers            A container with all subscribers.
   * \param[in]   provided_service_instance     The provider service instance configuration.
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - For each subscriber
   *   - If no eventgroups configured
   *     - Subscribe to all eventgroups offered by the server
   *   - Otherwise
   *     - Subscribe to configured eventgroups
   * \endinternal
   */
  static void RegisterStaticEventgroupSubscriptions(
      server::ServerEventHandlerInterface* remote_client_communication,
      configuration::ConfigurationTypesAndDefs::ServiceSubscriberConfigurationContainer const& static_subscribers,
      configuration::ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer::const_reference
          provided_service_instance) noexcept {
    someip_daemon_core::logging::AraComLogger const logger{
        someip_daemon_core::logging::kRemoteClientManagerLoggerContextId,
        someip_daemon_core::logging::kRemoteClientManagerLoggerContextDescription,
        ara::core::StringView{"RemoteClientIntegrator"}};
    for (configuration::ConfigurationTypesAndDefs::ServiceSubscriberConfiguration const& subscriber :
         static_subscribers) {
      // Only UDP is supported
      ara::core::Optional<service_discovery::message::options::IpEndpointOption> const tcp_endpoint{ara::core::nullopt};

      // The IP address and port from the udp endpoint configuration must be valid
      someip_daemon_core::IpAddress const udp_address{subscriber.address_};
      assert(subscriber.udp_port_ != configuration::ConfigurationTypesAndDefs::kInvalidPort);
      amsr::net::ip::Port const udp_port{subscriber.udp_port_};

      ara::core::Optional<service_discovery::message::options::IpEndpointOption> const udp_endpoint{
          {subscriber.address_, udp_port}};

      logger.LogVerbose(
          [&provided_service_instance, &udp_address, &udp_port](ara::log::LogStream& s) {
            s << "Registering static eventgroup subscriptions (0x";
            s << ara::log::HexFormat(provided_service_instance.service_id_);
            s << ",0x";
            s << ara::log::HexFormat(provided_service_instance.instance_id_);
            s << ",0x";
            s << ara::log::HexFormat(provided_service_instance.major_version_);
            s << ",0x";
            s << ara::log::HexFormat(provided_service_instance.minor_version_);
            s << ") a subscriber at (";
            s << udp_address.ToString();
            s << ":";
            s << udp_port.port;
            s << ") for specific eventgroups";
          },
          static_cast<char const*>(__func__), __LINE__);

      if (subscriber.eventgroup_.empty()) {
        // Subscribe to all eventgroups offered by the server
        for (configuration::ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroup const eventgroup :
             provided_service_instance.service_discovery_.eventgroups_) {
          logger.LogVerbose(
              [&eventgroup](ara::log::LogStream& s) {
                s << "Subscription to eventgroup (0x";
                s << ara::log::HexFormat(eventgroup.id_);
                s << ")";
              },
              static_cast<char const*>(__func__), __LINE__);
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_SomeIpDaemon_M0.3.1_FalsePositive
          static_cast<void>(
              remote_client_communication->SubscribeEventgroup(udp_endpoint, tcp_endpoint, eventgroup.id_));
        }
      } else {
        // Subscribe only to configured eventgroups
        for (someip_protocol::internal::EventgroupId const eventgroup : subscriber.eventgroup_) {
          logger.LogVerbose(
              [&eventgroup](ara::log::LogStream& s) {
                s << "Subscription to eventgroup (0x";
                s << ara::log::HexFormat(eventgroup);
                s << ")";
              },
              static_cast<char const*>(__func__), __LINE__);
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_SomeIpDaemon_M0.3.1_FalsePositive
          static_cast<void>(remote_client_communication->SubscribeEventgroup(udp_endpoint, tcp_endpoint, eventgroup));
        }
      }
    }
  }
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_INTEGRATOR_H_

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
/**        \file  remote_server_factory.h
 *        \brief  Remote Server Factory.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_FACTORY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_FACTORY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <unordered_map>
#include <utility>
#include "amsr/someip_daemon_core/client/remote_server.h"
#include "amsr/someip_daemon_core/client/remote_server_connection_manager_factory.h"
#include "amsr/someip_daemon_core/client/remote_server_factory_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/packet_router/application_packet_router_interface.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief RemoteServerFactory.
 * \tparam ConnectionManagerType Type of ConnectionManager
 * \tparam RemoteServerType      Type of Remote server
 */
template <typename ConnectionManagerType, typename RemoteServerType = client::RemoteServer<ConnectionManagerType>>
class RemoteServerFactory final : public RemoteServerFactoryInterface {
 private:
  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;

  /*!
   * \brief Shared pointer to a RemoteServerInterface.
   */
  using RemoteServerInterfaceSharedPtr = std::shared_ptr<RemoteServerInterface>;

  /*!
   * \brief List of required eventgroups for a specific RemoteServer.
   */
  using RequiredEventgroupsMap =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

  /*!
   * \brief A timeout list for UDP Method message accumulation purposes.
   */
  using MethodsMessageAccumulationTimeoutMap = std::unordered_map<
      someip_protocol::internal::MethodId, std::chrono::nanoseconds, std::hash<someip_protocol::internal::MethodId>,
      std::equal_to<someip_protocol::internal::MethodId>,
      ara::core::PolymorphicAllocator<std::pair<someip_protocol::internal::MethodId const, std::chrono::nanoseconds>>>;

 public:
  /*!
   * \brief     Constructor.
   *
   * \param[in] packet_router                  The packet router
   * \param[in] connection_manager             The connection manager
   * \param[in] timer_manager                  The Timer manager
   * \param[in] reactor                        Reactor
   * \param[in] network_endpoint               The network endpoint
   * \param[in] required_eventgroups           The required eventgroups
   * \param[in] required_pdus                  The required PDUs
   * \param[in] machine_mapping                The machine mapping
   * \param[in] is_communication_only          Whether the requested RemoteServier has SD disabled
   * \param[in] sd_endpoint                    The SD endpoint
   * \param[in] method_props                   The method accumulation timeouts
   * \param[in] custom_subscription_endpoints  The custom subscription endpoints
   * \param[in] sec_com_config                 The secure communication configuration
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RemoteServerFactory(
      std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
      ConnectionManager& connection_manager, amsr::steady_timer::TimerManagerInterface* timer_manager,
      osabstraction::io::reactor1::Reactor1Interface& reactor,
      service_discovery::state_machine::client::ServiceDiscoveryClientInterface* service_discovery_client,
      configuration::types::RequiredNetworkEndpoint const& network_endpoint,
      RequiredEventgroupsMap const& required_eventgroups,
      configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const& required_pdus,
      configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping, bool const is_communication_only,
      service_discovery::message::scheduler::SchedulerInterface* sd_endpoint,
      MethodsMessageAccumulationTimeoutMap const& method_props,
      configuration::types::ServiceAddress const& custom_subscription_endpoints,
      configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config) noexcept
      : RemoteServerFactoryInterface(),
        packet_router_{packet_router},
        connection_manager_{connection_manager},
        timer_manager_{timer_manager},
        reactor_{reactor},
        service_discovery_client_{service_discovery_client},
        network_endpoint_{network_endpoint},
        required_eventgroups_{required_eventgroups},
        required_pdus_{required_pdus},
        machine_mapping_{machine_mapping},
        is_communication_only_{is_communication_only},
        sd_endpoint_{sd_endpoint},
        method_props_{method_props},
        custom_subscription_endpoints_{custom_subscription_endpoints},
        sec_com_config_{sec_com_config} {
    assert(packet_router != nullptr);
  }

  /*!
   * \brief   Destructor of RemoteServerFactory.
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~RemoteServerFactory() noexcept override = default;

  RemoteServerFactory() = delete;
  RemoteServerFactory(RemoteServerFactory const&) = delete;
  RemoteServerFactory(RemoteServerFactory&&) = delete;
  RemoteServerFactory& operator=(RemoteServerFactory const&) & = delete;
  RemoteServerFactory& operator=(RemoteServerFactory&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief  Create the remote server defined in the configuration.
   *
   * \param[in] service_deployment  The service deployment configuration.
   * \param[in] instance_id         A SOME/IP service instance identifier.
   *
   * \return      The remote server.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Create a RemoteServerConnectionManagerFactory object.
   * - Create a RemoteServer object.
   * - Initialize eventgroups.
   * - If SD is disabled
   *   - Initialize static SD.
   * - Return the created and initialized remote server.
   * \endinternal
   */
  RemoteServerInterfaceSharedPtr CreateRemoteServer(
      configuration::types::SomeIpServiceInterfaceDeployment const& service_deployment,
      someip_protocol::internal::InstanceId const instance_id) const noexcept override {
    // Create a remote server connection manager factory object
    // Note: this object is responsible for creating RemoteServerConnectionManager objects, which are responsible for
    // connection handling. The object is injected from outside to enhance testability
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
    amsr::UniquePtr<RemoteServerConnectionManagerFactory<ConnectionManager>> remote_server_connection_manager_factory{
        memory::MemoryUtilities::CreateUniquePtr<RemoteServerConnectionManagerFactory<ConnectionManager>>(
            connection_manager_)};

    std::shared_ptr<RemoteServerType> remote_server{memory::MemoryUtilities::CreateSharedPtr<RemoteServerType>(
        service_deployment, instance_id, network_endpoint_, required_eventgroups_, required_pdus_,
        is_communication_only_, connection_manager_, packet_router_, sd_endpoint_, timer_manager_, reactor_,
        method_props_, custom_subscription_endpoints_, std::move(remote_server_connection_manager_factory),
        sec_com_config_, service_discovery_client_)};

    // Initialize eventgroups
    InitializeEventgroups(service_deployment, instance_id, remote_server);

    // Initialize Static SD
    if (is_communication_only_) {
      InitializeStaticSD(remote_server);
    }

    // Let the compiler cast the shared pointer to the interface the created object implements
    return remote_server;
  }

 private:
  /*!
   * \brief  Initialize eventgroups for a remote server instance
   *
   * \param[in] service_deployment  The service deployment configuration.
   * \param[in] service_deployment  The instance ID.
   * \param[in] remote_server       The remote server
   *
   * \return      The remote server.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - If eventgroups are configured
   *   - Inject a client event dispatcher in the remote server
   * \endinternal
   */
  static void InitializeEventgroups(configuration::types::SomeIpServiceInterfaceDeployment const& service_deployment,
                                    someip_protocol::internal::InstanceId const instance_id,
                                    std::shared_ptr<RemoteServerType> remote_server) noexcept {
    // If there are eventgroups configured in the required service instance then inject a client event dispatcher
    if (!service_deployment.communication_deployment.event_groups.empty()) {
      configuration::types::SomeIpServiceInstanceId const service_instance_id{
          {service_deployment.deployment_id.service_interface_id,
           {service_deployment.deployment_id.service_interface_version.major_version,
            service_deployment.deployment_id.service_interface_version.minor_version}},
          instance_id};
      remote_server->RegisterClientEventDispatcher(
          memory::MemoryUtilities::CreateUniquePtr<amsr::someip_daemon_core::client::ClientEventDispatcher>(
              service_deployment.communication_deployment.events, service_instance_id));
    }
  }

  /*!
   * \brief  Initialize static service discovery for a remote server instance
   *
   * \param[in] remote_server  The remote server
   *
   * \return      The remote server.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Extract the IP addresses from the Static SD configuration (unicast and multicast).
   * - Initialize static SD on the given remote server instance.
   * \endinternal
   */
  void InitializeStaticSD(std::shared_ptr<RemoteServerType> remote_server) const noexcept {
    assert(machine_mapping_.provider_.has_value());

    ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> tcp_endpoint{};
    ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> udp_endpoint{};
    ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress>
        event_multicast_endpoint{};

    if ((machine_mapping_.provider_->address_.has_value()) && (machine_mapping_.provider_->tcp_port_.has_value())) {
      someip_daemon_core::IpAddress const referenced_address{machine_mapping_.provider_->address_.value()};
      configuration::types::Port const tcp_port{machine_mapping_.provider_->tcp_port_.value()};
      amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress const tcp_endpoint_value{
          referenced_address, tcp_port};
      tcp_endpoint.emplace(tcp_endpoint_value);
    }
    if ((machine_mapping_.provider_->address_.has_value()) && (machine_mapping_.provider_->udp_port_.has_value())) {
      someip_daemon_core::IpAddress const referenced_address{machine_mapping_.provider_->address_.value()};
      configuration::types::Port const udp_port{machine_mapping_.provider_->udp_port_.value()};
      amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress const udp_endpoint_value{
          referenced_address, udp_port};
      udp_endpoint.emplace(udp_endpoint_value);
    }

    if (machine_mapping_.provider_->event_multicast_address_.has_value() &&
        machine_mapping_.provider_->event_multicast_port_.has_value()) {
      ara::core::Result<someip_daemon_core::IpAddress> const event_multicast_address{
          machine_mapping_.provider_->event_multicast_address_.value()};
      configuration::types::Port const event_multicast_port{machine_mapping_.provider_->event_multicast_port_.value()};
      amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress const event_multicast_value{
          event_multicast_address.Value(), event_multicast_port};
      event_multicast_endpoint.emplace(event_multicast_value);
    }

    amsr::someip_daemon_core::configuration::types::ServiceAddress const remote_server_address{tcp_endpoint,
                                                                                               udp_endpoint};

    remote_server->InitializeStaticSD(remote_server_address, event_multicast_endpoint);
  }

  /*!
   * \brief The Packet Router.
   */
  std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router_;

  /*!
   * \brief The Connection Manager.
   */
  ConnectionManager& connection_manager_;

  /*!
   * \brief The Timer Manager.
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief The Reactor.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief The service discovery client state machine.
   * \note  This pointer may be null for communication only remote servers.
   */
  service_discovery::state_machine::client::ServiceDiscoveryClientInterface* service_discovery_client_;

  /*!
   * \brief The network endpoint
   */
  configuration::types::RequiredNetworkEndpoint const network_endpoint_;

  /*!
   * \brief The required eventgroups
   */
  RequiredEventgroupsMap const required_eventgroups_;

  /*!
   * \brief The required PDUs
   */
  configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const required_pdus_;

  /*!
   * \brief The machine mapping
   */
  configuration::ConfigurationTypesAndDefs::MachineMapping const machine_mapping_;

  /*!
   * \brief Boolean indicating whether this RSI is configured without SD
   */
  bool const is_communication_only_;

  /*!
   * \brief A pointer to the SD endpoint
   * \note  This pointer may be null for communication only remote servers
   */
  service_discovery::message::scheduler::SchedulerInterface* sd_endpoint_;

  /*!
   * \brief The method props
   */
  MethodsMessageAccumulationTimeoutMap const method_props_;

  /*!
   * \brief The custom subscription endpoints
   */
  configuration::types::ServiceAddress const custom_subscription_endpoints_;

  /*!
   * \brief The secure communication configuration
   */
  configuration::ConfigurationTypesAndDefs::SecComConfig const sec_com_config_;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_FACTORY_H_

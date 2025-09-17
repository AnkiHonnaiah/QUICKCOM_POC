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
/**        \file   server_initializer.h
 *         \brief  Server Initializer.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_SERVER_INITIALIZER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_SERVER_INITIALIZER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <sstream>
#include <utility>

#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/util/configuration_types_builder.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/packet_router/application_packet_router_interface.h"
#include "amsr/someip_daemon_core/server/local_server_impl.h"
#include "amsr/someip_daemon_core/server/local_server_manager.h"
#include "amsr/someip_daemon_core/server/remote_client_integrator.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_endpoint.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/language/location.h"

namespace amsr {
namespace someip_daemon_core {
namespace init {

/*!
 * \brief ServerInitializer.
 *
 * \tparam ConnectionManagerType Type of ConnectionManager
 */
template <typename ConnectionManagerType>
class ServerInitializer final {
 protected:
  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;

  /*!
   * \brief Type alias for the service instance communication type.
   */
  using ServiceInstanceCommunicationType = configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType;

 public:
  /*!
   * \brief Default constructor.
   *
   * \pre -
   * \context Init
   * \steady  FALSE
   */
  ServerInitializer() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  ~ServerInitializer() noexcept = default;

  ServerInitializer(ServerInitializer const&) = delete;
  ServerInitializer(ServerInitializer&&) = delete;
  ServerInitializer& operator=(ServerInitializer const&) & = delete;
  ServerInitializer& operator=(ServerInitializer&&) & = delete;

  /*!
   * \brief       InitializeProvidedServices.
   * \param[in]   configuration                A configuration.
   * \param[in]   service_discovery            The Service Discovery.
   * \param[in]   timer_manager                The Timer Manager.
   * \param[in]   reactor                      Reactor, used to trigger SW events for connection cleaup.
   * \param[in]   connection_manager           The connection manager.
   * \param[in]   local_server_manager         A reference to the local server manager.
   * \param[in]   packet_router                The packet router.
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Check that an IPC channel exists in the configuration.
   *   - For each configured provided service instance in the IPC channel:
   *     - Call InitializeProvidedService
   * \endinternal
   */
  static void InitializeProvidedServices(
      configuration::Configuration const& configuration,
      service_discovery::ServiceDiscoveryInterface& service_discovery,
      amsr::steady_timer::TimerManagerInterface* timer_manager, osabstraction::io::reactor1::Reactor1Interface& reactor,
      ConnectionManager& connection_manager, server::LocalServerManager& local_server_manager,
      std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router) noexcept {
    someip_daemon_core::logging::AraComLogger const logger{
        someip_daemon_core::logging::kRemoteClientManagerLoggerContextId,
        someip_daemon_core::logging::kRemoteClientManagerLoggerContextDescription,
        ara::core::StringView{"ServerInitializer"}};

    // Iterate over the provided service instances available in the configuration.

    for (configuration::ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer::const_reference
             provided_service_instance_cfg : configuration.GetIpcChannel().GetProvidedServiceInstances()) {
      InitializeProvidedService(configuration, service_discovery, timer_manager, reactor, connection_manager,
                                local_server_manager, packet_router, logger, provided_service_instance_cfg);
    }
  }

  /*!
   * \brief       DeinitializeProvidedServices.
   * \param[in]   configuration                A configuration.
   * \param[in]   service_discovery            The Service Discovery.
   *
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Check that an IPC channel exists in the configuration.
   *   - For each configured provided service instance in the IPC channel:
   *     - Unregister the service discovery server from service discovery endpoint.
   * \endinternal
   */
  static void DeinitializeProvidedServices(configuration::Configuration const& configuration,
                                           service_discovery::ServiceDiscoveryInterface& service_discovery) noexcept {
    someip_daemon_core::logging::AraComLogger const logger{
        someip_daemon_core::logging::kRemoteClientManagerLoggerContextId,
        someip_daemon_core::logging::kRemoteClientManagerLoggerContextDescription,
        ara::core::StringView{"ServerInitializer"}};

    logger.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    // Iterate over the provided service instances available in the configuration.
    for (configuration::ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer::const_reference
             provided_service_instance_cfg : configuration.GetIpcChannel().GetProvidedServiceInstances()) {
      // Unregister the service discovery server
      UnregisterServersFromSdEndpoint(service_discovery, provided_service_instance_cfg);
    }
  }

 private:
  /*!
   * \brief       Perform initialization for a single provided service instance.
   * \param[in]   configuration                 A configuration.
   * \param[in]   service_discovery             The Service Discovery.
   * \param[in]   timer_manager                 The Timer Manager.
   * \param[in]   reactor                       Reactor, used to trigger SW events for connection cleaup.
   * \param[in]   connection_manager            The connection manager.
   * \param[in]   local_server_manager          A reference to the local server manager.
   * \param[in]   packet_router                 The packet router.
   * \param[in]   logger                        The logger.
   * \param[in]   provided_service_instance_cfg The provided service instance to initialize.
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Get communication type and build the service deployment.
   * - Based on the field event configuration build the Provided field event container.
   * - Create LocalServerImpl.
   * - Go through all the port mappings of a provided service instance.
   *  - Get service discovery endpoint.
   *  - If service discovery endpoint exists,
   *   - Create a new remote client responsible for dispatching of events to subscribers.
   *   - Register the remote client in local server.
   *  - Else, log an error about missing endpoint (possibly a configuration issue).
   * - Emplace the local server implementation in the local server manager.
   * - If emplace fails, log fatal and abort.
   * \endinternal
   */
  static void InitializeProvidedService(
      configuration::Configuration const& configuration,
      service_discovery::ServiceDiscoveryInterface& service_discovery,
      amsr::steady_timer::TimerManagerInterface* timer_manager, osabstraction::io::reactor1::Reactor1Interface& reactor,
      ConnectionManager& connection_manager, server::LocalServerManager& local_server_manager,
      std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
      someip_daemon_core::logging::AraComLogger const& logger,
      configuration::ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer::const_reference
          provided_service_instance_cfg) noexcept {
    // Get communication type
    ServiceInstanceCommunicationType const communication_type{configuration.GetServerServiceInstanceCommunicationType(
        provided_service_instance_cfg.service_id_, provided_service_instance_cfg.instance_id_,
        someip_protocol::internal::MajorVersion{}, someip_protocol::internal::MinorVersion{})};

    // Build the service deployment ID
    configuration::types::SomeIpServiceInstanceId const service_interface_id{
        configuration::types::SomeIpServiceInterfaceDeploymentId{
            provided_service_instance_cfg.service_id_,
            configuration::types::SomeIpServiceVersion{provided_service_instance_cfg.major_version_,
                                                       provided_service_instance_cfg.minor_version_}},
        provided_service_instance_cfg.instance_id_};

    // Build the Provided field event container.
    configuration::util::ConfigurationTypesBuilder::ProvidedFieldEvents const provided_field_event_container{
        configuration::util::ConfigurationTypesBuilder::BuildProvidedSomeIpFieldEvent(service_interface_id,
                                                                                      configuration)};

    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
    amsr::UniquePtr<server::LocalServerImpl> local_server_impl{
        memory::MemoryUtilities::CreateUniquePtr<server::LocalServerImpl>(
            service_interface_id, communication_type, packet_router, provided_field_event_container)};

    // Go through all machine mappings of a provided service instance
    for (configuration::ConfigurationTypesAndDefs::MachineMappingContainer::const_reference machine_mapping :
         provided_service_instance_cfg.port_mappings_) {
      assert(machine_mapping.address_.has_value());
      logger.LogDebug(
          [&provided_service_instance_cfg, &machine_mapping, &communication_type](ara::log::LogStream& s) {
            s << "Creating RemoteClient for ServiceId: 0x";
            s << ara::log::HexFormat(provided_service_instance_cfg.service_id_);
            s << ", instance id: 0x";
            s << ara::log::HexFormat(provided_service_instance_cfg.instance_id_);
            s << ", major version: 0x";
            s << ara::log::HexFormat(provided_service_instance_cfg.major_version_);
            s << ", minor version: 0x";
            s << ara::log::HexFormat(provided_service_instance_cfg.minor_version_);
            s << ", address: ";
            s << machine_mapping.address_.value().ToString();
            s << ". ";
            if (communication_type == ServiceInstanceCommunicationType::kCommunicationOnly) {
              s << "(Communication Only)";
            } else if (communication_type == ServiceInstanceCommunicationType::kSdOnly) {
              s << "(SD Only)";
            } else {
              s << "(SD & Communication)";
            }
          },
          static_cast<char const*>(__func__), __LINE__);

      // VECTOR NC AutosarC++17_10-A7.1.1: MD_SomeIpDaemon_AutosarC++17_10-A7.1.1_Immutable_shall_be_qualified_const
      service_discovery::ServiceDiscoveryEndpoint* sd_endpoint{
          service_discovery.GetSdEndpoint(machine_mapping.address_.value().ToString())};
      // To create the remote client in case we use SD we must have a SD endpoint for the service discovery server but
      // in case of SD is disabled its not required to have a SD endpoint.
      if ((sd_endpoint != nullptr) || (communication_type == ServiceInstanceCommunicationType::kCommunicationOnly)) {
        // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
        amsr::UniquePtr<amsr::someip_daemon_core::server::RemoteClientInterface> remote_client{
            server::RemoteClientIntegrator<ConnectionManagerType>::CreateRemoteClient(
                service_interface_id, machine_mapping, connection_manager, configuration, timer_manager, reactor,
                sd_endpoint, provided_service_instance_cfg, communication_type)};

        local_server_impl->RegisterRemoteClient(std::move(remote_client));
      } else {
        // Usually should not happen, indicates a problem in configuration.
        logger.LogError(
            [&provided_service_instance_cfg, &machine_mapping](ara::log::LogStream& s) {
              s << "Failed to create a remote client (ServiceId: 0x";
              s << ara::log::HexFormat(provided_service_instance_cfg.service_id_);
              s << ", MajorVersion: 0x";
              s << ara::log::HexFormat(provided_service_instance_cfg.major_version_);
              s << ", InstanceId: 0x";
              s << ara::log::HexFormat(provided_service_instance_cfg.instance_id_);
              s << ") because no service discovery endpoint with address ";
              s << machine_mapping.address_.value().ToString();
              s << " could be found and service discovery is configured.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }
    bool const emplace_result{
        local_server_manager.EmplaceLocalServer(service_interface_id, std::move(local_server_impl))};
    if (!emplace_result) {
      logger.LogFatalAndAbort(
          [&service_interface_id](ara::core::String& abort_msg) noexcept {
            std::ostringstream string_stream{};
            string_stream << "Failed to emplace a local server in the local server manager (ServiceId: 0x";
            string_stream << std::hex << service_interface_id.deployment_id.service_interface_id;
            string_stream << ", MajorVersion: 0x";
            string_stream << std::hex
                          << static_cast<std::uint16_t>(
                                 service_interface_id.deployment_id.service_interface_version.major_version);
            string_stream << ", InstanceId: 0x";
            string_stream << std::hex << service_interface_id.instance_id;
            string_stream << ")";
            abort_msg.append(string_stream.str());
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Unregister the server discovery server from service discovery endpoint.
   *
   * \param[in]   service_discovery             The Service Discovery
   * \param[in]   provided_service_instance_cfg The provided service instance to unregister
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Go through all the port mappings of a provided service instance.
   * - Get the service discovery endpoint and unregister the service discovery server.
   * \endinternal
   */
  static void UnregisterServersFromSdEndpoint(
      service_discovery::ServiceDiscoveryInterface& service_discovery,
      configuration::ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer::const_reference
          provided_service_instance_cfg) noexcept {
    // Go through all machine mappings of a provided service instance
    for (configuration::ConfigurationTypesAndDefs::MachineMappingContainer::const_reference machine_mapping :
         provided_service_instance_cfg.port_mappings_) {
      if ((machine_mapping.communication_type_ != ServiceInstanceCommunicationType::kCommunicationOnly) &&
          (machine_mapping.address_.has_value())) {
        service_discovery::ServiceDiscoveryEndpoint* const sd_endpoint{
            service_discovery.GetSdEndpoint(machine_mapping.address_.value().ToString())};
        if (sd_endpoint != nullptr) {
          // unregister the sd server from service discovery endpoint.
          typename service_discovery::ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const
              sd_endpoint_key{provided_service_instance_cfg.service_id_, provided_service_instance_cfg.instance_id_,
                              provided_service_instance_cfg.major_version_,
                              provided_service_instance_cfg.minor_version_};
          sd_endpoint->UnregisterServer(sd_endpoint_key);
        }
      }
    }
  }
};

}  // namespace init
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_SERVER_INITIALIZER_H_

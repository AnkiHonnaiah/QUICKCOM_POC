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
/*!        \file  configuration.h
 *        \brief  Global configuration module for one SOME/IP daemon.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CONFIGURATION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CONFIGURATION_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include <unordered_map>

#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/map.h"
#include "ara/core/memory_resource.h"
#include "ara/core/optional.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief Assembled configuration of all applications and the SOME/IP daemon.
 * \details This class represents the internal data model containing the necessary configuration
 * parameters for the SOME/IP daemon. This is explicitly separated from a certain
 * configuration intermediate representation such as JSON to allow testing without additional mocks, interfaces etc.
 */
class Configuration : public ConfigurationTypesAndDefs {
 public:
  /*!
   * \brief  A timeout list for UDP Method message accumulation purposes.
   */
  using MethodsMessageAccumulationTimeoutMap = std::unordered_map<
      someip_protocol::internal::MethodId, std::chrono::nanoseconds, std::hash<someip_protocol::internal::MethodId>,
      std::equal_to<someip_protocol::internal::MethodId>,
      ara::core::PolymorphicAllocator<std::pair<someip_protocol::internal::MethodId const, std::chrono::nanoseconds>>>;

  /*!
   * \brief  A timeout list for Events message accumulation purposes.
   */
  using EventsMessageAccumulationTimeoutMap = std::unordered_map<
      someip_protocol::internal::EventId, std::chrono::nanoseconds, std::hash<someip_protocol::internal::EventId>,
      std::equal_to<someip_protocol::internal::EventId>,
      ara::core::PolymorphicAllocator<std::pair<someip_protocol::internal::EventId const, std::chrono::nanoseconds>>>;

  // VECTOR NC AutosarC++17_10-A12.1.6: MD_SomeIpDaemon_AutosarC++17_10-A12.1.6_UseInheritingConstructor
  /*!
   * \brief Constructor of Configuration, calling baseclass constructor.
   *
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  Configuration() noexcept : ConfigurationTypesAndDefs() {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  virtual ~Configuration() noexcept;

  /*!
   * \brief Deleted copy constructor.
   */
  Configuration(Configuration const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  Configuration(Configuration&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(Configuration const&) & -> Configuration& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(Configuration&&) & -> Configuration& = delete;

  /*!
   * \brief Maximum number of callbacks that can be registered at the reactor
   */
  static constexpr std::uint16_t kNumberReactorCallbacks{1024U};

  /*!
   * \brief Check if configuration is valid.
   * \return true if configuration is valid, otherwise false is returned.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto IsValid() const noexcept -> bool;

  /*!
   * \brief Returns a container of all services (provided and required).
   *
   * \return A container of services read from the configuration.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto GetServices() const noexcept -> ServiceContainer const&;

  /*!
   * \brief Returns an immutable IPC channel configuration object.
   * \details Currently only a single IPC channel is supported, so this returns the first. Any valid configuration
   *          json should contain an IpcChannel object.
   *
   * \return A reference to the first IPC channel if an IPC channel is present.
   * \pre AddIpcChannels has been called with a valid IpcChannel.
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto GetIpcChannel() const noexcept -> IpcChannel const&;

  /*!
   * \brief Returns a container of all IPC channels.
   *
   * \return A container of IPC channels read from the configuration.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto GetIpcChannels() const noexcept -> IpcChannelsContainer const&;

  /*!
   * \brief Returns an immutable service configuration object.
   *
   * \param[in] deployment_id A SOME/IP deployment identifier.
   *
   * \return A pointer to a service if a service with this SOME/IP service ID is
   * part of the global configuration object; a nullptr otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto GetService(types::SomeIpServiceInterfaceDeploymentId deployment_id) const noexcept -> Service const*;

  /*!
   * \brief Searches for a method configuration object in the configuration
   * based on a given service ID and method ID.
   *
   * \param[in] service_id A SOME/IP service identifier.
   * \param[in] major_version A major version value.
   * \param[in] method_id A SOME/IP method identifier.
   *
   * \return A pointer to a method if a service with this service ID and major version is present and the method is
   * part of this service; a nullptr otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto GetMethod(someip_protocol::internal::ServiceId service_id, someip_protocol::internal::MajorVersion major_version,
                 someip_protocol::internal::MethodId method_id) const noexcept -> Method const*;

  /*!
   * \brief Returns an event.
   *
   * \param[in] service_id A SOME/IP service identifier.
   * \param[in] major_version A major version value.
   * \param[in] event_id A SOME/IP event identifier.
   *
   * \return A pointer to an event if a service with this service ID and major version is present
   * and the event is part of the service; a nullptr otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto GetEvent(someip_protocol::internal::ServiceId service_id, someip_protocol::internal::MajorVersion major_version,
                someip_protocol::internal::EventId event_id) const noexcept -> Event const*;

  /*!
   * \brief Returns the communication type configured for a service instance.
   * \details A service instance may support several communication types (one for
   *          each machine mapping), but as fan-out is not yet supported, this function
   *          returns the communication type of the first configured machine mapping.
   *          Major version and minor version parameters are unused until a service instance
   *          with different versions is supported.
   *          If the requested provided service does not exists in cofiguration, the daemon will abort.
   *
   * \param[in] service_id     A SOME/IP service identifier.
   * \param[in] instance_id    A SOME/IP instance identifier.
   * \param[in] major_version  The SOME/IP major version.
   * \param[in] minor_version  The SOME/IP minor version.
   *
   * \return The configured communication type for the given service instance.
   *
   * \pre        The provided service instance must exist in the configuration and IPC channel has been added.
   * \context    App
   * \reentrant  FALSE
   * \steady FALSE
   */
  auto GetServerServiceInstanceCommunicationType(someip_protocol::internal::ServiceId service_id,
                                                 someip_protocol::internal::InstanceId instance_id,
                                                 someip_protocol::internal::MajorVersion major_version,
                                                 someip_protocol::internal::MinorVersion minor_version) const noexcept
      -> ConfigurationTypesAndDefs::ServiceInstanceCommunicationType;

  /*!
   * \brief Checks if the given service id is found in this configuration.
   *
   * \param[in] service_id the service id to look for.
   *
   * \return true if the service is found, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto HasService(someip_protocol::internal::ServiceId service_id) const noexcept -> bool;

  /*!
   * \brief Checks if the given service id and major version pair is found in this configuration.
   *
   * \param[in] service_id the service id to look for.
   * \param[in] major_version the major version to look for.
   *
   * \return true if the pair is found, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto HasService(someip_protocol::internal::ServiceId service_id,
                  someip_protocol::internal::MajorVersion major_version) const noexcept -> bool;

  /*!
   * \brief Checks if the given method is found in this configuration.
   *
   * \param[in] service_id the service id of the method to look for.
   * \param[in] major_version A major version value.
   * \param[in] method_id the method id to look for.
   *
   * \return true if the method is found, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto HasMethod(someip_protocol::internal::ServiceId service_id, someip_protocol::internal::MajorVersion major_version,
                 someip_protocol::internal::MethodId method_id) const noexcept -> bool;

  /*!
   * \brief Checks if the given event is found in this configuration.
   *
   * \param[in] service_id the service id of the event to look for.
   * \param[in] major_version A major version value.
   * \param[in] event_id the event id to look for.
   *
   * \return true if the event is found, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto HasEvent(someip_protocol::internal::ServiceId service_id, someip_protocol::internal::MajorVersion major_version,
                someip_protocol::internal::EventId event_id) const noexcept -> bool;

  /*!
   * \brief Checks if, for the given required SOME/IP service instance, any event or method is configured
   *        to use TCP protocol.
   *
   * \tparam ServiceInstance Either a RequiredServiceInstance or ProvidedServiceInstance
   *
   * \param[in] instance_config The instance configuration
   *
   * \return true if a remote tcp endpoint is required, false otherwise
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Check if a method using TCP is required
   * - Check if an event using TCP is required
   * - Return true if either of those checks is true
   * \endinternal
   */
  template <typename ServiceInstance>
  auto IsTcpRequired(ServiceInstance const& instance_config) const noexcept -> bool {
    types::SomeIpServiceInterfaceDeploymentId const deployment_id{
        instance_config.service_id_, {instance_config.major_version_, instance_config.minor_version_}};
    // Check if any method requires TCP
    bool const tcp_method_required{HasMethodWithProto(deployment_id, Protocol::kTCP)};

    // Check if any event requires TCP
    bool const tcp_event_required{
        EventGroupContainerContainsTcpEvent(deployment_id, instance_config.service_discovery_.eventgroups_)};

    return tcp_method_required || tcp_event_required;
  }

  /*!
   * \brief Checks if, for the given required SOME/IP service instance, any event or method is configured
   *        to use UDP protocol.
   *
   * \tparam ServiceInstance Either a RequiredServiceInstance or ProvidedServiceInstance
   *
   * \param[in] instance_config The instance configuration
   *
   * \return true if a remote udp endpoint is required, false otherwise
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Check if a method using UDP is required
   * - Check if an event using UDP is required
   * - Return true if either of those checks is true
   * \endinternal
   */
  template <typename ServiceInstance>
  auto IsUdpRequired(ServiceInstance const& instance_config) const noexcept -> bool {
    types::SomeIpServiceInterfaceDeploymentId const deployment_id{
        instance_config.service_id_, {instance_config.major_version_, instance_config.minor_version_}};
    // Check if any method requires UDP
    bool const udp_method_required{HasMethodWithProto(deployment_id, Protocol::kUDP)};

    // Check if any event requires UDP
    bool const udp_event_required{
        EventGroupContainerContainsUdpEvent(deployment_id, instance_config.service_discovery_.eventgroups_)};

    return udp_method_required || udp_event_required;
  }

  /*!
   * \brief Returns an eventgroup from the configuration.
   *
   * \param[in] deployment_id A SOME/IP deployment identifier.
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return An pointer to an event group if a service with this service ID is present
   * and the event group ID is part of the service; a nullptr otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetEventgroup(types::SomeIpServiceInterfaceDeploymentId deployment_id,
                     someip_protocol::internal::EventId eventgroup_id) const noexcept -> Eventgroup const*;

  /*!
   * \brief Check if the given eventgroup contains any TCP event.
   *
   * \param[in] deployment_id A SOME/IP deployment identifier.
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return true if the given eventgroup contains at least one event with TCP protocol, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ContainsTcpEvent(types::SomeIpServiceInterfaceDeploymentId deployment_id,
                        someip_protocol::internal::EventgroupId eventgroup_id) const noexcept -> bool;

  /*!
   * \brief Check if the given eventgroup contains any UDP event.
   *
   * \param[in] deployment_id A SOME/IP deployment identifier.
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return true if the given eventgroup contains at least one event with UDP protocol, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ContainsUdpEvent(types::SomeIpServiceInterfaceDeploymentId const deployment_id,
                        someip_protocol::internal::EventgroupId const eventgroup_id) const noexcept -> bool;

  /*!
   * \brief Returns a required service instance SD eventgroup.
   *
   * \param[in] service_id A SOME/IP service identifier.
   * \param[in] instance_id A SOME/IP service instance identifier.
   * \param[in] major_version A major version of a SOME/IP service.
   * \param[in] minor_version A minor version of a SOME/IP service.
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \return A pointer to a required service instance SD eventgroup if a required service instance with the tuple
   * of given parameters (service ID, instance ID, major and minor version) is part of the global configuration object
   * and an eventgroup based on the given event group ID is part of this required service instance; otherwise a nullptr.
   * \pre One IPC channel has been added.
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetRequiredServiceInstanceSdEventgroup(someip_protocol::internal::ServiceId service_id,
                                              someip_protocol::internal::InstanceId instance_id,
                                              someip_protocol::internal::MajorVersion major_version,
                                              someip_protocol::internal::MinorVersion minor_version,
                                              someip_protocol::internal::EventId eventgroup_id) const noexcept
      -> RequiredServiceInstanceSdEventgroup const*;

  /*!
   * \brief Returns a container of all cyclic timers configurations.
   *
   * \return A container of cyclic timers configurations.
   * \steady FALSE
   */
  auto GetCyclicTimerConfig() const noexcept -> CyclicTimerConfigContainer const&;

  /*!
   * \brief      Returns a container of all repetition timers configurations.
   * \return     A container of cyclic timers configurations.
   * \pre        -
   * \context    Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \vprivate
   * \steady FALSE
   */
  auto GetRepetitionOfferTimerConfig() const noexcept -> RepetitionOfferTimerConfigContainer const&;

  /*!
   * \brief Returns a provided service instance SD eventgroup.
   *
   * \param[in] service_id A SOME/IP service identifier.
   * \param[in] instance_id A SOME/IP service instance identifier.
   * \param[in] major_version A major version of a SOME/IP service.
   * \param[in] minor_version A minor version of a SOME/IP service.
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \return A pointer to a provided service instance SD eventgroup if a provided service instance with the tuple
   * of given parameters (service ID, instance ID, major and minor version) is present and an eventgroup based on the
   * given event group ID is part of this provided service instance; otherwise a nullptr.
   * \pre One IPC channel has been added.
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetProvidedServiceInstanceSdEventgroup(someip_protocol::internal::ServiceId service_id,
                                              someip_protocol::internal::InstanceId instance_id,
                                              someip_protocol::internal::MajorVersion major_version,
                                              someip_protocol::internal::MinorVersion minor_version,
                                              someip_protocol::internal::EventId eventgroup_id) const noexcept
      -> ProvidedServiceInstanceSdEventgroup const*;

  /*!
   * \brief Returns a container of network endpoints.
   *
   * \return A container of network endpoints.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetNetworkEndpoints() const noexcept -> NetworkEndpointContainer const&;

  /*!
   * \brief Returns a network endpoint.
   *
   * \param[in] address An IP address.
   *
   * \return A pointer to a network endpoint from the global configuration if there is a network endpoint based on the
   * given IP address is available; a nullptr otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady TRUE
   */
  auto GetNetworkEndpoint(someip_daemon_core::IpAddress const& address) const noexcept -> NetworkEndpoint const*;

  /*!
   * \brief Returns the UUID for the trustzone / HSM crypto provider.
   *
   * \return A UUID identifying the trustzone / HSM crypto provider.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetTrustzoneCryptoProviderUuid() const noexcept -> CryptoProviderUuid const&;

  /*!
   * \brief Returns the machines PSK identity hint.
   *
   * \return A PSK identity hint.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetMachinePskIdentityHint() const noexcept -> PskIdentityHint const&;

  /*!
   * \brief Determines whether an SD endpoint with the specified IP address and port exists or not.
   *
   * \param[in] address A unicast IP address.
   * \param[in] port A port number.
   * \return true if the specified SD endpoint exists and false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto HasSdAddressAndPort(someip_daemon_core::IpAddress const& address,
                           someip_protocol::internal::Port port) const noexcept -> bool;

  /*!
   * \brief Returns all eventgroups of a SOME/IP service which contain the specified event.
   *
   * \param[in] service_id A SOME/IP service identifier.
   * \param[in] event_id An event identifier.
   * \return A container of eventgroups which contain the specified event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto EventToEventgroups(types::SomeIpServiceInterfaceDeploymentId deployment_id,
                          someip_protocol::internal::EventId event_id) const noexcept -> EventgroupIdContainer;

  /*!
   * \brief Add services of an application to the global configuration of the SOME/IP daemon.
   * \details This merging sequence will add services which are not part of the global configuration yet. If a service
   * of the application matches with a service already part of the global configuration, the service of the
   * application will not be added to the global configuration. If the services match, they shall be identical. If there
   * is an inconsistency, the merge process will be aborted.
   * \param[in] services A container of services to insert into the global configuration.
   * \return True if the services were added successfully. Otherwise false is returned.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto AddServices(ServiceContainer const& services) noexcept -> bool;

  /*!
   * \brief Add IPC channels of an application to the global configuration of the SOME/IP daemon.
   * \details This merging sequence will add IPC channels which are not part of the global configuration yet. If an IPC
   *          channel of the application matches with an IPC channel already part of the global configuration, the IPC
   *          channel required and provided service instances lists will be merged together with the lists of the
   *          already existing IPC channel. If there is an inconsistency, the merge process will be aborted.
   * \param[in] ipc_channels A container of IPC channels to insert into the global configuration.
   * \return True if the IPC channels were added successfully. Otherwise false is returned.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */
  auto AddIpcChannels(IpcChannelsContainer const& ipc_channels) noexcept -> bool;

  /*!
   * \brief Add network endpoints of an application to the global configuration of the SOME/IP daemon.
   * \details This method will add network endpoints to the list of network endpoints for the global configuration.
   *          Network configuration related to ports, is propagated to the ports before any configuration is added.
   *          If the network endpoint already exists in the list a check will be made to see if the network endpoints
   *          can be merged. If there is an inconsistency, the merge process will be stopped and false is returned.
   * \param[in] network_endpoints The network endpoints to be added to the global configuration.
   * \return true if the network endpoints were added successfully. Otherwise false is returned.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto AddNetworkEndpoints(NetworkEndpointContainer const& network_endpoints) noexcept -> bool;

  /*!
   * \brief Add SecureConnections to the global SecureConnections configuration. The SecureIds are updated to global
   * unique sequence IDs.
   * \details The SecureConnections are validated afterwards in ValidateConfigurationConsistency().
   * \param[in] parsed_secure_connections  SecureConnections to be added to the global configuration.
   * \param[in] parsed_ipc_channels Non-empty container of IPC Channels parsed from one JSON
   * \param[in] parsed_network_endpoints Network Endpoints parsed from one JSON
   * \return true, if all SecureIds configured in all rsi and all nep are linked to a SecureConnection, false otherwise
   * \context     Init
   * \pre         -
   * \reentrant   FALSE
   * \threadsafe FALSE
   * \steady FALSE
   */
  auto AddSecureConnections(SecureConnectionContainer& parsed_secure_connections,
                            IpcChannelsContainer& parsed_ipc_channels,
                            NetworkEndpointContainer& parsed_network_endpoints) noexcept -> bool;

  /*!
   * \brief Validate the consistency of secure connections configured in all required service instances and network
   * endpoint ports
   * \details The validation is performed between all SecureConnections configured for one required service instance or
   * network endpoint port.
   * \return true, if all consistency checks succeed, false otherwise
   * \pre         One IPC channel has been added.
   * \context     ANY
   * \reentrant   FALSE
   * \steady FALSE
   */
  auto ValidateSecureConnectionsConsistency() const noexcept -> bool;

  /*!
   * \brief Check if a mismatch is found between what is already requested for the network endpoint and what is
   * supplied in the machine mapping.
   * \details If Either of these are true a match is found:
   *    1. Machine mapping is configured for "service discovery only" and the network endpoint has communication
   *    requested.
   *    2. Machine mapping is configured for "service discovery and communication" and the network endpoint is
   *    configured with service discovery only.
   * \param[in] port_it The port container to check.
   * \param[in] machine_mapping The machine mapping to check.
   * \return true if a mismatch was found, false otherwise.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto CheckCommunicationTypeMismatch(NetworkEndpointPort const& port_it,
                                             MachineMapping const& machine_mapping) noexcept -> bool;

  /*!
   * \brief Update the requested communication type of the network endpoint port according to the machine mapping.
   * \details If a mismatch is found (see CheckCommunicationTypeMismatch()) communication is requested.
   * \param[in,out] port_it The port container to update.
   * \param[in] machine_mapping The machine mapping to use.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  void UpdateRequestedCommunicationType(NetworkEndpointPort& port_it,
                                        MachineMapping const& machine_mapping) const noexcept;

  /*!
   * \brief Sets the requested communication of the endpoints referenced in the given machine mapping.
   * \details If the endpoints referenced in the machine mapping are present in the network endpoint container,
   *          they will be marked as needed for communication or service discovery only.
   *          If they are not present, the daemon will abort.
   *
   * \param[in] machine_mapping The machine mapping to request network endpoints for communication
   *
   * \pre       The network endpoint container has already been filled with all configured network endpoints
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  void RequestNetworkEndpoint(MachineMapping const& machine_mapping) noexcept;

  /*!
   * \brief Stores the machines PSK identity hint into the configuration object if not yet present.
   * \details This PSK identity hint is provided by the server during TLS handshake if a PSK based cipher suites is
   * negotiated. It hints the client side at which PSK to select for the connection. This hint is specified in the ARXML
   * machine design.
   * \param[in] psk_identity_hint The machines PSK identity hint.
   * \return True if stored successfully, otherwise false.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto AddMachinePskIdentityHint(PskIdentityHint const& psk_identity_hint) noexcept -> bool;

  /*!
   * \brief Checks if a port mapping objects matches with an endpoint and endpoint port
   * \param[in] port_mapping Network Endpoint Port Mapping
   * \param[in] endpoint Network Endpoint
   * \param[in] endpoint_port Network Endpoint Port
   * \return True when match, otherwise false
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto CheckUsesEndpoint(MachineMapping const& port_mapping, NetworkEndpoint const& endpoint,
                                NetworkEndpointPort const& endpoint_port) noexcept -> bool;

  /*!
   * \brief Checks if any port mapping objects within a port mapping container matches with an endpoint and endpoint
   * port
   * \param[in] port_mapping_container Network Endpoint Port Mapping Container
   * \param[in] endpoint Network Endpoint
   * \param[in] endpoint_port Network Endpoint Port
   * \return True when match, otherwise false
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto CheckUsesEndpoint(MachineMappingContainer const& port_mapping_container, NetworkEndpoint const& endpoint,
                                NetworkEndpointPort const& endpoint_port) noexcept -> bool;

  /*!
   * \brief Update the socket configurations on TCP ports using the network/machine settings.
   * \param[in] ne Network Endpoint
   * \param[in] ne_port Network Endpoint Port
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static void UpdateNetworkEndpointTcpPortsSocketOptions(NetworkEndpoint const& network_endpoint,
                                                         NetworkEndpointPort& ne_port) noexcept;

  /*!
   * \brief Update the socket configurations on UDP ports using the network/machine settings.
   * \param[in] ne Network Endpoint
   * \param[in] ne_port Network Endpoint Port
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static void UpdateNetworkEndpointUdpPortsSocketOptions(NetworkEndpoint const& network_endpoint,
                                                         NetworkEndpointPort& ne_port) noexcept;

  /*!
   * \brief Validate the consistency of the configuration
   * \return True if the configuration is consistent, otherwise false.
   * \pre One IPC channel has been added.
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateConfigurationConsistency() const noexcept -> bool;

  /*!
   * \brief Validate that an event is only allowed to be referenced by multiple eventgroups if all the referencing
   * eventgroups use unicast communication (MulticastThreshold = 0).
   * \return true if the MulticastThreshold of all eventgroups is 0, false otherwise.
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateEventGroupMulticastThreshold() const noexcept -> bool;

  /*!
   * \brief Validate that the TCP counter thresholds are configured correctly: when the feature is enabled, it is not
   * supported to have static service discovery and an infinite TTL.
   * \param[in] ne Network Endpoint
   * \param[in] ne_port Network Endpoint Port
   * \return true if configuration is correct, false otherwise.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateTcpCounterThresholds(NetworkEndpoint const& network_endpoint,
                                    NetworkEndpointPort const& ne_port) const noexcept -> bool;

  /*!
   * \brief Validate that the UDP bundling thresholds are consistent and configured correctly.
   * \return true if configuration is correct, false otherwise.
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateUdpBundlingThreshold() const noexcept -> bool;

  /*!
   * \brief Validate the TCP counter thresholds for a ProvidedServiceInstance.
   * \param[in] ne Network Endpoint
   * \param[in] ne_port Network Endpoint Port
   * \return true if configuration is correct, false otherwise.
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateTcpCounterThresholdsPsi(NetworkEndpoint const& network_endpoint,
                                       NetworkEndpointPort const& ne_port) const noexcept -> bool;

  /*!
   * \brief Validate the TCP counter thresholds for a RequiredServiceInstance.
   * \param[in] ne Network Endpoint
   * \param[in] ne_port Network Endpoint Port
   * \return true if configuration is correct, false otherwise.
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateTcpCounterThresholdsRsi(NetworkEndpoint const& network_endpoint,
                                       NetworkEndpointPort const& ne_port) const noexcept -> bool;

  /*!
   * \brief Validate that there is an event group for each service discovery eventgroup in the service.
   * \return true if validation condition succeeds.
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateEventGroupId() const noexcept -> bool;

  /*!
   * \brief Check if two method/event/event groups collections for one service interface differ
   * from each other.
   * \details This method allows re-ordering of elements within its container.
   * \tparam ContainerT Deduced type of both containers to compare.
   *
   * \param[in] container1 The first container of methods / events
   * \param[in] container2 The second container of methods to compare to the first one.
   * \return true if both method containers are equal; returns false if:
   * - The size of the method/event collections differ.
   * - The protocols for the same method/event ID differ.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  template <typename ContainerT>
  static auto CheckEquality(ContainerT const& container1, ContainerT const& container2) noexcept -> bool;

  /*!
   * \brief Check if any element of a collection exists in the other collection.
   *
   * \tparam ContainerT Deduced type of both containers to compare.
   * \param[in] container1 The first container of elements.
   * \param[in] container2 The second container of elements to compare to the first one.
   * \return true if both containers contain the same element; returns false if:
   *   - One of the collections is empty.
   *   - No element of one container exists in the second container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */
  template <typename ContainerT>
  static auto CheckOverlapping(ContainerT const& container1, ContainerT const& container2) noexcept -> bool;

  /*!
   * \brief Check if at least one machine mapping is found in both containers taking into account all fields except
   * communication type.
   * \param[in] machine_mappings1 The first container with machine mappings .
   * \param[in] machine_mappings2 The second container with machine mappings.
   * \return true if at least one machine mapping is found in both containers according to the criteria above.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto CheckMachineMappingsOverlap(MachineMappingContainer const& machine_mappings1,
                                          MachineMappingContainer const& machine_mappings2) noexcept -> bool;

  /*!
   * \brief Adds cyclic timer configuration if they are not already in the configuration.
   * \param[in] provided_service_instance ProvidedServiceInstance reference that contains the cyclic delay and
   * associated endpoint address to add to the cyclic timer configuration container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void AddCyclicTimerConfiguration(IpcChannel::ProvidedServiceInstance const& provided_service_instance) noexcept;

  /*!
   * \brief      Adds repetition offer timer configuration if they are not already in the configuration.
   * \param[in]  provided_service_instance ProvidedServiceInstance reference that contains the initial
   *             repetitions base delay and associated endpoint address to add to the repetition offer
   *             timer configuration container.
   * \pre        -
   * \context    Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \vprivate
   * \steady FALSE
   */
  void AddRepetitionOfferTimerConfiguration(
      IpcChannel::ProvidedServiceInstance const& provided_service_instance) noexcept;

  /*!
   * \brief Checks whether the overload protection is enabled for an event.
   * \param[in] event_ptr Pointer to the event configuration
   * \return true if the overload protection is enabled for the given event
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto IsOverloadProtectionEnabled(Event const* event_ptr) noexcept -> bool;

  /*!
   * \brief Checks whether the event is signal based (PDU) or not.
   * \param[in] service_id ServiceID
   * \param[in] event_id EventID
   * \return true if the event is signal based, false otherwise.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto IsSignalBasedEvent(someip_protocol::internal::ServiceId service_id,
                          someip_protocol::internal::EventId event_id) const noexcept -> bool;

  /*!
   * \brief Assigns the passed value to flag.
   * \pre This method must only be invoked with consistent values, meaning always with "true" or always with "false"
   * depending on the initial value used. This is to ensure consistency between merging application configurations.
   * \param[in] value The value for the flag.
   * \steady FALSE
   */
  void SetTlsCryptoModeRemoteFlag(bool value) noexcept;

  /*!
   * \brief Get the value of the TLS crypto mode remote flag.
   * \return The flag's value.
   * \steady FALSE
   */
  auto GetTlsCryptoModeRemoteFlag() const noexcept -> bool;

  /*!
   * \brief Assigns the passed value to merge RSI with different minor version flag.
   * \pre   This method must only be invoked with consistent values, meaning always with "true" or always with "false"
   *        depending on the initial value used. This is to ensure consistency between merging application
   *        configurations.
   * \param[in] value The value for the flag.
   * \steady FALSE
   */
  void SetMergeRSIWithDifferentMinorVersionFlag(bool value) noexcept;

  /*!
   * \brief Get the value of the merge RSI with different minor version flag.
   * \return The flag's value.
   * \steady FALSE
   */
  auto GetMergeRSIWithDifferentMinorVersionFlag() const noexcept -> bool;

  /*!
   * \brief Validate that service_discovery/eventgroups of StaticSD is present in port_mappings/subscribers/eventgroups.
   * \param[in] psi ProvidedServiceInstance.
   * \return true if validation condition succeeds.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateEventGroupId(IpcChannel::ProvidedServiceInstance const& psi) const noexcept -> bool;

  /*!
   * \brief Display warning message if service_discovery/eventgroups of StaticSD is not present in
   * port_mappings/subscribers/eventgroups.
   * \param[in] machine_mapping MachineMapping.
   * \param[in] event_group_id_list List of eventgroupids.
   * \return true if validation condition succeeds.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto CheckIfStaticSdReferencingValidEventgroupIds(MachineMapping const& machine_mapping,
                                                    EventgroupIdContainer event_group_id_list) const noexcept -> bool;

  /*!
   * \brief Method to validate the MulticastThreshold of eventgroup is zero and if
   *        it is not zero generate warning message.
   * \param[in] event_map_it EventMap iterator.
   * \return true if validate condition succeeds.

   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateEventMapping(
      ara::core::Map<someip_protocol::internal::EventId, ProvidedServiceInstanceSdEventgroupContainer>::iterator
          event_map_it) const noexcept -> bool;

  /*!
   * \brief Get the UDP message accumulation timeouts for Events
   * \param[in] sid Service ID
   * \param[in] iid Instance ID
   * \return timeout values
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetEventAccumulationTimeouts(someip_protocol::internal::ServiceId sid,
                                    someip_protocol::internal::InstanceId iid) const noexcept
      -> EventsMessageAccumulationTimeoutMap;

  /*!
   * \brief Get the UDP message accumulation timeouts for Method Requests.
   * \param[in] sid Service ID.
   * \param[in] iid Instance ID.
   * \param[in] major_version Major version.
   * \param[in] minor_version Minor version.
   * \return Timeout values.
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetMethodRequestAccumulationTimeouts(someip_protocol::internal::ServiceId sid,
                                            someip_protocol::internal::InstanceId iid,
                                            someip_protocol::internal::MajorVersion major_version,
                                            someip_protocol::internal::MinorVersion minor_version) const noexcept
      -> MethodsMessageAccumulationTimeoutMap;

  /*!
   * \brief Validate that IP addresses of all machine mappings in all provided service instances
   *        belong to different/isolated networks.
   * \param[in] provided_service_instances The container of provided service instances.
   * \return "true" if all machine mappings map to different networks and "false" otherwise.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateNetworksIsolation(
      IpcChannel::ProvidedServiceInstanceContainer const& provided_service_instances) const noexcept -> bool;

  /*!
   * \brief Validate the consistency of the Network Endpoints in respect it is not reused and the Tcp counter feature is
   * configured correctly.
   * \details This check validates that no TCP endpoint is reused by a required and a provided service instance and
   * additionally validates the if the Tcp counter feature is correctly configured.
   * \return true if the configuration is consistent, otherwise false.
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateNetworkEndpoints() const noexcept -> bool;

  /*!
   * \brief Check if the IP address of the first machine mapping belongs
   *        to the network of the second machine mapping or not.
   * \param[in] machine_mapping_1 First machine mapping.
   * \param[in] machine_mapping_2 Second machine mapping.
   * \return "true" if no overlap is detected and "false" otherwise.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto CheckNetworksIsolationInPortMappings(MachineMapping const& machine_mapping_1,
                                            MachineMapping const& machine_mapping_2) const noexcept -> bool;

  /*!
   * \brief Method to propagate the UDP message accumulation threshold to the network endpoints ports.
   * \pre This method depends on data from all the Provided/RequiredServiceInstances and NetworkEndpoints. As
   * "AddNetworkEndpoints" and "AddIpcChannels" might do validation and modification on all the contents of the raw
   * data, this method has to be called once both Addx methods named above have been called.
   * \pre One IPC channel has been added.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  void FillUdpAccumulationThresholdOnNetworkEndpoints() noexcept;

  /*!
   * \brief Getter for the SecCom configuration.
   * \returns A constant reference to the SecCom configuration.
   * \context ANY
   * \reentrant TRUE
   * \pre -
   * \threadsafe TRUE
   * \vprivate Component private.
   * \steady TRUE
   */
  auto GetSecComConfig() const noexcept -> SecComConfig const&;

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief An instance of a logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kConfigurationLoggerContextId,
                                                    someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                                                    ara::core::StringView{"Configuration"}};

  /*!
   * \brief Flag indicating validity of the configuration.
   */
  bool is_valid_{true};

  /*!
   * \brief Network endpoints collected from the application configurations.
   */
  NetworkEndpointContainer network_endpoints_{};

  /*!
   * \brief Collection of all required AND provided services read from the configuration.
   */
  ServiceContainer services_{};

  /*!
   * \brief Collection of all IPC channels read from the configuration.
   */
  IpcChannelsContainer ipc_channels_{};

  /*!
   * \brief Crypto provider UUID for the trustzone / HSM crypto provider.
   */
  CryptoProviderUuid trustzone_crypto_provider_uuid_{};

  /*!
   * \brief The local machines PSK identity hint.
   */
  PskIdentityHint machine_psk_identity_hint_{};

  /*!
   * \brief Flag to enable remote only crypto operations with TLS.
   * \details True enables this mode, false uses performance mode. By default false.
   */
  ara::core::Optional<bool> tls_crypto_mode_remote_{ara::core::nullopt};

  /*!
   * \brief Flag to indicate merge different required service instances that differ in minor version together or not.
   * \details True merges the two required service instances, false otherwise.
   */
  bool merge_rsi_with_different_minor_versions_{};

  /*!
   * \brief The SecCom configuration.
   */
  SecComConfig sec_com_config_{};

  /*!
   * \brief Container of cyclic timer configurations.
   */
  CyclicTimerConfigContainer cyclic_timers_{};

  /*!
   * \brief Container of repetition offer timer configurations.
   */
  RepetitionOfferTimerConfigContainer repetition_offer_timers_{};

  // VECTOR Enable VectorC++-V11-0-2
 private:
  /*!
   * \brief Enum to store the required service instance validation results.
   */
  enum class RsiValidationResult : std::uint8_t { kInvalid, kValidNew, kValidDuplicate, kValidMergeable };
  /*!
   * \brief Validate the required service instance with the existing RSIs in the configuration in the global
   *        configuration.
   * \details Validates the RSI both for internal consistency and against existing RSIs in the configuration.
   * \param[in] instance_to_add The required service instance to be validated with the existing RSIs.
   * \return An enum containing a value indicating whether the requested service instance is a duplicate, invalid,
   *         new, or a merge with an existing instance is required.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateRequiredServiceInstance(IpcChannel::RequiredServiceInstance const& instance_to_add) noexcept
      -> RsiValidationResult;

  /*!
   * \brief Validate the provided service instance with the existing PSIs in the global configuration.
   * \details Validates the PSI both for internal consistency and against existing PSIs in the configuration.
   * \param[in] provided_service_instance  The provided service instance to be added to the ipc channel.
   * \return true if the provided service instance is consistent. Otherwise false is returned.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \trace SPEC-4981412
   */
  auto ValidateProvidedServiceInstance(IpcChannel::ProvidedServiceInstance const& provided_service_instance) noexcept
      -> bool;

  /*!
   * \brief Validates the provided service provided_service_instance with respects to events and fields.
   * \details Checks that the PSI does not have eventgroups unless events and fields are also configured.
   *          Checks that communication is not disabled if the PSI provides events or field notifications.
   * \param provided_service_instance
   * \return true if no configuration errors related to events and fields were found.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateProvidedServiceInstanceEventsAndFields(
      IpcChannel::ProvidedServiceInstance const& provided_service_instance) const noexcept -> bool;

  /*!
   * \brief Update each port with port releated cofiguration from network.
   * \param[in,out] network_endpoints The network endpoints that will hold the updated ports configurations.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static void PropagateNetworkInfoToPorts(NetworkEndpointContainer& network_endpoints) noexcept;

  /*!
   * \brief Check if any eventgroup from the given eventgroup container contains any UDP event.
   *
   * \tparam EventgroupContainer Either a RequiredServiceInstanceSdEventgroupContainer or
   * ProvidedServiceInstanceSdEventgroupContainer
   *
   * \param[in] deployment_id         A SOME/IP deployment identifier.
   * \param[in] eventgroup_container  A SOME/IP eventgroup container.
   *
   * \return true if the given eventgroup container contains at least one event with UDP protocol, false otherwise.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - For each eventgroup within the eventgroup container
   *   - Check if the eventgroup contains a UDP event. Break loop if true.
   * \endinternal
   */
  template <typename EventGroupContainer>
  auto EventGroupContainerContainsUdpEvent(types::SomeIpServiceInterfaceDeploymentId const deployment_id,
                                           EventGroupContainer const& eventgroup_container) const noexcept -> bool {
    // PTP-B-Configuration_ContainsUdpEvent
    bool result{false};

    typename EventGroupContainer::const_iterator const eventgroup_it_begin{eventgroup_container.begin()};
    typename EventGroupContainer::const_iterator const eventgroup_it_end{eventgroup_container.end()};

    for (typename EventGroupContainer::const_iterator eventgroup_it{eventgroup_it_begin};
         eventgroup_it != eventgroup_it_end; eventgroup_it++) {
      result = ContainsUdpEvent(deployment_id, eventgroup_it->id_);
      if (result) {
        break;
      }
    }

    // PTP-E-Configuration_ContainsTcpEvent
    return result;
  }

  /*!
   * \brief Check if any eventgroup from the given eventgroup container contains any TCP event.
   *
   * \tparam EventgroupContainer Either a RequiredServiceInstanceSdEventgroupContainer or
   * ProvidedServiceInstanceSdEventgroupContainer
   *
   * \param[in] deployment_id         A SOME/IP deployment identifier.
   * \param[in] eventgroup_container  A SOME/IP eventgroup container.
   *
   * \return true if the given eventgroup container contains at least one event with TCP protocol, false otherwise.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - For each eventgroup within the eventgroup container
   *   - Check if the eventgroup contains a TCP event. Break loop if true.
   * \endinternal
   */
  template <typename EventGroupContainer>
  auto EventGroupContainerContainsTcpEvent(types::SomeIpServiceInterfaceDeploymentId const deployment_id,
                                           EventGroupContainer const& eventgroup_container) const noexcept -> bool {
    // PTP-B-Configuration_ContainsTcpEvent
    bool result{false};

    typename EventGroupContainer::const_iterator const eventgroup_it_begin{eventgroup_container.begin()};
    typename EventGroupContainer::const_iterator const eventgroup_it_end{eventgroup_container.end()};

    for (typename EventGroupContainer::const_iterator eventgroup_it{eventgroup_it_begin};
         eventgroup_it != eventgroup_it_end; eventgroup_it++) {
      result = ContainsTcpEvent(deployment_id, eventgroup_it->id_);
      if (result) {
        break;
      }
    }

    // PTP-E-Configuration_ContainsTcpEvent
    return result;
  }

  /*!
   * \brief Checks whether a given service is configured with any method using the given protocol
   * \param[in] deployment_id The deployment id
   * \param[in] proto         The protocol
   *
   * \return true if any method is configured with the given protocol
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady FALSE
   */
  auto HasMethodWithProto(types::SomeIpServiceInterfaceDeploymentId deployment_id, Protocol proto) const noexcept
      -> bool;

  /*!
   * \brief Checks if the two instances are consistent.
   * \param[in] lhs Left hand side instance
   * \param[in] rhs Right hand side instance
   * \return True if they are consistent, otherwise false.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto RequiredServiceInstanceConsistencyChecks(IpcChannel::RequiredServiceInstance const& lhs,
                                                IpcChannel::RequiredServiceInstance const& rhs) const noexcept -> bool;

  /*!
   * \brief Merge two RSI which differ in minor version.
   * \details Merge the service discovery eventgroups of current_instance into existing_instance and set required
   *          minor version of existing_instance to ANY.
   * \param[in] instance_to_add The required service instance to add to the configuration.
   * \param[in, out] existing_instance The instance which already exists in the configuration.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  void MergeRequiredServiceInstances(IpcChannel::RequiredServiceInstance const& instance_to_add,
                                     IpcChannel::RequiredServiceInstance& existing_instance) const noexcept;

  /*!
   * \brief Check two RSI which differ in minor version is mergeable or not.
   * \details Checks the service discovery eventgroups of current_instance with existing_instance.
   *          It is considered an error if eventgroups with the same ID exists in both instances but have different
   *          configurations. If the service discovery eventgroups of instances is not duplicate, set merge_needed flag
   *          as true.
   * \param[in] instance_to_add The required service instance to add to the configuration.
   * \param[in] existing_instance The instance which already exists in the configuration.
   * \param[out] merge_needed flag to indicate that two RSI needs to be merged or not.
   * \return true is no errors were detected, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto CheckMergeRequiredServiceInstances(IpcChannel::RequiredServiceInstance const& instance_to_add,
                                          IpcChannel::RequiredServiceInstance const& existing_instance,
                                          bool& merge_needed) const noexcept -> bool;

  /*!
   * \brief Validate the dtls cookie verification enabled in secure connections protocol consistency.
   * \details This check validates that the dtls cookie verification enabled flag is only configured for secure
   * connections used in UDP ports (server) or port mappings (client) and must have a value. The flag is not allowed
   * on TCP ports or port mappings.
   * \return true if the protocol configuration is consistent, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto ValidateDtlsCookieVerificationEnabledProtocolConsistency(SecureConnection const& secure_connection,
                                                                Protocol const& protocol) const -> bool;

  /*!
   * \brief Validate the dtls cookie verification enabled in secure connections
   * \details This check validates that the dtls cookie verification enabled flag has the same value
   * for every secure connection.
   * \return true if the configuration is consistent, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateDtlsCookieVerificationEnabledSecureConnection() const -> bool;

  /*!
   * \brief Validate the dtls cookie verification enabled in secure endpoints
   * \details This check validates that the dtls cookie verification enabled flag has the same value
   * for every endpoint.
   * \return true if the configuration is consistent, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateDtlsCookieVerificationEnabled() const -> bool;

  /*!
   * \brief Validate the consistency of the RequiredServiceInstances
   * \details This check validates that a RSI with no configured ports must not
   *          have any methods configured in its service interface
   * \return true if the configuration is consistent, otherwise false.
   * \pre One IPC channel has been added.
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateRequiredServiceInstances() const noexcept -> bool;

  /*!
   * \brief Checks if a Required Service Instance has either TCP or UDP ports configured
   * \param[in] rsi Required Service Instance
   * \return true if there are either TCP no UDP port configured for the required service instance
   *              machine mapping
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto HasPortsConfigured(IpcChannel::RequiredServiceInstance const& rsi) noexcept -> bool;

  /*!
   * \brief Ensure that the given required service instance complies with
   *        multicast-only communication feature
   * \param[in] rsi Required Service Instance
   * \details This check validates that a RSI with no configured ports must not
   *          have any methods configured in its service interface
   * \return true if the configuration is consistent, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateMulticastOnlyCommunication(IpcChannel::RequiredServiceInstance const& rsi) const noexcept -> bool;

  /*!
   * \brief Ensure consistency of required and configured transport protocols
   * \param[in] rsi Required Service Instance
   * \details This check validates that a RSI requiring a transport protocol has
   *          this transport protocol configured in its machine mapping
   * \return true if the configuration is consistent, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateRsiRequiredTransportProtocols(IpcChannel::RequiredServiceInstance const& rsi) const noexcept -> bool;

  /*!
   * \brief Validate the consistency of the ProvidedServiceInstances
   * \details This check validates that a PSI with no configured ports must not
   *          have any methods configured in its service interface
   * \return true if the configuration is consistent, otherwise false.
   * \pre One IPC channel has been added.
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateProvidedServiceInstances() const noexcept -> bool;

  /*!
   * \brief Validate if the RequiredServiceInstance has a unicast network endpoint when a field notifier is configured.
   * \details This check makes sure that the rsi configuration has port_mappings set up. If it doesn't, then it checks
   *          if there are any events with the field notifier.
   * \param[in] rsi Required Service Instance
   * \return false if there is no unicast network endpoint configured and the RSI has a field notifier, otherwise true.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateRsiHasUnicastEndpointWhenFieldConfigured(IpcChannel::RequiredServiceInstance const& rsi) const noexcept
      -> bool;

  /*!
   * \brief Validate if the RequiredServiceInstance has a field notifier configured.
   * \param[in] rsi Required Service Instance
   * \return true if Required Service Instance has a field notifier, otherwise false.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateIfRsiHasFieldConfigured(IpcChannel::RequiredServiceInstance const& rsi) const noexcept -> bool;

  /*!
   * \brief Checks if a Provided Service Instance has either TCP or UDP ports configured
   * \param[in] psi Provided Service Instance
   * \return true if there are either TCP no UDP port configured for the provided service instance
   *              machine mapping
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto HasPortsConfigured(IpcChannel::ProvidedServiceInstance const& psi) noexcept -> bool;

  /*!
   * \brief Ensure consistency of required and configured transport protocols for a Provided Service Instance.
   * \param[in] psi Provided Service Instance
   * \details This check validates that a PSI requiring a transport protocol has
   *          this transport protocol configured in its machine mapping
   * \return true if the configuration is consistent, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidatePsiRequiredTransportProtocols(IpcChannel::ProvidedServiceInstance const& psi) const noexcept -> bool;

  /*!
   * \brief Checks if all machine mappings for a provided service instance have a TCP port configured.
   * \param[in] psi Provided Service Instance
   * \details This check validates that a PSI has configured a TCP port in all its machine mappings.
   * \return true if all machine mappings configure a TCP port, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto HasTcpPortConfiguredInAllMachineMappings(IpcChannel::ProvidedServiceInstance const& psi) noexcept -> bool;

  /*!
   * \brief Checks if all machine mappings for a provided service instance have a UDP port configured.
   * \param[in] psi Provided Service Instance
   * \details This check validates that a PSI has configured a UDP port in all its machine mappings.
   * \return true if all machine mappings configure a UDP port, otherwise false.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto HasUdpPortConfiguredInAllMachineMappings(IpcChannel::ProvidedServiceInstance const& psi) noexcept -> bool;

  /*!
   * \brief   Add network endpoint ports to the global configuration of a network endpoint.
   * \details This method will add ports to the list of ports for the global configuration of a network endpoint. If
   * the port already exists in the list a check will be made to see if the ports can be merged. If there is an
   * inconsistency, the merge process will be stopped and false is returned.
   * \param[in] network_endpoint The network endpoint to add the ports from.
   * \param[in] global_network_endpoint_ports The list of ports for the global configuration of the same network
   * endpoint.
   * \return true if the network endpoint ports were added successfully, otherwise false.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto AddNetworkEndpointPorts(NetworkEndpoint const& network_endpoint,
                               NetworkEndpointPortContainer& global_network_endpoint_ports) const noexcept -> bool;

  /*!
   * \brief Append SecureIds of a port which should be merged to the SecuredIds of the existing port
   * \param[in] existing_port Existing port
   * \param[in] port Port which should be merged into the existing port
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static void AppendSecureIdsForMergedPorts(NetworkEndpointPort& existing_port,
                                            NetworkEndpointPort const& port) noexcept;

  /*!
   * \brief Check whether the service has signal based events.
   * \param[in] service Service
   * \return true if the Service has signal based event, false otherwise.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto CheckIfS2SEventSet(Service const& service) noexcept -> bool;

  /*!
   * \brief Checks if the two services are consistent.
   * \param[in] service_to_insert New service to add to configuration.
   * \param[in] existing_service Service which already exists in configuration.
   * \return True if they are consistent, otherwise false.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ServiceConsistencyChecks(ConfigurationTypesAndDefs::Service const& service_to_insert,
                                ConfigurationTypesAndDefs::Service const& existing_service) const noexcept -> bool;

  /*!
   * \brief Update a secure id in a given SecureIdContainer from a given old value to a given new value.
   * \param[in] old_secure_id  Previous secure id used for matching
   * \param[in] new_secure_id  New secure id replaces the old_secure_id
   * \param[in] secure_id_container The SecureIdContainer to update
   * \context     ANY
   * \pre         -
   * \reentrant   FALSE
   * \steady FALSE
   */
  static void UpdateSecureIdInSecureIdArray(SecureId const& old_secure_id, SecureId const& new_secure_id,
                                            ConfigurationTypesAndDefs::SecureIdContainer& secure_id_container) noexcept;

  /*!
   * \brief Update parsed SecureIds of SecureConnections with globally unique sequence SecureIds
   * \details Parsed SecureIds are locally used in one Json file. Different Json files can refer to different
   * SecureConnections using the same local SecureId. Therefore, the local SecureId must be replaced with a globally
   * unique SecureId.
   * \param[in] parsed_secure_connections SecureConnections parsed from one Json file
   * \param[in] parsed_rsi_container RequiredServiceInstances parsed from one Json file
   * \param[in] parsed_network_endpoint_container NetworkEndpoints parsed from one Json file
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  void UpdateSecureIds(SecureConnectionContainer& parsed_secure_connections,
                       IpcChannel::RequiredServiceInstanceContainer& parsed_rsi_container,
                       NetworkEndpointContainer& parsed_network_endpoint_container) const noexcept;

  /*!
   * \brief Validate Dtls cookie verification enabled for all SecureConnections on required service instances
   * \return true, validation succeeds, false otherwise
   * \context ANY
   * \pre One IPC channel has been added.
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateDtlsCookieVerificationEnabledRsiThenNep() const noexcept -> bool;

  /*!
   * \brief Validate Dtls cookie verification enabled for all SecureConnections on network endpoint ports
   * \param[in] first_udp_endpoint Flag indicating if a UDP endpoint was already validated
   * \param[in] udp_cookie_is_set Flag indicating if first read UDP endpoint has the Dtls cookie set
   * \return true, validation succeeds, false otherwise
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ValidateDtlsCookieVerificationEnabledNep(bool first_udp_endpoint, bool udp_cookie_is_set) const noexcept -> bool;

  /*! Friend tests for accessing sec_com_config_. */
  FRIEND_TEST(UT__SomeIpd__ValidateDtlsCookieVerificationEnabled, TwoTCPSecureEndpoints_NoCookieFlag);
  FRIEND_TEST(UT__SomeIpd__ValidateDtlsCookieVerificationEnabled, TwoTCPSecureEndpoint_OneWithCookieFlagConfig);
  FRIEND_TEST(UT__SomeIpd__ValidateDtlsCookieVerificationEnabled,
              OneEmptyCookieFlagConfigTCP_TwoConsistentCookieFlagConfigUDP);
  FRIEND_TEST(UT__SomeIpd__ValidateDtlsCookieVerificationEnabled, OneEmptyTCP_TwoDifferentFlagConfigUDP);
  FRIEND_TEST(UT__SomeIpd__ValidateDtlsCookieVerificationEnabled, OneValuedTCP_TwoConsistentCookieFlagConfigUDP);
  FRIEND_TEST(UT__SomeIpd__ValidateDtlsCookieVerificationEnabled, ThreeDifferentCookieFlagConfigUDP);
  FRIEND_TEST(UT__SomeIpd__ValidateDtlsCookieVerificationEnabled, ThreeConsistentCookieFlagConfigUDP);
  FRIEND_TEST(UT__SomeIpd__ValidateDtlsCookieVerificationEnabled,
              TwoConsistentCookieFlagConfigUDP_OneEmptyUDPSecureEnpoint);
  FRIEND_TEST(Cdd__SomeIpd__ConnectionManagerSecureCom, LoadPskConfigAndCipherSuites);
};

}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CONFIGURATION_H_

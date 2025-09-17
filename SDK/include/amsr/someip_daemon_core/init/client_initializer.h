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
/**        \file  client_initializer.h
 *         \brief  Client Initializer.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_CLIENT_INITIALIZER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_CLIENT_INITIALIZER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/client/required_service_instance_interface.h"
#include "amsr/someip_daemon_core/client/required_service_instance_manager_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/connection_manager/connection_manager.h"
#include "amsr/someip_daemon_core/network/interface_card_monitor_manager_interface.h"
#include "amsr/someip_daemon_core/packet_router/application_packet_router_interface.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_factory_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/map.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace init {

/*!
 * \brief ClientInitializer.
 */
class ClientInitializer final {
 private:
  /*!
   * \brief Shared pointer to a ServiceDiscoveryClientInterface.
   */
  using ServiceDiscoveryClientInterfaceSharedPtr =
      std::shared_ptr<service_discovery::state_machine::client::ServiceDiscoveryClientInterface>;

 public:
  /*!
   * \brief Default constructor.
   *
   * \pre -
   * \context Init
   * \steady  FALSE
   */
  ClientInitializer() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  ~ClientInitializer() noexcept = default;

  ClientInitializer(ClientInitializer const&) = delete;
  ClientInitializer(ClientInitializer&&) = delete;
  ClientInitializer& operator=(ClientInitializer const&) & = delete;
  ClientInitializer& operator=(ClientInitializer&&) & = delete;

  /*!
   * \brief       InitializeRequiredServices.
   *
   * \param[in]   configuration                           The configuration. No two instances of same
   *                                                      RequiredServiceInstances id is allowed.
   * \param[in]   required_service_instance_manager       The Required Service Instance manager
   * \param[in]   service_discovery                       The Service Discovery
   * \param[in]   sd_client_factory                       The Service Discovery Client Factory
   * \param[in]   timer_manager                           The Timer Manager
   * \param[in]   reactor                                 The Reactor
   * \param[in]   statistics_handler                      The Statistics Handler
   * \param[in]   connection_manager                      The Connection manager
   * \param[in]   packet_router                           The packet router.
   * \param[in]   interface_card_monitor_manager          The Network Interface Monitor Manager
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void InitializeRequiredServices(
      configuration::Configuration const& configuration,
      client::RequiredServiceInstanceManagerInterface& required_service_instance_manager,
      service_discovery::ServiceDiscoveryInterface& service_discovery,
      service_discovery::state_machine::client::ServiceDiscoveryClientFactoryInterface const& sd_client_factory,
      amsr::steady_timer::TimerManagerInterface* timer_manager, osabstraction::io::reactor1::Reactor1Interface& reactor,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
      connection_manager::ConnectionManager& connection_manager,
      std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
      network::InterfaceCardMonitorManagerInterface& interface_card_monitor_manager) noexcept;

  /*!
   * \brief       DeinitializeRequiredServices.
   *
   * \param[in]   configuration                      The configuration.
   * \param[in]   required_service_instance_manager  The Required Service Instance manager
   * \param[in]   service_discovery                  The Service Discovery
   * \param[in]   interface_card_monitor_manager     The Network Interface Monitor Manager
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void DeinitializeRequiredServices(
      configuration::Configuration const& configuration,
      client::RequiredServiceInstanceManagerInterface& required_service_instance_manager,
      service_discovery::ServiceDiscoveryInterface& service_discovery,
      network::InterfaceCardMonitorManagerInterface& interface_card_monitor_manager) noexcept;

 private:
  /*!
   * \brief Perform initialization for a single required service instance.
   *
   * @param[in] configuration                     The configuration. No two instances of same RequiredServiceInstances
   *                                              id is allowed.
   * @param[in] required_service_instance_manager The Required Service Instance manager
   * @param[in] service_discovery                 The Service Discovery
   * @param[in] sd_client_factory                 The Service Discovery Client Factory
   * @param[in] timer_manager                     The Timer Manager
   * @param[in] reactor                           The Reactor
   * @param[in] statistics_handler                The Statistics Handler
   * @param[in] connection_manager                The Connection manager
   * @param[in] packet_router                     The packet router.
   * @param[in] interface_card_monitor_manager    The Network Interface Monitor Manager
   * @param[in] logger                            The logger
   * @param[in] rsi_configuration                 The required service instance to initialize
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void InitializeRequiredService(
      configuration::Configuration const& configuration,
      client::RequiredServiceInstanceManagerInterface& required_service_instance_manager,
      service_discovery::ServiceDiscoveryInterface& service_discovery,
      service_discovery::state_machine::client::ServiceDiscoveryClientFactoryInterface const& sd_client_factory,
      amsr::steady_timer::TimerManagerInterface* timer_manager, osabstraction::io::reactor1::Reactor1Interface& reactor,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
      connection_manager::ConnectionManager& connection_manager,
      std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
      network::InterfaceCardMonitorManagerInterface& interface_card_monitor_manager,
      someip_daemon_core::logging::AraComLogger const& logger,
      configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const& rsi_configuration) noexcept;

  /*!
   * \brief       Create a required service instance for instance id and emplace it in the required service
   *              instance manager container
   *
   * \param[in] timer_manager                         The timer manager
   * \param[in] reactor                               The reactor
   * \param[in] connection_manager                    The connection manager
   * \param[in] required_service_instance_deployment  The service deployment
   * \param[in] instance_id                           The instance id
   * \param[in] required_service_instance_manager     The required service instance manager, to emplace
   *                                                  the created required service instance
   * \param[in] sd_client_interface_ptr               A pointer to the service discovery client
   * \param[in] packet_router                         The packet router.
   * \param[in] network_endpoint                      The network endpoint.
   * \param[in] required_eventgroups                  The required eventgroups.
   * \param[in] required_pdus                         The required PDUs
   * \param[in] machine_mapping                       The machine mapping
   * \param[in] is_communication_only                 Whether the requested RemoteServer has SD disabled
   * \param[in] sd_endpoint                           The SD endpoint
   * \param[in] method_props                          The method accumulation timeouts
   * \param[in] custom_subscription_endpoints         The custom subscription endpoints
   * \param[in] sec_com_config                        The secure communication configuration
   *
   * \return A shared pointer to the created required service instance
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static std::shared_ptr<client::RequiredServiceInstanceInterface> CreateRequiredServiceInstance(
      amsr::steady_timer::TimerManagerInterface* timer_manager, osabstraction::io::reactor1::Reactor1Interface& reactor,
      connection_manager::ConnectionManager& connection_manager,
      configuration::types::RequiredSomeIpServiceInstanceDeployment const& required_service_instance_deployment,
      someip_protocol::internal::InstanceId const instance_id,
      client::RequiredServiceInstanceManagerInterface& required_service_instance_manager,
      ServiceDiscoveryClientInterfaceSharedPtr sd_client_interface_ptr,
      std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
      configuration::types::RequiredNetworkEndpoint const& network_endpoint,
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>> const&
          required_eventgroups,
      configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const& required_pdus,
      configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping, bool const is_communication_only,
      service_discovery::message::scheduler::SchedulerInterface* sd_endpoint,
      configuration::Configuration::MethodsMessageAccumulationTimeoutMap const& method_props,
      configuration::types::ServiceAddress const& custom_subscription_endpoints,
      configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config) noexcept;

  /*!
   * \brief       Initialize the RequiredServiceInstance with ServiceDiscovery.
   *
   * \param[in]   service_deployment_id                 The service deployment id
   * \param[in]   instance_id                           The instance id
   * \param[in]   sd_endpoint                           The service discovery endpoint
   * \param[in]   sd_client_interface_ptr               A pointer to the service discovery client
   * \param[in]   required_service_instance_shared_ptr  A pointer to the required service instance
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void InitializeRequiredServiceInstanceWithSD(
      configuration::types::RequiredSomeIpServiceInstanceDeploymentId const& service_deployment_id,
      someip_protocol::internal::InstanceId const instance_id, service_discovery::ServiceDiscoveryEndpoint* sd_endpoint,
      ServiceDiscoveryClientInterfaceSharedPtr sd_client_interface_ptr,
      std::shared_ptr<client::RequiredServiceInstanceInterface> required_service_instance_shared_ptr) noexcept;

  /*!
   * \brief       Initialize the RequiredServiceInstance without ServiceDiscovery.
   *
   * \param[in]   rsi_configuration                     The required service instance configuration
   * \param[in]   required_service_instance_shared_ptr  A pointer to the required service instance
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void InitializeRequiredServiceInstanceWithoutSD(
      configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const& rsi_configuration,
      std::shared_ptr<client::RequiredServiceInstanceInterface> required_service_instance_shared_ptr) noexcept;

  /*!
   * \brief       Deinitialize the RequiredServiceInstance with ServiceDiscovery.
   *
   * \param[in]   rsi         The required service instance configuration
   * \param[in]   sd_endpoint The service discovery endpoint
   *
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void DeinitializeRequiredServiceInstanceWithSD(
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const& rsi,
      service_discovery::ServiceDiscoveryEndpoint* sd_endpoint) noexcept;

  /*!
   * \brief       Register the sd client observer into the interface card manager.
   *
   * \param[in]   rsi_configuration              The required service instance configuration
   * \param[in]   sd_client_interface_ptr        A pointer to the service discovery client
   * \param[in]   interface_card_monitor_manager The Interface card monitor manager
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void RegisterInterfaceCardObserver(
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const&
          rsi_configuration,
      ServiceDiscoveryClientInterfaceSharedPtr sd_client_interface_ptr,
      network::InterfaceCardMonitorManagerInterface& interface_card_monitor_manager) noexcept;
};

}  // namespace init
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_CLIENT_INITIALIZER_H_

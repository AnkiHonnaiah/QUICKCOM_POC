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
/**        \file  service_discovery_client_factory.h
 *        \brief  Factory for ServiceDiscoveryClient
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_FACTORY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_service_instance_config.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_factory_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {

/*!
 * \brief Factory for ServiceDiscoveryClient.
 *
 * \tparam ServiceDiscoveryClientType Exact type of ServiceDiscoveryClient (used for testing)
 */
template <typename ServiceDiscoveryClientType = ServiceDiscoveryClient<>>
class ServiceDiscoveryClientFactory final : public ServiceDiscoveryClientFactoryInterface {
 public:
  /*!
   * \brief Factory Constructor.
   *
   * \param[in] timer_manager               A timer manager.
   * \param[in] statistics_handler          A reference to the statistics handler.
   *
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryClientFactory(
      amsr::steady_timer::TimerManagerInterface* timer_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept
      : ServiceDiscoveryClientFactoryInterface(),
        timer_manager_{timer_manager},
        statistics_handler_{statistics_handler} {}

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief default destructor
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryClientFactory() noexcept override = default;

  ServiceDiscoveryClientFactory(ServiceDiscoveryClientFactory const&) = delete;
  ServiceDiscoveryClientFactory(ServiceDiscoveryClientFactory&&) = delete;
  ServiceDiscoveryClientFactory& operator=(ServiceDiscoveryClientFactory const&) & = delete;
  ServiceDiscoveryClientFactory& operator=(ServiceDiscoveryClientFactory&&) & = delete;

 private:
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Create ServiceDiscoveryClient instance.
   *
   * \param[in] required_service_instance_deployment  A reference to the service interface deployment.
   * \param[in] instance_id                           A SOME/IP service instance identifier.
   * \param[in] sdclient_service_instance_config      A reference to the service discovery service instance config.
   * \param[in] network_endpoint                      A reference to the network endpoint.
   * \param[in] required_eventgroups                  Required eventgroups timing configuration
   * \param[in] message_scheduler                     Message scheduler, used to send out find service SD entries.
   *
   * \return    The newly create ServiceDiscoveryClient instance.
   *
   * \pre       -
   * \context Init
   * \steady FALSE
   */
  std::shared_ptr<ServiceDiscoveryClientInterface> CreateSdClient(
      configuration::types::RequiredSomeIpServiceInstanceDeployment const& required_service_instance_deployment,
      amsr::someip_protocol::internal::InstanceId const instance_id,
      configuration::types::SomeipSdClientServiceInstanceConfig const& sdclient_service_instance_config,
      configuration::types::RequiredNetworkEndpoint const& network_endpoint,
      RequiredEventgroupsContainer const& required_eventgroups,
      message::scheduler::SchedulerInterface* message_scheduler) const noexcept override {
    std::shared_ptr<ServiceDiscoveryClientType> service_discovery_client{
        someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<ServiceDiscoveryClientType>(
            required_service_instance_deployment, instance_id, sdclient_service_instance_config, network_endpoint,
            required_eventgroups, timer_manager_, message_scheduler, statistics_handler_)};
    return service_discovery_client;
  }

  /*!
   * \brief A timer manager.
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief A statistics handler.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;
};
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_FACTORY_H_

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
/**        \file  service_discovery_client_factory_interface.h
 *        \brief  Interface for ServiceDiscoveryClientFactory
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_FACTORY_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_service_instance_config.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_interface.h"
#include "ara/core/map.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {

/*!
 * \brief Interface for ServiceDiscoveryClientFactory
 */
class ServiceDiscoveryClientFactoryInterface {
 public:
  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

  /*!
   * \brief default constructor
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryClientFactoryInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryClientFactoryInterface() noexcept = default;

  ServiceDiscoveryClientFactoryInterface(ServiceDiscoveryClientFactoryInterface const&) = delete;
  ServiceDiscoveryClientFactoryInterface(ServiceDiscoveryClientFactoryInterface&&) = delete;
  ServiceDiscoveryClientFactoryInterface& operator=(ServiceDiscoveryClientFactoryInterface const&) & = delete;
  ServiceDiscoveryClientFactoryInterface& operator=(ServiceDiscoveryClientFactoryInterface&&) & = delete;

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
  virtual std::shared_ptr<ServiceDiscoveryClientInterface> CreateSdClient(
      configuration::types::RequiredSomeIpServiceInstanceDeployment const& required_service_instance_deployment,
      amsr::someip_protocol::internal::InstanceId const instance_id,
      configuration::types::SomeipSdClientServiceInstanceConfig const& sdclient_service_instance_config,
      configuration::types::RequiredNetworkEndpoint const& network_endpoint,
      RequiredEventgroupsContainer const& required_eventgroups,
      message::scheduler::SchedulerInterface* message_scheduler) const noexcept = 0;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_FACTORY_INTERFACE_H_

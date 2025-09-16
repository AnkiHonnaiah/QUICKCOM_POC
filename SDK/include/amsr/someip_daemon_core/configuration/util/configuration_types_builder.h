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
/**        \file  configuration_types_builder.h
 *         \brief Configuration Types Builder.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_UTIL_CONFIGURATION_TYPES_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_UTIL_CONFIGURATION_TYPES_BUILDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <set>
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/provided_someip_eventgroup.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_service_instance_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/map.h"
#include "ara/core/memory_resource.h"
#include "ara/core/optional.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace util {

/*!
 * \brief   ConfigurationTypesBuilder.
 * \details Build configuration parameters from the big configuration object.
 */
class ConfigurationTypesBuilder final {
 private:
  /*!
   * \brief Alias for RequiredSomeIpServiceInterfaceDeployment
   */
  using RequiredSomeIpServiceInstanceDeployment =
      someip_daemon_core::configuration::types::RequiredSomeIpServiceInstanceDeployment;

  /*!
   * \brief Alias for RequiredSomeIpServiceInstanceDeploymentId
   */
  using RequiredSomeIpServiceInstanceDeploymentId =
      someip_daemon_core::configuration::types::RequiredSomeIpServiceInstanceDeploymentId;

  /*!
   * \brief Alias for SomeIpServiceInterfaceCommunicationDeployment
   */
  using SomeIpServiceInterfaceCommunicationDeployment =
      someip_daemon_core::configuration::types::SomeIpServiceInterfaceCommunicationDeployment;

  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer = ara::core::Map<
      ::amsr::someip_protocol::internal::EventgroupId,
      ara::core::Optional<someip_daemon_core::configuration::types::SomeipSdClientEventGroupTimingConfig>>;

 public:
  /*!
   * \brief Container of events configured for a provided service instance that are configured as fields
   */
  using ProvidedFieldEvents =
      std::set<amsr::someip_protocol::internal::EventId, std::less<amsr::someip_protocol::internal::EventId>,
               ara::core::PolymorphicAllocator<amsr::someip_protocol::internal::EventId>>;

  /*!
   * \brief Build the required service interface deployment configuration
   *
   * \param[in] logger        A logger.
   * \param[in] rsi           The required service instance configuration
   * \param[in] configuration The complete daemon configuration
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \return The required service instance deployment configuration
   */
  static RequiredSomeIpServiceInstanceDeployment BuildRequiredServiceInstanceDeployment(
      someip_daemon_core::logging::AraComLogger const& logger,
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const& rsi,
      someip_daemon_core::configuration::Configuration const& configuration) noexcept;

  /*!
   * \brief Build the network endpoint configuration
   *
   * \param[in] machine_mapping Machine mapping for the service instance
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \return The network endpoint configuration
   */
  static someip_daemon_core::configuration::types::RequiredNetworkEndpoint BuildRequiredNetworkEndpoint(
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping) noexcept;

  /*!
   * \brief Build the mapping between eventgroups and events
   *
   * \param[in] rsi The required service instance configuration
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \return The eventgroup timing configuration of the required eventgroups
   */
  static RequiredEventgroupsContainer BuildRequiredEventgroupsContainer(
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const&
          rsi) noexcept;

  /*!
   * \brief Check if only communication (static SD) is configured for a required service instance
   *
   * \param[in] rsi The required service instance configuration
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \return true if static sd is configured for this required service instance
   */
  static bool IsCommunicationOnly(
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const&
          rsi) noexcept;

  /*!
   * \brief Build the SD client service instance configuration
   *
   * \param[in] rsi The required service instance configuration
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \return The SD client timing configuration
   * \trace SPEC-10144749
   */
  static configuration::types::SomeipSdClientServiceInstanceConfig BuildSdClientServiceInstanceConfig(
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance const&
          rsi) noexcept;

  /*!
   * \brief Build the custom subscription endpoint options data structure
   *
   * \param[in] machine_mapping Machine mapping for the service instance
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \return The custom subscription endpoint options
   */
  static configuration::types::ServiceAddress BuildCustomSubscriptionEndpointOptions(
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping) noexcept;

  /*!
   * \brief Build a provided SOME/IP eventgroup map based on a provided service instance configuration
   *
   * \param[in] someip_service_instance_id  A provided service instance id
   * \param[in] machine_mapping             Machine mapping for the service instance
   * \param[in] config                      Configuration
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \return A map containing the provided eventgroups of the provided service instance by eventgroup id.
   */
  static ara::core::Map<someip_protocol::internal::EventgroupId, configuration::types::ProvidedSomeIpEventgroup>
  BuildProvidedSomeIpEventgroups(
      configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id,
      someip_daemon_core::configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping,
      Configuration const& config) noexcept;

  /*!
   * \brief Build a provided SOME/IP field event container based on a provided service instance configuration.
   *
   * \param[in] someip_service_instance_id  A provided service instance id
   * \param[in] config                      Configuration
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \return A set containing fields.
   */
  static ProvidedFieldEvents BuildProvidedSomeIpFieldEvent(
      configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id,
      Configuration const& config) noexcept;
};

}  // namespace util
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_UTIL_CONFIGURATION_TYPES_BUILDER_H_

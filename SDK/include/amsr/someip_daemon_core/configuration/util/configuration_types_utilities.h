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
/**        \file  configuration_types_utilities.h
 *         \brief Configuration Types Utilities.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_UTIL_CONFIGURATION_TYPES_UTILITIES_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_UTIL_CONFIGURATION_TYPES_UTILITIES_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_event_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_method_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/configuration/types/transport_protocol.h"
#include "ara/core/map.h"
#include "ara/core/optional.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace util {

/*!
 * \brief   ConfigurationTypesUtilities.
 * \details Utility class with common functionality needed to handle some configuration types.
 */
class ConfigurationTypesUtilities final {
 private:
  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

 public:
  /*!
   * \brief Checks if the given transport protocol communication is required by the configuration.
   *
   * \param[in] communication_deployment  Service deployment to access method, event and eventgroup configuration
   * \param[in] required_eventgroups      Required eventgroups timing configuration
   * \param[in] protocol                  Transport protocol to check
   *
   * \context Init
   * \return true if the given transport protocol communication is required by the configuration
   * \steady FALSE
   */
  static bool IsTransportProtocolRequired(
      configuration::types::SomeIpServiceInterfaceCommunicationDeployment const& communication_deployment,
      RequiredEventgroupsContainer const& required_eventgroups,
      configuration::types::TransportProtocol const protocol) noexcept;

  /*!
   * \brief Checks if one of the events given in the event container is configured in the event deployment with
   *        the given transport protocol
   *
   * \param[in] event_deployment  Event deployment information to access the configured transport protocol
   *                              for each event
   * \param[in] events            Container of events to check if they have the specified protocol
   * \param[in] protocol          Transport protocol to check
   *
   * \context App
   * \return true if any of the events in the event container has at least one event with the given transport protocol
   * \steady FALSE
   */
  static bool HasEventWithProto(ara::core::Vector<configuration::types::SomeIpEventDeployment> const& event_deployment,
                                ara::core::Vector<::amsr::someip_protocol::internal::EventId> const& events,
                                configuration::types::TransportProtocol const protocol) noexcept;

  /*!
   * \brief Test if an event group contains an event bound for transmission via a specific protocol
   *
   * \param[in] communication_deployment  Service deployment to access event and eventgroup configuration
   * \param[in] eventgroup_id             ID of the event group
   * \param[in] protocol                  The transport protocol
   *
   * \context Init|Reactor
   * \return true if the event group contains an event bound for transmission via the specified protocol
   * \steady FALSE
   */
  static bool HasRequiredEventWithProto(
      configuration::types::SomeIpServiceInterfaceCommunicationDeployment const& communication_deployment,
      someip_protocol::internal::EventgroupId const eventgroup_id,
      configuration::types::TransportProtocol const protocol) noexcept;

 private:
  /*!
   * \brief Checks if a method deployment has methods with the given protocol.
   *
   * \param[in] method_deployment Container of method deployment configuration.
   * \param[in] protocol          Transport protocol to check
   *
   * \context Init
   * \return true if the method deployment has at least one method with the given transport protocol
   * \steady FALSE
   */
  static bool HasMethodWithProto(
      ara::core::Vector<configuration::types::SomeIpMethodDeployment> const& method_deployment,
      configuration::types::TransportProtocol const protocol) noexcept;

  /*!
   * \brief Checks, for each required eventgroup, if any of the events belonging to that eventgroup are
   *        configured with the given transport protocol
   *
   * \param[in] communication_deployment  Service deployment to access event and eventgroup configuration
   * \param[in] required_eventgroups      Required eventgroups timing configuration to access the list of
   *                                      configured eventgroups
   * \param[in] protocol                  Transport protocol to check
   *
   * \context Init
   * \return true if the method deployment has at least one event with the given transport protocol
   * \steady FALSE
   */
  static bool HasRequiredEventWithProto(
      configuration::types::SomeIpServiceInterfaceCommunicationDeployment const& communication_deployment,
      RequiredEventgroupsContainer const& required_eventgroups,
      configuration::types::TransportProtocol const protocol) noexcept;
};

}  // namespace util
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_UTIL_CONFIGURATION_TYPES_UTILITIES_H_

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
/*!        \file  provided_service_instance_validator.h
 *        \brief  Validates Configuration object 'ProvidedServiceInstance'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'provided service instance' elements.
 */
class ProvidedServiceInstanceValidator final {
  /*!
   * \brief A typedef for MachineMappingContainer
   */
  using MachineMappingContainer =
      amsr::someip_daemon_core::configuration::ConfigurationTypesAndDefs::MachineMappingContainer;

  /*!
   * \brief A typedef for EventgroupIdContainer
   */
  using EventgroupIdContainer =
      amsr::someip_daemon_core::configuration::ConfigurationTypesAndDefs::EventgroupIdContainer;

 public:
  /*!
   * \brief Validation results.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The object is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief No service ID configured.
     */
    kMissingServiceId,

    /*!
     * \brief No instance ID configured.
     */
    kMissingInstanceId,

    /*!
     * \brief No service major version configured.
     */
    kMissingMajorVersion,

    /*!
     * \brief No service minor version configured.
     */
    kMissingMinorVersion,

    /*!
     * \brief No service discovery containers configured.
     */
    kMissingServiceDiscovery,

    /*!
     * \brief No port mapping container configured.
     */
    kMissingPortMappings,

    /*!
     * \brief Dynamic ports are not allowed for provided service instances.
     */
    kNotSupportedDynamicPorts,

    /*!
     * \brief Static Service discovery eventgroups should be part of provided eventgroups.
     */
    kSDEventGroupInvalid
  };

  /*!
   * \brief Translate the validation result to string for logging.
   * \param[in] validation_result Validation result to translate.
   * \return String representation of the passed-in validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Translate the validation result to string and return it.
   * \endinternal
   */
  static char const* ValidationResultToString(ValidationResult validation_result) {
    char const* ret{nullptr};

    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;

      case ValidationResult::kMissingServiceId:
        ret = "Service ID is missing";
        break;

      case ValidationResult::kMissingInstanceId:
        ret = "Instance ID is missing";
        break;

      case ValidationResult::kMissingMajorVersion:
        ret = "Major version is missing";
        break;

      case ValidationResult::kMissingMinorVersion:
        ret = "Minor version is missing";
        break;

      case ValidationResult::kMissingServiceDiscovery:
        ret = "Service discovery is not configured";
        break;

      case ValidationResult::kMissingPortMappings:
        ret = "Port mappings are missing";
        break;

      case ValidationResult::kNotSupportedDynamicPorts:
        ret = "Dynamic port assignment is not supported";
        break;

      case ValidationResult::kSDEventGroupInvalid:
        ret =
            "One of the port_mappings->subscribers->eventgroups is not part of the service discovery configuration of "
            "the corresponding provided service instance.";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check and returns if any machine mapping uses a dynamic port assignment.
   * \param[in] machine_mappings Container of machine mappings to investigate
   * \return true if a machine mapping with a dynamic port is found, otherwise false.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Iterate over the given machine mappings.
   *   - If any machine mapping is using a dynamic port assignment (either TCP or UDP), return true
   * - Return if a dynamic port assignment has been found.
   * \endinternal
   */
  static bool HasDynamicPortAssignment(MachineMappingContainer const& machine_mappings) {
    MachineMappingContainer::const_iterator const result{
        std::find_if(machine_mappings.cbegin(), machine_mappings.cend(),
                     [](MachineMappingContainer::value_type const& mapping_under_investigation) {
                       bool const has_dynamic_udp_port{mapping_under_investigation.udp_port_.has_value() &&
                                                       (mapping_under_investigation.udp_port_.value() == 0)};
                       bool const has_dynamic_tcp_port{mapping_under_investigation.tcp_port_.has_value() &&
                                                       (mapping_under_investigation.tcp_port_.value() == 0)};
                       return has_dynamic_tcp_port || has_dynamic_udp_port;
                     })};
    return result != machine_mappings.cend();
  }

  /*!
   * \brief Check and returns if machine mapping has static SD configured.
   * \param[in] machine_mappings Container of machine mappings to investigate
   * \return true if a machine mapping with static SD is found, otherwise false.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Iterate over the given machine mappings.
   *   - If any machine mapping is configured with static SD, return true
   * - Return true if static SD is configured, false otherwise.
   * \endinternal
   */
  static bool HasStaticSDConfigured(MachineMappingContainer const& machine_mappings) {
    MachineMappingContainer::const_iterator const result{
        std::find_if(machine_mappings.cbegin(), machine_mappings.cend(),
                     [](MachineMappingContainer::value_type const& mapping_under_investigation) {
                       return ((mapping_under_investigation.service_discovery_enabled_ == false) &&
                               (mapping_under_investigation.subscribers_.has_value()));
                     })};
    return result != machine_mappings.cend();
  }

  /*!
   * \brief Check and returns if port_mappings/subscribers/eventgroups are not present in service_discovery/eventgroups.
   * \param[in] machine_mappings Container of machine mappings (port mappings) to investigate.
   * \param[in] service_discovery Service discovery configurations of the provided instance.
   * \return true if all port_mappings/subscribers/eventgroups are present under service_discovery/eventgroups,
   *         otherwise false.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Iterate over the service discovery event group.
   *   - Add the provided event groups ids into a list.
   * - Iterate over the given machine mappings (port mappings).
   *   - Iterate over the event groups of the corresponding subscribers.
   *     - if event group ID is not present in the service discovery configurations
   *       - Return false.
   *     - Otherwise true.
   * \endinternal
   */
  static bool CheckIfStaticSdReferencingValidEventgroupIds(
      MachineMappingContainer const& machine_mappings,
      ConfigurationTypesAndDefs::ProvidedServiceInstanceServiceDiscovery const& service_discovery) {
    bool result{true};
    EventgroupIdContainer provided_event_group_id_list{};
    for (ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroup const& provided_eventgroup_ :
         service_discovery.eventgroups_) {
      static_cast<void>(provided_event_group_id_list.push_back(provided_eventgroup_.id_));
    }

    for (amsr::someip_daemon_core::configuration::ConfigurationTypesAndDefs::MachineMapping const& machine_mapping :
         machine_mappings) {
      if ((machine_mapping.subscribers_->data() != nullptr) &&
          (!(machine_mapping.subscribers_->data()->eventgroup_.empty())) && (!provided_event_group_id_list.empty())) {
        for (someip_protocol::internal::EventgroupId const& subscriber_eventgroup_id :
             machine_mapping.subscribers_->data()->eventgroup_) {
          EventgroupIdContainer::const_iterator const provided_eventgroup_ids_begin{
              provided_event_group_id_list.cbegin()};
          EventgroupIdContainer::const_iterator const provided_eventgroup_ids_end{provided_event_group_id_list.cend()};
          EventgroupIdContainer::const_iterator const it{
              std::find(provided_eventgroup_ids_begin, provided_eventgroup_ids_end, subscriber_eventgroup_id)};
          if (it == provided_eventgroup_ids_end) {
            result = false;
          }
        }
      }
    }

    return result;
  }

  /*!
   * \brief Check if this service parsed previously is valid.
   * \details
   * - Verify that the service ID is set (mandatory).
   * - Verify that the instance ID is set (mandatory).
   * - Verify that the service has a major version (mandatory).
   * - Verify that the service has a minor version (mandatory).
   * - Verify that a service discovery container exists.
   * - Verify that a port mapping container exists.
   * - Additional checks can be placed within this static member function.
   * \param[in] provided_service_instance_element The 'provided service instance' element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if either of ServiceIdElement, InstanceIdElement, MajorVersionElement, MinorVersionElement,
   *   PortMappingsContainerElement is not set
   *   - Set the validation result to corresponding configuration element is not configured.
   * - Check if ServiceDiscoveryElement is not set and no static SD configured,
   *   - Set the validation result to ServiceDiscoveryElement is not configured.
   * - Check if no dynamic port is used
   *   - Set the validation result to dynamic ports not supported.
   * - Check if static SD configured,
   *   - Check if service_discovery/eventgroups does not contain port_mappings/subscribers/eventgroups.
   *     - Set the validation result to subscriber event group is invalid.
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(ProvidedServiceInstanceConfigObject const& provided_service_instance_element) noexcept {
    ValidationResult result{};

    if (provided_service_instance_element.GetServiceId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingServiceId;
    } else if (provided_service_instance_element.GetInstanceId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingInstanceId;
    } else if (provided_service_instance_element.GetMajorVersion().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingMajorVersion;
    } else if (provided_service_instance_element.GetMinorVersion().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingMinorVersion;
    } else if ((provided_service_instance_element.GetServiceDiscovery().GetStatus() != ElementStatus::kSet) &&
               (!HasStaticSDConfigured(provided_service_instance_element.GetPortMappings().GetElement()))) {
      result = ValidationResult::kMissingServiceDiscovery;
    } else if (provided_service_instance_element.GetPortMappings().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPortMappings;
    } else if (HasDynamicPortAssignment(provided_service_instance_element.GetPortMappings().GetElement())) {
      result = ValidationResult::kNotSupportedDynamicPorts;
    } else if (HasStaticSDConfigured(provided_service_instance_element.GetPortMappings().GetElement())) {
      if (!CheckIfStaticSdReferencingValidEventgroupIds(
              provided_service_instance_element.GetPortMappings().GetElement(),
              provided_service_instance_element.GetServiceDiscovery().GetElement())) {
        result = ValidationResult::kSDEventGroupInvalid;
      }
    } else {
      result = ValidationResult::kOk;
    }

    return result;
  }
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_VALIDATOR_H_

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
/*!        \file  required_service_instance_validator.h
 *        \brief  Validates Configuration object 'RequiredServiceInstance'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_object.h"
#include "ara/core/map.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'required service instance' elements.
 */
class RequiredServiceInstanceValidator final {
  static constexpr someip_protocol::internal::MinorVersion const kAnyStringValue{UINT32_MAX};

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
    kMissingPortMapping,

    /*!
     * \brief Static SD cannot be used when the required service instance requires ALL InstanceIds
     */
    kRequiredInstanceAllUsesStaticSd,

    /*!
     * \brief Dynamic ports are not allowed for UDP.
     */
    kNotSupportedDynamicPortsWithUdp,

    /*!
     * \brief If version driven find behavior is configured with "minimumMinorVersion", the minor version must not be
     * any
     */
    kMinorVersionMustNotBeAny,

    /*!
     * \brief Communication must not be disabled for required service instances.
     */
    kCommunicationDisabled
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
  // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
  static char const* ValidationResultToString(ValidationResult validation_result) {
    char const* ret{nullptr};

    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;

      case ValidationResult::kMissingServiceId:
        ret = "No service ID";
        break;

      case ValidationResult::kMissingInstanceId:
        ret = "No instance ID";
        break;

      case ValidationResult::kMissingMajorVersion:
        ret = "No service major version";
        break;

      case ValidationResult::kMissingMinorVersion:
        ret = "No service minor version";
        break;

      case ValidationResult::kMissingServiceDiscovery:
        ret = "No service discovery containers";
        break;

      case ValidationResult::kMissingPortMapping:
        ret = "No port mapping container";
        break;

      case ValidationResult::kRequiredInstanceAllUsesStaticSd:
        ret = "Required service instance with instance ID ALL cannot use static service discovery";
        break;

      case ValidationResult::kNotSupportedDynamicPortsWithUdp:
        ret = "Dynamic port assignment is not supported with UDP";
        break;

      case ValidationResult::kMinorVersionMustNotBeAny:
        ret = "Required minor version must not be ANY when versionDrivenFindBehavior is minimumMinorVersion";
        break;

      case ValidationResult::kCommunicationDisabled:
        ret = "Communication must not be disabled for required service instance";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check and returns if machine mapping has static SD configured.
   * \param[in] machine_mapping object of machine mapping to investigate
   * \return true if a machine mapping with static SD is found, otherwise false.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Check if the machine mapping is configured with static SD,
   *   - Return true if static SD is configured, false otherwise.
   * \endinternal
   */
  static bool HasStaticSDConfigured(ConfigurationTypesAndDefs::MachineMapping const& machine_mapping) {
    bool result{false};
    if ((machine_mapping.service_discovery_enabled_ == false) && (machine_mapping.provider_.has_value())) {
      result = true;
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
   * - Verify that service discovery is not disabled while required instance id is set to All
   * - Additional checks can be placed within this static member function.
   * \param[in] required_service_instance_element The 'required service instance' element to verify.
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
   * - return the validation result
   * \endinternal
   */
  // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
  static ValidationResult Check(RequiredServiceInstanceConfigObject const& required_service_instance_element) noexcept {
    ValidationResult result{};

    if (required_service_instance_element.GetServiceId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingServiceId;
    } else if (required_service_instance_element.GetInstanceId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingInstanceId;
    } else if (required_service_instance_element.GetMajorVersion().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingMajorVersion;
    } else if (required_service_instance_element.GetMinorVersion().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingMinorVersion;
    } else if ((required_service_instance_element.GetServiceDiscovery().GetStatus() != ElementStatus::kSet) &&
               (!HasStaticSDConfigured(required_service_instance_element.GetPortMapping().GetElement()))) {
      result = ValidationResult::kMissingServiceDiscovery;
    } else if (required_service_instance_element.GetPortMapping().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPortMapping;
    } else if ((required_service_instance_element.GetInstanceId().GetElement() ==
                ConfigurationTypesAndDefs::kInstanceIdAll) &&
               (!required_service_instance_element.GetPortMapping().GetElement().service_discovery_enabled_)) {
      result = ValidationResult::kRequiredInstanceAllUsesStaticSd;
    } else if (required_service_instance_element.GetPortMapping().GetElement().udp_port_.has_value() &&
               (required_service_instance_element.GetPortMapping().GetElement().udp_port_.value() == 0)) {
      result = ValidationResult::kNotSupportedDynamicPortsWithUdp;
      // TPS_MANI_03619/[constr_3561]
    } else if ((required_service_instance_element.GetVersionDrivenFindBehavior().GetElement() ==
                ConfigurationTypesAndDefs::VersionDrivenFindBehavior::kMinimumMinorVersion) &&
               (required_service_instance_element.GetMinorVersion().GetElement() == kAnyStringValue)) {
      result = ValidationResult::kMinorVersionMustNotBeAny;
    } else if (required_service_instance_element.GetPortMapping().GetElement().communication_type_ ==
               ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kSdOnly) {
      result = ValidationResult::kCommunicationDisabled;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_VALIDATOR_H_

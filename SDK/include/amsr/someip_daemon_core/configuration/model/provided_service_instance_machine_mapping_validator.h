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
/*!        \file  provided_service_instance_machine_mapping_validator.h
 *        \brief  Validates Configuration object 'ProvidedServiceInstanceMachineMapping'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_MACHINE_MAPPING_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_MACHINE_MAPPING_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/machine_mapping_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'machine mapping' elements of 'provided service instances'.
 */
class ProvidedServiceInstanceMachineMappingValidator final {
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
     * \brief No IP address configured.
     */
    kMissingIpAddress,

    /*!
     * \brief There is no port configured for (at least) one of TCP or UDP.
     */
    kMissingPort,

    /*!
     * \brief Inconsistent configuration of event multicast address and port. Both parameters must be set or both
     * parameters must not be set.
     */
    kInconsistentEventMulticastAddressAndPort,

    /*!
     * \brief There is dynamic port configured for (at least) one of TCP or UDP.
     */
    kNotSupportedDynamicPorts,

    /*!
     * \brief SecureIds can only be set in machine mappings configuration of required service instances.
     */
    kNotSupportedSecureIds
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

      case ValidationResult::kMissingIpAddress:
        ret = "IP address is missing";
        break;

      case ValidationResult::kMissingPort:
        ret = "Port is not configured";
        break;

      case ValidationResult::kInconsistentEventMulticastAddressAndPort:
        ret = "Both address and port for event multicast address shall either be set or unset";
        break;

      case ValidationResult::kNotSupportedDynamicPorts:
        ret = "Dynamic port assignment is not supported";
        break;

      case ValidationResult::kNotSupportedSecureIds:
        ret = "SecureIds can only be set in machine mappings configuration of required service instances";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the IP address is set.
   * - Verify that a UDP or a TCP port is set (constr_3290).
   * - Verify that both event multicast address and port parameters are set or both parameters are not set.
   * - Verify that there is a port configured, that is not an dynamic port number (0).
   * - Additional checks can be placed within this static member function.
   * \param[in] machine_mapping The 'machine mapping' element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if IpAddressElement is not set
   *   - Set the validation result to no IP address is configured
   * - Otherwise, check if the either UDP or TCP port is not set
   *   - Set the validation result to no port configured for (at least) one of TCP or UDP
   * - Otherwise, check if the either UDP or TCP port is dynamic
   *   - Set the validation result to dynamic port is not supported.
   * - Otherwise, check if EventMulticastAddressElement and EventMulticastPortElement are set consistently
   *   - Set the validation result to inconsistent configuration of event multicast address and port
   * - Otherwise, check if secure ids are configured
   *   - Set the validation result to not supported secure ids
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(MachineMappingConfigObject const& machine_mapping) noexcept {
    ValidationResult result{};
    if (machine_mapping.address.GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingIpAddress;
    } else if ((machine_mapping.udp_port.GetStatus() != ElementStatus::kSet) &&
               (machine_mapping.tcp_port.GetStatus() != ElementStatus::kSet)) {
      result = ValidationResult::kMissingPort;
    } else if (((machine_mapping.udp_port.GetElement().has_value()) &&
                (machine_mapping.udp_port.GetElement().value() == 0)) ||
               ((machine_mapping.tcp_port.GetElement().has_value()) &&
                (machine_mapping.tcp_port.GetElement().value() == 0))) {
      result = ValidationResult::kNotSupportedDynamicPorts;
    } else if (machine_mapping.event_multicast_address.GetStatus() !=
               machine_mapping.event_multicast_port.GetStatus()) {
      result = ValidationResult::kInconsistentEventMulticastAddressAndPort;
    } else if (machine_mapping.secure_connection_ids.GetStatus() == ElementStatus::kSet) {
      result = ValidationResult::kNotSupportedSecureIds;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_MACHINE_MAPPING_VALIDATOR_H_

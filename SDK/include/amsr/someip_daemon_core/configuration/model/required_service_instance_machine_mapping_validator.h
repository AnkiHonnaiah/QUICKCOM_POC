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
/*!        \file  required_service_instance_machine_mapping_validator.h
 *        \brief  Validates Configuration object 'RequiredServiceInstanceMachineMapping'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_MACHINE_MAPPING_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_MACHINE_MAPPING_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/machine_mapping_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'machine mapping' elements of 'required service instances'.
 */
class RequiredServiceInstanceMachineMappingValidator final {
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
     * \brief Unexpected event multicast address configured.
     */
    kUnexpectedEventMulticastAddress,

    /*!
     * \brief Unexpected event multicast port configured.
     */
    kUnexpectedEventMulticastPort,
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

      case ValidationResult::kUnexpectedEventMulticastAddress:
        ret = "Unexpected event multicast address";
        break;

      case ValidationResult::kUnexpectedEventMulticastPort:
        ret = "Unexpected event multicast port";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Since the event multicast address and port is only permitted for provided instances, make sure that it isn't set
   *   for required instances.
   * \param[in] machine_mapping The 'machine mapping' element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if EventMulticastAddressElement is set
   *   - Set the validation result to unexpected event multicast address configured
   * - Otherwise, check if EventMulticastPortElement is set
   *   - Set the validation result to unexpected event multicast port configured
   * - Otherwise set the validation result to ok.
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(MachineMappingConfigObject const& machine_mapping) noexcept {
    ValidationResult result{};

    if (machine_mapping.event_multicast_address.GetStatus() == ElementStatus::kSet) {
      result = ValidationResult::kUnexpectedEventMulticastAddress;
    } else if (machine_mapping.event_multicast_port.GetStatus() == ElementStatus::kSet) {
      result = ValidationResult::kUnexpectedEventMulticastPort;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_MACHINE_MAPPING_VALIDATOR_H_

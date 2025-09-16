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
/*!        \file  required_service_instance_service_discovery_validator.h
 *        \brief  Validates Configuration object 'RequiredServiceInstanceServiceDiscovery'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_service_discovery_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'service discovery' elements of 'required service instances'.
 */
class RequiredServiceInstanceServiceDiscoveryValidator final {
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
     * \brief No TTL configured or TTL value not > 0.
     */
    kMissingOrInvalidTtl,

    /*!
     * \brief initial_delay_min_ns value not greater than or equal to 0.
     */
    kInvalidInitialDelayMin,

    /*!
     * \brief initial_delay_max_ns value not greater than or equal to 0.
     */
    kInvalidInitialDelayMax,

    /*!
     * \brief initial_repetitions_base_delay_ns value not greater than or equal 0.
     */
    kInvalidInitialRepetitionsBaseDelay,

    /*!
     * \brief Inconsistent configuration of "initial_delay_min_ns", "initial_delay_max_ns", "initial_repetitions_max"
     * and "initial_repetitions_base_delay". If service discovery delay is configured, "initial_delay_min_ns" and
     * "initial_delay_max_ns" must be set.
     */
    kInconsistentServiceDiscoveryDelay,

    /*!
     * \brief No event groups configured.
     */
    kMissingEventGroups
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

      case ValidationResult::kMissingOrInvalidTtl:
        ret = "No TTL configured or is not greater than zero";
        break;

      case ValidationResult::kInvalidInitialDelayMin:
        ret = "Minimum initial delay is not greater than or equal to zero";
        break;

      case ValidationResult::kInvalidInitialDelayMax:
        ret = "Maximum initial delay is not greater than or equal to zero";
        break;

      case ValidationResult::kInvalidInitialRepetitionsBaseDelay:
        ret = "Initial repetitions' base delay is not greater than or equal to zero";
        break;

      case ValidationResult::kInconsistentServiceDiscoveryDelay:
        ret = "Both minimum and maximum initial delays shall be set when initial service discovery delay is configured";
        break;

      case ValidationResult::kMissingEventGroups:
        ret = "Event groups are missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the TTL is set and value is > 0.
   * - Verify that the event groups are set.
   * - Verify that the initial_repetitions_base_delay_ns is greater or equal 0.
   * - Verify that the initial_delay_min_ns is greater or equal 0.
   * - Verify that the initial_delay_max_ns is greater or equal 0.
   * - Verify that the service discovery delay is consistent
   * - Additional checks can be placed within this static member function.
   * \param[in] event_group The 'required service instance service discovery event group' element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if the TtlElement is not set or is less or equal to 0
   *   - Set the validation result to no or invalid Ttl configured
   * - Otherwise, check if EventgroupsElement is not set
   *   - Set the validation result to no event group configured
   * - Otherwise, check if InitialRepetitionsBaseDelayNsElement is less than 0
   *   - Set the validation result to invalid InitialRepetitionsBaseDelayNsElement configured
   * - Otherwise, check if InitialDelayMinElement is less than 0
   *   - Set the validation result to invalid InitialDelayMinElement configured
   * - Otherwise, check if InitialDelayMaxElement is less than 0
   *   - Set the validation result to invalid InitialDelayMaxElement configured
   * - Otherwise, check if the service discovery delay is consistent
   *   - Set the validation result to inconsistent service discovery delay
   * \endinternal
   */
  static ValidationResult Check(RequiredServiceInstanceServiceDiscoveryObject const& service_discovery) noexcept {
    ValidationResult result{};

    if ((service_discovery.GetTtl().GetStatus() != ElementStatus::kSet) ||
        (service_discovery.GetTtl().GetElement() <= 0U)) {
      result = ValidationResult::kMissingOrInvalidTtl;
    } else if (service_discovery.GetEventGroups().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingEventGroups;
    } else if ((service_discovery.GetInitialRepetitionsBaseDelay().GetStatus() == ElementStatus::kSet) &&
               (service_discovery.GetInitialRepetitionsBaseDelay().GetElement() < std::chrono::nanoseconds(0U))) {
      result = ValidationResult::kInvalidInitialRepetitionsBaseDelay;
    } else if ((service_discovery.GetInitialDelayMin().GetStatus() == ElementStatus::kSet) &&
               (service_discovery.GetInitialDelayMin().GetElement() < std::chrono::nanoseconds(0U))) {
      result = ValidationResult::kInvalidInitialDelayMin;
    } else if ((service_discovery.GetInitialDelayMax().GetStatus() == ElementStatus::kSet) &&
               (service_discovery.GetInitialDelayMax().GetElement() < std::chrono::nanoseconds(0U))) {
      result = ValidationResult::kInvalidInitialDelayMax;
    } else if (((service_discovery.GetInitialDelayMin().GetStatus() == ElementStatus::kSet) ||
                (service_discovery.GetInitialDelayMax().GetStatus() == ElementStatus::kSet) ||
                (service_discovery.GetInitialRepetitionsMax().GetStatus() == ElementStatus::kSet) ||
                (service_discovery.GetInitialRepetitionsBaseDelay().GetStatus() == ElementStatus::kSet))

               && ((service_discovery.GetInitialDelayMin().GetStatus() != ElementStatus::kSet) ||
                   (service_discovery.GetInitialDelayMax().GetStatus() != ElementStatus::kSet))) {
      result = ValidationResult::kInconsistentServiceDiscoveryDelay;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_VALIDATOR_H_

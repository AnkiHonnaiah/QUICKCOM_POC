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
/*!        \file  provided_service_instance_service_discovery_event_group_validator.h
 *        \brief  Validator for event groups of provided service instance service discovery.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_service_discovery_event_group_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'service discovery event group' elements of 'provided service instances'.
 */
class ProvidedServiceInstanceServiceDiscoveryEventGroupValidator final {
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
     * \brief No event group ID configured.
     */
    kMissingEventGroupId,

    /*!
     * \brief Inconsistent configuration of "request_response_delay_min_ns" and
     *        "request_response_delay_max_ns". Both parameters must configured
     *         or both parameters must be removed.
     */
    kInconsistentRequestResponseDelay,

    /*!
     * \brief request_response_delay_min not greater or equal 0.
     */
    kInvalidRequestResponseDelayMin,

    /*!
     * \brief request_response_delay_max not greater or equal 0.
     */
    kInvalidRequestResponseDelayMax,

    /*!
     * \brief request_response_delay_max not greater or equal than request_response_delay_min.
     */
    kRequestResponseDelayMaxNotGreaterOrEqualMin,
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

      case ValidationResult::kMissingEventGroupId:
        ret = "Event group ID is missing";
        break;

      case ValidationResult::kInconsistentRequestResponseDelay:
        ret = "Both minimum and maximum request response delays shall either be set or unset";
        break;

      case ValidationResult::kInvalidRequestResponseDelayMin:
        ret = "Minimum request response delay is not greater than or equal to zero";
        break;

      case ValidationResult::kInvalidRequestResponseDelayMax:
        ret = "Maximum request response delay is not greater than or equal to zero";
        break;

      case ValidationResult::kRequestResponseDelayMaxNotGreaterOrEqualMin:
        ret = "Maximum request response delay is not greater than or equal to the minimum";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the event group ID is set.
   * - Verify that 'request_response_delay_min' and 'request_response_delay_max' are greater or equal 0.
   * - Verify that 'request_response_delay_max' >= 'request_response_delay_min'.
   * - Verify that request response delay configuration is consistent.
   * - Additional checks can be placed within this static member function.
   * \param[in] event_group The 'provided service instance service discovery event group' element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if the IdElement is not set
   *   - Set the validation result to no Event Multicast threshold configured.
   * - Otherwise, check if RequestResponseDelayMinElement is set and less than 0 nanoseconds
   *   - Set the validation result to invalid RequestResponseDelayMin
   * - Otherwise, check if RequestResponseDelayMaxElement is set and less than 0 nanoseconds
   *   - Set the validation result to invalid RequestResponseDelayMax
   * - Otherwise, check if RequestResponseDelayMaxElement is set and less than RequestResponseDelayMinElement
   *   - Set the validation result to RequestResponseDelayMaxNsElement is not greater or equal to
   *   RequestResponseDelayMinElement
   * - Otherwise, check if RequestResponseDelayMinNsElement is not consistent with RequestResponseDelayMaxNsElement
   *   - Set the validation result to inconsistent configuration
   * - Otherwise, set validation result to ok
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(
      ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject const& event_group) noexcept {
    ValidationResult result{};

    if (event_group.GetId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingEventGroupId;
    } else if ((event_group.GetRequestResponseDelayMinNs().GetStatus() == ElementStatus::kSet) &&
               (event_group.GetRequestResponseDelayMinNs().GetElement() < std::chrono::nanoseconds(0U))) {
      result = ValidationResult::kInvalidRequestResponseDelayMin;
    } else if ((event_group.GetRequestResponseDelayMaxNs().GetStatus() == ElementStatus::kSet) &&
               (event_group.GetRequestResponseDelayMaxNs().GetElement() < std::chrono::nanoseconds(0U))) {
      result = ValidationResult::kInvalidRequestResponseDelayMax;
    } else if (((event_group.GetRequestResponseDelayMinNs().GetStatus() == ElementStatus::kSet) &&
                (event_group.GetRequestResponseDelayMaxNs().GetStatus() == ElementStatus::kSet)) &&
               (event_group.GetRequestResponseDelayMaxNs().GetElement() <
                event_group.GetRequestResponseDelayMinNs().GetElement())) {
      result = ValidationResult::kRequestResponseDelayMaxNotGreaterOrEqualMin;
    } else if (event_group.GetRequestResponseDelayMaxNs().GetStatus() !=
               event_group.GetRequestResponseDelayMinNs().GetStatus()) {
      result = ValidationResult::kInconsistentRequestResponseDelay;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_VALIDATOR_H_

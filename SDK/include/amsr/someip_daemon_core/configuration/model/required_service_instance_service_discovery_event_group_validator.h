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
/*!        \file  required_service_instance_service_discovery_event_group_validator.h
 *        \brief  Validates Configuration object 'RequiredServiceInstanceServiceDiscoveryEventGroup'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_service_discovery_event_group_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'service discovery event group' elements of 'required service instances'.
 */
class RequiredServiceInstanceServiceDiscoveryEventGroupValidator final {
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
     * \brief No TTL configured or TTL value not > 0.
     */
    kMissingOrInvalidTtl,

    /*!
     * \brief Inconsistent configuration of request_response_delay_min_ns and
     *        request_response_delay_max_ns. Both parameters must configured
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

    /*!
     * \brief SubscribeRetry configuration partially missing or incorrect.
     */
    kMissingOrInvalidSubscribeRetry
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
        ret = "No event group ID";
        break;

      case ValidationResult::kMissingOrInvalidTtl:
        ret = "No TTL configured or is not greater than zero";
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

      case ValidationResult::kMissingOrInvalidSubscribeRetry:
        ret = "SubscribeRetry configuration partially missing or incorrect";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the event group is set.
   * - Verify that the TTL is set and value is > 0.
   * - Verify that 'request_response_delay_min' and 'request_response_delay_max' are greater or equal 0.
   * - Verify that 'request_response_delay_max' >= 'request_response_delay_min'.
   * - Verify that request response delay configuration is consistent.
   * - Verify that eventgroup retry configuration is consistent
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
   * - Check if EventgroupIdElement is not set
   *   - Set the validation result to no event group ID configured
   * - Otherwise, check if TtlElement is not set or it is less than or equal to 0
   *   - Set the validation result to no TTL configured or TTL value not > 0
   * - Otherwise, check if RequestResponseDelayMinElement is set and less than 0 nanoseconds
   *   - Set the validation result to RequestResponseDelayMinElement is not greater or equal to 0 nanoseconds
   * - Otherwise, check if RequestResponseDelayMaxElement is set and less than 0 nanoseconds
   *   - Set the validation result to RequestResponseDelayMaxElement is not greater or equal to 0 nanoseconds
   * - Otherwise, check if RequestResponseDelayMaxElement is less than RequestResponseDelayMinElement and that they are
   * both set
   *   - Set the validation result to RequestResponseDelayMaxNsElement is not greater or equal to
   *   RequestResponseDelayMinElement
   * - Otherwise, check if RequestResponseDelayElement is consistent
   *   - Set the validation result to RequestResponseDelayElement is inconsistent
   * - Otherwise, check if SubscribeRetryElement is invalid
   *   - Set the validation result to SubscribeRetryElement is missing or invalid
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(RequiredServiceInstanceServiceDiscoveryEventGroupObject const& event_group) noexcept {
    ValidationResult result{ValidationResult::kOk};

    // Lambda functions for finding "kMissingOrInvalidSubscribeRetry"
    auto const check_subscribe_eventgroup_retry_delay_zero = [&event_group]() -> bool {
      return (event_group.GetSubscribeEventgroupRetryDelay().GetStatus() == ElementStatus::kSet) &&
             (event_group.GetSubscribeEventgroupRetryDelay().GetElement() == std::chrono::seconds(0U));
    };

    auto const check_subscribe_eventgroup_retry_max_not_set_and_delay_set = [&event_group]() -> bool {
      return (event_group.GetSubscribeEventgroupRetryMax().GetStatus() != ElementStatus::kSet) &&
             (event_group.GetSubscribeEventgroupRetryDelay().GetStatus() == ElementStatus::kSet);
    };

    auto const check_subscribe_eventgroup_retry_max_set_zero_and_delay_set = [&event_group]() -> bool {
      return (event_group.GetSubscribeEventgroupRetryMax().GetStatus() == ElementStatus::kSet) &&
             (event_group.GetSubscribeEventgroupRetryMax().GetElement() == 0U) &&
             (event_group.GetSubscribeEventgroupRetryDelay().GetStatus() == ElementStatus::kSet);
    };

    auto const check_subscribe_eventgroup_retry_max_set_greaterthanzero_delay_not_set = [&event_group]() -> bool {
      return (event_group.GetSubscribeEventgroupRetryMax().GetStatus() == ElementStatus::kSet) &&
             (event_group.GetSubscribeEventgroupRetryMax().GetElement() > 0U) &&
             (event_group.GetSubscribeEventgroupRetryDelay().GetStatus() != ElementStatus::kSet);
    };

    // If else chain
    if (event_group.GetEventGroupId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingEventGroupId;
    } else if ((event_group.GetTtl().GetStatus() != ElementStatus::kSet) || (event_group.GetTtl().GetElement() <= 0U)) {
      result = ValidationResult::kMissingOrInvalidTtl;
    } else if ((event_group.GetRequestResponseDelayMin().GetStatus() == ElementStatus::kSet) &&
               (event_group.GetRequestResponseDelayMin().GetElement() < std::chrono::nanoseconds(0U))) {
      result = ValidationResult::kInvalidRequestResponseDelayMin;
    } else if ((event_group.GetRequestResponseDelayMax().GetStatus() == ElementStatus::kSet) &&
               (event_group.GetRequestResponseDelayMax().GetElement() < std::chrono::nanoseconds(0U))) {
      result = ValidationResult::kInvalidRequestResponseDelayMax;
    } else if ((event_group.GetRequestResponseDelayMin().GetStatus() == ElementStatus::kSet) &&
               (event_group.GetRequestResponseDelayMax().GetStatus() == ElementStatus::kSet) &&
               (event_group.GetRequestResponseDelayMax().GetElement() <
                event_group.GetRequestResponseDelayMin().GetElement())) {
      result = ValidationResult::kRequestResponseDelayMaxNotGreaterOrEqualMin;
    } else if (event_group.GetRequestResponseDelayMax().GetStatus() !=
               event_group.GetRequestResponseDelayMin().GetStatus()) {
      result = ValidationResult::kInconsistentRequestResponseDelay;
    } else if (check_subscribe_eventgroup_retry_delay_zero() ||
               check_subscribe_eventgroup_retry_max_not_set_and_delay_set() ||
               check_subscribe_eventgroup_retry_max_set_zero_and_delay_set() ||
               check_subscribe_eventgroup_retry_max_set_greaterthanzero_delay_not_set()) {
      result = ValidationResult::kMissingOrInvalidSubscribeRetry;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_VALIDATOR_H_

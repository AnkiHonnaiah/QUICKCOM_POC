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
/*!        \file  provided_service_instance_service_discovery_validator.h
 *        \brief  Validator for provided service instance service discovery.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_service_discovery_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'service discovery' elements of 'provided service instances'.
 */
class ProvidedServiceInstanceServiceDiscoveryValidator final {
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
     * \brief No TTL configured.
     */
    kMissingOrInvalidTtl,

    /*!
     * \brief Inconsistent configuration of "initial_delay_min_ns", "initial_delay_max_ns", "initial_repetitions_max"
     * and "initial_repetitions_base_delay". If service discovery delay is configured, "initial_delay_min_ns" and
     * "initial_delay_max_ns" must be set.
     */
    kInconsistentServiceDiscoveryDelay,

    /*!
     * \brief Inconsistent configuration of "request_response_delay_min_ns" and
     *        "request_response_delay_max_ns". Both parameters must configured
     *         or both parameters must be removed.
     */
    kInconsistentRequestResponseDelay,

    /*!
     * \brief request_response_delay_max not greater or equal than request_response_delay_min.
     */
    kRequestResponseDelayMaxNotGreaterOrEqualMin,

    /*!
     * \brief No event groups configured.
     */
    kMissingEventGroups,

    /*!
     * \brief cyclic_offer_delay_ns is 0 or not configured and TTL is configured with any value other than 0xFFFFFF.
     */
    kTtlNotCompatibleWithOfferCyclicDelay
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
  static auto ValidationResultToString(ValidationResult validation_result) -> char const* {
    char const* ret{nullptr};
    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;

      case ValidationResult::kMissingOrInvalidTtl:
        ret = "TTL is not configured or the value is not greater than zero";
        break;

      case ValidationResult::kInconsistentServiceDiscoveryDelay:
        ret = "Both minimum and maximum initial delays shall be set when initial service discovery delay is configured";
        break;

      case ValidationResult::kInconsistentRequestResponseDelay:
        ret = "Both minimum and maximum request response delays shall either be set or unset";
        break;

      case ValidationResult::kRequestResponseDelayMaxNotGreaterOrEqualMin:
        ret = "Maximum request response delay is not greater than or equal to the minimum";
        break;

      case ValidationResult::kMissingEventGroups:
        ret = "Event groups are missing";
        break;

      case ValidationResult::kTtlNotCompatibleWithOfferCyclicDelay:
        ret = "If offer cyclic delay is set to zero or not present, offer time to live must be set to 0xFFFFFF";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the TTL is set.
   * - Verify that the event groups are set.
   * - Verify that TTL is compatible with offer cyclic delay.
   * - Verify that the service discovery delay is consistent
   * - Verify that request_response_delay_max >= request_response_delay_min.
   * - Verify that the request response delay is consistent
   * - Additional checks can be placed within this static member function.
   * \param[in] service_discovery The 'required service instance service discovery' element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if the TtlElement is not set
   *   - Set the validation result to no or invalid Ttl configured
   * - Otherwise, check if EventgroupsElement is not set
   *   - Set the validation result to no event group configured
   * - Otherwise, check if CyclicOfferDelayNsElement is set to 0 and Ttl is set to 0xFFFFFF
   *   - Set the validation result to set OfferTimeToLive to 0xFFFFFF
   * - Otherwise, check if the service discovery delay is consistent
   *   - Set the validation result to inconsistent service discovery delay
   * - Otherwise, check if RequestResponseDelayMaxElement is less than RequestResponseDelayMinElement
   *   - Set the validation result to RequestResponseDelayMaxNsElement is not greater or equal to
   *   RequestResponseDelayMinElement
   * - Otherwise, check if the request response delay is consistent
   *   - Set the validation result to inconsistent request response delay
   * \endinternal
   */
  // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
  static ValidationResult Check(ProvidedServiceInstanceServiceDiscoveryConfigObject const& service_discovery) noexcept {
    ValidationResult result{};

    if (service_discovery.GetTtl().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingOrInvalidTtl;
    } else if (service_discovery.GetEventgroups().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingEventGroups;
    } else if ((service_discovery.GetCyclicOfferDelayNs().GetStatus() == ElementStatus::kSet) &&
               (service_discovery.GetCyclicOfferDelayNs().GetElement() == std::chrono::nanoseconds(0U)) &&
               (service_discovery.GetTtl().GetStatus() == ElementStatus::kSet) &&
               (service_discovery.GetTtl().GetElement() != ConfigurationTypesAndDefs::kTtlInfinite)) {
      result = ValidationResult::kTtlNotCompatibleWithOfferCyclicDelay;
    } else if (((service_discovery.GetInitialDelayMinNs().GetStatus() == ElementStatus::kSet) ||
                (service_discovery.GetInitialDelayMaxNs().GetStatus() == ElementStatus::kSet) ||
                (service_discovery.GetInitialRepetitionsMax().GetStatus() == ElementStatus::kSet) ||
                (service_discovery.GetInitialRepetitionsBaseDelayNs().GetStatus() == ElementStatus::kSet)) &&
               ((service_discovery.GetInitialDelayMinNs().GetStatus() != ElementStatus::kSet) ||
                (service_discovery.GetInitialDelayMaxNs().GetStatus() != ElementStatus::kSet))) {
      result = ValidationResult::kInconsistentServiceDiscoveryDelay;
    } else if ((service_discovery.GetRequestResponseDelayMinNs().GetStatus() == ElementStatus::kSet) &&
               (service_discovery.GetRequestResponseDelayMaxNs().GetStatus() == ElementStatus::kSet) &&
               (service_discovery.GetRequestResponseDelayMaxNs().GetElement() <
                service_discovery.GetRequestResponseDelayMinNs().GetElement())) {
      result = ValidationResult::kRequestResponseDelayMaxNotGreaterOrEqualMin;
    } else if (service_discovery.GetRequestResponseDelayMaxNs().GetStatus() !=
               service_discovery.GetRequestResponseDelayMinNs().GetStatus()) {
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_VALIDATOR_H_

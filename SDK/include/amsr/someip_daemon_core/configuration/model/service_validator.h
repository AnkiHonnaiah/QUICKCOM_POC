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
/*!        \file  service_validator.h
 *        \brief  Validates Configuration object 'Service'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SERVICE_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SERVICE_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/intermediate_service_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for service elements.
 */
class ServiceValidator final {
 public:
  /*!
   * \brief Validation results for one service elements.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The service object is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief The service object lacks a service ID.
     */
    kMissingServiceId,

    /*!
     * \brief The service object lacks a major version.
     */
    kMissingMajorVersion,

    /*!
     * \brief The service object lacks a minor version.
     */
    kMissingMinorVersion
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

      case ValidationResult::kMissingMajorVersion:
        ret = "Major version is missing";
        break;

      case ValidationResult::kMissingMinorVersion:
        ret = "Minor version is missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this service parsed previously is valid.
   * \details
   * - Verify that the service ID is set (mandatory).
   * - Verify that the service has a major version (mandatory).
   * - Verify that the service has a minor version (mandatory).
   * - Additional checks can be placed within this static member function.
   * \param[in] service_element The service element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if either of ServiceIdElement, MajorVersionElement, MinorVersionElement is not set
   *   - Set the validation result to corresponding configuration element is not configured
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(configuration::model::ServiceConfigObject const& service_element) noexcept {
    ValidationResult result{};

    if (service_element.GetServiceId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingServiceId;
    } else if (service_element.GetMajorVersion().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingMajorVersion;
    } else if (service_element.GetMinorVersion().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingMinorVersion;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SERVICE_VALIDATOR_H_

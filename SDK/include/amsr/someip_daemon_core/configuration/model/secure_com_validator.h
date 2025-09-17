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
/*!        \file  secure_com_validator.h
 *        \brief  Validates Configuration object 'SecureCom'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {
/*!
 * \brief Groups consistency checks for secure channel elements.
 */
class SecureComValidator final {
 public:
  /*!
   * \brief Validation results for one secure com object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The secure channel is valid
     */
    kOk = 0x00U,

    /*!
     * \brief The secure com object lacks a PSK identity hint for the machine
     */
    kMissingMachinePskIdentityHint,

    /*!
     * \brief The secure com object lacks secure_endpoints
     */
    kMissingSecureEndpoints,

    /*!
     * \brief The secure com object can either be configured with SecureEndpoints or SecureConnections
     */
    kEitherSecureConnectionsOrSecureEndpoints
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

      case ValidationResult::kMissingMachinePskIdentityHint:
        ret = "PSK identity hint for the machine is missing";
        break;

      case ValidationResult::kMissingSecureEndpoints:
        ret = "Secure endpoints are missing";
        break;

      case ValidationResult::kEitherSecureConnectionsOrSecureEndpoints:
        ret = "Secure communication can only be configured either with SecureEndpoints or SecureConnections";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if the latest parsed secure com configuration object is valid.
   * \details Verify that either SecureConnection is present or both secure_endpoints and PSK identity. It is mandatory
   * that one of the two cases are true.
   * \param[in] secure_com_object The secure com object to verify.
   * \return validation_result ValidationResult::kOk if the secure com object has all mandatory elements set, otherwise
   * other element in enum ValidationResult reflecting correct status
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if SecureConnections and at least one of both PskIdentityHint and SecureEndpoints options are configured
   *   - Set the validation result to secure com object can be configured either with SecureConnections or
   * SecureEndpoints
   * - Otherwise
   *   - Check if PskIdentityElement is not set
   *     - Set the validation result to secure com object lacks a PSK identity hint for the machine
   *   - Otherwise, check if SecureEndpointsElement
   *     - Set the validation result to secure com object lacks end point
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(SecureComConfigObject const& secure_com_object) noexcept {
    ValidationResult result{};
    if ((secure_com_object.GetSecureConnections().GetStatus() == ElementStatus::kSet) &&
        ((secure_com_object.GetPskIdentityHint().GetStatus() == ElementStatus::kSet) ||
         (secure_com_object.GetSecureEndpoints().GetStatus() == ElementStatus::kSet))) {
      result = ValidationResult::kEitherSecureConnectionsOrSecureEndpoints;
    } else if (secure_com_object.GetSecureConnections().GetStatus() != ElementStatus::kSet) {
      if (secure_com_object.GetPskIdentityHint().GetStatus() != ElementStatus::kSet) {
        result = ValidationResult::kMissingMachinePskIdentityHint;
      } else if (secure_com_object.GetSecureEndpoints().GetStatus() != ElementStatus::kSet) {
        result = ValidationResult::kMissingSecureEndpoints;
      } else {
        result = ValidationResult::kOk;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_VALIDATOR_H_

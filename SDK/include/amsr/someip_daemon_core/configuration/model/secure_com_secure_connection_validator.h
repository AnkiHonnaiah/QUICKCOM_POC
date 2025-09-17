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
/*!        \file  secure_com_secure_connection_validator.h
 *        \brief  Validates Configuration object 'SecureComSecureConnection'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {
/*!
 * \brief Groups consistency checks for 'secure connection' elements of 'secure com'.
 */
class SecureComSecureConnectionValidator final {
 public:
  /*!
   * \brief Validation results for one secure connection object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The secure connection object is valid
     */
    kOk = 0x00U,

    /*!
     * \brief The secure connection lacks secure id
     */
    kMissingSecureId,

    /*!
     * \brief The secure connection lacks TLS version
     */
    kMissingTlsVersion,

    /*!
     * \brief The secure connection lacks cipher suite ID
     */
    kMissingCipherSuiteId,

    /*!
     * \brief The secure connection lacks priority
     */
    kMissingPriority,

    /*!
     * \brief The secure connection lacks the configuration object. One of client PSK configuration, server PSK
     * configuration or certification configuration must be set.
     */
    kMissingConfiguration
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

      case ValidationResult::kMissingSecureId:
        ret = "Missing secure ID";
        break;

      case ValidationResult::kMissingTlsVersion:
        ret = "Missing TLS version";
        break;

      case ValidationResult::kMissingCipherSuiteId:
        ret = "Missing cipher suite ID";
        break;

      case ValidationResult::kMissingPriority:
        ret = "Missing priority";
        break;

      case ValidationResult::kMissingConfiguration:
        ret =
            "The configuration is missing. One of client PSK configuration, server PSK configuration or certification "
            "configuration must be configured.";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if the latest parsed secure connection is valid.
   * \details
   * \param[in] secure_endpoint The secure connection object to verify.
   * \return ValidationResult::kOk if the secure connection object has all mandatory elements set, otherwise other
   * element in enum ValidationResult reflecting correct status.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if SecureId is not set
   *    - Set result to kMissingSecureId
   * - Otherwise, check if Tls version is not set
   *    - Set result to kMissingTlsVersion
   * - Otherwise, check if cipher suite ID is not set
   *    - Set kMissingCipherSuiteId
   * - Otherwise, check if priority is not set
   *    - Set kMissingPriority
   * -Otherwise, check if no Psk and no certificate configuration is set
   *    - Set kMissingConfiguration
   * Otherwise,
   *    Set result to kOk
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(SecureComSecureConnectionObject const& secure_connection) noexcept {
    ValidationResult result{};

    if (secure_connection.GetSecureId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingSecureId;
    } else if (secure_connection.GetTlsVersion().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingTlsVersion;
    } else if (secure_connection.GetCipherSuiteId().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingCipherSuiteId;
    } else if (secure_connection.GetPriority().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPriority;
    } else if ((secure_connection.GetPskConfig().GetStatus() != ElementStatus::kSet) &&
               (secure_connection.GetCertConfig().GetStatus() != ElementStatus::kSet)) {
      result = ValidationResult::kMissingConfiguration;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_VALIDATOR_H_

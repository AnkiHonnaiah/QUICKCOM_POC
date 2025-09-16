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
/*!        \file  secure_com_secure_connection_cert_config_validator.h
 *        \brief  Validates Configuration object 'SecureComSecureConnectionCertConfig'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_cert_config_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for 'cert config' elements of 'secure connections'.
 */
class SecureComSecureConnectionCertConfigValidator final {
 public:
  /*!
   * \brief Validation results for one secure connection certificate configuration object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The secure connection certificate configuration object is valid
     */
    kOk = 0x00U,

    /*!
     * \brief The secure connection certificate configuration lacks private key UUID
     */
    kMissingPrivateKeyUuid,

    /*!
     * \brief The secure connection certificate configuration lacks certification chain label list
     */
    kMissingCertChainLabelList
  };

  /*!
   * \brief Translates the validation result to string for logging.
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

      case ValidationResult::kMissingPrivateKeyUuid:
        ret = "Missing private key UUID";
        break;

      case ValidationResult::kMissingCertChainLabelList:
        ret = "Missing certificate chain label list";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if the latest parsed secure connection certificate configuration is valid.
   * \param[in] cert_config The secure connection certificate configuration object to verify.
   * \return ValidationResult::kOk if the secure connection certificate configuration object has all mandatory elements
   * set, otherwise other element in enum ValidationResult reflecting correct status.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if private key UUID is not set
   *    - Set result to kMissingPrivateKeyUuid
   * - Check if certificate chain label list is not set
   *    - Set result to kMissingCertChainLabelList
   * - Otherwise,
   *    - Set result to kOk
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(SecureComSecureConnectionCertConfigObject const& cert_config) noexcept {
    ValidationResult result{};

    if (cert_config.GetPrivateKeyUuid().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPrivateKeyUuid;
    } else if (cert_config.GetCertificateChainLabelList().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingCertChainLabelList;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_VALIDATOR_H_

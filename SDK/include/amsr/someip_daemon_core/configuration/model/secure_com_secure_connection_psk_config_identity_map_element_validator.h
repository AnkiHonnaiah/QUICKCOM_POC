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
/*!        \file  secure_com_secure_connection_psk_config_identity_map_element_validator.h
 *        \brief  Validates Configuration object 'SecureComSecureConnectionPskConfigIdentityMapElement'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENT_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENT_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_psk_config_identity_map_element_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {
/*!
 * \brief Groups consistency checks for 'psk config identity map' element of a 'secure connection'.
 */
class SecureComSecureConnectionPskConfigIdentityMapElementValidator final {
 public:
  /*!
   * \brief Validation results for one secure connection psk config identity map element object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The psk identity map element is valid
     */
    kOk = 0x00U,

    /*!
     * \brief The psk identity map element lacks psk identity
     */
    kMissingPskIdentity,

    /*!
     * \brief The psk identity map element lacks psk UUID
     */
    kMissingPskUuid
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

      case ValidationResult::kMissingPskIdentity:
        ret = "Missing PSK identity";
        break;

      case ValidationResult::kMissingPskUuid:
        ret = "Missing PSK UUID";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if the latest parsed secure connection psk configuration identity map element is valid.
   * \param[in] identity_map_element The secure connection psk configuration identity map element object to verify.
   * \return ValidationResult::kOk if the identity map element object has all mandatory elements set, otherwise other
   * element in enum ValidationResult reflecting correct status.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result
   * - Check if Psk identity is not set
   *    - Set result to kMissingPskIdentity
   * - Otherwise, check if Psk UUID is not set
   *    - Set result to kMissingPskUuid
   * -Otherwise,
   *    - Set result to kOk
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(
      SecureComSecureConnectionPskConfigIdentityMapElementObject const& identity_map_element) noexcept {
    ValidationResult result{};

    if (identity_map_element.GetPskIdentity().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPskIdentity;
    } else if (identity_map_element.GetPskUuid().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPskUuid;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENT_VALIDATOR_H_

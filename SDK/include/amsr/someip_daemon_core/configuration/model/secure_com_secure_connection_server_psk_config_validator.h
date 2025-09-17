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
/*!        \file  secure_com_secure_connection_server_psk_config_validator.h
 *        \brief  Validates server-side Configuration object 'SecureComSecureConnectionPskConfig'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_SERVER_PSK_CONFIG_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_SERVER_PSK_CONFIG_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_psk_config_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {
/*!
 * \brief Groups consistency checks for server-side 'psk config' element of a 'secure connection'.
 */
class SecureComSecureConnectionServerPskConfigValidator final {
 public:
  /*!
   * \brief Validation results for one server-side PSK configuration object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The server-side secure connection psk config is valid
     */
    kOk = 0x00U,

    /*!
     * \brief The server-side secure connection psk config lacks psk identity map
     */
    kMissingPskIdentityMap,

    /*!
     * \brief The server-side secure connection psk config has an empty psk identity map
     */
    kEmptyPskIdentityMap,

    /*!
     * \brief The server-side secure connection psk config lacks psk identity hint.
     */
    kMissingPskIdentityHint,

    /*!
     * \brief It is invalid to set psk_identity_hint in psk identity map element for the server-side secure connection
     * psk config.
     */
    kInvalidPskIdentityHint
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

      case ValidationResult::kMissingPskIdentityMap:
        ret = "Missing PSK identity map";
        break;

      case ValidationResult::kEmptyPskIdentityMap:
        ret = "Empty PSK identity map";
        break;

      case ValidationResult::kMissingPskIdentityHint:
        ret = "Missing PSK identity hint";
        break;

      case ValidationResult::kInvalidPskIdentityHint:
        ret = "Invalid to set 'psk_identity_hint' in 'psk_identity_map' in server PSK config";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if no entries in the PSK identity map have a PSK identity hint set.
   * \param[in] psk_config The secure connection psk config object to verify.
   * \return true, if no entry has PSK identity hint set, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the result to false
   * - Iterate over all map entries
   *    - Check if PSK identity hint is set
   *      - Set result to false
   *      - Stop iteration
   *    - Otherwise,
   *      - Set result to true
   * - return the result
   * \endinternal
   */
  static auto NoPskIdHintSetInPskIdMap(SecureComSecureConnectionPskConfigObject const& psk_config) -> bool {
    bool result{false};
    for (ConfigurationTypesAndDefs::PskIdentityMapElement const& identity_map_element :
         psk_config.GetPskIdentityMap().GetElement()) {
      if (identity_map_element.psk_identity_hint.has_value()) {
        result = false;
        break;
      }
      result = true;
    }
    return result;
  }

  /*!
   * \brief Check if the latest parsed secure connection psk config is valid.
   * \details
   * \param[in] psk_config The server-side secure connection psk config object to verify.
   * \return ValidationResult::kOk if The server-side secure connection psk config object has all mandatory elements
   * set, otherwise other element in enum ValidationResult reflecting correct status.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if a psk_identity_hint is set inside a psk identity map element, store the result
   * - Check if psk identity map is not set
   *    - Set result to kMissingPskIdentityMap
   * - Check if psk identity map is empty
   *    - Set result to kEmptyPskIdentityMap
   * - Check if PskIdentityHint is not set
   *    - Set result to kMissingPskIdentityHint
   * - Check if at least one psk_identity_hint is set inside a psk identity map element
   *    - Set result to kInvalidPskIdentityHint
   * - return the validation result
   * \endinternal
   */
  static auto Check(SecureComSecureConnectionPskConfigObject const& psk_config) noexcept -> ValidationResult {
    ValidationResult result{};

    if (psk_config.GetPskIdentityMap().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPskIdentityMap;
    } else if (psk_config.GetPskIdentityMap().GetElement().empty()) {
      result = ValidationResult::kEmptyPskIdentityMap;
    } else if (psk_config.GetPskIdentityHint().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPskIdentityHint;
    } else if (!NoPskIdHintSetInPskIdMap(psk_config)) {
      result = ValidationResult::kInvalidPskIdentityHint;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_SERVER_PSK_CONFIG_VALIDATOR_H_

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
/*!        \file  someipd_posix_json_validator.h
 *        \brief  Validates Configuration object 'SomeIpdPosixJson'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIPD_POSIX_JSON_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIPD_POSIX_JSON_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/someipd_posix_json_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {
/*!
 * \brief Groups consistency checks for someipd posix json info.
 */
class SomeIpdPosixJsonValidator final {
 public:
  /*!
   * \brief Validation results for the someipd posix JSON info object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The someipd posix json info is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief The applications container is missing or it is not in correct format: Must be 'applications': []
     */
    kMissingApplications
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

      case ValidationResult::kMissingApplications:
        ret = "Applications are missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this someipd posix json parsed previously is valid.
   * \details
   * - Verify that an applications container exists.
   * - Additional checks can be placed within this static member function.
   * \param[in] someipd_posix The 'someipd posix' root element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if ApplicationContainerElement is not set
   *   - Set the validation result to applications container is missing or it is not in correct format
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(SomeIpdPosixJsonObject const& someipd_posix) noexcept {
    ValidationResult result{};

    if (someipd_posix.GetApplicationContainer().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingApplications;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIPD_POSIX_JSON_VALIDATOR_H_

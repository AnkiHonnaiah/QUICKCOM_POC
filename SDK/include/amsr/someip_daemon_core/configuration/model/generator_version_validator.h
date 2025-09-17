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
/*!        \file  generator_version_validator.h
 *        \brief  Validates config object for 'generatorVersion'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_GENERATOR_VERSION_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_GENERATOR_VERSION_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/generator_version_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for generator version element.
 */
class GeneratorVersionValidator {
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
     * \brief No name configured.
     */
    kMissingName,

    /*!
     * \brief No commit ID configured.
     */
    kMissingCommitId
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

      case ValidationResult::kMissingName:
        ret = "Name is missing";
        break;

      case ValidationResult::kMissingCommitId:
        ret = "Commit ID is missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the name is set.
   * - Verify that the commit Id is set.
   * \param[in] generator_version The generator version element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if name element for GeneratorVersionConfigObject is not set or the name element is empty
   *   - Set the validation result to no name is configured
   * - Otherwise, check if the commit ID for GeneratorVersionConfigObject is not set or it is empty
   *   - Set the validation result to commit ID is not configured
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(GeneratorVersionConfigObject const& generator_version) noexcept {
    ValidationResult result{};
    bool const name_set{generator_version.GetName().GetStatus() == ElementStatus::kSet};
    bool const name_empty{name_set && generator_version.GetName().GetElement().empty()};
    bool const commit_id_set{generator_version.GetCommitId().GetStatus() == ElementStatus::kSet};
    bool const commit_id_empty{commit_id_set && generator_version.GetCommitId().GetElement().empty()};

    if ((!name_set) || name_empty) {
      result = ValidationResult::kMissingName;
    } else if ((!commit_id_set) || commit_id_empty) {
      result = ValidationResult::kMissingCommitId;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_GENERATOR_VERSION_VALIDATOR_H_

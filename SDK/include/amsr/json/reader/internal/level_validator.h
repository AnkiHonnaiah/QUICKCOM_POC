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
/**       \file     level_validator.h
 *        \brief    Allows semi-automatic checks for single object and single array parsers.
 *
 *        \details  Keeps track of the nesting depth of structures and allows only one level of depth.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_LEVEL_VALIDATOR_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_LEVEL_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/json_error_domain.h"

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief A validator for structure elements.
 *
 * \details -
 */
class LevelValidator {
 public:
  /*!
   * \brief Constructs a new Level Validator object.
   * \param object_already_open If true the validator assumes that the object is already open.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit LevelValidator(bool object_already_open = false) noexcept : entered_{object_already_open} {}

  /*!
   * \brief Tries to enter a structure.
   * \return kRunning if no structure has been entered yet, or the error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if already inside a structure}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If not inside a structure:
   *   - Set the signal that a structure has been entered.
   *   - Return kRunning.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto Enter() noexcept -> ParserResult {
    ParserResult result{ParserState::kRunning};

    if (!entered_) {
      entered_ = true;
    } else {
      // VCA_VAJSON_WITHIN_SPEC
      result.EmplaceError(JsonErrc::kUserValidationFailed, "Did not expect nested elements");
    }

    return result;
  }

  /*!
   * \brief Tries to leave a structure.
   * \return kFinished if inside a structure, or the error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if not inside a structure.}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If inside a structure:
   *   - Set the signal that a structure has been left.
   *   - Return kRunning.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto Leave() noexcept -> ParserResult {
    ParserResult result{ParserState::kFinished};

    if (entered_) {
      entered_ = false;
    } else {
      // VCA_VAJSON_WITHIN_SPEC
      result.EmplaceError(JsonErrc::kUserValidationFailed, "Cannot leave level");
    }

    return result;
  }

  /*!
   * \brief Returns true if currently inside a structure.
   * \return True if inside a structure.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsInside() const noexcept -> bool { return this->entered_; }

 private:
  /*!
   * \brief Signals that a level has been entered.
   */
  bool entered_{false};
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_LEVEL_VALIDATOR_H_

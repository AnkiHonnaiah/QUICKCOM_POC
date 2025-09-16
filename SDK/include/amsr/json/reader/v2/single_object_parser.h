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
/**       \file
 *        \brief    A specialized parser to parse a single object.
 *
 *        \details  Provides a callback for every element it encounters and keeps track of the nesting depth.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_SINGLE_OBJECT_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_SINGLE_OBJECT_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/json/reader/internal/level_validator.h"
#include "amsr/json/reader/v2/parser.h"

namespace amsr {
namespace json {
namespace v2 {
/*!
 * \brief A parser for a single object, i.e. an object without sub-objects.
 *
 * \details Handles the OnStartObject & OnEndObject callbacks by itself. Calls OnKey for every key it encounters and
 *          Finalize on object end.
 *
 * \vpublic
 */
// VCA_VAJSON_MOLE_1298
class SingleObjectParser : public v2::Parser {
 public:
  /*!
   * \brief Constructs a SingleObjectParser.
   * \spec
   *   requires true;
   * \endspec
   * \param doc JSON document to parse.
   * \param object_already_open Specify if the object has already been opened. Defaults to false.
   */
  explicit SingleObjectParser(JsonData& doc, bool object_already_open = false) noexcept
      : v2::Parser{doc}, validator_{object_already_open} {}

  /*!
   * \brief Default event for the start of objects.
   * \return kRunning if not in an object, or the error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if already in an object}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate
   *
   * \internal
   * - If not inside an object:
   *   - Set the flag that an object has been entered.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  auto OnStartObject() noexcept -> ParserResult final { return this->validator_.Enter(); }

  /*!
   * \brief Default event for the end of objects.
   * \return kRunning if in an object, or the error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if not in an object}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate
   *
   * \internal
   * - Check if inside an object:
   *   - Call the Finalize callback and return its Result.
   * \endinternal
   */
  auto OnEndObject(std::size_t) noexcept -> ParserResult final {
    return this->validator_.Leave().Filter([this](ParserState) {
      // VCA_VAJSON_LAMBDA_CAPTURE
      return this->Finalize();
    });
  }

  /*!
   * \brief Default event for unexpected elements that aborts the parsing.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if there is no callback registered for the event}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate
   */
  auto OnUnexpectedEvent() noexcept -> ParserResult override {
    return ParserResult{MakeErrorCode(JsonErrc::kUserValidationFailed, "Expected to parse an object of elements.")};
  }

 protected:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_JSON_AutosarC++17_10-M9.3.3_non_const_method
  /*!
   * \brief Default event if the entire object has been successfully parsed.
   * \return The empty Result.
   *
   * \details Default implementation does nothing. User implementation could be used to validate parser results or fill
   *          in inout parameters etc.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  virtual auto Finalize() noexcept -> Result<void> { return Result<void>{}; }

 private:
  /*!
   * \brief Validates if the only one level of object has been entered.
   */
  internal::LevelValidator validator_;
};
}  // namespace v2
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_SINGLE_OBJECT_PARSER_H_

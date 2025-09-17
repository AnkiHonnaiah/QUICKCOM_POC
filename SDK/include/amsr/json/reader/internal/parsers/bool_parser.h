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
/**       \file     bool_parser.h
 *        \brief    A specialized parser for boolean values.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_BOOL_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_BOOL_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/json/reader/internal/parsers/virtual_parser.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace json {
namespace internal {
/*! \brief A parser that only parses a single boolean value. */
// VCA_VAJSON_MOLE_1298
class BoolParser final : public VirtualParser {
  /*! \brief Type of function to be executed when bool values are read. */
  using Fn = vac::language::FunctionRef<Result<void>(bool)>;

 public:
  /*!
   * \brief Constructs a BoolParser.
   * \param doc JSON document to parse.
   * \param fn Function to execute on the bool.
   *
   * \details Callback must take the bool and return Result<void>.
   *
   * \context ANY
   * \pre Callback does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  // VCA_VAJSON_INTERNAL_CALL
  BoolParser(JsonData& doc, Fn const& fn) noexcept : VirtualParser{doc}, fn_{fn} {}

  /*!
   * \brief Event for Bools.
   * \param v Parsed boolean value.
   * \return kFinished if the callback function succeeds, or its error.
   *
   * \error{The error returned by the callback function}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - Execute the callback with the parsed bool value.
   * - If the callback succeeds:
   *   - Return kFinished.
   * - Otherwise:
   *   - Return the error of the callback.
   * \endinternal
   */
  auto OnBool(bool v) noexcept -> ParserResult final {
    // VCA_VAJSON_WITHIN_SPEC
    return Result<void>{std::forward<Fn>(this->fn_)(v)}.Map([]() { return ParserState::kFinished; });
  }

  /*!
   * \brief Default event for unexpected elements that aborts the parsing.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no bool value is parsed}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnUnexpectedEvent() noexcept -> ParserResult final {
    return ParserResult{MakeErrorCode(JsonErrc::kUserValidationFailed, "Expected to parse a boolean.")};
  }

 private:
  /*! \brief Function to be executed on the boolean value. */
  Fn fn_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_BOOL_PARSER_H_

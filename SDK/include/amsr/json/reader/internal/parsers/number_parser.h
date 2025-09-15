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
/**       \file     number_parser.h
 *        \brief    A specialized parser for number values.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_NUMBER_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_NUMBER_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/json/reader/internal/parsers/virtual_parser.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief A parser that only parses a single number value.
 * \tparam T Type of number to parse.
 */
template <typename T>
// VCA_VAJSON_MOLE_1298
class NumberParser final : public VirtualParser {
  /*! \brief Type of function to be executed when the numbers are read. */
  using Fn = vac::language::FunctionRef<Result<void>(T)>;

 public:
  /*!
   * \brief Constructs a NumberParser.
   * \param doc JSON document to parse.
   * \param fn Function to execute on the number.
   *
   * \details Callback must take the number of type T and return Result<void>.
   *
   * \context ANY
   * \pre Callback does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  // VCA_VAJSON_INTERNAL_CALL
  NumberParser(JsonData& doc, Fn const& fn) noexcept : VirtualParser{doc}, fn_{fn} {}

  /*!
   * \brief Event for Numbers.
   * \param number that is parsed.
   * \return kFinished if the callback function succeeds, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the number could not be converted to the wanted type.}
   * \error{The error returned by the callback function}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the JsonNumber could be successfully converted to a number of type T:
   *   - Execute the callback with the number.
   *   - If the callback succeeds:
   *     - Return kRunning.
   * - Otherwise:
   *   - Return the error.
   * \endinternal
   */
  auto OnNumber(JsonNumber number) noexcept -> ParserResult final {
    // VCA_VAJSON_WITHIN_SPEC
    return number.TryAs<T>().AndThen(std::forward<Fn>(this->fn_)).Map([]() { return ParserState::kFinished; });
  }

  /*!
   * \brief Default event for unexpected elements that aborts the parsing.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no number is parsed}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto OnUnexpectedEvent() noexcept -> ParserResult final {
    return ParserResult{MakeErrorCode(JsonErrc::kUserValidationFailed, "Expected to parse a number.")};
  }

 private:
  /*! \brief Function to be executed on the number value. */
  Fn fn_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_NUMBER_PARSER_H_

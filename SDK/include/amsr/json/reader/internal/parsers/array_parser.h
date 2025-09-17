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
/**       \file     array_parser.h
 *        \brief    A specialized parser for arrays.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_ARRAY_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_ARRAY_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/json/reader/v2/single_array_parser.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace json {
namespace internal {
/*! \brief A parser that only parses multiple values of the same type. */
// VCA_VAJSON_MOLE_1298
class ArrayParser final : public v2::SingleArrayParser {
  /*! \brief Type of function to be executed when the keys are read. */
  using Fn = vac::language::FunctionRef<Result<void>(std::size_t)>;

 public:
  /*!
   * \brief Constructs an ArrayParser.
   * \param doc JSON document to parse.
   * \param fn Function to execute on every array element.
   *
   * \details Callback must take the array index as a std::size_t and return Result<void>.
   *
   * \context ANY
   * \pre Callback does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ArrayParser(JsonData& doc, Fn const& fn) noexcept : v2::SingleArrayParser{doc}, fn_{fn} {}

  /*!
   * \brief Event for array Elements.
   * \return kRunning if the callback function succeeds, or its error.
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
   * - Execute the callback with the current array index.
   * - If the callback succeeds:
   *   - Return kRunning.
   * - Otherwise:
   *   - Return the error of the callback.
   * \endinternal
   */
  auto OnElement() noexcept -> ParserResult final {
    // VCA_VAJSON_WITHIN_SPEC
    return std::forward<Fn>(this->fn_)(this->GetIndex()).Map([]() { return ParserState::kRunning; });
  }

 private:
  /*! \brief Function to be executed on every element. */
  Fn fn_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_ARRAY_PARSER_H_

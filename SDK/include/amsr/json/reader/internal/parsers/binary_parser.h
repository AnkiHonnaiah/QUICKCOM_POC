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
 *        \brief    A specialized parser for binary content.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_BINARY_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_BINARY_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/json/reader/internal/parsers/virtual_parser.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace json {
namespace internal {
/*! \brief A parser that only parses a single binary value. */
// VCA_VAJSON_MOLE_1298
class BinaryParser final : public VirtualParser {
  /*! \brief Type of function to be executed when a binary values are read. */
  using Fn = vac::language::FunctionRef<Result<void>(Bytes)>;

 public:
  /*!
   * \brief Constructs a BinaryParser.
   * \param doc JSON document to parse.
   * \param fn Function to execute when the binary content is read.
   *
   * \details Callback must take the binary content as amsr::core::Span<char> and return Result<void>.
   *
   * \context ANY
   * \pre Callback does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  // VCA_VAJSON_INTERNAL_CALL
  BinaryParser(JsonData& doc, Fn const& fn) noexcept : VirtualParser{doc}, fn_{fn} {}

  /*!
   * \brief Event for binary content.
   * \param view Read binary content.
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
   * - Execute the callback with the read binary content.
   * - If the callback succeeds:
   *   - Return kFinished.
   * - Otherwise:
   *   - Return the error of the callback.
   * \endinternal
   */
  auto OnBinary(Bytes view) noexcept -> ParserResult final {
    // VCA_VAJSON_WITHIN_SPEC
    return this->fn_(view).Map([]() { return ParserState::kFinished; });
  }

  /*!
   * \brief Default event for unexpected elements that aborts the parsing.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if no binary content is parsed}
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
    return ParserResult{MakeErrorCode(JsonErrc::kUserValidationFailed, "Expected to parse binary content.")};
  }

 private:
  /*! \brief Function to be executed on binary content. */
  Fn fn_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_BINARY_PARSER_H_

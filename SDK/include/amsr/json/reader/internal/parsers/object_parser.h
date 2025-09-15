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
/**     \file       object_parser.h
 *      \brief      A specialized parser for objects.
 *      \details    Enables easy parsing of simple object structures.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_OBJECT_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_OBJECT_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/json/reader/v2/single_object_parser.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace json {
namespace internal {
/*! \brief A parser that parses key value pairs. */
// VCA_VAJSON_MOLE_1298
class ObjectParser final : public v2::SingleObjectParser {
  /*! \brief Type of function to be executed when the objects are read. */
  using Fn = vac::language::FunctionRef<Result<void>(StringView)>;

 public:
  /*!
   * \brief Constructs an ObjectParser.
   * \param doc JSON document to parse.
   * \param fn Function to execute on every object key.
   * \param object_already_open Specify if the object has already been opened. Defaults to false.
   * \details Callback must take the object key as an amsr::core::StringView and return Result<void>.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ObjectParser(JsonData& doc, Fn&& fn, bool object_already_open = false) noexcept
      : v2::SingleObjectParser{doc, object_already_open}, fn_{std::move(fn)} {}

  /*!
   * \brief Event for object keys.
   * \param key Parsed key.
   * \return kRunning if the callback function succeeds, or its error.
   *
   * \error{The error returned by the callback function}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - Execute the callback with the current key.
   * - If the callback succeeds:
   *   - Return kRunning.
   * - Otherwise:
   *   - Return the error of the callback.
   * \endinternal
   */
  auto OnKey(amsr::core::StringView key) noexcept -> ParserResult final {
    // VCA_VAJSON_WITHIN_SPEC
    return std::forward<Fn>(this->fn_)(key).Map([]() { return ParserState::kRunning; });
  }

 private:
  /*! \brief Function to be executed on every element. */
  Fn fn_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_OBJECT_PARSER_H_

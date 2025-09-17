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
/**       \file     structure_parser.h
 *        \brief    A SAX-Style JSON parser.
 *
 *        \details  Parses JSON text from a stream and sends events synchronously to an Implementer.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRUCTURE_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRUCTURE_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/language/function_ref.h"

#include "amsr/json/reader/internal/depth_counter.h"
#include "amsr/json/reader/internal/json_ops.h"
#include "amsr/json/reader/internal/ref.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/types.h"

namespace amsr {
namespace json {
namespace internal {
// VECTOR NC Metric-OO.WMC.One: MD_JSON_Metric.OO.WMC.One_parser
/*!
 * \brief A SAX-style JSON parser.
 * \tparam Implementer to use.
 *
 * \details Parses JSON text from a stream and sends events synchronously to an Implementer.
 *
 * \vprivate Component private.
 *
 * \trace DSGN-JSON-Reader-Data-Items
 */
template <typename Implementer>
class StructureParser final {
  /*!
   * \brief Callback implementer.
   */
  Ref<Implementer> implementer_;

  /*!
   * \brief Operations on JSON document.
   */
  JsonOps json_ops_;

 public:
  /*!
   * \brief Constructs a StructureParser.
   * \param implementer The implementer for this parser.
   * \param doc JSON document to parse.
   *
   * \context ANY
   * \pre Implementer does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  StructureParser(Implementer& implementer, JsonData& doc) noexcept;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default move constructor.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  StructureParser(StructureParser&&) noexcept = default;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default move assignment.
   * \return A reference to the moved into object.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto operator=(StructureParser&&) & noexcept -> StructureParser& = default;

  /*!
   * \brief Deleted copy constructor.
   */
  StructureParser(StructureParser const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   */
  auto operator=(StructureParser const&) & -> StructureParser& = delete;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default destructor.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ~StructureParser() noexcept = default;

  /*!
   * \brief Parses the file until the current parser is finished.
   * \return The empty Result, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if parsing has failed due to invalid JSON data.}
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if an unknown value has been encountered.}
   * \error{The error of a callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto Parse() noexcept -> Result<void>;

  /*!
   * \brief Parses the file until the current parser is finished.
   * \return kRunning, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if parsing has failed due to invalid JSON data.}
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if an unknown value has been encountered.}
   * \error{The error of a callback function, if the callback returned an error.}
   *
   * \details Parserstate will be kRunning to allow for further parsing.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto SubParse() noexcept -> ParserResult;

  /*!
   * \brief Returns a reference to the Json file.
   * \return Reference to the Json file.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonDocument() noexcept -> JsonData&;

  /*!
   * \brief Returns a reference to the Json file.
   * \return Const reference to the Json file.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonDocument() const noexcept -> JsonData const&;

 private:
  /*!
   * \brief Parses a "null" value.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if no valid 'null' literal was encountered.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseNull() noexcept -> ParserResult;

  /*!
   * \brief Parses a "true" value.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if no valid string was encountered.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseTrue() noexcept -> ParserResult;

  /*!
   * \brief Parses a "false" value.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the 'false' literal cannot be parsed}
   * \error{The error of the callback function, if the callback returned an error}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseFalse() noexcept -> ParserResult;

  /*!
   * \brief Parses a number value.
   * \param cur The current character.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the number literal cannot be parsed}
   * \error{The error of the callback function, if the callback returned an error}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseNumber(char cur) noexcept -> ParserResult;

  /*!
   * \brief Parses a string value.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the next token is no valid string.}
   * \error{The error of the callback function, if the callback returned an error}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseString() noexcept -> ParserResult;

  /*!
   * \brief Parses an unescaped string value.
   * \param string The unescaped string to parse.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the next token is no valid string.}
   * \error{The error of the callback function, if the callback returned an error}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseUnescapedString(CStringView string) noexcept -> ParserResult;

  /*!
   * \brief Parses a JSON string and removes JSON escape characters.
   * \return A string view into the parsed string, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if an invalid JSON string was encountered.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ReadJsonString() noexcept -> Result<CStringView>;

  /*!
   * \brief Unescapes a single character.
   * \param escaped The character to unescape.
   * \return The unescaped character.
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, If an unknown/unsupported escape sequence was encountered.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static auto UnescapeChar(char escaped) noexcept -> Result<char>;

  /*!
   * \brief Parses the start of an Object.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the object cannot be opened.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseStartObject() noexcept -> ParserResult;

  /*!
   * \brief Parses the end of an Object.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, If no end of an object can be parsed.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseEndObject() noexcept -> ParserResult;

  /*!
   * \brief Parses the start of an Array.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the array cannot be opened.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseStartArray() noexcept -> ParserResult;

  /*!
   * \brief Parses the end of an Array.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if in an object or on toplevel.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseEndArray() noexcept -> ParserResult;

  /*!
   * \brief Parses a comma in a list/object.
   * \return kRunning if the parser is in an object or array, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the comma at this position is invalid}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseComma() noexcept -> ParserResult;

  /*!
   * \brief Parses the length tag of a value.
   * \return A Result containing the length, or an error.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseLength() noexcept -> Result<std::uint32_t>;

  /*!
   * \brief Parses binary content.
   * \param callback to execute on the binary content.
   * \return The Result of the callback, or an error.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ReadBinary(vac::language::FunctionRef<ParserResult(StringView)> callback) noexcept -> ParserResult;

  /*!
   * \brief Parses a binary key.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if no key is allowed at this point.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseBinaryKey() noexcept -> ParserResult;

  /*!
   * \brief Parses a binary string.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if no value is allowed at this point.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseBinaryString() noexcept -> ParserResult;

  /*!
   * \brief Parses a binary value.
   * \return The Result of the implementer callback, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if no value is allowed at this point.}
   * \error{The error of the callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseBinaryValue() noexcept -> ParserResult;

  /*!
   * \brief Parses any JSON value.
   * \return kRunning if the parser has not finished yet, kFinished if the end of the file is reached, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the JSON data is invalid.}
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if an unknown value has been encountered.}
   * \error{The error of a callback function, if the callback returned an error.}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ParseValue() noexcept -> ParserResult;

  /*!
   * \brief Parses a (more or less valid) number.
   * \param first The first character of the number.
   * \return The parsed number string.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto GetNumber(char first) noexcept -> CStringView;

  /*!
   * \brief Returns the Json structure state.
   * \return The Json structure state.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetState() noexcept -> DepthCounter&;

  /*!
   * \brief Returns the reference to the encoded file.
   * \return Reference to the encoded file.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonOps() noexcept -> JsonOps&;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRUCTURE_PARSER_H_

/**********************************************************************************************************************
 *  COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 * -----------------------------------------------------------------------------------------------------------------*/
/**       \file     strict_parser.h
 *        \brief    Contains a SAX-style JSON parser.
 *
 *        \details  Provides callbacks for implementing parsers.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRICT_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRICT_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/internal/parsers/structure_parser.h"
#include "amsr/json/reader/internal/parsers/structure_parser_impl.h"
#include "amsr/json/reader/json_data.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/json/util/types.h"

namespace amsr {
namespace json {
namespace internal {
// VECTOR NC Metric-OO.WMC.One: MD_JSON_Metric.OO.WMC.One_parser
/*!
 * \brief A SAX-style JSON parser.
 * \tparam Child Type of parser which is called for every event that it implements.
 *
 * \details The child parser may implement callbacks for all types of elements it expects to appear. If the parser
 *          encounters any other type, it calls OnUnexpectedEvent. If this callback is not overwritten by the child
 *          parser, the default implementation aborts parsing.
 *
 * \vprivate Component private.
 *
 * \trace DSGN-JSON-Reader-Data-Items
 */
template <typename Child>
class StrictParser {
 public:
  /*!
   * \brief Constructs a Parser.
   * \param doc to parse.
   *
   * \context ANY
   * \pre Child parser does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  explicit StrictParser(JsonData& doc) noexcept : parser_{*this, doc} {}

  /*!
   * \brief Deleted copy constructor.
   */
  StrictParser(StrictParser const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   */
  auto operator=(StrictParser const&) & -> StrictParser& = delete;

  /*!
   * \brief Deleted move constructor.
   */
  StrictParser(StrictParser&&) = delete;

  /*!
   * \brief Deleted move assignment.
   */
  auto operator=(StrictParser&&) & -> StrictParser& = delete;

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
  // VCA_VAJSON_MOLE_1298
  virtual ~StrictParser() noexcept = default;

  /*!
   * \brief Parses file until the current parser is finished.
   * \return The empty Result, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if an unknown value has been encountered.}
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if parsing has failed due to invalid JSON data.}
   * \error{The error of a callback function, if the callback returned an error}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto Parse() noexcept -> Result<void> { return this->parser_.Parse(); }

  /*!
   * \brief Parses file until the current parser is finished.
   * \return kRunning, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kUserValidationFailed, if an unknown value has been encountered}
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if parsing has failed due to invalid JSON data.}
   * \error{The error of a callback function, if the callback returned an error}
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
  auto SubParse() noexcept -> ParserResult { return this->parser_.SubParse(); }

  // Default implementations to fall back to!
  /*!
   * \brief Default event for Null.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  // VCA_VAJSON_THIS_DEREF
  auto OnNull() noexcept -> ParserResult { return this->GetChild().OnUnexpectedEvent(); }

  /*!
   * \brief Default event for Bools.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  auto OnBool(bool) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->GetChild().OnUnexpectedEvent();
  }

  /*!
   * \brief Default event for Numbers.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  auto OnNumber(JsonNumber) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->GetChild().OnUnexpectedEvent();
  }

  /*!
   * \brief Default event for Strings.
   * \details The provided CStringView is only valid until any other method or parser operating on the same document is
   *          called.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  auto OnString(CStringView) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->GetChild().OnUnexpectedEvent();
  }

  /*!
   * \brief Default event for binary strings.
   * \return An error, because binary content is not supported by the v1 parser implementation.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate Component private.
   */
  constexpr static auto OnBinaryString(StringView) noexcept -> ParserResult {
    return ParserResult{MakeErrorCode(
        JsonErrc::kUserValidationFailed,
        "StrictParser::OnBinaryString: v1 parsers do not support Vector Proprietary JSON. Use v2 parsers instead!")};
  }

  /*!
   * \brief Default event for Keys.
   * \details The provided CStringView is only valid until any other method or parser operating on the same document is
   *          called.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  auto OnKey(CStringView) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->GetChild().OnUnexpectedEvent();
  }

  /*!
   * \brief Default event for binary keys.
   * \return An error, because binary content is not supported by the v1 parser implementation.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate Component private.
   */
  constexpr static auto OnBinaryKey(StringView) noexcept -> ParserResult {
    return ParserResult{MakeErrorCode(
        JsonErrc::kUserValidationFailed,
        "StrictParser::OnBinaryKey: v1 parsers do not support Vector Proprietary JSON. Use v2 parsers instead!")};
  }

  /*!
   * \brief Default event for the start of objects.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  auto OnStartObject() noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->GetChild().OnUnexpectedEvent();
  }

  /*!
   * \brief Default event for the end of objects.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  auto OnEndObject(std::size_t) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->GetChild().OnUnexpectedEvent();
  }

  /*!
   * \brief Default event for the start of arrays.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  auto OnStartArray() noexcept -> ParserResult { return this->GetChild().OnUnexpectedEvent(); }

  /*!
   * \brief Default event for the end of arrays.
   * \return The result of the OnUnexpectedEvent callback or its error.
   *
   * \error{The error of the callback function, if the callback returned an error}
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
   * - Call the child's OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  auto OnEndArray(std::size_t) noexcept -> ParserResult { return this->GetChild().OnUnexpectedEvent(); }

  /*!
   * \brief Default event for binary values.
   * \return An error, because binary values are not supported by the v1 parser implementation.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate Component private.
   */
  constexpr static auto OnBinary(Bytes) noexcept -> ParserResult {
    return ParserResult{MakeErrorCode(
        JsonErrc::kUserValidationFailed,
        "StrictParser::OnBinary: v1 parsers do not support Vector Proprietary JSON. Use v2 parsers instead!")};
  }

  /*!
   * \brief Default event for default callbacks that aborts parsing.
   * \return An error result.
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \details This callback is called by all default event callbacks which are not overridden by the child parser. It
   *          allows the parser to continue parsing if an unexpected event is encountered (i.e. a JSON element for which
   *          no callback is implemented by the child parser). It may be overridden by the child parser if a different
   *          behaviour is desired.
   */
  constexpr static auto OnUnexpectedEvent() noexcept -> ParserResult {
    return ParserResult{
        MakeErrorCode(JsonErrc::kUserValidationFailed, "Use of default method not allowed in this context.")};
  }

  /*!
   * \brief Returns a reference to the CRTP child.
   * \return Reference to the CRTP child.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetChild() noexcept -> Child& {
    // VECTOR NL AutosarC++17_10-M5.2.3, VectorC++-V5.2.3: MD_JSON_FBoundedPolymorphism
    return *static_cast<Child*>(this);
  }

 protected:
  /*!
   * \brief Returns the current key.
   * \return The current key.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetCurrentKey() const noexcept -> vac::container::CStringView { return this->GetJsonDocument().GetCurrentKey(); }

  /*!
   * \brief Returns a reference to the JSON file.
   * \return Reference to the JSON file.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonDocument() noexcept -> JsonData& { return this->parser_.GetJsonDocument(); }

  /*!
   * \brief Returns a reference to the JSON file.
   * \return Const reference to the JSON file.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonDocument() const noexcept -> JsonData const& { return this->parser_.GetJsonDocument(); }

 private:
  /*!
   * \brief Generic JSON structure parser.
   */
  StructureParser<StrictParser> parser_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_STRICT_PARSER_H_

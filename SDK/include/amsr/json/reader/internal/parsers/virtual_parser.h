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
/**       \file     virtual_parser.h
 *        \brief    Contains a SAX-style JSON parser.
 *
 *        \details  Provides callbacks for implementing parsers.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_VIRTUAL_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_VIRTUAL_PARSER_H_

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
 *
 * \details The VirtualParser implements an interface for parsers and is based on dynamic polymorphism.
 *          The derived parser can implement callbacks for all types of elements it expects to appear. If the parser
 *          encounters any other type, it calls OnUnexpectedEvent. If this callback is not overridden by the derived
 *          parser, the default implementation aborts parsing.
 *
 * \vprivate Component private.
 *
 * \trace DSGN-JSON-Reader-Data-Items
 */
class VirtualParser {
 public:
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_JSON_AutosarC++17_10-A11.3.1_structure_parser
  /*!
   * \brief Friend declaration, in order to make StructureParser able to access private methods of VirtualParser.
   */
  template <typename T>
  friend class StructureParser;

  /*!
   * \brief Constructs a VirtualParser.
   * \param doc to parse.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  explicit VirtualParser(JsonData& doc) noexcept;

  /*!
   * \brief Deleted copy constructor.
   */
  VirtualParser(VirtualParser const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   */
  auto operator=(VirtualParser const&) & -> VirtualParser& = delete;

  /*!
   * \brief Deleted move constructor.
   */
  VirtualParser(VirtualParser&&) = delete;

  /*!
   * \brief Deleted move assignment.
   */
  auto operator=(VirtualParser&&) & -> VirtualParser& = delete;

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
  virtual ~VirtualParser() noexcept = default;

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
  virtual auto Parse() noexcept -> Result<void>;

  /*!
   * \brief Parses file until the current parser is finished.
   * \return A ParserResult.
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
  auto SubParse() noexcept -> ParserResult;

  /*!
   * \brief Returns a reference to the CRTP child.
   * \return Reference to the CRTP child.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetChild() noexcept -> VirtualParser&;

 protected:
  /*!
   * \brief Returns the current key.
   * \return THe current key.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetCurrentKey() const noexcept -> CStringView;

  /*!
   * \brief Returns a reference to the JSON file.
   * \return Reference to the JSON file.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonDocument() noexcept -> JsonData&;

  /*!
   * \brief Returns a reference to the JSON file.
   * \return Const reference to the JSON file.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonDocument() const noexcept -> JsonData const&;

 private:
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
   */
  virtual auto OnNull() noexcept -> ParserResult;

  /*!
   * \brief Default event for Bools.
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
   */
  virtual auto OnBool(bool) noexcept -> ParserResult;

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
   */
  virtual auto OnNumber(JsonNumber) noexcept -> ParserResult;

  /*!
   * \brief Default event for Strings.
   * \details The provided StringView is only valid until any other method or parser operating on the same document is
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
   */
  virtual auto OnString(StringView) noexcept -> ParserResult;

  /*!
   * \brief Forwarding function to accept CStringView strings from parent parser.
   * \param view Parsed string.
   * \return The result of the OnString callback or its error.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnString(CStringView view) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->OnString(StringView{view});
  }

  /*!
   * \brief Forwarding function to accept binary strings.
   * \param view Parsed binary string.
   * \return The result of the OnString callback or its error.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnBinaryString(StringView view) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->OnString(view);
  }

  /*!
   * \brief Default event for Keys.
   * \details The provided StringView is only valid until any other method or parser operating on the same document is
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
   */
  virtual auto OnKey(StringView) noexcept -> ParserResult;

  /*!
   * \brief Forwarding function to accept CStringView keys from parent parser.
   * \param view Parsed key.
   * \return The result of the OnKey callback or its error.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnKey(CStringView view) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->OnKey(StringView{view});
  }

  /*!
   * \brief Forwarding function to accept binary keys.
   * \param view Parsed binary key.
   * \return The result of the OnKey callback or its error.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnBinaryKey(StringView view) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->OnKey(view);
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
   */
  virtual auto OnStartObject() noexcept -> ParserResult;

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
   */
  virtual auto OnEndObject(std::size_t) noexcept -> ParserResult;

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
   */
  virtual auto OnStartArray() noexcept -> ParserResult;

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
   */
  virtual auto OnEndArray(std::size_t) noexcept -> ParserResult;

  /*!
   * \brief Default event for binary content.
   * \details The provided Span is only valid until any other method or parser operating on the same document is called.
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
   * - Call the OnUnexpectedEvent callback that determines how to handle the unexpected event and return its
   *   result.
   * \endinternal
   */
  virtual auto OnBinary(Bytes) noexcept -> ParserResult {
    // VCA_VAJSON_THIS_DEREF
    return this->OnUnexpectedEvent();
  }

  /*!
   * \brief Default event for default callbacks that aborts parsing.
   * \return An error result.
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \details This callback is called by all default event callbacks which are not overridden by the derived parser.
   * It allows the parser to continue parsing if an unexpected event is encountered (i.e. a JSON element for which no
   * callback is implemented by the derived parser). It may be overridden by the derived parser if a different
   * behaviour is desired.
   */
  virtual auto OnUnexpectedEvent() noexcept -> ParserResult;

  /*!
   * \brief Generic JSON structure parser.
   */
  StructureParser<VirtualParser> parser_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_PARSERS_VIRTUAL_PARSER_H_

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
/*!        \file
 *        \brief  JSON parser of method runtime configurations.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_METHOD_JSON_PARSER_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_METHOD_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/v2/single_object_parser.h"
#include "ara/core/result.h"
#include "vac/container/string_literals.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/method_config.h"
#include "amsr/comtrace/internal/config/parser/json_parser_util.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief   Parser for the method runtime JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic validation (datatypes, multiplicities, ...).
 *
 * \unit ComTrace::TraceConfig
 */
class MethodJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Name alias MethodConfig.
   */
  using MethodConfig = ::amsr::comtrace::internal::config::MethodConfig;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   *
   * \param[in,out] json             JSON document to parse. The reference must stay valid as long as this instance
   *                                 exists.
   * \param[in,out] config           A reference to the configuration object to be filled by the parser. The reference
   *                                 must stay valid as long as this instance exists.
   * \param[in]     is_field_method  Whether the method being parsed is a field getter/setter.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  MethodJsonParser(JsonData& json, MethodConfig& config, bool is_field_method = false) noexcept;

  MethodJsonParser(MethodJsonParser const&) noexcept = delete;
  auto operator=(MethodJsonParser const&) noexcept -> MethodJsonParser& = delete;
  MethodJsonParser(MethodJsonParser&&) noexcept = delete;
  auto operator=(MethodJsonParser&&) noexcept -> MethodJsonParser& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~MethodJsonParser() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  /*!
   * \brief Callback on a new key parsed from the JSON object.
   *
   * \param[in] json_key  Parsed key string.
   *
   * \return Parser result.
   *
   * \error ComTraceErrc::json_parsing_failure  If JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnKey(StringView json_key) noexcept -> ParserResult final;

  /*!
   * \brief Callback if an unexpected event happened during JSON parsing.
   *
   * \return Always a parser result with related error code.
   *
   * \error ComTraceErrc::json_parsing_failure  In any case.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnUnexpectedEvent() noexcept -> ParserResult final;

  /*!
   * \brief Callback to validate that all configuration elements have been accepted.
   *
   * \return Either nothing or the error.
   *
   * \error ComTraceErrc::json_parsing_failure  If validation fails.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto Finalize() noexcept -> ara::core::Result<void> final;

 private:
  /*! JSON key for the short name. */
  static constexpr StringView kShortNameKey{"shortname"_sv};
  /*! JSON key for tracing of method request send */
  static constexpr StringView kTraceRequestSendKey{"trace_request_send"_sv};
  /*! JSON key for tracing of method request received */
  static constexpr StringView kTraceRequestReceivedKey{"trace_request_received"_sv};
  /*! JSON key for tracing of method response send */
  static constexpr StringView kTraceResponseSendKey{"trace_response_send"_sv};
  /*! JSON key for tracing of method response received */
  static constexpr StringView kTraceResponseReceivedKey{"trace_response_received"_sv};

  /*!
   * \brief Parse a shortname JSON key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseShortname() noexcept -> ParserResult;

  /*!
   * \brief Parse a request send JSON key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseRequestSend() noexcept -> ParserResult;

  /*!
   * \brief Parse a request received JSON key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseRequestReceived() noexcept -> ParserResult;

  /*!
   * \brief Parse a request send JSON key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseResponseSend() noexcept -> ParserResult;

  /*!
   * \brief Parse a response received JSON key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseResponseReceived() noexcept -> ParserResult;

  std::size_t number_of_short_names_found_{0};              /*!< Number of key 'shortname' found. */
  std::size_t number_of_trace_request_sends_found_{0};      /*!< Number of key 'trace_request_send' found. */
  std::size_t number_of_trace_request_receiveds_found_{0};  /*!< Number of key 'trace_request_received' found. */
  std::size_t number_of_trace_response_sends_found_{0};     /*!< Number of key 'trace_response_send' found. */
  std::size_t number_of_trace_response_receiveds_found_{0}; /*!< Number of key 'trace_response_received' found. */

  /*! Reference to configuration struct filled by the parser. */
  MethodConfig& config_;

  /*! Whether the method being parsed is a field getter/setter. */
  bool const is_field_method_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_METHOD_JSON_PARSER_H_

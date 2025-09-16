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
 *        \brief  JSON parser of event configuration.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_EVENT_JSON_PARSER_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_EVENT_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/v2/single_object_parser.h"
#include "ara/core/result.h"
#include "vac/container/string_literals.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/event_config.h"
#include "amsr/comtrace/internal/config/parser/json_parser_util.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief   Parser for the event JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic validation (datatypes, multiplicities, ...).
 *
 * \unit ComTrace::TraceConfig
 */
class EventJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Name alias EventConfig.
   */
  using EventConfig = ::amsr::comtrace::internal::config::EventConfig;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   *
   * \param[in,out] json               JSON document to parse. The reference must stay valid as long as this instance
   *                                   exists.
   * \param[in,out] config             A reference to the configuration object to be filled by the parser. The reference
   *                                   must stay valid as long as this instance exists.
   * \param[in]     is_field_notifier  Whether the event being parsed is a field notifier.
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
  EventJsonParser(JsonData& json, EventConfig& config, bool is_field_notifier = false) noexcept;

  EventJsonParser(EventJsonParser const&) noexcept = delete;
  auto operator=(EventJsonParser const&) noexcept -> EventJsonParser& = delete;
  EventJsonParser(EventJsonParser&&) noexcept = delete;
  auto operator=(EventJsonParser&&) noexcept -> EventJsonParser& = delete;

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
  ~EventJsonParser() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

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

  /*! JSON key for tracing of event send */
  static constexpr StringView kTraceSendKey{"trace_send"_sv};

  /*! JSON key for tracing of event send allocate */
  static constexpr StringView kTraceSendAllocateKey{"trace_send_allocate"_sv};

  /*! JSON key for tracing of event read sample */
  static constexpr StringView kTraceReadSampleKey{"trace_read_sample"_sv};

  std::size_t number_of_short_names_found_{0};          /*!< Number of key 'shortname' found. */
  std::size_t number_of_trace_sends_found_{0};          /*!< Number of key 'trace_send' found. */
  std::size_t number_of_trace_send_allocates_found_{0}; /*!< Number of key 'trace_send_allocate' found. */
  std::size_t number_of_trace_read_samples_found_{0};   /*!< Number of key 'trace_read_sample' found. */

  /*! Reference to configuration struct filled by the parser. */
  EventConfig& config_;

  /*! Whether the event being parsed is a field notifier. */
  bool const is_field_notifier_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_EVENT_JSON_PARSER_H_

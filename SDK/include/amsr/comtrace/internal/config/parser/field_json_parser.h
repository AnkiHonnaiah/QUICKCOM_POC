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
 *        \brief  JSON parser of field configuration.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_FIELD_JSON_PARSER_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_FIELD_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/v2/single_object_parser.h"
#include "ara/core/result.h"
#include "vac/container/string_literals.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/event_config.h"
#include "amsr/comtrace/internal/config/field_config.h"
#include "amsr/comtrace/internal/config/method_config.h"
#include "amsr/comtrace/internal/config/parser/json_parser_util.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief   Parser for the field runtime JSON configurations.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic validation (datatypes, multiplicities, ...).
 *
 * \unit ComTrace::TraceConfig
 */
class FieldJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*! Type-alias for FieldConfig. */
  using FieldConfig = ::amsr::comtrace::internal::config::FieldConfig;

  /*! Type-alias for Field Getter configuration. */
  using FieldGetterConfig = ::amsr::comtrace::internal::config::MethodConfig;

  /*! Type-alias for Field Setter configuration. */
  using FieldSetterConfig = ::amsr::comtrace::internal::config::MethodConfig;

  /*! Type-alias for Field Notifier configuration. */
  using FieldNotifierConfig = ::amsr::comtrace::internal::config::EventConfig;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   *
   * \param[in,out] json    JSON document to parse. The reference must stay valid as long as this instance exists.
   * \param[in,out] config  A reference to the configuration object to be filled by the parser. The reference must stay
   *                        valid as long as this instance exists.
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
  FieldJsonParser(JsonData& json, FieldConfig& config) noexcept;

  FieldJsonParser(FieldJsonParser const&) noexcept = delete;
  auto operator=(FieldJsonParser const&) noexcept -> FieldJsonParser& = delete;
  FieldJsonParser(FieldJsonParser&&) noexcept = delete;
  auto operator=(FieldJsonParser&&) noexcept -> FieldJsonParser& = delete;

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
  ~FieldJsonParser() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

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
   * \brief Callback if an unexpected field happened during JSON parsing.
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
  /*! JSON key for the field getter. */
  static constexpr StringView kFieldGetterKey{"getter"_sv};
  /*! JSON key for the field setter. */
  static constexpr StringView kFieldSetterKey{"setter"_sv};
  /*! JSON key for the field notifier. */
  static constexpr StringView kFieldNotifierKey{"notifier"_sv};
  /*! JSON key for the short name. */
  static constexpr StringView kShortNameKey{"shortname"_sv};

  std::size_t number_of_setters_found_{0};     /*!< Number of key 'setter' found. */
  std::size_t number_of_notifiers_found_{0};   /*!< Number of key 'notifiers' found. */
  std::size_t number_of_getters_found_{0};     /*!< Number of key 'getter' found. */
  std::size_t number_of_short_names_found_{0}; /*!< Number of key 'shortname' found. */

  /*! Reference to configuration struct filled by the parser. */
  FieldConfig& config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_FIELD_JSON_PARSER_H_

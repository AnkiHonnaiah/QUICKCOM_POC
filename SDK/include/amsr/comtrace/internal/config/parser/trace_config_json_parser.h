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
 *        \brief  JSON parser for the TraceConfig.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_TRACE_CONFIG_JSON_PARSER_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_TRACE_CONFIG_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/v2/single_object_parser.h"
#include "ara/core/result.h"
#include "vac/container/string_literals.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/parser/json_parser_util.h"
#include "amsr/comtrace/internal/config/trace_config.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief   Parser for the TraceConfig JSON file.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic validation (datatypes, multiplicities, ...).
 *
 * \unit ComTrace::TraceConfig
 */
class TraceConfigJsonParser final : public amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for TraceConfig.
   */
  using TraceConfig = ::amsr::comtrace::internal::config::TraceConfig;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   *
   * \param[in]  json    JSON document to parse.
   * \param[out] config  A reference to the configuration object to be filled by the parser.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  TraceConfigJsonParser(JsonData& json, TraceConfig& config) noexcept;

  TraceConfigJsonParser(TraceConfigJsonParser const&) noexcept = delete;
  auto operator=(TraceConfigJsonParser const&) noexcept -> TraceConfigJsonParser& = delete;
  TraceConfigJsonParser(TraceConfigJsonParser&&) noexcept = delete;
  auto operator=(TraceConfigJsonParser&&) noexcept -> TraceConfigJsonParser& = delete;

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
  ~TraceConfigJsonParser() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

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
   * \brief Parse the generator version.
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
  auto ParseGeneratorVersion() noexcept -> ParserResult;

 private:
  /*! JSON key for the generator version. */
  static constexpr StringView kGeneratorVersionKey{"generatorVersion"_sv};

  /*! JSON key for the services. */
  static constexpr StringView kServicesKey{"services"_sv};

  std::size_t number_of_service_keys_found_{0};       /*!< Number of key 'services' found. */
  std::size_t number_of_generator_versions_found_{0}; /*!< Number of key 'generator_version' found. */

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  TraceConfig& config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_TRACE_CONFIG_JSON_PARSER_H_

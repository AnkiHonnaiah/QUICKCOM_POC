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
 *        \brief  JSON parser for the GeneratorVersion.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_GENERATOR_VERSION_JSON_PARSER_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_GENERATOR_VERSION_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/v2/single_object_parser.h"
#include "ara/core/result.h"
#include "vac/container/string_literals.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/generator_version_config.h"
#include "amsr/comtrace/internal/config/parser/json_parser_util.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief   Parser for the generator version JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic validation (datatypes, multiplicities, ...).
 *
 * \unit ComTrace::TraceConfig
 */
class GeneratorVersionJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for GeneratorVersionConfig.
   */
  using GeneratorVersionConfig = ::amsr::comtrace::internal::config::GeneratorVersionConfig;

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
   *
   * \spec requires true; \endspec
   */
  GeneratorVersionJsonParser(JsonData& json, GeneratorVersionConfig& config) noexcept;

  GeneratorVersionJsonParser(GeneratorVersionJsonParser const&) noexcept = delete;
  auto operator=(GeneratorVersionJsonParser const&) noexcept -> GeneratorVersionJsonParser& = delete;
  GeneratorVersionJsonParser(GeneratorVersionJsonParser&&) noexcept = delete;
  auto operator=(GeneratorVersionJsonParser&&) noexcept -> GeneratorVersionJsonParser& = delete;

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
  ~GeneratorVersionJsonParser() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

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

 private:
  /*! JSON key for the generator name. */
  static constexpr StringView kGeneratorNameKey{"name"_sv};

  /*! JSON key for the generator commit ID. */
  static constexpr StringView kGeneratorCommitIdKey{"commitId"_sv};

  std::size_t number_of_names_found_{0};      /*!< Number of key 'name' found. */
  std::size_t number_of_commit_ids_found_{0}; /*!< Number of key 'commitId' found. */

  /*! Reference to configuration struct filled by the parser. */
  GeneratorVersionConfig& config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_GENERATOR_VERSION_JSON_PARSER_H_

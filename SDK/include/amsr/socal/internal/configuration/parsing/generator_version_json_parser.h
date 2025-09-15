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
/*!        \file  socal/internal/configuration/parsing/generator_version_json_parser.h
 *        \brief  JSON parser for the GeneratorVersion.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_GENERATOR_VERSION_JSON_PARSER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_GENERATOR_VERSION_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/socal/internal/configuration/generator_version_config.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {
namespace parsing {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOCAL_AutosarC++17_10-M7.3.4_usingDirectivesShallNotBeUsed
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief   Parser for the SomeIpBinding JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...).
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class GeneratorVersionJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   * \param[in]  json    JSON document to parse.
   * \param[out] config  A reference to the configuration object to be filled by the parser.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  GeneratorVersionJsonParser(::amsr::json::JsonData& json, configuration::GeneratorVersionConfig& config) noexcept;

  GeneratorVersionJsonParser(GeneratorVersionJsonParser const&) noexcept = delete;
  GeneratorVersionJsonParser& operator=(GeneratorVersionJsonParser const&) & = delete;
  GeneratorVersionJsonParser(GeneratorVersionJsonParser&&) noexcept = delete;
  GeneratorVersionJsonParser& operator=(GeneratorVersionJsonParser&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SOCAL_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ~GeneratorVersionJsonParser() noexcept override = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Callback on a new key parsed from the JSON object.
   * \param[in] json_key  Parsed key string.
   * \return Parser result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final;

  /*!
   * \brief Callback if an unexpected event happened during JSON parsing.
   * \return Always a parser result with related error code.
   * \error ComErrc::kJsonParsingFailed  In any case.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ParserResult OnUnexpectedEvent() noexcept final;

 private:
  /*!
   * \brief JSON key for the generator name.
   */
  static constexpr ::amsr::core::StringView kGeneratorNameKey{"name"_sv};

  /*!
   * \brief JSON key for the generator commit ID.
   */
  static constexpr ::amsr::core::StringView kGeneratorCommitIdKey{"commitId"_sv};

  /*!
   * \brief Parse the generator name from JSON.
   * \return Parser result.
   * \error SomeIpBindingErrc::json_parsing_failure  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ParserResult ParseGeneratorName() noexcept;

  /*!
   * \brief Parse the generator commit ID.
   * \return Parser result.
   * \error SomeIpBindingErrc::json_parsing_failure  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ParserResult ParseGeneratorCommitId() noexcept;

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  configuration::GeneratorVersionConfig& config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_GENERATOR_VERSION_JSON_PARSER_H_

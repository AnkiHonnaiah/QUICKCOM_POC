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
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_GENERATOR_VERSION_JSON_PARSER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_GENERATOR_VERSION_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ipc_binding/internal/config/parser/json_parser_util.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/runtime_configuration/generator_version_config.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief   Parser for the IpcBinding generator version JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...).
 *
 * \steady FALSE
 *
 * \unit IpcBinding::RuntimeConfigJsonParser
 */
class GeneratorVersionJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*! Type-alias for GeneratorVersionConfig */
  using GeneratorVersionConfig = amsr::ipc_binding_core::internal::runtime_configuration::GeneratorVersionConfig;

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
  GeneratorVersionJsonParser(JsonData& json, GeneratorVersionConfig& config) noexcept;

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
  // VCA_IPCB_CLASS_DECLARATION
  ~GeneratorVersionJsonParser() noexcept final = default;

  GeneratorVersionJsonParser(GeneratorVersionJsonParser const&) noexcept = delete;
  auto operator=(GeneratorVersionJsonParser const&) noexcept -> GeneratorVersionJsonParser& = delete;
  GeneratorVersionJsonParser(GeneratorVersionJsonParser&&) noexcept = delete;
  auto operator=(GeneratorVersionJsonParser&&) noexcept -> GeneratorVersionJsonParser& = delete;

  /*!
   * \brief Callback on a new key parsed from the JSON object.
   *
   * \param[in] json_key  Parsed key string.
   *
   * \return Parser result.
   *
   * \error IpcBindingErrc::json_parsing_failure  If JSON parsing failed.
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
   * \error IpcBindingErrc::json_parsing_failure  If validation fails.
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
   * \error IpcBindingErrc::json_parsing_failure  In any case.
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
  /*!
   * \brief JSON key for the generator name.
   */
  static constexpr StringView kGeneratorNameKey{"name"_sv};

  /*!
   * \brief JSON key for the generator commit ID.
   */
  static constexpr StringView kGeneratorCommitIdKey{"commitId"_sv};

  /*!
   * \brief Parse the generator name from JSON.
   *
   * \return Parser result.
   *
   * \error IpcBindingErrc::json_parsing_failure  If JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseGeneratorName() noexcept -> ParserResult;

  /*!
   * \brief Parse the generator commit ID.
   *
   * \return Parser result.
   *
   * \error IpcBindingErrc::json_parsing_failure  If JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseGeneratorCommitId() noexcept -> ParserResult;

  std::size_t number_of_names_found_{0};      /*!< Number of key 'name' found.*/
  std::size_t number_of_commit_ids_found_{0}; /*!< Number of key 'commitId' found.*/

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  amsr::ipc_binding_core::internal::runtime_configuration::GeneratorVersionConfig& config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_GENERATOR_VERSION_JSON_PARSER_H_

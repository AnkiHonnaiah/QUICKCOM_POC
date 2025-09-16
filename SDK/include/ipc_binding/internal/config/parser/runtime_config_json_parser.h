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
/**        \file
 *        \brief  JSON parser for the IpcBinding runtime configuration.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_RUNTIME_CONFIG_JSON_PARSER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_RUNTIME_CONFIG_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding/internal/config/parser/json_parser_util.h"
#include "ipc_binding_core/internal/runtime_configuration/generator_version_config.h"
#include "ipc_binding_core/internal/runtime_configuration/runtime_config.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief   Parser for the IpcBinding JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...).
 *
 * \unit       IpcBinding::RuntimeConfigJsonParser
 * \complexity Interaction with nested parsers necessary.
 */
class RuntimeConfigJsonParser final : public amsr::json::v2::SingleObjectParser {
 public:
  /*! Type name alias for RuntimeConfig */
  using RuntimeConfig = ipc_binding_core::internal::runtime_configuration::RuntimeConfig;

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
  RuntimeConfigJsonParser(JsonData& json, RuntimeConfig& config) noexcept;

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
  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
  ~RuntimeConfigJsonParser() noexcept final = default;

  RuntimeConfigJsonParser(RuntimeConfigJsonParser const&) noexcept = delete;
  auto operator=(RuntimeConfigJsonParser const&) noexcept -> RuntimeConfigJsonParser& = delete;
  RuntimeConfigJsonParser(RuntimeConfigJsonParser&&) noexcept = delete;
  auto operator=(RuntimeConfigJsonParser&&) noexcept -> RuntimeConfigJsonParser& = delete;

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

  /*!
   * \brief Parse the generator version.
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
  auto ParseGeneratorVersion() noexcept -> ParserResult;

 private:
  /*! JSON key for the runtime processing mode. */
  static constexpr StringView kRuntimeProcessingModeKey{"runtime_processing_mode"_sv};
  /*! JSON enumeration value for the runtime processing mode 'Polling'. */
  static constexpr StringView kRuntimeProcessingModePollingValue{"Polling"_sv};
  /*! JSON enumeration value for the runtime processing mode 'ThreadDriven'. */
  static constexpr StringView kRuntimeProcessingModeThreadDrivenValue{"ThreadDriven"_sv};
  /*! JSON key for the service interfaces description. */
  static constexpr StringView kServicesKey{"services"_sv};
  /*! JSON key for the generator version. */
  static constexpr StringView kGeneratorVersionKey{"generatorVersion"_sv};

  /*!
   * \brief Parse the runtime processing mode enum from JSON.
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
  auto ParseRuntimeProcessingMode() noexcept -> ParserResult;

  /*!
   * \brief Parse the services from JSON.
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
  auto ParseServices() noexcept -> ParserResult;

  std::size_t number_of_service_keys_found_{0};            /*!< Number of key 'services' found.*/
  std::size_t number_of_generator_versions_found_{0};      /*!< Number of key 'generator_version' found. */
  std::size_t number_of_runtime_processing_mode_found_{0}; /*!< Number of key 'runtime_processing_mode' found. */

  /*! Reference to configuration struct filled by the parser. */
  ipc_binding_core::internal::runtime_configuration::RuntimeConfig& config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_RUNTIME_CONFIG_JSON_PARSER_H_

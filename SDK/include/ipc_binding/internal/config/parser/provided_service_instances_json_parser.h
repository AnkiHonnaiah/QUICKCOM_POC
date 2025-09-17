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
 *        \brief  JSON parser for a 'ProvidedServiceInstance' runtime configurations.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_PROVIDED_SERVICE_INSTANCES_JSON_PARSER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_PROVIDED_SERVICE_INSTANCES_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding/internal/config/parser/json_parser_util.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/runtime_configuration/provided_service_instance_config.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief Parser for a single required service instance configuration.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::RuntimeConfigJsonParser
 */
class ProvidedServiceInstanceJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*! Type name alias for amsr::ipc_binding_core::internal::runtime_configuration::ProvidedServiceInstanceConfig. */
  using ProvidedServiceInstanceConfig =
      ::amsr::ipc_binding_core::internal::runtime_configuration::ProvidedServiceInstanceConfig;

  /*!
   * \brief Constructor for ProvidedServiceInstanceJsonParser.
   *
   * \param[in] json    The loaded JSON data from the configuration file. The reference must stay valid as long as this
   *                    instance exists.
   * \param[in] config  Reference to service instance config. The reference must stay valid as long as this instance
   *                    exists.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  ProvidedServiceInstanceJsonParser(JsonData& json, ProvidedServiceInstanceConfig& config) noexcept;

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
  ~ProvidedServiceInstanceJsonParser() noexcept final = default;  // VCA_IPCB_MOLE_1298

  ProvidedServiceInstanceJsonParser(ProvidedServiceInstanceJsonParser const&) noexcept = delete;
  auto operator=(ProvidedServiceInstanceJsonParser const&) noexcept -> ProvidedServiceInstanceJsonParser& = delete;
  ProvidedServiceInstanceJsonParser(ProvidedServiceInstanceJsonParser&&) noexcept = delete;
  auto operator=(ProvidedServiceInstanceJsonParser&&) noexcept -> ProvidedServiceInstanceJsonParser& = delete;

 protected:
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

 private:
  /*! JSON key for instance identifier. */
  static constexpr StringView kInstanceIdentifierKey{"instance_id"_sv};
  /*! JSON key for instance specifier. */
  static constexpr StringView kInstanceSpecifierKey{"instance_specifier"_sv};
  /*! JSON key for domain number. */
  static constexpr StringView kDomainKey{"domain"_sv};
  /*! JSON key for port number. */
  static constexpr StringView kPortKey{"port"_sv};
  /*! JSON key for expected client integrity level config. */
  static constexpr StringView kExpectedIntegrityLevelKey{"expected_client_integrity_level"_sv};
  /*! JSON key for expected client integrity level QM. */
  static constexpr StringView kExpectedIntegrityLevelQM{"QM"_sv};
  /*! JSON key for expected client integrity level ASIL A. */
  static constexpr StringView kExpectedIntegrityLevelAsilA{"ASIL_A"_sv};
  /*! JSON key for expected client integrity level ASIL B. */
  static constexpr StringView kExpectedIntegrityLevelAsilB{"ASIL_B"_sv};
  /*! JSON key for expected client integrity level ASIL C. */
  static constexpr StringView kExpectedIntegrityLevelAsilC{"ASIL_C"_sv};
  /*! JSON key for expected client integrity level ASIL D. */
  static constexpr StringView kExpectedIntegrityLevelAsilD{"ASIL_D"_sv};

  /*!
   * \brief Callback to parse the value for 'instance_id' key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnInstanceIdentifier() noexcept -> ParserResult;

  /*!
   * \brief Callback to parse the value for 'instance_specifier' key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnInstanceSpecifier() noexcept -> ParserResult;

  /*!
   * \brief Callback to parse the value for 'domain' key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnDomain() noexcept -> ParserResult;

  /*!
   * \brief Callback to parse the value for 'port' key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnPort() noexcept -> ParserResult;

  /*!
   * \brief Callback to parse the array of 'expected_client_integrity_level' key.
   *
   * \return A result to indicate whether parsing should continue.
   *
   * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnExpectedIntegrityLevel() noexcept -> ParserResult;

  std::size_t number_of_instance_ids_found_{0};       /*!< Number of key 'instance_id' found.*/
  std::size_t number_of_instance_specifier_found_{0}; /*!< Number of key 'instance_specifier' found.*/
  std::size_t number_of_domain_found_{0};             /*!< Number of key 'domain' found.*/
  std::size_t number_of_port_found_{0};               /*!< Number of key 'port' found.*/
  std::size_t number_of_expected_integrity_level_found_{
      0}; /*!< Number of key 'expected_client_integrity_level' found.*/

  /*! Reference to service instance config. */
  ProvidedServiceInstanceConfig& config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_PROVIDED_SERVICE_INSTANCES_JSON_PARSER_H_

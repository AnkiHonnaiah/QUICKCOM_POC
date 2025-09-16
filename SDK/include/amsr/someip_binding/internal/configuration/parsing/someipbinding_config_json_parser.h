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
/**        \file  someipbinding_config_json_parser.h
 *        \brief  JSON parser for the SomeIpBinding configuration.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::SomeIpBindingConfigJsonParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_SOMEIPBINDING_CONFIG_JSON_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_SOMEIPBINDING_CONFIG_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/someip_binding/internal/configuration/parsing/generator_version_json_parser.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding_core/internal/error_domain.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {
namespace parsing {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief   Parser for the SomeIpBinding JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...)
 */
class SomeIpBindingConfigJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief       Overriding base class method.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> Parse() noexcept final { return ::amsr::json::v2::SingleObjectParser::Parse(); }

  /*!
   * \brief       Construct the JSON parser.
   * \details     Start parsing using Parse() API.
   * \param[in]   json    JSON document to parse.
   * \param[out]  config  A reference to the configuration object to be filled by the parser.
   * \param[out]  generator_version_config  A reference to the generator version object to be filled by the parser.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  SomeIpBindingConfigJsonParser(
      amsr::json::JsonData& json, amsr::someip_binding::internal::configuration::SomeIpBindingConfig& config,
      amsr::someip_binding::internal::configuration::GeneratorVersionConfig& generator_version_config) noexcept;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SomeIpBindingConfigJsonParser() noexcept final = default;

  SomeIpBindingConfigJsonParser(SomeIpBindingConfigJsonParser const&) noexcept = delete;
  SomeIpBindingConfigJsonParser& operator=(SomeIpBindingConfigJsonParser const&) & = delete;
  SomeIpBindingConfigJsonParser(SomeIpBindingConfigJsonParser&&) noexcept = delete;
  SomeIpBindingConfigJsonParser& operator=(SomeIpBindingConfigJsonParser&&) & = delete;

  /*!
   * \brief       Callback on a new key parsed from the JSON object.
   * \param[in]   json_key Parsed key string.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_key_not_found_failure if JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult OnKey(amsr::core::StringView json_key) noexcept final;

  /*!
   * \brief       Callback if an unexpected event happened during JSON parsing.
   * \return      Always a parser result with related error code.
   * \error       SomeIpBindingErrc::json_parsing_unexpected_event_failure in any case.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult OnUnexpectedEvent() noexcept final;

  /*!
   * \brief Callback to validate that all configuration elements have been accepted.
   * \return either nothing or the error.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> Finalize() noexcept final;

 private:
  /*!
   * \brief JSON key for the runtime processing mode.
   */
  static constexpr amsr::core::StringView kRuntimeProcessingModeKey{"runtime_processing_mode"_sv};

  /*!
   * \brief JSON enumeration value for the runtime processing mode 'Polling'.
   */
  static constexpr amsr::core::StringView kRuntimeProcessingModePollingValue{"Polling"_sv};

  /*!
   * \brief JSON enumeration value for the runtime processing mode 'ThreadDriven'.
   */
  static constexpr amsr::core::StringView kRuntimeProcessingModeThreadDrivenValue{"ThreadDriven"_sv};

  /*!
   * \brief JSON key for the SomeIpDaemon connection retrial enabled value.
   */
  static constexpr amsr::core::StringView kSomeIpDaemonConnectionRetrialEnabledKey{
      "someipdaemon_connection_retrial_enabled"_sv};

  /*!
   * \brief JSON key for the SomeIpDaemon connection retrial interval ms value.
   */
  static constexpr amsr::core::StringView kSomeIpDaemonConnectionRetrialIntervalMsKey{
      "someipdaemon_connection_retrial_interval_ms"_sv};

  /*!
   * \brief JSON key for the maximum size of the IPC transmission queue.
   */
  static constexpr amsr::core::StringView kIpcMaxQueueSizeKey{"ipc_max_queue_size"_sv};

  /*!
   * \brief JSON key for the service interfaces description.
   */
  static constexpr amsr::core::StringView kServicesKey{"services"_sv};

  /*!
   * \brief JSON key for the e2e profiles json array.
   */
  static constexpr amsr::core::StringView kE2EProfilesKey{"e2e_profiles"_sv};

  /*!
   * \brief JSON key for the ipc channels json array.
   */
  static constexpr amsr::core::StringView kIpcChannelsKey{"ipc_channels"_sv};

  /*!
   * \brief JSON key for the generator version.
   */
  static constexpr amsr::core::StringView kGeneratorVersionKey{"generatorVersion"_sv};

  /*!
   * \brief       Parse the runtime processing mode enum from JSON.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_string_parsing_failure
   *              String parsing failed.
   *              SomeIpBindingErrc::json_parsing_unsupported_runtime_processing_mode_failure
   *              JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseRuntimeProcessingMode() noexcept;

  /*!
   * \brief       Parse the SomeIpDaemon address port.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseSomeIpDaemonConnectionRetrialEnabled() noexcept;

  /*!
   * \brief       Parse the SomeIpDaemon connection retrial enabled.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseSomeIpDaemonConnectionRetrialIntervalMs() noexcept;

  /*!
   * \brief       Parse the maximum size of the IPC transmission queue.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseIpcMaxQueueSize() noexcept;

  /*!
   * \brief       Parse the configuration for service interfaces.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseServices() noexcept;

  /*!
   * \brief       Parse the configuration for e2e profiles.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_duplicate_e2e_profile_shortname_in_list_failure
   *              if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseE2EProfiles() noexcept;

  /*!
   * \brief       Parse the configuration of IPC channels.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseIpcChannels() noexcept -> ParserResult;

  /*!
   * \brief       Parse the generator version.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseGeneratorVersion() noexcept;

  /*!
   * \brief       Check in instances that e2eprop points to an existing e2e profile.
   * \return      a value if all references point to existing e2e profiles.
   * \error       SomeIpBindingErrc::json_parsing_e2e_profile_shortname_not_found_failure if e2eprop points to an e2e
   *              profile that does not exist
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> ValidateReferencedE2EProfilesInE2EProps() const noexcept;

  /*!
   * \brief     Map error code to binding specific JSON parsing failure error code.
   * \param[in] someip_binding_error_code SomeIp ErrorCode to be mapped to.
   * \param[in] error_code ErrorCode to be mapped.
   * \return    Mapped ErrorCode
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::ErrorCode MapToBindingError(
      ::amsr::someip_binding_core::internal::SomeIpBindingErrc const& someip_binding_error_code,
      amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  amsr::someip_binding::internal::configuration::SomeIpBindingConfig& config_;

  /*!
   * \brief Reference to generator version struct filled by the parser.
   */
  amsr::someip_binding::internal::configuration::GeneratorVersionConfig& generator_version_config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_SOMEIPBINDING_CONFIG_JSON_PARSER_H_

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
/*!        \file  configuration_parser.h
 *        \brief  Parser for someip_config.json (formerly application-gen.json).
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_CONFIGURATION_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_CONFIGURATION_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/configuration_object.h"
#include "amsr/someip_daemon_core/configuration/model/someip_config_validator.h"
#include "amsr/someip_daemon_core/configuration/parsing/generator_version_parser.h"
#include "amsr/someip_daemon_core/configuration/parsing/ipc_channels_parser.h"
#include "amsr/someip_daemon_core/configuration/parsing/network_endpoints_parser.h"
#include "amsr/someip_daemon_core/configuration/parsing/secure_com_parser.h"
#include "amsr/someip_daemon_core/configuration/parsing/services_parser.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace parsing {

/*!
 * \brief Parses full 'someip_config.json' from JSON.
 */
class ConfigurationParser final : public amsr::json::Parser<ConfigurationParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<ConfigurationParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::SomeIpConfigValidator;

  /*!
   * \brief Type alias for validation result.
   */
  using ValidationResult = typename Validation::ValidationResult;

  /*!
   * \brief Type alias for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Type alias for ApplicationGenConfigObject
   */
  using ApplicationGenConfigObject = configuration::model::ConfigurationObject;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Create parser for a JSON document which writes parsed elements to given containers.
   * \details Inputs for this parser are the containers of 'services', 'ipc_channels', 'network_endpoints',
   *          'secure_communication' and the 'generatorVersion' objects to fill.
   * \param[in] file JSON document to parse.
   * \param[out] services_container Reference to the container element where to store 'services'.
   * \param[out] ipc_channels_container Reference to the container element where to store 'ipc_channels'.
   * \param[out] network_end_points_container Reference to the container element where to store 'network_endpoints'.
   * \param[out] secure_com_container Reference to the container element where to store 'secure_communication'.
   * \param[out] generator_version Reference to the struct element where to store 'generatorVersion'.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ConfigurationParser(amsr::json::JsonData& file, ConfigurationTypesAndDefs::ServiceContainer& services_container,
                      ConfigurationTypesAndDefs::IpcChannelsContainer& ipc_channels_container,
                      ConfigurationTypesAndDefs::NetworkEndpointContainer& network_end_points_container,
                      ConfigurationTypesAndDefs::SecureCom& secure_com_container,
                      ConfigurationTypesAndDefs::GeneratorVersion& generator_version) noexcept
      : Parser{file},
        logger_(someip_daemon_core::logging::kConfigurationLoggerContextId,
                someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                ara::core::StringView{"ApplicationGenConfigParser"}),
        services_container_{services_container},
        ipc_channels_container_{ipc_channels_container},
        network_end_points_container_{network_end_points_container},
        secure_com_container_{secure_com_container},
        generator_version_{generator_version},
        application_gen_config_element_wrapper_{services_container, ipc_channels_container,
                                                network_end_points_container, secure_com_container, generator_version} {
  }

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ConfigurationParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ConfigurationParser(ConfigurationParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ConfigurationParser(ConfigurationParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ConfigurationParser const&) -> ConfigurationParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ConfigurationParser&&) -> ConfigurationParser& = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Callback on end array found in JSON.
   * \param[in] val value returned by the parser.
   * \return An error code, because it indicates an erroneously configured key, as the application_gen_config.json
   * should not contain any arrays.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndArray(std::size_t val) const noexcept;

  /*!
   * \brief Callback on number found in JSON.
   * \param[in] num value returned by the parser.
   * \return An error code, because it indicates an erroneously configured key, as the application_gen_config.json
   * should not contain any numbers.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) const noexcept;

  /*!
   * \brief Callback on string found in JSON.
   * \param[in] val value returned by the parser.
   * \return An error code, because it indicates an erroneously configured key, as the application_gen_config.json
   * should not contain any strings.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) const noexcept;

  /*!
   * \brief Callback on null found in JSON.
   * \return An error code, because it indicates an erroneously configured key, as the application_gen_config.json
   * should not contain any nulls.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNull() const noexcept;

  /*!
   * \brief Callback on bool found in JSON.
   * \param[in] val value returned by the parser.
   * \return An error code, because it indicates an erroneously configured key, as the application_gen_config.json
   * should not contain any bools.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnBool(bool val) const noexcept;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) const;

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnUnexpectedEvent() noexcept;

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the POD structure for services instances.
   */
  ConfigurationTypesAndDefs::ServiceContainer& services_container_;

  /*!
   * \brief Reference to the POD structure for IPC channels instances.
   */
  ConfigurationTypesAndDefs::IpcChannelsContainer& ipc_channels_container_;

  /*!
   * \brief Reference to the POD structure for network end points.
   */
  ConfigurationTypesAndDefs::NetworkEndpointContainer& network_end_points_container_;

  /*!
   * \brief Reference to the POD structure for secure communication instances.
   */
  ConfigurationTypesAndDefs::SecureCom& secure_com_container_;

  /*!
   * \brief Reference to the POD structure for generator version.
   */
  ConfigurationTypesAndDefs::GeneratorVersion& generator_version_;

  /*!
   * \brief Intermediate object for all sub-hierarchies of someip_config.json.
   */
  ApplicationGenConfigObject application_gen_config_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_CONFIGURATION_PARSER_H_

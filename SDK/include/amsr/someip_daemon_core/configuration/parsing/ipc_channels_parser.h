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
/*!        \file  ipc_channels_parser.h
 *        \brief  Sub-parser for IPC channel objects represented in JSON.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_IPC_CHANNELS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_IPC_CHANNELS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/ipc_channel_validator.h"
#include "amsr/someip_daemon_core/configuration/parsing/provided_service_instances_parser.h"
#include "amsr/someip_daemon_core/configuration/parsing/required_service_instances_parser.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace parsing {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals.
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief Parses IPC channel elements from JSON into a container of IPC channels.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed IPC channel meets all constraints.
 * 3. Push to IPC channels container if the IPC channel configuration is valid.
 */
class IpcChannelsParser final : public amsr::json::Parser<IpcChannelsParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<IpcChannelsParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::IpcChannelValidator;

  /*!
   * \brief Type alias for validation result.
   */
  using ValidationResult = typename Validation::ValidationResult;

  /*!
   * \brief Type alias for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Type alias for easy access to ParserState.
   */
  using ParserState = amsr::json::ParserState;

  /*!
   * \brief Type alias for the parser base used multiple times.
   */
  using Base = amsr::json::Parser<IpcChannelsParser>;

  /*!
   * \brief Domain JSON key.
   */
  static constexpr CStringView kDomainKey{"domain"_sv};

  /*!
   * \brief Port JSON key.
   */
  static constexpr CStringView kPortKey{"port"_sv};

  /*!
   * \brief Required Service Instances JSON key.
   */
  static constexpr CStringView kRequiredServiceInstancesKey{"required_service_instances"_sv};

  /*!
   * \brief Provided Service Instances JSON key.
   */
  static constexpr CStringView kProvidedServiceInstancesKey{"provided_service_instances"_sv};

  /*!
   * \brief Input for this parser is the container of IPC channels from the config to fill.
   * \param[in]  file JSON document to parse.
   * \param[out] ipc_channels A reference to the container which is extended on every intermediate IPC channels element
   *             that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  IpcChannelsParser(amsr::json::JsonData &file, ConfigurationTypesAndDefs::IpcChannelsContainer &ipc_channels) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~IpcChannelsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  IpcChannelsParser(IpcChannelsParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  IpcChannelsParser(IpcChannelsParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(IpcChannelsParser const &) -> IpcChannelsParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(IpcChannelsParser &&) -> IpcChannelsParser & = delete;

  /*!
   * \brief Number callback is called by the parser for the Domain and the Port.
   * \param[in] num value parsed.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnNumber(amsr::json::JsonNumber num) noexcept -> ParserResult;

  /*!
   * \brief Start object means that a new IPC channel object was found.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnStartObject() noexcept -> ParserResult;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param[in] key The current key in JSON.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnKey(CStringView key) noexcept -> ParserResult;

  /*!
   * \brief Callback on string found in the JSON.
   * \param[in] val JSON data to parse
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnString(CStringView val) const noexcept -> ParserResult;

  /*!
   * \brief End Array is called if all IPC channels have been parsed. This is the exit criteria for this sub-parser.
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto OnEndArray(std::size_t) noexcept -> ParserState;

  /*!
   * \brief End Object is called on each IPC channel end.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnEndObject(std::size_t) noexcept -> ParserResult;

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto OnUnexpectedEvent() noexcept -> ParserResult;

 private:
  /*!
   * \brief On every new IPC channel, object attributes have to be reset.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void PreProcess() noexcept;

  /*!
   * \brief Validate the parsed IPC channel object.
   * \details Validation implemented as protected API to support validation-independent test implementation.
   * \return Validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto Validate() const noexcept -> model::IpcChannelValidator::ValidationResult;

  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the IPC channel container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::IpcChannelsContainer &ipc_channels_;

  /*!
   * \brief This POD structure will be added to the container of IPC channels, if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::IpcChannel ipc_channel_storage_;

  /*!
   * \brief Current intermediate IPC channel object to fill with information.
   */
  model::IpcChannelConfigObject ipc_channel_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_IPC_CHANNELS_PARSER_H_

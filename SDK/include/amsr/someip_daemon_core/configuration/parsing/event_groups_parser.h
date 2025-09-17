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
/*!        \file  event_groups_parser.h
 *        \brief  Sub-Parser for event groups.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_GROUPS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_GROUPS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/event_group_validator.h"
#include "amsr/someip_daemon_core/configuration/parsing/event_id_container_parser.h"
#include "vac/container/string_literals.h"

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
 * \brief Parses event groups from JSON format into a container of event groups.
 */
class EventGroupsParser final : public amsr::json::Parser<EventGroupsParser> {
 public:
  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::EventGroupValidator;

  /*!
   * \brief Type alias for the parser base used multiple times.
   */
  using EventGroupsParserBase = amsr::json::Parser<EventGroupsParser>;

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
   * \brief events JSON key.
   */
  static constexpr CStringView kEventsKey{"events"_sv};

  /*!
   * \brief Id JSON key.
   */
  static constexpr CStringView kIdKey{"id"_sv};

  /*!
   * \brief Input for this parser is the container of event groups from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] event_groups A reference to the container, which is extended for every valid event group parsed.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  EventGroupsParser(amsr::json::JsonData& file, ConfigurationTypesAndDefs::EventgroupContainer& event_groups) noexcept
      : EventGroupsParserBase{file},
        event_groups_{event_groups},
        event_group_storage_{},
        event_group_element_wrapper_{event_group_storage_},
        logger_{someip_daemon_core::logging::kConfigurationLoggerContextId,
                someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                ara::core::StringView{"EventGroupsParser"}} {}

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  ~EventGroupsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  EventGroupsParser(EventGroupsParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  EventGroupsParser(EventGroupsParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(EventGroupsParser const&) -> EventGroupsParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(EventGroupsParser&&) -> EventGroupsParser& = delete;

  /*!
   * \brief On every new event group element, member variables have to be reset.
   * \pre -
   * \context ANY
   * \reentrant TRUE
   * \steady FALSE
   */
  void PreProcess() noexcept;

  /*!
   * \brief The post processing sequence will be invoked after a complete event group entry from JSON is parsed.
   * \details Only place the parsed event group into the event group container, if all mandatory elements are set.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  model::EventGroupValidator::ValidationResult PostProcess() const noexcept;

  /*!
   * \brief Number callback for the event group object.
   * \param[in] num The value only could be the ID of this event group.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Callback on string event.
   * \param[in] val JSON data to parse
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used for spawning sub-parsers within event groups.
   * \param[in] key Current key.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief If this is called by the parser, the array with the events part of this event group is called.
   * \return ParserResult with ParserState::kFinished.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static amsr::json::ParserState OnEndArray(std::size_t) noexcept;

  /*!
   * \brief Start object means that a new event group object was found.
   * \return ParserResult with ParserState::kRunning.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief End Object is called on each event group end. A closing } is received.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept;

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
   * \brief Reference to the container of event groups which is extended after the validation step.
   */
  ConfigurationTypesAndDefs::EventgroupContainer& event_groups_;

  /*!
   * \brief This POD structure will be added to the container of event groups, if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::Eventgroup event_group_storage_;

  /*!
   * \brief Current intermediate event group object to fill with information.
   */
  model::EventGroupConfigObject event_group_element_wrapper_;

  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  someip_daemon_core::logging::AraComLogger logger_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_GROUPS_PARSER_H_

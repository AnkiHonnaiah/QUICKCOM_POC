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
/*!        \file  events_parser.h
 *        \brief  Sub-parser for events.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENTS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENTS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/event_validator.h"
#include "amsr/someip_daemon_core/configuration/parsing/event_someip_tp_parser.h"
#include "amsr/someip_daemon_core/configuration/parsing/overload_protection_filter_parser.h"
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
 * \brief Parses event elements from JSON into a container of events.
 */
class EventsParser final : public amsr::json::Parser<EventsParser> {
 public:
  /*!
   * \brief Type alias for the parser base used multiple times.
   */
  using EventsParserBase = amsr::json::Parser<EventsParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::EventValidator;

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
   * \brief Proto JSON key.
   */
  static constexpr CStringView kProtoKey{"proto"_sv};

  /*!
   * \brief Field JSON key.
   */
  static constexpr CStringView kFieldKey{"field"_sv};

  /*!
   * \brief Signal Based JSON key.
   */
  static constexpr CStringView kSignalBasedKey{"signal_based"_sv};

  /*!
   * \brief Id JSON key.
   */
  static constexpr CStringView kIdKey{"id"_sv};

  /*!
   * \brief someip_tp JSON key.
   */
  static constexpr CStringView kSomeIpTpKey{"someip_tp"_sv};

  /*!
   * \brief Name JSON key.
   */
  static constexpr CStringView kNameKey{"name"_sv};

  /*!
   * \brief TCP protocol.
   */
  static constexpr CStringView kTcp{"tcp"_sv};

  /*!
   * \brief UDP protocol.
   */
  static constexpr CStringView kUdp{"udp"_sv};

  /*!
   * \brief OverloadProtectionFilter JsonKey.
   */
  static constexpr CStringView kOverloadProtectionFilterKey{"overload_protection_filter"_sv};

  /*!
   * \brief TimesStampFlag JsonKey.
   */
  static constexpr CStringView kTimeStampFlagKey{"timestamp_flag"_sv};

  /*!
   * \brief Input for this parser is the container of events from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] events A reference to the events container, which is extended for every
   * valid event parsed by this parser.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  EventsParser(amsr::json::JsonData& file, ConfigurationTypesAndDefs::EventContainer& events) noexcept
      : EventsParserBase{file},
        events_{events},
        event_storage_{0U,
                       false,
                       ConfigurationTypesAndDefs::Protocol::kTCP,
                       false,
                       {false, static_cast<std::chrono::nanoseconds>(0U), 0U, 0U, 0U, 1U},
                       {static_cast<std::chrono::nanoseconds>(0U)},
                       false},
        event_element_wrapper_{event_storage_},
        logger_{someip_daemon_core::logging::kConfigurationLoggerContextId,
                someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                ara::core::StringView{"EventsParser"}} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~EventsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  EventsParser(EventsParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  EventsParser(EventsParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(EventsParser const&) -> EventsParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(EventsParser&&) -> EventsParser& = delete;

  /*!
   * \brief On every new event element variables have to be reset.
   * \steady FALSE
   */
  void PreProcess() noexcept;

  /*!
   * \brief The post processing sequence will be invoked after a complete method entry from JSON is parsed.
   * \details Only place the parsed method into the method container, if all mandatory elements are set.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  model::EventValidator::ValidationResult PostProcess() const noexcept;

  /*!
   * \brief On string event.
   * \param[in] val Parsed string value
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept;

  /*!
   * \brief Number callback for one method object.
   * \param[in] num Parsed numeric value.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Bool callback.
   * \param[in] val Parsed boolean value
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnBool(bool val) noexcept;

  /*!
   * \brief Used for spawning sub-parsers within events.
   * \param[in] key The current key in JSON.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Start object means that a new event object was found in JSON.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief On end array the array of events is finished. Exit criteria for
   * the array of event objects.
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static amsr::json::ParserState OnEndArray(std::size_t) noexcept;

  /*!
   * \brief End Object is called on each event end.
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
   * \brief Reference to the container of events that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::EventContainer& events_;

  /*!
   * \brief This POD structure will be added to the container of events, if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::Event event_storage_;

  /*!
   * \brief Current intermediate event object to fill with information.
   */
  model::EventConfigObject event_element_wrapper_;

  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  someip_daemon_core::logging::AraComLogger logger_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENTS_PARSER_H_

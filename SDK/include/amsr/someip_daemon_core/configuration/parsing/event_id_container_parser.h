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
/*!        \file  event_id_container_parser.h
 *        \brief  Sub-Parser for the event_id container for one event group.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_ID_CONTAINER_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_ID_CONTAINER_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "ara/core/string_view.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace parsing {

// VECTOR Disable AutosarC++17_10-A10.2.1: MD_SomeIpDaemon_AutosarC++17_10_A10.2.1_redefinition_of_non_virtual_function

/*!
 * \brief The following sub-parser is spawned by the EventgroupsParser for the array of event IDs belonging to this
 * event group.
 */
class EventIdContainerParser final : public amsr::json::Parser<EventIdContainerParser> {
 public:
  /*!
   * \brief Type alias for the parser base used multiple times.
   */
  using EventIdContainerParserBase = amsr::json::Parser<EventIdContainerParser>;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Type alias for easy access to ParserState.
   */
  using ParserState = amsr::json::ParserState;

  /*!
   * \brief Parser for EventIdContainer
   * \param[in] file JSON document to parse.
   * \param[out] container A reference to the container, which is extended for every valid event id parsed.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  EventIdContainerParser(amsr::json::JsonData& file, ConfigurationTypesAndDefs::EventIdContainer& container) noexcept
      : EventIdContainerParserBase{file}, events_container_{container} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~EventIdContainerParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  EventIdContainerParser(EventIdContainerParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  EventIdContainerParser(EventIdContainerParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(EventIdContainerParser const&) -> EventIdContainerParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(EventIdContainerParser&&) -> EventIdContainerParser& = delete;

  /*!
   * \brief Number callback is called for every ID of an event contained.
   * \param[in] num The event id.
   * \return ParserResult with ParserState::kRunning or an error code.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept {
    ParserResult result{amsr::json::ParserState::kRunning};

    if (is_inside_sub_array_) {
      ara::core::Optional<someip_protocol::internal::EventId> id{num.As<someip_protocol::internal::EventId>()};
      if (id.has_value()) {
        events_container_.push_back(*id);
      } else {
        result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
        logger_.LogError([](ara::log::LogStream& s) { s << "Invalid format for event ID."; },
                         static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      logger_.LogError(
          [](ara::log::LogStream& s) { s << "Events not in expected format; valid format: 'events': []."; },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Callback on string event.
   * \return An error code as the string values are not expected.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(vac::container::CStringView) {
    // Since there are no keys which should have a String value, return an error code here.
    ParserResult const result{ParserResult{SomeIpDaemonErrc::json_parsing_failed}};

    logger_.LogError(
        [this](ara::log::LogStream& s) {
          vac::container::CStringView const current_key{this->GetCurrentKey()};
          char const* const current_key_cstr{current_key.c_str()};
          s << "Unexpected string key found: " << current_key_cstr << ".";
        },
        static_cast<char const*>(__func__), __LINE__);

    return result;
  }

  /*!
   * \brief Entry criteria for this sub-parser.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartArray() noexcept {
    is_inside_sub_array_ = true;

    return ParserResult{amsr::json::ParserState::kRunning};
  }

  /*!
   * \brief Exit criteria for this sub-parser.
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnEndArray(std::size_t) noexcept { return ParserResult{amsr::json::ParserState::kFinished}; }

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnUnexpectedEvent() { return amsr::json::ParserState::kRunning; }

 private:
  /*!
   * \brief Events container
   */
  ConfigurationTypesAndDefs::EventIdContainer& events_container_;

  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kConfigurationLoggerContextId,
                                                    someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                                                    ara::core::StringView{"EventIdContainerParser"}};

  /*!
   * \brief Flag to distinguish if parser is inside a sub-value array.
   */
  bool is_inside_sub_array_{false};
};

// VECTOR Enable AutosarC++17_10-A10.2.1

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_ID_CONTAINER_PARSER_H_

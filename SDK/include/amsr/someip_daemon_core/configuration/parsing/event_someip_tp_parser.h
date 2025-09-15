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
/*!        \file  event_someip_tp_parser.h
 *        \brief  Parser for SOME/IP-TP configuration of events.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_SOMEIP_TP_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_SOMEIP_TP_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/event_someip_tp_object.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "vac/container/c_string_view.h"
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
 * \brief Parses SOME/IP-TP elements for service events from JSON.
 */
class EventSomeIpTpParser final : public amsr::json::Parser<EventSomeIpTpParser> {
 public:
  /*!
   * \brief Type-alias for the parser base used multiple times.
   */
  using EventSomeIpTpParserBase = amsr::json::Parser<EventSomeIpTpParser>;

  /*!
   * \brief Type alias for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief separation_time_ns JSON key.
   */
  static constexpr CStringView kSeparationTimeKey{"separation_time_ns"_sv};

  /*!
   * \brief maximum_segment_length JSON key.
   */
  static constexpr CStringView kMaximumSegmentLengthKey{"maximum_segment_length"_sv};

  /*!
   * \brief maximum_rx_message_length JSON key.
   */
  static constexpr CStringView kMaximumRxMessageLengthKey{"maximum_rx_message_length"_sv};

  /*!
   * \brief rx_buffer_count JSON key.
   */
  static constexpr CStringView kRxBufferCountKey{"rx_buffer_count"_sv};

  /*!
   * \brief burst_size JSON key.
   */
  static constexpr CStringView kBurstSizeKey{"burst_size"_sv};

  /*!
   * \brief Input for this parser is the SOME/IP-TP object to fill.
   * \param[in] file JSON document to parse.
   * \param[out] someip_tp A reference to the POD SOME/IP-TP object that is written to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  EventSomeIpTpParser(amsr::json::JsonData& file, ConfigurationTypesAndDefs::EventSomeIpTp& someip_tp) noexcept
      : EventSomeIpTpParserBase{file},
        someip_tp_element_wrapper_{someip_tp},
        logger_{someip_daemon_core::logging::kConfigurationLoggerContextId,
                someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                ara::core::StringView{"EventSomeIpTpParser"}} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~EventSomeIpTpParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  EventSomeIpTpParser(EventSomeIpTpParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  EventSomeIpTpParser(EventSomeIpTpParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(EventSomeIpTpParser const&) -> EventSomeIpTpParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(EventSomeIpTpParser&&) -> EventSomeIpTpParser& = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to detect unknown JSON keys.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kRunning or error code if any unknown JSON key was detected.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) const noexcept;

  /*!
   * \brief End Object will end this parser - exit criteria.
   * \return amsr::json::ParserState::kFinished
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept;

  /*!
   * \brief Number callback is called by the parser for the separation times and segment lengths.
   * \param[in] num Parsed value belonging to the key.
   * \return ParserResult with ParserState::kRunning or an error code.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Callback on string.
   * \param[in] val JSON data to parse
   * \return ParserResult with ParserState::kRunning or an error code.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept;

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
   * \brief Current intermediate event SOME/IP-TP object to fill with information.
   */
  model::EventSomeIpTpConfigObject someip_tp_element_wrapper_;

  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief For accessing someip_tp_element_wrapper_, which is private. The wrapper is used to test GetIsActive, which
   * is never called by the parser.
   */
  FRIEND_TEST(UT__EventSomeIpTpParser, ParseEventSomeIpTp);
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_EVENT_SOMEIP_TP_PARSER_H_

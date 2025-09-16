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
/*!        \file  method_someip_tp_parser.h
 *        \brief  Parser for SOME/IP-TP JSON configuration of methods.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_METHOD_SOMEIP_TP_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_METHOD_SOMEIP_TP_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/method_someip_tp_object.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace parsing {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief Parses a SOME/IP-TP object of a service method from JSON.
 */
class MethodSomeIpTpParser final : public amsr::json::Parser<MethodSomeIpTpParser> {
 public:
  /*!
   * \brief Type-alias for the parser base used multiple times within this class.
   */
  using MethodSomeIpTpParserBase = amsr::json::Parser<MethodSomeIpTpParser>;

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
   * \brief separation_time_request JSON key.
   */
  static constexpr CStringView kSeparationTimeRequestKey{"separation_time_request_ns"_sv};

  /*!
   * \brief separation_time_response JSON key.
   */
  static constexpr CStringView kSeparationTimeResponseKey{"separation_time_response_ns"_sv};

  /*!
   * \brief maximum_segment_length_request JSON key.
   */
  static constexpr CStringView kMaxSegmentLengthRequestKey{"maximum_segment_length_request"_sv};

  /*!
   * \brief maximum_segment_length_response JSON key.
   */
  static constexpr CStringView kMaxSegmentLengthResponseKey{"maximum_segment_length_response"_sv};

  /*!
   * \brief maximum_rx_message_length JSON key.
   */
  static constexpr CStringView kMaxRxMessageLengthKey{"maximum_rx_message_length"_sv};

  /*!
   * \brief rx_buffer_count JSON key.
   */
  static constexpr CStringView kRxBufferCountKey{"rx_buffer_count"_sv};

  /*!
   * \brief burst_size_request JSON key.
   */
  static constexpr CStringView kBurstSizeRequestKey{"burst_size_request"_sv};

  /*!
   * \brief burst_size_response JSON key.
   */
  static constexpr CStringView kBurstSizeResponseKey{"burst_size_response"_sv};

  /*!
   * \brief Input for this parser is the container of SOME/IP-TP from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] someip_tp A reference to the internal SOME/IP-TP data structure.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  MethodSomeIpTpParser(amsr::json::JsonData& file, ConfigurationTypesAndDefs::MethodSomeIpTp& someip_tp) noexcept
      : MethodSomeIpTpParserBase{file},
        someip_tp_element_wrapper_{someip_tp},
        logger_{someip_daemon_core::logging::kConfigurationLoggerContextId,
                someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                ara::core::StringView{"MethodSomeIpTpParser"}} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~MethodSomeIpTpParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  MethodSomeIpTpParser(MethodSomeIpTpParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  MethodSomeIpTpParser(MethodSomeIpTpParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(MethodSomeIpTpParser const&) -> MethodSomeIpTpParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(MethodSomeIpTpParser&&) -> MethodSomeIpTpParser& = delete;

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
   * \brief End Object will end this parser.
   * \return amsr::json::ParserState::kFinished as the SOME/IP-TP object ends at this point. It will return to
   * the parent parser.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept;

  /*!
   * \brief Number callback of the SOME/IP-TP parser of methods is called for the separation times and the maximum
   * lengths of segments and re-assembled message.
   * \param[in] num parsed numeric value.
   * \return true in case of parsing correct elements.
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
   * \return An error code, because it indicates an erroneously configured key, as this parser should not
   * handle any strings.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) const noexcept;

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
   * \brief Set the default separation time request attribute if not set
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultSeparationTimeRequestIfNotSet() noexcept;

  /*!
   * \brief Set the default separation time response attribute if not set
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultSeparationTimeResponseIfNotSet() noexcept;

  /*!
   * \brief Set the default maximum segment length request attribute if not set
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultMaximumSegmentLengthRequestIfNotSet() noexcept;

  /*!
   * \brief Set the default segment length response attribute if not set
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultMaximumSegmentLengthResponseIfNotSet() noexcept;

  /*!
   * \brief Set the default maximum rx message length attribute if not set
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultMaximumRxMessageLengthIfNotSet() noexcept;

  /*!
   * \brief Set the default burst size request attribute if not set
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultBurstSizeRequestIfNotSet() noexcept;

  /*!
   * \brief Set the default burst size response attribute if not set
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultBurstSizeResponseIfNotSet() noexcept;

  /*!
   * \brief Set the default number of rx buffers attribute if not set
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultRxBufferCountIfNotSet() noexcept;

  /*!
   * \brief Current intermediate method SOME/IP-TP object to fill with information.
   */
  model::MethodSomeIpTpConfigObject someip_tp_element_wrapper_;

  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  amsr::someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief To access someip_tp_element_wrapper_ which is private. This is used to test
   * MethodSomeIpTpConfigObject::GetIsActive which is never called by the parser.
   */
  FRIEND_TEST(UT__MethodSomeIpTpParser, ParseMethodSomeIpTp);
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_METHOD_SOMEIP_TP_PARSER_H_

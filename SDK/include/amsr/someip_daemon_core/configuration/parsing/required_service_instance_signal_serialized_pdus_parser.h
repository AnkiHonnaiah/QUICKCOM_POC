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
/*!        \file  required_service_instance_signal_serialized_pdus_parser.h
 *        \brief  JSON parser for 'RequiredServiceInstanceSignalSerializedPdus'.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDUS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDUS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_signal_serialized_pdu_object.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_signal_serialized_pdu_validator.h"
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
 * \brief Parses a container of 'required service instance signal serialized pdu'.
 */
class RequiredServiceInstanceSignalSerializedPduParser final
    : public amsr::json::Parser<RequiredServiceInstanceSignalSerializedPduParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<RequiredServiceInstanceSignalSerializedPduParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::RequiredServiceInstanceSignalSerializedPduValidator;

  /*!
   * \brief Type-alias for validation result.
   */
  using ValidationResult = typename Validation::ValidationResult;

  /*!
   * \brief Type-alais for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief PDU ID key.
   */
  static constexpr CStringView kPduIdKey{"pdu_id"_sv};

  /*!
   * \brief Event ID key.
   */
  static constexpr CStringView kEventIdKey{"event_id"_sv};

  /*!
   * \brief Input for this parser is the container of 'SignalSerializedPdu' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] pdus A reference to the element which is written by the parser.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  RequiredServiceInstanceSignalSerializedPduParser(
      amsr::json::JsonData& file, ConfigurationTypesAndDefs::SignalSerializedPduContainer& pdus) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstanceSignalSerializedPduParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstanceSignalSerializedPduParser(RequiredServiceInstanceSignalSerializedPduParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstanceSignalSerializedPduParser(RequiredServiceInstanceSignalSerializedPduParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(RequiredServiceInstanceSignalSerializedPduParser const&)
      -> RequiredServiceInstanceSignalSerializedPduParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(RequiredServiceInstanceSignalSerializedPduParser&&)
      -> RequiredServiceInstanceSignalSerializedPduParser& = delete;

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
   * \brief Callback on end of a JSON array.
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnEndArray(std::size_t) noexcept;

  /*!
   * \brief Callback on start of a JSON object.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept;

  /*!
   * \brief Callback on number found in JSON.
   * \param[in] num value returned by the parser.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Callback on string found in JSON.
   * \param[in] val JSON data to parse
   * \return An error code, because it indicates an erroneously configured key, as this parser should not
   * handle any strings.
   * \error amsr::json::JsonErrc::kUserValidationFailed
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
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::SignalSerializedPduContainer& pdus_;

  /*!
   * \brief This POD structure will be added to the container if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::SignalSerializedPdu pdu_;

  /*!
   * \brief Element wrapper object to fill with parsed information.
   */
  model::RequiredServiceInstanceSignalSerializedPduObject pdu_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDUS_PARSER_H_

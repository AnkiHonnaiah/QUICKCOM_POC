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
/*!        \file  someip_method_event_props_parser.h
 *        \brief Parser for "SomeipMethodProps" and "SomeipEventProps", as on the JSON both are a flattened.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SOMEIP_METHOD_EVENT_PROPS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SOMEIP_METHOD_EVENT_PROPS_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/reader/v1/parser.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/someip_method_event_props_object.h"
#include "amsr/someip_daemon_core/configuration/model/someip_method_event_props_validator.h"
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
 * \brief Parses 'required service instances' elements from JSON into a container of services.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'required service instances' meet all constraints.
 * 3. Push to 'required service instances' container.
 */
class SomeipMethodEventPropsParser final : public amsr::json::Parser<SomeipMethodEventPropsParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<SomeipMethodEventPropsParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::SomeipMethodEventPropsValidator;

  /*!
   * \brief Type-alias for validation result.
   */
  using ValidationResult = typename Validation::ValidationResult;

  /*!
   * \brief Type-alias for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Service ID JSON key.
   */
  static constexpr CStringView kIdKey{"id"_sv};

  /*!
   * \brief Instance ID JSON key.
   */
  static constexpr CStringView kTimeoutKey{"timeout_ns"_sv};

  /*!
   * \brief Major version JSON key.
   */
  static constexpr CStringView kIsTriggerKey{"is_trigger"_sv};

  /*!
   * \brief Type-alias for RequiredServiceInstanceConfigObject
   */
  using SomeipMethodEventPropsObject = configuration::model::SomeipMethodEventPropsObject;

  /*!
   * \brief Input for this parser is the container of 'required service instances' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] container A reference to the container.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SomeipMethodEventPropsParser(amsr::json::JsonData &file,
                               ConfigurationTypesAndDefs::SomeipMethodEventPropsContainer &container) noexcept;

  /*!
   *
   * \brief Destructor.
   * \steady FALSE
   */
  ~SomeipMethodEventPropsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SomeipMethodEventPropsParser(SomeipMethodEventPropsParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SomeipMethodEventPropsParser(SomeipMethodEventPropsParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SomeipMethodEventPropsParser const &) -> SomeipMethodEventPropsParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SomeipMethodEventPropsParser &&) -> SomeipMethodEventPropsParser & = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnKey(CStringView key) const noexcept -> ParserResult;

  /*!
   * \brief Start object means that a new required service object was found.
   * \return ParserResult with ParserState::kRunning.
   * \steady FALSE
   */
  auto OnStartObject() noexcept -> ParserResult;

  /*!
   * \brief Callback on end of JSON array.
   * \return ParserResult with ParserState::kFinished.
   * \steady FALSE
   */
  static auto OnEndArray(std::size_t) noexcept -> ParserResult;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnEndObject(std::size_t) noexcept -> ParserResult;

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
  auto OnNumber(amsr::json::JsonNumber num) noexcept -> ParserResult;

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
  auto OnString(CStringView val) const noexcept -> ParserResult;

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
  auto OnBool(bool val) noexcept -> ParserResult;

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto OnUnexpectedEvent() noexcept -> ParserResult;

 protected:
  /*!
   * \brief Validate the parsed intermediate 'required service instance' object.
   * \details Validation implemented as protected API to support validation-independent test implementation.
   * \return Validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto Validate() const noexcept -> ValidationResult;

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::SomeipMethodEventPropsContainer &container_;

  /*!
   * \brief This POD structure will be added to the container of 'required service instances', if
   * the parsed element is valid.
   */
  SomeipMethodEventPropsObject::ValueType pod_;

  /*!
   * \brief Current intermediate object to fill with information.
   */
  SomeipMethodEventPropsObject wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SOMEIP_METHOD_EVENT_PROPS_PARSER_H_

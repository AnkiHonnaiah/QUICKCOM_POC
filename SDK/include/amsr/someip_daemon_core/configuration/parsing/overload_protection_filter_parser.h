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
/*!        \file  overload_protection_filter_parser.h
 *        \brief  JSON parser for 'overload_protection_filter' for events.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_OVERLOAD_PROTECTION_FILTER_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_OVERLOAD_PROTECTION_FILTER_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/overload_protection_filter_object.h"
#include "amsr/someip_daemon_core/configuration/model/overload_protection_filter_validator.h"
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
 * \brief Parses a single 'overload_protection_filter' element from JSON.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'OverloadProtectionFilterParser' meets all constraints.
 */
class OverloadProtectionFilterParser final : public amsr::json::Parser<OverloadProtectionFilterParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<OverloadProtectionFilterParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::OverloadProtectionFilterValidator;

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
   * \brief Minimum interval JSON key.
   */
  static constexpr CStringView kMinimumIntervalKey{"min_interval_ns"_sv};

  /*!
   * \brief Type alias for OverloadProtectionFilterFilterConfigObject
   */
  using FilterConfigObject = configuration::model::OverloadProtectionFilterConfigObject;

  /*!
   * \brief Input for this parser is the container of 'OverloadProtectionFilter' from the configuration to fill.
   * \param[in] file JSON document to parse.
   * \param[out] filter A reference to the 'filter' POD structure to be filled with the parsed values from JSON.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  OverloadProtectionFilterParser(amsr::json::JsonData& file,
                                 ConfigurationTypesAndDefs::OverloadProtectionFilter& filter) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~OverloadProtectionFilterParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  OverloadProtectionFilterParser(OverloadProtectionFilterParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  OverloadProtectionFilterParser(OverloadProtectionFilterParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(OverloadProtectionFilterParser const&) -> OverloadProtectionFilterParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(OverloadProtectionFilterParser&&) -> OverloadProtectionFilterParser& = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kRunning or error code if sub-parsers failed.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) const noexcept;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) const noexcept;

  /*!
   * \brief Callback on number event.
   * \param[in] num JSON data to parse.
   * \return An error code, because it indicates an erroneously configured key, as this parser should not
   *         handle any numbers.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Callback on boolean found in JSON.
   * \param[in] val Parsed boolean value.
   * \return An error code, because it indicates an erroneously configured key, as this parser should not handle any
   * booleans.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnBool(bool val) noexcept;

  /*!
   * \brief Callback on string event.
   * \param[in] val JSON data to parse.
   * \return Parser's result with "ParserState::kRunning" or an error code.
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

 protected:
  /*!
   * \brief Validate the parsed 'overload_protection_filter'.
   * \details Validation implemented as protected API to support validation-independent test implementation.
   * \return Validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ValidationResult Validate() const noexcept;

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Current intermediate 'overload_protection_filter' object to fill with information.
   */
  FilterConfigObject filter_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_OVERLOAD_PROTECTION_FILTER_PARSER_H_

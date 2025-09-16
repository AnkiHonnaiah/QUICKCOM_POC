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
/*!        \file  required_service_instance_service_discovery_parser.h
 *        \brief  JSON parser for 'RequiredServiceInstanceServiceDiscovery'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/reader/v1/parser.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_service_discovery_object.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_service_discovery_validator.h"
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
 * \brief Parses a single 'required service instance service discovery' element from JSON.
 */
class RequiredServiceInstanceServiceDiscoveryParser final
    : public amsr::json::Parser<RequiredServiceInstanceServiceDiscoveryParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<RequiredServiceInstanceServiceDiscoveryParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::RequiredServiceInstanceServiceDiscoveryValidator;

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
   * \brief TTL JSON key.
   */
  static constexpr CStringView kTtlKey{"ttl"_sv};

  /*!
   * \brief InitialDelayMin JSON key.
   */
  static constexpr CStringView kInitialDelayMinKey{"initial_delay_min_ns"_sv};

  /*!
   * \brief InitialDelayMax JSON key.
   */
  static constexpr CStringView kInitialDelayMaxKey{"initial_delay_max_ns"_sv};

  /*!
   * \brief InitialRepetitionsBaseDelay JSON key.
   */
  static constexpr CStringView kInitialRepetitionsBaseDelayKey{"initial_repetitions_base_delay_ns"_sv};

  /*!
   * \brief InitialRepetitionsMax JSON key.
   */
  static constexpr CStringView kInitialRepetitionsMaxKey{"initial_repetitions_max"_sv};

  /*!
   * \brief Event groups JSON key.
   */
  static constexpr CStringView kEventGroupsKey{"eventgroups"_sv};

  /*!
   * \brief Input for this parser is the container of 'RequiredServiceInstanceService' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] service_discovery A reference to the element which is written by the parser.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  RequiredServiceInstanceServiceDiscoveryParser(
      amsr::json::JsonData& file,
      ConfigurationTypesAndDefs::RequiredServiceInstanceServiceDiscovery& service_discovery) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstanceServiceDiscoveryParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstanceServiceDiscoveryParser(RequiredServiceInstanceServiceDiscoveryParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstanceServiceDiscoveryParser(RequiredServiceInstanceServiceDiscoveryParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(RequiredServiceInstanceServiceDiscoveryParser const&)
      -> RequiredServiceInstanceServiceDiscoveryParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(RequiredServiceInstanceServiceDiscoveryParser&&)
      -> RequiredServiceInstanceServiceDiscoveryParser& = delete;

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
  auto OnKey(CStringView key) noexcept -> ParserResult;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnEndObject(std::size_t) const noexcept -> ParserResult;

  /*!
   * \brief Callback on number found in JSON
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
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto OnUnexpectedEvent() noexcept -> ParserResult;

  /*!
   * \brief Start object means that a new 'Required service instance service discovery' object was found.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnStartObject() noexcept -> ParserResult;

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to POD structure to fill with parsed information.
   */
  ConfigurationTypesAndDefs::RequiredServiceInstanceServiceDiscovery& service_discovery_;

  /*!
   * \brief Element wrapper object to fill with parsed information.
   */
  model::RequiredServiceInstanceServiceDiscoveryObject service_discovery_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_PARSER_H_

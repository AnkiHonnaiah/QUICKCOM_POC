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
/*!        \file  required_service_instance_service_discovery_event_groups_parser.h
 *        \brief  JSON parser for 'RequiredServiceInstanceServiceDiscoveryEventGroups'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUPS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUPS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_service_discovery_event_group_object.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_service_discovery_event_group_validator.h"
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
 * \brief Parses a container of 'required service instance service discovery event group' elements from JSON.
 */
class RequiredServiceInstanceServiceDiscoveryEventGroupsParser final
    : public amsr::json::Parser<RequiredServiceInstanceServiceDiscoveryEventGroupsParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<RequiredServiceInstanceServiceDiscoveryEventGroupsParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::RequiredServiceInstanceServiceDiscoveryEventGroupValidator;

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
   * \brief Event group ID key.
   */
  static constexpr CStringView kEventGroupdIdKey{"id"_sv};

  /*!
   * \brief SubscribeEventgroupRetryDelay JSON key.
   */
  static constexpr CStringView kSubscribeEventgroupRetryDelayKey{"subscribe_eventgroup_retry_delay_ms"_sv};

  /*!
   * \brief SubscribeEventgroupRetryMax JSON key.
   */
  static constexpr CStringView kSubscribeEventgroupRetryMaxKey{"subscribe_eventgroup_retry_max"_sv};

  /*!
   * \brief TTL JSON key.
   */
  static constexpr CStringView kTtlKey{"ttl"_sv};

  /*!
   * \brief RequestResponseDelayMin JSON key.
   */
  static constexpr CStringView kRequestResponseDelayMinKey{"request_response_delay_min_ns"_sv};

  /*!
   * \brief RequestResponseDelayMax JSON key.
   */
  static constexpr CStringView kRequestResponseDelayMaxKey{"request_response_delay_max_ns"_sv};

  /*!
   * \brief Input for this parser is the container of 'RequiredServiceInstanceSdEventgroup' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] event_groups A reference to the container which is extended on every intermediate parsed element that
   * is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  RequiredServiceInstanceServiceDiscoveryEventGroupsParser(
      amsr::json::JsonData& file,
      ConfigurationTypesAndDefs::RequiredServiceInstanceSdEventgroupContainer& event_groups) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstanceServiceDiscoveryEventGroupsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstanceServiceDiscoveryEventGroupsParser(
      RequiredServiceInstanceServiceDiscoveryEventGroupsParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstanceServiceDiscoveryEventGroupsParser(RequiredServiceInstanceServiceDiscoveryEventGroupsParser&&) =
      delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(RequiredServiceInstanceServiceDiscoveryEventGroupsParser const&)
      -> RequiredServiceInstanceServiceDiscoveryEventGroupsParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(RequiredServiceInstanceServiceDiscoveryEventGroupsParser&&)
      -> RequiredServiceInstanceServiceDiscoveryEventGroupsParser& = delete;

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
  ParserResult OnEndObject(std::size_t) const noexcept;

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
  ConfigurationTypesAndDefs::RequiredServiceInstanceSdEventgroupContainer& event_groups_;

  /*!
   * \brief This POD structure will be added to the container if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::RequiredServiceInstanceSdEventgroup event_group_;

  /*!
   * \brief Element wrapper object to fill with parsed information.
   */
  model::RequiredServiceInstanceServiceDiscoveryEventGroupObject event_group_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUPS_PARSER_H_

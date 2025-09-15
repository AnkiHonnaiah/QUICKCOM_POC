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
/*!       \file   provided_service_instance_service_discovery_event_groups_parser.h
 *        \brief  JSON parser header for 'provided_service_instance_service_discovery_event_groups'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUPS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUPS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_service_discovery_event_group_validator.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
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
 * \brief Parses a single 'Provided service instance service discovery event group' element from JSON.
 * \details Event group of service discovery of provided service instance.
 * 1. Parse / read in
 * 2. Validate that the parsed 'Provided service instance service discovery event groups' meet all constraints.
 * 3. Push to 'Provided service instance service discovery event groups' container.
 */
class ProvidedServiceInstanceServiceDiscoveryEventGroupsParser final
    : public amsr::json::Parser<ProvidedServiceInstanceServiceDiscoveryEventGroupsParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<ProvidedServiceInstanceServiceDiscoveryEventGroupsParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::ProvidedServiceInstanceServiceDiscoveryEventGroupValidator;

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
   * \brief Id JSON key.
   */
  static constexpr CStringView kIdKey{"id"_sv};

  /*!
   * \brief Event multicast threshold JSON key.
   */
  static constexpr CStringView kEventMulticastThresholdKey{"event_multicast_threshold"_sv};

  /*!
   * \brief Request Response delay min JSON key.
   */
  static constexpr CStringView kRequestResponseDelayMinNsKey{"request_response_delay_min_ns"_sv};

  /*!
   * \brief Request Response delay max JSON key.
   */
  static constexpr CStringView kRequestResponseDelayMaxNsKey{"request_response_delay_max_ns"_sv};

  /*!
   * \brief Type-alias for ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject
   */
  using ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject =
      configuration::model::ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject;

  /*!
   * \brief Input for this parser is the container of c from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] provided_service_instance_service_discovery_event_groups A reference to the container which is extended
   * on every valid intermediate 'Provided service instance service discovery event groups' element.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ProvidedServiceInstanceServiceDiscoveryEventGroupsParser(
      amsr::json::JsonData &file, ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroupContainer
                                      &provided_service_instance_service_discovery_event_groups) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedServiceInstanceServiceDiscoveryEventGroupsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ProvidedServiceInstanceServiceDiscoveryEventGroupsParser(
      ProvidedServiceInstanceServiceDiscoveryEventGroupsParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ProvidedServiceInstanceServiceDiscoveryEventGroupsParser(
      ProvidedServiceInstanceServiceDiscoveryEventGroupsParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ProvidedServiceInstanceServiceDiscoveryEventGroupsParser const &)
      -> ProvidedServiceInstanceServiceDiscoveryEventGroupsParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ProvidedServiceInstanceServiceDiscoveryEventGroupsParser &&)
      -> ProvidedServiceInstanceServiceDiscoveryEventGroupsParser & = delete;

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
  ParserResult OnKey(CStringView key) const noexcept;

  /*!
   * \brief Start object means that a new 'Provided service instance service discovery event group' object was found.
   * \return ParserResult with ParserState::kRunning.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief End Array
   * \return ParserResult with ParserState::kFinished.
   * \steady FALSE
   */
  static ParserResult OnEndArray(std::size_t) noexcept;

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

 protected:
  /*!
   * \brief Validate the parsed 'Provided service instance service discovery event group'.
   * \remark Validation implemented as protected API to support validation-independent test implementation.
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
   * \brief Reference to the 'Provided service instance service discovery event group' container that is extended after
   * the validation step.
   */
  ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroupContainer
      &provided_service_instance_service_discovery_event_groups_;

  /*!
   * \brief This POD structure will be added to the container of 'Provided service instance service discovery event
   * groups', if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroup
      provided_service_instance_service_discovery_event_group_;

  /*!
   * \brief Current intermediate 'Provided service instance service discovery event group' object to fill with
   * information.
   */
  ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject
      provided_service_instance_service_discovery_event_group_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUPS_PARSER_H_

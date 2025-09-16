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
/*!        \file  provided_service_instances_parser.h
 *        \brief  JSON parser for 'ProvidedServiceInstances'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCES_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCES_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_validator.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
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
 * \brief Parses 'provided service instances' elements from JSON into a container of services.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'provided service instances' meet all constraints.
 * 3. Push to 'provided service instances' container.
 */
class ProvidedServiceInstancesParser final : public amsr::json::Parser<ProvidedServiceInstancesParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<ProvidedServiceInstancesParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::ProvidedServiceInstanceValidator;

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
  static constexpr CStringView kServiceIdKey{"service_id"_sv};

  /*!
   * \brief Instance ID JSON key.
   */
  static constexpr CStringView kInstanceIdKey{"instance_id"_sv};

  /*!
   * \brief Major version JSON key.
   */
  static constexpr CStringView kMajorVersionKey{"major_version"_sv};

  /*!
   * \brief Minor version JSON key.
   */
  static constexpr CStringView kMinorVersionKey{"minor_version"_sv};

  /*!
   * \brief Service discovery JSON key.
   */
  static constexpr CStringView kServiceDiscoveryKey{"service_discovery"_sv};

  /*!
   * \brief Port mappings JSON key.
   */
  static constexpr CStringView kPortMappingsKey{"port_mappings"_sv};

  /*!
   * \brief Event props JSON key.
   */
  static constexpr CStringView kEventPropsKey{"event_props"_sv};

  /*!
   * \brief Method props JSON key.
   */
  static constexpr CStringView kMethodPropsKey{"method_props"_sv};

  /*!
   * \brief Type-alias for ProvidedServiceInstanceConfigObject
   */
  using ProvidedServiceInstanceConfigObject = configuration::model::ProvidedServiceInstanceConfigObject;

  /*!
   * \brief Input for this parser is the container of 'provided service instances' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] provided_service_instances A reference to the container which is extended on every intermediate
   * 'provided service instances' element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ProvidedServiceInstancesParser(
      amsr::json::JsonData &file,
      ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer &provided_service_instances) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedServiceInstancesParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ProvidedServiceInstancesParser(ProvidedServiceInstancesParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ProvidedServiceInstancesParser(ProvidedServiceInstancesParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ProvidedServiceInstancesParser const &) -> ProvidedServiceInstancesParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ProvidedServiceInstancesParser &&) -> ProvidedServiceInstancesParser & = delete;

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
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Start object means that a new provided service object was found.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief Callback on end of JSON array.
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
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

 protected:
  /*!
   * \brief Validate the parsed intermediate 'provided service instance' object.
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
   * \brief Reference to the 'provided service instance' container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstanceContainer &provided_service_instances_;

  /*!
   * \brief This POD structure will be added to the container of 'provided service instances', if
   * the parsed element is valid.
   */
  ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstance provided_service_instance_;

  /*!
   * \brief Current intermediate 'provided service instance' object to fill with information.
   */
  ProvidedServiceInstanceConfigObject provided_service_instance_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCES_PARSER_H_

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
/*!        \file  required_service_instances_parser.h
 *        \brief  JSON parser for 'RequiredServiceInstances'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCES_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCES_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_validator.h"
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
 * \brief Parses 'required service instances' elements from JSON into a container of services.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'required service instances' meet all constraints.
 * 3. Push to 'required service instances' container.
 */
class RequiredServiceInstancesParser final : public amsr::json::Parser<RequiredServiceInstancesParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<RequiredServiceInstancesParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::RequiredServiceInstanceValidator;

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
   * \brief Version driven find behavior key.
   */
  static constexpr CStringView kVersionDrivenFindBehaviorKey{"version_driven_find_behavior"_sv};

  /*!
   * \brief Service discovery JSON key.
   */
  static constexpr CStringView kServiceDiscoveryKey{"service_discovery"_sv};

  /*!
   * \brief Port mapping JSON key.
   */
  static constexpr CStringView kPortMappingKey{"port_mapping"_sv};

  /*!
   * \brief Required PDUs key.
   */
  static constexpr CStringView kRequiredPdusKey{"required_signal_serialized_pdus"_sv};

  /*!
   * \brief Event props JSON key.
   */
  static constexpr CStringView kMethodPropsKey{"method_props"_sv};

  /*!
   * \brief Type-alias for RequiredServiceInstanceConfigObject
   */
  using RequiredServiceInstanceConfigObject = configuration::model::RequiredServiceInstanceConfigObject;

  /*!
   * \brief Input for this parser is the container of 'required service instances' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] required_service_instances A reference to the container which is extended on every intermediate
   * 'required service instances' element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  RequiredServiceInstancesParser(
      amsr::json::JsonData &file,
      ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstanceContainer &required_service_instances) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstancesParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstancesParser(RequiredServiceInstancesParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstancesParser(RequiredServiceInstancesParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(RequiredServiceInstancesParser const &) -> RequiredServiceInstancesParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(RequiredServiceInstancesParser &&) -> RequiredServiceInstancesParser & = delete;

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
   * \brief Start object means that a new required service object was found.
   * \return ParserResult with ParserState::kRunning.
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief Callback on end of JSON array.
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
  ParserResult OnString(CStringView string) noexcept;

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
   * \brief Validate the parsed intermediate 'required service instance' object.
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
   * \brief Reference to the 'required service instance' container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstanceContainer &required_service_instances_;

  /*!
   * \brief This POD structure will be added to the container of 'required service instances', if
   * the parsed element is valid.
   */
  ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance required_service_instance_;

  /*!
   * \brief Current intermediate 'required service instance' object to fill with information.
   */
  RequiredServiceInstanceConfigObject required_service_instance_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_REQUIRED_SERVICE_INSTANCES_PARSER_H_

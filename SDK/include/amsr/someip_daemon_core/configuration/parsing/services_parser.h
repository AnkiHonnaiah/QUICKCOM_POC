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
/*!        \file  services_parser.h
 *        \brief  Sub-parser for service objects represented in JSON.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICES_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICES_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/service_validator.h"
#include "amsr/someip_daemon_core/configuration/parsing/event_groups_parser.h"
#include "amsr/someip_daemon_core/configuration/parsing/events_parser.h"
#include "amsr/someip_daemon_core/configuration/parsing/methods_parser.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "vac/container/c_string_view.h"

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
 * \brief Parses service elements from JSON into a container of services.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed service meets all constraints.
 * 3. Push to service container if the service configuration is valid.
 */
class ServicesParser final : public amsr::json::Parser<ServicesParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<ServicesParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::ServiceValidator;

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
   * \brief Type alias for easy access to ParserState.
   */
  using ParserState = amsr::json::ParserState;

  /*!
   * \brief Type alias for the parser base used multiple times.
   */
  using Base = amsr::json::Parser<ServicesParser>;

  /*!
   * \brief Name JSON key.
   */
  static constexpr CStringView kNameKey{"name"_sv};

  /*!
   * \brief Service_id JSON key.
   */
  static constexpr CStringView kServiceIdKey{"service_id"_sv};

  /*!
   * \brief Major version JSON key.
   */
  static constexpr CStringView kMajorVersionKey{"major_version"_sv};

  /*!
   * \brief Minor version JSON key.
   */
  static constexpr CStringView kMinorVersionKey{"minor_version"_sv};

  /*!
   * \brief Methods JSON key.
   */
  static constexpr CStringView kMethodsKey{"methods"_sv};

  /*!
   * \brief Events JSON key.
   */
  static constexpr CStringView kEventsKey{"events"_sv};

  /*!
   * \brief Event group JSON key.
   */
  static constexpr CStringView kEventGroupsKey{"eventgroups"_sv};

  /*!
   * \brief Input for this parser is the container of services from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] services A reference to the container which is extended on every intermediate service element that is
   * valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ServicesParser(amsr::json::JsonData &file, ConfigurationTypesAndDefs::ServiceContainer &services) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ServicesParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ServicesParser(ServicesParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ServicesParser(ServicesParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ServicesParser const &) -> ServicesParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ServicesParser &&) -> ServicesParser & = delete;

  /*!
   * \brief Number callback is called by the parser for the service ID, the major
   * and the minor version.
   * \param[in] num value parsed.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Start object means that a new service object was found.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief Used for spawning sub-parsers within methods.
   * \param[in] key The current key in JSON.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Callback on string found in the JSON.
   * \param[in] val JSON data to parse
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) const noexcept;

  /*!
   * \brief End Array is called if all services have been parsed. This is the exit criteria for this sub-parser.
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserState OnEndArray(std::size_t) noexcept;

  /*!
   * \brief End Object is called on each service end.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept;

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
   * \brief On every new service, object attributes have to be reset.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void PreProcess() noexcept;

  /*!
   * \brief Validate the parsed service object.
   * \details Validation implemented as protected API to support validation-independent test implementation.
   * \return Validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  model::ServiceValidator::ValidationResult Validate() const noexcept;

  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the service container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::ServiceContainer &services_;

  /*!
   * \brief This POD structure will be added to the container of services, if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::Service service_storage_;

  /*!
   * \brief Current intermediate service object to fill with information.
   */
  model::ServiceConfigObject service_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICES_PARSER_H_

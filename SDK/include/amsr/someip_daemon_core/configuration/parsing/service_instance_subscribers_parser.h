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
/*!        \file  service_instance_subscribers_parser.h
 *        \brief  vajson parser header for "service_instance_subscribers"
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICE_INSTANCE_SUBSCRIBERS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICE_INSTANCE_SUBSCRIBERS_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_subscriber_validator.h"
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
 * \brief Parses a single 'provided service instances subscriber' elements from JSON.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'provided service instances subscribers' meet all constraints.
 * 3. Push to 'provided service instances subscribers' container.
 */
class ServiceInstanceSubscribersParser final : public amsr::json::Parser<ServiceInstanceSubscribersParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<ServiceInstanceSubscribersParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::ProvidedServiceInstanceSubscriberValidator;

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
   * \brief Address JSON key.
   */
  static constexpr CStringView kAddressKey{"address"_sv};

  /*!
   * \brief Udp port JSON key.
   */
  static constexpr CStringView kUdpPortKey{"udp_port"_sv};

  /*!
   * \brief Eventgroups JSON key.
   */
  static constexpr CStringView kEventgroupsKey{"eventgroups"_sv};

  /*!
   * \brief Type-alias for ProvidedServiceInstancesSubscriberConfigObject
   */
  using ProvidedServiceInstanceSubscriberConfigObject =
      configuration::model::ProvidedServiceInstanceSubscriberConfigObject;

  /*!
   * \brief Input for this parser is the container of 'provided service instances subscriber' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] service_instance_subscribers A reference to the container which is extended on
   * every intermediate 'provided service instances subscribers' element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ServiceInstanceSubscribersParser(
      amsr::json::JsonData &file,
      ConfigurationTypesAndDefs::ServiceSubscriberConfigurationContainer &service_instance_subscribers) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ServiceInstanceSubscribersParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ServiceInstanceSubscribersParser(ServiceInstanceSubscribersParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ServiceInstanceSubscribersParser(ServiceInstanceSubscribersParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ServiceInstanceSubscribersParser const &) -> ServiceInstanceSubscribersParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ServiceInstanceSubscribersParser &&) -> ServiceInstanceSubscribersParser & = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kRunning or an error if sub-parsers failed.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) const noexcept;

  /*!
   * \brief Start object means that a new 'static_sd_provided_service_instances_subscribers' object was found.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief Start Array
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartArray() noexcept;

  /*!
   * \brief End Array
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndArray(std::size_t) noexcept;

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
   * \brief Callback on string found in JSON
   * \param[in] val value returned by the parser.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept;

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
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnUnexpectedEvent();

 protected:
  /*!
   * \brief Validate the parsed intermediate object.
   * \details Validation implemented as protected API to support validation-independent test implementation.
   * \return Validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ValidationResult Validate() const;

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Flag to distinguish if parser is inside a sub-value array.
   */
  bool is_inside_sub_array_{false};

  /*!
   * \brief Reference to the container of subscribers which is extended after the validation step.
   */
  ConfigurationTypesAndDefs::ServiceSubscriberConfigurationContainer &service_subscriber_container;

  /*!
   * \brief This POD structure of 'provided service instances subscribers'.
   */
  ConfigurationTypesAndDefs::ServiceSubscriberConfiguration service_subscriber_;

  /*!
   * \brief Current intermediate 'provided service instances subscriber' object to fill with information.
   */
  ProvidedServiceInstanceSubscriberConfigObject service_instance_subscriber_element_wrapper_;

  /*!
   * \brief Container of eventgroup Ids.
   */
  ConfigurationTypesAndDefs::EventgroupIdContainer service_instances_subscriber_eventgroup_ids_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SERVICE_INSTANCE_SUBSCRIBERS_PARSER_H_

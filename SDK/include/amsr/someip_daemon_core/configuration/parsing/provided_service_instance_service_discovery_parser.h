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
/*!       \file   provided_service_instance_service_discovery_parser.h
 *        \brief  JSON parser header for 'provided_service_instance_service_discovery'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/provided_service_instance_service_discovery_validator.h"
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
 * \brief Parses a single 'Provided service instance service discovery' elements from JSON.
 */
class ProvidedServiceInstanceServiceDiscoveryParser final
    : public amsr::json::Parser<ProvidedServiceInstanceServiceDiscoveryParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<ProvidedServiceInstanceServiceDiscoveryParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::ProvidedServiceInstanceServiceDiscoveryValidator;

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
   * \brief Initial repetitions max JSON key.
   */
  static constexpr CStringView kInitialRepetitionsMaxKey{"initial_repetitions_max"_sv};

  /*!
   * \brief Initial delay min ns JSON key.
   */
  static constexpr CStringView kInitialDelayMinNsKey{"initial_delay_min_ns"_sv};

  /*!
   * \brief Initial delay max ns JSON key.
   */
  static constexpr CStringView kInitialDelayMaxNsKey{"initial_delay_max_ns"_sv};

  /*!
   * \brief Initial repetitions base delay ns JSON key.
   */
  static constexpr CStringView kInitialRepetitionsBaseDelayNsKey{"initial_repetitions_base_delay_ns"_sv};

  /*!
   * \brief Cyclic offer delay ns JSON key.
   */
  static constexpr CStringView kCyclicOfferDelayNsKey{"cyclic_offer_delay_ns"_sv};

  /*!
   * \brief Request response delay min ns JSON key.
   */
  static constexpr CStringView kRequestResponseDelayMinNsKey{"request_response_delay_min_ns"_sv};

  /*!
   * \brief Request response delay max ns JSON key.
   */
  static constexpr CStringView kRequestResponseDelayMaxNsKey{"request_response_delay_max_ns"_sv};

  /*!
   * \brief Eventgroups JSON key.
   */
  static constexpr CStringView kEventgroupsKey{"eventgroups"_sv};

  /*!
   * \brief Type-alias for ProvidedServiceInstanceServiceDiscoveryConfigObject
   */
  using ProvidedServiceInstanceServiceDiscoveryConfigObject =
      configuration::model::ProvidedServiceInstanceServiceDiscoveryConfigObject;

  /*!
   * \brief Input for this parser is the container of 'Provided service instance service discovery' from the config to
   * fill.
   * \param[in] file JSON document to parse.
   * \param[out] provided_service_instance_service_discovery A reference to the 'Provided service instance service
   * discovery' POD structure to be filled with the parsed values from JSON.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ProvidedServiceInstanceServiceDiscoveryParser(amsr::json::JsonData &file,
                                                ConfigurationTypesAndDefs::ProvidedServiceInstanceServiceDiscovery
                                                    &provided_service_instance_service_discovery) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedServiceInstanceServiceDiscoveryParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ProvidedServiceInstanceServiceDiscoveryParser(ProvidedServiceInstanceServiceDiscoveryParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ProvidedServiceInstanceServiceDiscoveryParser(ProvidedServiceInstanceServiceDiscoveryParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ProvidedServiceInstanceServiceDiscoveryParser const &)
      -> ProvidedServiceInstanceServiceDiscoveryParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(ProvidedServiceInstanceServiceDiscoveryParser &&)
      -> ProvidedServiceInstanceServiceDiscoveryParser & = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param key Parsed key string.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Start object means that a new 'Provided service instance service discovery' object was found.
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
   * \param num value returned by the parser.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \trace SPEC-10144689
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Callback on string found in JSON.
   * \param val JSON data to parse
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
   * \brief Validate the parsed 'Provided service instance service discovery'.
   * \remark Validation implemented as protected API to support validation-independent test implementation.
   * \return Validation result.
   * \steady FALSE
   */
  ValidationResult Validate() const noexcept;

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the POD structure of 'Provided service instance service discovery'.
   */
  ConfigurationTypesAndDefs::ProvidedServiceInstanceServiceDiscovery &provided_service_instance_service_discovery_;

  /*!
   * \brief Intermediate 'Provided service instance service discovery' object to fill with information.
   */
  ProvidedServiceInstanceServiceDiscoveryConfigObject provided_service_instance_service_discovery_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_PARSER_H_

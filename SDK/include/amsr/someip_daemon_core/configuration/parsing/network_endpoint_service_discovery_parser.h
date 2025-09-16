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
/*!       \file   network_endpoint_service_discovery_parser.h
 *        \brief  JSON parser header for 'network_endpoint_service_discovery'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_SERVICE_DISCOVERY_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_SERVICE_DISCOVERY_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_service_discovery_validator.h"
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
 * \brief Parses a single 'network endpoint service discovery' elements from JSON.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'network endpoint service discovery' meets all constraints.
 */
class NetworkEndpointServiceDiscoveryParser final : public amsr::json::Parser<NetworkEndpointServiceDiscoveryParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<NetworkEndpointServiceDiscoveryParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::NetworkEndpointServiceDiscoveryValidator;

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
   * \brief Multicast address JSON key.
   */
  static constexpr CStringView kMulticastAddressKey{"multicast_address"_sv};

  /*!
   * \brief Port JSON key.
   */
  static constexpr CStringView kPortKey{"port"_sv};

  /*!
   * \brief Type-alias for NetworkEndpointServiceDiscoveryConfigObject
   */
  using NetworkEndpointServiceDiscoveryConfigObject = configuration::model::NetworkEndpointServiceDiscoveryConfigObject;

  /*!
   * \brief Input for this parser is the container of 'Network endpoint service discovery' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] network_endpoint_service_discovery A reference to the 'Network endpoint service discovery' element to
   * be filled with parsed data.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  NetworkEndpointServiceDiscoveryParser(
      amsr::json::JsonData& file,
      ConfigurationTypesAndDefs::NetworkEndpointServiceDiscovery& network_endpoint_service_discovery) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~NetworkEndpointServiceDiscoveryParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  NetworkEndpointServiceDiscoveryParser(NetworkEndpointServiceDiscoveryParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  NetworkEndpointServiceDiscoveryParser(NetworkEndpointServiceDiscoveryParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(NetworkEndpointServiceDiscoveryParser const&) -> NetworkEndpointServiceDiscoveryParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(NetworkEndpointServiceDiscoveryParser&&) -> NetworkEndpointServiceDiscoveryParser& = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
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
   * \brief Callback on string found in JSON
   * \param[in] val value returned by the parser.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
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
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
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
  static ParserResult OnUnexpectedEvent() noexcept;

 protected:
  /*!
   * \brief Validate the parsed object.
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
   * \brief Current intermediate 'Network endpoint service discovery' object to fill with information.
   */
  NetworkEndpointServiceDiscoveryConfigObject network_endpoint_service_discovery_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_SERVICE_DISCOVERY_PARSER_H_

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
/*!       \file   network_endpoint_port_keep_alive_parser.h
 *        \brief  JSON parser for 'network_endpoint_port_keep_alive'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/reader/v1/parser.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_port_keep_alive_object.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_port_keep_alive_validator.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "someip-protocol/internal/someip_posix_types.h"
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
 * \brief Parses a single 'Network endpoint port keep alive' elements from JSON.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'Network endpoint port keep alive' meets all constraints.
 */
class NetworkEndpointPortKeepAliveParser final : public amsr::json::Parser<NetworkEndpointPortKeepAliveParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<NetworkEndpointPortKeepAliveParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::NetworkEndpointPortKeepAliveValidator;

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
   * \brief Idle time s JSON key.
   */
  static constexpr CStringView const kIdleTimeSKey{"idle_time_s"_sv};

  /*!
   * \brief Alive interval s JSON key.
   */
  static constexpr CStringView const kAliveIntervalSKey{"alive_interval_s"_sv};

  /*!
   * \brief Retry count JSON key.
   */
  static constexpr CStringView const kRetryCountKey{"retry_count"_sv};

  /*!
   * \brief Type-alias for NetworkEndpointPortKeepAliveConfigObject
   */
  using NetworkEndpointPortKeepAliveConfigObject = configuration::model::NetworkEndpointPortKeepAliveConfigObject;

  /*!
   * \brief Input for this parser is the container of 'network endpoint port keep alive' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] network_endpoint_port_keep_alive A reference to the 'network endpoint port keep alive' element to be
   * filled with parsed data.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  NetworkEndpointPortKeepAliveParser(
      amsr::json::JsonData& file,
      someip_protocol::internal::KeepAliveParameters& network_endpoint_port_keep_alive) noexcept;

  /*!
   *
   * \brief Destructor.
   * \steady FALSE
   */
  ~NetworkEndpointPortKeepAliveParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  NetworkEndpointPortKeepAliveParser(NetworkEndpointPortKeepAliveParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  NetworkEndpointPortKeepAliveParser(NetworkEndpointPortKeepAliveParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(NetworkEndpointPortKeepAliveParser const&) -> NetworkEndpointPortKeepAliveParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(NetworkEndpointPortKeepAliveParser&&) -> NetworkEndpointPortKeepAliveParser& = delete;

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
  auto OnKey(CStringView key) const noexcept -> ParserResult;

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
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnNumber(amsr::json::JsonNumber num) noexcept -> ParserResult;

  /*!
   * \brief Callback on string.
   * \param[in] val JSON data to parse
   * \return An error code, because it indicates an erroneously configured key, as this parser should not
   * handle any strings.
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

 protected:
  /*!
   * \brief Validate the parsed object.
   * \remark Validation implemented as protected API to support validation-independent test implementation.
   * \return Validation result.
   * \steady FALSE
   */
  auto Validate() const noexcept -> ValidationResult;

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Current intermediate 'Network endpoint port keep alive' object to fill with information.
   */
  NetworkEndpointPortKeepAliveConfigObject network_endpoint_port_keep_alive_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_PARSER_H_

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
/*!       \file   network_endpoints_parser.h
 *        \brief  JSON parser header for 'network_endpoints'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINTS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINTS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/reader/v1/parser.h"
#include "amsr/json/util/number.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_object.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_validator.h"
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
 * \brief Parses 'network endpoints' elements from JSON.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'network endpoint' meet all constraints.
 * 3. Push to 'network endpoints' container.
 */
class NetworkEndpointsParser final : public amsr::json::Parser<NetworkEndpointsParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<NetworkEndpointsParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::NetworkEndpointValidator;

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
   * \brief Name JSON key.
   */
  static constexpr CStringView kNameKey{"name"_sv};

  /*!
   * \brief Address JSON key.
   */
  static constexpr CStringView kAddressKey{"address"_sv};

  /*!
   * \brief Mtu JSON key.
   */
  static constexpr CStringView kMtuKey{"mtu"_sv};

  /*!
   * \brief Service discovery JSON key.
   */
  static constexpr CStringView kServiceDiscoveryKey{"service_discovery"_sv};

  /*!
   * \brief Keep alive JSON key.
   */
  static constexpr CStringView kKeepAliveKey{"keep_alive"_sv};

  /*!
   * \brief Ports JSON key.
   */
  static constexpr CStringView kPortsKey{"ports"_sv};

  /*!
   * \brief JSON key for bulk read count.
   */
  static constexpr CStringView kBulkReadCountKey{"bulk_read_count"_sv};

  /*!
   * \brief JSON key for bulk read maximum number of consecutive receive calls.
   */
  static constexpr CStringView kBulkReadMaxNoOfConsecutiveReceiveCallsKey{
      "bulk_read_max_no_of_consecutive_receive_calls"_sv};

  /*!
   * \brief JSON key for bulk read minimum number of datagrams to continue consecutive receive calls.
   */
  static constexpr CStringView kBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsKey{
      "bulk_read_min_no_of_datagrams_to_continue_consecutive_receive_calls"_sv};

  /*!
   * \brief JSON key for bulk read receive period in micro seconds.
   */
  static constexpr CStringView kBulkReadReceivePeriodUsKey{"bulk_read_receive_period_us"_sv};

  /*!
   * \brief JSON key for TCP Nagle enabled.
   */
  static constexpr CStringView kTcpNagleEnabledKey{"tcp_nagle_enabled"_sv};

  /*!
   * \brief JSON key for Quality of Service (Qos, vlanPriority) setting.
   */
  static constexpr CStringView kQosKey{"qos"_sv};

  /*!
   * \brief Number of invalid tcp messages before disconnection JSON key.
   */
  static constexpr CStringView kNoOfInvalidTcpMessagesBeforeDisconnectionKey{
      "no_of_invalid_tcp_messages_before_disconnection"_sv};

  /*!
   * \brief Number of consecutive valid tcp messages before error reset JSON key.
   */
  static constexpr CStringView kNoOfConsecutiveValidTcpMessagesBeforeErrorResetKey{
      "no_of_consecutive_valid_tcp_messages_before_error_reset"_sv};

  /*!
   * \brief Type-alias for NetworkEndpointConfigObject
   */
  using NetworkEndpointConfigObject = configuration::model::NetworkEndpointConfigObject;

  /*!
   * \brief Input for this parser is the container of 'network endpoints' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] network_endpoints A reference to the container which is extended on every intermediate
   * 'network endpoint' element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  NetworkEndpointsParser(amsr::json::JsonData &file,
                         ConfigurationTypesAndDefs::NetworkEndpointContainer &network_endpoints) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~NetworkEndpointsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  NetworkEndpointsParser(NetworkEndpointsParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  NetworkEndpointsParser(NetworkEndpointsParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(NetworkEndpointsParser const &) -> NetworkEndpointsParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(NetworkEndpointsParser &&) -> NetworkEndpointsParser & = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param key Parsed key string.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnKey(CStringView key) noexcept -> ParserResult;

  /*!
   * \brief Start object means that a new network endpoint object was found.
   * \return ParserResult with ParserState::kRunning.
   * \steady FALSE
   */
  auto OnStartObject() noexcept -> ParserResult;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnEndObject(std::size_t) noexcept -> ParserResult;

  /*!
   * \brief End Array
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto OnEndArray(std::size_t) noexcept -> ParserResult;

  /*!
   * \brief Callback on string found in JSON
   * \param val value returned by the parser.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnString(CStringView val) noexcept -> ParserResult;

  /*!
   * \brief Callback on number found in JSON
   * \param num value returned by the parser.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnNumber(amsr::json::JsonNumber num) noexcept -> ParserResult;

  /*!
   * \brief Bool callback.
   * \param[in] val Parsed boolean value
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto OnBool(bool val) noexcept -> ParserResult;

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
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto Validate() const noexcept -> ValidationResult;

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the 'network endpoints' container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::NetworkEndpointContainer &network_endpoints_;

  /*!
   * \brief Temporary POD structure for a single 'network endpoint'.
   */
  ConfigurationTypesAndDefs::NetworkEndpoint network_endpoint_;

  /*!
   * \brief Current intermediate 'network endpoint' object to fill with information.
   */
  NetworkEndpointConfigObject network_endpoint_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINTS_PARSER_H_

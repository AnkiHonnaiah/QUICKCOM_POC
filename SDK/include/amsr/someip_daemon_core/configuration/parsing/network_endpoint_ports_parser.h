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
/*!       \file   network_endpoint_ports_parser.h
 *        \brief  JSON parser header for 'network endpoint ports'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_PORTS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_PORTS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_ports_validator.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/optional.h"
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
 * \brief Parses a single 'network endpoint port' elements from JSON.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'network endpoint ports' meet all constraints.
 * 3. Push to 'network endpoint ports' container.
 */
class NetworkEndpointPortsParser final : public amsr::json::Parser<NetworkEndpointPortsParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<NetworkEndpointPortsParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::NetworkEndpointPortsValidator;

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
   * \brief Port JSON key.
   */
  static constexpr CStringView kPortKey{"port"_sv};

  /*!
   * \brief Proto JSON key.
   */
  static constexpr CStringView kProtoKey{"proto"_sv};

  /*!
   * \brief Qos JSON key.
   */
  static constexpr CStringView kQosKey{"qos"_sv};

  /*!
   * \brief Keep alive JSON key.
   */
  static constexpr CStringView kKeepAliveKey{"keep_alive"_sv};

  /*!
   * \brief Socket Option linger JSON key.
   */
  static constexpr CStringView kSocketOptionLingerKey{"enable_so_linger"_sv};

  /*!
   * \brief Option For Tcp Nagle's Algorithm JSON key.
   */
  static constexpr CStringView kSocketOptionNaglesAlgorithmKey{"tcp_nagles_algorithm"_sv};

  /*!
   * \brief Option for Bulk read receive period us JSON key.
   */
  static constexpr CStringView kSocketOptionBulkReadReceivePeriodUsKey{"bulk_read_receive_period_us"_sv};

  /*!
   * \brief Protocol udp constant.
   */
  static constexpr CStringView kUdp{"udp"_sv};

  /*!
   * \brief Protocol tcp constant.
   */
  static constexpr CStringView kTcp{"tcp"_sv};

  /*!
   * \brief Number of invalid tcp messages before disconnection JSON key.
   */
  static constexpr CStringView kNoOfInvalidTcpMessagesBeforeDisconnection{
      "no_of_invalid_tcp_messages_before_disconnection"_sv};

  /*!
   * \brief Number of consecutive valid tcp messages before error reset JSON key.
   */
  static constexpr CStringView kNoOfConsecutiveValidTcpMessagesBeforeErrorReset{
      "no_of_consecutive_valid_tcp_messages_before_error_reset"_sv};

  /*!
   * \brief Secure connection IDs JSON key.
   */
  static constexpr CStringView kSecureIdsKey{"secure_ids"_sv};

  /*!
   * \brief Type-alias for NetworkEndpointPortConfigObject
   */
  using NetworkEndpointPortConfigObject = configuration::model::NetworkEndpointPortConfigObject;

  /*!
   * \brief Input for this parser is the container of 'Network endpoint port' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] network_endpoint_ports A reference to the container which is extended on every intermediate
   * 'Network endpoint ports' element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  NetworkEndpointPortsParser(amsr::json::JsonData &file,
                             ConfigurationTypesAndDefs::NetworkEndpointPortContainer &network_endpoint_ports) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~NetworkEndpointPortsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  NetworkEndpointPortsParser(NetworkEndpointPortsParser const &) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  NetworkEndpointPortsParser(NetworkEndpointPortsParser &&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(NetworkEndpointPortsParser const &) -> NetworkEndpointPortsParser & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(NetworkEndpointPortsParser &&) -> NetworkEndpointPortsParser & = delete;

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
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Start object means that a new network endpoint ports object was found.
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
   * \return ParserState::kRunning if in secure_ids sub-array or ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndArray(std::size_t) noexcept;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
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
   * \brief Bool callback.
   * \param[in] val Parsed boolean value
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnBool(bool val) noexcept;

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
   * \brief Flag to distinguish if parser is inside a sub-value array.
   */
  bool is_inside_sub_array_{false};

  /*!
   * \brief Intermediate object to store parsed secure IDs.
   */
  ConfigurationTypesAndDefs::SecureIdContainer secure_ids_;

  /*!
   * \brief Temporary POD structure for a single 'network endpoint port'.
   */
  ConfigurationTypesAndDefs::NetworkEndpointPort network_endpoint_port_{
      ConfigurationTypesAndDefs::kInvalidPort /* Port number */,
      ConfigurationTypesAndDefs::Protocol::kTCP /* protocol */,
      {
          kQosDefault /* Qos option */,
          ara::core::nullopt /* KeepAlive option */,
          ara::core::nullopt /* Default Socket option lingering */,
          ara::core::nullopt /* Socket Nagle not present */,
          kBulkReadReceivePeriodUsDefault /* Bulk read receive period us not present */,
          0U /* Invalid messages before disconnect */,
          0U /* Invalid consecutive messages before error reset */,
      } /* service options */,
      ConfigurationTypesAndDefs::ComRequestedType::kNotRequested /* request for endpoint */,
      0ULL /* UDP message accumulation threshold */,
      ara::core::nullopt /* Secure Ids */,
  };

  /*!
   * \brief Reference to the 'Network endpoint port' container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::NetworkEndpointPortContainer &network_endpoint_ports_;

  /*!
   * \brief Current intermediate 'Network endpoint port' object to fill with information.
   */
  NetworkEndpointPortConfigObject network_endpoint_port_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_NETWORK_ENDPOINT_PORTS_PARSER_H_

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
/*!        \file  secure_com_secure_connections_parser.h
 *        \brief  JSON parser for 'SecureComSecureConnections'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTIONS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTIONS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_validator.h"
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
 * \brief Parses 'secure connection' elements from JSON a secure connection object.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'secure connection' meets all constraints.
 * 3. Push to 'Secure connection' container.
 */
class SecureComSecureConnectionsParser final : public amsr::json::Parser<SecureComSecureConnectionsParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<SecureComSecureConnectionsParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::SecureComSecureConnectionValidator;

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
   * \brief Secure connection secure id JSON key.
   */
  static constexpr CStringView kSecureIdKey{"secure_id"_sv};

  /*!
   * \brief Secure connection TLS version JSON key.
   */
  static constexpr CStringView kTlsVersionKey{"tls_version"_sv};

  /*!
   * \brief Secure connection cipher suite id JSON key.
   */
  static constexpr CStringView kCipherSuiteIdIdKey{"cipher_suite_id"_sv};

  /*!
   * \brief Secure connection is priority key.
   */
  static constexpr CStringView kPriorityKey{"priority"_sv};

  /*!
   * \brief Secure connection client PSK config JSON key.
   */
  static constexpr CStringView kClientPskConfigKey{"client_psk_config"_sv};

  /*!
   * \brief Secure connection server PSK config JSON key.
   */
  static constexpr CStringView kServerPskConfigKey{"server_psk_config"_sv};

  /*!
   * \brief Secure connection certification config JSON key.
   */
  static constexpr CStringView kCertConfigKey{"cert_config"_sv};

  /*!
   * \brief Secure endpoint dtls cookie verification enabled key.
   */
  static constexpr CStringView kDtlsCookieVerificationEnabledKey{"dtls_cookie_verification_enabled"_sv};

  /*!
   * \brief Type alias for SecureComSecureConnectionObject
   */
  using SecureComSecureConnectionObject = configuration::model::SecureComSecureConnectionObject;

  /*!
   * \brief Input for this parser is the container of 'secureConnection' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] secure_connections A reference to the container which is extended on every intermediate
   * 'secure connection' element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureComSecureConnectionsParser(amsr::json::JsonData& file,
                                   ConfigurationTypesAndDefs::SecureConnectionContainer& secure_connections) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionsParser(SecureComSecureConnectionsParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionsParser(SecureComSecureConnectionsParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionsParser const&) -> SecureComSecureConnectionsParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SecureComSecureConnectionsParser&&) -> SecureComSecureConnectionsParser& = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to instantiate sub-parsers for sub-hierarchies.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kRunning or an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Start object means that a Secure connection object was found.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

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
   * \brief Callback on end of JSON array.
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnEndArray(std::size_t) noexcept;

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
   * \brief Callback on string found in JSON.
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
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Reference to the 'secure connections' container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::SecureConnectionContainer& secure_connections_;

  /*!
   * \brief This POD structure will be added to the container of secure connections, if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::SecureConnection secure_connection_;

  /*!
   * \brief Current intermediate 'secure com connection' object to fill with information.
   */
  SecureComSecureConnectionObject secure_connection_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTIONS_PARSER_H_

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
/*!        \file  secure_com_secure_endpoint_parser.h
 *        \brief  JSON parser for 'SecureComSecureEndpoint'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_ENDPOINT_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_ENDPOINT_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_endpoint_validator.h"
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
 * \brief Parses 'secure endpoint' elements from JSON into a container of services.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'secure com channel mappings' meets all constraints.
 * 3. Push to 'Secure endpoint container.
 */
class SecureComSecureEndpointParser final : public amsr::json::Parser<SecureComSecureEndpointParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<SecureComSecureEndpointParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::SecureComSecureEndpointValidator;

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
   * \brief Secure endpoint address key.
   */
  static constexpr CStringView kAddressKey{"address"_sv};

  /*!
   * \brief Secure endpoint port key.
   */
  static constexpr CStringView kPortKey{"port"_sv};

  /*!
   * \brief Secure endpoint transport protocol JSON key.
   */
  static constexpr CStringView kTransportProtocolKey{"transport_proto"_sv};

  /*!
   * \brief Secure endpoint is server key.
   */
  static constexpr CStringView kIsServerKey{"is_server"_sv};

  /*!
   * \brief Secure endpoint cipher suite ids key.
   */
  static constexpr CStringView kCipherSuiteIdsKey{"cipher_suite_ids"_sv};

  /*!
   * \brief Secure endpoint PSK UUID JSON key.
   */
  static constexpr CStringView kPskUuidKey{"psk_uuid"_sv};

  /*!
   * \brief Secure endpoint PSK identity JSON key.
   */
  static constexpr CStringView kPskIdentityKey{"psk_identity"_sv};

  /*!
   * \brief Secure endpoint PSK identity hint JSON key.
   */
  static constexpr CStringView kPskIdentityHintKey{"psk_identity_hint"_sv};

  /*!
   * \brief Secure endpoint dtls cookie verificatoin enabled key.
   */
  static constexpr CStringView kDtlsCookieVerificationEnabledKey{"dtls_cookie_verification_enabled"_sv};

  /*!
   * \brief TCP protocol.
   */
  static constexpr CStringView kTcp{"tcp"_sv};

  /*!
   * \brief UDP protocol.
   */
  static constexpr CStringView kUdp{"udp"_sv};

  /*!
   * \brief Type alias for SecureComChannelMappingObject
   */
  using SecureComSecureEndpointObject = configuration::model::SecureComSecureEndpointObject;

  /*!
   * \brief Input for this parser is the container of 'secureEndpoint' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] secure_endpoints A reference to the container which is extended on every intermediate
   * 'secure endpoint' element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureComSecureEndpointParser(amsr::json::JsonData& file,
                                ConfigurationTypesAndDefs::SecureEndpointContainer& secure_endpoints) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureEndpointParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureEndpointParser(SecureComSecureEndpointParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureEndpointParser(SecureComSecureEndpointParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureEndpointParser const&) -> SecureComSecureEndpointParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SecureComSecureEndpointParser&&) -> SecureComSecureEndpointParser& = delete;

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
  ParserResult OnKey(CStringView key) const noexcept;

  /*!
   * \brief Start object means that a Secure endpoint object was found.
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
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept;

  /*!
   * \brief Callback on boolean found in JSON.
   * \param[in] val value returned by the parser.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnBool(bool val) noexcept;

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
   * \brief Flag to distinguish if parser is inside a sub-value array.
   * \details This is mandatory for pure numerical arrays only.
   */
  bool is_inside_sub_array_{false};

  /*!
   * \brief Reference to the cipher suite id container that is extended for every suite ID found.
   */
  ConfigurationTypesAndDefs::CipherSuiteIdContainer secure_channel_cipher_suite_ids_;

  /*!
   * \brief Reference to the 'secure endpoints' container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::SecureEndpointContainer& secure_endpoints_;

  /*!
   * \brief This POD structure will be added to the container of secure endpoints, if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::SecureEndpoint secure_endpoint_;

  /*!
   * \brief Current intermediate 'secure com endpoint' object to fill with information.
   */
  SecureComSecureEndpointObject secure_endpoint_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_ENDPOINT_PARSER_H_

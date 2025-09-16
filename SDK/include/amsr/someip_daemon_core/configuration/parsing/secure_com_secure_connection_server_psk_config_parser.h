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
/*!        \file  secure_com_secure_connection_server_psk_config_parser.h
 *        \brief  JSON parser for 'SecureComSecureConnectionPskConfig'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_SERVER_PSK_CONFIG_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_SERVER_PSK_CONFIG_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_server_psk_config_validator.h"
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
 * \brief Parses 'secure connection server psk config' elements from JSON a PSK configuration object and validates
 * if PSK configuration object meets all server-side requirements.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'secure connection server psk config' meets all constraints.
 */
class SecureComSecureConnectionServerPskConfigParser final
    : public amsr::json::Parser<SecureComSecureConnectionServerPskConfigParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<SecureComSecureConnectionServerPskConfigParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::SecureComSecureConnectionServerPskConfigValidator;

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
   * \brief Secure connection psk config identity hint JSON key.
   */
  static constexpr CStringView kPskIdentityHint{"psk_identity_hint"_sv};

  /*!
   * \brief Secure connection psk config server identity map key.
   */
  static constexpr CStringView kPskIdentityMap{"psk_identity_map"_sv};

  /*!
   * \brief Type alias for SecureComSecureConnectionObject
   */
  using SecureComSecureConnectionPskConfigObject = configuration::model::SecureComSecureConnectionPskConfigObject;

  /*!
   * \brief Input for this parser is a PskConfig of a SecureConnection to fill.
   * \param[in] file JSON document to parse.
   * \param[out] psk_config A reference to the psk config element which is stored into a secure connection element if
   * that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureComSecureConnectionServerPskConfigParser(amsr::json::JsonData& file,
                                                 ConfigurationTypesAndDefs::PskConfig& psk_config) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionServerPskConfigParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionServerPskConfigParser(SecureComSecureConnectionServerPskConfigParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionServerPskConfigParser(SecureComSecureConnectionServerPskConfigParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionServerPskConfigParser const&)
      -> SecureComSecureConnectionServerPskConfigParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SecureComSecureConnectionServerPskConfigParser&&)
      -> SecureComSecureConnectionServerPskConfigParser& = delete;

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
   * \brief Start object means that a Secure connection cert config object was found.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) const noexcept;

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
   * \brief Server PSK configuration.
   */
  ConfigurationTypesAndDefs::PskConfig& psk_config_;

  /*!
   * \brief Current intermediate 'secure com secure connection psk config' object to fill with information..
   */
  SecureComSecureConnectionPskConfigObject psk_config_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_SERVER_PSK_CONFIG_PARSER_H_

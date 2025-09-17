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
/*!        \file  secure_com_secure_connection_client_psk_config_parser.h
 *        \brief  JSON parser for 'SecureComSecureConnectionPskConfig'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_CLIENT_PSK_CONFIG_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_CLIENT_PSK_CONFIG_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_client_psk_config_validator.h"
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
 * \brief Parses 'secure connection client psk config' elements from JSON into a PSK configuration object and validates
 * if PSK configuration object meets all client-side requirements.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'secure connection cert config' meets all constraints.
 */
class SecureComSecureConnectionClientPskConfigParser final
    : public amsr::json::Parser<SecureComSecureConnectionClientPskConfigParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<SecureComSecureConnectionClientPskConfigParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::SecureComSecureConnectionClientPskConfigValidator;

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
   * \brief secure connection client psk config identity map key.
   */
  static constexpr CStringView kPskIdentityMap{"psk_identity_map"_sv};

  /*!
   * \brief Type alias for SecureComSecureConnectionPskConfigObject
   */
  using SecureComSecureConnectionPskConfigObject = configuration::model::SecureComSecureConnectionPskConfigObject;

  /*!
   * \brief Input for this parser is a 'psk config' object from a secure connection configuration object to fill a
   * client-side PSK configuration.
   * \param[in] file JSON document to parse.
   * \param[out] psk_config A reference to the psk config element which is stored into a secure connection element if
   * that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureComSecureConnectionClientPskConfigParser(amsr::json::JsonData& file,
                                                 ConfigurationTypesAndDefs::PskConfig& psk_config) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionClientPskConfigParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionClientPskConfigParser(SecureComSecureConnectionClientPskConfigParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionClientPskConfigParser(SecureComSecureConnectionClientPskConfigParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionClientPskConfigParser const&)
      -> SecureComSecureConnectionClientPskConfigParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SecureComSecureConnectionClientPskConfigParser&&)
      -> SecureComSecureConnectionClientPskConfigParser& = delete;

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
   * \brief Client PSK configuration.
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_CLIENT_PSK_CONFIG_PARSER_H_

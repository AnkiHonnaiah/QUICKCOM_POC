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
/*!        \file  secure_com_secure_connection_cert_config_parser.h
 *        \brief  JSON parser for 'SecureComSecureConnectionCertConfig'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_cert_config_validator.h"
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
 * \brief Parses 'secure connection cert config' element from JSON into a certificate configuration object.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'secure connection cert config' meets all constraints.
 */
class SecureComSecureConnectionCertConfigParser final
    : public amsr::json::Parser<SecureComSecureConnectionCertConfigParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<SecureComSecureConnectionCertConfigParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::SecureComSecureConnectionCertConfigValidator;

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
   * \brief Secure connection cert config private key UUID JSON key.
   */
  static constexpr CStringView kPrivateKeyUuidKey{"private_key_uuid"_sv};

  /*!
   * \brief Secure connection cert config certificate chain label list JSON key.
   */
  static constexpr CStringView kCertChainLabeListKey{"certificate_chain_label_list"_sv};

  /*!
   * \brief Type alias for SecureComSecureConnectionCertConfigObject
   */
  using SecureComSecureConnectionCertConfigObject = configuration::model::SecureComSecureConnectionCertConfigObject;

  /*!
   * \brief Input for this parser is an object of 'cert config' from the secure connection configuration object.
   * \param[in] file JSON document to parse.
   * \param[out] cert_config A reference to the cert config element which is stored into a secure connection element if
   * that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureComSecureConnectionCertConfigParser(amsr::json::JsonData& file,
                                            ConfigurationTypesAndDefs::CertConfig& cert_config) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionCertConfigParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionCertConfigParser(SecureComSecureConnectionCertConfigParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionCertConfigParser(SecureComSecureConnectionCertConfigParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionCertConfigParser const&)
      -> SecureComSecureConnectionCertConfigParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SecureComSecureConnectionCertConfigParser&&) -> SecureComSecureConnectionCertConfigParser& = delete;

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
   * \brief This POD structure will be returned and added to a secure connection, if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::CertConfig& cert_config_;

  /*!
   * \brief List of all certificate labels.
   */
  ConfigurationTypesAndDefs::CertChainLabelContainer certificate_label_list_;

  /*!
   * \brief Current intermediate 'secure com secure connection cert config' object to fill with information.
   */
  SecureComSecureConnectionCertConfigObject secure_connection_cert_config_element_wrapper_;

  /*!
   * \brief Flag to distinguish if parser is inside a sub-value array.
   */
  bool is_inside_sub_array_{false};
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_PARSER_H_

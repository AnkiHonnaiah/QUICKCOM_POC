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
/*!        \file  secure_com_secure_connection_psk_config_identity_map_elements_parser.h
 *        \brief  JSON parser for 'SecureComSecureConnectionPskConfigIdentityMapElement'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENTS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENTS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_connection_psk_config_identity_map_element_validator.h"
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
 * \brief Parses 'secure connection psk configuration identity map element' from JSON into a container of psk identity
 * map elements.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'secure connection psk configuration identity map element' meets all constraints.
 * 3. Push to 'secure connection psk configuration identity map element' container.
 */
class SecureComSecureConnectionPskConfigIdentityMapElementsParser final
    : public amsr::json::Parser<SecureComSecureConnectionPskConfigIdentityMapElementsParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<SecureComSecureConnectionPskConfigIdentityMapElementsParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::SecureComSecureConnectionPskConfigIdentityMapElementValidator;

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
   * \brief Secure connection psk config identity map element psk identity hint JSON key.
   */
  static constexpr CStringView kPskIdentityHintKey{"psk_identity_hint"_sv};

  /*!
   * \brief Secure connection psk config identity map element psk identifier JSON key.
   */
  static constexpr CStringView kPskIdentityKey{"psk_id"_sv};

  /*!
   * \brief Secure connection psk config identity map element psk UUID JSON key.
   */
  static constexpr CStringView kPskUuidKey{"psk_uuid"_sv};

  /*!
   * \brief Type alias for SecureComSecureConnectionObject
   */
  using SecureComSecureConnectionPskConfigIdentityMapElementObject =
      configuration::model::SecureComSecureConnectionPskConfigIdentityMapElementObject;

  /*!
   * \brief Input for this parser is the container of 'psk identity map elements' from the PSK configuration object to
   * fill.
   * \param[in] file JSON document to parse.
   * \param[out] psk_identity_map_element_container A reference to the psk identity map element.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureComSecureConnectionPskConfigIdentityMapElementsParser(
      amsr::json::JsonData& file,
      ConfigurationTypesAndDefs::PskIdentityMapElementContainer& psk_identity_map_element_container) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionPskConfigIdentityMapElementsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionPskConfigIdentityMapElementsParser(
      SecureComSecureConnectionPskConfigIdentityMapElementsParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionPskConfigIdentityMapElementsParser(
      SecureComSecureConnectionPskConfigIdentityMapElementsParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionPskConfigIdentityMapElementsParser const&)
      -> SecureComSecureConnectionPskConfigIdentityMapElementsParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SecureComSecureConnectionPskConfigIdentityMapElementsParser&&)
      -> SecureComSecureConnectionPskConfigIdentityMapElementsParser& = delete;

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
   * \brief Start object means that a Secure connection object was found.
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
   * \brief This POD structure will be added to the container of psk identity map elements, if the parsed element is
   * valid.
   */
  ConfigurationTypesAndDefs::PskIdentityMapElement psk_identity_map_element_;

  /*!
   * \brief Reference to the 'psk identity map elements' container that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::PskIdentityMapElementContainer& psk_identity_map_element_container_;

  /*!
   * \brief Current intermediate 'secure com connection' object to fill with information.
   */
  SecureComSecureConnectionPskConfigIdentityMapElementObject secure_connection_psk_config_identity_map_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENTS_PARSER_H_

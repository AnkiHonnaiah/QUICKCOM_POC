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
/*!       \file   secure_com_parser.h
 *        \brief  JSON parser header for 'sec_com'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_validator.h"
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
 * \brief Parses a single 'Sec com' elements from JSON.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'Sec com' meet all constraints.
 * 3. Push to 'Sec com' container.
 */
class SecureComParser final : public amsr::json::Parser<SecureComParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<SecureComParser>;

  /*!
   * \brief Type alias for used validation class.
   */
  using Validation = configuration::model::SecureComValidator;

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
   * \brief Machine PSK identity hint key.
   */
  static constexpr CStringView kMachinePskIdentityHintKey{"psk_identity_hint"_sv};

  /*!
   * \brief Secure endpoints JSON key.
   */
  static constexpr CStringView kSecureEndpointsKey{"secure_endpoints"_sv};

  /*!
   * \brief Secure connections JSON key.
   */
  static constexpr CStringView kSecureConnectionsKey{"secure_connections"_sv};

  /*!
   * \brief Type-alias for SecComConfigObject
   */
  using SecComConfigObject = configuration::model::SecureComConfigObject;

  /*!
   * \brief Input for this parser is the 'Sec com' object from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] sec_com A reference to the object to extend for every element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureComParser(amsr::json::JsonData& file, configuration::ConfigurationTypesAndDefs::SecureCom& sec_com) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComParser(SecureComParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComParser(SecureComParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComParser const&) -> SecureComParser& = delete;

  /*!
   * \brief Delete move assignment operator.
   */
  auto operator=(SecureComParser&&) -> SecureComParser& = delete;

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
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Start object means that a new SecureCom object was found.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

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
   * \param val value returned by the parser.
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
   * \brief This POD structure of 'Sec com'.
   */
  configuration::ConfigurationTypesAndDefs::SecureCom& secure_com_;

  /*!
   * \brief Current intermediate 'Sec com' object to fill with information.
   */
  SecComConfigObject secure_com_element_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SECURE_COM_PARSER_H_

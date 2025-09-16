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
/*!        \file  someipd_posix_json_parser.h
 *        \brief  Root parser for someipd-posix.json file.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SOMEIPD_POSIX_JSON_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SOMEIPD_POSIX_JSON_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/model/someipd_posix_json_validator.h"
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
 * \brief Parses 'someipd posix json info' elements from JSON into a someipd posix json info.
 * \details
 * 1. Parse / read in
 * 2. Validate that the parsed 'someipd posix json info' meet all constraints.
 * 3. Push to 'someipd posix json info' container.
 */
class SomeIpdPosixJsonParser final : public amsr::json::Parser<SomeIpdPosixJsonParser> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<SomeIpdPosixJsonParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::SomeIpdPosixJsonValidator;

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
   * \brief Someip daemon posix json info port key.
   */
  static constexpr CStringView kPortKey{"someipd_ipc_port"_sv};

  /*!
   * \brief Someip daemon posix json info domain key.
   */
  static constexpr CStringView kDomainKey{"someipd_ipc_domain"_sv};

  /*!
   * \brief Someip daemon posix json info applications key.
   */
  static constexpr CStringView kApplicationsKey{"applications"_sv};

  /*!
   * \brief Someip daemon in process trustzone provider UUID key.
   */
  static constexpr CStringView kTrustzoneCryptoProviderUuidKey{"trustzone_crypto_provider_uuid"_sv};

  /*!
   * \brief SOME/IP daemon in process trustzone provider UUID key.
   */
  static constexpr CStringView kTlsEnableRemoteCryptoModeKey{"tls_crypto_mode_remote"_sv};

  /*!
   * \brief Someip daemon posix json info merge RSI with different minor version key.
   */
  static constexpr CStringView kMergeRSIWithDifferentMinorVersionKey{"merge_rsi_with_different_minor_versions"_sv};

  /*!
   * \brief Input for this parser is the 'someipd posix json info' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] someipd_posix_json A reference to the object to extend for every element that is valid.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SomeIpdPosixJsonParser(amsr::json::JsonData& file,
                         configuration::ConfigurationTypesAndDefs::SomeIpdPosixJson& someipd_posix_json) noexcept;

  /*!
   *
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SomeIpdPosixJsonParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SomeIpdPosixJsonParser(SomeIpdPosixJsonParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SomeIpdPosixJsonParser(SomeIpdPosixJsonParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SomeIpdPosixJsonParser const&) -> SomeIpdPosixJsonParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SomeIpdPosixJsonParser&&) -> SomeIpdPosixJsonParser& = delete;

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
   * \brief Callback on start of JSON object.
   * \return ParserResult with ParserState::kRunning
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
   * \brief End Array
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndArray(std::size_t) noexcept;

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
   * \brief Callback on number found in JSON.
   * \param[in] num value returned by the parser.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) const noexcept;

  /*!
   * \brief Callback on String found in JSON.
   * \param[in] val value returned by the parser.
   * \return ParserResult{ParserState::kRunning} if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept;

  /*!
   * \brief Callback on null found in JSON.
   * \return An error code, because it indicates an erroneously configured key, as the someipd_posix.json should not
   * contain any null.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNull() const noexcept;

  /*!
   * \brief Callback on bool found in JSON.
   * \param[in] val value returned by the parser.
   * \return An error code, because it indicates an erroneously configured key, as the someipd_posix.json should not
   * contain any bools.
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
   * \brief Reference to the application container that is extended for every application found.
   */
  ConfigurationTypesAndDefs::ApplicationContainer someipd_posix_json_applications_;

  /*!
   * \brief Reference to the POD structure of this 'someipd_posix' json.
   */
  ConfigurationTypesAndDefs::SomeIpdPosixJson& someipd_posix_json_;

  /*!
   * \brief Current someipd posix json object to fill with information.
   */
  configuration::model::SomeIpdPosixJsonObject someipd_posix_json_element_wrapper_;

  /*!
   * \brief For testing GetMergeRSIWithDifferentMinorVersion() using SomeIpdPosixJsonObject, which is private.
   */
  FRIEND_TEST(UT__SomeIpdPosixJsonParser, MergeRSIWithDifferentMinorVersions);

  /*!
   * \brief For testing GetTlsRemoteCryptoMode() using SomeIpdPosixJsonObject, which is private.
   */
  FRIEND_TEST(UT__SomeIpdPosixJsonParser, RemoteCryptoModeEnabled);

  /*!
   * \brief For testing GetTrustzoneCryptoProviderUuid() using SomeIpdPosixJsonObject, which is private.
   */
  FRIEND_TEST(UT__SomeIpdPosixJsonParser, ParsingWithAllSubelements);
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_SOMEIPD_POSIX_JSON_PARSER_H_

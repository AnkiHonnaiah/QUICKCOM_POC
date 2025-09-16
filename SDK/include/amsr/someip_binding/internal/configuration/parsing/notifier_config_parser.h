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
/**        \file  notifier_config_parser.h
 *        \brief  JSON parser for an notifier deployment.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::NotifierConfigParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_NOTIFIER_CONFIG_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_NOTIFIER_CONFIG_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/someip_binding/internal/configuration/notifier_config.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {
namespace parsing {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief   Parser for the Notifier JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...)
 */
class NotifierConfigParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief Name alias for amsr::someip_binding::internal::configuration::NotifierConfig.
   */
  using NotifierConfig = ::amsr::someip_binding::internal::configuration::NotifierConfig;

  /*!
   * \brief       Overriding base class method.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> Parse() noexcept final { return ::amsr::json::v2::SingleObjectParser::Parse(); }

  /*!
   * \brief         Construct the JSON parser.
   * \details       Start parsing using Parse() API.
   * \param[in,out] json    JSON document to parse.
   * \param[in,out] config  A reference to the configuration object to be filled by the parser.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  NotifierConfigParser(amsr::json::JsonData& json, NotifierConfig& config) noexcept;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~NotifierConfigParser() final = default;

  NotifierConfigParser(NotifierConfigParser const&) noexcept = delete;
  NotifierConfigParser& operator=(NotifierConfigParser const&) & = delete;
  NotifierConfigParser(NotifierConfigParser&&) noexcept = delete;
  NotifierConfigParser& operator=(NotifierConfigParser&&) & = delete;

  /*!
   * \brief       Callback on a new key parsed from the JSON object.
   * \param[in]   json_key Parsed key string.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult OnKey(amsr::core::StringView json_key) noexcept final;

  /*!
   * \brief       Callback if an unexpected event happened during JSON parsing.
   * \return      Always a parser result with related error code.
   * \error       SomeIpBindingErrc::json_parsing_failure in any case.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult OnUnexpectedEvent() noexcept final;

  /*!
   * \brief Callback to validate that all configuration elements have been accepted.
   * \return either nothing or the error.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> Finalize() noexcept final;

 private:
  /*! JSON key for the notifier id. */
  static constexpr amsr::core::StringView kNotifierIdKey{"id"_sv};

  /*! JSON key for the disable E2E check. */
  static constexpr amsr::core::StringView kDisbaleE2ECheckKey{"disable_e2e_check"_sv};

  /*! JSON key for the disable session handling.
   * \trace SPEC-13650689
   */
  static constexpr amsr::core::StringView kDisableSessionHandlingKey{"disable_session_handling"_sv};

  /*!
   * \brief       Parse the notifier ID from JSON.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseNotifierId() noexcept;

  /*!
   * \brief       Parse the disable E2E check from JSON.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseDisableE2eCheck() noexcept;

  /*!
   * \brief       Parse the disable session handling from JSON.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseDisableSessionHandling() noexcept;

  /*!
   * \brief     Map error code to binding specific JSON parsing failure error code.
   * \param[in] error_code ErrorCode to be mapped.
   * \return    Mapped ErrorCode
   * \error     SomeIpBindingErrc::json_parsing_failure in any case.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::ErrorCode MapToBindingError(amsr::core::ErrorCode const& error_code) noexcept;

  std::size_t number_of_notifier_ids_found_{0};              /*!< Number of key 'notifier_id' found.*/
  std::size_t number_of_disable_e2e_checks_found_{0};        /*!< Number of key 'disable_e2e_check' found.*/
  std::size_t number_of_disable_session_handlings_found_{0}; /*!< Number of key 'disable_session_handling' found.*/

  /*! Reference to configuration struct filled by the parser. */
  NotifierConfig& config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_NOTIFIER_CONFIG_PARSER_H_

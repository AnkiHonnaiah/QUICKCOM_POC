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
/**        \file  e2e_profile_config_parser.h
 *        \brief  JSON parser for the E2EProfile configuration.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::E2EProfileConfigParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_E2E_PROFILE_CONFIG_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_E2E_PROFILE_CONFIG_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/someip_binding/internal/configuration/e2e_profile_config.h"
#include "amsr/someip_binding_core/internal/error_domain.h"
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

// VECTOR Next Construct Metric-OO.WMC.One: MD_SOMEIPBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief   Parser for the E2EProfileConfig JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...)
 */
class E2EProfileConfigParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*! Type-alias for easy access to ParserResult. */
  using ParserResult = ::amsr::json::ParserResult;

  /*! Name alias for amsr::someip_binding::internal::configuration::E2EProfileConfig. */
  using E2EProfileConfig = ::amsr::someip_binding::internal::configuration::E2EProfileConfig;

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
   * \param[in]     json    JSON document to parse.
   * \param[in,out] config  A reference to the configuration object to be filled by the parser.
   *
   * \pre           -
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  E2EProfileConfigParser(amsr::json::JsonData& json, E2EProfileConfig& config) noexcept;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~E2EProfileConfigParser() final = default;

  E2EProfileConfigParser(E2EProfileConfigParser const&) noexcept = delete;
  E2EProfileConfigParser& operator=(E2EProfileConfigParser const&) & = delete;
  E2EProfileConfigParser(E2EProfileConfigParser&&) noexcept = delete;
  E2EProfileConfigParser& operator=(E2EProfileConfigParser&&) & = delete;

  /*!
   * \brief       Callback on a new key parsed from the JSON object.
   * \param[in]   json_key Parsed key string.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_key_not_found_failure if JSON parsing failed.
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
   * \error       SomeIpBindingErrc::json_parsing_unexpected_event_failure in any case.
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
   * \brief       Parse the shortname
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_string_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseShortname() noexcept;

  /*!
   * \brief       Parse the profile name
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_unsupported_e2e_profile_configuration_failure
   *              Unsupported E2E profile configuration.
   * \error       SomeIpBindingErrc::json_parsing_string_parsing_failure
   *              String parsing failed.
   * \pre -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseProfileName() noexcept;

  /*!
   * \brief       Parse the max delta counter
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseMaxDeltaCounter() noexcept;

  /*!
   * \brief       Parse the min ok state init
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseMinOkStateInit() noexcept;

  /*!
   * \brief       Parse the max error state init
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseMaxErrorStateInit() noexcept;

  /*!
   * \brief       Parse the min ok state valid
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseMinOkStateValid() noexcept;

  /*!
   * \brief       Parse the max error state valid
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseMaxErrorStateValid() noexcept;

  /*!
   * \brief       Parse the min ok state invalid
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseMinOkStateInvalid() noexcept;

  /*!
   * \brief       Parse the max error state invalid
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseMaxErrorStateInvalid() noexcept;

  /*!
   * \brief       Parse the window size valid
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseWindowSizeValid() noexcept;

  /*!
   * \brief       Parse the window size init
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseWindowSizeInit() noexcept;

  /*!
   * \brief       Parse the window size invalid
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseWindowSizeInvalid() noexcept;

  /*!
   * \brief       Parse the clear from valid to invalid.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseClearToInvalid() noexcept;

  /*!
   * \brief       Parse the transit to invalid extended.
   * \return      Parser result.
   * \error       SomeIpBindingErrc::json_parsing_number_parsing_failure if JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ParserResult ParseTransitToInvalidExtended() noexcept;

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
  /*! JSON key for the shortname. */
  static constexpr amsr::core::StringView kShortnameKey{"shortname"_sv};
  /*! JSON key for the profile name. */
  static constexpr amsr::core::StringView kProfileNameKey{"profile_name"_sv};
  /*! JSON key for the max delta counter. */
  static constexpr amsr::core::StringView kMaxDeltaCounterKey{"max_delta_counter"_sv};
  /*! JSON key for the min ok state init . */
  static constexpr amsr::core::StringView kMinOkStateInitKey{"min_ok_state_init"_sv};
  /*! JSON key for the max error state init. */
  static constexpr amsr::core::StringView kMaxErrorStateInitKey{"max_error_state_init"_sv};
  /*! JSON key for the min ok state valid. */
  static constexpr amsr::core::StringView kMinOkStateValidKey{"min_ok_state_valid"_sv};
  /*! JSON key for the max error state valid. */
  static constexpr amsr::core::StringView kMaxErrorStateValidKey{"max_error_state_valid"_sv};
  /*! JSON key for the min ok state invalid. */
  static constexpr amsr::core::StringView kMinOkStateInvalidKey{"min_ok_state_invalid"_sv};
  /*! JSON key for the max error state invalid. */
  static constexpr amsr::core::StringView kMaxErrorStateInvalidKey{"max_error_state_invalid"_sv};
  /*! JSON key for the window size valid. */
  static constexpr amsr::core::StringView kWindowSizeValidKey{"window_size_valid"_sv};
  /*! JSON key for the window size init. */
  static constexpr amsr::core::StringView kWindowSizeInitKey{"window_size_init"_sv};
  /*! JSON key for the window size invalid. */
  static constexpr amsr::core::StringView kWindowSizeInvalidKey{"window_size_invalid"_sv};
  /*! JSON key for the clear from valid to invalid. */
  static constexpr amsr::core::StringView kClearToInvalidKey{"clear_from_valid_to_invalid"_sv};
  /*! JSON key for the transi to invalid extended. */
  static constexpr amsr::core::StringView kTransitToInvalidExtendedKey{"transit_to_invalid_extended"_sv};

  /*!
   * \brief       Parse the event ID from JSON.
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
  ParserResult ParseEventId() noexcept;

  /*!
   * \brief       Parse the data ID from JSON.
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
  ParserResult ParseDataId() noexcept;

  /*!
   * \brief     Map error code to binding specific JSON parsing failure error code.
   * \param[in] someip_binding_error_code SomeIp ErrorCode to be mapped to.
   * \param[in] error_code ErrorCode to be mapped.
   * \return    Mapped ErrorCode
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::ErrorCode MapToBindingError(
      ::amsr::someip_binding_core::internal::SomeIpBindingErrc const& someip_binding_error_code,
      amsr::core::ErrorCode const& error_code) noexcept;

  std::size_t number_of_shortnames_found_{0};               /*!< Number of key 'shortname' found. */
  std::size_t number_of_profile_names_found_{0};            /*!< Number of key 'profile_name' found. */
  std::size_t number_of_max_delta_counters_found_{0};       /*!< Number of key 'max_delta_counter' found. */
  std::size_t number_of_min_ok_state_inits_found_{0};       /*!< Number of key 'min_ok_state_init' found. */
  std::size_t number_of_max_error_state_inits_found_{0};    /*!< Number of key 'max_error_state_init' found. */
  std::size_t number_of_min_ok_state_valids_found_{0};      /*!< Number of key 'max_ok_state_valid' found. */
  std::size_t number_of_max_error_state_valids_found_{0};   /*!< Number of key 'max_error_state_valid' found. */
  std::size_t number_of_min_ok_state_invalids_found_{0};    /*!< Number of key 'min_ok_state_invalid' found. */
  std::size_t number_of_max_error_state_invalids_found_{0}; /*!< Number of key 'max_error_state_invalid' found. */
  std::size_t number_of_window_size_inits_found_{0};        /*!< Number of key 'window_size_init' found. */
  std::size_t number_of_window_size_valids_found_{0};       /*!< Number of key 'window_size_valid' found. */
  std::size_t number_of_window_size_invalids_found_{0};     /*!< Number of key 'window_size_invalid' found. */
  std::size_t number_of_clear_to_invalid_found_{0};         /*!< Number of key 'clear to invalid' found. */
  std::size_t number_of_transit_to_invalid_extended_found_{
      0}; /*!< Number of key 'transit to invalid extended' found. */

  /*! Reference to configuration struct filled by the parser. */
  E2EProfileConfig& config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_E2E_PROFILE_CONFIG_PARSER_H_

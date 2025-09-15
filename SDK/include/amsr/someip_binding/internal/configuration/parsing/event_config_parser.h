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
/**        \file  event_config_parser.h
 *        \brief  JSON parser for an event.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::EventConfigParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_EVENT_CONFIG_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_EVENT_CONFIG_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
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
 * \brief   Parser for the events JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...)
 */
class EventConfigParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief Name alias for amsr::someip_binding::internal::configuration::EventConfig.
   */
  using EventConfig = ::amsr::someip_binding::internal::configuration::EventConfig;

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
  EventConfigParser(amsr::json::JsonData& json, EventConfig& config) noexcept;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~EventConfigParser() final = default;

  EventConfigParser(EventConfigParser const&) noexcept = delete;
  EventConfigParser& operator=(EventConfigParser const&) & = delete;
  EventConfigParser(EventConfigParser&&) noexcept = delete;
  EventConfigParser& operator=(EventConfigParser&&) & = delete;

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
  /*! JSON key for the event id. */
  static constexpr amsr::core::StringView kEventIdKey{"id"_sv};

  /*! JSON key for the shortname. */
  static constexpr amsr::core::StringView kShortnameKey{"shortname"_sv};

  /*! JSON key for the serialization. */
  static constexpr amsr::core::StringView kSerializationKey{"serialization"_sv};

  /*! JSON enumeration value for the serialization 'SIGNAL-BASED'. */
  static constexpr amsr::core::StringView kSerializationSignalBasedValue{"SIGNAL-BASED"_sv};

  /*! JSON enumeration value for the serialization 'SOMEIP'. */
  static constexpr amsr::core::StringView kSerializationSomeIPValue{"SOMEIP"_sv};

  /*! JSON key for the Pdu header extension Tx. */
  static constexpr amsr::core::StringView kPduHdrExtnTxKey{"pdu_hdr_extn_tx"_sv};

  /*! JSON key for the Pdu header extension Rx. */
  static constexpr amsr::core::StringView kPduHdrExtnRxKey{"has_pdu_hdr_extn_rx"_sv};

  /*! JSON key for the disable E2E check. */
  static constexpr amsr::core::StringView kDisbaleE2ECheckKey{"disable_e2e_check"_sv};

  /*! JSON key for the disable session handling.
   * \trace SPEC-13650689
   */
  static constexpr amsr::core::StringView kDisableSessionHandlingKey{"disable_session_handling"_sv};

  /*! JSON key for the e2e update bit. */
  static constexpr ara::core::StringView kE2EUpdateBitPositionKey{"e2e_update_bit_position"_sv};

  /*! JSON key for the e2e protected offset. */
  static constexpr ara::core::StringView kE2EProtectedOffsetKey{"e2e_protected_offset"_sv};

  /*! JSON key for the has signal based e2e range value. */
  static constexpr ara::core::StringView kHasSignalBasedE2eRangeKey{"has_signal_based_e2e_range"_sv};

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
   * \brief       Parse the shortname from JSON.
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
  ParserResult ParseShortname() noexcept;

  /*!
   * \brief       Parse the serialization from JSON.
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
  ParserResult ParseSerialization() noexcept;

  /*!
   * \brief       Parse the Pdu header extension fields for Tx from JSON.
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
  ParserResult ParsePduHeaderExtensionTx() noexcept;

  /*!
   * \brief       Parse the Pdu header extension flag for Rx from JSON.
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
  ParserResult ParsePduHeaderExtensionRx() noexcept;

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
   * \brief       Parse the E2E update bit value from JSON.
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
  ParserResult ParseE2EUpdateBitPosition() noexcept;

  /*!
   * \brief       Parse the E2E protected offset value from JSON.
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
  ParserResult ParseE2EProtectedOffset() noexcept;

  /*!
   * \brief       Parse the has signal based e2e range value from JSON.
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
  ParserResult ParseHasSignalBasedE2eRange() noexcept;

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

  std::size_t number_of_event_ids_found_{0};                  /*!< Number of key 'event_id' found.*/
  std::size_t number_of_shortnames_found_{0};                 /*!< Number of key 'shortname' found.*/
  std::size_t number_of_serializations_found_{0};             /*!< Number of key 'serialization' found.*/
  std::size_t number_of_pdu_hdr_ext_tx_fields_found_{0};      /*!< Number of items for key 'pdu_hdr_extn_tx' found.*/
  std::size_t number_of_pdu_hdr_ext_rx_flag_found_{0};        /*!< Number of key 'has_pdu_hdr_extn_rx' found.*/
  std::size_t number_of_disable_e2e_checks_found_{0};         /*!< Number of key 'disable_e2e_check' found.*/
  std::size_t number_of_disable_session_handlings_found_{0};  /*!< Number of key 'disable_session_handling' found.*/
  std::size_t number_of_e2e_update_bit_position_found_{0};    /*!< Number of key 'e2e_update_bit' found.*/
  std::size_t number_of_e2e_protected_offset_found_{0};       /*!< Number of key 'e2e_protected_offset' found.*/
  std::size_t number_of_has_signal_based_e2e_range_found_{0}; /*!< Number of key 'has_signal_based_e2e_range' found.*/
  bool is_signal_based_{false};                               /*!< Value indicating serialization is signal based.*/

  /*! Reference to configuration struct filled by the parser. */
  EventConfig& config_;

  /*!
   * \brief This pdu header extension fields will be added to the configuration, if the parsed element is valid.
   */
  EventConfig::PduHeaderExtensionTx pdu_header_extension_tx{};
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_EVENT_CONFIG_PARSER_H_

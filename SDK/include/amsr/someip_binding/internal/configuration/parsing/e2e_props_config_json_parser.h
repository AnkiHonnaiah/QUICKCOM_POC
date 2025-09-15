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
/**        \file  e2e_props_config_json_parser.h
 *        \brief  JSON parser for the E2EProps configuration.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::E2EPropsConfigJsonParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_E2E_PROPS_CONFIG_JSON_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_E2E_PROPS_CONFIG_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/someip_binding/internal/configuration/e2e_props_config.h"
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

// VECTOR NC Metric-OO.WMC.One: MD_SOMEIPBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief   Parser for the E2E props JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...)
 */
class E2EPropsConfigJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*! Type-alias for easy access to ParserResult. */
  using ParserResult = ::amsr::json::ParserResult;

  /*! Name alias for amsr::someip_binding::internal::configuration::E2EPropsConfig. */
  using E2EPropsConfig = ::amsr::someip_binding::internal::configuration::E2EPropsConfig;

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
  E2EPropsConfigJsonParser(amsr::json::JsonData& json, E2EPropsConfig& config) noexcept;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~E2EPropsConfigJsonParser() final = default;

  E2EPropsConfigJsonParser(E2EPropsConfigJsonParser const&) noexcept = delete;
  E2EPropsConfigJsonParser& operator=(E2EPropsConfigJsonParser const&) & = delete;
  E2EPropsConfigJsonParser(E2EPropsConfigJsonParser&&) noexcept = delete;
  E2EPropsConfigJsonParser& operator=(E2EPropsConfigJsonParser&&) & = delete;

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
  static constexpr amsr::core::StringView kEventIdKey{"event_id"_sv};

  /*! JSON key for the data id. */
  static constexpr amsr::core::StringView kDataIdKey{"data_id"_sv};

  /*! JSON key for the data length. */
  static constexpr amsr::core::StringView kDataLengthKey{"data_length"_sv};

  /*! JSON key for the minimum data length. */
  static constexpr amsr::core::StringView kMinDataLengthKey{"min_data_length"_sv};

  /*! JSON key for the maximum data length. */
  static constexpr amsr::core::StringView kMaxDataLengthKey{"max_data_length"_sv};

  /*! JSON key for the counter offset. */
  static constexpr amsr::core::StringView kCounterOffsetKey{"counter_offset"_sv};

  /*! JSON key for the CRC offset. */
  static constexpr amsr::core::StringView kCrcOffsetKey{"crc_offset"_sv};

  /*! JSON key for the Header offset. */
  static constexpr amsr::core::StringView kHeaderOffsetKey{"header_offset"_sv};

  /*! JSON key for the data ID nibble offset. */
  static constexpr amsr::core::StringView kDataIdNibbleOffsetKey{"data_id_nibble_offset"_sv};

  /*! JSON key for the data ID mode. */
  static constexpr amsr::core::StringView kDataIdModeKey{"data_id_mode"_sv};

  /*! JSON value for data ID mode both. */
  static constexpr amsr::core::StringView kDataIdModeBothValue{"Both"_sv};

  /*! JSON value for data ID mode alt. */
  static constexpr amsr::core::StringView kDataIdModeAltValue{"Alt"_sv};

  /*! JSON value for data ID mode low. */
  static constexpr amsr::core::StringView kDataIdModeLowValue{"Low"_sv};

  /*! JSON value for data ID mode nibble. */
  static constexpr amsr::core::StringView kDataIdModeNibbleValue{"Nibble"_sv};

  /*! JSON key for the max no new or repeated data. */
  static constexpr amsr::core::StringView kMaxNoNewOrRepeatedDataKey{"max_no_new_or_repeated_data"_sv};

  /*! JSON key for the sync counter init. */
  static constexpr amsr::core::StringView kSyncCounterInitKey{"sync_counter"_sv};

  /*! JSON key for the data id list. */
  static constexpr amsr::core::StringView kDataIdListKey{"data_id_list"_sv};

  /*! JSON key for the E2E profile short name */
  static constexpr amsr::core::StringView kE2EProfileShortNameKey{"e2e_profile_shortname"_sv};

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
   * \brief       Parse the data length from JSON.
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
  ParserResult ParseDataLength() noexcept;

  /*!
   * \brief       Parse the minimum data length from JSON.
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
  ParserResult ParseMinDataLength() noexcept;

  /*!
   * \brief       Parse the maximum data length from JSON.
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
  ParserResult ParseMaxDataLength() noexcept;

  /*!
   * \brief       Parse the counter offset from JSON.
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
  ParserResult ParseCounterOffset() noexcept;

  /*!
   * \brief       Parse the CRC offset from JSON.
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
  ParserResult ParseCrcOffset() noexcept;

  /*!
   * \brief       Parse the Header offset from JSON.
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
  ParserResult ParseHeaderOffset() noexcept;

  /*!
   * \brief       Parse the data ID nibble offset from JSON.
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
  ParserResult ParseDataIdNibbleOffset() noexcept;

  /*!
   * \brief       Parse the data ID mode from JSON.
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
  ParserResult ParseDataIdMode() noexcept;

  /*!
   * \brief       Parse the maximum allowed failed counter checks from JSON.
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
  ParserResult ParseMaxNoNewOrRepeatedData() noexcept;

  /*!
   * \brief       Parse the number of checks from JSON.
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
  ParserResult ParseSyncCounterInit() noexcept;

  /*!
   * \brief       Parse the data id list from JSON.
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
  ParserResult ParseDataIdList() noexcept;
  /*!
   * \brief       Parse the E2EProfileShortName
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
  ParserResult ParseE2EShortName() noexcept;

  /*!
   * \brief       Check for duplicates in the parsed data ID list.
   * \return      True if duplicates are found.
   * \pre         ParseDataIdList() must be called before.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool HasDuplicateDataIds() const noexcept;

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

  std::size_t number_of_event_ids_found_{0};              /*!< Number of key 'event_id' found. */
  std::size_t number_of_data_ids_found_{0};               /*!< Number of key 'data_id' found. */
  std::size_t number_of_data_lengths_found_{0};           /*!< Number of key 'data_length' found. */
  std::size_t number_of_min_data_lengths_found_{0};       /*!< Number of key 'min_data_length' found. */
  std::size_t number_of_max_data_lengths_found_{0};       /*!< Number of key 'max_data_length' found. */
  std::size_t number_of_counter_offsets_found_{0};        /*!< Number of key 'counter_offset' found. */
  std::size_t number_of_crc_offsets_found_{0};            /*!< Number of key 'crc_offset' found. */
  std::size_t number_of_header_offsets_found_{0};         /*!< Number of key 'offset' found. */
  std::size_t number_of_data_id_nibble_offsets_found_{0}; /*!< Number of key 'data_id_nibble_offset' found. */
  std::size_t number_of_data_id_modes_found_{0};          /*!< Number of key 'data_id_mode' found. */
  std::size_t number_of_max_no_new_or_repeated_datas_found_{
      0};                                                 /*!< Number of key 'max_no_new_or_repeated_data' found. */
  std::size_t number_of_sync_counters_found_{0};          /*!< Number of key 'sync_counter' found. */
  std::size_t number_of_data_id_list_item_found_{0};      /*!< Number of items for key 'data_id_list' found. */
  std::size_t number_of_e2e_profile_shortnames_found_{0}; /*!< Number of items for key 'e2e_profile_shortname' found. */

  /*! Reference to configuration struct filled by the parser. */
  E2EPropsConfig& config_;

  /*!
   * \brief This data id list will be added to the configuration, if the parsed element is valid.
   */
  E2EPropsConfig::DataIdList data_id_list_{};
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_E2E_PROPS_CONFIG_JSON_PARSER_H_

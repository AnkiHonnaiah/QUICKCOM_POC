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
/**        \file
 *        \brief  JSON parser of event runtime configurations.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_EVENT_JSON_PARSER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_EVENT_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding/internal/config/parser/json_parser_util.h"
#include "ipc_binding_core/internal/runtime_configuration/event_config.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief   Parser for the event runtime JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...).
 *
 * \unit IpcBinding::RuntimeConfigJsonParser
 */
class EventJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Name alias for amsr::ipc_binding_core::internal::runtime_configuration::EventConfig.
   */
  using EventConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::EventConfig;

  /*!
   * \brief Type-alias for EventType.
   */
  using EventType = ::amsr::ipc_binding_core::internal::runtime_configuration::EventType;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   *
   * \param[in,out] json    JSON document to parse. The reference must stay valid as long as this instance exists.
   * \param[in,out] config  A reference to the configuration object to be filled by the parser. The reference must stay
   *                        valid as long as this instance exists.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  EventJsonParser(JsonData& json, EventConfig& config) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_IPCB_CLASS_DECLARATION
  ~EventJsonParser() noexcept final = default;

  EventJsonParser(EventJsonParser const&) noexcept = delete;
  auto operator=(EventJsonParser const&) noexcept -> EventJsonParser& = delete;
  EventJsonParser(EventJsonParser&&) noexcept = delete;
  auto operator=(EventJsonParser&&) noexcept -> EventJsonParser& = delete;

  /*!
   * \brief Callback on a new key parsed from the JSON object.
   *
   * \param[in] json_key  Parsed key string.
   *
   * \return Parser result.
   *
   * \error IpcBindingErrc::json_parsing_failure  If JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnKey(StringView json_key) noexcept -> ParserResult final;

  /*!
   * \brief Callback if an unexpected event happened during JSON parsing.
   *
   * \return Always a parser result with related error code.
   *
   * \error IpcBindingErrc::json_parsing_failure  In any case.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnUnexpectedEvent() noexcept -> ParserResult final;

  /*!
   * \brief Callback to validate that all configuration elements have been accepted.
   *
   * \return Either nothing or the error.
   *
   * \error IpcBindingErrc::json_parsing_failure  If validation fails.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto Finalize() noexcept -> ara::core::Result<void> final;

 private:
  /*! JSON key for the event id. */
  static constexpr StringView kEventIdKey{"id"_sv};

  /*! JSON key for the short name. */
  static constexpr StringView kShortNameKey{"shortname"_sv};

  /*!
   * \brief Parse the event ID from JSON.
   *
   * \return Parser result.
   *
   * \error IpcBindingErrc::json_parsing_failure  If JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseId() noexcept -> ParserResult;

  /*!
   * \brief Parse the short name from JSON.
   *
   * \return Parser result.
   *
   * \error IpcBindingErrc::json_parsing_failure  If JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto ParseShortName() noexcept -> ParserResult;

  std::size_t number_of_event_ids_found_{0};   /*!< Number of key 'id' found.*/
  std::size_t number_of_short_names_found_{0}; /*!< Number of key 'shortname' found.*/

  /*! Reference to configuration struct filled by the parser. */
  EventConfig& config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_EVENT_JSON_PARSER_H_

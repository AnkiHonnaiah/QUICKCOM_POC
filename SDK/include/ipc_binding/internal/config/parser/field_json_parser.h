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
 *        \brief  JSON parser of field runtime configurations.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_FIELD_JSON_PARSER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_FIELD_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/core/result.h"

#include "ipc_binding/internal/config/parser/json_parser_util.h"
#include "ipc_binding_core/internal/runtime_configuration/field_config.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief   Parser for the field runtime JSON configurations.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration class.
 *          - Basic check of parsed data types (valid number format...).
 *
 * \unit IpcBinding::RuntimeConfigJsonParser
 */
class FieldJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*! Type-alias for amsr::ipc_binding_core::internal::runtime_configuration::FieldConfig. */
  using FieldConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::FieldConfig;
  /*! Type-alias for FieldGetterConfig. */
  using FieldGetterConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig;
  /*! Type-alias for FieldNotifierConfig. */
  using FieldNotifierConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::EventConfig;
  /*! Type-alias for FieldSetterConfig. */
  using FieldSetterConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig;
  /*! Type-alias for field notifier identifier type. */
  using FieldId = ipc_binding_core::internal::ipc_protocol::InstanceId;

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
  FieldJsonParser(JsonData& json, FieldConfig& config) noexcept;

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
  ~FieldJsonParser() noexcept final = default;

  FieldJsonParser(FieldJsonParser const&) noexcept = delete;
  auto operator=(FieldJsonParser const&) noexcept -> FieldJsonParser& = delete;
  FieldJsonParser(FieldJsonParser&&) noexcept = delete;
  auto operator=(FieldJsonParser&&) noexcept -> FieldJsonParser& = delete;

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
   * \brief Callback if an unexpected field happened during JSON parsing.
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
  /*! JSON key for the field setter. */
  static constexpr StringView kFieldSetterKey{"setter"_sv};
  /*! JSON key for the field getter. */
  static constexpr StringView kFieldGetterKey{"getter"_sv};
  /*! JSON key for the field notifier. */
  static constexpr StringView kFieldNotifierKey{"notifier"_sv};
  /*! JSON key for the short name. */
  static constexpr StringView kShortNameKey{"shortname"_sv};

  /*!
   * \brief Parse the field setter from JSON.
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
  auto ParseFieldSetter() noexcept -> ParserResult;

  /*!
   * \brief Parse the field getter from JSON.
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
  auto ParseFieldGetter() noexcept -> ParserResult;

  /*!
   * \brief Parse the field notifier from JSON.
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
  auto ParseFieldNotifier() noexcept -> ParserResult;

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

  std::size_t number_of_setters_found_{0};     /*!< Number of key 'setter' found.*/
  std::size_t number_of_notifiers_found_{0};   /*!< Number of key 'notifiers' found.*/
  std::size_t number_of_getters_found_{0};     /*!< Number of key 'getter' found.*/
  std::size_t number_of_short_names_found_{0}; /*!< Number of key 'shortname' found.*/

  /*! Reference to configuration struct filled by the parser. */
  FieldConfig& config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_FIELD_JSON_PARSER_H_

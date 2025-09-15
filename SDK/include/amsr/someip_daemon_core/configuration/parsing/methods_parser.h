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
/*!        \file  methods_parser.h
 *        \brief  Module for parsing service methods from JSON.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_METHODS_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_METHODS_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/method_validator.h"
#include "amsr/someip_daemon_core/configuration/parsing/method_someip_tp_parser.h"
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
 * \brief Parses service method objects from JSON into a container of methods.
 */
class MethodsParser final : public amsr::json::Parser<MethodsParser> {
 public:
  /*!
   * \brief Type alias for the parser base used multiple times.
   */
  using MethodsParserBase = amsr::json::Parser<MethodsParser>;

  /*!
   * \brief Type alias for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Type alias for easy access to ParserState.
   */
  using ParserState = amsr::json::ParserState;

  /*!
   * \brief Proto JSON key.
   */
  static constexpr CStringView kProtoKey{"proto"_sv};

  /*!
   * \brief Name JSON key.
   */
  static constexpr CStringView kNameKey{"name"_sv};

  /*!
   * \brief Id JSON key.
   */
  static constexpr CStringView kIdKey{"id"_sv};

  /*!
   * \brief someip_tp JSON key.
   */
  static constexpr CStringView kSomeIpTpKey{"someip_tp"_sv};

  /*!
   * \brief Input for this parser is the container of methods from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] methods A reference to the container, which is extended for every valid method parsed.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  MethodsParser(amsr::json::JsonData& file, ConfigurationTypesAndDefs::MethodContainer& methods) noexcept
      : MethodsParserBase{file},
        methods_{methods},
        method_storage_{},
        method_element_wrapper_{method_storage_},
        logger_{someip_daemon_core::logging::kConfigurationLoggerContextId,
                someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                ara::core::StringView{"MethodsParser"}} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~MethodsParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  MethodsParser(MethodsParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  MethodsParser(MethodsParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(MethodsParser const&) -> MethodsParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(MethodsParser&&) -> MethodsParser& = delete;

  /*!
   * \brief On every new method, element variables have to be reset as the
   * same object is used again for the next method.
   * \steady FALSE
   */
  void PreProcess() noexcept;

  /*!
   * \brief The post processing sequence will be invoked after a complete
   * method entry from JSON is parsed.
   * \details Only place the parsed method into the method container, if all
   * mandatory elements are set.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  model::MethodValidator::ValidationResult PostProcess() const noexcept;

  /*!
   * \brief On string. The protocol for this method is a string: Either "tcp" or "udp".
   * \param[in] val Parsed string value
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept;

  /*!
   * \brief Number callback for one method object.
   * \param[in] num Parsed number. For methods this can be the method ID.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept;

  /*!
   * \brief Used for spawning sub-parsers within methods.
   * \param[in] key The current key in JSON.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) noexcept;

  /*!
   * \brief Start object means that a new method object begins.
   * \details Clears temporary method structure fpr the new method object.
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartObject() noexcept;

  /*!
   * \brief End Array gets called when all methods of the array have been parsed.
   * \return ParserResult with ParserState::kFinished.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndArray(std::size_t) const noexcept;

  /*!
   * \brief End Object is called on each service end.
   * \return ParserResult with ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept;

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
   * \brief Reference to the container of methods that is extended after the validation step.
   */
  ConfigurationTypesAndDefs::MethodContainer& methods_;

  /*!
   * \brief This POD structure will be added to the container of methods, if the parsed element is valid.
   */
  ConfigurationTypesAndDefs::Method method_storage_;

  /*!
   * \brief Current intermediate method object to fill with information.
   */
  model::MethodConfigObject method_element_wrapper_;

  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  someip_daemon_core::logging::AraComLogger logger_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_METHODS_PARSER_H_

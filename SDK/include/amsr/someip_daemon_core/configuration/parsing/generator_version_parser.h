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
/*!        \file  generator_version_parser.h
 *        \brief  Header file for generator_version parser
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_GENERATOR_VERSION_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_GENERATOR_VERSION_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/json/reader.h"
#include "amsr/someip_daemon_core/configuration/model/generator_version_validator.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace parsing {

/*!
 * \brief Parses Generator Version from JSON.
 */
class GeneratorVersionParser : public amsr::json::Parser<GeneratorVersionParser> {
 public:
  /*!
   * \brief Type alias for parser base class.
   */
  using Parser = amsr::json::Parser<GeneratorVersionParser>;

  /*!
   * \brief Type-alias for used validation class.
   */
  using Validation = configuration::model::GeneratorVersionValidator;

  /*!
   * \brief Type-alias for validation result.
   */
  using ValidationResult = typename Validation::ValidationResult;

  /*!
   * \brief Type alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Type alias for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Constructor of generator version parser.
   * \param[in] file JSON document to parse.
   * \param[out] generator_version A reference to the 'generator version' POD structure to be filled with the parsed
   * values from JSON.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  GeneratorVersionParser(amsr::json::JsonData& file,
                         ConfigurationTypesAndDefs::GeneratorVersion& generator_version) noexcept;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~GeneratorVersionParser() override = default;

  /*!
   * \brief Deleted copy constructor.
   */
  GeneratorVersionParser(GeneratorVersionParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  GeneratorVersionParser(GeneratorVersionParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(GeneratorVersionParser const&) -> GeneratorVersionParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(GeneratorVersionParser&&) -> GeneratorVersionParser& = delete;

  /*!
   * \brief Check if keys are valid.
   * \param[in] key The current key in JSON.
   * \return ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) const noexcept;

  /*!
   * \brief Number callback.
   * \param[in] num Parsed numeric value.
   * \return An error code (SomeIpDaemonErrc::json_parsing_failed) is returned because no number values are
   * expected for this parser.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) const noexcept;

  /*!
   * \brief String callback.
   * \param[in] val Parsed string value
   * \return ParserState::kRunning if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept;

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) const noexcept;

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnUnexpectedEvent() noexcept;

 protected:
  /*!
   * \brief Validate the parsed object.
   * \details Validation implemented as protected API to support validation-independent test implementation.
   * \return Validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual ValidationResult Validate() const noexcept;

 private:
  /*!
   * \brief The logger is used to print out warnings and errors during parsing.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Current intermediate generator version object to fill with information.
   */
  model::GeneratorVersionConfigObject generator_version_config_wrapper_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_GENERATOR_VERSION_PARSER_H_

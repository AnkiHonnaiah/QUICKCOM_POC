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
/*!        \file  socal_pport_parser.h
 *        \brief  JSON parser for Socal Pport configuration.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_PPORT_PARSER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_PPORT_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {
namespace parsing {

/*!
 * \brief   Parser for the Socal p-port JSON configuration.
 * \details Performed steps of the parser:
 *          - Parse the JSON file contents into a provided configuration object.
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class SocalPportParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief Type-alias for the configuration object of a p-port.
   */
  using PportConfig = AssignmentName;

  /*!
   * \brief   Construct the JSON parser.
   * \details Start parsing using Parse() API.
   * \param[in]  json    JSON document to parse.
   * \param[out] config  A reference to the configuration object to be filled by the parser.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  SocalPportParser(::amsr::json::JsonData& json, PportConfig& config) noexcept;

  SocalPportParser(SocalPportParser const&) noexcept = delete;
  SocalPportParser& operator=(SocalPportParser const&) & = delete;
  SocalPportParser(SocalPportParser&&) noexcept = delete;
  SocalPportParser& operator=(SocalPportParser&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SOCAL_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~SocalPportParser() noexcept override = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Callback on a new key parsed from the JSON object.
   * \param[in] json_key  Parsed key string.
   * \return Parser result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final;

  /*!
   * \brief Callback to validate that all configuration elements have been accepted.
   * \return either nothing or the error.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<void> Finalize() noexcept final;

 private:
  /*!
   * \brief P-port JSON key.
   */
  static constexpr vac::container::CStringView kPportKey{"pport"_sv};

  /*!
   * \brief Parse the p-port.
   * \return Parser result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ParserResult ParsePport() noexcept;

  /*!
   * \brief Map error code to Socal specific JSON parsing failure error code.
   * \param[in] error_code  ErrorCode to be mapped.
   * \return Mapped ErrorCode
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static ::amsr::core::ErrorCode MapToSocalError(::amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief P-port type found.
   */
  std::size_t pport_found_{0};

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  PportConfig& config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_PPORT_PARSER_H_

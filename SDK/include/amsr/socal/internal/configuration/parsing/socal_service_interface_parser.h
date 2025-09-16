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
/*!        \file  socal_service_interface_parser.h
 *        \brief  Parser for the Socal service interface JSON element.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_SERVICE_INTERFACE_PARSER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_SERVICE_INTERFACE_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/json/reader.h"
#include "amsr/json/reader/json_parser.h"
#include "amsr/socal/internal/configuration/configuration.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {
namespace parsing {
/*!
 * \brief Parser for the Socal service interface JSON element.
 *
 * \unit Socal::configuration::ConfigurationProvider
 */
class SocalServiceInterfaceParser : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type-name alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief Constructs the service interface JSON parser.
   * \param[in]     json    JSON document to parse.
   * \param[in,out] config  The service interfaces config object to be filled by the parser.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  SocalServiceInterfaceParser(::amsr::json::JsonData& json, ServiceInterfacesConfig& config) noexcept;

  SocalServiceInterfaceParser(SocalServiceInterfaceParser const&) noexcept = delete;
  SocalServiceInterfaceParser operator=(SocalServiceInterfaceParser const&) & = delete;
  SocalServiceInterfaceParser(SocalServiceInterfaceParser&&) noexcept = delete;
  SocalServiceInterfaceParser& operator=(SocalServiceInterfaceParser&&) & = delete;

  /*!
   * \brief Destroys the service interface JSON parser.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~SocalServiceInterfaceParser() noexcept override = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

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
   * \return Result containing no value or an error.
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
   * \brief Parse shortname path.
   * \return Parse result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ParserResult ParseShortnamePath() noexcept;

  /*!
   * \brief Parse required service instances.
   * \return Parse result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ParserResult ParseRequiredInstanceSpecifierContainer() noexcept;

  /*!
   * \brief Parse provided service instances.
   * \return Parse result.
   * \error ComErrc::kJsonParsingFailed  If JSON parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ParserResult ParseProvidedInstanceSpecifierContainer() noexcept;

  /*!
   * \brief Check whether the instance specifier is duplicated.
   * \param[in] parsed_instance_specifier  The instance specifier to search for.
   * \return True if the instance specifier is duplicated, false otherwise.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  bool IsInstanceSpecifierDuplicated(std::string const& parsed_instance_specifier) const noexcept;

  /*!
   * \brief Map error code to Socal specific JSON parsing failure error code.
   * \param[in] error_code  ErrorCode to be mapped.
   * \return Mapped ErrorCode
   * \steady FALSE
   */
  static ::amsr::core::ErrorCode MapToSocalError(::amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief Shortname path Json key.
   */
  static constexpr ::amsr::core::StringView kShortnamePathKey{"shortname_path"_sv};

  /*!
   * \brief Provided instance specifiers Json key.
   */
  static constexpr ::amsr::core::StringView kProvidedInstanceSpecifiersKey{"provided_instance_specifiers"_sv};

  /*!
   * \brief Required instance specifiers Json key.
   */
  static constexpr ::amsr::core::StringView kRequiredInstanceSpecifiersKey{"required_instance_specifiers"_sv};

  /*!
   * \brief Parsed shortname path string.
   */
  std::string shortname_path_{};

  /*!
   * \brief Parsed provided instance specifiers.
   */
  InstanceSpecifierContainer provided_instance_specifiers_{};

  /*!
   * \brief Parsed required instance specifiers.
   */
  InstanceSpecifierContainer required_instance_specifiers_{};

  /*!
   * \brief Reference to configuration struct filled by the parser.
   */
  ServiceInterfacesConfig& service_interface_config_;
};
}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr
#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_PARSING_SOCAL_SERVICE_INTERFACE_PARSER_H_

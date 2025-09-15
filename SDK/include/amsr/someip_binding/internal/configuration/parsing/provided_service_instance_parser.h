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
/*!        \file  provided_service_instance_parser.h
 *        \brief  JSON parser for a 'ProvidedServiceInstance'.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::ProvidedServiceInstanceParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/v2/single_object_parser.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/parsing/e2e_props_config_json_parser.h"
#include "amsr/someip_binding/internal/configuration/provided_service_instance_config.h"
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
 * \brief Parser for a single provided service instance configuration.
 */
// VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
class ProvidedServiceInstanceParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  /*!
   * \brief Type name alias for amsr::json::v2::SingleObjectParser.
   */
  using SingleObjectParser = ::amsr::json::v2::SingleObjectParser;

  /*!
   * \brief Type name alias for amsr::someip_binding::internal::configuration::ServiceIdentifier.
   */
  using ServiceIdentifierConfig = ::amsr::someip_binding::internal::configuration::ServiceIdentifier;

  /*!
   * \brief Type name alias for amsr::someip_binding::internal::configuration::InstanceIdentifier.
   */
  using InstanceIdentifierConfig = ::amsr::someip_binding::internal::configuration::InstanceIdentifier;

  /*!
   * \brief Type name alias for amsr::someip_binding::internal::configuration::InstanceSpecifier.
   */
  using InstanceSpecifierConfig = ::amsr::someip_binding::internal::configuration::InstanceSpecifier;

  /*!
   * \brief Type name alias for amsr::someip_binding::internal::configuration::MajorVersion.
   */
  using MajorVersionConfig = ::amsr::someip_binding::internal::configuration::MajorVersion;

  /*!
   * \brief Name alias for amsr::someip_binding::internal::configuration::E2EPropsConfig.
   */
  using E2EPropsConfig = ::amsr::someip_binding::internal::configuration::E2EPropsConfig;

  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

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
  auto Parse() noexcept -> amsr::core::Result<void> final { return ::amsr::json::v2::SingleObjectParser::Parse(); }

  /*!
   * \brief Constructor for ProvidedServiceInstanceParser.
   * \param[in] json   The loaded JSON data from the configuration file.
   * \param[in] config Reference to provided service instance config.
   * \pre -
   * \context Init.
   */
  ProvidedServiceInstanceParser(amsr::json::JsonData& json, ProvidedServiceInstanceConfig& config) noexcept;

 protected:
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
  auto OnKey(amsr::core::StringView json_key) noexcept -> ParserResult final;

  /*!
   * \brief Callback to validate that all configuration elements have been accepted.
   * \return either nothing or the error.
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
  auto Finalize() noexcept -> amsr::core::Result<void> final;

 private:
  /*!
   * \brief JSON key for service identifier.
   */
  static constexpr amsr::core::StringView kServiceIdentifierKey{"service_id"_sv};

  /*!
   * \brief JSON key for instance identifier.
   */
  static constexpr amsr::core::StringView kInstanceIdentifierKey{"instance_id"_sv};

  /*!
   * \brief JSON key for instance specifier.
   */
  static constexpr amsr::core::StringView kInstanceSpecifierKey{"instance_specifier"_sv};

  /*!
   * \brief JSON key for major version.
   */
  static constexpr amsr::core::StringView kMajorVersionKey{"major_version"_sv};

  /*!
   * \brief JSON key for e2e props config.
   */
  static constexpr amsr::core::StringView kE2EPropsKey{"e2e_props"_sv};

  /*!
   * \brief Callback to parse the value for 'service_id' key.
   * \return a result to indicate whether parsing should continue.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnServiceIdentifier() noexcept -> ParserResult;

  /*!
   * \brief Callback to parse the value for 'instance_id' key.
   * \return a result to indicate whether parsing should continue.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnInstanceIdentifier() noexcept -> ParserResult;

  /*!
   * \brief Callback to parse the value for 'instance_specifier' key.
   * \return a result to indicate whether parsing should continue.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnInstanceSpecifier() noexcept -> ParserResult;

  /*!
   * \brief Callback to parse the value for 'major_version' key.
   * \return a result to indicate whether parsing should continue.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnMajorVersion() noexcept -> ParserResult;

  /*!
   * \brief Callback to parse the array of 'e2e_props' key.
   * \return a result to indicate whether parsing should continue.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace SPEC-4980040
   * \spec
   *   requires true;
   * \endspec
   */
  auto OnE2EProps() noexcept -> ParserResult;

  /*!
   * \brief         Callback to parse the value for one 'e2e_props_config' object.
   * \param[in,out] e2e_props_config Reference to the e2e props object.
   * \return        a result to indicate whether parsing should continue.
   * \pre           -
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto ParseE2EPropsObject(E2EPropsConfig& e2e_props_config) noexcept -> amsr::core::Result<void>;

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
  static auto MapToBindingError(amsr::core::ErrorCode const& error_code) noexcept -> amsr::core::ErrorCode;

  std::size_t number_of_service_ids_found_{0};         /*!< Number of key 'service_id' found.*/
  std::size_t number_of_instance_ids_found_{0};        /*!< Number of key 'instance_identifier' found.*/
  std::size_t number_of_major_versions_found_{0};      /*!< Number of key 'major_version' found.*/
  std::size_t number_of_instance_specifiers_found_{0}; /*!< Number of key 'instance_specifier' found.*/

  /*! Reference to provided service instance config. */
  ProvidedServiceInstanceConfig& config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCE_PARSER_H_

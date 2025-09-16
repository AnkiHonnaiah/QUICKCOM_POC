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
/*!        \file  services_parser.h
 *        \brief  JSON parser for services configuration element.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::ServicesParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_SERVICES_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_SERVICES_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/json/reader/v2/single_array_parser.h"
#include "amsr/json/reader/v2/single_object_parser.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
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
 * \brief Parser for the services element inside the SomeIpBinding configuration.
 */
// VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
class ServicesParser final : public ::amsr::json::v2::SingleArrayParser {
 public:
  /*!
   * \brief Name alias for amsr::json::v2::SingleArrayParser.
   */
  using SingleArrayParser = ::amsr::json::v2::SingleArrayParser;

  /*!
   * \brief Name alias for amsr::someip_binding::internal::configuration::SomeIpBindingConfig.
   */
  using SomeIpBindingConfig = ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig;

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
  amsr::core::Result<void> Parse() noexcept final { return ::amsr::json::v2::SingleArrayParser::Parse(); }

  /*!
   * \brief Constructor for ServicesParser.
   * \param[in,out] json the loaded JSON data from the configuration file.
   * \param[in,out] config the object holding the parsed connfiguration.
   * \pre -
   * \context Init
   */
  ServicesParser(amsr::json::JsonData& json, SomeIpBindingConfig& config) noexcept;

 protected:
  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = ::amsr::json::ParserResult;

  /*!
   * \brief       Callback on a JSON array element encounter from the parsed JSON object.
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
  ParserResult OnElement() noexcept final;

 private:
  /*!
   * \brief Parser for a single service interface configuration.
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  class ServiceParser final : public ::amsr::json::v2::SingleObjectParser {
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
     * \brief Type name alias for amsr::someip_binding::internal::configuration::ShortnamePath.
     */
    using ShortnamePathConfig = ::amsr::someip_binding::internal::configuration::ShortnamePath;

    /*!
     * \brief Type name alias for amsr::someip_binding::internal::configuration::MajorVersion.
     */
    using MajorVersionConfig = ::amsr::someip_binding::internal::configuration::MajorVersion;

    /*!
     * \brief Type name alias for amsr::someip_binding::internal::configuration::MinorVersion.
     */
    using MinorVersionConfig = ::amsr::someip_binding::internal::configuration::MinorVersion;

    /*!
     * \brief Type name alias for amsr::someip_binding::internal::configuration::IsGeneric.
     */
    using IsGenericConfig = ::amsr::someip_binding::internal::configuration::IsGeneric;

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
     * \brief Constructor for ServiceParser.
     * \param[in,out] json   The loaded JSON data from the configuration file.
     * \param[in,out] config Reference to the service interface config.
     * \pre -
     * \context Init.
     */
    ServiceParser(amsr::json::JsonData& json, configuration::ServiceInterfaceConfig& config) noexcept;

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
    ParserResult OnKey(amsr::core::StringView json_key) noexcept final;

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
    amsr::core::Result<void> Finalize() noexcept final;

   private:
    /*! JSON key for service identifier. */
    static constexpr amsr::core::StringView kServiceIdentifierKey{"service_id"_sv};

    /*! JSON key for shortname path. */
    static constexpr amsr::core::StringView kShortnamePathKey{"shortname_path"_sv};

    /*! JSON key for major version. */
    static constexpr amsr::core::StringView kMajorVersionKey{"major_version"_sv};

    /*! JSON key for minor version. */
    static constexpr amsr::core::StringView kMinorVersionKey{"minor_version"_sv};

    /*! JSON key for is_generic. */
    static constexpr amsr::core::StringView kIsGenericKey{"is_generic"_sv};

    /*! JSON key for events. */
    static constexpr amsr::core::StringView kEventsKey{"events"_sv};

    /*! JSON key for fields. */
    static constexpr amsr::core::StringView kFieldsKey{"fields"_sv};

    /*! JSON key for methods. */
    static constexpr amsr::core::StringView kMethodsKey{"methods"_sv};

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
    ParserResult OnServiceIdentifier() noexcept;

    /*!
     * \brief Callback to parse the value for 'shortname_path' key.
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
    ParserResult OnShortnamePath() noexcept;

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
    ParserResult OnMajorVersion() noexcept;

    /*!
     * \brief Callback to parse the value for 'minor_version' key.
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
    ParserResult OnMinorVersion() noexcept;

    /*!
     * \brief       Callback to parse the value for 'is_generic' key.
     * \return      A result to indicate whether parsing should continue.
     *
     * \pre         -
     *
     * \context     Init
     *
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     *
     * \spec
     *   requires true;
     * \endspec
     */
    ParserResult OnIsGeneric() noexcept;

    /*!
     * \brief Callback to parse the array of 'events' key.
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
    ParserResult ParseEvents() noexcept;

    /*!
     * \brief Callback to parse the array of 'fields' key.
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
    ParserResult ParseFields() noexcept;

    /*!
     * \brief Callback to parse the value for one 'event' object.
     * \param[out] event Reference to the event object.
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
    amsr::core::Result<void> ParseEventObject(configuration::EventConfig& event) noexcept;

    /*!
     * \brief Callback to parse the value for one 'field' object.
     * \param[out] field Reference to the field config object.
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
    amsr::core::Result<void> ParseFieldObject(configuration::FieldConfig& field) noexcept;

    /*!
     * \brief Callback to parse the array of 'methods' key.
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
    ParserResult ParseMethods() noexcept;

    /*!
     * \brief Callback to parse the value for one 'method' object.
     * \param[out] method Reference to the method configuration object.
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
    amsr::core::Result<void> ParseMethodsObject(configuration::MethodConfig& method) noexcept;

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

    std::size_t number_of_service_ids_found_{0};     /*!< Number of key 'service_id' found.*/
    std::size_t number_of_shortname_paths_found_{0}; /*!< Number of key 'shortname_path' found.*/
    std::size_t number_of_major_versions_found_{0};  /*!< Number of key 'major_version' found.*/
    std::size_t number_of_minor_versions_found_{0};  /*!< Number of key 'minor_version' found.*/
    std::size_t number_of_is_generic_found_{0};      /*!< Number of key 'is_generic' found.*/

    /*! Reference to the service interface config. */
    configuration::ServiceInterfaceConfig& config_;
  };

  /*!
   * \brief Reference for the SomeIpBindingConfig configuration object.
   */
  SomeIpBindingConfig& someip_binding_config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_SERVICES_PARSER_H_

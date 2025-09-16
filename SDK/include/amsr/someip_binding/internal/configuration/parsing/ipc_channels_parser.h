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
/*!        \file  ipc_channels_parser.h
 *        \brief  JSON parser for ipc channels element.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::IpcChannelsParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_IPC_CHANNELS_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_IPC_CHANNELS_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/json/reader/v2/single_array_parser.h"
#include "amsr/json/reader/v2/single_object_parser.h"
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
 * \brief Parser for the IPC channels element inside the SomeIpBinding configuration.
 */
// VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
class IpcChannelsParser final : public ::amsr::json::v2::SingleArrayParser {
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
  auto Parse() noexcept -> amsr::core::Result<void> final { return ::amsr::json::v2::SingleArrayParser::Parse(); }

  /*!
   * \brief Constructor for IpcChannelsParser.
   * \param[in,out] json the loaded JSON data from the configuration file.
   * \param[in,out] config the object holding the parsed configuration.
   * \pre -
   * \context Init
   */
  IpcChannelsParser(amsr::json::JsonData& json, SomeIpBindingConfig& config) noexcept;

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
  auto OnElement() noexcept -> ParserResult final;

 private:
  /*!
   * \brief Parser for a single ipc channel configuration.
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  class IpcChannelParser final : public ::amsr::json::v2::SingleObjectParser {
   public:
    /*!
     * \brief Type name alias for amsr::json::v2::SingleObjectParser.
     */
    using SingleObjectParser = ::amsr::json::v2::SingleObjectParser;

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
     * \brief Constructor for IpcChannelParser.
     * \param[in,out] json   The loaded JSON data from the configuration file.
     * \param[in,out] config Reference to the ipc channel config.
     * \pre -
     * \context Init.
     */
    IpcChannelParser(amsr::json::JsonData& json, configuration::IpcChannelConfig& config) noexcept;

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
    /*! JSON key for IPC channel domain. */
    static constexpr amsr::core::StringView kDomainKey{"domain"_sv};

    /*! JSON key for IPC channel port. */
    static constexpr amsr::core::StringView kPortKey{"port"_sv};

    /*! JSON key for the required service instances.*/
    static constexpr amsr::core::StringView kRequiredServiceInstancesKey{"required_service_instances"_sv};

    /*!* \brief JSON key for the provided service instances.*/
    static constexpr amsr::core::StringView kProvidedServiceInstancesKey{"provided_service_instances"_sv};

    /*!
     * \brief Callback to parse the value for 'port' key.
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
    auto ParsePort() noexcept -> ParserResult;
    /*!
     * \brief Callback to parse the value for 'domain' key.
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
    auto ParseDomain() noexcept -> ParserResult;

    /*!
     * \brief       Parse the configuration for required service instances.
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
    auto ParseRequiredServiceInstances() noexcept -> ParserResult;

    /*!
     * \brief       Parse the configuration for provided service instances.
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
    auto ParseProvidedServiceInstances() noexcept -> ParserResult;
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

    std::size_t number_of_ports_found_{0};   /*!< Number of key 'port' found.*/
    std::size_t number_of_domains_found_{0}; /*!< Number of key 'domain' found.*/
    std::size_t number_of_required_service_instances_found_{
        0}; /*!< Number of key 'required_service_instances_found' found.*/
    std::size_t number_of_provided_service_instances_found_{
        0}; /*!< Number of key 'provided_service_instances_found' found.*/

    /*! Reference to the ipc channel config. */
    configuration::IpcChannelConfig& config_;
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

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_IPC_CHANNELS_PARSER_H_

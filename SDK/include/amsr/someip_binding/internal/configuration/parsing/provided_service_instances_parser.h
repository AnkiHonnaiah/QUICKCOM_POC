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
/*!        \file
 *        \brief  JSON parser for provided service instances configuration.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::ProvidedServiceInstancesParser
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCES_PARSER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCES_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/v2/single_array_parser.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/parsing/provided_service_instance_parser.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {
namespace parsing {

/*!
 * \brief Parser for the provided service instances element inside the SomeIpBinding configuration.
 */
// VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
class ProvidedServiceInstancesParser final : public ::amsr::json::v2::SingleArrayParser {
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
   * \brief Constructor for ProvidedServiceInstanceParser.
   * \param[in,out] json the loaded JSON data from the configuration file.
   * \param[in,out] config the object holding the parsed IPC channel configuration.
   * \pre -
   * \context Init
   */
  ProvidedServiceInstancesParser(amsr::json::JsonData& json, configuration::IpcChannelConfig& config) noexcept;

 protected:
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
  ProvidedServiceInstanceParser::ParserResult OnElement() noexcept final;

 private:
  /*!
   * \brief Reference for the IpcChannelConfig configuration object.
   */
  configuration::IpcChannelConfig& ipc_channel_config_;
};

}  // namespace parsing
}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PARSING_PROVIDED_SERVICE_INSTANCES_PARSER_H_

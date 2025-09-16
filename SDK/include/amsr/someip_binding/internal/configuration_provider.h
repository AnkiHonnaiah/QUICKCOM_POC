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
/*!        \file  someip_binding/internal/configuration_provider.h
 *        \brief  Provides the configuration for the someipbinding.
 *         \unit  SomeIpBinding::SomeIpBinding::Configuration::ConfigurationProvider
 *
 *   \complexity  This unit provides Getters and Validator functionality for the configuration at start-up.
 *                It can still be considered of low complexity based on the following reasons:
 *                  - It has no further interactions with the component during runtime.
 *                  - It can be completely tested.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PROVIDER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/filestream/input_file_stream.h"
#include "amsr/someip_binding/internal/configuration/generator_version_config.h"
#include "amsr/someip_binding/internal/configuration/parsing/someipbinding_config_json_parser.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding/internal/json_data_loader.h"
#include "amsr/someip_binding/internal/runtime_interface.h"
#include "amsr/someip_binding_core/internal/error_domain.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "ara/com/runtime.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief Configuration provider.
 * \tparam JsonDataLoaderType The type of the json data loader.
 * \tparam SomeIpBindingConfigJsonParserType The type of the json data parser.
 */
template <typename JsonDataLoaderType = amsr::someip_binding::internal::JsonDataLoader,
          typename SomeIpBindingConfigJsonParserType =
              amsr::someip_binding::internal::configuration::parsing::SomeIpBindingConfigJsonParser>
class ConfigurationProvider final {
 public:
  /*!
   * \brief Constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ConfigurationProvider() noexcept {}

  /*!
   * \brief Use default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~ConfigurationProvider() noexcept = default;

  ConfigurationProvider(ConfigurationProvider const&) = delete;
  ConfigurationProvider(ConfigurationProvider&&) = delete;
  ConfigurationProvider& operator=(ConfigurationProvider const&) & = delete;
  ConfigurationProvider& operator=(ConfigurationProvider&&) & = delete;

  /*!
   * \brief       Get the configuration for the SomeipBinding.
   * \param[in]   config_json_path Path to configuration JSON file.
   * \return      An amsr::core::Result with an error code or empty if initialization succeeds.
   * \error       amsr::someip_binding_core::internal::SomeIpBindingErrc::json_loading_failure
   *                if component specific JSON configuration could not be found or loaded.
   *              amsr::someip_binding_core::internal::SomeIpBindingErrc::json_parsing_failure
   *                if component specific JSON configuration parsing failed.
   *              amsr::someip_binding_core::internal::SomeIpBindingErrc::invalid_json_config
   *                if there is no matching service interface exists for an instance.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Load configuration from JSON file and return the result.
   * \endinternal
   */
  auto GetConfiguration(::amsr::core::StringView const config_json_path) const noexcept
      -> amsr::core::Result<std::unique_ptr<configuration::SomeIpBindingConfig>> {
    return ReadConfigurationFromComponentJson(config_json_path);
  }

 private:
  /*!
   * \brief       Read configuration from json file.
   * \param[in]   config_json_path Path to configuration JSON file.
   * \return      An amsr::core::Result with the configuration if succeeds or an error code.
   * \error       amsr::someip_binding_core::internal::SomeIpBindingErrc::json_loading_failure
   *                if component specific JSON configuration could not be found or loaded.
   *              amsr::someip_binding_core::internal::SomeIpBindingErrc::json_parsing_failure
   *                if component specific JSON configuration parsing failed.
   *              amsr::someip_binding_core::internal::SomeIpBindingErrc::invalid_json_config
   *                if there is no matching service interface exists for an instance.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create a json data loader and load the data.
   * - If json data could be loaded successfully.
   *    - Create json data parser and pass the configuration object to be filled during parsing.
   *    - If json data could be parsed successfully.
   *      - Find and set the matching service interfaces for each service instance as a reference.
   *        - If no matching service interfaces found
   *          - set the failure as result.
   *        - Otherwise, store the configuration as result.
   *    - Otherwise set the parse failure as result.
   * - Otherwise set the data loader failure as result.
   * - Return the result.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.1_A15.5.2_A15.5.3_GoogleTest
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.1_A15.5.2_A15.5.3_GoogleTest
  auto ReadConfigurationFromComponentJson(::amsr::core::StringView const config_json_path) const noexcept
      -> amsr::core::Result<std::unique_ptr<configuration::SomeIpBindingConfig>> {
    ::amsr::core::Result<std::unique_ptr<configuration::SomeIpBindingConfig>> result{
        amsr::someip_binding_core::internal::SomeIpBindingErrc::json_parsing_failure};
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_SOMEIPBINDING_AutosarC++17_10-A7.2.1_outside_range_of_enumerator_FP
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    JsonDataLoaderType json_data_loader{};
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    amsr::core::Result<amsr::json::JsonData> json_data{json_data_loader.GetJsonData(config_json_path)};
    if (json_data.HasValue()) {
      // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
      amsr::someip_binding::internal::configuration::GeneratorVersionConfig generator_version_config{};
      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      std::unique_ptr<configuration::SomeIpBindingConfig> someip_binding_config{
          std::make_unique<configuration::SomeIpBindingConfig>()};
      // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      SomeIpBindingConfigJsonParserType parser{json_data.Value(), *someip_binding_config, generator_version_config};
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      amsr::core::Result<void> const parse_result{parser.Parse()};
      if (parse_result.HasValue()) {
        amsr::core::Result<void> const set_references_result{
            ValidateAndSetReferenceToServiceInterface(*someip_binding_config)};  // VCA_SOMEIPBINDING_PASSING_REFERENCE
        if (set_references_result.HasValue()) {
          // generator version is not used, but for logging
          logger_.LogVerbose(
              [&generator_version_config](ara::log::LogStream& s) {
                // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
                amsr::core::StringView const generator_name{generator_version_config.GetGeneratorName()};
                // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
                amsr::core::StringView const commit_id{generator_version_config.GetCommitId()};
                s << "Generator Version [name=" << generator_name << ", commit_id=" << commit_id << "]";
              },
              static_cast<char const*>(__func__), __LINE__);
          result.EmplaceValue(std::move(someip_binding_config));
        } else {
          result.EmplaceError(set_references_result.Error());
        }
      } else {
        result.EmplaceError(parse_result.Error());
      }
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    } else {
      result.EmplaceError(json_data.Error());
    }

    return result;
  }

  /*!
   * \brief          Validate the SomeipBinding configuration to ensure that a service interface is present for each
   *                 required and provided instance and set the corresponding reference to the service instances.
   * \param[in, out] configuration  SomeipBinding configuration.
   * \return         An amsr::core::Result with an error code or empty if configuration is valid
   * \error          amsr::someip_binding_core::internal::SomeIpBindingErrc::invalid_json_config if there is no
   *                 service interface exists for an instance.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Iterate through the IPC channels:
   *   - If a service interface doesn't exists for each required and provided instance,
   *     - set the configuration invalid flag to true.
   * - If its a valid configuration,
   *   - Store the  result with the value.
   * - Return the result.
   * \endinternal
   */
  static auto ValidateAndSetReferenceToServiceInterface(
      amsr::someip_binding::internal::configuration::SomeIpBindingConfig const& configuration) noexcept
      -> amsr::core::Result<void> {
    ::amsr::core::Result<void> result{amsr::someip_binding_core::internal::SomeIpBindingErrc::invalid_json_config};
    bool invalid_config{false};
    for (std::reference_wrapper<amsr::someip_binding::internal::configuration::IpcChannelConfig const> const&
             ipc_channel_config : configuration.GetIpcChannels()) {
      amsr::someip_binding::internal::configuration::SomeIpBindingConfig::ServiceInterfaceConfigRefContainer const&
          service_interfaces{configuration.GetServiceInterfaces()};

      // VCA_SOMEIPBINDING_PASSING_REFERENCE
      if (!SetServiceInterfaceReferences(ipc_channel_config, service_interfaces)) {
        invalid_config = true;
      }
    }
    if (!invalid_config) {
      result.EmplaceValue();
    }
    return result;
  }

  /*!
   * \brief          Set the reference to service interface in each required and provided service instances in the
                     SomeIpBinding configuration.
   * \param[in, out] ipc_channel_config  IPC channel configuration.
   * \param[in]      service_interfaces  Vector of service interfaces configuration.
   * \return         true if service interface exists for all service instances, false otherwise.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If no required instances are configured,
   *   - set service interface found flag to true.
   * - Otherwise, iterate through required service instance configs:
   *   - Iterate through service interface configs:
   *     - If the service id and major version of service matches with the required instance,
   *       - set the reference to the corresponding service interface in the required service instance's
   *         configuration.
   *   - If no matching service interface found,
   *     - stop iterating through further required instances.
   * - If no provided instances are configured,
   *   - set service interface found flag to true.
   * - Otherwise, iterate through provided service instance configs:
   *   - Iterate through service interface configs:
   *     - If the service id and major version of service matches with the provided instance,
   *       - set the reference to the corresponding service interface in the provided service instance's
   *         configuration.
   *   - If no matching service interface is found,
   *       - stop iterating through further provided instances.
   * - If service interface is found for both required and provided instances,
   *   - set the result to true.
   * - Return the result.
   * \endinternal
   */
  // VECTOR NC Metric-HIS.PATH: MD_SOMEIPBINDING_Metric-HIS.PATH
  // VECTOR NC Metric-HIS.VG: MD_SOMEIPBINDING_Metric-HIS.VG_FinalConfigurationValidation
  static auto SetServiceInterfaceReferences(
      amsr::someip_binding::internal::configuration::IpcChannelConfig const& ipc_channel_config,
      amsr::someip_binding::internal::configuration::SomeIpBindingConfig::ServiceInterfaceConfigRefContainer const&
          service_interfaces) noexcept -> bool {
    bool result{false};
    bool required_instance_service_interface_found{false};
    bool provided_instance_service_interface_found{false};
    if ((ipc_channel_config.GetRequiredServiceInstanceConfigs().empty())) {
      required_instance_service_interface_found = true;
    } else {
      for (std::reference_wrapper<
               amsr::someip_binding::internal::configuration::RequiredServiceInstanceConfig const> const&
               required_instance_config_ref : ipc_channel_config.GetRequiredServiceInstanceConfigs()) {
        for (std::reference_wrapper<amsr::someip_binding::internal::configuration::ServiceInterfaceConfig const> const&
                 service_interface_ref : service_interfaces) {
          amsr::someip_binding::internal::configuration::RequiredServiceInstanceConfig const& required_instance_config{
              required_instance_config_ref.get()};
          amsr::someip_binding::internal::configuration::ServiceInterfaceConfig const& service_interface{
              service_interface_ref.get()};
          if ((required_instance_config.GetServiceIdentifier().service_identifier ==
               service_interface.GetServiceId().service_identifier) &&
              (required_instance_config.GetMajorVersion().major_version ==
               service_interface.GetMajorVersion().major_version)) {
            // VECTOR NC AutosarC++17_10-A5.2.3: MD_SOMEIPBINDING_AutosarC++17_10-A5.2.3_const_cast
            const_cast<configuration::RequiredServiceInstanceConfig&>(required_instance_config)
                .SetServiceInterfaceRef(service_interface);  // VCA_SOMEIPBINDING_PASSING_REFERENCE
            required_instance_service_interface_found = true;
            break;
          }
        }
        if (!required_instance_service_interface_found) {
          break;
        }
      }
    }
    if ((ipc_channel_config.GetProvidedServiceInstanceConfigs().empty())) {
      provided_instance_service_interface_found = true;
    } else {
      for (std::reference_wrapper<
               amsr::someip_binding::internal::configuration::ProvidedServiceInstanceConfig const> const&
               provided_instance_config_ref : ipc_channel_config.GetProvidedServiceInstanceConfigs()) {
        for (std::reference_wrapper<amsr::someip_binding::internal::configuration::ServiceInterfaceConfig const> const&
                 service_interface_ref : service_interfaces) {
          amsr::someip_binding::internal::configuration::ProvidedServiceInstanceConfig const& provided_instance_config{
              provided_instance_config_ref.get()};
          amsr::someip_binding::internal::configuration::ServiceInterfaceConfig const& service_interface{
              service_interface_ref.get()};
          if ((provided_instance_config.GetServiceIdentifier().service_identifier ==
               service_interface.GetServiceId().service_identifier) &&
              (provided_instance_config.GetMajorVersion().major_version ==
               service_interface.GetMajorVersion().major_version)) {
            // VECTOR NC AutosarC++17_10-A5.2.3: MD_SOMEIPBINDING_AutosarC++17_10-A5.2.3_const_cast
            const_cast<configuration::ProvidedServiceInstanceConfig&>(provided_instance_config)
                .SetServiceInterfaceRef(service_interface);  // VCA_SOMEIPBINDING_PASSING_REFERENCE
            provided_instance_service_interface_found = true;
            break;
          }
        }
        if (!provided_instance_service_interface_found) {
          break;
        }
      }
    }
    if ((required_instance_service_interface_found && provided_instance_service_interface_found)) {
      result = true;
    }
    return result;
  }

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger const logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, "ConfigurationProvider"_sv};
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_PROVIDER_H_

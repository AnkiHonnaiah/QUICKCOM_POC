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
/*!        \file  someip_config_validator.h
 *        \brief  Validator for configuration object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_CONFIG_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_CONFIG_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/configuration_object.h"
#include "ara/core/map.h"
#include "ara/core/string.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups checks for parsed SOME/IP configuration element.
 */
class SomeIpConfigValidator final {
 public:
  /*!
   * \brief Validation results for one SOME/IP configuration object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The SOME/IP configuration object is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief The services object is missing in the parsed SOME/IP configuration object.
     */
    kMissingServices,

    /*!
     * \brief The IPC channels object is missing in the parsed SOME/IP configuration object.
     */
    kMissingIpcChannels,

    /*!
     * \brief The network end points object is missing in the parsed SOME/IP configuration object.
     */
    kMissingNetworkEndPoints,

    /*!
     * \brief Different UdpCollectionBufferSizeThresholds for the same endpoint.
     */
    kDifferentUdpCollectionBufferSizeThresholds,

    /*!
     * \brief The provided or required service instance doesn't match with a configured service.
     */
    kNonMatchingProvidedOrRequiredServiceId,
  };

  /*!
   * \brief Check if this SOME/IP configuration object parsed previously is valid.
   * \details
   * - Verify that the Services object is present (mandatory).
   * - Verify that the IPC Channels object is present (mandatory).
   * - Verify that the Network end points object is present (mandatory).
   * - Verify that the UDP message accumulation threshold has the same for value each service instance across the model.
   * - Verify that the Provided and Required Service Instances has a Service Id that matches with a configured service.
   * - Additional checks can be placed within this static member function.
   * \param[in] app_gen_config_object The application_gen_config element to validate (whether
   * all mandatory elements are set or not set).
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if either of ServicesContainerElement, IpcChannelsContainerElement or NetworkEndPointsElement is not set
   *   - Set the validation result to corresponding configuration element is not configured
   * - return the validation result
   * \endinternal
   */
  static auto Check(configuration::model::ConfigurationObject const& app_gen_config_object) noexcept
      -> ValidationResult {
    ValidationResult validation_result{ValidationResult::kOk};

    // Mandatory fields for application gen json
    if (app_gen_config_object.GetServices().GetStatus() != ElementStatus::kSet) {
      validation_result = ValidationResult::kMissingServices;
    } else if ((app_gen_config_object.GetIpcChannels().GetStatus() != ElementStatus::kSet) ||
               app_gen_config_object.GetIpcChannels().GetElement().empty()) {
      validation_result = ValidationResult::kMissingIpcChannels;
    } else if (app_gen_config_object.GetNetworkEndPoints().GetStatus() != ElementStatus::kSet) {
      validation_result = ValidationResult::kMissingNetworkEndPoints;
    } else if (!CheckUdpCollectionBufferSizeThresholdsGlobally(app_gen_config_object)) {
      validation_result = ValidationResult::kDifferentUdpCollectionBufferSizeThresholds;
    } else if (!CheckProvidedAndRequiredServiceInstances(app_gen_config_object)) {
      validation_result = ValidationResult::kNonMatchingProvidedOrRequiredServiceId;
    } else {
      validation_result = ValidationResult::kOk;
    }

    return validation_result;
  }

 private:
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  /*!
   * \brief Check for machine mappings to the same endpoint containing different "UdpCollectionBufferSizeThresholds".
   * \param[in] cfg description
   * \return validation result
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   * \internal
   * - Iterate each Provided and Required Service Instance port mappings.
   * - Store every mapping in a dictionary. Key=endpoint, Value=threshold.
   * - Check if there was a stored entry, if there was and the threshold is different, fail validation.
   * \endinternal
   */
  static auto CheckUdpCollectionBufferSizeThresholdsGlobally(
      configuration::model::ConfigurationObject const& cfg) noexcept -> bool {
    std::uintptr_t n_errors{0ULL};

    ara::core::Map<ara::core::String, ConfigurationTypesAndDefs::UdpCollectionBufferSizeThreshold> thresholds;
    auto const validate_threshold =
        [&thresholds](someip_daemon_core::IpAddress const& address, someip_protocol::internal::Port port,
                      ConfigurationTypesAndDefs::UdpCollectionBufferSizeThreshold threshold) {
          ara::core::String key{address.ToString()};
          key += ":" + std::to_string(static_cast<std::int32_t>(port));

          decltype(thresholds)::iterator const it{thresholds.find(key)};
          bool ret{};
          if (it == thresholds.end()) {
            static_cast<void>(thresholds.emplace(std::make_pair(std::move(key), threshold)));
            ret = true;
          } else {
            ret = it->second == threshold;
          }
          return ret;
        };

    // at this point it is known that provided and required service instances
    // are present.
    using PSIType = ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstance;
    using IpcChannel = ConfigurationTypesAndDefs::IpcChannel;
    for (IpcChannel const& ipc_channel : cfg.GetIpcChannels().GetElement()) {
      for (PSIType const& psi : ipc_channel.GetProvidedServiceInstances()) {
        using MappingType = ConfigurationTypesAndDefs::MachineMapping;
        if (!psi.event_props_.empty()) {
          for (MappingType const& mapping : psi.port_mappings_) {
            if ((mapping.udp_port_.has_value()) && (mapping.address_.has_value())) {
              bool const success{validate_threshold(*mapping.address_, *mapping.udp_port_,
                                                    mapping.udp_collection_buffer_size_threshold_)};
              n_errors += static_cast<std::uintptr_t>(!success);
            }
          }
        }
      }
    }
    using RSIType = ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance;
    for (IpcChannel const& ipc_channel : cfg.GetIpcChannels().GetElement()) {
      for (RSIType const& rsi : ipc_channel.GetRequiredServiceInstances()) {
        // Only if required methods are configured and UDP is used for transmission we need to validate.
        if ((!rsi.method_props_.empty()) && (rsi.port_mapping_.udp_port_.has_value()) &&
            (rsi.port_mapping_.address_.has_value())) {
          bool const success{validate_threshold(*rsi.port_mapping_.address_, *rsi.port_mapping_.udp_port_,
                                                rsi.port_mapping_.udp_collection_buffer_size_threshold_)};
          n_errors += static_cast<std::uintptr_t>(!success);
        }
      }
    }
    return (n_errors == 0);
  }

  /*!
   * \brief Check that the Service Id for the Provided and Required Service Instances matches with a configured service.
   * \param[in] config The configuration object
   * \return validation result
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   * \internal
   * - Iterate each Provided and Required Service Instance.
   * - Iterate each configured service.
   * - Check that each Provided and Required Service Instance matches with a configured service else fail validation.
   * \endinternal
   */
  static auto CheckProvidedAndRequiredServiceInstances(configuration::model::ConfigurationObject const& config) noexcept
      -> bool {
    bool result{true};

    auto const find_in_service = [&config](someip_protocol::internal::ServiceId service_id) -> bool {
      bool is_found{false};
      for (ConfigurationTypesAndDefs::Service const& service : config.GetServices().GetElement()) {
        if (service.id_ == service_id) {
          is_found = true;
          break;
        }
      }
      return is_found;
    };

    using provided_service_instance_type = ConfigurationTypesAndDefs::IpcChannel::ProvidedServiceInstance;
    using IpcChannel = ConfigurationTypesAndDefs::IpcChannel;
    for (IpcChannel const& ipc_channel : config.GetIpcChannels().GetElement()) {
      for (provided_service_instance_type const& provided_service_instance :
           ipc_channel.GetProvidedServiceInstances()) {
        if (!find_in_service(provided_service_instance.service_id_)) {
          result = false;
          break;
        }
      }
    }

    if (result) {
      using required_service_instance_type = ConfigurationTypesAndDefs::IpcChannel::RequiredServiceInstance;
      for (IpcChannel const& ipc_channel : config.GetIpcChannels().GetElement()) {
        for (required_service_instance_type const& required_service_instance :
             ipc_channel.GetRequiredServiceInstances()) {
          if (!find_in_service(required_service_instance.service_id_)) {
            result = false;
            break;
          }
        }
      }
    }
    return result;
  }
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_CONFIG_VALIDATOR_H_

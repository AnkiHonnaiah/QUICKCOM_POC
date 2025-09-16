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
/*!        \file  required_service_instance_provider_validator.h
 *        \brief  Validates Config object 'RequiredServiceInstanceProvider'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_PROVIDER_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_PROVIDER_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/required_service_instance_provider_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for static service discovery 'required service instance provider' element.
 */
class RequiredServiceInstanceProviderValidator final {
 public:
  /*!
   * \brief Validation results.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The object is valid.
     */
    kOk = 0x00U,

    /*!
     * \brief No address element configured.
     */
    kMissingAddress,

    /*!
     * \brief No valid UDP port element configured.
     */
    kInvalidUdpPort,

    /*!
     * \brief No valid TCP port element configured.
     */
    kInvalidTcpPort,

    /*!
     * \brief Neither UDP not TCP port elements are configured.
     */
    kNoPortsConfigured,

    /*!
     * \brief No multicast address element configured, needed if multicast port is defined.
     */
    kMissingMulticastAddress,

    /*!
     * \brief No UDP port element configured, needed when multicast address is defined.
     */
    kMulticastNoUdpPortConfigured,

    /*!
     * \brief No valid multicast port element configured, needed when multicast address is defined.
     */
    kInvalidMulticastPort,

    /*!
     * \brief No multicast port element is configured, needed when multicast address is defined.
     */
    kNoMulticastPortConfigured,

    /*!
     * \brief Invalid multicast address element configured.
     */
    kInvalidMulticastAddress
  };

  /*!
   * \brief Translate the validation result to string for logging.
   * \param[in] validation_result Validation result to translate.
   * \return String representation of the passed-in validation result.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Translate the validation result to string and return it.
   * \endinternal
   */
  // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
  static auto ValidationResultToString(ValidationResult validation_result) -> char const* {
    char const* ret{nullptr};

    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;

      case ValidationResult::kMissingAddress:
        ret = "No address";
        break;

      case ValidationResult::kInvalidUdpPort:
        ret = "No valid UDP port";
        break;

      case ValidationResult::kInvalidTcpPort:
        ret = "No valid TCP port";
        break;

      case ValidationResult::kNoPortsConfigured:
        ret = "Neither UDP nor TCP ports are configured";
        break;

      case ValidationResult::kMissingMulticastAddress:
        ret = "No multicast address element configured";
        break;

      case ValidationResult::kMulticastNoUdpPortConfigured:
        ret = "No UDP port configured, required by multicast";
        break;

      case ValidationResult::kInvalidMulticastPort:
        ret = "No valid multicast port configured, required by multicast";
        break;

      case ValidationResult::kNoMulticastPortConfigured:
        ret = "No multicast port configured, required by multicast";
        break;

      case ValidationResult::kInvalidMulticastAddress:
        ret = "Invalid multicast address configured";
        break;
    }
    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \param[in] static_sd_instance_provider The 'static service discovery instance subscribers' element to
   * verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok.
   * - Check if AddressElement is not set, UdpPortElement is not set or invalid, TcpPortElement is not set or invalid,
   * - If EventMulticastAddress element is set:
   *   - Check if UdpPortElement is not set, MulticastPortElement is not set or invalid.
   * - If EventMulticastPort element is set:
   *   - Check if EventMulticastAddress is not set.
   * - If EventMulticastAddress element is set:
   *   - Check if EventMulticastAddress is an IP Address, set InvalidMulticastAddress if not.
   * - Set the validation result to the corresponding configuration element if not configured.
   * - Return the validation result.
   * \endinternal
   */
  static ValidationResult Check(
      RequiredServiceInstanceProviderConfigObject const& static_sd_instance_provider) noexcept {
    ValidationResult result{};

    if (static_sd_instance_provider.GetIpAddress().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingAddress;
    } else if ((static_sd_instance_provider.GetUdpPort().GetStatus() == ElementStatus::kNotSet) &&
               (static_sd_instance_provider.GetTcpPort().GetStatus() == ElementStatus::kNotSet)) {
      result = ValidationResult::kNoPortsConfigured;
    } else if (static_sd_instance_provider.GetUdpPort().GetElement() == ConfigurationTypesAndDefs::kInvalidPort) {
      result = ValidationResult::kInvalidUdpPort;
    } else if (static_sd_instance_provider.GetTcpPort().GetElement() == ConfigurationTypesAndDefs::kInvalidPort) {
      result = ValidationResult::kInvalidTcpPort;
    } else if ((static_sd_instance_provider.GetEventMulticastAddress().GetStatus() == ElementStatus::kSet) &&
               (static_sd_instance_provider.GetUdpPort().GetStatus() == ElementStatus::kNotSet)) {
      result = ValidationResult::kMulticastNoUdpPortConfigured;
    } else if ((static_sd_instance_provider.GetEventMulticastAddress().GetStatus() == ElementStatus::kSet) &&
               (static_sd_instance_provider.GetEventMulticastPort().GetStatus() == ElementStatus::kNotSet)) {
      result = ValidationResult::kNoMulticastPortConfigured;
    } else if ((static_sd_instance_provider.GetEventMulticastAddress().GetStatus() == ElementStatus::kSet) &&
               (static_sd_instance_provider.GetEventMulticastPort().GetElement() ==
                ConfigurationTypesAndDefs::kInvalidPort)) {
      result = ValidationResult::kInvalidMulticastPort;
    } else if ((static_sd_instance_provider.GetEventMulticastPort().GetStatus() == ElementStatus::kSet) &&
               (static_sd_instance_provider.GetEventMulticastAddress().GetStatus() != ElementStatus::kSet)) {
      result = ValidationResult::kMissingMulticastAddress;
    } else {
      result = ValidationResult::kOk;
    }
    return result;
  }
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_PROVIDER_VALIDATOR_H_

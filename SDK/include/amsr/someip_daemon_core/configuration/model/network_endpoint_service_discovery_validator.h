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
/*!        \file  network_endpoint_service_discovery_validator.h
 *        \brief  Validates config object for 'network_endpoint_service_discovery'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_SERVICE_DISCOVERY_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_SERVICE_DISCOVERY_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_service_discovery_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for network endpoint service discovery.
 */
class NetworkEndpointServiceDiscoveryValidator final {
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
     * \brief No address configured.
     */
    kMissingMulticastAddress,

    /*!
     * \brief No or invalid port number configured.
     */
    kMissingOrInvalidPort
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
  static char const* ValidationResultToString(ValidationResult validation_result) {
    char const* ret{nullptr};

    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;

      case ValidationResult::kMissingMulticastAddress:
        ret = "Multicast address is missing";
        break;

      case ValidationResult::kMissingOrInvalidPort:
        ret = "Port number is invalid or missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the multicast address is set (mandatory).
   * - Verify that the port is set (mandatory) and no invalid port number is used.
   * \param[in] network_endpoint_service_discovery The network endpoint service discovery element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if MulticastAddressElement is not set
   *   - Set the validation result to no address is configured
   * - Otherwise, check if PortElement is not set or it is invalid
   *   - Set the validation result to missing or invalid port
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(
      NetworkEndpointServiceDiscoveryConfigObject const& network_endpoint_service_discovery) noexcept {
    ValidationResult result{};

    if (network_endpoint_service_discovery.GetMulticastAddress().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingMulticastAddress;
    } else if ((network_endpoint_service_discovery.GetPort().GetStatus() != ElementStatus::kSet) ||
               (network_endpoint_service_discovery.GetPort().GetElement() == ConfigurationTypesAndDefs::kInvalidPort)) {
      result = ValidationResult::kMissingOrInvalidPort;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_SERVICE_DISCOVERY_VALIDATOR_H_

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
/*!        \file  network_endpoint_port_keep_alive_validator.h
 *        \brief  Validates config object for 'network_endpoint_port_keep_alive'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_port_keep_alive_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for network endpoint keep alive.
 */
class NetworkEndpointPortKeepAliveValidator final {
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
     * \brief The keep alive time is invalid.
     */
    kInvalidIdleTime,

    /*!
     * \brief The alive interval is invalid.
     */
    kInvalidAliveInterval,

    /*!
     * \brief The retry count is invalid.
     */
    kInvalidRetryCount
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

      case ValidationResult::kInvalidIdleTime:
        ret = "The idle time is invalid. Must be greater than zero";
        break;

      case ValidationResult::kInvalidAliveInterval:
        ret = "The alive interval is invalid. Must be greater than zero";
        break;

      case ValidationResult::kInvalidRetryCount:
        ret = "The retry count is invalid. Must be greater than zero";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the Idle time >= 1
   * - Verify that the alive time >= 1
   * - Verify that the retry count >= 1
   * \param[in] network_endpoint_port_keep_alive The network endpoint keep alive element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if IdleTimeSElement is set and if the IdleTimeSElement is less or equal to 0
   *   - Set the validation result to IdleTimeSElement is invalid
   * - Otherwise, check if AliveIntervalSElement is set and if the AliveIntervalSElement is less or equal to 0
   *   - Set the validation result to AliveIntervalSElement is invalid
   * - Otherwise, check if RetryCountElement is not set and if RetryCountElement is less or equal to 0
   *   - Set the validation result to RetryCountElement is invalid
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(
      NetworkEndpointPortKeepAliveConfigObject const& network_endpoint_port_keep_alive) noexcept {
    ValidationResult result{};

    if ((network_endpoint_port_keep_alive.GetIdleTimeS().GetStatus() == ElementStatus::kSet) &&
        (network_endpoint_port_keep_alive.GetIdleTimeS().GetElement().value().count() <= 0)) {
      result = ValidationResult::kInvalidIdleTime;
    } else if ((network_endpoint_port_keep_alive.GetAliveIntervalS().GetStatus() == ElementStatus::kSet) &&
               (network_endpoint_port_keep_alive.GetAliveIntervalS().GetElement().value().count() <= 0)) {
      result = ValidationResult::kInvalidAliveInterval;
    } else if ((network_endpoint_port_keep_alive.GetRetryCount().GetStatus() == ElementStatus::kSet) &&
               (network_endpoint_port_keep_alive.GetRetryCount().GetElement().value() <= 0)) {
      result = ValidationResult::kInvalidRetryCount;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_VALIDATOR_H_

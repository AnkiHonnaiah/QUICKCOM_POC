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
/*!        \file  network_endpoint_ports_validator.h
 *        \brief  Validates configuration object for 'network_endpoint_port'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORTS_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORTS_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_port_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for network endpoint ports element.
 */
class NetworkEndpointPortsValidator final {
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
     * \brief No protocol configured.
     */
    kMissingProtocol,

    /*!
     * \brief No port number configured.
     */
    kMissingPort,

    /*!
     * \brief KeepAlive parameters are only allowed if TCP protocol is used.
     */
    kInvalidKeepAliveParametersNonTcpProtocol,

    /*!
     * \brief Socket option linger parameter is only allowed if TCP protocol is used.
     */
    kEnableSocketOptionLingerNonTcpProtocol,

    /*!
     * \brief Socket option Tcp Nagle parameter is only allowed if TCP protocol is used.
     */
    kEnableSocketOptionTcpNagleNonTcpProtocol,

    /*!
     * \brief Socket option Quality of Service (Qos, vlanPriority) must be within range 0-7.
     */
    kSocketOptionQosOutOfRange,

    /*!
     * \brief If value for error counter reset is greater than 0, value for disconnect counter shall be greater than 0
     * as well.
     */
    kInvalidTcpThresholdCounterConfiguration
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

      case ValidationResult::kMissingProtocol:
        ret = "Protocol is missing";
        break;

      case ValidationResult::kMissingPort:
        ret = "Port number is missing";
        break;

      case ValidationResult::kInvalidKeepAliveParametersNonTcpProtocol:
        ret = "Keep alive parameters are only allowed for TCP protocol";
        break;

      case ValidationResult::kEnableSocketOptionLingerNonTcpProtocol:
        ret = "Socket option linger parameter is only allowed for TCP protocol";
        break;

      case ValidationResult::kEnableSocketOptionTcpNagleNonTcpProtocol:
        ret = "Socket option Tcp Nagle parameter is only allowed for TCP protocol";
        break;

      case ValidationResult::kSocketOptionQosOutOfRange:
        ret = "Socket option Qos (vlanPriority) needs to be within range 0-7";
        break;

      case ValidationResult::kInvalidTcpThresholdCounterConfiguration:
        ret =
            "If threshold no_of_consecutive_valid_tcp_messages_before_error_reset is set to a value different than "
            "zero, then threshold no_of_invalid_tcp_messages_before_disconnection shall be set to a value different "
            "than zero as well.";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   * \details
   * - Verify that the protocol is set. (mandatory)
   * - Verify that the port is set (mandatory) and no invalid port number is used.
   * - KeepAlive parameters are only allowed if TCP protocol is used.
   * - Socket Option linger is only allowed when TCP protocol is used.
   * - Socket Option Tcp Nagle is only allowed when TCP protocol is used.
   * - Socket Option Quality of Service (Qos, vlanPriority) is within range.
   * \param[in] network_endpoint_ports The network endpoint ports element to verify.
   * \return kOk if the element is valid, otherwise the related ValidationResult.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if ProtoElement is not set
   *   - Set the validation result to transport protocol is not configured
   * - Otherwise, check if PortElement is not set or it is invalid
   *   - Set the validation result to missing or invalid port
   * - Otherwise, check if KeepAliveElement is true and the ProtoElement is not TCP
   *   - Set the validation result to KeepAlive parameters are only allowed if TCP protocol is used
   * - Otherwise, check if TcpNagleElement is true and the ProtoElement is not TCP
   *   - Set the validation result to TcpNagle parameter is only allowed if TCP protocol is used
   * - Otherwise, check if QosElement is outside the correct range
   *   - Set the validation result to Qos out of range
   * - Otherwise, check if TCP disconnect counter is set to 0, when TCP error reset counter is not set to 0
   *   - Set the validation result to invalid tcp threshold counter configuration.
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(NetworkEndpointPortConfigObject const& network_endpoint_ports) noexcept {
    ValidationResult result{};

    if (network_endpoint_ports.GetProto().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingProtocol;
    } else if (network_endpoint_ports.GetPort().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPort;
    } else if (network_endpoint_ports.GetKeepAlive().GetElement().has_value() &&
               (network_endpoint_ports.GetProto().GetElement() != ConfigurationTypesAndDefs::Protocol::kTCP)) {
      result = ValidationResult::kInvalidKeepAliveParametersNonTcpProtocol;
    } else if (network_endpoint_ports.GetSocketOptionLinger().GetElement() &&
               (network_endpoint_ports.GetProto().GetElement() != ConfigurationTypesAndDefs::Protocol::kTCP)) {
      result = ValidationResult::kEnableSocketOptionLingerNonTcpProtocol;
    } else if (network_endpoint_ports.GetSocketOptionTcpNagle().GetElement() &&
               (network_endpoint_ports.GetProto().GetElement() != ConfigurationTypesAndDefs::Protocol::kTCP)) {
      result = ValidationResult::kEnableSocketOptionTcpNagleNonTcpProtocol;
    } else if (network_endpoint_ports.GetQos().GetElement().has_value() &&
               (network_endpoint_ports.GetQos().GetElement().value() > ConfigurationTypesAndDefs::kQosMax)) {
      result = ValidationResult::kSocketOptionQosOutOfRange;
    } else if ((network_endpoint_ports.GetNoOfConsecutiveValidTcpMessagesBeforeErrorReset().GetElement() != 0U) &&
               (network_endpoint_ports.GetNoOfInvalidTcpMessagesBeforeDisconnection().GetElement() == 0U)) {
      result = ValidationResult::kInvalidTcpThresholdCounterConfiguration;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORTS_VALIDATOR_H_

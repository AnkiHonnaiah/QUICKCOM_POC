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
/*!        \file  network_endpoint_validator.h
 *        \brief  Validates configuration object for 'network_endpoint'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "ara/core/vector.h"

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/model/network_endpoint_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Groups consistency checks for network endpoint.
 */
class NetworkEndpointValidator final {
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
    kMissingAddress,

    /*!
     * \brief No MTU configured.
     */
    kMissingMtu,

    /*!
     * \brief No service discovery configured.
     */
    kMissingServiceDiscovery,

    /*!
     * \brief Bulk read count is not valid.
     */
    kInvalidBulkReadCount,

    /*!
     * \brief Bulk read receive period in micro seconds is not valid.
     */
    kInvalidBulkReadReceivePeriodUs,

    /*!
     * \brief Quality of Service (Qos, vlanPriority) must be within range 0-7.
     */
    kQosOutOfRange,

    /*!
     * \brief MTU is too small for the configured address type (IPv4/IPv6).
     */
    kMtuTooSmall,

    /*!
     * \brief If value for error counter reset is greater than 0, value for disconnect counter shall be greater than
     * 0 as well.
     */
    kInvalidTcpThresholdCounterConfiguration,

    /*!
     * \brief Bulk read minimum number of datagrams to continue consecutive receive calls is not valid, when configured
     * it shall be smaller than or equal to bulk_read_count.
     */
    kInvalidBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsConfiguration,

    /*!
     * \brief Bulk read maximum number of consecutive receive calls is not valid, when configured it shall be greater
     * than 0.
     */
    kInvalidBulkReadMaxNoOfConsecutiveReceiveCallsConfiguration
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
  static ara::core::String ValidationResultToString(ValidationResult validation_result) {
    ara::core::String ret{};

    switch (validation_result) {
      case ValidationResult::kOk:
        ret = "The object is valid";
        break;

      case ValidationResult::kMissingAddress:
        ret = "Address is missing";
        break;

      case ValidationResult::kMissingMtu:
        ret = "MTU is missing";
        break;

      case ValidationResult::kMissingServiceDiscovery:
        ret = "Service discovery is missing";
        break;

      case ValidationResult::kInvalidBulkReadCount:
        ret = "Invalid bulk read count. Maximum bulk read count: " + std::to_string(kBulkReadCountMax);
        break;

      case ValidationResult::kInvalidBulkReadReceivePeriodUs:
        ret = "Invalid bulk read receive period micro seconds. Maximum bulk read receive period: " +
              std::to_string(kBulkReadReceivePeriodUsMax);
        break;

      case ValidationResult::kQosOutOfRange:
        ret = "Qos (vlanPriority) option needs to be within range 0-7";
        break;

      case ValidationResult::kMtuTooSmall:
        ret = "MTU is too small. No room for UDP + IP headers";
        break;

      case ValidationResult::kInvalidTcpThresholdCounterConfiguration:
        ret =
            "If threshold no_of_consecutive_valid_tcp_messages_before_error_reset is set to a value different than "
            "zero, then threshold no_of_invalid_tcp_messages_before_disconnection shall be set to a value different "
            "than zero as well.";
        break;
      case ValidationResult::kInvalidBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsConfiguration:
        ret =
            "Invalid bulk read minimum number of datagrams to continue consecutive receive calls. Must be smaller or "
            "equal to bulk read count.";
        break;
      case ValidationResult::kInvalidBulkReadMaxNoOfConsecutiveReceiveCallsConfiguration:
        ret = "Invalid bulk read maximum number of consecutive receive calls. Must be greater than zero.";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if this parsed object is valid.
   *
   * \details
   * - Verify that the address is set.
   * - Verify that the MTU is set.
   * - Verify that the service discovery is set.
   * - Verify that at least one port is configured.
   * - Verify that bulk read count is configured.
   * - Verify that bulk read receive period is configured.
   * - Verify that the MTU value is larger than the IP header size for the used IP addresstype.
   * - Verify that if bulk read minimum number of datagrams to continue consecutive receive call is configured, that it
   *   is in valid range.
   * - Verify that if bulk read maximum number of consecutive receive calls is configured, that it is in valid range.
   *
   * \param[in] network_endpoint The network endpoint element to verify.
   *
   * \return "kOk" if the element is valid, otherwise the related "ValidationResult".
   *
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - If address is not set,
   *   - Set the validation result to indicate that no address is configured.
   * - Else if MTU is not set,
   *   - Set the validation result to indicate that no MTU is configured.
   * - Else if MTU is smaller than the overhead for IP address type,
   *   - Set the validation result to MTU too small.
   * - Else if service discovery information is not set,
   *   - Set the validation result to indicate that no service discovery is configured.
   * - Else if bulk read count is not set,
   *   - Set the validation result to indicate that no bulk read count is configured.
   * - Else if quality of service (QoS) is out of range,
   *   - Set the validation result to qos out of range.
   * - Else if check if TCP disconnect counter is set to 0, when TCP error reset counter is not set to 0
   *   - Set the validation result to invalid tcp threshold counter configuration.
   * - Else if bulk read minimum number of datagrams to continue consecutive receive call is configured, then if
   *   larger than bulk read count,
   *   - Set the valudation result to invalid bulk read minimum number of datagrams to continue consecutive receive.
   * - Else if bulk read maximum number of consecutive receive calls if configured, then if not valid range,
   *   - Set the valudation result to invalid bulk read maximum number of consecutive receive calls.
   * - Else
   *   - Set the validation result to be OK.
   * - Return the validation result.
   * \endinternal
   */
  static ValidationResult Check(NetworkEndpointConfigObject const& network_endpoint) noexcept {
    ValidationResult result{};

    if (network_endpoint.GetAddress().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingAddress;
    } else if ((network_endpoint.GetMtu().GetStatus() != ElementStatus::kSet)) {
      result = ValidationResult::kMissingMtu;
      // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
    } else if ((!network_endpoint.GetUdpPayloadBytes()) || (*network_endpoint.GetUdpPayloadBytes() == 0U)) {
      result = ValidationResult::kMtuTooSmall;

    } else if ((network_endpoint.GetServiceDiscovery().GetStatus() != ElementStatus::kSet)) {
      result = ValidationResult::kMissingServiceDiscovery;

    } else if ((network_endpoint.GetBulkReadCount().GetElement() < kBulkReadCountMin) ||
               (network_endpoint.GetBulkReadCount().GetElement() > kBulkReadCountMax)) {
      result = ValidationResult::kInvalidBulkReadCount;

    } else if (network_endpoint.GetQos().GetElement().has_value() &&
               (network_endpoint.GetQos().GetElement().value() > ConfigurationTypesAndDefs::kQosMax)) {
      result = ValidationResult::kQosOutOfRange;

    } else if ((network_endpoint.GetNoOfConsecutiveValidTcpMessagesBeforeErrorReset().GetElement() != 0U) &&
               (network_endpoint.GetNoOfInvalidTcpMessagesBeforeDisconnection().GetElement() == 0U)) {
      result = ValidationResult::kInvalidTcpThresholdCounterConfiguration;

    } else if ((network_endpoint.GetBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls()
                    .GetElement()
                    .has_value()) &&
               (network_endpoint.GetBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls().GetElement() >
                network_endpoint.GetBulkReadCount().GetElement())) {
      result = ValidationResult::kInvalidBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsConfiguration;
    } else if ((network_endpoint.GetBulkReadMaxNoOfConsecutiveReceiveCalls().GetElement().has_value()) &&
               (network_endpoint.GetBulkReadMaxNoOfConsecutiveReceiveCalls().GetElement() == 0U)) {
      result = ValidationResult::kInvalidBulkReadMaxNoOfConsecutiveReceiveCallsConfiguration;
    } else {
      result = ValidationResult::kOk;
    }

    return result;
  }

 private:
  static constexpr ConfigurationTypesAndDefs::BulkReadCountType const kBulkReadCountMin{1U};
  static constexpr ConfigurationTypesAndDefs::BulkReadCountType const kBulkReadCountMax{
      amsr::net::ip::Udp::Socket::GetMaxMessagesForBulkReceive()};
  static constexpr ConfigurationTypesAndDefs::BulkReadReceivePeriodUsType const kBulkReadReceivePeriodUsMax{
      std::numeric_limits<ConfigurationTypesAndDefs::BulkReadReceivePeriodUsType>::max()};
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_VALIDATOR_H_

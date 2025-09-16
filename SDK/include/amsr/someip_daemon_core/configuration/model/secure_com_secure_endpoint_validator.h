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
/*!        \file  secure_com_secure_endpoint_validator.h
 *        \brief  Validates Configuration object 'SecureComSecureEndpoint'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_ENDPOINT_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_ENDPOINT_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/vector.h"

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/secure_com_secure_endpoint_object.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {
/*!
 * \brief Groups consistency checks for secure endpoint elements.
 */
class SecureComSecureEndpointValidator final {
 public:
  /*!
   * \brief Validation results for one secure endpoint object.
   */
  enum class ValidationResult : std::uint32_t {
    /*!
     * \brief The secure channel mapping is valid
     */
    kOk = 0x00U,

    /*!
     * \brief The secure channel mapping object lacks port
     */
    kMissingPort,

    /*!
     * \brief The secure channel mapping object lacks address
     */
    kMissingAddress,

    /*!
     * \brief The secure channel mapping object lacks transport protocol
     */
    kMissingProtocol,

    /*!
     * \brief The secure channel mapping object lacks a is server flag
     */
    kMissingIsServer,

    /*!
     * \brief The secure channel object lacks cipher suite ids
     */
    kMissingCipherSuiteIds,

    /*!
     * \brief The secure channel mapping object lacks a PSK uuid
     */
    kMissingPskUuid,

    /*!
     * \brief The secure channel mapping object lacks a PSK identity
     */
    kMissingPskIdentity,

    /*!
     * \brief The secure channel mapping object lacks a PSK identity hint
     */
    kMissingPskIdentityHint,

    /*!
     * \brief The secure channel mapping object lacks a dtls cookie verification enabled
     */
    kMissingDtlsCookieVerificationEnabled
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

      case ValidationResult::kMissingPort:
        ret = "Missing port";
        break;

      case ValidationResult::kMissingAddress:
        ret = "Address is missing";
        break;

      case ValidationResult::kMissingProtocol:
        ret = "Protocol is missing";
        break;

      case ValidationResult::kMissingIsServer:
        ret = "Server is missing";
        break;

      case ValidationResult::kMissingCipherSuiteIds:
        ret = "Cipher suite IDs are missing";
        break;

      case ValidationResult::kMissingPskUuid:
        ret = "PSK UUID is missing";
        break;

      case ValidationResult::kMissingPskIdentity:
        ret = "PSK identity is missing";
        break;

      case ValidationResult::kMissingPskIdentityHint:
        ret = "PSK identity hint is missing";
        break;

      case ValidationResult::kMissingDtlsCookieVerificationEnabled:
        ret = "Dtls cookie verification enabled is missing";
        break;
    }

    return ret;
  }

  /*!
   * \brief Check if the latest parsed secure channel mapping is valid.
   * \details
   * - Verify that the port is set (mandatory) and no invalid port number is used.
   * - Verify that the address is present (mandatory).
   * - Verify that the transport protocol is present (mandatory).
   * - Verify that the is server flag is present (mandatory).
   * - Verify that the cipher suite ids array is present (mandatory).
   * - Verify that the PSK UUID is set (mandatory).
   * - Verify that the PSK identity is present (mandatory).
   * - Verify that the PSK identity hint is present, mandatory if server flag is set to false.
   * - Verify that the dtls cookie verification enabled is present (mandatory)
   * \param[in] secure_endpoint The secure channel mapping object to verify.
   * \return ValidationResult::kOk if the secure endpoint object has all mandatory elements set, otherwise other element
   * in enum ValidationResult reflecting correct status.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Initiate the validation result to be ok
   * - Check if PortElement is not set or the value is invalid
   *   - Set the validation result to secure channel mapping object lacks port
   * - Otherwise, check if AddressElement, ProtoElement, or IsServerElement is not set
   *   - Set the validation result to the corresponding configuration element is not configured
   * - Otherwise, check if CipherSuiteIdContainerElement is not set or is empty
   *   - Set the validation result to secure channel object lacks cipher suite IDs
   * - Otherwise, check if PskUuidElement or PskIdentityElement is not set
   *   - Set the validation result to corresponding configuration element is not configured
   * - Otherwise, check if PskIdentityElement is not set and the end point is not server
   *   - Set the validation result to secure channel mapping object lacks a PSK identity hint
   * - Otherwise, check if DtlsCookieVerificationEnabledElement is not configured
   *   - Set the validation result to secure channel mapping object lacks a dtls cookie verification enabled
   * - return the validation result
   * \endinternal
   */
  static ValidationResult Check(SecureComSecureEndpointObject const& secure_endpoint) noexcept {
    ValidationResult result{};

    if (secure_endpoint.GetPort().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPort;
    } else if (secure_endpoint.GetAddress().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingAddress;
    } else if (secure_endpoint.GetProto().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingProtocol;
    } else if (secure_endpoint.GetIsServer().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingIsServer;
    } else if ((secure_endpoint.GetCipherSuiteIds().GetStatus() != ElementStatus::kSet) ||
               (secure_endpoint.GetCipherSuiteIds().GetElement().empty())) {
      result = ValidationResult::kMissingCipherSuiteIds;
    } else if (secure_endpoint.GetPskUuid().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPskUuid;
    } else if (secure_endpoint.GetPskIdentity().GetStatus() != ElementStatus::kSet) {
      result = ValidationResult::kMissingPskIdentity;
    } else if ((secure_endpoint.GetPskIdentityHint().GetStatus() != ElementStatus::kSet) &&
               (secure_endpoint.GetIsServer().GetElement() == false)) {
      result = ValidationResult::kMissingPskIdentityHint;
    } else if ((secure_endpoint.GetDtlsCookieVerificationEnabled().GetStatus() != ElementStatus::kSet) &&
               (secure_endpoint.GetProto().GetElement() == ConfigurationTypesAndDefs::Protocol::kUDP)) {
      result = ValidationResult::kMissingDtlsCookieVerificationEnabled;
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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_ENDPOINT_VALIDATOR_H_

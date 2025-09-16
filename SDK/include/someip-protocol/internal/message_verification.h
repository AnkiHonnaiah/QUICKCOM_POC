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
/**       \file   someip-protocol/internal/message_verification.h
 *        \brief  Provides the message verification utilities.
 *        \unit   SomeIpProtocol::Messages::MessageVerification
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_VERIFICATION_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_VERIFICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_protocol {
namespace internal {

/*!
 * \brief General message verification class for SOME/IP method requests / responses, events and errors.
 * \tparam MessageType Contains the data to verify against according to the protocol specification.
 * E.g.: The expected return code and the expected message type.
 * If there are specific measures for a concrete message type, the derived
 * class shall override the behavior from the base.
 * \vprivate Product private
 */
template <typename MessageType>
class MessageVerification {
 public:
  MessageVerification() = delete;
  /*!
   * \brief   Default destructor.
   * \pre     -
   * \context Any
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~MessageVerification() noexcept = default;

  MessageVerification(MessageVerification const&) = delete;
  MessageVerification(MessageVerification&&) = delete;
  MessageVerification& operator=(MessageVerification const&) & = delete;
  MessageVerification& operator=(MessageVerification&&) & = delete;
  /*!
   * \brief Executes all the checks specified according the AUTOSAR COM_SWS specification.
   * \param header_expected Expected SOME/IP header from the deployment.
   * \param header_request SOME/IP header of the method request.
   * \return E_OK if all checks were successful - specific error return code if any of the
   * verification steps failed.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-4980088
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Set return code to kOk.
   * - Call VerifyInterfaceVersion and store result.
   * - If result is false
   *   - set return code to kWrongInterfaceVersion.
   * - Return return code.
   * \endinternal
   */
  static amsr::someip_protocol::internal::SomeIpReturnCode DoAllDeploymentChecks(
      amsr::someip_protocol::internal::SomeIpMessageHeader const& header_expected,
      amsr::someip_protocol::internal::SomeIpMessageHeader const& header_request) noexcept {
    amsr::someip_protocol::internal::SomeIpReturnCode return_code{
        amsr::someip_protocol::internal::SomeIpReturnCode::kOk};

    bool const valid_interface_v{
        VerifyInterfaceVersion(header_expected.interface_version_, header_request.interface_version_)};

    if (!valid_interface_v) {
      return_code = amsr::someip_protocol::internal::SomeIpReturnCode::kWrongInterfaceVersion;
    }

    return return_code;
  }

  /*!
   * \brief Verify that the interface version matches the major version of the
   * SOME/IP service interface deployment.
   *
   * \param expected_major_version Major version from the SOME/IP deployment.
   * \param interface_version The SOME/IP interface version to check against.
   * \return true if the interface version from the request SOME/IP header
   * matches the expected one, false if not.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static bool VerifyInterfaceVersion(amsr::someip_protocol::internal::MajorVersion expected_major_version,
                                     amsr::someip_protocol::internal::InterfaceVersion interface_version) noexcept {
    return (expected_major_version == interface_version);
  }

  /*!
   * \brief Do infrastructural checks that do not have any dependencies to the deployment configuration.
   * \param[in] header_request SOME/IP header from the request.
   * \return return code E_OK if all infrastructural checks were successful - specific error return code
   * if one of the following checks could not be verified successfully.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-4980078
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Verify protocol version.
   * - If protocol version is valid
   *   - Verify payload length.
   *   - Verify return code.
   *   - If payload or return code is invalid
   *     - Return kNotOk.
   *   - Otherwise
   *     - Verify message type.
   *     - If message type is invalid
   *       - Return kWrongMessageType.
   *     - Otherwise
   *       - Verify client id.
   *       - If client id is invalid.
   *         - Return kNotOk.
   *       - Otherwise return kOK.
   * - Otherwise
   *   - Return kWrongProtocolVersion.
   * \endinternal
   */
  static amsr::someip_protocol::internal::SomeIpReturnCode DoInfrastructuralChecks(
      amsr::someip_protocol::internal::SomeIpMessageHeader const& header_request) noexcept {
    amsr::someip_protocol::internal::SomeIpReturnCode return_code{
        amsr::someip_protocol::internal::SomeIpReturnCode::kNotOk};
    bool const valid_protocol_v{VerifyProtocolVersion(header_request.protocol_version_)};

    if (valid_protocol_v) {
      bool const valid_length{VerifyPayloadLength(header_request.length_)};
      bool const valid_return_code{MessageType::VerifyReturnCode(header_request.return_code_)};

      if (valid_length && valid_return_code) {
        bool const valid_client_id{MessageType::VerifyClientId(header_request.client_id_)};
        // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
        if (valid_client_id) {
          return_code = amsr::someip_protocol::internal::SomeIpReturnCode::kOk;
        }
      }
    } else {
      return_code = amsr::someip_protocol::internal::SomeIpReturnCode::kWrongProtocolVersion;
    }

    return return_code;
  }

  /*!
   * \brief Do infrastructural checks that do not have any dependencies to the deployment configuration.
   * \return return code E_OK because the length set in PDU header can be zero
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-4980078
   * \spec
   *   requires true;
   * \endspec
   *
   */
  static amsr::someip_protocol::internal::SomeIpReturnCode DoInfrastructuralChecks(
      amsr::someip_protocol::internal::PduMessageHeader const&) noexcept {
    // length set in header can be zero
    return amsr::someip_protocol::internal::SomeIpReturnCode::kOk;
  }

  /*!
   * \brief Verify the protocol version of the request / response / notification.
   * \remark Override in derived class if necessary.
   * \remark Same for all verification steps (methods and events).
   * \param protocol_version Protocol version from the request.
   * \return true if the protocol version matches with the value from the specification, false if not.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static bool VerifyProtocolVersion(amsr::someip_protocol::internal::ProtocolVersion protocol_version) noexcept {
    return (protocol_version == kProtocolVersion);
  }

  /*!
   * \brief Verify that the length is effectively larger than 7.
   * \remark Override in derived class if necessary.
   * \remark Same for all verification steps (method requests & responses, events and errors).
   * \param length SOME/IP length value from the incoming response / request / notification / error.
   * \return true if the length is greater than the value from the specification, false if not.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static bool VerifyPayloadLength(amsr::someip_protocol::internal::LengthField length) noexcept {
    return (length >= kMinimumPayloadLength);
  }

  /*!
   * \brief Verify that the return code is set to the one specified for this
   * message type.
   * \param return_code The return_code from the SOME/IP response / request / notification / error.
   * \return true if the return code is set to E_OK, false if it's the wrong return code for a request.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-4981306
   * \spec
   *   requires true;
   * \endspec
   */
  static bool VerifyReturnCode(amsr::someip_protocol::internal::SomeIpReturnCode return_code) noexcept {
    return (return_code == MessageType::kReturnCodeExpected);
  }

  /*!
   * \brief Verify the client id of this SOME/IP message.
   * \return true. All client ids are normally valid.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-4981288
   * \spec
   *   requires true;
   * \endspec
   */
  static bool VerifyClientId(amsr::someip_protocol::internal::ClientId) noexcept { return true; }
};

/*!
 * \brief Upon reception of a SOME/IP request message certain check shall be conducted.
 *
 * \trace SPEC-4980088
 * \trace SPEC-4980123
 */
class RequestMessageVerification final : public MessageVerification<RequestMessageVerification> {
 public:
  /*!
   * \brief Expected return code for a method request. Verify that the return code is set to E_OK (0x00).
   * \trace SPEC-4981306
   */
  static constexpr amsr::someip_protocol::internal::SomeIpReturnCode kReturnCodeExpected{
      amsr::someip_protocol::internal::SomeIpReturnCode::kOk};
};

/*!
 * \brief Upon reception of a SOME/IP request & no return message certain check shall be conducted.
 *
 * \trace SPEC-4980088
 */
class RequestNoReturnMessageVerification final : public MessageVerification<RequestNoReturnMessageVerification> {
 public:
  /*!
   * \brief Expected return code for a fire & forget request. Verify that the return code is set to E_OK (0x00).
   * \trace SPEC-4981306
   */
  static constexpr amsr::someip_protocol::internal::SomeIpReturnCode kReturnCodeExpected{
      amsr::someip_protocol::internal::SomeIpReturnCode::kOk};
};

/*!
 * \brief Class for verification of received event messages.
 *
 * \trace SPEC-4980078
 * \trace SPEC-4980113
 */
class EventMessageVerification final : public MessageVerification<EventMessageVerification> {
 public:
  /*!
   * \brief Expected return code for an event notification message.
   * Verify that the return code is set to E_OK (0x00).
   */
  static constexpr amsr::someip_protocol::internal::SomeIpReturnCode kReturnCodeExpected{
      amsr::someip_protocol::internal::SomeIpReturnCode::kOk};

  /*!
   * \brief Verify that the client id of this SOME/IP message is set to 0.
   * \param client_id The client_id of the SOME/IP events and field notification messages.
   * \return true if client_id is 0, false otherwise.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static inline bool VerifyClientId(amsr::someip_protocol::internal::ClientId client_id) noexcept {
    return (client_id == 0U);
  }
};

/*!
 * \brief Class for verification of response messages.
 *
 * \trace SPEC-4980099
 * \trace SPEC-4980134
 * \trace SPEC-4980102
 */
class ResponseMessageVerification final : public MessageVerification<ResponseMessageVerification> {
 public:
  /*!
   * \brief   Verify the SOME/IP return code for response messages
   * \details For AUTOSAR versions R19-03 and above, messages of type kResponse are not allowed
   *          to contain return error codes. Nevertheless, for backward compatibilities with clients
   *          implementing an older version, such messages shall still be forwarded to the application.
   *          Similar to error messages the return code could be within a range of 0x01 to 0x5E.
   * \param[in] return_code SOME/IP return code to verify.
   * \return true if return_code is within the range of 0x20 to 0x5E or if it is kOk (0x00).
   * \pre -
   * \context Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static inline bool VerifyReturnCode(amsr::someip_protocol::internal::SomeIpReturnCode return_code) noexcept {
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    return (return_code == kReturnCodeExpected) ||
           ((return_code >= amsr::someip_protocol::internal::SomeIpReturnCode::kRangeServiceErrorsStart) &&
            (return_code <= amsr::someip_protocol::internal::SomeIpReturnCode::kRangeServiceErrorsEnd));
  }

 private:
  /*!
   * \brief Expected return code for a response message. Verify that the return code is set to E_OK (0x00).
   */
  static constexpr amsr::someip_protocol::internal::SomeIpReturnCode kReturnCodeExpected{
      amsr::someip_protocol::internal::SomeIpReturnCode::kOk};
};

/*!
 * \brief Class for verification of error messages.
 *
 * \trace SPEC-4980099
 * \trace SPEC-4980134
 */
class ErrorMessageVerification final : public MessageVerification<ErrorMessageVerification> {
 public:
  /*!
   * \brief Expected return code for an error response. Verify that the return code is set to E_NOT_OK (0x01).
   * \remark Shall not be 0x00 (E_OK).
   *
   * \trace SPEC-4981470
   */
  static constexpr amsr::someip_protocol::internal::SomeIpReturnCode kReturnCodeExpected{
      amsr::someip_protocol::internal::SomeIpReturnCode::kNotOk};

  /*!
   * \brief For error messages the return code could be within a range of 0x01 to 0x5E.
   * \param return_code SOME/IP return code to verify.
   * \return true if return_code is within the range of 0x01 to 0x5E.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static inline bool VerifyReturnCode(amsr::someip_protocol::internal::SomeIpReturnCode return_code) noexcept {
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    return ((return_code >= amsr::someip_protocol::internal::SomeIpReturnCode::kNotOk) &&
            (return_code <= amsr::someip_protocol::internal::SomeIpReturnCode::kRangeServiceErrorsEnd));
  }
};

}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_VERIFICATION_H_

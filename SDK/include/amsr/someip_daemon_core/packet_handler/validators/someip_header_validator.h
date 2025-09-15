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
/**        \file  someip_header_validator.h
 *        \brief  SOME/IP Header Validator.
 *
 *      \details  This class checks whether SOME/IP header of a packet is valid or not.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_SOMEIP_HEADER_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_SOMEIP_HEADER_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/packet_handler/validators/validation_common.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/message_verification.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {
namespace validators {

/*!
 * \brief This class checks whether SOME/IP header of incoming packets is consistent or not.
 */
class SomeIpHeaderValidator final {
 public:
  /*!
   * \brief Default constructor.
   *
   * \pre -
   * \context Init
   * \steady  FALSE
   */
  SomeIpHeaderValidator() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  ~SomeIpHeaderValidator() noexcept = default;

  SomeIpHeaderValidator(SomeIpHeaderValidator const&) = delete;
  SomeIpHeaderValidator(SomeIpHeaderValidator&&) = delete;
  SomeIpHeaderValidator& operator=(SomeIpHeaderValidator const&) = delete;
  SomeIpHeaderValidator& operator=(SomeIpHeaderValidator&&) = delete;

  /*!
   * \brief Check if the given SOME/IP packet has a valid header.
   *
   * \param[in] header SOME/IP packet header to be validated.
   *
   * \steady  TRUE
   * \return Validation status.
   *
   * \internal
   * - Set the return value to indicate validation failure
   * - If the message type is "notification"
   *   - Verify via the event message verifier and set the return value as the result of the verification
   * - Else if the message type is "response"
   *   - Verify via the "response" message verifier and set the return value as the result of the verification
   * - Else if the message type is "request"
   *   - Verify via the "request" message verifier and set the return value as the result of the verification
   * - Else if the message type is "fire-and-forget"
   *   - Verify via the "fire-and-forget" message verifier and set the return value as the result of the verification
   * - Else if the message type is "error"
   *   - Verify via the "error" message verifier and set the return value as the result of the verification
   * - Else
   *   - Set the return value to indicate validation failure due to wrong message type
   * - Return the return value
   * \endinternal
   */
  static PacketValidatorReturnCode IsHeaderValid(someip_protocol::internal::SomeIpMessageHeader const& header) {
    PacketValidatorReturnCode result{PacketValidatorReturnCode::kNotOk};

    switch (header.message_type_) {
      case someip_protocol::internal::SomeIpMessageType::kNotification: {
        result = ToPacketValidatorReturnCode(
            someip_protocol::internal::EventMessageVerification::DoInfrastructuralChecks(header));
        break;
      }
      case someip_protocol::internal::SomeIpMessageType::kResponse: {
        result = ToPacketValidatorReturnCode(
            someip_protocol::internal::ResponseMessageVerification::DoInfrastructuralChecks(header));
        break;
      }
      case someip_protocol::internal::SomeIpMessageType::kRequest: {
        result = ToPacketValidatorReturnCode(
            someip_protocol::internal::RequestMessageVerification::DoInfrastructuralChecks(header));
        break;
      }
      case someip_protocol::internal::SomeIpMessageType::kRequestNoReturn: {
        result = ToPacketValidatorReturnCode(
            someip_protocol::internal::RequestNoReturnMessageVerification::DoInfrastructuralChecks(header));
        break;
      }
      case someip_protocol::internal::SomeIpMessageType::kError: {
        result = ToPacketValidatorReturnCode(
            someip_protocol::internal::ErrorMessageVerification::DoInfrastructuralChecks(header));
        break;
      }
      default: {
        result = PacketValidatorReturnCode::kHeaderValidationError_WrongMessageType;
        break;
      }
    }

    return result;
  }
};

}  // namespace validators
}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_SOMEIP_HEADER_VALIDATOR_H_

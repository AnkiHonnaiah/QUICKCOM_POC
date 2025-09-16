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
/**        \file  transmission_protocol_validator.h
 *        \brief  Transmission Protocol Validator.
 *
 *      \details  Validator for the correctness of the transmission protocol (UDP/TCP) for
 *                remotely incoming packets.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_TRANSMISSION_PROTOCOL_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_TRANSMISSION_PROTOCOL_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/packet_handler/validators/validation_common.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {
namespace validators {

/*!
 * \brief This class validates the correctness of the transmission protocol (UDP/TCP) for
 *        remotely incoming packets.
 */
class TransmissionProtocolValidator final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in] config The configuration.
   * \steady FALSE
   */
  explicit TransmissionProtocolValidator(configuration::Configuration const& config) noexcept : config_{config} {}

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  ~TransmissionProtocolValidator() noexcept = default;

  TransmissionProtocolValidator(TransmissionProtocolValidator const&) = delete;
  TransmissionProtocolValidator(TransmissionProtocolValidator&&) = delete;
  TransmissionProtocolValidator& operator=(TransmissionProtocolValidator const&) = delete;
  TransmissionProtocolValidator& operator=(TransmissionProtocolValidator&&) = delete;

  /*!
   * \brief Check if the given SOME/IP packet is received with the correct transmission protocol (UDP/TCP).
   *
   * \param[in] header The SOME/IP packet header to be validated.
   * \param[in] protocol The remote source protocol used to receive the message.
   *
   * \steady  TRUE
   * \return "PacketValidatorReturnCode".
   *         "kNotOk" if received using a wrong transmission protocol, "kOk" otherwise.
   *
   * \trace SPEC-4980084
   * \trace SPEC-4980095
   * \trace SPEC-4980109
   * \trace SPEC-4980119
   * \trace SPEC-4980130
   *
   * \internal
   * - Set the return value to kNotOk.
   * - If message type is "request", "request-no-return", "response" or "error"
   *   - Check the protocol used by the method
   *   - If the protocol used by the method is same as expected
   *     - Set the return value to indicate successful validation
   *   - Else
   *     - Set the return value to indicate failed validation
   * - Else if message type is "notification"
   *   - Check the protocol used by the event
   *   - If the protocol used by the event is same as expected
   *     - Set the return value to indicate successful validation
   *   - Else
   *     - Set the return value to indicate failed validation
   * - Else if message type is "kTpRequest", "kTpRequestNoReturn", "kTpNotification", "kTpResponse" or "kTpError"
   *   - Do not perform any validation; set the return value to indicate successful validation.
   * - Return the return value
   * \endinternal
   */
  PacketValidatorReturnCode CheckTransmissionProtocol(someip_protocol::internal::SomeIpMessageHeader const& header,
                                                      configuration::Configuration::Protocol const protocol) const {
    PacketValidatorReturnCode result{PacketValidatorReturnCode::kNotOk};

    // Check transmission protocol.
    switch (header.message_type_) {
      case someip_protocol::internal::SomeIpMessageType::kRequest:
      case someip_protocol::internal::SomeIpMessageType::kRequestNoReturn:
      case someip_protocol::internal::SomeIpMessageType::kResponse:
      case someip_protocol::internal::SomeIpMessageType::kError: {
        configuration::Configuration::Method const* const method_ptr{
            config_.GetMethod(header.service_id_, header.interface_version_, header.method_id_)};
        // We have already checked that the method exists.
        assert(method_ptr != nullptr);
        // Check if the protocol is as expected.
        result = (method_ptr->proto_ == protocol) ? PacketValidatorReturnCode::kOk
                                                  : PacketValidatorReturnCode::kTransmissionProtocolError_MethodNotOk;
        break;
      }
      case someip_protocol::internal::SomeIpMessageType::kNotification: {
        configuration::Configuration::Event const* const event_ptr{
            config_.GetEvent(header.service_id_, header.interface_version_, header.method_id_)};
        // We have already checked that the event exists.
        assert(event_ptr != nullptr);
        // Check if the protocol is as expected.
        result = (event_ptr->proto_ == protocol) ? PacketValidatorReturnCode::kOk
                                                 : PacketValidatorReturnCode::kTransmissionProtocolError_EventNotOk;
        break;
      }
      case someip_protocol::internal::SomeIpMessageType::kTpError: {
        result = PacketValidatorReturnCode::kOk;
        break;
      }

      // As "header.message_type_" is an integer.
      default:
        // Nothing to do.
        break;
    }

    return result;
  }

 private:
  /*!
   * \brief Reference to the configuration.
   */
  configuration::Configuration const& config_;
};

}  // namespace validators
}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_TRANSMISSION_PROTOCOL_VALIDATOR_H_

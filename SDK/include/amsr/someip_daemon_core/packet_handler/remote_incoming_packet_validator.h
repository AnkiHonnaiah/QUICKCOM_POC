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
/**        \file  remote_incoming_packet_validator.h
 *        \brief  Remote incoming packet validator.
 *
 *      \details  This class validates incoming packets from remote sources (other daemons).
 *                It does all the required validations on the packet.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_REMOTE_INCOMING_PACKET_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_REMOTE_INCOMING_PACKET_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/packet_handler/validators/configuration_validator.h"
#include "amsr/someip_daemon_core/packet_handler/validators/security_validator.h"
#include "amsr/someip_daemon_core/packet_handler/validators/someip_header_validator.h"
#include "amsr/someip_daemon_core/packet_handler/validators/transmission_protocol_validator.h"
#include "amsr/someip_daemon_core/packet_handler/validators/validation_common.h"
// Note this include is required for the RemoteSourceIdentifier which is present in network packet router interface.
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/message_builder.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {

/*!
 * \brief This class is responsible to do all required validations and checks of incoming
 *        packets from remote sources.
 */
class RemoteIncomingPacketValidator final {
 public:
  /*!
   * \brief Type alias for "RemoteSourceIdentifier".
   */
  using RemoteSourceIdentifier = packet_router::RemoteSourceIdentifier;

  /*!
   * \brief Constructor.
   *
   * \param[in] config Configuration.
   * \param[in,out] identity_access_manager The Identity Access Manager to use for checking access rights.
   * \param[in] statistics_handler A reference to the statistics handler.
   *
   * \steady  FALSE
   */
  RemoteIncomingPacketValidator(
      configuration::Configuration const& config, iam::IamInterface& identity_access_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept
      : identity_access_manager_{identity_access_manager},
        someip_header_validator_{},
        config_validator_{config},
        transmission_protocol_validator_{config},
        security_validator_{identity_access_manager},
        statistics_handler_{statistics_handler} {}

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  ~RemoteIncomingPacketValidator() noexcept = default;

  RemoteIncomingPacketValidator(RemoteIncomingPacketValidator const&) = delete;
  RemoteIncomingPacketValidator(RemoteIncomingPacketValidator&&) = delete;
  RemoteIncomingPacketValidator& operator=(RemoteIncomingPacketValidator const&) = delete;
  RemoteIncomingPacketValidator& operator=(RemoteIncomingPacketValidator&&) = delete;

  /*!
   * \brief Validate the incoming SOME/IP packet.
   *        If any of the validations fails, handle the failure.
   *
   * \param[in] source_identifier The source identifier of the remote packet source from which
   *                              the passed SOME/IP message comes.
   * \param[in] header SOME/IP header of the packet.
   *
   * \steady  TRUE
   * \return "ValidationResult" structure.
   *
   * \internal
   * - Check the validity of the SOME/IP header
   * - If the result of the check is positive
   *   - Check the transmission protocol against the configuration
   *   - Otherwise, report error to the statistics handler
   * - If the result of checks is positive
   *   - Check for access and identity (IAM)
   *     - If result of the above check indicates access denial
   *       - Set the result as "security-validation-error"
   *   - Otherwise, report error to the statistics handler
   * - Return the result of check(s)
   * \endinternal
   */
  validators::PacketValidatorReturnCode Validate(RemoteSourceIdentifier const source_identifier,
                                                 amsr::someip_protocol::internal::SomeIpMessageHeader const& header,
                                                 someip_protocol::internal::InstanceId const instance_id) const {
    validators::PacketValidatorReturnCode result{someip_header_validator_.IsHeaderValid(header)};

    if (result == validators::PacketValidatorReturnCode::kOk) {
      // Check received method/event against configuration.
      result = config_validator_.CheckAgainstConfiguration(header);

      if (result == validators::PacketValidatorReturnCode::kOk) {
        // Check transmission protocol.
        result = transmission_protocol_validator_.CheckTransmissionProtocol(header, source_identifier.protocol_);

        if (result == validators::PacketValidatorReturnCode::kOk) {
          someip_protocol::internal::SomeIpMessageType const msg_type{header.message_type_};
          if ((msg_type == someip_protocol::internal::SomeIpMessageType::kRequest) ||
              (msg_type == someip_protocol::internal::SomeIpMessageType::kRequestNoReturn)) {
            // Access and identity check.
            someip_protocol::internal::IpAddress const from_address{source_identifier.address_.ToString()};
            someip_protocol::internal::Port const from_port{source_identifier.port_.port};
            someip_daemon_core::iam::IamInterface::NetworkEndpoint const& sender{from_address, from_port};
            someip_protocol::internal::MajorVersion const major_version{header.interface_version_};
            someip_protocol::internal::ServiceId const service_id{header.service_id_};
            someip_protocol::internal::MethodId const method_id{header.method_id_};

            bool const is_granted{
                identity_access_manager_.CheckMethod(service_id, major_version, instance_id, method_id, sender)};
            if (!is_granted) {
              result = validators::PacketValidatorReturnCode::kSecurityValidationError;
            }
          }
        } else {
          // Report statistics
          statistics_handler_.ReportInvalidSomeIpMessage();
        }
      } else {
        // Report statistics
        if (result == validators::PacketValidatorReturnCode::kConfigurationError_UnknownService) {
          statistics_handler_.ReportInvalidSomeIpServiceId();
        } else if (result == validators::PacketValidatorReturnCode::kConfigurationError_WrongInterfaceVersion) {
          statistics_handler_.ReportInvalidSomeIpInterfaceVersion();
        } else if (result == validators::PacketValidatorReturnCode::kConfigurationError_UnknownMethod) {
          statistics_handler_.ReportInvalidSomeIpMethodId();
        } else {
          statistics_handler_.ReportInvalidSomeIpMessage();
        }
      }

    } else {
      // Report statistics
      if (result == validators::PacketValidatorReturnCode::kHeaderValidationError_WrongProtocolVersion) {
        statistics_handler_.ReportInvalidSomeIpProtocolVersion();
      } else {
        statistics_handler_.ReportInvalidSomeIpHeader();
      }
    }

    return result;
  }

 private:
  /*!
   * \brief Reference to the identity access manager.
   */
  iam::IamInterface& identity_access_manager_;

  // --------- Validators ----------------------
  /*!
   * \brief Validator for SOME/IP header.
   */
  validators::SomeIpHeaderValidator someip_header_validator_;

  /*!
   * \brief Validator for configuration.
   */
  validators::ConfigurationValidator config_validator_;

  /*!
   * \brief Validator for the transmission protocol used (UDP vs TCP).
   */
  validators::TransmissionProtocolValidator transmission_protocol_validator_;

  /*!
   * \brief Security validator.
   */
  validators::SecurityValidator security_validator_;

  /*!
   * \brief The statistics handler.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;
};

}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_REMOTE_INCOMING_PACKET_VALIDATOR_H_

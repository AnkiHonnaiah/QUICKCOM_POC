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
/**        \file  remote_event_packet_validator.h
 *        \brief  Remote event packet validator.
 *
 *      \details  This class validates incoming events from remote sources (other daemons).
 *                It does all the required validations on the packet.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_REMOTE_EVENT_PACKET_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_REMOTE_EVENT_PACKET_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/packet_handler/validators/configuration_validator.h"
#include "amsr/someip_daemon_core/packet_handler/validators/remote_incoming_packet_validator_error.h"
#include "amsr/someip_daemon_core/packet_handler/validators/validation_common.h"
#include "ara/core/result.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/message_verification.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {

/*!
 * \brief This class is responsible to do all required validations and checks of incoming
 *        events from remote sources.
 */
class RemoteEventPacketValidator final {
 public:
  /*!
   * \brief Type alias for event pointer.
   */
  using EventPtr = configuration::Configuration::Event const* const;

  /*!
   * \brief Constructor.
   *
   * \param[in] config Configuration.
   * \param[in] statistics_handler A reference to the statistics handler.
   *
   * \context Init
   * \steady  FALSE
   */
  RemoteEventPacketValidator(
      configuration::Configuration const& config,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept
      : config_{config}, statistics_handler_{statistics_handler} {}

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  ~RemoteEventPacketValidator() noexcept = default;

  RemoteEventPacketValidator(RemoteEventPacketValidator const&) = delete;
  RemoteEventPacketValidator(RemoteEventPacketValidator&&) = delete;
  RemoteEventPacketValidator& operator=(RemoteEventPacketValidator const&) = delete;
  RemoteEventPacketValidator& operator=(RemoteEventPacketValidator&&) = delete;

  /*!
   * \brief Validate the incoming SOME/IP packet.
   *        If any of the validations fails, handle the failure.
   *
   * \param[in] protocol  The transport protocol of the remote packet.
   * \param[in] header    SOME/IP header of the packet.
   *
   * \return ara::core::Result with a pointer to the event configuration or an error
   *
   * \context Network
   * \steady  TRUE
   *
   * \internal
   * - Check the validity of the SOME/IP header
   * - If the result of the check is positive
   *   - Check the transmission protocol against the configuration
   *   - Otherwise, report error to the statistics handler
   * - If the result of checks is positive
   *   - Set the result as "security-validation-error"
   *   - Otherwise, report error to the statistics handler
   * - Return the result of check(s)
   * \endinternal
   */
  ara::core::Result<EventPtr> Validate(configuration::ConfigurationTypesAndDefs::Protocol const protocol,
                                       amsr::someip_protocol::internal::SomeIpMessageHeader const& header) const {
    ara::core::Result<EventPtr> validation_result{validators::RemoteIncomingPacketValidatorError::kNotOk};

    EventPtr event_ptr{config_.GetEvent(header.service_id_, header.interface_version_, header.method_id_)};

    if (event_ptr != nullptr) {
      // Check received header for correctness.
      validators::PacketValidatorReturnCode const result{validators::ToPacketValidatorReturnCode(
          someip_protocol::internal::EventMessageVerification::DoInfrastructuralChecks(header))};

      if (result == validators::PacketValidatorReturnCode::kOk) {
        // Check transmission protocol.
        if (event_ptr->proto_ == protocol) {
          validation_result.EmplaceValue(event_ptr);
        } else {
          // Emplace error
          validation_result.EmplaceError(
              validators::RemoteIncomingPacketValidatorError::kTransmissionProtocolError_EventNotOk);
          // Report statistics
          statistics_handler_.ReportInvalidSomeIpMessage();
        }

      } else {
        // Emplace error
        validation_result.EmplaceError(TranslateErrorCode(result));

        if (result == validators::PacketValidatorReturnCode::kHeaderValidationError_WrongProtocolVersion) {
          statistics_handler_.ReportInvalidSomeIpProtocolVersion();
        } else {
          statistics_handler_.ReportInvalidSomeIpHeader();
        }
      }

    } else {
      // There is a configuration error. Call the configuration validator to get the specific error
      validators::PacketValidatorReturnCode const result{config_validator_.CheckAgainstConfiguration(header)};

      // The check must have an error, otherwise we would not be in this branch
      assert(result != validators::PacketValidatorReturnCode::kOk);

      // Emplace error
      validation_result.EmplaceError(TranslateErrorCode(result));

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

    return validation_result;
  }

  /*!
   * \brief Validate the incoming PDU packet.
   *        If any of the validations fails, handle the failure.
   *
   * \param[in] protocol  The transport protocol of the remote packet.
   * \param[in] header    PDU header of the packet.
   *
   * \return ara::core::Result with a pointer to the event configuration or an error
   *
   * \context Network
   * \steady  TRUE
   *
   * \internal
   * - Check the validity of the PDU header against configuration
   * - If the result of the check is positive
   *   - Check the transmission protocol against the configuration
   * - Return the result of check(s)
   * \endinternal
   */
  ara::core::Result<EventPtr> ValidatePdu(configuration::ConfigurationTypesAndDefs::Protocol const protocol,
                                          amsr::someip_protocol::internal::PduMessageHeader const& header) const {
    ara::core::Result<EventPtr> validation_result{validators::RemoteIncomingPacketValidatorError::kNotOk};

    EventPtr event_ptr{config_.GetEvent(someip_protocol::internal::PduMessageHeaderUtility::GetServiceId(header),
                                        // Use of kMajorVersionAny because PDU packets do not support MajorVersion
                                        someip_protocol::internal::kMajorVersionAny,
                                        someip_protocol::internal::PduMessageHeaderUtility::GetMethodId(header))};

    if (event_ptr != nullptr) {
      // Check transmission protocol.
      if (event_ptr->proto_ == protocol) {
        validation_result.EmplaceValue(event_ptr);
      } else {
        validation_result.EmplaceError(
            validators::RemoteIncomingPacketValidatorError::kTransmissionProtocolError_EventNotOk);
      }
    } else {
      // There is a configuration error. Call the configuration validator to get the specific error
      validators::PacketValidatorReturnCode const result{config_validator_.CheckAgainstConfiguration(header)};
      // Emplace error
      validation_result.EmplaceError(TranslateErrorCode(result));
    }

    return validation_result;
  }

 private:
  /*!
   * \brief Translation between error code types
   *
   * \param[in] error_code The error code to translate
   *
   * \return The validation error in RemoteIncomingPacketValidationError format
   *
   * \context Network
   * \steady  TRUE
   *
   * \internal
   * - Check input error code and translate it to RemoteIncomingPacketValidationError format
   * \endinternal
   */
  static validators::RemoteIncomingPacketValidatorError TranslateErrorCode(
      validators::PacketValidatorReturnCode const error_code) {
    validators::RemoteIncomingPacketValidatorError translated_error{
        validators::RemoteIncomingPacketValidatorError::kNotOk};
    ara::core::Map<validators::PacketValidatorReturnCode, validators::RemoteIncomingPacketValidatorError>
        translate_error_code_map{
            {validators::PacketValidatorReturnCode::kHeaderValidationError_WrongProtocolVersion,
             validators::RemoteIncomingPacketValidatorError::kHeaderValidationError_WrongProtocolVersion},
            {validators::PacketValidatorReturnCode::kHeaderValidationError_MalformedMessage,
             validators::RemoteIncomingPacketValidatorError::kHeaderValidationError_MalformedMessage},
            {validators::PacketValidatorReturnCode::kHeaderValidationError_WrongMessageType,
             validators::RemoteIncomingPacketValidatorError::kHeaderValidationError_WrongMessageType},
            {validators::PacketValidatorReturnCode::kConfigurationError_UnknownMethod,
             validators::RemoteIncomingPacketValidatorError::kConfigurationError_UnknownMethod},
            {validators::PacketValidatorReturnCode::kConfigurationError_UnknownEvent,
             validators::RemoteIncomingPacketValidatorError::kConfigurationError_UnknownEvent},
            {validators::PacketValidatorReturnCode::kConfigurationError_WrongInterfaceVersion,
             validators::RemoteIncomingPacketValidatorError::kConfigurationError_WrongInterfaceVersion},
            {validators::PacketValidatorReturnCode::kConfigurationError_UnknownService,
             validators::RemoteIncomingPacketValidatorError::kConfigurationError_UnknownService},
            {validators::PacketValidatorReturnCode::kTransmissionProtocolError_MethodNotOk,
             validators::RemoteIncomingPacketValidatorError::kTransmissionProtocolError_MethodNotOk},
            {validators::PacketValidatorReturnCode::kTransmissionProtocolError_EventNotOk,
             validators::RemoteIncomingPacketValidatorError::kTransmissionProtocolError_EventNotOk},
            {validators::PacketValidatorReturnCode::kSecurityValidationError,
             validators::RemoteIncomingPacketValidatorError::kSecurityValidationError},
        };

    auto const iter = translate_error_code_map.find(error_code);

    if (iter != translate_error_code_map.end()) {
      translated_error = iter->second;
    }

    return translated_error;
  }

  /*!
   * \brief Reference to the configuration.
   */
  configuration::Configuration const& config_;

  /*!
   * \brief The statistics handler.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  // --------- Validators ----------------------
  /*!
   * \brief Validator for configuration.
   */
  validators::ConfigurationValidator const config_validator_{config_};
};

}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_REMOTE_EVENT_PACKET_VALIDATOR_H_

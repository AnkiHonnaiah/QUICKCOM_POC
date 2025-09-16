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
/**        \file  local_incoming_packet_validator.h
 *        \brief  Local incoming packet validator
 *
 *      \details  This class does all the required validations on the header of the packet.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_LOCAL_INCOMING_PACKET_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_LOCAL_INCOMING_PACKET_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/credentials.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/packet_handler/validators/configuration_validator.h"
#include "amsr/someip_daemon_core/packet_handler/validators/local_incoming_packet_validator_error.h"
#include "amsr/someip_daemon_core/packet_handler/validators/security_validator.h"
#include "amsr/someip_daemon_core/packet_handler/validators/someip_header_validator.h"
#include "amsr/someip_daemon_core/packet_handler/validators/validation_common.h"
#include "ara/core/result.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {

/*!
 * \brief This class is responsible to do all required validations and checks of incoming
 *        packets from local sources.
 */
class LocalIncomingPacketValidator final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in] config                   Configuration.
   * \param[in] identity_access_manager  The Identity Access Manager to use for checking access rights.
   *
   * \pre -
   * \context Init
   * \steady  FALSE
   *
   */
  LocalIncomingPacketValidator(configuration::Configuration const& config,
                               iam::IamInterface& identity_access_manager) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  ~LocalIncomingPacketValidator() noexcept = default;

  LocalIncomingPacketValidator(LocalIncomingPacketValidator const&) = delete;
  LocalIncomingPacketValidator(LocalIncomingPacketValidator&&) = delete;
  LocalIncomingPacketValidator& operator=(LocalIncomingPacketValidator const&) = delete;
  LocalIncomingPacketValidator& operator=(LocalIncomingPacketValidator&&) = delete;

  /*!
   * \brief Validate the incoming SOME/IP packet and return the result.
   *
   * \param[in] source_identifier The source identifier of the local packet source from which
   *                              the passed SOME/IP message comes.
   * \param[in] header            SOME/IP header of the packet.
   *
   * \return validation result.
   *
   * \pre -
   * \context App
   * \steady  TRUE
   */
  ara::core::Result<void> Validate(amsr::ipc::Credentials const source_identifier,
                                   amsr::someip_protocol::internal::SomeIpMessageHeader const& header,
                                   amsr::someip_protocol::internal::InstanceId const instance_id) const noexcept;

  /*!
   * \brief Validate the incoming PDU packet and return the result.
   *
   * \param[in] header PDU header of the packet.
   *
   * \return Validation result.
   *
   * \pre -
   * \context App
   * \steady  TRUE
   */
  ara::core::Result<void> Validate(amsr::someip_protocol::internal::PduMessageHeader const& header) const noexcept;

 private:
  /*!
   * \brief Translation between error code types
   *
   * \param[in] error_code The error code to translate
   *
   * \return The validation error in LocalIncomingPacketValidationError format
   *
   * \context App
   * \steady  TRUE
   */
  static validators::LocalIncomingPacketValidatorError TranslateErrorCode(
      validators::PacketValidatorReturnCode const error_code);

  // --------- Validators ----------------------
  /*!
   * \brief Validator for configuration.
   */
  validators::ConfigurationValidator const config_validator_;

  /*!
   * \brief Security validator.
   */
  validators::SecurityValidator const security_validator_;

  /*!
   * \brief For testing the default case of TranslateErrorCode, which is private.
   */
  FRIEND_TEST(UT__LocalIncomingPacketValidator, TranslateErrorCode_InvalidParameter);
};

}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_LOCAL_INCOMING_PACKET_VALIDATOR_H_

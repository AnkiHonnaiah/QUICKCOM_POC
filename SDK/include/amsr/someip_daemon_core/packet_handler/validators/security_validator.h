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
/**        \file  security_validator.h
 *        \brief  Security Validator
 *
 *      \details  This class checks if the incoming packet should be accepted
 *                or not based on security parameters.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_SECURITY_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_SECURITY_VALIDATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_handler {
namespace validators {

/*!
 * \brief This class checks if an incoming packet is allowed or not.
 */
class SecurityValidator final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in,out] identity_access_manager Identity and Access Manager.
   * \context Init
   * \steady  FALSE
   */
  explicit SecurityValidator(iam::IamInterface& identity_access_manager) noexcept
      : identity_access_manager_{identity_access_manager} {}

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context Shutdown
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~SecurityValidator() noexcept = default;

  SecurityValidator(SecurityValidator const&) = delete;
  SecurityValidator(SecurityValidator&&) = delete;
  SecurityValidator& operator=(SecurityValidator const&) = delete;
  SecurityValidator& operator=(SecurityValidator&&) = delete;

  /*!
   * \brief Check if the given SOME/IP packet from the given local sender shall be forwarded or not
   *        (Identity and Access Management).
   *
   * \param[in] local_source_identifier SOME/IP message's source identifier.
   * \param[in] header SOME/IP packet header to be checked.
   *
   *
   * \steady  TRUE
   * \return "true" if the access check passes, "false" otherwise.
   *
   * \trace SPEC-4663370
   *
   * \internal
   * - If the message type is "request" or "request-no-return/fire-and-forget"
   *   - Set the return value as the result of access check of the method
   * - Else if the message type is "notification"
   *   - Set the return value as the result of access check of the event
   * - Else
   *   - Set the return value to indicate allowed access
   * - Return the return value
   * \endinternal
   */
  bool CheckAccess(amsr::ipc::Credentials const local_source_identifier,
                   someip_protocol::internal::SomeIpMessageHeader const& header,
                   someip_protocol::internal::InstanceId const instance_id) const {
    bool result{true};

    someip_protocol::internal::SomeIpMessageType const msg_type{header.message_type_};
    someip_protocol::internal::ServiceId const service_id{header.service_id_};
    someip_protocol::internal::MajorVersion const major_version{header.interface_version_};

    switch (msg_type) {
      case someip_protocol::internal::SomeIpMessageType::kRequest:
      case someip_protocol::internal::SomeIpMessageType::kRequestNoReturn: {
        someip_protocol::internal::MethodId const method_id{header.method_id_};
        result = identity_access_manager_.CheckMethod(service_id, major_version, instance_id, method_id,
                                                      local_source_identifier);
        break;
      }
      case someip_protocol::internal::SomeIpMessageType::kNotification: {
        someip_protocol::internal::EventId const event_id{header.method_id_};
        result = identity_access_manager_.CheckEventSubscription(service_id, major_version, instance_id, event_id,
                                                                 local_source_identifier);
        break;
      }
      default: {
        result = true;
        break;
      }
    }

    return result;
  }

 private:
  /*!
   * \brief Reference to the identity access manager.
   */
  iam::IamInterface& identity_access_manager_;
};

}  // namespace validators
}  // namespace packet_handler
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_HANDLER_VALIDATORS_SECURITY_VALIDATOR_H_

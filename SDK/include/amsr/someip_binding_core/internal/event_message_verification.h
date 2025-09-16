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
/**        \file  amsr/someip_binding_core/internal/event_message_verification.h
 *        \brief  Event Message Verification.
 *        \unit   SomeIpBinding::SomeIpBindingCore::SomeIpTypes
 *
 *      \details  This file provides the SOME/IP protocol type and constant definitions.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENT_MESSAGE_VERIFICATION_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENT_MESSAGE_VERIFICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/communication_error_domain.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Class for verification of received event messages.
 *
 * \trace SPEC-4980078
 * \trace SPEC-4980113
 */
class EventMessageVerification final {
 public:
  EventMessageVerification() noexcept = delete;
  ~EventMessageVerification() noexcept = delete;
  EventMessageVerification(EventMessageVerification const&) = delete;
  EventMessageVerification(EventMessageVerification&&) = delete;
  EventMessageVerification& operator=(EventMessageVerification const&) & = delete;
  EventMessageVerification& operator=(EventMessageVerification&&) & = delete;

  /*!
   * \brief Do infrastructural checks that do not have any dependencies to the deployment configuration.
   * \param[in] header SOME/IP header from the request.
   * \return return code E_OK if all infrastructural checks were successful - specific error return code
   *         if one of the following checks could not be verified successfully.
   * \error  CommunicationErrorDomain::Errc::wrong_protocol_version If wrong protocol version is received.
   * \error  CommunicationErrorDomain::Errc::payload_length_too_short If payload length is shorter than expected payload
   *         is received.
   * \error  CommunicationErrorDomain::Errc::wrong_return_code If wrong return code is received.
   * \pre -
   * \context     Reactor|App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace       SPEC-4980078
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::Result<void> DoInfrastructuralChecks(
      amsr::someip_protocol::internal::SomeIpMessageHeader const& header) noexcept;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENT_MESSAGE_VERIFICATION_H_

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *         \brief This file contains the declaration of HandshakeState class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_H_

#include <cstdint>
#include <functional>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/handshakestatemachine/handshake_state_machine_interface.h"
#include "tls/internal/messages/message_manager.h"
#include "tls/internal/record_protocol/record_protocol_interfaces.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

/*!
 * \vprivate This class is component private.
 * \brief Class describing a state in the Client or Server handshake state machine.
 * \details Each state is responsible for reacting to different handshake, alert or change-cipher messages.
 *          They also often have timeouts if a certain message isn't received in time.
 */
class HandshakeState {
 public:
  /*!
   * \brief Constructor.
   * \param[in] handshake_state_handle The HandshakeStateHandle for this state.
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  HandshakeState(HandshakeStateHandle handshake_state_handle, HandshakeStateMachineInterface& ref_to_hsm,
                 messages::MessageManager& ref_to_msg_mngr,
                 record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  HandshakeState(HandshakeState const&) = delete;
  HandshakeState(HandshakeState&&) = delete;
  HandshakeState& operator=(HandshakeState const&) = delete;
  HandshakeState& operator=(HandshakeState&&) = delete;

  /*!
   * \brief Destructor.
   */
  virtual ~HandshakeState() = default;

  /*!
   * \brief Makes the HandshakeState instance ready for use. It is called when fetched from the pool.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   *                This parameter includes two types of information.
   *                1 - Traceability data which shall be set by the caller and used for internal logging activities
   *                during the execution of a flow and
   *                2 - Error information that shall be set in case of an error occurring during the flow.
   *                If the caller is interested in the error state after the call, the caller should check for errors
   *                using the "has_error" variable. See "TlsCommFlowContext" for more information.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void OpenForAction(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Cleans the objects internal data.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void Cleanup() noexcept;

  /*!
   * \brief Function executed upon entering the state
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void OnEnter(common::TlsCommFlowContext& cxt) noexcept = 0;

  /*!
   * \brief Function executed right before exiting the state.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void OnExit() noexcept;

  /*!
   * \brief Called when there is a new handshake message received from network.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer Received handshake data including the handshake header as bytes, so the first byte should be the
   *            HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6561862, SPEC-6561860
   */
  virtual void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer,
                                          bool retransmit) noexcept = 0;

  /*!
   * \brief Called when a ChangeCipherSpec message is received from the network.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  ChangeCipherSpec message, which is only one byte of value 1.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void OnChangeCipherMessageReceived(common::TlsCommFlowContext& cxt,
                                             ara::core::Vector<std::uint8_t> const& buffer) noexcept = 0;

  /*!
   * \brief This operation is called to start the handshake procedure.
   * \details This operation should be called only on a client provider and only in the state ClientStateDisconnected.
   *          Otherwise, the operation will return with an error. To be able to repeat a call to InitiateHandshake(),
   *          the provider must be transitioned to ClientStateDisconnected by calling the operation CloseConnection().
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error "TlsCommErrorCode::kTlsErrInvalidState" is reported if a call to "InitiateHandshake" occurs in any state
   *        other than "ClientStateDisconnected".
   * \vprivate This method is component private.
   */
  virtual void InitiateHandshake(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Callback that is activated when the timer expires.
   * \details This callback can indicate whether the timer should be scheduled again. Report an error if it is called
   *          in any of the states ClientStateConnected, ClientStateDisconnected or ServerStateDisconnected.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error "TlsCommErrorCode::kTlsErrInvalidState" is reported if a call to "InitiateHandshake" occurs in any of the
   *        "ClientStateConnected", "ClientStateDisconnected" or "ServerStateDisconnected" states.
   * \trace SPEC-6562048
   */
  virtual void OnTimerEvent(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Called when an alert message is received from the network.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting.
   * \param[in] buffer Alert message; should be two bytes long.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6561841, SPEC-6561843, SPEC-6561845, SPEC-6561838, SPEC-6561843, SPEC-6561828, SPEC-6561845
   */
  void OnAlertMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief Updates the wanted state and returns true if this state wants the state machine to change into another
   *        state.
   * \param[out] wanted_state The state to change into will be returned in this reference
   * \return True if this state wants the state machine to change state. Otherwise, false.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  bool IsStateChangeWanted(HandshakeStateHandle& wanted_state) const noexcept;

  /*!
   * \brief Returns the current HandshakeStateHandle.
   * \return HandshakeStateHandle
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  HandshakeStateHandle GetHandshakeStateHandle() const noexcept;

  /*!
   * \brief Sends a flight (a series of messages) of Handshake messages.
   * \pre The messages should be prepared before sending
   * \param[in, out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] flight Container with sequenced handshake messages for sending
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void SendFlight(common::TlsCommFlowContext& cxt,
                  ara::core::Vector<std::reference_wrapper<messages::HandshakeMessage>> const& flight) noexcept;

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_libseccom_V11.0.2_mutableMemberShallBePrivate
  /*!
   * \brief The HandshakeStateHandle for this state.
   */
  HandshakeStateHandle const handshake_state_handle_;

  /*!
   * \brief The wanted state to change to.
   */
  HandshakeStateHandle wanted_state_;

  /*!
   * \brief Reference to the state machine this state is part of.
   */
  HandshakeStateMachineInterface& ref_handshake_state_machine_;

  /*!
   * \brief Reference to the message manager containing all handshake message objects used by this state.
   */
  messages::MessageManager& ref_message_manager_;

  /*!
   * \brief Reference to record protocol used for sending messages.
   */
  record_protocol::RecordProtocolInterface& ref_record_protocol_;

  /*!
   * \brief Timeout in milliseconds before going to fatal error state (TLS) or resending last messages (DTLS).
   */
  std::uint32_t time_out_{0U};

  /*!
   * \brief Number of timeouts before going to fatal error. Only DTLS.
   */
  std::uint32_t dtls_timeouts_left_{0U};
  // VECTOR Enable VectorC++-V11.0.2

 private:
  /*!
   * \brief Check if a numerical value belongs to the enumeration value representing different alert descriptions or
   *        not.
   * \param[in] value Numerical value to check.
   * \return "true" if the value belongs to the enumeration value and "false" otherwise.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  static bool IsDescriptionValid(std::uint8_t const value);
};

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_H_

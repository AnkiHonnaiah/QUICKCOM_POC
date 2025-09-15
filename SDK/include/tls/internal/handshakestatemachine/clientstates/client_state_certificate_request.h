/*!********************************************************************************************************************
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
/*!        \file  client_state_certificate_request.h
 *         \brief Provides the definition of the ClientStateCertificateRequest class.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CERTIFICATE_REQUEST_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CERTIFICATE_REQUEST_H_

#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*!
 * \brief Represents the client state after receiving a CertificateRequest message.
 */
class ClientStateCertificateRequest final : public HandshakeState {
 public:
  /*!
   * \brief     Creates a new state instance.
   *
   * \param[in] handshake_state_machine The associated state machine.
   * \param[in] message_manager         The associated message manager.
   * \param[in] record_protocol         The associated record protocol.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  ClientStateCertificateRequest(HandshakeStateMachineInterface& handshake_state_machine,
                                messages::MessageManager& message_manager,
                                record_protocol::RecordProtocolInterface& record_protocol) noexcept;

  ClientStateCertificateRequest(ClientStateCertificateRequest const&) noexcept = delete;

  ClientStateCertificateRequest(ClientStateCertificateRequest&&) noexcept = delete;

  auto operator=(ClientStateCertificateRequest const&) noexcept -> ClientStateCertificateRequest& = delete;

  auto operator=(ClientStateCertificateRequest&&) noexcept -> ClientStateCertificateRequest& = delete;

  /*!
   * \brief     Destroys the state.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  ~ClientStateCertificateRequest() noexcept final = default;

  /*!
   * \copydoc amsr::tls::internal::handshakestatemachine::HandshakeState::OnEnter
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \copydoc amsr::tls::internal::handshakestatemachine::HandshakeState::OnHandshakeMessageReceived
   */
  void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t>& buffer,
                                  bool retransmit) noexcept final;

  /*!
   * \copydoc amsr::tls::internal::handshakestatemachine::HandshakeState::OnTimerEvent
   */
  void OnTimerEvent(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \copydoc amsr::tls::internal::handshakestatemachine::HandshakeState::OnChangeCipherMessageReceived
   */
  void OnChangeCipherMessageReceived(common::TlsCommFlowContext& cxt,
                                     ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

 private:
  /*!
   * \brief Indicates whether messages have been retransmitted during the current timeout period.
   */
  bool did_retransmit_{false};

  /**
   * \brief         Retransmits messages by re-sending the client hello message.
   *
   * \param[in,out] cxt The flow context to use for error reporting.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void Retransmit(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Used to access non-public data members.
   */
  FRIEND_TEST(Cdd__ClientStateCertificateRequest, OnTimerEvent);

  /*!
   * \brief Used to access non-public data members.
   */
  FRIEND_TEST(UT__ClientHandshakeStatePool__ClientStateCertificateRequest, OnTimerEvent);
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CERTIFICATE_REQUEST_H_

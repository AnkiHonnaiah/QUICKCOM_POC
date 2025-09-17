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
/*!        \file
 *         \brief Provides the definition of the ClientStateServerKeyExchange class.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_KEY_EXCHANGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_KEY_EXCHANGE_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*!
 * \brief Represents the client state after ClientStateServerHello if a ServerKeyExchange message was received.
 */
class ClientStateServerKeyExchange final : public HandshakeState {
 public:
  /*!
   * \brief     Creates a state.
   *
   * \param[in] handshake_state_machine The associated handshake state machine.
   * \param[in] message_manager         The associated message manager.
   * \param[in] record_protocol         The associated record protocol.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  ClientStateServerKeyExchange(HandshakeStateMachineInterface& handshake_state_machine,
                               messages::MessageManager& message_manager,
                               record_protocol::RecordProtocolInterface& record_protocol) noexcept;

  ClientStateServerKeyExchange(ClientStateServerKeyExchange const&) noexcept = delete;

  ClientStateServerKeyExchange(ClientStateServerKeyExchange&&) noexcept = delete;

  auto operator=(ClientStateServerKeyExchange const&) noexcept -> ClientStateServerKeyExchange& = delete;

  auto operator=(ClientStateServerKeyExchange&&) noexcept -> ClientStateServerKeyExchange& = delete;

  /*!
   * \brief     Destroys the state.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  ~ClientStateServerKeyExchange() final = default;

  /*!
   * \copydoc amsr::tls::internal::handshakestatemachine::HandshakeState::OnEnter
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Process the received message and prepare configuration for next state ClientStateServerHelloDone.
   * \details Retransmit the ClientHello message if retransmission is triggered. Otherwise deserialize the received
   * message and prepare the configuration for transition to ClientStateServerHelloDone. If any error happens during
   * this process or an unexpected message is received, it will report the respective error.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
   * \trace SPEC-6561862, SPEC-6561860, SPEC-6561864
   */
  void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t>& buffer,
                                  bool retransmit) noexcept final;

  /*!
   * \brief Called when a ChangeCipherSpec message is received from the network.
   * \details Report an error if the message is received while the protocol is TLS
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  ChangeCipherSpec message, which is only one byte of value 1.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if Unexpected handshake message received
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnChangeCipherMessageReceived(common::TlsCommFlowContext& cxt,
                                     ara::core::Vector<uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Callback that is activated when the timer expires.
   * \details Retransmit the flight of this state and consume one attempt of remaining retransmission-when-timout
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \error TlsCommErrorCode::kTlsAlertHandshakeFailure is reported when handshake in Tls mode fails for timeout
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnTimerEvent(common::TlsCommFlowContext& cxt) noexcept final;

 private:
  /*!
   * \brief Retransmit flight
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre cxt has no previous error information
   * \reentrant FALSE
   * \trace SPEC-6562008
   */
  void RetransmitFlight(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief         Processes a received ServerHelloDone message.
   *
   * \param[in,out] cxt    The flow context to use for error reporting.
   * \param[in]     buffer The serialized message buffer.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void OnServerHelloDoneMessageReceived(common::TlsCommFlowContext& cxt,
                                        ara::core::Vector<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief         Processes a received CertificateRequest message.
   *
   * \param[in,out] cxt    The flow context to use for error reporting.
   * \param[in]     buffer The serialized message buffer.
   *
   * \pre           -
   * \context       ANY
   * \reentrant     FALSE
   */
  void OnCertificateRequestMessageReceived(common::TlsCommFlowContext& cxt,
                                           ara::core::Vector<std::uint8_t> const& buffer) noexcept;

  /*!
   * \brief Used to ensure that we only retransmit once per timeout.
   */
  bool only_once_{true};

  /*!
   * \brief Used to access non-public data members.
   */
  FRIEND_TEST(Cdd__ClientStateServerKeyExchange, OnTimerEvent);

  /*!
   * \brief Used to access non-public data members.
   */
  FRIEND_TEST(UT__ClientHandshakeStatePool_HandshakeStateMachine__ClientStateServerKeyExchange, OnTimerEvent);
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_KEY_EXCHANGE_H_

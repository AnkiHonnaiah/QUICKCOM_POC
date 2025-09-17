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
 *         \brief   This file contains the declaration for ClientStateServerHello class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_HELLO_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_HELLO_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/handshake_state.h"
#include "tls/internal/handshakestatemachine/handshake_state_machine_interface.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*!
 * \brief State entered after ClientStateClientHello if a ServerHello message is received from the server.
 * \vprivate This class is component private.
 */
class ClientStateServerHello final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ClientStateServerHello
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  ClientStateServerHello(HandshakeStateMachineInterface& ref_to_hsm, messages::MessageManager& ref_to_msg_mngr,
                         record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor */
  ~ClientStateServerHello() final = default;

  ClientStateServerHello(ClientStateServerHello const&) = delete;
  ClientStateServerHello(ClientStateServerHello&&) = delete;
  ClientStateServerHello& operator=(ClientStateServerHello const&) = delete;
  ClientStateServerHello& operator=(ClientStateServerHello&&) = delete;

  /*!
   * \brief Function executed upon entering the state ClientStateServerHello
   * \details Set and start the timer for this state. The timer handles retransmission, if it times out the last flight
   * of messages will be re-transmitted.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and
   * error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Process the received message and prepare configuration for next state
   * \details Deserialize the received message. Then if it was a ServerKeyExchange message, prepare the configuration
   * for the state ClientStateServerKeyExchange. If it was a ServerHelloDone message, prepare it for the next state
   * ClientStateServerHelloDone.
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
   * \param[in, out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre cxt has no previous error information
   * \reentrant FALSE
   * \trace SPEC-6562008
   */
  void RetransmitFlight(common::TlsCommFlowContext& cxt) noexcept;

  /*! \brief Boolean variable that keeps track so we only retransmit once. */
  bool only_once_{true};

  /*!
   * \brief Friend test for internal members' access (test2017).
   */
  FRIEND_TEST(Cdd__TlsClientStateServerHello_WithHandshakeStateMachineMock,
              Server_Certificate_Message_With_Multiple_Labels_Success);

  /*!
   * \brief Friend test for internal members' access (test2017).
   */
  FRIEND_TEST(Cdd__TlsClientStateServerHello_WithHandshakeStateMachineMock,
              Server_Certificate_Message_With_Multiple_Labels_Failure);

  /*!
   * \brief Friend test for internal members' access (test2020).
   */
  FRIEND_TEST(UT__ClientHandshakeStatePool_TlsClientStateServerHello_WithHandshakeStateMachineMock,
              Server_Certificate_Message_With_Multiple_Labels_Success);

  /*!
   * \brief Friend test for internal members' access (test2020).
   */
  FRIEND_TEST(UT__ClientHandshakeStatePool_TlsClientStateServerHello_WithHandshakeStateMachineMock,
              Server_Certificate_Message_With_Multiple_Labels_Failure);
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_HELLO_H_

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
 *        \brief  This is the header file for the server state of client change cipher spec
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CLIENT_CHANGE_CIPHER_SPEC_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CLIENT_CHANGE_CIPHER_SPEC_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/handshake_state.h"
#include "tls/internal/handshakestatemachine/handshake_state_machine_interface.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace serverstates {

/*!
 * \brief State entered after ServerStateClientKeyExchange if a ChangeCipherSpec message is received from the server.
 * \vprivate This class is component private.
 * \trace SPEC-6561829
 */
class ServerStateClientChangeCipherSpec final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ServerStateClientChangeCipherSpec
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \reentrant FALSE
   */
  ServerStateClientChangeCipherSpec(HandshakeStateMachineInterface& ref_to_hsm,
                                    messages::MessageManager& ref_to_msg_mngr,
                                    record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;
  /*! \brief Default destructor */
  ~ServerStateClientChangeCipherSpec() final = default;

  ServerStateClientChangeCipherSpec(ServerStateClientChangeCipherSpec const&) = delete;
  ServerStateClientChangeCipherSpec(ServerStateClientChangeCipherSpec&&) = delete;
  ServerStateClientChangeCipherSpec& operator=(ServerStateClientChangeCipherSpec const&) = delete;
  ServerStateClientChangeCipherSpec& operator=(ServerStateClientChangeCipherSpec&&) = delete;

  /*!
   * \brief Function executed upon entering the state ServerStateClientChangeCipherSpec
   * \details Fetch the read security parameter, then set and start the timer for this state. The timer handles
   * retransmission, if it times out the last flight of messages will be re-transmitted.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Process the received message and prepare configuration for next state ServerStateConnected.
   * \details Retransmit the ServerHello, ServerKeyExchange and ServerHelloDone messages if retransmission is triggered
   * Otherwise deserialize the received message and prepare the configuration for transition to ServerStateConnected. If
   * any error happens during this process or an unexpected message is received, it will report the respective error.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if empty handshake message received
   * \trace SPEC-6561860
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
  /*! \brief Boolean variable that keeps track so we only retransmit once. */
  bool only_once_{true};

  /*! \brief Friend test declaration for OnTimerEvent. */
  FRIEND_TEST(Cdd__handshakestatemachine_serverstateclientchangecipherspec_dtls, OnTimerEvent);

  /*! \brief Friend test declaration for OnTimerEvent. */
  FRIEND_TEST(UT__ServerHandshakeStatePool_ServerStateClientChangeCipherSpec_Dtls, OnTimerEvent);
};

}  // namespace serverstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CLIENT_CHANGE_CIPHER_SPEC_H_

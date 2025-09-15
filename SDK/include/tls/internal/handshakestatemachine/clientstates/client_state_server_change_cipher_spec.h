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
 *         \brief This file contains the declaration for ClientStateServerChangeCipherSpec class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_CHANGE_CIPHER_SPEC_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_CHANGE_CIPHER_SPEC_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*!
 * \brief State entered after ClientStateServerHelloDone when a ChangeCipherSpec message is received from the server. On
 * enter it calls the record protocol with new security parameters used for reception of data.
 * \vprivate This class is component private.
 * \trace SPEC-6561829
 */
class ClientStateServerChangeCipherSpec final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ClientStateServerChangeCipherSpec
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  ClientStateServerChangeCipherSpec(HandshakeStateMachineInterface& ref_to_hsm,
                                    messages::MessageManager& ref_to_msg_mngr,
                                    record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor. */
  ~ClientStateServerChangeCipherSpec() final = default;

  ClientStateServerChangeCipherSpec(ClientStateServerChangeCipherSpec const&) = delete;
  ClientStateServerChangeCipherSpec(ClientStateServerChangeCipherSpec&&) = delete;
  ClientStateServerChangeCipherSpec& operator=(ClientStateServerChangeCipherSpec const&) = delete;
  ClientStateServerChangeCipherSpec& operator=(ClientStateServerChangeCipherSpec&&) = delete;

  /*!
   * \brief Function executed upon entering the state ClientStateServerChangeCipherSpec
   * \details Set the read security parameter then set and start the timer for this state. The timer handles
   * retransmission, if it times out the last flight of messages will be re-transmitted.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Process the received message and prepare configuration for next state ClientStateConnected
   * \details Retransmit the ClientKeyExchange, ClientChangeCipherSpec and ClientFinished messages if retransmission is
   * triggered. Otherwise verify the received message and prepare the configuration for transition to
   * ClientStateConnected. If any error happens during this process or an unexpected message is received, it will
   * report the respective error.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
   * \error TlsCommErrorCode::kTlsAlertDecryptError is reported if decrypt fails.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6561862, SPEC-6561860, SPEC-6561864
   * \vprivate This method is component private.
   */
  void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t>& buffer,
                                  bool retransmit) noexcept final;

  /*!
   * \brief Called when a ChangeCipherSpec message is received from the network.
   * \details Report an error if the message is received while the protocol is TLS
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  ChangeCipherSpec message, which is only one byte of value 1.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
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

  /*! \brief Boolean variable that keeps track so we only retransmit once. */
  bool only_once_{true};

  /*! \brief Friend test declarations */
  FRIEND_TEST(Cdd__DTLSClientStateServerChangeCipherSpec, OnTimerEvent_DTLS);

  /*! \brief Friend test declarations */
  FRIEND_TEST(UT__ClientHandshakeStatePool_DtlsClientStateServerChangeCipherSpec, OnTimerEvent_DTLS);
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_CHANGE_CIPHER_SPEC_H_

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
 *         \brief This file constains the declaration for ClientStateServerHelloDone class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_HELLO_DONE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_HELLO_DONE_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*!
 * \brief State entered after the ClientStateServerKeyExchange if a ServerHelloDone message is received from the server.
 * \vprivate This class is component private.
 */
class ClientStateServerHelloDone final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ClientStateServerHelloDone
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  ClientStateServerHelloDone(HandshakeStateMachineInterface& ref_to_hsm, messages::MessageManager& ref_to_msg_mngr,
                             record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  ClientStateServerHelloDone(ClientStateServerHelloDone const&) noexcept = delete;

  ClientStateServerHelloDone(ClientStateServerHelloDone&&) noexcept = delete;

  auto operator=(ClientStateServerHelloDone const&) & noexcept -> ClientStateServerHelloDone& = delete;

  auto operator=(ClientStateServerHelloDone&&) & noexcept -> ClientStateServerHelloDone& = delete;

  /*! \brief Default destructor */
  ~ClientStateServerHelloDone() noexcept final = default;

  /*!
   * \brief Function executed upon entering the state ClientStateServerHelloDone
   * \details Prepare and send ClientKeyExchange message and then ClientChangeCipherSpec message. If sending those
   * messages is successful, then it will send ClientFinished message. Set and start the timer for this state. The timer
   * handles retransmission, if it times out the last flight of messages will be re-transmitted.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-5506681, SPEC-5506685, SPEC-6562008
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Retransmit handshake messages if retransmission is triggered
   * \details Retransmit the ClientKeyExchange, ClientChangeCipherSpec and ClientFinished messages if retransmission
   * is triggered. Otherwise if an unexpected message is received, it will report the respective error.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
   * \trace SPEC-6561862, SPEC-6561860, SPEC-6561864, SPEC-6561971
   */
  void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t>& buffer,
                                  bool retransmit) noexcept final;

  /*!
   * \brief Called when a ChangeCipherSpec message is received from the network.
   * \details Process the received message and prepare configuration for transition to state
   * ClientStateServerChangeCipherSpec
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  ChangeCipherSpec message, which is only one byte of value 1.
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
   * \trace SPEC-6562008
   * \vprivate This method is component private.
   */
  void OnTimerEvent(common::TlsCommFlowContext& cxt) noexcept final;

 private:
  /*!
   * \brief         Sends the client "Certificate" message.
   *
   * \param[in,out] cxt The flow context used for error reporting.
   */
  void SendCertificateMessage(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief         Sends the "ClientKeyExchange" message.
   *
   * \param[in,out] cxt The flow context used for error reporting.
   */
  void SendClientKeyExchangeMessage(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief         Sends the "CertificateVerify" message.
   *
   * \param[in,out] cxt The flow context used for error reporting.
   */
  void SendCertificateVerifyMessage(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief         Sends the "ChangeCipherSpec" message.
   *
   * \param[in,out] cxt The flow context used for error reporting.
   */
  void SendChangeCipherSpecMessage(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Prepares the client finished message and sends it upon successful creation of the verification data
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void SendFinishedMessage(common::TlsCommFlowContext& cxt) noexcept;

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

  /*! \brief Used to access non-public data members. */
  FRIEND_TEST(Cdd__ClientStateServerHelloDoneDtls, OnTimerEventDtls);

  /*! \brief Friend test declaration for OnEnter */
  FRIEND_TEST(Cdd__TlsClientStateServerHelloDone, OnEnter);

  /*! \brief Friend test declaration for OnTimerEventDtls */
  FRIEND_TEST(UT__ClientHandshakeStatePool_DtlsClientStateServerHelloDone, OnTimerEventDtls);

  /*! \brief Friend test declaration for OnEnter */
  FRIEND_TEST(UT__ClientHandshakeStatePool_TlsClientStateServerHelloDone, OnEnter);
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_HELLO_DONE_H_

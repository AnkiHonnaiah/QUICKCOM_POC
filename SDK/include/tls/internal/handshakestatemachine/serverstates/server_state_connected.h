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
 *        \brief  This file contains the declaration for ServerStateConnected class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CONNECTED_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CONNECTED_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace serverstates {

/*!
 * \brief Server entered after ServerStateClientChangeCipherSpec if a Finished message is received from the Client
 * \vprivate This class is component private.
 */
class ServerStateConnected final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ServerStateConnected
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \reentrant FALSE
   */
  ServerStateConnected(HandshakeStateMachineInterface& ref_to_hsm, messages::MessageManager& ref_to_msg_mngr,
                       record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor */
  ~ServerStateConnected() final = default;

  ServerStateConnected(ServerStateConnected const&) = delete;
  ServerStateConnected(ServerStateConnected&&) = delete;
  ServerStateConnected& operator=(ServerStateConnected const&) = delete;
  ServerStateConnected& operator=(ServerStateConnected&&) = delete;

  /*!
   * \brief Make both the ServerStateConnected instance ready for communication
   * \details This operation must be called before any other operation belonging to the instance is used.
   * Will make necessary initiations needed for its functionality. It will also call corresponding operations in objects
   * contained by this object.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OpenForAction(common::TlsCommFlowContext&) noexcept final;

  /*!
   * \brief Function executed upon entering the state ServerStateConnected
   * \details Send the ChangeCipherSpec message to client, then Set the write security parameter and send the
   * ServerFinished message. If it is successful then set and start the timer for this state. When the timer times out,
   * it resets the handshake message aggregator to be ready for reconnecting
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6562788, SPEC-6562084, SPEC-6562008
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Callback that is activated when the timer expires.
   * \details Reset the message aggregator and handshake configuration to be ready for new handshake request from a
   * client
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6562084
   * \vprivate This method is component private.
   */
  void OnTimerEvent(common::TlsCommFlowContext& cxt) noexcept final;

  // VECTOR Enable AutosarC++17_10-M7.1.2
  /*!
   * \brief Process the received message and continue the handshake process based on received message.
   * \details Retransmit the ClientChangeCipherSpec and ServerFinished messages if retransmission is triggered.
   * Otherwise complete the handshake process by sending the last ChangeCipherSpec and then ServerFinished message
   * upon receiving ClientFinished message. Call the HandleClientHellomessage() if the received message is ClientHello
   * message. If any error happens during this process or an unexpected message is received, it will report the
   * respective error.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \error TlsCommErrorCode::kTlsRenegotiationRejected is reported if renegotiation is not supported
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
   * \trace SPEC-6561860, SPEC-6562084
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
   * \brief Function executed right before exiting the state.
   * \details Notify the record layer to disconnect
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnExit() noexcept final;

 private:
  /*! \brief Time in milli seconds for DTLS connected state to stay active */
  uint32_t dtls_connected_state_active_timer_{0U};

  /*! \brief Indicates if the DTLS connected state still active or not */
  bool dtls_connected_state_active_{true};

 private:
  /*!
   * \brief Deserializes received ClientHello message and processes it
   * \details This function will only process ClientHello messages from received epoch 0 records. This indicates
   * that the client is either reconnecting or a new client. In either way the client must demonstrate reachability
   * by completing a cookie verification step.
   * \pre May only be called if cookie verification is enabled on a DTLS server
   * \param[in, out] cxt TlsCommFlowContext used for storing error information
   * \param[in] buffer Serialized byte buffer containing the client hello message
   * \context ANY
   * \reentrant FALSE
   */
  void HandleClientHelloMessage(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t> const& buffer) noexcept;

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

  /*! \brief Private friend test declaration for OnTimerEvent */
  FRIEND_TEST(Cdd__DTLSServerStateConnected, OnTimerEvent);

  /*! \brief Private friend test declaration for OnHandshakeMessageReceived */
  FRIEND_TEST(Cdd__DTLSServerStateConnected, OnHandshakeMessageReceived);

  /*! \brief Private friend test declaration for HandleClientHelloMessage */
  FRIEND_TEST(Cdd__DTLSServerStateConnected, HandleClientHelloMessage);

  /*! \brief Private friend test declaration for OnTimerEvent */
  FRIEND_TEST(UT__ServerHandshakeStatePool_DtlsServerStateConnected, OnTimerEvent);

  /*! \brief Private friend test declaration for OnHandshakeMessageReceived */
  FRIEND_TEST(UT__ServerHandshakeStatePool_DtlsServerStateConnected, OnHandshakeMessageReceived);

  /*! \brief Private friend test declaration for DTlsMbedClientReconnectWithCookieSendMultipleApp */
  FRIEND_TEST(UT__ServerHandshakeStatePool_DtlsServerStateConnected, HandleClientHelloMessage);

  /*! \brief Private friend test declaration for DTlsMbedClientReconnectWithCookieSendMultipleApp */
  FRIEND_TEST(Creq__IntegrationTestFixture, DTlsMbedClientReconnectWithCookieSendMultipleApp);
};

}  // namespace serverstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CONNECTED_H_

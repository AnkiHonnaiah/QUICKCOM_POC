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
/*!         \file
 *          \brief      The file constains the ServerStateClientHello class declaration
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CLIENT_HELLO_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CLIENT_HELLO_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/handshake_state.h"
#include "tls/internal/handshakestatemachine/handshake_state_machine_interface.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace serverstates {

/*!
 * \brief State entered after ServerStateDisconnected if a ClientHello message is received from the client.
 * \vprivate This class is component private.
 * \trace DSGN-SecCom-CipherService
 */
class ServerStateClientHello final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ServerStateClientHello
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \reentrant FALSE
   */
  ServerStateClientHello(HandshakeStateMachineInterface& ref_to_hsm, messages::MessageManager& ref_to_msg_mngr,
                         record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor */
  ~ServerStateClientHello() final = default;

  ServerStateClientHello(ServerStateClientHello const&) = delete;
  ServerStateClientHello(ServerStateClientHello&&) = delete;
  ServerStateClientHello& operator=(ServerStateClientHello const&) = delete;
  ServerStateClientHello& operator=(ServerStateClientHello&&) = delete;

  /*!
   * \brief This operation will close down the HandshakeState instance and return dynamic resources.
   * \details After the call to this operation, the object shall not be used. It will be returned to the pool as part of
   * the structure. The operation will call corresponding operations on objects contained by this instance. The purpose
   * of the operation is to return all allocated resources, clear relevant variables and return to a state which will
   * allow a call to OpenForAction().
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void CloseDown() noexcept;

  /*!
   * \brief Function executed upon entering the state ServerStateClientHello
   * \details Prepare and send ServerHello message then set and start timer for this state. The timer handles
   * retransmission, if it times out the last flight of messages will be re-transmitted.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsCryptoAdapterFailure is reported if creating random number generator fails
   * \error TlsCommErrorCode::kTlsAlertHandshakeFailure  is reported if could not find a cipher suite that match
   * \trace SPEC-6561872, SPEC-6561999, SPEC-5506675, SPEC-5506681, SPEC-6562035
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Process the received message and prepare configuration for next state ServerStateClientKeyExchange.
   * \details Retransmit the ServerHello, ServerKeyExchange and ServerHelloDone messages if retransmission is triggered
   * .Otherwise verify and deserialize the received message and prepare the configuration for transition to
   * ServerStateClientKeyExchange. If any error happens during this process or an unexpected message is received, it
   * will report the respective error.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \error TlsCommErrorCode::kTlsAlertHandshakeFailure is reported if handshake fails
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
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
   * \brief Sets the supported cipher suites in order of priority.
   * \param[in] cipher_suites_to_use List of accepted cipher suites to use.
   * \error TlsCommErrorCode::kTlsErrInternal is reported if setting the supported cipher suite fails
   * \vprivate This method is component private.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  void SetSupportedCipherSuites(ListOfCipherSuites const& cipher_suites_to_use) noexcept;

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
  /*! \brief The list of cipher suites to be used. */
  ListOfCipherSuites cipher_suites_to_use_{};

  /*!
   * \brief The last sent flight of handshake messages.
   */
  ara::core::Vector<std::reference_wrapper<messages::HandshakeMessage>> last_flight{};

  /*! \brief Boolean variable that keeps track so we only retransmit once. */
  bool only_once_{true};

  /*! \brief Friend test declaration for OnTimerEvent_DTLS. */
  FRIEND_TEST(Cdd__DTLSServerStateClientHello, OnTimerEvent_DTLS);

  /*! \brief Friend test declaration for OnTimerEvent_DTLS. */
  FRIEND_TEST(UT__ServerHandshakeStatePool_DtlsServerStateClientHello, OnTimerEvent_DTLS);

  /*! \brief Friend test declaration for ServerPoolCloseDown. */
  FRIEND_TEST(Cdd__HandshakeStatePool, ServerPoolCloseDown);

  /*! \brief Friend test declaration for ServerPoolCloseDown. */
  FRIEND_TEST(UT__ServerHandshakeStatePool, ServerPoolCloseDown);
};

}  // namespace serverstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_CLIENT_HELLO_H_

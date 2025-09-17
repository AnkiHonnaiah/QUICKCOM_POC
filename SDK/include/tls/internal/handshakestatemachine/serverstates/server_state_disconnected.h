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
 *        \brief  This file contains declaration for ServerStateDisconnected class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_DISCONNECTED_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_DISCONNECTED_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace serverstates {

/*!
 * \brief Initial state. Also transits to this state after a Disconnect() call and from fatal error state.
 * \vprivate This class is component private.
 */
class ServerStateDisconnected final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ServerStateDisconnected
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \reentrant FALSE
   */
  ServerStateDisconnected(HandshakeStateMachineInterface& ref_to_hsm, messages::MessageManager& ref_to_msg_mngr,
                          record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor */
  ~ServerStateDisconnected() final = default;

  ServerStateDisconnected(ServerStateDisconnected const&) = delete;
  ServerStateDisconnected(ServerStateDisconnected&&) = delete;
  ServerStateDisconnected& operator=(ServerStateDisconnected const&) = delete;
  ServerStateDisconnected& operator=(ServerStateDisconnected&&) = delete;

  /*!
   * \brief Called when the received ClientHelloMessage is deserialized successfully.
   * \details Handles deserialized ClientHelloMessage data and may reply with HelloVerifyRequest messages if cookie
   * verification is enabled (DTLS).
   * \param[in,out] cxt TlsCommFlowContext to log contextual information
   * \context ANY
   * \pre -
   * \error TlsCommErrorCode::kTlsAlertHandshakeFailure is reported if clientHello message contains wrong compression
   * method
   * \reentrant FALSE
   * \trace SPEC-8799957
   * \trace SPEC-6181118
   * \vprivate This method is component private.
   * \threadsafe FALSE
   */
  void HandleClientHelloMessage(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Report and log an error that the received ClientHelloMessage is not supported.
   * \param[out] cxt TlsCommFlowContext to log contextual information
   * \param[in] is_compression_methods_empty Boolean flag indicates if there is a compression method in the message.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \threadsafe FALSE
   */
  void LogInvalidMessageError(bool const is_compression_methods_empty, common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Prepare and send the alert message
   * \param[in] client_hello_version version of ClientHelloMessage
   * \param[in] server_version_tls Shows type of communication TLS/DTLS
   * \param[in] cxt TlsCommFlowContext to log contextual information
   * \context ANY
   * \pre -
   * \error TlsCommErrorCode::kTlsAlertHandshakeFailure is reported if clientHello message contains wrong compression
   * method.
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \threadsafe FALSE
   */
  void PrepareAndSendAlertMessage(record_protocol::ProtocolVersion const client_hello_version,
                                  record_protocol::ProtocolVersion const server_version_tls,
                                  common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Provide cookies, prepare and send HelloVerifyRequest message
   * \param[in] cxt TlsCommFlowContext to log contextual information
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \threadsafe FALSE
   */
  void FetchAndSendHelloVerifyRequestMessage(common::TlsCommFlowContext& cxt) noexcept;

  /*!
   * \brief Called when there is a new handshake message received from network.
   * \details Deserialize ClientHelloMessage data and handles it by calling the function HandleClientHelloMessage.
   * Report an error if a message of any other type received.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * HandshakeType, see RFC 5246
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
   * \trace SPEC-6561860
   * \trace SPEC-6562052
   * \trace SPEC-6562053
   * \trace SPEC-6562065
   * \vprivate This method is component private.
   * \threadsafe FALSE
   */
  void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t>& buffer,
                                  bool retransmit) noexcept final;

  /*!
   * \brief Called when a ChangeCipherSpec message is received from the network.
   * \details Report an error that an unexpected message is received.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  ChangeCipherSpec message, which is only one byte of value 1.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported that an Unexpected handshake message received
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnChangeCipherMessageReceived(common::TlsCommFlowContext& cxt,
                                     ara::core::Vector<uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Function executed upon entering the state ServerStateConnected
   * \details Prepare the configuration of the state
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

 private:
  /*!
   * \brief The cookie verification method implements a stateless client verification method
   * \param[in, out] cxt TlsCommFlowContext for storing error information
   * \param[in] client_hello_message the client hello message with the cookie to verify
   * \return true if verification succeeds otherwise false
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace SPEC-6562058
   */
  bool VerifyCookie(common::TlsCommFlowContext& cxt,
                    messages::ClientHelloMessage const& client_hello_message) const noexcept;
};

}  // namespace serverstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVERSTATES_SERVER_STATE_DISCONNECTED_H_

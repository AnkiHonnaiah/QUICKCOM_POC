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
 *         \brief This file contains the declaration for ClientStateConnected class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CONNECTED_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CONNECTED_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/common/common_general_types.h"
#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*!
 * \brief State entered after the ClientStateServerChangeCipherState.
 * \details This state means the client has finished the handshake procedure and the connection is ready to start
 * sending and receiving application data.
 * \vprivate This class is component private.
 */
class ClientStateConnected final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ClientStateConnected
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  ClientStateConnected(HandshakeStateMachineInterface& ref_to_hsm, messages::MessageManager& ref_to_msg_mngr,
                       record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor. */
  ~ClientStateConnected() final = default;

  ClientStateConnected(ClientStateConnected const&) = delete;
  ClientStateConnected(ClientStateConnected&&) = delete;
  ClientStateConnected& operator=(ClientStateConnected const&) = delete;
  ClientStateConnected& operator=(ClientStateConnected&&) = delete;

  /*!
   * \brief Function executed upon entering the state ClientStateConnected
   * \details Call the connect interface on record protocol and prepare the configuration of the state
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Function executed right before exiting the state.
   * \details Notify the record layer to disconnect
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnExit() noexcept final;

  /*!
   * \brief Handle reporting error upon receiving unrelated or unexpected messages.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \error TlsCommErrorCode::kTlsRenegotiationRejected is reported if renegotiation is not supported
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
   * \context ANY
   * \trace SPEC-6561862, SPEC-6561864, SPEC-6561860
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t>& buffer,
                                  bool retransmit) noexcept final;

  /*!
   * \brief Called when a ChangeCipherSpec message is received from the network.
   * \details Report an error if the message is received while the protocol is TLS
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  ChangeCipherSpec message, which is only one byte of value 1.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected ChangeCipherSpecMessage received
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnChangeCipherMessageReceived(common::TlsCommFlowContext& cxt,
                                     ara::core::Vector<uint8_t> const& buffer) noexcept final;

 private:
  /*! \brief Friend test declaration for OnEnter */
  FRIEND_TEST(Cdd__handshakestatemachine_clientstateconnected, OnEnter);

  /*! \brief Friend test declaration for OnExit */
  FRIEND_TEST(Cdd__handshakestatemachine_clientstateconnected, OnExit);

  /*! \brief Friend test declaration for OnHandshakeMessageReceived */
  FRIEND_TEST(Cdd__handshakestatemachine_clientstateconnected, OnHandshakeMessageReceived);

  /*! \brief Friend test declaration for OnChangeCipherMessageReceived */
  FRIEND_TEST(Cdd__handshakestatemachine_clientstateconnected, OnChangeCipherMessageReceived);

  /*! \brief Friend test declaration for OnChangeCipherMessageReceived in DTLS mode */
  FRIEND_TEST(Cdd__handshakestatemachine_clientstateconnected_dtls, OnChangeCipherMessageReceived);

  /*! \brief Friend test declaration for OnHandshakeMessageReceived in DTLS mode */
  FRIEND_TEST(Cdd__handshakestatemachine_clientstateconnected_dtls, OnHandshakeMessageReceived);

  /*! \brief Friend test declaration for OnEnter */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateConnected, OnEnter);

  /*! \brief Friend test declaration for OnExit */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateConnected, OnExit);

  /*! \brief Friend test declaration for OnHandshakeMessageReceived */
  FRIEND_TEST(UT__ClientHandshakeStatePool_clientstateconnected, OnHandshakeMessageReceived);

  /*! \brief Friend test declaration for OnChangeCipherMessageReceived */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateConnected, OnChangeCipherMessageReceived);

  /*! \brief Friend test declaration for OnChangeCipherMessageReceived in DTLS mode */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateConnected_dtls, OnChangeCipherMessageReceived);

  /*! \brief Friend test declaration for OnHandshakeMessageReceived in DTLS mode */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateConnected_dtls, OnHandshakeMessageReceived);
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CONNECTED_H_

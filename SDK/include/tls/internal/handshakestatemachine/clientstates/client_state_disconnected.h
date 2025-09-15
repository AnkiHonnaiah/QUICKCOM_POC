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
 *         \brief   This file contains the declaration for ClientStateDisconnected class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_DISCONNECTED_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_DISCONNECTED_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*!
 * \brief Initial state of the Client Handshake state machine. Also the state transited to on a Disconnect call to the
 * state machine and after leaving fatal error state.
 * \vprivate This class is component private.
 */
class ClientStateDisconnected final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ClientStateDisconnected
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  ClientStateDisconnected(HandshakeStateMachineInterface& ref_to_hsm, messages::MessageManager& ref_to_msg_mngr,
                          record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor */
  ~ClientStateDisconnected() final = default;

  ClientStateDisconnected(ClientStateDisconnected const&) = delete;
  ClientStateDisconnected(ClientStateDisconnected&&) = delete;
  ClientStateDisconnected& operator=(ClientStateDisconnected const&) = delete;
  ClientStateDisconnected& operator=(ClientStateDisconnected&&) = delete;

  /*!
   * \brief Handle reporting an error since no messages must be received in this state
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received
   * \context ANY
   * \trace SPEC-6561860
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t>& buffer,
                                  bool retransmit) noexcept final;

  /*!
   * \brief Called when a ChangeCipherSpec message is received from the network.
   * \details Report an error that an unexpected message is received.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  ChangeCipherSpec message, which is only one byte of value 1.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported that an unexpected ChangeCipherSpecMessage received
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnChangeCipherMessageReceived(common::TlsCommFlowContext& cxt,
                                     ara::core::Vector<uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Function executed upon entering the state ClientStateDisconnected
   * \details Prepare the configuration of the state
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * */
  void OnEnter(common::TlsCommFlowContext&) noexcept final;

  /*!
   * \brief This operation is called to start the handshake procedure.
   * \details Prepare the configuration for transition to state ClientStateClientHello
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void InitiateHandshake(common::TlsCommFlowContext& cxt) noexcept final;
  // VECTOR Enable AutosarC++17_10-M7.1.2

 private:
  /*! \brief Friend test declaration for ClientStateDisconnected */
  FRIEND_TEST(Cdd__ClientStateDisconnected, ClientStateDisconnected);

  /*! \brief Friend test declaration for InitiateHandshake */
  FRIEND_TEST(Cdd__ClientStateDisconnected, InitiateHandshake);

  /*! \brief Friend test declaration for InitiateHandshake */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateDisconnected, InitiateHandshake);

  /*! \brief Friend test declaration for OnEnter */
  FRIEND_TEST(Cdd__ClientStateDisconnected, OnEnter);

  /*! \brief Friend test declaration for ClientStateDisconnected */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateDisconnected, ClientStateDisconnected);

  /*! \brief Friend test declaration for InitiateHandshake */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateDisconnected, InitiateHandshake);

  /*! \brief Friend test declaration for InitiateHandshake */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateDisconnected, InitiateHandshake);

  /*! \brief Friend test declaration for OnEnter */
  FRIEND_TEST(UT__ClientHandshakeStatePool_ClientStateDisconnected, OnEnter);
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_DISCONNECTED_H_

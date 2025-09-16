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
 *         \brief   This file contains the declaration for ClientStateServerCertificateExchange class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_CERTIFICATE_EXCHANGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_CERTIFICATE_EXCHANGE_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/handshake_state.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*!
 * \brief State entered after ClientStateClientHello if a ServerHello message is received from the server and if
 *        a certificate-based cipher suite is configured to be used.
 * \vprivate This class is component private.
 */
class ClientStateServerCertificateExchange final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ClientStateServerCertificateExchange.
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with.
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with.
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  ClientStateServerCertificateExchange(HandshakeStateMachineInterface& ref_to_hsm,
                                       messages::MessageManager& ref_to_msg_mngr,
                                       record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor */
  ~ClientStateServerCertificateExchange() final = default;

  ClientStateServerCertificateExchange(ClientStateServerCertificateExchange const&) = delete;
  ClientStateServerCertificateExchange(ClientStateServerCertificateExchange&&) = delete;
  ClientStateServerCertificateExchange& operator=(ClientStateServerCertificateExchange const&) = delete;
  ClientStateServerCertificateExchange& operator=(ClientStateServerCertificateExchange&&) = delete;

  /*!
   * \brief Function executed upon entering the state ClientStateServerCertificateExchange.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Process the received message and prepare configuration for next state.
   * \details Deserialize the received message. Then if it was a ServerKeyExchangeMessage message, prepare the
   *          configuration for the state ClientStateServerKeyExchange.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting.
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   *                    HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if unexpected handshake message received.
   * \trace SPEC-6561862, SPEC-6561860, SPEC-6561864
   */
  void OnHandshakeMessageReceived(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t>& buffer,
                                  bool retransmit) noexcept final;

  /*!
   * \brief Called when a ChangeCipherSpec message is received from the network.
   * \details Report an error if the message is received while the protocol is TLS.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting.
   * \param[in] buffer  ChangeCipherSpec message, which is only one byte of value 1.
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if Unexpected handshake message received.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnChangeCipherMessageReceived(common::TlsCommFlowContext& cxt,
                                     ara::core::Vector<uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Callback that is activated when the timer expires.
   * \details Calling this method for this class shall lead to an abort since the library only supports certificate
   *          with TLS.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void OnTimerEvent(common::TlsCommFlowContext& cxt) noexcept final;
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_SERVER_CERTIFICATE_EXCHANGE_H_

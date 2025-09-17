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
 *         \brief This file contains the declaration for ClientStateClientHello class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CLIENT_HELLO_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CLIENT_HELLO_H_

#include <cstddef>
#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/common/common_general_types.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/handshakestatemachine/handshake_state.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace clientstates {

/*! \brief Required forward declaration of unit test class because of namespace conflicts */
class Cdd__HandshakeStatePool_ClientPoolCloseDown_Test;

/*! \brief Required forward declaration of unit test class because of namespace conflicts */
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class UT__ClientHandshakeStatePool_OpenForAction_Success_CorrectCipherSuiteListSize_Test;

/*! \brief Required forward declaration of unit test class because of namespace conflicts */
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class UT__ClientHandshakeStatePool_ClientPoolCloseDown_Success_Test;

/*! \brief Required forward declaration of unit test class because of namespace conflicts */
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class UT__ClientHandshakeStatePool_ClientPoolCloseDown_Failure_Test;

/*! \brief Required forward declaration of unit test class because of namespace conflicts */
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class UT__ClientHandshakeStatePool_OpenForAction_Failed_ExceedListSize_Test;

/*! \brief Required forward declaration of unit test class because of namespace conflicts */
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class UT__ClientHandshakeStatePool_OpenForAction_Failed_NoInstanceCreated_Test;

/*! \brief Required forward declaration of unit test class because of namespace conflicts */
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class UT__ClientHandshakeStatePool_OpenForAction_Failed_InvalidListCipher_Test;

/*!
 * \brief State entered after InitiateHandshake is called.
 * \details On entry, a ClientHello handshake message is sent. The state then waits for a ServerHello handshake message
 * before transiting to ClientStateServerHello.
 * \vprivate This class is component private.
 * \trace DSGN-SecCom-CipherService
 */
class ClientStateClientHello final : public HandshakeState {
 public:
  /*!
   * \brief Constructor of ClientStateClientHello
   * \param[in] ref_to_hsm Reference to the handshake state machine this state is associated with
   * \param[in] ref_to_msg_mngr Reference to the message manager this state is associated with
   * \param[in] ref_to_rp Reference to the record protocol this state is associated with
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  ClientStateClientHello(HandshakeStateMachineInterface& ref_to_hsm, messages::MessageManager& ref_to_msg_mngr,
                         record_protocol::RecordProtocolInterface& ref_to_rp) noexcept;

  /*! \brief Default destructor */
  ~ClientStateClientHello() final = default;

  ClientStateClientHello(ClientStateClientHello const&) = delete;
  ClientStateClientHello(ClientStateClientHello&&) = delete;
  ClientStateClientHello& operator=(ClientStateClientHello const&) = delete;
  ClientStateClientHello& operator=(ClientStateClientHello&&) = delete;

  /*!
   * \brief Sets the supported cipher suites for this client from the supplied list.
   * \param[in] list_of_suites List of crypto suites
   * \param[in] no_of_suites Number of crypto suites in the list.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-6561872
   */
  void SetSupportedCipherSuites(ArrOfSuites const& list_of_suites, std::size_t no_of_suites) noexcept;

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
   * \brief Function executed upon entering the state ClientStateClientHello .
   * \details Prepare and send ClientHello message to server. Set and start the timer of this state. The timer handles
   * retransmission, if it times out the last flight of messages will be re-transmitted.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \error TlsCommErrorCode::kTlsCryptoAdapterFailure is reported if handshakestatemachine::utility::GenerateRNG fails
   * \trace SPEC-8797234
   */
  void OnEnter(common::TlsCommFlowContext& cxt) noexcept final;

  /*!
   * \brief Called when there is a new handshake message received from network.
   * \details Handles ServerHello message by calling HandleServerHelloMessage().
   * Handles HelloVerifyRequest messages by replying with a new ClientHello message containing the cookie
   * from the HelloVerifyRequest messages (DTLS).
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer  Received handshake data including the handshake header as bytes, so the first byte should be the
   * HandshakeType, see RFC 5246
   * \param[in] retransmit HandshakeAggregator consider this as a retransmit
   * \vprivate This method is component private.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsAlertUnexpectedMessage is reported if empty handshake message received or unexpected
   * handshake message received
   * \trace SPEC-6562058, SPEC-6561862, SPEC-6561860, SPEC-6561864
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
  /*! \brief Candidate cipher suites to be used in the handshake procedure. */
  ListOfCipherSuites cipher_suites_{};

  /*!
   * \brief Checks if the selected cipher suite by the server is the last one in the cipher_suites_ vector sent by the
   * client.
   * \details In DTLS mode with cookie verification the protocol version of the most recent HelloVerifyRequest message
   * is compared to the protocol version in the ServerHello message.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer ServerHelloMessage message
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsAlertHandshakeFailure is reported if no matching cipher suite was found.
   * \error TlsCommErrorCode::kTlsAlertHandshakeFailure is reported if the ServerHello message and the
   * HelloVerifyRequest message have different versions.
   * \trace SPEC-6562058
   */
  void HandleServerHelloMessage(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t> const& buffer) noexcept;

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

  /*! \brief Friend test declaration for SetSupportedCipherSuitesAndCloseDown*/
  FRIEND_TEST(Cdd__TLSClientStateClientHello, SetSupportedCipherSuitesAndCloseDown);

  /*! \brief Friend test declaration for OnEnter*/
  FRIEND_TEST(Cdd__TLSClientStateClientHello, OnEnter);

  /*! \brief Friend test declaration for HandleServerHelloMessage*/
  FRIEND_TEST(Cdd__TLSClientStateClientHello, HandleServerHelloMessage);

  /*! \brief Friend test declaration for OnTimerEvent_DTLS*/
  FRIEND_TEST(Cdd__DTLSClientStateClientHello, OnTimerEvent_DTLS);

  /*! \brief Friend test declaration for GetRandomVector*/
  FRIEND_TEST(Cdd__TLSClientStateClientHello, GetRandomVector);

  /*! \brief Friend test declaration for SetSupportedCipherSuitesAndCloseDown*/
  FRIEND_TEST(UT__ClientHandshakeStatePool_TlsClientStateClientHello, SetSupportedCipherSuitesAndCloseDown);

  /*! \brief Friend test declaration for OnEnter*/
  FRIEND_TEST(UT__ClientHandshakeStatePool_TlsClientStateClientHello, OnEnter);

  /*! \brief Friend test declaration for HandleServerHelloMessage*/
  FRIEND_TEST(UT__ClientHandshakeStatePool_TlsClientStateClientHello, HandleServerHelloMessage);

  /*! \brief Friend test declaration for OnTimerEvent_DTLS*/
  FRIEND_TEST(UT__ClientHandshakeStatePool_DtlsClientStateClientHello, OnTimerEvent_DTLS);

  /*! \brief Friend test declaration for GetRandomVector*/
  FRIEND_TEST(UT__ClientHandshakeStatePool_TlsClientStateClientHello, GetRandomVector);

  /*! \brief Friend test declaration for DTlsVectorClientReconnectWithForgedClientHello*/
  FRIEND_TEST(Creq__IntegrationTestFixture, DTlsVectorClientReconnectWithForgedClientHello);

  /*! \brief Friend test declaration for ClientPoolCloseDown*/
  FRIEND_TEST(Cdd__HandshakeStatePool, ClientPoolCloseDown);

  /*! \brief Friend test declaration for ClientPoolCloseDown_Success*/
  FRIEND_TEST(UT__ClientHandshakeStatePool, ClientPoolCloseDown_Success);

  /*! \brief Friend test declaration for ClientPoolCloseDown_Failure*/
  FRIEND_TEST(UT__ClientHandshakeStatePool, ClientPoolCloseDown_Failure);

  /*! \brief Friend test declaration for OpenForAction_Success_CorrectCipherSuiteListSize*/
  FRIEND_TEST(UT__ClientHandshakeStatePool, OpenForAction_Success_CorrectCipherSuiteListSize);

  /*! \brief Friend test declaration for OpenForAction_Failed_ExceedListSize*/
  FRIEND_TEST(UT__ClientHandshakeStatePool, OpenForAction_Failed_ExceedListSize);

  /*! \brief Friend test declaration for OpenForAction_Failed_InvalidListCipher*/
  FRIEND_TEST(UT__ClientHandshakeStatePool, OpenForAction_Failed_InvalidListCipher);

  /*! \brief Friend test declaration for ClientPoolCloseDown*/
  FRIEND_TEST(UT__ClientHandshakeStatePool_HandshakeStatePool, ClientPoolCloseDown);
};

}  // namespace clientstates
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENTSTATES_CLIENT_STATE_CLIENT_HELLO_H_

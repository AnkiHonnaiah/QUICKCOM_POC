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
 *         \brief This file contains the implementation of ClientHandshakeStatePool class
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENT_HANDSHAKE_STATE_POOL_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENT_HANDSHAKE_STATE_POOL_H_

#include <memory>  // unique_ptr

#include "tls/internal/common/common_general_types.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_certificate_request.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_client_hello.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_connected.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_disconnected.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_server_certificate_exchange.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_server_change_cipher_spec.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_server_hello.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_server_hello_done.h"
#include "tls/internal/handshakestatemachine/clientstates/client_state_server_key_exchange.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/handshakestatemachine/handshake_state_pool_interface.h"
#include "tls/internal/messages/message_manager.h"
#include "tls/internal/record_protocol/record_protocol_interfaces.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

/*!
 * \brief A class containing all handshake states necessary for a TLS client state machine.
 * \details It also helps simplifies fetching of states via enums, to ensure that state data is correctly preserved.
 * \vprivate This class is component private.
 */
class ClientHandshakeStatePool final : public HandshakeStatePoolInterface {
 public:
  /*! \brief Using inheriting constructor */
  using HandshakeStatePoolInterface::HandshakeStatePoolInterface;

  /*! \brief Default constructor. */
  ClientHandshakeStatePool() noexcept = default;

  /*! \brief Default destructor. */
  ~ClientHandshakeStatePool() noexcept final = default;

  ClientHandshakeStatePool(ClientHandshakeStatePool const&) = delete;
  ClientHandshakeStatePool(ClientHandshakeStatePool&&) = delete;
  ClientHandshakeStatePool& operator=(ClientHandshakeStatePool const&) = delete;
  ClientHandshakeStatePool& operator=(ClientHandshakeStatePool&&) = delete;

  /*!
   * \copydoc ::amsr::tls::internal::handshakestatemachine::HandshakeStatePoolInterface::CreateInstance
   * \vprivate This method is component private.
   */
  void CreateInstance(HandshakeStateMachineInterface& state_machine, messages::MessageManager& handshake_msg_mgr,
                      record_protocol::RecordProtocolInterface& record_protocol) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::handshakestatemachine::HandshakeStatePoolInterface::OpenForAction
   * \vprivate This method is component private.
   */
  void OpenForAction(common::TlsCommFlowContext& cxt, ListOfCipherSuites& cipher_suites_to_use) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::handshakestatemachine::HandshakeStatePoolInterface::CloseDown
   * \vprivate This method is component private.
   */
  void CloseDown() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::handshakestatemachine::HandshakeStatePoolInterface::GetState
   * \vprivate This method is component private.
   */
  HandshakeState& GetState(HandshakeStateHandle handshake_state_handle) const noexcept final;

  /*!
   * \brief Cleans any resources used by the ClientHandshakeStatePool, so the pool is ready to initiate new connections.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void Cleanup() const noexcept final;

 private:
  /*!
   * \brief Validates input arguments and then call HandshakeState::OpenForAction() on the passed state
   * \details If the ptr_handshake_state is a nullptr, it will abort
   * \param[in, out] cxt
   * \param[in] ptr_handshake_state Pointer to a handshake state
   * \context ANY
   * \pre ptr_handshake_state cannot be a nullptr
   * \reentrant FALSE
   */
  static void SafeOpenForAction(common::TlsCommFlowContext& cxt, HandshakeState* ptr_handshake_state) noexcept;

  /*!
   * \brief Validates input arguments before calling Cleanup on the passed state
   * \param[in] ptr_handshake_state Pointer to a handshake state
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  static void SafeCleanup(HandshakeState* ptr_handshake_state) noexcept;

  /*!
   * \brief Converts the ListOfCryptoSuites into an uint16_t array containing the crypto suites in order.
   * \details If the size of ListOfCryptoSuites is larger than size of arr_of_suites, it will abort with message "The
   * list of cipher suites contains too many elements"
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] cipher_suites_to_use A vector of all suites to use.
   * \param[in,out] arr_of_suites An array that is to be filled and used with the crypto cipher suites.
   * \context ANY
   * \pre The size of the input cipher suits list should be smaller than the limitation(10)
   * \reentrant FALSE
   * \error TlsCommErrorCode::kTlsErrInvalidArgument is reported if cipher suite is invalid, and could not convert
   * suites list to array
   */
  static void ConvertCipherSuiteListToArray(common::TlsCommFlowContext& cxt, ListOfCipherSuites& cipher_suites_to_use,
                                            ArrOfSuites& arr_of_suites) noexcept;

  /*! \brief Unique pointer wrapping a ClientStateDisconnected object */
  std::unique_ptr<clientstates::ClientStateDisconnected> disconnected_state_{nullptr};

  /*! \brief Unique pointer wrapping a ClientStateClientHello object */
  std::unique_ptr<clientstates::ClientStateClientHello> client_hello_state_{nullptr};

  /*! \brief Unique pointer wrapping a ClientStateServerHello object */
  std::unique_ptr<clientstates::ClientStateServerHello> server_hello_state_{nullptr};

  /*! \brief Unique pointer wrapping a ClientStateServerCertificateExchange object */
  std::unique_ptr<clientstates::ClientStateServerCertificateExchange> server_certificate_exchange_state_{nullptr};

  /*! \brief Unique pointer wrapping a ClientStateServerKeyExchange object */
  std::unique_ptr<clientstates::ClientStateServerKeyExchange> server_key_exchange_state_{nullptr};

  /*! \brief The associated CertificateRequest state. */
  std::unique_ptr<clientstates::ClientStateCertificateRequest> certificate_request_state_{nullptr};

  /*! \brief Unique pointer wrapping a ClientStateServerHelloDone object */
  std::unique_ptr<clientstates::ClientStateServerHelloDone> server_hello_done_state_{nullptr};

  /*! \brief Unique pointer wrapping a ClientStateServerChangeCipherSpec object */
  std::unique_ptr<clientstates::ClientStateServerChangeCipherSpec> server_change_cipher_spec_state_{nullptr};

  /*! \brief Unique pointer wrapping a ClientStateConnected object */
  std::unique_ptr<clientstates::ClientStateConnected> connected_state_{nullptr};

  /*! \brief Friend test declaration for ClientSafeOpenForAction */
  FRIEND_TEST(Cdd__HandshakeStatePool, ClientSafeOpenForAction);

  /*! \brief Friend test declaration for ClientPoolCloseDown */
  FRIEND_TEST(clientstates::Cdd__HandshakeStatePool, ClientPoolCloseDown);

  /*! \brief Friend test declaration for ClientSafeCleanup */
  FRIEND_TEST(Cdd__HandshakeStatePool, ClientSafeCleanup);

  /*! \brief Friend test declaration for ClientSafeOpenForAction_InvalidArgument */
  FRIEND_TEST(UT__ClientHandshakeStatePool, ClientSafeOpenForAction_InvalidArgument);

  /*! \brief Friend test declaration for ClientSafeOpenForAction_ValidArgument */
  FRIEND_TEST(UT__ClientHandshakeStatePool, ClientSafeOpenForAction_ValidArgument);

  /*! \brief Friend test declaration for ClientPoolCloseDown_Success */
  FRIEND_TEST(clientstates::UT__ClientHandshakeStatePool, ClientPoolCloseDown_Success);

  /*! \brief Friend test declaration for ClientPoolCloseDown_Failure */
  FRIEND_TEST(clientstates::UT__ClientHandshakeStatePool, ClientPoolCloseDown_Failure);

  /*! \brief Friend test declaration for ClientSafeCleanup */
  FRIEND_TEST(UT__ClientHandshakeStatePool, ClientSafeCleanup);

  /*! \brief Friend test declaration for OpenForAction_Success_CorrectCipherSuiteListSize */
  FRIEND_TEST(clientstates::UT__ClientHandshakeStatePool, OpenForAction_Success_CorrectCipherSuiteListSize);

  /*! \brief Friend test declaration for ClientPoolCloseDown */
  FRIEND_TEST(clientstates::UT__ClientHandshakeStatePool, OpenForAction_Failed_ExceedListSize);

  /*! \brief Friend test declaration for OpenForAction_Failed_NoInstanceCreated */
  FRIEND_TEST(clientstates::UT__ClientHandshakeStatePool, OpenForAction_Failed_NoInstanceCreated);

  /*! \brief Friend test declaration for OpenForAction_Failed_InvalidListCipher */
  FRIEND_TEST(clientstates::UT__ClientHandshakeStatePool, OpenForAction_Failed_InvalidListCipher);
};

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_CLIENT_HANDSHAKE_STATE_POOL_H_

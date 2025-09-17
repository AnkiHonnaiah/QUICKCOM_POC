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
/*!        \file  server_handshake_state_pool.h
 *         \brief This file contains the declaration of ServerHandshakeStatePool class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVER_HANDSHAKE_STATE_POOL_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVER_HANDSHAKE_STATE_POOL_H_

#include <memory>
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/handshakestatemachine/handshake_state_machine_interface.h"
#include "tls/internal/handshakestatemachine/handshake_state_pool_interface.h"
#include "tls/internal/handshakestatemachine/serverstates/server_state_client_change_cipher_spec.h"
#include "tls/internal/handshakestatemachine/serverstates/server_state_client_hello.h"
#include "tls/internal/handshakestatemachine/serverstates/server_state_client_key_exchange.h"
#include "tls/internal/handshakestatemachine/serverstates/server_state_connected.h"
#include "tls/internal/handshakestatemachine/serverstates/server_state_disconnected.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace serverstates {
class Cdd__HandshakeStatePool_ServerPoolCloseDown_Test;
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class UT__ServerHandshakeStatePool_ServerPoolCloseDown_Test;
// VECTOR NL AutosarC++17_10-M3.2.4: MD_libseccom_M3.2.4_test2020_type_without_definition
class UT__ClientHandshakeStatePool_ServerPoolCloseDown_Test;
}  // namespace serverstates

/*!
 * \brief A handshake state pool containing the necessary states for a TLS/DTLS server.
 * \vprivate This class is component private.
 */
class ServerHandshakeStatePool final : public HandshakeStatePoolInterface {
 public:
  /*! \brief Using inheriting constructor */
  using HandshakeStatePoolInterface::HandshakeStatePoolInterface;

  /*! \brief Default constructor. */
  ServerHandshakeStatePool() noexcept = default;

  /*! \brief Default destructor. */
  ~ServerHandshakeStatePool() noexcept final = default;

  ServerHandshakeStatePool(ServerHandshakeStatePool const&) = delete;
  ServerHandshakeStatePool(ServerHandshakeStatePool&&) = delete;
  ServerHandshakeStatePool& operator=(ServerHandshakeStatePool const&) = delete;
  ServerHandshakeStatePool& operator=(ServerHandshakeStatePool&&) = delete;

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
   * \copydoc ::amsr::tls::internal::handshakestatemachine::HandshakeStatePoolInterface::Cleanup
   * \vprivate This method is component private.
   */
  void Cleanup() const noexcept final;

 private:
  /*!
   * \brief Validates input arguments before calling OpenForAction on the passed state
   * \param[in, out] cxt
   * \param[in] ptr_handshake_state Pointer to a handshake state
   * \context ANY
   * \pre ptr_handshake_state cannot be a nullptr
   * \reentrant FALSE
   */
  static void SafeOpenForAction(common::TlsCommFlowContext& cxt, HandshakeState* ptr_handshake_state) noexcept;

  /*!
   * \brief Validates input arguments before calling CloseDown on the passed state
   * \param[in] ptr_handshake_state Pointer to a handshake state
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  static void SafeCloseDown(HandshakeState* ptr_handshake_state) noexcept;

  /*!
   * \brief Validates input arguments before calling Cleanup on the passed state
   * \param[in] ptr_handshake_state Pointer to a handshake state
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  static void SafeCleanup(HandshakeState* ptr_handshake_state) noexcept;

  /*! \brief ServerStateDisconnected instance */
  std::unique_ptr<serverstates::ServerStateDisconnected> server_state_disconnected_{nullptr};

  /*! \brief ServerStateClientHello instance */
  std::unique_ptr<serverstates::ServerStateClientHello> server_state_client_hello_{nullptr};

  /*! \brief ServerStateClientKeyExchange instance */
  std::unique_ptr<serverstates::ServerStateClientKeyExchange> server_state_client_key_exchange_{nullptr};

  /*! \brief ServerStateClientChangeCipherSpec instance */
  std::unique_ptr<serverstates::ServerStateClientChangeCipherSpec> server_state_client_change_cipher_spec_{nullptr};

  /*! \brief ServerStateConnected instance */
  std::unique_ptr<serverstates::ServerStateConnected> server_state_connected_{nullptr};

  /*! \brief Friend test declaration for ServerSafeOpenForAction */
  FRIEND_TEST(Cdd__HandshakeStatePool, ServerSafeOpenForAction);

  /*! \brief Friend test declaration for ServerPoolCloseDown */
  FRIEND_TEST(serverstates::Cdd__HandshakeStatePool, ServerPoolCloseDown);

  /*! \brief Friend test declaration for ServerSafeCleanup */
  FRIEND_TEST(Cdd__HandshakeStatePool, ServerSafeCleanup);

  /*! \brief Friend test declaration for ServerSafeOpenForAction */
  FRIEND_TEST(UT__ServerHandshakeStatePool, ServerSafeOpenForAction);

  /*! \brief Friend test declaration for ServerPoolCloseDown */
  FRIEND_TEST(serverstates::UT__ServerHandshakeStatePool, ServerPoolCloseDown);

  /*! \brief Friend test declaration for ServerSafeCleanup */
  FRIEND_TEST(UT__ServerHandshakeStatePool, ServerSafeCleanup);
};

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_SERVER_HANDSHAKE_STATE_POOL_H_

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
 *         \brief This file contains the declaration of "HandshakeStatePoolInterface" class.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_POOL_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_POOL_INTERFACE_H_

#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/message_manager.h"
#include "tls/internal/record_protocol/record_protocol_interfaces.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

class HandshakeStateMachineInterface;
class HandshakeState;

/*!
 * \brief The interface for a HandshakeStatePool, which is an object keeping one instance of every required handshake
 * state in a negotiation, for either the server or client side.
 * \vprivate This class is component private.
 */
class HandshakeStatePoolInterface {
 public:
  /*!
   * \brief This operation performs initialization activities that is separated from the constructor.
   * \details The operation is called once when the object is created. The object is part of a pooled structure where
   * the structure is created during the initialization of the library and the internal pool. After that, the operations
   * OpenForAction() and CloseDown() are used to control the lifecycle.
   * \param[in] state_machine A reference to the HandshakeStateMachine. Only passed on to the states in the
   * pool, not used by the pool itself.
   * \param[in] handshake_msg_mgr Reference to the MessageManager. Only passed on to the states in
   * the pool, not used by the pool itself.
   * \param[in] record_protocol Reference to an object implementing the
   * RecordProtocolInterface. Only passed on to the states in the pool, not used by the pool itself.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void CreateInstance(HandshakeStateMachineInterface& state_machine,
                              messages::MessageManager& handshake_msg_mgr,
                              record_protocol::RecordProtocolInterface& record_protocol) noexcept = 0;

  /*!
   * \brief Makes the HandshakeState instance ready for use. It is called when fetched from the pool.
   * \details This operation must be called before any other operation belonging to the instance is used. Will make
   * necessary initiations needed for its functionality. It will also call corresponding operations in objects contained
   * by this object.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * This parameter includes two types of information. 1) Traceability data which shall be set by the caller and
   * used for internal logging activities during the execution of a flow and 2) error information that shall be set
   * in case of an error occurring during the flow. If the caller is interested in the error state after the call,
   * the caller should check for errors using the has_error variable. See TlsCommFlowContext for more information.
   * \param[in] cipher_suites_to_use Defines the crypto suites which shall be supported. The parameter is a list of
   * uint16_t values in priority order where each value corresponds to a certain crypto suite.
   * See the ListOfCryptoSuites for more details. Only passed on to the states in the pool, not used by the pool itself.
   * \context ANY
   * \pre cxt has no error when the function is called
   * \reentrant FALSE
   * \trace DSGN-SecCom-CipherService
   */
  virtual void OpenForAction(common::TlsCommFlowContext& cxt, ListOfCipherSuites& cipher_suites_to_use) noexcept = 0;

  /*!
   * \brief This operation will close down the HandshakeStatePool instance implementing this interface and return
   * dynamic resources.
   * \details After the call to this operation, the object shall not be used. It will be returned to
   * the pool as part of the structure. The operation will call corresponding operations on objects contained by this
   * instance. The purpose of the operation is to return all allocated resources, clear relevant variables and return to
   * a state which will allow a call to OpenForAction().
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void CloseDown() noexcept = 0;

  /*!
   * \brief Gets the HandshakeState associated with the HandshakeStateHandle for server/client.
   * \param[in] handshake_state_handle The handle of the desired state.
   * \return HandshakeState& A reference to the HandshakeState instance.
   * \context ANY
   * \pre the related handshake state pool must have been initialized beforehand.
   * \reentrant FALSE
   */
  virtual HandshakeState& GetState(HandshakeStateHandle handshake_state_handle) const noexcept = 0;

  /*!
   * \brief Reset the HandshakeStatePoolInterface into a clean state, resetting any resource used for the last
   * connection, and makes it ready to be reused.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  virtual void Cleanup() const noexcept = 0;

  /*! \brief Destructor */
  virtual ~HandshakeStatePoolInterface() = default;

  HandshakeStatePoolInterface(HandshakeStatePoolInterface const&) = delete;
  HandshakeStatePoolInterface(HandshakeStatePoolInterface&&) = delete;
  HandshakeStatePoolInterface& operator=(HandshakeStatePoolInterface const&) = delete;
  HandshakeStatePoolInterface& operator=(HandshakeStatePoolInterface&&) = delete;

 protected:
  /*! \brief Protected constructor for abstract class */
  HandshakeStatePoolInterface() = default;
};

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_POOL_INTERFACE_H_

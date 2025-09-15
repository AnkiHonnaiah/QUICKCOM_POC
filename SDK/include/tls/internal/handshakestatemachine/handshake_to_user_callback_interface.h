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
 *         \brief  Callback interface for the handshake protocol to communicate with the TLS user i.e. the application.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_TO_USER_CALLBACK_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_TO_USER_CALLBACK_INTERFACE_H_

#include "ara/core/vector.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

/*!
 * \brief Callback interface for the handshake protocol to communicate with the TLS user i.e. the application.
 * layer.
 * \vprivate This class is component private.
 */
class HandshakeToUserCallbackInterface {
 public:
  /*!
   * \brief Constructor.
   */
  HandshakeToUserCallbackInterface() = default;

  HandshakeToUserCallbackInterface(HandshakeToUserCallbackInterface const&) = delete;
  HandshakeToUserCallbackInterface(HandshakeToUserCallbackInterface&&) = delete;
  HandshakeToUserCallbackInterface& operator=(HandshakeToUserCallbackInterface const&) = delete;
  HandshakeToUserCallbackInterface& operator=(HandshakeToUserCallbackInterface&&) = delete;

  /*!
   * \brief Destructor.
   */
  virtual ~HandshakeToUserCallbackInterface() noexcept = default;

  /*!
   * \brief Callback that is called whenever the HandshakeStateMachine goes into the connected state.
   * \details When a successful handshake has negotiated and a secure communication channel has been opened this
   *          callback will be called.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace DSGN-SecCom-ConnectionStates, DSGN-SecCom-OnConnected
   */
  virtual void OnConnected() noexcept = 0;

  /*!
   * \brief Callback that is called whenever the HandshakeStateMachine goes into the disconnected state.
   * \details When there is no secure communication channel for the application layer to use, this callback will be
   *          called.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace DSGN-SecCom-ConnectionStates, DSGN-SecCom-OnDisconnected
   */
  virtual void OnDisconnected() noexcept = 0;

  /*!
   * \brief Callback that is called whenever a Handshake procedure starts.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace DSGN-SecCom-ConnectionStates
   */
  virtual void OnHandshakeStarted() noexcept = 0;

  /*!
   * \brief Callback that is called during a handshake procedure.
   * \details This callback reports TLS extensions back.
   * \param[in] extensions Specifies the Tls extension type value.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \trace DSGN-SecCom-ConnectionStates
   */
  virtual void ReportTlsExtensions(ara::core::Vector<TlsExtensionData> extensions) noexcept = 0;
};

}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_TO_USER_CALLBACK_INTERFACE_H_

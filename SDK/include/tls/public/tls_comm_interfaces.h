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
/**
 * \file  tls_comm_interfaces.h
 *
 * \brief Defines various interfaces used as part of the TLS Library external API.
 *
 * \details
 * Contains Error reporting interface and the base provider interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_INTERFACES_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_INTERFACES_H_

#include <memory>

#include "tls/internal/common/common_general_types.h"
#include "tls/public/tls_comm_error_definitions.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {

/*!
 * \brief The interface realized by (D)TLS communication providers.
 * \details
 * This interface together with the user callback interface form the public APIs for the TLS library.
 * A (D)TLS communication provider contains the operations needed to receive messages for processing.
 * Two different sources of message information is supported. 1) Data from the communication party (application) and
 * 2) data from the transport (network).
 * The current implementation of the TLS library is single threaded. It means that not more than one
 * thread at a time is expected when calling this operation.
 * The design assumes that a reactor(event driven) pattern is used for the threading. That means that only one thread
 * has access to the TLS library concurrently.
 * Upon successful reception and processing of application messages from the transport layer the user callback
 * is called to forward the application data to the user. If an error has occurred during processing,
 * leading to disconnection, the OnDisconnect callback will be triggered to inform the user.
 * This pure virtual class is realized by the internal concrete classes TlsClientCommProvider and TlsServerCommProvider.
 */
class TlsCommBase {
 public:
  /*! \brief Default constructor.*/
  TlsCommBase() = default;
  /*! \brief Not copyable */
  TlsCommBase(TlsCommBase const&) = delete;
  /*! \brief Not movable */
  TlsCommBase(TlsCommBase&&) = delete;
  /*! \brief Not copyable */
  TlsCommBase& operator=(TlsCommBase const&) = delete;
  /*! \brief Not movable */
  TlsCommBase& operator=(TlsCommBase&&) = delete;
  /*!
   * \brief Getter for obtaining the communication role of this provider.
   * \return The communication role. For supported values, see EnTlsCommRole.
   * \context User
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   */
  virtual EnTlsCommRole GetProviderRole() const noexcept = 0;

  /*!
   * \brief Getter to obtain the current connection state of this provider
   * \return The connection state. For supported values, see EnTlsCommRoleState.
   * \context User
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \trace DSGN-SecCom-GetConnectionState
   */
  virtual EnTlsCommRoleState GetConnectionState() const noexcept = 0;

  /*!
   * \brief This operation is called to initiate a handshake procedure on this provider.
   * \details
   * This operation is not waiting for the handshake to be completed and returns after sending the initial handshake
   * message for initiation. A call to this shall only be made on a client provider, if it is called on a server an
   * error will be logged and the request fails silently.
   * This operation will first check if the provider is in a disconnected state. If not, the operation
   * will log an error and silently fail. To be able to repeat a successful call to InitiateHandshake() the provider
   * must be in a disconnected state, this can be achieved by calling CloseConnection on the same provider once.
   * When the handshake is initiated it will continue until TlsCommCallbackInterface::OnConnected() is called.
   * If an error occurs internally during initiation the connection will not be established, an error is logged and
   * the operation fails silently. The user can track the status by making a call to GetConnectionState on the same
   * provider.
   *
   * \context User
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \trace DSGN-SecCom-InitiateHandshake
   */
  virtual void InitiateHandshake() noexcept = 0;

  /*!
   * \brief This operation shall be called by the user when application data shall be processed for sending on this
   * provider.
   * \details When a secure connection is successfully established, this operation shall be used to send application
   * messages via this provider. In the context of SomeIp-Daemon it means that data is read from an IPC socket,
   * representing a communication channel associated with an application, and sent to this provider for further
   * processing. Depending on the selected cipher suite the message may be encrypted and then sent out to transport
   * via the TlsCommCallbackInterface::WriteToTransport() callback operation.
   *
   * \param[in] buffer View of a raw data buffer. Maximum supported buffer size is 2^32 bytes.
   * \error TlsCommErrorCode::kTlsErrOk is returned if the operation was successful
   * \error TlsCommErrorCode::kTlsErrInvalidArgument is returned if the passed buffer exceeds boundaries [0, 2^32]
   * \error TlsCommErrorCode::kTlsErrInternal is returned if the implementation failed to process the passed data
   * \context User
   * \return TlsCommErrorCode value indicating success or failure
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \pre -
   * \trace DSGN-SecCom-ApplicationServices
   */
  virtual TlsCommErrorCode HandleReadFromCommParty(ReadOnlyBufferView buffer) noexcept = 0;

  /*!
   * \brief This operation shall be called by the user when received data from the transport layer shall be processed by
   * this provider.
   * \details This operation is used as input channel for the information received on the transport channel. Normally,
   * the information comes from a socket endpoint associated with a TCP or UDP connection. Depending on the current
   * status of the connection received messages are authenticated and may be decrypted before processed internally
   * or forwarded to the user in case of application messages.
   *
   * \param[in] buffer Read only buffer view of raw data. Maximum supported buffer size is 2^32 bytes.
   * \error TlsCommErrorCode::kTlsErrOk is returned if the operation was successful
   * \error TlsCommErrorCode::kTlsErrInvalidArgument is returned if the passed buffer exceeds boundaries [0, 2^32]
   * \error TlsCommErrorCode::kTlsErrInternal is returned if the implementation failed to process the passed data
   * \return TlsCommErrorCode value indicating success or failure
   * \context User
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \pre -
   * \trace DSGN-SecCom-TransportServices
   */
  virtual TlsCommErrorCode HandleReadFromTransport(ReadOnlyBufferView buffer) noexcept = 0;

  /*!
   * \brief This operation will close an ongoing connection or interrupt an ongoing handshake.
   * \details This operation will immediately stop the possibility to send application data through the TLS library.
   *
   * \context User
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   * \trace SPEC-6561837, SPEC-6561841, DSGN-SecCom-CloseConnection, DSGN-SecCom-ConnectionStates
   */
  virtual void CloseConnection() noexcept = 0;

  /*! \brief Virtual default destructor. */
  virtual ~TlsCommBase() = default;
};

/*!
 * \brief This declaration defines the unique pointer type for communication providers.
 */
using TlsCommProviderUniquePtr = std::unique_ptr<TlsCommBase>;

}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_INTERFACES_H_

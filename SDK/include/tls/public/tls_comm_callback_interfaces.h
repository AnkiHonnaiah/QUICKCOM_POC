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
 * \file  tls_comm_callback_interfaces.h
 *
 * \brief Includes parts of the declarations of the TLS Library external callback interfaces.
 *
 * \details These APIs are used when the library reports status to the user. This interface together with
 * amsr::tls::TlsCommBase interface form the APIs for the TLS library.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_CALLBACK_INTERFACES_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_CALLBACK_INTERFACES_H_

#include "amsr/core/string_view.h"
#include "tls/public/tls_comm_error_definitions.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {

/*!
 * \brief The callback interface for the TLS library external API.
 * \details This pure virtual class is required to be implemented by the user. (D)TLS service providers
 * use this interface to send information to the user application. This includes use cases such as data that
 * has been received from the transport layer and which after successful processing can be supplied to the user
 * application via the WriteToCommParty callback. Other use cases are notification of events, such as signalling
 * to the user application that the secure connection was successfully established, via the OnConnected callback.
 */
class TlsCommCallbackInterface {
 public:
  /*! \brief Default constructor.*/
  TlsCommCallbackInterface() = default;
  /*! \brief Not moveable */
  TlsCommCallbackInterface(TlsCommCallbackInterface&&) = delete;
  /*! \brief Not copyable */
  TlsCommCallbackInterface& operator=(TlsCommCallbackInterface const&) = delete;
  /*! \brief Not moveable */
  TlsCommCallbackInterface& operator=(TlsCommCallbackInterface&&) = delete;
  /*!
   * \brief Called by the (D)TLS provider after a successful handshake
   * \context (D)TLS Provider
   * \pre -
   * \reentrant FALSE
   *
   * \trace DSGN-SecCom-OnConnected
   */
  virtual void OnConnected() = 0;

  /*!
   * \brief Called by a (D)TLS provider when the connection is disconnected.
   * \details The connection can be disconnected as a result of several different events. The most common are:
   *  - Call to TlsCommBase::CloseConnection() from the user
   *  - An unsuccessful handshake
   *  - Any kind of failure
   * \param[in] err_string An error message.
   * \param[in] err_code An error code.
   * \context (D)TLS Provider
   * \pre -
   * \reentrant FALSE
   *
   * \trace DSGN-SecCom-OnDisconnected
   */
  virtual void OnDisconnected(amsr::core::StringView const err_string, TlsCommErrorCode const err_code) = 0;

  /*!
   * \brief Called internally by a (D)TLS provider when data needs to be sent over the transport layer.
   * \details The data buffer contains serialized and possibly encrypted messages. The most common messages are
   * Handshake messages and user application data messages.
   * \param[in] buffer A read only view of a raw data buffer.
   * \context (D)TLS Provider
   * \pre -
   * \reentrant FALSE
   *
   * \trace DSGN-SecCom-TransportServices, DSGN-SecCom-HandleRecordProtocolMessages, DSGN-SecCom-InitiateHandshake
   */
  virtual void WriteToTransport(ReadOnlyBufferView buffer) = 0;

  /*!
   * \brief Called internally by a (D)TLS provider when data needs to be sent to the user application.
   * \details The data buffer contains user application messages received from the connected peer.
   * \param buffer A read only view of a raw data buffer.
   * \context (D)TLS Provider
   * \pre -
   * \reentrant FALSE
   *
   * \trace DSGN-SecCom-ApplicationServices, DSGN-SecCom-TransportServices
   */
  virtual void WriteToCommParty(ReadOnlyBufferView buffer) = 0;

  /*! \brief The default destructor for the interface. */
  virtual ~TlsCommCallbackInterface() = default;

 protected:
  /*! \brief Default copy Ctor */
  TlsCommCallbackInterface(TlsCommCallbackInterface const&) = default;
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_CALLBACK_INTERFACES_H_

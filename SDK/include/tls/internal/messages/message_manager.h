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
/*!        \file  message_manager.h
 *        \brief  This file declares the class for message manager
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_MESSAGE_MANAGER_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_MESSAGE_MANAGER_H_

#include <cstdint>  // uint16_t

#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/alert_message.h"
#include "tls/internal/messages/certificate_message.h"
#include "tls/internal/messages/certificate_request_message.h"
#include "tls/internal/messages/certificate_verify_message.h"
#include "tls/internal/messages/change_cipher_spec_message.h"
#include "tls/internal/messages/client_hello_message.h"
#include "tls/internal/messages/client_key_exchange_dh_message.h"
#include "tls/internal/messages/client_key_exchange_psk_message.h"
#include "tls/internal/messages/extensions/hello_message_extensions.h"
#include "tls/internal/messages/finished_message.h"
#include "tls/internal/messages/hello_verify_request_message.h"
#include "tls/internal/messages/server_hello_done_message.h"
#include "tls/internal/messages/server_hello_message.h"
#include "tls/internal/messages/server_key_exchange_dh_message.h"
#include "tls/internal/messages/server_key_exchange_psk_message.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

// VECTOR NC Metric-OO.WMC.One: MD_libseccom_Metric-OO.WMC.One_methodsperclassjustabovelimit
/*!
 * \brief Manages all messages sent and received during TLS handshake procedure.
 * \vprivate This method is component private.
 */
class MessageManager final {
 public:
  /*! \brief Default constructor. */
  MessageManager() noexcept = default;

  MessageManager(MessageManager const&) = delete;

  MessageManager(MessageManager&&) = delete;

  /*! \brief Default destructor. */
  ~MessageManager() noexcept = default;

  auto operator=(MessageManager const&) -> MessageManager& = delete;

  auto operator=(MessageManager&&) -> MessageManager& = delete;

  /*!
   * \brief Sets the role of the message manager. TLS/DTLS and client/server.
   * \param role_to_take The communication role to take.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  void SetCommRole(EnTlsCommRole role_to_take) noexcept;

  /*!
   * \brief     Sets the hint for client authentication usage.
   *
   * \param[in] enabled True if client authentication is being used; false otherwise.
   */
  void SetUsingClientAuthentication(bool enabled) noexcept;

  /*!
   * \brief   Indicates whether client authentication is used.
   *
   * \details This is intended to be used by handshake states to determine
   *          whether messages related to client authentication should be
   *          expected and/or sent.
   *
   * \return  True if client authentication is used; false otherwise.
   */
  auto IsUsingClientAuthentication() const noexcept -> bool;

  /*!
   * \brief Gets a reference to the ClientHello message.
   * \return A reference to the ClientHello message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetClientHelloMessage() noexcept -> ClientHelloMessage&;

  /*!
   * \brief Gets a reference to the HelloVerify message.
   * \return A reference to the HelloVerify message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetHelloVerifyRequestMessage() noexcept -> HelloVerifyRequestMessage&;

  /*!
   * \brief Gets a reference to the ServerHello message.
   * \return A reference to the ServerHello message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetServerHelloMessage() noexcept -> ServerHelloMessage&;

  /*!
   * \brief Gets a reference to the ServerKeyExchange message.
   * \return A reference to the ServerKeyExchange message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetServerKeyExchangePskMessage() noexcept -> ServerKeyExchangePskMessage&;

  /*!
   * \brief     Returns the associated certificate request message.
   *
   * \return    A certificate request message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto GetCertificateRequestMessage() noexcept -> CertificateRequestMessage&;

  /*!
   * \brief Gets a reference to the ServerHelloDone message.
   * \return A reference to the ServerHelloDone message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetServerHelloDoneMessage() noexcept -> ServerHelloDoneMessage&;

  /*!
   * \brief Gets a reference to the ClientKeyExchange message.
   * \return A reference to the ClientKeyExchange message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetClientKeyExchangePskMessage() noexcept -> ClientKeyExchangePskMessage&;

  /*!
   * \brief Gets a reference to the CertificateVerify message.
   * \return A reference to the CertificateVerify message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetCertificateVerifyMessage() noexcept -> CertificateVerifyMessage&;

  /*!
   * \brief Gets a reference to the ClientFinished message.
   * \return A reference to the ClientFinished message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetClientFinishedMessage() noexcept -> FinishedMessage&;

  /*!
   * \brief Gets a reference to the ServerFinished message.
   * \return A reference to the ServerFinished message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetServerFinishedMessage() noexcept -> FinishedMessage&;

  /*!
   * \brief Gets a reference to the ClientChangeCipherSpec message.
   * \return A reference to the ClientChangeCipherSpec message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetClientChangeCipherSpecMessage() noexcept -> ChangeCipherSpecMessage&;

  /*!
   * \brief Gets a reference to the ServerChangeCipherSpec message.
   * \return A reference to the ServerChangeCipherSpec message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetServerChangeCipherSpecMessage() noexcept -> ChangeCipherSpecMessage&;

  /*!
   * \brief     Returns the associated server certificate message.
   *
   * \return    The certificate message used by the server.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto GetServerCertificateMessage() noexcept -> CertificateMessage&;

  /*!
   * \brief     Returns the associated client certificate message.
   *
   * \return    The certificate message used by the client.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   */
  auto GetClientCertificateMessage() noexcept -> CertificateMessage&;

  /*!
   * \brief Gets a reference to the ServerKeyExchangeDhMessage message.
   * \return A reference to the ServerKeyExchangeDhMessage message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetServerKeyExchangeDhMessage() noexcept -> ServerKeyExchangeDhMessage&;

  /*!
   * \brief Gets a reference to the ClientKeyExchangeDhMessage message.
   * \return A reference to the ClientKeyExchangeDhMessage message.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto GetClientKeyExchangeDhMessage() noexcept -> ClientKeyExchangeDhMessage&;

  /*!
   * \brief Sets the message sequence numbers for DTLS server messages to the expected sequence numbers for the next
   * valid sequence number signature when cookie verification is used
   * \param[in] new_server_hello_seq_nr Value of the last ServerHello message sequence number, used as reference point
   * \trace SPEC-6562068
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  void UpdateServerHandshakeMessageSeqNrOnHelloVerify(std::uint16_t new_server_hello_seq_nr) noexcept;

  /*!
   * \brief Updates the client handshake message sequence numbers to match the expected sequence on the server
   * \details This should be called whenever a ClientHello message is retransmitted, e.g. cookie verification reply
   * \param[in] new_client_hello_seq_nr The sequence number value to use as base reference
   * \trace SPEC-6562068
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  void UpdateClientHandshakeMessageSeqNrOnHelloVerify(std::uint16_t new_client_hello_seq_nr) noexcept;

  /*!
   * \brief Cleans the objects internal data.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  void Cleanup() noexcept;

 private:
  /*! \brief Initializing an instance of ClientHelloMessage */
  ClientHelloMessage client_hello_message_{};

  /*! \brief Initializing an instance of HelloVerifyRequestMessage */
  HelloVerifyRequestMessage hello_verify_request_message_{};

  /*! \brief Initializing an instance of ServerHelloMessage */
  ServerHelloMessage server_hello_message_{};

  /*! \brief Initializing an instance of ServerKeyExchangePskMessage */
  ServerKeyExchangePskMessage server_key_exchange_psk_message_{};

  /*! \brief The associated certificate request message. */
  CertificateRequestMessage certificate_request_message_{};

  /*! \brief Initializing an instance of ServerHelloDoneMessage */
  ServerHelloDoneMessage server_hello_done_message_{};

  /*! \brief Initializing an instance of ClientKeyExchangePskMessage */
  ClientKeyExchangePskMessage client_key_exchange_psk_message_{};

  /*! \brief Initializing an instance of CertificateVerifyMessage */
  CertificateVerifyMessage certificate_verify_message_{};

  /*! \brief Initializing an instance of FinishedMessage for client */
  FinishedMessage client_finished_message_{handshakestatemachine::kMessageSeqClientFinished};

  /*! \brief Initializing an instance of FinishedMessage for server */
  FinishedMessage server_finished_message_{handshakestatemachine::kMessageSeqServerFinished};

  /*! \brief Initializing an instance of ChangeCipherSpecMessage for client */
  ChangeCipherSpecMessage client_change_cipher_spec_message_{};

  /*! \brief Initializing an instance of ChangeCipherSpecMessage for server  */
  ChangeCipherSpecMessage server_change_cipher_spec_message_{};

  /*! \brief The certificate message dedicated to the server. */
  CertificateMessage server_certificate_message_{handshakestatemachine::kMessageSeqServerCertificate};

  /*! \brief The certificate message dedicated to the client. */
  CertificateMessage client_certificate_message_{handshakestatemachine::kMessageSeqClientCertificate};

  /*! \brief Initializing an instance of DH ServerKeyExchangeDhMessage */
  ServerKeyExchangeDhMessage server_key_exchange_dh_message_{};

  /*! \brief Initializing an instance of DH ClientKeyExchangeDhMessage */
  ClientKeyExchangeDhMessage client_key_exchange_dh_message_{};

  /*! \brief Indicates whether client authentication is used in the current handshake. */
  bool is_using_client_authentication_{false};

  /*! \brief Friend test declarations */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);

  /*! \brief Friend test declarations */
  FRIEND_TEST(UT__MessageManager, Cleanup);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_MESSAGE_MANAGER_H_

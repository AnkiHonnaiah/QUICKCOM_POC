/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  authentication_interface.h
 *         \brief This file contains the declaration of the abstract class AuthenticationInterface.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_AUTHENTICATION_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_AUTHENTICATION_INTERFACE_H_

#include <cstdint>
#include <functional>

#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/vector.h"
#include "ara/core/vector.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls/internal/messages/message_manager.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace authentication {

/*!
 * \brief Authentication interface.
 *
 * \vprivate This class is component private.
 */
class AuthenticationInterface {
 public:
  /*!
   * \brief Default constructor.
   */
  AuthenticationInterface() noexcept = default;

  /*!
   * \brief Default virtual destructor.
   */
  virtual ~AuthenticationInterface() noexcept = default;

  AuthenticationInterface(AuthenticationInterface const &) noexcept = delete;
  AuthenticationInterface(AuthenticationInterface &&) noexcept = delete;
  AuthenticationInterface &operator=(AuthenticationInterface const &) noexcept = delete;
  AuthenticationInterface &operator=(AuthenticationInterface &&) noexcept = delete;

  /*!
   * \brief Prepare certificate message.
   * \param[in] message_manager Message manager.
   * \param[in] provider Interface to the certificate provider.
   * \param[in] labels labels used to load a certificate chain.
   * \error kCryptoRuntimeError Crypto runtime error which can occur if loading of certificates fails.
   * \return A wrapped reference to the certificate handshake message.
   * \context Handshake
   * \pre -
   *
   * \vprivate This function is component private.
   */
  virtual amsr::core::Result<std::reference_wrapper<messages::HandshakeMessage>> PrepareCertificateMessage(
      messages::MessageManager &message_manager,
      tls_crypto_adapter::internal::CertificateProviderInterface const &provider,
      ara::core::Vector<amsr::core::String> const &labels) const noexcept = 0;

  /*!
   * \brief Handle server certificate message.
   * \details Operation includes the following:
   *          - Deserialize the server CertificateMessage.
   *          - Parse the received blob of certificates and create the Certificate objects.
   *          - Load the root certificate, verify it, and set it as the root of trust.
   *          - Verify the certificate chain.
   *          - If verified, get the public key of the server certificate and set it on the server CertificateMessage.
   * \param[in] raw_message Buffer containing the received record.
   * \param[in] message_manager Message manager.
   * \param[in] provider Interface to the certificate provider.
   * \param[in] root_label label used to load the root certificate.
   * \return Nothing if successful; an error code otherwise.
   * \error kCryptoRuntimeError Crypto runtime error.
   * \context Handshake
   * \pre -
   * \vprivate This function is component private.
   */
  virtual amsr::core::Result<void> OnServerCertificateMessageReceived(
      ara::core::Vector<std::uint8_t> const &raw_message, messages::MessageManager &message_manager,
      tls_crypto_adapter::internal::CertificateProviderInterface const &provider,
      amsr::core::String const &root_label) const noexcept = 0;

  /*!
   * \brief     Handles a received CertificateRequest message.
   *
   * \details   This function performs the following steps.
   *            1. The CertificateRequest message is deserialized.
   *            2. The client certificates are loaded.
   *            3. The compatibility with the CertificateRequest message is checked.
   *            4. If the certificates are compatible, they're added to the client CertificateMessage.
   *
   * \param[in] raw_message     The serialized CertificateRequest message.
   * \param[in] message_manager The associated message manager.
   * \param[in] provider        The certificate provider to use.
   * \param[in] labels          The labels associated with the certificate.
   * \return    Nothing if successful; an error code otherwise.
   *
   * \pre       -
   * \context   Handshake
   * \reentrant FALSE
   */
  virtual auto OnCertificateRequestMessageReceived(amsr::core::Vector<std::uint8_t> const &raw_message,
                                                   messages::MessageManager &message_manager,
                                                   tls_crypto_adapter::internal::CertificateProviderInterface &provider,
                                                   amsr::core::Vector<amsr::core::String> const &labels) const noexcept
      -> amsr::core::Result<void> = 0;
};

}  // namespace authentication
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_AUTHENTICATION_INTERFACE_H_

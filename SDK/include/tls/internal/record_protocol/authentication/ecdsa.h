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
/*!        \file  ecdsa.h
 *         \brief This file contains the declaration of constants, functions and classes that will be
 *                implemented and used in ecdsa.cpp file.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_ECDSA_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_ECDSA_H_

#include <functional>
#include <memory>
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "ara/core/vector.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls/internal/messages/message_manager.h"
#include "tls/internal/record_protocol/authentication/authentication_interface.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace authentication {

/*!
 * \brief Ecdsa authentication.
 *
 * \vprivate This class is component private.
 */
class Ecdsa final : public AuthenticationInterface {
 public:
  /*! \brief Default destructor. */
  ~Ecdsa() noexcept final = default;
  /*! \brief Default constructor. */
  Ecdsa() noexcept = default;
  /*! \brief Not copy constructible. */
  Ecdsa(Ecdsa const &) noexcept = delete;
  /*! \brief Not move constructible. */
  Ecdsa(Ecdsa &&) noexcept = delete;
  /*! \brief Not copy assignable. */
  Ecdsa &operator=(Ecdsa const &) noexcept = delete;
  /*! \brief Not move assignable. */
  Ecdsa &operator=(Ecdsa &&) noexcept = delete;

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
  amsr::core::Result<std::reference_wrapper<messages::HandshakeMessage>> PrepareCertificateMessage(
      messages::MessageManager &message_manager,
      tls_crypto_adapter::internal::CertificateProviderInterface const &provider,
      ara::core::Vector<amsr::core::String> const &labels) const noexcept final;

  /*! \copydoc record_protocol::authentication::AuthenticationInterface::OnServerCertificateMessageReceived */
  amsr::core::Result<void> OnServerCertificateMessageReceived(
      ara::core::Vector<std::uint8_t> const &raw_message, messages::MessageManager &message_manager,
      tls_crypto_adapter::internal::CertificateProviderInterface const &provider,
      amsr::core::String const &root_label) const noexcept final;

  /*! \copydoc record_protocol::authentication::AuthenticationInterface::OnCertificateRequestMessageReceived */
  auto OnCertificateRequestMessageReceived(amsr::core::Vector<std::uint8_t> const &raw_message,
                                           messages::MessageManager &message_manager,
                                           tls_crypto_adapter::internal::CertificateProviderInterface &provider,
                                           amsr::core::Vector<amsr::core::String> const &labels) const noexcept
      -> amsr::core::Result<void> final;

  /*!
   * \brief Creates an ECDSA instance.
   * \return Unique pointer to an ECDSA authentication interface.
   * \context Handshake
   * \pre -
   *
   * \vprivate This function is component private.
   */
  static std::unique_ptr<AuthenticationInterface> Create() noexcept;
};

}  // namespace authentication
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_ECDSA_H_

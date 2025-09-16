/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   authn_stream_cipher_encryptor.h
 *        \brief  This file contains the declaration of AuthnStreamCipherEncryptor class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_AUTHN_STREAM_CIPHER_ENCRYPTOR_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_AUTHN_STREAM_CIPHER_ENCRYPTOR_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "ara/crypto/cryp/authn_stream_cipher_ctx.h"
#include "ara/crypto/cryp/symmetric_key.h"
#include "crypto_adapter_error_definitions.h"
#include "crypto_adapter_interface.h"
#include "crypto_adapter_types.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief AuthnStreamCipherEncryptor implementation.
 * \vprivate This class is component private.
 */
class AuthnStreamCipherEncryptor final : public ::amsr::tls_crypto_adapter::internal::EncryptorInterface {
 public:
  /*!
   * \brief Constructs an object of AuthnStreamCipherEncryptor class
   * \context Any
   * \pre cipher and key cannot be nullptr and the key must be compatible with the cipher
   * \param[in] cipher A unique pointer to stream cipher used for encryption
   * \param[in] key A unique pointer to the symmetric key algorithm context
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  AuthnStreamCipherEncryptor(ara::crypto::cryp::AuthnStreamCipherCtx::Uptr cipher,
                             ara::crypto::cryp::SymmetricKey::Uptrc key);

  /*!
   * \copydoc EncryptorInterface::Encrypt
   */
  amsr::core::Result<ReadOnlyBufferView> Encrypt(ReadOnlyBufferView nonce, ReadOnlyBufferView additional_data,
                                                 ReadOnlyBufferView payload) noexcept final;

 private:
  /*! The offset of the current buffer */
  std::size_t current_buffer_offset_{0U};
  /*! The buffer to accommodate the encryption data */
  std::array<std::uint8_t, kMaxEncryptionBufferSize> buffer_{};
  /*! Unique smart pointer to the authentication stream cipher context */
  ara::crypto::cryp::AuthnStreamCipherCtx::Uptr cipher_{nullptr};
  /*! Unique smart pointer to the symmetric key */
  ara::crypto::cryp::SymmetricKey::Uptrc key_{nullptr};
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_AUTHN_STREAM_CIPHER_ENCRYPTOR_H_

/*!********************************************************************************************************************
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
/*!        \file  ed25519_signature_verifier.h
 *        \brief  This file contains the declaration for Ed25519SignatureVerifier.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_ED25519_SIGNATURE_VERIFIER_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_ED25519_SIGNATURE_VERIFIER_H_

#include <memory>
#include <utility>
#include "ara/core/vector.h"
#include "ara/crypto/cryp/public_key.h"
#include "ara/crypto/cryp/verifier_public_ctx.h"
#include "crypto_adapter_interface.h"
#include "crypto_adapter_types.h"
#include "tls_crypto_adapter/internal/crypto_adapter_error_definitions.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief This verifier uses a Ed25519 public verifier context with a public key.
 * \details ECDSA (Elliptic curve digital signature algorithm) Ed25519.
 */
class Ed25519SignatureVerifier final : public SignatureVerifierInterface {
 public:
  /*! \brief Unique pointer type for Ed25519SignatureVerifier. */
  using Uptr = std::unique_ptr<Ed25519SignatureVerifier>;

  Ed25519SignatureVerifier(Ed25519SignatureVerifier const&) = delete;

  Ed25519SignatureVerifier(Ed25519SignatureVerifier&&) = delete;

  Ed25519SignatureVerifier& operator=(Ed25519SignatureVerifier const&) = delete;

  Ed25519SignatureVerifier& operator=(Ed25519SignatureVerifier&&) = delete;

  /*! \brief Default destructor. */
  ~Ed25519SignatureVerifier() noexcept final = default;

  /*!
   * \brief Constructor, takes ownership over the passed arguments.
   * \param[in] verifier_ctx A unique pointer to a VerifierPublicCtx.
   * \param[in] key A unique pointer to a constant PublicKey.
   * \pre VerifierPublicCtx and PublicKey shall not be nullptr.
   */
  Ed25519SignatureVerifier(ara::crypto::cryp::VerifierPublicCtx::Uptr verifier_ctx,
                           ara::crypto::cryp::PublicKey::Uptrc key)
      : SignatureVerifierInterface(), verifier_ctx_{std::move(verifier_ctx)}, key_{std::move(key)} {
    if ((verifier_ctx_ == nullptr) || (key_ == nullptr)) {
      amsr::core::Abort("Ed25519SignatureVerifier shall not be created with nullptr arguments!");
    }
  }

  /*!
   * \copydoc SignatureVerifierInterface::VerifySignature
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if creating Signature Verifier fails.
   */
  amsr::core::Result<bool, CryptoAdapterErrorCode> VerifySignature(ara::core::Vector<std::uint8_t> data,
                                                                   Signature signature) const noexcept final;

 private:
  /*!
   * \brief The Verifier context used to verify digital signatures.
   */
  ara::crypto::cryp::VerifierPublicCtx::Uptr verifier_ctx_;
  /*!
   * \brief The public key which is set on the verifier context.
   */
  ara::crypto::cryp::PublicKey::Uptrc key_;
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_ED25519_SIGNATURE_VERIFIER_H_

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
/*!        \file  ed25519_signature_generator.h
 *        \brief  This file contains the declaration for Ed25519SignatureGenerator.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_ED25519_SIGNATURE_GENERATOR_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_ED25519_SIGNATURE_GENERATOR_H_

#include <memory>
#include <utility>
#include "ara/core/vector.h"
#include "ara/crypto/cryp/private_key.h"
#include "ara/crypto/cryp/signer_private_ctx.h"
#include "crypto_adapter_interface.h"
#include "crypto_adapter_types.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief This generator uses a Ed25519 signer context with a private key.
 * \details ECDSA (Elliptic curve digital signature algorithm) Ed25519.
 */
class Ed25519SignatureGenerator final : public SignatureGeneratorInterface {
 public:
  Ed25519SignatureGenerator(Ed25519SignatureGenerator const&) = delete;

  Ed25519SignatureGenerator(Ed25519SignatureGenerator&&) = delete;

  /*! \brief Default destructor. */
  ~Ed25519SignatureGenerator() noexcept final = default;

  Ed25519SignatureGenerator& operator=(Ed25519SignatureGenerator const&) = delete;

  Ed25519SignatureGenerator& operator=(Ed25519SignatureGenerator&&) = delete;

  /*!
   * \brief Constructor, takes ownership over the passed arguments.
   * \param[in] signer_ctx A unique pointer to a SignerPrivateCtx.
   * \param[in] key A unique pointer to a constant PrivateKey.
   * \pre SignerPrivateCtx and PrivateKey shall not be nullptr.
   */
  Ed25519SignatureGenerator(ara::crypto::cryp::SignerPrivateCtx::Uptr signer_ctx,
                            ara::crypto::cryp::PrivateKey::Uptrc key)
      : SignatureGeneratorInterface(), signer_ctx_{std::move(signer_ctx)}, key_{std::move(key)} {
    if ((signer_ctx_ == nullptr) || (key_ == nullptr)) {
      amsr::core::Abort("Ed25519Generator shall not be created with nullptr arguments!");
    }
  }

  /*!
   * \copydoc SignatureGeneratorInterface::GenerateSignature
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if Signature Generation fails.
   */
  amsr::core::Result<Signature, CryptoAdapterErrorCode> GenerateSignature(RawData const& raw_data) const final;

 private:
  /*!
   * \brief The Signer context used to create digital signatures.
   */
  ara::crypto::cryp::SignerPrivateCtx::Uptr signer_ctx_;
  /*!
   * \brief The private key which is set on the Signer context.
   */
  ara::crypto::cryp::PrivateKey::Uptrc key_;
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_ED25519_SIGNATURE_GENERATOR_H_

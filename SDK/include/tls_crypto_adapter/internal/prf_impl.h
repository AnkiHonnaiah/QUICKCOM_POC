/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   prf_impl.h
 *        \brief  This file contains the declaration of PrfImpl class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_PRF_IMPL_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_PRF_IMPL_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include "amsr/core/string_view.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/key.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "tls_crypto_adapter/internal/crypto_adapter_error_definitions.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"
#include "tls_crypto_adapter/internal/crypto_adapter_types.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief Pseudo Random Function(PRF) implementation.
 * \vprivate This class is component private.
 */
class PrfImpl final : public ::amsr::tls_crypto_adapter::internal::PrfInterface {
 public:
  /*! \brief Upper limit for the number of bytes the PRF can currently generate. */
  static constexpr std::size_t kMaxNumByte{256U};

  /*!
   * \brief Constructs an object of PrfImpl type.
   * \pre key_derive_function and input_key cannot be nullptr. The TrustedContainer of the current key cannot be
   * nullptr.
   * \param[in] key_derive_function Key derivation function primitive.
   * \param[in] input_key Key primitive.
   * \context Any
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  explicit PrfImpl(ara::crypto::cryp::KeyDerivationFunctionCtx::Uptr key_derive_function,
                   ara::crypto::cryp::Key::Uptrc input_key);

  /*!
   * \copydoc PrfInterface::Generate
   * \pre Key blocks from the derived key cannot be nullptr
   * \error EnCryptoAdapterErrors::kInvalidArgument is returned if any of the arguments are invalid
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if provided domain parameters object is not in the
   *        completed state or its content (at least algorithm) is incompatible with algId or permissions associated
   *        with an object presented by the secret argument disallow its usage in this context.
   * \return If successful returns kCryptoAdapterErrOk, otherwise any of the other described error codes
   * \vprivate This method is component private.
   */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode Generate(
      amsr::core::StringView label, ::amsr::tls_crypto_adapter::internal::ReadOnlyBufferView seed,
      ::amsr::tls_crypto_adapter::internal::WriteableBufferView destination) const noexcept final;

 private:
  /*!
   * \brief Transfers a crypto key to a different crypto provider.
   * \param[in] input_key The key to transfer.
   * \param[in] target_owner A reference to the crypto provider which the key shall be transferred to.
   * \return The key with the target crypto provider.
   * \context Any
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  static ara::crypto::cryp::Key::Uptrc TransferKey(ara::crypto::cryp::Key::Uptrc input_key,
                                                   ara::crypto::cryp::CryptoProvider& target_owner) noexcept;

 private:
  /*! Algorithm name used as crypto algorithm ID */
  amsr::core::StringView const kAlgNameKeyBlock{"TLS.v1_2_KEYBLOCK-2048"};
  /*! Crypto algoritm ID used to derive the key block */
  ara::crypto::CryptoAlgId alg_id_keyblock_{ara::crypto::kAlgIdUndefined};
  /*! A unique pointer to Key Derivation Function interface */
  ara::crypto::cryp::KeyDerivationFunctionCtx::Uptr key_derive_function_ctx_{nullptr};
  /*! A uniqe pointer to Generalized Key object interface */
  ara::crypto::cryp::Key::Uptrc prf_key_{nullptr};
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_PRF_IMPL_H_

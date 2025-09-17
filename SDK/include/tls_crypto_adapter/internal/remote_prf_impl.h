/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file remote_prf_impl.h
 *        \brief Class declaration of "RemotePrfImpl".
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_REMOTE_PRF_IMPL_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_REMOTE_PRF_IMPL_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "ara/crypto/cryp/key.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "tls_crypto_adapter/internal/crypto_adapter_error_definitions.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief Pseudo Random Function (PRF) implementation.
 * \vprivate This class is component private.
 */
class RemotePrfImpl final : public ::amsr::tls_crypto_adapter::internal::PrfInterface {
 public:
  /*!
   * \brief Constructs an object of "PrfImpl" type.
   * \param[in] key_derive_function Key derivation function primitive.
   * \param[in] input_key Key primitive.
   * \pre None of input arguments shall be a "nullptr".
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  explicit RemotePrfImpl(ara::crypto::cryp::KeyDerivationFunctionCtx::Uptr key_derive_function,
                         ara::crypto::cryp::Key::Uptrc input_key);

  /*!
   * \copydoc PrfInterface::Generate
   * \error "EnCryptoAdapterErrors::kInvalidArgument" is returned if any of the arguments are invalid.
   * \error "EnCryptoAdapterErrors::kCryptoProvideException" is returned if any error occurred during a crypto
   *        operation.
   * \return If successful, returns "kCryptoAdapterErrOk", otherwise any of the other described error codes.
   * \vprivate This method is component private.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode Generate(
      amsr::core::StringView label, ::amsr::tls_crypto_adapter::internal::ReadOnlyBufferView seed,
      ::amsr::tls_crypto_adapter::internal::WriteableBufferView destination) const noexcept final;

 private:
  /*!
   * \brief A unique pointer to the key derivation function interface.
   */
  ara::crypto::cryp::KeyDerivationFunctionCtx::Uptr key_derive_function_ctx_{nullptr};

  /*!
   * \brief A unique pointer to generalized key object interface.
   */
  ara::crypto::cryp::Key::Uptrc prf_key_{nullptr};
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_REMOTE_PRF_IMPL_H_

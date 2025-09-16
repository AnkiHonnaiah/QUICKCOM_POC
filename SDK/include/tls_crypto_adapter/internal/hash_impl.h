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
/*!       \file   hash_impl.h
 *        \brief  This file contains the declaration of HashImpl class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_HASH_IMPL_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_HASH_IMPL_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "ara/core/vector.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "crypto_adapter_error_definitions.h"
#include "crypto_adapter_interface.h"
#include "crypto_adapter_types.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief Hash implementation.
 * \vprivate This class is component private.
 */
class HashImpl final : public ::amsr::tls_crypto_adapter::internal::HashInterface {
 public:
  /*!
   * \brief Constructs an object of HashImpl type
   * \context Any
   * \pre hash_function must be valid and cannot be a nullptr
   * \param[in] hash_function A unique pointer to hash function interface
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  explicit HashImpl(ara::crypto::cryp::HashFunctionCtx::Uptr hash_function);

  /*!
   * \copydoc HashInterface::Start
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if the call to start() on the current hash function has
   * encountered a runtime error.
   * \return If successful returns kCryptoAdapterErrOk, otherwise any of the other described error codes
   * \vprivate This method is component private.
   */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode Start() noexcept(true) final;

  /*!
   * \copydoc HashInterface::Update
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if a call to update on the current hash function with the
   * provided data has encountered a runtime error.
   * \return If successful returns kCryptoAdapterErrOk, otherwise any of the other described error codes
   * \vprivate This method is component private.
   */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode Update(
      ::amsr::tls_crypto_adapter::internal::ReadOnlyBufferView data) noexcept(true) final;

  /*!
   * \copydoc HashInterface::Finish
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if a call to finish() or getDigest() on the current
   * hash function has encountered a runtime error.
   * \return If successful returns a read only buffer view of destination buffer with the used part, otherwise an
   * ErrorCode containing error information
   * \vprivate This method is component private.
   */
  amsr::core::Result<::amsr::tls_crypto_adapter::internal::ReadOnlyBufferView,
                     ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode>
  Finish(size_t buffer_size) noexcept(true) final;

 private:
  /*! A buffer for storing the hash operation result in HashImpl::Finish */
  ara::core::Vector<uint8_t> buffer_{};

  /*! A unique pointer to the hash function interface */
  ara::crypto::cryp::HashFunctionCtx::Uptr hash_function_{nullptr};
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_HASH_IMPL_H_

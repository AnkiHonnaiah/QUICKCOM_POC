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
/*!       \file   mac_generator_impl.h
 *        \brief  This file contains the declaration of MacGeneratorImpl class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_MAC_GENERATOR_IMPL_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_MAC_GENERATOR_IMPL_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "ara/core/vector.h"
#include "ara/crypto/cryp/message_authn_code_ctx.h"
#include "ara/crypto/cryp/symmetric_key.h"
#include "crypto_adapter_error_definitions.h"
#include "crypto_adapter_interface.h"
#include "crypto_adapter_types.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief MAC generator implementation.
 * \vprivate This class is component private.
 */
class MacGeneratorImpl final : public ::amsr::tls_crypto_adapter::internal::MacGeneratorInterface {
 public:
  /*!
   * \brief Constructs an object of MacGeneratorImpl type
   * \pre mac and key cannot be nullptr and the key must be compatible with the mac context
   * \param[in] mac A unique pointer to Keyed Message Authentication Code Context interface
   * \param[in] key A unique pointer to the symmetric key algorithm context
   * \context Any
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  MacGeneratorImpl(ara::crypto::cryp::MessageAuthnCodeCtx::Uptr mac, ara::crypto::cryp::SymmetricKey::Uptrc key);

  /*!
   * \copydoc MacGeneratorInterface::Start
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if the call to start() on the current mac generator has
   * encountered a runtime error.
   * \return If successful returns kCryptoAdapterErrOk, otherwise any of the other described error codes
   * \vprivate This method is component private.
   */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode Start() noexcept(true) final;

  /*!
   * \copydoc MacGeneratorInterface::Update
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if a call to update on the current mac generator with the
   * provided data has encountered a runtime error.
   * \return If successful returns kCryptoAdapterErrOk, otherwise any of the other described error codes
   * \vprivate This method is component private.
   */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode Update(
      ::amsr::tls_crypto_adapter::internal::ReadOnlyBufferView data) noexcept(true) final;

  /*!
   * \copydoc MacGeneratorInterface::Finish
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if a call to finish() or getDigest() on the current
   * mac generator has encountered a runtime error.
   * \return If successful returns a read only buffer view of destination buffer with the used part, otherwise an
   * ErrorCode containing error information
   * \vprivate This method is component private.
   */
  amsr::core::Result<::amsr::tls_crypto_adapter::internal::ReadOnlyBufferView,
                     ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode>
  Finish(size_t buffer_size) noexcept(true) final;

  /*!
   * \copydoc MacGeneratorInterface::Generate
   * \error EnCryptoAdapterErrors::kRuntimeError is returned if a call to finish() or getDigest() on the current
   * mac generator has encountered a runtime error.
   * \return If successful returns a read only buffer view of destination buffer with the used part, otherwise an
   * ErrorCode containing error information
   * \vprivate This method is component private.
   */
  amsr::core::Result<ReadOnlyBufferView> Generate(ReadOnlyBufferView data, std::size_t buffer_size) noexcept final;

 private:
  /*! A buffer to store the generated MAC within MacGeneratorImpl::Finish() */
  ara::core::Vector<uint8_t> buffer_{};

  /*! A unique pointer to the MAC context */
  ara::crypto::cryp::MessageAuthnCodeCtx::Uptr mac_;

  /*! A unique pointer to the key used for creating the MAC context */
  ara::crypto::cryp::SymmetricKey::Uptrc key_;
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_MAC_GENERATOR_IMPL_H_

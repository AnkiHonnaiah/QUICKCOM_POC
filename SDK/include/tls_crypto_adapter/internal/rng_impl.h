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
/*!       \file   rng_impl.h
 *        \brief  This file contains the declaration of RngImpl class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_RNG_IMPL_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_RNG_IMPL_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "crypto_adapter_error_definitions.h"
#include "crypto_adapter_interface.h"
#include "crypto_adapter_types.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief Random Number Generator(RNG) implementation.
 * \vprivate This class is component private.
 */
class RngImpl : public ::amsr::tls_crypto_adapter::internal::RngInterface {
 public:
  /*!
   * \brief Constructs an object of RngImpl type
   * \pre rng must be valid and cannot be a nullptr
   * \param[in] rng A unique pointer to Random Number Generator (RNG) Context interface
   * \context Any
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  explicit RngImpl(ara::crypto::cryp::RandomGeneratorCtx::Sptr rng);

  /*!
   * \copydoc RngInterface::Generate
   * \vprivate This method is component private.
   */
  ::amsr::tls_crypto_adapter::internal::CryptoAdapterErrorCode Generate(
      ::amsr::tls_crypto_adapter::internal::WriteableBufferView destination) noexcept override;

 private:
  /*! A shared smart pointer to Random Number Generator interface */
  ara::crypto::cryp::RandomGeneratorCtx::Sptr const rng_;
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_RNG_IMPL_H_

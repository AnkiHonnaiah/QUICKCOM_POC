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
/*!        \file  ara/crypto/cryp/random_generator_ctx.h
 *        \brief  Random Number Generator (RNG) Context interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/stream_starter.h"
#include "ara/crypto/cryp/symmetric_key_context.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Random Number Generator (RNG) Context interface.
 * \details Any user of this interface should create shared pointers
 *          to it only by calls of the method shared_from_this()!
 * \vpublic
 */
class RandomGeneratorCtx : public std::enable_shared_from_this<RandomGeneratorCtx>,
                           public SymmetricKeyContext,
                           public StreamStarter {
 public:
  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<RandomGeneratorCtx>;

  /*!
   * \brief Update internal state of the RNG by additional entropy.
   * \param[in] entropy A memory region with the additional entropy value.
   * \return true if the method is supported.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool AddEntropy(ReadOnlyMemRegion entropy) noexcept = 0;

  /*!
   * \brief Fill whole provided buffer by generated random sequence.
   * \param[out] output A target buffer for filling by the generated random sequence.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext if this context implements a deterministic
   *                   RNG, but it was not initialized by a key value.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Generate(WritableMemRegion output) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_RANDOM_GENERATOR_CTX_H_

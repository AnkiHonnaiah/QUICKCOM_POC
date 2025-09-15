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
/*!        \file  ara/crypto/cryp/symmetric_block_cipher_ctx.h
 *        \brief  Interface of a Symmetric Block Cipher Context with padding.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/block_cryptor.h"
#include "ara/crypto/cryp/symmetric_key_context.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Interface of a Symmetric Block Cipher Context with padding.
 * \vpublic
 */
class SymmetricBlockCipherCtx : public SymmetricKeyContext, public BlockCryptor {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<SymmetricBlockCipherCtx, CustomDeleter>;

  /*!
   * \brief Get fixed size of the input/output block of data.
   * \details GetBlockSize() == BlockCryptor::maxInputSize(true) == BlockCryptor::maxOutputSize(true).
   * \return Size of the data block in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetBlockSize() const noexcept = 0;

  /*!
   * \brief Processes provided blocks without padding.
   * \details The in and out buffers must have equal size and this
   *          size MUST be divisible by the block size (see GetBlockSize()).
   * \param[out] out An output data buffer.
   * \param[in] in An input data buffer.
   * \details The \c in and \c out buffers must have same size and this size must be divisible by the block size
   *      (see \c GetBlockSize()).
   *       Pointers to the input and output buffers must be aligned to the block-size boundary!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by
   *                the block size (see \c GetBlockSize())
   * \error SecurityErrorDomain::kIncompatibleArguments  if sizes of the input and output buffer are not
   *                equal
   * \error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers partially intersect
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ProcessBlocks(WritableMemRegion out, ReadOnlyMemRegion in) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_BLOCK_CIPHER_CTX_H_

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
/*!        \file  ara/crypto/cryp/signer_private_ctx.h
 *        \brief  Signature Private key Context interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNER_PRIVATE_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNER_PRIVATE_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/private_key_context.h"
#include "ara/crypto/cryp/signature.h"
#include "ara/crypto/cryp/signature_handler.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Signature Private key Context interface.
 * \vpublic
 */
class SignerPrivateCtx : public PrivateKeyContext, public SignatureHandler {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<SignerPrivateCtx, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Sign a provided digest value stored in the hash-function context.
   * \param[in] hash  a finalized hash-function context that contains a digest value ready for sign
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
   * \return unique smart pointer to serialized signature
   * \details This method must put the hash-function algorithm ID and a \a COUID of the used key-pair to the resulting
   *       signature object!
   * \details The user supplied \c context may be used for such algorithms as: Ed25519ctx, Ed25519ph, Ed448ph.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if hash-function algorithm does not comply with
   *                the signature algorithm specification of this context
   * \error SecurityErrorDomain::kInvalidInputSize  if the user supplied \c context has incorrect
   *                (or unsupported) size
   * \error SecurityErrorDomain::kProcessingNotFinished  if the method \c hash.Finish() was not called
   *                before the call of this method
   * \error SecurityErrorDomain::kUninitializedContext  this context was not initialized by a key value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index was not
   *                allocated
   * \error SecurityErrorDomain::kInsufficientResource  if capacity of slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<Signature::Uptrc> Sign(HashFunctionCtx const& hash,
                                                   ReservedObjectIndex reserved_index = kAllocObjectOnHeap,
                                                   ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Sign a directly provided hash or message value.
   * \param[out] signature  a buffer for the resulting signature
   * \param[in] value  the (pre-)hashed or direct message value that should be signed
   * \param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
   * \return actual size of the signature value stored to the output buffer
   * \details This method can be used for implementation of the "multiple passes" signature algorithms that process
   *       a message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
   *       implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the input \c value or \c context arguments
   *                are incorrect / unsupported
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output \c signature buffer
   *                is not enough
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> Sign(WritableMemRegion signature, ReadOnlyMemRegion value,
                                              ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Sign a directly provided hash or message value.
   * \tparam Alloc  a custom allocator type of the output container
   * \param[out] signature  pre-reserved managed container for resulting signature
   * \param[in] value  the (pre-)hashed or direct message value that should be signed
   * \param[in] context  an optional user supplied "context" (its support depends from concrete algorithm)
   * \details This method can be used for implementation of the "multiple passes" signature algorithms that process
   *       a message directly, i.e. without "pre-hashing" (like Ed25519ctx). But also this method is suitable for
   *       implementation of the traditional signature schemes with pre-hashing (like Ed25519ph, Ed448ph, ECDSA).
   * \details This method sets the size of the output container according to actually saved value!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the input \c value or \c context arguments
   *                are incorrect / unsupported
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output \c signature container
   *                is not enough
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Resize the given signature buffer
   * - Sign the given buffer and copy the signature
       into the signature buffer
   * - If computing the signature succeeds
   *   - Resize the signature buffer to the size
         of the signature
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc = DefBytesAllocator>
  ara::core::Result<void> Sign(ByteVectorT<Alloc>& signature, ReadOnlyMemRegion value,
                               ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept {
    signature.resize(signature.capacity());
    ara::core::Result<std::size_t> signature_size{Sign(WritableMemRegion{signature}, value, context)};
    ara::core::Result<void> result{};
    if (signature_size) {
      signature.resize(signature_size.Value());
    } else {
      result.EmplaceError(signature_size.Error());
    }
    return result;
  }
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SIGNER_PRIVATE_CTX_H_

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
/*!        \file  ara/crypto/cryp/symmetric_key_wrapper_ctx.h
 *        \brief  Interface of a symmetric key wrap algorithm (for AES it should be compatible with RFC3394 or RFC5649).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/key.h"
#include "ara/crypto/cryp/key_material.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/symmetric_key_context.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Interface of a symmetric key wrap algorithm (for AES it should be compatible with RFC3394 or RFC5649).
 * \details Key Wrapping of a whole key object (including associated meta-data) can be done
 *          by exportXxx() / importXxx() methods, but without compliance to RFC3394 or RFC5649.
 * \vpublic
 */
class SymmetricKeyWrapperCtx : public SymmetricKeyContext {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<SymmetricKeyWrapperCtx, CustomDeleter>;

  /*!
   * \brief Get expected granularity of the target key (block size).
   * \details If the class implements RFC3394 (KW without padding)
   *          then this method should return 8 (i.e. 8 octets = 64 bits).
   * \details If the class implements RFC5649 (KW with padding)
   *          then this method should return 1 (i.e. 1 octet = 8 bits).
   * \return Size of the block in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetTargetKeyGranularity() const noexcept = 0;

  /*!
   * \brief Get maximum length of the target key supported by the implementation.
   * \details This method can be useful for some implementations different from RFC3394 / RFC5649.
   * \return Maximum length of the target key in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetMaxTargetKeyLength() const noexcept = 0;

  /*!
   * \brief Calculates size of the wrapped key in bytes from original key length in bits.
   * \details This method can be useful for some implementations different from RFC3394 / RFC5649.
   * \param[in] key_length Original key length in bits.
   * \return Size of the wrapped key in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t CalculateWrappedKeySize(std::size_t key_length) const noexcept = 0;

  /*!
   * \brief Execute the "key wrap" operation for the provided key material.
   * \param[out] wrapped  an output buffer for the wrapped key
   * \param[in] key  a key that should be wrapped
   * \details This method should be compliant to RFC3394 or RFC5649, if an implementation is based on the AES block
   *       cipher and applied to an AES key.
   * \details Method \c CalculateWrappedKeySize() can be used for size calculation of the required output buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity  if the size of the \c wrapped buffer is not enough
   *                for storing the result
   * \error SecurityErrorDomain::kInvalidInputSize  if the \c key object has an unsupported length
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> WrapKeyMaterial(WritableMemRegion wrapped, KeyMaterial const& key) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Execute the "key unwrap" operation for provided BLOB and produce \c SecretSeed object.
   * \param[in] wrapped_seed  a memory region that contains wrapped seed
   * \param[in] target_alg_id  the target symmetric algorithm identifier (also defines a target seed-length)
   * \param[in] allowed_usage  allowed usage scope of the target seed
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap.
   * \return unique smart pointer to \c SecretSeed object, which keeps unwrapped key material
   * \details This method should be compliant to RFC3394 or RFC5649, if implementation is based on the AES block cipher
   *       and applied to an AES key material.
   * \details The created \c SecretSeed object has following attributes: session and non-exportable (because it was
   *       imported without meta-information).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidInputSize  if the size of provided wrapped seed is unsupported
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
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
  virtual ara::core::Result<SecretSeed::Uptrc> UnwrapSeed(
      ReadOnlyMemRegion wrapped_seed, AlgId target_alg_id, SecretSeed::Usage allowed_usage,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Execute the "key unwrap" operation for provided BLOB and produce \c Key object.
   * \param[in] wrapped_key  a memory region that contains wrapped key
   * \param[in] alg_id  an identifier of the target symmetric crypto algorithm
   * \param[in] allowed_usage  bit-flags that define a list of allowed transformations' types in which the target key
   *            can be used
   * \param[in] params  an optional pointer to domain parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89)
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to \c Key object, which keeps unwrapped key material
   * \details This method should be compliant to RFC3394 or RFC5649, if implementation is based on the AES block cipher
   *       and applied to an AES key.
   * \details The created \c Key object has following attributes: session and non-exportable (because it was imported
   *       without meta-information)!
   * \details If (params != nullptr) then the domain parameters object must be in the completed state (see
   *       \c DomainParameters)!
   * \details If (params != nullptr) then at least the parameters' \a COUID must be saved to the dependency
   *       field of the produced key object.
   * \details \c SymmetricKey may be unwrapped in following way:
   *          SymmetricKey::Uptrc key = SymmetricKey::Cast(UnwrapKey(wrapped_key, ...)); \n
   *       \c PrivateKey may be unwrapped in following way:
   *          PrivateKey::Uptrc key = PrivateKey::Cast(UnwrapKey(wrapped_key, ...)); \n
   *       In both examples the \c Cast() method may additionally \b throw the \c BadObjectTypeException if an
   *       actual type of the unwrapped key differs from the target one!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidInputSize  if the size of provided wrapped key is unsupported
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but provided domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object has an incomplete state
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
  virtual ara::core::Result<Key::Uptrc> UnwrapKey(
      ReadOnlyMemRegion wrapped_key, AlgId alg_id, AllowedUsageFlags allowed_usage,
      DomainParameters::Sptrc params = nullptr,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  /*!
   * \brief Execute the "key unwrap" operation for provided BLOB and produce a Key object of expected type.
   * \tparam ExpectedKey  the expected type of concrete key
   * \param[in] wrapped_key  a memory region that contains wrapped key
   * \param[in] alg_id  an identifier of the target symmetric crypto algorithm
   * \param[in] allowed_usage  bit-flags that define a list of allowed transformations' types in which the target key
   *            can be used
   * \param[in] params  an optional pointer to domain parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89)
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to \c ExpectedKey object, which keeps unwrapped key material
   * \details For additional details see \c UnwrapKey()
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidInputSize  if the size of provided wrapped key is unsupported
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but provided domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object has an incomplete state
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
  template <typename ExpectedKey>
  auto UnwrapConcreteKey(ReadOnlyMemRegion wrapped_key, AlgId alg_id, AllowedUsageFlags allowed_usage,
                         DomainParameters::Sptrc params = nullptr,
                         ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept
      -> ara::core::Result<typename ExpectedKey::Uptrc> {
    return UnwrapKey(wrapped_key, alg_id, allowed_usage, params, reserved_index).AndThen([](Key::Uptrc key) {
      return ExpectedKey::Cast(std::move(key));
    });
  }
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SYMMETRIC_KEY_WRAPPER_CTX_H_

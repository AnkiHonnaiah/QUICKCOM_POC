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
/*!        \file  ara/crypto/cryp/public_key.h
 *        \brief  General Asymmetric Public Key interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PUBLIC_KEY_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PUBLIC_KEY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/key.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief General Asymmetric Public Key interface.
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.2.1: MD_CRYPTO_AutosarC++17_10-M10.2.1_unique_names
class PublicKey : public Key, public Serializable {
 public:
  /*!
   * \brief Static mapping of this interface to specific value of \c KeyType enumeration.
   * \vpublic
   */
  static KeyType const sKeyType{KeyType::kPublicKey};

  /*!
   * \brief Static mapping of this interface to specific value of \c CryptoObjectType enumeration.
   * \vpublic
   */
  static CryptoObjectType const sObjectType{CryptoObjectType::kPublicKey};

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<PublicKey const, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Checks the key for its correctness.
   * \param[in] strong_check The severeness flag that indicates type of the required check:
                strong (if \c true) or fast (if \c false).
   * \return true if the key is correct.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool CheckKey(bool strong_check = true) const noexcept = 0;

  /*!
   * \brief Calculate hash of the Public Key value.
   * \param[out] hash  a buffer preallocated for the resulting hash value
   * \param[in] hash_func  a hash-function instance that should be used the hashing
   * \return actual size of the hash value stored to the output buffer
   * \details The original public key value BLOB is available via the \c Serializable interface.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity  if size of the hash buffer is not enough for
   *                storing of the result
   * \error SecurityErrorDomain::kIncompleteArgState  if the \c hash_func context is not initialized by
   *                required domain parameters
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> HashPublicKey(WritableMemRegion hash,
                                                       HashFunctionCtx& hash_func) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Calculate hash of the Public Key value.
   * \tparam Alloc  a custom allocator type of the output container
   * \param[out] hash  pre-reserved managed container for the resulting hash value
   * \param[in] hash_func  a hash-function instance that should be used the hashing
   * \details This method sets the size of the output container according to actually saved value!
   * \details The original public key value BLOB is available via the Serializable interface.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the hash buffer is not enough for storing of the
   * result
   * \error SecurityErrorDomain::kIncompleteArgState  if the \c hash_func context is not initialized by required
   * domain parameters
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Resize the given output buffer
   * - Hash the the public key and copy the digest
   *   into the output buffer
   * - If computing the hash succeeds
   *   - Resize the output buffer to the size
         of the digest
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc = DefBytesAllocator>
  ara::core::Result<void> HashPublicKey(ByteVectorT<Alloc>& hash, HashFunctionCtx& hash_func) const noexcept {
    hash.resize(hash.capacity());
    ara::core::Result<std::size_t> size_result{HashPublicKey(WritableMemRegion{hash}, hash_func)};
    ara::core::Result<void> result{};
    if (size_result) {
      hash.resize(size_result.Value());
    } else {
      result.EmplaceError(size_result.Error());
    }
    return result;
  }
  /*!
   * \brief Downcast and move unique smart pointer of the base \c Key interface to the \c PublicKey one.
   * \param[in] key  unique smart pointer to the constant \c Key interface
   * \return unique smart pointer to downcasted constant interface \c PublicKey
   * \error SecurityErrorDomain::kBadObjectType  if an actual type of the \c key is not \c PublicKey::Uptrc
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  static ara::core::Result<PublicKey::Uptrc> Cast(Key::Uptrc&& key) noexcept;
};

inline ara::core::Result<PublicKey::Uptrc> PublicKey::Cast(Key::Uptrc&& key) noexcept {
  return Downcast<PublicKey>(std::move(key));
}

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PUBLIC_KEY_H_

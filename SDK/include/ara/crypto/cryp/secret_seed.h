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
/*!        \file  ara/crypto/cryp/secret_seed.h
 *        \brief  Secret Seed object interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SECRET_SEED_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SECRET_SEED_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"

#include "ara/crypto/cryp/crypto_object.h"
#include "ara/crypto/cryp/key_material.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/restricted_use_object.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Secret Seed object interface.
 * \details This object contains a raw bit sequence of specific length
 *          (without any filtering of allowed/disallowed values)!
 * \details The secret seed value can be loaded only to a non-key input
 *          of a cryptographic transformation context (like IV/salt/nonce)!
 * \details Bit length of the secret seed is specific to concrete crypto algorithm
 *          and corresponds to maximum of its input/output/salt block-length.
 * \vpublic
 */
class SecretSeed : public KeyMaterial, public RestrictedUseObject {
 public:
  /*!
   * \brief Static mapping of this interface to specific value of \c CryptoObjectType enumeration.
   * \vpublic
   */
  static CryptoObjectType const sObjectType{CryptoObjectType::kSecretSeed};

  /*!
   * \brief Unique smart pointer of a constant interface instance.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<SecretSeed const, CustomDeleter>;

  /*!
   * \brief Unique smart pointer of a volatile interface instance.
   * \vpublic
   */
  using Uptr = std::unique_ptr<SecretSeed, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Clone this Secret Seed object to new session object.
   * \param[in] xor_delta  optional "delta" value that must be XOR-ed with the "cloned" copy of the original seed
   * \param[in] reserved_index  the optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to "cloned" session \c SecretSeed object
   * \note Created object instance is session and non-exportable, \c AllowedUsageFlags attribute of the "cloned"
   *       object is identical to this attribute of the source object!
   * \note If size of the \c xor_delta argument is less than the value size of this seed then only correspondent
   *       number of leading bytes of the original seed should be XOR-ed, but the rest should be copied without
   *       change.
   *       If size of the \c xor_delta argument is larger than the value size of this seed then extra bytes of
   *       the \c xor_delta should be ignored.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
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
  virtual ara::core::Result<SecretSeed::Uptr> Clone(
      ara::crypto::ReadOnlyMemRegion xor_delta = ReadOnlyMemRegion{},
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Set value of this seed object as a "jump" from an initial state to specified number of steps,
   *           according to "counting" expression defined by a cryptographic algorithm associated with this object.
   * \param[in] from  source object that keeps the initial value for jumping from
   * \param[in] steps  number of steps for the "jump"
   * \return reference to this updated object
   * \note \c steps may have positive and negative values that correspond to forward and backward direction of the
   *       "jump" respectively, but 0 value means only copy \c from value to this seed object.
   * \note Seed size of the \c from argument always must be greater or equal of this seed size!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleObject  if this object and the \c from argument are associated
   *                with incompatible cryptographic algorithms
   * \error SecurityErrorDomain::kInvalidInputSize  if value size of the \c from seed is less then
   *                value size of this one
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<SecretSeed&> JumpFrom(SecretSeed const& from, std::int64_t steps) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Set next value of the secret seed according to "counting" expression defined by a cryptographic
   *           algorithm associated with this object.
   * \return reference to this updated object
   * \note If the associated cryptographic algorithm doesn't specify a "counting" expression then generic increment
   *       operation must be implemented as default (little-endian notation, i.e. first byte is least significant).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual SecretSeed& Next() noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Set value of this seed object as a "jump" from it's current state to specified number of steps,
   *           according to "counting" expression defined by a cryptographic algorithm associated with this object.
   * \param[in] steps  number of "steps" for jumping (forward or backward) from the current state
   * \return reference to this updated object
   * \note \c steps may have positive and negative values that correspond to forward and backward direction of the
   *       "jump" respectively, but 0 value means no changes of the current seed value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual SecretSeed& Jump(std::int64_t steps) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  // Attention! Deviation: Replaced operator overloading with normal function to comply with C++ language restrictions
  /*!
   * \brief XOR value of this seed object with another one and save result to this object.
   * \param[in] source  right argument for the XOR operation
   * \return reference to this updated object
   * \note If seed sizes in this object and in the \c source argument are different then only correspondent number
   *       of leading bytes in this seed object should be updated.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual SecretSeed& Xor(SecretSeed const& source) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  // Attention! Deviation: Replaced operator overloading with normal function to comply with C++ language restrictions
  /*!
   * \brief XOR value of this seed object with provided memory region and save result to this object.
   * \param[in] source  right argument for the XOR operation
   * \return reference to this updated object
   * \note If seed sizes in this object and in the \c source argument are different then only correspondent number
   *       of leading bytes of this seed object should be updated.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual SecretSeed& Xor(ara::crypto::ReadOnlyMemRegion source) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_SECRET_SEED_H_

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
/*!        \file  ara/crypto/cryp/key_decapsulator_private_ctx.h
 *        \brief  Asymmetric Key Encapsulation Mechanism (KEM) Private key Context interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DECAPSULATOR_PRIVATE_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DECAPSULATOR_PRIVATE_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/key_encapsulator.h"
#include "ara/crypto/cryp/private_key_context.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/symmetric_key.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Asymmetric Key Encapsulation Mechanism (KEM) Private key Context interface.
 * \vpublic
 */
class KeyDecapsulatorPrivateCtx : public PrivateKeyContext, public KeyEncapsulator {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<KeyDecapsulatorPrivateCtx, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Decapsulates key material.
   * \param[in] input  a buffer with the encapsulated seed (its size should be equal \c GetEncapsulatedSize() bytes)
   * \param[in] allowed_usage  the allowed usage scope of the target seed
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to \c SecretSeed object, which keeps the key material decapsulated from the input
   *          buffer
   * \details Returned Key Material object should be used for derivation of a symmetric key.
   * \details Produced \c SecretSeed object has following attributes: session, non-exportable, AlgID = this KEM AlgID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a private key
   *                value
   * \error SecurityErrorDomain::kInsufficientCapacity  if the \c output.size() is not enough to save
   *                the decapsulation result
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
  virtual ara::core::Result<SecretSeed::Uptrc> DecapsulateSeed(
      ReadOnlyMemRegion input, AllowedUsageFlags allowed_usage = kAllowKdfMaterialAnyUsage,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Decapsulate Key Encryption Key (KEK).
   * \param[in] input  an input buffer (its size should be equal \c GetEncapsulatedSize() bytes)
   * \param[in] kdf  a context of a key derivation function, which should be used for the target KEK production
   * \param[in] kek_alg_id  an algorithm ID of the target KEK
   * \param[in] salt  an optional salt value (if used, it should be unique for each instance of the target key)
   * \param[in] ctx_label  an pptional application specific "context label" (it can identify purpose of the target
   *            key and/or communication parties)
   * \param[in] params  an optional pointer to domain parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89)
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to a symmetric key object derived from a key material decapsulated from the input
   *          block
   * \details Produced \c SymmetricKey object has following attributes: session, non-exportable, Key Usage:
   *       \c kAllowKeyImporting.
   * \details If (params != nullptr) then the domain parameters object must be in the completed state
   *       (see \c DomainParameters)!
   * \details If (params != nullptr) then at least the parameters' COUID must be saved to the dependency field
   *       of the generated key object.
   * \details This method can be used for direct production of the target key, without creation of the intermediate
   *       \c SecretSeed object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a private key
   *                value
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c kek_alg_id specifies incorrect algorithm
   * \error SecurityErrorDomain::kInvalidInputSize  if (input.size() <> this->GetEncapsulatedSize())
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr), but provided domain
   *                parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr), but provided domain
   *                parameters object has an incomplete state
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
  virtual ara::core::Result<SymmetricKey::Uptrc> DecapsulateKey(
      ReadOnlyMemRegion input, KeyDerivationFunctionCtx& kdf, AlgId kek_alg_id,
      ReadOnlyMemRegion salt = ReadOnlyMemRegion(), ReadOnlyMemRegion ctx_label = ReadOnlyMemRegion(),
      DomainParameters::Sptrc params = nullptr,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DECAPSULATOR_PRIVATE_CTX_H_

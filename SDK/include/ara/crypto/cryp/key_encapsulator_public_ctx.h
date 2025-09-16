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
/*!        \file  ara/crypto/cryp/key_encapsulator_public_ctx.h
 *        \brief  Asymmetric Key Encapsulation Mechanism (KEM) Public key Context interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_PUBLIC_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_PUBLIC_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "ara/crypto/cryp/key_encapsulator.h"
#include "ara/crypto/cryp/public_key_context.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/symmetric_key.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Asymmetric Key Encapsulation Mechanism (KEM) Public key Context interface.
 * \vpublic
 */
class KeyEncapsulatorPublicCtx : public PublicKeyContext, public KeyEncapsulator {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<KeyEncapsulatorPublicCtx, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Encapsulate key material (secret seed).
   * \param[out] output  an output buffer (its size should be at least \c GetEncapsulatedSize() bytes)
   * \param[out] out_size  a variable for getting the actual size of output data (encapsulated key) in bytes
   * \param[in] allowed_usage  the allowed usage scope of the target seed
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to \c SecretSeed object, which keeps the randomly renerated key material
   *          encapsulated to the output buffer
   * \details Returned key material should be used for derivation of a symmetric key.
   * \details Only first \c GetEncapsulatedSize() bytes of the output buffer can be updated by this method.
   * \details Produced \c SecretSeed object has following attributes: session, non-exportable, AlgID = this KEM AlgID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by
   *                a public key value
   * \error SecurityErrorDomain::kInsufficientCapacity  if the \c output.size() is not enough to save
   *                the encapsulation result
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
  virtual ara::core::Result<SecretSeed::Uptrc> EncapsulateSeed(
      WritableMemRegion output, std::size_t& out_size, AllowedUsageFlags allowed_usage = kAllowKdfMaterialAnyUsage,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Encapsulate key material (secret seed).
   * \tparam Alloc  a custom allocator type of the output container
   * \param[out] output  a managed container for output data, i.e. for the "encapsulated key" (its capacity should
   *             be at least \c GetEncapsulatedSize() bytes)
   * \param[in] allowed_usage  allowed usage scope of the target seed
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to \c SecretSeed object, which keeps the randomly renerated key material
   *          encapsulated to the output buffer
   * \details Returned key material should be used for derivation of a symmetric key.
   * \details This method sets the size of the output container according to actually saved value.
   * \details Produced \c SecretSeed object has following attributes: session, non-exportable, AlgID = this KEM AlgID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by
   *                a public key value
   * \error SecurityErrorDomain::kInsufficientCapacity  if the \c output.size() is not enough to save
   *                the encapsulation result
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
  /*!
   * \internal
   * - If the output capacity is less than the fixed encapsulated seed size
   *   - Resize the output to the fixed size
   * - Encapsulate the seed
   * - If encapsulating the seed succeeds
   *   - Resize the output to the encapsulated seed size
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc = DefBytesAllocator>
  ara::core::Result<SecretSeed::Uptrc> EncapsulateSeed(
      ByteVectorT<Alloc>& output, SecretSeed::Usage allowed_usage = kAllowKdfMaterialAnyUsage,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept {
    if (GetEncapsulatedSize() <= output.capacity()) {
      output.resize(GetEncapsulatedSize());
    }
    std::size_t out_size{0};
    ara::core::Result<SecretSeed::Uptrc> encapsulated_seed{
        EncapsulateSeed(WritableMemRegion{output}, out_size, allowed_usage, reserved_index)};
    ara::core::Result<SecretSeed::Uptrc> result{nullptr};
    if (encapsulated_seed) {
      output.resize(out_size);
      result.EmplaceValue(std::move(encapsulated_seed).Value());
    } else {
      result.EmplaceError(encapsulated_seed.Error());
    }
    return result;
  }

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Encapsulate Key Encryption Key (KEK).
   * \param[out] output  an output buffer (its size should be at least \c GetEncapsulatedSize() bytes)
   * \param[out] out_size  a variable for getting the actual size of output data (encapsulated key) in bytes
   * \param[in] kdf  a context of a key derivation function, which should be used for the target KEK production
   * \param[in] kek_alg_id  an algorithm ID of the target KEK
   * \param[in] salt  an optional salt value (if used, it should be unique for each instance of the target key)
   * \param[in] ctx_label  an optional application specific "context label" (it can identify purpose of the target key
   *            and/or communication parties)
   * \param[in] params  an optional pointer to domain parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89)
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to a symmetric key object derived from a randomly renerated material encapsulated to
   *          the output buffer
   * \details Only first \c GetEncapsulatedSize() bytes of the output buffer should be updated by this method.
   * \details Produced \c SymmetricKey object has following attributes: session, non-exportable, Allowed Key Usage:
   *       \c kAllowKeyExporting.
   * \details If (params != nullptr) then the domain parameters object must be in the completed state
   *       (see \c DomainParameters)!
   * \details If (params != nullptr) then at least the parameters' COUID must be saved to the dependency field
   *       of the produced key object.
   * \details This method can be used for direct production of the target key, without creation of the intermediate
   *       \c SecretSeed object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by
   *                a public key value
   * \error SecurityErrorDomain::kInvalidArgument  if \c kek_alg_id specifies incorrect algorithm
   * \error SecurityErrorDomain::kInsufficientCapacity  if the \c output.size() is not enough to save
   *                the encapsulation result
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but the domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but the domain parameters object has incomplete state
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
  virtual ara::core::Result<SymmetricKey::Uptrc> EncapsulateKey(
      WritableMemRegion output, std::size_t& out_size, KeyDerivationFunctionCtx& kdf, AlgId kek_alg_id,
      ReadOnlyMemRegion salt = ReadOnlyMemRegion(), ReadOnlyMemRegion ctx_label = ReadOnlyMemRegion(),
      DomainParameters::Sptrc params = nullptr,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Encapsulate Key Encryption Key (KEK).
   * \tparam Alloc  a custom allocator type of the output container
   * \param[out] output  a managed container for the output data, i.e. for the "encapsulated key" (its capacity
   *             should be at least \c GetEncapsulatedSize() bytes)
   * \param[in] kdf  a context of a key derivation function, which should be used for the target KEK production
   * \param[in] kek_alg_id  an algorithm ID of the target KEK
   * \param[in] salt  an optional salt value (if used, it should be unique for each instance of the target key)
   * \param[in] ctx_label  an optional application specific "context label" (it can identify purpose of the target
   *            key and/or communication parties)
   * \param[in] params  an optional pointer to domain parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89)
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to a symmetric key object derived from randomly renerated material encapsulated
   *           to the output buffer
   * \details Only first \c GetEncapsulatedSize() bytes of the output buffer should be updated by this method.
   * \details Produced \c SymmetricKey object has following attributes: session, non-exportable, Allowed Key Usage:
   *       \c kAllowKeyExporting.
   * \details If (params != nullptr) then the domain parameters object must be in the completed state
   *       (see \c DomainParameters)!
   * \details If (params != nullptr) then at least the parameters' COUID must be saved to the dependency field
   *       of the produced key object.
   * \details This method can be used for direct production of the target key, without creation of the intermediate
   *       \c SecretSeed object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by
   *                a public key value
   * \error SecurityErrorDomain::kInvalidArgument  if \c kek_alg_id specifies incorrect algorithm
   * \error SecurityErrorDomain::kInsufficientCapacity  if the \c output.size() is not enough to save
   *                the encapsulation result
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but the domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but the domain parameters object has incomplete state
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
  /*!
   * \internal
   * - If the output capacity is less than the fixed encapsulated key size
   *   - Resize the output to the fixed size
   * - Encapsulate the key
   * - If encapsulating the key succeeds
   *   - Resize the output to the encapsulated key size
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc = DefBytesAllocator>
  ara::core::Result<SymmetricKey::Uptrc> EncapsulateKey(
      ByteVectorT<Alloc>& output, KeyDerivationFunctionCtx& kdf, AlgId kek_alg_id,
      ReadOnlyMemRegion salt = ReadOnlyMemRegion(), ReadOnlyMemRegion ctx_label = ReadOnlyMemRegion(),
      DomainParameters::Sptrc params = nullptr,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept {
    if (GetEncapsulatedSize() <= output.capacity()) {
      output.resize(GetEncapsulatedSize());
    }
    std::size_t out_size{0};
    ara::core::Result<SymmetricKey::Uptrc> encapsulated_key{
        EncapsulateKey(WritableMemRegion{output}, out_size, kdf, kek_alg_id, salt, ctx_label, params, reserved_index)};
    ara::core::Result<SymmetricKey::Uptrc> result{nullptr};
    if (encapsulated_key) {
      output.resize(out_size);
      result.EmplaceValue(std::move(encapsulated_key).Value());
    } else {
      result.EmplaceError(encapsulated_key.Error());
    }
    return result;
  }
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_ENCAPSULATOR_PUBLIC_CTX_H_

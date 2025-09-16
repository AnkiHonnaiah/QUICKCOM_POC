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
/*!        \file  ara/crypto/cryp/key_agreement_private_ctx.h
 *        \brief  Key Agreement Private key Context interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/private_key_context.h"
#include "ara/crypto/cryp/public_key.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/symmetric_key.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Key Agreement Private key Context interface.
 * \vpublic
 */
class KeyAgreementPrivateCtx : public PrivateKeyContext {
 public:
  /*!
   * \brief Unique smart pointer of this interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<KeyAgreementPrivateCtx, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Produce a common secret seed via execution of the key-agreement algorithm between this private key
   *           and a public key of another side.
   * \details Produced \c SecretSeed object has following attributes: session, non-exportable, AlgID (this Key-Agreement
   *       Algorithm ID).
   * \param[in] other_side_key  the public key of the other side of the Key-Agreement
   * \param[in] allowed_usage  the allowed usage scope of the target seed
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique pointer to \c SecretSeed object, which contains the key material produced by the Key-Agreement
   *          algorithm
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kIncompatibleObject  if the public and private keys correspond to different
   *                algorithms or reference to different domain parameters
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
  virtual ara::core::Result<SecretSeed::Uptrc> AgreeSeed(
      PublicKey const& other_side_key, AllowedUsageFlags allowed_usage = kAllowKdfMaterialAnyUsage,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  // Attention! Deviation: Produced SymmetricKey object has the attribute "exportable" instead of "non-exportable" if
  //                       applicable depending on the concrete crypto provider implementation.
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_autosar_api
  /*!
   * \brief Produce a common symmetric key via execution of the key-agreement algorithm between this private key
   *           and a public key of another side.
   * \details Produced \c SymmetricKey object has following attributes: session, exportable.
   * \details This method can be used for direct production of the target key, without creation of the intermediate
   *       \c SecretSeed object.
   * \param[in] other_side_key  the public key of the other side of the Key-Agreement
   * \param[in] kdf  the Context of a Key Derivation Function, which should be used for the target key production
   * \param[in] target_alg_id  identifier of the target symmetric algorithm (also defines a target key-length)
   * \param[in] allowed_usage  the allowed usage scope of the target key
   * \param[in] salt  an optional salt value (if used, it should be unique for each instance of the target key)
   * \param[in] ctx_label  an optional application specific "context label" (it can identify purpose of the target
   *            key and/or communication parties)
   * \param[in] params  an optional pointer to Domain Parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89)
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return a unique pointer to \c SecretSeed object, which contains the key material produced by the Key-Agreement
   *          algorithm
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kIncompatibleObject  if the public and private keys correspond to different
   *                algorithms or reference to different domain parameters
   *                or if (params != nullptr), but provided domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object is in an incompleted state
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
  virtual ara::core::Result<SymmetricKey::Uptrc> AgreeKey(
      PublicKey const& other_side_key, KeyDerivationFunctionCtx& kdf, AlgId target_alg_id,
      RestrictedUseObject::Usage allowed_usage, ReadOnlyMemRegion salt = ReadOnlyMemRegion(),
      ReadOnlyMemRegion ctx_label = ReadOnlyMemRegion(), DomainParameters::Sptrc params = nullptr,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_AGREEMENT_PRIVATE_CTX_H_

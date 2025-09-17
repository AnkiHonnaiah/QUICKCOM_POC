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
/*!        \file  ara/crypto/cryp/key_diversifier_ctx.h
 *        \brief  Interface of Symmetric Keys Diversification algorithms Context.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DIVERSIFIER_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DIVERSIFIER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/symmetric_key.h"
#include "ara/crypto/cryp/symmetric_key_context.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Interface of Symmetric Keys Diversification algorithms Context.
 * \vpublic
 */
class KeyDiversifierCtx : public SymmetricKeyContext {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<KeyDiversifierCtx, CustomDeleter>;

  /*!
   * \brief Get a fixed size of the target key ID required by diversification algorithm.
   * \details Returned value is constant for each instance of the interface,
   *          i.e. independent from configuration by the \c Init() call.
   * \return Size of the key ID in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetKeyIdSize() const noexcept = 0;

  /*!
   * \brief Get a fixed size of an application specific "filler" required by the diversificator initialization.
   * \details Returned value is constant for each instance of the interface,
   *          i.e. independent from configuration by the \c Init() call.
   * \return Size of the application specific filler in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetFillerSize() const noexcept = 0;

  /*!
   * \brief Get the bit-length of target (diversified) keys.
   * \details Returned value is configured by the context factory method,
   *          i.e. independent from configuration by the \c Init() calls.
   * \return The length of target (diversified) keys in bits.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetTargetKeyBitLength() const noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Execute initialization of the diversifier context by setting a public "filler" value.
   * \param[in] app_filler  an application specific "filler" value
   * \param[in] target_alg_id  the identifier of target symmetric crypto algorithm
   * \param[in] allowed_usage  bit-flags that define a list of allowed transformations' types in which the target key
   *            can be used
   * \param[in] params  an optional pointer to the domain parameters required for full specification of the
   *            symmetric transformation (e.g. variable S-boxes of GOST28147-89)
   * \details If (GetFillerSize() == 0) then the app_filler argument can have 0 size, because it is ignored in
   *       any case.
   * \details If (params != nullptr) then at least the parameters' COUID must be saved to the dependency field
   *       of the generated key object.
   * \details If (target_alg_id == kAlgIdAny) then a diversified key can be loaded to any symmetric context that
   *       supports same key length (if the "allowed usage" flags are also satisfied)!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the \c app_filler is incorrect,
   *                i.e. if (app_filler.size() < GetFillerSize());
   * \error SecurityErrorDomain::kIncompatibleArguments  if \c target_alg_id specifies a cryptographic
   *                algorithm different from a symmetric one with key length equal to \c GetTargetKeyLength();
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but provided domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object is in an incompleted state
   * \error SecurityErrorDomain::kUsageViolation  if \c allowed_usage specifies usage of the slave key
   *                incompatible with prototyped value of the master key loaded to the context
   * \reentrant FALSE
   * \synchronous TRUE
   *  \vpublic
   */
  virtual ara::core::Result<void> Init(ReadOnlyMemRegion app_filler, AlgId target_alg_id = kAlgIdAny,
                                       Key::Usage allowed_usage = kAllowPrototypedOnly,
                                       DomainParameters::Sptrc params = nullptr) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Execute initialization of the diversifier context by setting a secret "filler" value.
   * \param[in] app_filler  the application specific "filler" value
   * \param[in] target_alg_id  identifier of the target symmetric crypto algorithm
   * \param[in] allowed_usage  bit-flags that define a list of allowed transformations' types in which the target key
   *            can be used
   * \param[in] params  an optional pointer to Domain Parameters required for full specification of the target
   *            symmetric transformation (e.g. variable S-boxes of GOST28147-89), and therefore should be associated
   *            with the target symmetric key
   * \details If (GetFillerSize() == 0) then the app_filler argument can have 0 size, because it is ignored in
   *       any case.
   * \details If (params != nullptr) then at least the parameters' COUID must be saved to the dependency field
   *       of the generated key object.
   * \details If (target_alg_id == kAlgIdAny) then a diversified key can be loaded to any symmetric context that
   *       supports same key length (if the "allowed usage" flags are also satisfied)!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the \c app_filler is incorrect,
   *                i.e. if (app_filler.size() < GetFillerSize());
   * \error SecurityErrorDomain::kIncompatibleArguments  if \c target_alg_id specifies a cryptographic
   *                algorithm different from a symmetric one with key length equal to \c GetTargetKeyLength();
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but provided domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object has an incomplete state
   * \error SecurityErrorDomain::kUsageViolation  if this transformation type is prohibited by the
   *                "allowed usage" restrictions of the provided \c app_filler object;
   *                or if \c allowed_usage specifies usage of the slave key incompatible with prototyped
   *                value of the master key loaded to the content
   * \reentrant FALSE
   * \synchronous TRUE
   *  \vpublic
   */
  virtual ara::core::Result<void> Init(SecretSeed const& app_filler, AlgId target_alg_id = kAlgIdAny,
                                       Key::Usage allowed_usage = kAllowPrototypedOnly,
                                       DomainParameters::Sptrc params = nullptr) noexcept = 0;

  /*!
   * \brief Get the symmetric algorithm ID of target (slave) keys.
   * \return the symmetric algorithm ID of target keys, configured by last call of the \c Init() method
   * \details If the context was not configured yet by a call of the \c Init() method then \c kAlgIdUndefined should be
   *       returned.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual AlgId GetTargetAlgId() const noexcept = 0;

  /*!
   * \brief Get allowed key usage of target (slave) keys.
   * \return allowed key usage bit-flags of target keys
   * \details Returned value depends from the master key prototype and the argument \c target_alg_id of last call of
   *       the \c Init() method.
   * \details If the context was not configured yet by a call of the \c Init() method then a prototyped value of the
   *       master key should be returned.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual Key::Usage GetTargetAllowedUsage() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Execute the key diversification from provided key ID.
   * \param[in] target_key_id  an ID of the target key
   * \param[in] is_session  the "session" (or "temporary") attribute for the target key (if \c true)
   * \param[in] is_exportable  the exportability attribute for the target key (if \c true)
   * \param[in] reserved_index  an optional index of reserved object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the diversified slave key
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext  if the context was not initialized by a key value;
   *                or if (0 < GetFillerSize()), but the context was not initialized by a "filler" value
   *                via call of the \c Init() method
   * \error SecurityErrorDomain::kInvalidInputSize if size of the \c target_key_id are incorrect,
   *                e.g. if (target_key_id.size() < GetKeyIdSize())
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<SymmetricKey::Uptrc> Diversify(
      ReadOnlyMemRegion target_key_id, bool is_session = true, bool is_exportable = false,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DIVERSIFIER_CTX_H_

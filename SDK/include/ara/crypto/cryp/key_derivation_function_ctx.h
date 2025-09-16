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
/*!        \file  ara/crypto/cryp/key_derivation_function_ctx.h
 *        \brief  Key Derivation Function interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/key_material.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/symmetric_key.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Key Derivation Function interface.
 * \vpublic
 */
class KeyDerivationFunctionCtx : public CryptoContext {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<KeyDerivationFunctionCtx, CustomDeleter>;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Configure the number of iterations that will be applied by default.
   * \details Implementation can restrict minimal and/or maximal value of the iterations number.
   * \param[in] iterations Required number of the base function iterations (0 means implementation default number).
   * \return Actual number of iterations configured in the context now.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint32_t ConfigIterations(std::uint32_t iterations = 0) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Derive a symmetric key from provided key material (with optional public salt).
   * \details If (params != nullptr) then the domain parameters object
   *          must be in the completed state (see DomainParameters)!
   * \details If (params != nullptr) then at least the parameters' COUID
   *          must be saved to the dependency field of the produced key object.
   * \details The byte sequence provided via argument ctx_label can include a few
   *          fields with different meaning separated by single 0x00 byte.
   * \param[in] target_alg_id  The target symmetric algorithm identifier (also defines a target key-length).
   * \param[in] allowed_usage  The allowed usage scope of the target key.
   * \param[in] source_km  The source key material for the key derivation execution.
   * \param[in] salt  An optional salt value (if used, it should be unique for each instance of the target key).
   * \param[in] ctx_label  An optional application specific "context label" (it can identify purpose of the target
   *            key and/or communication parties).
   * \param[in] is_session  The "session" (or "temporary") attribute for the target key (if \c true).
   * \param[in] is_exportable  The exportability attribute for the target key (if \c true).
   * \param[in] params  An optional pointer to Domain Parameters required for full specification of the symmetric
   *            transformation (e.g. variable S-boxes of GOST28147-89).
   * \param[in] reserved_index  An optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap.
   * \param[in] iterations Required number of base function iterations (0 means the current default number).
   * \return Unique smart pointer to created instance of derived symmetric key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error securityerrordomain::kbruteforcerisk  If key length of the \c sourcekm is below of
   *                 an internally defined limitation.
   * \error securityerrordomain::kemptycontainer  If the domain parameters are required,
   *                 but (params == nullptr) was passed.
   * \error securityerrordomain::kincompleteargstate  If (params != nullptr), but provided domain
   *                 parameters object has an incomplete state.
   * \error securityerrordomain::kincompatibleobject  If content (at least algorithm) of provided domain
   *                 parameters is incompatible with \c targetalgid.
   * \error securityerrordomain::kinvalidargument  If any of arguments is incorrect.
   * \error securityerrordomain::kbusyresource  If the slot specified by \c reservedindex is busy yet.
   * \error securityerrordomain::kunreservedresource  If the slot specified by \c reservedindex was not
   *                 allocated.
   * \error securityerrordomain::kinsufficientresource  If capacity of slot specified by \c reservedindex
   *                 is not enough for placing of the target object.
   * \error securityerrordomain::kbadalloc  If (reservedindex == kallocobjectonheap),
   *                 but allocation memory on the heap is impossible.
   * \error securityerrordomain::kusageviolation  If this transformation type is prohibited by the
   *                 "allowed usage" restrictions of the provided \c source_km object.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<SymmetricKey::Uptrc> DeriveKey(
      AlgId target_alg_id, Key::Usage allowed_usage, KeyMaterial const& source_km,
      ReadOnlyMemRegion salt = ReadOnlyMemRegion(), ReadOnlyMemRegion ctx_label = ReadOnlyMemRegion(),
      bool is_session = true, bool is_exportable = false, DomainParameters::Sptrc params = nullptr,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap, std::uint32_t iterations = 0) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Derive a symmetric key from provided key material (with secret salt).
   * \details If (params != nullptr) then the domain parameters object
   *          must be in the completed state (see DomainParameters)!
   * \details If (params != nullptr) then at least the parameters' COUID
   *          must be saved to the dependency field of the produced key object.
   * \details The byte sequence provided via argument ctx_label can include a few
   *          fields with different meaning separated by single 0x00 byte.
   * \param[in] target_alg_id Target symmetric algorithm identifier (also defines a target key-length).
   * \param[in] allowed_usage Allowed usage scope of the target key.
   * \param[in] source_km Source key material for the key derivation execution.
   * \param[in] salt  The secret salt value (an additional secret value independent from the \c source_km).
   * \param[in] ctx_label Optional application specific "context label"
   *                     (it can identify purpose of the target key and/or communication parties).
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target key (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key (if true).
   * \param[in] params An optional pointer to Domain Parameters required for full specification
   *                   of the symmetric transformation (e.g. variable S-boxes of GOST28147-89).
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for
   *                          this allocation or default marker, which says to allocate on the heap.
   * \param[in] iterations Required number of base function iterations (0 means the current default number).
   * \return Unique smart pointer to created instance of derived symmetric key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBruteForceRisk  If key length of the \c source_km is below of
   *                 an internally defined limitation.
   * \error SecurityErrorDomain::kEmptyContainer  If the domain parameters are required,
   *                 but (params == nullptr) was passed.
   * \error SecurityErrorDomain::kIncompleteArgState  If (params != nullptr), but provided domain
   *                 parameters object has an incomplete state.
   * \error SecurityErrorDomain::kIncompatibleObject  If content (at least algorithm) of provided domain
   *                 parameters is incompatible with \c target_alg_id.
   * \error SecurityErrorDomain::kInvalidArgument  If any of arguments is incorrect.
   * \error SecurityErrorDomain::kBusyResource  If the slot specified by \c reserved_index is busy yet.
   * \error SecurityErrorDomain::kUnreservedResource  If the slot specified by \c reserved_index was not
   *                 allocated.
   * \error SecurityErrorDomain::kInsufficientResource  If capacity of slot specified by \c reserved_index
   *                 is not enough for placing of the target object.
   * \error SecurityErrorDomain::kBadAlloc  If (reserved_index == kAllocObjectOnHeap),
   *                 but allocation memory on the heap is impossible.
   * \error SecurityErrorDomain::kUsageViolation  If this transformation type is prohibited by the
   *                 "allowed usage" restrictions of the provided \c source_km object.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<SymmetricKey::Uptrc> DeriveKey(AlgId target_alg_id, Key::Usage allowed_usage,
                                                           KeyMaterial const& source_km, SecretSeed const& salt,
                                                           ReadOnlyMemRegion ctx_label = ReadOnlyMemRegion(),
                                                           bool is_session = true, bool is_exportable = false,
                                                           DomainParameters::Sptrc params = nullptr,
                                                           ReservedObjectIndex reserved_index = kAllocObjectOnHeap,
                                                           std::uint32_t iterations = 0) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Derive a "slave" key material (secret seed) from provided "master" key material (with optional public salt).
   * \details The byte sequence provided via argument ctx_label can include a few
   *          fields with different meaning separated by single 0x00 byte.
   * \param[in] target_alg_id Target symmetric algorithm identifier (also defines a target seed-length).
   * \param[in] allowed_usage Allowed usage scope of the target seed.
   * \param[in] source_km Source key material for the key derivation execution.
   * \param[in] salt Optional salt value (if used, it should be unique for each instance of the target key).
   * \param[in] ctx_label Optional application specific "context label"
   *                     (it can identify purpose of the target key and/or communication parties).
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target key material (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key material (if true).
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for
   *                          this allocation or default marker, which says to allocate on the heap.
   * \param[in] iterations Required number of base function iterations (0 means the current default number).
   * \return Unique smart pointer to created SecretSeed object, which keeps the derived key material.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBruteForceRisk if key length of the sourceKm is below of an
   * internally defined limitation
   * \error  SecurityErrorDomain::kBusyResource if the slot specified by reservedIndex is busy yet
   * \error SecurityErrorDomain::kUnreservedResource if the slot specified by reservedIndex was
   * not allocated
   * \error SecurityErrorDomain::kInsufficientResource if capacity of slot specified by reserved
   * Index is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc if (reservedIndex == kAllocObjectOnHeap), but
   * allocation memory on the heap is impossible
   * \error SecurityErrorDomain::kUsageViolation if this transformation type is prohibited by the
   * "allowed usage" restrictions of the provided sourceKm object
   * \error SecurityErrorDomain::kInvalidArgument if any of arguments is incorrect, and the error
   * condition is not covered by the cases described above
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<SecretSeed::Uptrc> DeriveSeed(AlgId target_alg_id, SecretSeed::Usage allowed_usage,
                                                          KeyMaterial const& source_km,
                                                          ReadOnlyMemRegion salt = ReadOnlyMemRegion(),
                                                          ReadOnlyMemRegion ctx_label = ReadOnlyMemRegion(),
                                                          bool is_session = true, bool is_exportable = false,
                                                          ReservedObjectIndex reserved_index = kAllocObjectOnHeap,
                                                          std::uint32_t iterations = 0) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Derive a "slave" key material (secret seed) from provided "master" key material (with secret salt).
   * \param[in] target_alg_id Target symmetric algorithm identifier (also defines a target seed-length).
   * \param[in] allowed_usage Allowed usage scope of the target seed.
   * \param[in] source_km Source key material for the key derivation execution.
   * \param[in] salt A secret salt value (an additional secret value independent from the source_km).
   * \param[in] ctx_label Optional application specific "context label"
   *                     (it can identify purpose of the target key and/or communication parties).
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target key material (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key material (if true).
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for
   *                          this allocation or default marker, which says to allocate on the heap.
   * \param[in] iterations Required number of base function iterations (0 means the current default number).
   * \return Unique smart pointer to created SecretSeed object, which keeps the derived key material.
   * \details The byte sequence provided via argument ctx_label can include a few
   *          fields with different meaning separated by single 0x00 byte.
   * \context ANY
   * \pre -
   * \error SecurityErrorDomain::kBruteForceRisk if key length of the sourceKm is below of an
   * internally defined limitation
   * \error SecurityErrorDomain::kBusyResource if the slot specified by reservedIndex is busy yet
   * \error SecurityErrorDomain::kUnreservedResource if the slot specified by reservedIndex was
   * not allocated
   * \error SecurityErrorDomain::kInsufficientResource if capacity of slot specified by reserved
   * Index is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc if (reservedIndex == kAllocObjectOnHeap), but
   * allocation memory on the heap is impossible
   * \error SecurityErrorDomain::kUsageViolation if this transformation type is prohibited by the
   * "allowed usage" restrictions of the provided sourceKm or salt objects
   * \error SecurityErrorDomain::kInvalidArgument if any of arguments is incorrect, and the error
   * condition is not covered by the cases described above
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<SecretSeed::Uptrc> DeriveSeed(AlgId target_alg_id, SecretSeed::Usage allowed_usage,
                                                          KeyMaterial const& source_km, SecretSeed const& salt,
                                                          ReadOnlyMemRegion ctx_label = ReadOnlyMemRegion(),
                                                          bool is_session = true, bool is_exportable = false,
                                                          ReservedObjectIndex reserved_index = kAllocObjectOnHeap,
                                                          std::uint32_t iterations = 0) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_DERIVATION_FUNCTION_CTX_H_

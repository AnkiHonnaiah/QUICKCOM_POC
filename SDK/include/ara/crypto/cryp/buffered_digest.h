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
/*!        \file  ara/crypto/cryp/buffered_digest.h
 *        \brief  General interface for buffered computation of a digest (MAC/HMAC/hash).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_BUFFERED_DIGEST_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_BUFFERED_DIGEST_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/key_material.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/signature.h"
#include "ara/crypto/cryp/stream_starter.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief General interface for buffered computation of a digest (MAC/HMAC/hash).
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
class BufferedDigest : public virtual StreamStarter {
 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  BufferedDigest() = default;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~BufferedDigest() noexcept = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  BufferedDigest(BufferedDigest&& /*other*/) noexcept = default;

 public:
  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  BufferedDigest(BufferedDigest const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  BufferedDigest& operator=(BufferedDigest const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  BufferedDigest& operator=(BufferedDigest&& /*other*/) = delete;

  /*!
   * \brief Get the output digest size.
   * \return Size of the full output from this digest-function in bytes.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetDigestSize() const noexcept = 0;

  /*!
   * \brief Update the digest calculation context by a new part of the message.
   * \param[in] in A part of input message that should be processed.
   * \details This method is dedicated for cases then the KeyMaterial is a part of the "message".
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotStarted if the digest calculation was not initiated
   *                                                   by a call of the Start() method.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Update(KeyMaterial const& in) noexcept = 0;

  /*!
   * \brief Update the digest calculation context by a new part of the message.
   * \param[in] in A part of input message that should be processed.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotStarted if the digest calculation was not initiated
                                                       by a call of the Start() method.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Update(ReadOnlyMemRegion in) noexcept = 0;

  /*!
   * \brief Update the digest calculation context by a new part of the message.
   * \details This method is convenient for processing of constant tags.
   * \param[in] in A byte value that is a part of input message.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotStarted if the digest calculation was not initiated
   *                                                   by a call of the Start() method.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Update(Byte in) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Finish the digest calculation and optionally produce the "signature" object.
   * \details Only after call of this method the digest can be signed, verified, extracted or compared!
   * \details If the signature object produced by a plain hash-function then the dependence COUID of
   *          the "signature" should be set to COUID of domain parameters used by this context,
   *          but the "hash algorithm ID" field of the "signature" should be set according to own algorithm ID
   *          (i.e. equal to CryptoPrimitiveId::GetPrimitiveId()).
   * \details If the signature object produced by a keyed MAC/HMAC/AE/AEAD algorithm then the dependence COUID of
   *          the "signature" should be set to COUID of used symmetric key,
   *          but the "hash algorithm ID" field of the "signature" should be set to kAlgIdNone (0).
   * \param[in] make_signature_object If this argument is true then
   *                                the method will also produce the signature object.
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for
   *                          this allocation or default marker, which says to allocate on the heap.
   * \return Unique smart pointer to created signature object, if (make_signature_object == true) or
   *         nullptr if (make_signature_object == false).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotStarted if the digest calculation was not
   *                                                   initiated by a call of the Start() method.
   * \error SecurityErrorDomain::kBusyResource if the slot specified by reserved_index is busy yet.
   * \error SecurityErrorDomain::kUnreservedResource if the slot specified by reserved_index was not allocated.
   * \error SecurityErrorDomain::kInsufficientResource if size of specified slot is not
   *                                                   enough for placing of the target object.
   * \error SecurityErrorDomain::kBadAlloc if (reserved_index == kAllocObjectOnHeap),
   *                                       but allocation memory on the heap is impossible.
   * \error SecurityErrorDomain::kUsageViolation if the buffered digest belongs to a MAC/HMAC context
   *                                             initialized by a key without kAllowSignature permission,
   *                                             but (make_signature_object == true).
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<Signature::Uptrc> Finish(
      bool make_signature_object, ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Finish the digest calculation.
   * \details Only after call of this method the digest can be signed, verified, extracted or compared!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotStarted if the digest calculation was not
   *                                                   initiated by a call of the Start() method.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Finish() noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get requested part of calculated digest to existing memory buffer.
   * \details Entire digest value is kept in the context up to next call Start(),
   *          therefore any its part can be extracted again or verified.
   * \details If (full_digest_size <= offset) then return_size = 0 bytes;
   * \details else return_size = min(output.size(), (full_digest_size - offset)) bytes.
   * \param[out] output An output buffer for storing the requested digest fragment (or fully).
   * \param[in] offset Position of the first byte of digest that should be placed to the output buffer.
   * \return Number of digest bytes really stored to the output buffer (they are always <= output.size()
   *         and denoted below as return_size).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotFinished if the digest calculation was not
   *                                                    finished by a call of the Finish() method.
   * \error SecurityErrorDomain::kUsageViolation if the buffered digest is part of MAC/HMAC context
   *                                             initialized by a key without kAllowSignature permission.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetDigest(WritableMemRegion output, std::size_t offset = 0) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Gets requested part of calculated digest to pre-reserved managed container.
   * \details This method sets the size of the output container according to actually saved value.
   * \details Entire digest value is kept in the context up to next call start(),
   *          therefore any its part can be extracted again or verified.
   * \details If (full_digest_size <= offset) then return_size = 0 bytes;
   * \details else return_size = min(output.capacity(), (full_digest_size - offset)) bytes.
   * \tparam Alloc Custom allocator type of the output container.
   * \param[out] output A managed container for storing the requested digest fragment (or fully).
   * \param[in] offset Position of first byte of digest that should be placed to the output buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotFinished if the digest calculation was not
   *                                                    finished by a call of the Finish() method.
   * \error SecurityErrorDomain::kUsageViolation if the buffered digest is part of MAC/HMAC context
   *                                             initialized by a key without kAllowSignature permission.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Resize the given output buffer
   * - Get the digest and copy it (starting from the given offset)
   *   into the output buffer
   * - If getting the digest succeeds
   *   - Resize the output buffer to the digest size
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc = DefBytesAllocator>
  inline ara::core::Result<void> GetDigest(ByteVectorT<Alloc>& output, std::size_t offset = 0) const noexcept {
    output.resize(output.capacity());
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    WritableMemRegion const mem_reg{output};
    ara::core::Result<void> error;
    ara::core::Result<std::size_t> const output_size{GetDigest(mem_reg, offset)};
    if (output_size) {
      output.resize(output_size.Value());
    } else {
      error.EmplaceError(output_size.Error());
    }
    return error;
  }

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Compare the calculated digest against an expected value.
   * \details Entire digest value is kept in the context up to next call Start(),
   *          therefore any its part can be verified again or extracted.
   * \details If (full_digest_size <= offset) || (expected.size() == 0) then return false;
   *          else comparison_size = min(expected.size(), (full_digest_size - offset)) bytes.
   * \param[in] expected A memory region containing an expected digest value.
   * \param[in] offset Position of first byte in calculated digest for the comparison starting.
   * \return true if the expected bytes sequence is identical to first bytes of calculated digest.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotFinished if the digest calculation was not
   *                                                    finished by a call of the Finish() method.
   * \error SecurityErrorDomain::kBruteForceRisk if the buffered digest is a part of MAC/HMAC context,
   *                                             which was initialized by a key without kAllowSignature permission,
   *                                             but actual size of requested digest is less than 8 bytes
   *                                             (it is a protection from the brute-force attack).
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<bool> Compare(ReadOnlyMemRegion expected, std::size_t offset = 0) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Check the calculated digest against an expected "signature" object.
   * \details Entire digest value is kept in the context up to next call Start(),
   *          therefore it can be verified again or extracted.
   * \param[in] expected The signature object containing an expected digest value.
   * \return true if value and meta-information of the provided "signature" object is identical to calculated digest
   *         and current configuration of the context respectively; but false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kProcessingNotFinished if the digest calculation was not
   *                                                    finished by a call of the Finish() method.
   * \error SecurityErrorDomain::kIncompatibleObject if the provided "signature" object was
   *                                                 produced by another crypto primitive type.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<bool> Check(Signature const& expected) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_BUFFERED_DIGEST_H_

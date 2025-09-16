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
/*!        \file  ara/crypto/cryp/stream_cipher_ctx.h
 *        \brief  Generalized Stream Cipher Context interface (cover all modes of operation).
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/secret_seed.h"
#include "ara/crypto/cryp/stream_starter.h"
#include "ara/crypto/cryp/symmetric_key_context.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Generalized Stream Cipher Context interface (cover all modes of operation).
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
class StreamCipherCtx : public SymmetricKeyContext, public virtual StreamStarter {
 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  StreamCipherCtx() = default;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~StreamCipherCtx() noexcept override = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  StreamCipherCtx(StreamCipherCtx&& /*other*/) noexcept = default;

 public:
  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  StreamCipherCtx(StreamCipherCtx const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  StreamCipherCtx& operator=(StreamCipherCtx const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  StreamCipherCtx& operator=(StreamCipherCtx&& /*other*/) = delete;

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<StreamCipherCtx, CustomDeleter>;

  /*!
   * \brief Returns default expected size of Initialization Vector (IV).
   * \return Default expected size of IV in bytes.
   * \vpublic
   */
  std::size_t GetIvSize() const noexcept override = 0;

  /*!
   * \brief Returns block size of base algorithm.
   * \return Size of block in bytes.
   * \vpublic
   */
  std::size_t GetBlockSize() const noexcept override = 0;

  /*!
   * \brief Checks the operation mode for the bytewise property.
   * \return true if the mode can process a message the byte by byte (without padding) or
   *         false if only the block by block (with padding).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsBytewiseMode() const noexcept = 0;

  /*!
   * \brief Checks if the seek operation is supported in the current mode.
   * \return true the seek operation is supported in the current mode or false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsSeekableMode() const noexcept = 0;

  /*!
   * \brief Verifies validity of specific IV length.
   * \param[in] iv_size Length of the IV in bytes.
   * \return true if provided IV length is supported by the algorithm.
   * \vpublic
   */
  bool IsValidIvSize(std::size_t iv_size) const noexcept override = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Set the position of the next byte within the stream of the encryption/decryption gamma.
   * \param[in] offset  the offset value in bytes, relative to begin or current position in the gamma stream
   * \param[in] from_begin  the starting point for positioning within the stream: from begin (if \c true) or from
   *            current position (if \c false)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnsupported  if the seek operation is not supported by the current mode
   * \error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
   *                the \c Start() method
   * \error SecurityErrorDomain::kBelowBoundary  if the \c offset value is incorrect (in context of the
   *                the \c from_begin argument), i.e. it points before begin of the stream (note: it is an optional
   *                error condition)
   * \error SecurityErrorDomain::kInvalidArgument  if the offset is not aligned to the required boundary
   *                (see \c IsBytewiseMode())
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Seek(std::int64_t offset, bool from_begin = true) noexcept = 0;

  /*!
   * \brief Processe initial parts of message aligned to the block-size boundary.
   * \param[out] out  an output data buffer
   * \param[in] in  an input data buffer
   * \details It is a copy-optimized method that doesn't use the internal cache buffer! It can be used only before
   *       processing of any non-aligned to the block-size boundary data.
   *       Pointers to the input and output buffers must be aligned to the block-size boundary!
   * \details The input and output buffers may completely coincide, but they must not partially intersect!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleArguments  if sizes of the input and output buffers
   *                are not equal
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the input buffer is not divisible by
   *                the block size (see \c GetBlockSize())
   * \error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers partially intersect
   * \error SecurityErrorDomain::kInvalidUsageOrder  if this method is called after processing of
   *                non-aligned data (to the block-size boundary)
   * \error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
   *                the \c Start() method
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ProcessBlocks(WritableMemRegion out, ReadOnlyMemRegion in) noexcept = 0;

  /*!
   * \brief Processe initial parts of message aligned to the block-size boundary.
   * \param[in,out] in_out  an input and output data buffer, i.e. the whole buffer should be updated
   * \note It is a copy-optimized method that doesn't use internal cache buffer! It can be used up to first non-block
   *       aligned data processing.
   *       <b>Pointer to the input-output buffer must be aligned to the block-size boundary!</b>
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidInputSize  if size of the \c in_out buffer is not divisible by
   *                the block size (see \c GetBlockSize())
   * \error SecurityErrorDomain::kInvalidUsageOrder  if this method is called after processing of
   *                non-aligned data (to the block-size boundary)
   * \error SecurityErrorDomain::kProcessingNotStarted  if the data processing was not started by a call of
   *                the \c Start() method
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ProcessBlocks(ReadWriteMemRegion in_out) noexcept = 0;

  /*!
   * \brief Processe a non-final part of message (that is not aligned to the block-size boundary).
   * \param[out] out  an output data buffer
   * \param[in] in  an input data buffer
   * \return actual size of output data (i.e. the number of leading bytes updated in the output buffer)
   * \note If (IsBytewiseMode() == false) then it must be:
   *       bs = GetBlockSize(), out.size() >= (((in.size() + bs - 1) / bs) * bs)
   * \note If (IsBytewiseMode() == true) then it must be: out.size() >= in.size()
   * \note The input and output buffers must not intersect!
   * \note This method is "copy inefficient", therefore it should be used only in conditions when an application
   *       cannot control the chunking of the original message!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity  if the output buffer has capacity
   *                insufficient for placing of the transformation result
   * \error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers intersect
   * \error SecurityErrorDomain::kProcessingNotStarted  if data processing was not started by a call of
   *                the \c Start() method
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> ProcessBytes(WritableMemRegion out, ReadOnlyMemRegion in) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Processes a non-final part of message (that is not aligned to the block-size boundary).
   * \tparam Alloc  a custom allocator type of the output container
   * \param[out] out  a managed container for the output data
   * \param[in] in  an input data buffer
   * \note This method sets size of the output container according to actually saved value.
   * \note If (IsBytewiseMode() == false) then it must be:
   *       bs = GetBlockSize(), out.capacity() >= (((in.size() + bs - 1) / bs) * bs)
   * \note If (IsBytewiseMode() == true) then it must be: out.capacity() >= in.size()
   * \note This method is "copy inefficient", therefore it should be used only in conditions when an application
   *       cannot control the chunking of the original message!
   * \note The input buffer must not point inside the output container!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output container is not enough
   * \error SecurityErrorDomain::kInOutBuffersIntersect  if the input buffer points inside of
   *                the preallocated output container
   * \error SecurityErrorDomain::kProcessingNotStarted  if data processing was not started by a call of
   *                the \c Start() method
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Resize the given output buffer
   * - Process the buffer
   * - If processing the buffer succeeds
   *   - Resize the buffer to the processed buffer size
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc = DefBytesAllocator>
  ara::core::Result<void> ProcessBytes(ara::core::Vector<Byte, Alloc>& out, ReadOnlyMemRegion in) noexcept {
    ara::core::Result<void> result{};

    out.resize(out.capacity());
    ara::core::Result<std::size_t> inner_result{ProcessBytes(WritableMemRegion{out}, in)};

    if (inner_result.HasValue()) {
      out.resize(inner_result.Value());
    } else {
      result.EmplaceError(inner_result.Error());
    }

    return result;
  }

  /*!
   * \brief Processes the final part of message (that may be not aligned to the block-size boundary).
   * \param[out] out  an output data buffer
   * \param[in] in  an input data buffer
   * \return actual size of output data (i.e. the number of leading bytes that were updated in the output buffer)
   * \note If (IsBytewiseMode() == false) then it must be:
   *        bs = GetBlockSize(),
   *        out.size() >= (((in.size() + bs * (IsDirectTransform() ? 2 : 1) - 1) / bs) * bs)
   * \note If (IsBytewiseMode() == true) then it must be: out.size() >= in.size()
   * \note The input and output buffers must not intersect!
   * \note Usage of this method is mandatory for processing of the last data chunk in block-wise modes!
   * \note This method may be used for processing of a whole message in a single call (in any mode)!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output buffer is not enough
   * \error SecurityErrorDomain::kInOutBuffersIntersect  if the input and output buffers intersect
   * \error SecurityErrorDomain::kProcessingNotStarted  if data processing was not started by a call of
   *                 the \c Start() method
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> FinishBytes(WritableMemRegion out, ReadOnlyMemRegion in) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Processes the final part of message (that may be not aligned to the block-size boundary).
   * \tparam Alloc  a custom allocator type of the output container
   * \param[out] out  a managed container for output data
   * \param[in] in  an input data buffer
   * \note This method sets the size of the output container according to actually saved value.
   * \note If (IsBytewiseMode() == false) then it must be:
   *        bs = GetBlockSize(),
   *        out.capacity() >= (((in.size() + bs * (IsDirectTransform() ? 2 : 1) - 1) / bs) * bs)
   * \note If (IsBytewiseMode() == true) then it must be: out.capacity() >= in.size()
   * \note Usage of this method is mandatory for processing of the last data chunk in block-wise modes!
   * \note This method may be used for processing of a whole message in a single call (in any mode)!
   * \note The input buffer must not point inside the output container!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrc::kInsufficientCapacity  if capacity of the output container is not enough
   * \error SecurityErrc::kInOutBuffersIntersect  if the input and output buffers intersect
   * \error SecurityErrc::kProcessingNotStarted  if data processing was not started by a call of
   *                 the \c Start() method
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Resize the given output buffer
   * - Process the buffer
   * - If processing the buffer succeeds
   *   - Resize the buffer to the processed buffer size
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc = DefBytesAllocator>
  ara::core::Result<void> FinishBytes(ByteVectorT<Alloc>& out, ReadOnlyMemRegion in) noexcept {
    ara::core::Result<void> result{};

    out.resize(out.capacity());
    ara::core::Result<std::size_t> inner_result{FinishBytes(WritableMemRegion{out}, in)};

    if (inner_result) {
      out.resize(inner_result.Value());
    } else {
      result.EmplaceError(inner_result.Error());
    }

    return result;
  }

  /*!
   * \brief Count number of bytes now kept in the context cache.
   * \return number of bytes now kept in the context cache
   * \note In block-wise modes if an application has supplied input data chunks with incomplete last block
   *        then the context saves the rest part of the last (incomplete) block to internal "cache" memory
   *        and wait a next call for additional input to complete this block.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t CountBytesInCache() const noexcept = 0;

  /*!
   * \brief Estimate maximal number of input bytes that may be processed for filling of an output buffer
   *            without overflow.
   * \param[in] output_capacity  capacity of the output buffer
   * \return maximum number of input bytes
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Compute the granularity
   * - Compute the useable capacity
   * - Compute the maximum input size and return it
   * \endinternal
   */
  std::size_t EstimateMaxInputSize(std::size_t output_capacity) const noexcept {
    std::size_t const granularity{IsBytewiseMode() ? 1 : GetBlockSize()};
    if (granularity < 1) {
      ara::core::Abort("ara::crypto::cryp::StreamCipherCtx::EstimateMaxInputSize(): Block size must not be 0!");
    }
    // VECTOR NC AutosarC++17_10-A4.7.1: MD_CRYPTO_AutosarC++17_10-A4.7.1_arithmetic_computation_overflow
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_arithmetic_computation_overflow
    std::size_t const usable_capacity{(output_capacity / granularity) * granularity};
    return ((usable_capacity <= CountBytesInCache()) ? 0 : (usable_capacity - CountBytesInCache()));
  }

  /*!
   * \brief Estimate minimal required capacity of the output buffer, which is enough for saving a result of
   *            input data processing.
   * \param[in] input_size  size of input data
   * \param[in] is_final  flag that indicates processing of the last data chunk (if \c true)
   * \return required capacity of the output buffer (in bytes)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Compute the granularity
   * - Check the operation mode for the bytewise property.
   * - Check if the transformation is direct
   * - Compute the maximum padding size
   * - Compute the required capacity and return it
   * \endinternal
   */
  std::size_t EstimateRequiredCapacity(std::size_t input_size, bool is_final = false) const noexcept {
    std::size_t const granularity{IsBytewiseMode() ? 1 : GetBlockSize()};
    bool const is_bytewise_mode{IsBytewiseMode()};
    bool const is_direct_transform{IsDirectTransform()};

    std::size_t const max_padding_size{(is_final && (!is_bytewise_mode) && is_direct_transform) ? granularity : 0};
    if (granularity < 1) {
      ara::core::Abort("ara::crypto::cryp::StreamCipherCtx::EstimateRequiredCapacity(): Block size must not be 0!");
    }
    return (((input_size + CountBytesInCache() + max_padding_size) / granularity) * granularity);
  }

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_autosar_api
  /*!
   * \brief Get actual bit-length of an IV loaded to the context.
   * \param[out] iv_uid optional pointer to a buffer for saving an COUID of a IV object now loaded to the context
   * \return actual length of the IV (now set to the algorithm context) in bits
   * \note If any IV was not set to the context yet then 0 is returned.
   * \note If the context was initialized by a \c SecretSeed object then the output buffer \c *iv_uid must be filled
   *       by COUID of this loaded IV object, in other cases \c *iv_uid must be filled by all zeros.
   * \vpublic
   */
  std::size_t GetActualIvBitLength(CryptoObjectUid* iv_uid = nullptr) const noexcept override = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H_

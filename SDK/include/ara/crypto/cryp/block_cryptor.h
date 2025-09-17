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
/*!        \file  ara/crypto/cryp/block_cryptor.h
 *        \brief  General interface for stateless encryption / decryption of a single data block with padding.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_BLOCK_CRYPTOR_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_BLOCK_CRYPTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vector>

#include "ara/crypto/common/exceptions.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "crypto/common/identifiable.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief General interface for stateless encryption / decryption of a single data block with padding.
 * \details The Block Cryptor context should include a defenition of a padding scheme applicable by default.
 * \details Use non-default value of argument suppress_padding only if you know exactly what you are doing!
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
class BlockCryptor : public virtual ::crypto::common::Identifiable {
 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  BlockCryptor() = default;

  /*!
   * \brief Virtual destructor.
   * \vprivate Component Private
   */
  ~BlockCryptor() noexcept override = default;

 public:
  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  BlockCryptor(BlockCryptor const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  BlockCryptor(BlockCryptor&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  BlockCryptor& operator=(BlockCryptor const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \vprivate Component Private
   */
  BlockCryptor& operator=(BlockCryptor&& /*other*/) & noexcept = delete;

  /*!
   * \brief Return the transformation direction of the current configuration of the Block Cryptor context.
   * \return true if the Block Cryptor context is configured for encryption and false for decryption.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsEncryption() const noexcept = 0;

  /*!
   * \brief Indicate that the currently configured transformation accepts only complete blocks of input data.
   * \return true if the transformation requires the maximum size of input data and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  bool IsMaxInputOnly() const noexcept { return !IsEncryption(); }

  /*!
   * \brief Indicate that the currently configured transformation can produce only complete blocks of output data.
   * \return true if the transformation can produce only the maximum size of output data and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  bool IsMaxOutputOnly() const noexcept { return IsEncryption(); }

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get maximum expected size of the input data block.
   * \param[in] suppress_padding if true then the method calculates the size for the case when the whole space of the
   *                             plain data block is used for the payload only
   * \return maximum size of the input data block in bytes.
   * \details If (IsEncryption() == false) then a value returned by this method is independent from the suppress_padding
   *          argument and it will be equal to the block size.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetMaxInputSize(bool suppress_padding = false) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get maximum possible size of the output data block.
   * \param[in] suppress_padding if true then the method calculates the size for the case when the whole space of the
   *                             plain data block is used for the payload only
   * \return maximum size of the output data block in bytes.
   * \details If (IsEncryption() == false) then a value returned by this method is independent from the suppress_padding
   *          argument and it will be equal to the block size.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetMaxOutputSize(bool suppress_padding = false) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
   * \param[in] out The output buffer to write to the result to
   * \param[in] in The input data block
   * \param[in] suppress_padding if true then the method calculates the size for the case when the whole space of the
   *                             plain data block is used for the payload only
   * \return actual size of output data (it always <= out.size()) or 0 if the input data block has incorrect
   *         content.
   * \details Encryption with (suppress_padding == true) expects that:
   *          in.size() == GetMaxInputSize(true) && out.size() >= GetMaxOutputSize(true).
   * \details Encryption with (suppress_padding == false) expects that:
   *          in.size() <= GetMaxInputSize(false) && in.size() > 0 && out.size() >= GetMaxOutputSize(false).
   * \details Decryption expects that:
   *          in.size() == GetMaxInputSize() && out.size() >= GetMaxOutputSize(suppress_padding).
   * \details The case (out.size() < GetMaxOutputSize()) should be used with caution, only if you are strictly
   *          certain about the size of the output data!
   * \details In case of (suppress_padding == true) the actual size of plain text should be equal to full size
   *          of the plain data block (defined by the algorithm)!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncorrectInputSize if the mentioned above rules about the input size is violated
   * \error SecurityErrorDomain::kInsufficientCapacity if the out.size() is not enough to store the transformation
   *                                                   result
   * \error SecurityErrorDomain::kUninitializedContext if the context was not initialized by a key value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> ProcessBlock(WritableMemRegion out, ReadOnlyMemRegion in,
                                                      bool suppress_padding = false) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Process (encrypt / decrypt) an input block according to the cryptor configuration.
   * \tparam Alloc a custom allocator type of the output container
   * \param[in] out The output buffer to write to the result to
   * \param[in] in The input data block
   * \param[in] suppress_padding if true then the method calculates the size for the case when the whole space of the
   *                             plain data block is used for the payload only
   * \return actual size of output data (it always <= out.size()) or 0 if the input data block has incorrect
   *         content.
   * \details Encryption with (suppress_padding == true) expects that:
   *          in.size() == GetMaxInputSize(true) && out.size() >= GetMaxOutputSize(true).
   * \details Encryption with (suppress_padding == false) expects that:
   *          in.size() <= GetMaxInputSize(false) && in.size() > 0 && out.size() >= GetMaxOutputSize(false).
   * \details Decryption expects that:
   *          in.size() == GetMaxInputSize() && out.size() >= GetMaxOutputSize(suppress_padding).
   * \details The case (out.size() < GetMaxOutputSize()) should be used with caution, only if you are strictly
   *          certain about the size of the output data!
   * \details In case of (suppress_padding == true) the actual size of plain text should be equal to full size
   *          of the plain data block (defined by the algorithm)!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncorrectInputSize if the mentioned above rules about the input size is violated
   * \error SecurityErrorDomain::kInsufficientCapacity if the out.size() is not enough to store the transformation
   *                                                   result
   * \error SecurityErrorDomain::kUninitializedContext if the context was not initialized by a key value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Resize the given output buffer
   * - Process the buffer
   * - If processing the buffer succeeds
   *   - Resize the buffer to the block size
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc = DefBytesAllocator>
  ara::core::Result<void> ProcessBlock(ByteVectorT<Alloc>& out, ReadOnlyMemRegion in,
                                       bool suppress_padding = false) const noexcept {
    ara::core::Result<void> result{ara::crypto::SecurityErrc::kRuntimeFault};
    out.resize(out.capacity());
    ara::core::Result<std::size_t> process_block_result{ProcessBlock(WritableMemRegion{out}, in, suppress_padding)};
    if (process_block_result) {
      out.resize(process_block_result.Value());
      result.EmplaceValue();
    } else {
      result.EmplaceError(process_block_result.Error());
    }
    return result;
  }
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_BLOCK_CRYPTOR_H_

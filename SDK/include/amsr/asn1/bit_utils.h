/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  bit_utils.h
 *        \brief  Helper class for bit manipulation
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BIT_UTILS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BIT_UTILS_H_

#include <algorithm>
#include <cmath>
#include <limits>
#include "amsr/asn1/asn1_error_domain.h"
#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace asn1 {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Information about input bits to be appended.
 */
struct BitsInfo {
  /*!
   * \brief Input bits to be added.
   */
  std::uint8_t const input;
  /*!
   * \brief Number of valid bits in the \c input.
   */
  std::uint8_t const valid_bits;
};

/*!
 * \brief Converts byte stream to integer.
 * \tparam T Type to be checked
 * \details Takes any range of uint8 stream as input and creates an integer.
 * \param[in] input Iteratable sequence of bytes.
 * \return Unsigned integer (uint64)
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \error Asn1Errc::kUnsupportedNumeric If the byte stream is longer than 64 bits.
 * \exceptionsafety STRONG
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
template <typename T>
static auto BitsToUint64(T const& input) noexcept -> amsr::core::Result<std::uint64_t> {
  amsr::core::Result<std::uint64_t> result{amsr::core::Result<std::uint64_t>::FromError(
      Asn1Errc::kUnsupportedNumeric,
      "Failed to convert a byte stream larger than 64 bits to an unsigned 64-bit integer."_sv)};
  if (input.size() <= sizeof(std::uint64_t)) {
    std::uint64_t output{0};
    amsr::core::Array<std::uint8_t, sizeof(std::uint64_t)> data{};
    static_cast<void>(std::copy_backward(input.cbegin(), input.cend(), data.end()));

    output = static_cast<std::uint64_t>(data[7]) | (static_cast<std::uint64_t>(data[6]) << 8) |
             (static_cast<std::uint64_t>(data[5]) << 16) | (static_cast<std::uint64_t>(data[4]) << 24) |
             (static_cast<std::uint64_t>(data[3]) << 32) | (static_cast<std::uint64_t>(data[2]) << 40) |
             (static_cast<std::uint64_t>(data[1]) << 48) | (static_cast<std::uint64_t>(data[0]) << 56);
    result.EmplaceValue(output);
  }
  return result;
}

/*!
 * \brief Converts byte stream to integer.
 * \details Takes any range of uint8 stream as input and creates an integer.
 * \tparam T Type to be checked
 * \param[in] input Iteratable sequence of bytes.
 * \return Unsigned integer (uint64)
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \error Asn1Errc::kUnsupportedNumeric If the byte stream is longer than 32 bits.
 * \exceptionsafety STRONG
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
template <typename T>
static auto BitsToInt32(T const& input) noexcept -> amsr::core::Result<std::int32_t> {
  amsr::core::Result<std::int32_t> result{amsr::core::Result<std::int32_t>::FromError(
      Asn1Errc::kUnsupportedNumeric,
      "Failed to convert a byte stream larger than 32 bits to a signed 32-bit integer."_sv)};
  constexpr std::size_t kBufferSize{sizeof(std::int32_t)};
  if (input.size() <= kBufferSize) {
    std::int32_t output{0};
    amsr::core::Array<std::uint8_t, kBufferSize> data{};
    amsr::core::Array<std::uint8_t, kBufferSize>::iterator const last{
        std::copy_backward(input.cbegin(), input.cend(), data.end())};
    std::size_t const size_diff{data.size() - input.size()};
    if ((size_diff > 0) && ((input[0] & 0x80U) == 0x80U)) {
      static_cast<void>(std::fill(data.begin(), last, 0xFFU));
    }

    // VECTOR NC AutosarC++17_10-M5.0.21: MD_VAASN1_AutosarC++17_10-M5.0.21_raw_bytes_to_signed_type
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAASN1_AutosarC++17_10-M0.3.1_overflow_truncate
    // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAASN1_AutosarC++17_10-A4.7.1_overflow_truncate
    output = static_cast<std::int32_t>(data[3]) | (static_cast<std::int32_t>(data[2]) << 8) |
             (static_cast<std::int32_t>(data[1]) << 16) | (static_cast<std::int32_t>(data[0]) << 24);
    result.EmplaceValue(output);
  }
  return result;
}

/*!
 * \brief Bit stream utility to manipulate bit sequence.
 */
class BitStream {
 public:
  /*!
   * \brief Adds number of bits to bit stream.
   * \details Takes one byte of input and adds valid bits of that byte to existing bit stream.
   * \param[in] bits_info Input bits and valid bits information.
   * \return void result.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error Asn1Errc::kInvalidContent If the count of passed bits is 0 or over 8.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   *
   * \internal
   * - Check if bit to be added is valid.
   * - Add input bits, if all bit can be added in last byte.
   * - Other wise, If there are unused bits in last byte
   *   - Fill unused bits of last byte with input.
   *   - Add new byte and add remaining input bits.
   * \endinternal
   */
  auto AddBits(BitsInfo const& bits_info) noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(
        Asn1Errc::kInvalidContent, "Tried to add an invalid amount of bits to a bit stream."_sv)};
    if ((kMaxUnusedBits >= bits_info.valid_bits) && (kMinUnusedBits != bits_info.valid_bits)) {
      std::uint8_t valid{bits_info.valid_bits};
      if (unused_ >= valid) {
        result = UpdateAByte(bits_info.input, valid);
      } else {
        if (kMinUnusedBits < unused_) {
          valid = static_cast<std::uint8_t>(valid - unused_);
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAASN1_AutosarC++17_10-M0.3.1_overflow_truncate
          // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAASN1_AutosarC++17_10-A4.7.1_overflow_truncate
          std::uint8_t const partial_input{
              static_cast<std::uint8_t>(static_cast<std::uint8_t>(bits_info.input) >> valid)};
          result = UpdateAByte(partial_input, unused_);
        }
        data_.push_back(0);
        unused_ = kMaxUnusedBits;
        result = UpdateAByte(bits_info.input, valid);
      }
    }
    return result;
  }

  /*!
   * \brief Converts current bit stream to unsigned integer.
   * \details Produces integer from bit stream created so far.
   * \return Result containing Unsigned integer (uint64) on success.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kInvalidContent If this bit stream is empty.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   *
   * \internal
   * - Convert Bit to unsigned integer.
   * - If there are unused bits
   *   - Shift unused bits to right.
   * - Return value.
   * \endinternal
   */
  auto GetUint64() const noexcept -> amsr::core::Result<std::uint64_t> {
    amsr::core::Result<std::uint64_t> result{amsr::core::Result<std::uint64_t>::FromError(
        Asn1Errc::kInvalidContent, "Tried to convert an empty bit stream to an unsigned 64-bit integer."_sv)};
    if (!data_.empty()) {
      // FIXME: This seems to implicitly operate on the last byte of the stream, while bit streams longer than 64
      //  bits shouldn't be convertible. The documentation does not specify this sufficiently.
      ara::core::Vector<std::uint8_t> bytes{data_};
      std::uint8_t& last{bytes[bytes.size() - 1U]};
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAASN1_AutosarC++17_10-M0.3.1_overflow_truncate
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAASN1_AutosarC++17_10-A4.7.1_overflow_truncate
      std::uint16_t const shifted{static_cast<std::uint16_t>(static_cast<std::uint16_t>(last) << unused_)};
      last = static_cast<std::uint8_t>(shifted & 0xFFU);
      if (amsr::core::Result<std::uint64_t> const& resp{BitsToUint64(bytes)}) {
        result.EmplaceValue(*resp >> unused_);
      } else {
        result.EmplaceError(resp.Error());
      }
    }
    return result;
  }

 private:
  /*!
   * \brief Updates a single byte of stream.
   * \details Updates last byte of the stream by adding valid bits at the end.
   * \param[in] input Data to be updated.
   * \param[in] valid Data encoding type.
   * \return void result.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kRuntimeFault If this bit stream is empty.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   *
   * \internal
   * - Insert valid bits at end of stream.
   * \endinternal
   */
  auto UpdateAByte(std::uint8_t const input, std::uint8_t valid) -> amsr::core::Result<void> {
    amsr::core::Result<void> retval{
        amsr::core::Result<void>::FromError(Asn1Errc::kRuntimeFault, "Tried to update an empty bit stream's byte."_sv)};
    if (!data_.empty()) {
      std::uint8_t& out{data_[data_.size() - 1U]};
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAASN1_AutosarC++17_10-M0.3.1_overflow_truncate
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAASN1_AutosarC++17_10-A4.7.1_overflow_truncate
      std::uint16_t const shifted{static_cast<std::uint16_t>(static_cast<std::uint16_t>(out) << valid)};
      out = static_cast<std::uint8_t>(shifted & static_cast<std::uint8_t>(0xFF));
      constexpr amsr::core::Array<std::uint8_t const, kMaxUnusedBits> shifted_mask{0xFFU, 0x7FU, 0x3FU, 0x1FU,
                                                                                   0x0FU, 0x07U, 0x03U, 0x01U};
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAASN1_AutosarC++17_10-M0.3.1_overflow_truncate
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAASN1_AutosarC++17_10-A4.7.1_overflow_truncate
      std::uint8_t const shift{static_cast<std::uint8_t>(BitStream::kMaxUnusedBits - valid)};
      if (shift < shifted_mask.size()) {
        out = out | (input & shifted_mask[shift]);
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAASN1_AutosarC++17_10-M0.3.1_overflow_truncate
        // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAASN1_AutosarC++17_10-A4.7.1_overflow_truncate
        unused_ = static_cast<std::uint8_t>(unused_ - valid);
        retval.EmplaceValue();
      } else {
        retval.EmplaceError(Asn1Errc::kRuntimeFault, "Internal array access out of bounds. Check 'valid' argument.");
      }
    } else {
      retval.EmplaceError(Asn1Errc::kRuntimeFault, "Function unexpectedly called with empty data.");
    }
    return retval;
  }

  /*!
   * \brief Maximum unused bits possible.
   */
  static constexpr std::uint8_t kMaxUnusedBits{8};
  /*!
   * \brief Minimum unused bits possible.
   */
  static constexpr std::uint8_t kMinUnusedBits{0};
  /*!
   * \brief Sequence if bits added.
   */
  ara::core::Vector<std::uint8_t> data_{};
  /*!
   * \brief Number of unused bit in last byte.
   */
  std::uint8_t unused_{0};
};

}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BIT_UTILS_H_

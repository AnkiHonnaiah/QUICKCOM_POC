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
/*!        \file  hex_string.h
 *        \brief  Hex string utilities.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_HEX_STRING_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_HEX_STRING_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <limits>
#include <utility>

#include "amsr/charconv/to_number_unsigned.h"
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/iostream/stringstream/input_string_stream.h"
#include "amsr/iostream/utility.h"
#include "amsr/loguti/unwrap.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/exceptions.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/util/contract.h"
#include "crypto/common/util/logger.h"
#include "vac/container/string_literals.h"
#include "vac/language/location.h"

namespace crypto {
namespace common {
namespace util {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Hex string utility class.
 * \vprivate Product Private
 */
class HexString final {
 public:
  /*!
   * \brief Alias for the char type.
   * \vpublic
   */
  using CharacterType = char;

  /*!
   * \brief Calculates the byte size for a given hexadecimal string length.
   * \param[in] hex_string_len Hexadecimal string length.
   * \return Number of bytes required to hold the binary data represented by the hexadecimal string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto ByteSize(std::size_t hex_string_len) noexcept -> std::size_t {
    return (hex_string_len >> std::size_t{1}) + (hex_string_len & std::size_t{1});
  }

  /*!
   * \brief Calculates the byte size for a given hexadecimal string.
   * \param[in] hex_string Hexadecimal string.
   * \return Number of bytes required to hold the binary data represented by the hexadecimal string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto ByteSize(amsr::core::String const& hex_string) noexcept -> std::size_t {
    return ByteSize(hex_string.length());
  }

  /*!
   * \brief Reads a hex string into a mem region.
   * \param[in] hex_string The hex string to be read.
   * \param[in] memory The mem region to be filled.
   * \return A view onto the input region that has been filled.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If \c memory is too small to hold the result or if one character
   * is not hexadecimal encoded.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto ToMemRegion(amsr::core::String const& hex_string, ara::crypto::WritableMemRegion memory) noexcept
      -> amsr::core::Result<ara::crypto::WritableMemRegion> {
    amsr::core::Result<ara::crypto::WritableMemRegion> result{ara::crypto::SecurityErrc::kInvalidArgument};
    std::size_t const size_req{ByteSize(hex_string)};
    if (memory.size() >= size_req) {
      amsr::stream::stringstream::InputStringStream const iss{hex_string};
      result = iss.ReadEncodedByteArray(memory.subspan(0, size_req), false, false).MapError([](amsr::core::ErrorCode) {
        return ara::crypto::MakeErrorCode(ara::crypto::SecurityErrc::kInvalidArgument, 0);
      });
    }
    return result;
  }

  /*!
   * \brief Converts a hex string into a byte vector.
   * \param[in] hex_string The hex string to be converted.
   * \return A vector containing the conversion result.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If \c memory is too small to hold the result or if one character
   * is not hexadecimal encoded.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto ToVector(amsr::core::String const& hex_string) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    ara::core::Vector<uint8_t> vec(ByteSize(hex_string.length()));
    return vec.empty() ? amsr::core::Result<ara::core::Vector<std::uint8_t>>{std::move(vec)}
                       : ToMemRegion(hex_string, vec).Replace<ara::core::Vector<std::uint8_t>>(std::move(vec));
  }

  /*!
   * \brief Converts the contents of a memory region into a hex string representation.
   * \param[in] memory Input memory region to be converted.
   * \param[in] lower_case Indicates if the output should be formatted lower case. Defaults to uppercase.
   * \return The resulting hex string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kBadAlloc If an output string of the required size cannot be allocated.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto FromMemRegion(ara::crypto::ReadOnlyMemRegion const& memory, bool lower_case = false) noexcept
      -> amsr::core::Result<amsr::core::String> {
    amsr::core::Result<amsr::core::String> result{ara::crypto::SecurityErrc::kBadAlloc};
    if (memory.empty()) {
      result.EmplaceValue("00");
    } else {
      if (memory.size() <= (std::numeric_limits<std::size_t>::max() >> static_cast<size_t>(1))) {
        ara::core::Vector<char> hex_string_vec(memory.size() << static_cast<size_t>(1));
        amsr::stream::stringstream::OutputStringStream oss{hex_string_vec};
        oss.SetLetterCase(lower_case ? amsr::stream::LetterCase::kLowercase : amsr::stream::LetterCase::kUppercase);
        if (oss.Write(memory, amsr::stream::Base::kHex).HasValue()) {
          result.EmplaceValue(oss.AsStringView().ToString());
        }
      } else {
        // intentionally left empty
      }
    }
    return result;
  }

  /*!
   * \brief Converts the contents of a memory region into a hex string representation.
   * \param[in] memory Input memory region to be converted.
   * \param[in] lower_case Indicates if the output should be formatted lower case. Defaults to uppercase.
   * \return The resulting hex string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto FromMemRegionUnwrapped(ara::crypto::ReadOnlyMemRegion const& memory, bool lower_case = false) noexcept
      -> amsr::core::String {
    return amsr::loguti::Unwrap(common::util::HexString::FromMemRegion(memory, lower_case),
                                common::util::kCommonLoggingContext, "Could not convert MemRegion to HexString."_sv);
  }

  /*!
   * \brief Converts the contents of a byte vector into a hex string representation.
   * \param[in] vec Input vector to be converted.
   * \param[in] lower_case Indicates if the output should be formatted lower case. Defaults to uppercase.
   * \return The resulting hex string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity If the vector is too large.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NL AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  static auto FromVector(ara::core::Vector<std::uint8_t> const& vec, bool lower_case = false) noexcept
      -> amsr::core::Result<amsr::core::String> {
    return FromMemRegion(vec, lower_case);
  }
};

}  // namespace util
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_HEX_STRING_H_

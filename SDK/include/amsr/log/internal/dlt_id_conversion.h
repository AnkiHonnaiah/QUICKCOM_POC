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
/*!        \file  dlt_id_conversion.h
 *        \brief  Conversion to and from DLT IDs.
 *
 *         \unit  LogAndTrace::Logging::StringConversions
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_DLT_ID_CONVERSION_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_DLT_ID_CONVERSION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>

#include "amsr/core/array.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "vac/iterators/range.h"

#include "amsr/log/log_types.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief   String is converted to unsigned integer.
 *
 * \tparam  StringType  The type of char container to convert to an integer.
 *                      The character type must be 1 byte in size.
 *                      Must be an amsr::core::String or amsr::core::StringView.
 *
 * \param   str         Input string
 *
 * \return  String as integer
 * \spec
 *   requires true;
 * \endspec
 *
 * \details The leading characters of the string are directly converted to their byte representation
 *          until the size of the unsigned integer type is reached.
 *          All characters exceeding the size of the integer are discarded.
 */
template <typename StringType>
auto StringToDltId(StringType const& str) noexcept -> IdType {
  static_assert((sizeof(decltype(str[0])) == 1u), "Size of string element is not one byte!");
  constexpr std::size_t kByteSize{8};
  constexpr std::size_t kLastIndex{sizeof(IdType) - 1};
  IdType ret_val{0U};
  for (std::size_t const i : vac::iterators::range<std::size_t>(0, std::min(sizeof(IdType), str.size()))) {
    ret_val = static_cast<IdType>(ret_val | (static_cast<IdType>(str[i]) << ((kLastIndex - i) * kByteSize)));
  }
  return ret_val;
}

/*!
 * \brief   Converts ASCII chars given as unsigned integer to a core::String with the corresponding size.
 *
 * \details zero-byte is interpreted as a null terminator for the string.
 *
 * \param   value           To be converted.
 *
 * \return  The string representation of the bytes of the unsigned integer.
 * \spec
 *   requires true;
 * \endspec
 *
 * \remarks   When std::uint32_t is used as value and 0x34333231 is provided
 *            the result will be a core::String with the content "4321".
 *
 */
// VECTOR NC AutosarC++17_10-M5.0.16: MD_LOG_AutosarC++17_10-M5.0.16_FalsePositive
inline auto DltIdToString(IdType const value) noexcept -> core::String {
  static_assert(std::is_unsigned<IdType>::value, "Type to be converted from is not unsigned integer.");
  constexpr std::size_t kSize{sizeof(IdType)};
  constexpr std::size_t kNum{kSize + 1};
  std::size_t i{0};
  core::Array<char, kNum> buffer{0};
  for (char& byte : core::Span<char>{buffer}.first(kSize)) {
    constexpr std::size_t kLastIndex{kSize - 1};
    constexpr IdType kBitMask{0xff};
    // VECTOR NC AutosarC++17_10-A4.7.1: MD_LOG_AutosarC++17_10-A4.7.1
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_LOG_AutosarC++17_10-M0.3.1
    std::size_t const bits_to_shift{8 * (kLastIndex - i)};
    // VECTOR NC AutosarC++17_10-A4.7.1: MD_LOG_AutosarC++17_10-A4.7.1
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_LOG_AutosarC++17_10-M0.3.1
    // VCA_LOGAPI_VALID_OBJECT
    byte = static_cast<char>(kBitMask & (value >> bits_to_shift));
    ++i;
  }
  return core::String{buffer.data()};
}
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_DLT_ID_CONVERSION_H_

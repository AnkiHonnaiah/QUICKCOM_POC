/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Conversion of string to integer.
 *        \unit   osabstraction::net
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_UTILITIES_STRING_TO_INT_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_UTILITIES_STRING_TO_INT_H_

#include <cstdint>
#include <cstdlib>
#include <limits>

#include "amsr/charconv/to_number_signed.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"

#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace net {
namespace internal {
namespace utilities {

/*!
 * \brief Converts a String to an integer.
 *
 * \param[in]       str  Valid amsr::core::StringView containing an integral number.
 *
 * \return Integral value of the passed number.
 *
 * \error  osabstraction::OsabErrc::kApiError  The conversion failed because the passed string did not contain a number
 *                                             or the number cannot be stored in a signed 64 bit integral type.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
/*!
 * \internal
 * - Convert string to int64 using amsr::charconv::ToNumber.
 * - Check if the conversions is success.
 * - Return result.
 * \endinternal
 */
inline ::amsr::core::Result<std::int64_t> ConvertToInt64(::amsr::core::StringView const& str) noexcept {
  ::amsr::core::Result<std::int64_t> result{osabstraction::OsabErrc::kApiError};

  amsr::charconv::ToNumber<std::int64_t> to_number{str};
  amsr::core::Result<std::int64_t> const value_result{to_number.Convert(amsr::charconv::NumberBase::kDecimal)};
  if (!value_result.HasValue()) {
    if (value_result.CheckError(amsr::charconv::CharconvErrc::kResultOutOfRange)) {
      result.EmplaceError(
          MakeErrorCode(osabstraction::OsabErrc::kApiError, "Conversion from string to integer failed."));
    } else {
      result.EmplaceError(MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                        "Conversion from string to integer failed. String does not contain a number."));
    }
  } else {
    result.EmplaceValue(value_result.Value());
  }
  return result;
}

/*!
 * \brief Converts a String to an integer.
 *
 * \param[in]       str  Valid amsr::core::StringView containing an integral number.
 *
 * \return Integral value of the passed number.
 *
 * \error  osabstraction::OsabErrc::kApiError  The conversion failed because the passed string did not contain a number
 *                                             or the number cannot be stored in an unsigned 32 bit integral type.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
/*!
 * \internal
 * - Convert to signed 64 bit integer
 * - Check if conversion succeeded
 * - If conversion failed
 *   - output the error
 * - If conversion succeeded
 *   - check if the value fits into an unsigned 32 bit integer
 *   - If the value does not fit
 *     - output an error that states that the number represented by the string is out of range.
 *   - If the value fits
 *     - output the value as unsigned 32 bit integer.
 * \endinternal
 */
inline ::amsr::core::Result<std::uint32_t> ConvertToUint32(::amsr::core::StringView const& str) noexcept {
  return ConvertToInt64(str).AndThen([](std::int64_t converted_value) -> ::amsr::core::Result<std::uint32_t> {
    // VECTOR Next Line AutosarC++17_10-M19.3.1: MD_OSA_M19.3.1_PosixApi
    ::amsr::core::Result<std::uint32_t> result{
        MakeErrorCode(osabstraction::OsabErrc::kApiError, ERANGE,
                      "Conversion from string to integer failed. Converted value out of range.")};
    if ((converted_value >= 0) &&
        (converted_value <= static_cast<std::int64_t>(std::numeric_limits<std::uint32_t>::max()))) {
      result = ::amsr::core::Result<std::uint32_t>(static_cast<std::uint32_t>(converted_value));
    }
    return result;
  });
}

}  // namespace utilities
}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_UTILITIES_STRING_TO_INT_H_

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
 *        \brief  Defines template helpers.
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_TYPE_HELPER_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_TYPE_HELPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <system_error>

#include "amsr/core/optional.h"
#include "amsr/core/span.h"
#include "vac/language/compile_time.h"

namespace amsr {
namespace charconv {

/*!
 * \brief Specifies the case of the letters A to E in hexadecimal strings.
 * \vprivate Product Private
 */
enum class LetterCase : std::uint8_t { kLowerCase, kUpperCase };

/*!
 * \brief Specifies formatting parameters.
 * \vprivate Product Private
 */
struct ExtendedFormat {
  /*! Minimum field width */
  std::uint8_t width{static_cast<std::uint8_t>(0)};
  /*! Case of letters */
  LetterCase abc_case{LetterCase::kUpperCase};
  /*! Fill character if output does not fill width */
  char fillchar{' '};
};

/*! Default extended format specification for interface definition. */
constexpr ExtendedFormat extended_format_default{0, LetterCase::kUpperCase, ' '};

/*! Default hexadecimal format specification for std::uint8_t. */
constexpr ExtendedFormat hex_format_default_uint8{2, LetterCase::kUpperCase, '0'};

/*!
 * \brief Specifies parameters for floating point output.
 * \vprivate Product Private
 */
enum class FPFormat : std::uint8_t { kScientific = 1, kFixed = 2, kGeneral = kFixed | kScientific };

/*! Default floating point format specification for interface definition. */
constexpr FPFormat fp_format_default{FPFormat::kGeneral};

/*!
 * \brief Enum for setting different number bases
 * \vprivate Product Private
 */
enum class NumberBase : std::uint8_t { kBinary = 2, kDecimal = 10, kHex = 16 };

/*!
 * \brief Check if type is an int (does not include bool!)
 * \tparam T type to check
 * \vprivate Component Private
 */
template <typename T>
using is_int =
    typename vac::language::compile_time::all<!std::is_same<T, bool>::value, std::is_integral<T>::value>::type;

/*!
 * \brief Checks if given type is signed but not a char type.
 * \tparam T type to check.
 * \vprivate Component Private
 */
template <typename T>
using is_signed_and_not_char =
    typename vac::language::compile_time::all<std::is_signed<T>::value, !std::is_same<T, char>::value>;

/*!
 * \brief Checks if given type is a char type and signed.
 * \tparam T type to check.
 * \vprivate Component Private
 */
template <typename T>
using is_char_and_signed =
    typename vac::language::compile_time::all<std::is_signed<T>::value, std::is_same<T, char>::value>;

/*!
 * \brief Checks if given type is unsigned but not a char type.
 * \tparam T type to check.
 * \vprivate Component Private
 */
template <typename T>
using is_unsigned_and_not_char =
    typename vac::language::compile_time::all<std::is_unsigned<T>::value, !std::is_same<T, char>::value>;

/*!
 * \brief Checks if given type is a char type and unsigned.
 * \tparam T type to check.
 * \vprivate Component Private
 */
template <typename T>
using is_char_and_unsigned =
    typename vac::language::compile_time::all<std::is_unsigned<T>::value, std::is_same<T, char>::value>;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a std::int8_t.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint8_t kSInt8MaxLength{4};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a std::uint8_t.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint8_t kUInt8MaxLength{3};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a std::int16_t.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint8_t kSInt16MaxLength{6};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a std::uint16_t.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint8_t kUInt16MaxLength{5};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a std::int32_t.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint8_t kSInt32MaxLength{11};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a std::uint32_t.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint8_t kUInt32MaxLength{10};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a std::int64_t.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint8_t kSInt64MaxLength{20};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a std::uint64_t.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint8_t kUInt64MaxLength{20};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CharacterConversion_M3.4.1_visibility
/*!
 * \brief Maximum buffer length to convert a floating point value.
 * \details - std::numeric_limits<double>::max() requires 327 characters in fixed format. To represent -1.79769e+308,
 * the following characters are needed: '-', '1', 308 digits, '.'  and 16 decimal places.
 * \remark The constant can be used to guarantee an adequate buffer size.
 */
constexpr std::uint16_t kFPMaxLength{327};

/*!
 * \brief Number of bits in one byte.
 */
constexpr std::uint32_t kBitsInByte{8};

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_TYPE_HELPER_H_

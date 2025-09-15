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
 *        \brief  Utility functions for charconv
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_UTILITY_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_UTILITY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <iterator>  // for std advance

#include "amsr/charconv/type_helper.h"
#include "amsr/charconv_common/internal/const_buffer_pointer.h"
#include "amsr/core/string.h"

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Checks if ASCII character is a digit.
 * \details Substitute for std::isdigit since it requires an unsigned chars as parameter. The use as well as the
 * conversion leads to Bauhaus violations.
 * \param[in] ch Character to convert
 * \return \c true if \c ch is digit, \c false if not.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::Utility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
bool IsDigit(std::uint8_t ch) noexcept;

/*!
 * \brief Checks if ASCII character is a hexadecimal digit
 * \details Substitute for std::isxdigit since it requires an unsigned chars as parameter. The use as well as the
 * conversion leads to Bauhaus violations.
 * \param[in] ch Character to convert.
 * \return \c true if \c ch is hexadecimal digit, \c false if not.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::Utility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
bool IsXDigit(std::uint8_t ch) noexcept;

/*!
 * \brief Converts a character into the number represented by it.
 * \details e.g. '0' - > 0, 'b' -> 11
 * \param[in] ch Character
 * \param[in] base Number base
 * \return Number represented by ch
 * \context ANY
 * \pre ch has to be a valid decimal or hex digit
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::Utility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
std::uint8_t AsciiToInt(char const ch, NumberBase base) noexcept;

/*!
 * \brief Converts an integer digit to character.
 * \param[in] digit The digit to convert, valid range [0-9].
 * \return The character.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::Utility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
char DigitToAscii(std::uint64_t digit) noexcept;

/*!
 * \brief Sets fill char and width of stream.
 * \param[in] fmt Formatting parameters for hexadecimal output.
 * \param[in,out] stream Stream to write to.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \trace SPEC-9718161, SPEC-9718162
 * \unit amsr::charconv::internal::Utility
 * \steady UNKNOWN
 * \vprivate Component Private
 */
//  SPEC-9718161 (width), SPEC-9718162 (fill character)
void SetStreamFillCharAndWidth(ExtendedFormat const& fmt, std::stringstream& stream) noexcept;

/*!
 * \brief Set case of stream.
 * \param[in] fmt Formatting parameters for hexadecimal output.
 * \param[in,out] stream Stream to write to.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \trace SPEC-9718160
 * \unit amsr::charconv::internal::Utility
 * \steady UNKNOWN
 * \vprivate Component Private
 */
// SPEC-9718160 (letter case)
void SetStreamCase(ExtendedFormat const& fmt, std::stringstream& stream) noexcept;

/*!
 * \brief Returns the contents of stream as string.
 * \details Replaces str() function as it might throw an exception of type std::length_error.
 * \param[in,out] stream Stream.
 * \return Contents of stream.
 * \context ANY
 * \pre Stream is not empty.
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::Utility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
amsr::core::String ToString(std::stringstream& stream) noexcept;

/*!
 * \brief Skip all leading zeros of a textual representation of a binary number.
 * \param[in] padded_string Textual representation of binary number.
 * \return Textual representation of binary number without leading zeros.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::Utility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
amsr::core::String SkipLeadingZeros(amsr::core::String padded_string);

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_UTILITY_H_

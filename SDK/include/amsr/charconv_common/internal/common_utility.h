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

#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_COMMON_UTILITY_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_COMMON_UTILITY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/charconv/type_helper.h"
#include "amsr/charconv_common/internal/buffer_pointer.h"
#include "amsr/charconv_common/internal/const_buffer_pointer.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Checks if a character is a 'space' character
 * \details Replaces std::isspace (not known to QNX compiler). Space characters are horizontal tab (\\h), line feed
 * (\\l), vertical tab (\\v), form feed (\\f), carriage return (\\r) and space (' ')
 * \param[in] ch Character to check
 * \return \c true if \c ch is a whitespace character, \c false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::CommonUtility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
bool IsSpace(std::uint8_t ch) noexcept;

/*!
 * \brief Constructs a message to abort after a failed assert.
 * \details \c location_str is included in the message if the total size of the message is less than \c max_size.
 * \param[in] location_str Calling location in string form.
 * \param[in] max_size Maximum size allowed.
 * \return Constructed string.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::AssembleFailedAssertMessage
 * \vprivate Component Private
 * \steady UNKNOWN
 */
amsr::core::String AssembleFailedAssertMessage(amsr::core::String const location_str, std::size_t max_size) noexcept;

/*!
 * \brief Aborts if \c condition is not fulfilled.
 * \param[in] condition The condition.
 * \param[in] location Calling location.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::CommonUtility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
void Assert(bool condition, amsr::core::StringView location) noexcept;

/*!
 * \brief Aborts if maximum size of string is smaller than required size.
 * \param[in] str A string.
 * \param[in] required_size The number of elements the string has to be able to store.
 * \param[in] msg The abort message.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::charconv::internal::CommonUtility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
void AssertStringIsLargeEnough(amsr::core::String str, std::uint64_t required_size,
                               amsr::core::StringView msg) noexcept;

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_COMMON_UTILITY_H_

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
 *        \brief  Provides utility function for stream classes.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_UTILITY_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_UTILITY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sstream>

#include "amsr/charconv/charconv_error_domain.h"
#include "amsr/charconv/to_textual.h"
#include "amsr/core/abort.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/posix_error_domain.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/stream_error_domain.h"
#include "amsr/iostream/types.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {

/*!
 * \brief Returns EOF.
 * \return EOF as std::int64_t.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \vprivate Component Private
 * \steady UNKNOWN
 */
std::int64_t Eof() noexcept;

/*!
 * \brief Returns the character as std::int64_t.
 * \param[in] character Character to be returned as std::int64_t.
 * \return The character as std::int64_t.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \vprivate Component Private
 * \steady UNKNOWN
 */
std::int64_t GetCharacterAsInt(char character);

/*!
 * \brief Aborts if \c condition is not fulfilled.
 * \param[in] condition The condition.
 * \param[in] file_name File name.
 * \param[in] line Line.
 * \param[in] msg Abort message.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \vprivate Component Private
 * \steady UNKNOWN
 */
void Assert(bool condition, char const* file_name, std::uint64_t const line, amsr::core::StringView msg) noexcept;

/*!
 * \brief Aborts if \c result has no value.
 * \param[in] result The result.
 * \param[in] file_name File name.
 * \param[in] line Line.
 * \param[in] msg Abort message.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \vprivate Component Private
 * \steady UNKNOWN
 */
void AssertHasValue(amsr::core::Result<amsr::core::Span<char>> result, char const* file_name, std::uint64_t const line,
                    amsr::core::StringView msg) noexcept;

/*!
 * \brief Aborts with specifying file_name, line and message.
 * \param[in] file_name File name.
 * \param[in] line Line.
 * \param[in] msg Abort message.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \vprivate Component Private
 * \steady UNKNOWN
 */
void Abort(char const* file_name, std::uint64_t const line, amsr::core::StringView msg) noexcept;

/*!
 * \brief Aborts with specifying file_name, line and message.
 * \param[in] text Abort message.
 * \param[in] line Line.
 * \param[in] msg Abort message.
 * \param[in] buffer Buffer to convert line.
 * \param[in] abort_msg_max_size Maximum size of abort message.
 * \context ANY
 * \pre Parameter text needs to be valid when the function is called. Aborts if abort_msg_max_size > text.max_size().
 * \threadsafe TRUE
 * \reentrant FALSE
 * \unit amsr::stream::Utility
 * \vprivate Component Private
 * \steady UNKNOWN
 */
[[noreturn]] void AssembleMessageAndAbort(amsr::core::String& text, std::uint64_t const line,
                                          amsr::core::StringView msg, amsr::core::Span<char> buffer,
                                          std::size_t abort_msg_max_size);

/*!
 * \brief Checks if preconditions for memcpy are fulfilled.
 * \details \c dest must not be nullptr and the destination must not overlap with the source [\c first, \c last).
 * \param[in] dest Destination to copy to.
 * \param[in] first Begin of source.
 * \param[in] last One-past-the end pointer of source.
 * \param[in] count Number of bytes to copy.
 * \return \c true if preconditions are fulfilled, otherwise \c false.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \vprivate Component Private
 * \steady UNKNOWN
 */
bool FulfillsMemcpyPrerequisitesCheck(char const* dest, char const* first, char const* last,
                                      std::size_t count) noexcept;

/*!
 * \brief Checks if preconditions for memcpy are fulfilled.
 * \details \c dest must not be nullptr and the destination must not overlap with the source [\c first, \c last).
 * \tparam T Type of span, char or char const.
 * \param[in] dest Destination to copy to.
 * \param[in] first Begin of source.
 * \param[in] last One-past-the end pointer of source.
 * \param[in] count Number of bytes to copy.
 * \return \c true if preconditions are fulfilled, otherwise \c false.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \steady UNKNOWN
 * \vprivate Component Private
 */
template <typename T,
          std::enable_if_t<std::is_same<T, char>::value || std::is_same<T, char const>::value, std::int32_t> = 0>
bool FulfillsMemcpyPrerequisites(amsr::core::Span<T> dest, char const* first, char const* last,
                                 std::size_t count) noexcept {
  return FulfillsMemcpyPrerequisitesCheck(dest.data(), first, last, count);
}

/*!
 * \brief Checks if preconditions for memcpy are fulfilled.
 * \details \c dest must not be nullptr and the destination must not overlap with the source [\c first, \c last).
 * \tparam T Type of span, vac::language:byte or vac::language::byte const.
 * \param[in] dest Destination to copy to.
 * \param[in] first Begin of source.
 * \param[in] last One-past-the end pointer of source.
 * \param[in] count Number of bytes to copy.
 * \return \c true if preconditions are fulfilled, otherwise \c false.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \steady UNKNOWN
 * \vprivate Component Private
 */
template <typename T, std::enable_if_t<std::is_same<T, vac::language::byte>::value ||
                                           std::is_same<T, vac::language::byte const>::value,
                                       std::int32_t> = 0>
bool FulfillsMemcpyPrerequisites(amsr::core::Span<T> dest, char const* first, char const* last,
                                 std::size_t count) noexcept {
  // VECTOR NC AutosarC++17_10-A5.2.4: MD_IoStream_A5.2.4_reinterpret_cast
  char const* const dest_it{reinterpret_cast<char const*>(dest.data())};

  return FulfillsMemcpyPrerequisitesCheck(dest_it, first, last, count);
}

/*!
 * \brief Checks if preconditions for memcpy are fulfilled.
 * \details \c dest must not be nullptr and the destination must not overlap with the source [\c first, \c last).
 * \tparam T Type of span, char.
 * \param[in] dest Destination to copy to.
 * \param[in] first Begin of source.
 * \param[in] last One-past-the end pointer of source.
 * \param[in] count Number of bytes to copy.
 * \return \c true if preconditions are fulfilled, otherwise \c false.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \unit amsr::stream::Utility
 * \spec requires true; \endspec
 * \steady UNKNOWN
 * \vprivate Component Private
 */
template <typename T, std::enable_if_t<std::is_same<T, char>::value, std::int32_t> = 0>
bool FulfillsMemcpyPrerequisites(amsr::core::Span<T> dest, vac::language::byte const* first,
                                 vac::language::byte const* last, std::size_t count) noexcept {
  // VECTOR NC AutosarC++17_10-A5.2.4: MD_IoStream_A5.2.4_reinterpret_cast
  char const* const first_it{reinterpret_cast<char const*>(first)};
  // VECTOR NC AutosarC++17_10-A5.2.4: MD_IoStream_A5.2.4_reinterpret_cast
  char const* const last_it{reinterpret_cast<char const*>(last)};

  return FulfillsMemcpyPrerequisitesCheck(dest.data(), first_it, last_it, count);
}

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_UTILITY_H_

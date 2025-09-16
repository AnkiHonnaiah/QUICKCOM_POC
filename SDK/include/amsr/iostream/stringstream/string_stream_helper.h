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
 *        \brief  Common operations of InputStringStream and OutputStringStream classes
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_STRING_STREAM_HELPER_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_STRING_STREAM_HELPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <tuple>

#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/stream_error_domain.h"
#include "amsr/iostream/types.h"
#include "amsr/iostream/utility.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace stream {
namespace stringstream {

/*!
 * \brief Checks if a chunk of memory of size \c size fits into the range [\c first, \c last).
 * \tparam SpanItType Iterator type.
 * \param[in] first Start of memory block.
 * \param[in] last End of memory block (excluded).
 * \param[in] size Size of the memory block to compare with.
 * \return \c true if range is large enough, \c false otherwise
 * \context ANY
 * \pre [first, last) is a valid interval.
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::stream::stringstream::StringStreamHelper
 * \steady UNKNOWN
 * \vprivate Component Private
 *
 * \internal
 * condition                                                | return
 * ---------------------------------------------------------------------
 * [first, last) is a valid interval and larger than size   | true
 * otherwise                                                | false
 * \endinternal
 */
template <typename SpanItType, std::enable_if_t<std::is_same<SpanItType, BufferIterator>::value ||
                                                    std::is_same<SpanItType, ConstBufferIterator>::value,
                                                std::int32_t> = 0>
bool IsRangeLargeEnough(SpanItType first, SpanItType last, std::size_t size) noexcept {
  std::ptrdiff_t const left_size{std::distance(first.pos, last.pos)};

  return (left_size >= 0) && (static_cast<std::size_t>(left_size) >= size);
}

/*!
 * \brief Checks if a chunk of memory of size \c size fits into the range [\c current, \c last) resp. [\c first, \c
 * current) for negative offset.
 * \tparam SpanItType Iterator type.
 * \param[in] first Start of memory block.
 * \param[in] current Current position in memory block.
 * \param[in] last End of memory block (excluded).
 * \param[in] offset Size of the memory block to compare with.
 * \return \c true if range is large enough, \c false otherwise
 * \context ANY
 * \pre [first, last) is a valid interval and \c current is in it.
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::stream::stringstream::StringStreamHelper
 * \steady UNKNOWN
 * \vprivate Component Private
 *
 * \internal
 * condition                                                                          | return
 * -------------------------------------------------------------------------------------------
 * [current, last) resp. [first, current] is a valid interval and larger than offset  | true
 * otherwise                                                                          | false
 * \endinternal
 */
template <typename SpanItType, std::enable_if_t<std::is_same<SpanItType, BufferIterator>::value ||
                                                    std::is_same<SpanItType, ConstBufferIterator>::value,
                                                std::int32_t> = 0>
bool IsRangeLargeEnough(SpanItType first, SpanItType current, SpanItType last, std::int64_t offset) noexcept {
  std::ptrdiff_t left_size{};
  bool range_is_large_enough{false};
  if (offset < 0) {
    left_size = std::distance(current.pos, first.pos);
    range_is_large_enough = left_size <= offset;
  } else {
    left_size = std::distance(current.pos, last.pos);
    range_is_large_enough = left_size >= offset;
  }

  return range_is_large_enough;
}

/*!
 * \brief Returns the current string position.
 * \tparam SpanItType Iterator type.
 * \param[in] first Pointer to begin of buffer.
 * \param[in] cur_pos Pointer to current position in buffer.
 * \return The current string position, measured in bytes from the beginning of the string.
 * \context ANY
 * \pre [first, cur_pos] is a valid interval.
 * \threadsafe FALSE
 * \error StreamErrc::kInvalidPosition Position in stream cannot be determined.
 * \reentrant FALSE
 * \unit amsr::stream::stringstream::StringStreamHelper
 * \steady UNKNOWN
 * \vprivate Component Private
 *
 * \internal
 * - Return distance between first and current position if >= 0.
 * \endinternal
 */
template <typename SpanItType, std::enable_if_t<std::is_same<SpanItType, BufferIterator>::value ||
                                                    std::is_same<SpanItType, ConstBufferIterator>::value,
                                                std::int32_t> = 0>
amsr::core::Result<std::uint64_t> Tell(SpanItType first, SpanItType cur_pos) noexcept {
  using vac::container::literals::operator""_sv;
  amsr::core::Result<std::uint64_t> result{amsr::core::Result<std::uint64_t>::FromError(
      StreamErrc::kInvalidPosition, "StringStream::Tell: Position in stream cannot be determined."_sv.c_str())};

  std::ptrdiff_t const diff{std::distance(first.pos, cur_pos.pos)};
  result.EmplaceValue(amsr::charconv::ConvertOrAbort<std::size_t>(diff));

  return result;
}

/*!
 * \brief Changes the position of the string stream.
 * \tparam SpanItType Iterator type.
 * \param[in] first Pointer to begin of buffer.
 * \param[in] cur_pos Pointer to current position in buffer.
 * \param[in] last Pointer to end of buffer.
 * \param[in] offset Offset to change the position with.
 * \param[in] whence Specifies how the offset is interpreted:
 * - kBeg: offset is a count of characters from the beginning of the string.
 * - kEnd: offset is a count of characters from the end of the string.
 * - kCur: offset is a count of characters from the current string position.
 * \return A tuple of the updated position in the string, measured in bytes from the beginning of the string, and the
 * iterator pointing to the updated position.
 * \context ANY
 * \pre [first, cur_pos] is a valid interval.
 * \threadsafe FALSE
 * \error StreamErrc::kInvalidArgument The whence argument is not valid, or the resulting offset is not valid.
 * \reentrant FALSE
 * \unit amsr::stream::stringstream::StringStreamHelper
 * \steady UNKNOWN
 * \vprivate Component Private
 *
 * \internal
 * #20 If seek direction is kBeg or kEnd, set current position to first or last.
 * #30 If updated seek position is within limits of internal buffer:
 *     - Advance current position by offset.
 *     - Assert that updated cur_pos_ is greater or equal first_
 *     - Update result.
 * \endinternal
 */
template <typename SpanItType, std::enable_if_t<std::is_same<SpanItType, BufferIterator>::value ||
                                                    std::is_same<SpanItType, ConstBufferIterator>::value,
                                                std::int32_t> = 0>
auto Seek(SpanItType const& first, SpanItType const& cur_pos, SpanItType const& last, std::int64_t offset,
          SeekDirection const whence) noexcept -> std::tuple<amsr::core::Result<std::uint64_t>, SpanItType> {
  using vac::container::literals::operator""_sv;
  amsr::core::Result<std::uint64_t> result{amsr::core::Result<std::uint64_t>::FromError(
      StreamErrc::kInvalidArgument, "StringStream::Seek: The resulting offset is not valid."_sv.c_str())};
  SpanItType updated_pos{cur_pos};  // VCA_IOSTREAM_OVERLAP_OK

  // #20
  if (whence == SeekDirection::kBeg) {
    updated_pos = first;  // VCA_IOSTREAM_OVERLAP_OK
  } else if (whence == SeekDirection::kEnd) {
    updated_pos = last;  // VCA_IOSTREAM_OVERLAP_OK
  } else {               // whence is SeekDirection::kCur
  }

  // VCA_IOSTREAM_OVERLAP_OK
  if (IsRangeLargeEnough(first, updated_pos, last, offset)) {  // #30
    std::advance(updated_pos.pos, offset);
    std::ptrdiff_t const diff_first_cur{std::distance(first.pos, updated_pos.pos)};
    result.EmplaceValue(amsr::charconv::ConvertOrAbort<std::size_t>(diff_first_cur));
  }

  return std::make_tuple(result, updated_pos);
}

}  // namespace stringstream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_STRING_STREAM_HELPER_H_

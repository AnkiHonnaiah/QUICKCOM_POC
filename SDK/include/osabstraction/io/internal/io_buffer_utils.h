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
 *        \brief  Iterator for a buffer that is scattered over multiple chunks of memory.
 *        \unit   osabstraction::IOBuffer
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_INTERNAL_IO_BUFFER_UTILS_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_INTERNAL_IO_BUFFER_UTILS_H_

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/span.h"

namespace osabstraction {
namespace io {
namespace internal {

/*!
 * \brief           Calculates the size of all chunks of memory combined.
 *
 * \param[in]       chunks                                    The chunks.
 *
 * \tparam  EntryType Entry that represents one cohesive chunk of memory that is part of an IoBufferQueues underlying
 *                    memory. An EntryType object contains at least the following members:
 *                    - std::size_t size;
 *
 * \return          The allover size of memory of all chunks.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
template <typename EntryType>
auto CalculateAlloverSize(::amsr::core::Span<EntryType> const& chunks) noexcept -> std::size_t {
  std::size_t size_buffer{0};
  static_cast<void>(std::for_each(chunks.cbegin(), chunks.cend(), [&size_buffer](EntryType current_chunk) -> void {
    size_buffer += current_chunk.size;
  }));
  return size_buffer;
}

// VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_OSA_M5.0.16_pointer_arithmetic_on_non_array_target_false_positive
/*!
 * \brief           Finds from an offset (as number of bytes) the chunk and byte within that chunk.
 *
 * \details         Abstracts the memory areas of the chunk to one continuous memory block. That means, from the point
 *                  of view of the offset, the offset of the first byte of chunk n is 1 + the offset of the last byte of
 *                  chunk n - 1. Put into other words, the offset of some byte is the size of all previous chunks + the
 *                  offset of that byte within its chunk.
 *
 * \param[in]       chunks                                    The chunks.
 * \param[in]       offset                                    The offset from the first byte of the first chunk.
 *
 * \tparam  EntryType Entry that represents one cohesive chunk of memory that is part of an IoBufferQueues underlying
 *                    memory. An EntryType object contains at least the following members:
 *                    - std::size_t size;
 *
 * \return          A pair where the first entry is the index of the chunk in which the byte is located and the second
 *                  entry is the offset of that byte within the chunk. No value if the offset is the same as or greater
 *                  than the overall size of all chunks together.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
template <typename EntryType>
auto SplitOffset(::amsr::core::Span<EntryType> const& chunks, std::size_t offset) noexcept
    -> ::amsr::core::Optional<std::pair<std::size_t, std::size_t>> {
  ::amsr::core::Optional<std::pair<std::size_t, std::size_t>> result{};
  std::size_t chunk_counter{0};
  bool success{false};
  for (EntryType const& current_chunk : chunks) {
    // Compare for >= to skip all chunks of size 0 in case the offset is 0.
    if (offset >= current_chunk.size) {
      offset -= current_chunk.size;
    } else {
      success = true;
      break;
    }
    chunk_counter++;
  }
  if (success) {
    result.emplace(std::pair<std::size_t, std::size_t>{chunk_counter, offset});
  }
  return result;
}

// VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_OSA_M5.0.16_OutOfBoundsCheck_FalsePositive
/*!
 * \brief           Serializes chunks of memory into a single buffer of contiguous bytes.
 *
 * \details         Only copies as much data as fits into the destination buffer.
 *
 * \param[in]       src      The buffer to copy from. The first copied byte is byte [offset].
 * \param[in]       dst      The buffer to copy to. The first written-to byte is byte [0].
 * \param[in]       offset   The offset of the first copied byte from the start of the source buffer.
 *
 * \tparam  EntryType Entry that represents one cohesive chunk of memory. An EntryType object contains at least the
 *                    following members:
 *                    - std::size_t size;
 *                    - void* (optionally const) base_pointer;
 *
 * \return          The number of bytes copied to the destination buffer. 0 if the source or destination buffer have
 *                  size 0 or the offset is beyond the last element of the source buffer.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
template <typename EntryType>
auto SerializeChunks(::amsr::core::Span<EntryType> src, ::amsr::core::Span<std::uint8_t> dst,
                     std::size_t offset = 0) noexcept -> std::size_t {
  std::size_t size_count{0};
  ::amsr::core::Optional<std::pair<std::size_t, std::size_t>> const first_byte_position{SplitOffset(src, offset)};

  if (first_byte_position.has_value()) {
    ::amsr::core::Span<std::uint8_t> remaining_destination_space{dst};
    ::amsr::core::Span<EntryType> relevant_chunks{src.subspan(first_byte_position.value().first)};

    std::size_t byte_offset{first_byte_position.value().second};

    for (EntryType const& current_chunk : relevant_chunks) {
      std::size_t const copied_size{std::min(current_chunk.size - byte_offset, remaining_destination_space.size())};
      // VECTOR Next Line AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_VoidPointerToPointerConversionForIoBuffer
      std::uint8_t const* first_copied_byte{static_cast<std::uint8_t const*>(current_chunk.base_pointer)};

      if ((byte_offset >= current_chunk.size) && (byte_offset != 0)) {  // COV_OSAB_INV_STATE
        ara::core::Abort("Internal assertion failed, offset into array larger than array size.");
      }

      // VECTOR Next Construct AutosarC++17_10-M5.0.15: MD_OSA_M5.0.15_PointerArithmeticIovec
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      first_copied_byte = &(first_copied_byte[byte_offset]);

      // VCA_OSA_STL_MEMCPY
      static_cast<void>(std::memcpy(remaining_destination_space.data(), first_copied_byte, copied_size));

      size_count += copied_size;
      remaining_destination_space = remaining_destination_space.subspan(copied_size);

      if (remaining_destination_space.empty()) {
        break;
      }

      byte_offset = 0;
    }
  }

  return size_count;
}

// VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_OSA_M5.0.16_OutOfBoundsCheck_FalsePositive
/*!
 * \brief           Deserializes a single buffer of contiguous bytes to chunks of memory.
 *
 * \details         Only copies as much data as fits into the destination buffer.
 *
 * \param[in]       src      The buffer to copy from. The first copied byte is byte [0].
 * \param[in]       dst      The buffer to copy to. The first written-to byte is byte [offset].
 * \param[in]       offset   The offset of the first copied byte from the start of the destination buffer.
 *
 * \tparam  EntryType Entry that represents one cohesive chunk of memory. An EntryType object contains at least the
 *                    following members:
 *                    - std::size_t size;
 *                    - void* base_pointer;
 *
 * \return          The number of bytes copied to the destination buffer.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
template <typename EntryType>
auto DeserializeSingleBuffer(::amsr::core::Span<std::uint8_t const> src, ::amsr::core::Span<EntryType> dst,
                             std::size_t offset = 0) noexcept -> std::size_t {
  std::size_t size_count{0};

  ::amsr::core::Optional<std::pair<std::size_t, std::size_t>> const first_byte_position{SplitOffset(dst, offset)};

  if (first_byte_position.has_value()) {
    ::amsr::core::Span<std::uint8_t const> remaining_source_space{src};
    ::amsr::core::Span<EntryType> relevant_chunks{dst.subspan(first_byte_position.value().first)};
    std::size_t byte_offset{first_byte_position.value().second};

    for (EntryType& current_chunk : relevant_chunks) {
      if (remaining_source_space.empty()) {
        break;
      }

      std::size_t const copied_size{std::min(current_chunk.size - byte_offset, remaining_source_space.size())};
      // VECTOR Next Line AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_VoidPointerToPointerConversionForIoBuffer
      std::uint8_t* first_copied_byte{static_cast<std::uint8_t*>(current_chunk.base_pointer)};

      if ((byte_offset >= current_chunk.size) && (byte_offset != 0)) {  // COV_OSAB_INV_STATE
        ara::core::Abort("Internal assertion failed, offset into array larger than array size.");
      }

      // VECTOR Next Construct AutosarC++17_10-M5.0.15: MD_OSA_M5.0.15_PointerArithmeticIovec
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      first_copied_byte = &(first_copied_byte[byte_offset]);

      // VCA_OSA_STL_MEMCPY, VCA_OSA_INBOUND
      static_cast<void>(std::memcpy(first_copied_byte, remaining_source_space.data(), copied_size));
      remaining_source_space = remaining_source_space.subspan(copied_size);

      size_count += copied_size;

      byte_offset = 0;
    }
  }

  return size_count;
}

}  // namespace internal
}  // namespace io
}  // namespace osabstraction

// COV_JUSTIFICATION_BEGIN
//   \ID COV_OSAB_INV_STATE
//     \ACCEPT XF
//     \REASON [COV_MSR_INV_STATE]
// COV_JUSTIFICATION_END

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_INTERNAL_IO_BUFFER_UTILS_H_

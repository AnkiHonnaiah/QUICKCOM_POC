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

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_INTERNAL_IO_BUFFER_QUEUE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_INTERNAL_IO_BUFFER_QUEUE_H_

#include <algorithm>  // Required for std::min and std::copy
#include <cstdint>
#include <iterator>  // Required for std::advance and std::distance
#include <type_traits>

#include "amsr/core/abort.h"
#include "amsr/core/array.h"
#include "amsr/core/span.h"

#include "osabstraction/io/internal/io_buffer_utils.h"
#include "osabstraction/io/io_buffer.h"

namespace osabstraction {
namespace io {
namespace internal {

/*!
 * \brief   Iterator over a buffer that is made up of chunks of memory and that is being processed over time.
 *
 * \details Memory being processed means future calls to acquire a representation of the memory handled by the buffer
 *          output a representation that does not contain the already processed memory any more.
 *
 *          Internally the class manages the first chunk of all chunks that have unprocessed memory left (this first
 *          chunk is referred to as "current chunk") separately because it (the current chunk) may have already some of
 *          its memory processed, all other chunks are guaranteed to have all their memory unprocessed.
 *
 * \tparam  EntryType Entry that represents one continuous chunk of memory that is part of this buffers underlying
 *          memory. An EntryType object contains at least the following members:
 *                    - void (const)* base_pointer;
 *                    - std::size_t size;
 */
template <typename EntryType>
class IoBufferQueue final {
 private:
  /*!
   * \brief Type used for the view of the bytes of the current chunk.
   */
  using ByteViewType = typename std::conditional<std::is_same<EntryType, MutableIOBuffer>::value, std::uint8_t,
                                                 std::uint8_t const>::type;

  // Constraints required because a void pointer is converted to a pointer of this type.
  static_assert(sizeof(ByteViewType) == 1, "ByteViewType of IoBufferQueue must have size of 1 byte.");
  static_assert(std::alignment_of<ByteViewType>::value == 1,
                "ByteViewType of IoBufferQueue must have alignment requirement of 1.");
  static_assert(std::is_standard_layout<ByteViewType>::value,
                "ByteViewType of IoBufferQueue must be of standard layout.");

 public:
  /*!
   * \brief           Creates an empty IO buffer queue.
   *
   * \details         The buffer consists of 0 bytes.
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
  IoBufferQueue() noexcept = default;

  /*!
   * \brief           Creates a new IO buffer queue.
   *
   * \details         All memory managed by the object is marked as not yet processed.
   *
   * \param[in]       chunks                                    All chunks of memory the buffer shall consist of.
   *
   * \note            The memory will be processed from the first byte of the first chunk to the last byte to the last
   *                  chunk.
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
  explicit IoBufferQueue(::amsr::core::Span<EntryType> chunks) noexcept
      : remaining_size_{CalculateAlloverSize(chunks)},
        complete_chunks_{(chunks.size() <= 1) ? ::amsr::core::Span<EntryType>{} : chunks.subspan(1)},
        current_chunk_unprocessed_memory_{
            chunks.empty()
                ? ::amsr::core::Span<ByteViewType>{}
                // VECTOR Disable AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_VoidPointerToPointerConversionForIoBuffer
                // VECTOR Disable AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastIoBufferAccess
                // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_PointerNullAtDereference_false_positive
                : ::amsr::core::Span<ByteViewType>{
                      reinterpret_cast<ByteViewType*>(chunks.cbegin()->base_pointer),
                      // VECTOR Enable AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_VoidPointerToPointerConversionForIoBuffer
                      // VECTOR Enable AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastIoBufferAccess
                      // VECTOR NC AutosarC++17_10-M0.3.1:MD_OSA_M0.3.1_PointerNullAtDereference_false_positive
                      chunks.cbegin()->size}} {}

  /*!
   * \brief           Marks parts of the buffer as already processed.
   *
   * \details         Should the buffer have less not-yet-processed bytes than are requested to be processed, only as
   *                  many bytes as remaining are marked as processed.
   *
   * \param[in]       consumed_bytes                            Number of bytes to mark as processed.
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
  /*!
   * \internal
   * - Check if any memory is remaining.
   * - If memory is remaining
   *   - check if the remaining memory of the current chunk is enough to satisfy the consumed amount.
   *   - If the remaining memory of the current chunk is sufficient
   *     - update the data structures that store how much memory the current chunk has left.
   *   - If the remaining memory of the current chunk is not sufficient
   *     - update the data structures that store which chunk is the current chunk and how much memory it has left.
   *   - Update the remaining size.
   * \endinternal
   */
  void Consume(std::size_t consumed_bytes) noexcept {
    if (remaining_size_ > 0) {
      std::size_t const reduced_by{std::min(consumed_bytes, remaining_size_)};

      if (reduced_by < current_chunk_unprocessed_memory_.size()) {
        // Never entered in case the current chunk has exactly 0 unprocessed bytes left because std::size_t is
        // unsigned and can thus not be smaller than 0.
        current_chunk_unprocessed_memory_ =
            current_chunk_unprocessed_memory_.subspan(reduced_by);  // VCA_OSA_EXT_CALL_OBJ_PASS_VALUE
      } else {
        UpdateCurrentChunk(reduced_by);
      }

      this->remaining_size_ -= reduced_by;
    }
  }

  /*!
   * \brief           Checks if the entire buffer has already processed.
   *
   * \return          True if the entire buffer has been processed, else false.
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
  bool CompletelyProcessed() const noexcept { return remaining_size_ == 0; }

  /*!
   * \brief           Outputs the number of bytes of the buffer that have not been processed yet.
   *
   * \return          The number of bytes that have not been processed yet.
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
  std::size_t GetRemainingSize() const noexcept { return remaining_size_; }

  /*!
   * \brief           Outputs a representation of the memory managed by this buffer.
   *
   * \details         After parts or all of this memory has been used, it should be marked as processed with Consume().
   *
   * \param[in, out]  buffer                                    Buffer to write the representation of the memory to.
   *
   * \return          The number of chunks of memory that have unprocessed bytes left in them, 0 if the entire buffer
   *                  has been processed.
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
  /*!
   * \internal
   * - If all memory has already been processed
   *   - output that 0 entries have unprocessed memory.
   * - If the input buffer can hold anything or some memory is not yet processed
   *   - fill the input buffer until it is either completely filled or all entries of remaining chunks with
   *     unprocessed memory have been copied to it.
   *   - Output the number of entries that have been copied to the buffer.
   * \endinternal
   */
  std::size_t GetMemory(::amsr::core::Span<EntryType> buffer) const noexcept {
    std::size_t copied_entries{0};

    if (!CompletelyProcessed()) {
      copied_entries = FillBuffer(buffer);  // VCA_OSA_EXT_CALL_OBJ_PASS_VALUE
    }

    return copied_entries;
  }

 private:
  /*!
   * \brief           Updates the cached information about the first chunk that has unprocessed memory.
   *
   * \param[in]       consumed_bytes                            Number of bytes to update the information about.
   *
   * \context         ANY
   *
   * \pre             The current chunk must have at most as many bytes as the number in consumed_bytes.
   *                  (consumed_bytes >= current_chunk_unprocessed_memory_.size()).
   * \pre             The buffer must have at least one unprocessed byte (remaining_size_ > 0).
   * \pre             The buffer must have enough unprocessed bytes (remaining_size_ >= consumed_bytes).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Search the next chunk that will have unprocessed memory left after all bytes have been consumed.
   * - Update the data structures that manage the unprocessed memory.
   * \endinternal
   */
  void UpdateCurrentChunk(std::size_t consumed_bytes) noexcept {
    consumed_bytes -= current_chunk_unprocessed_memory_.size();
    current_chunk_unprocessed_memory_ = ::amsr::core::Span<ByteViewType>{};

    typename ::amsr::core::Span<EntryType>::iterator current_chunk{complete_chunks_.begin()};

    while ((current_chunk != complete_chunks_.end()) && (consumed_bytes > current_chunk->size)) {
      consumed_bytes -= current_chunk->size;
      std::advance(current_chunk, 1);  // VCA_OSA_STL_CALL_OBJ_ITERATE
    }

    if (current_chunk != complete_chunks_.end()) {
      // This path is always entered in case there is at least one chunk with all memory unprocessed when this function
      // is entered (that means (complete_chunks_.begin() == complete_chunks_.end()) does not hold true when the
      // function is entered).
      //
      // Reason:
      // The preconditions assure (remaining_size_ >= consumed_bytes) that at most as much memory as still available is
      // consumed. This means that after the second-to-last chunk is consumed at most as many bytes as are in the last
      // chunk remain to be consumed. The effect of this is that after consuming the second-to-last chunk, current_chunk
      // is advanced to point to the last chunk. Checking the loop condition can not hold at this point as the bytes
      // remaining to be consumed are at most as many bytes as are contained in the last chunk so the check
      // (consumed_bytes > current_chunk->size) of the loop condition can not hold true. This means the loop is not
      // entered. As current_chunk at this point still refers the last chunk this if path is entered.

      // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_VoidPointerToPointerConversionForIoBuffer
      // VECTOR Next Construct AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastIoBufferAccess
      current_chunk_unprocessed_memory_ =
          ::amsr::core::Span<ByteViewType>{reinterpret_cast<ByteViewType*>(current_chunk->base_pointer),
                                           current_chunk->size}
              .subspan(consumed_bytes);

      std::advance(current_chunk, 1);
      complete_chunks_ = ::amsr::core::Span<EntryType>{current_chunk, complete_chunks_.end()};
    }
  }

  /*!
   * \brief           Fills a buffer until it is either full or contains all entries of unprocessed memory.
   *
   * \param[in, out]  buffer                                    Buffer to fill.
   *
   * \return          The number of entries the buffer has been filled with.
   *
   * \context         ANY
   *
   * \pre             There is unprocessed memory left.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - If the buffer can hold at least one entry:
   *   - Set the first entry of the buffer to represent the remaining memory of the first chunk that has unprocessed
   *     memory left.
   *   - Copy entries of the chunks with completely unprocessed memory to the buffer until it is full or all entries
   *     have been copied.
   *   - Output the number of entries that have been added to the buffer.
   * \endinternal
   */
  std::size_t FillBuffer(::amsr::core::Span<EntryType> buffer) const noexcept {
    std::size_t copied_elements{0};
    typename ::amsr::core::Span<EntryType>::iterator complete_chunk_target{buffer.begin()};
    if (complete_chunk_target != buffer.end()) {
      // VCA_OSA_INBOUND
      *complete_chunk_target =
          EntryType{current_chunk_unprocessed_memory_.data(), current_chunk_unprocessed_memory_.size()};
      std::advance(complete_chunk_target, 1);  // VCA_OSA_STL_CALL_OBJ_ITERATE

      std::size_t const amount_copied_complete_chunks{std::min(buffer.size() - 1, complete_chunks_.size())};
      ::amsr::core::Span<EntryType> const copied_complete_chunks{complete_chunks_.first(amount_copied_complete_chunks)};

      if (!copied_complete_chunks.empty()) {
        // VCA_OSA_STL_COPY
        static_cast<void>(
            std::copy(copied_complete_chunks.cbegin(), copied_complete_chunks.cend(), complete_chunk_target));
      }

      // + 1 for the first chunk that has unprocessed memory left as it is not part of complete_chunks_.
      copied_elements = amount_copied_complete_chunks + 1;
    }

    return copied_elements;
  }

  /*!
   * \brief Number of bytes that have not yet been processed.
   */
  std::size_t remaining_size_{0};

  /*!
   * \brief The chunks of the buffer that have all memory unprocessed.
   *
   * \note  There is one chunk that has unprocessed memory left that is not managed by this span. That is the chunk
   *        whose memory is managed by current_chunk_unprocessed_memory_ (the current chunk).
   */
  ::amsr::core::Span<EntryType> complete_chunks_{};

  /*!
   * \brief Memory of the current chunk that has not yet been processed.
   */
  ::amsr::core::Span<ByteViewType> current_chunk_unprocessed_memory_{};
};

}  // namespace internal
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_INTERNAL_IO_BUFFER_QUEUE_H_

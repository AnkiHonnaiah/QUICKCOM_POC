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
 *        \brief  Manages a lock-free ring buffer in shared memory.
 *        \unit   osabstraction::ipc::IpcConnectionMessageHandling_QNX
 *        \unit   osabstraction::ipc::IpcConnectionMessageHandling_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_RING_BUFFER_VIEW_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_RING_BUFFER_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_RingBufferView
/*!
 * \brief    Manages a lock-free ring buffer in shared memory.
 *
 * \details  This class does not own the shared memory. It only provides functions to operate on the ring buffer in the
 *           shared memory.
 */
class RingBufferView final {
  // Assert that the atomics that are used in the ring buffer are always lock-free, otherwise they cannot be used to
  // synchronize access to the ring buffer in the shared memory. It is assumed that lock-free atomics are also address-
  // free on all architectures supported by AMSR. This is also a precondition for the lock-free ring buffer algorithm to
  // work in shared memory but this cannot be checked by the compiler.
  static_assert(ATOMIC_INT_LOCK_FREE == 2,
                "std::atomic<std::uint32_t> is not always lock-free and can therefore not be used in shared memory.");

  // Assert that std::atomic<std::uint32_t> has the same size as std::uint32_t. This reduces the risk of a
  // reinterpret_cast between the two types.
  static_assert(
      sizeof(std::atomic<std::uint32_t>) == sizeof(std::uint32_t),
      "std::atomic<std::uint32_t> does not have the same size as std::uint32_t and can therefore not be used.");

 public:
  /*!
   * \brief           Creates a RingBufferView that manages no ring buffer.
   *
   * \details         No operations should be done on the RingBufferView except for copy/move assignment or destruction.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  RingBufferView() noexcept : RingBufferView(nullptr, nullptr, ::amsr::core::Span<std::uint8_t>{}) {}

  /*!
   * \brief           Creates a RingBufferView that manages a ring buffer.
   *
   * \param[in]       head_index    Pointer to the location in the shared memory where the head index of the ring buffer
   *                                is located.
   * \param[in]       tail_index    Pointer to the location in the shared memory where the tail index of the ring buffer
   *                                is located.
   * \param[in]       data_buffer   View on the byte array that is used to store the ring buffer data. The byte array
   *                                shall not be larger than UINT32_MAX / 2 bytes.
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
   *  - Initialize all fields.
   *  - Assert that the size of the passed buffer does not exceed the maximum supported size. The maximum supported size
   *    guarantees that the pointer arithmetics performed by this class never overflow.
   * \endinternal
   */
  RingBufferView(std::atomic<std::uint32_t>* head_index, std::atomic<std::uint32_t>* tail_index,
                 ::amsr::core::Span<std::uint8_t> data_buffer) noexcept
      : head_index_ptr_{head_index}, tail_index_ptr_{tail_index}, data_buffer_{data_buffer} {
    if (data_buffer.size() > (std::numeric_limits<std::uint32_t>::max() / 2)) {
      ::amsr::core::Abort("Buffer passed to RingBufferRef constructor is too large.");
    }
  }

  /*!
   * \brief Copy construct RingBufferView
   *
   * \param[in]   other   RingBufferView that shall be copied.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  RingBufferView(RingBufferView const& other) noexcept = default;

  /*!
   * \brief Move construct RingBufferView
   *
   * \param[in]   other   RingBufferView that shall be moved.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  RingBufferView(RingBufferView&& other) noexcept = default;
  /*!
   * \brief Assigns new RingBufferView
   *
   * \param[in]   other   RingBufferView that shall be copied.
   *
   * \return Reference to this object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  RingBufferView& operator=(RingBufferView const& other) & noexcept = default;

  /*!
   * \brief Moves RingBufferView
   *
   * \param[in]   other   RingBufferView that shall be moved.
   *
   * \return Reference to this object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  RingBufferView& operator=(RingBufferView&& other) & noexcept = default;

  /*!
   * \brief           Destroys the RingBufferView.
   *
   * \details         Does not invalidate the ring buffer in the shared memory.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~RingBufferView() noexcept = default;

  /*!
   * \brief           Loads the atomic head index of the ring buffer in the shared memory and caches it locally.
   *
   * \details         Other RingBufferView APIs then work on this cached value and do not load the atomic head index
   *                  again.
   *                  This function also performs a sanity check of the loaded head index: It is checked that the
   *                  head index does not point outside of the buffer and that it did not advance in the wrong
   *                  direction.
   *
   * \error           osabstraction::OsabErrc::kProtocolError   Sanity check failed, the IPC protocol was violated.
   *                                                            This object is now in an invalid state and calls to
   *                                                            APIs other than GetCapacity() can produce undefined
   *                                                            results and should be avoided.
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Get the number of bytes that the ring buffer currently contains.
   *  - Load the atomic head index with sequentially consistent memory ordering and update the cached head index
   *    with the returned value.
   *  - If the head index does now point outside of the buffer or if the number of bytes that the ring buffer now
   *    contains decreased
   *    - Return a protocol error.
   *  - Otherwise return success.
   * \endinternal
   */
  auto LoadHeadIndex() noexcept -> ::amsr::core::Result<void> {
    ::amsr::core::Result<void> result{};

    std::uint32_t const used_space_before{GetUsedSpace()};
    current_head_index_ = head_index_ptr_->load(std::memory_order_seq_cst);
    if ((current_head_index_ >= data_buffer_.size()) || (used_space_before > GetUsedSpace())) {
      result.EmplaceError(
          osabstraction::MakeErrorCode(osabstraction::OsabErrc::kProtocolError,
                                       "Head index of ring buffer in shared memory was corrupted by the IPC peer."));
    }

    return result;
  }

  /*!
   * \brief           Loads the atomic tail index of the ring buffer in the shared memory and caches it locally.
   *
   * \details         Other RingBufferView APIs then work on this cached value and do not load the atomic tail index
   *                  again.
   *                  This function also performs a sanity check of the loaded tail index: It is checked that the
   *                  tail index does not point outside of the buffer and that it did not advance in the wrong
   *                  direction.
   *
   * \error           osabstraction::OsabErrc::kProtocolError   Sanity check failed, the IPC protocol was violated.
   *                                                            This object is now in an invalid state and calls to APIs
   *                                                            other than GetCapacity() can produce undefined results
   *                                                            and should be avoided.
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Get the number of bytes that the ring buffer currently contains.
   *  - Load the atomic tail index with sequentially consistent memory ordering and update the cached tail index
   *    with the returned value.
   *  - If the tail index does now point outside of the buffer or if the number of bytes that the ring buffer now
   *    contains increased
   *    - Return a protocol error.
   *  - Otherwise return success.
   * \endinternal
   */
  auto LoadTailIndex() noexcept -> ::amsr::core::Result<void> {
    ::amsr::core::Result<void> result{};

    std::uint32_t const used_space_before{GetUsedSpace()};
    current_tail_index_ = tail_index_ptr_->load(std::memory_order_seq_cst);
    if ((current_tail_index_ >= data_buffer_.size()) || (used_space_before < GetUsedSpace())) {
      result.EmplaceError(osabstraction::OsabErrc::kProtocolError,
                          "Tail index of ring buffer in shared memory was corrupted by the IPC peer.");
    }

    return result;
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectSharedMemory
  /*!
   * \brief           Stores the locally cached head index to the atomic head index of the ring buffer in the shared
   *                  memory.
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Store the locally cached head index to the atomic head index of the ring buffer in the shared memory with
   *    sequentially consistent memory ordering.
   * \endinternal
   */
  void StoreHeadIndex() noexcept {
    // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
    head_index_ptr_->store(current_head_index_, std::memory_order_seq_cst);
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectSharedMemory
  /*!
   * \brief           Stores the locally cached tail index to the atomic tail index of the ring buffer in the shared
   *                  memory.
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Store the locally cached tail index to the atomic tail index of the ring buffer in the shared memory with
   *    sequentially consistent memory ordering.
   * \endinternal
   */
  void StoreTailIndex() noexcept {
    // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
    tail_index_ptr_->store(current_tail_index_, std::memory_order_seq_cst);
  }

  /*!
   * \brief           Checks if the ring buffer is empty.
   *
   * \details         This function works with the cached index values.
   *
   * \return          true if the ring buffer is empty, false otherwise.
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Return if the ring buffer is empty. The ring buffer is empty if head and tail indices are equal.
   * \endinternal
   */
  auto IsEmpty() const noexcept -> bool { return current_head_index_ == current_tail_index_; }

  /*!
   * \brief           Checks if the ring buffer is full.
   *
   * \details         This function works with the cached index values.
   *
   * \return          true if the ring buffer is full, false otherwise.
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Return if the ring buffer is full. The ring buffer is full if the used space of the ring buffer is equal to
   *    the ring buffer capacity.
   * \endinternal
   */
  auto IsFull() const noexcept -> bool { return GetUsedSpace() == GetCapacity(); }

  /*!
   * \brief           Gets the number of bytes that the ring buffer currently contains.
   *
   * \details         This function works with the cached index values.
   *
   * \return          Number of bytes in the ring buffer.
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - If the head index is larger or equal to the tail index
   *    - The number of bytes in the ring buffer is equal to the difference of the head and the tail index.
   *  - Otherwise
   *    - The number of bytes in the ring buffer is equal to the sum of the data buffer size and the head index
   *      minus the tail index.
   *  - Return the calculated number of bytes.
   * \endinternal
   */
  auto GetUsedSpace() const noexcept -> std::uint32_t {
    std::uint32_t used_space{};
    if (current_head_index_ >= current_tail_index_) {
      used_space = current_head_index_ - current_tail_index_;
    } else {
      // RingBufferView constructor guarantees that data_buffer is not be larger than UINT32_MAX/2 bytes.
      used_space = (static_cast<std::uint32_t>(data_buffer_.size()) + current_head_index_) - current_tail_index_;
    }
    return used_space;
  }

  /*!
   * \brief           Gets the free space of the ring buffer.
   *
   * \details         This function works with the cached index values.
   *
   * \return          Free space of the ring buffer (in bytes).
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Return the difference between the ring buffer capacity and the used space of the ring buffer.
   * \endinternal
   */
  auto GetFreeSpace() const noexcept -> std::uint32_t { return GetCapacity() - GetUsedSpace(); }

  /*!
   * \brief           Gets the maximum number of bytes that can be stored in the ring buffer.
   *
   * \details         This function works with the cached index values.
   *
   * \return          Capacity of the ring buffer (in bytes).
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Return the capacity of the ring buffer. The capacity is 1 byte smaller than the buffer size as 1 byte of the
   *    buffer always has to be unused so that the head index is not equal to the tail index if the buffer is
   *    completely full. Otherwise one could not differentiate between the buffer being completely full and completely
   *    empty. The byte that is always unused is the byte that the head index points to.
   * \endinternal
   */
  auto GetCapacity() const noexcept -> std::uint32_t {
    // RingBufferView constructor guarantees that data_buffer is not be larger than UINT32_MAX/2 bytes.
    return static_cast<std::uint32_t>(data_buffer_.size()) - 1;
  }

  /*!
   * \brief           Writes data into the ring buffer.
   *
   * \details         This function works with the cached index values.
   *
   * \param[in]       input_buffer   View on the data that shall be copied into the ring buffer. The ring buffer has
   *                                 to have enough free space for the data (see GetFreeSpace()).
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Calculate the space in the data buffer from the head index until the end.
   *  - If the calculated space is sufficient for the data that shall be written
   *    - Copy the data into the data buffer starting at the location that the head index points to.
   *  - Otherwise
   *    - Copy the first part of the data into the data buffer starting at the location that the head index points
   *      to until the end of the data buffer is reached.
   *    - Copy the rest of the data into the data buffer starting at the beginning of the data buffer.
   *  - Advance the head index by the number of bytes that were written.
   * \endinternal
   */
  void Write(::amsr::core::Span<std::uint8_t const> input_buffer) noexcept {
    // We require from the user that the size of the input_buffer is not larger than the free space. Because the
    // free space will always be less than the data_buffer_size_ it is guaranteed that at least 1 byte of the data
    // buffer will always be empty.

    ::amsr::core::Span<std::uint8_t> const remaining_buffer_space{data_buffer_.subspan(current_head_index_)};
    std::size_t const first_chunk_size{std::min(remaining_buffer_space.size(), input_buffer.size())};
    std::size_t const second_chunk_size{input_buffer.size() - first_chunk_size};

    // VCA_OSA_STL_MEMCPY
    std::memcpy(remaining_buffer_space.data(), input_buffer.data(), first_chunk_size);
    if (second_chunk_size != 0) {
      // VCA_OSA_STL_MEMCPY
      std::memcpy(data_buffer_.data(), input_buffer.subspan(first_chunk_size).data(), second_chunk_size);
    }

    AdvanceIndex(current_head_index_, static_cast<std::uint32_t>(input_buffer.size()));
  }

  /*!
   * \brief           Reads data from the ring buffer but does not remove the read data from the ring buffer.
   *
   * \details         This function works with the cached index values.
   *
   * \param[out]      output_buffer   View on a buffer where the read data shall be copied to. This buffer is always
   *                                  filled completely. The ring buffer has to have enough data available to fill
   *                                  the buffer (see GetUsedSpace()).
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Calculate the space in the data buffer from the tail index until the end.
   *  - If the calculated space is sufficient for the data that shall be read
   *    - Copy the data from the data buffer starting at the location that the tail index points to.
   *  - Otherwise
   *    - Copy the first part of the data from the data buffer starting at the location that the tail index points
   *      to until the end of the data buffer is reached.
   *    - Copy the rest of the data from the data buffer starting at the beginning of the data buffer.
   * \endinternal
   */
  void Peek(::amsr::core::Span<std::uint8_t> output_buffer) const noexcept {
    ::amsr::core::Span<std::uint8_t const> const remaining_buffer_space{data_buffer_.subspan(current_tail_index_)};
    std::size_t const first_chunk_size{std::min(remaining_buffer_space.size(), output_buffer.size())};
    std::size_t const second_chunk_size{output_buffer.size() - first_chunk_size};

    // VCA_OSA_STL_MEMCPY
    std::memcpy(output_buffer.data(), remaining_buffer_space.data(), first_chunk_size);
    if (second_chunk_size != 0) {
      // VCA_OSA_STL_MEMCPY
      std::memcpy(output_buffer.subspan(first_chunk_size).data(), data_buffer_.data(), second_chunk_size);
    }
  }

  /*!
   * \brief           Discards bytes from the read end of the ring buffer.
   *
   * \details         This function works with the cached index values.
   *
   * \param[in]       num_bytes   Number of bytes that shall be discarded. The ring buffer has to contain at least
   *                              num_bytes bytes (see GetUsedSpace()).
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Advance the tail index by the passed number of bytes.
   * \endinternal
   */
  void Discard(std::uint32_t num_bytes) noexcept { AdvanceIndex(current_tail_index_, num_bytes); }

  /*!
   * \brief           Reads data from the ring buffer.
   *
   * \details         This function works with the cached index values.
   *
   * \param[out]      output_buffer   View on a buffer where the read data shall be copied to. This buffer is always
   *                                  filled completely. The ring buffer has to have enough data available to fill
   *                                  the buffer (see GetUsedSpace()).
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Peek the ring buffer.
   *  - Discard the peeked bytes.
   * \endinternal
   */
  void Read(::amsr::core::Span<std::uint8_t> output_buffer) noexcept {
    Peek(output_buffer);
    Discard(static_cast<std::uint32_t>(output_buffer.size()));
  }

 private:
  /*!
   * \brief           Advances an index (head or tail index) by a certain number of bytes.
   *
   * \details         This function works with the cached index values.
   *
   * \param[in]       index_to_advance  Reference to the index that shall be advanced.
   * \param[in]       count             Number of bytes that the index shall be advanced by. This shall not cause
   *                                    the ring buffer to overflow.
   *
   * \context         ANY
   *
   * \pre             The RingBufferView manages a ring buffer.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Advance the passed index by the passed number of bytes.
   *  - If the passed index now points outside of the data buffer
   *    - A wrap-around of the index occurred. Subtract the size of the data buffer from the index.
   * \endinternal
   */
  void AdvanceIndex(std::uint32_t& index_to_advance, std::uint32_t count) const noexcept {
    index_to_advance += count;
    if (index_to_advance >= data_buffer_.size()) {
      // RingBufferView constructor guarantees that data_buffer is not be larger than UINT32_MAX/2 bytes.
      index_to_advance -= static_cast<std::uint32_t>(data_buffer_.size());
    }
  }

  /*!
   * \brief   Points to the atomic head index of the ring buffer in the shared memory.
   */
  std::atomic<std::uint32_t>* head_index_ptr_{};

  /*!
   * \brief   Locally cached value of the ring buffer head index.
   */
  std::uint32_t current_head_index_{0};

  /*!
   * \brief   Points to the atomic tail index of the ring buffer in the shared memory.
   */
  std::atomic<std::uint32_t>* tail_index_ptr_{};

  /*!
   * \brief   Locally cached value of the ring buffer tail index.
   */
  std::uint32_t current_tail_index_{0};

  /*!
   * \brief     View to managed buffer.
   *
   * \details   The managed memory is defined once during creation.
   */
  ::amsr::core::Span<std::uint8_t> data_buffer_{};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_RING_BUFFER_VIEW_H_

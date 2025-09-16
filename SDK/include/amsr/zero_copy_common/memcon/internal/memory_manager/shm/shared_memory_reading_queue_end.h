/*!********************************************************************************************************************
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
/*!       \file
 *        \brief  Definition and Implementation of SharedMemoryReadingQueueEnd class.
 *        \unit   ZeroCopyCommon::MemCon::MemoryManager
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_READING_QUEUE_END_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_READING_QUEUE_END_H_

#include "amsr/core/optional.h"
#include "amsr/core/result.h"

#include "amsr/memory/container/types.h"
#include "amsr/memory/read_writable_typed_memory_view.h"
#include "amsr/memory/readable_typed_memory_view.h"

#include "amsr/zero_copy_common/common/zero_copy_common_error_domain.h"
#include "amsr/zero_copy_common/external/memory/container/spsc_queue_consumer.h"
#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {
namespace shm {

/*!
 * \brief SharedMemoryReadingQueueEnd class.
 * \details Allow to peek and pop SlotIndex instances from the queue.
 *          Do not allow to push SlotIndex instances to the queue.
 *          Class is nothrow constructible and destructible.
 *          All queue operations are done lock-free by using atomic values in shared memory.
 * \vprivate Component Private
 */
class SharedMemoryReadingQueueEnd final : public ::amsr::zero_copy_common::logic::internal::ReadingQueueEndInterface {
 public:
  /*!
   * \brief  Alias of the SlotIndex and QueueIndex types.
   */
  using BufferType = NumSlots::Type;
  using IndexType = ::amsr::memory::container::QueueIndex;

  /*!
   * \brief       Construct a SharedMemoryReadingQueueEnd.
   * \param[in]   view_to_buffer  view to the readable mapped memory for the queue elements buffer.
   *                              Size of the view_to_buffer has to be lower or equal than UINT32_MAX/2.
   *                              This is checked in SPSCQueueConsumer.
   *                              The pointed-to memory must outlive this instance of SharedMemoryReadingQueueEnd.
   *                              This cannot be checked.
   * \param[in]   view_to_head    view to the readable mapped memory for the queue head index.
   *                              Referenced view_to_head address has to be 8-byte aligned.
   *                              Size of the span has to be at least 1. This is checked in SPSCQueueConsumer.
   *                              The pointed-to memory must outlive this instance of SharedMemoryReadingQueueEnd.
   *                              This cannot be checked.
   * \param[in]   view_to_tail    view to the readable, writable mapped memory for the queue tail index.
   *                              Referenced view_to_tail address has to be 8-byte aligned.
   *                              Size of the span has to be at least 1. This is checked in SPSCQueueConsumer.
   *                              The pointed-to memory must outlive this instance of SharedMemoryReadingQueueEnd.
   *                              This cannot be checked.
   * \context     ANY
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \vprivate Component Private
   */
  explicit SharedMemoryReadingQueueEnd(::amsr::memory::ReadableTypedMemoryView<BufferType> view_to_buffer,
                                       ::amsr::memory::ReadableTypedMemoryView<IndexType> view_to_head,
                                       ::amsr::memory::ReadWritableTypedMemoryView<IndexType> view_to_tail) noexcept
      : ::amsr::zero_copy_common::logic::internal::ReadingQueueEndInterface{},
        spsc_consumer_{view_to_buffer, view_to_head, view_to_tail} {};

  /*!
   * \brief Destruct the SharedMemoryReadingQueueEnd.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~SharedMemoryReadingQueueEnd() noexcept final = default;

  /*!
   * \brief Construct a SharedMemoryReadingQueueEnd by copying.
   * \vprivate Component Private
   */
  SharedMemoryReadingQueueEnd(SharedMemoryReadingQueueEnd const&) noexcept = delete;

  /*!
   * \brief Construct a SharedMemoryReadingQueueEnd by moving.
   * \vprivate Component Private
   */
  SharedMemoryReadingQueueEnd(SharedMemoryReadingQueueEnd&&) noexcept = default;

  /*!
   * \brief Copy another SharedMemoryReadingQueueEnd into this instance.
   * \vprivate Component Private
   */
  auto operator=(SharedMemoryReadingQueueEnd const&) & noexcept -> SharedMemoryReadingQueueEnd& = delete;

  /*!
   * \brief Move another SharedMemoryReadingQueueEnd into this instance.
   * \vprivate Component Private
   */
  auto operator=(SharedMemoryReadingQueueEnd&&) & noexcept -> SharedMemoryReadingQueueEnd& = default;

  /*!
   * \copydoc ::amsr::zerocopycommon::logic::internal::ReadingQueueEndInterface::Peek()
   */
  auto Peek() noexcept -> ::amsr::core::Result<::amsr::core::Optional<BufferType>> final {
    BufferType index{};
    ::amsr::core::Result<::amsr::core::Optional<BufferType>> result{
        ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kQueueError};
    // VCA_ZEROCOPYCOMMON_SPC_15_SPSCQUEUE_CONSUMER_PRODUCER_MEMBER_FUNCTIONS
    ::amsr::core::Result<bool> peek_result{spsc_consumer_.Peek(index)};

    if (peek_result.HasValue()) {
      if (peek_result.Value()) {
        result.EmplaceValue(::amsr::core::Optional<BufferType>{index});
      } else {
        result.EmplaceValue(::amsr::core::Optional<BufferType>{});
      }
    }
    return result;
  }

  /*!
   * \copydoc ::amsr::zerocopycommon::logic::internal::ReadingQueueEndInterface::Pop()
   */
  auto Pop() noexcept -> ::amsr::core::Result<::amsr::core::Optional<BufferType>> final {
    BufferType index{};
    ::amsr::core::Result<::amsr::core::Optional<BufferType>> result{
        ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kQueueError};
    // VCA_ZEROCOPYCOMMON_SPC_15_SPSCQUEUE_CONSUMER_PRODUCER_MEMBER_FUNCTIONS
    ::amsr::core::Result<bool> consume_result{spsc_consumer_.Consume(index)};

    if (consume_result.HasValue()) {
      if (consume_result.Value()) {
        result.EmplaceValue(::amsr::core::Optional<BufferType>{index});
      } else {
        result.EmplaceValue(::amsr::core::Optional<BufferType>{});
      }
    }
    return result;
  };

 private:
  /*!
   * \brief  Consumer object of single-producer-single-consumer queue.
   */
  ::amsr::zero_copy_common::external::memory::container::SPSCQueueConsumer<BufferType> spsc_consumer_;
};

}  // namespace shm
}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_READING_QUEUE_END_H_

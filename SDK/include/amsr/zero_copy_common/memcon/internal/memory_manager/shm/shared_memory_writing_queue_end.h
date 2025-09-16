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
/*!        \file
 *        \brief  Definition and Implementation of SharedMemoryWritingQueueEnd class.
 *         \unit  ZeroCopyCommon::MemCon::MemoryManager
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_WRITING_QUEUE_END_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_WRITING_QUEUE_END_H_

#include "amsr/core/optional.h"
#include "amsr/core/result.h"

#include "amsr/memory/container/types.h"
#include "amsr/memory/read_writable_typed_memory_view.h"
#include "amsr/memory/readable_typed_memory_view.h"

#include "amsr/zero_copy_common/common/zero_copy_common_error_domain.h"
#include "amsr/zero_copy_common/external/memory/container/spsc_queue_producer.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"

#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {
namespace shm {
/*!
 * \brief SharedMemoryWritingQueueEnd class.
 * \details Allow to push SlotIndex instances to the queue.
 *          Do not allow to peek and pop SlotIndex instances from the queue.
 *          Class is nothrow constructible and destructible.
 *          All queue operations are done lock-free by using atomic values in shared memory.
 * \vprivate Component Private
 */
class SharedMemoryWritingQueueEnd final : public ::amsr::zero_copy_common::logic::internal::WritingQueueEndInterface {
 public:
  /*!
   * \brief  Alias of the SlotIndex and QueueIndex types.
   */
  using BufferType = NumSlots::Type;
  using IndexType = ::amsr::memory::container::QueueIndex;

  /*!
   * \brief       Construct a SharedMemoryWritingQueueEnd.
   * \param[in]   view_to_buffer  view to the readable, writable mapped memory for the queue elements buffer.
   *                              Size of the view_to_buffer has to be lower or equal than UINT32_MAX/2.
   *                              This is checked in SPSCQueueProducer.
   *                              The pointed-to memory must outlive this instance of SharedMemoryWritingQueueEnd.
   *                              This cannot be checked.
   * \param[in]   view_to_head    view to the readable, writable mapped memory for the queue head index.
   *                              Referenced view_to_head address has to be 8-byte aligned.
   *                              Size of the span has to be at least 1. This is checked in SPSCQueueProducer.
   *                              The pointed-to memory must outlive this instance of SharedMemoryWritingQueueEnd.
   *                              This cannot be checked.
   * \param[in]   view_to_tail    view to the readable mapped memory for the queue tail index.
   *                              Referenced view_to_tail address has to be 8-byte aligned.
   *                              Size of the span has to be at least 1. This is checked in SPSCQueueProducer.
   *                              The pointed-to memory must outlive this instance of SharedMemoryWritingQueueEnd.
   *                              This cannot be checked.
   * \context     ANY
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \vprivate Component Private
   */
  explicit SharedMemoryWritingQueueEnd(::amsr::memory::ReadWritableTypedMemoryView<BufferType> view_to_buffer,
                                       ::amsr::memory::ReadWritableTypedMemoryView<IndexType> view_to_head,
                                       ::amsr::memory::ReadableTypedMemoryView<IndexType> view_to_tail) noexcept
      : ::amsr::zero_copy_common::logic::internal::WritingQueueEndInterface{},
        spsc_producer_{view_to_buffer, view_to_head, view_to_tail} {};

  /*!
   * \brief Destruct the SharedMemoryWritingQueueEnd.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~SharedMemoryWritingQueueEnd() noexcept final = default;

  /*!
   * \brief Construct a SharedMemoryWritingQueueEnd by copying.
   * \vprivate Component Private
   */
  SharedMemoryWritingQueueEnd(SharedMemoryWritingQueueEnd const&) noexcept = delete;

  /*!
   * \brief Construct a SharedMemoryWritingQueueEnd by moving.
   * \vprivate Component Private
   */
  SharedMemoryWritingQueueEnd(SharedMemoryWritingQueueEnd&&) noexcept = default;

  /*!
   * \brief Copy another SharedMemoryWritingQueueEnd into this instance.
   * \vprivate Component Private
   */
  auto operator=(SharedMemoryWritingQueueEnd const&) & noexcept -> SharedMemoryWritingQueueEnd& = delete;

  /*!
   * \brief Move another SharedMemoryWritingQueueEnd into this instance.
   * \vprivate Component Private
   */
  auto operator=(SharedMemoryWritingQueueEnd&&) & noexcept -> SharedMemoryWritingQueueEnd& = default;

  /*!
   * \copydoc ::amsr::zerocopycommon::logic::internal::WritingQueueEndInterface::Push()
   */
  auto Push(BufferType index) noexcept -> ::amsr::core::Result<bool> final {
    ::amsr::core::Result<bool> result{::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kQueueError};
    // VCA_ZEROCOPYCOMMON_SPC_15_SPSCQUEUE_CONSUMER_PRODUCER_MEMBER_FUNCTIONS
    ::amsr::core::Result<bool> produce_result{spsc_producer_.Produce(index)};

    if (produce_result.HasValue()) {
      result.EmplaceValue(produce_result.Value());
    }
    return result;
  }

 private:
  /*!
   * \brief  Producer object of single-producer-single-consumer queue.
   */
  ::amsr::zero_copy_common::external::memory::container::SPSCQueueProducer<BufferType> spsc_producer_;
};

}  // namespace shm
}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_WRITING_QUEUE_END_H_

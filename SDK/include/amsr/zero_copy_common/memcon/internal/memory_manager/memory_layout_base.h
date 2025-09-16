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
 *         \brief Definition of class MemoryLayoutBase.
 *         \unit ZeroCopyCommon::MemCon::MemoryManager
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_MEMORY_LAYOUT_BASE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_MEMORY_LAYOUT_BASE_H_

#include <cstddef>
#include <memory>
#include <utility>

#include "amsr/memory/container/types.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_memory_view.h"
#include "amsr/memory/readable_memory_view.h"

#include "amsr/zero_copy_common/logic/internal/readable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/writable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {

/*!
 * \brief    Base for a memory layout which is used to configure slot and queue memory in a given memory technology.
 * \details  A memory layout has the knowledge of the internal structures of a slot and a queue in a given memory
 *           technology. It provides information such as the memory size required for slots and queues, and
 *           derives slot descriptors and queue ends to access readable and writable parts of a slot and a queue.
 * \vprivate Component Private
 */
class MemoryLayoutBase {
 public:
  /*!
   * \brief Type of a WritableSlotDescriptorPtr.
   */
  using WritableSlotDescriptorPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::WritableSlotDescriptor>;

  /*!
   * \brief Type of a ReadableSlotDescriptorPtr.
   */
  using ReadableSlotDescriptorPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ReadableSlotDescriptor>;

  /*!
   * \brief Type of a WritingQueueEndPtr.
   */
  using WritingQueueEndPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::WritingQueueEndInterface>;

  /*!
   * \brief Type of a ReadingQueueEndPtr.
   */
  using ReadingQueueEndPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ReadingQueueEndInterface>;

  /*!
   * \brief    Destruct the MemoryLayoutBase.
   * \vprivate Component Private
   */
  virtual ~MemoryLayoutBase() noexcept = default;

  /*!
   * \brief       Get the required memory size for the slot memory.
   * \details     The slot memory size is proportional to the slot content size, slot content alignment and the number
   *              of slots. Internally, each slot at least consists of a global visibility flag and the binary slot
   *              content. The actual layout is specific for each memory technology.
   * \return      Required slot memory size in bytes.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto GetSlotMemorySize() const noexcept -> MemorySize = 0;

  /*!
   * \brief       Get the required memory size for the whole queue memory.
   * \details     The queue memory size shall be used for the allocation of queue memory. It is proportional to the
   *              number of slots and the maximum number of receivers used for Zero-Copy communication.
   *              Internally, each queue at least consists of a head index, a tail index and the buffer to store all
   *              slot indices. The actual layout including alignment and padding is specific for each memory
   *              technology. For each receiver a writable head index, tail index and buffer is reserved in the queue
   *              memory to represent one queue. If the maximum number of receivers configured is greater than one, all
   *              queues are stored sequentially inside the queue memory.
   * \return      Required queue memory size in bytes.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto GetQueueMemorySize() const noexcept -> MemorySize = 0;

  /*!
   * \brief       Get the minimum memory size for one queue containing head index, tail index and buffer.
   * \details     The minimum memory size of a queue is only derived from the queue elements and does not include any
   *              padding between these elements. It can be used to check if the size of a given queue memory view to
   *              GetWritingQueueEnd()/GetReadingQueueEnd() is sufficient to store one queue. The queue size itself is
   *              proportional to the number of slots used for Zero-Copy communication.
   *              Internally, one queue at least consists of a head index, a tail index and the buffer to store all
   *              slot indices which are stored sequentially in memory. The actual layout is specific for each memory
   *              technology.
   * \return      Memory size of one queue in bytes.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto GetMinimumSizeOfOneQueue() const noexcept -> MemorySize = 0;

  /*!
   * \brief       Get the configuration parameters for a given queue index.
   * \details     Each queue inside the queue memory is defined with its head/tail index and its buffer storage. To
   *              locate those parts in the overall queue memory a queue configuration can be requested. The provided
   *              index refers to the queue for which the configuration shall be obtained because one queue memory can
   *              consist of multiple queues configured by the maximum number of receivers.
   * \param       queue_index The queue index which refers to the corresponding queue in the queue memory.
   *                          The queue index must be less than the maximum number of receivers configured. This
   *                          is NOT CHECKED as this is a unit-internal API.
   * \return      Queue configuration containing offset and size for each head index, tail index and buffer.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto GetQueueConfig(MaxNumberReceivers::Type queue_index) const noexcept -> QueueMemoryConfig = 0;

  /*!
   * \brief       Get a writable slot descriptor for a specific slot in a slot memory view.
   * \details     A writable slot descriptor refers to a slot content and its corresponding global visibility flag
   *              inside the given memory region.
   * \param       slot_index        Index to a slot for which a WritableSlotDescriptorPtr is returned.
   *                                Index value must be less than the configured number of slots. This is NOT CHECKED as
   *                                this is a unit-internal API.
   * \param       slot_memory_view  ReadWritableMemoryView to the slot memory from which the memory for the requested
   *                                slot_index is derived and a WritableSlotDescriptorPtr is created.
   *                                The view size must not be less than total slot memory size returned by
   *                                GetSlotMemorySize(). This is NOT CHECKED as this is a unit-internal API.
   *                                The user must ensure that the underlying memory the slot_memory_view refers to
   *                                outlives each WritableSlotDescriptorPtr created by this function. This cannot be
   *                                checked.
   *                                The slot memory view must fulfill the minimum required alignment for the chosen
   *                                memory technology. This is NOT CHECKED as this is a unit-internal API.
   * \return      A WritableSlotDescriptorPtr.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto GetWritableSlotDescriptor(NumSlots::Type slot_index,
                                         ::amsr::memory::ReadWritableMemoryView slot_memory_view) const noexcept
      -> WritableSlotDescriptorPtr = 0;

  /*!
   * \brief       Get a readable slot descriptor for a specific slot in a slot memory view.
   * \details     A readable slot descriptor refers to a slot content and its corresponding global visibility flag
   *              inside the given memory region.
   * \param       slot_index        Index to a slot for which a ReadableSlotDescriptorPtr is returned.
   *                                Index value must be less than the configured number of slots. This is NOT CHECKED as
   *                                this is a unit-internal API.
   * \param       slot_memory_view  ReadableMemoryView to the slot memory from which the memory for the requested
   *                                slot_index is derived and a ReadableSlotDescriptorPtr is created.
   *                                The view size must not be less than total slot memory size returned by
   *                                GetSlotMemorySize(). This is NOT CHECKED as this is a unit-internal API.
   *                                The user must ensure that the underlying memory the slot_memory_view refers to
   *                                outlives each ReadableSlotDescriptorPtr created by this function. This cannot be
   *                                checked.
   *                                The slot memory view must fulfill the minimum required alignment for the chosen
   *                                memory technology. This is NOT CHECKED as this is a unit-internal API.
   * \return      A ReadableSlotDescriptorPtr.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto GetReadableSlotDescriptor(NumSlots::Type slot_index,
                                         ::amsr::memory::ReadableMemoryView slot_memory_view) const noexcept
      -> ReadableSlotDescriptorPtr = 0;

  /*!
   * \brief       Get an initialized writing queue end by providing writable and readable memory.
   * \details     A WritingQueueEnd points to a unidirectional queue between the user of this queue end and the user of
   *              a reading queue end created from the same memory regions. The user which works on the WritingQueueEnd
   *              can push values to the queue but has no access to peek/pop values from this queue. This means that for
   *              a WritingQueueEnd the head index and the buffer are placed into the read-writable memory and the
   *              tail index is placed into the readable memory.
   *              Only the head index is initialized. Thus, the queue may only be used when the corresponding reading
   *              queue end is also initialized.
   * \param       queue_index           The index of the receiver for which the queue end
   *                                    is initialized. The index refers to the offset in the allocated queue memory.
   *                                    The index must be less than the maximum number of receivers configured at
   *                                    construction. This is NOT CHECKED as this is a unit-internal API.
   * \param       writable_view         ReadWritableMemoryView to the queue memory allocated by the user of this
   *                                    function.
   *                                    The view size must not be be less than total queue memory size returned by
   *                                    GetQueueMemorySize(). This is NOT CHECKED as this is a unit-internal API.
   *                                    The writable view must fulfill the minimum required alignment for the chosen
   *                                    memory technology. This is NOT CHECKED as this is a unit-internal API.
   * \param       readable_queue_config Configuration parameters including offset and size for head index, tail index
   *                                    and the queue buffer. The offsets and sizes point to the \p readable_view also
   *                                    passed to this function.
   *                                    The configuration together with the queue memory view must be checked previously
   *                                    with IsReadableQueueMemoryConfigValid(). This is NOT CHECKED as this is a
   *                                    unit-internal API.
   * \param       readable_view         ReadableMemoryView to the queue memory allocated by another instance and mapped
   *                                    by this instance.
   *                                    The view size must not be be less than total queue memory size returned by
   *                                    GetOneQueueMemorySize(). This is NOT CHECKED as this is a unit-internal API.
   *                                    The readable view must fulfill the minimum required alignment for the chosen
   *                                    memory technology. This is NOT CHECKED as this is a unit-internal API.
   * \return      A WritingQueueEndPtr based on the provided memory areas and the configuration.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto GetWritingQueueEnd(MaxNumberReceivers::Type queue_index,
                                  ::amsr::memory::ReadWritableMemoryView writable_view,
                                  QueueMemoryConfig readable_queue_config,
                                  ::amsr::memory::ReadableMemoryView readable_view) const noexcept
      -> WritingQueueEndPtr = 0;

  /*!
   * \brief       Get an initialized reading queue end by providing writable and readable memory.
   * \details     A ReadingQueueEnd points to a unidirectional queue between the user of this queue end and the user of
   *              a writing queue end created from the same memory regions. The user which works on the ReadingQueueEnd
   *              can peek and pop values from the queue and has no access to push a value to the queue. This means that
   *              for a ReadingQueueEnd the head index and the buffer are placed into the readable memory and the tail
   *              index is placed into the read-writable memory.
   *              The initialization is done for the tail index and the queue must only be used if the corresponding
   *              WritingQueueEndPtr is also initialized.
   * \param       queue_index           The index of the receiver for which the queue end is initialized. The index
   *                                    refers to the offset in the allocated queue memory.
   *                                    The index must be less than maximum number of receivers configured at
   *                                    construction. This is NOT CHECKED as this is a unit-internal API.
   * \param       writable_view         ReadWritableMemoryView to the queue memory allocated by the user of this
   *                                    function.
   *                                    The view size must not be be less than total queue memory size returned by
   *                                    GetQueueMemorySize(). This is NOT CHECKED as this is a unit-internal API.
   *                                    The writable view must fulfill the minimum required alignment for the chosen
   *                                    memory technology. This is NOT CHECKED as this is a unit-internal API.
   * \param       readable_queue_config Configuration parameters including offset and size for head index, tail index
   *                                    and the queue buffer. The offsets and sizes point to the \p readable_view passed
   *                                    to this function.
   *                                    The configuration together with the queue memory view must be checked previously
   *                                    with IsReadableQueueMemoryConfigValid(). This is NOT CHECKED as this is a
   *                                    unit-internal API.
   * \param       readable_view         ReadableMemoryView to the queue memory allocated by another instance and mapped
   *                                    by this instance.
   *                                    The view size must not be be less than total queue memory size returned by
   *                                    GetOneQueueMemorySize(). This is NOT CHECKED as this is a unit-internal API.
   *                                    The readable view must fulfill the minimum required alignment for the chosen
   *                                    memory technology. This is NOT CHECKED as this is a unit-internal API.
   * \return      A ReadingQueueEndPtr based on the provided memory areas and the configuration.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual auto GetReadingQueueEnd(MaxNumberReceivers::Type queue_index,
                                  ::amsr::memory::ReadWritableMemoryView writable_view,
                                  QueueMemoryConfig readable_queue_config,
                                  ::amsr::memory::ReadableMemoryView readable_view) const noexcept
      -> ReadingQueueEndPtr = 0;

  /*!
   * \brief         Check if the queue memory configuration is valid with respect to the readable queue memory view.
   * \details       To avoid problems when creating the queue ends the configuration is checked that no out-of-bounds
   *                access will happen, the configured memory areas are not overlapping and the alignment requirements
   *                for the queue parts are met.
   * \param         readable_queue_config Queue memory configuration to validate with respect to the queue memory view.
   * \param         readable_view         Readable queue memory view provided to validate with respect to the queue
   *                                      memory config.
   *                                      The view size must not be be less than total queue memory size returned by
   *                                      GetOneQueueMemorySize(). This is NOT CHECKED as this is a unit-internal API.
   * \return        True, if queue memory configuration is valid. False, if not.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto IsReadableQueueMemoryConfigValid(QueueMemoryConfig queue_memory_config,
                                                ::amsr::memory::ReadableMemoryView queue_memory_view) const noexcept
      -> bool = 0;

 protected:
  /*!
   * \brief Construct a MemoryLayoutBase.
   * \param[in]     number_slots           Number of slots configured for the slot memory.
   *                                       Must be greater than 0. This is NOT CHECKED as this is a unit-internal API.
   * \param[in]     slot_content_size      Slot content size in bytes for each slot.
   *                                       Must be greater than 0. This is NOT CHECKED as this is a unit-internal API.
   * \param[in]     slot_content_alignment Slot content alignment in bytes of each slot.
   *                                       Must be a power of two. This is NOT CHECKED as this is a unit-internal API.
   * \param[in]     max_number_receivers   Maximum number of receivers which are configured for this instance.
   *                                       The required queue memory space is proportional to this parameter.
   *                                       Must be greater than 0. This is NOT CHECKED as this is a unit-internal API.
   * \context       ANY
   * \pre           -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  MemoryLayoutBase(NumSlots number_slots, SlotContentSize slot_content_size,
                   SlotContentAlignment slot_content_alignment, MaxNumberReceivers max_number_receivers) noexcept
      : number_slots_{number_slots},
        slot_content_size_{slot_content_size},
        slot_content_alignment_{slot_content_alignment},
        max_number_receivers_{max_number_receivers} {};

  /*!
   * \brief Construct a MemoryLayoutBase by copying.
   * \vprivate Component Private
   */
  MemoryLayoutBase(MemoryLayoutBase const&) noexcept = default;

  /*!
   * \brief Construct a MemoryLayoutBase by moving.
   * \vprivate Component Private
   */
  MemoryLayoutBase(MemoryLayoutBase&&) noexcept = default;

  /*!
   * \brief Do not copy another MemoryLayoutBase into this instance.
   * \vprivate Component Private
   */
  auto operator=(MemoryLayoutBase const&) & noexcept -> MemoryLayoutBase& = delete;

  /*!
   * \brief Do not move another MemoryLayoutBase into this instance.
   * \vprivate Component Private
   */
  auto operator=(MemoryLayoutBase&&) & noexcept -> MemoryLayoutBase& = delete;

  /*!
   * \brief Number of slots.
   */
  NumSlots const number_slots_;

  /*!
   * \brief Slot content size in bytes of each slot.
   */
  SlotContentSize const slot_content_size_;

  /*!
   * \brief Slot content alignment in bytes of each slot.
   */
  SlotContentAlignment const slot_content_alignment_;

  /*!
   * \brief Maximum number of receivers.
   */
  MaxNumberReceivers const max_number_receivers_;
};

}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_MEMORY_LAYOUT_BASE_H_

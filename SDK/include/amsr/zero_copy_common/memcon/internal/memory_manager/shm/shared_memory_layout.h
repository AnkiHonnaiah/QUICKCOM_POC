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
 *        \brief Definition of SharedMemoryLayout class.
 *         \unit ZeroCopyCommon::MemCon::MemoryManager
 *   \complexity The high complexity of the class SharedMemoryLayout from the unit MemoryManager is caused by the high
 *               number of inter-module calls to the libosabstraction required for memory handling.
 *               The libosabstraction module for memory handling provides a API with many different configuration
 *               features and own-defined types. This is the reason why many inter-module calls are detected.
 *               In summary, the complexity is increased by using another module for memory handling and forwarding
 *               many calls which is needed for correct behavior. All the shared memory based inter-module calls are
 *               already split to the SharedMemoryLayout class which is the most reasonable encapsulation from design
 *               perspective. Therefore, the risk is considered low.
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_LAYOUT_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_LAYOUT_H_

#include <memory>
#include <utility>

#include "amsr/core/span.h"
#include "vac/container/static_vector.h"

#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_memory_view.h"
#include "amsr/memory/readable_memory_view.h"

#include "amsr/zero_copy_common/memcon/internal/memory_manager/memory_layout_base.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/shm/shared_memory_readable_slot_descriptor.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/shm/shared_memory_reading_queue_end.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/shm/shared_memory_writable_slot_descriptor.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/shm/shared_memory_writing_queue_end.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {
namespace shm {

/*!
 * \brief    SharedMemoryLayout class.
 * \vprivate Component Private
 */
class SharedMemoryLayout final : public MemoryLayoutBase {
 public:
  /*!
   * \brief         Construct a SharedMemoryLayout.
   * \details       During the construction of the SharedMemoryLayout the whole slot memory and queue memory is planned.
   *                This means that all further used offsets and sizes are precalculated.
   * \param[in]     number_slots           Number of slots configured for the slot memory.
   *                                       Must be greater than 0. This is NOT CHECKED as this is a unit-internal API.
   * \param[in]     slot_content_size      Slot content size in bytes for each slot.
   *                                       Must be greater than 0. This is NOT CHECKED as this is a unit-internal API.
   * \param[in]     slot_content_alignment Slot content alignment in bytes of each slot.
   *                                       Must be greater than 0 and a power of two. This is NOT CHECKED as this is a
   *                                       unit-internal API.
   * \param[in]     max_number_receivers   Maximum number of receivers which are configured for this instance.
   *                                       The required queue memory space is proportional to this parameter.
   *                                       Must be greater than 0. This is NOT CHECKED as this is a unit-internal API.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  explicit SharedMemoryLayout(NumSlots number_slots, SlotContentSize slot_content_size,
                              SlotContentAlignment slot_content_alignment,
                              MaxNumberReceivers max_number_receivers) noexcept;

  /*!
   * \brief Destruct the SharedMemoryLayout.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~SharedMemoryLayout() noexcept final = default;

  /*!
   * \brief Do not construct a SharedMemoryLayout by copying.
   * \vprivate Component Private
   */
  SharedMemoryLayout(SharedMemoryLayout const&) noexcept = delete;

  /*!
   * \brief Construct a SharedMemoryLayout by moving.
   * \vprivate Component Private
   */
  SharedMemoryLayout(SharedMemoryLayout&&) noexcept = default;

  /*!
   * \brief Do not copy another SharedMemoryLayout into this instance.
   * \vprivate Component Private
   */
  auto operator=(SharedMemoryLayout const&) & noexcept -> SharedMemoryLayout& = delete;

  /*!
   * \brief Do not move another SharedMemoryLayout into this instance.
   * \vprivate Component Private
   */
  auto operator=(SharedMemoryLayout&&) & noexcept -> SharedMemoryLayout& = delete;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::GetSlotMemorySize
   */
  auto GetSlotMemorySize() const noexcept -> MemorySize final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::GetQueueMemorySize
   */
  auto GetQueueMemorySize() const noexcept -> MemorySize final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::GetOneQueueMemorySize
   */
  auto GetMinimumSizeOfOneQueue() const noexcept -> MemorySize final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::GetQueueConfig
   */
  auto GetQueueConfig(MaxNumberReceivers::Type queue_index) const noexcept -> QueueMemoryConfig final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::GetWritableSlotDescriptor
   */
  auto GetWritableSlotDescriptor(NumSlots::Type slot_index,
                                 ::amsr::memory::ReadWritableMemoryView slot_memory_view) const noexcept
      -> WritableSlotDescriptorPtr final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::GetReadableSlotDescriptor
   */
  auto GetReadableSlotDescriptor(NumSlots::Type slot_index,
                                 ::amsr::memory::ReadableMemoryView slot_memory_view) const noexcept
      -> ReadableSlotDescriptorPtr final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::GetWritingQueueEnd
   */
  auto GetWritingQueueEnd(MaxNumberReceivers::Type queue_index, ::amsr::memory::ReadWritableMemoryView writable_view,
                          QueueMemoryConfig readable_queue_config,
                          ::amsr::memory::ReadableMemoryView readable_view) const noexcept -> WritingQueueEndPtr final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::GetReadingQueueEnd
   */
  auto GetReadingQueueEnd(MaxNumberReceivers::Type queue_index, ::amsr::memory::ReadWritableMemoryView writable_view,
                          QueueMemoryConfig readable_queue_config,
                          ::amsr::memory::ReadableMemoryView readable_view) const noexcept -> ReadingQueueEndPtr final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::memory_manager::MemoryLayoutBase::IsReadableQueueMemoryConfigValid
   */
  auto IsReadableQueueMemoryConfigValid(QueueMemoryConfig queue_memory_config,
                                        ::amsr::memory::ReadableMemoryView queue_memory_view) const noexcept
      -> bool final;

 private:
  /*!
   * \brief Type of a container holding the configuration structs for all slots.
   */
  using SlotConfigStorage = ::vac::container::StaticVector<OneSlotConfig>;

  /*!
   * \brief Type of a container holding the configuration structs for all queues.
   */
  using QueueConfigStorage = ::vac::container::StaticVector<QueueMemoryConfig>;

  /*!
   * \brief Type of the index stored in a queue.
   */
  using QueueIndexType =
      ::amsr::zero_copy_common::memcon::internal::memory_manager::shm::SharedMemoryWritingQueueEnd::IndexType;

  /*!
   * \brief Type of the elements stored in a queue.
   */
  using QueueBufferType =
      ::amsr::zero_copy_common::memcon::internal::memory_manager::shm::SharedMemoryWritingQueueEnd::BufferType;

  /*!
   * \brief Required queue index alignment derived from SharedMemoryWritingQueueEnd/-ReadingQueueEnd.
   */
  static constexpr ::amsr::memory::Alignment const kQueueIndexAlignment{8};
  // Assert that the minimum alignment requirement for a queue index is met.
  static_assert(std::alignment_of<QueueIndexType>::value <= kQueueIndexAlignment.value,
                "The minimum alignment requirement for the queue index type is not met by the SharedMemoryLayout.");

  /*!
   * \brief   Alignment for the global visibility flag to ensure atomic read/write operations.
   * \details There is no clear statement by C++14 about the alignment requirements of an atomic value to be used
   *          atomically. Since C++20, there is a possibility to use std::atomic_ref<T>::required_alignment, but for
   *          C++14 there is no API. Thus, we chose an alignment of 8 bytes for the std::atomic<bool> to ensure that it
   *          is aligned for 64-bit architectures.
   */
  static constexpr ::amsr::memory::Alignment const kGlobalVisibilityFlagAlignment{8};
  // Assert that the minimum alignment requirement for a global visibility flag is met.
  static_assert(
      std::alignment_of<std::atomic<bool>>::value <= kGlobalVisibilityFlagAlignment.value,
      "The minimum alignment requirement for the global visibility flag type is not met by the SharedMemoryLayout.");

  /*!
   * \brief         Configure the slot memory.
   * \details       Configuration uses MemoryLayoutPlanner to determine the size and offset of each slot, and the
   *                total memory size for number_slots_ slots.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void ConfigureSlotMemory() noexcept;

  /*!
   * \brief         Configure the queue memory.
   * \details       Configuration uses MemoryLayoutPlanner to determine the size and offset of each queue and the
   *                total memory size for max_number_receivers_ queues.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void ConfigureQueueMemory() noexcept;

  /*!
   * \brief         Check if the provided memory area config is inside the provided memory view.
   * \details       Offset and size of single memory area can also cause an integer overflow. This function is able to
   *                detect integer overflows and handles them appropriately which means that the memory area is not
   *                inside the memory view.
   * \param         memory_area_config Configuration of a memory area by using offset and size.
   * \param         memory_view        Readable memory view used for the check.
   * \return        True if memory area is inside, else false.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static auto IsMemoryAreaInsideMemoryView(MemoryAreaConfig memory_area_config,
                                           ::amsr::memory::ReadableMemoryView memory_view) noexcept -> bool;

  /*!
   * \brief         Check if the memory areas of the provided queue memory config have overlapping parts.
   * \param         queue_memory_config Configuration of a queue memory including head/tail index and the buffer.
   *                                    The offsets and sizes inside the configuration must not cause an integer
   *                                    overflow on addition. This is not checked.
   * \return        True if memory areas are overlapping, else false.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static auto AreMemoryAreasOverlapping(QueueMemoryConfig queue_memory_config) noexcept -> bool;

  /*!
   * \brief         Check if the memory areas are aligned correctly to establish a
   *                SharedMemoryReadingQueueEnd/-WritingQueueEnd.
   * \param         queue_memory_config Configuration of a queue memory including head/tail index and the buffer.
   *                                    Memory areas must not point outside the memory view. This is not checked.
   *                                    This can be checked by the user with the IsMemoryAreaInsideMemoryView() API.
   * \param         memory_view         Readable memory view used for the check.
   * \return        True if memory areas are aligned correctly, else false.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static auto IsQueueMemoryAlignmentValid(QueueMemoryConfig queue_memory_config,
                                          ::amsr::memory::ReadableMemoryView memory_view) noexcept -> bool;

  /*!
   * \brief Slot memory size needed for this Zero-Copy instance.
   */
  MemorySize slot_memory_size_{};

  /*!
   * \brief Queue memory size needed for this Zero-Copy instance.
   */
  MemorySize queue_memory_size_{};

  /*!
   * \brief A container for slot configuration parameters with number_slots_ elements.
   */
  SlotConfigStorage slot_config_vector_{};

  /*!
   * \brief A container for queue configuration parameters with max_number_receivers_ elements.
   */
  QueueConfigStorage queue_config_vector_{};
};

}  // namespace shm
}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_LAYOUT_H_

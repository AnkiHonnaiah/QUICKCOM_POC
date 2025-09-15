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
 *        \brief Definition of the ServerMemoryManager class.
 *        \unit ZeroCopyCommon::MemCon::MemoryManager
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SERVER_MEMORY_MANAGER_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SERVER_MEMORY_MANAGER_H_

#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "vac/container/static_vector.h"

#include "amsr/memory/memory_provider_interface.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_memory_view.h"
#include "amsr/memory/readable_memory_view.h"

#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/writable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/memory_layout_base.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/memory_technology.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/shm/shared_memory_layout.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/shm/shared_memory_provider.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {

/*!
 * \brief       Server-side memory manager implementation for usage by MemConServer.
 * \details     ServerMemoryManager is responsible to abstract different memory technologies and layouts.
 *              ServerMemoryManager does not own readable or writable memory but it is only responsible for allocating,
 *              mapping and partitioning memory and creating shared communication primitives (e.g. QueueEnds,
 *              SlotDescriptors).
 * \vprivate    Component private
 */
class ServerMemoryManager final {
 public:
  /*!
   * \brief Type used for ReadableMemoryView.
   */
  using ReadableMemoryView = ::amsr::memory::ReadableMemoryView;

  /*!
   * \brief Type used for ReadWritableMemoryView.
   */
  using ReadWritableMemoryView = ::amsr::memory::ReadWritableMemoryView;

  /*!
   * \brief Type used for WritingQueueEndPtr.
   */
  using WritingQueueEndPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::WritingQueueEndInterface>;

  /*!
   * \brief Type used for ReadingQueueEndPtr.
   */
  using ReadingQueueEndPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ReadingQueueEndInterface>;

  /*!
   * \brief Type used for WritableSlotDescriptorPtr.
   */
  using WritableSlotDescriptorPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::WritableSlotDescriptor>;

  /*!
   * \brief       Construct a new ServerMemoryManager.
   * \param       number_slots           Number of slots to be allocated and initialized by this instance.
   *                                     Must be greater than 0.
   * \param       slot_content_size      Slot content size in bytes for each slot.
   *                                     Must be greater than 0.
   * \param       slot_content_alignment Slot content alignment in bytes of each slot.
   *                                     Must be greater than 0 and a power of two.
   * \param       max_number_receivers   Maximum number of receivers which are supported by this instance.
   *                                     Must be greater than 0.
   * \param       memory_technology      Memory technology used for shared memory communication facilities.
   *                                     The default value is SharedMemory.
   *                                     Must be one of the pre-defined values in enum class MemoryTechnology.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  ServerMemoryManager(NumSlots number_slots, SlotContentSize slot_content_size,
                      SlotContentAlignment slot_content_alignment, MaxNumberReceivers max_number_receivers,
                      MemoryTechnology memory_technology = MemoryTechnology::kSharedMemory) noexcept;

  /*!
   * \brief Destruct the ServerMemoryManager.
   * \vprivate Component Private
   */
  ~ServerMemoryManager() noexcept = default;

  /*!
   * \brief Do not construct a ServerMemoryManager by copying.
   * \vprivate Component Private
   */
  ServerMemoryManager(ServerMemoryManager const&) = delete;

  /*!
   * \brief Construct a ServerMemoryManager by moving.
   * \vprivate Component Private
   */
  ServerMemoryManager(ServerMemoryManager&&) noexcept = default;

  /*!
   * \brief Do not copy another ServerMemoryManager into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerMemoryManager const&) -> ServerMemoryManager& = delete;

  /*!
   * \brief Move another ServerMemoryManager into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerMemoryManager&&) & noexcept -> ServerMemoryManager& = default;

  /*!
   * \brief       Allocate read-writable slot memory used by the server for instantiating slot descriptors.
   * \details     The allocated read-writable slot memory consists of the configured number of slots with the specified
   *              slot content size and alignment each. Additionally, each slot might have protocol related parts which
   *              are not specified by the user and depend on the configured memory technology. The size of the whole
   *              slot memory is proportionate to the number of slots and their slot content size.
   *              The allocated read-writable slot memory fulfills the minimum alignment requirements needed for the
   *              chosen memory technology.
   * \return      Read-writable slot memory.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kMemoryError If the allocation of slot memory
   *              failed because of insufficient system resources/privileges, an invalid size or missing support for
   *              this operating system.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate    Component Private
   *
   */
  auto AllocateSlotMemory() const noexcept -> ::amsr::core::Result<ReadWritableMemoryPtr>;

  /*!
   * \brief       Allocate read-writable queue memory used by the server for instantiating queues.
   * \details     The allocated read-writable queue memory consists of head indices and buffers of available queues and
   *              tail indices of free queues for the configured maximum number of receivers. All receivers are
   *              handled within the same allocated queue memory at different offsets.
   *              The size of the whole queue memory is proportionate to the number of slots and the configured maximum
   *              number of receivers.
   *              The allocated read-writable queue memory fulfills the minimum alignment requirements needed for the
   *              chosen memory technology.
   * \return      Read-writable queue memory.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kMemoryError If the allocation of queue memory
   *              failed because of insufficient system resources/privileges, an invalid size or missing support for
   *              this operating system.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate    Component Private
   */
  auto AllocateQueueMemory() const noexcept -> ::amsr::core::Result<ReadWritableMemoryPtr>;

  /*!
   * \brief       Get the writable slot descriptors for the read-writable slot memory.
   * \details     The number of writable slot descriptors is configured on construction with number_slots.
   * \param       slot_memory_view View to the allocated read-writable slot memory.
   *                               The view must point to the memory previously allocated by AllocateSlotMemory() to
   *                               meet all size and alignment requirements. This cannot be checked. Only the correct
   *                               size of the memory view is checked.
   *                               The underlying slot memory must be valid for the whole lifetime of the writable slot
   *                               descriptors. This cannot be checked.
   * \return      Writable slot descriptors.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto GetWritableSlotDescriptors(ReadWritableMemoryView slot_memory_view) const noexcept
      -> ::vac::container::StaticVector<WritableSlotDescriptorPtr>;

  /*!
   * \brief       Map a queue memory exchange handle provided by a receiver.
   * \details     The size of the memory exchange handle for the client queue memory is expected to have at least the
   *              size to fit one queue. The minimum size of one queue is memory technology specific.
   *              The mapped readable client queue memory fulfills the minimum alignment requirements needed for the
   *              chosen memory technology.
   * \param       queue_memory_exchange_handle Memory exchange handle provided by a receiver to be mapped.
   *                                           Must not be a nullptr.
   * \return      Readable queue memory.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kMemoryError If the mapping of client queue
   *              memory failed due to an invalid handle, insufficient resources or an invalid size of the handle.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   * requires true;
   * \endspec
   * \vprivate    Component Private
   */
  auto MapClientQueueMemory(MemoryExchangeHandlePtr queue_memory_exchange_handle_ptr) const noexcept
      -> ::amsr::core::Result<ReadableMemoryPtr>;

  /*!
   * \brief       Initialize writing and reading queue ends used for Zero-Copy communication.
   * \details     The server handles multiple queues in one shared memory area and can initialize a specific one by the
   *              client_index. If a client_index is reused for a new connected client, the queue memory is reset.
   *              The queue ends returned refer to two different queues. These queue ends must only be used when the
   *              corresponding queue ends for the readable and writable queue are also initialized.
   * \param       client_index               Index of the client for which the queue ends are initialized.
   *                                         Index must be less than max_number_receivers configured at construction.
   * \param       server_queue_memory_view   Read-writable view to the whole server allocated queue memory.
   *                                         The view must point to the whole memory previously allocated by
   *                                         AllocateQueueMemory() to meet all size and alignment requirements. This
   *                                         cannot be checked. Only the size of the memory view is checked.
   *                                         The underlying queue memory must be valid for the whole lifetime of the
   *                                         queue ends. This cannot be checked.
   * \param       client_queue_memory_config Queue configuration parameters for the client allocated queue memory.
   * \param       client_queue_memory_view   Readable view to the whole client allocated queue memory.
   *                                         The view must point to the whole memory previously mapped by
   *                                         MapClientQueueMemory() to meet all size and alignment requirements. This
   *                                         cannot be checked. Only the size of the memory view is checked.
   *                                         The underlying queue memory must be valid for the whole lifetime of the
   *                                         queue ends. This cannot be checked.
   * \return      Writing and reading queue ends.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kInvalidConfiguration If queue initialization
   *              fails due to invalid configuration data provided or an invalid client queue memory size. The
   *              configuration is invalid if the offsets or sizes point outside the client queue memory view, are
   *              overlapping or the internal alignment requirements are not met.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto InitializeQueueEnds(MaxNumberReceivers::Type client_index, ReadWritableMemoryView server_queue_memory_view,
                           QueueMemoryConfig client_queue_memory_config,
                           ReadableMemoryView client_queue_memory_view) const noexcept
      -> ::amsr::core::Result<std::pair<WritingQueueEndPtr, ReadingQueueEndPtr>>;

  /*!
   * \brief       Get queue configuration parameters for the given queue index.
   * \details     This method returns the queue configuration which specifies offset and size of the writable head and
   *              tail indices and the offset and size for the queue buffer at a given index. This queue configuration
   *              information needs to be exchanged with the client during handshake.
   * \param       queue_index Index for which the configuration parameters are to be returned.
   *                          Index must be less than max_number_receivers configured at construction.
   * \return      QueueMemoryConfig for given queue index.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetQueueConfig(MaxNumberReceivers::Type queue_index) const noexcept -> QueueMemoryConfig;

  /*!
   * \brief       Assert that the slot content alignment is a power of two.
   * \details     An unsigned integer is a power of two if only one bit equals to 1. If an unsigned integer is
   *              decremented by one all lower bits switch to 1 and the previously set bit goes to 0. A bitwise AND
   *              operation will return 0 when comparing the original and the decremented value, if the original value
   *              was a power of two.
   * \param       alignment Alignment which is checked.
   *                        The configured slot content alignment must be a power of two.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  static void AssertAlignmentIsPowerOfTwo(SlotContentAlignment alignment) noexcept;

 private:
  /*!
   * \brief       Assert that the number of slots is greater than 0.
   * \param       number_slots Number of slots.
   *                           Must be greater than 0.
   * \context     ANY
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  static void AssertNumberSlotsGreaterZero(NumSlots number_slots) noexcept;

  /*!
   * \brief       Assert that the slot content size is greater than 0.
   * \param       slot_content_size Slot content size in bytes.
   *                                Must be greater than 0.
   * \context     ANY
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  static void AssertSlotContentSizeGreaterZero(SlotContentSize slot_content_size) noexcept;

  /*!
   * \brief         Assert if a provided receiver index is valid.
   * \param[in]     client_index  Client index to check.
   *                              Must be less than max_number_receivers.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void AssertClientIndexValid(MaxNumberReceivers::Type client_index) const noexcept;

  /*!
   * \brief         Assert that the maximum number of receivers is not 0.
   * \param[in]     max_number_receivers  Maximum number of receivers to be checked.
   *                                      Must be greater than 0.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  static void AssertMaxNumberReceiversGreaterZero(MaxNumberReceivers max_number_receivers) noexcept;

  /*!
   * \brief         Assert that slot memory view size is valid.
   * \param[in]     slot_memory_view View to the whole slot memory.
   *                                 Must have the same size as the memory returned by AllocateSlotMemory().
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void AssertSlotMemoryViewSizeValid(::amsr::memory::ReadWritableMemoryView slot_memory_view) const noexcept;

  /*!
   * \brief         Assert that read-writable server queue memory view size is valid.
   * \param[in]     queue_memory_view View to the whole allocated server queue memory.
   *                                  Must have the same size as the memory returned by AllocateQueueMemory().
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void AssertServerQueueMemoryViewSizeValid(::amsr::memory::ReadWritableMemoryView queue_memory_view) const noexcept;

  /*!
   * \brief         Assert that readable client queue memory view size is valid.
   * \param[in]     queue_memory_view View to the whole mapped client queue memory.
   *                                  Must have the same size as the memory returned by MapClientQueueMemory().
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void AssertClientQueueMemoryViewSizeValid(::amsr::memory::ReadableMemoryView queue_memory_view) const noexcept;

  /*!
   * \brief The number of slots configured for this instance.
   */
  NumSlots number_slots_;

  /*!
   * \brief Size in bytes of a slot content.
   */
  SlotContentSize slot_content_size_;

  /*!
   * \brief Memory alignment for the content of a slot.
   */
  SlotContentAlignment slot_content_alignment_;

  /*!
   * \brief The maximum number of receivers that are considered for resource allocation.
   */
  MaxNumberReceivers max_number_receivers_;

  /*!
   * \brief Type of memory technology used for slot and queue memory.
   */
  MemoryTechnology memory_technology_;

  /*!
   * \brief An unique pointer to the memory provider.
   */
  std::unique_ptr<::amsr::memory::MemoryProviderInterface> memory_provider_{nullptr};

  /*!
   * \brief An unique pointer to the memory layout.
   */
  std::unique_ptr<MemoryLayoutBase> memory_layout_{nullptr};
};

}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SERVER_MEMORY_MANAGER_H_

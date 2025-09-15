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
 *        \brief Definition of ClientMemoryManager.
 *        \unit  ZeroCopyCommon::MemCon::MemoryManager
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_CLIENT_MEMORY_MANAGER_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_CLIENT_MEMORY_MANAGER_H_

#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "vac/container/static_vector.h"

#include "amsr/memory/memory_provider_interface.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_memory_view.h"
#include "amsr/memory/readable_memory_view.h"

#include "amsr/zero_copy_common/logic/internal/readable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/memory_layout_base.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/memory_technology.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/phys_contig_shm/phys_contig_shared_memory_provider.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/shm/shared_memory_layout.h"
#include "amsr/zero_copy_common/memcon/internal/memory_manager/shm/shared_memory_provider.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {

/*!
 * \brief    Client-side memory manager implementation for usage by MemConClient.
 * \details  ClientMemoryManager is responsible to abstract different memory technologies and layouts.
 *           ClientMemoryManager does not own readable or writable memory but it is only responsible for allocating,
 *           mapping memory, creating shared communication primitives (e.g. QueueEnds, SlotDescriptors).
 * \vprivate Component private
 */
class ClientMemoryManager final {
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
   * \brief Type used for WritingQueueEndInterface.
   */
  using WritingQueueEndPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::WritingQueueEndInterface>;

  /*!
   * \brief Type used for ReadingQueueEndInterface.
   */
  using ReadingQueueEndPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ReadingQueueEndInterface>;

  /*!
   * \brief Type used for ReadableSlotDescriptor.
   */
  using ReadableSlotDescriptorPtr = std::unique_ptr<::amsr::zero_copy_common::logic::internal::ReadableSlotDescriptor>;

  /*!
   * \brief       Construct a ClientMemoryManager.
   * \param       number_slots           Number of slots used by the Zero-Copy Client.
   *                                     Must be greater than 0.
   * \param       slot_content_size      Slot content size in bytes for each slot.
   *                                     Must be greater than 0.
   * \param       slot_content_alignment Slot content alignment in bytes of each slot.
   *                                     Must be greater than 0 and a power of two.
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
  ClientMemoryManager(NumSlots number_slots, SlotContentSize slot_content_size,
                      SlotContentAlignment slot_content_alignment,
                      MemoryTechnology memory_technology = MemoryTechnology::kSharedMemory) noexcept;

  /*!
   * \brief Destruct the ClientMemoryManager.
   * \vprivate Component Private
   */
  ~ClientMemoryManager() noexcept = default;

  /*!
   * \brief Do not construct a ClientMemoryManager by copying.
   * \vprivate Component Private
   */
  ClientMemoryManager(ClientMemoryManager const&) = delete;

  /*!
   * \brief Construct a ClientMemoryManager by moving.
   * \vprivate Component Private
   */
  ClientMemoryManager(ClientMemoryManager&&) noexcept = default;

  /*!
   * \brief Do not copy another ClientMemoryManager into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientMemoryManager const&) & noexcept -> ClientMemoryManager& = delete;

  /*!
   * \brief Move another ClientMemoryManager into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientMemoryManager&&) & noexcept -> ClientMemoryManager& = default;

  /*!
   * \brief       Map readable slot memory used by the client for instantiating readable slot descriptors.
   * \details     The size of the memory exchange handle for the slot memory is expected to have at least the size of
   *              the slot memory amount calculated locally for the configured parameters. This means that the
   *              calculated layout must fit into the memory referred by the memory exchange handle.
   *              The mapped readable slot memory fulfills the minimum alignment requirements needed for the chosen
   *              memory technology.
   * \param       readable_slot_memory_handle Memory exchange handle with read access which is used to create
   *                                          ReadableSlotDescriptors from.
   *                                          Must not be a nullptr.
   * \return      Readable slot memory.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kMemoryError If the mapping of slot
   *              memory failed due to an invalid handle, insufficient resources or an invalid size of the handle.
   * \context     ANY
   * \pre         Shall not be called on a moved-from object.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto MapSlotMemory(MemoryExchangeHandlePtr readable_slot_memory_handle_ptr) const noexcept
      -> ::amsr::core::Result<ReadableMemoryPtr>;

  /*!
   * \brief       Allocate read-writable queue memory used by the client for instantiating a reading and a writing queue
                  end.
   * \details     The allocated read-writable queue memory consists of a head index and a buffer of the free queue and
   *              a tail index of the available queue.
   *              The allocated read-writable queue memory fulfills the minimum alignment requirements needed for the
   *              chosen memory technology.
   * \return      Read-writable queue memory.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kMemoryError If the allocation of
   *              queue memory failed because of insufficient system resources/privileges, an invalid size or missing
   *              support for this operating system.
   * \context     ANY
   * \pre         Shall not be called on a moved-from object.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto AllocateQueueMemory() const noexcept -> ::amsr::core::Result<ReadWritableMemoryPtr>;

  /*!
   * \brief       Get all readable slot descriptors for the readable slot memory view.
   * \details     The number of readable slot descriptors is configured on construction with number_slots.
   * \param       slot_memory_view View to the server allocated readable slot memory.
   *                               The view must point to the memory previously mapped by MapSlotMemory() to meet all
   *                               size and alignment requirements. This cannot be checked. Only the correct size of the
   *                               memory view is checked.
   *                               The underlying slot memory must be valid for the whole lifetime of the readable slot
   *                               descriptors. This cannot be checked.
   * \return      Readable slot descriptors.
   * \context     ANY
   * \pre         Shall not be called on a moved-from object.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto GetReadableSlotDescriptors(ReadableMemoryView slot_memory_view) const noexcept
      -> ::vac::container::StaticVector<ReadableSlotDescriptorPtr>;

  /*!
   * \brief       Map a queue memory exchange handle provided by a server.
   * \details     The size of the memory exchange handle for the server queue memory is expected to have at least the
   *              size to fit one queue. The minimum size of one queue is memory technology specific.
   *              The mapped readable server queue memory fulfills the minimum alignment requirements needed for the
   *              chosen memory technology.
   * \param       queue_memory_exchange_handle Memory exchange handle provided by a server to be mapped.
   *                                           Must not be a nullptr.
   * \return      Readable memory which owns the mapped memory area.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kMemoryError If the mapping of server
   *              queue memory fails due to an invalid handle, insufficient resources or an invalid size of the handle.
   * \context     ANY
   * \pre         Shall not be called on a moved-from object.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto MapServerQueueMemory(MemoryExchangeHandlePtr queue_memory_exchange_handle_ptr) const noexcept
      -> ::amsr::core::Result<ReadableMemoryPtr>;

  /*!
   * \brief       Initialize writing and reading queue ends used for Zero-Copy communication.
   * \details     If this function is called twice for the same parameter set, the queue memory will be reset.
   *              The queue ends returned refer to two different queues. These queue ends must only be used when the
   *              corresponding queue ends for the readable and writable queue are also initialized.
   * \param[in]   client_queue_memory_view   Read-writable view to the whole client allocated queue memory.
   *                                         The view must point to the whole memory previously allocated by
   *                                         AllocateQueueMemory() to meet all size and alignment requirements. This
   *                                         cannot be checked. Only the size of the memory view is checked.
   *                                         The underlying queue memory must be valid for the whole lifetime of the
   *                                         queue ends. This cannot be checked.
   * \param       server_queue_memory_config Queue configuration parameters for the server allocated queue memory.
   * \param       server_queue_memory_view   Readable view to the whole server allocated queue memory.
   *                                         The view must point to the whole memory previously mapped by
   *                                         MapServerQueueMemory() to meet all size and alignment requirements. This
   *                                         cannot be checked. Only the size of the memory view is checked.
   *                                         The underlying queue memory must be valid for the whole lifetime of the
   *                                         queue ends. This cannot be checked.
   * \return      Writing and reading queue ends.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kInvalidConfiguration If queue initialization
   *              fails due to invalid configuration data provided or an invalid server queue memory size.The
   *              configuration is invalid if the offsets or sizes point outside the server queue memory view, are
   *              overlapping or the internal alignment requirements are not met.
   * \context     ANY
   * \pre         Shall not be called on a moved-from object.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto InitializeQueueEnds(ReadWritableMemoryView client_queue_memory_view,
                           QueueMemoryConfig server_queue_memory_config,
                           ReadableMemoryView server_queue_memory_view) const noexcept
      -> ::amsr::core::Result<std::pair<WritingQueueEndPtr, ReadingQueueEndPtr>>;

  /*!
   * \brief       Get queue configuration parameters for the client queue memory.
   * \details     This method returns the queue configuration which specifies offset and size of the writable head and
   *              tail indices and the offset and size for the client allocated queue buffer. This queue configuration
   *              information needs to be exchanged with the server during handshake.
   * \return      QueueMemoryConfig for client queue memory.
   * \context     ANY
   * \pre         Shall not be called on a moved-from object.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto GetQueueConfig() const noexcept -> QueueMemoryConfig;

  /*!
   * \brief       Check whether the given slot content alignment is a power of two.
   * \details     An unsigned integer is a power of two if only one bit equals to 1. If an unsigned integer is
   *              decremented by one all lower bits switch to 1 and the previously set bit goes to 0. A bitwise AND
   *              operation will return 0 when comparing the original and the decremented value, if the original value
   *              was a power of two.
   *              Note: This function does not accept 0 as a power of two.
   * \param       alignment Alignment which is checked.
   * \return      True, if the given \p alignment is a power of two. False, otherwise.
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
  static auto IsSlotContentAlignmentValid(SlotContentAlignment alignment) noexcept -> bool;

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
   * \brief         Assert that the size of the provided slot memory view is valid.
   * \details       The slot memory view must point to the whole memory previously mapped with MapSlotMemory().
   * \param[in]     slot_memory_view View to the whole slot memory.
   *                                 Must point to the whole memory previously mapped with MapSlotMemory(). This cannot
   *                                 be checked, but the correct size is checked.
   * \context       ANY
   * \pre           Shall not be called on a moved-from object.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void AssertSlotMemoryViewSizeValid(::amsr::memory::ReadableMemoryView slot_memory_view) const noexcept;

  /*!
   * \brief         Assert that read-writable client queue memory view size is valid.
   * \param         queue_memory_view View to the whole allocated client queue memory.
   *                                  Must have the same size as the memory returned by AllocateQueueMemory(). This
   *                                  cannot be checked.
   * \context       ANY
   * \pre           Shall not be called on a moved-from object.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void AssertClientQueueMemoryViewSizeValid(::amsr::memory::ReadWritableMemoryView queue_memory_view) const noexcept;

  /*!
   * \brief         Assert that readable server queue memory view size is valid.
   * \param         queue_memory_view View to the whole mapped server queue memory.
   *                                  Must have the same size as the memory returned by MapServerQueueMemory(). This
   *                                  cannot be checked.
   * \context       ANY
   * \pre           Shall not be called on a moved-from object.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  void AssertServerQueueMemoryViewSizeValid(::amsr::memory::ReadableMemoryView queue_memory_view) const noexcept;

  /*!
   * \brief Number of slots used for communication.
   */
  NumSlots number_slots_;

  /*!
   * \brief Size in bytes of one slot used for message exchange.
   */
  SlotContentSize slot_content_size_;

  /*!
   * \brief Alignment in bytes of a message stored in a slot.
   */
  SlotContentAlignment slot_content_alignment_;

  /*!
   * \brief Memory technology used for communication.
   */
  MemoryTechnology memory_technology_;

  /*!
   * \brief Memory layout for used memory_technology.
   */
  std::unique_ptr<MemoryLayoutBase> memory_layout_{nullptr};

  /*!
   * \brief Memory provider used for allocation of new resources.
   */
  std::unique_ptr<::amsr::memory::MemoryProviderInterface> memory_provider_{nullptr};
};

}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_CLIENT_MEMORY_MANAGER_H_

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
 *         \brief Definition and Implementation of the SharedMemoryWritableSlotDescriptor class.
 *         \unit ZeroCopyCommon::MemCon::MemoryManager
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_WRITABLE_SLOT_DESCRIPTOR_H
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_WRITABLE_SLOT_DESCRIPTOR_H

#include <atomic>
#include <cstddef>
#include <cstdint>

#include "amsr/core/abort.h"
#include "amsr/core/span.h"

#include "amsr/zero_copy_common/logic/internal/writable_slot_descriptor.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {
namespace shm {

/*!
 * \brief    SharedMemoryWritableSlotDescriptor class.
 * \details  A SharedMemoryWritableSlotDescriptor offers read and write access to one message slot in shared memory.
 *
 * \trace    DSGN-ZeroCopyCommon-MessageSlotAbstraction,
 *           DSGN-ZeroCopyCommon-MessageSlotAccess
 * \vprivate Component Private
 */
// VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
class SharedMemoryWritableSlotDescriptor final
    : public ::amsr::zero_copy_common::logic::internal::WritableSlotDescriptor {
 public:
  // Assert that the atomic that is used for global visibility flag is always lock-free, otherwise it cannot be used to
  // synchronize access to the flag in the underlying memory. It is assumed that lock-free atomics are also
  // address- free on all architectures supported by AMSR. This is also a precondition for the lock-free flag
  // access to work in underlying memory but this cannot be checked by the compiler.
  static_assert(ATOMIC_BOOL_LOCK_FREE == 2,
                "std::atomic<std::bool> is not always lock-free and can therefore not be used in underlying memory.");

  /*!
   * \brief       Construct a SharedMemoryWritableSlotDescriptor.
   * \param[in]   global_visibility_flag  Reference to the global visibility flag.
   *                                      It must outlive the current instance of SharedMemoryWritableSlotDescriptor.
   *                                      This cannot be checked.
   * \param[in]   message_slot            View to the read-writable memory for the message slot.
   *                                      The pointed-to memory must outlive the current instance of
   *                                      SharedMemoryWritableSlotDescriptor.
   *                                      This cannot be checked.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  explicit SharedMemoryWritableSlotDescriptor(std::atomic<bool>& global_visibility_flag,
                                              ::amsr::core::Span<std::uint8_t> message_slot) noexcept
      : ::amsr::zero_copy_common::logic::internal::WritableSlotDescriptor{},
        global_visible_flag_(global_visibility_flag),
        message_slot_(message_slot){};

  /*!
   * \brief       Reset the memory for the referenced slot into a sane default state.
   * \details     This should reset (if required for the implementation) debug data into a sane
   *              default state. It should also clear the global visibility flag of this slot.
   *              This will be called before the slot is used by a server instance.
   *              Currently, there is no debug data that needs to be reset.
   * \context     ANY
   * \pre -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  void ResetSlotMemory() noexcept final { ClearVisibilityFlag(); };

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::WritableSlotDescriptor::GetWritableData()
   */
  auto GetWritableData() noexcept -> ::amsr::core::Span<std::uint8_t> final { return message_slot_; }

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::WritableSlotDescriptor::SetVisibilityFlag()
   */
  void SetVisibilityFlag() noexcept final { global_visible_flag_.store(true); }

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::WritableSlotDescriptor::ClearVisibilityFlag()
   */
  void ClearVisibilityFlag() noexcept final { global_visible_flag_.store(false); }

  /*!
   * \brief       Update debug data for the "Send Message" debug point.
   * \details     This method is invoked directly before the server sends the slot to any connected clients.
   *              This write operation to shared memory must be non-blocking.
   *              Currently, there is no debug data that needs to be updated before a slot is sent.
   * \context     ANY
   * \pre -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \vprivate    Component Private
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInDebugPointFunction
  void DebugPointSendSlot() noexcept final{};

  /*!
   * \brief       Update debug data for the "get slot" debug point.
   * \details     This method is invoked right before the server returns a slot to the user.
   *              This write operation to shared memory must be non-blocking.
   *              Currently, there is no debug data that needs to be updated before a slot is acquired.
   * \context     ANY
   * \pre -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \vprivate    Component Private
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInDebugPointFunction
  void DebugPointGetSlot() noexcept final{};

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ReadableSlotDescriptor::GetReadableData()
   */
  auto GetReadableData() const noexcept -> ::amsr::core::Span<std::uint8_t const> final { return message_slot_; }

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ReadableSlotDescriptor::IsSlotVisible()
   */
  auto IsSlotVisible() const noexcept -> bool final { return global_visible_flag_.load(); }

 private:
  /*!
   * \brief Reference to Global visibility flag.
   */
  std::atomic<bool>& global_visible_flag_;

  /*!
   * \brief View to Message slot.
   */
  ::amsr::core::Span<std::uint8_t> const message_slot_;
};

}  // namespace shm
}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_SHM_SHARED_MEMORY_WRITABLE_SLOT_DESCRIPTOR_H

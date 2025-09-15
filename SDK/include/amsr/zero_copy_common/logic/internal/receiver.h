/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file
 *        \brief  Definition of class Receiver.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::ReceiverManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_H_

#include <memory>

#include "amsr/core/abort.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"

#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/receiver_connection_state.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A Receiver managed by the ReceiverManager class.
 * \details A Receiver contains a free queue and an available queue as well as a ReceiverHandle that represents this
 *          Receiver.
 *          The Receiver bundles the functionality of a pair of Receiver-specific queues.
 * \vprivate Component Private
 */
// VCA_ZEROCOPYCOMMON_LNG_03_RECEIVER_UNIQUE_PTR_DESTRUCTOR_INVALID_OBJECT
// VCA_ZEROCOPYCOMMON_LNG_03_RECEIVER_UNIQUE_PTR_DESTRUCTOR_INVALID_OBJECT
// VCA_ZEROCOPYCOMMON_SPC_15_RECEIVER_UNIQUE_PTR_DESTRUCTOR_FUNCTION_CONTRACT
// VCA_ZEROCOPYCOMMON_SPC_15_RECEIVER_UNIQUE_PTR_DESTRUCTOR_FUNCTION_CONTRACT
// VCA_ZEROCOPYCOMMON_LNG_03_RECEIVER_RECEIVER_DESTRUCTOR_INVALID_OBJECT
class Receiver final {
 public:
  /*!
   * \brief Construct a Receiver.
   * \param handle ReceiverHandle of the Receiver.
   * \param free_queue_ptr Free queue of the Receiver.
   * \param available_queue_ptr Available queue of the Receiver.
   * \context ANY
   * \pre The \p free_queue_ptr and \p available_queue_ptr must not contain nullptr.
   *      The \p free_queue_ptr and \p available_queue_ptr must point to valid objects. This cannot be checked.
   *      The \p free_queue_ptr and \p available_queue_ptr must provide valid implementations of the respective base
   *      class. This cannot be checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Receiver(ReceiverHandle const& handle, std::unique_ptr<ReadingQueueEndInterface> free_queue_ptr,
           std::unique_ptr<WritingQueueEndInterface> available_queue_ptr) noexcept
      : handle_{handle},
        // VCA_ZEROCOPYCOMMON_LNG_01_RECEIVER_VALID_DESCRIPTOR_UNIQUE_POINTERS
        free_queue_ptr_{std::move(free_queue_ptr)},
        // VCA_ZEROCOPYCOMMON_LNG_01_RECEIVER_VALID_DESCRIPTOR_UNIQUE_POINTERS
        available_queue_ptr_{std::move(available_queue_ptr)} {
    if ((!free_queue_ptr_) || (!available_queue_ptr_)) {
      amsr::core::Abort("FreeQueue and AvailableQueue must not contain a nullptr.");
    }
  }

  /*!
   * \brief Get the connection state of this receiver.
   * \vprivate Component Private
   */
  auto GetState() const noexcept -> ReceiverConnectionState { return receiver_connection_state_; }

  /*!
   * \brief Set the connection state of this receiver to ReceiverConnectionState::kCorrupted.
   * \vprivate Component Private
   */
  void SetStateCorrupted() noexcept { receiver_connection_state_ = ReceiverConnectionState::kCorrupted; }

  /*!
   * \brief Get ReceiverHandle of the Receiver.
   * \vprivate Component Private
   */
  auto GetHandle() const noexcept -> ReceiverHandle const& { return handle_; }

  /*!
   * \brief Peek in the free queue to get a SlotIndex, if available.
   * \return Optional containing a SlotIndex if available, empty Optional otherwise.
   * \error amsr::zero_copy_common::common::ZeroCopyCommonErrc::kQueueError If there is an error when using the queue.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto PeekFreeQueue() const noexcept -> amsr::core::Result<amsr::core::Optional<SlotHandle::SlotIndex>> {
    // VCA_ZEROCOPYCOMMON_SLC_10_RECEIVER_UNIQUE_PTR_ACCESS
    // VCA_ZEROCOPYCOMMON_SLC_22_RECEIVER_QUEUE_INTERFACE_CALLING_NULL_POINTER
    return free_queue_ptr_->Peek();
  }

  /*!
   * \brief Pop a SlotIndex from the free queue, if available.
   * \return Optional containing a SlotIndex if available, empty Optional otherwise.
   * \error amsr::zero_copy_common::common::ZeroCopyCommonErrc::kQueueError If there is an error when using the queue.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto PopFreeQueue() const noexcept -> amsr::core::Result<amsr::core::Optional<SlotHandle::SlotIndex>> {
    // VCA_ZEROCOPYCOMMON_SLC_10_RECEIVER_UNIQUE_PTR_ACCESS
    // VCA_ZEROCOPYCOMMON_SLC_22_RECEIVER_QUEUE_INTERFACE_CALLING_NULL_POINTER
    return free_queue_ptr_->Pop();
  }

  /*!
   * \brief Push a SlotIndex to the available queue, if the queue is not full.
   * \param index SlotIndex to push.
   * \return True if SlotIndex was pushed, false if the queue was full.
   * \error amsr::zero_copy_common::common::ZeroCopyCommonErrc::kQueueError If there is an error when using the queue.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto PushAvailableQueue(SlotHandle::SlotIndex index) const noexcept -> amsr::core::Result<bool> {
    // VCA_ZEROCOPYCOMMON_SLC_10_RECEIVER_UNIQUE_PTR_ACCESS
    // VCA_ZEROCOPYCOMMON_SLC_22_RECEIVER_QUEUE_INTERFACE_CALLING_NULL_POINTER
    return available_queue_ptr_->Push(index);
  }

 private:
  /*!
   * \brief The ReceiverConnectionState of this receiver.
   */
  ReceiverConnectionState receiver_connection_state_{ReceiverConnectionState::kOk};

  /*!
   * \brief The ReceiverHandle representing this Receiver.
   */
  ReceiverHandle handle_;

  /*!
   * \brief The free queue of this Receiver.
   */
  std::unique_ptr<ReadingQueueEndInterface> free_queue_ptr_;

  /*!
   * \brief The available queue of this Receiver.
   */
  std::unique_ptr<WritingQueueEndInterface> available_queue_ptr_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_H_

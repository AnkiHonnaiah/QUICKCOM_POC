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
/*!        \file
 *        \brief Definition of the ReceiverClass class.
 *        \unit ZeroCopyCommon::ZeroCopyLogic::BorrowedManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_CLASS_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_CLASS_H_

#include "amsr/core/optional.h"

#include "amsr/zero_copy_common/logic/internal/borrowed_bitmap.h"
#include "amsr/zero_copy_common/logic/internal/class_handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A ReceiverClass describes a set of receivers sharing an upper limit on the number of slots they can borrow.
 * \details A ReceiverClass has an upper limit on the number of slots that can be held (borrowed) by it.
 *          The number of currently held slots for one ReceiverClass is determined by building the union of the
 *          sets of slots held by each receiver registered to the ReceiverClass.
 *          The ReceiverClass can answer the question if borrowing a new slot for a receiver would violate its
 *          ReceiverClass limit.
 *
 *          The ReceiverClass assumes that the ClassManager has transformed ReceiverHandles to their indices and that
 *          the ClassManager forwards requests to the correct, registered ReceiverClass (for a given ReceiverHandle).
 *          The ReceiverClass has an upper limit on the index values that can be managed which are held by the
 *          ClassManager, i.e. receiver_index < BorrowedBitmap::kReceiverLimit must hold.
 *
 *          The ReceiverClass takes place in the caching mechanism of the ClassManager.
 *          Therefore, it is required that the ClassManager forwards all changes to borrowed status to the respective
 *          ReceiverClass instance in order to keep the cache coherent.
 *          The correct forwarding can not be checked and must be fulfilled within this unit (by the ClassManager).
 * \vprivate Component Private
 */
class ReceiverClass final {
 public:
  /*!
   * \brief Type used to describe the maximum number of slots that can be held by a ReceiverClass.
   */
  using ClassLimit = SlotHandle::SlotIndex;

  /*!
   * \brief Create a new ReceiverClass with a limit on the maximum slots held by the class.
   * \param[in] class_limit The maximum number of slots that may be held by this ReceiverClass.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  explicit ReceiverClass(ClassLimit class_limit) noexcept : class_limit_{class_limit} {}

  /*!
   * \brief Register a receiver to this ReceiverClass.
   * \details If the receiver is already registered, nothing is changed.
   * \param[in] receiver_index The index identifying the receiver to register to the class.
   *                           It must hold that receiver_index < BorrowedBitmap::kReceiverLimit.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void RegisterReceiverToClass(ReceiverHandle::ReceiverIndex receiver_index) noexcept {
    BorrowedBitmap::AssertReceiverIndexWithinStaticLimits(receiver_index);
    registered_receivers_.SetReceiver(receiver_index);
  }

  /*!
   * \brief Remove a receiver from this ReceiverClass.
   * \details If a non-registered index is removed, nothing is changed.
   * \param[in] receiver_index The index identifying the receiver to remove from the class.
   *                           It must hold that receiver_index < BorrowedBitmap::kReceiverLimit.
   * \context ANY
   * \pre Must call ReleaseSlotForReceiver for any slots still held by the given receiver to keep the cache coherent.
   *      This precondition can not be checked and must be ensured by the caller.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RemoveReceiverFromClass(ReceiverHandle::ReceiverIndex receiver_index) noexcept {
    BorrowedBitmap::AssertReceiverIndexWithinStaticLimits(receiver_index);
    registered_receivers_.ClearReceiver(receiver_index);
  }

  /*!
   * \brief Try to acquire a slot for the given receiver respecting the ReceiverClass limit.
   *        Does nothing if the slot is already borrowed by the receiver.
   * \details Checks if the class limit would be exceeded by the given receiver acquiring the slot.
   *          If the limit allows acquiring the slot, the acquisition is performed.
   *          Internally, the cache of currently held slots of this ReceiverClass is updated if required.
   * \param[in] receiver_index The index identifying the receiver to try to acquire a slot for.
   *                           It must hold that receiver_index < BorrowedBitmap::kReceiverLimit.
   * \param[in,out] slot_bitmap The BorrowedBitmap of the slot the given receiver should acquire.
   *                             Will be modified to reflect the new borrowed status if the class limit allows.
   * \return True, if the update is allowed according to the class limit and the slot is now borrowed by the receiver.
   * \return False, if the update is not allowed because the class limit is reached.
   * \context ANY
   * \pre The receiver_index must be registered to this ReceiverClass.
   * \pre Must be called whenever a slot is acquired for a receiver of this ReceiverClass to keep the cache coherent.
   *      Closely related, all releases of a slot need to be performed using ReleaseSlotForReceiver().
   *      This precondition can not be checked and must be ensured by the caller.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto TryAcquiringSlotForReceiver(ReceiverHandle::ReceiverIndex receiver_index, BorrowedBitmap& slot_bitmap) noexcept
      -> bool;

  /*!
   * \brief Release a slot for the given receiver respecting the ReceiverClass limit.
   *        Does nothing if the slot is not borrowed by the given receiver.
   * \details Internally, the cache of currently held slots of this ReceiverClass is updated if required.
   * \param[in] receiver_index The index identifying the receiver to release the slot.
   *                           It must hold that receiver_index < BorrowedBitmap::kReceiverLimit.
   * \param[in,out] slot_bitmap The BorrowedBitmap of the slot the given receiver should release.
   *                             Will be modified to reflect the new borrowed status.
   * \context ANY
   * \pre The receiver_index must be registered to this ReceiverClass.
   * \pre Must be called whenever a slot is released for a receiver of this ReceiverClass to keep the cache coherent.
   *      Closely related, all acquisitions of a slot need to be performed using TryAcquiringSlotForReceiver().
   *      This precondition can not be checked and must be ensured by the caller.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ReleaseSlotForReceiver(ReceiverHandle::ReceiverIndex receiver_index, BorrowedBitmap& slot_bitmap) noexcept;

  /*!
   * \brief Check if any of the registered receivers of this class holds the given slot.
   * \param[in] slot_bitmap The BorrowedBitmap describing the borrowed status of the slot to be checked.
   * \return True, if any receiver registered in this ReceiverClass borrows the slot, False otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  auto IsSlotBorrowedByAnyClassMember(BorrowedBitmap const& slot_bitmap) const noexcept -> bool {
    BorrowedBitmap const slots_borrowed_by_class_members{slot_bitmap.BitwiseAnd(registered_receivers_)};
    return slots_borrowed_by_class_members.IsAnySet();
  }

 private:
  /*!
   * \brief The maximum number of slots that can be held by receivers in this class.
   */
  ClassLimit class_limit_;

  /*!
   * \brief The current number of slots held by receivers in this class.
   * \details A new instance always starts with 0 slots held.
   *          Calls to TryAcquiringSlotForReceiver and ReleaseSlotForReceiver modify this number.
   */
  ClassLimit number_of_slots_held_{0};

  /*!
   * \brief Bitmap storing if a receiver is part of this ReceiverClass.
   * \details For each registered receiver_index, the corresponding bit is set in this bitmap.
   *          This work closely coupled with the borrowed_array, holding one bitmap entry per slot.
   *          A new instance always starts with a default constructed, empty Bitmap.
   */
  BorrowedBitmap registered_receivers_{};
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_CLASS_H_

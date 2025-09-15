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
 *        \brief Definition of the BorrowedManager class.
 *        \unit ZeroCopyCommon::ZeroCopyLogic::BorrowedManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_BORROWED_MANAGER_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_BORROWED_MANAGER_H_

#include "amsr/core/optional.h"

#include "vac/container/static_vector.h"
#include "vac/testing/test_adapter.h"

#include "amsr/zero_copy_common/logic/internal/borrowed_bitmap.h"
#include "amsr/zero_copy_common/logic/internal/class_handle.h"
#include "amsr/zero_copy_common/logic/internal/class_manager.h"
#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver_class.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver_limit.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Manager keeping track of which slots are currently borrowed to which receiver or to the sender.
 * \details Message slots are temporarily borrowed to receivers for reading during Zero-Copy communication.
 *          Additionally, the sender might temporarily borrow a slot for writing data before sending.
 *
 *          The BorrowedManager keeps track of which slots are currently borrowed to which receiver or to the sender.
 *          The BorrowedManager also offers helpers related to limiting how many slots certain receivers
 *          can borrow at once (rate limiting).
 *          Most notably, receivers can be assigned a ReceiverClass and limits can be placed on the maximum number of
 *          slots that can be borrowed by the receivers contained in a ReceiverClass at once.
 *          The BorrowedManager itself does not check if the given number of slots and class limits make semantic sense,
 *          it solely answers the above questions given the configured resources.
 *
 *          A BorrowedManager uses the Handle concept to be used together with other managers and should be configured
 *          with the correct group and limits to interop with neighboring managers.
 *          ClassHandles are assumed valid if they were created by this manager instance.
 *          ReceiverHandles are assumed valid if the Handle::Group is correct and their index < max_receiver_number.
 *          ReceiverHandles are assumed registered if RegisterReceiver was called and RemoveReceiver was not called.
 *          SlotHandles are assumed valid if the Handle::Group is correct and their index < number_of_slots.
 * \vprivate Component Private
 */
// VCA_ZEROCOPYCOMMON_SPC_15_BORROWED_MANAGER_VECTOR_DESTRUCTOR_FUNCTION_CONTRACT
class BorrowedManager final {
 public:
  /*!
   * \brief The limit on the maximum number of receivers that can be managed by this BorrowedManager.
   * \details Such a limit originates from implementation limitations of the bitmap representation used internally.
   */
  static constexpr ReceiverHandle::ReceiverIndex kReceiverLimit{BorrowedBitmap::kReceiverLimit};

  /*!
   * \brief Create a BorrowedManager for a fixed number of slots, a maximum number of receivers and a maximum number of
   *        classes.
   * \details A BorrowedManager instance is most likely created to cooperate with other managers.
   *          The user of the BorrowedManager should ensure that the passed configuration parameters
   *          are suited for the set of managers working together, otherwise method preconditions might be violated.
   * \param[in] group The group identifier all cooperating managers belong to.
   * \param[in] number_of_slots The number of slots this BorrowedManager is tracking and borrowing.
   *                            It must hold that number_of_slots <= std::numeric_limits<std::ptrdiff_t>::max().
   * \param[in] max_receiver_number The maximum number of receivers that can be registered in this BorrowedManager.
   *                                It must hold that max_receiver_number <= BorrowedBitmap::kReceiverLimit.
   * \param[in] max_class_number The maximum number of classes that can be created for this BorrowedManager.
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
  BorrowedManager(Handle::Group const& group, SlotHandle::SlotIndex number_of_slots,
                  ReceiverHandle::ReceiverIndex max_receiver_number,
                  ClassHandle::ReceiverClassIndex max_class_number) noexcept;

  /*!
   * \brief Create a new ReceiverClass with an upper limit on the number of slots that can be borrowed at the same time.
   * \param[in] max_slots_held_by_class The upper limit of slots that can be borrowed by receivers registered to the
   *                                    newly created ReceiverClass.
   * \return A ClassHandle representing the newly created ReceiverClass.
   * \context ANY
   * \pre The configured maximum number of classes must not be created already, i.e.
   *      CreateReceiverClass must have been called less than max_class_number times before.
   *      This precondition can be checked for in advance with CanCreateReceiverClass().
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  auto CreateReceiverClass(ReceiverClass::ClassLimit max_slots_held_by_class) noexcept -> ClassHandle;

  /*!
   * \brief Check if a new ReceiverClass can be created or if the limit for this manager is reached.
   * \return True, if a new ReceiverClass can be created, false, if the maximum ReceiverClass number is reached.
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
  auto CanCreateReceiverClass() const noexcept -> bool {
    return class_manager_.GetCurrentClassNumber() < class_manager_.GetMaxClassNumber();
  }

  /*!
   * \brief Get the current number of ReceiverClasses registered to this manager.
   * \return The number of registered ReceiverClasses to this manager.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetCurrentClassNumber() const noexcept -> ClassHandle::ReceiverClassIndex {
    return class_manager_.GetCurrentClassNumber();
  }

  /*!
   * \brief Register a ReceiverHandle to be tracked by this manager instance.
   * \details A receiver must be registered before slots can be borrowed for it.
   * \param[in] receiver A ReceiverHandle of the receiver to be tracked by this manager.
   *                     The given ReceiverHandle must be valid for this manager.
   * \param[in] receiver_class A ClassHandle of the ReceiverClass the tracked receiver should belong to.
   *                           The given ClassHandle must be valid for this manager.
   * \context ANY
   * \pre A ReceiverHandle with the same index must not already be registered to this manager instance.
   *
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void RegisterReceiver(ReceiverHandle const& receiver, ClassHandle const& receiver_class) noexcept;

  /*!
   * \brief Remove a ReceiverHandle from being tracked by this manager.
   * \details After removing a Receiver, no more slots can be borrowed for it using this manager.
   *          Removing a receiver frees the slots held by it in the manager,
   *          i.e. on future requests these slots might be given out by the manager instance again.
   * \param[in] receiver A ReceiverHandle to be removed from this manager.
   *                     The given ReceiverHandle must be valid for this manager.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void RemoveReceiver(ReceiverHandle const& receiver) noexcept;

  /*!
   * \brief Find and acquire a free slot for use by the sender (server).
   * \details Besides the registered Receivers, the sender (server) can temporarily borrow a slot
   *          to fill the related message data.
   *          After this operation returned a slot successfully the sender (server) borrows the slot for usage.
   * \return A SlotHandle if there are remaining free slots.
   * \return An empty Optional if there are no more free slots (all slots are borrowed by the sender or receivers).
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
  auto AcquireSendSlot() noexcept -> amsr::core::Optional<SlotHandle>;

  /*!
   * \brief Releases a slot for usage by the sender (server).
   * \details After this operation the sender (server) no longer borrows the slot for usage.
   * \param[in] slot The SlotHandle to be released by the sender (server).
   *                 The given SlotHandle must be valid for this manager.
   * \context ANY
   * \pre The SlotHandle must be currently borrowed by the sender (server).
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void ReleaseSendSlot(SlotHandle const& slot) noexcept;

  /*!
   * \brief Acquires a specific slot for usage by a receiver.
   * \details After this operation the receiver borrows the slot for usage (if the receiver's class limit allows it).
   *          The class limit allows acquiring the slot if borrowing the slot to the given receiver keeps the total
   *          number of borrowed slots by all receivers in the shared ReceiverClass below the configured class limit.
   * \param[in] receiver The ReceiverHandle referencing the receiver that should borrow the slot.
   *                     The given ReceiverHandle must be valid for this manager.
   * \param[in] slot The SlotHandle for the slot that should be borrowed.
   *                 The given SlotHandle must be valid for this manager.
   * \return ReceiverLimit::kNotLimited if the receiver has successfully borrowed the slot.
   * \return ReceiverLimit::kClassLimited if the receiver has reached its class limit and could not borrow the slot.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   * \pre The requested slot must not be already borrowed by the given receiver.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  auto AcquireReceiverSlot(ReceiverHandle const& receiver, SlotHandle const& slot) noexcept -> ReceiverLimit;

  /*!
   * \brief Release a specific slot to be no longer used by a receiver.
   * \details After this operation the receiver no longer borrows the slot for usage.
   * \param[in] receiver The ReceiverHandle referencing the receiver that should no longer borrow the slot.
   *                     The given ReceiverHandle must be valid for this manager.
   * \param[in] slot The SlotHandle for the slot that should be released for the given receiver.
   *                 The given SlotHandle must be valid for this manager.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   * \pre The slot must be currently borrowed by the receiver.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void ReleaseReceiverSlot(ReceiverHandle const& receiver, SlotHandle const& slot) noexcept;

  /*!
   * \brief Check if the given slot is currently borrowed by the given receiver.
   * \param[in] receiver The ReceiverHandle for the receiver to be checked.
   *                     The given ReceiverHandle must be valid for this manager.
   * \param[in] slot The SlotHandle for the slot to be checked.
   *                 The given SlotHandle must be valid for this manager.
   * \return True, if the given slot is borrowed by the given receiver, false otherwise.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  auto IsSlotBorrowedByReceiver(ReceiverHandle const& receiver, SlotHandle const& slot) const noexcept -> bool;

  /*!
   * \brief Check if the given slot is free, i.e. the slot is not borrowed by anyone.
   * \param[in] slot The SlotHandle for the slot to be checked.
   *                 The given SlotHandle must be valid for this manager.
   * \return True, if the given slot is free, false, if the given slot is borrowed by any receiver or the sender.
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
  auto IsSlotFree(SlotHandle const& slot) const noexcept -> bool;

  /*!
   * \brief Check if the given ClassHandle is valid for this manager instance.
   * \details A valid class handle is one returned by CreateReceiverClass(), therefore belonging to this manager's group
   *          and its index being within its known number of ReceiverClasses.
   * \param[in] class_handle The ClassHandle to be checked.
   * \return True, if the given ClassHandle is valid for this manager, false otherwise.
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
  auto IsClassHandleValid(ClassHandle const& class_handle) const noexcept -> bool {
    return class_handle.IsCompatibleToManager(group_, class_manager_.GetCurrentClassNumber());
  }

  /*!
   * \brief Get the class (in form of a ClassHandle) a given receiver is registered to in this manager.
   * \param[in] receiver The ReceiverHandle for the receiver to get the ClassHandle for.
   *                     The given ReceiverHandle must be valid for this manager.
   * \return The ClassHandle the receiver belongs to.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetReceiverClass(ReceiverHandle const& receiver) const noexcept -> ClassHandle;

 private:
  /*!
   * \brief The Handle::Group this manager belongs to. All Handles passed out and in must belong to this group.
   */
  Handle::Group group_;

  /*!
   * \brief The number of slots this manager can borrow.
   * \details Internal data structures are setup for this fixed size.
   *          SlotHandles are only valid for this manager if they are compatible with this size limit.
   */
  SlotHandle::SlotIndex number_of_slots_;

  /*!
   * \brief The maximum number of receivers this manager is configured for.
   * \details Internal data structures are setup for this fixed size.
   *          ReceiverHandles are only valid for this manager if they are compatible with this size limit.
   */
  ReceiverHandle::ReceiverIndex max_receiver_number_;

  /*!
   * \brief Array of BorrowedBitmaps, each entry describes the borrowed status of one slot.
   */
  vac::container::StaticVector<BorrowedBitmap> borrowed_array_;

  /*!
   * \brief The internal class_manager keeping track of class limits important for borrowing rules.
   */
  ClassManager class_manager_;

  /*!
   * \brief Asserts that the given ReceiverHandle is valid for this manager (aborts if it is not).
   * \details This is an internal helper method, aborting if any preconditions are not held.
   * \param[in] receiver_handle The ReceiverHandle to check.
   *                            The given ReceiverHandle must be valid for this manager.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertReceiverHandleValid(ReceiverHandle const& receiver_handle) const noexcept {
    if (!receiver_handle.IsCompatibleToManager(group_, max_receiver_number_)) {
      amsr::core::Abort("The given ReceiverHandle is invalid for this BorrowedManager instance.");
    }
  }

  /*!
   * \brief Asserts that the given ReceiverHandle is valid and registered to this manager (aborts if it is not).
   * \details This is an internal helper method, aborting if any preconditions are not held.
   * \param[in] receiver_handle The ReceiverHandle to check.
   *                            The given ReceiverHandle must be valid for this manager.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AssertReceiverHandleValidAndRegistered(ReceiverHandle const& receiver_handle) const noexcept;

  /*!
   * \brief Asserts that the given ClassHandle is valid for this manager (aborts if it is not).
   * \details This is an internal helper method, aborting if any preconditions are not held.
   * \param[in] class_handle The ClassHandle to check.
   *                         The given ClassHandle must be valid for this manager.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertClassHandleValid(ClassHandle const& class_handle) const noexcept {
    if (!IsClassHandleValid(class_handle)) {
      amsr::core::Abort("The given ClassHandle is invalid for this BorrowedManager instance.");
    }
  }

  /*!
   * \brief Asserts that the given SlotHandle is valid for this manager (aborts if it is not).
   * \details This is an internal helper method, aborting if any preconditions are not held.
   * \param[in] slot_handle The SlotHandle to check.
   *                        The given SlotHandle must be valid for this manager.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertSlotHandleValid(SlotHandle const& slot_handle) const noexcept {
    if (!slot_handle.IsCompatibleToManager(group_, number_of_slots_)) {
      amsr::core::Abort("The given SlotHandle is invalid for this BorrowedManager instance.");
    }
  }

  /*!
   * \brief Asserts that the given SlotHandle is valid for this manager and returns its borrowed array row (const).
   * \details This is an internal helper method, aborting if any preconditions are not held.
   * \param[in] slot_handle The SlotHandle to check.
   *                        The given SlotHandle must be valid for this manager.
   * \return A reference to the BorrowedBitmap of the given slot.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AssertSlotHandleValidAndGetBitmap(SlotHandle const& slot_handle) const noexcept -> BorrowedBitmap const& {
    AssertSlotHandleValid(slot_handle);
    // VCA_ZEROCOPYCOMMON_SPC_15_BORROWED_MANAGER_STATIC_VECTOR_AT_AFTER_ASSERT
    return borrowed_array_.at(slot_handle.GetIndex());
  }

  /*!
   * \brief Asserts that the given SlotHandle is valid for this manager and returns its borrowed array row.
   * \details This is an internal helper method, aborting if any preconditions are not held.
   * \param[in] slot_handle The SlotHandle to check.
   *                        The given SlotHandle must be valid for this manager.
   * \return A reference to the BorrowedBitmap of the given slot.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AssertSlotHandleValidAndGetBitmap(SlotHandle const& slot_handle) noexcept -> BorrowedBitmap& {
    AssertSlotHandleValid(slot_handle);
    // VCA_ZEROCOPYCOMMON_SPC_15_BORROWED_MANAGER_STATIC_VECTOR_AT_AFTER_ASSERT
    return borrowed_array_.at(slot_handle.GetIndex());
  }

  /*!
   * \brief Friend test to allow access to the class_manager_ to check for concrete ReceiverClasses.
   */
  FRIEND_TEST(UT__BorrowedManager__Manager, RegisterReceiver);
  /*!
   * \brief Friend test to allow access to the class_manager_ to check for concrete ReceiverClasses.
   */
  FRIEND_TEST(UT__BorrowedManager__Manager, RemoveReceiverReAdd);
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_BORROWED_MANAGER_H_

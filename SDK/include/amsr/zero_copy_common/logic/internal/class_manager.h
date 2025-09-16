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
 *        \brief Definition of the ClassManager class.
 *        \unit ZeroCopyCommon::ZeroCopyLogic::BorrowedManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLASS_MANAGER_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLASS_MANAGER_H_

#include "amsr/core/optional.h"
#include "vac/container/static_vector.h"

#include "amsr/zero_copy_common/logic/internal/borrowed_bitmap.h"
#include "amsr/zero_copy_common/logic/internal/class_handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver_class.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Manager tracking ReceiverClasses for registered Receivers and keeping track of the classes' slot limits.
 * \details The ClassManager assigns each registered receiver to exactly one ReceiverClass.
 *          A configured number of ReceiverClasses can be created for one ClassManager and each ReceiverClass has
 *          an upper limit on the total number of slots borrowed by receivers of this class at any point in time.
 *          The ClassManager allows queries asking if a receiver can still acquire a certain slot given its limits.
 *
 *          The ClassManager does not know the state of all slots. Rather, it acts as a cache that is notified about
 *          every change in borrowed status of a given slot's BorrowedBitmap.
 *          Therefore, it must be strictly used as a unit-private helper to not violate the contract required to keep
 *          the cache up to date (TryAcquiringSlotForReceiver and ReleaseSlotForReceiver must be exclusively used
 *          to update the BorrowedBitmaps of slots).
 *          In return, the ClassManager does not need to know about the number of slots and offers fast lookup times.
 *
 *          The ClassManager is unit-private and DOES NOT check preconditions, as it is a unit-internal API.
 *          ClassHandles are assumed valid if the Handle::Group is correct and their index < GetCurrentClassNumber().
 *          ReceiverHandles are assumed valid if the Handle::Group is correct and their index < max_receiver_number.
 *          ReceiverHandles are assumed registered if RegisterReceiver was called and RemoveReceiver was not called.
 *
 * \vprivate Component Private
 */
// VCA_ZEROCOPYCOMMON_SPC_15_CLASS_MANAGER_VECTOR_DESTRUCTOR_FUNCTION_CONTRACT
class ClassManager final {
 public:
  /*!
   * \brief Create a ClassManager for a fixed number of classes and a maximum number of receivers.
   * \param[in] group The group identifier all cooperating managers belong to.
   * \param[in] max_receiver_number The maximum number of receivers that can be registered to this ClassManager.
   *                                It must hold that max_receiver_number <= BorrowedBitmap::kReceiverLimit.
   * \param[in] max_class_number The maximum number of classes that can be created for this ClassManager.
   * \context ANY
   * \pre The max_receiver_number and max_class_number must match the related BorrowedManager.
   *      This precondition can not be checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ClassManager(Handle::Group const& group, ReceiverHandle::ReceiverIndex max_receiver_number,
               ClassHandle::ReceiverClassIndex max_class_number) noexcept;

  /*!
   * \brief Create a new ReceiverClass with an upper limit on the number of slots that can be borrowed at the same time.
   * \param[in] max_slots_held_by_class The upper limit of slots concurrently borrowed by receivers registered to the
   *                                    newly created ReceiverClass.
   * \return A ClassHandle representing the newly created ReceiverClass.
   * \context ANY
   * \pre The configured maximum number of classes must not be created already, i.e.
   *      GetCurrentClassNumber() < GetMaxClassNumber() must hold.
   *      Precondition is NOT CHECKED as this is a unit-internal API.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto CreateReceiverClass(ReceiverClass::ClassLimit max_slots_held_by_class) noexcept -> ClassHandle;

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
   *
   * \spec requires true; \endspec
   */
  auto GetCurrentClassNumber() const noexcept -> ClassHandle::ReceiverClassIndex { return receiver_classes_.size(); }

  /*!
   * \brief Get the maximum number of ReceiverClasses supported by this manager.
   * \return The maximum number of ReceiverClasses supported by this manager.
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
  auto GetMaxClassNumber() const noexcept -> ClassHandle::ReceiverClassIndex { return receiver_classes_.capacity(); }

  /*!
   * \brief Register a ReceiverHandle to be tracked by this manager instance.
   * \details A receiver must be registered before slots can be borrowed for it.
   * \param[in] receiver A ReceiverHandle of the receiver to be tracked by this manager.
   *                     The given ReceiverHandle must be valid for this manager.
   *                     Precondition is NOT CHECKED as this is a unit-internal API.
   * \param[in] receiver_class A ClassHandle of the ReceiverClass the tracked receiver should belong to.
   *                           The given ClassHandle must be valid for this manager.
   *                           Precondition is NOT CHECKED as this is a unit-internal API.
   * \context ANY
   * \pre A receiver with the same index must not already be registered to this manager instance.
   *      Can be checked with CanRegisterReceiver().
   *      Precondition is NOT CHECKED as this is a unit-internal API.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RegisterReceiver(ReceiverHandle const& receiver, ClassHandle const& receiver_class) noexcept;

  /*!
   * \brief Check if the given ReceiverHandle can be registered.
   * \details A ReceiverHandle can be registered, if there is no other receiver registered at the same index.
   * \param[in] receiver The ReceiverHandle to be checked.
   *                     The given ReceiverHandle must be valid for this manager.
   *                     Precondition is NOT CHECKED as this is a unit-internal API.
   * \return True, if the ReceiverHandle can be registered, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto CanRegisterReceiver(ReceiverHandle const& receiver) const noexcept -> bool;

  /*!
   * \brief Remove a ReceiverHandle from being tracked by this manager.
   * \param[in] receiver A ReceiverHandle to be removed from this manager.
   *                     The given ReceiverHandle must be valid for this manager.
   *                     Precondition is NOT CHECKED as this is a unit-internal API.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   *      Precondition is NOT CHECKED as this is a unit-internal API.
   * \pre The caller must ensure that no more Slots are borrowed by the given receiver to keep the cache consistent.
   *      This can be achieved by calling ReleaseSlotForReceiver for each (borrowed) slot before calling this method.
   *      Precondition is NOT CHECKED as this is a unit-internal API.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RemoveReceiver(ReceiverHandle const& receiver) noexcept;

  /*!
   * \brief Try to acquire a slot for the given receiver respecting the Receiver's class limit.
   *        Does nothing if the slot is already borrowed by the receiver.
   * \details Checks if the class limit would be exceeded by the given receiver acquiring the slot.
   *          If the limit allows acquiring the slot, the acquisition is performed.
   * \param[in] receiver The ReceiverHandle for the receiver to acquire a new slot.
   *                     The given ReceiverHandle must be valid for this manager.
   *                     Precondition is NOT CHECKED as this is a unit-internal API.
   * \param[in,out] slot_bitmap The BorrowedBitmap of the slot the given receiver should acquire.
   *                             Will be modified to reflect the new borrowed status if the class limit allows.
   * \return True, if the update is allowed according to the class limit and the slot is now borrowed by the receiver.
   * \return False, if the update is not allowed because the class limit is reached.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   *      Precondition is NOT CHECKED as this is a unit-internal API.
   * \pre The caller must perform all borrowed status changes using the ClassManager's methods to keep the cache valid.
   *      This means, only TryAcquiringSlotForReceiver and ReleaseSlotForReceiver must be used to update the borrowed
   *      status of a slot.
   *      Precondition is NOT CHECKED as this is a unit-internal API.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto TryAcquiringSlotForReceiver(ReceiverHandle const& receiver, BorrowedBitmap& slot_bitmap) noexcept -> bool;

  /*!
   * \brief Releases a slot for the given receiver.
   *        Does nothing if the slot is not borrowed by the receiver.
   * \param[in] receiver The ReceiverHandle for the receiver to release the slot.
   *                     The given ReceiverHandle must be valid for this manager.
   *                     Precondition is NOT CHECKED as this is a unit-internal API.
   * \param[in,out] slot_bitmap The BorrowedBitmap of the slot the given receiver should release.
   *                             Will be modified to reflect the new borrowed status.
   * \context ANY
   * \pre The ReceiverHandle must be registered to this manager.
   *      Precondition is NOT CHECKED as this is a unit-internal API.
   * \pre The caller must perform all borrowed status changes using the ClassManager's methods to keep the cache valid.
   *      This means, only TryAcquiringSlotForReceiver and ReleaseSlotForReceiver must be used to update the borrowed
   *      status of a slot.
   *      Precondition is NOT CHECKED as this is a unit-internal API.
   *
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ReleaseSlotForReceiver(ReceiverHandle const& receiver, BorrowedBitmap& slot_bitmap) noexcept;

  /*!
   * \brief Get the class (in form of a ClassHandle) a given receiver is registered to in this manager.
   * \param[in] receiver The ReceiverHandle for the receiver to get the ClassHandle for.
   *                     The given ReceiverHandle must be valid for this manager.
   *                     Precondition is NOT CHECKED as this is a unit-internal API.
   * \return The ClassHandle the receiver belongs to, if the receiver is registered to the manager.
   * \return An empty Optional, if the receiver is not registered to the manager.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetReceiverClass(ReceiverHandle const& receiver) const noexcept -> amsr::core::Optional<ClassHandle>;

 private:
  /*!
   * \brief Struct mapping one ReceiverHandle to one ClassHandle.
   * \details We store the full ReceiverHandle to detect if the exact given ReceiverHandle is contained in a mapping.
   *          Without this, it could happen that a ReceiverHandle with same index but different id is mistakenly
   *          accepted for the wrong mapping.
   */
  struct ReceiverToClassMapping {
    /*!
     * \brief The ReceiverHandle for the receiver being mapped to the below ReceiverClass.
     */
    ReceiverHandle receiver;
    /*!
     * \brief The ClassHandle for the ReceiverClass the above receiver is mapped to.
     */
    ClassHandle receiver_class;
  };

  /*!
   * \brief Optional ReceiverToClassMapping. Empty indicates no registered mapping.
   */
  using OptionalReceiverToClassMapping = amsr::core::Optional<ReceiverToClassMapping>;

  /*!
   * \brief Type holding the information on which receiver is registered to which ReceiverClass.
   * \details This manager handles up to a static maximum number of receivers.
   *          The mapping from receiver to ReceiverClass is done by holding an array of Optional<...> values,
   *          the correct mapping can be looked up by the ReceiverHandle's index.
   *          A filled Optional indicates an existing mapping for a given index.
   *          A vector of Optionals is used instead of a map for efficiency reasons (libvac only has a static tree map).
   */
  using ReceiverToClassVector = vac::container::StaticVector<OptionalReceiverToClassMapping>;

  /*!
   * \brief Type holding the ReceiverClasses managed by this ClassManager.
   * \details Lookup from ClassHandles to actual classes is done by index.
   */
  using ReceiverClassVector = vac::container::StaticVector<ReceiverClass>;

  /*!
   * \brief The group this manager belongs to.
   */
  Handle::Group group_;

  /*!
   * \brief Container holding the mapping from registered receivers to ReceiverClasses
   */
  ReceiverToClassVector receiver_to_class_mappings_{};

  /*!
   * \brief Container holding all managed ReceiverClasses.
   */
  ReceiverClassVector receiver_classes_{};
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLASS_MANAGER_H_

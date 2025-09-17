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
 *        \brief  Implementation of the SlotManager.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::SlotManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_MANAGER_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_MANAGER_H_

#include <type_traits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/optional.h"
#include "vac/container/static_vector.h"

#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/logic/internal/readable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/slot_manager_information.h"
#include "amsr/zero_copy_common/logic/internal/slot_token.h"
#include "amsr/zero_copy_common/logic/internal/writable_slot_descriptor.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Class managing Readable- or Writable-SlotDescriptors, giving access to them using SlotHandles and SlotTokens.
 * \details Manages a list of SlotDescriptors.
 *          Each SlotDescriptor can be referred to by SlotHandles and consequently accessed using this SlotHandle.
 *          Each SlotDescriptor can be referred to by exactly one SlotToken instance that can be obtained from the
 *          manager and returned to the manager.
 * \tparam DescriptorType The type of SlotDescriptors stored in this manager.
 *                        The type must inherit either from ReadableSlotDescriptor or WritableSlotDescriptor.
 *                        The type must fulfill the specification described in the above virtual interfaces.
 *                        The correct function of the type must be ensured by fully testing against the interface spec.
 * \vprivate Component Private
 */
template <class DescriptorType>
class SlotManager {  // VCA_ZEROCOPYCOMMON_SPC_15_SLOT_MANAGER_VECTOR_DESTRUCTOR_FUNCTION_CONTRACT
  static_assert(std::is_base_of<ReadableSlotDescriptor, DescriptorType>::value,
                "Slot manager objects must inherit from ReadableSlotDescriptor or WritableSlotDescriptor.");

  /*!
   * \brief Type of information held on each slot in this SlotManager.
   */
  using SlotInformation = SlotManagerInformation<DescriptorType>;
  static_assert(std::is_nothrow_move_constructible<SlotInformation>::value,
                "Elements stored in the vector shall be nothrow move constructible.");
  static_assert(std::is_nothrow_destructible<SlotInformation>::value,
                "Elements stored in the vector shall be nothrow destructible.");

  /*!
   * \brief Vector type holding SlotManagerInformation objects of this SlotManager.
   */
  using SlotStorage = vac::container::StaticVector<SlotInformation>;

 public:
  /*!
   * \brief Create a new SlotManager belonging to a specific manager group.
   * \param[in] group The group identifier all cooperating managers belong to.
   * \param[in] max_slot_number The maximum number of SlotDescriptors this SlotManager can hold.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  SlotManager(Handle::Group group, SlotHandle::SlotIndex max_slot_number)
      : slots_{}, group_{group}, max_slot_number_{max_slot_number} {
    slots_.reserve(max_slot_number_);
  };

  /*!
   * \brief Check if this SlotManager holds Writable- or ReadableSlotDescriptors.
   * \return True if the SlotDescriptors managed are of type WritableSlotDescriptor.
   * \return False if the SlotDescriptors managed are only of type ReadableSlotDescriptor.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static constexpr auto IsManagingWritableSlotDescriptors() noexcept -> bool {
    return std::is_base_of<WritableSlotDescriptor, DescriptorType>::value;
  }

  /*!
   * \brief Add a SlotDescriptor to be managed by this SlotManager instance.
   * \details This will transfer ownership of the given SlotDescriptor to the SlotManager.
   *          The passed SlotDescriptor is alive as long as the SlotManager instance exists.
   *          The user of the SlotManager must ensure that the memory referenced by the SlotDescriptor
   *          outlives this SlotManager.
   * \param[in] slot_descriptor SlotDescriptor to be added to the SlotManager.
                                Must not be a nullptr. This is not checked.
   * \return A SlotHandle referring to the now managed SlotDescriptor.
   * \context ANY
   * \pre The maximum number of managed slots must not be reached, i.e. CanAddSlot() must return true.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \trace DSGN-ZeroCopyCommon-InitializeSharedMemoryMessageRegions
   * \vprivate Component Private
   */
  auto AddSlot(std::unique_ptr<DescriptorType> slot_descriptor) noexcept -> SlotHandle {
    SlotHandle::SlotIndex const inserted_slot_index{GetCurrentSlotNumber()};
    if (inserted_slot_index >= max_slot_number_) {
      amsr::core::Abort("Maximum number of slots that can be managed by this instance exceeded.");
    }
    SlotHandle const inserted_slot_handle{ConstructSlotHandle(inserted_slot_index)};

    SlotToken access_token{SlotToken{inserted_slot_handle}};
    // VCA_ZEROCOPYCOMMON_LNG_01_SLOT_MANAGER_VALID_DESCRIPTOR_UNIQUE_POINTER
    SlotInformation inserted_information{std::move(slot_descriptor), std::move(access_token)};
    // VCA_ZEROCOPYCOMMON_SPC_15_SLOT_MANAGER_VECTOR_EMPLACE_BACK_FUNCTION_CONTRACT
    slots_.template emplace_back(std::move(inserted_information));

    return inserted_slot_handle;
  }

  /*!
   * \brief Check if a new SlotDescriptor can be added or if the limit for this SlotManager is reached.
   * \return True, if a new SlotDescriptor can be added, false, if the maximum number is reached.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto CanAddSlot() const noexcept -> bool {
    bool const can_add_slot{GetCurrentSlotNumber() < max_slot_number_};
    return can_add_slot;
  }

  /*!
   * \brief Check if a given SlotHandle is valid for this SlotManager instance.
   * \param[in] handle The SlotHandle to be checked.
   * \return True, if the given SlotHandle is valid for this SlotManager, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsSlotHandleValid(SlotHandle const& handle) const noexcept -> bool {
    SlotHandle::SlotIndex const current_slot_number{GetCurrentSlotNumber()};
    return handle.IsCompatibleToManager(group_, current_slot_number);
  }

  /*!
   * \brief Check if a given SlotToken is valid for this SlotManager instance.
   * \param[in] token The SlotToken to be checked.
   * \return True, if the given SlotToken is valid for this SlotManager, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsSlotTokenValid(SlotToken const& token) const noexcept -> bool {
    bool result{false};

    SlotHandle const handle{token.GetHandle()};
    bool const is_handle_valid{IsSlotHandleValid(handle)};
    if (is_handle_valid) {
      // VCA_ZEROCOPYCOMMON_SPC_15_SLOT_MANAGER_VECTOR_AT_FUNCTION_CONTRACT
      SlotInformation const& slot_information{slots_.at(handle.GetIndex())};
      bool const is_token_owned_by_manager{slot_information.IsAccessTokenOwnedByManager()};
      result = !is_token_owned_by_manager;
    } else {
      result = false;
    }

    return result;
  }

  /*!
   * \brief Get a SlotHandle referring to the SlotDescriptor with the given index.
   * \param[in] index The index of the SlotDescriptor managed by this SlotManager instance.
   * \return The SlotHandle of the SlotDescriptor if it exists.
   * \return An empty optional if the given index is not present in this SlotManager instance.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetSlotHandleForIndex(SlotHandle::SlotIndex index) const noexcept -> amsr::core::Optional<SlotHandle> {
    amsr::core::Optional<SlotHandle> opt_result_handle{};

    bool const index_in_bounds{index < slots_.size()};
    if (index_in_bounds) {
      opt_result_handle.template emplace(ConstructSlotHandle(index));
    }

    return opt_result_handle;
  }

  /*!
   * \brief Get the current number of slots managed by this SlotManager instance.
   * \return The current number of slots managed by this SlotManager.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetCurrentSlotNumber() const noexcept -> SlotHandle::SlotIndex {
    // The slot_ vector size is limited by the max_slot_number_, which is of type SlotIndex.
    // Therefore, the cast is always valid.
    return static_cast<SlotHandle::SlotIndex>(slots_.size());
  }

  /*!
   * \brief Get a reference to a SlotDescriptor for the given SlotHandle.
   * \param[in] handle A SlotHandle to a SlotDescriptor managed by this SlotManager.
   * \return A reference to the requested SlotDescriptor.
             The returned reference stays valid for the whole lifetime of this SlotManager instance.
   * \context ANY
   * \pre The given SlotHandle must be valid for this SlotManager instance.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \trace DSGN-ZeroCopyCommon-MessageSlotAccess
   * \vprivate Component Private
   */
  auto GetSlotDescriptor(SlotHandle handle) noexcept -> DescriptorType& {
    SlotInformation& slot_information{AssertHandleValidAndGetSlotInformation(handle)};
    // VCA_ZEROCOPYCOMMON_LNG_02_SLOT_MANAGER_VALID_DESCRIPTOR_REFERENCE_RETURN_VALUE
    return slot_information.GetDescriptor();
  }

  /*!
   * \brief Get a reference to a SlotDescriptor for the given SlotToken.
   * \details Can be used to grant access to slot message data for the holder of a SlotToken.
   * \param[in] token A SlotToken to a SlotDescriptor managed by this SlotManager.
   * \return A reference to the requested SlotDescriptor.
             The returned reference stays valid for the whole lifetime of this SlotManager instance.
   * \context ANY
   * \pre The given SlotToken must be valid for this SlotManager instance.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \trace DSGN-ZeroCopyCommon-MessageSlotAccess
   * \vprivate Component Private
   */
  auto GetSlotDescriptor(SlotToken const& token) noexcept -> DescriptorType& {
    SlotInformation& slot_information{AssertTokenValidAndGetSlotInformation(token)};
    // VCA_ZEROCOPYCOMMON_LNG_02_SLOT_MANAGER_VALID_DESCRIPTOR_REFERENCE_RETURN_VALUE
    return slot_information.GetDescriptor();
  }

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::SlotManager::GetSlotDescriptor(SlotToken const&)
   */
  auto GetSlotDescriptor(SlotToken const& token) const noexcept -> DescriptorType const& {
    SlotInformation const& slot_information{AssertTokenValidAndGetSlotInformation(token)};
    // VCA_ZEROCOPYCOMMON_LNG_02_SLOT_MANAGER_VALID_DESCRIPTOR_REFERENCE_RETURN_VALUE
    return slot_information.GetDescriptor();
  }

  /*!
   * \brief Get the single SlotTaken available for the slot referred to by the given SlotHandle.
   * \param[in] handle The SlotHandle to the slot for which the SlotToken should be received.
   * \return The SlotToken to the requested slot if available.
   * \return Empty optional if the SlotToken is currently in use (not returned to the manager).
   * \context ANY
   * \pre The SlotHandle must be valid for this SlotManager instance.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetSlotAccessToken(SlotHandle handle) noexcept -> amsr::core::Optional<SlotToken> {
    SlotInformation& slot_information{AssertHandleValidAndGetSlotInformation(handle)};
    amsr::core::Optional<SlotToken> opt_result_token{};

    bool const access_token_owned_by_server{slot_information.IsAccessTokenOwnedByManager()};
    if (access_token_owned_by_server) {
      opt_result_token.template emplace(slot_information.GetAccessToken());
    }

    return opt_result_token;
  }

  /*!
   * \brief Return the single SlotToken available for a slot back to the SlotManager.
   * \param[in] token The SlotToken to be returned.
   * \context ANY
   * \pre The given SlotToken must be valid for this SlotManager instance.
   *      The given SlotToken is currently not returned to the manager.
   *      This should not happen under normal use and indicates that a SlotToken was duplicated and returned
   *      twice to the SlotManager (only possible with programming mistakes, e.g. re-using a moved value).
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ReturnSlotAccessToken(SlotToken token) noexcept {
    SlotInformation& slot_information{AssertTokenValidAndGetSlotInformation(token)};

    // We now own the SlotToken to the slot again.
    slot_information.ReturnAccessToken(std::move(token));
  }

 private:
  /*!
   * \brief Construct a SlotHandle for this SlotManager's group with the given index without any bounds checking.
   * \param[in] index The index of the constructed SlotHandle.
   * \return The newly created SlotHandle for this SlotManager.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ConstructSlotHandle(SlotHandle::SlotIndex index) const noexcept -> SlotHandle {
    return SlotHandle{group_, index};
  }

  /*!
   * \brief Assert that the SlotHandle is valid for this SlotManager and return the associated SlotManagerInformation.
   * \param[in] handle The SlotHandle to be checked.
   * \return A reference to the DescriptorInformation referred to by the given SlotHandle.
   * \context ANY
   * \pre The SlotHandle's group must match this SlotManager's group and the SlotHandle's index must be
   *      within the SlotManager's bounds, i.e. index < GetCurrentSlotNumber().
   *      If this precondition is violated abort is called.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AssertHandleValidAndGetSlotInformation(SlotHandle handle) noexcept -> SlotInformation& {
    bool const is_handle_valid{IsSlotHandleValid(handle)};
    if (!is_handle_valid) {
      amsr::core::Abort("The given handle is invalid for this SlotManager.");
    }

    // VCA_ZEROCOPYCOMMON_SPC_15_SLOT_MANAGER_VECTOR_AT_FUNCTION_CONTRACT
    SlotInformation& slot_information{slots_.at(handle.GetIndex())};
    return slot_information;
  }

  /*!
   * \brief Assert that the SlotToken is valid for this SlotManager and return the SlotManagerInformation for it.
   * \param[in] token The SlotToken to be checked.
   * \return The SlotManagerInformation for the valid SlotToken.
   * \context ANY
   * \pre The given SlotToken must be currently 'handed out' by the SlotManager to a user (not owned by the
   *      SlotManager). The SlotHandle referenced by the SlotToken must be valid for this SlotManager.
   *      If this precondition is violated abort is called.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AssertTokenValidAndGetSlotInformation(SlotToken const& token) noexcept -> SlotInformation& {
    bool const is_token_valid{IsSlotTokenValid(token)};
    if (!is_token_valid) {
      amsr::core::Abort("The given token is invalid for this SlotManager.");
    }

    SlotInformation& slot_information{slots_.at(token.GetHandle().GetIndex())};
    return slot_information;
  }

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::SlotManager::AssertTokenValidAndGetSlotInformation
   */
  auto AssertTokenValidAndGetSlotInformation(SlotToken const& token) const noexcept -> SlotInformation const& {
    bool const is_token_valid{IsSlotTokenValid(token)};
    if (!is_token_valid) {
      amsr::core::Abort("The given token is invalid for this SlotManager.");
    }

    SlotInformation const& slot_information{slots_.at(token.GetHandle().GetIndex())};
    return slot_information;
  }

  /*!
   * \brief Vector of SlotManagerInformation objects, describing the currently managed slots.
   */
  SlotStorage slots_;

  /*!
   * \brief The group of managers this SlotManager instance belongs to.
   */
  Handle::Group group_;

  /*!
   * \brief The maximum number of slots that can be held by this SlotManager.
   */
  SlotHandle::SlotIndex max_slot_number_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_MANAGER_H_

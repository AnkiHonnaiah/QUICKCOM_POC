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
 *        \brief  Implementation of SlotHandle.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::SlotManager
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_HANDLE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_HANDLE_H_

#include <cstddef>
#include <cstdint>
#include <limits>

#include "amsr/zero_copy_common/logic/internal/handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A SlotHandle representing one SlotDescriptor managed by a SlotManager.
 * \details A SlotHandle belongs to exactly one SlotManager and contains an index into the manager.
 *          This index is guaranteed to be in the range [0; max_number_of_slots).
 * \vprivate Component Private
 */
class SlotHandle final : public Handle {
 public:
  /*!
   * \brief Type used for slot indices.
   * \details The index is sent over the queue between process boundaries.
   *          As the queue's implementation currently limits its size to uint32,
   *          there is also no need for more slots (as all slots can potentially be in one queue at one time).
   */
  using SlotIndex = std::uint32_t;

  /*!
   * \brief Static assert to check that the SlotIndex type is of expected range compared to std::size_t.
   */
  static_assert(std::numeric_limits<SlotIndex>::max() <= std::numeric_limits<std::size_t>::max(),
                "The platform must have a std::size_t type with bigger range than SlotIndex (std::uint32_t) as we "
                "create arrays/vectors/containers of up to SlotIndex::maxValue() size. If the static_assert does not "
                "hold it could be possible to create or address containers beyond their expected index range.");

  /*!
   * \brief Create a SlotHandle representing one SlotDescriptor.
   * \param[in] group The group of the SlotManager this SlotHandle should represent.
   * \param[in] index The index of the SlotDescriptor this SlotHandle should represent.
   * \context ANY
   * \pre Must only create SlotHandles with a valid index for the given manager group.
   *      This precondition can not be enforced automatically and must be fulfilled by the caller.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  SlotHandle(Handle::Group group, SlotIndex index) noexcept : Handle{group}, index_{index} {};

  /*!
   * \brief Destroy this SlotHandle.
   */
  ~SlotHandle() = default;

  /*!
   * \brief Create a copy of a SlotHandle.
   * \vprivate Component  Private.
   */
  SlotHandle(SlotHandle const&) noexcept = default;

  /*!
   * \brief Create a SlotHandle by moving the content of another SlotHandle into the new instance.
   * \vprivate Component Private.
   */
  SlotHandle(SlotHandle&&) noexcept = default;

  /*!
   * \brief Copy the content of the given SlotHandle into this SlotHandle.
   * \vprivate Component Private
   */
  auto operator=(SlotHandle const&) & noexcept -> SlotHandle& = default;

  /*!
   * \brief Move the content of the given SlotHandle into this SlotHandle.
   * \vprivate Component Private
   */
  auto operator=(SlotHandle&&) & noexcept -> SlotHandle& = default;

  /*!
   * \brief Get the index of the SlotDescriptor that this SlotHandle represents.
   * \details This index is guaranteed to be in the range [0; max_number_of_slots) for the related manager.
   * \return The index of the SlotDescriptor that this SlotHandle represents.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetIndex() const noexcept -> SlotIndex { return index_; }

  /*!
   * \brief Perform a basic check if this SlotHandle is valid for a given Manager instance.
   * \details The basic check ensures that the Handle's group matches the Manager's group
   *          and that the Handle's index fits within the expected number of maximum_handles for the Manager.
   *          If this check succeeds, a manager can safely assume GetIndex() < max_handle_number.
   * \param[in] manager_group The Handle::Group of the Manager to check compatibility for.
   * \param[in] max_handle_number The maximum number of Handles this GetIndex() must be below.
   * \return True, if the Handle is compatible with the given Manager parameters, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsCompatibleToManager(Handle::Group const& manager_group, SlotIndex max_handle_number) const noexcept -> bool {
    bool const is_handle_group_matching{IsSameHandleGroup(manager_group)};
    bool const is_index_within_bounds{index_ < max_handle_number};
    bool const is_handle_compatible{is_handle_group_matching && is_index_within_bounds};
    return is_handle_compatible;
  }

  /*!
   * \brief Compare two SlotHandles.
   * \param[in] other The other slot SlotHandle to compare to.
   * \return True, if both SlotHandles refer to the same SlotDescriptor.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto operator==(SlotHandle const& other) const noexcept -> bool {
    bool const group_equals{IsSameHandleGroup(other)};
    bool const index_equals{index_ == other.index_};
    return group_equals && index_equals;
  }

 private:
  /*!
   * \brief The index of the SlotDescriptor that this SlotHandle represents.
   */
  SlotIndex index_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_HANDLE_H_

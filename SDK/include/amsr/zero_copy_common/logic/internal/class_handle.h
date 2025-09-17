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
 *        \brief Declaration and implementation of the ClassHandle class.
 *        \unit ZeroCopyCommon::ZeroCopyLogic::BorrowedManager
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLASS_HANDLE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLASS_HANDLE_H_

#include <cstddef>

#include "amsr/zero_copy_common/logic/internal/handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A ClassHandle representing one ReceiverClass in the BorrowedManager.
 * \details A ClassHandle belongs to exactly one ClassManager (therefore one BorrowedManager)
 *          and contains an index into the ClassManager.
 *          This index is guaranteed to be in the range [0; number_of_classes).
 * \vprivate Product Private
 */
class ClassHandle final : public Handle {
 public:
  /*!
   * \brief Type used for ReceiverClass indices in the ClassManager.
   */
  using ReceiverClassIndex = std::size_t;

  /*!
   * \brief Create a ClassHandle representing one ReceiverClass.
   * \details A ClassHandle should always be created for a specific ClassManager instance,
   *          having the Handle::Group of the manager and an index within the limits of the manager.
   * \param[in] group The group of the ClassManager.
   * \param[in] index The index of the ReceiverClass this ClassHandle should represent.
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
  ClassHandle(Handle::Group const& group, ReceiverClassIndex index) noexcept : Handle{group}, index_{index} {};

  /*!
   * \brief Destroy this ClassHandle.
   * \vprivate Product Private
   *
   * \spec requires true; \endspec
   */
  ~ClassHandle() noexcept = default;

  /*!
   * \brief Create a copy of a ClassHandle.
   * \vprivate Product Private
   *
   * \spec requires true; \endspec
   */
  ClassHandle(ClassHandle const&) noexcept = default;

  /*!
   * \brief Create a ClassHandle by moving the content of another ClassHandle into the new instance.
   * \vprivate Product Private
   *
   * \spec requires true; \endspec
   */
  ClassHandle(ClassHandle&&) noexcept = default;

  /*!
   * \brief Copy the content of the given ClassHandle into this ClassHandle.
   * \param[in] handle The ClassHandle to copy.
   * \return The copied ClassHandle.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   *
   * \spec requires true; \endspec
   */
  auto operator=(ClassHandle const& handle) & noexcept -> ClassHandle& = default;

  /*!
   * \brief Move the content of the given ClassHandle into this ClassHandle.
   * \param[in] handle The ClassHandle to move from.
   * \return The moved-to ClassHandle.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   *
   * \spec requires true; \endspec
   */
  auto operator=(ClassHandle&& handle) & noexcept -> ClassHandle& = default;

  /*!
   * \brief Get the index of the ReceiverClass that this ClassHandle represents.
   * \details This index is guaranteed to be in the range [0; number_of_classes) for the related ClassManager it is
   *          created from.
   * \return The index of the ReceiverClass that this ClassHandle represents.
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
  auto GetIndex() const noexcept -> ReceiverClassIndex { return index_; }

  /*!
   * \brief Perform a basic check if this ClassHandle is valid for a given Manager instance.
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
   *
   * \spec requires true; \endspec
   */
  auto IsCompatibleToManager(Handle::Group const& manager_group, ReceiverClassIndex max_handle_number) const noexcept
      -> bool {
    bool const is_handle_group_matching{IsSameHandleGroup(manager_group)};
    bool const is_index_within_bounds{index_ < max_handle_number};
    bool const is_handle_compatible{is_handle_group_matching && is_index_within_bounds};
    return is_handle_compatible;
  }

  /*!
   * \brief Compare two ClassHandles.
   * \param[in] other The other ClassHandle to compare to.
   * \return True, if both ClassHandles refer to the same ReceiverClass, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   *
   * \spec requires true; \endspec
   */
  auto operator==(ClassHandle const& other) const noexcept -> bool {
    bool const group_equals{IsSameHandleGroup(other)};
    bool const index_equals{index_ == other.index_};
    return group_equals && index_equals;
  }

 private:
  /*!
   * \brief The index of the ReceiverClass that this ClassHandle represents.
   */
  ReceiverClassIndex index_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLASS_HANDLE_H_

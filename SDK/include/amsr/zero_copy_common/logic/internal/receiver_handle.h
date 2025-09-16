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
 *        \brief  Definition of class ReceiverHandle.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::ReceiverManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_HANDLE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_HANDLE_H_

#include <cstddef>

#include "amsr/zero_copy_common/logic/internal/handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A ReceiverHandle representing a Receiver managed by the ReceiverManager class.
 * \details A ReceiverHandle belongs to exactly one ReceiverManager and contains an index.
 *          This index is guaranteed to be in the range [0; max_number_receivers).
 *          Additionally, a ReceiverHandle contains an id.
 *          There are no guarantees regarding the uniqueness of the id as it is ONLY used internally in this unit.
 * \vprivate Component Private
 */
class ReceiverHandle final : public Handle {
 public:
  /*!
   * \brief Type used for receiver ids.
   */
  using ReceiverId = std::size_t;

  /*!
   * \brief Type used for receiver indices.
   */
  using ReceiverIndex = std::size_t;

  /*!
   * \brief Construct a ReceiverHandle.
   * \param group Group of the ReceiverHandle.
   * \param id ReceiverId of the ReceiverHandle.
   * \param index ReceiverIndex of the ReceiverHandle.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ReceiverHandle(Handle::Group const& group, ReceiverId id, ReceiverIndex index) noexcept
      : Handle{group}, id_{id}, index_{index} {}

  /*!
   * \brief Destruct the ReceiverHandle.
   * \vprivate Component Private
   */
  ~ReceiverHandle() noexcept = default;

  /*!
   * \brief Construct a ReceiverHandle by copying.
   * \vprivate Component Private
   */
  ReceiverHandle(ReceiverHandle const&) noexcept = default;

  /*!
   * \brief Construct a ReceiverHandle by moving.
   * \vprivate Component Private
   */
  ReceiverHandle(ReceiverHandle&&) noexcept = default;

  /*!
   * \brief Copy another ReceiverHandle into this instance.
   * \vprivate Component Private
   */
  auto operator=(ReceiverHandle const&) & noexcept -> ReceiverHandle& = default;

  /*!
   * \brief Move another ReceiverHandle into this instance.
   * \vprivate Component Private
   */
  auto operator=(ReceiverHandle&&) & noexcept -> ReceiverHandle& = default;

  /*!
   * \brief Perform a basic check if this ReceiverHandle is valid for a given Manager instance.
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
  auto IsCompatibleToManager(Handle::Group const& manager_group, ReceiverIndex max_handle_number) const noexcept
      -> bool {
    bool const is_handle_group_matching{IsSameHandleGroup(manager_group)};
    bool const is_index_within_bounds{index_ < max_handle_number};
    bool const is_handle_compatible{is_handle_group_matching && is_index_within_bounds};
    return is_handle_compatible;
  }

  /*!
   * \brief Determine if two ReceiverHandle instances are equal.
   * \details Two ReceiverHandle instances are equal if the following conditions are met: same handle group, same
   *          receiver id and same receiver index.
   * \param other ReceiverHandle to determine equality for.
   * \return True if ReceiverHandle instances are equal, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto operator==(ReceiverHandle const& other) const noexcept -> bool {
    return IsSameHandleGroup(other) && (id_ == other.id_) && (index_ == other.index_);
  }

  /*!
   * \brief Determine if two ReceiverHandle instances are unequal.
   * \details Negated value of equal-to operator.
   * \param other ReceiverHandle to determine inequality for.
   * \return True if ReceiverHandle instances are unequal, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto operator!=(ReceiverHandle const& other) const noexcept -> bool { return !(*this == other); }

  /*!
   * \brief Get receiver id.
   * \vprivate Component Private
   */
  auto GetId() const noexcept -> ReceiverId { return id_; }

  /*!
   * \brief Get receiver index.
   * \vprivate Component Private
   */
  auto GetIndex() const noexcept -> ReceiverIndex { return index_; }

 private:
  /*!
   * \brief The id of this ReceiverHandle.
   */
  ReceiverId id_;

  /*!
   * \brief The index of this ReceiverHandle.
   */
  ReceiverIndex index_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_HANDLE_H_

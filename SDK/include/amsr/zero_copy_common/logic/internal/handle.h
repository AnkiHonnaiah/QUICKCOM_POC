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
 *        \brief  Handle base class for any handle used within the Zero-Copy Logic.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::Handle
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_HANDLE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_HANDLE_H_

#include <cstdint>

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief The Handle class is the base for all handles used within the Zero-Copy Logic.
 * \vprivate Component Private
 */
class Handle {
 public:
  /*!
   * \brief Type for a Group.
   * \vprivate Component Private
   */
  struct Group {
    /*!
     * \brief Type for Group identifier.
     */
    using GroupIdentifier = std::uint64_t;

    /*!
     * \brief Group identifier.
     */
    GroupIdentifier identifier{};
  };

  /*!
   * \brief Check if this Handle belongs to a specific Handle group.
   * \param other_group Handle group to be checked for equality.
   * \return True if this Handle belongs to the given group, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  constexpr auto IsSameHandleGroup(Group const& other_group) const noexcept -> bool {
    return handle_group_.identifier == other_group.identifier;
  }

  /*!
   * \brief Check if this Handle belongs to the same group as the given Handle.
   * \param other_handle Handle to be checked for equality regarding Handle group.
   * \return True if this Handle belongs to the same group, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  constexpr auto IsSameHandleGroup(Handle const& other_handle) const noexcept -> bool {
    return handle_group_.identifier == other_handle.handle_group_.identifier;
  }

 protected:
  /*!
   * \brief Constructor for a Handle called by a derived class.
   * \param group Group set by the derived class.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  constexpr explicit Handle(Group const& group) noexcept : handle_group_{group} {}

  /*!
   * \brief Copy constructor for a Handle.
   * \param handle Handle to be copied.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Handle(Handle const& handle) noexcept = default;

  /*!
   * \brief Move constructor for a Handle.
   * \param handle Handle to be moved.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Handle(Handle&& handle) noexcept = default;

  /*!
   * \brief Destructor for a Handle called by a derived class.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~Handle() noexcept = default;

  /*!
   * \brief Replace the Handle contents with the copy of another.
   * \param[in] other Handle to be copied from.
   * \return Reference to Handle.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto operator=(Handle const& other) & noexcept -> Handle& = default;

  /*!
   * \brief Move another Handle to this Handle.
   * \param[in] other Handle to be moved.
   * \return Reference to Handle.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto operator=(Handle&& other) & noexcept -> Handle& = default;

 private:
  /*!
   * \brief Group of this handle.
   */
  Group handle_group_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_HANDLE_H_

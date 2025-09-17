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
 *        \brief  Definition of class ReceiverId.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_RECEIVER_ID_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_RECEIVER_ID_H_

#include <cstddef>

#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {

/*!
 * \brief    A ReceiverId representing a receiver managed by the MemConServer.
 * \details  A ReceiverId belongs to exactly one receiver registered at a specific MemConServer.
 * \vprivate Product Private
 */
class ReceiverId final : public logic::internal::Handle {
 public:
  /*!
   * \brief Type used for a group of handles.
   */
  using Group = logic::internal::Handle::Group;

  /*!
   * \brief Type used for the unique receiver identifier.
   */
  using IdType = std::size_t;

  /*!
   * \brief Type used for receiver index.
   */
  using IndexType = MaxNumberReceivers::Type;

  /*!
   * \brief       Construct a ReceiverId.
   * \param       group Group of the ReceiverId.
   * \param       id    Id of the ReceiverId.
   * \param       index Index of the ReceiverId.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  ReceiverId(Handle::Group group, IdType id, IndexType index) noexcept : Handle{group}, id_{id}, index_{index} {}

  /*!
   * \brief Destruct the ReceiverId.
   * \vprivate Product Private
   */
  ~ReceiverId() noexcept = default;

  /*!
   * \brief Construct a ReceiverId by copying.
   * \vprivate Product Private
   */
  ReceiverId(ReceiverId const&) noexcept = default;

  /*!
   * \brief Construct a ReceiverId by moving.
   * \vprivate Product Private
   */
  ReceiverId(ReceiverId&&) noexcept = default;

  /*!
   * \brief Copy another ReceiverId into this instance.
   * \vprivate Product Private
   */
  auto operator=(ReceiverId const&) & noexcept -> ReceiverId& = default;

  /*!
   * \brief Move another ReceiverId into this instance.
   * \vprivate Product Private
   */
  auto operator=(ReceiverId&&) & noexcept -> ReceiverId& = default;

  /*!
   * \brief       Determine if two ReceiverId instances are equal.
   * \details     Two ReceiverId instances are equal if the following conditions are met: same handle group, same index
   *              and same id.
   * \param       other ReceiverId to determine equality for.
   * \return      True if ReceiverId instances are equal, false otherwise.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  constexpr auto operator==(ReceiverId const& other) const noexcept -> bool {
    return IsSameHandleGroup(other) && (id_ == other.id_) && (index_ == other.index_);
  }

  /*!
   * \brief       Determine if two ReceiverId instances are unequal.
   * \details     Negated value of equal-to operator.
   * \param       other ReceiverId to determine inequality for.
   * \return      True if ReceiverId instances are unequal, false otherwise.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  auto operator!=(ReceiverId const& other) const noexcept -> bool { return !(*this == other); }

  /*!
   * \brief Get receiver id value.
   * \vprivate Component Private
   */
  constexpr auto GetId() const noexcept -> IdType { return id_; }

  /*!
   * \brief Get receiver index value.
   * \vprivate Component Private
   */
  constexpr auto GetIndex() const noexcept -> IndexType { return index_; }

 private:
  /*!
   * \brief The id of this ReceiverId.
   */
  IdType id_;

  /*!
   * \brief The index of this ReceiverId.
   */
  IndexType index_;
};

}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_RECEIVER_ID_H_

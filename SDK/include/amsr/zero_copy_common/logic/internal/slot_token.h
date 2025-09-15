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
 *        \brief  Implementation of SlotToken.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::SlotManager
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_TOKEN_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_TOKEN_H_

#include <utility>

#include "amsr/zero_copy_common/logic/internal/slot_handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A move-only SlotToken representing one SlotDescriptor managed by a SlotManager.
 * \details A SlotToken performs a similar task like a SlotHandle, however, it enforces the move-only property
 *         and is meant to be handed out as a unique access right to the data of a single slot.
 * \vprivate Component Private
 */
class SlotToken final {
 public:
  /*!
   * \brief Create a move-only SlotToken referring to the given slot.
   * \param[in] handle The SlotHandle of the slot this SlotToken should refer to.
   * \context ANY
   * \pre Must only be called by a SlotManager.
   *      This precondition can not be enforced automatically and must be fulfilled by the caller.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit SlotToken(SlotHandle handle) noexcept : handle_{handle} {};

  /*!
   * \brief Destroy this SlotToken.
   */
  ~SlotToken() = default;

  // Move-Only
  SlotToken(SlotToken const& other) noexcept = delete;

  /*!
   * \brief Move a SlotToken to a new owner.
   * \vprivate Component Private.
   */
  SlotToken(SlotToken&&) noexcept = default;

  // Move-Only
  auto operator=(SlotToken const&) noexcept -> SlotToken& = delete;

  /*!
   * \brief Move the SlotToken to a new owner.
   * \vprivate Component Private
   */
  auto operator=(SlotToken&&) & noexcept -> SlotToken& = default;

  /*!
   * \brief Get the SlotHandle this SlotToken refers to.
   * \vprivate Component Private
   */
  auto GetHandle() const noexcept -> SlotHandle { return handle_; }

 private:
  /*!
   * \brief The SlotHandle referring to the slot this move-only SlotToken refers to.
   */
  SlotHandle handle_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_TOKEN_H_

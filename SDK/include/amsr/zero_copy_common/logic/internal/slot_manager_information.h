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
 *        \brief  Implementation of the SlotManagerInformation helper class.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::SlotManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_MANAGER_INFORMATION_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_MANAGER_INFORMATION_H_

#include <type_traits>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/zero_copy_common/logic/internal/slot_token.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Data stored for each slot managed in a SlotManager instance.
 * \details For each slot a SlotDescriptor and a SlotToken is stored in the manager.
 *          The manager can hand out the SlotToken for a slot temporarily.
 *          This Class is exclusively used by the SlotManager and is part of internal, detailed unit implementation.
 * \vprivate Component Private
 */
template <typename DescriptorType>
// VCA_ZEROCOPYCOMMON_LNG_03_SLOT_MANAGER_INFORMATION_DESTRUCTOR_INVALID_OBJECT
class SlotManagerInformation {
 public:
  /*!
   * \brief Initialize the information held for a single slot in the SlotManager.
   * \param[in] slot_descriptor The SlotDescriptor to be stored. Must be valid.
   * \param[in] slot_token The SlotToken for the passed in SlotDescriptor. Must be valid for the given SlotDescriptor.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  SlotManagerInformation(std::unique_ptr<DescriptorType> slot_descriptor, SlotToken slot_token) noexcept
      : descriptor_{std::move(slot_descriptor)}, access_token_{std::move(slot_token)} {};

  /*!
   * \brief Get access to a reference of the SlotDescriptor held.
   * \return Return a reference to the SlotDescriptor stored in this SlotManagerInformation.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetDescriptor() noexcept -> DescriptorType& { return *descriptor_; };

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::SlotManagerInformation::GetDescriptor
   */
  auto GetDescriptor() const noexcept -> DescriptorType const& { return *descriptor_; };

  /*!
   * \brief Return if the manager currently holds the single SlotToken for this slot.
   * \return Return true if the SlotManagerInformation currently holds the SlotToken for this slot.
   *         Return false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsAccessTokenOwnedByManager() const noexcept -> bool { return access_token_.has_value(); }

  /*!
   * \brief Get the single SlotToken to this slot previously owned by the manager.
   * \details This will make the server no longer own the SlotToken for this slot.
   * \return The SlotToken previously owned by the manager.
   * \context ANY
   * \pre IsAccessTokenOwnedByManager() must be true, otherwise the behaviour is undefined.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetAccessToken() noexcept -> SlotToken {
    amsr::core::Optional<SlotToken> result_optional{};
    access_token_.swap(result_optional);
    return std::move(result_optional.value());
  }

  /*!
   * \brief Return the single SlotToken to this slot to be owned by the manager.
   * \param[in] token The SlotToken to be returned to the manager.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ReturnAccessToken(SlotToken token) noexcept { access_token_.template emplace(std::move(token)); }

 private:
  /*!
   * \brief The actual SlotDescriptor stored in the manager.
   */
  std::unique_ptr<DescriptorType> descriptor_;

  /*!
   * \brief The unique SlotToken available for this SlotDescriptor.
   * \details Each SlotDescriptor has exactly one SlotToken.
   *          This SlotToken is temporarily given out by the manager to users.
   */
  amsr::core::Optional<SlotToken> access_token_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SLOT_MANAGER_INFORMATION_H_

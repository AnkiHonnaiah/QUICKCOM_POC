/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *         \brief  Default access control interface implementation.
 *         \unit   ZeroCopyBinding::ZeroCopyBindingCore::AccessControl
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_DEFAULT_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_DEFAULT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/zero_copy_binding/core/internal/access_control/access_control_interface.h"

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

namespace access_control {

/*!
 * \brief Default access control interface implementation always allowing access.
 *
 * \unit ZeroCopyBinding::ZeroCopyBindingCore::AccessControl
 */
class AccessControlDefault final : public AccessControlInterface {
 public:
  /*!
   * \brief Default constructor.
   */
  AccessControlDefault() noexcept = default;

  /*!
   * \brief Default destructor.
   */
  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR
  ~AccessControlDefault() noexcept final = default;

  AccessControlDefault(AccessControlDefault const&) = delete;
  AccessControlDefault(AccessControlDefault&&) = delete;
  auto operator=(AccessControlDefault const&) -> AccessControlDefault& = delete;
  auto operator=(AccessControlDefault&&) -> AccessControlDefault& = delete;

  /*!
   * \copydoc amsr::zero_copy_binding::core::internal::access_control::AccessControlInterface::CheckAccessControlProxySubscribeEventTx
   */
  auto CheckAccessControlProxySubscribeEventTx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                               PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::zero_copy_binding::core::internal::access_control::AccessControlInterface::CheckAccessControlSkeletonSubscribeEventRx
   */
  auto CheckAccessControlSkeletonSubscribeEventRx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                  PeerId peer_id) const noexcept -> bool final;
};

}  // namespace access_control

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_DEFAULT_H_

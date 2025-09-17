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
/**        \file
 *        \brief  Default access control.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ACCESS_CONTROL_DEFAULT_ACCESS_CONTROL_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ACCESS_CONTROL_DEFAULT_ACCESS_CONTROL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace access_control {

/*!
 * \brief Default access control allowing access.
 *
 * \unit IpcBinding::IpcBindingCore::AccessControl
 */
class DefaultAccessControl final : public AccessControlInterface {
 public:
  /*!
   * \brief Initializes the access control.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  DefaultAccessControl() noexcept = default;

  /*!
   * \brief Destroy the access control.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ~DefaultAccessControl() noexcept final = default;  // VCA_IPCB_MOLE_1298

  DefaultAccessControl(DefaultAccessControl const&) noexcept = delete;
  DefaultAccessControl(DefaultAccessControl&&) noexcept = delete;
  auto operator=(DefaultAccessControl const&) noexcept -> DefaultAccessControl& = delete;
  auto operator=(DefaultAccessControl&&) noexcept -> DefaultAccessControl& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxyMethodRequestTx
   */
  auto CheckAccessControlProxyMethodRequestTx(ServiceId service_id, InstanceId instance_id, MethodId method_id,
                                              PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxyMethodResponseRx
   */
  auto CheckAccessControlProxyMethodResponseRx(ServiceId service_id, InstanceId instance_id, MethodId method_id,
                                               PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxyMethodErrorResponseRx
   */
  auto CheckAccessControlProxyMethodErrorResponseRx(ServiceId service_id, InstanceId instance_id, MethodId method_id,
                                                    PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxyMethodAppErrorResponseRx
   */
  auto CheckAccessControlProxyMethodAppErrorResponseRx(ServiceId service_id, InstanceId instance_id, MethodId method_id,
                                                       PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxyMethodRequestNoReturnTx
   */
  auto CheckAccessControlProxyMethodRequestNoReturnTx(ServiceId service_id, InstanceId instance_id, MethodId method_id,
                                                      PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxySubscribeEventTx
   */
  auto CheckAccessControlProxySubscribeEventTx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                               PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxySubscribeEventAckRx
   */
  auto CheckAccessControlProxySubscribeEventAckRx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                  PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxySubscribeEventNAckRx
   */
  auto CheckAccessControlProxySubscribeEventNAckRx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                   PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxyUnsubscribeEventTx
   */
  auto CheckAccessControlProxyUnsubscribeEventTx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                 PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlProxyNotificationRx
   */
  auto CheckAccessControlProxyNotificationRx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                             PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlSkeletonMethodRequestRx
   */
  auto CheckAccessControlSkeletonMethodRequestRx(ServiceId service_id, InstanceId instance_id, MethodId method_id,
                                                 PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlSkeletonMethodRequestNoReturnRx
   */
  auto CheckAccessControlSkeletonMethodRequestNoReturnRx(ServiceId service_id, InstanceId instance_id,
                                                         MethodId method_id, PeerId peer_id) const noexcept
      -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlSkeletonSubscribeEventRx
   */
  auto CheckAccessControlSkeletonSubscribeEventRx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                  PeerId peer_id) const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::access_control::AccessControlInterface::CheckAccessControlSkeletonUnsubscribeEventRx
   */
  auto CheckAccessControlSkeletonUnsubscribeEventRx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                    PeerId peer_id) const noexcept -> bool final;
};

}  // namespace access_control
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ACCESS_CONTROL_DEFAULT_ACCESS_CONTROL_H_

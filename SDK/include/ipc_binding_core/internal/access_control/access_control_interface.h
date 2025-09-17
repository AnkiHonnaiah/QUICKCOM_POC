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
 *        \brief  Abstract access control interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/credentials.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace access_control {

/*!
 * \brief Access control interface to check access control on transmission/reception of ipc messages.
 *
 * \unit IpcBinding::IpcBindingCore::AccessControl
 */
class AccessControlInterface {
 public:
  /*!
   * \brief Alias for IPC service ID.
   */
  using ServiceId = amsr::ipc_binding_core::internal::ipc_protocol::ServiceId;

  /*!
   * \brief Alias for IPC service instance ID.
   */
  using InstanceId = amsr::ipc_binding_core::internal::ipc_protocol::InstanceId;

  /*!
   * \brief Alias for IPC method ID.
   */
  using MethodId = amsr::ipc_binding_core::internal::ipc_protocol::MethodId;

  /*!
   * \brief Alias for IPC event ID.
   */
  using EventId = amsr::ipc_binding_core::internal::ipc_protocol::EventId;

  /*!
   * \brief Alias for peer identity.
   */
  using PeerId = amsr::ipc::Credentials;

  /*!
   * \brief Constructor.
   *
   * \context App
   * \steady  FALSE
   */
  AccessControlInterface() noexcept = default;

  /*!
   * \brief Destructor.
   *
   * \context App
   * \steady  FALSE
   */
  virtual ~AccessControlInterface() noexcept = default;

  AccessControlInterface(AccessControlInterface const&) noexcept = delete;
  auto operator=(AccessControlInterface const&) noexcept -> AccessControlInterface& = delete;
  AccessControlInterface(AccessControlInterface&&) noexcept = delete;
  auto operator=(AccessControlInterface&&) noexcept -> AccessControlInterface& = delete;

  /*!
   * \brief Check access control at proxy side for the method request transmission to the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] method_id    IPC method ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is granted, False If access control is denied.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto CheckAccessControlProxyMethodRequestTx(ServiceId service_id, InstanceId instance_id, MethodId method_id,
                                                      PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the method response reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] method_id    IPC method ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto CheckAccessControlProxyMethodResponseRx(ServiceId service_id, InstanceId instance_id, MethodId method_id,
                                                       PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the method error response reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] method_id    IPC method ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto CheckAccessControlProxyMethodErrorResponseRx(ServiceId service_id, InstanceId instance_id,
                                                            MethodId method_id, PeerId peer_id) const noexcept
      -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the method application error response reception from the peer
   *        partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] method_id    IPC method ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto CheckAccessControlProxyMethodAppErrorResponseRx(ServiceId service_id, InstanceId instance_id,
                                                               MethodId method_id, PeerId peer_id) const noexcept
      -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the method request with no return transmission to the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] method_id    IPC method ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto CheckAccessControlProxyMethodRequestNoReturnTx(ServiceId service_id, InstanceId instance_id,
                                                              MethodId method_id, PeerId peer_id) const noexcept
      -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the event subscribe transmission to the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] event_id     IPC event ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto CheckAccessControlProxySubscribeEventTx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                       PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the event subscribe Ack reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] event_id     IPC event ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto CheckAccessControlProxySubscribeEventAckRx(ServiceId service_id, InstanceId instance_id,
                                                          EventId event_id, PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the event subscribe NAck reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] event_id     IPC event ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto CheckAccessControlProxySubscribeEventNAckRx(ServiceId service_id, InstanceId instance_id,
                                                           EventId event_id, PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the event unsubscribe transmission to the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] event_id     IPC event ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto CheckAccessControlProxyUnsubscribeEventTx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                         PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at proxy side for the event notification reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] event_id     IPC event ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto CheckAccessControlProxyNotificationRx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                     PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at skeleton side for the method request reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] method_id    IPC method ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto CheckAccessControlSkeletonMethodRequestRx(ServiceId service_id, InstanceId instance_id,
                                                         MethodId method_id, PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at skeleton side for the method request no return reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] method_id    IPC method ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto CheckAccessControlSkeletonMethodRequestNoReturnRx(ServiceId service_id, InstanceId instance_id,
                                                                 MethodId method_id, PeerId peer_id) const noexcept
      -> bool = 0;

  /*!
   * \brief Check access control at skeleton side for the event subsribe reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] event_id     IPC event ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto CheckAccessControlSkeletonSubscribeEventRx(ServiceId service_id, InstanceId instance_id,
                                                          EventId event_id, PeerId peer_id) const noexcept -> bool = 0;

  /*!
   * \brief Check access control at skeleton side for the event unsubscribe reception from the peer partner.
   *
   * \param[in] service_id   IPC service ID.
   * \param[in] instance_id  IPC service instance ID.
   * \param[in] event_id     IPC event ID.
   * \param[in] peer_id      Peer identity of the partner.
   *
   * \return True If access control is 'Granted', False If access control is 'Denied'.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto CheckAccessControlSkeletonUnsubscribeEventRx(ServiceId service_id, InstanceId instance_id,
                                                            EventId event_id, PeerId peer_id) const noexcept
      -> bool = 0;
};

}  // namespace access_control
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_INTERFACE_H_

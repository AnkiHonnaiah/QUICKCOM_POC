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
 *         \brief  Provides the access control interface which can be implemented by the user.
 *         \unit   ZeroCopyBinding::ZeroCopyBindingCore::AccessControl
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_INTERFACE_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/credentials.h"
#include "amsr/zero_copy_binding/common/internal/config/types.h"

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

namespace access_control {

/*!
 * \brief Access control interface to check access control on transmission/reception of ipc messages.
 *
 * \unit ZeroCopyBinding::ZeroCopyBindingCore::AccessControl
 */
class AccessControlInterface {
 public:
  /*!
   * \brief Alias for ZeroCopyBinding service ID.
   */
  using ServiceId = ::amsr::zero_copy_binding::common::internal::config::ServiceId;

  /*!
   * \brief Alias for ZeroCopyBinding service instance ID.
   */
  using InstanceId = ::amsr::zero_copy_binding::common::internal::config::InstanceId;

  /*!
   * \brief Alias for ZeroCopyBinding event ID.
   */
  using EventId = ::amsr::zero_copy_binding::common::internal::config::EventId;

  /*!
   * \brief Alias for peer identity.
   */
  using PeerId = ::amsr::ipc::Credentials;

  /*!
   * \brief Default constructor.
   */
  AccessControlInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   */
  virtual ~AccessControlInterface() noexcept = default;

  AccessControlInterface(AccessControlInterface const&) = delete;
  auto operator=(AccessControlInterface const&) -> AccessControlInterface& = delete;
  AccessControlInterface(AccessControlInterface&&) = delete;
  auto operator=(AccessControlInterface&&) -> AccessControlInterface& = delete;

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
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \context       Reactor
   */
  virtual auto CheckAccessControlProxySubscribeEventTx(ServiceId service_id, InstanceId instance_id, EventId event_id,
                                                       PeerId peer_id) const noexcept -> bool = 0;

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
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \context       Reactor
   */
  virtual auto CheckAccessControlSkeletonSubscribeEventRx(ServiceId service_id, InstanceId instance_id,
                                                          EventId event_id, PeerId peer_id) const noexcept -> bool = 0;
};

}  // namespace access_control

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCESS_CONTROL_ACCESS_CONTROL_INTERFACE_H_

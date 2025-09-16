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
 *        \brief  Defines an interface for proxy event backends.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_EVENT_BACKEND_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_EVENT_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/message.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Interface for proxy event backends.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ProxyEventBackendInterface {
 public:
  /*!
   * \brief Constructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyEventBackendInterface() noexcept = default;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual ~ProxyEventBackendInterface() noexcept = default;

  ProxyEventBackendInterface(ProxyEventBackendInterface const&) noexcept = delete;
  ProxyEventBackendInterface(ProxyEventBackendInterface&&) noexcept = delete;
  auto operator=(ProxyEventBackendInterface const&) noexcept -> ProxyEventBackendInterface& = delete;
  auto operator=(ProxyEventBackendInterface&&) noexcept -> ProxyEventBackendInterface& = delete;

  /*!
   * \brief Set the connection state.
   *
   * \param[in] connection_state  Boolean indicating new connection state (connected or disconnected).
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto SetConnectionState(bool connection_state) noexcept -> void = 0;

  /*!
   * \brief Set the service instance state.
   *
   * \param[in] service_state  Boolean indicating new service state (ServiceUp or ServiceDown).
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto SetServiceState(bool service_state) noexcept -> void = 0;

  /*!
   * \brief Forwards the notification message to its destination.
   *
   * \param[in] notification  The notification message to pass to the realized sink.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto OnNotificationReceived(ipc_protocol::NotificationMessage&& notification) noexcept -> void = 0;

  /*!
   * \brief Forwards a subscription ACK from the the skeleton to the proxy.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto OnSubscribeAckReceived() noexcept -> void = 0;

  /*!
   * \brief Forwards an subscription NACK message to a provided service instance.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto OnSubscribeNAckReceived() noexcept -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_EVENT_BACKEND_INTERFACE_H_

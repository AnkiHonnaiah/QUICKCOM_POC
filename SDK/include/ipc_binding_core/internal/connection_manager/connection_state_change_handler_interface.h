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
 *        \brief  ConnectionStateChangeHandlerInterface used as a registered listener for connection state changes.
 *
 *      \details  Called from the reactor thread.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_STATE_CHANGE_HANDLER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_STATE_CHANGE_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/shared_ptr.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

class ProxyRouterConnectorInterface;

namespace connection_manager {

/*!
 * \brief Interface used as a registered listener for connection state changes.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionManagerProxy
 */
class ConnectionStateChangeHandlerInterface {
 public:
  /*!
   * \brief Type alias for the ProxyRouterConnector interface.
   */
  using ProxyRouterConnectorInterface = ::amsr::ipc_binding_core::internal::ProxyRouterConnectorInterface;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context Shutdown
   * \steady  FALSE
   */
  virtual ~ConnectionStateChangeHandlerInterface() noexcept = default;

  ConnectionStateChangeHandlerInterface(ConnectionStateChangeHandlerInterface const&) noexcept = delete;
  ConnectionStateChangeHandlerInterface(ConnectionStateChangeHandlerInterface&&) noexcept = delete;
  auto operator=(ConnectionStateChangeHandlerInterface const&) noexcept
      -> ConnectionStateChangeHandlerInterface& = delete;
  auto operator=(ConnectionStateChangeHandlerInterface&&) noexcept -> ConnectionStateChangeHandlerInterface& = delete;

  /*!
   * \brief Indicates that a connection to the remote server has been established.
   *
   * \param[in] router_connector  ProxyRouterConnector corresponding to the current ConnectionProxy.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto OnConnected(amsr::WeakPtr<ProxyRouterConnectorInterface> const& router_connector) noexcept -> void = 0;

  /*!
   * \brief Indicates that a connection to a remote server got disconnected.
   *
   * \param[in] disconnect_reason  Disconnect reason informed by ConnectionProxy.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto OnDisconnected(amsr::core::Result<void> const disconnect_reason) noexcept -> void = 0;

 protected:
  /*!
   * \brief Default construct an interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionStateChangeHandlerInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_STATE_CHANGE_HANDLER_INTERFACE_H_

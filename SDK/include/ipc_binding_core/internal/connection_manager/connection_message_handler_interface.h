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
 *        \brief  Interface of Connection Handler for SafeIpc Connections
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MESSAGE_HANDLER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MESSAGE_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/ipc_packet.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Handler for transmission and reception of packets via SafeIpc connection.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionMessageHandler
 */
class ConnectionMessageHandlerInterface {
 public:
  /*!
   * \brief Type alias for IpcPacketShared.
   */
  using IpcPacketShared = ipc_protocol::IpcPacketShared;

  ConnectionMessageHandlerInterface(ConnectionMessageHandlerInterface const&) noexcept = delete;
  auto operator=(ConnectionMessageHandlerInterface const&) noexcept -> ConnectionMessageHandlerInterface& = delete;
  ConnectionMessageHandlerInterface(ConnectionMessageHandlerInterface const&&) noexcept = delete;
  auto operator=(ConnectionMessageHandlerInterface const&&) noexcept -> ConnectionMessageHandlerInterface& = delete;

  /*!
   * \brief Destroy the ConnectionMessageHandler.
   *
   * \pre         -
   * \context     ANY | !Reactor (must not be called from callback functions provided to BasicIPC on this connection).
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~ConnectionMessageHandlerInterface() noexcept = default;

  /*!
   * \brief   Reset states of ConnectionMessageHandler back to 'Idle'.
   * \details Might be used after the underlying SafeIpc connection was re-connected after an error.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto Reset() noexcept -> void = 0;

  /*!
   * \brief   Send a packet.
   * \details There is not guarantee given that the packet is actually sent when the API returns. In case the SafeIpc
   *          connection is busy the packets will be processed asynchronous (via reactor) or enqueued for later
   *          processing.
   *
   * \param[in] packet  Packet to be sent.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto Send(IpcPacketShared&& packet) noexcept -> void = 0;

  /*!
   * \brief   Add a packet to the send queue.
   * \details This does not trigger the send operation for the packet. But the enqueued packet will be processed by the
   *          next call to Send(), the asynchronous processing (reactor) or SendQueued().
   *
   * \param[in] packet  Packet to be enqueued.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto AddToSendQueue(IpcPacketShared&& packet) noexcept -> void = 0;

  /*!
   * \brief Send all enqueued packets.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE (TRUE in case immediate transmission possible via SafeIpc connection).
   * \steady      TRUE
   */
  virtual auto SendQueued() noexcept -> void = 0;

  /*!
   * \brief   Start reception of packets.
   * \details Packet reception will be handled asynchronously in the reactor context.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto StartReception() noexcept -> void = 0;

 protected:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionMessageHandlerInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MESSAGE_HANDLER_INTERFACE_H_

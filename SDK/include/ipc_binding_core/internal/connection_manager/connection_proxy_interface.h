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
 *        \brief  A connection established by a client to a server as seen by the client.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/connection_manager/connection_state.h"
#include "ipc_binding_core/internal/connection_manager/connection_state_change_handler_interface.h"
#include "ipc_binding_core/internal/connection_manager/transmit_message_handler_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Interface for proxy-side BasicIPC communication connection.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ConnectionProxyInterface {
 public:
  /*!
   * \brief The type alias for a remote IPC unicast address.
   */
  using IpcUnicastAddress = ipc_protocol::IpcUnicastAddress;

  /*!
   * \brief Type alias for the ConnectionStateChangeHandlerInterface pointer.
   */
  using StateChangeHandlerInterfacePtr = amsr::WeakPtr<ConnectionStateChangeHandlerInterface>;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ConnectionProxyInterface() noexcept = default;

  ConnectionProxyInterface(ConnectionProxyInterface const&) noexcept = delete;
  auto operator=(ConnectionProxyInterface const&) noexcept -> ConnectionProxyInterface& = delete;
  ConnectionProxyInterface(ConnectionProxyInterface const&&) noexcept = delete;
  auto operator=(ConnectionProxyInterface const&&) noexcept -> ConnectionProxyInterface& = delete;

  /*!
   * \brief Returns the remote IPC unicast address of the server.
   *
   * \return The remote IPC unicast address of the server.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto GetAddress() const noexcept -> IpcUnicastAddress const& = 0;

  /*!
   * \brief Get the current connection state.
   *
   * \return Current connection state.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto GetConnectionState() noexcept -> ConnectionState = 0;

  /*!
   * \brief   Initiates the establishment of the connection to the remote IPC server.
   * \details Registers a state change handler.
   *
   * \param[in]     provided_service_instance  The provided service instance for which a connection has been requested.
   * \param[in,out] state_change_handler       The state change handler to be connected.
   *
   * \return Connection state.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  virtual auto Connect(ProvidedServiceInstanceId provided_service_instance,
                       StateChangeHandlerInterfacePtr state_change_handler) noexcept -> ConnectionState = 0;

  /*!
   * \brief Deregisters state change handler. Initiates the termination of the connection to the remote IPC server upon
   *        unregistering of all the handlers.
   *
   * \param[in] provided_service_instance  The ID of the provided service instance to be disconnected.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto Disconnect(ProvidedServiceInstanceId provided_service_instance) noexcept -> void = 0;

  /*!
   * \brief Send a packet.
   *
   * \param[in] packet  An IPC packet to be forwarded.
   *
   * \return True if the packet has been enqueued and scheduled for sending. False if the packet cannot be transmitted.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  virtual auto Send(ipc_protocol::IpcPacketShared&& packet) noexcept -> bool = 0;

  /*!
   * \brief Getter for transmit_message_handler.
   *
   * \return The underlying transmit_message_handler.
   *
   * \context   Reactor
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual auto GetTransmitMessageHandler() noexcept -> TransmitMessageHandlerInterface& = 0;

 protected:
  /*!
   * \brief Default constructor.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  ConnectionProxyInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_INTERFACE_H_

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
 *        \brief  A connection established by a client to a server as seen by the server.
 *         \unit IpcBinding::IpcBindingCore::ConnectionSkeleton
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/ipc_protocol/message.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Type definition for connection id.
 */
using ConnectionId = std::uint32_t;

/*!
 * \brief   Encapsulates BasicIPC communication services for IPC ConnectionSkeleton.
 * \details A ConnectionSkeleton is established by a client to a server as seen by the server.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionSkeleton
 */
class ConnectionSkeletonInterface {
 public:
  /*!
   * \brief Disconnects from the generic connection and cleans up all the used resources.
   *
   * \pre       The reactor shall not be using any callback provided to BasicIPC on this connection.
   * \context   ANY | !Reactor (must not be called from callback functions provided to BasicIPC on this connection).
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~ConnectionSkeletonInterface() noexcept = default;

  ConnectionSkeletonInterface(ConnectionSkeletonInterface const&) noexcept = delete;
  ConnectionSkeletonInterface(ConnectionSkeletonInterface const&&) noexcept = delete;
  auto operator=(ConnectionSkeletonInterface const&) noexcept -> ConnectionSkeletonInterface& = delete;
  auto operator=(ConnectionSkeletonInterface const&&) noexcept -> ConnectionSkeletonInterface& = delete;

  /*!
   * \brief Start asynchronous reception of IPC messages.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  virtual auto ReceiveAsync() noexcept -> void = 0;

  /*!
   * \brief Send a method response message to the connected proxy.
   *
   * \param[in] response  Response message to be sent.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      TRUE
   */
  virtual auto SendResponse(ipc_protocol::ResponseMessage&& response) noexcept -> void = 0;

  /*!
   * \brief Send an error response message to the connected proxy.
   *
   * \param[in] error_response  ErrorResponse message to be sent.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      TRUE
   */
  virtual auto SendErrorResponse(ipc_protocol::ErrorResponseMessage&& error_response) noexcept -> void = 0;

  /*!
   * \brief Send an ApplicationError message to the connected proxy.
   *
   * \param[in] message  ApplicationError message to be sent.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      TRUE
   */
  virtual auto SendApplicationError(ipc_protocol::ApplicationErrorMessage&& message) noexcept -> void = 0;

  /*!
   * \brief Send a notification message to the connected proxy.
   *
   * \param[in] notification  Notification message to be sent.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      TRUE
   */
  virtual auto SendNotification(ipc_protocol::NotificationMessage&& notification) noexcept -> void = 0;

  /*!
   * \brief Send an event subscription Ack message to the connected proxy.
   *
   * \param[in] message  SubscribeEventAck message to be sent.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      FALSE
   */
  virtual auto SendSubscribeAck(ipc_protocol::SubscribeEventAckMessage&& message) noexcept -> void = 0;

  /*!
   * \brief Send an event subscription NAck message to the connected proxy.
   *
   * \param[in] message  SubscribeEventNAck message to be sent.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE (if SafeIpc connection is idle and packet fits into remaining buffer).
   * \steady      FALSE
   */
  virtual auto SendSubscribeNAck(ipc_protocol::SubscribeEventNAckMessage&& message) noexcept -> void = 0;

  /*!
   * \brief Returns the ID of the connection.
   *
   * \return The ConnectionId.
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
  virtual auto GetId() const noexcept -> ConnectionId = 0;

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
  ConnectionSkeletonInterface() noexcept = default;
};

}  // namespace connection_manager

/*!
 * \brief Type definition for pointer to ConnectionSkeleton.
 */
using ConnectionSkeletonPtr = amsr::SharedPtr<connection_manager::ConnectionSkeletonInterface>;

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_INTERFACE_H_

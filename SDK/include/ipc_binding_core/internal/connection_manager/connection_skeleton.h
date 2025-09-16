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
 *        \brief  Connection for communication from skeleton to proxy.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>

#include "amsr/shared_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_message_handler.h"
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_message_handler.h"
#include "ipc_binding_core/internal/connection_manager/generic_connection.h"
#include "ipc_binding_core/internal/connection_manager/server_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Encapsulates SafeIpc communication services for IPC ConnectionSkeleton.
 * \details A ConnectionSkeleton is established by a client to a server as seen by the server.
 *
 * \unit       IpcBinding::IpcBindingCore::ConnectionSkeleton
 * \complexity Many calls to 'IpcProtocol' and 'SharedRefPointer' units.
 */
class ConnectionSkeleton final : public ConnectionSkeletonInterface,
                                 public amsr::EnableSharedFromThis<ConnectionSkeleton> {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Constructs a ConnectionSkeleton and returns a shared pointer to its interface.
   *
   * \param[in,out] reactor          Used Reactor.
   * \param[in,out] server           The server which accepted IPC connection. The object must be valid for
   *                                 ConnectionSkeleton's entire life time.
   * \param[in,out] skeleton_router  The skeleton router that shall be used by ConnectionSkeleton. The object must be
   *                                 valid for this object's entire life time.
   * \param[in,out] connection       Unique pointer to a valid accepted IPC connection.
   * \param[in,out] trace_monitor    The trace monitor. The object must be valid for ConnectionSkeleton's entire life
   *                                 time.
   * \param[in]     access_control   Reference to an access control implementation. The object must be valid for the
   *                                 entire lifetime of this object.
   *
   * \return Shared pointer to a ConnectionSkeletonInterface.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto Create(ReactorInterface& reactor, ServerInterface& server, SkeletonRouterInterface& skeleton_router,
                     amsr::UniquePtr<GenericConnection>&& connection, trace::TraceMonitor const& trace_monitor,
                     access_control::AccessControlInterface const& access_control) noexcept
      -> amsr::SharedPtr<ConnectionSkeletonInterface>;

  ConnectionSkeleton() noexcept = delete;

  /*!
   * \brief Disconnects from the SafeIpc connection and cleans up all the used resources.
   *
   * \pre       The reactor shall not be using any callback provided to SafeIpc on this connection.
   * \context   Reactor (must not be called from callback functions provided to SafeIpc on this connection).
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~ConnectionSkeleton() noexcept final;

  ConnectionSkeleton(ConnectionSkeleton const&) noexcept = delete;
  auto operator=(ConnectionSkeleton const&) noexcept -> ConnectionSkeleton& = delete;
  ConnectionSkeleton(ConnectionSkeleton const&&) noexcept = delete;
  auto operator=(ConnectionSkeleton const&&) noexcept -> ConnectionSkeleton& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface::ReceiveAsync
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto ReceiveAsync() noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface::SendResponse
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto SendResponse(ipc_protocol::ResponseMessage&& response) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface::SendErrorResponse
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto SendErrorResponse(ipc_protocol::ErrorResponseMessage&& error_response) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface::SendApplicationError
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto SendApplicationError(ipc_protocol::ApplicationErrorMessage&& message) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface::SendNotification
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto SendNotification(ipc_protocol::NotificationMessage&& notification) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface::SendSubscribeAck
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto SendSubscribeAck(ipc_protocol::SubscribeEventAckMessage&& message) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface::SendSubscribeNAck
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto SendSubscribeNAck(ipc_protocol::SubscribeEventNAckMessage&& message) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionSkeletonInterface::GetId
   */
  auto GetId() const noexcept -> ConnectionId final;

 private:
  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for Reactor callback handle.
   */
  using ReactorCallbackHandle = osabstraction::io::reactor1::CallbackHandle;

  /*!
   * \brief Type alias for Reactor event types.
   */
  using ReactorEventTypes = ::osabstraction::io::reactor1::EventTypes;

  /*!
   * \brief Initializes all the resources of ConnectionSkeleton.
   *
   * \param[in,out] reactor          Used Reactor.
   * \param[in,out] server           The server which accepted IPC connection. The object must be valid for
   *                                 ConnectionSkeleton's entire life time.
   * \param[in,out] skeleton_router  The skeleton router that shall be used by ConnectionSkeleton. The object must be
   *                                 valid for this object's entire life time.
   * \param[in,out] connection       The accepted IPC connection.
   * \param[in,out] trace_monitor    The trace monitor. The object must be valid for ConnectionSkeleton's entire life
   *                                 time.
   * \param[in]     access_control   Reference to an access control implementation. The object must be valid for the
   *                                 entire lifetime of this object.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  ConnectionSkeleton(ReactorInterface& reactor, ServerInterface& server, SkeletonRouterInterface& skeleton_router,
                     amsr::UniquePtr<GenericConnection>&& connection, trace::TraceMonitor const& trace_monitor,
                     access_control::AccessControlInterface const& access_control) noexcept;

  /*!
   * \brief Build the used logger incl. a prefix visualizing the unicast address.
   *
   * \param[in] address  Unicast address.
   *
   * \return Constructed logger.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto BuildLogger(ipc_protocol::IpcUnicastAddress const& address) noexcept -> Logger;

  /*!
   * \brief Creates a unique connection skeleton ID.
   *
   * \return The created connection skeleton ID.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  static auto CreateNextId() noexcept -> ConnectionId;

  // ---- Connection State ----------------------------------------------------

  /*!
   * \brief Indicates whether the connection is still established.
   *
   * \return True if the connection is still established, false otherwise.
   *
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto IsConnected() const noexcept -> bool;

  /*!
   * \brief Initiates the termination of the connection to the remote IPC client.
   *
   * \pre       -
   * \context   ANY (but the calling thread must be synchronized with Reactor thread).
   * \reentrant FALSE
   * \steady    FALSE
   */
  void Disconnect() const noexcept;

  // ---- Error Handling ------------------------------------------------------

  /*!
   * \brief Handle an occurred SafeIpcError deferred via reactor thread.
   *
   * \param[in] error     Describes the occurred error condition.
   * \param[in] location  Code location where the error occurred.
   *
   * \pre       -
   * \context   App|Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  void HandleIpcErrorAndTerminateDeferred(amsr::core::ErrorCode const& error,
                                          Logger::LogLocation const& location) noexcept;

  /*!
   * \brief Notify Server that this connection instance shall be destroyed.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  void TriggerDestruction() noexcept;

  /*!
   * \brief Next connection skeleton ID.
   */
  static std::atomic<ConnectionId> next_id_;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  /*!
   * \brief Reference to the reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief Handle of Reactor software event for processing of SafeIpc errors.
   */
  ReactorCallbackHandle reactor_handle_safeipc_error_{};

  /*!
   * \brief A reference to the IPC Server.
   */
  ServerInterface& server_;

  /*!
   * \brief Our logger.
   */
  Logger logger_;

  /*!
   * \brief Decodes and dispatches incoming packets.
   */
  ConnectionSkeletonMessageHandler message_dispatcher_;

  /*!
   * \brief The IPC connection.
   */
  amsr::UniquePtr<GenericConnection> const connection_;

  /*!
   * \brief Handler for message transmission / reception via SafeIpc.
   */
  ConnectionMessageHandler message_handler_;

  /*!
   * \brief Reference to the trace monitor.
   */
  trace::TraceMonitor const& trace_monitor_;

  /*!
   * \brief Peer id of the connected partner.
   */
  amsr::ipc::Credentials peer_id_;

  /*!
   * \brief   Connection skeleton ID.
   * \details For each created connection skeleton we create a new local unique ID. This is used to distinguish the
   *          connection skeletons in the subscribers map of the event xfs. The IDs are used to avoid a comparison of
   *          the pointer address managed by the weak pointer.
   */
  ConnectionId const connection_id_;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_SKELETON_H_

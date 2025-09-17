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
 *        \brief  Connection for communication from proxy to skeleton.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>

#include "amsr/ipc/credentials.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/shared_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_message_handler.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_connection.h"
#include "ipc_binding_core/internal/connection_manager/proxy_router_connector_interface.h"
#include "ipc_binding_core/internal/connection_manager/receive_message_handler.h"
#include "ipc_binding_core/internal/connection_manager/transmit_message_handler.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Connection for communication from proxy to skeleton.
 * \details Abstraction of osabstraction BasicIPC communication services.
 *
 * \unit       IpcBinding::IpcBindingCore::ConnectionProxy
 * \complexity Many calls to 'IpcProtocol' and 'SharedRefPointer' units.
 */
class ConnectionProxy final : public ConnectionProxyInterface, public amsr::EnableSharedFromThis<ConnectionProxy> {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief The type alias for Service Instance Integrity Level.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Initializes all the resources of ConnectionProxy.
   *
   * \param[in,out] reactor                   Used Reactor.
   * \param[in,out] connection_manager_proxy  The connection manager proxy.The object must be valid for
   *                                          ConnectionProxy's entire life time.
   * \param[in,out] router_connector          The router connector. The object must be valid for ConnectionProxy's
   *                                          entire life time.
   * \param[in,out] connection                The connection. The object must be valid for ConnectionProxy's entire life
   *                                          time.
   * \param[in]     address                   The remote IPC unicast address of an IPC server to which this connection
   *                                          is meant to be established.
   * \param[in]     trace_monitor             The trace monitor.
   * \param[in]     access_control            Reference to an access control implementation. The object must be valid
   *                                          for the entire lifetime of this object.
   * \param[in]     integrity_level           The expected IntegrityLevel for this connection.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  ConnectionProxy(ReactorInterface& reactor, ConnectionManagerProxyInterface& connection_manager_proxy,
                  amsr::SharedPtr<ProxyRouterConnectorInterface> const& router_connector,
                  amsr::UniquePtr<GenericConnection>&& connection, IpcUnicastAddress const& address,
                  trace::TraceMonitor const& trace_monitor,
                  access_control::AccessControlInterface const& access_control,
                  IntegrityLevel integrity_level) noexcept;

  /*!
   * \brief Disconnect from IPC connection.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  ~ConnectionProxy() noexcept final;

  ConnectionProxy(ConnectionProxy const&) noexcept = delete;
  auto operator=(ConnectionProxy const&) noexcept -> ConnectionProxy& = delete;
  ConnectionProxy(ConnectionProxy const&&) noexcept = delete;
  auto operator=(ConnectionProxy const&&) noexcept -> ConnectionProxy& = delete;

  /*!
   * \brief Construct a connection for communication from proxy to skeleton.
   *
   * \param[in,out] reactor                   Used Reactor.
   * \param[in,out] connection_manager_proxy  The connection manager proxy. The object must be valid for
   *                                          ConnectionProxy's entire life time.
   * \param[in,out] router_connector          The router connector. The object must be valid for ConnectionProxy's
   *                                          entire life time.
   * \param[in,out] connection                The connection. The object must be valid for ConnectionProxy's entire life
   *                                          time.
   * \param[in]     address                   The remote IPC unicast address of an IPC server to which this connection
   *                                          is meant to be established.
   * \param[in]     trace_monitor             The trace monitor.
   * \param[in]     access_control            Reference to an access control implementation. The object must be valid
   *                                          for the entire lifetime of this object.
   * \param[in]     integrity_level           The expected IntegrityLevel for this connection.
   *
   * \return Shared pointer to a ConnectionProxy interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto Create(ReactorInterface& reactor, ConnectionManagerProxyInterface& connection_manager_proxy,
                     amsr::SharedPtr<ProxyRouterConnectorInterface> const& router_connector,
                     amsr::UniquePtr<GenericConnection>&& connection, IpcUnicastAddress const& address,
                     trace::TraceMonitor const& trace_monitor,
                     access_control::AccessControlInterface const& access_control,
                     IntegrityLevel integrity_level) noexcept -> amsr::SharedPtr<ConnectionProxyInterface>;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface::GetAddress
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetAddress() const noexcept -> IpcUnicastAddress const& final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface::GetConnectionState
   *
   * \spec requires true; \endspec
   */
  auto GetConnectionState() noexcept -> ConnectionState final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface::GetTransmitMessageHandler
   */
  auto GetTransmitMessageHandler() noexcept -> TransmitMessageHandlerInterface& final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface::Connect
   */
  auto Connect(ProvidedServiceInstanceId provided_service_instance,
               StateChangeHandlerInterfacePtr state_change_handler) noexcept -> ConnectionState final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface::Disconnect
   *
   * \spec requires true; \endspec
   */
  auto Disconnect(ProvidedServiceInstanceId provided_service_instance) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyInterface::Send
   *
   * \spec requires true; \endspec
   */
  auto Send(ipc_protocol::IpcPacketShared&& packet) noexcept -> bool final;

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
   * \brief The container type for the registered state change handlers.
   */
  using StateChangeHandlerContainer = ara::core::Map<ProvidedServiceInstanceId, StateChangeHandlerInterfacePtr>;

  /*!
   * \brief Build the used logger incl. a prefix visualizing the unicast address.
   *
   * \param[in] address  Unicast address.
   *
   * \return Constructed logger.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto BuildLogger(IpcUnicastAddress const& address) noexcept -> Logger;

  // ---- Connect / Disconnect ------------------------------------------------

  /*!
   * \brief Initiates the establishment of the connection to the remote IPC server.
   *
   * \pre         Only allowed to call when the connection is disconnected.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  auto Connect() noexcept -> void;

  /*!
   * \brief Initiates the termination of the connection to the remote IPC server.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto CloseIpcConnection() noexcept -> void;

  /*!
   * \brief   Finishes an asynchronous connection establishment.
   * \details Called by the ConnectCompletionCallback to notify about a finished connection establishment. The final
   *          result may be connected or disconnected.
   *
   * \param[in] result  The asynchronous connection establishment result.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnConnectFinished(amsr::core::Result<void> const& result) noexcept -> void;

  /*!
   * \brief Notify state change handlers about the current connection state.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto NotifyStateChangeHandlers() noexcept -> void;

  /*!
   * \brief Notify state change handlers the connection status is "connected".
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto NotifyStateChangeHandlersOnConnected() noexcept -> void;

  /*!
   * \brief Notify state change handlers the connection status is "Disconnected".
   *
   * \param[in] disconnect_reason  An additional information to the remote server. During autoupdate use case, the
   *                               parameter is required by remote server to try auto reconnection or not due to delayed
   *                               execution of TriggerDestruction. The disconnect reason parameter can take 2 values as
   *                               below.
   *                               - IpcBindingErrc::communication_failure: The error can occur when message
   *                                 transmission/reception fails. The remote server tries to auto reconnect.
   *                               - Empty result: The empty results is used when connection establishement
   *                                 passes/fails. The remote server shall not retry to auto reconnect.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto NotifyStateChangeHandlersOnDisconnected(amsr::core::Result<void> const disconnect_reason) noexcept -> void;

  // ---- Error Handling ------------------------------------------------------

  /*!
   * \brief Handle an occurred SafeIpc error.
   *
   * \param[in] error     Describes the occurred error condition.
   * \param[in] location  Code location where the error occurred.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto HandleIpcErrorAndTerminateDeferred(amsr::core::ErrorCode const& error,
                                          Logger::LogLocation const& location) noexcept -> void;

  /*!
   * \brief Deferred processing of an SafeIpc error. Triggers destruction of the ConnectionProxy via
   *        ConnectionManagerProxy.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto TriggerDestruction() noexcept -> void;

  /*!
   * \brief Used logger.
   */
  Logger logger_;

  /*!
   * \brief Reference to the reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief Handle of Reactor software event for processing of SafeIpc errors.
   */
  ReactorCallbackHandle reactor_handle_safeipc_error_{};

  /*!
   * \brief Reference to ConnectionManagerProxy.
   */
  ConnectionManagerProxyInterface& connection_manager_proxy_;

  /*!
   * \brief The SafeIpc connection.
   */
  amsr::UniquePtr<GenericConnection> const connection_;

  /*!
   * \brief Handler for message transmission / reception via SafeIpc.
   */
  ConnectionMessageHandler connection_message_handler_;

  /*!
   * \brief Connection state.
   */
  ConnectionState connection_state_{ConnectionState::kDisconnected};

  /*!
   * \brief Mutex to protect the current connections state.
   */
  std::mutex connection_state_lock_{};

  /*!
   * \brief List of registered state change handlers.
   */
  StateChangeHandlerContainer state_change_handlers_{};

  /*!
   * \brief IntegrityLevel expected by this Required Service Instance.
   */
  IntegrityLevel const expected_server_integrity_level_;

  /*!
   * \brief The remote IPC unicast address of the server.
   */
  IpcUnicastAddress const address_;

  /*!
   * \brief Reference to the associated router connector.
   */
  amsr::SharedPtr<ProxyRouterConnectorInterface> router_connector_;

  /*!
   * \brief   Decodes and dispatches incoming packets.
   * \details A unique_ptr is used here to delay construction of the contained object into the constructor body as
   *          parameters need to be checked first.
   */
  amsr::UniquePtr<ReceiveMessageHandler> receive_message_handler_{nullptr};

  /*!
   * \brief Decodes and dispatches outgoing packets.
   */
  TransmitMessageHandler transmit_message_handler_;

  /*!
   * \brief Peer id of the connected partner.
   */
  amsr::ipc::Credentials peer_id_{0};
};

/*!
 * \brief   Logs a BasicIpc connection establishment error.
 * \details Logs an appropriate error message. Aborts if it is a development error and the error shows that the state
 *          machine has been corrupted.
 *
 * \param[in,out] logger    Logger to be used.
 * \param[in]     error     BasicIpc error code.
 * \param[in]     location  Origin of the log output.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE if access to logger is synchronized.
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \spec requires true; \endspec
 */
void LogBasicIpcConnectError(logging::AraComLogger const& logger, amsr::core::ErrorCode const& error,
                             logging::AraComLogger::LogLocation const& location) noexcept;

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_H_

// clang-format off
/*!
 * \exclusivearea amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::connection_state_lock_
 *                Used for protection of the connection state.
 *
 * \protects amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::connection_state_
 *
 * \usedin   amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::GetConnectionState
 * \usedin   amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::Connect
 * \usedin   amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::OnConnectFinished
 * \usedin   amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::CloseIpcConnection
 * \usedin   amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::Send
 * \usedin   amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::NotifyStateChangeHandlers
 * \usedin   amsr::ipc_binding_core::internal::connection_manager::ConnectionProxy::TriggerDestruction
 *
 * \exclude  -
 * \length   SHORT Modification of numeric connection state.
 *
 * \endexclusivearea
 */
// clang-format on

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
 *        \brief  Remote Server
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include "amsr/core/result.h"
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "amsr/shared_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/proxy_router_connector_interface.h"
#include "ipc_binding_core/internal/connection_manager/transmit_message_handler_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/proxy_router_mapper.h"
#include "ipc_binding_core/internal/remote_server_interface.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/service_discovery/required_service_instance_listener_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Forward declaration of RemoteServerManager to avoid circular dependency.
 */
class RemoteServerManagerInterface;

/*!
 * \brief Proxy-side representation of a single remote service instance.
 *
 * \unit IpcBinding::IpcBindingCore::RemoteServer
 */
class RemoteServer final : public RemoteServerInterface, public amsr::EnableSharedFromThis<RemoteServer> {
 public:
  /*!
   * \brief Type alias for the RuntimeProcessingMode.
   */
  using RuntimeProcessingMode = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeProcessingMode;

  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Type alias for the ServiceDiscoveryInterface.
   */
  using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Type alias for the ConnectionManagerProxyInterface.
   */
  using ConnectionManagerProxyInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerProxyInterface;

  /*!
   * \brief Type alias for the ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for the integrity level.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Type alias for the server address.
   */
  using IpcUnicastAddress = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcUnicastAddress;

  /*!
   * \brief Type alias for the server sink.
   */
  using TransmitMessageHandlerInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::TransmitMessageHandlerInterface;

  /*!
   * \brief Construct a RemoteServer and returns a shared pointer to it.
   *
   * \param[in,out] reactor                             Reference to the Reactor.
   * \param[in]     runtime_processing_mode             The used runtime processing mode.
   * \param[in,out] service_discovery                   Reference to the ServiceDiscovery.
   * \param[in,out] required_service_instance_listener  Reference to the listener for the corresponding required service
   *                                                    instance.
   * \param[in,out] conman_proxy                        Reference to the used ConnectionManagerProxyInterface.
   * \param[in]     provided_service_instance           The ProvidedServiceInstance the RemoteServer shall connect to.
   * \param[in]     integrity_level                     The expected integrity level of the IPC connection.
   * \param[in,out] remote_server_manager               Remote server manager reference to request back for release.
   *
   * \return Shared pointer to the RemoteServer.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  static auto Create(ReactorInterface& reactor, RuntimeProcessingMode runtime_processing_mode,
                     ServiceDiscoveryInterface& service_discovery,
                     service_discovery::RequiredServiceInstanceListenerInterface& required_service_instance_listener,
                     ConnectionManagerProxyInterface& conman_proxy,
                     ProvidedServiceInstanceId const& provided_service_instance, IntegrityLevel integrity_level,
                     RemoteServerManagerInterface& remote_server_manager) noexcept -> amsr::SharedPtr<RemoteServer>;

  RemoteServer(RemoteServer const&) noexcept = delete;
  RemoteServer(RemoteServer&&) noexcept = delete;
  auto operator=(RemoteServer const&) noexcept -> RemoteServer& = delete;
  auto operator=(RemoteServer&&) noexcept -> RemoteServer& = delete;

  /*!
   * \brief Destroy a RemoteServer.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  ~RemoteServer() noexcept final;

  /*!
   * \brief Request the service instance.
   *
   * \param[in] client_id  A unique client ID to represent an instance of ProxyBackend.
   * \param[in] router     A proxyRouter corresponding to the ProxyBackend.
   *
   * \pre        -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   */
  void Request(ipc_binding_core::internal::ipc_protocol::ClientId client_id,
               ipc_binding_core::internal::ProxyRouterInterface& router) noexcept final;

  /*!
   * \brief Release the service instance.
   *
   * \param[in] client_id  A unique client ID to represent an instance of ProxyBackend.
   *
   * \pre        -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   */
  void Release(ipc_binding_core::internal::ipc_protocol::ClientId client_id) noexcept final;

  /*!
   * \brief   Get the current connection state to the remote server.
   * \details Be aware that this API only returns the currently known connection state. The connection can be already
   *          terminated with the next reactor cycle.
   *
   * \return True if connection to the remote server is established. Otherwise false.
   *
   * \pre        -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   *
   * \spec requires true; \endspec
   */
  auto IsConnected() const noexcept -> bool final;

  // ---- ConnectionStateChangeHandlerInterface ----

  /*!
   * \copydoc ipc_binding_core::internal::connection_manager::ConnectionStateChangeHandlerInterface::OnConnected
   */
  auto OnConnected(
      amsr::WeakPtr<ipc_binding_core::internal::ProxyRouterConnectorInterface> const& router_connector) noexcept
      -> void final;

  /*!
   * \copydoc ipc_binding_core::internal::connection_manager::ConnectionStateChangeHandlerInterface::OnDisconnected
   *
   * \spec requires true; \endspec
   */
  auto OnDisconnected(amsr::core::Result<void> const disconnect_reason) noexcept -> void final;

  // ---- ServiceInstanceListenerInterface ----

  /*!
   * \copydoc ipc_binding_core::internal::service_discovery::ServiceInstanceListenerInterface::OnServiceInstanceUp
   */
  auto OnServiceInstanceUp(IpcUnicastAddress const& address) noexcept -> void final;

  /*!
   * \copydoc ipc_binding_core::internal::service_discovery::ServiceInstanceListenerInterface::OnServiceInstanceDown
   */
  auto OnServiceInstanceDown() noexcept -> void final;

 private:
  /*!
   * \brief Construct a RemoteServer.
   *
   * \param[in,out] reactor                             Reference to the Reactor.
   * \param[in]     runtime_processing_mode             The used runtime processing mode.
   * \param[in,out] service_discovery                   Reference to the ServiceDiscovery.
   * \param[in,out] required_service_instance_listener  Reference to the listener for the corresponding required service
   *                                                    instance.
   * \param[in,out] conman_proxy                        Reference to the used ConnectionManagerProxyInterface.
   * \param[in]     provided_service_instance           The ProvidedServiceInstance the RemoteServer shall connect to.
   * \param[in]     integrity_level                     The expected integrity level of the IPC connection.
   * \param[in,out] remote_server_manager               Remote server manager reference to request back for release.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  RemoteServer(ReactorInterface& reactor, RuntimeProcessingMode runtime_processing_mode,
               ServiceDiscoveryInterface& service_discovery,
               service_discovery::RequiredServiceInstanceListenerInterface& required_service_instance_listener,
               ConnectionManagerProxyInterface& conman_proxy,
               ProvidedServiceInstanceId const& provided_service_instance, IntegrityLevel integrity_level,
               RemoteServerManagerInterface& remote_server_manager) noexcept;

  /*!
   * \brief   Get the current connection state to the ConnectionSkeleton.
   * \details The API returns actual connection state. The connection can be already terminated with the next reactor
   *          cycle.
   *
   * \return True if connection to the remote server is established. Otherwise False.
   *
   * \pre        -
   * \context    Reactor
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   */
  auto IsConnectedInternal() const noexcept -> bool;

  /*!
   * \brief Type alias for the used Logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Maximum duration to wait until a new connection is established.
   */
  static std::chrono::seconds constexpr kConnectionEstablishedTimeout{10};

  /*!
   * \brief Blocking wait until connection is established.
   *
   * \pre        -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   */
  auto WaitForConnectionEstablishment() noexcept -> void;

  /*!
   * \brief Disconnect from the remote service instance.
   *
   * \pre        Connection must be established.
   * \context    Reactor
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  auto Disconnect() noexcept -> void;

  /*!
   * \brief Update the connection state.
   *
   * \param[in] connected  New connection state.
   *
   * \pre        -
   * \context    Reactor
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  auto UpdateConnectionState(bool connected) noexcept -> void;

  /*!
   * \brief Release registered router mapper from connection manager proxy.
   *
   * \pre        -
   * \context    Reactor
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  auto ReleaseRouterMapper() noexcept -> void;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_;

  /*!
   * \brief Reference to the reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief The used runtime processing mode.
   */
  RuntimeProcessingMode const runtime_processing_mode_;

  /*!
   * \brief Reference to the service discovery.
   */
  ServiceDiscoveryInterface& service_discovery_;

  /*!
   * \brief Reference to the listener for the corresponding required service instance.
   */
  service_discovery::RequiredServiceInstanceListenerInterface& required_service_instance_listener_;

  /*!
   * \brief Reference to the used ConnectionManagerProxyInterface.
   */
  ConnectionManagerProxyInterface& conman_proxy_;

  /*!
   * \brief Remote provided service instance ID.
   */
  ProvidedServiceInstanceId provided_service_instance_;

  /*!
   * \brief The expected integrity level of the IPC connection.
   */
  IntegrityLevel const integrity_level_;

  /*!
   * \brief Counter keeping track of the users of this remote service instance.
   */
  std::size_t request_counter_{0};

  /*!
   * \brief IPC address off the remote service instance.
   */
  ::amsr::core::Optional<IpcUnicastAddress> connection_address_{};

  /*!
   * \brief Connection state.
   */
  std::atomic_bool connected_{false};

  /*!
   * \brief Mutex to protect connection state updates between app and reactor thread.
   */
  std::mutex connected_lock_{};

  /*!
   * \brief ConditionVariable for active wait until connection is established.
   */
  std::condition_variable connected_cv_{};  // VCA_IPCB_STD_FUNCTION_CALLED_WITHOUT_PARAMETER

  /*!
   * \brief Remote server manager reference to request back for release.
   */
  RemoteServerManagerInterface& remote_server_manager_;

  /*!
   * \brief ProxyRouterMapper object to map multiple ProxyBackends to specific ProxyRouters .
   */
  ProxyRouterMapper router_mapper_{};  // VCA_IPCB_CALL_TO_PROXY_ROUTER_MAPPER_FUNC

  /*!
   * \brief Pointer to the currently stored RouterConnector.
   */
  amsr::WeakPtr<ipc_binding_core::internal::ProxyRouterConnectorInterface> router_connector_{};

  /*!
   * \brief   Service instance availability status.
   * \details True for ServiceInstance is up. False for ServiceInstance is Down.
   */
  std::atomic_bool service_instance_status_{false};
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_H_

// clang-format off
/*!
 * \exclusivearea
 * amsr::ipc_binding_core::internal::RemoteServer::connected_lock_
 * Protect the connection state used for blocking wait until connection is established.
 * \protects amsr::ipc_binding_core::internal::RemoteServer::connected_
 * \usedin   amsr::ipc_binding_core::internal::RemoteServer::WaitForConnectionEstablishment
 * \usedin   amsr::ipc_binding_core::internal::RemoteServer::UpdateConnectionState
 * \exclude  All other methods of the RemoteServer.
 * \length   LONG Wait until the IPC unicast connection to the remote server is established.
 * \endexclusivearea
 */
// clang-format on

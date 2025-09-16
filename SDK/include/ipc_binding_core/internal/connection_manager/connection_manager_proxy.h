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
 *        \brief  Manages proxy's connection.
 *
 *      \details  The connection manager is responsible for establishing of connections to remote servers.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>

#include "amsr/shared_ptr.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   IPC connection manager.
 * \details Manages client connections.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionManagerProxy
 */
class ConnectionManagerProxy final : public ConnectionManagerProxyInterface {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Releases the resources of connection manager.
   *
   * \pre     -
   * \context Shutdown
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  ~ConnectionManagerProxy() noexcept final;

  ConnectionManagerProxy() noexcept = delete;
  ConnectionManagerProxy(ConnectionManagerProxy const&) noexcept = delete;
  auto operator=(ConnectionManagerProxy const&) noexcept -> ConnectionManagerProxy& = delete;
  ConnectionManagerProxy(ConnectionManagerProxy&&) noexcept = delete;
  auto operator=(ConnectionManagerProxy&&) noexcept -> ConnectionManagerProxy& = delete;

  /*!
   * \copydoc ConnectionManagerProxyInterface::OnDisconnect
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  void OnDisconnect(ConnectionProxyInterface* connection) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerProxyInterface::Connect
   */
  auto Connect(amsr::WeakPtr<ConnectionStateChangeHandlerInterface> state_change_handler,
               IpcUnicastAddress const& address, IntegrityLevel const integrity_level,
               ProvidedServiceInstanceId const provided_service_instance,
               ProxyRouterMapperInterface& router_mapper) noexcept -> ConnectionState final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerProxyInterface::Disconnect
   *
   * \steady FALSE
   *
   * \spec requires true; \endspec
   */
  void Disconnect(ProvidedServiceInstanceId provided_service_instance, IpcUnicastAddress const& address) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerProxyInterface::ReleaseRouterMapper
   */
  void ReleaseRouterMapper(IpcUnicastAddress const& address,
                           ProvidedServiceInstanceId provided_service_instance) noexcept final;

  /*!
   * \brief Creates a unique pointer containing a connection manager proxy.
   *
   * \param[in,out] reactor                   The reactor interface. The object must be valid for Server's entire life
   *                                          time.
   * \param[in,out] trace_monitor             The trace monitor reference. The object must exist for the whole lifetime
   *                                          of this object.
   * \param[in]     access_control            Reference to an access control implementation. The object must be valid
   *                                          for the entire lifetime of this object.
   * \param[in]     connection_proxy_factory  Factory for construction of The ConnectionProxy instances.
   *
   * \return Valid unique pointer to a connection manager proxy.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto Create(ReactorInterface& reactor, trace::TraceMonitor const& trace_monitor,
                     access_control::AccessControlInterface const& access_control,
                     amsr::SharedPtr<ConnectionProxyFactoryInterface> connection_proxy_factory) noexcept
      -> amsr::UniquePtr<ConnectionManagerProxyInterface>;

 private:
  /*!
   * \brief Initializes the resources of connection manager.
   *
   * \param[in,out] reactor                   The reactor interface. The object must be valid for Server's entire life
   *                                          time.
   * \param[in,out] trace_monitor             The trace monitor reference. The object must exist for the whole lifetime
   *                                          of this object.
   * \param[in]     access_control            Reference to an access control implementation. The object must be valid
   *                                          for the entire lifetime of this object.
   * \param[in]     connection_proxy_factory  Factory for construction of the ConnectionProxy instances.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionManagerProxy(ReactorInterface& reactor, trace::TraceMonitor const& trace_monitor,
                         access_control::AccessControlInterface const& access_control,
                         amsr::SharedPtr<ConnectionProxyFactoryInterface> connection_proxy_factory) noexcept;

  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = logging::IpcBindingLogBuilder;

  /*!
   * \brief   The client pointer type.
   * \details We have to use a shared pointer because the object is both stored in a map and also referenced by the
   *          corresponding router connector.
   */
  using ClientPtr = amsr::SharedPtr<ConnectionProxyInterface>;

  /*!
   * \brief The container type for storing connection proxies.
   */
  using ConnectionProxyMap = ara::core::Map<IpcUnicastAddress, ClientPtr>;

  /*!
   * \brief The container type for storing router connectors.
   */
  using RouterConnectorMap = ara::core::Map<IpcUnicastAddress, amsr::SharedPtr<ProxyRouterConnectorInterface>>;

  /*!
   * \brief Type alias for reactor callback handle.
   */
  using ReactorCallbackHandle = osabstraction::io::reactor1::CallbackHandle;

  /*!
   * \brief Type alias for Reactor event types.
   */
  using ReactorEventTypes = ::osabstraction::io::reactor1::EventTypes;

  /*!
   * \brief Maximum duration to wait until a new connection is established.
   */
  static std::chrono::seconds constexpr kConnectionEstablishedTimeout{10};

  /*!
   * \brief Sleep duration until connection establishment state polling is repeated for reactor thread.
   */
  static std::chrono::milliseconds constexpr kConnectionStatePollingSleep{1};

  /*!
   * \brief A reference to an IPC Service Discovery.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief Handle of reactor software event for delayed destruction of terminated connections.
   */
  ReactorCallbackHandle reactor_handle_delayed_destruction_{};

  /*!
   * \brief Our logger.
   */
  Logger logger_;

  /*!
   * \brief A map of all active connections.
   */
  ConnectionProxyMap connection_proxies_;

  /*!
   * \brief A map of all router connectors.
   */
  RouterConnectorMap router_connectors_;

  /*!
   * \brief The container of all terminated ConnectionProxy's.
   */
  ara::core::Vector<ClientPtr> terminated_connections_;

  /*!
   * \brief Reference to the trace monitor.
   */
  trace::TraceMonitor const& trace_monitor_;

  /*!
   * \brief Reference to the access control.
   */
  access_control::AccessControlInterface const& access_control_;

  /*!
   * \brief Factory to create a ConnectionProxy.
   */
  amsr::SharedPtr<ConnectionProxyFactoryInterface> const connection_proxy_factory_;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_H_

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
 *        \brief  ConnectionManagerProxyInterface extended by ConnectionManagerProxy for handling unicast
 *                connection/disconnection
 *
 *      \details  Called from the reactor thread.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/integrity_level.h"
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_state.h"
#include "ipc_binding_core/internal/connection_manager/connection_state_change_handler_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_mapper_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Interface extended by ConnectionManagerProxy for handling unicast connection/disconnection.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionManagerProxy
 */
class ConnectionManagerProxyInterface {
 public:
  /*!
   * \brief Type alias for the server address.
   */
  using IpcUnicastAddress = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcUnicastAddress;

  /*!
   * \brief The type alias for Service Instance Integrity Level.
   */
  using IntegrityLevel = amsr::ipc::IntegrityLevel;

  /*!
   * \brief Type alias for the ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for the ProxyRouterMapperInterface.
   */
  using ProxyRouterMapperInterface = ::amsr::ipc_binding_core::internal::ProxyRouterMapperInterface;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context Shutdown
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ConnectionManagerProxyInterface() noexcept = default;

  ConnectionManagerProxyInterface(ConnectionManagerProxyInterface const&) noexcept = delete;
  ConnectionManagerProxyInterface(ConnectionManagerProxyInterface&&) noexcept = delete;
  auto operator=(ConnectionManagerProxyInterface const&) noexcept -> ConnectionManagerProxyInterface& = delete;
  auto operator=(ConnectionManagerProxyInterface&&) noexcept -> ConnectionManagerProxyInterface& = delete;

  /*!
   * \brief Removes the passed ConnectionProxy.
   *
   * \param[in] connection  Pointer to the ConnectionProxy which needs to be removed.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto OnDisconnect(ConnectionProxyInterface* connection) noexcept -> void = 0;

  /*!
   * \brief Initiates the establishment of the connection to the remote IPC server.
   *
   * \param[in,out] state_change_handler       State change handler to be registered as a listener for connection state
   *                                           changes.
   * \param[in]     address                    The remote IPC unicast address of an IPC server to which this connection
   *                                           is meant to be established.
   * \param[in]     integrity_level            The expected IntegrityLevel for the required service instance.
   * \param[in]     provided_service_instance  The ProvidedServiceInstance for which a connection has been requested.
   * \param[in]     router_mapper              The ProxyRouterMapper object corresponding to the
   *                                           provided_service_instance.
   *
   * \return ConnectionState
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto Connect(amsr::WeakPtr<ConnectionStateChangeHandlerInterface> state_change_handler,
                       IpcUnicastAddress const& address, IntegrityLevel const integrity_level,
                       ProvidedServiceInstanceId const provided_service_instance,
                       ProxyRouterMapperInterface& router_mapper) noexcept -> ConnectionState = 0;

  /*!
   * \brief Initiates the termination of the connection to the remote IPC server.
   *
   * \param[in] provided_service_instance  Provided service instance to be deregistered as a listener for connection
   *                                       state changes.
   * \param[in] address                    The remote IPC unicast address of an IPC server.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto Disconnect(ProvidedServiceInstanceId provided_service_instance,
                          IpcUnicastAddress const& address) noexcept -> void = 0;

  /*!
   * \brief Release the registered router mapper.
   *
   * \param[in] address                    The remote IPC unicast address of an IPC server.
   * \param[in] provided_service_instance  The ProvidedServiceInstance for which a disconnection has been requested.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto ReleaseRouterMapper(IpcUnicastAddress const& address,
                                   ProvidedServiceInstanceId provided_service_instance) noexcept -> void = 0;

 protected:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionManagerProxyInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_INTERFACE_H_

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
 *        \brief  The ProxyRouterConnectorInterface is used to connect ConnectionProxy and PacketRouter unit.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_PROXY_ROUTER_CONNECTOR_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_PROXY_ROUTER_CONNECTOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/backend_interfaces/proxy_method_backend_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_mapper_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Interface for Connection unit to reach Event and Method backends. And for PacketRouter unit to reach
 *        ConnectionProxy.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ProxyRouterConnectorInterface {
 public:
  /*!
   * \brief Type alias for a service instance identifier.
   */
  using ServiceInstanceIdentifier = ::amsr::ipc_binding_core::internal::ipc_protocol::ServiceInstanceIdentifier;
  /*!
   * \brief Constructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyRouterConnectorInterface() noexcept = default;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual ~ProxyRouterConnectorInterface() noexcept = default;

  ProxyRouterConnectorInterface(ProxyRouterConnectorInterface&&) noexcept = delete;
  ProxyRouterConnectorInterface(ProxyRouterConnectorInterface const&) noexcept = delete;
  auto operator=(ProxyRouterConnectorInterface const&) noexcept -> ProxyRouterConnectorInterface& = delete;
  auto operator=(ProxyRouterConnectorInterface&&) noexcept -> ProxyRouterConnectorInterface& = delete;

  /*!
   * \brief Registers a RouterMapper under the given ServiceInstanceIdentifier .
   *
   * \param[in]     service_instance_id  Unique key that represents one specific RouterMapper.
   * \param[in,out] router_mapper        ProxyRouterMapper to be stored for routing incoming messages.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto AddRouterMapper(ServiceInstanceIdentifier const& service_instance_id,
                               ProxyRouterMapperInterface& router_mapper) noexcept -> void = 0;

  /*!
   * \brief Unregisters a RouterMapper under the given ServiceInstanceIdentifier.
   *
   * \param[in] service_instance_id  Unique key that represents one specific ProxyRouterMapper.
   *
   * \return Current size of the RouterMapperMap.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto ReleaseRouterMapper(ServiceInstanceIdentifier const& service_instance_id) noexcept -> std::size_t = 0;

  /*!
   * \brief Update the weak_ptr with a new ConnectionProxy instance.
   *
   * \param[in,out] connection_proxy  The ConnectionProxy pointer.
   *
   * \pre         -
   * \context     App|Reactor
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto SetConnectionProxy(
      amsr::SharedPtr<connection_manager::ConnectionProxyInterface> connection_proxy) noexcept -> void = 0;

  /*!
   * \brief Try to access the currently stored ConnectionProxy.
   *
   * \return A shared_ptr to the currently stored ConnectionProxy object. Attention: In case the ConnectionProxy cannot
   *         be accessed, an empty shared_ptr will be returned. This might happen if the managed ConnectionProxy
   *         instance is not existing anymore.
   *
   * \pre         -
   * \context     App|Reactor
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetConnectionProxy() noexcept -> amsr::SharedPtr<connection_manager::ConnectionProxyInterface> = 0;

  /*!
   * \brief Get the EventXf over RouterMapper -> Router.
   *
   * \param[in] service_instance_id  ServiceInstanceIdentifier of the RouterMapper.
   * \param[in] client_id            ClientId of the Router.
   * \param[in] event_id             EventId of the ProxyEventBackendInterface.
   *
   * \return Pointer to the EventXf.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   */
  virtual auto GetEventXf(ServiceInstanceIdentifier const& service_instance_id, ipc_protocol::ClientId client_id,
                          ipc_protocol::EventId event_id) noexcept -> ProxyEventBackendInterface* = 0;

  /*!
   * \brief Get the MethodXf over RouterMapper -> Router.
   *
   * \param[in] service_instance_id  ServiceInstanceIdentifier of the RouterMapper.
   * \param[in] client_id            ClientId of the Router.
   * \param[in] method_id            MethodId of the ProxyMethodBackendInterface.
   *
   * \return Pointer to the MethodXf.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   */
  virtual auto GetMethodXf(ServiceInstanceIdentifier const& service_instance_id, ipc_protocol::ClientId client_id,
                           ipc_protocol::MethodId method_id) noexcept -> ProxyMethodBackendInterface* = 0;

  /*!
   * \brief Get all EventXfs with the given event id.
   *
   * \param[in] service_instance_id  ServiceInstanceIdentifier of the RouterMapper.
   * \param[in] event_id             EventId of the ProxyEventBackendInterface.
   *
   * \return Vector of ProxyEventXf's, or an empty result if the router mapper was not found.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   */
  virtual auto GetAllEventXfs(ServiceInstanceIdentifier const& service_instance_id,
                              ipc_protocol::EventId event_id) noexcept
      -> amsr::core::Result<ProxyEventXfVector const&> = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_PROXY_ROUTER_CONNECTOR_INTERFACE_H_

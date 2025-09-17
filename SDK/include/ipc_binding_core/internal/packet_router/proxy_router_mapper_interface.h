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
 *        \brief  The ProxyRouterMapperInterface links between PacketRouter unit and ConnectionProxy unit.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_MAPPER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_MAPPER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/vector.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/backend_interfaces/proxy_method_backend_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_state_change_handler_interface.h"
#include "ipc_binding_core/internal/packet_router/service_instance_state_listener_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Forward declarations.
 */
class ProxyRouterInterface;
class ProxyRouterConnectorInterface;

/*!
 * \brief Type alias for vector of EventXf references.
 */
using ProxyEventXfVector = ara::core::Vector<std::reference_wrapper<ProxyEventBackendInterface>>;

/*!
 * \brief Interface to manage multiple routers.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterProxy
 */
class ProxyRouterMapperInterface : public ServiceInstanceStateListenerInterface {
 public:
  /*!
   * \brief Constructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyRouterMapperInterface() noexcept = default;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ~ProxyRouterMapperInterface() noexcept override = default;

  ProxyRouterMapperInterface(ProxyRouterMapperInterface const&) noexcept = delete;
  ProxyRouterMapperInterface(ProxyRouterMapperInterface&&) noexcept = delete;
  auto operator=(ProxyRouterMapperInterface const&) noexcept -> ProxyRouterMapperInterface& = delete;
  auto operator=(ProxyRouterMapperInterface&&) noexcept -> ProxyRouterMapperInterface& = delete;

  /*!
   * \brief Register the router under the given ClientId .
   *
   * \param[in]     client_id  Proxy client id.
   * \param[in,out] router     Reference to the proxy router.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual auto AddClient(ipc_protocol::ClientId client_id, ProxyRouterInterface& router) noexcept -> void = 0;

  /*!
   * \brief Unregister the router under the given client_id.
   *
   * \param[in] client_id  Proxy client id.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual auto ReleaseClient(ipc_protocol::ClientId client_id) noexcept -> void = 0;

  /*!
   * \brief Get the event_xf stored under the given client_id and event_id.
   *
   * \param[in] client_id  ClientId of the ProxyRouter.
   * \param[in] event_id   EventId of the ProxyEventBackendInterface.
   *
   * \return Pointer to the event xf.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   */
  virtual auto GetEventXf(ipc_protocol::ClientId client_id, ipc_protocol::EventId event_id) noexcept
      -> ProxyEventBackendInterface* = 0;

  /*!
   * \brief Get the method_xf stored under the given client_id and method_id.
   *
   * \param[in] client_id  CLientId of the ProxyRouter.
   * \param[in] method_id  MethodId of the ProxyMethodBackendInterface.
   *
   * \return Pointer to the method xf.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   */
  virtual auto GetMethodXf(ipc_protocol::ClientId client_id, ipc_protocol::MethodId method_id) noexcept
      -> ProxyMethodBackendInterface* = 0;

  /*!
   * \brief Get all event_xfs of the underlying routers with the given event_id.
   *
   * \param[in] event_id  EventId of the ProxyEventBackendInterfaces.
   *
   * \return Vector of pointers to the event xfs.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   */
  virtual auto GetAllEventXfs(ipc_protocol::EventId event_id) noexcept -> ProxyEventXfVector const& = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_MAPPER_INTERFACE_H_

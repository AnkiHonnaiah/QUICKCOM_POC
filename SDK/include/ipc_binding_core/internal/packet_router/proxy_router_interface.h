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
 *        \brief  The ProxyRouterInterface routes incoming and outgoing messages between transformation unit and
 *                connection proxy unit.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/backend_interfaces/proxy_fire_and_forget_method_backend_interface.h"
#include "ipc_binding_core/internal/backend_interfaces/proxy_method_backend_interface.h"
#include "ipc_binding_core/internal/connection_manager/proxy_router_connector_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/packet_router/service_instance_state_listener_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Interface for routing between proxy and connection.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterProxy
 */
class ProxyRouterInterface : public ServiceInstanceStateListenerInterface {
 public:
  /*!
   * \brief Constructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyRouterInterface() noexcept = default;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ~ProxyRouterInterface() noexcept override = default;

  ProxyRouterInterface(ProxyRouterInterface const&) noexcept = delete;
  ProxyRouterInterface(ProxyRouterInterface&&) noexcept = delete;
  auto operator=(ProxyRouterInterface const&) noexcept -> ProxyRouterInterface& = delete;
  auto operator=(ProxyRouterInterface&&) noexcept -> ProxyRouterInterface& = delete;

  /*!
   * \brief Returns the underlying ConnectionProxy object behind the currently set RouterConnector.
   *
   * \return Pointer to the ConnectionProxy object.
   *
   * \pre     SetRouterConnector must have been called before.
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto GetConnectionProxy() const noexcept -> amsr::SharedPtr<connection_manager::ConnectionProxyInterface> = 0;

  /*!
   * \brief Sets the RouterConnector in the object.
   *
   * \param[in,out] router_connector  The RouterConnector pointer.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto SetRouterConnector(amsr::WeakPtr<ProxyRouterConnectorInterface> const& router_connector) noexcept
      -> void = 0;

  /*!
   * \brief Register the corresponding ProxyEvent backend for the given EventId.
   *
   * \param[in]     event_id  EventId for the event represented by a specific ProxyEvent backend.
   * \param[in,out] event_xf  Reference to the event xf.
   *
   * \pre     EventXf with the same EventId may not exist.
   * \context App
   * \steady  FALSE
   */
  virtual auto AddEventXf(ipc_protocol::EventId event_id, ProxyEventBackendInterface& event_xf) noexcept -> void = 0;

  /*!
   * \brief Register the corresponding ProxyMethod backend for the given MethodId..
   *
   * \param[in]     method_id  MethodId of the ProxyMethodBackendInterface.
   * \param[in,out] method_xf  Reference to the method xf.
   *
   * \pre     MethodXf with the same MethodId may not exist.
   * \context App
   * \steady  FALSE
   */
  virtual auto AddMethodXf(ipc_protocol::MethodId method_id, ProxyMethodBackendInterface& method_xf) noexcept
      -> void = 0;

  /*!
   * \brief Register the corresponding ProxyMethod backend for the given MethodId..
   *
   * \param[in]     method_id  MethodId of the ProxyFireAndForgetMethodXfBackend.
   * \param[in,out] method_xf  Reference to the method fire and forget xf.
   *
   * \pre     FireAndForgetMethodX with the same MethodId may not exist.
   * \context App
   * \steady  FALSE
   */
  virtual auto AddFireAndForgetMethodXf(ipc_protocol::MethodId method_id,
                                        ProxyFireAndForgetMethodBackendInterface& method_xf) noexcept -> void = 0;

  /*!
   * \brief Get the EventXf stored under the given event id.
   *
   * \param[in] event_id  EventId of the ProxyEventBackendInterface.
   *
   * \return Pointer to the event xf.
   *
   * \pre     EventXf with the given EventId must exist.
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto GetEventXf(ipc_protocol::EventId event_id) noexcept -> ProxyEventBackendInterface* = 0;

  /*!
   * \brief Get the MethodXf stored under the given method id.
   *
   * \param[in] method_id  MethodId of the ProxyMethodBackendInterface.
   *
   * \return Pointer to the method xf.
   *
   * \pre     MethodXf with the given MethodId must exist.
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto GetMethodXf(ipc_protocol::MethodId method_id) noexcept -> ProxyMethodBackendInterface* = 0;

  /*!
   * \brief Release the event_xf stored under the given event_id.
   *
   * \param[in] event_id  EventId of the ProxyEventBackendInterface.
   *
   * \pre     EventXf with the given EventId must exist.
   * \context App
   * \steady  FALSE
   */
  virtual auto ReleaseEventXf(ipc_protocol::EventId event_id) noexcept -> void = 0;

  /*!
   * \brief Release the method_xf stored under the given method_id.
   *
   * \param[in] method_id  MethodId of the ProxyMethodBackendInterface.
   *
   * \pre     MethodXf with the given MethodId must exist.
   * \context App
   * \steady  FALSE
   */
  virtual auto ReleaseMethodXf(ipc_protocol::MethodId method_id) noexcept -> void = 0;

  /*!
   * \brief Release the method_xf stored under the given method_id.
   *
   * \param[in] method_id  MethodId of the ProxyFireAndForgetMethodBackendInterface.
   *
   * \pre     FireAndForgetMethodXf with the given MethodId must exist.
   * \context App
   * \steady  FALSE
   */
  virtual auto ReleaseFireAndForgetMethodXf(ipc_protocol::MethodId method_id) noexcept -> void = 0;

  /*!
   * \brief Indicates that a connection to the remote server has been established.
   *
   * \param[in] router_connector  ProxyRouterConnector corresponding to the current ConnectionProxy.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto OnConnected(amsr::WeakPtr<ProxyRouterConnectorInterface> const& router_connector) noexcept -> void = 0;

  /*!
   * \brief Indicates that a connection to a remote server got disconnected.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto OnDisconnected() noexcept -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_INTERFACE_H_

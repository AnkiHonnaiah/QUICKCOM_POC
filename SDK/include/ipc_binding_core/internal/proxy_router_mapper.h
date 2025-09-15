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
 *        \brief  The ProxyRouterMapper maps multiple routers for specific ProxyXfs.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PROXY_ROUTER_MAPPER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PROXY_ROUTER_MAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/shared_ptr.h"
#include "ara/core/map.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_mapper_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Class to map multiple routers for specific ProxyXfs.
 *
 * \unit IpcBinding::IpcBindingCore::RemoteServer
 */
class ProxyRouterMapper final : public ::amsr::ipc_binding_core::internal::ProxyRouterMapperInterface {
 public:
  /*!
   * \brief Type alias for ProxyMethodBackendInterface.
   */
  using ProxyMethodBackendInterface = ::amsr::ipc_binding_core::internal::ProxyMethodBackendInterface;

  /*!
   * \brief Type alias for ProxyEventXfVector.
   */
  using ProxyEventXfVector = ::amsr::ipc_binding_core::internal::ProxyEventXfVector;

  /*!
   * \brief Type alias for ProxyEventBackendInterface.
   */
  using ProxyEventBackendInterface = ::amsr::ipc_binding_core::internal::ProxyEventBackendInterface;

  /*!
   * \brief Type-alias for MethodId.
   */
  using MethodId = ::amsr::ipc_binding_core::internal::ipc_protocol::MethodId;

  /*!
   * \brief Type-alias for EventId.
   */
  using EventId = ::amsr::ipc_binding_core::internal::ipc_protocol::EventId;

  /*!
   * \brief Type-alias for ClientId.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  /*!
   * \brief Type-alias for ProxyRouterInterface.
   */
  using ProxyRouterInterface = ::amsr::ipc_binding_core::internal::ProxyRouterInterface;
  /*!
   * \brief Constructor.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyRouterMapper() noexcept;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ~ProxyRouterMapper() noexcept final = default;  // VCA_IPCB_MOLE_1298

  ProxyRouterMapper(ProxyRouterMapper const&) noexcept = delete;
  ProxyRouterMapper(ProxyRouterMapper&&) noexcept = delete;
  auto operator=(ProxyRouterMapper const&) noexcept -> ProxyRouterMapper& = delete;
  auto operator=(ProxyRouterMapper&&) noexcept -> ProxyRouterMapper& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ServiceInstanceStateListenerInterface::OnServiceInstanceUp()
   */
  void OnServiceInstanceUp() noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ServiceInstanceStateListenerInterface::OnServiceInstanceDown()
   */
  void OnServiceInstanceDown() noexcept final;

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
  void OnConnected(amsr::WeakPtr<ProxyRouterConnectorInterface> const& router_connector) noexcept;

  /*!
   * \brief Indicates that a connection to a remote server got disconnected.
   *
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady    FALSE
   */
  void OnDisconnected() noexcept;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterMapperInterface::AddClient
   */
  void AddClient(ClientId client_id, ProxyRouterInterface& router) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterMapperInterface::ReleaseClient
   */
  void ReleaseClient(ClientId client_id) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterMapperInterface::GetEventXf
   */
  auto GetEventXf(ClientId client_id, EventId event_id) noexcept -> ProxyEventBackendInterface* final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterMapperInterface::GetMethodXf
   */
  auto GetMethodXf(ClientId client_id, MethodId method_id) noexcept -> ProxyMethodBackendInterface* final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterMapperInterface::GetAllEventXfs
   */
  auto GetAllEventXfs(EventId event_id) noexcept -> ProxyEventXfVector const& final;

 private:
  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for a map holding the routers.
   */
  using RouterMap = ara::core::Map<ClientId, ProxyRouterInterface*>;

  /*!
   * \brief Map containing all the registered routers.
   */
  RouterMap router_table_{};

  /*!
   * \brief Preallocated vector that is used in GetAllEventXfs. It is used to temporarily store all event xfs with a
   *        specific event id, and thus needs to be preallocated. The preallocation happens during the initialization
   *        phase when routers are being added to the RouterMapper and is done in increments by 10 to avoid too many
   *        resize calls.
   */
  ProxyEventXfVector preallocated_event_xfs_{};

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PROXY_ROUTER_MAPPER_H_

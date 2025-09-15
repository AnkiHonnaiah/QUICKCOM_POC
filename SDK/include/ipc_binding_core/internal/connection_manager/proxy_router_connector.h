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
 *        \brief  The ProxyRouterConnector is used to connect ConnectionProxy and PacketRouter unit.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_PROXY_ROUTER_CONNECTOR_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_PROXY_ROUTER_CONNECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>

#include "amsr/shared_ptr.h"
#include "ara/core/map.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/proxy_router_connector_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Class for Connection unit to reach Event and Method backends. And for PacketRouter unit to reach
 *        ConnectionProxy.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ProxyRouterConnector final : public ProxyRouterConnectorInterface {
 public:
  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  ~ProxyRouterConnector() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  ProxyRouterConnector(ProxyRouterConnector&&) noexcept = delete;
  ProxyRouterConnector(ProxyRouterConnector const&) noexcept = delete;
  auto operator=(ProxyRouterConnector const&) noexcept -> ProxyRouterConnector& = delete;
  auto operator=(ProxyRouterConnector&&) noexcept -> ProxyRouterConnector& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterConnectorInterface::AddRouterMapper
   */
  auto AddRouterMapper(ServiceInstanceIdentifier const& service_instance_id,
                       ProxyRouterMapperInterface& router_mapper) noexcept -> void final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterConnectorInterface::ReleaseRouterMapper
   */
  auto ReleaseRouterMapper(ServiceInstanceIdentifier const& service_instance_id) noexcept -> std::size_t final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterConnectorInterface::SetConnectionProxy
   */
  auto SetConnectionProxy(amsr::SharedPtr<connection_manager::ConnectionProxyInterface> connection_proxy) noexcept
      -> void final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterConnectorInterface::GetConnectionProxy
   */
  auto GetConnectionProxy() noexcept -> amsr::SharedPtr<connection_manager::ConnectionProxyInterface> final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterConnectorInterface::GetEventXf
   */
  auto GetEventXf(ServiceInstanceIdentifier const& service_instance_id, ipc_protocol::ClientId client_id,
                  ipc_protocol::EventId event_id) noexcept -> ProxyEventBackendInterface* final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterConnectorInterface::GetMethodXf
   */
  auto GetMethodXf(ServiceInstanceIdentifier const& service_instance_id, ipc_protocol::ClientId client_id,
                   ipc_protocol::MethodId method_id) noexcept -> ProxyMethodBackendInterface* final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterConnectorInterface::GetAllEventXfs
   */
  auto GetAllEventXfs(ServiceInstanceIdentifier const& service_instance_id, ipc_protocol::EventId event_id) noexcept
      -> amsr::core::Result<ProxyEventXfVector const&> final;

  /*!
   * \brief Create a ProxyRouterConnector and return a pointer to its interface.
   *
   * \return ProxyRouterConnector object.
   *
   * \pre     Should only be called by the ConnectionProxyFactory.
   * \context ANY
   * \steady  FALSE
   */
  static auto Create() noexcept -> amsr::SharedPtr<ProxyRouterConnectorInterface>;

 private:
  /*!
   * \brief Constructor.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  ProxyRouterConnector() noexcept;

  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for storing RouterMappers in a map.
   */
  using RouterMapperMap = ara::core::Map<ServiceInstanceIdentifier, ProxyRouterMapperInterface*>;

  /*!
   * \brief Holds the currently registered RouterMappers.
   */
  RouterMapperMap router_mapper_table_{};

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_{logging::kIpcLoggerContextId, logging::kIpcLoggerContextDescription,
                 ::amsr::core::StringView{"ProxyRouterConnector"}};

  /*!
   * \brief Pointer to ConnectionProxy. Used to transmit messages via SafeIPC.
   */
  amsr::WeakPtr<connection_manager::ConnectionProxyInterface> connection_proxy_{};

  /*!
   * \brief Mutex to protect parallel access to connection_proxy_.
   */
  std::mutex connection_proxy_lock_{};
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_PROXY_ROUTER_CONNECTOR_H_

// clang-format off
/*!
 * \exclusivearea amsr::ipc_binding_core::internal::ProxyRouterConnector::connection_proxy_lock_
 *                Protects from parallel access to the ConnectionProxy weak_ptr.
 *
 * \protects amsr::ipc_binding_core::internal::ProxyRouterConnector::connection_proxy_
 * \usedin   amsr::ipc_binding_core::internal::ProxyRouterConnector::GetConnectionProxy
 * \usedin   amsr::ipc_binding_core::internal::ProxyRouterConnector::SetConnectionProxy
 * \exclude  All other methods of ProxyRouterConnector.
 * \length   SHORT Assignment of weak_ptr.
 * \endexclusivearea
 */
// clang-format on

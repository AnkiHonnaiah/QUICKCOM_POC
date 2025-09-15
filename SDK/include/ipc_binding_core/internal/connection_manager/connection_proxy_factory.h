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
 *        \brief  Factory for construction of ConnectionProxy objects.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_FACTORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/ipc/integrity_level.h"
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_connection.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Factory for construction of ConnectionProxy objects.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ConnectionProxyFactory final : public ConnectionProxyFactoryInterface {
 public:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ConnectionProxyFactory() noexcept = default;

  /*!
   * \brief Default destroys the interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~ConnectionProxyFactory() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  ConnectionProxyFactory(ConnectionProxyFactory const&) noexcept = delete;
  auto operator=(ConnectionProxyFactory const&) noexcept -> ConnectionProxyFactory& = delete;
  ConnectionProxyFactory(ConnectionProxyFactory&&) noexcept = delete;
  auto operator=(ConnectionProxyFactory&&) noexcept -> ConnectionProxyFactory& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyFactoryInterface::CreateConnectionProxy
   */
  auto CreateConnectionProxy(ReactorInterface& reactor, ConnectionManagerProxyInterface& connection_manager_proxy,
                             amsr::SharedPtr<ProxyRouterConnectorInterface> const& router_connector,
                             amsr::UniquePtr<GenericConnection>&& connection, IpcUnicastAddress const& address,
                             trace::TraceMonitor const& trace_monitor,
                             access_control::AccessControlInterface const& access_control,
                             amsr::ipc::IntegrityLevel const integrity_level) noexcept
      -> amsr::SharedPtr<ConnectionProxyInterface> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionProxyFactoryInterface::CreateRouterConnector
   */
  auto CreateRouterConnector() const noexcept -> amsr::SharedPtr<ProxyRouterConnectorInterface> final;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_FACTORY_H_

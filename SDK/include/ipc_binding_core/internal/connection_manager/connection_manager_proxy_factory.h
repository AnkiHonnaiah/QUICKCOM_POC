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
 *        \brief  Constructs a connection manager proxy.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_FACTORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_factory_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Constructs a connection manager proxy.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionManagerProxy
 */
class ConnectionManagerProxyFactory final : public ConnectionManagerProxyFactoryInterface {
 public:
  /*!
   * \brief Construct the factory.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ConnectionManagerProxyFactory() noexcept = default;

  /*!
   * \brief Destroy the factory.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~ConnectionManagerProxyFactory() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  ConnectionManagerProxyFactory(ConnectionManagerProxyFactory const&) noexcept = delete;
  auto operator=(ConnectionManagerProxyFactory const&) noexcept -> ConnectionManagerProxyFactory& = delete;
  ConnectionManagerProxyFactory(ConnectionManagerProxyFactory&&) noexcept = delete;
  auto operator=(ConnectionManagerProxyFactory&&) noexcept -> ConnectionManagerProxyFactory& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerProxyFactoryInterface::Create
   */
  auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor, trace::TraceMonitor const& trace_monitor,
              access_control::AccessControlInterface const& access_control,
              amsr::SharedPtr<ConnectionProxyFactoryInterface> connection_proxy_factory) noexcept
      -> amsr::UniquePtr<ConnectionManagerProxyInterface> final;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_FACTORY_H_

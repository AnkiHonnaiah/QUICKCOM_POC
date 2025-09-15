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
 *        \brief  Interface to construct a binding core.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/aracom_ipc_binding_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_factory_interface.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Defines an interface to create a generic ipcbinding core.
 *
 * \unit IpcBinding::IpcBindingCore::LifeCycleManagerCore
 */
class AraComIpcBindingFactoryInterface {
 public:
  /*!
   * \brief Type-alias for ServiceDiscoveryInterface.
   */
  using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Default destroys the interface.
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
  virtual ~AraComIpcBindingFactoryInterface() noexcept = default;

  AraComIpcBindingFactoryInterface(AraComIpcBindingFactoryInterface const&) noexcept = delete;
  auto operator=(AraComIpcBindingFactoryInterface const&) noexcept -> AraComIpcBindingFactoryInterface& = delete;
  AraComIpcBindingFactoryInterface(AraComIpcBindingFactoryInterface&&) noexcept = delete;
  auto operator=(AraComIpcBindingFactoryInterface&&) noexcept -> AraComIpcBindingFactoryInterface& = delete;

  /*!
   * \brief Creates a ipcbinding core and returns the unique pointer.
   *
   * \param[in,out] reactor              A pointer to the reactor for the ServiceDiscovery and the ConnectionManager.
   * \param[in,out] cm_proxy_factory     Valid unique pointer to a connection manager proxy factory.
   * \param[in,out] cm_skeleton_factory  Valid unique pointer to a connection manager skeleton factory.
   * \param[in,out] service_discovery    The ServiceDiscovery instance.
   * \param[in,out] access_control       Reference to an access control implementation.
   * \param[in,out] trace_monitor        Reference to the trace monitor for packet tracing.
   *
   * \return Unique pointer to a newly allocated ipcbinding core.
   *
   * \context    Init
   * \threadsafe FALSE
   * \steady     FALSE
   */
  virtual auto Create(
      osabstraction::io::reactor1::Reactor1Interface& reactor,
      amsr::UniquePtr<connection_manager::ConnectionManagerProxyFactoryInterface> cm_proxy_factory,
      amsr::UniquePtr<connection_manager::ConnectionManagerSkeletonFactoryInterface> cm_skeleton_factory,
      amsr::UniquePtr<ServiceDiscoveryInterface> service_discovery,
      access_control::AccessControlInterface& access_control,
      ipc_binding_core::internal::trace::TraceMonitor const& trace_monitor) noexcept
      -> amsr::UniquePtr<AraComIpcBindingInterface> = 0;

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
  AraComIpcBindingFactoryInterface() noexcept = default;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_FACTORY_INTERFACE_H_

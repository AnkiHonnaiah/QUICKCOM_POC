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
 *        \brief  Main class for ara::com IPC binding
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_factory_interface.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Glues the server and client functionality together in one class. Assembles the service discovery and packet
 *        router.
 *
 * \unit IpcBinding::IpcBindingCore::LifeCycleManagerCore
 */
class AraComIpcBindingInterface {
 public:
  /*!
   * \brief Type-alias for ServiceDiscoveryInterface.
   */
  using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Destroys the object.
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
  virtual ~AraComIpcBindingInterface() noexcept = default;

  AraComIpcBindingInterface(AraComIpcBindingInterface const&) noexcept = delete;
  auto operator=(AraComIpcBindingInterface const&) noexcept -> AraComIpcBindingInterface& = delete;
  AraComIpcBindingInterface(AraComIpcBindingInterface&&) noexcept = delete;
  auto operator=(AraComIpcBindingInterface&&) noexcept -> AraComIpcBindingInterface& = delete;

  /*!
   * \brief Get the SkeletonRouter instance.
   *
   * \return Reference to the SkeletonRouter instance.
   *
   * \pre         Can be called in any context after the initial phase.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetSkeletonRouter() noexcept -> SkeletonRouterInterface& = 0;

  /*!
   * \brief Returns the factory for creating proxy routers.
   *
   * \return The proxy router factory, that is a member of this class.
   *
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetProxyRouterFactory() noexcept -> ProxyRouterFactoryInterface& = 0;

  /*!
   * \brief Getter function for IpcServiceDiscovery.
   *
   * \return Returns a reference to the IpcServiceDiscovery instance.
   *
   * \context ANY
   */
  virtual auto GetServiceDiscovery() noexcept -> ServiceDiscoveryInterface& = 0;

  /*!
   * \brief Getter function for ConnectionManagerProxy.
   *
   * \return Returns a reference to the ConnectionManagerProxy instance.
   *
   * \context ANY
   * \steady  FALSE
   */
  virtual auto GetConnectionManagerProxy() noexcept -> connection_manager::ConnectionManagerProxyInterface& = 0;

  /*!
   * \brief Getter function for ConnectionManagerSkeleton.
   *
   * \return Returns a reference to the ConnectionManagerSkeleton instance.
   *
   * \context ANY
   * \steady  FALSE
   */
  virtual auto GetConnectionManagerSkeleton() noexcept -> connection_manager::ConnectionManagerSkeletonInterface& = 0;

 protected:
  /*!
   * \brief Initialize the IPC binding and passes the reactor for remote communication.
   *
   * \pre        -
   * \context    Init
   * \threadsafe FALSE
   * \steady     FALSE
   */
  AraComIpcBindingInterface() noexcept = default;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_INTERFACE_H_

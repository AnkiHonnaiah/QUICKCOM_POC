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
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/aracom_ipc_binding_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_factory_interface.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Glues the server and client functionality together in one class. Assembles the service discovery and packet
 *        router.
 *
 * \unit IpcBinding::IpcBindingCore::LifeCycleManagerCore
 */
class AraComIpcBinding : public AraComIpcBindingInterface {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

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
  ~AraComIpcBinding() noexcept override;

  AraComIpcBinding(AraComIpcBinding const&) noexcept = delete;
  auto operator=(AraComIpcBinding const&) noexcept -> AraComIpcBinding& = delete;
  AraComIpcBinding(AraComIpcBinding&&) noexcept = delete;
  auto operator=(AraComIpcBinding&&) noexcept -> AraComIpcBinding& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface::GetSkeletonRouter
   */
  auto GetSkeletonRouter() noexcept -> SkeletonRouterInterface& final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface::GetProxyRouterFactory
   */
  auto GetProxyRouterFactory() noexcept -> ProxyRouterFactoryInterface& final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface::GetServiceDiscovery
   */
  auto GetServiceDiscovery() noexcept -> ServiceDiscoveryInterface& final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface::GetConnectionManagerProxy
   */
  auto GetConnectionManagerProxy() noexcept -> connection_manager::ConnectionManagerProxyInterface& final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface::GetConnectionManagerSkeleton
   */
  auto GetConnectionManagerSkeleton() noexcept -> connection_manager::ConnectionManagerSkeletonInterface& final;

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
  static auto Create(ReactorInterface& reactor,
                     amsr::UniquePtr<connection_manager::ConnectionManagerProxyFactoryInterface> cm_proxy_factory,
                     amsr::UniquePtr<connection_manager::ConnectionManagerSkeletonFactoryInterface> cm_skeleton_factory,
                     amsr::UniquePtr<ServiceDiscoveryInterface> service_discovery,
                     access_control::AccessControlInterface& access_control,
                     ipc_binding_core::internal::trace::TraceMonitor const& trace_monitor) noexcept
      -> amsr::UniquePtr<AraComIpcBindingInterface>;

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = logging::AraComLogger;

  /*!
   * \brief Initialize the IPC binding and passes the reactor for remote communication.
   *
   * \param[in,out] reactor              A pointer to the reactor for the ServiceDiscovery and the ConnectionManager.
   * \param[in,out] cm_proxy_factory     Valid unique pointer to a connection manager proxy factory.
   * \param[in,out] cm_skeleton_factory  Valid unique pointer to a connection manager skeleton factory.
   * \param[in,out] service_discovery    The ServiceDiscovery instance.
   * \param[in,out] access_control       Reference to an access control implementation.
   * \param[in,out] trace_monitor        Reference to the trace monitor for packet tracing.
   *
   * \pre        -
   * \context    Init
   * \threadsafe FALSE
   * \steady     FALSE
   */
  AraComIpcBinding(osabstraction::io::reactor1::Reactor1Interface& reactor,
                   amsr::UniquePtr<connection_manager::ConnectionManagerProxyFactoryInterface> cm_proxy_factory,
                   amsr::UniquePtr<connection_manager::ConnectionManagerSkeletonFactoryInterface> cm_skeleton_factory,
                   amsr::UniquePtr<ServiceDiscoveryInterface> service_discovery,
                   access_control::AccessControlInterface& access_control,
                   ipc_binding_core::internal::trace::TraceMonitor const& trace_monitor) noexcept;

  /*!
   * \brief Precondition check.
   *
   * \param[in] condition      The condition to check.
   * \param[in] error_message  The error message.
   * \param[in] location       Original of the precondition check.
   *
   * \context    Init
   * \threadsafe FALSE
   * \steady     FALSE
   */
  void Assert(bool condition, core::StringView error_message, Logger::LogLocation const& location) const noexcept;

  /*!
   * \brief The reference to access control.
   */
  access_control::AccessControlInterface& access_control_;

  /*!
   * \brief SkeletonRouter is instantiated by this class.
   */
  SkeletonRouter skeleton_router_{};

  /*!
   * \brief Proxy's connection manager to connect to remote servers.
   */
  amsr::UniquePtr<connection_manager::ConnectionManagerProxyInterface> connection_manager_proxy_{};

  /*!
   * \brief Skeleton's connection manager to accept remote connections.
   */
  amsr::UniquePtr<connection_manager::ConnectionManagerSkeletonInterface> connection_manager_skeleton_{};

  /*!
   * \brief The ServiceDiscovery instance.
   */
  amsr::UniquePtr<ServiceDiscoveryInterface> service_discovery_;

  /*!
   * \brief Reference to reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief "ProxyRouterFactory" is instantiated by this class.
   */
  amsr::UniquePtr<ProxyRouterFactoryInterface> proxy_router_factory_{};

  /*!
   * \brief Logger
   */
  Logger logger_;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_H_

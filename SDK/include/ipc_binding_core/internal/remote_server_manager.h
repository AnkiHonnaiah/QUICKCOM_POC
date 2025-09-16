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
 *        \brief  Remote Server Manager
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_MANAGER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>

#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "amsr/shared_ptr.h"
#include "ara/core/map.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/remote_server_interface.h"
#include "ipc_binding_core/internal/remote_server_manager_interface.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/service_discovery/required_service_instance_listener_manager_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_IPCBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief Manage lazy-instantiation and reuse for RemoteServer instances. There might be multiple proxy objects for a
 *        single service instance. The RemoteServerManager ensures that only a single RemoteServer is instantiated for a
 *        single service instance. Every proxy object (for the same service instance) will then get access to the exact
 *        same RemoteServer.
 *
 * \unit IpcBinding::IpcBindingCore::RemoteServer
 */
class RemoteServerManager final : public RemoteServerManagerInterface {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Type alias for remote server shared pointer.
   */
  using RemoteServerSharedPtr = amsr::SharedPtr<RemoteServerInterface>;

  /*!
   * \brief Type alias for the RuntimeProcessingMode.
   */
  using RuntimeProcessingMode = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeProcessingMode;

  /*!
   * \brief Type alias for the ServiceDiscoveryInterface.
   */
  using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Type alias for the RequiredServiceInstanceManager.
   */
  using RequiredServiceInstanceManager =
      ::amsr::ipc_binding_core::internal::service_discovery::RequiredServiceInstanceListenerManagerInterface;

  /*!
   * \brief Type alias for the ConnectionManagerProxyInterface.
   */
  using ConnectionManagerProxyInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerProxyInterface;

  /*!
   * \brief Constructor
   *
   * \param[in,out] reactor                            Reference to the used Reactor.
   * \param[in]     runtime_processing_mode            The used runtime processing mode.
   * \param[in,out] required_service_instance_manager  Reference to the RequiredServiceInstanceManager.
   * \param[in,out] service_discovery                  Reference the ServiceDiscoveryInterface.
   * \param[in,out] conman_proxy                       Reference to the used ConnectionManagerProxyInterface.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  RemoteServerManager(ReactorInterface& reactor, RuntimeProcessingMode runtime_processing_mode,
                      RequiredServiceInstanceManager& required_service_instance_manager,
                      ServiceDiscoveryInterface& service_discovery,
                      ConnectionManagerProxyInterface& conman_proxy) noexcept;

  /*!
   * \brief Destructor
   *
   * \pre     All requested RemoteServers must have been released.
   * \context Shutdown
   * \steady  FALSE
   */
  ~RemoteServerManager() noexcept final;

  RemoteServerManager(RemoteServerManager const&) noexcept = delete;
  RemoteServerManager(RemoteServerManager&&) noexcept = delete;
  auto operator=(RemoteServerManager const&) noexcept -> RemoteServerManager& = delete;
  auto operator=(RemoteServerManager&&) noexcept -> RemoteServerManager& = delete;

  /*!
   * \copydoc RemoteServerManagerInterface::RequestRemoteServer
   */
  auto RequestRemoteServer(ProvidedServiceInstanceId const& provided_service_instance,
                           RequiredServiceInstanceId const& required_service_instance,
                           IntegrityLevel integrity_level) noexcept -> RemoteServerSharedPtr final;

  /*!
   * \copydoc RemoteServerManagerInterface::ReleaseRemoteServer
   */
  void ReleaseRemoteServer(ProvidedServiceInstanceId const& provided_service_instance) noexcept final;

 private:
  /*!
   * \brief Type alias for the used Logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_;

  /*!
   * \brief Reference to the reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief The used runtime processing mode.
   */
  RuntimeProcessingMode const runtime_processing_mode_;

  /*!
   * \brief Reference to the service discovery.
   */
  ServiceDiscoveryInterface& service_discovery_;

  /*!
   * \brief The required service instance manager.
   */
  RequiredServiceInstanceManager& required_service_instance_manager_;

  /*!
   * \brief Connection manager client.
   */
  ConnectionManagerProxyInterface& connection_manager_proxy_;

  /*!
   * \brief Map to store already created remote servers.
   */
  using RemoteServerMap = ara::core::Map<ProvidedServiceInstanceId, RemoteServerSharedPtr>;

  /*!
   * \brief Mutex to protect the map of RemoteServer instances.
   */
  std::mutex remote_servers_lock_{};

  /*!
   * \brief Map of remote servers.
   */
  RemoteServerMap remote_servers_{};
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_MANAGER_H_

// clang-format off
/*!
 * \exclusivearea
 * amsr::ipc_binding_core::internal::RemoteServerManager::remote_servers_lock_
 * Protect the read/write access to the service instance-specific RemoteServer instances map.
 * \protects amsr::ipc_binding_core::internal::RemoteServerManager::remote_servers_
 * \usedin   amsr::ipc_binding_core::internal::RemoteServerManager::RequestRemoteServer
 * \usedin   amsr::ipc_binding_core::internal::RemoteServerManager::ReleaseRemoteServer
 * \exclude  All other methods of the RemoteServerManager.
 * \length   MEDIUM Lazy-instantiation / destruction of RemoteServer instances.
 * \endexclusivearea
 */
// clang-format on

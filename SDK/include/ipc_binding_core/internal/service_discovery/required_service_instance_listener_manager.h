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
 *        \brief  Manager for required service instance listeners.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_MANAGER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "ara/core/map.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/required_service_instance_id.h"
#include "ipc_binding_core/internal/service_discovery/required_service_instance_listener.h"
#include "ipc_binding_core/internal/service_discovery/required_service_instance_listener_manager_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace service_discovery {
/*!
 * \brief Manager for required service instance listeners.
 *
 * \unit IpcBinding::IpcBinding::RequiredServiceInstanceManager
 */
class RequiredServiceInstanceListenerManager final : public RequiredServiceInstanceListenerManagerInterface {
 public:
  /*!
   * \brief Type alias for IpcServiceDiscoveryInterface.
   */
  using IpcServiceDiscovery = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Constructor.
   *
   * \param[in,out] ipc_service_discovery  Reference to the ipc service discovery. It must remain valid for the lifetime
   *                                       of this object.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit RequiredServiceInstanceListenerManager(IpcServiceDiscovery& ipc_service_discovery) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
  ~RequiredServiceInstanceListenerManager() noexcept final = default;

  RequiredServiceInstanceListenerManager(RequiredServiceInstanceListenerManager const&) noexcept = delete;
  auto operator=(RequiredServiceInstanceListenerManager const&) noexcept
      -> RequiredServiceInstanceListenerManager& = delete;
  RequiredServiceInstanceListenerManager(RequiredServiceInstanceListenerManager&&) noexcept = delete;
  auto operator=(RequiredServiceInstanceListenerManager&&) noexcept -> RequiredServiceInstanceListenerManager& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::service_discovery::RequiredServiceInstanceListenerManagerInterface::CreateListener
   */
  auto CreateListener(SocalProxyServiceDiscovery& socal_service_discovery,
                      RequiredServiceInstanceId const& required_service_instance_id) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::service_discovery::RequiredServiceInstanceListenerManagerInterface::GetListener
   */
  auto GetListener(RequiredServiceInstanceId const& required_service_instance_id) noexcept
      -> RequiredServiceInstanceListenerInterface& final;

 private:
  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the LogBuilder.
   */
  using LogBuilder = ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for the map of listeners.
   */
  using ListenersMap = ara::core::Map<RequiredServiceInstanceId, RequiredServiceInstanceListener>;

  /*!
   * \brief Reference to IpcServiceDiscovery.
   */
  IpcServiceDiscovery& ipc_service_discovery_;

  /*!
   * \brief Used logger.
   */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 amsr::core::StringView{"RequiredServiceInstanceListenerManager"}};

  /*!
   * \brief The map of registerd required service instance listeners.
   */
  ListenersMap listeners_{};
};
}  // namespace service_discovery
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_MANAGER_H_

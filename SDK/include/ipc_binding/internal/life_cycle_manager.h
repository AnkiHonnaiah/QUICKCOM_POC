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
 *        \brief  Lifecycle Manager for the IpcBinding
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/comtrace/internal/config/trace_config_interface.h"
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"

#include "ipc_binding/internal/runtime_interface.h"
#include "ipc_binding/internal/target_dependent/deinitializer_interface.h"
#include "ipc_binding/internal/target_dependent/initializer_interface.h"
#include "ipc_binding_core/internal/aracom_ipc_binding_factory_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/remote_server_manager_interface.h"
#include "ipc_binding_core/internal/runtime_configuration/runtime_config.h"
#include "ipc_binding_core/internal/service_discovery/required_service_instance_listener_manager_interface.h"
#include "ipc_binding_core/internal/service_discovery/service_discovery_factory_interface.h"
#include "ipc_binding_xf_common/internal/trace/trace_sink_factory_interface.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief Lifecycle management of the IpcBinding sub-components (Core, TransformationLayer).
 *
 * \unit IpcBinding::ComponentLifeCycleManagement
 */
class LifeCycleManager final {
 public:
  /*!
   * \brief Type alias for StringView.
   */
  using StringView = ::ara::core::StringView;

  /*! Type-Alias for the AraComIpcBindingFactoryInterface */
  using AraComIpcBindingFactoryInterface = ipc_binding_core::internal::AraComIpcBindingFactoryInterface;

  /*! Type-Alias for the InitializerInterface */
  using InitializerInterface = amsr::ipc_binding::internal::target_dependent::InitializerInterface;

  /*! Type-Alias for the DeinitializerInterface */
  using DeinitializerInterface = amsr::ipc_binding::internal::target_dependent::DeinitializerInterface;

  /*! Type-Alias for the ServiceDiscoveryFactoryInterface */
  using ServiceDiscoveryFactoryInterface =
      ipc_binding_core::internal::service_discovery::ServiceDiscoveryFactoryInterface;

  /*! Type-Alias for the ServiceDiscoveryInterface */
  using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*! Type-Alias for the TraceSinkFactoryInterface */
  using TraceSinkFactoryInterface = ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface;

  /*! Type-Alias for the ComTrace TraceConfigInterface */
  using TraceConfigInterface = ::amsr::comtrace::internal::config::TraceConfigInterface;

  /*!
   * \brief Constructor of LifeCycleManager.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   *
   * \internal
   * - Set initialization state to 'not initialized'.
   * - Store references of all managed IpcBinding sub-component instances.
   * \endinternal
   */
  LifeCycleManager() noexcept = default;

  /*!
   * \brief Destroy the LifeCycleManager.
   *
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~LifeCycleManager() noexcept = default;

  LifeCycleManager(LifeCycleManager const&) noexcept = delete;
  auto operator=(LifeCycleManager const&) noexcept -> LifeCycleManager& = delete;
  LifeCycleManager(LifeCycleManager&&) noexcept = delete;
  auto operator=(LifeCycleManager&&) noexcept -> LifeCycleManager& = delete;

  /*!
   * \brief Initialize the IpcBinding sub-components.
   *
   * \param[in,out] runtime                         Valid runtime to communicate with socal.
   * \param[in,out] lifecycle_manager_core_factory  Factory to construct the LifecycleManagerCore.
   * \param[in]     initializer                     An initializer.
   * \param[in]     deinitializer                   An deinitializer.
   * \param[in]     service_discovery_factory       ServiceDiscovery factory.
   * \param[in]     trace_sink_factory              Trace sink factory.
   * \param[in]     trace_config                    Trace configuration model hierarchy.
   * \param[in]     runtime_config_json_path        Path to runtime configuration JSON file (default available).
   *
   * \return Empty result value or an error.
   *
   * \error IpcBindingErrc::json_loading_failure  If component specific JSON configuration could not be found or loaded.
   * \error IpcBindingErrc::json_parsing_failure  If component specific JSON configuration parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_config_variant
  auto Initialize(amsr::UniquePtr<RuntimeInterface> runtime,
                  amsr::UniquePtr<AraComIpcBindingFactoryInterface> lifecycle_manager_core_factory,
                  amsr::UniquePtr<InitializerInterface> initializer,
                  amsr::UniquePtr<DeinitializerInterface> deinitializer,
                  amsr::UniquePtr<ServiceDiscoveryFactoryInterface> service_discovery_factory,
                  amsr::UniquePtr<TraceSinkFactoryInterface> trace_sink_factory,
                  TraceConfigInterface const& trace_config,
                  StringView runtime_config_json_path = StringView{"./etc/ipcbinding_config.json"}) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Deinitialize and Destroy all sub-components.
   *
   * \return Always a positive empty result.
   *
   * \pre         The related reactor thread must be stopped.
   * \pre         All proxy and skeleton instances using IpcBinding deployment must have been destroyed.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto Deinitialize() noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Get the BindingCore main instance.
   *
   * \return Reference to the BindingCore main instance.
   *
   * \pre         Initialize() called.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetBindingCore() noexcept -> amsr::ipc_binding_core::internal::AraComIpcBindingInterface&;

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Alias for service discovery required service instance identifier.
   */
  using RequiredServiceInstanceId = ::amsr::ipc_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Alias for service discovery provided service instance identifier.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for ConnectionManagerProxyInterface.
   */
  using ConnectionManagerProxyInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerProxyInterface;

  /*!
   * \brief Type alias for the RequiredServiceInstanceListenerManagerInterface.
   */
  using RequiredServiceInstanceListenerManagerInterface =
      ipc_binding_core::internal::service_discovery::RequiredServiceInstanceListenerManagerInterface;

  /*!
   * \brief Register InstanceSpecifier to InstanceIdentifier mappings with the Runtime.
   *
   * \pre         Configuration parsing done + runtime instantiated.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto RegisterInstanceSpecifierMappings() noexcept -> void;

  /*!
   * \brief Deregister all required and provided instance specifier and instance identifier mappings from runtime.
   *
   * \pre         All proxy and skeleton instances using IpcBinding deployment must have been destroyed.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto DeregisterInstanceSpecifierMappings() noexcept -> void;

  /*!
   * \brief Instantiate and build the runtime configuration.
   *
   * \param[in] runtime_config_json_path  Path to runtime configuration JSON file.
   * \param[in] trace_config              Trace configuration model hierarchy.
   *
   * \return Empty result value or an error.
   *
   * \error IpcBindingErrc::json_loading_failure  If component specific JSON configuration could not be found or loaded.
   * \error IpcBindingErrc::json_parsing_failure  If component specific JSON configuration parsing failed.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  auto BuildRuntimeConfig(StringView runtime_config_json_path, TraceConfigInterface const& trace_config) noexcept
      -> ara::core::Result<void>;

  /*!
   * \brief Perform the integrity check and parse the runtime configuration.
   *
   * \param[in] runtime_config_json_path  Path to runtime configuration JSON file.
   *
   * \return Empty result value or an error.
   *
   * \error IpcBindingErrc::json_parsing_failure  If component specific JSON configuration could not be parsed.
   *
   * \pre     Environment variable "AMSR_PROCESS_SHORT_NAME_PATH" must be set to Process short name path.
   * \context Init
   * \steady  FALSE
   */
  auto ValidateAndParse(StringView runtime_config_json_path) noexcept -> ara::core::Result<void>;

  /*!
   * \brief Initialize all logical-units and units.
   *
   * \param[in,out] lifecycle_manager_core_factory  Factory to construct the LifecycleManagerCore.
   * \param[in,out] service_discovery_factory       Factory to construct the ServiceDiscovery.
   *
   * \pre         All proxy and skeleton instances using IpcBinding deployment must have been destroyed.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto InitializeUnits(amsr::UniquePtr<AraComIpcBindingFactoryInterface> lifecycle_manager_core_factory,
                       amsr::UniquePtr<ServiceDiscoveryFactoryInterface> service_discovery_factory) noexcept -> void;

  /*!
   * \brief Deinitialize all logical-units and units.
   *
   * \pre         All proxy and skeleton instances using IpcBinding deployment must have been destroyed.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto DeinitializeUnits() noexcept -> void;

  /*!
   * \brief Destroy all all logical-units and units.
   *
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto DestroyUnits() noexcept -> void;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 StringView{"LifeCycleManager"}};

  /*!
   * \brief Runtime configuration.
   */
  ::ara::core::Optional<::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig> runtime_config_{};

  /*!
   * \brief Runtime instance.
   */
  amsr::UniquePtr<RuntimeInterface> runtime_{};

  /*!
   * \brief BindingCore instance.
   */
  amsr::UniquePtr<amsr::ipc_binding_core::internal::AraComIpcBindingInterface> binding_core_{};

  /*!
   * \brief RequiredServiceInstanceManager instance.
   */
  amsr::UniquePtr<RequiredServiceInstanceListenerManagerInterface> required_service_instance_manager_;

  /*!
   * \brief RemoteServerManager pointer.
   */
  amsr::UniquePtr<amsr::ipc_binding_core::internal::RemoteServerManagerInterface> remote_server_manager_{};

  /*!
   * \brief Initializer instance.
   */
  amsr::UniquePtr<amsr::ipc_binding::internal::target_dependent::InitializerInterface> initializer_{};

  /*!
   * \brief Deinitializer instance.
   */
  amsr::UniquePtr<amsr::ipc_binding::internal::target_dependent::DeinitializerInterface> deinitializer_{};

  /*!
   * \brief Trace sink instance.
   */
  amsr::UniquePtr<::amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface> trace_sink_factory_{};
};

}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr
#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_

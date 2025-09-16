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
 *        \brief  Initializer for required service instances.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_REQUIRED_SERVICE_INSTANCE_BACKEND_INITIALIZER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_REQUIRED_SERVICE_INSTANCE_BACKEND_INITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/generic/singleton_wrapper.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/socal/internal/factory/backend_factory_registry_interface.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_interface.h"

#include "ipc_binding/internal/proxy_backend_factory.h"
#include "ipc_binding_core/internal/instance_id_util.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"
#include "ipc_binding_core/internal/service_discovery/required_service_instance_listener_manager_interface.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief Static helper class for initialization of required service instances.
 *
 * \unit IpcBinding::IpcBinding::RequiredServiceInstance
 */
class RequiredServiceInstanceBackendInitializer final {
 public:
  /*!
   * \brief Type alias for AraComIpcBindingInterface.
   */
  using AraComIpcBindingInterface = ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface;

  /*!
   * \brief Type alias for the SOCAL ProxyServiceDiscoveryInterface.
   */
  using ProxyServiceDiscoveryInterface = ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInterface;

  /*!
   * \brief Type alias for a singleton access of SOCAL ProxyServiceDiscoveryInterface.
   */
  using ServiceDiscoverySingletonAccess =
      ::amsr::generic::SingletonAccess<::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInterface>;

  /*!
   * \brief Type alias for the SingletonAccess to SOCAL ProxyBackendFactoryRegistryInterface.
   *
   * \tparam ProxyBackendInterfaceType  ProxyBackend's backend interface.
   */
  template <typename ProxyBackendInterfaceType>
  using ProxyBackendFactoryRegistrySingletonAccess = ::amsr::generic::SingletonAccess<
      ::amsr::socal::internal::factory::ProxyBackendFactoryRegistryInterface<ProxyBackendInterfaceType>>;

  /*!
   * \brief Alias for service discovery service instance identifier.
   */
  using RequiredServiceInstanceId = ::amsr::ipc_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Type alias for IntegrityLevel.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Type alias for the ServiceInstanceListener unique_ptr.
   */
  using ServiceInstanceListenerPtr =
      amsr::UniquePtr<::amsr::ipc_binding_core::internal::service_discovery::ServiceInstanceListenerInterface>;

  /*!
   * \brief Runtime service configuration type.
   */
  using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Type alias for trace sink factory interface.
   */
  using TraceSinkFactoryInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface;

  /*!
   * \brief Type alias for the RequiredServiceInstanceManager.
   */
  using RequiredServiceInstanceManagerInterface =
      ::amsr::ipc_binding_core::internal::service_discovery::RequiredServiceInstanceListenerManagerInterface;

  /*!
   * \brief Type alias for Remote Server Manager interface.
   */
  using RemoteServerManagerInterface = ::amsr::ipc_binding_core::internal::RemoteServerManagerInterface;

  RequiredServiceInstanceBackendInitializer() noexcept = delete;
  RequiredServiceInstanceBackendInitializer(RequiredServiceInstanceBackendInitializer const&) noexcept = delete;
  auto operator=(RequiredServiceInstanceBackendInitializer const&) noexcept
      -> RequiredServiceInstanceBackendInitializer& = delete;
  RequiredServiceInstanceBackendInitializer(RequiredServiceInstanceBackendInitializer&&) noexcept = delete;
  auto operator=(RequiredServiceInstanceBackendInitializer&&) noexcept
      -> RequiredServiceInstanceBackendInitializer& = delete;

  /*!
   * \brief   Destroy the service discovery initializer.
   * \details Destructor for this utility class (static APIs only) provided to avoid potential undefined behavior in
   *          case of future extension (e.g. non-static members).
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~RequiredServiceInstanceBackendInitializer() noexcept = default;

  /*!
   * \brief Initializes all required service instances.
   *
   * \tparam ProxyBackend  ProxyBacked type.
   *
   * \param[in,out] binding_core                       A reference to IpcBinding.
   * \param[in,out] service_discovery                  The SingletonAccess to Socal ServiceDiscovery reference. The
   *                                                   reference must be valid for this object's entire lifetime.
   * \param[in,out] proxy_factory_registry             The SingletonAccess to the socal ProxyFactoryRegistry.
   * \param[in,out] required_service_instance_manager  Reference to the RequiredServiceInstanceManager.
   * \param[in,out] remote_server_manager              Reference to the RemoteServerManager.
   * \param[in]     service_config                     The service deployment configuration.
   * \param[in]     trace_sink_factory                 Reference to the trace sink factory.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Loop through all the RequiredServiceInstances.
   *   - Create a new RequiredServiceInstanceListener for the RequiredServiceInstance.
   *   - Initialize ProxyBackend object for the required service instance.
   * \endinternal
   */
  template <typename ProxyBackend>
  static void InitializeRequiredServiceInstances(
      AraComIpcBindingInterface& binding_core, ServiceDiscoverySingletonAccess service_discovery,
      ProxyBackendFactoryRegistrySingletonAccess<typename ProxyBackend::ProxyBackendInterface> proxy_factory_registry,
      RequiredServiceInstanceManagerInterface& required_service_instance_manager,
      RemoteServerManagerInterface& remote_server_manager, ServiceConfig const& service_config,
      TraceSinkFactoryInterface const& trace_sink_factory) noexcept {
    for (ServiceConfig::RequiredServiceInstanceConfigRef const required_service_instance :
         service_config.GetRequiredServiceInstances()) {
      RequiredServiceInstanceId const required_service_instance_id{
          service_config.GetId(),
          // VCA_IPCB_REQUIRED_SERVICE_INSTANCE_BACKEND_INITIALIZER_UNDEFINED_FUNCTION, VCA_IPCB_VALID_RUNTIME_CONFIGURATION_OBJECT
          required_service_instance.get().GetInstanceIdentifier(), service_config.GetMajorVersion(),
          // VCA_IPCB_REQUIRED_SERVICE_INSTANCE_BACKEND_INITIALIZER_UNDEFINED_FUNCTION, VCA_IPCB_VALID_RUNTIME_CONFIGURATION_OBJECT
          required_service_instance.get().GetMinorVersion()};

      // VCA_IPCB_FORWARD_VALID_PARAMETER_OBJECT
      required_service_instance_manager.CreateListener(*service_discovery, required_service_instance_id);

      // VCA_IPCB_REQUIRED_SERVICE_INSTANCE_BACKEND_INITIALIZER_UNDEFINED_FUNCTION, VCA_IPCB_VALID_RUNTIME_CONFIGURATION_OBJECT
      IntegrityLevel const expected_integrity_level{required_service_instance.get().GetExpectedServerIntegrityLevel()};
      InitializeProxyBackendFactory<ProxyBackend>(binding_core,
                                                  // VCA_IPCB_FORWARD_VALID_PARAMETER_OBJECT
                                                  proxy_factory_registry, remote_server_manager,
                                                  required_service_instance_id, expected_integrity_level,
                                                  service_config, trace_sink_factory);
    }
  }

  /*!
   * \brief Deinitialize required service instances.
   *
   * \tparam ProxyBackendInterfaceType  Interface type of ProxyBackend.
   *
   * \param[in]     service_config          The service deployment configuration.
   * \param[in,out] proxy_factory_registry  The SingletonAccess to the socal ProxyFactoryRegistry.
   *
   * \pre         SOCAL service discovery has been initialized.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Loop through all the required service instances.
   *   - Abort further process executing in case the deregistration from Socal failed.
   * \endinternal
   */
  template <typename ProxyBackendInterfaceType>
  static void DeinitializeRequiredServiceInstances(
      ServiceConfig const& service_config,
      ProxyBackendFactoryRegistrySingletonAccess<ProxyBackendInterfaceType> proxy_factory_registry) noexcept {
    for (ServiceConfig::RequiredServiceInstanceConfigRef const required_service_instance :
         service_config.GetRequiredServiceInstances()) {
      RequiredServiceInstanceId const required_service_instance_id{
          service_config.GetId(),
          // VCA_IPCB_REQUIRED_SERVICE_INSTANCE_BACKEND_INITIALIZER_UNDEFINED_FUNCTION, VCA_IPCB_VALID_RUNTIME_CONFIGURATION_OBJECT
          required_service_instance.get().GetInstanceIdentifier(), service_config.GetMajorVersion(),
          // VCA_IPCB_REQUIRED_SERVICE_INSTANCE_BACKEND_INITIALIZER_UNDEFINED_FUNCTION, VCA_IPCB_VALID_RUNTIME_CONFIGURATION_OBJECT
          required_service_instance.get().GetMinorVersion()};

      ::amsr::socal::internal::RequiredInstanceIdentifier const required_instance_identifier{
          InstanceIdUtil::BuildAraComInstanceIdentifier(required_service_instance_id)};

      // VCA_IPCB_DEREGISTER_FACTORY_FUNCTION, VCA_IPCB_SINGLETON_ACCESS_DESTRUCTOR, VCA_IPCB_VALID_PROXY_SD_OBJECT
      ::amsr::core::Result<void> const result{proxy_factory_registry->DeregisterFactory(required_instance_identifier)};
      if (!result.HasValue()) {
        Logger const logger{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                            ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                            ::amsr::core::StringView{"RequiredServiceInstanceBackendInitializer"}};

        logger.TerminateOnViolation(
            "Deregistration of RequiredServiceInstance from SOCAL ServiceDiscovery failed.",
            [&required_service_instance_id](::ara::log::LogStream& s) {
              s << " RequiredServiceInstance (";
              LogBuilder::LogServiceInstanceId(
                  s,
                  required_service_instance_id);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              s << ").";
            },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
    }
  }

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
   * \brief Type alias for the instance id utility.
   */
  using InstanceIdUtil = ::amsr::ipc_binding_core::internal::InstanceIdUtil;

  /*!
   * \brief Create and initialize SdProxyBackend instance.
   *
   * \tparam ProxyBackend  ProxyBackend type.
   *
   * \param[in,out] binding_core                  A reference to IpcBinding.
   * \param[in,out] proxy_factory_registry        The SingletonAccess to the socal ProxyFactoryRegistry.
   * \param[in,out] remote_server_manager         RemoteServerManagerInterface reference.
   * \param[in]     required_service_instance_id  Required service instance ID.
   * \param[in]     integrity_level               The integrity level for the required service instance.
   * \param[in]     service_config                The runtime service config for this service.
   * \param[in]     trace_sink_factory            Reference to the trace sink factory.
   *
   * \pre         SOCAL service discovery has been initialized.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Build a Socal RequiredInstanceIdentifier from the given required service instance ID.
   * - Instantiate a new ProxyBackendFactory object for the required service instance.
   * - Register the the new ProxyBackendFactory object with Socal.
   * - Abort further process executing in case the registration with Socal failed.
   * \endinternal
   */
  template <typename ProxyBackend>
  static void InitializeProxyBackendFactory(
      AraComIpcBindingInterface& binding_core,
      ProxyBackendFactoryRegistrySingletonAccess<typename ProxyBackend::ProxyBackendInterface> proxy_factory_registry,
      RemoteServerManagerInterface& remote_server_manager,
      RequiredServiceInstanceId const& required_service_instance_id, IntegrityLevel const integrity_level,
      ServiceConfig const& service_config, TraceSinkFactoryInterface const& trace_sink_factory) noexcept {
    using ProxyBackendFactoryType = ::amsr::ipc_binding::internal::ProxyBackendFactory<ProxyBackend>;

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_IPCBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<ProxyBackendFactoryType>                              // IGNORE_BLACKLIST external API
        proxy_backend_factory{std::make_unique<ProxyBackendFactoryType>(  // IGNORE_BLACKLIST external API
            binding_core, remote_server_manager, required_service_instance_id, integrity_level, service_config,
            trace_sink_factory)};

    // Register into Socal
    ::amsr::socal::internal::RequiredInstanceIdentifier const required_instance_identifier{
        ipc_binding_core::internal::InstanceIdUtil::BuildAraComInstanceIdentifier(required_service_instance_id)};

    ::amsr::core::Result<void> const result{
        // VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
        proxy_factory_registry->RegisterFactory(required_instance_identifier, std::move(proxy_backend_factory))};

    if (!result.HasValue()) {
      Logger const logger{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                          ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                          ::amsr::core::StringView{"RequiredServiceInstanceBackendInitializer"}};
      logger.TerminateOnViolation(
          "Registration of RequiredServiceInstance with SOCAL ServiceDiscovery failed.",
          [&required_service_instance_id](::ara::log::LogStream& s) {
            s << " RequiredServiceInstance (";
            LogBuilder::LogServiceInstanceId(
                s, required_service_instance_id);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ").";
          },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
  }  // VCA_IPCB_INSTANCE_IDENTIFIER_DESTRUCTOR
};

}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_REQUIRED_SERVICE_INSTANCE_BACKEND_INITIALIZER_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END

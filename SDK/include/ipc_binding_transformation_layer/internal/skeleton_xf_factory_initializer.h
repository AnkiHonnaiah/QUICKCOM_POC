/********************************************************************************************************************************
 *  COPYRIGHT
 *  -----------------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -----------------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ---------------------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Transformation layer factory initializer at skeleton side.
 *
 *      \details  -
 *
 *******************************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_INITIALIZER_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_INITIALIZER_H_

/*******************************************************************************************************************************
 *  INCLUDES
 *******************************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/instance_id_util.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"
#include "ipc_binding_transformation_layer/internal/skeleton_xf_factory.h"
#include "ipc_binding_xf_common/internal/trace/trace_sink_factory_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief Helper functions to initialize and de-initialize the SkeletonXfFactories.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProvidedServiceInstance
 */
class SkeletonXfFactoryInitializer final {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Runtime service configuration type.
   */
  using ServiceConfig = ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Type alias for IpcServiceDiscoveryInterface.
   */
  using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Type alias for ConnectionManagerSkeletonInterface.
   */
  using ConnectionManagerSkeletonInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerSkeletonInterface;

  /*!
   * \brief Type alias for SkeletonRouterInterface.
   */
  using SkeletonRouterInterface = ::amsr::ipc_binding_core::internal::SkeletonRouterInterface;

  /*!
   * \brief Type alias for the InstanceIdUtil.
   */
  using InstanceIdUtil = ::amsr::ipc_binding_core::internal::InstanceIdUtil;

  /*!
   * \brief Type alias for trace sink factory interface.
   */
  using TraceSinkFactoryInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface;

  /*!
   * \brief Type-alias for RuntimeProcessingMode.
   */
  using RuntimeProcessingMode = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeProcessingMode;

  SkeletonXfFactoryInitializer() noexcept = delete;
  SkeletonXfFactoryInitializer(SkeletonXfFactoryInitializer const&) noexcept = delete;
  auto operator=(SkeletonXfFactoryInitializer const&) noexcept -> SkeletonXfFactoryInitializer& = delete;
  SkeletonXfFactoryInitializer(SkeletonXfFactoryInitializer&&) noexcept = delete;
  auto operator=(SkeletonXfFactoryInitializer&&) noexcept -> SkeletonXfFactoryInitializer& = delete;
  ~SkeletonXfFactoryInitializer() noexcept = delete;

  /*!
   * \brief Create SkeletonXfFactory and register to the socal SkeletonXfFactoryRegistry.
   *
   * \tparam SkeletonXf                 Generated SkeletonXf type.
   * \tparam SkeletonXfFactoryRegistry  Socal SkeletonXf factory registry type.
   *
   * \param[in,out] reactor                      Reference to the reactor.
   * \param[in,out] service_discovery            Reference to the ServiceDiscovery.
   * \param[in,out] connection_manager_skeleton  Reference to the ConnectionManagerSkeleton.
   * \param[in,out] skeleton_router              Reference to the SkeletonRouterInterface.
   * \param[in,out] skeletonxf_factory_registry  Reference to the socal SkeletonXfFactoryRegistry.
   * \param[in]     trace_sink_factory           Reference to the trace sink factory.
   * \param[in]     service_config               Reference to runtime service configuration.
   * \param[in]     runtime_processing_mode      The configured RuntimeProcessingMode.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - For each provided service instance:
   *   - Instantiate SkeletonXfFactory instance.
   *   - Register provided instance identifier and SkeletonXfFactory instance with Socal SkeletonXfFactoryRegistry.
   *   - If registration with Socal failed, abort with a violation.
   * \endinternal
   */
  template <typename SkeletonXf, typename SkeletonXfFactoryRegistry>
  static void Initialize(ReactorInterface& reactor, ServiceDiscoveryInterface& service_discovery,
                         ConnectionManagerSkeletonInterface& connection_manager_skeleton,
                         SkeletonRouterInterface& skeleton_router,
                         SkeletonXfFactoryRegistry& skeletonxf_factory_registry,
                         TraceSinkFactoryInterface const& trace_sink_factory, ServiceConfig const& service_config,
                         RuntimeProcessingMode const runtime_processing_mode) noexcept {
    using SkeletonXfFactory = SkeletonXfFactory<SkeletonXf>;

    // VCA_IPCB_INITIALIZER_UNDEFINED_FUNCTION
    for (ServiceConfig::ProvidedServiceInstanceConfigRef const provided_service_instance_config_ref :
         service_config.GetProvidedServiceInstances()) {
      ::amsr::ipc_binding_core::internal::runtime_configuration::ProvidedServiceInstanceConfig const&
          provided_service_instance_config{provided_service_instance_config_ref.get()};

      ProvidedServiceInstanceId const provided_service_instance_id{
          service_config.GetId(), provided_service_instance_config.GetInstanceIdentifier(),
          service_config.GetMajorVersion(), service_config.GetMinorVersion()};

      // -- Instantiate SkeletonXfFactory and register it with SOCAL --

      // VECTOR NL AutosarC++17_10-A18.5.8: MD_IPCBINDING_AutosarC++17_10_A18.5.8_false_positive
      std::unique_ptr<SkeletonXfFactory> factory{std::make_unique<SkeletonXfFactory>(  // IGNORE_BLACKLIST external API
          reactor, service_discovery, connection_manager_skeleton, skeleton_router, trace_sink_factory,
          provided_service_instance_id, service_config, provided_service_instance_config, runtime_processing_mode)};

      ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id{
          InstanceIdUtil::BuildAraComInstanceIdentifier(provided_service_instance_id)};

      ara::core::Result<void> const register_factory_result{
          // VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
          skeletonxf_factory_registry.RegisterFactory(provided_instance_id, std::move(factory))};

      if (!register_factory_result) {
        Logger const logger{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                            ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                            ::amsr::core::StringView{"SkeletonXfFactoryInitializer"}};

        logger.TerminateOnViolation(
            "Registration of ProvidedServiceInstance with SOCAL ServiceDiscovery failed.",
            [&provided_service_instance_id](::ara::log::LogStream& s) {
              s << " ProvidedServiceInstance (";
              LogBuilder::LogServiceInstanceId(
                  s, provided_service_instance_id);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              s << ").";
            },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
    }
  }

  /*!
   * \brief Deregister SkeletonXfFactory from socal SkeletonXfFactoryRegistry.
   *
   * \tparam SkeletonXfFactoryRegistry  Socal SkeletonXf factory registry type.
   *
   * \param[in]     service_config               The runtime service config for this service.
   * \param[in,out] skeletonxf_factory_registry  Reference to the socal SkeletonXfFactoryRegistry.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - For each provided service instance.
   *   - Deregister SkeletonXfFactory instance to socal SkeletonXfFactoryRegistry for the given provided instance
   *     identifier.
   *   - Check if deregistration is successful.
   *     - If not, report failed deregistration and abort.
   * \endinternal
   */
  template <typename SkeletonXfFactoryRegistry>
  static void Deinitialize(ServiceConfig const& service_config,
                           SkeletonXfFactoryRegistry& skeletonxf_factory_registry) noexcept {
    ServiceConfig::ServiceIdentifier const service_identifier{service_config.GetId()};
    ServiceConfig::MajorVersion const service_major_version{service_config.GetMajorVersion()};
    ServiceConfig::MinorVersion const service_minor_version{service_config.GetMinorVersion()};

    // VCA_IPCB_INITIALIZER_UNDEFINED_FUNCTION
    for (ServiceConfig::ProvidedServiceInstanceConfigRef const provided_service_instance :
         service_config.GetProvidedServiceInstances()) {
      ServiceConfig::ProvidedServiceInstanceConfigRef::type::InstanceIdentifier const service_instance_identifier{
          // VCA_IPCB_INITIALIZER_UNDEFINED_FUNCTION, VCA_IPCB_VALID_RUNTIME_CONFIGURATION_OBJECT
          provided_service_instance.get().GetInstanceIdentifier()};
      ProvidedServiceInstanceId const provided_service_instance_id{service_identifier, service_instance_identifier,
                                                                   service_major_version, service_minor_version};

      ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id{
          InstanceIdUtil::BuildAraComInstanceIdentifier(provided_service_instance_id)};

      // VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
      ara::core::Result<void> const result{skeletonxf_factory_registry.DeregisterFactory(provided_instance_id)};

      if (!result.HasValue()) {
        Logger const logger{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                            ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                            ::amsr::core::StringView{"SkeletonXfFactoryInitializer"}};
        logger.TerminateOnViolation(  // COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
            "Deregistration with SOCAL skeletonxf factory registry failed.",
            [&provided_service_instance_id](::ara::log::LogStream& s) {
              s << "Failed to deregister SkeletonXfFactory (";
              LogBuilder::LogServiceInstanceId(
                  s, provided_service_instance_id);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
              s << ") with the Socal ServiceDiscovery.";
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
   * \brief Type alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_INITIALIZER_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
//
// \ID COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
//   \ACCEPT  XX
//   \REASON  CallCoverage gap reported as logger is a separate unit.
//            As the error scenario is handled as violation no further unit interactions are possible.
//            Therefore no separate test of CT or MUT level necessary.
// COV_JUSTIFICATION_END

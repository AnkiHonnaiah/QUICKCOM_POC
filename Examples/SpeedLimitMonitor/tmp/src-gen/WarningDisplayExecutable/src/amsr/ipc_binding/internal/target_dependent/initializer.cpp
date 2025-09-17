/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  WarningDisplayExecutable/src/amsr/ipc_binding/internal/target_dependent/initializer.cpp
 *        \brief  Initializer for the Transformation Layer
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_ipcbinding
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding/internal/required_service_instance_backend_initializer.h"
#include "ipc_binding/internal/target_dependent/initializer.h"
#include "ipc_binding_transformation_layer/internal/skeleton_xf_factory_initializer.h"
#include <memory>
#include "amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_proxy_xf.h"
#include "ipc_binding/internal/config_validator_interface.h"
#include "services/speedlimitservice/speedlimitservice_proxy.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace target_dependent {

// VECTOR NC AutosarC++17_10-A3.3.2: MD_IPCBINDING_AutosarC++17_10-A3.3.2_not_POD_obj_static_code_generated
// VCA_IPCB_STD_CONSTRUCTOR_CALLED
Initializer::DeployedServicesVector const Initializer::kDeployedServiceInterfaces{
    {ShortnamePath{"/ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService"}}
};

auto Initializer::ValidateRuntimeConfig(ConfigValidatorInterfacePtr validator, RuntimeConfig const& runtime_config) const noexcept -> void {
  validator->AssertGeneratedCodeMatchesConfig(runtime_config.GetServices(), kDeployedServiceInterfaces); // VCA_IPCB_INITIALIZER_CONFIG_VALIDATOR
}

// VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_config_variant
auto Initializer::Initialize(ReactorInterface& reactor,
                             RuntimeConfig const& runtime_config,
                             TraceSinkFactoryInterface const& trace_sink_factory,
                             BindingCoreType& binding_core,
                             RequiredServiceInstanceManagerInterface& required_service_instance_manager,
                             RemoteServerManagerInterface& remote_server_manager) const noexcept -> void {
  // VCA_IPCB_LIFECYCLE_MANAGER_VALID_OBJECT
  logger_.LogDebug([](::ara::log::LogStream& s) { s << "Initializing Required- and ProvidedServiceInstances."; },
                  Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

  InitializeRequiredServiceInstances(runtime_config, trace_sink_factory, binding_core,
                                     required_service_instance_manager, remote_server_manager);

  static_cast<void>(reactor);
} // VCA_IPCB_STD_UNIQUE_POINTER_DESTRUCTOR

void Initializer::InitializeRequiredServiceInstances(RuntimeConfig const& runtime_config,
                                                     TraceSinkFactoryInterface const& trace_sink_factory,
                                                     BindingCoreType& binding_core,
                                                     RequiredServiceInstanceManagerInterface& required_service_instance_manager,
                                                     RemoteServerManagerInterface& remote_server_manager) const noexcept {
  // Service Config for Service Interface 'SpeedLimitService'
  // is_generic: false
  {
    ShortnamePath::type const service_shortname_path{"/ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService"};

    logger_.LogDebug(
        [&service_shortname_path](Logger::LogStream& s) {
          // VCA_IPCB_LOGSTREAM_CONCATENATION_IS_SILENT
        s << "Initializing RequiredServiceInstances of ServiceInterfaceDeployment '" << service_shortname_path << "'.";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    ServiceConfig const& service_config{runtime_config.GetServiceConfigChecked(service_shortname_path)};

    using Proxy = ::services::speedlimitservice::proxy::SpeedLimitServiceProxy;
    using ProxyBackend = ::amsr::ipc_binding_transformation_layer::internal::gen_services::gen_speedlimitservice::SpeedLimitServiceProxyXf;

    amsr::ipc_binding::internal::RequiredServiceInstanceBackendInitializer::InitializeRequiredServiceInstances<ProxyBackend>(
            binding_core,
            Proxy::GetServiceDiscovery(),
            Proxy::GetFactoryRegistry(),
            required_service_instance_manager,
            remote_server_manager,
            service_config,
            trace_sink_factory);
  }  // VCA_IPCB_DESTRUCTOR_STD_DEFINED_FUNCTION
}


}  // namespace target_dependent
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
//   \ACCEPT  XX
//   \REASON  CallCoverage gap reported as logger is a separate unit.
//            As the error scenario is handled as violation no further unit interactions are possible.
//            Therefore no separate test of CT or MUT level necessary.
// COV_JUSTIFICATION_END



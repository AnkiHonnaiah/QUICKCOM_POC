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
/**        \file  SpeedMonitorExecutable/src/amsr/ipc_binding/internal/target_dependent/deinitializer.cpp
 *        \brief  Deinitializer for the Transformation Layer
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
#include "ipc_binding/internal/target_dependent/deinitializer.h"
#include "ipc_binding/internal/required_service_instance_backend_initializer.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"
#include "ipc_binding_transformation_layer/internal/skeleton_xf_factory_initializer.h"
#include "services/speedlimitservice/speedlimitservice_skeleton.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace target_dependent {

void Deinitializer::Deinitialize(RuntimeConfig const& runtime_config) const noexcept {
  // VCA_IPCB_LIFECYCLE_MANAGER_VALID_OBJECT
  logger_.LogDebug([](Logger::LogStream& s) { s << "Deinitializing Required- and ProvidedServiceInstances."; },
                   Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

  DeinitializeRequiredServiceInstances(runtime_config);
  DeinitializeProvidedServiceInstances(runtime_config);
}

void Deinitializer::DeinitializeProvidedServiceInstances(RuntimeConfig const& runtime_config) const noexcept {

  // Service Config for Service Interface '/ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService'
  {
    ShortnamePath::type const service_shortname_path{"/ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService"};

    logger_.LogDebug(
        [&service_shortname_path](Logger::LogStream& s) {
          // VCA_IPCB_LOGSTREAM_CONCATENATION_IS_SILENT
        s << "Deinitializing ProvidedServiceInstances of ServiceInterfaceDeployment '" << service_shortname_path << "'.";
        },
        Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    ServiceConfig const& service_config{runtime_config.GetServiceConfigChecked(service_shortname_path)};

    using Skeleton = ::services::speedlimitservice::skeleton::SpeedLimitServiceSkeleton;

    ::amsr::ipc_binding_transformation_layer::internal::SkeletonXfFactoryInitializer::Deinitialize<
      Skeleton::SkeletonBackendFactoryRegistry::BaseType>(service_config, *(Skeleton::GetSkeletonFactoryRegistry()));
  }
}

// VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_config_variant
// VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_config_variant
void Deinitializer::DeinitializeRequiredServiceInstances(RuntimeConfig const& runtime_config) const noexcept {
    static_cast<void>(runtime_config);
}

}  // namespace target_dependent
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr


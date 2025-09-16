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
 *         \brief  Provide helper function templates that (de-)initialize the required services.
 *
 *       \details  The templates are designed in a way to reduce the generated code and to be also conveniently used
 *                 in test code.
 *
 *          \unit  ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ZeroCopyXfLifecycleManagement
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_XF_REQUIRED_INITIALIZER_TEMPLATES_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_XF_REQUIRED_INITIALIZER_TEMPLATES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <memory>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/factory_interface.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery_proxy_xf_interface.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/ara_com_instance_id_xf.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief       Factory function for required service instance initializer functions.
 * \tparam      ServiceDiscoveryProxyXfType    Type of the backend service discovery for the required service.
 * \tparam      ProxyXfFactoryType             Type of the proxy backend factory.
 * \param[in]   service_idx                    The index of the corresponding service in the configuration.
 * \param[in]   required_service_instance_idx  The index of the required service instance in the configuration.
 * \return      A lambda function that creates and initializes a backend service discovery and factory of the template
 *              types.
 * \context     Init
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component private
 * \internal
 * - Return a lambda function that does the following:
 *   - Feed the configuration view with the indices passed as function parameters.
 *   - Construct and initialize the ServiceDiscoveryProxyXf.
 *   - Move the backend service discovery into the passed back inserter.
 *   - Construct the proxy backend factory with the configuration view.
 *   - Get the factory registry and register the factory with the constructed identifier. Abort if this fails.
 * \endinternal
 */
template <typename ServiceDiscoveryProxyXfType, typename ProxyXfFactoryType>
auto MakeRequiredServiceInstanceInitializer(std::size_t const service_idx,
                                            std::size_t const required_service_instance_idx) noexcept
    -> common::internal::FactoryInterface::RequiredServiceInstanceInitializerFunction {
  return [service_idx, required_service_instance_idx](
             common::internal::config::ConfigView const& zero_copy_binding_config_view,
             std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core,
             common::internal::FactoryInterface::SDProxyXfInterfacesContainerInserterType back_inserter) {
    common::internal::config::ConfigView const updated_zero_copy_binding_config_view{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        zero_copy_binding_config_view.WithServiceIdx(service_idx).WithRequiredIdx(required_service_instance_idx)};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_ZEROCOPYBINDING_A18.5.8_movedObjectDetectedAsLocal
    std::unique_ptr<ServiceDiscoveryProxyXfType> sd_proxy_xf{
        // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
        std::make_unique<ServiceDiscoveryProxyXfType>(updated_zero_copy_binding_config_view, life_cycle_manager_core)};
    sd_proxy_xf->Initialize();               // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
    back_inserter = std::move(sd_proxy_xf);  // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_ZEROCOPYBINDING_A18.5.8_movedObjectDetectedAsLocal
    std::unique_ptr<ProxyXfFactoryType> proxy_xf_factory_ptr{
        // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
        std::make_unique<ProxyXfFactoryType>(updated_zero_copy_binding_config_view, life_cycle_manager_core)};

    // VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION
    ::amsr::socal::internal::RequiredInstanceIdentifier const required_instance_id{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        AraComInstanceIdXf::BuildAraComInstanceIdentifier(
            common::internal::service_discovery::RequiredServiceInstanceIdentifier{
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().service_id.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsRequiredConfig().instance_id.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().major_version.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().minor_version.value,
            })};

    // Register ProxyXfFactory to Socal
    ServiceDiscoveryProxyXfType::ProxyType::GetFactoryRegistry()
        // VCA_ZEROCOPYBINDING_LNG_03_VALID_OBJECT_SOCAL_SERVICE_DISCOVERY_PROXY, VCA_ZEROCOPYBINDING_SPC_15_SOCAL_SD_PROXY_REGISTER_FACTORY_FUNCTION_CONTRACT_FULFILLED
        ->RegisterFactory(required_instance_id, std::move(proxy_xf_factory_ptr))
        .ConsumeError([&required_instance_id](auto const&) {
          // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
          core::internal::logging::ZeroCopyLogger const logger{"MakeRequiredServiceInstanceInitializer"};
          LOG_FATAL_AND_ABORT(logger,  // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
                              [&required_instance_id](auto& stream) {
                                stream << "Registration to Socal failed (required instance id: " << required_instance_id
                                       << "), aborting.";
                              });
        });
  };
}

/*!
 * \brief       Factory function for required service instance deinitializer functions.
 * \tparam      ServiceDiscoveryProxyXfType    Type of the backend service discovery for the required service.
 * \tparam      ProxyXfFactoryType             Type of the proxy backend factory.
 * \param[in]   service_idx                    The index of the corresponding service in the configuration.
 * \param[in]   required_service_instance_idx  The index of the required service instance in the configuration.
 * \return      A lambda function that deinitializes and destructs a backend service discovery and factory of the
 *              template types.
 * \context     Shutdown
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component private
 * \internal
 * - Return a lambda function that does the following:
 *   - Construct the Socal provided instance identifier using the config view that was fed with the passed indices.
 *   - Get the factory registry and deregister the proxy backend factory from it. Abort if this fails.
 *   - Deinitialize the service discovery proxy backend.
 *   - Destruct the service discovery proxy backend by letting the owning unique pointer go out of scope.
 * \endinternal
 */
template <typename ServiceDiscoveryProxyXfType, typename ProxyXfFactoryType>
auto MakeRequiredServiceInstanceDeinitializer(std::size_t const service_idx,
                                              std::size_t const required_service_instance_idx) noexcept
    -> common::internal::FactoryInterface::RequiredServiceInstanceDeinitializerFunction {
  return [service_idx, required_service_instance_idx](
             common::internal::config::ConfigView const& zero_copy_binding_config_view,
             std::unique_ptr<common::internal::ServiceDiscoveryProxyXfInterface> sd_proxy_xf_ptr) {
    common::internal::config::ConfigView const updated_zero_copy_binding_config_view{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        zero_copy_binding_config_view.WithServiceIdx(service_idx).WithRequiredIdx(required_service_instance_idx)};

    // VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION
    ::amsr::socal::internal::RequiredInstanceIdentifier const required_instance_id{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        AraComInstanceIdXf::BuildAraComInstanceIdentifier(
            common::internal::service_discovery::RequiredServiceInstanceIdentifier{
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().service_id.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsRequiredConfig().instance_id.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().major_version.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().minor_version.value,
            })};

    // Deregister ProxyXfFactory from Socal
    ServiceDiscoveryProxyXfType::ProxyType::GetFactoryRegistry()
        // VCA_ZEROCOPYBINDING_LNG_03_VALID_OBJECT_SOCAL_SERVICE_DISCOVERY_PROXY, VCA_ZEROCOPYBINDING_SPC_15_CALLING_FUNCTION_OUTSIDE_COMPONENT
        ->DeregisterFactory(required_instance_id)
        .ConsumeError([&required_instance_id](auto const&) {
          // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
          core::internal::logging::ZeroCopyLogger const logger{"MakeRequiredServiceInstanceDeinitializer"};
          LOG_FATAL_AND_ABORT(  // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
              logger, [&required_instance_id](auto& stream) {
                stream << "Deregistration from Socal failed (required instance id: " << required_instance_id
                       << "), aborting.";
              });
        });

    if (!sd_proxy_xf_ptr) {
      ::amsr::core::Abort(
          "MakeRequiredServiceInstanceDeinitializer(): The passed ServiceDiscoveryProxyXf pointer is invalid.");
    }
    // VCA_ZEROCOPYBINDING_PREVIOUSLY_CHECKED_POINTER_VALID, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
    sd_proxy_xf_ptr->AsInitializeStateMachine()->Deinitialize();
    // ~ServiceDiscoveryProxyXfType() since unique_ptr goes out of scope.
  };
}

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_XF_REQUIRED_INITIALIZER_TEMPLATES_H_

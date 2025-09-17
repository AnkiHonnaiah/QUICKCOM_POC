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
 *         \brief  Provide helper function templates that (de-)initialize the provided services.
 *
 *       \details  The templates are designed in a way to reduce the generated code and to be also conveniently used
 *                 in test code.
 *
 *          \unit  ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ZeroCopyXfLifecycleManagement
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_XF_PROVIDED_INITIALIZER_TEMPLATES_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_XF_PROVIDED_INITIALIZER_TEMPLATES_H_

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
#include "amsr/zero_copy_binding/common/internal/service_discovery_skeleton_xf_interface.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/ara_com_instance_id_xf.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief       Factory function for provided service instance initializer functions.
 * \tparam      SkeletonXfFactoryType           Type of the skeleton xf factory.
 * \param[in]   service_idx                     The index of the corresponding service in the configuration.
 * \param[in]   provided_service_instance_idx   The index of the provided service instance in the configuration.
 * \return      A lambda function that creates and registers a backend service discovery of the template type.
 * \context     Init
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component private
 * \internal
 * - Return a lambda function that does the following:
 *   - Feed the configuration view with the indices passed as function parameters.
 *   - Construct the Socal provided instance identifier out of the data from the configuration view.
 *   - Construct the skeleton xf factory with the configuration view.
 *   - Get the factory registry and register the factory with the constructed identifier.
 * \endinternal
 */
template <typename SkeletonXfFactoryType>
auto MakeProvidedServiceInstanceInitializer(std::size_t const service_idx,
                                            std::size_t const provided_service_instance_idx) noexcept
    -> common::internal::FactoryInterface::ProvidedServiceInstanceInitializerFunction {
  return [service_idx, provided_service_instance_idx](
             common::internal::config::ConfigView const& zero_copy_binding_config_view,
             std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core,
             common::internal::FactoryInterface::SDSkeletonXfInterfacesContainerInserterType back_inserter) {
    common::internal::config::ConfigView updated_zero_copy_binding_config_view{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        zero_copy_binding_config_view.WithServiceIdx(service_idx).WithProvidedIdx(provided_service_instance_idx)};

    // VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION
    ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        AraComInstanceIdXf::BuildAraComInstanceIdentifier(
            common::internal::service_discovery::ProvidedServiceInstanceIdentifier{
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().service_id.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsProvidedConfig().instance_id.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().major_version.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().minor_version.value,
            })};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_ZEROCOPYBINDING_A18.5.8_movedObjectDetectedAsLocal
    // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    std::unique_ptr<SkeletonXfFactoryType> skeleton_xf_factory_ptr{std::make_unique<SkeletonXfFactoryType>(
        std::move(updated_zero_copy_binding_config_view), life_cycle_manager_core)};
    // VCA_ZEROCOPYBINDING_SPC_15_VALID_SKELETON_XF_FACTORY_SINGLETON_ACCESS
    SkeletonXfFactoryType::SkeletonType::GetSkeletonFactoryRegistry()
        // VCA_ZEROCOPYBINDING_SPC_15_VALID_XF_FACTORY_POINTER
        ->RegisterFactory(provided_instance_id, std::move(skeleton_xf_factory_ptr))
        .ConsumeError([&provided_instance_id](auto const&) {
          // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
          core::internal::logging::ZeroCopyLogger const logger{"MakeProvidedServiceInstanceInitializer"};
          LOG_FATAL_AND_ABORT(  // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
              logger, [&provided_instance_id](auto& stream) {
                stream << "Registration of SkeletonFactory (provided_instance_id=" << provided_instance_id
                       << ") with Socal SkeletonXfFactory registry failed, aborting.";
              });
        });
    // Dummy assignment to keep changes in LifeCycleManagerXf minimal between default and binding_if_legacy variant.
    back_inserter = nullptr;  // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  };
}

/*!
 * \brief       Factory function for provided service instance deinitializer functions.
 * \tparam      SkeletonXfFactoryType           Type of the skeleton xf factory.
 * \param[in]   service_idx                     The index of the corresponding service in the configuration.
 * \param[in]   provided_service_instance_idx   The index of the provided service instance in the configuration.
 * \return      A lambda function that deregisters a backend service discovery of the template type.
 * \context     Shutdown
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component private
 * \internal
 * - Return a lambda function that does the following:
 *   - Construct the Socal provided instance identifier using the config view that was fed with the passed indices.
 *   - Get the factory registry and deregister the skeleton xf factory from it.
 * \endinternal
 */
template <typename SkeletonXfFactoryType>
auto MakeProvidedServiceInstanceDeinitializer(std::size_t const service_idx,
                                              std::size_t const provided_service_instance_idx) noexcept
    -> common::internal::FactoryInterface::ProvidedServiceInstanceDeinitializerFunction {
  return [service_idx, provided_service_instance_idx](
             common::internal::config::ConfigView const& zero_copy_binding_config_view,
             std::unique_ptr<common::internal::ServiceDiscoverySkeletonXfInterface>) {
    common::internal::config::ConfigView const updated_zero_copy_binding_config_view{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        zero_copy_binding_config_view.WithServiceIdx(service_idx).WithProvidedIdx(provided_service_instance_idx)};

    // VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION
    ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id{
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        AraComInstanceIdXf::BuildAraComInstanceIdentifier(
            common::internal::service_discovery::ProvidedServiceInstanceIdentifier{
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().service_id.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsProvidedConfig().instance_id.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().major_version.value,
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                updated_zero_copy_binding_config_view.AsServiceConfig().minor_version.value,
            })};

    // VCA_ZEROCOPYBINDING_SPC_15_VALID_SKELETON_XF_FACTORY_SINGLETON_ACCESS
    SkeletonXfFactoryType::SkeletonType::GetSkeletonFactoryRegistry()
        // VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION
        ->DeregisterFactory(provided_instance_id)
        .ConsumeError([&provided_instance_id](auto const&) {
          // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
          core::internal::logging::ZeroCopyLogger const logger{"MakeProvidedServiceInstanceDeinitializer"};
          LOG_FATAL_AND_ABORT(  // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
              logger, [&provided_instance_id](auto& stream) {
                stream << "Deregistration of SkeletonFactory (provided_instance_id=" << provided_instance_id
                       << ") with Socal SkeletonXfFactory registry failed, aborting.";
              });
        });
    // ~ServiceDiscoverySkeletonXfInterface() since the unique_ptr is owned by the registry.
  };
}

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_XF_PROVIDED_INITIALIZER_TEMPLATES_H_

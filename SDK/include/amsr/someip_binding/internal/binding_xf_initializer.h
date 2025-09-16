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
/*!        \file  someip_binding/internal/binding_xf_initializer.h
 *        \brief  Initialization/Deinitialization functions for someip_binding
 *        \unit   SomeIpBinding::SomeIpBinding::BindingXfInitializer
 *
 *   \complexity  After removing JsonConfigTypes (amsr::someip_binding::internal::configuration::.*?Config::.*?) from
 *                the complexity analysis (/opt/ComplexityAnalysis/excludelist.yaml), the obtained metric is reduced
 *                below the predefined threshold, hence the finding disappears.
 *                This unit can then be considered of low complexity, since JsonConfigTypes does not contain any logic
 *                and it only returns JSON into config struct, with no further interactions with the component during
 *                runtime.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_XF_INITIALIZER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_XF_INITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include "amsr/core/result.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/provided_service_instance_config.h"
#include "amsr/someip_binding/internal/configuration/required_service_instance_config.h"
#include "amsr/someip_binding/internal/configuration/service_config_wrapper_interface.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding/internal/life_cycle_manager_interface.h"
#include "amsr/someip_binding/internal/runtime_interface.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/someip_binding_xf/internal/ara_com_instance_id_xf.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief Class that holds the global instantiation of SOME/IP binding and provides API to initialize and deinitialize
 * it.
 */
class BindingXfInitializer final {
 public:
  /*!
   * \brief Type alias for E2EProfileMap.
   */
  using E2EProfileMap = ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap;

  /*!
   * \brief Type alias for life cycle manager interface.
   */
  using LifeCycleManagerInterface = ::amsr::someip_binding::internal::LifeCycleManagerInterface;

  /*!
   * \brief Type alias for a function for mapping InstanceSpecifiers to InstanceIdentifiers.
   */
  using InstanceSpecifierToInstanceIdMappingFunction =
      vac::language::UniqueFunction<void(::amsr::core::InstanceSpecifier const&, ::ara::com::InstanceIdentifier const&),
                                    ara::core::PolymorphicAllocator<std::uint8_t>>;

  /*!
   * \brief Alias for SomeIpBindingConfig.
   */
  using SomeIpBindingConfig = ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig;

  /*!
   * \brief Type-alias for the SafeIpcPollingInterface.
   */
  using SafeIpcPollingInterface = ::amsr::someip_binding_core::internal::SafeIpcPollingInterface;

  /*!
   * \brief Initialize the SOME/IP binding.
   *
   * \param[in]   life_cycle_manager      The life cycle manager.
   * \param[in]   someip_binding_config   The runtime configuration of the binding.
   * \param[in]   runtime                 The runtime instance.
   * \return      Result storing no value or error, which indicates always a positive result.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   */
  static amsr::core::Result<void> Initialize(
      LifeCycleManagerInterface& life_cycle_manager, configuration::SomeIpBindingConfig const& someip_binding_config,
      // VECTOR NC AutosarC++17_10-M7.1.2: MD_SOMEIPBINDING_AutosarC++17_10-M7.1.2_parameter_is_unique_pointer
      std::unique_ptr<RuntimeInterface>& runtime) noexcept;

  /*!
   * \brief Deinitialize the SOME/IP binding.
   *
   * \param[in]   service_interface     The runtime configuration of the service interface.
   * \param[in]   someip_binding_config The runtime configuration of the binding.
   * \return      Result storing no value or error, which indicates always a positive result.
   * \pre         The related reactor thread must be stopped.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   */
  // VECTOR NC AutosarC++17_10-M3.2.4: MD_SOMEIPBINDING_AutosarC++17_10-M3.2.4_Function_with_generated_definition
  static amsr::core::Result<void> Deinitialize(
      configuration::ServiceConfigWrapperInterface const& service_interface,
      configuration::SomeIpBindingConfig const& someip_binding_config) noexcept;

 private:
  /*!
   * \brief          Instantiate SkeletonXfFactory instances for each configured provided service instance.
   *
   * \param[in]      someip_binding_config      The runtime configuration of the binding.
   * \param[in]      life_cycle_manager         The life cycle manager.
   * \param[in]      safe_ipc_polling           Contains reactor polling function, it is called while waiting for
   *                                            command response.
   *
   * \pre All provided service instances are mapped to at least one someip binding core within the lifecycle manager.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void InitializeSkeletonXfFactories(configuration::SomeIpBindingConfig const& someip_binding_config,
                                            LifeCycleManagerInterface& life_cycle_manager,
                                            std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling) noexcept;

  /*!
   * \brief       Deregisters SkeletonXfFactory instances from SOCAL for each configured provided service instance.
   *
   * \param[in]   service_interface     The runtime configuration of the service interface.
   * \param[in]   someip_binding_config The runtime configuration of the binding.
   *
   * \pre -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void DeInitializeSkeletonXfFactories(configuration::ServiceConfigWrapperInterface const& service_interface,
                                              configuration::SomeIpBindingConfig const& someip_binding_config) noexcept;

  /*!
   * \brief Instantiates ServiceDiscoveryProxyXf instances for each configured required service instance.
   *
   * \param[in] e2e_profiles               The map containing all e2e profiles.
   * \param[in] someip_binding_config      The runtime configuration of the binding.
   * \param[in] life_cycle_manager         The life cycle manager.
   *
   * \pre All required service instances are mapped to at least one someip binding core within the lifecyle manager.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void InitializeServiceDiscoveryProxyXfs(E2EProfileMap const& e2e_profiles,
                                                 configuration::SomeIpBindingConfig const& someip_binding_config,
                                                 LifeCycleManagerInterface& life_cycle_manager) noexcept;

  /*!
   * \brief Destroys ServiceDiscoveryProxyXf instances for each configured required service instance.
   *
   * \param[in]   service_interface     The runtime configuration of the service interface.
   * \param[in]   someip_binding_config The runtime configuration of the binding.
   * \pre -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void DeInitializeServiceDiscoveryProxyXfs(
      configuration::ServiceConfigWrapperInterface const& service_interface,
      configuration::SomeIpBindingConfig const& someip_binding_config) noexcept;
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_XF_INITIALIZER_H_

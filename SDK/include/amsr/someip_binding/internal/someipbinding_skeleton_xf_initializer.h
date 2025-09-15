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
/**        \file  someipbinding_skeleton_xf_initializer.h
 *        \brief  SomeIpBinding SkeletonXf Initializer
 *        \unit   SomeIpBinding::SomeIpBinding::BindingXfInitializer
 *
 *      \details  Helper class for SomeIpBinding SkeletonXf instances initialization.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_SKELETON_XF_INITIALIZER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_SKELETON_XF_INITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ios>
#include <map>
#include <memory>
#include <utility>
#include "amsr/core/optional.h"
#include "amsr/core/string_view.h"
#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/socal/internal/factory/backend_factory_registry.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/config_wrapper_interface.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding/internal/life_cycle_manager_interface.h"
#include "amsr/someip_binding/internal/runtime_interface.h"
#include "amsr/someip_binding_core/internal/dummy_polling_wrapper.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_wrapper.h"
#include "amsr/someip_binding_core/internal/server_manager.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/ara_com_instance_id_xf.h"
#include "amsr/someip_binding_xf/internal/skeleton_xf_factory.h"
#include "ara/com/instance_identifier.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief Static helper class for initialization of SomeIpBinding Xf instances.
 */
class SomeIpBindingSkeletonXfInitializer final {
 public:
  /*!
   * \brief Map type for E2E event protection properties. Key: event ID, Value: E2E protection properties.
   */
  using E2EPropsMap =
      std::map<::amsr::someip_protocol::internal::EventId,
               std::reference_wrapper<::amsr::someip_binding::internal::configuration::E2EPropsConfig const>>;

  /*!
   * \brief Type alias for E2EProfileMap.
   */
  using E2EProfileMap = ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap;

  /*!
   * \brief Configuration object that holds the complete Provided Service instances configuration.
   */
  using ProvidedServiceInstanceConfigMap =
      std::map<::amsr::someip_binding_core::internal::ProvidedServiceInstanceId, E2EPropsMap>;

  /*!
   * \brief Type alias for ProvidedServiceInstanceConfigs.
   */
  using ProvidedServiceInstanceConfig = ::amsr::someip_binding::internal::configuration::ProvidedServiceInstanceConfig;

  /*!
   * \brief Container type alias with references of ProvidedServiceInstanceConfigs.
   */
  using ProvidedServiceInstanceConfigContainer =
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::ProvidedServiceInstanceConfigRefContainer;

  /*!
   * \brief Type alias for service interface configuration reference.
   */
  using ServiceInterfaceConfigRef =
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::ServiceInterfaceConfigRef;

  /*!
   * \brief Type alias for validated service interface configuration.
   */
  using ValidatedServiceConfigRef =
      ::amsr::someip_binding::internal::configuration::ServiceConfigWrapperInterface::ValidatedServiceConfigRef;

  /*!
   * \brief Type-alias for the SafeIpcPollingInterface.
   */
  using SafeIpcPollingInterface = ::amsr::someip_binding_core::internal::SafeIpcPollingInterface;

  /*!
   * \brief Create and initialize an SkeletonXfFactory instance.
   *
   * \tparam  ServiceInterfaceSkeletonType   Skeleton type.
   * \tparam  SkeletonXf                     SkeletonXf type.
   *
   * \param[in] someip_binding_core          Reference to the SOME/IP Binding Core.
   *                                         This object must be valid for the registered SkeletonXfFactory's
   *                                         entire life time.
   * \param[in] provided_service_instance_id SOME/IP provided service instance ID.
   * \param[in] e2e_props_map                This map shall contain all E2E props for all events of this instance.
   *                                         Key: event ID, Value: E2E protection properties.
   * \param[in] config_wrapper               Reference to the configuration wrapper.
   *                                         This object must be valid for the registered SkeletonXfFactory's
   *                                         entire life time.
   * \param[in] someip_binding_config        The runtime configuration of the binding.
   *                                         This object must be valid for the registered SkeletonXfFactory's
   *                                         entire life time.
   * \param[in] safe_ipc_polling             Contains reactor polling function, it is called while waiting for
   *                                         a command response.
   *
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *  - Build ara::com instance id from the given SOME/IP instance ID.
   *  - Create the corresponding SkeletonXfFactory
   *  - Register the factory by moving it into SOCAL.
   * \endinternal
   */
  template <typename ServiceInterfaceSkeletonType, typename SkeletonXfType>
  static void InitializeSkeletonXfFactory(
      ::amsr::someip_binding_core::internal::SomeIpBindingCoreInterface& someip_binding_core,
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& provided_service_instance_id,
      E2EPropsMap const& e2e_props_map, configuration::ConfigWrapperInterface const& config_wrapper,
      amsr::someip_binding::internal::configuration::SomeIpBindingConfig const& someip_binding_config,
      std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling) noexcept {
    ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const& e2e_profile_config_map{
        someip_binding_config.GetE2EProfileConfigMap()};

    using SkeletonXfFactory = ::amsr::someip_binding_xf::internal::SkeletonXfFactory<SkeletonXfType>;

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<SkeletonXfFactory> skeleton_xf_factory{
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        std::make_unique<SkeletonXfFactory>(someip_binding_core, provided_service_instance_id, e2e_props_map,
                                            config_wrapper, e2e_profile_config_map, safe_ipc_polling)};

    // VCA_SOMEIPBINDING_TEMPLATED_INSTANCE_IDENTIFIER_CONSTRUCTOR
    ::amsr::socal::internal::ProvidedInstanceIdentifier const instance_identifier{
        ::amsr::someip_binding_xf::internal::AraComInstanceIdXf::BuildAraComInstanceIdentifier(
            provided_service_instance_id.GetServiceId(), provided_service_instance_id.GetMajorVersion(),
            provided_service_instance_id.GetInstanceId())};

    // Register the factory into SOCAL by moving the created implementation.
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    amsr::core::Result<void> const result{ServiceInterfaceSkeletonType::GetSkeletonFactoryRegistry()
                                              // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
                                              ->RegisterFactory(instance_identifier, std::move(skeleton_xf_factory))};

    if (!result.HasValue()) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"SomeIpBindingSkeletonXfInitializer"},
              [&provided_service_instance_id](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Unable to register SOME/IP SkeletonXf Factory into SOCAL. ";
                // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
                ::amsr::someip_binding_core::internal::logging::LogBuilder::LogProvidedServiceInstanceIdToStringStream(
                    s, provided_service_instance_id);
              },
              static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief  Create and initialize multiple SkeletonXfFactory instances of the given type.
   *
   * \tparam    ServiceInterfaceSkeletonType   Skeleton type.
   * \tparam    SkeletonXf                     SkeletonXf type.
   *
   * \param[in]  life_cycle_manager     Life cycle manager to access SOME/IP Binding resources.
   * \param[in]  shortname_path         Shortname path of service.
   * \param[in]  someip_binding_config  Reference to the runtime configuration of the binding.
   *                                    The object must be valid for the passed life_cycle_manager's entire life time.
   * \param[in]  safe_ipc_polling       Contains reactor polling function, it is called while waiting for
   *                                    command response.
   *
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *   - Prepare an E2E Props map to contain all E2E protection props for all E2E events and field notifiers.
   *   - For each given provided service instances - E2E props mapping:
   *     - Find the SomeIpBindingCore in which the ProvidedServiceInstance is registered.
   *     - Create corresponding SkeletonXfFactory.
   * \endinternal
   */
  template <typename ServiceInterfaceSkeletonType, typename SkeletonXfType>
  static void InitializeSkeletonXfFactories(
      ::amsr::someip_binding::internal::LifeCycleManagerInterface& life_cycle_manager,
      ::amsr::someip_binding::internal::configuration::ShortnamePath const& shortname_path,
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig const& someip_binding_config,
      std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling) noexcept {
    // Extract Service Interface from configuration for Service shortname path
    configuration::ServiceConfigWrapperInterface::ValidatedServiceConfigRef const service_interface{
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        life_cycle_manager.GetServiceConfig(shortname_path)};

    configuration::SomeIpBindingConfig::IpcChannelConfigRefContainer const& ipc_channels{
        someip_binding_config.GetIpcChannels()};

    for (auto const& channel_config : ipc_channels) {
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
      E2EProfileMap const& e2e_profiles{someip_binding_config.GetE2EProfileConfigMap()};
      std::map<::amsr::someip_binding_core::internal::ProvidedServiceInstanceId, E2EPropsMap> const e2e_props_map{
          SomeIpBindingSkeletonXfInitializer::ReadProvidedE2EPropsMaps(
              service_interface.get().service_config_ref_,
              // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
              channel_config.get().GetProvidedServiceInstanceConfigs(), e2e_profiles)};
      configuration::ConfigWrapperInterface const& config_wrapper{*(service_interface.get().config_wrapper_)};

      // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
      static_cast<void>(std::for_each(
          e2e_props_map.begin(), e2e_props_map.end(),
          [&life_cycle_manager, &config_wrapper, &someip_binding_config, &safe_ipc_polling](
              std::pair<::amsr::someip_binding_core::internal::ProvidedServiceInstanceId, E2EPropsMap> const& p) {
            // Find the SomeIpBindingCore in which the ProvidedServiceInstance is registered.
            std::size_t find_count{0};
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
            for (auto const& someip_binding_core : life_cycle_manager.GetBindingCores()) {
              // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
              if (someip_binding_core->IsProvidedServiceInstanceIdConfigured(p.first)) {
                find_count += 1;
                if (find_count == 1) {
                  // VCA_SOMEIPBINDING_PASSING_REFERENCE
                  InitializeSkeletonXfFactory<ServiceInterfaceSkeletonType, SkeletonXfType>(
                      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
                      *someip_binding_core, p.first, p.second, config_wrapper, someip_binding_config, safe_ipc_polling);
                }
              }
            }

            if (find_count != 1) {
              ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
                  .GetAccess()
                  ->LogFatalAndAbort(
                      ::amsr::core::StringView{"SomeIpBindingProxyXfInitializer"},
                      [&find_count](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                        s << "Unable to match Provided Service Instance to a unique SOME/IP Binding Core. Expected 1, "
                             "got ";
                        s << find_count;
                      },
                      static_cast<char const*>(__func__), __LINE__);
            }
          }));
    }
  }

  /*!
   * \brief         Destroys SkeletonXfFactories instances for the given provided service instances.
   *
   * \tparam        ServiceInterfaceSkeletonType   Skeleton type.
   * \tparam        SkeletonXf                     SkeletonXf type.
   *
   * \param[in]     service                        Service Interface Config ref.
   * \param[in]     someip_binding_config          The runtime configuration of the binding.
   *
   * \pre           -
   *
   * \context       Shutdown
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - For each provided service instance config
   *   - Check if the service id and major version of the provided instance matches with the given service.
   *     - Try deregister the provided service instance from Socal.
   *     - If deregistration was not successful, log and abort.
   * \endinternal
   */
  template <typename ServiceInterfaceSkeletonType, typename SkeletonXfType>
  static void DeInitializeSkeletonXfFactories(
      ValidatedServiceConfigRef const service,
      configuration::SomeIpBindingConfig const& someip_binding_config) noexcept {
    configuration::SomeIpBindingConfig::IpcChannelConfigRefContainer const& ipc_channels{
        someip_binding_config.GetIpcChannels()};
    for (auto const& channel_config : ipc_channels) {
      for (ProvidedServiceInstanceConfig const& instance_config :
           // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
           channel_config.get().GetProvidedServiceInstanceConfigs()) {
        if ((instance_config.GetServiceIdentifier().service_identifier ==
             // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
             service.get().service_config_ref_.get().GetServiceId().service_identifier) &&
            (instance_config.GetMajorVersion().major_version ==
             // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
             service.get().service_config_ref_.get().GetMajorVersion().major_version)) {
          // Provided Service Instance ID
          ::ara::com::InstanceIdentifier const instance_identifier{
              ::amsr::someip_binding_xf::internal::AraComInstanceIdXf::BuildAraComInstanceIdentifier(
                  instance_config.GetServiceIdentifier().service_identifier,
                  instance_config.GetMajorVersion().major_version,
                  instance_config.GetInstanceIdentifier().instance_identifier)};

          // VCA_SOMEIPBINDING_TEMPLATED_INSTANCE_IDENTIFIER_CONSTRUCTOR
          ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_id{instance_identifier};

          // Deregister the factory from SOCAL.
          // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
          amsr::core::Result<void> const result{(ServiceInterfaceSkeletonType::GetSkeletonFactoryRegistry())
                                                    // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
                                                    ->DeregisterFactory(provided_id)};
          if (!result.HasValue()) {
            ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
                .GetAccess()
                ->LogFatalAndAbort(
                    ::amsr::core::StringView{"SomeIpBindingSkeletonXfInitializer"},
                    [&service, &instance_config](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                      s << "SomeIpBindingSkeletonXfInitializer::DeInitializeSkeletonXfFactories: Unable to "
                        << "Unable to deregister SOME/IP SkeletonXf Factory from SOCAL. (Service ID: 0x";
                      s << std::hex;
                      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
                      s << service.get().service_config_ref_.get().GetServiceId().service_identifier;
                      s << ", ";
                      s << "Instance ID: 0x";
                      s << instance_config.GetInstanceIdentifier().instance_identifier;
                      s << ", ";
                      s << "Major Version: 0x";
                      s << static_cast<std::uint16_t>(
                          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
                          service.get().service_config_ref_.get().GetMajorVersion().major_version);
                      s << ", ";
                      s << "Minor Version: 0x";
                      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
                      s << service.get().service_config_ref_.get().GetMinorVersion().minor_version;
                      s << ")";
                    },
                    static_cast<char const*>(__func__), __LINE__);
          }
        }
      }
    }
  }

  /*!
   * \brief Emplace the given E2E event properties into the given map using the given event id as a key.
   *        Abort in case if failure.
   *
   * \param[in, out] e2e_props_map     The result map. It might have already members that shall not collide with
   *                                   the new entry.
   * \param[in]      e2e_props_config  The E2e props config for the given event.
   *
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void EmplaceE2eEventProtectionProps(
      E2EPropsMap& e2e_props_map,
      ::amsr::someip_binding::internal::configuration::E2EPropsConfig const& e2e_props_config) noexcept;

  /*!
   * \brief Emplace the given E2E event properties map into the given map using the given Provided instance ID as a
   * key. Abort in case if failure.
   *
   * \param[in, out] provided_e2e_props_map  The result map. It might have already members that shall not collide with
   *                                         the new entry.
   * \param[in]      provided_si_id          The new Required instance ID to insert into the map (as a
   *                                         key).
   * \param[in]      e2e_props_map           The E2e protection props for the all events of the given instance.
   *
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static void EmplaceProvidedServiceInstanceConfig(
      ProvidedServiceInstanceConfigMap& provided_e2e_props_map,
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& provided_si_id,
      E2EPropsMap const& e2e_props_map) noexcept;

  /*!
   * \brief Read all E2E props configurations from given Provided service instances for the given service interface.
   * \param[in]   service_interface          A reference to ServiceInterfaceConfig.
   * \param[in]   provided_service_instances A container of references to Provided Service Instance Configs.
   * \param[in]   e2e_profile_map            Reference to the map of all E2E profiles with the profile shortname as key.
   * \return      Map containing extracted E2E props from the model (Key = ProvidedServiceInstanceId, Value = E2E Map).
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto ReadProvidedE2EPropsMaps(
      ServiceInterfaceConfigRef const service_interface,
      configuration::SomeIpBindingConfig::ProvidedServiceInstanceConfigRefContainer const& provided_service_instances,
      E2EProfileMap const& e2e_profile_map) noexcept -> ProvidedServiceInstanceConfigMap;
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_SKELETON_XF_INITIALIZER_H_

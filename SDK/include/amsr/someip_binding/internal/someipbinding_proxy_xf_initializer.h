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
/**        \file  someipbinding_proxy_xf_initializer.h
 *        \brief  SomeIpBinding ProxyXf Initializer
 *        \unit   SomeIpBinding::SomeIpBinding::BindingXfInitializer
 *
 *      \details  Helper class for SomeIpBinding ProxyXf instances initialization.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_PROXY_XF_INITIALIZER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_PROXY_XF_INITIALIZER_H_

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
#include "amsr/socal/internal/service_discovery/proxy_service_discovery.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/config_wrapper_interface.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding/internal/life_cycle_manager_interface.h"
#include "amsr/someip_binding_core/internal/client_manager.h"
#include "amsr/someip_binding_core/internal/client_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/server_manager.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"
#include "amsr/someip_binding_xf/internal/ara_com_instance_id_xf.h"
#include "amsr/someip_binding_xf/internal/service_discovery/service_discovery_proxy_xf.h"
#include "ara/com/instance_identifier.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief Static helper class for initialization of SomeIpBinding Xf instances.
 */
class SomeIpBindingProxyXfInitializer final {
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
   * \brief Type alias for the ServiceListener.
   */
  using ServiceListenerUniquePtr =
      std::unique_ptr<::amsr::someip_binding_core::internal::service_discovery::ServiceListenerInterface>;

  /*!
   * \brief Configuration object that holds the complete Required Service instances configuration.
   */
  using RequiredServiceInstanceConfigMap =
      std::map<::amsr::someip_binding_core::internal::RequiredServiceInstanceId, E2EPropsMap>;

  /*!
   * \brief Container type alias with references of RequiredServiceInstanceConfigs.
   */
  using RequiredServiceInstanceConfigContainer =
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::RequiredServiceInstanceConfigRefContainer;

  /*!
   * \brief Type alias for RequiredServiceInstanceConfigs.
   */
  using RequiredServiceInstanceConfig = ::amsr::someip_binding::internal::configuration::RequiredServiceInstanceConfig;

  /*!
   * \brief Container type alias for service listeners.
   */
  using ServiceListenerContainer = ara::core::Vector<ServiceListenerUniquePtr>;

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
   * \brief Create and initialize SdProxyXf instance
   *
   * \tparam ServiceInterfaceProxyType     Proxy type
   * \tparam ProxyBackendInterfaceType     Interface type of ProxyXf
   * \tparam ProxyXf                       ProxyXf type
   *
   * \param[in] someip_binding_core        The SOME/IP binding core to request the required service instance.
   * \param[in] required_si_id             SOME/IP required service instance ID
   * \param[in] e2e_props_map              This map shall contain all E2E props for all events of this instance. Key:
   *                                       event ID, Value: E2E protection properties.
   * \param[in] config_wrapper             Reference to the configuration wrapper
   * \param[in] e2e_profile_config_map     Reference to the E2EProfileConfig map
   *
   * \pre         Configured required service instances have to be registered in a someip binding core registered in the
   *              lifecycle manager before use.
   * \context     Init
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *  - Build ara::com instance id from the given SOME/IP instance ID.
   *  - Create SD Proxy XF
   *  - Register the created instance by moving it into SOCAL.
   * \endinternal
   */
  template <typename ServiceInterfaceProxyType, typename ProxyBackendInterfaceType, typename ProxyXf>
  static void InitializeSdProxyXf(
      ::amsr::someip_binding_core::internal::SomeIpBindingCoreInterface& someip_binding_core,
      ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& required_si_id,
      E2EPropsMap const& e2e_props_map, configuration::ConfigWrapperInterface const& config_wrapper,
      E2EProfileMap const& e2e_profile_config_map) noexcept {
    // Type definitions
    using SdProxyXfType = ::amsr::someip_binding_xf::internal::service_discovery::ServiceDiscoveryProxyXf<
        ProxyXf, ProxyBackendInterfaceType, ServiceInterfaceProxyType>;

    // Required Service Instance ID
    ::amsr::someip_protocol::internal::ServiceId const service_id{required_si_id.GetServiceId()};
    ::amsr::someip_protocol::internal::MajorVersion const major_version{required_si_id.GetMajorVersion()};
    ::amsr::someip_protocol::internal::InstanceId const instance_id{required_si_id.GetInstanceId()};
    ::ara::com::InstanceIdentifier const instance_identifier{
        ::amsr::someip_binding_xf::internal::AraComInstanceIdXf::BuildAraComInstanceIdentifier(
            service_id, major_version, instance_id)};

    // Request the required service instance to the SOME/IP Binding Core.
    amsr::core::Result<someip_binding_core::internal::RequiredServiceInstance> rsi_request_result{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        someip_binding_core.RequestRequiredServiceInstance(required_si_id)};

    if (!rsi_request_result.HasValue()) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"SomeIpBindingProxyXfInitializer"},
              [&required_si_id](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Unable to request Required Service Instance from SOME/IP Binding Core. ";
                ::amsr::someip_binding_core::internal::logging::LogBuilder::LogRequiredServiceInstanceIdToStringStream(
                    s, required_si_id);
              },
              static_cast<char const*>(__func__), __LINE__);
    }

    // Create SD Proxy XF
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
    std::unique_ptr<SdProxyXfType> sd_proxy_xf{std::make_unique<SdProxyXfType>(
        required_si_id, e2e_props_map, config_wrapper, e2e_profile_config_map, std::move(rsi_request_result.Value()))};
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    ::amsr::socal::internal::RequiredInstanceIdentifier const req_id{instance_identifier};
    // Register into Socal
    ::amsr::core::Result<void> const result{
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        // VCA_SOMEIPBINDING_TRIVIAL_VALID_REFERENCE
        // VCA_SOMEIPBINDING_PROXY_SINGLETON_PTR
        ServiceInterfaceProxyType::GetFactoryRegistry()->RegisterFactory(req_id, std::move(sd_proxy_xf))};
    if (!result.HasValue()) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"SomeIpBindingProxyXfInitializer"},
              [&required_si_id](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Unable to register SOME/IP SdProxyXf into socal. ";
                ::amsr::someip_binding_core::internal::logging::LogBuilder::LogRequiredServiceInstanceIdToStringStream(
                    s, required_si_id);
              },
              static_cast<char const*>(__func__), __LINE__);
    }
  }  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT

  /*!
   * \brief Create and initialize multiple SdProxyXf instances of the given type.
   *
   * \tparam ServiceInterfaceProxyType   Proxy type
   * \tparam ProxyBackendInterfaceType   Interface type of ProxyXf
   * \tparam ProxyXf                     ProxyXf type
   *
   * \param[in]     e2e_profiles               The map containing all e2e profiles.
   * \param[in]     someip_binding_config      The runtime configuration of the binding.
   * \param[in,out] life_cycle_manager         Life cycle manager to store the SdProxyXf instances.
   * \param[in]     shortname_path             Shortname path of service
   *
   * \pre         Configured required service instances have to be registered in a someip binding core registered in the
   *              lifecycle manager before use.
   * \context     Init
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *   - Prepare an E2E Props map to contain all E2E protection props for all E2E events and field notifiers.
   *   - For each given required service instances - E2E props mapping:
   *     - Find the SomeIpBindingCore in which the RequiredServiceInstance is registered.
   *     - Create corresponding SdProxyXf.
   * \endinternal
   */
  template <typename ServiceInterfaceProxyType, typename ProxyBackendInterfaceType, typename ProxyXf>
  static void InitializeSdProxyXfs(
      E2EProfileMap const& e2e_profiles, configuration::SomeIpBindingConfig const& someip_binding_config,
      ::amsr::someip_binding::internal::LifeCycleManagerInterface& life_cycle_manager,
      ::amsr::someip_binding::internal::configuration::ShortnamePath const& shortname_path) noexcept {
    // Extract Service Interface from configuration for Service shortname path
    ValidatedServiceConfigRef const service_interface{
        life_cycle_manager.GetServiceConfig(shortname_path)};  // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED

    // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
    for (configuration::IpcChannelConfig const& ipc_channel : someip_binding_config.GetIpcChannels()) {
      std::map<::amsr::someip_binding_core::internal::RequiredServiceInstanceId, E2EPropsMap> const
          required_e2e_props_map{SomeIpBindingProxyXfInitializer::ReadRequiredE2EPropsMaps(
              // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
              service_interface.get().service_config_ref_, ipc_channel.GetRequiredServiceInstanceConfigs(),
              e2e_profiles)};

      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      static_cast<void>(std::for_each(
          required_e2e_props_map.begin(), required_e2e_props_map.end(),
          [&life_cycle_manager, &service_interface, &e2e_profiles](
              std::pair<::amsr::someip_binding_core::internal::RequiredServiceInstanceId, E2EPropsMap> const& p) {
            // Find the SomeIpBindingCore in which the RequiredServiceInstance is registered.
            std::size_t find_count{0};
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
            for (auto const& someip_binding_core : life_cycle_manager.GetBindingCores()) {
              // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
              if (someip_binding_core->IsRequiredServiceInstanceIdConfigured(p.first)) {
                find_count += 1;
                if (find_count == 1) {
                  // Manage lifecycle
                  // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
                  InitializeSdProxyXf<ServiceInterfaceProxyType, ProxyBackendInterfaceType, ProxyXf>(
                      *someip_binding_core, p.first, p.second, *(service_interface.get().config_wrapper_),
                      e2e_profiles);
                }
              }
            }

            if (find_count != 1) {
              ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
                  .GetAccess()
                  ->LogFatalAndAbort(
                      ::amsr::core::StringView{"SomeIpBindingProxyXfInitializer"},
                      [&find_count](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                        s << "Unable to match Required Service Instance to a unique SOME/IP Binding Core. Expected 1, "
                             "got ";
                        s << find_count;
                      },
                      static_cast<char const*>(__func__), __LINE__);
            }
          }));
    }
  }  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT

  /*!
   * \brief Emplace the given E2E event properties into the given map using the given event id as a key.
   *        Abort in case if failure.
   *
   * \param[in, out] e2e_props_map        The result map. It might have already members that shall not collide with the
   *                                      new entry.
   * \param[in]      e2e_props_config     The E2e props config for the given event.
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
   * \brief Emplace the given E2E event properties map into the given map using the given Required instance ID as a key.
   *        Abort in case if failure.
   *
   * \param[in, out] required_e2e_props_map  The result map. It might have already members that shall not collide with
   *                                         the new entry.
   * \param[in]      required_si_id          The new Required instance ID to insert into the map (as a
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
  static void EmplaceRequiredServiceInstanceConfig(
      RequiredServiceInstanceConfigMap& required_e2e_props_map,
      ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& required_si_id,
      E2EPropsMap const& e2e_props_map) noexcept;

  /*!
   * \brief Destroys ServiceDiscoveryProxyXf instances for given required service instance.
   *
   * \tparam        ServiceInterfaceProxyType  Proxy type
   * \tparam        ProxyBackendInterfaceType  Interface type of ProxyXf
   * \tparam        ProxySDType                Proxy service discovery type
   * \param[in]     service                    Optional Service Interface Config ref.
   * \param[in]     someip_binding_config      The runtime configuration of the binding.
   * \pre           -
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - For each required service instance config
   *   - Check if the service id and major version of the required instance matches with the given service.
   *     - Get the factory registry of the ServiceInterfaceProxy.
   *     - Try deregister the service instance from Socal.
   *     - If deregistration was not successful, log and abort.
   * \endinternal
   */
  template <typename ServiceInterfaceProxyType, typename ProxyBackendInterfaceType,
            typename ProxySDType = ::amsr::socal::internal::service_discovery::ProxyServiceDiscovery>
  static void DeInitializeServiceDiscoveryProxyXfs(
      ValidatedServiceConfigRef const service,
      configuration::SomeIpBindingConfig const& someip_binding_config) noexcept {
    configuration::SomeIpBindingConfig::IpcChannelConfigRefContainer const& ipc_channels{
        someip_binding_config.GetIpcChannels()};

    for (auto const& channel_config : ipc_channels) {
      for (RequiredServiceInstanceConfig const& instance_config :
           // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
           channel_config.get().GetRequiredServiceInstanceConfigs()) {
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        if ((service.get().service_config_ref_.get().GetServiceId().service_identifier ==
             instance_config.GetServiceIdentifier().service_identifier) &&
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            (service.get().service_config_ref_.get().GetMajorVersion().major_version ==
             instance_config.GetMajorVersion().major_version)) {
          // Required Service Instance ID
          ::ara::com::InstanceIdentifier const instance_identifier{
              ::amsr::someip_binding_xf::internal::AraComInstanceIdXf::BuildAraComInstanceIdentifier(
                  instance_config.GetServiceIdentifier().service_identifier,
                  instance_config.GetMajorVersion().major_version,
                  instance_config.GetInstanceIdentifier().instance_identifier)};

          // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
          ::amsr::socal::internal::RequiredInstanceIdentifier const req_id{instance_identifier};
          // Deregister from Socal.
          // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
          ::amsr::core::Result<void> const result{
              // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
              // VCA_SOMEIPBINDING_PROXY_SINGLETON_PTR
              ServiceInterfaceProxyType::GetFactoryRegistry()->DeregisterFactory(req_id)};
          if (!result.HasValue()) {
            ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
                .GetAccess()
                ->LogFatalAndAbort(
                    ::amsr::core::StringView{"SomeIpBindingProxyXfInitializer"},
                    [&service, &instance_config](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                      s << "Unable to deregister SOME/IP SdProxyXf from socal. (Service ID: 0x";
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
                      s << instance_config.GetMinorVersion().minor_version;
                      s << ")";
                    },
                    static_cast<char const*>(__func__), __LINE__);
          }
        }
      }
    }
  }

  /*!
   * \brief Read all E2E props configurations from given Required service instances for the given service interface.
   * \param[in]   service_interface          A reference to ServiceInterfaceConfig.
   * \param[in]   required_service_instances A container of references to Required Service Instance Configs.
   * \param[in]   e2e_profile_map            Reference to the map of all E2E profiles with the profile shortname as key
   * \return      Map containing extracted E2E props from the model (Key = RequiredServiceInstance, Value = E2E Map).
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto ReadRequiredE2EPropsMaps(
      ServiceInterfaceConfigRef const service_interface,
      configuration::SomeIpBindingConfig::RequiredServiceInstanceConfigRefContainer const& required_service_instances,
      E2EProfileMap const& e2e_profile_map) noexcept -> RequiredServiceInstanceConfigMap;

 private:
  /*!
   * \brief       Calculates the E2E offset
   * \details     Performs the calculation based on the event serialization type.
   * \param[in]   service_interface         ServiceInterfaceConfig to get the event/field from.
   * \param[in]   event_id                  Event id to find event/field.
   * \param[in]   e2e_prop_offset           E2E offset from E2E props in case serialization is signal based.
   * \return      E2E offset.
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static std::size_t CalculateE2EOffset(ServiceInterfaceConfigRef const& service_interface,
                                        ::amsr::someip_protocol::internal::EventId const event_id,
                                        std::size_t const e2e_prop_offset) noexcept;
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_PROXY_XF_INITIALIZER_H_

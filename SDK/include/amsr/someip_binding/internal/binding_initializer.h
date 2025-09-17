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
/*!        \file  someip_binding/internal/binding_initializer.h
 *        \brief  Initializes SOME/IP binding.
 *        \unit   SomeIpBinding::SomeIpBinding::BindingInitializer
 *
 *   \complexity  This unit uses many simple getters of config objects for mapping data types that lead to many inter
 *                unit calls. WIZRD-1802 was created to defer extracting a new unit after other pending changes.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_INITIALIZER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_INITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/someip_binding/internal/binding_core_initializer.h"
#include "amsr/someip_binding/internal/binding_xf_initializer.h"
#include "amsr/someip_binding/internal/configuration/ipc_channel_config.h"
#include "amsr/someip_binding/internal/configuration/service_interface_config.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding/internal/configuration_provider.h"
#include "amsr/someip_binding/internal/json_data_loader.h"
#include "amsr/someip_binding/internal/life_cycle_manager.h"
#include "amsr/someip_binding/internal/life_cycle_manager_interface.h"
#include "amsr/someip_binding/internal/socal_runtime_wrapper.h"
#include "amsr/someip_binding/internal/someipbinding_default_configuration.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_wrapper.h"
#include "amsr/someip_binding_core/internal/someip_binding_core.h"
#include "amsr/someip_binding_generic_layer/internal/generic_layer_initializer.h"
#include "amsr/steady_timer/timer_manager_interface.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief   Initialization class for SOME/IP binding.
 *
 * \tparam BindingXfInitializerType     Binding Initializer type.
 * \tparam RuntimeInterfaceType         Runtime type
 *
 */
template <typename BindingXfInitializerType = ::amsr::someip_binding::internal::BindingXfInitializer,
          typename GenericLayerInitializerType =
              ::amsr::someip_binding_generic_layer::internal::GenericLayerInitializer,
          typename RuntimeInterfaceType = ::amsr::someip_binding::internal::SocalRuntimeWrapper<>>
class BindingInitializer final {
 public:
  /*!
   * \brief Type-alias for the SafeIpcPollingWrapper.
   */
  using SafeIpcPollingWrapper = ::amsr::someip_binding_core::internal::SafeIpcPollingWrapper;

  /*!
   * \brief       Initializes the component if it has not been initialized before
   * \tparam      SomeIpBindingConfigJsonParserType type of the json data parser.
   * \return      An amsr::core::Result with an error code or empty if initialization succeeds
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the component is not already initialized,
   *   - Trigger the initialization.
   * - Otherwise, return initialization error.
   * \endinternal
   */
  template <typename SomeIpBindingConfigJsonParserType = configuration::parsing::SomeIpBindingConfigJsonParser>
  static auto InitializeComponent() noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{::amsr::someip_binding_core::internal::MakeErrorCode(
        ::amsr::someip_binding_core::internal::SomeIpBindingErrc::wrong_init_sequence, 0,
        "SomeIpBinding is already initialized!")};
    if (!IsComponentInitialized()) {
      result = Initialize<SomeIpBindingConfigJsonParserType>();
    }
    return result;
  }

  /*!
   * \brief       Deinitializes the component if it has been initialized before
   * \return      An empty amsr::core::Result
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - If the component is currently initalized,
   *   - Trigger the deinitialization and destruction of the someip_binding.
   * - Otherwise, silently ignore the request by returning a success response.
   * \endinternal
   */
  static auto DeinitializeComponent() noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{};
    if (IsComponentInitialized()) {
      // We should allow deinitialization even when the previous call to amsr::core::Initialize() has failed.
      result = Deinitialize();
    }
    return result;
  }

  /*!
   * \brief       Return the initialization status of the component
   * \return      True if the component is initialized, false otherwise
   *
   * \pre         -
   * \context     Init, Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto IsComponentInitialized() noexcept -> bool { return LifeCycleManager::GetInstance().IsInitialized(); }

  /*!
   * \brief       Iterate all registered connection actions to start the connection of the SomeIpBindingCore instances
   *              to the SomeIpDaemon. If the connection retrial feature is disabled, an error in the first connection
   *              attempt will lead to an abort. Otherwise, a connection retrial timer is scheduled that attempts to
   *              connect periodically in a timer. Once the connection is established, the service discovery for all
   *              registered required service instances is started.
   *
   * \pre         Initialize() has been called.
   *
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static void Start() noexcept {
    configuration::SomeIpBindingConfig& someip_config{
        LifeCycleManager::GetInstance().GetAccess()->GetSomeIpBindingConfig()};

    bool const connection_retrial_enabled{someip_config.GetSomeIpDaemonConnectionRetrialEnabled()};

    // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
    for (auto const& connection_action : LifeCycleManager::GetInstance().GetAccess()->GetConnectionActions()) {
      // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
      ::amsr::core::Result<void> const connection_result{connection_action->TryConnect()};

      if ((!connection_result.HasValue()) && (!connection_retrial_enabled)) {
        ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
            .GetAccess()
            ->LogFatalAndAbort(
                ::amsr::core::StringView{"BindingInitializer"},
                [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                  s << "Connection establishment between SomeIpDaemonClient and SomeIpDaemon failed.";
                },
                static_cast<char const*>(__func__), __LINE__);
      }
    }
  }

  /*!
   * \brief       Remove all instance specifiers mapping for both required and provided service instances.
   *
   * \param[in]   config  The runtime configuration of the binding.
   * \param[in]   runtime The runtime instance.
   *
   * \pre         -
   *
   * \context     Shutdown
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec
   *   requires runtime != nullptr;
   * \endspec
   */
  static void CleanInstanceSpecifiersToInstanceIdentifiersMapping(
      configuration::SomeIpBindingConfig const& config,
      // VECTOR NC AutosarC++17_10-M7.1.2: MD_SOMEIPBINDING_AutosarC++17_10-M7.1.2_parameter_is_unique_pointer
      std::unique_ptr<RuntimeInterface>& runtime) noexcept {
    // The functionality for handling and processing ara com instance identifiers and specifiers is duplicated currently
    // in both XF and Generic layers. This, however, will be later unified into one common util class within the core.
    // So, for now, use the utilities provided by the transformation layer.
    using AraComInstanceIdUtils = ::amsr::someip_binding_xf::internal::AraComInstanceIdXf;

    configuration::SomeIpBindingConfig::IpcChannelConfigRefContainer const& ipc_channels{
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        config.GetIpcChannels()};

    for (auto const& ipc_channel : ipc_channels) {
      // Required service instance mapping.
      configuration::SomeIpBindingConfig::IpcChannelConfig::RequiredServiceInstanceConfigRefContainer const
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          rsi_container{ipc_channel.get().GetRequiredServiceInstanceConfigs()};

      for (auto const& rsi : rsi_container) {
        ara::com::InstanceIdentifier const instance_identifier{AraComInstanceIdUtils::BuildAraComInstanceIdentifier(
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            rsi.get().GetServiceIdentifier().service_identifier,
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            rsi.get().GetMajorVersion().major_version,
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            rsi.get().GetInstanceIdentifier().instance_identifier)};
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        for (auto const& specifier : rsi.get().GetInstanceSpecifiers()) {
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          ::amsr::core::InstanceSpecifier const instance_specifier{specifier.instance_specifier};
          // VCA_SOMEIPBINDING_RUNTIME_OBJECT_LIFECYCLE
          runtime->RemoveRequiredInstanceSpecifierEntry(instance_specifier, instance_identifier);
        }
      }

      // Provided service instance mapping.
      configuration::SomeIpBindingConfig::IpcChannelConfig::ProvidedServiceInstanceConfigRefContainer const
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          psi_container{ipc_channel.get().GetProvidedServiceInstanceConfigs()};
      for (auto const& psi : psi_container) {
        ara::com::InstanceIdentifier const instance_identifier{AraComInstanceIdUtils::BuildAraComInstanceIdentifier(
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            psi.get().GetServiceIdentifier().service_identifier,
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            psi.get().GetMajorVersion().major_version,
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            psi.get().GetInstanceIdentifier().instance_identifier)};
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        for (auto const& specifier : psi.get().GetInstanceSpecifiers()) {
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          ::amsr::core::InstanceSpecifier const instance_specifier{specifier.instance_specifier};
          // VCA_SOMEIPBINDING_RUNTIME_OBJECT_LIFECYCLE
          runtime->RemoveProvidedInstanceSpecifierEntry(instance_specifier, instance_identifier);
        }
      }
    }
  }

 private:
  /*!
   * \brief       Initialize the Binding.
   * \tparam      SomeIpBindingConfigJsonParserType type of the json data parser.
   * \return      Result storing error or no value, which indicates a positive result.
   * \error       amsr::someip_binding_core::internal::SomeIpBindingErrc::json_loading_failure
   *                if component specific JSON configuration could not be found or loaded.
   *              amsr::someip_binding_core::internal::SomeIpBindingErrc::json_parsing_failure
   *                if component specific JSON configuration parsing failed.
   *              amsr::someip_binding_core::internal::SomeIpBindingErrc::invalid_json_config
   *                if no matching service interface exists for an instance.
   *
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \internal
   * - Get the configuration for the component.
   * - If a configuration is available.
   *   - Emplace the configuration into a unique pointer for further use.
   *   - Initialize the life-cycle manager with the configuration to construct the binding.
   *   - Store the result of the initialization.
   * - Otherwise, store the error as result.
   * - Return the result.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_STL_exceptions_noexcept
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.5.3_STL_exceptions_noexcept
  template <typename SomeIpBindingConfigJsonParserType = configuration::parsing::SomeIpBindingConfigJsonParser>
  static auto Initialize() noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{};

    // Initialize logging singleton
    if (!::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance().IsInitialized()) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance().Create();
    }

    // Get configuration
    ConfigurationProvider<JsonDataLoader, SomeIpBindingConfigJsonParserType> const configuration_provider{};

    amsr::core::Result<std::unique_ptr<configuration::SomeIpBindingConfig>> configuration_result{
        configuration_provider.GetConfiguration(kDefaultConfigJsonPath)};

    // Initialize
    if (configuration_result.HasValue()) {
      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      std::unique_ptr<configuration::SomeIpBindingConfig> someip_binding_config{
          std::move(configuration_result.Value())};
      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      // VCA_SOMEIPBINDING_UNKNOWN_CONSTRUCTOR_TEMPLATE_INSTANCE_FROM_TEST
      std::unique_ptr<RuntimeInterfaceType> runtime{std::make_unique<RuntimeInterfaceType>()};

      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      std::shared_ptr<SafeIpcPollingWrapper> const poll_wrapper{
          // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
          // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULL_POINTER
          // VCA_SOMEIPBINDING_FAILED_TO_DETERMINE_POINTS_TO_SET
          std::make_shared<SafeIpcPollingWrapper>(std::move(runtime->GetProcessPollingFunction()))};

      // Connection retrial information
      // VCA_SOMEIPBINDING_SOMEIP_BINDING_CONFIG_OBJECT_LIFECYCLE
      bool const connection_retrial_enabled{someip_binding_config->GetSomeIpDaemonConnectionRetrialEnabled()};
      std::chrono::milliseconds const connection_retrial_interval_ms{
          // VCA_SOMEIPBINDING_SOMEIP_BINDING_CONFIG_OBJECT_LIFECYCLE
          someip_binding_config->GetSomeIpDaemonConnectionRetrialIntervalMs()};

      // Set timer manager
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      ::amsr::steady_timer::TimerManagerInterface& timer_manager{runtime->GetSteadyTimerManager()};

      // Raw pointer is required due to ::amsr::steady_timer::TimerInterface base class
      ::amsr::steady_timer::TimerManagerInterface* timer_manager_raw_pointer{&timer_manager};

      // Acquire reactor reference before moving the runtime
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      osabstraction::io::reactor1::Reactor1& reactor{runtime->GetReactor()};

      LifeCycleManager::GetInstance().Create(std::move(someip_binding_config), std::move(runtime));

      configuration::SomeIpBindingConfig& config{LifeCycleManager::GetInstance().GetAccess()->GetSomeIpBindingConfig()};

      configuration::SomeIpBindingConfig::IpcChannelConfigRefContainer const& ipc_channels{config.GetIpcChannels()};

      // Create and initialize a SomeIpBindingCore per IPC channel.
      for (auto const& channel_config : ipc_channels) {
        // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
        // VECTOR NC AutosarC++17_10-A5.0.1: MD_SOMEIPBINDING_AutosarC++17_10_A5.0.1_falsepositive
        // VECTOR NC VectorC++-V5.0.1: MD_SOMEIPBINDING_VectorC++-V5.0.1_unsequenced_call_false_positive
        std::shared_ptr<::amsr::someip_binding_core::internal::SomeIpBindingCore> const someip_binding_core{
            // VCA_SOMEIPBINDING_PASSING_REFERENCE
            BindingCoreInitializer::Create(config, channel_config, poll_wrapper, reactor)};

        configuration::IpcChannelConfig::RequiredServiceInstanceConfigRefContainer const& rsi_configs{
            channel_config.get().GetRequiredServiceInstanceConfigs()};
        configuration::IpcChannelConfig::ProvidedServiceInstanceConfigRefContainer const& psi_configs{
            channel_config.get().GetProvidedServiceInstanceConfigs()};

        // Register all required and provided service instances into the message channel associated core
        BindingCoreInitializer::InitializeRequiredServiceInstances(rsi_configs, *someip_binding_core);

        BindingCoreInitializer::InitializeProvidedServiceInstances(psi_configs, *someip_binding_core);

        // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
        std::unique_ptr<LifeCycleManager::ConnectionAction> connection_action{
            std::make_unique<someip_binding_core::internal::ConnectionAction>(
                someip_binding_core, timer_manager_raw_pointer, connection_retrial_enabled,
                connection_retrial_interval_ms)};

        // Generic layer initialization.
        {
          typename GenericLayerInitializerType::RequiredServiceInstanceIds const rsi_ids{
              CollectGenericRequiredServiceInstanceIds(rsi_configs)};

          GenericLayerInitializerType::RegisterRequiredServiceInstances(rsi_ids, *someip_binding_core);
        }

        LifeCycleManager::GetInstance().GetAccess()->AddSomeIpBindingCore(someip_binding_core,
                                                                          std::move(connection_action));
      }

      std::unique_ptr<RuntimeInterface>& runtime_interface{LifeCycleManager::GetInstance().GetAccess()->GetRuntime()};

      RegisterInstanceSpecifiersToInstanceIdentifiersMapping(config, runtime_interface);

      LifeCycleManagerInterface& life_cycle_manager{*LifeCycleManager::GetInstance().GetAccess()};

      // Initialize All SdProxyXfs and SkeletonXfFactories using the configuration. For the SdProxyXfs, also push them
      // into the life_cycle_manager
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
      result = BindingXfInitializerType::Initialize(life_cycle_manager, config, runtime_interface);
    } else {
      result.EmplaceError(configuration_result.Error());
    }
    return result;
  }

  /*!
   * \brief       Deinitialize the Binding.
   * \return      Result storing no value or error, which indicates a positive result.
   *
   * \pre         This function must only be called once.
   * \pre         Reactor needs to be stopped to be sure that no timer callback is currently executed and the timer can
   *              be safely destructed.
   * \pre         Must be called before the timer manager is deinitialized.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \internal
   * - Deinitialize the life-cycle manager to destroy the binding.
   * - Return the result.
   * \endinternal
   */
  static auto Deinitialize() noexcept -> amsr::core::Result<void> {
    configuration::SomeIpBindingConfig& config{LifeCycleManager::GetInstance().GetAccess()->GetSomeIpBindingConfig()};
    amsr::someip_binding::internal::configuration::ServiceConfigWrapperInterface& config_wrapper_interface{
        // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
        LifeCycleManager::GetInstance().GetAccess()->GetServiceConfigWrapper()};

    // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
    amsr::core::Result<void> const result{BindingXfInitializerType::Deinitialize(config_wrapper_interface, config)};

    // Generic layer deinitialization.
    {
      configuration::SomeIpBindingConfig::IpcChannelConfigRefContainer const& ipc_channels{config.GetIpcChannels()};

      for (auto const& channel_config : ipc_channels) {
        typename GenericLayerInitializerType::RequiredServiceInstanceIds const rsi_ids{
            CollectGenericRequiredServiceInstanceIds(
                // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
                channel_config.get().GetRequiredServiceInstanceConfigs())};

        GenericLayerInitializerType::DeregisterRequiredServiceInstances(rsi_ids);
      }
    }

    LifeCycleManager::GetInstance().Destroy();
    return result;
  }

  /*!
   * \brief       Collect the set of generic required service instance IDs.
   * \param[in]   rsi_configs         Required service instance config set.
   * \return      Set of generic required service instance IDs that need to be initialized.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto CollectGenericRequiredServiceInstanceIds(
      configuration::IpcChannelConfig::RequiredServiceInstanceConfigRefContainer const& rsi_configs)
      -> someip_binding_generic_layer::internal::GenericLayerInitializer::RequiredServiceInstanceIds {
    someip_binding_generic_layer::internal::GenericLayerInitializer::RequiredServiceInstanceIds rsi_ids{};

    for (std::reference_wrapper<configuration::RequiredServiceInstanceConfig const> const required_instance_ref :
         rsi_configs) {
      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      configuration::RequiredServiceInstanceConfig const& required_instance{required_instance_ref.get()};

      // If the current RSI is generic, add it to the container. Else, skip it.
      if (required_instance.GetServiceInterfaceRef().GetIsGeneric().is_generic) {
        // clang-format off
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        rsi_ids.emplace_back(required_instance.GetServiceIdentifier().service_identifier, // COV_SOMEIPBINDING_GENERIC_PROXY_MISSING_INTEGRATION
                             required_instance.GetMajorVersion().major_version,           // COV_SOMEIPBINDING_GENERIC_PROXY_MISSING_INTEGRATION
                             required_instance.GetMinorVersion().minor_version,
                             required_instance.GetInstanceIdentifier().instance_identifier);
        // clang-format on
      }
    }

    return rsi_ids;
  }

  /*!
   * \brief       Initialize mapping of required/provided service instances to ports.
   *
   * \param[in]   config  The runtime configuration of the binding.
   * \param[in]   runtime The runtime instance.
   *
   * \pre         -
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec
   *   requires runtime != nullptr;
   * \endspec
   */
  static void RegisterInstanceSpecifiersToInstanceIdentifiersMapping(
      configuration::SomeIpBindingConfig const& config,
      // VECTOR NC AutosarC++17_10-M7.1.2: MD_SOMEIPBINDING_AutosarC++17_10-M7.1.2_parameter_is_unique_pointer
      std::unique_ptr<RuntimeInterface>& runtime) noexcept {
    // The functionality for handling and processing ara com instance identifiers and specifiers is duplicated
    // currently in both XF and Generic layers. This, however, will be later unified into one common util class within
    // the core. So, for now, use the utilities provided by the transformation layer.
    using AraComInstanceIdUtils = ::amsr::someip_binding_xf::internal::AraComInstanceIdXf;

    configuration::SomeIpBindingConfig::IpcChannelConfigRefContainer const& ipc_channels{
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        config.GetIpcChannels()};

    for (auto const& ipc_channel : ipc_channels) {
      // Required service instance mapping.
      configuration::SomeIpBindingConfig::IpcChannelConfig::RequiredServiceInstanceConfigRefContainer const
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          rsi_container{ipc_channel.get().GetRequiredServiceInstanceConfigs()};

      for (auto const& rsi : rsi_container) {
        ara::com::InstanceIdentifier const instance_identifier{AraComInstanceIdUtils::BuildAraComInstanceIdentifier(
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            rsi.get().GetServiceIdentifier().service_identifier,
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            rsi.get().GetMajorVersion().major_version,
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            rsi.get().GetInstanceIdentifier().instance_identifier)};
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        for (auto const& specifier : rsi.get().GetInstanceSpecifiers()) {
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          ::amsr::core::InstanceSpecifier const instance_specifier{specifier.instance_specifier};
          // VCA_SOMEIPBINDING_RUNTIME_OBJECT_LIFECYCLE
          runtime->MapRequiredInstanceSpecifierToInstanceId(instance_specifier, instance_identifier);
        }
      }

      // Provided service instance mapping.
      configuration::SomeIpBindingConfig::IpcChannelConfig::ProvidedServiceInstanceConfigRefContainer const
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          psi_container{ipc_channel.get().GetProvidedServiceInstanceConfigs()};
      for (auto const& psi : psi_container) {
        ara::com::InstanceIdentifier const instance_identifier{AraComInstanceIdUtils::BuildAraComInstanceIdentifier(
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            psi.get().GetServiceIdentifier().service_identifier,
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            psi.get().GetMajorVersion().major_version,
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            psi.get().GetInstanceIdentifier().instance_identifier)};
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        for (auto const& specifier : psi.get().GetInstanceSpecifiers()) {
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          ::amsr::core::InstanceSpecifier const instance_specifier{specifier.instance_specifier};
          // VCA_SOMEIPBINDING_RUNTIME_OBJECT_LIFECYCLE
          runtime->MapProvidedInstanceSpecifierToInstanceId(instance_specifier, instance_identifier);
        }
      }
    }
  }
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_INITIALIZER_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_SOMEIPBINDING_GENERIC_PROXY_MISSING_INTEGRATION
//   \ACCEPT  XX
//   \REASON  The reported gap comes from an initialization sequence related to generic proxy,
//            which is not yet triggered from any MUT or CT as it has not been fully integrated.
//            This path will be completely covered with WIZRD-1536, where new CTs will be created.
// COV_JUSTIFICATION_END

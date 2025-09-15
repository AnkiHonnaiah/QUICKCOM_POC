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
/**        \file  binding_core_initializer.h
 *        \brief  SomeIpBinding Initializer
 *        \unit   SomeIpBinding::SomeIpBinding::BindingCoreInitializer
 *
 *      \details  Helper class for SomeIpBinding instances initialization and registration.
 *
 *   \complexity  This unit uses many simple getters of config objects for mapping data types that lead to many inter
 *                unit calls. WIZRD-1802 was created to defer extracting a new unit after other pending changes.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_CORE_INITIALIZER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_CORE_INITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <map>
#include <memory>
#include <utility>
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/ipc_channel_config.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_core.h"
#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"
#include "amsr/someip_binding_core/internal/someip_daemon_client_wrapper.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief Static helper class for initialization of SomeIpBinding instances.
 */
class BindingCoreInitializer final {
 public:
  /*!
   * \brief Alias for SomeIpBindingConfig.
   */
  using SomeIpBindingConfig = ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig;

  /*!
   * \brief Type-alias for the Reactor.
   */
  using Reactor = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief Type alias for RequiredServiceInstanceId
   */
  using RequiredServiceInstanceId = ::amsr::someip_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Type alias for ProvidedServiceInstanceId
   */
  using ProvidedServiceInstanceId = ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Name alias for provided service instance initialization function.
   */
  using ProvidedServiceInstanceInitializationFunction =
      vac::language::UniqueFunction<void(configuration::ProvidedServiceInstanceConfig const&,
                                         configuration::ServiceInterfaceConfig const&),
                                    ara::core::PolymorphicAllocator<std::uint8_t>>;

  /*!
   * \brief Name alias for provided service instance initialization map.
   */
  using ProvidedServiceInstanceInitMap =
      std::map<configuration::ServiceIdentifier, ProvidedServiceInstanceInitializationFunction,
               configuration::ServiceIdentifierComparator>;

  /*!
   * \brief  Type alias for SomeIpBindingCore.
   */
  using SomeIpBindingCoreType = ::amsr::someip_binding_core::internal::SomeIpBindingCore;

  /*!
   * \brief Type-alias for the SafeIpcPollingInterface.
   */
  using SafeIpcPollingInterface = ::amsr::someip_binding_core::internal::SafeIpcPollingInterface;

  /*!
   * \brief Initialize the Binding.
   *
   * \tparam SomeIpDaemonClient Used client class to coordinate the communication with the SOME/IP daemon process.
   *
   * \param[in]   someip_binding_config        The runtime configuration of the binding.
   * \param[in]   channel_config               The IPC channel configuration associated to the core.
   * \param[in]   safe_ipc_polling             Contains reactor polling function, it is called while waiting for
   *                                           a command response.
   * \param[in]   reactor                      A reference to the reactor.
   *
   * \return      The created SOME/IP binding core instance.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename SomeIpDaemonClient = ::amsr::someip_daemon_client::internal::SomeIpDaemonClient<
                ::amsr::someip_daemon_client::internal::SomeIpDaemonClientDefaultTemplateConfiguration>>
  static auto Create(configuration::SomeIpBindingConfig const& someip_binding_config,
                     configuration::IpcChannelConfig const& channel_config,
                     std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling, Reactor& reactor) noexcept
      -> std::shared_ptr<SomeIpBindingCoreType> {
    bool const is_polling_mode{someip_binding_config.GetRuntimeProcessingMode() ==
                               configuration::RuntimeProcessingMode::polling};

    configuration::IpcChannelConfig::IpcDomain const ipc_domain{channel_config.GetIpcChannelDomain()};
    configuration::IpcChannelConfig::IpcPort const ipc_port{channel_config.GetIpcChannelPort()};
    configuration::SomeIpBindingConfig::IpcMaxQueueSize const ipc_max_queue_size{
        someip_binding_config.GetIpcMaxQueueSize()};

    someip_daemon_client::internal::SomeIpDaemonClientConfigModel const someip_daemon_client_config{
        ipc_domain, ipc_port, ipc_max_queue_size};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
    // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
    std::unique_ptr<SomeIpDaemonClient> someip_daemon_client{
        // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
        std::make_unique<SomeIpDaemonClient>(someip_daemon_client_config, reactor)};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
    std::unique_ptr<amsr::someip_binding_core::internal::SomeIpDaemonClientWrapper<SomeIpDaemonClient>>
        someip_daemon_client_wrapper{
            std::make_unique<amsr::someip_binding_core::internal::SomeIpDaemonClientWrapper<SomeIpDaemonClient>>(
                std::move(someip_daemon_client))};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SOMEIPBINDING_AutosarC++17_10_A5.0.1_falsepositive
    // VECTOR NC VectorC++-V5.0.1: MD_SOMEIPBINDING_VectorC++-V5.0.1_unsequenced_call_false_positive
    std::shared_ptr<SomeIpBindingCoreType> someip_binding_core{
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        std::make_shared<SomeIpBindingCoreType>(std::move(someip_daemon_client_wrapper),
                                                // VCA_SOMEIPBINDING_RUNTIME_OBJECT_LIFECYCLE
                                                safe_ipc_polling, is_polling_mode)};

    return someip_binding_core;
  }

  /*!
   * \brief       Initializes all SOME/IP Provided Service Instances.
   *
   * \param[in]   provided_service_instances  A container of references to Provided Service Instance Configs.
   * \param[out]  someip_binding_core         The SOME/IP binding core to emplace the required service instance.
   *
   * \pre         -
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *   - Iterate through provided service instance configs:
   *     - Iterate through service interface configs:
   *       - If the service id and major version of service matches with the provided instance,
   *         - Register the provided service instance into the Binding.
   * \endinternal
   */
  static void InitializeProvidedServiceInstances(
      SomeIpBindingConfig::ProvidedServiceInstanceConfigRefContainer const& provided_service_instances,
      someip_binding_core::internal::SomeIpBindingCoreInterface& someip_binding_core) noexcept;

  /*!
   * \brief       Initializes all SOME/IP Required Service Instances.
   *
   * \param[in]   required_service_instances  A container of references to Required Service Instance Configs.
   * \param[out]  someip_binding_core         The SOME/IP binding core to emplace the required service instance.
   *
   * \pre         -
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *   - Iterate through required service instance configs:
   *     - Iterate through service interface configs:
   *       - If the service id and major version of service matches with the required instance,
   *         - Register the required service instance into the Binding.
   * \endinternal
   */
  static void InitializeRequiredServiceInstances(
      SomeIpBindingConfig::RequiredServiceInstanceConfigRefContainer const& required_service_instances,
      someip_binding_core::internal::SomeIpBindingCoreInterface& someip_binding_core) noexcept;

  /*!
   * \brief       Extract the relevant service interface information that a client needs to instantiate events, methods
   *              and fields.
   * \param[in]   interface_config Service interface configuration.
   * \return      Service information.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   *  - Fill service information struct
   * \endinternal
   */
  static auto ExtractServiceInformation(configuration::ServiceInterfaceConfig const& interface_config) noexcept
      -> ::amsr::someip_binding_core::internal::ServiceInformation;
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_BINDING_CORE_INITIALIZER_H_

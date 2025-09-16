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
 *        \brief  Initializes the binding. The target-dependent .cpp file is generated.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_INITIALIZER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_INITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"

#include "ipc_binding/internal/target_dependent/initializer_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace target_dependent {

/*!
 * \brief Initializes the binding.
 *
 * \steady FALSE
 *
 * \unit       IpcBinding::Initializer
 * \complexity Unit accesses many units due to initialization functionality.
 */
class Initializer final : public InitializerInterface {
 public:
  /*!
   * \brief Use default constructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  Initializer() noexcept = default;

  Initializer(Initializer&&) noexcept = delete;
  Initializer(Initializer const&) noexcept = delete;
  auto operator=(Initializer const&) & -> Initializer& = delete;
  auto operator=(Initializer&&) & -> Initializer& = delete;

  /*!
   * \brief Destroys the object.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~Initializer() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \copydoc InitializerInterface::ValidateRuntimeConfig
   */
  auto ValidateRuntimeConfig(ConfigValidatorInterfacePtr validator, RuntimeConfig const& runtime_config) const noexcept
      -> void final;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_config_variant
  /*!
   * \copydoc InitializerInterface::Initialize
   */
  auto Initialize(ReactorInterface& reactor, RuntimeConfig const& runtime_config,
                  TraceSinkFactoryInterface const& trace_sink_factory, BindingCoreType& binding_core,
                  RequiredServiceInstanceManagerInterface& required_service_instance_manager,
                  RemoteServerManagerInterface& remote_server_manager) const noexcept -> void final;

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for ServiceDiscoveryInterface.
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
   * \brief Runtime service configuration type.
   */
  // VECTOR Next Line VectorC++-V0.1.5: MD_IPCBINDING_VectorC++-V0.1.5_unused_type_declaration_false_positive
  using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Runtime service configuration reference type.
   */
  // VECTOR Next Line VectorC++-V0.1.5: MD_IPCBINDING_VectorC++-V0.1.5_unused_type_declaration_false_positive
  using ServiceConfigRef = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig::ServiceConfigRef;

  /*!
   * \brief Shortname path type.
   */
  // VECTOR Next Line VectorC++-V0.1.5: MD_IPCBINDING_VectorC++-V0.1.5_unused_type_declaration_false_positive
  using ShortnamePath = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig::ShortnamePath;

  /*!
   * \brief Container to hold shortname paths.
   */
  using DeployedServicesVector =
      ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig::DeployedServicesVector;

  /*!
   * \brief Vector containing the shortname paths of all deployed service interfaces.
   */
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_IPCBINDING_AutosarC++17_10-A3.3.2_not_POD_obj_static_code_generated
  static DeployedServicesVector const kDeployedServiceInterfaces;

  /*!
   * \brief Registers all provided service instances.
   *
   * \param[in]     runtime_config  Used IPC binding runtime configuration.
   * \param[in,out] binding_core    Reference to BindingCore instance.
   *
   * \context    Init
   * \threadsafe FALSE
   * \steady     FALSE
   */
  void RegisterProvidedServiceInstances(RuntimeConfig const& runtime_config,
                                        BindingCoreType& binding_core) const noexcept;

  /*!
   * \brief Initialize the RequiredServiceInstances.
   *
   * \param[in]     runtime_config                     Used IPC binding runtime configuration.
   * \param[in]     trace_sink_factory                 Reference to the trace sink factory.
   * \param[in,out] binding_core                       Reference to BindingCore instance.
   * \param[in,out] required_service_instance_manager  Reference to the RequiredServiceInstanceManager.
   * \param[in,out] remote_server_manager              A reference to a unique_ptr that will be filled with a created
   *                                                   RemoteServer manager.
   *
   * \context    Init
   * \threadsafe FALSE
   * \steady     FALSE
   */
  void InitializeRequiredServiceInstances(RuntimeConfig const& runtime_config,
                                          TraceSinkFactoryInterface const& trace_sink_factory,
                                          BindingCoreType& binding_core,
                                          RequiredServiceInstanceManagerInterface& required_service_instance_manager,
                                          RemoteServerManagerInterface& remote_server_manager) const noexcept;

  /*!
   * \brief Instantiates and registers SkeletonXfFactory instance for each configured provided service instance to Socal
   *        SkeletonXfFactoryRegistry.
   *
   * \param[in,out] reactor                      Reference to the reactor.
   * \param[in,out] service_discovery            Reference to the ServiceDiscovery.
   * \param[in,out] connection_manager_skeleton  Reference to the ConnectionManagerSkeleton.
   * \param[in,out] skeleton_router              Reference to the SkeletonRouter.
   * \param[in]     trace_sink_factory           Reference to the trace sink factory.
   * \param[in]     runtime_config               Used IPC binding runtime configuration.
   *
   * \context    Init
   * \threadsafe FALSE
   * \steady     FALSE
   */
  void InitializeProvidedServiceInstances(ReactorInterface& reactor, ServiceDiscoveryInterface& service_discovery,
                                          ConnectionManagerSkeletonInterface& connection_manager_skeleton,
                                          SkeletonRouterInterface& skeleton_router,
                                          TraceSinkFactoryInterface const& trace_sink_factory,
                                          RuntimeConfig const& runtime_config) const noexcept;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 ::amsr::core::StringView{"Initializer"}};
};

}  // namespace target_dependent
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_INITIALIZER_H_

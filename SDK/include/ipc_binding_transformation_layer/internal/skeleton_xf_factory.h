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
 *        \brief  Transformation layer factory at skeleton side to create skeletonXf objects on demand.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "amsr/socal/internal/factory/skeleton_backend_factory_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/runtime_configuration/provided_service_instance_config.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"
#include "ipc_binding_xf_common/internal/trace/trace_sink_factory_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief Factory class to create and get the proxy backend from the bindings.
 *
 * \tparam SkeletonXf  Type of provided service instance.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProvidedServiceInstance
 */
template <typename SkeletonXf>
class SkeletonXfFactory final : public ::amsr::socal::internal::factory::SkeletonBackendFactoryInterface<
                                    typename SkeletonXf::SkeletonBackendInterface> {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Type-alias for RuntimeProcessingMode.
   */
  using RuntimeProcessingMode = amsr::ipc_binding_core::internal::runtime_configuration::RuntimeProcessingMode;

  /*!
   * \brief Type alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

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
   * \brief Type-alias for IpcUnicastAddress.
   */
  using IpcUnicastAddress = ipc_binding_core::internal::ipc_protocol::IpcUnicastAddress;

  /*!
   * \brief Type-alias for IntegrityLevel.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Type alias for SkeletonRouterInterface.
   */
  using SkeletonRouterInterface = ::amsr::ipc_binding_core::internal::SkeletonRouterInterface;

  /*!
   * \brief Runtime service configuration type.
   */
  using ServiceConfig = ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Type alias for ProvidedServiceInstanceConfig.
   */
  using ProvidedServiceInstanceConfig =
      ipc_binding_core::internal::runtime_configuration::ProvidedServiceInstanceConfig;

  /*!
   * \brief Type alias for the skeleton Xf pointer.
   */
  using SkeletonXfPtr =
      std::unique_ptr<typename SkeletonXf::SkeletonBackendInterface>;  // IGNORE_BLACKLIST external API

  /*!
   * \brief Alias for SkeletonBackendFactoryInterface.
   */
  using SkeletonBackendFactoryInterface =
      ::amsr::socal::internal::factory::SkeletonBackendFactoryInterface<typename SkeletonXf::SkeletonBackendInterface>;

  /*!
   * \brief Type alias for trace sink factory interface.
   */
  using TraceSinkFactoryInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface;

  /*!
   * \brief Constructor
   *
   * \param[in,out] reactor                           Reference to the reactor.
   * \param[in,out] service_discovery                 Reference to the ServiceDiscovery.
   * \param[in,out] connection_manager_skeleton       Reference to the ConnectionManagerSkeleton.
   * \param[in,out] skeleton_router                   Reference to the SkeletonRouter.
   * \param[in]     trace_sink_factory                Reference to the trace sink factory.
   * \param[in]     provided_service_instance_id      The ProvidedServiceInstanceId.
   * \param[in]     service_config                    Reference to runtime service configuration.
   * \param[in]     provided_service_instance_config  Reference to the ProvidedServiceInstance configuration.
   * \param[in]     runtime_processing_mode           The configured RuntimeProcessingMode.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Initialize all the resources of the factory.
   * \endinternal
   */
  SkeletonXfFactory(ReactorInterface& reactor, ServiceDiscoveryInterface& service_discovery,
                    ConnectionManagerSkeletonInterface& connection_manager_skeleton,
                    SkeletonRouterInterface& skeleton_router, TraceSinkFactoryInterface const& trace_sink_factory,
                    ProvidedServiceInstanceId const& provided_service_instance_id, ServiceConfig const& service_config,
                    ProvidedServiceInstanceConfig const& provided_service_instance_config,
                    RuntimeProcessingMode const runtime_processing_mode) noexcept
      : SkeletonBackendFactoryInterface{},
        reactor_{reactor},
        service_discovery_{service_discovery},
        connection_manager_skeleton_{connection_manager_skeleton},
        skeleton_router_{skeleton_router},
        trace_sink_factory_{trace_sink_factory},
        provided_service_instance_id_{provided_service_instance_id},
        unicast_address_{provided_service_instance_config.GetIpcDomain(),
                         provided_service_instance_config.GetIpcPort()},
        expected_client_integrity_level_{provided_service_instance_config.GetExpectedClientIntegrityLevel()},
        service_config_{service_config},
        runtime_processing_mode_{runtime_processing_mode},
        logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"SkeletonXfFactory"}} {}

  SkeletonXfFactory() noexcept = delete;
  SkeletonXfFactory(SkeletonXfFactory const&) noexcept = delete;
  SkeletonXfFactory(SkeletonXfFactory&&) noexcept = delete;
  auto operator=(SkeletonXfFactory const&) noexcept -> SkeletonXfFactory& = delete;
  auto operator=(SkeletonXfFactory&&) noexcept -> SkeletonXfFactory& = delete;

  /*!
   * \brief Destructor.
   *
   * \context     Shutdown
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
  ~SkeletonXfFactory() noexcept final = default;

  /*!
   * \brief   Create Skeleton xf(backend) and move to the caller.
   * \details The function is called from socal in context of skeleton construction.
   *
   * \return Result containing the Skeleton Xf pointer.
   *
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   *
   * \internal
   * - Create skeleton Xf and return.
   * \endinternal
   */
  auto CreateBackend() noexcept -> ::amsr::core::Result<SkeletonXfPtr> final {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_side_effect_logging
    logger_.LogDebug([](::ara::log::LogStream const&) {},
                     Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_IPCBINDING_AutosarC++17_10_A18.5.8_false_positive
    SkeletonXfPtr skeleton_xf_ptr
        // VCA_IPCB_STD_MAKE_UNIQUE_SAFE_FUNCTION
        {std::make_unique<SkeletonXf>(  // IGNORE_BLACKLIST external API
            reactor_, service_discovery_, connection_manager_skeleton_, skeleton_router_, trace_sink_factory_,
            provided_service_instance_id_, unicast_address_, expected_client_integrity_level_, service_config_,
            runtime_processing_mode_)};

    return ::amsr::core::Result<SkeletonXfPtr>{std::move(skeleton_xf_ptr)};
  }

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Reference to the Reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief Reference to ServiceDiscovery.
   */
  ServiceDiscoveryInterface& service_discovery_;

  /*!
   * \brief Reference to ConnectionManagerSkeleton.
   */
  ConnectionManagerSkeletonInterface& connection_manager_skeleton_;

  /*!
   * \brief Reference to SkeletonRouter.
   */
  SkeletonRouterInterface& skeleton_router_;

  /*!
   * \brief Reference to the trace sink factory.
   */
  TraceSinkFactoryInterface const& trace_sink_factory_;

  /*!
   * \brief Provided Service Instance id.
   */
  ProvidedServiceInstanceId provided_service_instance_id_;

  /*!
   * \brief The unicast address for the skeleton.
   */
  IpcUnicastAddress const unicast_address_;

  /*!
   * \brief The expected client integrity level.
   */
  IntegrityLevel const expected_client_integrity_level_;

  /*!
   * \brief Runtime service config.
   */
  ServiceConfig const& service_config_;

  /*!
   * \brief The configured RuntimeProcessingMode.
   */
  RuntimeProcessingMode const runtime_processing_mode_;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_;
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_H_

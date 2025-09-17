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
 *        \brief  Contains common functionality for the [Service]SkeletonXf classes that does not have to be generated.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_BASE_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/skeleton_backend_router_interface.h"
#include "ipc_binding_core/internal/packet_router/skeleton_router_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/reactor_sync_task.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/service_discovery/service_discovery_type_util.h"
#include "ipc_binding_transformation_layer/internal/skeleton_backend_router.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief Base class for the generated SkeletonXf classes with the static functionality.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonInstance
 */
class SkeletonXfBase {
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
   * \brief Type alias for ServiceDiscoveryInterface.
   */
  using ServiceDiscoveryInterface = ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface;

  /*!
   * \brief Type alias for ConnectionManagerSkeleton.
   */
  using ConnectionManagerSkeletonInterface =
      ::amsr::ipc_binding_core::internal::connection_manager::ConnectionManagerSkeletonInterface;

  /*!
   * \brief Type alias for SkeletonRouterInterface.
   */
  using SkeletonRouterInterface = ::amsr::ipc_binding_core::internal::SkeletonRouterInterface;

  /*!
   * \brief Type alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type-alias for IpcUnicastAddress.
   */
  using IpcUnicastAddress = ipc_binding_core::internal::ipc_protocol::IpcUnicastAddress;

  /*!
   * \brief Type-alias for Service Instance IntegrityLevel.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Construct a new SkeletonXfBase.
   *
   * \param[in,out] reactor                          Reference to the reactor.
   * \param[in,out] service_discovery                Reference to ServiceDiscovery.
   * \param[in,out] connection_manager_skeleton      Reference to ConnectionManagerSkeleton.
   * \param[in,out] skeleton_router                  Reference to SkeletonRouter.
   * \param[in]     provided_service_instance_id     The ProvidedServiceInstanceId.
   * \param[in]     unicast_address                  The unicast address for this skeleton.
   * \param[in]     expected_client_integrity_level  The expected client integrity level.
   * \param[in]     runtime_processing_mode          The configured RuntimeProcessingMode.
   * \param[in]     logger_prefix                    Prefix string for logger.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   */
  SkeletonXfBase(ReactorInterface& reactor, ServiceDiscoveryInterface& service_discovery,
                 ConnectionManagerSkeletonInterface& connection_manager_skeleton,
                 SkeletonRouterInterface& skeleton_router,
                 ProvidedServiceInstanceId const& provided_service_instance_id,
                 IpcUnicastAddress const& unicast_address, IntegrityLevel expected_client_integrity_level,
                 RuntimeProcessingMode runtime_processing_mode, ::amsr::core::StringView logger_prefix) noexcept;

  SkeletonXfBase() noexcept = delete;
  SkeletonXfBase(SkeletonXfBase const&) noexcept = delete;
  SkeletonXfBase(SkeletonXfBase&&) noexcept = delete;
  auto operator=(SkeletonXfBase const&) noexcept -> SkeletonXfBase& = delete;
  auto operator=(SkeletonXfBase&&) noexcept -> SkeletonXfBase& = delete;

  /*!
   * \brief Destroy SkeletonXfBase.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
  virtual ~SkeletonXfBase() noexcept = default;

 protected:
  /*!
   * \brief Type-alias for SkeletonBackendRouterInterface.
   */
  using SkeletonBackendRouterInterface = ipc_binding_core::internal::SkeletonBackendRouterInterface;

  /*!
   * \brief Get the SkeletonBackendRouter instance.
   *
   * \return Reference to the SkeletonBackendRouter instance.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetSkeletonBackendRouter() noexcept -> SkeletonBackendRouter&;

  /*!
   * \brief Register the SkeletonBackendRouter with the SkeletonRouter.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto RegisterWithSkeletonRouter() noexcept -> void;

  /*!
   * \brief Deregister the SkeletonBackendRouter from the SkeletonRouter.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto DeregisterFromSkeletonRouter() const noexcept -> void;

  /*!
   * \brief Offer this ProvidedServiceInstance.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OfferServiceBase() noexcept -> void;

  /*!
   * \brief Stop offering this ProvidedServiceInstance.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto StopOfferServiceBase() noexcept -> void;

 private:
  /*!
   * \brief Type-alias for Logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type-alias for LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type-alias for ReactorSyncTask.
   */
  using ReactorSyncTask = ::amsr::ipc_binding_core::internal::ReactorSyncTask<bool>;

  /*!
   * \brief Type-alias for ServiceInstanceIdentifier.
   */
  using ServiceInstanceIdentifier = ::amsr::ipc_binding_core::internal::ipc_protocol::ServiceInstanceIdentifier;

  /*!
   * \brief Type-alias for ServiceDiscoveryTypeUtil.
   */
  using ServiceDiscoveryTypeUtil = ::amsr::ipc_binding_core::internal::service_discovery::ServiceDiscoveryTypeUtil;

  /*!
   * \brief Offer this ProvidedServiceInstance.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OfferServiceSync() noexcept -> void;

  /*!
   * \brief Stop offering this ProvidedServiceInstance.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto StopOfferServiceSync() noexcept -> void;

  /*!
   * \brief The used logger.
   */
  Logger logger_;

  /*!
   * \brief Reference to the reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief Reference to the ServiceDiscovery.
   */
  ServiceDiscoveryInterface& service_discovery_;

  /*!
   * \brief Reference to the ConnectionManagerSkeleton.
   */
  ConnectionManagerSkeletonInterface& connection_manager_skeleton_;

  /*!
   * \brief Reference to the SkeletonRouter instance.
   */
  SkeletonRouterInterface& skeleton_router_;

  /*!
   * \brief The SkeletonBackendRouter instance.
   */
  SkeletonBackendRouter skeleton_backend_router_{};

  /*!
   * \brief The ProvidedServiceInstanceId.
   */
  ProvidedServiceInstanceId const provided_service_instance_id_;

  /*!
   * \brief The unicast address for the skeleton.
   */
  IpcUnicastAddress const unicast_address_;

  /*!
   * \brief The expected client integrity level.
   */
  IntegrityLevel const expected_client_integrity_level_;

  /*!
   * \brief The ServiceInstanceIdentifier.
   */
  ServiceInstanceIdentifier const service_instance_identifier_;

  /*!
   * \brief The configured RuntimeProcessingMode.
   */
  RuntimeProcessingMode const runtime_processing_mode_;
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_BASE_H_

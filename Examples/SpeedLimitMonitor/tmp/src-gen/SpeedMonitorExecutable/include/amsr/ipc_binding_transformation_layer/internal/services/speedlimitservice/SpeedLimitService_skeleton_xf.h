/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  SpeedMonitorExecutable/include/amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_skeleton_xf.h
 *        \brief  IPC binding of service skeleton for service 'SpeedLimitService'
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_ipcbinding
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

#ifndef SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_XF_H_
#define SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_skeleton_event_xf_backend.h"
#include "amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_skeleton_method_xf_backend.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"
#include "ipc_binding_transformation_layer/internal/events/skeleton_event_xf_backend_interface.h"
#include "ipc_binding_transformation_layer/internal/methods/skeleton_fire_and_forget_method_xf_backend_interface.h"
#include "ipc_binding_transformation_layer/internal/methods/skeleton_method_xf_backend_interface.h"
#include "ipc_binding_transformation_layer/internal/skeleton_xf_base.h"
#include "ipc_binding_xf_common/internal/trace/trace_sink_factory_interface.h"
#include "services/speedlimitservice/SpeedLimitService_skeleton_backend_interface.h"
#include "services/speedlimitservice/speedlimitservice_skeleton.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

namespace gen_services {
namespace gen_speedlimitservice {

/*!
 * \brief Provided service interface deployment of service interface 'SpeedLimitService'.
 *
 * \unit       IpcBinding::IpcBindingTransformationLayer::SkeletonInstance
 * \complexity Many calls to 'RuntimeConfiguration' unit to obtain service information.
 */
 // VECTOR NC AutosarC++17_10-A10.1.1: MD_IPCBINDING_AutosarC++17_10-A10.1.1_multiple_inheritance
class SpeedLimitServiceSkeletonXf final
    : public ::services::speedlimitservice::internal::SpeedLimitServiceSkeletonBackendInterface,
      public SkeletonXfBase {
 public:
  /*!
   * \brief Type-alias for the SkeletonBackendInterface.
   */
  using SkeletonBackendInterface = ::services::speedlimitservice::internal::SpeedLimitServiceSkeletonBackendInterface;

  /*!
   * \brief Type alias for the instance ID.
   */
  using InstanceId = ::amsr::ipc_binding_core::internal::ipc_protocol::InstanceId;

  /*!
   * \brief Type alias for the service runtime configuration.
   */
  using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Type alias for the trace sink factory interface.
   */
  using TraceSinkFactoryInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface;

  /*!
   * \brief Constructor of SkeletonXf for service 'SpeedLimitService'.
   *
   * \param[in,out] reactor                          Reference to the Reactor.
   * \param[in,out] service_discovery                Reference to ServiceDiscovery.
   * \param[in,out] connection_manager_skeleton      Reference to ConnectionManagerSkeleton.
   * \param[in,out] skeleton_router                  Reference to the SkeletonRouter.
   * \param[in]     trace_sink_factory               Reference to the trace sink factory.
   * \param[in]     provided_service_instance_id     The ProvidedServiceInstanceId.
   * \param[in]     unicast_address                  The unicast address for this skeleton.
   * \param[in]     expected_client_integrity_level  The expected client integrity level.
   * \param[in]     service_config                   The runtime service config for this service.
   * \param[in]     runtime_processing_mode          The configured RuntimeProcessingMode.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \steady      FALSE
   * \synchronous TRUE
   */
  SpeedLimitServiceSkeletonXf(
      ReactorInterface& reactor,
      ServiceDiscoveryInterface& service_discovery,
      ConnectionManagerSkeletonInterface& connection_manager_skeleton,
      SkeletonRouterInterface& skeleton_router,
      TraceSinkFactoryInterface const& trace_sink_factory,
      ProvidedServiceInstanceId const& provided_service_instance_id,
      IpcUnicastAddress const& unicast_address,
      IntegrityLevel expected_client_integrity_level,
      ServiceConfig const& service_config,
      RuntimeProcessingMode runtime_processing_mode) noexcept;

  /*!
   * \brief Destroy the SpeedLimitServiceSkeletonXf.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~SpeedLimitServiceSkeletonXf() noexcept final;

  SpeedLimitServiceSkeletonXf(SpeedLimitServiceSkeletonXf const&) noexcept = delete;
  auto operator=(SpeedLimitServiceSkeletonXf const&) noexcept -> SpeedLimitServiceSkeletonXf& = delete;
  SpeedLimitServiceSkeletonXf(SpeedLimitServiceSkeletonXf&&) noexcept = delete;
  auto operator=(SpeedLimitServiceSkeletonXf&&) noexcept -> SpeedLimitServiceSkeletonXf& = delete;

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
  void OfferService() noexcept final;

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
  void StopOfferService() noexcept final;


  // ---- Events ---------------------------------------------------------------------------------------------------

  /*!
   * \brief   Get the event xf backend object for the IPC binding service event 'SpeedOverLimitEvent'.
   * \details Event sample type: ::datatypes::OverLimitEnum.
   *
   * \return The IPC binding specific event management object/interface supporting event transmission.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  SpeedLimitServiceSkeletonEventXfBackendSpeedOverLimitEvent* GetEventManagerSpeedOverLimitEvent() noexcept final;
  // ---- Events --------------------------------------------------------------------------------------------------

  // ---- Fields ---------------------------------------------------------------------------------------------------
  // ---- Fields --------------------------------------------------------------------------------------------------

  // ---- Methods ---------------------------------------------------------------------------------------------------
 // ---- Methods ---------------------------------------------------------------------------------------------------

 private:
  /*!
   * \brief Type alias for ServiceInstanceIdentifier.
   */
  using ServiceInstanceIdentifier = ::amsr::ipc_binding_core::internal::ipc_protocol::ServiceInstanceIdentifier;

  /*!
   * \brief Type-alias for the MethodId type definition.
   */
  using MethodId = ::amsr::ipc_binding_core::internal::ipc_protocol::MethodId;

  /*!
   * \brief Type-alias for the SkeletonMethodXfBackendInterface type definition.
   */
  using SkeletonMethodXfBackendInterface =
      ::amsr::ipc_binding_transformation_layer::internal::methods::SkeletonMethodXfBackendInterface;

  /*!
   * \brief Type definition for the MethodIdMap.
   */
  using MethodIdMap = std::unordered_map<MethodId, SkeletonMethodXfBackendInterface&,
                                         ipc_binding_core::internal::runtime_configuration::MethodIdHash>;

  /*!
   * \brief Type-alias for the SkeletonFireAndForgetMethodXfBackendInterface type definition.
   */
  using SkeletonFireAndForgetMethodXfBackendInterface =
      ::amsr::ipc_binding_transformation_layer::internal::methods::SkeletonFireAndForgetMethodXfBackendInterface;

  /*!
   * \brief Type definition for the MethodFireForgetIdMap.
   */
  using MethodFireForgetIdMap = std::unordered_map<MethodId, SkeletonFireAndForgetMethodXfBackendInterface&,
                                                   ipc_binding_core::internal::runtime_configuration::MethodIdHash>;

  /*!
   * \brief Type-alias for the EventId type definition.
   */
  using EventId = ::amsr::ipc_binding_core::internal::ipc_protocol::EventId;

  /*!
   * \brief Type definition for the EventIdSet.
   */
  using EventIdSet = std::unordered_set<EventId, ipc_binding_core::internal::runtime_configuration::EventIdHash>;

  /*!
   * \brief Type-alias for the SkeletonEventXfBackendInterface type definition.
   */
  using SkeletonEventXfBackendInterface =
      ::amsr::ipc_binding_transformation_layer::internal::SkeletonEventXfBackendInterface;

  /*!
   * \brief Type definition for the NotifierIdMap.
   */
  using NotifierIdMap = std::unordered_map<EventId, SkeletonEventXfBackendInterface&,
                                           ipc_binding_core::internal::runtime_configuration::EventIdHash>;

  /*!
   * \brief Type alias for the method runtime configuration.
   */
  using MethodConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig;

  /*!
   * \brief Type alias for the event runtime configuration.
   */
  using EventConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::EventConfig;

  /*!
   * \brief Type alias for the field runtime configuration.
   */
  using FieldConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::FieldConfig;

  // ---- MethodXfs ------------------------------------------------------------------------------------------
  // ---- MethodXfs ------------------------------------------------------------------------------------------

  // ---- SkeletonEventXfBackend --------------------------------------------------------------------------------------------
  /*!
   * \brief Reference to the runtime configuration of event 'SpeedOverLimitEvent'.
   */
  EventConfig const& event_SpeedOverLimitEvent_config_;

  /*!
   * \brief Event xf backend for skeleton event 'SpeedOverLimitEvent'
   */
  SpeedLimitServiceSkeletonEventXfBackendSpeedOverLimitEvent event_SpeedOverLimitEvent_;

  // ---- SkeletonEventXfBackend -------------------------------------------------------------------------------------------

  // ---- FieldManagers --------------------------------------------------------------------------------------------
  // ---- FieldManagers -------------------------------------------------------------------------------------------


  /*!
   * \brief Set of event ids.
   */
  EventIdSet event_id_set_;
};

}  // namespace gen_speedlimitservice
}  // namespace gen_services

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_XF_H_


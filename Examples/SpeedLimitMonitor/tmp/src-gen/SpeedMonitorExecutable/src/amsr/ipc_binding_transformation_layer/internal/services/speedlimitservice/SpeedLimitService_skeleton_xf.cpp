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
/**        \file  SpeedMonitorExecutable/src/amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_skeleton_xf.cpp
 *        \brief  IpcBinding skeleton binding of service 'SpeedLimitService'
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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_skeleton_xf.h"
#include <utility>

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

namespace gen_services {
namespace gen_speedlimitservice {

SpeedLimitServiceSkeletonXf::SpeedLimitServiceSkeletonXf(
    ReactorInterface& reactor,
    ServiceDiscoveryInterface& service_discovery,
    ConnectionManagerSkeletonInterface& connection_manager_skeleton,
    SkeletonRouterInterface& skeleton_router,
    TraceSinkFactoryInterface const& trace_sink_factory,
    ProvidedServiceInstanceId const& provided_service_instance_id,
    IpcUnicastAddress const& unicast_address,
    IntegrityLevel expected_client_integrity_level,
    ServiceConfig const& service_config,
    RuntimeProcessingMode const runtime_processing_mode) noexcept
    : ::services::speedlimitservice::internal::SpeedLimitServiceSkeletonBackendInterface{},
      // VCA_IPCB_SKELETON_INSTANCE_VALID_SKELETONXFBASE
      SkeletonXfBase{reactor,
                     service_discovery,
                     connection_manager_skeleton,
                     skeleton_router,
                     provided_service_instance_id,
                     unicast_address,
                     expected_client_integrity_level,
                     runtime_processing_mode,
                     ::amsr::core::StringView{"SpeedLimitServiceSkeletonXf"}
      }

      // ---- Construction of Method / Events / Fields managers ----
      ,
      // VCA_IPCB_STD_BASICSTRING_CONSTRUCTOR
      event_SpeedOverLimitEvent_config_{service_config.GetEventConfigChecked("SpeedOverLimitEvent")},
      // VECTOR NC AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10_A5.0.1_SkeletonXf_member_initializer_list
      event_SpeedOverLimitEvent_{
          provided_service_instance_id,
          EventId{event_SpeedOverLimitEvent_config_.GetId().value},
          false, // no field event
          GetSkeletonBackendRouter().GetOfferState(), // VCA_IPCB_SKELETON_INSTANCE_VALID_SKELETONXFBASE
          // VCA_IPCB_VALID_TRACE_SINK_FACTORY
          trace_sink_factory.CreateTraceEventSink(event_SpeedOverLimitEvent_config_)},
      event_id_set_{  // VCA_IPCB_STD_CONSTRUCTOR_CALLED
          event_SpeedOverLimitEvent_config_.GetId()}{

  // VCA_IPCB_SKELETON_INSTANCE_VALID_SKELETONXFBASE
  GetSkeletonBackendRouter().RegisterEvents({
      {event_SpeedOverLimitEvent_config_.GetId(), event_SpeedOverLimitEvent_}});

  RegisterWithSkeletonRouter();  // VCA_IPCB_SKELETON_INSTANCE_VALID_SKELETONXFBASE
}

// VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
SpeedLimitServiceSkeletonXf::~SpeedLimitServiceSkeletonXf() noexcept {
  DeregisterFromSkeletonRouter();  // VCA_IPCB_SKELETON_INSTANCE_VALID_SKELETONXFBASE
}  // VCA_IPCB_DESTRUCTOR_STD_DEFINED_FUNCTION

void SpeedLimitServiceSkeletonXf::OfferService() noexcept {
  SkeletonXfBase::OfferServiceBase();  // VCA_IPCB_SKELETON_INSTANCE_VALID_SKELETONXFBASE
}

void SpeedLimitServiceSkeletonXf::StopOfferService() noexcept {
  SkeletonXfBase::StopOfferServiceBase();  // VCA_IPCB_SKELETON_INSTANCE_VALID_SKELETONXFBASE
}

// ---- Events ---------------------------------------------------------------------------------------------------------

// ---- Event 'SpeedOverLimitEvent' ----

SpeedLimitServiceSkeletonEventXfBackendSpeedOverLimitEvent* SpeedLimitServiceSkeletonXf::GetEventManagerSpeedOverLimitEvent() noexcept {
  return &event_SpeedOverLimitEvent_;
}

// ---- Fields ---------------------------------------------------------------------------------------------------------

// ---- Methods --------------------------------------------------------------------------------------------------------

}  // namespace gen_speedlimitservice
}  // namespace gen_services

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr


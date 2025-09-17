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
/**        \file  SpeedMonitorExecutable/include/amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_skeleton_event_xf_backend.h
 *        \brief  IPC skeleton event handling for events and field notifications of service 'SpeedLimitService'
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

#ifndef SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_EVENT_XF_BACKEND_H_
#define SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_EVENT_XF_BACKEND_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_protocol/internal/dataprototype_serialization/services_speedlimitservice/events/serializer_SpeedOverLimitEvent.h"
#include "datatypes/impl_type_overlimitenum.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_transformation_layer/internal/events/skeleton_event_xf_backend.h"
#include "services/speedlimitservice/SpeedLimitService.h"
#include "services/speedlimitservice/speedlimitservice_skeleton.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_IPCBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

namespace gen_services {
namespace gen_speedlimitservice {

// Forward-declaration for back-reference
class SpeedLimitServiceSkeletonXf;

// ---- Event 'SpeedOverLimitEvent' -------------------------------------------
/*!
 * \brief Skeleton-side configuration struct for event 'SpeedOverLimitEvent'.
 */
struct SkeletonIpcEventConfigurationSpeedOverLimitEvent {

  /*!
   * \brief Datatype of the event sample.
   * \details AsrPath: /Datatypes/OverLimitEnum
   */
  using SampleType = ::datatypes::OverLimitEnum;

  /*!
   * \brief Serializer for event payload.
   */
  using PayloadSerializer = ::amsr::someip_protocol::internal::dataprototype_serializer::services_speedlimitservice::events::SerializerSpeedOverLimitEvent;
};

/*!
 * \brief Skeleton event xf backend type for event 'SpeedOverLimitEvent'.
 */
using SpeedLimitServiceSkeletonEventXfBackendSpeedOverLimitEvent =
    ::amsr::ipc_binding_transformation_layer::internal::SkeletonEventXfBackend<SkeletonIpcEventConfigurationSpeedOverLimitEvent::SampleType, SkeletonIpcEventConfigurationSpeedOverLimitEvent::PayloadSerializer>;


}  // namespace gen_speedlimitservice
}  // namespace gen_services

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_EVENT_XF_BACKEND_H_


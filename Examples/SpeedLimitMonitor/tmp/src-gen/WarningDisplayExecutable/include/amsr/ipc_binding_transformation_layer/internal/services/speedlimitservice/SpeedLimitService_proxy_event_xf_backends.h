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
/**        \file  WarningDisplayExecutable/include/amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_proxy_event_xf_backends.h
 *        \brief  ProxyEventXfBackends for events and field notifications of service 'SpeedLimitService'
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

#ifndef WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_EVENT_XF_BACKENDS_H_
#define WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_EVENT_XF_BACKENDS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_protocol/internal/dataprototype_deserialization/services_speedlimitservice/events/deserializer_SpeedOverLimitEvent.h"
#include "datatypes/impl_type_overlimitenum.h"
#include "ipc_binding_transformation_layer/internal/events/proxy_event_xf_backend.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

namespace gen_services {
namespace gen_speedlimitservice {

// Forward-declaration for back-reference
class SpeedLimitServiceRequiredIpcServiceInstance;

// Forward-declaration for back-reference
class SpeedLimitServiceProxyXf;


// ---- Event SpeedOverLimitEvent ------------------------

/*!
 * \brief Configuration for Event 'SpeedOverLimitEvent'.
 */
struct ProxyEventXfConfigurationSpeedOverLimitEvent {
  /*!
   * \brief Datatype of the event sample.
   */
  using SampleType = ::datatypes::OverLimitEnum;

  /*!
   * \brief Deserializer for event payload.
   */
  using SampleDeserializerType = ::amsr::someip_protocol::internal::dataprototype_deserializer::services_speedlimitservice::events::DeserializerSpeedOverLimitEvent;
};

/*!
 * \brief ProxyEventXfBackend for event 'SpeedOverLimitEvent'.
 * \details Handles event subscriptions and notification deserialization.
 */
using SpeedLimitServiceProxyEventXfBackendSpeedOverLimitEvent =
    ::amsr::ipc_binding_transformation_layer::internal::ProxyEventXfBackend<ProxyEventXfConfigurationSpeedOverLimitEvent::SampleType,
     ProxyEventXfConfigurationSpeedOverLimitEvent::SampleDeserializerType>;


}  // namespace gen_speedlimitservice
}  // namespace gen_services

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_EVENT_XF_BACKENDS_H_


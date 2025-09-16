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
/**        \file  WarningDisplayExecutable/src/amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_proxy_xf.cpp
 *        \brief  Frame for IPC binding service instance class of service 'SpeedLimitService'
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
#include "amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_proxy_xf.h"
#include <utility>

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

namespace gen_services {
namespace gen_speedlimitservice {

SpeedLimitServiceProxyXf::SpeedLimitServiceProxyXf(
    RemoteServerManagerInterface& remote_server_manager,
    ProvidedServiceInstanceId const provided_service_instance_id,
    RequiredServiceInstanceId const required_service_instance_id,
    ClientId const client_id,
    ::amsr::ipc::IntegrityLevel const integrity_level,
    ServiceConfig const& service_config,
    TraceSinkFactoryInterface const& trace_sink_factory,
    ProxyRouterFactoryInterface const& proxy_router_factory) noexcept
      : ::services::speedlimitservice::internal::SpeedLimitServiceProxyBackendInterface{},
      provided_service_instance_id_{provided_service_instance_id},
      client_id_{client_id},
      // VCA_IPCB_VALID_PARAMETER_REFERENCE
      remote_server_{remote_server_manager.RequestRemoteServer(
          provided_service_instance_id_, required_service_instance_id, integrity_level)},
      // VCA_IPCB_PROXYINSTANCE_PROXY_ROUTER_FACTORY_VALID
      proxy_router_{proxy_router_factory.Create()}

      // ---- Construction of Method / Events / Fields XfBackends ----

      // Methods

      // Events
        ,
        // VCA_IPCB_BASICSTRING_CONSTRUCTOR
        event_SpeedOverLimitEvent_config_{service_config.GetEventConfigChecked("SpeedOverLimitEvent")},
        // VECTOR NC AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10_A5.0.1_ProxyXf_member_initializer_list
        // VCA_IPCB_PROXYINSTANCE_PROXYROUTER_VALID
        event_backend_SpeedOverLimitEvent_{
            provided_service_instance_id_,
            event_SpeedOverLimitEvent_config_.GetId(),
            client_id_,
            // VCA_IPCB_VALID_TRACE_SINK_FACTORY
            trace_sink_factory.CreateTraceEventSink(event_SpeedOverLimitEvent_config_),
            *proxy_router_
        }

      // Fields
{
  // VCA_IPCB_SERVICE_INTERFACE_PROXY_XF_SAFE_UNDEFINED_FUNCTION, VCA_IPCB_PROXYINSTANCE_PROXYROUTER_VALID
  remote_server_->Request(client_id_, *proxy_router_);
}

// VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
SpeedLimitServiceProxyXf::~SpeedLimitServiceProxyXf() noexcept {
  // VCA_IPCB_SERVICE_INTERFACE_PROXY_XF_SAFE_UNDEFINED_FUNCTION
  remote_server_->Release(client_id_);
}

::amsr::socal::SkeletonConnectionState SpeedLimitServiceProxyXf::ReadSkeletonConnectionState() noexcept {
  // VCA_IPCB_INTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
  return (remote_server_->IsConnected() ? ::amsr::socal::SkeletonConnectionState::kConnected
                                        : ::amsr::socal::SkeletonConnectionState::kDisconnected);
}

// ---- Events ---------------------------------------------------------------------------------------------------
SpeedLimitServiceProxyEventXfBackendSpeedOverLimitEvent& SpeedLimitServiceProxyXf::GetEventBackendSpeedOverLimitEvent() noexcept {
  return event_backend_SpeedOverLimitEvent_;
}
// ---- Events ---------------------------------------------------------------------------------------------------

  // ---- Fields ---------------------------------------------------------------------------------------------------
// ---- Fields ---------------------------------------------------------------------------------------------------

// ---- Methods ---------------------------------------------------------------------------------------------------
// ---- Methods ---------------------------------------------------------------------------------------------------

// ---- Fields ---------------------------------------------------------------------------------------------------
// ---- Fields ---------------------------------------------------------------------------------------------------

}  // namespace gen_speedlimitservice
}  // namespace gen_services

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr


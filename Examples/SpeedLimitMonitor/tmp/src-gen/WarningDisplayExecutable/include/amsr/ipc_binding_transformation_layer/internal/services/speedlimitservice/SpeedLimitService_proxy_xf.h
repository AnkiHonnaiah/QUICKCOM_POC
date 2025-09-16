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
/**        \file  WarningDisplayExecutable/include/amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_proxy_xf.h
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

#ifndef WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_XF_H_
#define WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/ipc_binding_transformation_layer/internal/services/speedlimitservice/SpeedLimitService_proxy_event_xf_backends.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_factory_interface.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/remote_server_interface.h"
#include "ipc_binding_core/internal/remote_server_manager_interface.h"
#include "ipc_binding_core/internal/required_service_instance_id.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"
#include "ipc_binding_xf_common/internal/trace/trace_sink_factory_interface.h"
#include "services/speedlimitservice/SpeedLimitService_proxy_backend_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

namespace gen_services {
namespace gen_speedlimitservice {

// VECTOR Next Construct Metric-OO.WMC.One: MD_IPCBINDING_Metric-OO.WMC.Generated_Code_One_methods_per_class
/*!
 * \brief Required service interface deployment of service interface 'SpeedLimitService'.
 *
 * \unit       IpcBinding::IpcBindingTransformationLayer::ProxyInstance
 * \complexity Many calls to ProxyEvent and ProxyMethod units to construct backends.
 */
class SpeedLimitServiceProxyXf final
    : public ::services::speedlimitservice::internal::SpeedLimitServiceProxyBackendInterface {
 public:
  /*!
   * \brief Type alias for the SpeedLimitServiceProxyBackendInterface.
   */
  using ProxyBackendInterface  = ::services::speedlimitservice::internal::SpeedLimitServiceProxyBackendInterface;

  /*!
   * \brief Type alias for Client ID.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  /*!
   * \brief Type alias for the service runtime configuration.
   */
  using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Type alias for the trace sink factory interface.
   */
  using TraceSinkFactoryInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface;

  /*!
   * \brief Type alias for the ProxyRouterFactoryInterface.
   */
  using ProxyRouterFactoryInterface = ::amsr::ipc_binding_core::internal::ProxyRouterFactoryInterface;

   /*!
   * \brief Type alias for the RemoteServerManagerInterface.
   */
  using RemoteServerManagerInterface = ::amsr::ipc_binding_core::internal::RemoteServerManagerInterface;

  /*!
   * \brief Type alias for the RemoteServerInterface.
   */
  using RemoteServerInterface = ::amsr::ipc_binding_core::internal::RemoteServerInterface;

  /*!
   * \brief Type alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for RequiredServiceInstanceId.
   */
  using RequiredServiceInstanceId = ::amsr::ipc_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Constructor of ProxyXf for service 'SpeedLimitService'.
   *
   * \param[in,out] remote_server_manager            RemoteServerManagerInterface reference.
   * \param[in]     provided_service_instance_id     The provided service instance ID.
   * \param[in]     required_service_instance_id     The required service instance ID.
   * \param[in]     client_id                        The client ID unique to this proxy instance.
   * \param[in]     integrity_level                  The integrity level for the required service instance.
   * \param[in]     service_config                   The runtime service config for this service.
   * \param[in]     trace_sink_factory               Reference to the trace sink factory.
   * \param[in]     proxy_router_factory             Reference to the ProxyRouterFactory.
   *
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
   SpeedLimitServiceProxyXf(
      RemoteServerManagerInterface& remote_server_manager,
      ProvidedServiceInstanceId provided_service_instance_id,
      RequiredServiceInstanceId required_service_instance_id,
      ClientId client_id,
      ::amsr::ipc::IntegrityLevel integrity_level,
      ServiceConfig const& service_config,
      TraceSinkFactoryInterface const& trace_sink_factory,
      ProxyRouterFactoryInterface const& proxy_router_factory) noexcept;

  /*!
   * \brief Destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~SpeedLimitServiceProxyXf() noexcept final;

  SpeedLimitServiceProxyXf(SpeedLimitServiceProxyXf const&) noexcept = delete;
  auto operator=(SpeedLimitServiceProxyXf const&) noexcept -> SpeedLimitServiceProxyXf& = delete;
  SpeedLimitServiceProxyXf(SpeedLimitServiceProxyXf&&) noexcept = delete;
  auto operator=(SpeedLimitServiceProxyXf&&) noexcept -> SpeedLimitServiceProxyXf& = delete;

  /*!
   * \brief Get the connection state between proxy and skeleton.
   *
   * \return Current connection state.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ::amsr::socal::SkeletonConnectionState ReadSkeletonConnectionState() noexcept final;

 // ---- Events ---------------------------------------------------------------------------------------------------

  /*!
   * \brief Getter function for the event 'SpeedOverLimitEvent'.
   *
   * \return A reference to a valid event backend.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  SpeedLimitServiceProxyEventXfBackendSpeedOverLimitEvent& GetEventBackendSpeedOverLimitEvent() noexcept final;


  // ---- Fields ---------------------------------------------------------------------------------------------------
  // ---- Fields ---------------------------------------------------------------------------------------------------

  // ---- Methods ----------
  // ---- Methods ----------


  // ---- Fields ----------

 private:
  /*!
   * \brief Type alias for Method ID.
   */
  using MethodId = ::amsr::ipc_binding_core::internal::ipc_protocol::MethodId;

  /*!
   * \brief Type alias for Method ID.
   */
  using EventId = ::amsr::ipc_binding_core::internal::ipc_protocol::EventId;

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

  /*!
   * \brief Type alias for the ProxyRouterInterface.
   */
  using ProxyRouterInterface = ::amsr::ipc_binding_core::internal::ProxyRouterInterface;

  /*!
   * \brief Provided service instance ID of this proxy instance.
   */
  ProvidedServiceInstanceId const provided_service_instance_id_;

  /*!
   * \brief The client ID of the ProxyXf instance.
   */
  ClientId const client_id_;

  /*!
   * \brief A shared pointer to the RemoteServer.
   */
  amsr::SharedPtr<RemoteServerInterface> remote_server_;

  /*!
   * \brief The ProxyRouter instance.
   */
  amsr::UniquePtr<ProxyRouterInterface> proxy_router_;

  // ---- Methods ----------
  // ---- Methods ----------

  // ---- Events ----------

  /*!
   * \brief Reference to the runtime configuration of event 'SpeedOverLimitEvent'.
   */
  EventConfig const& event_SpeedOverLimitEvent_config_;

  /*!
   * \brief Event backend for proxy event 'SpeedOverLimitEvent'.
   */
  SpeedLimitServiceProxyEventXfBackendSpeedOverLimitEvent event_backend_SpeedOverLimitEvent_;
  // ---- Events ----------

  // ---- Fields ----------
  // ---- Fields ------------------------------------------------------------------------------------------
};

}  // namespace gen_speedlimitservice
}  // namespace gen_services

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_XF_H_


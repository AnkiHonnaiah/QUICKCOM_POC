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
/**        \file  client_transport_to_daemon_interface.h
 *        \brief  "Thread safe" interface for client transport to daemon.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_TRANSPORT_TO_DAEMON_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_TRANSPORT_TO_DAEMON_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/future.h"
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/types.h"
#include "someip_daemon_client/internal/common_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Client Transport To Daemon interface.
 */
class ClientTransportToDaemonInterface {
 public:
  /*!
   * \brief Use default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ClientTransportToDaemonInterface() noexcept = default;

  /*!
   * \brief Use default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ClientTransportToDaemonInterface() noexcept = default;

  ClientTransportToDaemonInterface(ClientTransportToDaemonInterface const&) = delete;
  ClientTransportToDaemonInterface(ClientTransportToDaemonInterface&&) = delete;
  ClientTransportToDaemonInterface& operator=(ClientTransportToDaemonInterface const&) & = delete;
  ClientTransportToDaemonInterface& operator=(ClientTransportToDaemonInterface&&) & = delete;

  /*!
   * \brief       Call StartServiceDiscovery on SomeIpDaemonClient, and translate the result.
   * \param[in]   service_instance  SOME/IP Service instance ID.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \pre         -
   * \context     Init, App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> StartServiceDiscovery(
      RequiredServiceInstanceId const& service_instance) noexcept = 0;

  /*!
   * \brief       Call StopServiceDiscovery on SomeIpDaemonClient, and translate the result.
   * \param[in]   service_instance  SOME/IP Service instance ID.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \pre         -
   * \context     Shutdown, App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> StopServiceDiscovery(RequiredServiceInstanceId const& service_instance) noexcept = 0;

  /*!
   * \brief       Call RequestService on SomeIpDaemonClient, and translate the result.
   * \param[in]   service_instance  SOME/IP Service instance ID.
   * \return      Unique Client ID or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<::amsr::someip_protocol::internal::ClientId> RequestService(
      ProvidedServiceInstanceId const& service_instance) noexcept = 0;

  /*!
   * \brief       Call ReleaseService on SomeIpDaemonClient, and translate the result.
   * \param[in]   proxy_id  SOME/IP unique client identifier.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> ReleaseService(ProxyBindingIdentity const& proxy_id) noexcept = 0;

  /*!
   * \brief       Call SubscribeEvent on SomeIpDaemonClient, and translate the result.
   * \param[in]   proxy_binding_identity   Proxy binding identity, containing service id,
   *                                       instance id and client id.
   * \param[in]   event_id                 SOME/IP event id.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ClientError::kRuntimeResourceOutOf If IPC queue from SOME/IP Daemon is limited and full.
   * \error       ClientError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> SubscribeEvent(
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_binding_identity,
      ::amsr::someip_protocol::internal::EventId const event_id) noexcept = 0;

  /*!
   * \brief       Call UnsubscribeEvent on SomeIpDaemonClient, and translate the result.
   * \param[in]   proxy_binding_identity   Proxy binding identity, containing service id,
   *                                       instance id and client id.
   * \param[in]   event_id                 SOME/IP event id.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> UnsubscribeEvent(
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_binding_identity,
      ::amsr::someip_protocol::internal::EventId const event_id) noexcept = 0;

  /*!
   * \brief       Call SendMethodRequest on SomeIpDaemonClient, and translate the result.
   * \param[in]   instance_id  Method instance ID.
   * \param[in]   packet       Serialized method request to be sent out.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> SendMethodRequest(::amsr::someip_protocol::internal::InstanceId const instance_id,
                                                     MemoryBufferPtr packet) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_TRANSPORT_TO_DAEMON_INTERFACE_H_

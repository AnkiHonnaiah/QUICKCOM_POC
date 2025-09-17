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
/**        \file  amsr/someip_binding_core/internal/server_transport_to_daemon_interface.h
 *        \brief  "Thread safe" interface for server transport to daemon
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_TRANSPORT_TO_DAEMON_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_TRANSPORT_TO_DAEMON_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "osabstraction/io/common_types.h"
#include "someip-protocol/internal/types.h"
#include "someipd_app_protocol/internal/protocol_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Server Transport To Daemon interface.
 */
class ServerTransportToDaemonInterface {
 public:
  /*!
   * \brief Use default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ServerTransportToDaemonInterface() noexcept = default;

  /*!
   * \brief Use default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ServerTransportToDaemonInterface() noexcept = default;

  ServerTransportToDaemonInterface(ServerTransportToDaemonInterface const&) = delete;
  ServerTransportToDaemonInterface(ServerTransportToDaemonInterface&&) = delete;
  ServerTransportToDaemonInterface& operator=(ServerTransportToDaemonInterface const&) & = delete;
  ServerTransportToDaemonInterface& operator=(ServerTransportToDaemonInterface&&) & = delete;

  /*!
   * \brief       Call RequestLocalServer on SomeIpDaemonClient, and translate the result.
   * \details     This call will send a request to the Daemon, then wait until either
   *              the corresponding response is received, or
   *              the configured response timeout has been exceeded.
   *              The result is translated to a ServerError and returned.
   * \param[in]   service_instance  SOME/IP Service instance ID.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ServerError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ServerError::kSystemConfigurationNotAvailable if this service instance is not configured.
   * \error       ServerError::kSystemPrivilegesNoAccess if access is denied for this service.
   *              configured.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> RequestLocalServer(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance) noexcept = 0;

  /*!
   * \brief       Call ReleaseLocalServer on SomeIpDaemonClient, and translate the result.
   * \param[in]   service_instance  SOME/IP Service instance ID.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> ReleaseLocalServer(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance) noexcept = 0;

  /*!
   * \brief       Call OfferService on SomeIpDaemonClient, and translate the result.
   * \param[in]   service_instance  SOME/IP Service instance ID.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> OfferService(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance) noexcept = 0;

  /*!
   * \brief       Call StopOfferService on SomeIpDaemonClient, and translate the result.
   * \param[in]   service_instance  SOME/IP Service instance ID.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> StopOfferService(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance) noexcept = 0;

  /*!
   * \brief       Call Send on SomeIpDaemonClient, and translate the result.
   * \param[in]   instance_id  Service instance ID.
   * \param[in]   packet       Serialized SOME/IP Event to be sent out.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> SendEventNotification(amsr::someip_protocol::internal::InstanceId const instance_id,
                                                         MemoryBufferPtr packet) noexcept = 0;

  /*!
   * \brief       Call Send on SomeIpDaemonClient, and translate the result.
   * \param[in]   instance_id  Service instance ID.
   * \param[in]   packet       Serialized PDU to be sent out.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> SendPduEventNotification(
      amsr::someip_protocol::internal::InstanceId const instance_id, MemoryBufferPtr packet) noexcept = 0;

  /*!
   * \brief       Call Send on SomeIpDaemonClient, and translate the result.
   * \param[in]   instance_id  Service instance ID.
   * \param[in]   packet       Serialized method response to be sent out.
   * \param[in]   requester_address Contaions the IP address and port of the sender of the method request.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> SendMethodResponse(
      amsr::someip_protocol::internal::InstanceId const instance_id, MemoryBufferPtr packet,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_TRANSPORT_TO_DAEMON_INTERFACE_H_

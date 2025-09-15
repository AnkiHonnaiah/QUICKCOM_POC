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
/**        \file  amsr/someip_binding_core/internal/server_transport_to_daemon.h
 *        \brief  This class translates the calls between the SomeIpBinding and SomeIpDaemonClient
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::ServerTransportToDaemon
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_TRANSPORT_TO_DAEMON_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_TRANSPORT_TO_DAEMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>
#include "amsr/core/future.h"
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/polling_utilities.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/someip_binding_core/internal/server_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_server_error.h"
#include "amsr/someip_binding_core/internal/someip_daemon_client_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someipd_app_protocol/internal/protocol_types.h"

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using vac::container::literals::operator""_sv;

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Server Transport To Daemon.
 */
class ServerTransportToDaemon final : public ServerTransportToDaemonInterface {
 private:
  /*!
   * \brief Instance ID
   */
  using InstanceId = ::amsr::someip_protocol::internal::InstanceId;

  /*!
   * \brief Control message return code.
   */
  using ControlMessageReturnCode = ::amsr::someipd_app_protocol::internal::ControlMessageReturnCode;

  /*!
   * \brief Type-alias for the SafeIpcPollingInterface.
   */
  using SafeIpcPollingInterface = ::amsr::someip_binding_core::internal::SafeIpcPollingInterface;

 public:
  /*!
   * \brief Constructor
   * \param[in] someip_daemon_client SOME/IP Daemon Client.
   * \param[in] safe_ipc_polling  Contains poll function, used to trigger the reactor to receive responses.
   * \param[in] is_polling_mode   true if runtime polling mode is used, false otherwise.
   * \param[in] response_timeout  timeout to wait for responses.
   *
   * \context   Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \steady FALSE
   */
  ServerTransportToDaemon(SomeIpDaemonClientInterface& someip_daemon_client,
                          std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling, bool const is_polling_mode,
                          std::chrono::steady_clock::duration const response_timeout) noexcept;

  /*!
   * \brief       Default destructor.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ServerTransportToDaemon() noexcept final = default;

  ServerTransportToDaemon(ServerTransportToDaemon const&) = delete;
  ServerTransportToDaemon(ServerTransportToDaemon&&) = delete;
  ServerTransportToDaemonInterface& operator=(ServerTransportToDaemon const&) & = delete;
  ServerTransportToDaemon& operator=(ServerTransportToDaemon&&) & = delete;

 private:
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
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> RequestLocalServer(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance) noexcept final;

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
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> ReleaseLocalServer(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance) noexcept final;

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
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> OfferService(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance) noexcept final;

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
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> StopOfferService(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance) noexcept final;

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
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> SendEventNotification(InstanceId const instance_id, MemoryBufferPtr packet) noexcept final;

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
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> SendPduEventNotification(InstanceId const instance_id,
                                                    MemoryBufferPtr packet) noexcept final;

  /*!
   * \brief       Call Send on SomeIpDaemonClient, and translate the result.
   * \param[in]   instance_id  Service instance ID.
   * \param[in]   packet       Serialized method response to be sent out.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<void> SendMethodResponse(
      InstanceId const instance_id, MemoryBufferPtr packet,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) noexcept final;

  /*!
   * \brief Wait for the future being ready, or until the configured timeout happens.
   * \tparam T [1 variant] Exact type of the Result/Future.
   * \param[in] future  Future to wait on
   * \return Corresponding result of that future, or error if timeout happened.
   * \error ServerError::kRuntimeConnectionTimeout if timeout happens
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If runtime polling mode is used:
   *   - Trigger poll function until future has value or timeout is reached.
   *   - If timeout has been reached before response is received, return kTimeout error result.
   *   - If response is received,
   *     - If result is kOk, return void result.
   *     - else, return an error result.
   * - else if runtime event driven mode is used:
   *   - Wait on the future until future has value or timeout is reached.
   *   - If timeout has been reached before response is received, return kTimeout error result.
   *   - If response is received,
   *     - If result is kOk, return void result.
   *     - else, return an error result.
   * \endinternal
   */
  template <typename T>
  auto WaitForResponseT(amsr::core::Future<T>&& future) const noexcept -> amsr::core::Result<T> {
    // In this variable, the result from SomeIpDaemonClient is mapped, such that:
    //   (someipd_client_result has value && value is kOk) -> result has value void.
    //   (someipd_client_result has value && value is not kOk) -> result has error.
    //   (someipd_client_result has no value) -> result has error.
    //   (No value at all received) -> result has kTimeout error.
    amsr::core::Result<T> result{::amsr::someip_binding_core::internal::ServerError::kRuntimeConnectionTimeout
                                 /*kRuntimeConnectionTimeout*/};

    // Wait until response is received.
    if (is_polling_mode_) {
      bool const response_received{PollingUtilities::WaitForInPollingMode(
          safe_ipc_polling_, [&future]() { return (future.is_ready()); }, response_timeout_)};

      if (response_received) {
        result = future.GetResult();
      }
    } else {
      amsr::core::future_status const future_status{future.wait_for(response_timeout_)};

      if (future_status == amsr::core::future_status::ready) {
        result = future.GetResult();
      }
    }

    return result;
  }

  /*!
   * \brief Translate return code from SOME/IP Daemon Client into server error code.
   * \param[in] ret_code Return code to be translated.
   * \return translated return code.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::someip_binding_core::internal::ServerError TranslateError(
      ::amsr::someipd_app_protocol::internal::ControlMessageReturnCode const ret_code) noexcept;

  /*!
   * \brief Translate error code from SOME/IP Daemon Client into server error code.
   * \param[in] error_code Error code to be translated.
   * \return translated error code.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::someip_binding_core::internal::ServerError TranslateError(
      amsr::core::ErrorDomain::CodeType const error_code) noexcept;

  /*!
   * \brief Interface towards SomeIpDaemonClient
   */
  SomeIpDaemonClientInterface& someip_daemon_client_;

  /*!
   * \brief Interface for polling wrappers which contains reactor polling function, it is called while waiting for
   * a command response.
   */
  std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling_;

  /*!
   * \brief Check if polling mode is used.
   */
  bool const is_polling_mode_;

  /*!
   * \brief Lock for avoiding concurrent calling of RequestLocalServer
   */
  std::mutex request_local_server_lock_{};

  /*!
   * \brief Timeout a response shall be waited for until error result is returned.
   */
  std::chrono::steady_clock::duration const response_timeout_;

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, "ServerTransportToDaemon"_sv};
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_TRANSPORT_TO_DAEMON_H_

/*!
 * \exclusivearea ::amsr::someip_binding_core::internal::ServerTransportToDaemon::request_local_server_lock_
 *                Ensures not calling RequestLocalServer at the same time.
 *
 * \protects ::amsr::someip_binding_core::internal::ServerTransportToDaemon::someip_daemon_client_
 *
 * \usedin  ::amsr::someip_binding_core::internal::ServerTransportToDaemon::RequestLocalServer
 *
 * \length  LONG IPC communication.
 * \endexclusivearea
 */

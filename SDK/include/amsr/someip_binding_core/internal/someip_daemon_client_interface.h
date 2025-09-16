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
/**        \file  amsr/someip_binding_core/internal/someip_daemon_client_interface.h
 *        \brief  SOME/IP Daemon Client Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_DAEMON_CLIENT_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_DAEMON_CLIENT_INTERFACE_H_

#include "amsr/core/future.h"
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip_daemon_client/internal/client_interface.h"
#include "someip_daemon_client/internal/server_interface.h"
#include "someip_daemon_client/internal/types.h"
#include "someipd_app_protocol/internal/protocol_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class for communication between SomeIpBindingCore and SomeIpDaemonClient
 */
class SomeIpDaemonClientInterface {
 public:
  /*!
   * \brief IPC protocol control message return code
   */
  using ControlMessageReturnCode = amsr::someipd_app_protocol::internal::ControlMessageReturnCode;

  /*!
   * \brief Define default constructor.
   */
  SomeIpDaemonClientInterface() = default;

  /*!
   * \brief Define destructor.
   */
  virtual ~SomeIpDaemonClientInterface() = default;

  SomeIpDaemonClientInterface(SomeIpDaemonClientInterface const&) = delete;
  SomeIpDaemonClientInterface(SomeIpDaemonClientInterface&&) = delete;
  SomeIpDaemonClientInterface& operator=(SomeIpDaemonClientInterface const&) & = delete;
  SomeIpDaemonClientInterface& operator=(SomeIpDaemonClientInterface&&) & = delete;

  /*!
   * \brief Requests a service instance from the SOME/IP daemon.
   * \details Assembles a serialized stream, sends the serialized command to the SOME/IP daemon and waits until the
   *          response from the SOME/IP daemon is received.
   *
   * \param[in] proxy_creation_required_service_instance Required service instance id
   *
   * \return Future that containts a client ID from the SOME/IP daemon upon success. Otherwise, it contains an error.
   * \error       SomeIpDaemonClientErrc::kAccessDenied If the application has no right to request a service.
   * \error       SomeIpDaemonClientErrc::kIncorrectResponseMsgLength If the message deserialization failed.
   * \error       SomeIpDaemonClientErrc::kNotConnected If the Communication with SOME/IP Daemon failed.
   * \error       SomeIpDaemonClientErrc::kPayloadEmpty If an empty response message payload is received.
   * \error       SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured
   *              and queue is full.
   *
   * \pre Any pending future has to be awaited before calling RequestService again.
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous  FALSE
   *
   */
  virtual auto RequestService(::amsr::someip_daemon_client::internal::ProxyCreationRequiredServiceInstanceType const&
                                  proxy_creation_required_service_instance) noexcept
      -> amsr::core::Future<::amsr::someip_protocol::internal::ClientId> = 0;

  /*!
   * \brief Releases a previously requested SOME/IP client ID.
   * \param[in] requested_service Requested service.
   *
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured
   *        and queue is full.
   *
   * \pre The input client id is a previously allocated SOME/IP client id.
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous  TRUE
   *
   */
  virtual auto ReleaseService(
      ::amsr::someip_daemon_client::internal::RequestedServiceType const& requested_service) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Requests a start of service discovery of a service instance from the SOME/IP daemon.
   * \details After calling this function, an application must be ready to process requests destined to this service
   * instance.
   * \param[in] proxy_required_service_instance Required service instance ID
   *
   * \return Future containing return code to indicate request result, or an error code if IPC transmission fails.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous  FALSE
   *
   */
  virtual auto StartServiceDiscovery(amsr::someip_daemon_client::internal::ProxySDRequiredServiceInstanceType const&
                                         proxy_required_service_instance) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Requests a stop of service discovery of a service instance from the SOME/IP daemon.
   * \details After calling this function, an application must stop processing requests destined to this service
   * instance.
   * \param[in] proxy_required_service_instance Required service instance ID
   *
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous  TRUE
   *
   */
  virtual auto StopServiceDiscovery(amsr::someip_daemon_client::internal::ProxySDRequiredServiceInstanceType const&
                                        proxy_required_service_instance) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Lets the SOME/IP daemon know that an application wishes to receive an event of a service instance.
   * \details After calling this function, an application must be ready to process the event sent by this service
   *          instance.
   * \param[in] required_event Required event ID to subscribe
   *
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   *        queue is full.
   *
   * \pre -
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous  TRUE
   *
   */
  virtual auto SubscribeEvent(::amsr::someip_daemon_client::internal::RequiredEventType const& required_event) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Lets the SOME/IP daemon know that an application does not wish to receive an event of a service instance.
   * \details After calling this function, the SOME/IP daemon stops forwarding events with the specified id of the
   * specified service instance.
   * \param[in] required_event Required event ID to unsubscribe
   *
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous  TRUE
   *
   */
  virtual auto UnsubscribeEvent(
      ::amsr::someip_daemon_client::internal::RequiredEventType const& required_event) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Initiates the transmission of a SomeIp routing message.
   * \details The method may return before the given message has been transmitted.
   *          Outgoing routing messages might be queued.
   * \param[in] instance_id SOME/IP service instance identifier.
   * \param[in] packet A memory buffer containing a routing message.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre Start() has been called.
   * \context App
   * \reentrant FALSE
   * \synchronous TRUE
   *
   */
  virtual auto ProxySendSomeIpMessage(amsr::someip_protocol::internal::InstanceId const instance_id,
                                      MemoryBufferPtr&& packet) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Sets a receive handler for client-side incoming SOME/IP messages, asynchronous notifications of offered
   *        services and event subscription state changes.
   * \param[in] client_manager A pointer to the server-side handler object to which received SOME/IP messages,
   *                           offered service instances and event subscription state changes will be passed.
   *
   * \pre client_manager is not a nullptr.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   */
  virtual void SetClientManager(::amsr::someip_daemon_client::internal::ClientInterface* client_manager) noexcept = 0;

  /*!
   * \brief Requests a local server to the SOME/IP daemon.
   * \details After calling this function, this application will own a unique local server instance in the
   *          SOME/IP daemon and is allowed to call OfferService. No other Application can request this local server
   *          until this application has released it.
   * \param[in] provided_service_instance Id of the SOME/IP provided service instance.
   * \return Future containing return code to indicate request result, or an error code if IPC transmission fails.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context App
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous  FALSE
   *
   */
  virtual auto RequestLocalServer(
      amsr::someip_daemon_client::internal::ProvidedServiceInstanceType const& provided_service_instance) noexcept
      -> amsr::core::Future<ControlMessageReturnCode> = 0;

  /*!
   * \brief Releases a local server in the SOME/IP daemon.
   * \details After calling this function, the skeleton can no longer offer the service, and other skeletons can request
   *          ownership of the local server in the SOME/IP daemon.
   * \param[in] provided_service_instance Id of the SOME/IP provided service instance.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \synchronous  TRUE
   *
   */
  virtual auto ReleaseLocalServer(
      amsr::someip_daemon_client::internal::ProvidedServiceInstanceType const& provided_service_instance) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Offers a service instance to the SOME/IP daemon.
   * \details After calling this function, an application must be ready to process requests destined to this service
   * instance.
   * \param[in] provided_service_instance Id of the SOME/IP provided service instance.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \synchronous  FALSE
   *
   */
  virtual auto OfferService(
      amsr::someip_daemon_client::internal::ProvidedServiceInstanceType const& provided_service_instance) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Stops offering a service instance to the SOME/IP daemon.
   * \details After calling this function, the SOME/IP daemon stops forwarding requests destined to this service
   * instance.
   * \param[in] provided_service_instance Id of the SOME/IP provided service instance.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context    App
   * \threadsafe TRUE
   * \reentrant  FALSE
   * \synchronous  TRUE
   *
   */
  virtual auto StopOfferService(
      amsr::someip_daemon_client::internal::ProvidedServiceInstanceType const& provided_service_instance) noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Initiates the transmission of a SomeIp routing message.
   * \details The method may return before the given message has been transmitted.
   *          Outgoing routing messages might be queued.
   * \param[in] instance_id SOME/IP service instance identifier.
   * \param[in] packet A memory buffer containing a routing message.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context   App | Reactor
   * \reentrant FALSE
   *
   */
  virtual auto SkeletonSendSomeIpMessage(amsr::someip_protocol::internal::InstanceId const instance_id,
                                         MemoryBufferPtr&& packet) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief     Initiates the transmission of a PDU routing message.
   * \details   The method may return before the given message has been transmitted.
   *            Outgoing routing messages might be queued.
   * \param[in] instance_id SOME/IP service instance identifier.
   * \param[in] packet A memory buffer containing a routing message.
   * \return    Empty result in case of success or an error code otherwise.
   * \error     SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error     SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured
   * and queue is full.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   *
   */
  virtual auto SkeletonSendPduMessage(amsr::someip_protocol::internal::InstanceId const instance_id,
                                      MemoryBufferPtr&& packet) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Sets a receive handler for server-side incoming SOME/IP messages.
   * \param[in] server_manager A pointer to the server-side handler object to which received SOME/IP messages
   * will be passed.
   *
   * \pre server_manager is not a nullptr.
   * \context    Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   *
   */
  virtual void SetServerManager(::amsr::someip_daemon_client::internal::ServerInterface* server_manager) noexcept = 0;

  /*!
   * \brief Start all dynamic actions of the SOME/IP daemon client.
   * \details In this API context, asynchronous messages reception is started.
   *
   * \pre Connect() is called
   *
   */
  virtual void Start() noexcept = 0;

  /*!
   * \brief Connect to the SOME/IP daemon listening on the configured address.
   * \details This method initiates a new connection to SOME/IP daemon.
   * \return  Future of state showing if the connection has been established and false otherwise.
   * \error   amsr::ipc::Connection::ConnectAsync() error codes if the connect failed synchronously.
   * \error   SomeIpDaemonClientErrc::kAlreadyConnected The error is returned if connection is already established.
   *
   * \pre SomeIpDaemon must be running and must accept connection requests
   * \context Init
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate
   * \synchronous FALSE
   *
   */
  virtual auto Connect() noexcept -> amsr::core::Future<void> = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_DAEMON_CLIENT_INTERFACE_H_

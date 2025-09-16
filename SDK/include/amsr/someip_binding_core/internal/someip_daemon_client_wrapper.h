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
/**        \file  amsr/someip_binding_core/internal/someip_daemon_client_wrapper.h
 *        \brief  Wrapper for SomeIpDaemonClient
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::SomeIpDaemonClientWrapper
 *
 *      \details  This is a wrapper class for SomeIpDaemonClient unit of SomeIpDaemonClient component.
 *                The wrapper only forwards the calls to the corresponding functions of the SomeIpDaemonClient unit.
 *                It is mainly used to decouple both components and let testing of this component possible without
 *                depending on SomeIpDaemonClient component.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_DAEMON_CLIENT_WRAPPER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_DAEMON_CLIENT_WRAPPER_H_

#include <memory>
#include "amsr/someip_binding_core/internal/someip_daemon_client_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip_daemon_client/internal/someip_daemon_client.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Class that wraps all the SOME/IP DaemonClient functionalities
 */
template <typename SomeIpDaemonClient>
class SomeIpDaemonClientWrapper final : public SomeIpDaemonClientInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \param[in] someip_daemon_client Unique pointer to SomeIpDaemonClient.
   * \context   Init
   * \steady    FALSE
   */
  explicit SomeIpDaemonClientWrapper(std::unique_ptr<SomeIpDaemonClient> someip_daemon_client)
      : SomeIpDaemonClientInterface{},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        someip_daemon_client_{std::move(someip_daemon_client)},
        proxy_someip_daemon_client_{someip_daemon_client_->GetProxy()},
        skeleton_someip_daemon_client_{someip_daemon_client_->GetSkeleton()} {};

  /*!
   * \brief Define destructor.
   * \context   Shutdown
   * \steady    FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SomeIpDaemonClientWrapper() noexcept final = default;

  SomeIpDaemonClientWrapper(SomeIpDaemonClientWrapper const&) = delete;
  SomeIpDaemonClientWrapper(SomeIpDaemonClientWrapper&&) = delete;
  SomeIpDaemonClientWrapper& operator=(SomeIpDaemonClientWrapper const&) & = delete;
  SomeIpDaemonClientWrapper& operator=(SomeIpDaemonClientWrapper&&) & = delete;

  /*!
   * \brief Start all dynamic actions of the SOME/IP daemon client.
   * \details In this API context, asynchronous messages reception is started.
   * \context Init
   * \threadsafe FALSE
   * \vprivate
   * \steady     FALSE
   * \pre Connect() is called
   *
   */
  void Start() noexcept final {
    // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
    someip_daemon_client_->Start();
  }

  /*!
   * \brief Connect to the SOME/IP daemon listening on the configured address.
   * \details This method initiates a new connection to SOME/IP daemon.
   * \return  Future of state showing if the connection has been established and false otherwise.
   * \error   amsr::ipc::Connection::ConnectAsync() error codes if the connect failed synchronously.
   * \error   SomeIpDaemonClientErrc::kAlreadyConnected The error is returned if connection is already established.
   * \pre SomeIpDaemon must be running and must accept connection requests
   *
   * \context Init
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \vprivate
   * \synchronous  FALSE
   * \steady       FALSE
   * \internal
   * - Establish connection to the SOME/IP daemon (asynchronous)
   * \endinternal
   */
  auto Connect() noexcept -> amsr::core::Future<void> final {
    // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
    return someip_daemon_client_->Connect();
  }

  /*!
   * \brief Releases a previously requested SOME/IP client ID.
   * \param[in] requested_service Requested service.
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre The input client id is a previously allocated SOME/IP client id.
   * \context App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  auto ReleaseService(::amsr::someip_daemon_client::internal::RequestedServiceType const& requested_service) noexcept
      -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_PROXY_SOMEIPDAEMONCLIENT_LIFECYCLE
    return (*proxy_someip_daemon_client_.ReleaseService)(requested_service);
  }

  /*!
   * \brief Requests a start of service discovery of a service instance from the SOME/IP daemon.
   * \details After calling this function, an application must be ready to process requests destined to this service
   * instance.
   * \param[in] proxy_required_service_instance Required service instance ID
   * \return Future containing return code to indicate request result, or an error code if IPC transmission fails.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kDisconnected If disconnect called before response received.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   *
   */
  auto StartServiceDiscovery(amsr::someip_daemon_client::internal::ProxySDRequiredServiceInstanceType const&
                                 proxy_required_service_instance) noexcept -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_PROXY_SOMEIPDAEMONCLIENT_LIFECYCLE
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return (*proxy_someip_daemon_client_.StartServiceDiscovery)(proxy_required_service_instance);
  }

  /*!
   * \brief Requests a stop of service discovery of a service instance from the SOME/IP daemon.
   * \details After calling this function, an application must stop processing requests destined to this service
   * instance.
   * \param[in] proxy_required_service_instance Required service instance ID
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  auto StopServiceDiscovery(amsr::someip_daemon_client::internal::ProxySDRequiredServiceInstanceType const&
                                proxy_required_service_instance) noexcept -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_PROXY_SOMEIPDAEMONCLIENT_LIFECYCLE
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return (*proxy_someip_daemon_client_.StopServiceDiscovery)(proxy_required_service_instance);
  }

  /*!
   * \brief Requests a service instance from the SOME/IP daemon.
   * \details Assembles a serialized stream, sends the serialized command to the SOME/IP daemon. After calling this
   *          function, an application must be ready to process requests destined to this service instance.
   * \param[in]  proxy_creation_required_service_instance Required service instance Id.
   *
   * \return amsr::core::Future<::amsr::someip_protocol::internal::ClientId> containing a client ID from the
   *         SOME/IP daemon upon success. Otherwise, it contains an error.
   * \error  SomeIpDaemonClientErrc::kAccessDenied If the application has no right to request a service.
   * \error  SomeIpDaemonClientErrc::kIncorrectResponseMsgLength If the message deserialization failed.
   * \error  SomeIpDaemonClientErrc::kNotConnected If the Communication with SOME/IP Daemon failed.
   * \error  SomeIpDaemonClientErrc::kPayloadEmpty If an empty response message payload is received.
   * \error  SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured
   * and queue is full.
   *
   * \pre Any pending future has to be awaited before calling RequestService again.
   * \context App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   */
  auto RequestService(::amsr::someip_daemon_client::internal::ProxyCreationRequiredServiceInstanceType const&
                          proxy_creation_required_service_instance) noexcept
      -> amsr::core::Future<::amsr::someip_protocol::internal::ClientId> final {
    // VCA_SOMEIPBINDING_PROXY_SOMEIPDAEMONCLIENT_LIFECYCLE
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return (*proxy_someip_daemon_client_.RequestService)(proxy_creation_required_service_instance);
  }

  /*!
   * \brief Lets the SOME/IP daemon know that an application wishes to receive an event of a service instance.
   * \details After calling this function, an application must be ready to process the event sent by this service
   *          instance.
   * \param[in] required_event Required event ID to subscribe
   *
   * \return Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  auto SubscribeEvent(::amsr::someip_daemon_client::internal::RequiredEventType const& required_event) noexcept
      -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_PROXY_SOMEIPDAEMONCLIENT_LIFECYCLE
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return (*proxy_someip_daemon_client_.SubscribeEvent)(required_event);
  }

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
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  auto UnsubscribeEvent(::amsr::someip_daemon_client::internal::RequiredEventType const& required_event) noexcept
      -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_PROXY_SOMEIPDAEMONCLIENT_LIFECYCLE
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return (*proxy_someip_daemon_client_.UnsubscribeEvent)(required_event);
  }

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
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto ProxySendSomeIpMessage(amsr::someip_protocol::internal::InstanceId const instance_id,
                              MemoryBufferPtr&& packet) noexcept -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_PROXY_SOMEIPDAEMONCLIENT_LIFECYCLE
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    // VCA_SOMEIPBINDING_PASSING_REFERENCE
    return (*proxy_someip_daemon_client_.SendSomeIpMessage)(instance_id, std::move(packet));
  }

  /*!
   * \brief Sets a receive handler for client-side incoming SOME/IP messages, asynchronous notifications of offered
   *        services and event subscription state changes.
   * \param[in] client_manager A pointer to the server-side handler object to which received SOME/IP messages,
   *                           offered service instances and event subscription state changes will be passed.
   *
   * \pre client_manager is not a nullptr.
   * \context Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  void SetClientManager(::amsr::someip_daemon_client::internal::ClientInterface* client_manager) noexcept final {
    // VCA_SOMEIPBINDING_PROXY_SOMEIPDAEMONCLIENT_LIFECYCLE
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    proxy_someip_daemon_client_.SetClientManager(client_manager);
  }

  /*!
   * \brief Requests a local server to the SOME/IP daemon.
   * \details After calling this function, the skeleton will own a unique local server instance in the
   *          SOME/IP daemon and is allowed to call OfferService. No other Application can request this local server
   *          until this application has released it.
   * \param[in] provided_service_instance Id of the SOME/IP provided service instance.
   * \return Future containing return code to indicate request result, or an error code if IPC transmission fails.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   * \pre -
   * \context      App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   */
  auto RequestLocalServer(
      amsr::someip_daemon_client::internal::ProvidedServiceInstanceType const& provided_service_instance) noexcept
      -> amsr::core::Future<ControlMessageReturnCode> final {
    // VCA_SOMEIPBINDING_SKELETON_SOMEIPDAEMONCLIENT_LIFECYCLE
    return (*skeleton_someip_daemon_client_.RequestLocalServer)(provided_service_instance);
  }

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
   * \context      App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  auto ReleaseLocalServer(
      amsr::someip_daemon_client::internal::ProvidedServiceInstanceType const& provided_service_instance) noexcept
      -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_SKELETON_SOMEIPDAEMONCLIENT_LIFECYCLE
    return (*skeleton_someip_daemon_client_.ReleaseLocalServer)(provided_service_instance);
  }

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
   * \context      App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   */
  auto OfferService(
      amsr::someip_daemon_client::internal::ProvidedServiceInstanceType const& provided_service_instance) noexcept
      -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_SKELETON_SOMEIPDAEMONCLIENT_LIFECYCLE
    return (*skeleton_someip_daemon_client_.OfferService)(provided_service_instance);
  }

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
   * \context      App
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  auto StopOfferService(
      amsr::someip_daemon_client::internal::ProvidedServiceInstanceType const& provided_service_instance) noexcept
      -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_SKELETON_SOMEIPDAEMONCLIENT_LIFECYCLE
    return (*skeleton_someip_daemon_client_.StopOfferService)(provided_service_instance);
  }

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
   * \steady    TRUE
   */
  auto SkeletonSendSomeIpMessage(amsr::someip_protocol::internal::InstanceId const instance_id,
                                 MemoryBufferPtr&& packet) noexcept -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_SKELETON_SOMEIPDAEMONCLIENT_LIFECYCLE
    return (*skeleton_someip_daemon_client_.SendSomeIpMessage)(instance_id, std::move(packet));
  }

  /*!
   * \brief     Initiates the transmission of a PDU routing message.
   * \details   The method may return before the given message has been transmitted.
   *            Outgoing routing messages might be queued.
   * \param[in] instance_id SOME/IP service instance identifier.
   * \param[in] packet A memory buffer containing a routing message.
   * \return    Empty result in case of success or an error code otherwise.
   * \error SomeIpDaemonClientErrc::kNotConnected The error is returned if not connected.
   * \error SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if limited IPC queue is configured and
   * queue is full.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  auto SkeletonSendPduMessage(amsr::someip_protocol::internal::InstanceId const instance_id,
                              MemoryBufferPtr&& packet) noexcept -> amsr::core::Result<void> final {
    // VCA_SOMEIPBINDING_SKELETON_SOMEIPDAEMONCLIENT_LIFECYCLE
    return (*skeleton_someip_daemon_client_.SendPduMessage)(instance_id, std::move(packet));
  }

  /*!
   * \brief Sets a receive handler for server-side incoming SOME/IP messages.
   * \param[in] server_manager A pointer to the server-side handler object to which received SOME/IP messages
   * will be passed.
   *
   * \pre server_manager is not a nullptr.
   * \context    Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  void SetServerManager(::amsr::someip_daemon_client::internal::ServerInterface* server_manager) noexcept final {
    // VCA_SOMEIPBINDING_SKELETON_SOMEIPDAEMONCLIENT_LIFECYCLE
    skeleton_someip_daemon_client_.SetServerManager(server_manager);
  }

 private:
  /*!
   * \brief Type alias for ProxySomeIpDaemonClientType.
   */
  using ProxySomeIpDaemonClient = typename SomeIpDaemonClient::ProxySomeIpDaemonClient;

  /*!
   * \brief Type alias for SkeletonSomeIpDaemonClientType.
   */
  using SkeletonSomeIpDaemonClient = typename SomeIpDaemonClient::SkeletonSomeIpDaemonClient;

  /*!
   * \brief SomeIpDaemonClient Unique pointer.
   */
  std::unique_ptr<SomeIpDaemonClient> someip_daemon_client_;

  /*!
   * \brief Templated ProxySomeIpDaemonClient reference.
   */
  ProxySomeIpDaemonClient& proxy_someip_daemon_client_;

  /*!
   * \brief Templated SkeletonSomeIpDaemonClient reference.
   */
  SkeletonSomeIpDaemonClient& skeleton_someip_daemon_client_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_DAEMON_CLIENT_WRAPPER_H_

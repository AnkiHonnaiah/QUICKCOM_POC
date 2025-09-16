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
/*! \file
 *  \brief Server logic for handling service updates sent to the Ipc Service Discovery Daemon.
 *  \unit IpcServiceDiscovery::Centralized::SdServer
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_SD_SERVER_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_SD_SERVER_H_

#include "amsr/ipc_service_discovery/centralized/internal/ipc_server/ipc_server.h"
#include "amsr/ipc_service_discovery/centralized/internal/message/message_parser.h"
#include "amsr/ipc_service_discovery/centralized/internal/message/messages.h"
#include "amsr/ipc_service_discovery/centralized/internal/sd_server/client_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/sd_server/provided_service_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/sd_server/required_service_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/constants.h"
#include "amsr/ipc_service_discovery/common/internal/logger/logger.h"
#include "amsr/ipc_service_discovery/common/internal/reactor_software_event/reactor_software_event.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/reactor1_interface.h"
#include "ara/core/result.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Implementation of the SdServer handling connections and messages from clients participating
 *        in the IpcServiceDiscovery centralized backend.
 * \details The SdServer keeps track of the connected clients via the ClientRegistry and provided or
 *          required services using a pair of ProvidedServiceRegistry and RequiredServiceRegistry for
 *          each binding. If a client violates the IpcServiceDiscovery centralized backend protocol, the
 *          client will be dropped. The SdServer does not require any locks to protect its internal state
 *          because all the relevant modifications to it are done from reactor context.
 * \vprivate Component Private
 */
class SdServer final : MessageParserCustomArgs<ConnectionHandle> {
 public:
  using ClientRegistry = sd_server::ClientRegistry;
  using ProvidedServiceRegistry = sd_server::ProvidedServiceRegistry;
  using ProvidedServiceRegistryEntry = sd_server::ProvidedServiceRegistryEntry;
  using RequiredServiceRegistry = sd_server::RequiredServiceRegistry;
  using RequiredServiceRegistryEntry = sd_server::RequiredServiceRegistryEntry;

  /*!
   * \brief Construct the SdServer.
   * \param[in] reactor Reference to the reactor.
   * \param[in] server_address Local address of the internal ipc server acceptor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit SdServer(external::internal::osabstraction::Reactor1Interface& reactor,
                    UnicastAddress const& server_address) noexcept;

  /*!
   * \brief Destroy the SdServer and release all system resources.
   * \details After a stop call, there may still be ongoing asynchronous operations and timers. The caller must
   *          ensure that all pending asynchronous operations are finished or that they are not executed concurrently.
   *          This can be checked using the IsInUse() API.
   *          One way to assert this is by moving the cleanup code into the reactor thread context.
   * \context ANY
   * \pre No ongoing asynchronous operation.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~SdServer() noexcept;

  /*! Delete default constructor, copy and move constructors and operators. */
  SdServer() = delete;
  SdServer(SdServer const&) = delete;
  SdServer(SdServer&&) = delete;
  auto operator=(SdServer const&) -> SdServer = delete;
  auto operator=(SdServer&&) -> SdServer = delete;

  /*!
   * \brief Start the SdServer.
   * \details Start accepting connections from clients and processing of messages.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerCommunication
   * \spec requires true; \endspec
   */
  void Start() noexcept;

  /*!
   * \brief Stop the SdServer.
   * \details Stop accepting connections from clients and close all connections. Provide kShutdown as close connection
   *          cause.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerCommunication
   * \spec requires true; \endspec
   */
  void Stop() noexcept;

  /*!
   * \brief Check if the SdServer is still executing asynchronous actions.
   * \details This function should be called after Stop() was called if one
   *          wants to check if any asynchronous action is still executing. Note that this function always returns true
   *          if the SdServer is still running. See ~SdServer() why such a check may be necessary.
   * \return true if the SdServer is still running or any asynchronous action is currently executing,
   *         false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerIsInUse
   */
  auto IsInUse() noexcept -> bool;

 private:
  /*!
   * \brief Map for the ProvidedServiceRegistries.
   */
  using ProvidedServiceRegistryMap = std::unordered_map<BindingType, ProvidedServiceRegistry>;

  /*!
   * \brief Map for the RequiredServiceRegistries.
   */
  using RequiredServiceRegistryMap = std::unordered_map<BindingType, RequiredServiceRegistry>;

  /*!
   * \brief Executed upon a client connection being established.
   * \details Only logging is done during this routine, as the protocol expects the client to send an InitMessage next.
   * \param[in] connection_handle Handle to the connected client.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerCommunication
   */
  void OnConnected(ConnectionHandle connection_handle) const noexcept;

  /*!
   * \brief Executed upon reception of a message.
   * \details Parse the incoming message and trigger the relevant On<type>Message function.
   * \param[in] connection_handle Handle to the client that has sent the message.
   * \param[in] buffer_view View to the message contents.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerCommunication
   */
  void OnMessageReceived(ConnectionHandle connection_handle, ConstBufferConstView& buffer_view) noexcept;

  /*!
   * \brief Executed upon a client connection being terminated.
   * \details If the client was registered and has provided or required service instances before, the service instances
   *          listeners will be notified via StopOfferServiceMessage when the reason for the disconnect is not
   *          IpcServer::kShutdown or close_connection_cause contains an error. Additionally, the disconnected client
   *          is cleared as listener for all required service instances. Finally, the client will be unregistered from
   *          the ClientRegistry.
   * \param[in] connection_handle Handle to the connected client.
   * \param[in] close_connection_cause_result A result containing either a value reporting the reason of the disconnect
   *                                   or an error if caused by an error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerDisconnect
   */
  void OnDisconnected(ConnectionHandle connection_handle,
                      ::ara::core::Result<CloseConnectionCause> close_connection_cause_result) noexcept;

  /*!
   * \brief Executed upon reception of an InitMessage.
   * \details The version and binding type will be checked for validity. If the InitMessage is valid,
   *          the client will be registered in the ClientRegistry. If the message is not valid, the
   *          client will be dropped.
   * \param[in] message_result Result containing either the parsed message or an error.
   * \param[in] connection_handle Handle to the client that has sent the message.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolClientToServer
   */
  // VECTOR NC AutosarC++17_10-M8.4.2: MD_IPCSERVICEDISCOVERY_M8.4.2_differentNameUsedForParameter
  void OnInitMessage(::ara::core::Result<InitMessage> const& message_result,
                     ConnectionHandle connection_handle) noexcept final;

  /*!
   * \brief Executed upon reception of an OfferServiceMessage.
   * \details If the client is registered and the offer is valid, the ProvidedServiceRegistry is
   *          updated and requiring clients notified accordingly. Otherwise, the client has
   *          violated the protocol and will be dropped.
   * \param[in] message_result Result containing either the parsed message or an error.
   * \param[in] connection_handle Handle to the client that has sent the message.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolClientToServer
   */
  // VECTOR NC AutosarC++17_10-M8.4.2: MD_IPCSERVICEDISCOVERY_M8.4.2_differentNameUsedForParameter
  void OnOfferServiceMessage(::ara::core::Result<OfferServiceMessage> const& message_result,
                             ConnectionHandle connection_handle) noexcept final;

  /*!
   * \brief Executed upon reception of a StopOfferServiceMessage.
   * \details If the client is registered and the stop offer is valid, the ProvidedServiceRegistry is
   *          updated and requiring clients notified accordingly. Otherwise, the client has
   *          violated the protocol and will be dropped.
   * \param[in] message_result Result containing either the parsed message or an error.
   * \param[in] connection_handle Handle to the client that has sent the message.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolClientToServer
   */
  // VECTOR NC AutosarC++17_10-M8.4.2: MD_IPCSERVICEDISCOVERY_M8.4.2_differentNameUsedForParameter
  void OnStopOfferServiceMessage(::ara::core::Result<StopOfferServiceMessage> const& message_result,
                                 ConnectionHandle connection_handle) noexcept final;

  /*!
   * \brief Executed upon reception of a ListenServiceMessage.
   * \details If the client is registered and the listen is valid, the RequiredServiceRegistry is
   *          updated and - should matching offers already exist - the listening client notified accordingly.
   *          Otherwise, the client has violated the protocol and will be dropped.
   * \param[in] message_result Result containing either the parsed message or an error.
   * \param[in] connection_handle Handle to the client that has sent the message.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolClientToServer
   */
  // VECTOR NC AutosarC++17_10-M8.4.2: MD_IPCSERVICEDISCOVERY_M8.4.2_differentNameUsedForParameter
  void OnListenServiceMessage(::ara::core::Result<ListenServiceMessage> const& message_result,
                              ConnectionHandle connection_handle) noexcept final;

  /*!
   * \brief Executed upon reception of a StopListenServiceMessage.
   * \details If the client is registered and the stop listen is valid, the RequiredServiceRegistry is
   *          updated. Otherwise, the client has violated the protocol and will be dropped.
   * \param[in] message_result Result containing either the parsed message or an error.
   * \param[in] connection_handle Handle to the client that has sent the message.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolClientToServer
   */
  // VECTOR NC AutosarC++17_10-M8.4.2: MD_IPCSERVICEDISCOVERY_M8.4.2_differentNameUsedForParameter
  void OnStopListenServiceMessage(::ara::core::Result<StopListenServiceMessage> const& message_result,
                                  ConnectionHandle connection_handle) noexcept final;

  /*!
   * \brief Send an OfferServiceMessage to matching listeners.
   * \details If sending fails, the client is dropped.
   * \param[in] binding_type Type of binding the update concerns.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier the update concerns.
   * \param[in] unicast_address UnicastAddress of the updated service instance.
   * \context REACTOR
   * \pre Shall only be called from reactor context.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolClientToServer
   */
  void SendOfferToMatchingListeners(BindingType binding_type,
                                    ProvidedServiceInstanceIdentifier service_instance_identifier,
                                    UnicastAddress unicast_address) noexcept;

  /*!
   * \brief Send a StopOfferServiceMessage to matching listeners.
   * \details If sending fails, the client is dropped.
   * \param[in] binding_type Type of binding the update concerns.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier the update concerns.
   * \param[in] unicast_address UnicastAddress of the updated service instance.
   * \context REACTOR
   * \pre Shall only be called from reactor context.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolClientToServer
   */
  void SendStopOfferToMatchingListeners(BindingType binding_type,
                                        ProvidedServiceInstanceIdentifier service_instance_identifier,
                                        UnicastAddress unicast_address) noexcept;

  /*!
   * \brief Send matching offers to a client requiring a service instance.
   * \details If sending fails, the client is dropped.
   * \param[in] binding_type Type of binding the update concerns.
   * \param[in] connection_handle Handle to the client.
   * \param[in] service_instance_identifier ServiceInstanceIdentifier the client requires.
   * \context REACTOR
   * \pre Shall only be called from reactor context.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolClientToServer
   */
  void SendMatchingOffersToListener(BindingType binding_type, ConnectionHandle requiring_client,
                                    RequiredServiceInstanceIdentifier service_instance_identifier) noexcept;

  /*!
   * \brief Send a message to a client.
   * \details If sending fails, the client is dropped.
   * \param[in] connection_handle Handle to the client.
   * \param[in] buffer_view Buffer view to a message.
   * \context REACTOR
   * \pre Shall only be called from reactor context.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerSend
   */
  void Send(ConnectionHandle connection_handle, ConstBufferConstView buffer_view) noexcept;

  /*!
   * \brief Drop the client by closing the connection.
   * \details Note that, by design of the underlying IpcServer the OnDisconnect() callback will be triggered by the
   *          close.
   * \param[in] connection_handle Handle to the client.
   * \context REACTOR
   * \pre Shall only be called from reactor context.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdServerDisconnect
   */
  void DropClient(ConnectionHandle connection_handle) noexcept;

  /*!
   * \brief Get the ProvidedServiceRegistry for the given BindingType.
   * \param[in] binding_type The BindingType.
   * \return A reference to the ProvidedServiceRegistry.
   * \context ANY
   * \pre A ProvidedServiceRegistry for the given BindingType exists.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetProvidedServiceRegistry(BindingType binding_type) noexcept -> ProvidedServiceRegistry&;

  /*!
   * \brief Get the RequiredServiceRegistry for the given BindingType.
   * \param[in] binding_type The BindingType.
   * \return A reference to the RequiredServiceRegistry.
   * \context ANY
   * \pre A RequiredServiceRegistry for the given BindingType exists.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetRequiredServiceRegistry(BindingType binding_type) noexcept -> RequiredServiceRegistry&;

  /*! Logger for the SdServer unit. */
  Logger logger_{Constants::kLoggerPrefixSdServer};

  /*! IpcServer connected callback. */
  IpcServer::ConnectedCallback connected_callback_{
      // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_CONNECTION_CALLBACKS
      [this](ConnectionHandle connection_handle) { OnConnected(connection_handle); }};

  /*! IpcServer receive message callback. */
  IpcServer::ReceiveMessageCallback receive_message_callback_{
      [this](ConnectionHandle connection_handle, ConstBufferConstView& buffer_view) {
        // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_CONNECTION_CALLBACKS
        OnMessageReceived(connection_handle, buffer_view);
      }};

  /*! IpcServer disconnected callback. */
  IpcServer::DisconnectedCallback disconnected_callback_{
      [this](ConnectionHandle connection_handle, ::ara::core::Result<CloseConnectionCause> close_connection_cause) {
        // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_CONNECTION_CALLBACKS
        OnDisconnected(connection_handle, close_connection_cause);
      }};

  /*! The IpcServer. */
  IpcServer ipc_server_;

  /*! ClientRegistry for registration of clients. */
  ClientRegistry client_registry_{};

  /*! Mapping from binding type to ProvidedServiceRegistry. */
  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  ProvidedServiceRegistryMap provided_registry_map_{{BindingType{BindingType::Binding::kZeroCopy}, {}},
                                                    {BindingType{BindingType::Binding::kIpc}, {}}};

  /*! Mapping from binding type to RequiredServiceRegistry. */
  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  RequiredServiceRegistryMap required_registry_map_{{BindingType{BindingType::Binding::kZeroCopy}, {}},
                                                    {BindingType{BindingType::Binding::kIpc}, {}}};

  /*! Friend declarations for testing purposes. */
  FRIEND_TEST(UT__SdServer__SdServer, OnDisconnectedCustomCause);
  FRIEND_TEST(UT__SdServer__SdServer, OnDisconnectedClientsInLoop);
};
}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_SERVER_SD_SERVER_H_

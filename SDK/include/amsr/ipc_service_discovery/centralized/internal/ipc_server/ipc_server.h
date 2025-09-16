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
 *  \brief SafeIpc server to accept connections, receive messages and send responses to multiple SafeIpc clients.
 *  \unit IpcServiceDiscovery::Centralized::IpcServer
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_SERVER_IPC_SERVER_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_SERVER_IPC_SERVER_H_

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "amsr/ipc_service_discovery/centralized/internal/ipc_common/connection_manager.h"
#include "amsr/ipc_service_discovery/centralized/internal/ipc_service_discovery_centralized_error_domain.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/buffer_types.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/close_connection_cause.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/constants.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/logger/logger.h"
#include "amsr/ipc_service_discovery/common/internal/reactor_software_event/reactor_software_event.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/acceptor.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/connection.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/reactor1_interface.h"
#include "ara/core/result.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief A threadsafe SafeIpc server that can handle multiple connections to SafeIpc clients.
 * \details The IpcServer asynchronously accepts connections to SafeIpc clients. Sending messages
 *          is done synchronously and receiving messages happens asynchronously. The user callbacks are
 *          executed without the mutex held, so it is explicitly allowed and encouraged to use the
 *          IpcServer API in the callbacks.
 * \vprivate Component Private
 */
class IpcServer final {
  /*!
   * \brief Object pool for receive buffer containers.
   */
  using ReceiveBufferPool = ObjectPool<ReceiveBufferContainer>;

 public:
  /*!
   * \brief Error code type for the centralized backend.
   */
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;

  /*!
   * \brief Interface of the callback function indicates that a connection to a SafeIpc client could be
   *        established successfully.
   * \param[in] connection_handle The handle of the established connection.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerConnectionEstablishment
   */
  using ConnectedCallback = vac::language::UniqueFunction<void(ConnectionHandle connection_handle)>;

  /*!
   * \brief Interface of the callback function that handles the received message.
   * \details The IpcServer owns the memory, i.e. data consistency of the buffer is only guaranteed for the duration
   *          of this callback function. If the message needs to be stored in order to be processed further,
   *          it must be copied by the user.
   * \param[in] connection_handle The handle of the connection from which the message was received.
   * \param[in] buffer_view A const buffer view to the message buffer. Valid only during the function call.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerReceive
   */
  using ReceiveMessageCallback =
      vac::language::UniqueFunction<void(ConnectionHandle connection_handle, ConstBufferConstView& buffer_view)>;

  /*!
   * \brief Interface of the callback function indicates that a connection to a SafeIpc client was terminated.
   * \param[in] connection_handle The handle of the terminated connection.
   * \param[in] close_connection_cause A result containing either a value reporting the reason of the disconnect
   *                                   or an error if caused by an error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerCloseConnection
   */
  using DisconnectedCallback = vac::language::UniqueFunction<void(
      ConnectionHandle connection_handle, ::ara::core::Result<CloseConnectionCause> close_connection_cause)>;

  /*!
   * \brief Create an IpcServer.
   * \param[in] reactor The reactor must be valid as long as this IpcServer instance exists.
   * \param[in] connected_callback Callback to notify about a successful connection establishment
   *                               to an IpcClient. The IpcServer takes ownership of the passed callback.
   *                               It holds the ownership until the IpcServer is destroyed.
   * \param[in] receive_message_callback Callback to notify about a successfully received message.
   *                                     The IpcServer takes ownership of the passed callback.
   *                                     It holds the ownership until the IpcClient is destroyed.
   * \param[in] disconnected_callback Callback to notify about a connection termination to an IpcClient.
   *                                  The IpcServer takes ownership of the passed callback.
   *                                  It holds the ownership until the IpcServer is destroyed.
   * \param[in] server_address Local address of the internal acceptor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  IpcServer(external::internal::osabstraction::Reactor1Interface& reactor, ConnectedCallback connected_callback,
            ReceiveMessageCallback receive_message_callback, DisconnectedCallback disconnected_callback,
            UnicastAddress const& server_address) noexcept;

  /*!
   * \brief Destroy the IpcServer and releases all system resources.
   * \details After closing all connections, there may still be ongoing asynchronous operations and timers.
   *          The caller must ensure that all pending asynchronous operations are finished or that they are
   *          not executed concurrently. This can be checked using the IsInUse() API. One way to assert this
   *          is by moving the cleanup code into the reactor thread context.
   * \context ANY
   * \pre No ongoing asynchronous operation.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerDestruction
   */
  ~IpcServer() noexcept;

  IpcServer() = delete;
  IpcServer(IpcServer const&) = delete;
  IpcServer(IpcServer&&) = delete;
  auto operator=(IpcServer const&) -> IpcServer& = delete;
  auto operator=(IpcServer&&) -> IpcServer& = delete;

  /*!
   * \brief Start accepting of SafeIpc connections asynchronously.
   * \details The accept process is performed asynchronously. The connected callback informs the user
   *          about a successfully completed connect process. This function aborts if accepting new
   *          connections is not possible.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerConnectionEstablishment
   */
  void StartAcceptConnections() noexcept;

  /*!
   * \brief Stop accepting of SafeIpc connections asynchronously.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerConnectionEstablishment
   * \spec requires true; \endspec
   */
  void StopAcceptConnections() noexcept;

  /*!
   * \brief Close the connection to a SafeIpc client. Calls the DisconnectedCallback if a connection was closed.
   * \details The connection will be destroyed if it is not currently executing a callback. If
   *          the connection is currently executing a callback (which might be the very callback executing this
   *          function), a reactor software event will be scheduled in order to clean up closed but not yet
   *          destroyed connections. The reason for the close connection call, by default set to kDefault,
   *          is passed through to the DisconnectedCallback.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, this function returns an error.
   * \param[in] close_connection_cause A CloseConnectionCause indicating the reason for the close connections call.
   * \return A result containing either a value on success or an error if an error occurred.
   * \error IpcServiceDiscoveryCentralizedErrc::kNoSuchConnection No connection with that handle exists.
   * \error IpcServiceDiscoveryCentralizedErrc::kDisconnected The connection was already closed.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerCloseConnection
   * \spec requires true; \endspec
   */
  auto CloseConnection(ConnectionHandle connection_handle,
                       CloseConnectionCause close_connection_cause = CloseConnectionCause::kDefault) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Close all connections to SafeIpc clients and return a list of all successful closed connections.
   * \details A connection will be destroyed if it is not currently executing a callback. If
   *          the connection is currently executing a callback (which might be the very callback executing this
   *          function), a reactor software event will be scheduled in order to clean up closed but not yet
   *          destroyed connections. If a connection is closed its connection handle is added to the to be returned
   *          result vector.
   * \return A vector with the connection handles of all successful closed connections.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerCloseConnection
   * \spec requires true; \endspec
   */
  auto CloseConnections() noexcept -> std::vector<ConnectionHandle>;

  /*!
   * \brief Synchronously send a message to a SafeIpc client.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, this function returns an error.
   * \param[in] buffer_view A view to the buffer holding the serialized message that should be transmitted must not
   *                        exceed kMaximumMessageSize. The packet view must not be modified during the function call.
   * \return A result containing either a value on success or an error if an error occurred.
   * \error IpcServiceDiscoveryCentralizedErrc::kNoSuchConnection No connection with that handle exists.
   * \error IpcServiceDiscoveryCentralizedErrc::kDisconnected Connection was terminated during send.
   * \error IpcServiceDiscoveryCentralizedErrc::kSendBufferFull Send buffer of SafeIpc is full.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerSend
   * \spec requires true; \endspec
   */
  auto SendSync(ConnectionHandle connection_handle, ConstBufferConstView& buffer_view) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Check if the IpcServer is still executing asynchronous actions.
   * \details This function should be called after StopAcceptConnections() and CloseConnections() was called if one
   *          wants to check if any asynchronous action is still executing. Note that this function always returns true
   *          if the IpcServer is still connected to any SafeIpc client. See ~IpcServer() why such a check may be
   *          necessary.
   * \return true if the IpcServer is connected to any SafeIpc client or any asynchronous action is currently executing,
   *         false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerIsInUse
   * \spec requires true; \endspec
   */
  auto IsInUse() noexcept -> bool;

 private:
  /*!
   * \brief Get a reference to the ConnectionManager for the requested ConnectionHandle.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, this function returns an error.
   * \return A result containing either a reference to the ConnectionManager on success or an error if an error
   *         occurred.
   * \error IpcServiceDiscoveryCentralizedErrc::kNoSuchConnection No connection with that handle exists.
   * \context ANY
   * \pre Shall only be called from an exclusive area. This is an internal helper. No preconditions are checked.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetConnectionManager(ConnectionHandle connection_handle) noexcept -> ::ara::core::Result<ConnectionManager&>;

  /*!
   * \brief Asynchronously accept the next SafeIpc connection. OnAccept() is called upon accepting of a connection.
   * \return A result containing either a value on success or an error if an error occurred.
   * \error osabstraction::OsabErrc::kUninitialized Tried to AcceptAsync() for uninitialized acceptor. This happens if
   *                                                StartAcceptConnections() has not been called yet.
   * \error osabstraction::OsabErrc::kBusy AcceptAsync() operating already ongoing. This happens if
   *                                       AcceptNextConnection() is called multiple times without the OnAccept()
   *                                       function being triggered.
   * \error osabstraction::OsabErrc::kDisconnected AcceptAsync() operation was cancelled. See AcceptAsync() of Acceptor
   *                                               why this might happen.
   * \context ANY
   * \pre Shall only be called from an exclusive area. This is an internal helper. No preconditions are checked.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  auto AcceptNextConnection() noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Close the connection to a SafeIpc client.
   * \details A connection will be destroyed if it is not currently executing a callback. If the connection is
   *          currently executing a callback (which might be the very callback executing this function),
   *          a reactor software event will be scheduled in order to clean up closed but not yet destroyed connections.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, this function returns an error.
   * \return A result containing either a value on success or an error if an error occurred.
   * \error IpcServiceDiscoveryCentralizedErrc::kNoSuchConnection No connection with that handle exists.
   * \error IpcServiceDiscoveryCentralizedErrc::kDisconnected The connection was already closed.
   * \context ANY
   * \pre Shall only be called from an exclusive area. This is an internal helper. No preconditions are checked.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  auto CloseConnectionInternal(ConnectionHandle connection_handle) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Receive the next message from the SafeIpc connection. OnMessageAvailable() is called upon an available
   *        message and OnReceiveCompletion() is called upon completion of the receive operation.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, this function returns an error.
   * \return A result containing either a value on success or an error if an error occurred.
   * \error IpcServiceDiscoveryCentralizedErrc::kNoSuchConnection No connection with that handle exists.
   * \error osabstraction::OsabErrc::kUninitialized Tried to receive from not connected connection.
   * \error osabstraction::OsabErrc::kBusy An asynchronous receive operation is currently in progress.
   * \context ANY
   * \pre Shall only be called from an exclusive area. This is an internal helper. No preconditions are checked.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  auto ReceiveAsync(ConnectionHandle connection_handle) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Synchronously send a message over the SafeIpc connection.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, this function returns an error.
   * \param[in] buffer_view A ConstConstBufferView to the message buffer to be sent.
   * \return A result containing either a value on success or an error if an error occurred.
   * \error IpcServiceDiscoveryCentralizedErrc::kNoSuchConnection No connection with that handle exists.
   * \error IpcServiceDiscoveryCentralizedErrc::kDisconnected Connection was terminated during send.
   * \error IpcServiceDiscoveryCentralizedErrc::kSendBufferFull Send buffer of SafeIpc is full.
   * \context ANY
   * \pre Shall only be called from an exclusive area. This is an internal helper. No preconditions are checked.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto TransmitMessage(ConnectionHandle connection_handle, ConstBufferConstView& buffer_view) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Handle any error that occurs during the usage of a SafeIpc connection by closing that connection.
   *        Calls the DisconnectedCallback if a connection was closed.
   * \details The mutex is not held during the DisconnectedCallback call.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, this function returns an error.
   * \param[in] error_code The error code of the error that has occurred.
   * \context ANY
   * \pre Shall only be called outside an exclusive area. This is an internal helper. No preconditions are checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerErrorHandling
   */
  void HandleConnectionError(ConnectionHandle connection_handle, ::ara::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief Called upon accepting of a new SafeIpc connection. Calls the ConnectedCallback if a connection was
   *        established.
   * \details Constructs a new ConnectionManager and a unique ConnectionHandle for the incoming connection
   *          request and starts listening for incoming messages from that connection asynchronously. Then the
   *          asynchronous listener for the next incoming connection is started. If any error occurs during handling of
   *          an incoming connection, this function aborts.
   * \param[in] accept_result A rvalue reference to a Result containing either a valid SafeIpc connection or an error.
   * \context REACTOR
   * \pre Shall never be called outside the SafeIpc callback functions. This is an internal helper. No preconditions
   *      are checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerConnectionEstablishment
   */
  void OnAccept(::ara::core::Result<external::internal::osabstraction::Connection>&& accept_result) noexcept;

  /*!
   * \brief Called upon a new SafeIpc message being available.
   * \details Acquires a ReceiveBufferContainer for the ConnectionManager for the respective connection from
   *          the buffer pool.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, HandleConnectionError() is called.
   * \param[in] message_size The number of bytes that are available from the SafeIpc connection.
   *                         The message_size must not exceed kMaximumMessageSize.
   * \return A MutableIOBufferView to the acquired buffers.
   * \context REACTOR
   * \pre Shall never be called outside the SafeIpc callback functions. This is an internal helper. No preconditions
   *      are checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerReceive
   */
  auto OnMessageAvailable(ConnectionHandle connection_handle, std::size_t message_size) noexcept -> MutableIOBufferView;

  /*!
   * \brief Called upon the receive process for a new SafeIpc message being complete. Calls the ReceiveMessageCallback
   *        if receiving the message has succeeded.
   * \details Passes the received message to the user callback and listens for the next incoming message. Checks for
   *          any error during the receive process and calls HandleConnectionError() if an error occurred.
   *          The mutex is not held during the ReceiveMessageCallback call.
   * \param[in] connection_handle The handle of the connection. If there is no such connection with this
   *                              handle, HandleConnectionError() is called.
   * \param[in] receive_complete_result A Result containing the actual number of bytes read into the provided buffer.
   * \context REACTOR
   * \pre Shall never be called outside the SafeIpc callback functions. This is an internal helper.
   *      No preconditions are checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerReceive
   */
  void OnReceiveCompletion(ConnectionHandle connection_handle,
                           ::ara::core::Result<std::size_t> const& receive_complete_result) noexcept;

  /*!
   * \brief Called upon triggering the connection cleanup software event. Destroys all connections currently not in use.
   * \details Destroys all connections currently not in use, which should be all the connections that have been closed
   *          before this function executing. This holds because the software event is called within the Reactor
   *          context, and it is impossible for a SafeIpc callback to be executed at the same time.
   * \context REACTOR
   * \pre Shall never be called outside the ReactorSoftwareEvent callback functions. This is an internal helper.
   *      No preconditions are checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerCloseConnection
   */
  void OnConnectionCleanup() noexcept;

  /*!
   * \brief Called upon triggering the connection disconnected software event. Calls DisconnectedCallback for all closed
   *        connections.
   * \details This function is called from the registered reactor software event callback if the software event was
              triggered. The mutex is not held during the disconnect callback call.
   * \context REACTOR
   * \pre Shall never be called outside the ReactorSoftwareEvent callback functions. This is an internal helper.
   *      No preconditions are checked.
   * \threadsafe True
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcServerCloseConnection
   */
  void OnConnectionsDisconnectedEvent() noexcept;

  /*!
   * \brief Register and trigger the disconnected connection reactor software event and update the closed connections
   *        map.
   * \details The DisconnectedCallbacks shall be called within the reactor context. Therefore they are called
   *          within a reactor software event. This method checks if the event is already registered and if not
   *          registers and triggers the event. Additionally the internal closed_connections_ map is updated with the
   *          newly closed connections.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetupDisconnectedReactorSoftwareEvent(ConnectionHandle connection_handle,
                                             ::ara::core::Result<CloseConnectionCause> close_connection_cause) noexcept;

  /*! Logger for the IpcServer unit. */
  Logger logger_{Constants::kLoggerPrefixIpcServer};

  /*! Map for storage mapping connection handles to their connection manager. */
  std::unordered_map<ConnectionHandle, ConnectionManager> connections_{};

  /*! Pool for receive buffers. */
  ReceiveBufferPool receive_buffer_pool_{};

  /*! Reactor for software events */
  external::internal::osabstraction::Reactor1Interface& reactor_;

  /*! Acceptor for SafeIpc connections. */
  external::internal::osabstraction::Acceptor acceptor_;

  /*! Callback for established connection to a SafeIpc client. */
  ConnectedCallback connected_callback_;

  /*! Callback for a received SafeIpc message. */
  ReceiveMessageCallback receive_message_callback_;

  /*! Callback for terminated connection to a SafeIpc client. */
  DisconnectedCallback disconnected_callback_;

  /*! Callback for cleanup of closed connections in a reactor software event. */
  // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT_IPC_SERVER_CALLBACKS
  ReactorSoftwareEvent::SoftwareEventCallback connection_cleanup_callback_{[this]() { OnConnectionCleanup(); }};

  /*! Software event for cleanup of closed connections */
  ReactorSoftwareEvent connection_cleanup_software_event_{reactor_, std::move(connection_cleanup_callback_)};

  /*! Callback for calls of disconnected callbacks in a reactor software event. */
  ReactorSoftwareEvent::SoftwareEventCallback connection_disconnected_callback_{
      // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT_IPC_SERVER_CALLBACKS
      [this]() { OnConnectionsDisconnectedEvent(); }};

  /*! Reactor software event for disconnect callback calls. */
  ReactorSoftwareEvent connection_disconnected_software_event_{reactor_, std::move(connection_disconnected_callback_)};

  /*! Mutex used to protect internal state. */
  mutable std::mutex mutex_{};

  /*! Counter used to generate unique ConnectionHandles for this instance. */
  ConnectionHandle connection_handle_counter_{0};

  /*! Map for storage mapping of connection handle and closing cause, needed for calling DisconnectedCallbacks.*/
  std::unordered_map<ConnectionHandle, ::ara::core::Result<CloseConnectionCause>> connections_closed_{};

  /*! Friend declarations for testing purposes. */
  FRIEND_TEST(UT__IpcServer__IpcServer, HandleConnectionErrorNoSuchConnection);
};

/*!
 * \exclusivearea   ::amsr::ipc_service_discovery::internal::IpcServer::mutex_
 *                  Ensures consistency while reading/modifying the internal connections.
 * \protects        ::amsr::ipc_service_discovery::internal::IpcServer::connections_
                    ::amsr::ipc_service_discovery::internal::IpcServer::connections_closed_
 * \usedin
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::StartAcceptConnections
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::StopAcceptConnections
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::CloseConnection
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::CloseConnections
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::SendSync
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::IsInUse
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::HandleConnectionError
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::OnAccept
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::OnMessageAvailable
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::OnReceiveCompletion
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::OnConnectionCleanup
 *                  ::amsr::ipc_service_discovery::internal::IpcServer::OnConnectionsDisconnectedEvent
 * \exclude         All other methods.
 * \length          LONG
 *                  Handle messages and calling callbacks when mutex locked.
 *                  Depth call-tree sometimes > 1.
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_SERVER_IPC_SERVER_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END

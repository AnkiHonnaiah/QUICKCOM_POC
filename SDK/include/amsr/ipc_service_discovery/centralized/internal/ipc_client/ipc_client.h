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
 *  \brief SafeIpc client which establishes the connection to any SafeIpc acceptor by periodically trying to connect.
           The client sends messages to and receives messages from a SafeIpc server.
 *  \unit IpcServiceDiscovery::Centralized::IpcClient
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_CLIENT_IPC_CLIENT_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_CLIENT_IPC_CLIENT_H_

#include <cstdint>
#include <mutex>
#include "amsr/ipc_service_discovery/centralized/internal/ipc_common/receive_buffer_container.h"
#include "amsr/ipc_service_discovery/centralized/internal/ipc_service_discovery_centralized_error_domain.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/buffer_types.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/close_connection_cause.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/constants.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/message_buffer_type.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/logger/logger.h"
#include "amsr/ipc_service_discovery/common/internal/reactor_software_event/reactor_software_event.h"
#include "amsr/ipc_service_discovery/common/internal/types/types.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/connection.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/reactor1_interface.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/timer_interface.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/timer_manager_interface.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief A SafeIpc client to send and receive messages from a SafeIpc server.
 * \details The IpcClient establishes a SafeIpc connection to a SafeIpc acceptor. Sending messages
 *          is done synchronously and receiving messages happens asynchronously. Furthermore,
 *          the client implements a connection establishment retry mechanism. Note, that the
 *          acceptor does not necessarily have to be implemented by a SafeIpc server unit in
 *          this component. Thus, this implementation is agnostic to the actual peer behind the acceptor.
 *
 * \vprivate Component Private
 */
class IpcClient final {
 public:
  /*!
   * \brief Error code type for the centralized backend.
   */
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;

  /*!
   * \brief Interface of the callback function that handles the received message.
   * \details The IpcClient owns the memory, i.e. data consistency of the buffer is only guaranteed for the duration
   *          of this callback function. If the message needs to be stored in order to be processed further,
   *          it must be copied by the user.
   * \param[in] buffer_view A const buffer view to the reception buffer. Valid only during the function call.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcClientReceive
   */
  using ReceiveMessageCallback = vac::language::UniqueFunction<void(ConstBufferConstView& buffer_view)>;

  /*!
   * \brief Interface of the callback function indicates that a connection to a SafeIpc server could be
   *        established successfully.
   * \details This callback is called as soon as the asynchronous connection request to the server
   *          could be performed successfully. This means a SafeIpc connection to a SafeIpc server is existing.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcClientConnectionEstablishment
   */
  using ConnectedCallback = vac::language::UniqueFunction<void()>;

  /*!
   * \brief Interface of the callback function indicates that an existing connection to a SafeIpc server was lost.
   * \details This callback is called as soon as the connection to a SafeIpc server is lost.  This means a SafeIpc
   *          connection to a SafeIpc server no longer exists.
   * \param[in] close_connection_cause A value reporting the reason of the disconnect.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcClientConnectionEstablishment
   */
  using DisconnectedCallback = vac::language::UniqueFunction<void(CloseConnectionCause close_connection_cause)>;

  /*!
   * \brief Creates an IpcClient.
   * \param[in] reactor The reactor must be valid as long as this IpcClient instance exists.
   * \param[in] timer_manager The timer manager must be valid as long as this IpcClient instance exists.
   * \param[in] connected_callback Callback to notify about a successful connection establishment
   *                               to a SafeIpc server. The IpcClient takes ownership of the passed callback.
   *                               It holds the ownership until the IpcClient is destroyed.
   * \param[in] disconnected_callback Callback to notify about the loss of an existing connection to a SafeIpc server.
   *                                  The IpcClient takes ownership of the passed callback.
   *                                  It holds the ownership until the IpcClient is destroyed.
   * \param[in] receive_message_callback Callback to notify about a successfully received message.
   *                                     The IpcClient takes ownership of the passed callback.
   *                                     It holds the ownership until the IpcClient is destroyed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  IpcClient(external::internal::osabstraction::Reactor1Interface& reactor,
            external::internal::osabstraction::TimerManagerInterface& timer_manager,
            ConnectedCallback connected_callback, DisconnectedCallback disconnected_callback,
            ReceiveMessageCallback receive_message_callback) noexcept;

  /*!
   * \brief Destroys the IpcClient and releases all system resources.
   * \details After a close call, there may still be ongoing asynchronous operations and timers. The caller must
   *          assert that all pending asynchronous operations are finished or that they are not executed concurrently.
   *          This can be checked using the IsInUse() API.
   *          One way to assert this is by moving the cleanup code into the reactor thread context.
   * \context ANY
   * \pre No ongoing asynchronous operation of reactor and connection.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~IpcClient() noexcept;

  IpcClient() = delete;
  IpcClient(IpcClient const&) = delete;
  IpcClient(IpcClient&&) = delete;
  auto operator=(IpcClient const&) -> IpcClient& = delete;
  auto operator=(IpcClient&&) -> IpcClient& = delete;

  /*!
   * \brief Start the asynchronous connection establishment to a SafeIpc server.
   * \details The connect process is performed asynchronously. The connected callback informs the user
   *          about a successfully completed connect process. The connected callback may never get called
   *          if the server declines all connection requests.
   * \context ANY
   * \pre No existing connection to a SafeIpc server.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcClientConnectionEstablishment
   */
  void Connect() noexcept;

  /*!
   * \brief Disconnect from a SafeIpc server and clean up of resources.
   * \details The user defined callback for new messages will not be called after a close.
   *          Sending is not allowed anymore.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcClientClose
   */
  void Close() noexcept;

  /*!
   * \brief Synchronously send a message to a SafeIpc server.
   * \param[in] buffer_view A view to the buffer holding the serialized message that should be transmitted,
   *                         must not exceed kMaximumMessageSize. The packet view must not be modified
   *                         during the function call.
   * \error IpcServiceDiscoveryCentralizedErrc::kSendBufferFull Message does not fit in the message buffer.
   * \error IpcServiceDiscoveryCentralizedErrc::kDisconnected The connection has been terminated by a SafeIpc server.
   * \context ANY
   * \pre Existing connection to a SafeIpc server. This can be checked using the IsConnected() API.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcClientSend
   */
  auto SendSync(ConstBufferConstView buffer_view) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Checks if the IpcClient still uses any provided callback after it was closed.
   * \details This function should be called after the IpcClient was closed if one wants to check if any
   *          provided callback is still executing. Note that this function always returns true if the
   *          IpcClient is still connected (the IpcClient was once connected but Close() has not been called yet).
   *          See ~IpcClient() why such a check may be necessary.
   * \return true if the IpcClient is connected or any provided callback is currently executing, false otherwise.
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \synchronous TRUE
   * \threadsafe TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcClientIsInUse
   */
  auto IsInUse() const noexcept -> bool;

  /*!
   * \brief Checks if the IpcClient is connected.
   * \return true if the IpcClient is connected, false otherwise.
   * \context ANY
   * \pre Unused function. Do not use this function unless covered by component tests!
   * \reentrant TRUE
   * \synchronous TRUE
   * \threadsafe TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-IpcClientIsConnected
   */
  auto IsConnected() const noexcept -> bool;

 private:
  /*! Internal states. */
  enum class State : std::uint8_t {
    kDisconnected, /*!< Disconnected. */
    kConnecting,   /*!< Start connecting. */
    kConnected,    /*!< Connected. */
    kConnectRetry  /*!< Connecting is not possible. Connection establishment retry mechanism is started. */
  };

  /*!
   * \brief Start the asynchronous connect process to a SafeIpc server.
   * \context ANY
   * \pre Call only from an exclusive area.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  void StartConnect() noexcept;

  /*!
   * \brief Synchronously transmit a message to a SafeIpc server.
   * \param[in] buffer_view A view to the message that should be transmitted must not
   *                        exceed kMaximumMessageSize. The packet view must not be
   *                        modified during the function call.
   * \error IpcServiceDiscoveryCentralizedErrc::kSendBufferFull Message does not fit in the message buffer.
   * \error IpcServiceDiscoveryCentralizedErrc::kDisconnected The connection has been terminated by a SafeIpc server.
   * \context ANY
   * \pre Call only from an exclusive area.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto TransmitMessage(ConstBufferConstView buffer_view) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Start the asynchronous reception of the next message.
   * \context ANY
   * \pre Must be called in state kConnected.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  void ReceiveAsync() noexcept;

  /*!
   * \brief Start the connection establishment retry mechanism and log connection error.
   * \details The mutex is not held during the setting of the re-connect Timer.
   * \context ANY
   * \pre Call only from an exclusive area.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void StartConnectRetry() noexcept;

  /*!
   * \brief Callback function handle for timer event.
   * \details If during the connection establishment an error occurs, this timer is used
   *          to retry to connect after kConnectionEstablishmentRetryTime.
   * \context REACTOR
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnTimerTimeout() noexcept;

  /*!
   * \brief Called upon a new SafeIpc message being available.
   * \details Prepares the memory for the message.
   * \param[in] message_size The number of bytes that are available from the SafeIpc connection.
   *                         The message_size must not exceed kMaximumMessageSize.
   * \return A view to the buffer where the message should be copied to.
   * \context REACTOR
   *
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto OnMessageAvailable(std::size_t message_size) noexcept -> MutableIOBufferView;

  /*!
   * \brief Called upon the reception process for a new SafeIpc message being complete. Calls the
   *        ReceiveMessageCallback if receiving the message has succeeded.
   * \details Passes the received message to the user callback and listens for the next incoming message.
   *          The mutex is not held during the ReceiveMessageCallback call.
   * \param[in] receive_complete_result A Result containing the actual number of bytes read into the provided buffer.
   * \context REACTOR
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnReceiveCompletion(::ara::core::Result<std::size_t> const& receive_complete_result) noexcept;

  /*!
   * \brief Called when connect to a SafeIpc server completed.
   * \details Prepare the message and forward it to the user callback.
   *          The mutex is not held during the ConnectedCallback call.
   * \param[in] connect_complete_result The connect complete result.
   * \context REACTOR
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnConnectCompleted(::ara::core::Result<void> const&& connect_complete_result) noexcept;

  /*!
   * \brief Handle and map any error that occurs during the usage of a SafeIpc connection.
   * \return Mapped centralized error.
   * \param[in] error_code The error code of the SafeIpc connection.
   * \context ANY
   * \pre Call only from an exclusive area.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto HandleAndMapConnectionError(::ara::core::ErrorCode const& error_code) noexcept -> CentralizedErrc;

  /*!
   * \brief Called when connection to SafeIpc server is lost.
   * \details Call the DisconnectedCallback.
   * \context REACTOR
   * \pre Call only from an exclusive area.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnConnectionLost() noexcept;

  /*!
   * \brief Called when a reactor software event was triggered.
   * \details This function is called from the registered reactor software event callback if the software event was
              triggered. The mutex is not held during the on disconnect callback call.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnReactorSoftwareEvent() noexcept;

  /*! Callback for established connection to server. */
  ConnectedCallback connected_callback_{};

  /*! Callback for established connection to server. */
  DisconnectedCallback disconnected_callback_{};

  /*! Callback for a received message. */
  ReceiveMessageCallback receive_message_callback_{};

  /*! Callback for a reactor software event. */
  ReactorSoftwareEvent::SoftwareEventCallback software_event_callback_{};

  /*! Reactor software event abstraction. */
  ReactorSoftwareEvent reactor_software_event_;

  /*! Memory buffer container for incoming IPC messages. */
  ReceiveBufferContainer receive_buffer_container_{};

  /*! Logger. */
  Logger logger_{Constants::kLoggerPrefixIpcClient};

  /*! IPC connection. */
  external::internal::osabstraction::Connection connection_;

  /*! Timer for connection establishment retry. */
  std::unique_ptr<external::internal::osabstraction::TimerInterface> timer_ptr_{};

  /*! Internal close connection cause for passing the cause into the reactor software event. */
  CloseConnectionCause close_connection_cause_{CloseConnectionCause::kDefault};

  /*! Mutex used to make state_ access exclusive. */
  mutable std::mutex mutex_{};

  /*! Internal state */
  State state_{State::kDisconnected};
};

/*!
 * \exclusivearea   ::amsr::ipc_service_discovery::internal::IpcClient::mutex_
 *                  Ensures consistency while reading/modifying the internal state.
 * \protects        ::amsr::ipc_service_discovery::internal::IpcClient::state_
 * \usedin
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::Connect
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::Close
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::SendSync
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::HandleTimer
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::OnReactorSoftwareEvent
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::OnMessageAvailable
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::OnReceiveCompletion
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::OnConnectCompleted
 *                  ::amsr::ipc_service_discovery::internal::IpcClient::OnConnectionLost
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

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_CLIENT_IPC_CLIENT_H_

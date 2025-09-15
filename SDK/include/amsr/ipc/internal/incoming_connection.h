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
/*!        \file
 *        \brief  Provides a class to manage an incoming client connection request for a SafeIPC server.
 *        \unit   osabstraction::ipc::IpcAcceptor_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_INCOMING_CONNECTION_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_INCOMING_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <utility>
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "amsr/ipc/internal/channel/stream_ipc_channel.h"
#include "amsr/ipc/internal/messages.h"
#include "amsr/ipc/internal/reactor_dispatcher_interface.h"
#include "amsr/ipc/ipc_types.h"
#include "amsr/timer/timer.h"
#include "amsr/timer/timer_manager.h"
#include "osabstraction/io/file_descriptor.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Forward declaration of ConnectionBacklog class.
 */
class ConnectionBacklog;

/*!
 * \brief   Manages an incoming client connection request for a SafeIPC server.
 */
class IncomingConnection final : private amsr::timer::Timer {
 public:
  /*!
   * \brief           Creates a new IncomingConnection object that handles the connection establishment with a client in
   *                  the background (in Reactor callbacks).
   *
   * \details         HandleTimerExpiry() of the passed timer manager has to be called from the same thread as
   *                  Dispatch() of the passed reactor dispatcher.
   *
   * \param[in]       new_connection         File descriptor of accepted Unix Domain socket connection. The ownership
   *                                         of the file descriptor is passed to this object.
   * \param[in]       s2c_buffer_size_hint   Limit (in bytes) on how large the server-to-client shared memory buffers
   *                                         can be as per client request. This value is honored only if it is above the
   *                                         lower server-to-client buffer size limit
   *                                         (amsr::ipc::internal::stream_ipc_protocol::kMinBufferSize). If it is below,
   *                                         that lower limit is used instead.
   * \param[in]       associated_backlog     Backlog of incoming client connection requests that manages this
   *                                         IncomingConnection. Successful and failed connection establishment is
   *                                         reported to this backlog.
   * \param[in]       reactor_dispatcher     ReactorDispatcher to use for Unix Domain socket event notification. Has to
   *                                         exist for the whole lifetime of this object.
   * \param[in]       timer_manager          TimerManager that shall handle connection establishment timeouts. Has to
   *                                         exist for the whole lifetime of this object.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit IncomingConnection(osabstraction::io::FileDescriptor&& new_connection, BufferSizeS2C s2c_buffer_size_hint,
                              ConnectionBacklog& associated_backlog, ReactorDispatcherInterface& reactor_dispatcher,
                              amsr::timer::TimerManager& timer_manager) noexcept;

  IncomingConnection(IncomingConnection const&) noexcept = delete;
  IncomingConnection& operator=(IncomingConnection const&) & noexcept = delete;
  IncomingConnection(IncomingConnection&&) noexcept = delete;
  IncomingConnection& operator=(IncomingConnection&&) & noexcept = delete;

  /*!
   * \brief           Aborts connection establishment with the client if is has not finished yet and releases all
   *                  allocated resources that were not extracted yet.
   *
   * \context         !HandleTimerExpiry() of timer_manager
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~IncomingConnection() noexcept final;

  /*!
   * \brief           Checks if the connection establishment has completed successfully.
   *
   * \return          true if the connection establishment has completed successfully, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsEstablished() const noexcept -> bool { return state_ == State::kConnectionEstablished; };

  /*!
   * \brief           Checks if the connection establishment was aborted.
   *
   * \return          true if the connection establishment was aborted, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto HasError() const noexcept -> bool { return state_ == State::kError; }

  /*!
   * \brief           Extracts the Unix Domain socket which was used to establish the connection.
   *
   * \return          The file descriptor of the Unix Domain socket.
   *
   * \context         ANY
   *
   * \pre             Connection establishment completed successfully.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto ExtractSocket() noexcept -> osabstraction::io::FileDescriptor { return std::move(socket_); }

  /*!
   * \brief           Extracts the IPC channel that was created during the connection establishment.
   *
   * \return          The created IPC channel.
   *
   * \context         ANY
   *
   * \pre             Connection establishment completed successfully.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto ExtractIpcChannel() noexcept -> StreamIpcChannel&& { return std::move(ipc_channel_); }

 private:
  /*!
   * \brief           Handles connection establishment timeout, called by the TimerManager.
   *
   * \return          True if new timer is started, otherwise false.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto HandleTimer() noexcept -> bool final;

  /*!
   * \brief           Handles Unix Domain socket Reactor events.
   *
   * \param[in]       events   Reactor events that occurred.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void OnReactorEvent(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief           Handles reception of ClientToServerMessage1.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             Object is in kWaitingForClientToServerMessage1 state.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleClientToServerMessage1() noexcept;

  /*!
   * \brief           Handles sending of ServerToClientMessage1.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             Object is in kSendingServerToClientMessage1 state.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleServerToClientMessage1() noexcept;

  /*!
   * \brief           Handles reception of ClientToServerMessage2.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             Object is in kWaitingForClientToServerMessage2 state.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleClientToServerMessage2() noexcept;

  /*!
   * \brief           Aborts the connection establishment and go into error state.
   *
   * \details         Unregisters socket from ReactorDispatcher and notifies associated backlog about the error.
   *
   * \context         Reactor::HandleEvents() thread
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void AbortConnectionEstablishment() noexcept;

  /*!
   * \brief           Sets new events for which the ReactorDispatcher should wait for on the Unix Domain socket.
   *
   * \param[in]       events   New events that the Reactor should wait for.
   *
   * \context         ANY
   *
   * \pre             The socket is currently registered to the ReactorDispatcher.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void SetMonitoredReactorEvents(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief           Unregisters the Unix Domain socket from the ReactorDispatcher.
   *
   * \details         Resets socket_reactor_id_.
   *
   * \context         ANY
   *
   * \pre             The socket is currently registered to the ReactorDispatcher.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void UnregisterSocketFromReactor() noexcept;

  /*!
   * \brief   State of the connection establishment.
   */
  enum class State : std::uint32_t {
    /*!
     * \brief   Waiting for reception of ClientToServerMessage1 sent by the client.
     */
    kWaitingForClientToServerMessage1,

    /*!
     * \brief  Sending ServerToClientMessage1 to the client.
     */
    kSendingServerToClientMessage1,

    /*!
     * \brief   Waiting for reception of ClientToServerMessage2 sent by the client.
     */
    kWaitingForClientToServerMessage2,

    /*!
     * \brief  Connection is successfully established.
     */
    kConnectionEstablished,

    /*!
     * \brief  An error occurred and the connection establishment was aborted.
     */
    kError
  };

  /*!
   * \brief   Tracks the state of the connection establishment.
   */
  State state_{State::kWaitingForClientToServerMessage1};

  /*!
   * \brief   Hint on how large the server-to-client shared memory buffer shall at most be (in bytes).
   */
  BufferSizeS2C s2c_buffer_size_hint_;

  /*!
   * \brief   Request of the client for the size of the server-to-client shared memory buffer (in bytes).
   *
   * \details This member is the buffer into which the received value will be written. Thus, the fixed-size type.
   */
  std::uint64_t s2c_buffer_size_request_{};

  /*!
   * \brief   Backlog of incoming client connection requests that manages this IncomingConnection.
   * \details Successful and failed connection establishment is reported to this backlog.
   */
  ConnectionBacklog& associated_backlog_;

  /*!
   * \brief   ReactorDispatcher to use for Unix Domain socket event notification.
   */
  ReactorDispatcherInterface& reactor_dispatcher_;

  /*!
   * \brief   File descriptor of accepted Unix Domain socket connection.
   */
  osabstraction::io::FileDescriptor socket_;

  /*!
   * \brief   ID ReactorDisptacher callback registration.
   * \details Optional is empty if no callback is currently registered to the ReactorDisptacher.
   */
  ::amsr::core::Optional<ReactorDispatcherId> socket_reactor_id_{};

  /*!
   * \brief   ID of the Client-to-server shared memory.
   */
  ShmId client_to_server_shm_id_{};

  /*!
   * \brief   ID of the Server-to-client shared memory.
   * \details The optional only contains a value if the Server-to-client shared memory file has been created but not
   *          unlinked yet.
   */
  ::amsr::core::Optional<ShmId> server_to_client_shm_id_{};

  /*!
   * \brief   ID of the notification shared memory.
   */
  ShmId notification_shm_id_{};

  /*!
   * \brief   The IPC channel between this process and the peer process that is created during the connection
   *          establishment.
   */
  StreamIpcChannel ipc_channel_{};

  /*!
   * \brief   IPC protocol version that was reported by the client.
   */
  IpcProtocolVersion ipc_protocol_{};

  /*!
   * \brief   Type for a ClientToServerMessage1 deserializer.
   */
  using C2SMessage1 = ClientToServerMessage1<osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief   Type for a ServerToClientMessage1 serializer.
   */
  using S2CMessage1 = ServerToClientMessage1<osabstraction::io::ConstIOBuffer>;

  /*!
   * \brief   Type for a ClientToServerMessage2 deserializer.
   */
  using C2SMessage2 = ClientToServerMessage2<osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief   Storage for all possible message (de)serializers.
   */
  ::amsr::core::Variant<::amsr::core::monostate, C2SMessage1, S2CMessage1, C2SMessage2> messages_{};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_INCOMING_CONNECTION_H_

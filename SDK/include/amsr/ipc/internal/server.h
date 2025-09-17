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
 *        \brief  Provides Server class that accepts and establishes connections to multiple SafeIPC clients in
 *                parallel.
 *        \unit   osabstraction::ipc::IpcAcceptor_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_SERVER_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/ipc/connection.h"
#include "amsr/ipc/internal/connection_backlog.h"
#include "amsr/ipc/internal/reactor_dispatcher_interface.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/timer/timer_manager.h"
#include "osabstraction/io/file_descriptor.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Internal SafeIPC server class.
 *
 * \details Accepts and establishes connections to multiple SafeIPC clients parallel in the background. Incoming
 *          connections are added to an internal backlog.
 */
class Server final {
 public:
  /*!
   * \brief   Maximum number of ReactorDispatcher callback slots used by a Server object.
   */
  static constexpr std::size_t kMaxUsedReactorCallbackSlots{
      ConnectionBacklog::kMaxUsedReactorCallbackSlots +
      2};  // +2 for server socket reactor callback and connection establishment error software event

  /*!
   * \brief   ReactorDispatcher software event that shall be triggered when a connection establishment succeeded or when
   *          the Server encountered an unrecoverable error.
   */
  struct ServerSoftwareEvent {
    /*!
     * \brief   ReactorDispatcher software event.
     */
    ReactorDispatcherId value;
  };

  /*!
   * \brief           Creates a new SafeIPC server.
   *
   * \details         The server is not started yet. HandleTimerExpiry() of the passed timer manager has to be called
   *                  from the same thread as Dispatch() of the passed reactor dispatcher.
   *
   * \param[in]       reactor_dispatcher                    ReactorDispatcher to use for Unix Domain socket event
   *                                                        and software event notifications.
   * \param[in]       server_address                        SafeIPC server address of the server.
   * \param[in]       timer_manager                         TimerManager that shall handle connection establishment
   *                                                        timeouts.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit Server(ReactorDispatcherInterface& reactor_dispatcher, UnicastAddress server_address,
                  amsr::timer::TimerManager& timer_manager) noexcept
      : reactor_dispatcher_{reactor_dispatcher}, server_address_{server_address}, timer_manager_{timer_manager} {}

  Server(Server const&) noexcept = delete;
  Server& operator=(Server const&) & noexcept = delete;
  Server(Server&&) noexcept = delete;
  Server& operator=(Server&&) & noexcept = delete;

  /*!
   * \brief           Destroys the SafeIPC server.
   *
   * \context         ANY
   *
   * \pre             The server was stopped before with Stop() if it was started with Start().
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~Server() noexcept = default;

  /*!
   * \brief           Starts the server.
   *
   * \details         Clients will now be able to connect to it.
   *
   * \param[in]       s2c_buffer_size_hint      Hint (in bytes) on how large the server-to-client shared memory buffers
   *                                            shall at most be. The actual size is controlled by the connecting
   *                                            client. If a client requests a buffer size above this hint, the buffer
   *                                            size will be the value supplied as hint instead.
   *                                            This value is honored if it is above the lower server-to-client buffer
   *                                            size limit (amsr::ipc::internal::stream_ipc_protocol::kMinBufferSize).
   *                                            If it is below, that lower limit is used instead.
   * \param[in]       server_software_event     ReactorDispatcher software event that shall be triggered when either
   *                                            a connection was successfully established with a client or the server
   *                                            encountered an unrecoverable error. Note that multiple connection
   *                                            established and/or server error notifications might be coalesced into
   *                                            one software event if they occurred in the same HandleEvents() cycle.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create server.
   * \error           osabstraction::OsabErrc::kAddressNotAvailable     The address is not available or is currently in
   *                                                                    use.
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources to create server.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The file system in which the server shall be
   *                                                                    created does not support a required operation or
   *                                                                    a file system I/O error occurred.
   *
   * \context         ANY
   *
   * \pre             The server is not already running.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto Start(BufferSizeS2C s2c_buffer_size_hint, ServerSoftwareEvent server_software_event) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief           Stops the server and releases all allocated system resources.
   *
   * \details         Clients will not be able to connect anymore. All connections in the connection backlog will be
   *                  closed.
   *
   * \context         !HandleTimerExpiry() of timer_manager
   *
   * \pre             The server is currently running.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void Stop() noexcept;

  /*!
   * \brief           Gets the current server status.
   *
   * \return          Empty value if the server did not encounter an unrecoverable error, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kUnexpected   Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kResource     Not enough system resources to accept next connection.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto GetServerStatus() const noexcept -> ::amsr::core::Result<void> { return server_status_; }

  /*!
   * \brief           Checks if the server has any successfully established connection in his backlog.
   *
   * \return          true if the server has any successfully established connection in his backlog, false otherwise.
   *
   * \context         ANY
   *
   * \pre             The server is currently running.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto HasEstablishedConnection() const noexcept -> bool;

  /*!
   * \brief           Initializes SafeIPC Connection from a completely established incoming connection.
   *
   * \param[out]      next_connection   Connection to initialize.
   *
   * \error           osabstraction::OsabErrc::kResource      Insufficient resources to register next_connection to it's
   *                                                          reactor.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             The server is currently running and has a successfully established connection in his backlog.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto InitNextConnection(Connection& next_connection) noexcept -> ::amsr::core::Result<void>;

 private:
  /*!
   * \brief           Handle Reactor events for the Unix Domain socket server.
   *
   * \param[in]       events   Reactor events that occurred.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             The server is currently running and no unrecoverable error has occurred.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void OnServerSocketEvent(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief           Handles connection establishment error software event, triggered by the connection backlog.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             The server is currently running.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void OnConnectionEstablishmentError() noexcept;

  /*!
   * \brief           Registers the server socket to the ReactorDispatcher for read events.
   *
   * \param[in]       socket_fd   Server socket.
   *
   * \error           osabstraction::OsabErrc::kResource      Insufficient resources to register the server socket to
   *                                                          the ReactorDispatcher.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto RegisterServerSocketToReactor(osabstraction::io::NativeHandle socket_fd) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Enables read events for the server socket in the ReactorDispatcher.
   *
   * \context         ANY
   *
   * \pre             The server socket is registered to the ReactorDispatcher.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void EnableServerSocketReactorEvents() noexcept;

  /*!
   * \brief           Disables read events for the server socket in the ReactorDispatcher.
   *
   * \context         ANY
   *
   * \pre             The server socket is registered to the ReactorDispatcher.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void DisableServerSocketReactorEvents() noexcept;

  /*!
   * \brief           Unregisters the server socket from the ReactorDispatcher.
   *
   * \context         ANY
   *
   * \pre             The server socket is registered to the ReactorDispatcher.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void UnregisterServerSocketFromReactor() noexcept;

  /*!
   * \brief           Registers connection establishment error software event to the ReactorDispatcher.
   *
   * \error           osabstraction::OsabErrc::kResource      Insufficient resources to register the software event to
   *                                                          the ReactorDispatcher.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto RegisterConnEstabErrorEventToReactor() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Unregisters the connection establishment error software event from the ReactorDispatcher.
   *
   * \context         ANY
   *
   * \pre             The connection establishment error software event is registered to the ReactorDispatcher.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void UnregisterConnEstabErrorEventFromReactor() noexcept;

  /*!
   * \brief           Triggers the server software event.
   *
   * \context         ANY
   *
   * \pre             The server is currently running.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   */
  void TriggerServerSoftwareEvent() noexcept;

  /*!
   * \brief   ReactorDispatcher to use for Unix Domain socket event and software event notifications.
   */
  ReactorDispatcherInterface& reactor_dispatcher_;

  /*!
   * \brief   SafeIPC server address of the server.
   */
  UnicastAddress server_address_;

  /*!
   * \brief    TimerManager that shall handle connection establishment timeouts.
   */
  amsr::timer::TimerManager& timer_manager_;

  /*!
   * \brief    ReactorDispatcher software event that shall be triggered when either a connection was successfully
   *           established with a client or the server encountered an unrecoverable error.
   */
  ServerSoftwareEvent server_software_event_{};

  /*!
   * \brief    ReactorDispatcher software event that will be triggered by the connection backlog on connection
   *           establishment errors.
   */
  ConnectionBacklog::ConnectionEstablishmentErrorEvent connection_establishment_error_event_{};

  /*!
   * \brief    Limit on how large the server-to-client shared memory buffer can be (in bytes).
   */
  BufferSizeS2C s2c_buffer_size_hint_{0};

  /*!
   * \brief    Unix Domain socket server file descriptor.
   */
  osabstraction::io::FileDescriptor server_socket_{};

  /*!
   * \brief    ReactorDispatcher registration of the server socket.
   */
  ReactorDispatcherId server_socket_reactor_id_{};

  /*!
   * \brief    Backlog of incoming client connection requests.
   */
  ::amsr::core::Optional<ConnectionBacklog> backlog_{};

  /*!
   * \brief    Current status of the server.
   * \details  Contains an empty value if the server did not encounter an unrecoverable error, an error otherwise.
   */
  ::amsr::core::Result<void> server_status_{};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_SERVER_H_

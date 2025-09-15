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
 *        \brief  Provides a class to manage multiple incoming client connection requests for a SafeIPC server in
 *                parallel.
 *        \unit   osabstraction::ipc::IpcAcceptor_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONNECTION_BACKLOG_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONNECTION_BACKLOG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/array.h"
#include "amsr/core/optional.h"
#include "amsr/ipc/internal/incoming_connection.h"
#include "amsr/ipc/internal/reactor_dispatcher_interface.h"
#include "amsr/timer/timer_manager.h"
#include "osabstraction/io/file_descriptor.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Manages multiple incoming client connection requests for a SafeIPC server in parallel.
 */
class ConnectionBacklog final {
 public:
  /*!
   * \brief   Number of slots that the ConnectionBacklog should have.
   * \details This limits how many connections can be established in parallel.
   */
  static constexpr std::size_t kBacklogSize{32};

  /*!
   * \brief   Maximum number of ReactorDispatcher callback slots that the ConnectionBacklog needs.
   */
  static constexpr std::size_t kMaxUsedReactorCallbackSlots{kBacklogSize};

  /*!
   * \brief   ReactorDispatcher software event that shall be triggered when a connection is established successfully.
   */
  struct ConnectionEstablishedEvent {
    /*!
     * \brief   ReactorDispatcher software event.
     */
    ReactorDispatcherId value;
  };

  /*!
   * \brief   ReactorDispatcher software event that shall be triggered when a connection establishment failed.
   */
  struct ConnectionEstablishmentErrorEvent {
    /*!
     * \brief   ReactorDispatcher software event.
     */
    ReactorDispatcherId value;
  };

  /*!
   * \brief           Creates a new ConnectionBacklog object that manage multiple incoming client connection requests in
   *                  parallel in the background (in Reactor callbacks).
   *
   * \param[in]       reactor_dispatcher                    ReactorDispatcher to use for Unix Domain socket event
   *                                                        notifications. Has to exist for the whole lifetime of this
   *                                                        object.
   * \param[in]       timer_manager                         TimerManager that shall handle connection establishment
   *                                                        timeouts. Has to exist for the whole lifetime of this
   *                                                        object.
   * \param[in]       s2c_buffer_size_hint                  Hint (in bytes) on how large the server-to-client shared
   *                                                        memory buffers shall be as per client request. This value is
   *                                                        honored only if it is above the lower limit for it
   *                                                        (amsr::ipc::internal::stream_ipc_protocol::kMinBufferSize),
   *                                                        otherwise that lower limit is used instead.
   * \param[in]       connection_established_event          ReactorDispatcher software event that shall be triggered
   *                                                        when a connection was successfully established with a
   *                                                        client. Note that the software event callback might not be
   *                                                        called for every established connection if multiple
   *                                                        connections were established in the same HandleEvents()
   *                                                        cycle.
   * \param[in]       connection_establishment_error_event  ReactorDispatcher software event that shall be triggered
   *                                                        when a connection establishment with a client was aborted
   *                                                        because of an error. Note that the software event callback
   *                                                        might not be called for every connection establishment error
   *                                                        if multiple errors happened in the same HandleEvents()
   *                                                        cycle.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit ConnectionBacklog(ReactorDispatcherInterface& reactor_dispatcher, amsr::timer::TimerManager& timer_manager,
                             BufferSizeS2C s2c_buffer_size_hint,
                             ConnectionEstablishedEvent connection_established_event,
                             ConnectionEstablishmentErrorEvent connection_establishment_error_event) noexcept
      : reactor_dispatcher_{reactor_dispatcher},
        timer_manager_{timer_manager},
        s2c_buffer_size_hint_{s2c_buffer_size_hint},
        connection_established_event_{connection_established_event},
        connection_establishment_error_event_{connection_establishment_error_event} {}

  /*!
   * \brief           Destroys the ConnectionBacklog and aborts the connection establishment of all incoming
   *                  connection requests.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ~ConnectionBacklog() noexcept = default;

  ConnectionBacklog(ConnectionBacklog const&) noexcept = delete;
  ConnectionBacklog& operator=(ConnectionBacklog const&) & noexcept = delete;
  ConnectionBacklog(ConnectionBacklog&&) noexcept = delete;
  ConnectionBacklog& operator=(ConnectionBacklog&&) & noexcept = delete;

  /*!
   * \brief           Checks if all slots of the connection backlog are used.
   *
   * \return          true if all slots of the connection backlog are used, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto IsFull() const noexcept -> bool { return free_backlog_slots_ == 0; }

  /*!
   * \brief           Adds a new accepted Unix Domain socket connection to the backlog and starts establishing the
   *                  connection to this client.
   *
   * \param[in]       incoming_connection   File descriptor of the Unix domain socket connection to the client.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             The connection backlog is not full.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void AddIncomingConnection(osabstraction::io::FileDescriptor&& incoming_connection) noexcept;

  /*!
   * \brief           Checks if there is at least one successfully established connection in the backlog.
   *
   * \return          true if there is at least one successfully established connection in the backlog, false otherwise.
   *
   * \context         ANY
   *
   * \pre             The connection backlog is not full.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto HasEstablishedConnection() const noexcept -> bool { return established_connections_count_ != 0; }

  /*!
   * \brief           Gets a reference to the next successfully established connection.
   *
   * \details         RemoveLastConnection() should be called after all resources of the returned connection were
   *                  extracted.
   *
   * \return          Reference to the next successfully established connection.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             There is at least one successfully established connection in the backlog.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto GetNextConnection() noexcept -> IncomingConnection&;

  /*!
   * \brief           Destroys the last connection returned with GetNextConnection().
   *
   * \details         Should be called after all resources of the connection returned by GetNextConnection() were
   *                  extracted to free the backlog slot.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             GetNextConnection() was called before this RemoveLastConnection() call and the after the last
   *                  RemoveLastConnection() call.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void RemoveLastConnection() noexcept;

  /*!
   * \brief           Destroys all incoming connections where an error occurred and frees their backlog slots.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleConnectionEstablishmentErrors() noexcept;

  /*!
   * \brief           Triggers the connection established software event notification.
   *
   * \details         This function should be called by IncomingConnections that have successfully finished connection
   *                  establishment.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   */
  void ReportConnectionEstablishmentComplete() noexcept;

  /*!
   * \brief           Triggers the connection establishment error software event notification.
   *
   * \details         This function should be called by IncomingConnections that have encountered an error during
   *                  connection establishment.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   */
  void ReportConnectionEstablishmentError() noexcept;

 private:
  /*!
   * \brief           Triggers a ReactorDispatcher software event.
   *
   * \param[in]       id   ID of the ReactorDispatcher software event to trigger.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void TriggerReactorSoftwareEvent(ReactorDispatcherId id) noexcept;

  /*!
   * \brief   ReactorDispatcher to use for Unix Domain sockets event notification.
   */
  ReactorDispatcherInterface& reactor_dispatcher_;

  /*!
   * \brief   TimerManager that shall handle connection establishment timeouts.
   */
  amsr::timer::TimerManager& timer_manager_;

  /*!
   * \brief   Hint on how large the server-to-client shared memory buffer shall be (in bytes).
   */
  BufferSizeS2C s2c_buffer_size_hint_;

  /*!
   * \brief   ReactorDispatcher software event that shall be triggered when a connection was successfully established
   *          with a client.
   */
  ConnectionEstablishedEvent connection_established_event_;

  /*!
   * \brief   ReactorDispatcher software event that shall be triggered when a connection establishment with a client was
   *          aborted because of an error
   */
  ConnectionEstablishmentErrorEvent connection_establishment_error_event_;

  /*!
   * \brief   Type for a connection backlog slot.
   */
  using OptionalIncomingConnection = ::amsr::core::Optional<IncomingConnection>;

  /*!
   * \brief   Type for the connection backlog.
   */
  using Backlog = ::amsr::core::Array<OptionalIncomingConnection, kBacklogSize>;

  /*!
   * \brief   Connection backlog.
   */
  Backlog backlog_{};

  /*!
   * \brief   Number of currently unused backlog slots.
   */
  std::size_t free_backlog_slots_{kBacklogSize};

  /*!
   * \brief   Number of currently successfully established connections in the backlog.
   */
  std::size_t established_connections_count_{0};

  /*!
   * \brief   Last returned connection with GetNextConnection().
   */
  Backlog::iterator last_returned_connection_{};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONNECTION_BACKLOG_H_

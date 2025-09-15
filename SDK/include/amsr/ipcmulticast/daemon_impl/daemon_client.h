/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  Safe IPC DaemonClient offering a high-level API to send/receive IPC multicast datagrams.
 *
 *      \details  The client receives or sends the message from or to a specific server. After opening the connection,
 *                the client accept messages to send. If the connection is not established already or in error state the
 *                messages are stores, as long as the buffer can hold them. On connection then the messages are sent.
 *                On error messages are discarded.
 *
 *         \unit  amsr::ipcmulticast::ClientDaemonImpl::DaemonClient
 *
 *********************************************************************************************************************/

#ifndef LIB_IPCMULTICAST_DAEMON_IMPL_INCLUDE_AMSR_IPCMULTICAST_DAEMON_IMPL_DAEMON_CLIENT_H_
#define LIB_IPCMULTICAST_DAEMON_IMPL_INCLUDE_AMSR_IPCMULTICAST_DAEMON_IMPL_DAEMON_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/connection.h"
#include "amsr/ipc/ipc_types.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/ipcmulticast/daemon_impl/internal/packet_queue.h"
#include "amsr/ipcmulticast/types.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/log/logging.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace ipcmulticast {
namespace daemon_impl {

/*! UnicastAddress alias */
using UnicastAddress = amsr::ipc::UnicastAddress;

/*!
 * \brief Implementation of the deamon multicast client for Safe IPC.
 *
 * \trace DSGN-IpcMulticast-DaemonBasedMulticast
 */
class DaemonClient final {
 public:
  /*!
   * \brief       Constructs a new multicast client.
   *
   * \param[in]   reactor             The reactor. It must be valid as long as this Client instance exists.
   * \param[in]   timer_manager       The timer manager. It must be valid as long as this Client instance
   *                                  exists.
   * \param[in]   multicast_address   Unused multicast address.
   * \param[in]   unicast_address     The unicast address.
   * \param[in]   callback            The callback function. It muss be valid as long as this DaemonClient instance
   *                                  exists.
   *
   * \context     Init
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \vprivate    Vector product internal API
   */
  DaemonClient(osabstraction::io::reactor1::Reactor1Interface& reactor,
               amsr::steady_timer::TimerManagerInterface& timer_manager, MulticastAddress const& multicast_address,
               UnicastAddress const& unicast_address, MessageHandleCallback&& callback) noexcept;

  /*!
   * \brief       Constructs a new daemon multicast client with default unicast address.
   *
   * \param[in]   reactor             The reactor. It muss be valid as long as this DaemonClient instance exists.
   * \param[in]   timer_manager       The timer manager. It muss be valid as long as this DaemonClient instance
   *                                  exists.
   * \param[in]   multicast_address   Unused multicast address.
   * \param[in]   callback            The callback function. It muss be valid as long as this DaemonClient instance
   *                                  exists.
   *
   * \context     Init
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \vprivate    Vector product internal API
   */
  DaemonClient(osabstraction::io::reactor1::Reactor1Interface& reactor,
               amsr::steady_timer::TimerManagerInterface& timer_manager, MulticastAddress const& multicast_address,
               MessageHandleCallback&& callback) noexcept;

  /*!
   * \brief       Destroys object and releases all system resources.
   *
   * \details     After a close call there may still be ongoing asynchronous operations and timers. The caller must
   *              assert that all pending asynchronous operation finished or that they are not executed concurrently.
   *              One way to assert this is by moving the cleanup code into the reactor thread context.
   *
   * \context     ANY
   *
   * \pre         The connection is closed and no asynchronous operation is currently executing.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \trace       DSGN-IpcMulticast-DaemonBased-Client-Destruction
   */

  ~DaemonClient() noexcept;
  DaemonClient() = delete;
  DaemonClient(DaemonClient const&) = delete;
  DaemonClient(DaemonClient&&) = delete;
  auto operator=(DaemonClient const&) & -> DaemonClient& = delete;
  auto operator=(DaemonClient&&) & -> DaemonClient& = delete;

  /*!
   * \brief       Connects to the SafeIPC multicast server.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   *
   * \vprivate    Vector product internal API
   *
   * \trace       DSGN-IpcMulticast-DaemonBased-Client-ConnectionEstablishment
   * \spec
   *   requires true;
   * \endspec
   */
  void Connect() noexcept;

  /*!
   * \brief       Disconnects from the SafeIPC multicast server and cleans up all the used resources.
   *
   * \details     The user-defined callback for new messages will not be called, after a close. Sending is not allowed
   *              anymore.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   *
   * \vprivate    Vector product internal API
   *
   * \trace       DSGN-IpcMulticast-DaemonBased-Client-Close
   * \spec
   *   requires true;
   * \endspec
   */
  void Close() noexcept;

  /*!
   * \brief       Send an IPC multicast message.
   *
   * \details     If there is no connection the packet can be scheduled and send later.
   *
   * \param[in]   packet_view   A view to the packet that should be transmitted, must not
   *                            exceed kMaximumPacketsize. The packet view must not be modified during the function
   *                            call.
   *
   * \error       MulticastErrc::kInvalidApiState   The call is not allowed when not connected.
   * \error       IpcMulticastErrc::kInvalidArgument Provided packet_view exceeds kMaximumPacketSize
   *
   * \context     ANY
   *
   * \pre         Connection has to been opened via Connect()
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   *
   * \vprivate    Vector product internal API
   *
   * \trace       DSGN-IpcMulticast-DaemonBased-Client-Send
   */
  auto SendPacket(PacketView const& packet_view) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief       Start asynchronous receptions.
   *
   * \details     On reception the user defined callback is called. A new reception is start until the connection is
   *              closed.
   *
   * \error       MulticastErrc::kInvalidApiState   The call is not allowed when not connected.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous FALSE
   * \threadsafe  TRUE
   *
   * \vprivate    Vector product internal API
   *
   * \trace       DSGN-IpcMulticast-DaemonBased-Client-Receive
   * \spec
   *   requires true;
   * \endspec
   */
  auto ReceiveAsync() noexcept -> amsr::core::Result<void>;

 private:
  /*! Internal type for constant IO buffer container. */
  using ConstIOBufferContainer = std::array<osabstraction::io::ConstIOBuffer, 1U>;
  /*! Internal type for constant IO buffer view. */
  using ConstIOBufferView = amsr::core::Span<osabstraction::io::ConstIOBuffer>;
  /*! Internal type for mutable IO buffer container. */
  using MutableIOBufferContainer = std::array<osabstraction::io::MutableIOBuffer, 1U>;

  /*! Connection state */
  enum class ConnectionState : std::uint8_t {
    kClosed, /*!< Closed. */
    kOpen    /*!< Open, sending and receiving is permitted. */
  };

  /*! Internal connection state. */
  enum class InternalConnectionState : std::uint8_t {
    kDisconnected,     /*!< Disconnected. */
    kConnecting,       /*!< Starts the connection. */
    kConnected,        /*!< Connected. */
    kConnectionError,  /*!< An error appears during connection. */
    kConnectionTimeout /*!< Retry time reached. */
  };

  /*! Receiving state. */
  enum class ReceivingState : std::uint8_t {
    kIdle,     /*!< Idle, no waiting for receiving. */
    kReceiving /*!< Waiting for receiving. */
  };

  /*!
   * \brief       Implementation of Connect.
   *
   * \context     ANY
   *
   * \pre         Only calling from an exclusive area.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  void ConnectInternal() noexcept;

  /*!
   * \brief       Implementation of ReciveAsync.
   *
   * \context     ANY
   *
   * \pre         Only calling from an exclusive area.
   *
   * \reentrant   FALSE
   * \synchronous FALSE
   * \threadsafe  FALSE
   */
  void ReceiveAsyncInternal() noexcept;

  /*!
   * \brief       Transmit a packet to the multicast server.
   *
   * \param       packet_view   The packet view must not be modified during the function call.
   *
   * \return      A result, holding a potential error.
   *
   * \error       MulticastErrc::kSendError   If there was another error during sending.
   *
   * \context     ANY
   *
   * \pre         Only calling from an exclusive area.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto TransmitPacket(PacketView const& packet_view) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief       Called on message available.
   *
   * \details     Prepares the memory for the message.
   *
   * \param[in]   length   The length.
   *
   * \return      The memory.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto OnMessageAvailable(std::size_t length) noexcept -> amsr::core::Span<osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief       Called on receive completion.
   *
   * \details     Prepare the message and forward it to the user callback.
   *
   * \param[in]   receive_complete_result   The receive complete result.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   */
  void OnReceiveCompletion(amsr::core::Result<std::size_t>&& receive_complete_result) noexcept;

  /*!
   * \brief       Processes the received IPC multicast datagram.
   *
   * \details     Called by the ReceiveCompletionCallback to notify of the completion of a message reception.
   *
   * \param[in]   received_length   The length of the received message.
   *
   * \context     Reactor
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   */
  void OnMessageReceived(std::size_t received_length) noexcept;

  /*!
   * \brief       Called to control the timer.
   *
   * \details     Starts the timer on error state or stop it otherwise.
   *
   * \param[in]   callback_handle   The callback handle.
   *
   * \context     Reactor
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  void OnSoftwareEvent(osabstraction::io::reactor1::CallbackHandle callback_handle) const noexcept;

  /*!
   * \brief       Called when connect completed.
   *
   * \details     When the connection is established, stored messages are sent and a receive requests are executed.
   *
   * \param[in]   connect_complete_result   The connect complete result.
   *
   * \context     Reactor
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   */
  void OnConnectCompleted(amsr::core::Result<void> const&& connect_complete_result) noexcept;

  /*!
   * \brief       Callback function handle for timer event.
   *
   * \details     When during the connection establishment an error appears this timer is used to retrying to connect
   *              after kRetryTime.
   *
   * \return      false
   *
   * \context     Reactor
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   */
  auto HandleTimer() noexcept -> void;

  /*!
   * \brief       Enters the ConnectionError state.
   *
   * \param[in]   error   The error code.
   *
   * \context     ANY
   *
   * \pre         Is only called within an exclusive area.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  void OnError(amsr::core::ErrorCode const& error) noexcept;

  /*! A memory buffer for incoming IPC messages. */
  PacketBuffer receive_buffer_{};

  /*! A container of mutable I/O buffer for the receive memory buffer. */
  MutableIOBufferContainer receive_buffer_iovec_container_{};

  /*! Retry time out. */
  static constexpr std::chrono::nanoseconds kRetryTime{std::chrono::milliseconds{100}};

  /*! Maximum buffer size in bytes for outgoing packets. Must not be smaller than kMaximumPacketSize. */
  static constexpr std::size_t kPacketBufferSize{102400UL};

  /*! Configuration of the PacketQueue. */
  static constexpr internal::PacketQueueConfig kPacketQueueConfiguration{kPacketBufferSize,
                                                                         amsr::ipc::kMessageProtocolOverhead};

  /*! The queue of IPC packets which are pending for transmission. */
  internal::PacketQueue outgoing_messages_{kPacketQueueConfiguration};  // VCA_IPCMULTICAST_CALL_EXTERNAL_FUNCTION

  /*! Reference to the reactor. */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*! The IPC connection. */
  amsr::ipc::Connection connection_;

  /*! The connection state. */
  ConnectionState connection_state_{ConnectionState::kClosed};

  /*! The internal connection state. */
  InternalConnectionState internal_connection_state_{InternalConnectionState::kDisconnected};

  /*! The receiving state. */
  ReceivingState receiving_state_{ReceivingState::kIdle};

  /*! The unicast address. */
  UnicastAddress const address_;

  /*! A mutex used to make connection_state_, internal_connection_state_ and receiving_state_ access exclusive. */
  std::mutex mutex_{};

  /*! User defined callback function for message handling. */
  MessageHandleCallback callback_;

  /*! Reactor software event callback handle. */
  osabstraction::io::reactor1::CallbackHandle software_event_reactor_handle_{
      osabstraction::io::reactor1::kInvalidCallbackHandle};

  /*! The logger. */
  ara::log::Logger& logger_;

  /*! Timer for reconnect handling. */
  std::unique_ptr<amsr::steady_timer::TimerInterface> const timer_{};
};

}  // namespace daemon_impl
}  // namespace ipcmulticast
}  // namespace amsr

#endif  // LIB_IPCMULTICAST_DAEMON_IMPL_INCLUDE_AMSR_IPCMULTICAST_DAEMON_IMPL_DAEMON_CLIENT_H_

/*!
 * \exclusivearea ::amsr::ipcmulticast::daemon_impl::DaemonClient::mutex_
 * Used for protecting the state machines states from changes by concurrent threads.
 *
 * \protects ::amsr::ipcmulticast::daemon_impl::DaemonClient::connection_state_
 * ::amsr::ipcmulticast::daemon_impl::DaemonClient::internal_connection_state_
 * ::amsr::ipcmulticast::daemon_impl::DaemonClient::receiving_state_
 * \usedin ::amsr::ipcmulticast::daemon_impl::DaemonClient::Connect
 * \usedin ::amsr::ipcmulticast::daemon_impl::DaemonClient::Close
 * \usedin ::amsr::ipcmulticast::daemon_impl::DaemonClient::SendPacket
 * \usedin ::amsr::ipcmulticast::daemon_impl::DaemonClient::ReceiveAsync
 * \usedin ::amsr::ipcmulticast::daemon_impl::DaemonClient::OnReciveCompletion
 * \usedin ::amsr::ipcmulticast::daemon_impl::DaemonClient::HandleTimer
 * \usedin ::amsr::ipcmulticast::daemon_impl::DaemonClient::OnConnectCompleted
 * \length SHORT Handle connection callback or call and do next action in state machine.
 * \endexclusivearea
 */

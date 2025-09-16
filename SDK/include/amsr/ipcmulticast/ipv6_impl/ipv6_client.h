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
 *        \brief  An IPC Service Discovery Ipv6Client abstracts away IPC multicast communication and offers
 *                a high-level API to send/receive IPC multicast datagrams.
 *
 *      \details  On Linux, BasicIPC uses node-local IPv6 multicast communication on the loopback device.
 *                Therefore, the following steps are required in order to be able to use it:
 *                  1. Enable multicast on the loopback device which is disabled by default
 *                     sudo ip link set lo multicast on
 *                  2. Add a routing table entry for IPv6 node-local multicast addresses
 *                     sudo ip route add ff01::0/16 dev lo
 *
 *         \unit  amsr::ipcmulticast::ClientIpv6Impl::Ipv6Client
 *
 *********************************************************************************************************************/

#ifndef LIB_IPCMULTICAST_IPV6_IMPL_INCLUDE_AMSR_IPCMULTICAST_IPV6_IMPL_IPV6_CLIENT_H_
#define LIB_IPCMULTICAST_IPV6_IMPL_INCLUDE_AMSR_IPCMULTICAST_IPV6_IMPL_IPV6_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/ipcmulticast/types.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/log/logger.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/ipc1/multicast_receiver.h"
#include "osabstraction/io/ipc1/multicast_sender.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace ipcmulticast {
namespace ipv6_impl {

/*!
 * \brief Implementation of the ipv6 multicast client for Safe IPC.
 *
 * \trace DSGN-IpcMulticast-IPv6BasedMulticast
 */
class Ipv6Client final {
 public:
  /*! The receive buffer type for incoming IPC multicast messages. */
  using ReceiveBuffer = std::array<std::uint8_t, kMaximumPacketSize>;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCMULTICAST_AutosarC++17_10-M7.1.2_MutuableInterface
  /*!
   * \brief       Constructs a new instance.
   *
   * \param[in]   reactor             The reactor. It muss be valid as long as this Ipv6Client instance exists.
   * \param[in]   timer_manager       Unused reference of a ThreadSafeTimerManager. Needs for compatibility with the
   *                                  DeamonClient.
   * \param[in]   multicast_address   The IPC multicast address on which the endpoint shall listen for incoming messages
   *                                  and to which it shall send outgoing messages.
   * \param[in]   callback            The callback function. It muss be valid as long as this Ipv6Client instance
   *                                  exists.
   *
   * \context     Init
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  Ipv6Client(osabstraction::io::reactor1::Reactor1Interface& reactor,
             amsr::steady_timer::TimerManagerInterface& timer_manager, MulticastAddress const& multicast_address,
             MessageHandleCallback&& callback) noexcept;

  /*!
   * \brief       Destroys object and releases all system resources.
   *
   * \details     After a close call there may still be ongoing asynchronous operations. The caller must assert that all
   *              pending asynchronous operation finished or that they are not executed concurrently. One way to assert
   *              this is by moving the cleanup code into the reactor thread context.
   *
   * \context     ANY
   *
   * \pre         The connection is closed and no asynchronous operation is currently executing.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \trace       DSGN-IpcMulticast-Ipv6Based-Client-Destruction
   */
  ~Ipv6Client() noexcept;
  Ipv6Client() = delete;
  Ipv6Client(Ipv6Client const&) = delete;
  Ipv6Client(Ipv6Client&&) = delete;
  auto operator=(Ipv6Client const&) & -> Ipv6Client& = delete;
  auto operator=(Ipv6Client&&) & -> Ipv6Client& = delete;

  /*!
   * \brief       Connects the endpoint to the multicast address.
   *
   * \context     ANY
   *
   * \pre         Not in MessageHandleCallback.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   *
   * \vprivate    Vector product internal API
   *
   * \trace       DSGN-IpcMulticast-Ipv6Based-ConnectionEstablishment
   */
  void Connect() noexcept;

  /*!
   * \brief       Disconnects from the Endpoint connection and cleans up all the used resources.
   *
   * \details     The user-defined callback for new messages will not be called, after a close. Sending is not allowed
   *              anymore.
   *
   * \context     ANY
   *
   * \pre         -
   *
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   *
   * \vprivate    Vector product internal API
   *
   * \trace       DSGN-IpcMulticast-Ipv6Based-Close
   */
  void Close() noexcept;

  /*!
   * \brief       Schedules the transmission of an IPC multicast message.
   *
   * \param[in]   packet_view   A view to the packet buffer. A maximum view size of kMaximumPacketSize bytes is allowed.
   *
   * \error       IpcMulticastErrc::kInvalidApiState   The call is not allowed when not connected.
   * \error       IpcMulticastErrc::kInvalidArgument   Packet exceeds kMaximumPacketSize.
   *
   * \context     ANY (but the calling thread must be synchronized with Reactor thread)
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous FALSE
   * \threadsafe  TRUE
   *
   * \trace       DSGN-IpcMulticast-Ipv6Based-Send
   */
  auto SendPacket(PacketView const& packet_view) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief       Triggers an asynchronous reception.
   *
   * \details     The reception is called when dynamic action of the IPC binding started, and also on successful
   *              reception of every message
   *
   * \error       IpcMulticastErrc::kInvalidApiState   The call is not allowed when not connected or already listening.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous FALSE
   * \threadsafe  TRUE
   *
   * \trace       DSGN-IpcMulticast-Ipv6Based-Receive
   */
  auto ReceiveAsync() noexcept -> amsr::core::Result<void>;

 private:
  /*! Const io buffer alias */
  using ConstIOBufferContainer = std::array<osabstraction::io::ConstIOBuffer, 1>;

  /*! Mutable io buffer alias */
  using MutableIOBufferContainer = std::array<osabstraction::io::MutableIOBuffer, 1>;

  /*!
   * \brief Connection state.
   */
  enum class ConnectionState : std::uint8_t {
    kDisconnected = 0, /*!< Connection disconnected */
    kConnected         /*!< Connection connected */
  };

  /*!
   * \brief Type definition for transmission state.
   */
  enum class TransmissionState : std::uint8_t {
    kIdle = 0,         /*!< Transmission is idle. */
    kTransmitting = 1, /*!< Transmission is in use. */
  };

  /*!
   * \brief Type definition for receiving state.
   */
  enum class ReceivingState : std::uint8_t {
    kIdle = 0,      /*!< Receiving is idle. */
    kListening = 1, /*!< Receiving is listening. */
  };

  /*!
   * \brief       Sends the buffered IPC packet over the IPC multicast channel.
   *
   * \context     ANY
   *
   * \pre         Only calling from an exclusive area.
   *
   * \reentrant   FALSE
   * \synchronous FALSE
   * \threadsafe  FALSE
   */
  void TransmitPacketInternal() noexcept;

  /*!
   * \brief       Cleans up the used resources after transmission is successful
   *
   * \details     Called by the SendCompletionCallback to notify about the completion of a message transmission.
   *
   * \param       send_completion_result   The send completion result.
   *
   * \context     Reactor
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   */
  void OnSendCompleted(amsr::core::Result<void> const& send_completion_result) noexcept;

  /*!
   * \brief       Called on faulty send.
   *
   * \details     Called by the IPC system to notify about an error condition occurred during a message reception.
   *              amsr::core::Abort is called if an error has occurred during access of a system resource required for
   *              IPC communication.
   *
   * \param[in]   error_code  Indicates the error while transmission.
   *
   * \context     Reactor
   *
   * \pre         Only calling from an exclusive area.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  void SendError(amsr::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief       Triggers an asynchronous reception.
   *
   * \context     ANY
   *
   * \pre         Only calling from an exclusive area.
   *
   * \reentrant   FALSE
   * \synchronous FALSE
   * \threadsafe  FALSE
   */
  void ReceiveAsyncInternal();

  /*!
   * \brief       Disconnects this endpoint completely.
   *
   * \details     Called by the ReceiveCompletionCallback to notify about an error condition occurred during a message
   *              reception. amsr::core::Abort is called if an error has occurred during access of a system resource
   *              required for IPC communication.
   *
   * \param[in]   error_code   Indicates the error while reception.
   *
   * \context     Reactor
   *
   * \pre         Only calling from an exclusive area.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  void ReceiveError(amsr::core::ErrorCode const& error_code) const noexcept;

  /*!
   * \brief       Called when message received.
   *
   * \details     Called by the ReceiveCompletionCallback to notify of the completion of a message reception.
   *
   * \param[in]   receive_complete_result   The receive complete result.
   *
   * \context     Reactor
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   */
  void OnMessageReceived(amsr::core::Result<std::size_t>&& receive_complete_result) noexcept;

  /*! The callback function that process the message. */
  MessageHandleCallback callback_;

  /*! A memory buffer for incoming IPC multicast messages. */
  ReceiveBuffer receive_buffer_{};

  /*! A container of MutableIOBuffer for the receive memory buffer. */
  MutableIOBufferContainer receive_buffer_iovec_container_{
      {osabstraction::io::MutableIOBuffer{receive_buffer_.data(), receive_buffer_.size()}}};

  /*! The queue of IPC messages which are pending for transmission. */
  std::queue<PacketBuffer> outgoing_messages_{};

  /*! A memory buffer which is currently being transmitted. */
  PacketBuffer transmit_buffer_{};

  /*! A container of ConstIOBuffer for the transmission memory buffer. */
  ConstIOBufferContainer transmit_buffer_iovec_container_{{osabstraction::io::ConstIOBuffer{}}};

  /*! The IPC multicast receiver. */
  osabstraction::io::ipc1::MulticastReceiver multicast_receiver_;

  /*! The IPC multicast sender. */
  osabstraction::io::ipc1::MulticastSender multicast_sender_;

  /*!
   * \brief The current connection state of this endpoint.
   *
   * \details Once endpoint is created, state is kDisconnected.
   *          Directly after connecting to sender and receiver, state becomes kConnected.
   *          If connection did not succeed, or disconnection occurs, state turns back to kDisconnected.
   */
  ConnectionState connection_state_{ConnectionState::kDisconnected};

  /*! Indicates whether a message transmission is active. */
  TransmissionState transmitting_state_{TransmissionState::kIdle};

  /*! Indicates whether the client is listening or not. */
  ReceivingState receiving_state_{ReceivingState::kIdle};

  /*! A mutex used to make connection_state_ and transmitting_state_ access exclusive. */
  std::mutex mutex_{};

  /*! The logger. */
  ara::log::Logger& logger_;
};

}  // namespace ipv6_impl
}  // namespace ipcmulticast
}  // namespace amsr

#endif  // LIB_IPCMULTICAST_IPV6_IMPL_INCLUDE_AMSR_IPCMULTICAST_IPV6_IMPL_IPV6_CLIENT_H_

/*!
 * \exclusivearea ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::mutex_
 * Used for protecting the state machines states from changes by concurrent threads.
 *
 * \protects ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::connection_state_
 *           ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::transmitting_state_
 *
 * \usedin   ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::Connect
 *           ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::Close
 *           ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::SendPacket
 *           ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::ReceiveAsync
 *           ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::OnSendCompleted
 *           ::amsr::ipcmulticast::ipv6_impl::Ipv6Client::OnMessageReceived
 *
 * \length   SHORT Handle connection callback or call and do next action in state machine.
 * \endexclusivearea
 */

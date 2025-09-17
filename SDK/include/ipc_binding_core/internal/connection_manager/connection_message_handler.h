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
/**        \file
 *        \brief  Handler for sending and receiving packets via a SafeIpc connection.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MESSAGE_HANDLER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MESSAGE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <deque>
#include <mutex>
#include <utility>

#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "ara/core/memory_resource.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/language/unique_function.h"

#include "ipc_binding_core/internal/connection_manager/connection_message_handler_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_connection.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Thread-Safe Handler for transmission and reception of packets via SafeIpc connection.
 * \details Send Algorithm:
 *          - Every transmitted packet is first enqueued into the send queue.
 *          - If no transmission is currently ongoing (state 'Idle') the next enqueued packet is dequeued and
 *            transmitted via the SafeIpc connection.
 *            - The state will be set to 'Sending' before any packet is passed to SafeIpc.
 *            - If the transmission immediately finished, the state is directly set back to 'Idle'.
 *            - If asynchronous processing of the packet is necessary the SendCompletionCallback triggered by the
 *              SafeIpc connection will set the state back to 'Idle'.
 *          - In case more packets are still pending in the send queue a processing of these enqueued via the reactor
 *            thread is scheduled. The application thread which triggered the send operation shall in this case not get
 *            the extra task to send all already enqueued packets. This task is off-loaded to the reactor thread.
 *            - This scheduled reactor task will send all pending packets remaining in the send queue.
 *            - All enqueued packets are also sent when the SafeIpc connection notifies us of the completion of an
 *              asynchronous send operation, as this also happens always in the reactor context.
 *          - Any error reported by the SafeIpc connection will immediately force a state transition to 'Error' closing
 *            of the SafeIpc connection. This state will not be left anymore and prevents any further packet
 *            transmission.
 *
 *          Receive Algorithm:
 *          - With start of message reception processing an asynchronous receive handler is registered with the SafeIpc
 *            connection.
 *          - SafeIpc will notify the handler about the general availability of a new message. In context of this
 *            'MessageAvailableCallback' an empty memory buffer with the size of the available message is dynamically
 *            allocated and returned to the SafeIpc connection.
 *          - SafeIpc will copy the available message into the provided receive buffer and notify the connection handler
 *            again with the 'ReceiveCompletionCallback'.
 *          - After notification of the user about the received message the next asynchronous receive handler is
 *            registered with the SafeIpc connection.
 *          - Any error reported by the SafeIpc connection will lead to the immediate closing of the SafeIpc connection.
 *            This state will not be left anymore and prevents any further packet reception.
 *
 * \unit       IpcBinding::IpcBindingCore::ConnectionMessageHandler
 * \complexity The high number of inter-module calls predominantly comprises calls to libvac's smart pointers and
 *             libosabstraction's send operation. The elevated weighted complexity and nesting level are primarily
 *             attributed to the interaction with libosabstraction in the SafeIPCSend function. Such complexity is
 *             inherent in the nature of the send process.
 */
class ConnectionMessageHandler : public ConnectionMessageHandlerInterface {
 public:
  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used logger.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief Callback that notifies the user about an error occurred during transmission of packets.
   *
   * \param[in] error_code  The detailed error code provided by SafeIpc.
   * \param[in] location    The log location where the error originates from.
   *
   * \pre         -
   * \context     App|Reactor
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  using OnErrorCallback = vac::language::UniqueFunction<void(::amsr::core::ErrorCode const& error_code,
                                                             Logger::LogLocation const& location)>;

  /*!
   * \brief Callback that notifies the a new message was received.
   *
   * \param[in] packet  Buffer of the received packet.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  using OnMessageReceivedCallback = vac::language::UniqueFunction<void(IpcPacketShared&& packet)>;

  /*!
   * \brief Construct a ConnectionMessageHandler.
   *
   * \param[in,out] reactor                    Used Reactor.
   * \param[in,out] logger                     Logger.
   * \param[in,out] connection                 SafeIpc connection to be handled by the connection handler.
   * \param[in,out] on_error_callback          Callback function triggered in case of any error scenario. The handler
   *                                           takes ownership of the passed callback. It holds the ownership until the
   *                                           SafeIpc connection (\p connection) is closed.
   * \param[in,out] message_received_callback  Callback function triggered in case of a received packet.The handler
   *                                           takes ownership of the passed callback. It holds the ownership until the
   *                                           SafeIpc connection (\p connection) is closed.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionMessageHandler(Logger& logger, ReactorInterface& reactor, GenericConnection& connection,
                           OnErrorCallback&& on_error_callback,
                           OnMessageReceivedCallback&& message_received_callback) noexcept;

  /*!
   * \brief Destroy the ConnectionMessageHandler.
   *
   * \pre         It must be ensured that not called from a callback function provided on this connection.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ConnectionMessageHandler() noexcept override;

  ConnectionMessageHandler(ConnectionMessageHandler const&) noexcept = delete;
  auto operator=(ConnectionMessageHandler const&) noexcept -> ConnectionMessageHandler& = delete;
  ConnectionMessageHandler(ConnectionMessageHandler const&&) noexcept = delete;
  auto operator=(ConnectionMessageHandler const&&) noexcept -> ConnectionMessageHandler& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandlerInterface::Reset
   *
   * \spec requires true; \endspec
   */
  auto Reset() noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandlerInterface::Send
   *
   * \spec requires true; \endspec
   */
  auto Send(IpcPacketShared&& packet) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandlerInterface::AddToSendQueue
   *
   * \spec requires true; \endspec
   */
  auto AddToSendQueue(IpcPacketShared&& packet) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandlerInterface::SendQueued
   *
   * \spec requires true; \endspec
   */
  auto SendQueued() noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandlerInterface::StartReception
   *
   * \spec requires true; \endspec
   */
  auto StartReception() noexcept -> void final;

 protected:
  /*!
   * \brief   Send the next enqueued packet.
   * \details Function marked as protected due to testing purpose.
   *
   * \return True if more packets are still pending in the send queue. Otherwise false.
   *
   * \pre         -
   * \context     App|Reactor
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE (TRUE in case immediate transmission possible via SafeIpc connection).
   * \steady      TRUE
   */
  auto SendNextQueued() noexcept -> bool;

 private:
  /*!
   * \brief Type alias for Reactor callback handle.
   */
  using ReactorCallbackHandle = osabstraction::io::reactor1::CallbackHandle;

  /*!
   * \brief Type alias for Reactor event types.
   */
  using ReactorEventTypes = ::osabstraction::io::reactor1::EventTypes;

  /*!
   * \brief Sending / Transmission states.
   */
  enum class SendState : std::uint8_t {
    kIdle = 0,    /*!< No transmission currently ongoing. */
    kSending = 1, /*!< A send operation is currently ongoing. */
    kError = 2    /*!< Any error occurred. No further send operations possible. */
  };

  /*!
   * \brief Type of an immutable buffer view.
   */
  using SafeIpcImmutableBufferView = osabstraction::io::ConstIOBuffer;

  /*!
   * \brief Type of container with a single immutable packet view (I/O vector).
   */
  using SingleImmutableBufferViewContainer = ::amsr::core::Array<SafeIpcImmutableBufferView, 1>;

  /*!
   * \brief Type of an mutable buffer view.
   */
  using SafeIpcMutableBufferView = osabstraction::io::MutableIOBuffer;

  /*!
   * \brief Type of container with a single mutable buffer view (I/O vector).
   */
  using SafeIpcMutableBufferViewContainer = amsr::core::Span<SafeIpcMutableBufferView>;

  /*!
   * \brief Type of container with a single mutable packet view (I/O vector).
   */
  using SingleMutableBufferViewContainer = std::array<SafeIpcMutableBufferView, 1>;

  /*!
   * \brief Type alias for SafeIpc SendResult.
   */
  using SafeIpcSendResult = amsr::ipc::SendResult;

  /*!
   * \brief Type alias for SendResult.
   */
  using SendResult = ::amsr::core::Result<amsr::ipc::SendResult>;

  /*!
   * \brief Type of the send queue.
   */
  using SendQueue = std::deque<IpcPacketShared, ara::core::PolymorphicAllocator<IpcPacketShared>>;

  /*!
   * \brief   Send a single packet via the SafeIpc connection.
   * \details Transmission might happen immediately or asynchronous.
   *
   * \param[in] packet  Packet to be sent.
   *
   * \return SafeIpc send result indicating if immediate or asynchronous is used.
   *
   * \error osabstraction::OsabErrc::kUninitialized  Tried to send via not connected connection.
   * \error osabstraction::OsabErrc::kBusy           An asynchronous send operation is currently in progress.
   * \error osabstraction::OsabErrc::kSize           The size of the passed message exceeds the maximum supported
   *                                                 message size.
   * \error osabstraction::OsabErrc::kDisconnected   The connection has been terminated by the peer. SupportData of
   *                                                 error is set to amsr::ipc::kOrderlyDisconnect or
   *                                                 amsr::ipc::kAbnormalDisconnect.
   * \error osabstraction::OsabErrc::kResource       Not enough system resources to send memory exchange handle to peer.
   * \error osabstraction::OsabErrc::kProtocolError  Failed to send/receive notification to/from IPC peer or SafeIPC
   *                                                 protocol violation detected. The connection is broken and should be
   *                                                 closed.
   *
   * \pre         No transmission must be ongoing on the SafeIpc connection.
   * \context     App|Reactor
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE (TRUE in case immediate transmission possible via SafeIpc connection).
   * \steady      TRUE
   */
  auto SendSafeIpc(IpcPacketShared&& packet) noexcept -> SendResult;

  /*!
   * \brief Handle successful asynchronous send completion.
   *
   * \pre         An asynchronous send operation must be ongoing.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto OnSendCompleted() noexcept -> void;

  /*!
   * \brief Allocates the required memory to store the new incoming SafeIpc message.
   *
   * \param[in] message_length  The length of the available message.
   *
   * \return A view container to the memory buffer where the incoming message can be stored.
   *
   * \pre         An asynchronous receive operation must be ongoing.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto OnReceiveMessageAvailable(std::size_t message_length) noexcept -> SafeIpcMutableBufferViewContainer;

  /*!
   * \brief Reception of a new message completed.
   *
   * \param[in] result  Receive completion result of the SafeIpc connection.
   *
   * \pre         An asynchronous receive operation must be ongoing.
   * \pre         A memory buffer of the received must be allocated (OnReceiveMessageAvailable).
   * \pre         The actual received message size (\p result) must match with the allocated receive buffer.
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto OnReceiveMessageCompleted(::amsr::core::Result<std::size_t>&& result) noexcept -> void;

  /*!
   * \brief Handling potential errors reports by the SafeIpc connection during message send operation.
   *
   * \param[in] error     The occurred error code.
   * \param[in] location  Exact code location where the error occurred.
   *
   * \pre         -
   * \context     App|Reactor
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto HandleSafeIpcSendError(amsr::core::ErrorCode const& error, Logger::LogLocation const& location) noexcept -> void;

  /*!
   * \brief Handling potential errors reports by the SafeIpc connection during message receive operation.
   *
   * \param[in] error     The occurred error code.
   * \param[in] location  Exact code location where the error occurred.
   *
   * \pre         -
   * \context     App|Reactor
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto HandleSafeIpcReceiveError(amsr::core::ErrorCode const& error, Logger::LogLocation const& location) noexcept
      -> void;

  /*!
   * \brief Reference to the used logger.
   */
  Logger& logger_;

  /*!
   * \brief Reference to the reactor.
   */
  ReactorInterface& reactor_;

  /*!
   * \brief Handle of Reactor software event for processing of queued packets.
   */
  ReactorCallbackHandle reactor_handle_process_queued_packets_{};

  /*!
   * \brief Reference to the used SafeIpc connection.
   */
  GenericConnection& connection_;

  /*!
   * \brief Registered user callback triggered in case any error occurred.
   */
  OnErrorCallback on_error_callback_;

  // ---- Attributes Related To Message Send Handling ----

  /*!
   * \brief Current send/transmission state.
   */
  SendState send_state_{SendState::kIdle};

  /*!
   * \brief The queue of IPC messages which are pending for transmission.
   */
  // VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
  SendQueue send_queue_{ara::core::PolymorphicAllocator<IpcPacketShared>{ara::core::GetDefaultResource()}};

  /*!
   * \brief Mutex to protect the send_state_.
   */
  std::mutex send_states_lock_{};

  /*!
   * \brief   SharedPointer copy of the actual packet currently sending.
   * \details This SharedPointer copy must not be modified during an ongoing send operation. A view to this packet
   *          buffer is stored in single_packet_send_view_.
   */
  IpcPacketShared send_packet_{};

  /*!
   * \brief   Container (I/O vector) with a single buffer view provided to SafeIpc for packet transmission.
   * \details This contents of this buffer view must be kept valid until the SafeIpc send operation completed
   *          (synchronous or asynchronous). During ongoing send operation this view is pointing to the send_packet_
   *          buffer.
   */
  SingleImmutableBufferViewContainer send_single_packet_view_{};

  // ---- Attributes Related To Message Receive Handling ----

  /*!
   * \brief Registered user callback triggered in case a new received packet.
   */
  OnMessageReceivedCallback message_received_callback_;

  /*!
   * \brief A memory buffer for incoming SafeIpc packets.
   */
  IpcPacketShared receive_packet_{};

  /*!
   * \brief A container of I/O vectors for the receive memory buffer.
   */
  SingleMutableBufferViewContainer receive_single_packet_view_{};
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MESSAGE_HANDLER_H_

// clang-format off
/*!
 * \exclusivearea amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::send_states_lock_
 *                Used for protecting the send / transmissions state and queue.
 *
 * \protects amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::send_state_
 * \protects amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::send_queue_
 *
 * \usedin amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::Reset
 * \usedin amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::Send
 * \usedin amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::AddToSendQueue
 * \usedin amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::SendNextQueued
 * \usedin amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::OnSendCompleted
 * \usedin amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::HandleSafeIpcSendError
 * \usedin amsr::ipc_binding_core::internal::connection_manager::ConnectionMessageHandler::HandleSafeIpcReceiveError
 * \exclude -
 * \length SHORT Modification of numeric send state, push / pop from send queue (shared-ptr only).
 * \endexclusivearea
 */
// clang-format on

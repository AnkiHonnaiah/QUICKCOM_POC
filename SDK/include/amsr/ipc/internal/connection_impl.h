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
 *        \brief  Implementation of SafeIPC Connection class.
 *        \unit   osabstraction::ipc::IpcConnection_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONNECTION_IMPL_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONNECTION_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <utility>
#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/variant.h"
#include "amsr/ipc/credentials.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/ipc/internal/channel/stream_ipc_channel.h"
#include "amsr/ipc/internal/file_descriptor_queue.h"
#include "amsr/ipc/internal/incoming_connection.h"
#include "amsr/ipc/internal/message_reader.h"
#include "amsr/ipc/internal/message_writer.h"
#include "amsr/ipc/internal/messages.h"
#include "amsr/ipc/internal/stream_ipc_protocol_common.h"
#include "amsr/ipc/internal/stream_ipc_protocol_format_b.h"
#include "amsr/ipc/ipc_types.h"
#include "amsr/ipc/unicast_address.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/osab_error_domain.h"
#include "osabstraction/process/process_types.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipc {
namespace internal {

//  VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_ConnectionImpl
/*!
 * \brief   Implementation of SafeIPC Connection class.
 *
 * \details This class contains the actual implementation of the SafeIPC Connection. It is expected that the Connection
 *          contains a shared pointer to an ConnectionImpl object and forwards API calls to this ConnectionImpl object.
 */
class ConnectionImpl final : public std::enable_shared_from_this<ConnectionImpl> {
 public:
  /*!
   * \brief           Constructs a ConnectionImpl object.
   *
   * \details         The Connection is initially closed.
   *
   * \param[in]       reactor   Reactor interface for asynchronous operations. The reactor has to exist for the whole
   *                            lifetime of this Connection object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcClient,
   *                  DSGN-Osab-SafeIpcReactor
   */
  // VECTOR Next Construct VectorC++-V12.1.1: MD_OSA_V12.1.1_BaseClassInitInCtor_NoExplicitInit
  explicit ConnectionImpl(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept : reactor_{reactor} {}

  ConnectionImpl(ConnectionImpl const&) noexcept = delete;
  ConnectionImpl& operator=(ConnectionImpl const&) noexcept = delete;
  ConnectionImpl(ConnectionImpl&& other) noexcept = delete;
  ConnectionImpl& operator=(ConnectionImpl&&) noexcept = delete;

  /*!
   * \brief           Destroys the ConnectionImpl object.
   *
   * \details         This does not close the connection or frees system resources, this is done by Close().
   *                  The user has to avoid destructing the ConnectionImpl object while any user-provided callback, such
   *                  as the ConnectionCompletionCallback or other registered callbacks, is executing. This can be done
   *                  by using a separate software event reactor job, or by ensuring per design that ConnectionImpl is
   *                  destroyed in the same thread where Reactor::HandleEvents() is called.
   *
   * \context         ANY
   *
   * \pre             The ConnectionImpl object is in closed state and no Reactor callback does still use this object.
   *                  Should not be executed in the context of the user callback.
   *                  Reactor of this object should not execute this internal reactor callback while the destructor is
   *                  running. Usually this means that HandleEvents() of the reactor should not be running during the
   *                  execution of the destructor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcLifetimeManagement
   */
  ~ConnectionImpl() noexcept = default;

  /*!
   * \brief     Connects the ConnectionImpl object asynchronously to a remote address.
   *
   * \details
   * The operation is performed asynchronously. The completion callback informs about the completed connect process.
   * The completion callback may never get called if the server does not continue the connection establishment
   * protocol. If this error case is critical for the application, a timer should be used and the ConnectAsync()
   * operation aborted after timeout with a call to Close().
   *
   * \param[in] remote_address                Unicast address of the remote server to connect to.
   * \param[in] callback                      Callback to notify about the completion of the connect operation. The
   *                                          ConnectionImpl object takes ownership of the passed callback. It holds the
   *                                          ownership until the callback has been executed or until the ConnectionImpl
   *                                          object is closed.
   * \param[in] c2s_buffer_size_hint          Hint on how large the shared buffer used for client-to-server IPC messages
   *                                          shall be (in bytes). Consider that the amount of IPC data that can be
   *                                          stored in this buffer will be smaller than this size due to SafeIPC
   *                                          protocol overhead for each IPC message that is stored in the buffer (see
   *                                          amsr::ipc::kMessageProtocolOverhead and
   *                                          amsr::ipc::kMessageProtocolOverheadMemoryHandleExchange).
   *                                          The SafeIPC implementation does not honor this value if it is below the
   *                                          lower buffer size limit (see OsAbstraction Technical Reference).
   * \param[in] s2c_buffer_size_hint          Hint on how large the shared buffer used for server-to-client IPC messages
   *                                          shall be (in bytes). Consider that the amount of IPC data that can be
   *                                          stored in this buffer will be smaller than this size due to SafeIPC
   *                                          protocol overhead for each IPC message that is stored in the buffer (see
   *                                          amsr::ipc::kMessageProtocolOverhead and
   *                                          amsr::ipc::kMessageProtocolOverheadMemoryHandleExchange).
   *                                          The SafeIPC implementation does not honor this value if it is below the
   *                                          lower buffer size limit (see OsAbstraction Technical Reference) or above
   *                                          the maximum size hint set for the acceptor (see
   *                                          amsr::ipc::Acceptor::Init()). The actual resulting size of the
   *                                          server-to-client buffer can be queried using
   *                                          amsr::ipc::ConnectionImpl::GetReceiveBufferSize() after the connection has
   *                                          been established.
   *
   * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no
   *                                                                     category.
   * \error           osabstraction::OsabErrc::kAlreadyConnected         Tried to connect already connected connection.
   * \error           osabstraction::OsabErrc::kAddressNotAvailable      The server is not available.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges   Not allowed to connect to server. This can also
   *                                                                     happen if the server is not completely
   *                                                                     initialized yet.
   * \error           osabstraction::OsabErrc::kDisconnected             The peer aborted the connection establishment
   *                                                                     or is not available anymore.
   *                                                                     SupportData of error is not set.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError   The system environment is in an unexpected
   *                                                                     state. This might be caused by:
   *                                                                      - A file system I/O error.
   *                                                                      - A system service is not available.
   * \error           osabstraction::OsabErrc::kResource                 Not enough system resources to perform connect
   *                                                                     operation.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcClient,
   *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentClient,
   *                  DSGN-Osab-SafeIpcBufferSizeConfiguration,
   *                  DSGN-Osab-SafeIpcConnectionIsolation
   */
  auto ConnectAsync(UnicastAddress remote_address, ConnectCompletionCallback&& callback,
                    BufferSizeC2S c2s_buffer_size_hint, BufferSizeS2C s2c_buffer_size_hint) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief     Check the size of the next pending message to be received.
   *
   * \details
   * This function can be called before ReceiveSync() if you want to know the size of the next pending message so that
   * you can provide an appropriately sized buffer to ReceiveSync(). It is not mandatory to call this function before
   * ReceiveSync() and it does not make sense to do so if you 1) know the maximum size of the IPC messages that will be
   * received or 2) pass a buffer to ReceiveSync() that is as large as the receive buffer of the ConnectionImpl object.
   *
   * This function only checks the SafeIPC header of the next pending message which means that a following ReceiveSync()
   * may still return OsabErrc::kBusy after this function reported the size of the next message if the peer already sent
   * the SafeIPC header of the next message but not all data of the next message.
   *
   * \return          The size of the next pending message.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Connection is not connected.
   * \error           osabstraction::OsabErrc::kBusy            The receive buffer is empty or an asynchronous receive
   *                                                            operation is currently in progress.
   * \error           osabstraction::OsabErrc::kDisconnected    The connection has been terminated by the peer and no
   *                                                            more incoming messages are in the receive buffer.
   *                                                            SupportData of error is set to
   *                                                            amsr::ipc::kOrderlyDisconnect or
   *                                                            amsr::ipc::kAbnormalDisconnect.
   * \error           osabstraction::OsabErrc::kProtocolError   Failed to send/receive notification to/from IPC peer or
   *                                                            SafeIPC protocol violation detected. The connection is
   *                                                            broken and should be closed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcReceiveSync
   */
  auto GetPendingMsgInfo() noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief     Tries to receive a message synchronously.
   *
   * \details
   * The message is received either as a whole (when a value or the OsabErrc::kTruncation error is returned) or not at
   * all (when an error other than OsabErrc::kTruncation is returned).
   *
   * If an IPC message with an attached memory exchange handle is received with this function then the attached memory
   * exchange handle is discarded by SafeIPC and only the IPC data is received.
   *
   * Note that HandleEvents() of the Reactor of this ConnectionImpl object has to be called for peer disconnects to be
   * discovered. If HandleEvents() is never called (e.g. because only synchronous APIs are used), ReceiveSync() will
   * never return osabstraction::OsabErrc::kDisconnected even if the peer has closed the ConnectionImpl object or has
   * terminated. If only synchronous APIs are used but disconnects shall still be discovered, HandleEvents() of the
   * Reactor could be called in a background thread, for example.
   *
   * \param[in,out]   buffer     View on an IO buffer array that points to the memory segments where the received
   *                             message data shall be copied to. The IO buffer array that this span points to and
   *                             the memory segments that each IO buffer points to have to stay valid during this
   *                             function call.
   *
   * \return          The size of the received message.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to receive from not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            No whole IPC message is available in the receive buffer
   *                                                            or an asynchronous receive operation is currently in
   *                                                            progress.
   * \error           osabstraction::OsabErrc::kSize            The size of the next incoming message exceeds the size
   *                                                            of the receive buffer and can therefore never be
   *                                                            received synchronously.
   * \error           osabstraction::OsabErrc::kTruncation      The provided buffer was too small for the IPC message.
   *                                                            The buffer was filled with the beginning of the message
   *                                                            and the rest of the message was discarded.
   * \error           osabstraction::OsabErrc::kDisconnected    The connection has been terminated by the peer and no
   *                                                            more incoming messages are in the receive buffer.
   *                                                            SupportData of error is set to
   *                                                            amsr::ipc::kOrderlyDisconnect or
   *                                                            amsr::ipc::kAbnormalDisconnect.
   * \error           osabstraction::OsabErrc::kProtocolError   Failed to send/receive notification to/from IPC peer or
   *                                                            SafeIPC protocol violation detected. The connection is
   *                                                            broken and should be closed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcReceiveSync
   *
   */
  auto ReceiveSync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> buffer) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief     Triggers the reception of the next incoming message.
   *
   * \details
   * The message will be received asynchronously.
   *
   * If an IPC message with an attached memory exchange handle is received with this function then the attached memory
   * exchange handle is discarded by SafeIPC and only the IPC data is received.
   *
   * \param[in]       msg_available_callback       Callback that is called as soon as a message is received. The
   *                                               callback must return a view to the buffer where the message content
   *                                               should be copied to. The ConnectionImpl object takes ownership of the
   *                                               passed callback. It holds the ownership until the callback has been
   *                                               executed or until the ConnectionImpl object is closed.
   * \param[in]       receive_completion_callback  Callback to notify about the result of the reception.
   *                                               The ConnectionImpl object takes ownership of the passed callback. It
   *                                               holds the ownership until the callback has been executed or until the
   *                                               ConnectionImpl object is closed.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to receive from not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            An asynchronous receive operation is currently in
   *                                                            progress.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcReceiveAsync
   */
  auto ReceiveAsync(MessageAvailableCallback&& msg_available_callback,
                    ReceiveCompletionCallback&& receive_completion_callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief     Triggers the reception of the next incoming message.
   *
   * \details
   * The message will be received asynchronously.
   *
   * If an IPC message with an attached memory exchange handle is received with this function then the attached memory
   * exchange handle is returned in the called receive completion callback. Note that a message may be received without
   * an attached memory exchange handle even if the peer successfully sent a message with an attached memory exchange
   * handle, e.g. because of system resource limits.
   *
   * \param[in]       msg_available_callback       Callback that is called as soon as a message is received. The
   *                                               callback must return a view to the buffer where the message content
   *                                               should be copied to. The ConnectionImpl object takes ownership of the
   *                                               passed callback. It holds the ownership until the callback has been
   *                                               executed or until the ConnectionImpl object is closed.
   * \param[in]       receive_completion_callback  Callback to notify about the result of the reception.
   *                                               The ConnectionImpl object takes ownership of the passed callback. It
   *                                               holds the ownership until the callback has been executed or until the
   *                                               ConnectionImpl object is closed.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to receive from not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            An asynchronous receive operation is currently in
   *                                                            progress.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcReceiveAsync,
   *                  DSGN-Osab-SafeIpcLinuxTransferOfMemoryExchangeHandles
   */
  auto ReceiveAsync(MessageAvailableCallback&& msg_available_callback,
                    ReceiveCompletionExtendedCallback&& receive_completion_callback) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief     Triggers the chunkwise reception of the next incoming message.
   *
   * \details
   * The message will be received in chunks asynchronously.
   *
   * If an IPC message with an attached memory exchange handle is received with this function then the attached memory
   * exchange handle is discarded by SafeIPC and only the IPC data is received.
   *
   * \param[in]       msg_available_callback       Callback that is called as soon as a message is received. The
   *                                               callback must return a view to the buffer where the first message
   *                                               chunk content should be copied to.
   *                                               The ConnectionImpl takes ownership of the passed callback.
   *                                               It holds the ownership until the msg_chunk_received_callback has been
   *                                               executed or until the ConnectionImpl is closed.
   * \param[in]       msg_chunk_received_callback  Callback to notify about the result of every chunk reception.
   *                                               Returns the view to the buffer where the next message chunk
   *                                               content should be copied to.
   *                                               The ConnectionImpl takes ownership of the passed callback.
   *                                               It holds the ownership until either:
   *                                               - message has been completely received.
   *                                               - an error occurred during last chunk reception.
   *                                               - the ConnectionImpl is closed.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to receive from not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            An asynchronous receive operation is currently in
   *                                                            progress.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcReceiveAsyncChunks
   *
   * \vprivate        Vector product internal API
   */
  auto ReceiveAsync(MessageAvailableCallback&& msg_available_callback,
                    MessageChunkReceivedCallback&& msg_chunk_received_callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief     Tries to send a message synchronously.
   *
   * \details
   * The message is sent either as a whole (when a value is returned) or not at all (when an error is returned).
   *
   * Note that HandleEvents() of the Reactor of this ConnectionImpl object has to be called for peer disconnects to be
   * discovered. If HandleEvents() is never called (e.g. because only synchronous APIs are used), SendSync() will never
   * return osabstraction::OsabErrc::kDisconnected even if the peer has closed the ConnectionImpl object or has
   * terminated. If only synchronous APIs are used but disconnects shall still be discovered, HandleEvents() of the
   * Reactor could be called in a background thread, for example.
   *
   * \param[in]       message                 View on an IO buffer array that points to the memory segments that contain
   *                                          the message that shall be sent. The IO buffer array that this span points
   *                                          to and the memory segments that each IO buffer points to have to stay
   *                                          valid during this function call.
   * \param[in]       memory_exchange_handle  Memory exchange handle that shall be attached to the IPC message and sent
   *                                          to the peer. nullptr if no memory exchange handle shall be attached. Note
   *                                          that the peer is not guaranteed to receive an attached memory exchange
   *                                          handle, e.g. if he has insufficient resources to receive it in which case
   *                                          he would only receive the IPC data of the message.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to send via not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            Not enough space in the send buffer to send the message
   *                                                            synchronously or an asynchronous send operation is
   *                                                            currently in progress.
   * \error           osabstraction::OsabErrc::kSize            The size of the passed message exceeds the size of the
   *                                                            send buffer and can therefore never be sent
   *                                                            synchronously or the size of the passed message exceeds
   *                                                            the maximum supported message size.
   * \error           osabstraction::OsabErrc::kDisconnected    The connection has been terminated by the peer.
   *                                                            SupportData of error is set to
   *                                                            amsr::ipc::kOrderlyDisconnect or
   *                                                            amsr::ipc::kAbnormalDisconnect.
   * \error           osabstraction::OsabErrc::kResource        Not enough system resources to send memory exchange
   *                                                            handle to peer.
   * \error           osabstraction::OsabErrc::kProtocolError   Failed to send/receive notification to/from IPC peer or
   *                                                            SafeIPC protocol violation detected. The connection is
   *                                                            broken and should be closed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcSendSync,
   *                  DSGN-Osab-SafeIpcLinuxTransferOfMemoryExchangeHandles
   */
  auto SendSync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> message,
                std::unique_ptr<amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief     Tries to send a message.
   *
   * \details
   * First the function tries to send a message synchronously. If not possible the message will be sent asynchronously.
   *
   * Note that HandleEvents() of the Reactor of this ConnectionImpl object has to be called for peer disconnects to be
   * discovered. If HandleEvents() is never called (e.g. because Send() is expected to always complete synchronously),
   * Send() will never return osabstraction::OsabErrc::kDisconnected even if the peer has closed the ConnectionImpl
   * object or has terminated. If such a situation exists but disconnects shall still be discovered, HandleEvents() of
   * the Reactor could be called in a background thread, for example.
   *
   * \param[in]       message                 View on an IO buffer array that points to the memory segments that contain
   *                                          the message that shall be sent. The IO buffer array that this span points
   *                                          to and the memory segments that each IO buffer points to have to stay
   *                                          valid until the send operation completed or the ConnectionImpl object is
   *                                          closed.
   * \param[in]       callback                The callback which is called to report completion of the operation with
   *                                          the result status if the message could not be sent synchronously. The
   *                                          ConnectionImpl object takes ownership of the passed callback if the
   *                                          message could not be sent synchronously. It holds the ownership until the
   *                                          callback has been executed or until the ConnectionImpl object is closed.
   * \param[in]       memory_exchange_handle  Memory exchange handle that shall be attached to the IPC message and sent
   *                                          to the peer. nullptr if no memory exchange handle shall be attached. Note
   *                                          that the peer is not guaranteed to receive an attached memory exchange
   *                                          handle, e.g. if he has insufficient resources to receive it in which case
   *                                          he would only receive the IPC data of the message.
   *
   * \return          Returns whether the message has been sent synchronously.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to send via not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            An asynchronous send operation is currently in progress.
   * \error           osabstraction::OsabErrc::kSize            The size of the passed message exceeds the maximum
   *                                                            supported message size.
   * \error           osabstraction::OsabErrc::kDisconnected    The connection has been terminated by the peer.
   *                                                            SupportData of error is set to
   *                                                            amsr::ipc::kOrderlyDisconnect or
   *                                                            amsr::ipc::kAbnormalDisconnect.
   * \error           osabstraction::OsabErrc::kResource        Not enough system resources to send memory exchange
   *                                                            handle to peer.
   * \error           osabstraction::OsabErrc::kProtocolError   Failed to send/receive notification to/from IPC peer or
   *                                                            SafeIPC protocol violation detected. The connection is
   *                                                            broken and should be closed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE if kSendCompleted is returned, otherwise FALSE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcSend,
   *                  DSGN-Osab-SafeIpcLinuxTransferOfMemoryExchangeHandles
   */
  auto Send(::amsr::core::Span<osabstraction::io::ConstIOBuffer> message, SendCompletionCallback&& callback,
            std::unique_ptr<amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept
      -> ::amsr::core::Result<SendResult>;

  /*!
   * \brief     Triggers the sending of a message.
   *
   * \details
   * The message will be sent asynchronously.
   *
   * \param[in]       message                 View on an IO buffer array that points to the memory segments that contain
   *                                          the message that shall be sent. The IO buffer array that this span points
   *                                          to and the memory segments that each IO buffer points to have to stay
   *                                          valid until the send operation completed or the ConnectionImpl object is
   *                                          closed.
   * \param[in]       callback                The callback which is called to report completion of the operation with
   *                                          the result status. The ConnectionImpl object takes ownership of the passed
   *                                          callback. It holds the ownership until the callback has been executed or
   *                                          until the ConnectionImpl object is closed.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to send via not connected connection.
   * \error           osabstraction::OsabErrc::kBusy            An asynchronous send operation is currently in progress.
   * \error           osabstraction::OsabErrc::kSize            The size of the passed message exceeds the maximum
   *                                                            supported message size.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcSendAsync
   */
  auto SendAsync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> message,
                 SendCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief     Sends a notification to the IPC peer.
   *
   * \details
   * The notification is a one byte value that the peer will receive if he has registered a NotificationCallback and is
   * calling HandleEvents() on the Reactor of his IPC connection. The notification will be dropped if the peer does not
   * have a NotificationCallback registered when the notification is received by SafeIPC. Notifications will be received
   * by the peer in the same order that they were sent.
   * If too many notifications are sent and the peer does not handle Reactor events fast enough, not only may
   * SendNotification() fail with OsabErrc::kResource error but also sending or reception of IPC messages may fail with
   * OsabErrc::kProtocolError.
   *
   * \param[in]       notification            Notification byte to send to the IPC peer. The value of this byte must not
   *                                          exceed amsr::ipc::kMaxNotificationValue.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to send notification via not connected connection
   *                                                            (this is not returned if the connection was connected
   *                                                            but the peer then disconnected).
   * \error           osabstraction::OsabErrc::kApiError        Notification byte has invalid value.
   * \error           osabstraction::OsabErrc::kResource        Not enough system resources to send notification to
   *                                                            peer.
   * \error           osabstraction::OsabErrc::kProtocolError   Failed to send notification for an unexpected reason.
   *                                                            The connection is broken and should be closed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcLinuxUserNotifications
   */
  auto SendNotification(Notification notification) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief     Registers a callback to receive notifications sent by the IPC peer.
   *
   * \param[in]       callback            Callback to register. The callback is called during Reactor::HandleEvents()
   *                                      every time a notification that was sent by the peer is received until the
   *                                      connection is closed or the callback unregistered with
   *                                      UnregisterNotificationCallback(). The ConnectionImpl object takes ownership of
   *                                      the passed callback. It holds the ownership until
   *                                      UnregisterNotificationCallback() has been called.
   *
   * \error           osabstraction::OsabErrc::kApiError        A notification callback is already registered to this
   *                                                            connection.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcLinuxUserNotifications
   */
  auto RegisterNotificationCallback(NotificationCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief     Unregisters a possibly registered notification callback.
   *
   * \details   The previously registered callback will not be called anymore but a call to the notification callback
   *            may currently be ongoing. Does nothing if no notification callback was registered. Note that a Close()
   *            call does not unregister a possibly registered notification callback.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcLinuxUserNotifications
   */
  void UnregisterNotificationCallback() noexcept;

  /*!
   * \brief     Query the process ID of the communication peer.
   *
   * \return          Process ID of communication peer.
   *
   * \error           osabstraction::OsabErrc::kUnexpected      Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to get peer's PID via not connected connection.
   *                                                            This service does not fail with this error if the
   *                                                            connection was once connected but the peer disconnected.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcGetPeerProcessId
   */
  auto GetPeerProcessId() const noexcept -> ::amsr::core::Result<osabstraction::process::ProcessId>;

  /*!
   * \brief     Query the identity of the communication peer.
   *
   * \return          Credentials of communication peer.
   *
   * \error           osabstraction::OsabErrc::kUnexpected      Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to get peer's credentials via not connected
   *                                                            connection.
   *                                                            This service does not fail with this error if the
   *                                                            connection was once connected but the peer disconnected.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcGetPeerIdentity
   */
  auto GetPeerIdentity() const noexcept -> ::amsr::core::Result<Credentials>;

  /*!
   * \brief     Check the integrity level of the communication peer process.
   *
   * \details   The integrity level of a process cannot be configured on Linux. As such, every process is treated as a
   *            QM process which means that this function only returns true if the expected integrity level is QM.
   *
   * \param[in]       min_expected_level   Minimum integrity level that the peer process is expected to have.
   *
   * \return          true if the peer process has at least the expected integrity level, false otherwise.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to check integrity level of peer process via not
   *                                                            connected connection.
   *                                                            This service does not fail with this error if the
   *                                                            connection was once connected but the peer disconnected.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcGetPeerIntegrityLevel
   */
  auto CheckPeerIntegrityLevel(IntegrityLevel min_expected_level) const noexcept -> ::amsr::core::Result<bool>;

  /*!
   * \brief           Closes the ConnectionImpl object.
   *
   * \details
   * Stops all ongoing asynchronous operations, releases the system resources and puts the ConnectionImpl object back
   * into closed state. No callback will be called anymore but a callback might currently be ongoing. IsInUse() can be
   * used after this call to check if a provided callback is still executing. See ~Connection() why such a check may be
   * necessary. Note that this does not unregister a possibly registered notification callback.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcClose
   */
  void Close() noexcept;

  /*!
   * \brief           Closes the ConnectionImpl object if it is open and asserts that no user provided callback is
   *                  currently executing.
   *
   * \details
   * Stops all ongoing asynchronous operations, releases the system resources and puts the ConnectionImpl object back to
   * uninitialized state. No callback will be called anymore. Note that this does not unregister a possibly registered
   * notification callback.
   *
   * \context         ANY
   *
   * \pre             No user provided callback is currently executing.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcClose,
   *                  DSGN-Osab-SafeIpcLifetimeManagement
   */
  void CloseAndAssertNoCallbackExecuting() noexcept;

  /*!
   * \brief           Checks if the ConnectionImpl object is in open state or not.
   *
   * \details         A client side ConnectionImpl object is in open state after the ConnectAsync() call until the
   *                  Close() call. A server side ConnectionImpl object is in open state from the
   *                  AcceptCompletionCallback onwards until the Close() call.
   *
   * \return          true if the ConnectionImpl object is in open state, otherwise false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcClose
   */
  auto CheckIsOpen() const noexcept -> bool;

  /*!
   * \brief           Checks if the ConnectionImpl object still uses any provided callback after it was closed.
   *
   * \details         This function should be called after the ConnectionImpl object was closed if you want to check if
   *                  any provided callback is still executing. Note that this function always returns true if the
   *                  ConnectionImpl object is still open (the ConnectionImpl object was once opened but Close() was not
   *                  called yet). See ~Connection() why such a check may be necessary.
   *
   * \return          true if the ConnectionImpl object is open or any provided callback is currently executing, false
   *                  otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcLifetimeManagement,
   *                  DSGN-Osab-SafeIpcClose
   */
  auto IsInUse() const noexcept -> bool;

  /*!
   * \brief           Returns the send buffer size of the ConnectionImpl object.
   *
   * \details         The size of the send buffer is defined by this process during the ConnectionImpl::ConnectAsync()
   *                  call (for client side connections) or the AcceptorImpl::Init() call (for server side connections)
   *                  and can be queried with this function after the connection was established.
   *                  Note that the send buffer of the ConnectionImpl object does not have to be equivalent to the send
   *                  buffer size that was requested during the ConnectionImpl::ConnectAsync() call or the
   *                  AcceptorImpl::Init() call if the requested send buffer size is not allowed (either too large or
   *                  too small).
   *
   * \return          Send buffer size of the ConnectionImpl object.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Connection is not connected.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcBufferSizeConfiguration
   */
  auto GetSendBufferSize() const noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief           Returns the receive buffer size of the ConnectionImpl object.
   *
   * \details         The size of the receive buffer is defined by the IPC peer and can be queried with this function
   *                  after the connection was established.
   *
   * \return          Receive buffer size of the ConnectionImpl object.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Connection is not connected.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcBufferSizeConfiguration
   */
  auto GetReceiveBufferSize() const noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief           Initializes a server side ConnectionImpl object.
   *
   * \param[in]       incoming_connection   The incoming client connection established by the Acceptor in the background
   *                                        that shall be used to initialize this ConnectionImpl object. The connection
   *                                        establishment of the incoming connection must be finished. The resources
   *                                        allocated by this incoming connection are moved into this object.
   *                                        The incoming connection is not used anymore after this call.
   *
   * \error           osabstraction::OsabErrc::kResource      Insufficient resources to register the initialized
   *                                                          connection to it's reactor.
   *
   * \context         Acceptor::AcceptCompletionCallback
   *
   * \pre             This ConnectionImpl object is in closed state.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer
   */
  auto InitServerConnection(IncomingConnection& incoming_connection) noexcept -> ::amsr::core::Result<void>;

 private:
  /*!
   * \brief   States of ConnectionImpl's connection state machine.
   */
  enum class ConnectionState : std::uint32_t {
    /*!
     * \brief   ConnectionImpl is closed.
     */
    kClosed,

    /*!
     * \brief   ConnectionImpl encountered an error during connection establishment and released allocated resources.
     */
    kConnectError,

    /*!
     * \brief   ConnectionImpl is currently executing an ConnectAsync() operation.
     */
    kConnecting,

    /*!
     * \brief   ConnectionImpl is connected.
     */
    kConnected
  };

  /*!
   * \brief   States of ConnectionImpl's connection establishment state machine.
   */
  enum class ConnectionEstablishmentState : std::uint32_t {
    /*!
     * \brief   ConnectionImpl is waiting for the completion of the non-blocking connect().
     */
    kCompleteConnect,

    /*!
     * \brief   ConnectionImpl is sending ClientToServerMessage1 to the server.
     */
    kSendingClientToServerMessage1,

    /*!
     * \brief   ConnectionImpl is waiting for reception of ServerToClientMessage1 from the server.
     */
    kWaitingForServerToClientMessage1,

    /*!
     * \brief   ConnectionImpl is sending ClientToServerMessage2 to the server.
     */
    kSendingClientToServerMessage2
  };

  /*!
   * \brief   States of ConnectionImpl's data transfer state machine.
   */
  enum class DataTransferState : std::uint32_t {
    /*!
     * \brief   Default state, the peer has not closed the connection.
     *
     * \details IPC messages are allowed to be sent and received.
     *          Allowed state transitions from kSendAndReceive (excluding a reset of the state on Close()):
     *          kReceiveOnly, kProtocolError
     */
    kSendAndReceive,

    /*!
     * \brief   The IPC peer closed the connection but there might be more data in the receive buffer to be received.
     *
     * \details IPC messages are only allowed to be received but not sent.
     *          Allowed state transitions from kReceiveOnly (excluding a reset of the state on Close()):
     *          kDisconnected, kProtocolError
     */
    kReceiveOnly,

    /*!
     * \brief   The IPC peer closed the connection and the receive buffer was emptied.
     *
     * \details IPC messages are not allowed to be sent or received.
     *          Allowed state transitions from kDisconnected(excluding a reset of the state on Close()):
     *          kProtocolError
     */
    kDisconnected,

    /*!
     * \brief   A protocol error occurred on the IPC connection.
     *
     * \details IPC messages are not allowed to be sent or received.
     *          Allowed state transitions from kProtocolError(excluding a reset of the state on Close()):
     *          -
     */
    kProtocolError
  };

  /*!
   * \brief         Checks if the ConnectionImpl object is in open state or not.
   *
   * \details       Same functionality as CheckIsOpen() but does not acquire the mutex.
   *
   * \return        true if the ConnectionImpl object is in open state, otherwise false.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CheckIsOpenInternal() const noexcept -> bool { return !CheckIsClosedInternal(); }

  /*!
   * \brief         Checks if the ConnectionImpl resources have been already released.
   *
   * \return        true if the ConnectionImpl object already released acquired resources, otherwise false.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CheckAreResourcesReleased() const noexcept -> bool {
    return ((connection_state_ == ConnectionState::kClosed) || (connection_state_ == ConnectionState::kConnectError));
  }

  /*!
   * \brief         Checks if the ConnectionImpl object is in closed state or not.
   *
   * \return        true if the ConnectionImpl object is in closed state, otherwise false.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CheckIsClosedInternal() const noexcept -> bool { return connection_state_ == ConnectionState::kClosed; }

  /*!
   * \brief         Checks if a user provided callback is currently executing.
   *
   * \return        true if a user provided callback is currently executing, otherwise false.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CheckUserCallbackExecuting() const noexcept -> bool { return user_callback_executing_; }

  /*!
   * \brief         Closes the ConnectionImpl object.
   *
   * \details       Implements the closing functionality for Close() and CloseAndAssertNoCallbackExecuting(). Only
   *                closes the object if it is still open.
   *
   * \context       ANY
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void CloseInternal() noexcept;

  /*!
   * \brief         Releases all acquired resources.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void ReleaseResources() noexcept;

  /*!
   * \brief         Verifies function contract of ReceiveAsync() and returns error if contract is violated.
   *
   * \error         osabstraction::OsabErrc::kUninitialized    Connection is not connected.
   * \error         osabstraction::OsabErrc::kBusy             Asynchronous receive already ongoing.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto ReceiveAsyncInternalVerifyContract() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief         Starts asynchronous receive process.
   *
   * \param[in]     chunkwise_receive  specifies if chunkwise receive has been requested.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void ReceiveAsyncInternalStartReceive(bool chunkwise_receive) noexcept;

  /*!
   * \brief         Switches the data transfer state to kSendAndReceive.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void SwitchToSendAndReceiveState() noexcept;

  /*!
   * \brief         Switches the data transfer state to kReceiveOnly.
   *
   * \param[in]     disconnect_error   The disconnect error that shall be reported to the user in future API calls.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void SwitchToReceiveOnlyState(::amsr::core::ErrorCode const& disconnect_error) noexcept;

  /*!
   * \brief         Switches the data transfer state to kDisconnected.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void SwitchToDisconnectedState() noexcept;

  /*!
   * \brief         Switches the data transfer state to kProtocolError.
   *
   * \param[in]     protocol_error   The protocol error that shall be reported to the user in future API calls.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void SwitchToProtocolErrorState(::amsr::core::ErrorCode const& protocol_error) noexcept;

  /*!
   * \brief         Get the current data transfer error.
   *
   * \details       The type of error depends on the current data transfer state:
   *                kSendAndReceive: undefined / kReceiveOnly: OsabErrc::kDisconnected /
   *                kDisconnected: OsabErrc::kDisconnected / kProtocolError: OsabErrc::kProtocolError
   *
   * \return        -
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto GetDataTransferError() const noexcept -> ::amsr::core::ErrorCode { return data_transfer_error_; }

  /*!
   * \brief         Calls the connect completion callback and sets the connection_state_.
   *
   * \details       The mutex is not held during the connect completion callback call.
   *
   * \param[in]     connect_result   Connect result that shall be passed to the user callback. If this contains a value,
   *                                 the connection_state_ is set to kConnected. If this contains an error, the
   *                                 connection_state_ is set to kConnectError and all allocated resources are freed.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           connection_state_ is kConnecting.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void CallConnectCompletionCallback(::amsr::core::Result<void> connect_result) noexcept;

  /*!
   * \brief         Calls the receive completion callback or the receive completion extended callback (whichever the
   *                user provided) and resets the asynchronous receive operation ongoing flag.
   *
   * \details       The mutex is not held during the callback call.
   *
   * \param[in]     result   Receive result that shall be passed to the user callback.
   *
   * \return        True if the connection was closed during the execution of the callback, false otherwise.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           connection_state_ is kConnected.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CallReceiveCompletionCallback(::amsr::core::Result<ReceiveResult> result) noexcept -> bool;

  /*!
   * \brief         Calls the receive completion callback or the receive completion extended callback (whichever the
   *                user provided) if the connection was not closed yet and if an asynchronous receive operation is
   *                currently ongoing.
   *
   * \details       The mutex is not held during the possible receive completion (extended) callback call.
   *
   * \param[in,out] connection_was_closed   In-out parameter.
   *                                        As input: Specifies if the connection was already closed. This function does
   *                                        nothing if this is the case (or if no asynchronous receive operation is
   *                                        ongoing).
   *                                        As output: True if the input was already true or if the connection was
   *                                        closed during the receive completion callback call, false otherwise.
   * \param[in]     result                  Receive result that shall be passed to the user callback if the callback is
   *                                        called.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void CheckAndCallReceiveCompletionCallback(bool& connection_was_closed,
                                             ::amsr::core::Result<ReceiveResult> result) noexcept;

  /*!
   * \brief         Calls the send completion callback and resets the asynchronous send operation ongoing flag.
   *
   * \details       The mutex is not held during the send completion callback call.
   *
   * \param[in]     result   Send result that shall be passed to the user callback.
   *
   * \return        True if the connection was closed during the execution of the send completion callback, false
   *                otherwise.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           connection_state_ is kConnected.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CallSendCompletionCallback(::amsr::core::Result<void> result) noexcept -> bool;

  /*!
   * \brief         Calls the send completion callback if the connection was not closed yet and if an asynchronous
   *                send operation is currently ongoing.
   *
   * \details       The mutex is not held during the possible send completion callback call.
   *
   * \param[in,out] connection_was_closed   In-out parameter.
   *                                        As input: Specifies if the connection was already closed. This function does
   *                                        nothing if this is the case (or if no asynchronous send operation is
   *                                        ongoing).
   *                                        As output: True if the input was already true or if the connection was
   *                                        closed during the send completion callback call, false otherwise.
   * \param[in]     result                  Send result that shall be passed to the user callback if the callback is
   *                                        called.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void CheckAndCallSendCompletionCallback(bool& connection_was_closed, ::amsr::core::Result<void> result) noexcept;

  /*!
   * \brief         Calls the notification callback if it is registered.
   *
   * \details       The mutex is not held during the possible notification callback call.
   *
   * \param[in]     notification            Notification value that shall be passed to the user callback if the callback
   *                                        is called.
   *
   * \return        -
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto CheckAndCallNotificationCallback(Notification notification) noexcept -> bool;

  /*!
   * \brief         Handles Reactor events for the Unix Domain socket.
   *
   * \details       This also includes handling of received user notifications.
   *
   * \param[in]     callback_handle   Handle of the registered Reactor callback that was called.
   * \param[in]     events            Reactor events that occurred.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    TRUE
   */
  void OnSocketEvent(osabstraction::io::reactor1::CallbackHandle callback_handle,
                     osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief         Handles ConnectionImpl software events.
   *
   * \param[in]     callback_handle   Handle of the registered Reactor callback that was called.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    TRUE
   * \steady        FALSE
   */
  void OnSoftwareEvent(osabstraction::io::reactor1::CallbackHandle callback_handle) noexcept;

  /*!
   * \brief         Handles events during data transfer.
   *
   * \details       Checks the data_transfer_state_ and the ongoing asynchronous operations and performs the appropriate
   *                send and/or receive operations and calls the user callbacks (message available callback, receive
   *                completion callback, receive completion extended callback ,send completion callback). This function
   *                is expected to be called with the mutex held from both OnSocketEvent() and OnSoftwareEvent().
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           connection_state_ is kConnected.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    TRUE
   */
  void HandleDataTransfer() noexcept;

  /*!
   * \brief         Checks if an asynchronous receive operation from the IPC channel is currently allowed.
   *
   * \details       Takes into account:
   *                  - If an asynchronous receive operation is currently ongoing.
   *                  - If the data transfer state allows receiving IPC messages.
   *
   * \return        true if receiving is allowed (an asynchronous receive operation is currently ongoing and the data
   *                transfer state is kSendAndReceive or kReceiveOnly), false otherwise.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto AsyncReceiveAllowed() const noexcept -> bool;

  /*!
   * \brief         Checks if an asynchronous send operation to the IPC channel is currently allowed.
   *
   * \details       Takes into account:
   *                  - If an asynchronous send operation is currently ongoing.
   *                  - If the data transfer state allows sending IPC messages.
   *
   * \return        true if sending is allowed (an asynchronous send operation is currently ongoing and the data
   *                transfer state is kSendAndReceive), false otherwise.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto AsyncSendAllowed() const noexcept -> bool;

  /*!
   * \brief         Tries to receive IPC header and IPC data of the currently ongoing asynchronous receive operation
   *                from the Reactor context and calls the user callbacks (message available callback, receive
   *                completion (extended) callback).
   *
   * \return        true if the connection was closed during a user callback call, false otherwise.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           An asynchronous receive operation from the IPC channel is currently allowed
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto ReceiveInReactorContext() noexcept -> bool;

  /*!
   * \brief         Calls the message available callback and provides the returned receive buffers to the message
                    reader.
   *
   * \param         message_size   Size of the IPC message that is about to be received.
   *
   * \return        The receive buffers provided by the user in the message available callback or an empty optional if
   *                the connection was closed during the call to the message available callback.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto ProvideReceiveBuffers(std::uint32_t message_size) noexcept
      -> ::amsr::core::Optional<::amsr::core::Span<osabstraction::io::MutableIOBuffer>>;

  /*!
   * \brief         Calls the message chunk received callback and provides the returned receive buffers to the message
   *                reader.
   *
   * \details       This is only used for intermediate message chunks, the last message chunk received callback of each
   *                message is not called by this functor, instead by CallReceiveCompletionCallback().
   *
   * \param         remaining_message_size   Remaining amount of bytes to be received, for the being processed message.
   *
   * \return        The receive buffers provided by the user in the message chunk received callback or an empty optional
   *                if the connection was closed during the call to the message chunk received callback.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto ProvideIntermediateChunkReceiveBuffers(std::uint32_t remaining_message_size) noexcept
      -> ::amsr::core::Optional<::amsr::core::Span<osabstraction::io::MutableIOBuffer>>;

  /*!
   * \brief         Tries to send IPC header and IPC data of the currently ongoing asynchronous send operation
   *                from the Reactor context and calls the user callback (send completion callback).
   *
   * \return        true if the connection was closed during a user callback call, false otherwise.
   *
   * \context       Reactor::HandleEvents()
   *
   * \pre           An asynchronous send operation to the IPC channel is currently allowed
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto SendInReactorContext() noexcept -> bool;

  /*!
   * \brief         Handles connection establishment Reactor events.
   *
   * \context       Reactor::HandleEvents() thread.
   *
   * \pre           connection_state_ is kConnecting.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void HandleConnectionEstablishment() noexcept;

  /*!
   * \brief         Handles completion of non-blocking connect() of Unix Domain socket.
   *
   * \context       Reactor::HandleEvents() thread.
   *
   * \pre           connection_state_ is kConnecting and ConnectionEstablishmentState is kCompleteConnect.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void HandleConnectCompletion() noexcept;

  /*!
   * \brief           Handles sending of ClientToServerMessage1.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             connection_state_ is kConnecting and ConnectionEstablishmentState is
   *                  kSendingClientToServerMessage1.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleClientToServerMessage1() noexcept;

  /*!
   * \brief           Handles reception of ServerToClientMessage1.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             connection_state_ is kConnecting and ConnectionEstablishmentState is
   *                  kWaitingForServerToClientMessage1.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleServerToClientMessage1() noexcept;

  /*!
   * \brief           Handles sending of ClientToServerMessage2.
   *
   * \context         Reactor::HandleEvents() thread.
   *
   * \pre             connection_state_ is kConnecting and ConnectionEstablishmentState is
   *                  kSendingClientToServerMessage2.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleClientToServerMessage2() noexcept;

  /*!
   * \brief           Registers the socket to the Reactor.
   *
   * \param[in]       socket_fd   Unix Domain socket of the connection.
   * \param[in]       events      Types of events that the socket shall be registered for.
   *
   * \error           osabstraction::OsabErrc::kResource      Insufficient resources to register the socket to the
   *                                                          Reactor.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto RegisterSocketToReactor(osabstraction::io::NativeHandle socket_fd,
                               osabstraction::io::reactor1::EventTypes events) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Sets new events for which the Reactor should wait for on the Unix Domain socket.
   *
   * \param[in]       events   New events that the Reactor should wait for.
   *
   * \context         ANY
   *
   * \pre             The socket is currently registered to the Reactor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void SetMonitoredSocketEvents(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief           Unregisters the Unix Domain socket from the Reactor.
   *
   * \context         ANY
   *
   * \pre             The socket is currently registered to the Reactor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void UnregisterSocketFromReactor() noexcept;

  /*!
   * \brief           Sends a read write notification byte over the Unix Domain socket to the peer.
   *
   * \details         Failure to send the byte because the peer closed the  Unix Domain socket connection is not treated
                      as an error and success is returned.
   *
   * \error           osabstraction::OsabErrc::kProtocolError   Failed to send dummy byte to the peer.
   *
   * \context         ANY
   *
   * \pre             The socket is currently registered to the Reactor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto SendReadWriteNotification() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Shares a file descriptor over the Unix Domain socket.
   *
   * \details         Failure to share the file descriptor because the peer closed the Unix Domain socket connection is
   *                  not treated as an error and success is returned.
   *
   * \param[in]       fd   Valid file descriptor to share.
   *
   * \error           osabstraction::OsabErrc::kResource        Insufficient system resource to send file descriptor or
   *                                                            the peer process exceeded its file descriptor limit or
   *                                                            message could not be sent because socket buffer is full.
   * \error           osabstraction::OsabErrc::kProtocolError   Unexpected failure to send file descriptor to the peer.
   *
   * \context         ANY
   *
   * \pre             The socket is currently registered to the Reactor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto ShareFileDescriptor(osabstraction::io::NativeHandle fd) const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Receive notification bytes and file descriptors from the Unix Domain socket that the peer sent and
   *                  handle user notifications.
   *
   * \details         Multiple notification bytes in the socket can be read in one ReceiveNotificationsAndFds() call.
   *                  Received file descriptors are enqueued in the received file descriptor queue.
   *                  The notification callback is called for every received user notification callback if the user
   *                  registered a notification callback.
   *                  Failure to receive notification bytes because the socket buffer is empty (spurious wakeup) is not
   *                  treated as error.
   *
   * \return          true if a SafeIPC internal notification was received, false otherwise.
   *
   * \error           osabstraction::OsabErrc::kDisconnected    Peer disconnected. kAbnormalClose support data is set if
   *                                                            ReceiveFromSocket() failed with kDisconnected,
   *                                                            kOrderlyClose is returned if the disconnect byte was
   *                                                            read.
   * \error           osabstraction::OsabErrc::kUninitialized   Connection was closed during call of user notification
   *                                                            callback.
   * \error           osabstraction::OsabErrc::kProtocolError   Peer sent bytes with invalid value or receiving from
   *                                                            the socket failed unexpectedly.
   *
   * \context         ANY
   *
   * \pre             The socket is currently registered to the Reactor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto ReceiveNotificationsAndFds() noexcept -> ::amsr::core::Result<bool>;

  /*!
   * \brief         Registers the ConnectionImpl software event to the Reactor.
   *
   * \error         osabstraction::OsabErrc::kResource      Insufficient resources to register the software event to
   *                                                        the Reactor.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  auto RegisterSoftwareEventToReactor() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief         Unregisters the ConnectionImpl software event from the Reactor.
   *
   * \context       ANY
   *
   * \pre           The the ConnectionImpl software event is registered to the Reactor.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   */
  void UnregisterSoftwareEventFromReactor() noexcept;

  /*!
   * \brief         Triggers the ConnectionImpl software event.
   *
   * \context       ANY
   *
   * \pre           The the ConnectionImpl software event is registered to the Reactor.
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   */
  void TriggerSoftwareEvent() noexcept;

  /*!
   * \brief   Tracks the ConnectionImpl's general state.
   */
  ConnectionState connection_state_{ConnectionState::kClosed};

  /*!
   * \brief   Tracks the state of the connection establishment.
   * \details The state is only valid if connection_state_ is kConnecting.
   */
  ConnectionEstablishmentState connection_establishment_state_{ConnectionEstablishmentState::kCompleteConnect};

  /*!
   * \brief   Tracks the state of the data transfer.
   * \details The state is only valid if connection_state_ is kConnected.
   */
  DataTransferState data_transfer_state_{DataTransferState::kSendAndReceive};

  /*!
   * \brief   Error to be returned to the user in case an operation shall fail because of the current data transfer
   *          state.
   * \details If the error is OsabErrc::kDisconnected: The SupportData of the error code is set to kOrderlyDisconnect or
   *          kAbnormalDisconnect and indicates how the peer disconnected.
   */
  ::amsr::core::ErrorCode data_transfer_error_{osabstraction::OsabErrc::kUninitialized};

  /*!
   * \brief   Indicates if an asynchronous receive operation is currently ongoing.
   */
  bool receive_operation_ongoing_{false};

  /*!
   * \brief   Indicates if an asynchronous send operation is currently ongoing.
   */
  bool send_operation_ongoing_{false};

  /*!
   * \brief   Indicates if a user provided callback is currently executing.
   */
  bool user_callback_executing_{false};

  /*!
   * \brief   ConnectionImpl's Reactor.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief   File descriptor of the Unix domain socket associated with this connection.
   */
  osabstraction::io::FileDescriptor socket_{};

  /*!
   * \brief   Handle of the ConnectionImpl's socket event reactor callback.
   * \details kInvalidCallbackHandle if the ConnectionImpl's socket event callback is not registered in the reactor.
   */
  osabstraction::io::reactor1::CallbackHandle socket_reactor_handle_{
      osabstraction::io::reactor1::kInvalidCallbackHandle};

  /*!
   * \brief   Handle of the ConnectionImpl's software event reactor callback.
   * \details kInvalidCallbackHandle if the ConnectionImpl's software event callback is not registered in the reactor.
   */
  osabstraction::io::reactor1::CallbackHandle software_event_reactor_handle_{
      osabstraction::io::reactor1::kInvalidCallbackHandle};

  /*!
   * \brief   ID of the Client-to-server shared memory.
   * \details The optional only contains a value if the Client-to-server shared memory file has been created but not
   *          unlinked yet.
   */
  ::amsr::core::Optional<ShmId> client_to_server_shm_id_{};

  /*!
   * \brief   ID of the Server-to-client shared memory.
   */
  ShmId server_to_client_shm_id_{};

  /*!
   * \brief   ID of the notification shared memory.
   * \details The optional only contains a value if the notification shared memory file has been created but not
   *          unlinked yet.
   */
  ::amsr::core::Optional<ShmId> notification_shm_id_{};

  /*!
   * \brief   The IPC channel between this process and the peer process that is created during the connection
   *          establishment.
   */
  StreamIpcChannel ipc_channel_{};

  /*!
   * \brief   Manages reads from the IPC channel.
   */
  MessageReader message_reader_{ipc_channel_};

  /*!
   * \brief   Manages writes to the IPC channel.
   */
  MessageWriter message_writer_{ipc_channel_};

  /*!
   * \brief   Location to store a received common header.
   */
  stream_ipc_protocol::CommonHeader common_header_receive_buffer_{};

  /*!
   * \brief   Location to store a received format B header.
   */
  stream_ipc_protocol::FormatBHeader format_b_header_receive_buffer_{};

  /*!
   * \brief   Location to store a common header that should be sent.
   */
  stream_ipc_protocol::CommonHeader common_header_send_buffer_{};

  /*!
   * \brief   Location to store a format B header that should be sent.
   */
  stream_ipc_protocol::FormatBHeader format_b_header_send_buffer_{};

  /*!
   * \brief   Queue of file descriptors that were received from the socket.
   * \details The queue can contain invalid file descriptors in case file descriptor exchange bytes were received
   *          without file descriptors.
   */
  FileDescriptorQueue received_fds_queue_{};

  /*!
   * \brief   True if the data of an IPC message that should have an attached file descriptor was completely received
   *          but the attached file descriptor was not received yet.
   */
  bool waiting_for_received_fd_{false};

  /*!
   * \brief   Function object that calls ProvideReceiveBuffers() of a ConnectionImpl object.
   * \details A function_ref to this function object is passed to the MessageReader during PrepareStreamRead().
   */
  class ProvideReceiveBuffersFunctor {
   public:
    /*!
     * \brief   Store the this pointer of the ConnectionImpl object.
     */
    explicit ProvideReceiveBuffersFunctor(ConnectionImpl* connection_instance) noexcept
        : connection_instance_{connection_instance} {}

    /*!
     * \brief   Call ProvideReceiveBuffers() on the stored ConnectionImpl pointer.
     */
    auto operator()(std::uint32_t message_size) noexcept
        -> ::amsr::core::Optional<::amsr::core::Span<osabstraction::io::MutableIOBuffer>> {
      return connection_instance_->ProvideReceiveBuffers(message_size);
    }

   private:
    /*!
     * \brief   ConnectionImpl object whose ProvideReceiveBuffers() method should be called.
     */
    ConnectionImpl* connection_instance_;
  };

  /*!
   * \brief   Function object that calls ProvideIntermediateChunkReceiveBuffers() of a ConnectionImpl object.
   * \details A function_ref to this function object is passed to the MessageReader during PrepareStreamRead().
   */
  class ProvideNextChunkReceiveBuffersFunctor {
   public:
    /*!
     * \brief   Store the this pointer of the ConnectionImpl object.
     */
    explicit ProvideNextChunkReceiveBuffersFunctor(ConnectionImpl* connection_instance) noexcept
        : connection_instance_{connection_instance} {}

    /*!
     * \brief   Call ProvideIntermediateChunkReceiveBuffers() on the stored ConnectionImpl pointer.
     */
    auto operator()(std::uint32_t remaining_message_size) noexcept
        -> ::amsr::core::Optional<::amsr::core::Span<osabstraction::io::MutableIOBuffer>> {
      return connection_instance_->ProvideIntermediateChunkReceiveBuffers(remaining_message_size);
    }

   private:
    /*!
     * \brief   ConnectionImpl object whose ProvideIntermediateChunkReceiveBuffers() method should be called.
     */
    ConnectionImpl* connection_instance_;
  };

  /*!
   * \brief   Function object that calls ProvideReceiveBuffers() of this object.
   */
  ProvideReceiveBuffersFunctor provide_receive_buffers_functor_{this};

  /*!
   * \brief   Function object that calls ProvideIntermediateChunkReceiveBuffers() of this object.
   */
  ProvideNextChunkReceiveBuffersFunctor provide_next_chunk_receive_buffers_functor_{this};

  /*!
   * \brief   Connect completion callback provided by the user.
   */
  ConnectCompletionCallback connect_completion_callback_{};

  /*!
   * \brief   Message available callback provided by the user.
   */
  MessageAvailableCallback msg_available_callback_{};

  /*!
   * \brief   Receive completion callback provided by the user.
   * \details Only one among:
   *          - receive_completion_callback_
   *          - receive_completion_extended_callback_
   *          - msg_chunk_received_callback_
   *          can contain a function.
   */
  ReceiveCompletionCallback receive_completion_callback_{};

  /*!
   * \brief   Receive completion extended callback provided by the user.
   * \details Only one among:
   *          - receive_completion_callback_
   *          - receive_completion_extended_callback_
   *          - msg_chunk_received_callback_
   *          can contain a function.
   */
  ReceiveCompletionExtendedCallback receive_completion_extended_callback_{};

  /*!
   * \brief   Message chunk received callback provided by the user.
   * \details Only one among:
   *          - receive_completion_callback_
   *          - receive_completion_extended_callback_
   *          - msg_chunk_received_callback_
   *          can contain a function.
   */
  MessageChunkReceivedCallback msg_chunk_received_callback_{};

  /*!
   * \brief   Send completion callback provided by the user.
   */
  SendCompletionCallback send_completion_callback_{};

  /*!
   * \brief   Notification callback provided by the user.
   * \details Empty optional if no notification callback is currently registered.
   */
  ::amsr::core::Optional<NotificationCallback> notification_callback_{};

  /*!
   * \brief   Hint how large the shared buffer used for client-to-server IPC messages shall be (in bytes).
   */
  BufferSizeC2S c2s_buffer_size_hint_{};

  /*!
   * \brief   Hint how large the shared buffer used for server-to-client IPC messages shall be (in bytes).
   *
   * \details This member is the buffer from which the sent value will be read. Thus, the fixed-size type.
   */
  std::uint64_t s2c_buffer_size_hint_{};

  /*!
   * \brief   IPC protocol version used by this ConnectionImpl object.
   */
  IpcProtocolVersion ipc_protocol_{stream_ipc_protocol::kId};

  /*!
   * \brief   Type for a ClientToServerMessage1 serializer.
   */
  using C2SMessage1 = ClientToServerMessage1<osabstraction::io::ConstIOBuffer>;

  /*!
   * \brief   Type for a ServerToClientMessage1 deserializer.
   */
  using S2CMessage1 = ServerToClientMessage1<osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief   Type for a ClientToServerMessage2 serializer.
   */
  using C2SMessage2 = ClientToServerMessage2<osabstraction::io::ConstIOBuffer>;

  /*!
   * \brief   Storage for all possible message (de)serializers.
   */
  ::amsr::core::Variant<::amsr::core::monostate, C2SMessage1, S2CMessage1, C2SMessage2> messages_{};

  /*!
   * \brief   Mutex to secure the exclusive access to the class members.
   */
  mutable std::mutex mutex_{};
};

/*!
 * \exclusivearea ::amsr::ipc::internal::ConnectionImpl::mutex_
 * Serializes all ConnectionImpl API calls and all Reactor callback executions of Reactor callbacks registered by the
 * ConnectionImpl.
 *
 * \protects All members of an ConnectionImpl object.
 * \usedin   ::amsr::ipc::internal::ConnectionImpl::ConnectAsync
 *           ::amsr::ipc::internal::ConnectionImpl::GetPendingMsgInfo
 *           ::amsr::ipc::internal::ConnectionImpl::ReceiveSync
 *           ::amsr::ipc::internal::ConnectionImpl::ReceiveAsync
 *           ::amsr::ipc::internal::ConnectionImpl::SendSync
 *           ::amsr::ipc::internal::ConnectionImpl::Send
 *           ::amsr::ipc::internal::ConnectionImpl::SendAsync
 *           ::amsr::ipc::internal::ConnectionImpl::SendNotification
 *           ::amsr::ipc::internal::ConnectionImpl::RegisterNotificationCallback()
 *           ::amsr::ipc::internal::ConnectionImpl::UnregisterNotificationCallback()
 *           ::amsr::ipc::internal::ConnectionImpl::GetPeerIdentity
 *           ::amsr::ipc::internal::ConnectionImpl::Close
 *           ::amsr::ipc::internal::ConnectionImpl::CloseAndAssertNoCallbackExecuting
 *           ::amsr::ipc::internal::ConnectionImpl::CheckIsOpen
 *           ::amsr::ipc::internal::ConnectionImpl::IsInUse
 *           ::amsr::ipc::internal::ConnectionImpl::GetSendBufferSize
 *           ::amsr::ipc::internal::ConnectionImpl::GetReceiveBufferSize
 *           ::amsr::ipc::internal::ConnectionImpl::InitServerConnection
 *           ::amsr::ipc::internal::ConnectionImpl::OnSocketEvent
 *           ::amsr::ipc::internal::ConnectionImpl::OnSoftwareEvent
 * \exclude  ConnectCompletionCallback / MessageAvailableCallback / ReceiveCompletionCallback /
 *           ReceiveCompletionExtendedCallback / SendCompletionCallback / NotificationCallback.
 * \length   LONG The lock is held during connection establishment (creation of shared memories), closing, getting peer
 *           information, (un)registering of notification callback and sending and reception of notifications and IPC
 *           data.
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONNECTION_IMPL_H_

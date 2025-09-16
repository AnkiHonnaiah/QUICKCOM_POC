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
 *        \brief  Provides SafeIPC Connection class.
 *         \unit  osabstraction::ipc::IpcConnection_QNX
 *         \unit  osabstraction::ipc::IpcConnection_LinuxPikeOs
 *   \complexity  The IpcConnection_QNX unit exceeds following metrics:
 *                - SUM, tCYC, mTCYC: Reason is that independent functionalities are implemented in the same
 *                  unit/call-tree (bottlenecks are entry function to handle a pulse event or a software event). There
 *                  are two groups of mostly independent, complex functionalities which are involved here:
 *                    - Connection establishment and data transfer: they have a temporal dependency, because
 *                      data transfer can only happen after connection establishment is completed, otherwise they are
 *                      completely unrelated to each other.
 *                    - Send and receive in reactor context: they may be performed or not, depending on some global
 *                      state of the Connection object, otherwise they are completely unrelated: i.e. they access
 *                      different ring buffers, execute different callbacks.
 *                  Decision is not to split the IpcConnection_QNX in the mentioned groups (which would bring tCYC,
 *                  mTCYC metrics to allowed values), because each independent functionality can and has been fully
 *                  tested on its own.
 *                - mLVL: Reason is that an additional level (member function) has been used to avoid lambdas
 *                  complexity being accounted to the function where the lambda is defined (instead of the
 *                  function where the lambda is executed). This is a workaround for a tooling issue.
 *                - mIMU: Cannot be avoided, since SafeIpc code is spread among different units.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_CONNECTION_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/credentials.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/ipc/internal/incoming_connection_fwd.h"
#include "amsr/ipc/internal/stream_ipc_protocol_common.h"
#include "amsr/ipc/ipc_types.h"
#include "amsr/ipc/unicast_address.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/osab_error_domain.h"
#include "osabstraction/process/process_types.h"

namespace amsr {
namespace ipc {

namespace internal {
class ConnectionImpl;
}  // namespace internal

//  VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_Connection
/*!
 * \brief   SafeIPC Connection class.
 * \details The main class for SafeIPC to handle data transfer and connection establishment for the client side during
 *          unicast based communication.
 */
class Connection final {
 public:
  /*!
   * \brief     Callback that notifies user about a completed asynchronous connect operation.
   *
   * \details   The Connection is still open if the ConnectCompletionCallback reported an error,
   *            Close() is necessary in order to restart connection establishment using
   *            ConnectAsync(). All Connection APIs are allowed to be called from an ConnectCompletionCallback.
   *
   * \param[in] result The result of the connect operation.
   *                   Possible errors:
   *
   *                   osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *                   osabstraction::OsabErrc::kAddressNotAvailable     The server is not available.
   *                   osabstraction::OsabErrc::kInsufficientPrivileges  (Linux only) Not allowed to
   *                                                                     connect to server. This can also happen if the
   *                                                                     server is not completely initialized yet.
   *                   osabstraction::OsabErrc::kDisconnected            The peer aborted the connection establishment
   *                                                                     or is not available anymore.
   *                                                                     SupportData of error is not set.
   *                   osabstraction::OsabErrc::kProtocolError           The SafeIPC protocol was violated or the peer
   *                                                                     did not establish the connection in time or the
   *                                                                     system environment is in an unexpected state
   *                                                                     (which might be caused by interference of an
   *                                                                     outside process, a wrong system integration or
   *                                                                     a failure of a system service, e.g. the file
   *                                                                     system).
   *                   osabstraction::OsabErrc::kResource                Not enough system resources to perform connect
   *                                                                     operation.
   *
   * \context         Reactor1::HandleEvents()
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-SafeIpcReactor,
   *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentClient,
   *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentClient,
   *                  DSGN-Osab-SafeIpcClient
   *
   * \vprivate        Vector product internal API
   */
  using ConnectCompletionCallback = amsr::ipc::ConnectCompletionCallback;

  /*!
   * \brief           Constructs a Connection object.
   *
   * \details         The Connection is initially closed.
   *
   * \param[in]       reactor   Reactor interface for asynchronous operations. The reactor has to exist for the whole
   *                            lifetime of this Connection object. A Connection uses up to 3 Reactor callback
   *                            slots.
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
   *
   * \vprivate        Vector product internal API
   */
  explicit Connection(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept;

  /*!
   * \brief           Move constructs Connection from other Connection.
   *
   * \param[in]       other   Other Connection used to construct this Connection.
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
   * \trace           DSGN-Osab-SafeIpcServer
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Connection(Connection&& other) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief           Move assigns other Connection. This Connection is closed if it is open.
   *
   * \param[in]       other   Other Connection to move into this one.
   *
   * \return          Reference to this object.
   *
   * \context         ANY
   *
   * \pre             No callback (ConnectCompletionCallback / MessageAvailableCallback / ReceiveCompletionCallback /
   *                  SendCompletionCallback) is currently executing for this Connection.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcServer
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Connection& operator=(Connection&& other) & noexcept;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief           Destroys this object. The Connection is closed by this destructor if it is open.
   *
   * \context         ANY
   *
   * \pre             No callback (ConnectCompletionCallback / MessageAvailableCallback / ReceiveCompletionCallback /
   *                  SendCompletionCallback) is currently executing. If you are not sure that this is not the case,
   *                  IsInUse() can be used after the Connection was closed to check if any callback is still executing.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-SafeIpcLifetimeManagement
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~Connection() noexcept;

  Connection(Connection const&) noexcept = delete;
  Connection& operator=(Connection const&) noexcept = delete;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Connects the Connection asynchronously to a remote address.
   *
   * \details
   * The operation is performed asynchronously. The completion callback informs about the completed connect process.
   * The completion callback may never get called if the server does not continue the connection establishment
   * protocol. If this error case is critical for the application, a timer should be used and the ConnectAsync()
   * operation aborted after timeout with a call to Close().
   *
   * \param[in] remote_address                Unicast address of the remote server to connect to.
   * \param[in] callback                      Callback to notify about the completion of the connect operation. The
   *                                          Connection takes ownership of the passed callback. It holds the ownership
   *                                          until the callback has been executed or until the Connection is closed.
   * \param[in] c2s_buffer_size_hint          Optional parameter that gives a hint how large the shared buffer used for
   *                                          client-to-server IPC messages shall be (in bytes). Consider that the
   *                                          amount of IPC data that can be stored in this buffer will be smaller than
   *                                          this size due to SafeIPC protocol overhead for each IPC message that is
   *                                          stored in the buffer (see amsr::ipc::kMessageProtocolOverhead and
   *                                          amsr::ipc::kMessageProtocolOverheadMemoryHandleExchange).
   *                                          The SafeIPC implementation does not honor this value if it is below the
   *                                          lower buffer size limit (see OsAbstraction Technical Reference).
   *
   * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no
   *                                                                     category.
   * \error           osabstraction::OsabErrc::kAlreadyConnected         Tried to connect already connected connection.
   * \error           osabstraction::OsabErrc::kAddressNotAvailable      The server is not available.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges   Not allowed to connect to server. This can also
   *                                                                     happen if the server is not completely
   *                                                                     initialized yet.
   * \error           osabstraction::OsabErrc::kDisconnected             (Linux only) The peer aborted the
   *                                                                     connection establishment or is not available
   *                                                                     anymore.
   *                                                                     SupportData of error is not set.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError   The system environment is in an unexpected
   *                                                                     state. This might be caused by:
   *                                                                      - A file system I/O error or corruption.
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
   *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentClient,
   *                  DSGN-Osab-SafeIpcBufferSizeConfiguration,
   *                  DSGN-Osab-SafeIpcConnectionIsolation
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto ConnectAsync(UnicastAddress remote_address, ConnectCompletionCallback&& callback,
                    std::size_t c2s_buffer_size_hint = internal::stream_ipc_protocol::GetDefaultBufferSize()) noexcept
      -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Connects the Connection asynchronously to a remote address.
   *
   * \details
   * The operation is performed asynchronously. The completion callback informs about the completed connect process.
   * The completion callback may never get called if the server does not continue the connection establishment
   * protocol. If this error case is critical for the application, a timer should be used and the ConnectAsync()
   * operation aborted after timeout with a call to Close().
   *
   * \param[in] remote_address                Unicast address of the remote server to connect to.
   * \param[in] callback                      Callback to notify about the completion of the connect operation. The
   *                                          Connection takes ownership of the passed callback. It holds the ownership
   *                                          until the callback has been executed or until the Connection is closed.
   * \param[in] c2s_buffer_size_hint          Hint how large the shared buffer used for client-to-server IPC messages
   *                                          shall be (in bytes). Consider that the amount of IPC data that can be
   *                                          stored in this buffer will be smaller than this size due to SafeIPC
   *                                          protocol overhead for each IPC message that is stored in the buffer (see
   *                                          amsr::ipc::kMessageProtocolOverhead and
   *                                          amsr::ipc::kMessageProtocolOverheadMemoryHandleExchange).
   *                                          The SafeIPC implementation does not honor this value if it is below the
   *                                          lower buffer size limit (see OsAbstraction Technical Reference).
   * \param[in] s2c_buffer_size_hint          Hint how large the shared buffer used for server-to-client IPC messages
   *                                          shall be (in bytes). Consider that the amount of IPC data that can be
   *                                          stored in this buffer will be smaller than this size due to SafeIPC
   *                                          protocol overhead for each IPC message that is stored in the buffer (see
   *                                          amsr::ipc::kMessageProtocolOverhead and
   *                                          amsr::ipc::kMessageProtocolOverheadMemoryHandleExchange).
   *                                          The SafeIPC implementation does not honor this value if it is below the
   *                                          lower buffer size limit (see OsAbstraction Technical Reference) or
   *                                          above the maximum size hint set for the acceptor (see
   *                                          amsr::ipc::Acceptor::Init()). The actual resulting size of the
   *                                          server-to-client buffer can be queried using
   *                                          amsr::ipc::Connection::GetReceiveBufferSize() after the connection has
   *                                          been established.
   *
   * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no
   *                                                                     category.
   * \error           osabstraction::OsabErrc::kAlreadyConnected         Tried to connect already connected connection.
   * \error           osabstraction::OsabErrc::kAddressNotAvailable      The server is not available.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges   Not allowed to connect to server. This can also
   *                                                                     happen if the server is not completely
   *                                                                     initialized yet.
   * \error           osabstraction::OsabErrc::kDisconnected             (Linux only) The peer aborted the
   *                                                                     connection establishment or is not available
   *                                                                     anymore.
   *                                                                     SupportData of error is not set.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError   The system environment is in an unexpected
   *                                                                     state. This might be caused by:
   *                                                                      - A file system I/O error or corruption.
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
   *                  DSGN-Osab-SafeIpcQNXConnectionEstablishmentClient,
   *                  DSGN-Osab-SafeIpcBufferSizeConfiguration,
   *                  DSGN-Osab-SafeIpcConnectionIsolation
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto ConnectAsync(UnicastAddress remote_address, ConnectCompletionCallback&& callback,
                    BufferSizeC2S c2s_buffer_size_hint, BufferSizeS2C s2c_buffer_size_hint) noexcept
      -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Check the size of the next pending message to be received.
   *
   * \details
   * This function can be called before ReceiveSync() if you want to know the size of the next pending message so that
   * you can provide an appropriately sized buffer to ReceiveSync(). It is not mandatory to call this function before
   * ReceiveSync() and it does not make sense to do so if you 1) know the maximum size of the IPC messages that will be
   * received or 2) pass a buffer to ReceiveSync() that is as large as the receive buffer of the Connection.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetPendingMsgInfo() noexcept -> ::amsr::core::Result<std::size_t>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Tries to receive a message synchronously.
   *
   * \details
   * The message is received either as a whole (when a value or the OsabErrc::kTruncation error is returned) or not at
   * all (when an error other than OsabErrc::kTruncation is returned).
   *
   * If an IPC message with an attached memory exchange handle is received by this function then the attached memory
   * exchange handle is discarded by SafeIPC and only the IPC data is received.
   *
   * Note that HandleEvents() of the Reactor of this Connection has to be called for peer disconnects to be discovered.
   * If HandleEvents() is never called (e.g. because only synchronous APIs are used), ReceiveSync() will never return
   * osabstraction::OsabErrc::kDisconnected even if the peer has closed the Connection or has terminated. If only
   * synchronous APIs are used but disconnects shall still be discovered, HandleEvents() of the Reactor could be called
   * in a background thread, for example.
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
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto ReceiveSync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> buffer) noexcept
      -> ::amsr::core::Result<std::size_t>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
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
   *                                               should be copied to. The Connection takes ownership of the passed
   *                                               callback. It holds the ownership until the callback has been executed
   *                                               or until the Connection is closed.
   * \param[in]       receive_completion_callback  Callback to notify about the result of the reception. The Connection
   *                                               takes ownership of the passed callback. It holds the ownership until
   *                                               the callback has been executed or until the Connection is closed.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
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
   *                                               should be copied to. The Connection takes ownership of the passed
   *                                               callback. It holds the ownership until the callback has been executed
   *                                               or until the Connection is closed.
   * \param[in]       receive_completion_callback  Callback to notify about the result of the reception. The Connection
   *                                               takes ownership of the passed callback. It holds the ownership until
   *                                               the callback has been executed or until the Connection is closed.
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
   *                  DSGN-Osab-SafeIpcQNXTransferOfMemoryExchangeHandles,
   *                  DSGN-Osab-SafeIpcLinuxTransferOfMemoryExchangeHandles
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
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
   *                                               The Connection takes ownership of the passed callback.
   *                                               It holds the ownership until the msg_chunk_received_callback has been
   *                                               executed or until the Connection is closed.
   * \param[in]       msg_chunk_received_callback  Callback to notify about the result of every chunk reception.
   *                                               Returns the view to the buffer where the next message chunk
   *                                               content should be copied to.
   *                                               The Connection takes ownership of the passed callback.
   *                                               It holds the ownership until either:
   *                                               - message has been completely received.
   *                                               - an error occurred during last chunk reception.
   *                                               - the Connection is closed.
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
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto ReceiveAsync(MessageAvailableCallback&& msg_available_callback,
                    MessageChunkReceivedCallback&& msg_chunk_received_callback) noexcept -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Tries to send a message synchronously.
   *
   * \details
   * The message is sent either as a whole (when a value is returned) or not at all (when an error is returned).
   *
   * Note that HandleEvents() of the Reactor of this Connection has to be called for peer disconnects to be discovered.
   * If HandleEvents() is never called (e.g. because only synchronous APIs are used), SendSync() will never return
   * osabstraction::OsabErrc::kDisconnected even if the peer has closed the Connection or has terminated. If only
   * synchronous APIs are used but disconnects shall still be discovered, HandleEvents() of the Reactor could be called
   * in a background thread, for example.
   *
   * \param[in]       message                 View on an IO buffer array that points to the memory segments that contain
   *                                          the message that shall be sent. The IO buffer array that this span points
   *                                          to and the memory segments that each IO buffer points to have to stay
   *                                          valid during this function call.
   * \param[in]       memory_exchange_handle  Memory exchange handle that shall be attached to the IPC message and sent
   *                                          to the peer. nullptr if no memory exchange handle shall be attached. Note
   *                                          that the peer is not guaranteed to receive an attached memory exchange
   *                                          handle, e.g. if he has insufficient resources to receive it in which case
   *                                          he would only receive the IPC data of the message. The Memory exchange
   *                                          handle must not point to a MemoryExchangeHandleInterface implementation
   *                                          object that is not defined by OsAbstraction.
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
   *                  DSGN-Osab-SafeIpcQNXTransferOfMemoryExchangeHandles,
   *                  DSGN-Osab-SafeIpcLinuxTransferOfMemoryExchangeHandles
   *
   * \vprivate        Vector product internal API
   */
  auto SendSync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> message,
                std::unique_ptr<amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept
      -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Tries to send a message.
   *
   * \details
   * First the function tries to send a message synchronously. If not possible the message will be sent asynchronously.
   *
   * If the message is sent asynchronously, it is not specified how much data the peer has to receive before the
   * asynchronous send operation can complete.
   *
   * Note that HandleEvents() of the Reactor of this Connection has to be called for peer disconnects to be discovered.
   * If HandleEvents() is never called (e.g. because Send() is expected to always complete synchronously), Send() will
   * never return osabstraction::OsabErrc::kDisconnected even if the peer has closed the Connection or has terminated.
   * If such a situation exists but disconnects shall still be discovered, HandleEvents() of the Reactor could be called
   * in a background thread, for example.
   *
   * \param[in]       message                 View on an IO buffer array that points to the memory segments that contain
   *                                          the message that shall be sent. The IO buffer array that this span points
   *                                          to and the memory segments that each IO buffer points to have to stay
   *                                          valid until the send operation completed or the Connection is closed.
   * \param[in]       callback                The callback which is called to report completion of the operation with
   *                                          the result status if the message could not be sent synchronously. The
   *                                          Connection takes ownership of the passed callback if the message could not
   *                                          be sent synchronously. It holds the  ownership until the callback has been
   *                                          executed or until the Connection is closed.
   * \param[in]       memory_exchange_handle  Memory exchange handle that shall be attached to the IPC message and sent
   *                                          to the peer. nullptr if no memory exchange handle shall be attached. Note
   *                                          that the peer is not guaranteed to receive an attached memory exchange
   *                                          handle, e.g. if he has insufficient resources to receive it in which case
   *                                          he would only receive the IPC data of the message. The Memory exchange
   *                                          handle must not point to a MemoryExchangeHandleInterface implementation
   *                                          object that is not defined by OsAbstraction.
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
   *                  DSGN-Osab-SafeIpcQNXTransferOfMemoryExchangeHandles,
   *                  DSGN-Osab-SafeIpcLinuxTransferOfMemoryExchangeHandles
   *
   * \vprivate        Vector product internal API
   */
  auto Send(::amsr::core::Span<osabstraction::io::ConstIOBuffer> message, SendCompletionCallback&& callback,
            std::unique_ptr<amsr::memory::MemoryExchangeHandleInterface> memory_exchange_handle = nullptr) noexcept
      -> ::amsr::core::Result<SendResult>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief     Triggers the sending of a message.
   *
   * \details
   * The message will be sent asynchronously.
   *
   * If not enough free space currently exists in the send buffer for the whole message, it is not specified how much
   * data the peer has to receive before the asynchronous send operation can complete.
   *
   * \param[in]       message                 View on an IO buffer array that points to the memory segments that contain
   *                                          the message that shall be sent. The IO buffer array that this span points
   *                                          to and the memory segments that each IO buffer points to have to stay
   *                                          valid until the send operation completed or the Connection is closed.
   * \param[in]       callback                The callback which is called to report completion of the operation with
   *                                          the result status. The Connection takes ownership of the passed callback.
   *                                          It holds the ownership until the callback has been executed or until the
   *                                          Connection object is closed.
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
   *
   * \vprivate        Vector product internal API
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
   * If too many notifications are sent and the peer does not handle Reactor events them fast enough, not only may
   * SendNotification() fail with OsabErrc::kResource error but also sending or reception of IPC messages may fail with
   * OsabErrc::kProtocolError.
   *
   * \param[in]       notification            Notification byte to send to the IPC peer. The value of this byte must not
   *                                          exceed amsr::ipc::kMaxNotificationValue.
   *
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to send notification via not connected connection
   *                                                            (this is not returned if the connection was connected
   *                                                            but the peer then disconnected).
   * \error           osabstraction::OsabErrc::kApiError        Notification byte has invalid value or
   * \error           osabstraction::OsabErrc::kResource        Not enough system resources to send notification to
   *                                                            peer.
   * \error           osabstraction::OsabErrc::kProtocolError   Failed to send notification because the peer violated
   *                                                            the SafeIPC protocol or for an unexpected reason. The
   *                                                            connection is broken and should be closed.
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
   * \trace           DSGN-Osab-SafeIpcQNXUserNotifications,
   *                  DSGN-Osab-SafeIpcLinuxUserNotifications
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
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
   *                                                            connection or
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
   * \trace           DSGN-Osab-SafeIpcQNXUserNotifications,
   *                  DSGN-Osab-SafeIpcLinuxUserNotifications
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
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
   * \trace           DSGN-Osab-SafeIpcQNXUserNotifications,
   *                  DSGN-Osab-SafeIpcLinuxUserNotifications
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
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
   * \error           osabstraction::OsabErrc::kApiError        (PikeOS only) Getting the peer process ID
   *                                                            is not implemented for this operating system.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetPeerProcessId() const noexcept -> ::amsr::core::Result<osabstraction::process::ProcessId>;

  /*!
   * \brief     Query the identity of the communication peer.
   *
   * \return          Credentials of communication peer.
   *
   * \error           osabstraction::OsabErrc::kUnexpected      (Linux only) Unexpected error that is
   *                                                            mapped to no category.
   * \error           osabstraction::OsabErrc::kUninitialized   Tried to get peer's credentials via not connected
   *                                                            connection.
   *                                                            This service does not fail with this error if the
   *                                                            connection was once connected but the peer disconnected.
   * \error           osabstraction::OsabErrc::kApiError        (PikeOS only) Getting the peer identity is not
   *                                                            implemented for this operating system.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetPeerIdentity() const noexcept -> ::amsr::core::Result<Credentials>;

  /*!
   * \brief     Check the integrity level of the communication peer process.
   *
   * \details   The integrity level of a process cannot be configured on Linux and PikeOS. As such, every
   *            process is treated as a QM process on these operating systems and this function then only returns true
   *            if the expected integrity level is QM.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto CheckPeerIntegrityLevel(IntegrityLevel min_expected_level) const noexcept -> ::amsr::core::Result<bool>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  /*!
   * \brief           Closes the Connection.
   *
   * \details
   * Stops all ongoing asynchronous operations and reception of notifications, releases the system resources and puts
   * the Connection back into closed state. No callback will be called anymore but a callback might currently be
   * ongoing. IsInUse() can be used after this call to check if a provided callback is still executing. See
   * ~Connection() why such a check may be necessary. Note that this does not unregister a possibly registered
   * notification callback.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void Close() noexcept;

  /*!
   * \brief           Checks if the Connection is in open state or not.
   *
   * \details         A client side Connection is in open state after the ConnectAsync() call until the Close() call.
   *                  A server side Connection is in open state from the AcceptCompletionCallback onwards until the
   *                  Close() call.
   *
   * \return          true if the Connection is in open state, otherwise false.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto CheckIsOpen() const noexcept -> bool;

  /*!
   * \brief           Checks if the Connection still uses any provided callback after it was closed.
   *
   * \details         This function should be called after the Connection was closed if you want to check if any
   *                  provided callback is still executing. Note that this function always returns true if the
   *                  Connection is still open (the Connection was once opened but Close() was not called yet).
   *                  See ~Connection() why such a check may be necessary.
   *
   * \return          true if the Connection is open or any provided callback is currently executing, false otherwise.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsInUse() const noexcept -> bool;

  /*!
   * \brief           Returns the send buffer size of the Connection.
   *
   * \details         The size of the send buffer is defined by this process during the Connection::ConnectAsync() call
   *                  (for client and server side connections) or the Acceptor::Init() call (limit for server side
   *                  connections) and can be queried with this function after the connection was established.
   *                  Note that the send buffer of the Connection does not have to be equivalent to the send buffer size
   *                  that was requested during the Connection::ConnectAsync() call or the Acceptor::Init() call if the
   *                  requested send buffer size is not allowed (either too large or too small).
   *
   * \return          Send buffer size of the Connection.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetSendBufferSize() const noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief           Returns the receive buffer size of the Connection.
   *
   * \details         The size of the receive buffer is defined by the IPC peer and can be queried with this function
   *                  after the connection was established.
   *
   * \return          Receive buffer size of the Connection.
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
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetReceiveBufferSize() const noexcept -> ::amsr::core::Result<std::size_t>;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_shared_ptr
  // VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_OSA_M7.1.2_os_differences_init_server_connection
  /*!
   * \brief           Initializes a server side Connection.
   *
   * \param[in]       incoming_connection   The incoming client connection established in the background that shall be
   *                                        used to initialize this Connection.
   *
   * \error           osabstraction::OsabErrc::kResource      (Linux only) Insufficient resources to
   *                                                          register the initialized connection to it's reactor.
   *
   * \context         Acceptor::AcceptCompletionCallback
   *
   * \pre             This Connection is in closed state and the connection establishment of incoming_connection was
   *                  finished.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcQNXConnectionEstablishmentServer,
   *                  DSGN-Osab-SafeIpcPOSIXConnectionEstablishmentServer
   *
   * \vprivate        Vector component internal API
   */
  auto InitServerConnection(internal::IncomingConnection& incoming_connection) noexcept -> ::amsr::core::Result<void>;

 private:
  /*!
   * \brief   Implementation of the Connection.
   * \details Ownership of this ConnectionImpl object is shared between this Connection object and all Reactor callbacks
   *          that the ConnectionImpl object registers. This prevents the ConnectionImpl object from being destroyed
   *          while it is still in use in the Reactor.
   */
  std::shared_ptr<internal::ConnectionImpl> connection_impl_;
};

}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_CONNECTION_H_

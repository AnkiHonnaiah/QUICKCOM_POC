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
 *        \brief  Basic stream socket class for stream communication.
 *        \unit   osabstraction::net
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_STREAM_SOCKET_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_STREAM_SOCKET_H_

#include <utility>

#include "vac/language/unique_function.h"

#include "amsr/core/result.h"

#include "osabstraction/io/common_types.h"

#include "amsr/net/internal/basic_socket.h"
#include "amsr/net/internal/os_constants.h"
#include "amsr/net/internal/os_specific_constants.h"
#include "amsr/net/internal/socket_option.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "amsr/core/abort.h"
#include "amsr/core/array.h"
#include "amsr/core/error_code.h"
#include "amsr/core/span.h"
#include "osabstraction/io/internal/io_buffer_queue.h"
#include "osabstraction/io/io_buffer.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Internal connection state.
 */
enum class BasicStreamSocketState : std::uint32_t {
  /*!
   * \brief The socket is not connected.
   */
  kDisconnected = 0,

  /*!
   * \brief Connection establishment in progress.
   *
   * \details
   * This state is only possible during an asynchronous connect operation.
   * Different states are required to distinguish kinds of connect operations to avoid possible race conditions.
   */
  kConnectingAsync = 1,

  /*!
   * \brief Connection establishment in progress.
   *
   * \details
   * This state is only possible during synchronous connect operation in non-blocking mode.
   * Different states are required to distinguish kinds of connect operations to avoid possible race conditions.
   */
  kConnectingSync = 2,

  /*!
   * \brief The socket is connected, sending and receiving is possible.
   */
  kConnected = 3,

  /*!
   * \brief The socket is connected but sending and receiving has both been shut down.
   */
  kDormant = 4,

  /*!
   * \brief Connect failed.
   *
   * If connect fails, the socket is in an unspecified state and has to be closed.
   */
  kConnectError = 5,
};

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_BasicStreamSocket
/*!
 * \brief Implements data exchange via streams.
 *
 * \details
 * All basic socket operations like open/close and the reactor handling is implemented in the base class. This class
 * extents this by providing connect service as well as stream oriented communication methods.
 * For the behavior of this socket in unconventional circumstances see DSGN-Osab-BasicIP_unconventional_circumstances.
 * \tparam  Protocol  The protocol that shall be used.
 */
template <class Protocol>
class BasicStreamSocket final : public BasicSocket<Protocol, BasicStreamSocket<Protocol>> {
 public:
  /*!
   * \copydoc amsr::net::internal::BasicSocket::ProtocolType
   */
  using ProtocolType = Protocol;

  /*!
   * \copydoc amsr::net::internal::BasicSocket::EndpointType
   */
  using EndpointType = typename ProtocolType::Endpoint;

  /*!
   * \brief Notifies about a completed asynchronous connect operation.
   *
   * \param[in] result The result of the connect operation. Possible errors:
   *
   *                   osabstraction::OsabErrc::kDisconnected  The connection has been terminated by peer or the peer is
   *                                                           not reachable.
   *                   osabstraction::OsabErrc::kUnexpected    Unexpected error that is mapped to no category.
   *
   * \context         Reactor context
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  using ConnectCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<void>&& result)>;

  /*!
   * \brief            Notifies about a completed send operation.
   *
   * \details          An asynchronous send operation is completed, if the amount of data that can be hold by the buffer
   *                   has been processed.
   *
   * \param[in] result The result of the send operation. No error means as many bytes as requested have been sent.
   *                   Should an error be reported, some or all data may not have been sent. Possible errors:
   *
   * \error           osabstraction::OsabErrc::kUnexpected             Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kResource               Not enough system resources to conduct operation.
   * \error           osabstraction::OsabErrc::kDisconnected           The connection has been terminated by peer or the
   *                                                                   peer is no longer reachable (this includes but is
   *                                                                   not limited to that the route to the peer has
   *                                                                   been configured as blackhole in the routing table
   *                                                                   after establishing the connection).
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges Permission to access the socket denied.
   *
   * \context         Reactor context
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  using SendCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<void>&& result)>;

  /*!
   * \brief Notifies about a completed asynchronous receive operation.
   *
   * \param[in] result The result of the receive operation. No error means as many bytes as requested have been
   *                   received. Should an error be reported, some data may already have been received but the received
   *                   data is lost, the output buffer is in an undefined state. Possible errors:
   *
   * \error           osabstraction::OsabErrc::kUnexpected             Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kResource               Not enough system resources to conduct operation.
   * \error           osabstraction::OsabErrc::kDisconnected           The connection has been terminated by peer or the
   *                                                                   peer is no longer reachable or has no more data
   *                                                                   to send.
   *
   * \context         Reactor context
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  using ReceiveCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<void>&& result)>;

  /*!
   * \brief Notifies about a completed asynchronous receive operation.
   *
   * \param[in] received_bytes  Number of bytes that were received and written to the provided buffer.
   *                            Should an error be reported, some data may already have been received but the received
   *                            data is lost, the output buffer is in an undefined state. Possible errors:
   *
   * \error           osabstraction::OsabErrc::kUnexpected             Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kResource               Not enough system resources to conduct operation.
   * \error           osabstraction::OsabErrc::kDisconnected           The connection has been terminated by peer or the
   *                                                                   peer is no longer reachable or has no more data
   *                                                                   to send.
   *
   * \context         Reactor context
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  using ReceiveSomeCompletionCallback =
      vac::language::UniqueFunction<void(::amsr::core::Result<std::size_t>&& received_bytes)>;

  /*!
   * \brief Enables or disables the TCP No Delay socket option.
   *
   * \details
   * Usually modern operating systems try to aggregate stream data chunks into big packages to reduce the traffic
   * on the network bus. As a consequence small chunks may be delayed by a few milliseconds (see Nagle's algorithm).
   * This option enables or disables this feature.
   *
   * \trace DSGN-Osab-TCP_OptionDelay
   */
  using SockOptTcpNoDelay = SocketOptionBoolean<IPPROTO_TCP, TCP_NODELAY>;

  /*!
   * \brief Enables or disables the keep alive socket option.
   *
   * \details
   * This option allows to have keep-alive messages being sent cyclically to verify the connection is still up.
   *
   * \trace DSGN-Osab-TCP_OptionKeepAlive
   */
  using SockOptTcpKeepAlive = SocketOptionBoolean<SOL_SOCKET, SO_KEEPALIVE>;

  /*!
   * \brief Set the number of TCP Keep Alive probes that need to be sent before the connection is dropped.
   *
   * \trace DSGN-Osab-TCP_OptionKeepAliveCount
   */
  using SockOptTcpKeepCount = SocketOptionTcpKeepCount;

  /*!
   * \brief Set the interval until the first TCP Keep Alive probe shall be be sent since the last data transfer.
   *
   * \trace DSGN-Osab-TCP_OptionKeepAliveIdle
   */
  using SockOptTcpKeepIdle = SocketOptionTcpKeepIdle;

  /*!
   * \brief Set the interval between each transmission of a TCP Keep Alive probe.
   *
   * \trace DSGN-Osab-TCP_OptionKeepAliveInterval
   */
  using SockOptTcpKeepInterval = SocketOptionTcpKeepInterval;

  /*!
   * \brief Enables or disables the lingering for a TCP socket.
   *
   * \trace DSGN-Osab-BasicIP_OptionLingerZero
   */
  using SockOptTcpLingerZero = SocketOptionLingerZero;

  /*!
   * \brief Construct a stream socket for synchronous communication.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  BasicStreamSocket() noexcept = default;

  // VECTOR NC AutosarC++17_10-A12.1.6: MD_OSA_A12.1.6_ProtectedInheritance
  /*!
   * \brief Construct a stream socket for synchronous and asynchronous communication.
   *
   * \param[in] reactor Reactor interface for asynchronous operations. The reactor has to exist for the whole
   *                    lifetime of this BasicStreamSocket object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  explicit BasicStreamSocket(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
      : BasicSocket<Protocol, BasicStreamSocket<Protocol>>{reactor} {}

  /*!
   * \brief Destroys a socket
   *
   * \details         No ongoing asynchronous call is allowed during destruction.
   *                  The user has to avoid destructing the BasicStreamSocket object while the internal reactor
   *                  callback is executing. This can be done by using a separate software event reactor job,
   *                  or by ensuring per design that BasicStreamSocket is destroyed in the same thread where
   *                  Reactor::HandleEvents() is called.
   *
   * \context         ANY
   *
   * \pre             No ongoing asynchronous callback.
   *                  Should not be executed in the context of the user callback.Reactor of this object should
   *                  not execute this internal reactor callback while the destructor is running.
   *                  Usually this means that HandleEvents() of the reactor should not be running during the
   *                  execution of the destructor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-BasicIP_Destruct
   *
   * \vprivate        Vector product internal API
   */
  /*!
   * \internal
   * - Close the socket if it is not closed yet.
   * - Abort if object is still open to prevent access after delete.
   * \endinternal
   */
  ~BasicStreamSocket() noexcept final {
    // Ignore error. We check after the close if close succeeded. If not we have to abort anyway at this point.
    static_cast<void>(this->Close());

    if (!this->CheckIsClosed()) {
      ::amsr::core::Abort("Failed to close socket in destructor. Object has been destroyed while it was still in use.");
    }
  }

  /*!
   * \brief Deleted copy constructor
   */
  BasicStreamSocket(BasicStreamSocket const& other) = delete;

  /*!
   * \brief Moves the other socket into this one.
   *
   * \param[in] other   Other socket used to construct this socket. Its lock must not be locked. It must not have an
   *                    asynchronous operation ongoing on it.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  BasicStreamSocket(BasicStreamSocket&& other) noexcept;

  /*!
   * \brief Deleted assignment operator
   */
  BasicStreamSocket& operator=(BasicStreamSocket const& other) = delete;

  // VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_SocketBase_MoveOperator
  /*!
   * \brief Closes this socket and moves other socket into this one. Its lock must not be locked.
   *
   * \param[in] other Other socket
   *
   * \return Reference to this object.
   *
   * \context         ANY
   *
   * \pre             No ongoing asynchronous operation.
   * \pre             The lock of this object is not locked.
   *
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto operator=(BasicStreamSocket&& other) & noexcept -> BasicStreamSocket&;

  /*!
   * \brief Binds a socket to a local endpoint.
   *
   * \param[in] endpoint The local network endpoint. The endpoint must use the same network stack as this socket
   *                     (specified during Open() previously).
   *
   * \details
   * The endpoint will be the destination address for incoming communication and the source address for outgoing
   * communication.
   *
   * \return The result of the service.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open or already bound or the socket
   *                                                            uses a different network stack than the passed endpoint.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to bind address.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kAddressError              The address is already in use on the local machine.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \trace DSGN-Osab-BasicIP_Bind
   */
  auto Bind(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Connects the socket to a remote endpoint.
   *
   * \details
   * The operations is performed synchronously. In the non-blocking case, GetRemoteEndpoint() can be used to check if
   * establishing the connection is complete.
   * Note that GetRemoteEndpoint() outputting that the connection is established is not sufficient to conclude that
   * data can actually be transferred over the connection. It is, however, necessary to conclude that establishing the
   * connection may succeed. The only thing that is sufficient to conclude that the connection has been established is
   * that data is successfully received from the remote peer. Establishing the connection requires the remote peer to
   * accept the connection request (see BasicAcceptorSocket).
   * Using this function in non-blocking mode is discouraged, ConnectAsync should be considered for non-blocking
   * connection establishment.
   * If connect fails, the socket has to be closed and cannot be used for another further operations.
   *
   * \param[in] endpoint Endpoint that matches the protocol family passed during open call (e.g Do not try to connect
   *                     to an IPv6 endpoint if you opened an IPv4 socket). The endpoint must use the same network
   *                     stack as this socket (specified during Open() previously).
   *
   * \return result of service.
   *
   * \error osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges to connect to
   *                                                          address.
   * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kDisconnected            The connection has been terminated by peer or the peer is
   *                                                          not reachable or the route to the peer is configured as
   *                                                          blackhole in the routing table.
   * \error osabstraction::OsabErrc::kApiError                The socket is not open or the socket is already connected
   *                                                          or the protocol of the supplied endpoint does not match
   *                                                          the protocol this socket was created for or the socket
   *                                                          uses a different network stack than the passed endpoint.
   * \error osabstraction::OsabErrc::kResource                No ephemeral ports or routing cache entries available.
   * \error osabstraction::OsabErrc::kAddressNotAvailable     Cannot establish connection because local endpoint is
   *                                                          already used by another socket.
   * \error osabstraction::OsabErrc::kBusy                    The connection is being established, use GetRemoteEndpoint
   *                                                          to poll for completion of connection establishment.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE for blocking mode
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Synchronous_Connect_Blocking, DSGN-Osab-TCP_Synchronous_Connect_NonBlocking_Success,
   *                  DSGN-Osab-TCP_Synchronous_Connect_NonBlocking_RequestRefused
   *
   * \vprivate        Vector product internal API
   */
  auto Connect(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Connects the socket asynchronously to a remote endpoint.
   *
   * \details
   * The operations is performed asynchronously. The completion callback informs about the completed connect process.
   * If the operation fails, the socket has to be closed and cannot be used for another further operations.
   *
   * \param[in] endpoint Endpoint that matches the protocol family passed during open call (e.g Do not try to connect
   *                     to an IPv6 endpoint if you opened an IPv4 socket). The endpoint must use the same network
   *                     stack as this socket (specified during Open() previously).
   *
   * \param[in] callback Callback to notify the completion of the connect operations. The socket will take ownership
   *                     of the passed callback. Holds the ownership till the callback has been executed or
   *                     till the socket closed.
   *
   * \return result of service.
   *
   * \error osabstraction::OsabErrc::kDisconnected              The route to the peer is configured as blackhole in the
   *                                                            routing table.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to connect to
   *                                                            address.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open or the socket is already
   *                                                            connected or the protocol of the supplied endpoint does
   *                                                            not match the protocol this socket was created for or
   *                                                            the socket uses a different network stack than the
   *                                                            passed endpoint.
   * \error osabstraction::OsabErrc::kResource                  No ephemeral ports or routing cache entries available.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       Cannot establish connection because local endpoint is
   *                                                            already used by another socket.
   *
   * \context         ANY
   *
   * \pre             Blocking mode is disabled.
   * \pre             Socket can perform asynchronous operations.
   * \pre             -
   *
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Connect
   *
   * \vprivate        Vector product internal API
   */
  auto ConnectAsync(EndpointType const& endpoint, ConnectCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends data synchronously to the connected endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. Must consist
   *                             of at most kMaxIOBufferArraySize entries. The io_buffers_view has to exist for the
   *                             whole lifetime of the class object.
   *
   * \return Number of bytes that have actually been sent.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to conduct operation.
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable (this includes but is not limited
   *                                                            to that the route to the peer has been configured as
   *                                                            blackhole in the routing table after establishing the
   *                                                            connection).
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous send operation.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Permission to access the socket denied.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Synchronous_Send_NormalCase, DSGN-Osab-TCP_Synchronous_Send_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Synchronous_Send_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto SendSync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Sends data synchronously to the connected endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in] buffer_view     View to an array of bytes. Must consist of at most kMaxIOBufferSize bytes. The
   *                            buffer_view has to exist for the whole lifetime of the class object.
   *
   * \return Number of bytes that have actually been sent.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to conduct operation.
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable (this includes but is not limited
   *                                                            to that the route to the peer has been configured as
   *                                                            blackhole in the routing table after establishing the
   *                                                            connection).
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous send operation.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Permission to access the socket denied.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Synchronous_Send_NormalCase, DSGN-Osab-TCP_Synchronous_Send_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Synchronous_Send_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto SendSync(::amsr::core::Span<std::uint8_t const> buffer_view) noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Sends data asynchronously to the connected endpoint.
   *
   * \details
   * The operation is performed asynchronously. The completion callback informs about the completed send operation.
   * The result informs about the success or failure of the operation.
   * The socket must be in non-blocking mode and it must stay in that mode till the operation finishes in the
   * completion callback.
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The socket
   *                             will take exclusive access rights of the buffers and will hold the exclusive access
   *                             rights till the callback has been executed or till the socket is closed
   *                             (CheckIsClosed() == true). Must at most consist of kMaxIOBufferArraySize entries.
   *
   * \param[in] callback Callback to notify the completion of the send operation. An asynchronous send operation is
   *                     completed, if the amount of data that can be hold by the buffer has been processed. The socket
   *                     will take ownership of the passed callback and will hold the ownership till the callback has
   *                     been executed or till the socket is closed (CheckIsClosed() == true).
   *
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable (this includes but is not limited
   *                                                            to that the route to the peer has been configured as
   *                                                            blackhole in the routing table after establishing the
   *                                                            connection).
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous send operation.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Send_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongStateWhileEventHandling
   *
   * \vprivate        Vector product internal API
   */
  auto SendAsync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                 SendCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends data asynchronously to the connected endpoint.
   *
   * \details
   * The operation is performed asynchronously. The completion callback informs about the completed send operation.
   * The result informs about the success or failure of the operation.
   * The socket must be in non-blocking mode and it must stay in that mode till the operation finishes in the
   * completion callback.
   *
   * \param[in] buffer_view  View to an array of bytes. The buffer's size limit is osabstraction::io::kMaxIOBufferSize.
   *                         The socket will take exclusive access rights of the buffers and will hold the exclusive
   *                         access rights till the callback has been executed or till the socket is closed
   *                         (CheckIsClosed() == true). Must at most consist of kMaxIOBufferSize bytes.
   *
   * \param[in] callback Callback to notify the completion of the send operation. An asynchronous send operation is
   *                     completed, if the amount of data that can be hold by the buffer has been processed. The socket
   *                     will take ownership of the passed callback and will hold the ownership till the callback has
   *                     been executed or till the socket is closed (CheckIsClosed() == true).
   *
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable (this includes but is not limited
   *                                                            to that the route to the peer has been configured as
   *                                                            blackhole in the routing table after establishing the
   *                                                            connection).
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous send operation.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Send_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongStateWhileEventHandling
   *
   * \vprivate        Vector product internal API
   */
  auto SendAsync(::amsr::core::Span<std::uint8_t const> buffer_view, SendCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Tries to send data to the connected endpoint synchronously, if not all data can be sent immediately sends
   *        the remaining data asynchronously.
   *
   * \details
   * The operation may be performed asynchronously. That means:
   * - In case the operation is performed asynchronously: The completion callback informs about operation completion,
   *                                                      exclusive access rights of the input buffer are released when
   *                                                      the callback is called.
   * - In case the operation can be completed immediately: The completion callback is not called, exclusive access
   *                                                       rights of the input buffer are released when the function
   *                                                       returns.
   * The return value indicates whether the operation is performed asynchronously or not.
   * Should the operation not complete immediately it is handled as any other asynchronous send operation. Note that
   * this also applies to the precondition of asynchronous send operations that no other asynchronous send operation is
   * allowed to be in progress when an asynchronous send operation is requested.
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The socket
   *                             will take exclusive access rights of the buffers and will hold the exclusive access
   *                             rights till the callback has been executed or till the socket is closed
   *                             (CheckIsClosed() == true). Must at most consist of kMaxIOBufferArraySize entries.
   *
   * \param[in] callback Callback to notify the completion of the send operation. An asynchronous send operation is
   *                     completed, if the amount of data that can be hold by the buffer has been processed. The socket
   *                     will take ownership callback until the operation is completed. Should all data be sent
   *                     immediately ownership is released when this function returns.
   *
   * \return Information if all data was sent or if asynchronous handling is necessary to complete sending.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to conduct operation.
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable (this includes but is not limited
   *                                                            to that the route to the peer has been configured as
   *                                                            blackhole in the routing table after establishing the
   *                                                            connection).
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open or not connected.
   *                                                            Also could happen when there is an ongoing asynchronous
   *                                                            send operation.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Permission to access the socket denied.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE unless asynchronous behaviour is required.
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Optimistic_Send_CompletedAtOnce,
   *                  DSGN-Osab-TCP_Optimistic_Send_CompletedAsynchronously,
   *                  DSGN-Osab-TCP_Optimistic_Send_ConnectionSeparated, DSGN-Osab-TCP_Optimistic_Send_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto Send(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
            SendCompletionCallback&& callback) noexcept -> ::amsr::core::Result<osabstraction::io::SendResult>;

  /*!
   * \brief Tries to send data to the connected endpoint synchronously, if not all data can be sent immediately sends
   *        the remaining data asynchronously.
   *
   * \details
   * The operation may be performed asynchronously. That means:
   * - In case the operation is performed asynchronously: The completion callback informs about operation completion,
   *                                                      exclusive access rights of the input buffer are released when
   *                                                      the callback is called.
   * - In case the operation can be completed immediately: The completion callback is not called, exclusive access
   *                                                       rights of the input buffer are released when the function
   *                                                       returns.
   * The return value indicates whether the operation is performed asynchronously or not.
   * Should the operation not complete immediately it is handled as any other asynchronous send operation. Note that
   * this also applies to the precondition of asynchronous send operations that no other asynchronous send operation is
   * allowed to be in progress when an asynchronous send operation is requested.
   *
   * \param[in] buffer_view  View to an array of bytes. The buffer's size limit is osabstraction::io::kMaxIOBufferSize.
   *                         The socket will take exclusive access rights of the buffers and will hold the exclusive
   *                         access rights till the callback has been executed or till the socket is closed
   *                         (CheckIsClosed() == true). Must at most consist of kMaxIOBufferSize bytes.
   *
   * \param[in] callback Callback to notify the completion of the send operation. An asynchronous send operation is
   *                     completed, if the amount of data that can be hold by the buffer has been processed. The socket
   *                     will take ownership of the passed callback until the operation is completed. Should all data be
   *                     sent immediately ownership is released when this function returns.
   *
   * \return Information if all data was sent or if asynchronous handling is necessary to complete sending.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to conduct operation.
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable (this includes but is not limited
   *                                                            to that the route to the peer has been configured as
   *                                                            blackhole in the routing table after establishing the
   *                                                            connection).
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open or not connected.
   *                                                            Also could happen when there is an ongoing asynchronous
   *                                                            send operation.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Permission to access the socket denied.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE unless asynchronous behaviour is required.
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Optimistic_Send_CompletedAtOnce,
   *                  DSGN-Osab-TCP_Optimistic_Send_CompletedAsynchronously,
   *                  DSGN-Osab-TCP_Optimistic_Send_ConnectionSeparated, DSGN-Osab-TCP_Optimistic_Send_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto Send(::amsr::core::Span<std::uint8_t const> buffer_view, SendCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<osabstraction::io::SendResult>;

  /*!
   * \brief Receives data synchronously from the connected endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in, out] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                                  incoming data will be written to the provided buffers. The content of the buffers
   *                                  is undefined if the service reports an error. The number of written bytes will be
   *                                  returned. Must at most consist of kMaxIOBufferArraySize entries. The
   *                                  io_buffers_view has to exist for the whole lifetime of the class object.
   *
   * \return Number of bytes that have actually been received.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to conduct operation.
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable or has no more data to send.
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous receive operation.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Synchronous_Receive_NormalCase,
   *                  DSGN-Osab-TCP_Synchronous_Receive_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Synchronous_Receive_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto ReceiveSync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Receives data synchronously from the connected endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming data will be written to the provided buffer.
   *                               The content of the buffer is undefined if the service reports an error. The number of
   *                               written bytes will returned. Must at most consist of kMaxIOBufferSize bytes. The
   *                               buffer_view has to exist for the whole lifetime of the class object.
   *
   * \return Number of bytes that have actually been received.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to conduct operation.
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable or has no more data to receive.
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous receive operation.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Synchronous_Receive_NormalCase,
   *                  DSGN-Osab-TCP_Synchronous_Receive_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Synchronous_Receive_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto ReceiveSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Receives data asynchronously from the connected endpoint.
   *
   * \details
   * The operation is performed asynchronously. The completion callback informs about the completed receive operation
   * The result informs about the success or failure of the operation.
   *
   * \param[in, out] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                                  incoming data will be written to the provided buffers. The content of the buffers
   *                                  is undefined if the service reports an error.
   *                                  The socket will take exclusive access rights of the buffer and will hold the
   *                                  exclusive access rights till the callback has been executed or till the socket is
   *                                  closed (CheckIsClosed() == true). Must at most consist of kMaxIOBufferArraySize
   *                                  entries.
   *
   * \param[in] callback Callback to notify the completion of the receive operation. The socket will take ownership
   *                     of the passed callback until the operation is completed. Should the callback report an error,
   *                     data may have been lost and the buffer is in an undefined state. Possible errors:
   *
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the
   *                                                            peer is no longer reachable.
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous receive operation.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector product internal API
   */
  auto ReceiveAsync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                    ReceiveCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Receives data asynchronously from the connected endpoint.
   *
   * \details
   * The operation is performed asynchronously. The completion callback informs about the completed receive operation
   * The result informs about the success or failure of the operation.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming data will be written to the provided buffers.
   *                               The content of the buffer is undefined if the service reports an error.
   *                               The socket will take exclusive access rights of the buffer and will hold the
   *                               exclusive access rights till the callback has been executed or till the socket is
   *                               closed (CheckIsClosed() == true). Must at most consist of kMaxIOBufferSize bytes.
   *
   * \param[in] callback Callback to notify the completion of the receive operation. The socket will take ownership
   *                     of the passed callback until the operation is completed. Should the callback report an error,
   *                     data may have been lost and the buffer is in an undefined state. Possible errors:
   *
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the
   *                                                            peer is no longer reachable.
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous receive operation.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector product internal API
   */
  auto ReceiveAsync(::amsr::core::Span<std::uint8_t> buffer_view, ReceiveCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Receives some data asynchronously from the connected endpoint.
   *
   * \details
   * The operation is performed asynchronously. The completion callback is invoked as soon as data was received, even
   * if the received bytes are less than the capacity of the provided buffer. Consider using the ReceiveAsync() method
   * to ensure that the requested amount of data is received before the asynchronous operation completes. The result
   * informs about the success or failure of the operation.
   *
   * \param[in, out] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                                  incoming data will be written to the provided buffers. The content of the buffers
   *                                  is undefined if the service reports an error. The number of written bytes will be
   *                                  returned.
   *                                  The socket will take exclusive access rights of the buffer and will hold the
   *                                  exclusive access rights till the callback has been executed or till the socket is
   *                                  closed (CheckIsClosed() == true). Must at most consist of kMaxIOBufferArraySize
   *                                  entries.
   *
   * \param[in] callback Callback to notify the completion of the receive operation. The socket will take ownership
   *                     of the passed callback until the operation is completed. Should the callback report an error,
   *                     data may have been lost and the buffer is in an undefined state. Possible errors:
   *
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the
   *                                                            peer is no longer reachable.
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous receive operation.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_Some_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector product internal API
   */
  auto ReceiveAsyncSome(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                        ReceiveSomeCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Receives some data asynchronously from the connected endpoint.
   *
   * \details
   * The operation is performed asynchronously. The completion callback is invoked as soon as data was received. Even
   * the received bytes are less than the capacity of the provided buffer. Consider using the ReceiveAsync() method to
   * ensure that the requested amount of data is received before the asynchronous operation completes. The result
   * informs about the success or failure of the operation.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming data will be written to the provided buffers.
   *                               The content of the buffer is undefined if the service reports an error.
   *                               The socket will take exclusive access rights of the buffer and will hold the
   *                               exclusive access rights till the callback has been executed or till the socket is
   *                               closed (CheckIsClosed() == true). Must at most consist of kMaxIOBufferSize bytes.
   *
   * \param[in] callback Callback to notify the completion of the receive operation. The socket will take ownership
   *                     of the passed callback until the operation is completed. Should the callback report an error,
   *                     data may have been lost and the buffer is in an undefined state. Possible errors:
   *
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the
   *                                                            peer is no longer reachable.
   * \error osabstraction::OsabErrc::kApiError                  Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous receive operation.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_Some_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector product internal API
   */
  auto ReceiveAsyncSome(::amsr::core::Span<std::uint8_t> buffer_view, ReceiveSomeCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Handles asynchronous notifications by the reactor.
   *
   * \details Main entry point for the socket state machine. The function dispatches the event to different state
   *          machines depending on the connection state and the incoming events.
   *
   * \note The function is public to reduce the template complexity.
   *
   * \param[in] events The reactor callback events.
   *
   * \context         Reactor context.
   *
   * \pre             Blocking mode is disabled.
   * \pre             Socket can perform asynchronous operations.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void OnReactorEvents(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief Close has been called.
   *
   * \details BasicSocket::Close() has been called by the user and the socket is closing. Reset internal state machine.
   *
   * \context         Called with a Close() call.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void OnCloseEvent() noexcept;

  /*!
   * \brief Checks whether socket is bound.
   *
   * \return          True if the socket is bound, false otherwise
   *
   * \context         ANY.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto IsSocketBound() const noexcept -> bool {
    this->AssertLockHeldByThread();

    return is_socket_bound_to_endpoint_;
  }

  /*!
   * \brief Assigns a native handle and sets the socket into connected state.
   *
   * \details
   * Calls internal AssignHandle function and sets the state to connected.
   *
   * \post The native handle has been assigned or the native handle has been closed.
   *
   * \param[in] handle       A native handle.
   * \param[in] protocol     The protocol that was used to create the handle.
   * \param[in] netstack_id  The ID of the network stack that was used to create the handle.
   *
   * \return result of service.
   *
   * \error           osabstraction::OsabErrc::kResource      No memory to register another callback or system limit
   *                                                          reached.
   * \context         Acceptor Accept function.
   *
   * \pre             GetNativeHandle() == kInvalidNativeHandle
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto AssignHandleAndConnectSocket(ProtocolType const& protocol, osabstraction::io::NativeHandle handle,
                                    net_utils::NetworkStackId const& netstack_id) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \copydoc amsr::net::internal::BasicSocket::GetSocketLocalEndpoint()
   *
   * \steady          TRUE
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetLocalEndpoint() noexcept -> ::amsr::core::Result<EndpointType>;

  /*!
   * \copydoc amsr::net::internal::BasicSocket::GetSocketPhysicalAddress()
   *
   * \steady          TRUE
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetPhysicalAddress() noexcept -> ::amsr::core::Result<amsr::net::EthernetAddress>;

  /*!
   * \brief Returns the remotely bound network endpoint.
   *
   * \details Can be used to check if connection establishment is complete when requesting a connection non-blocking
   *          synchronously. A remote endpoint being output means that establishing the connection may succeed but it
   *          is not sufficient to conclude that the remote endpoint is able to receive the data sent.
   *
   * \return The remote network endpoint.
   *
   * \error osabstraction::OsabErrc::kResource           Not enough system resources to acquire the address.
   * \error osabstraction::OsabErrc::kApiError           The socket is not open or not connected or no connection is
   *                                                     being established.
   * \error osabstraction::OsabErrc::kUnexpected         Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kBusy               A connection is currently being established.
   * \error osabstraction::OsabErrc::kDisconnected       The socket was once connected but the connection has been
   *                                                     separated.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TCP_AcquirePeerAddress_ConnectionEstablishment_Success,
   *                  DSGN-Osab-TCP_AcquirePeerAddress_ConnectionEstablishment_Failure,
   *                  DSGN-Osab-TCP_AcquirePeerAddress_DataTransfer_Success,
   *                  DSGN-Osab-TCP_AcquirePeerAddress_DataTransfer_Failure
   *
   * \vprivate        Vector product internal API
   */
  auto GetRemoteEndpoint() noexcept -> ::amsr::core::Result<EndpointType>;

 private:
  /*!
   * \brief Moves the content from other socket into this one.
   *
   * \details Moves other socket into this one. Closes this socket if it is still open.
   *
   * \post Other socket is in state connection error and shall not be used anymore.
   *       This socket contains now the context of other socket.
   *
   * \param[in] other   The other socket which content shall be moved into this one. Its lock must not be locked. It
   *                    must not have an asynchronous operation ongoing on it.
   *
   * \context         -
   *
   * \pre             No ongoing asynchronous operation.
   * \pre             The lock of this object is not locked.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void MoveStreamSocket(BasicStreamSocket&& other) noexcept;

  /*!
   * \brief Returns the last error code of a socket.
   *
   * \details
   * Some POSIX function need to look up the last error code. External components shall not use the error code because
   * it is operation system dependent.
   */
  using SockOptError = SocketOptionSignedInteger<SOL_SOCKET, SO_ERROR>;

  /*!
   * \brief Handles asynchronous reactor events during connection establishment.
   *
   * \param[in] events The reactor callback events.
   *
   * \details Finishes the connection establishment. The asynchronous process completes by calling the completion
   *          callback either with a positive or negative result.
   *
   * \context         Reactor context.
   *
   * \pre             connection_state_ == BasicStreamSocketState::kConnectingAsync
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Connect
   *
   * \vprivate        Vector product internal API
   */
  void HandleConnectionEstablishment(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief Handles asynchronous reactor events during data transfer.
   *
   * \details Finishes the data transfer if enough data has been transferred. The asynchronous service completes by
   *          calling the respective completion callback (read or write) either with a positive or negative result.
   *
   * \param[in] events The reactor callback events.
   *
   * \context         Reactor context.
   *
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Send_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongStateWhileEventHandling,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_Some_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void HandleDataTransfer(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief Handles send during asynchronous data transfer.
   *
   * \details Finishes sending if all data has been sent. The asynchronous service completes by calling the respective
              completion callback either with a positive or negative result.
   *
   * \context         Reactor context.
   *
   * \pre             Send buffer must consist of kMaxIOBufferArraySize or fewer entries.
   * \pre             Asynchronous send operation must be in progress.
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Send_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void HandleSend() noexcept;

  /*!
   * \brief           Handles the reactor read event.
   *
   * \details         Depending on if the receive buffer must be completely filled before the user is notified the
   *                  HandleReceive() routine is used to handle the read event. Else the HandleReceiveSome() routine is
   *                  used.
   *
   * \context         Reactor context.
   *
   * \pre             Receive buffer must consist of kMaxIOBufferArraySize or fewer entries.
   * \pre             Asynchronous receive operation must be in progress.
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_Some_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void HandleReadEvent() noexcept;

  /*!
   * \brief Handles receive during asynchronous data transfer.
   *
   * \details Finishes receiving if enough data has been received. The asynchronous service completes by calling the
   *          respective completion callback either with a positive or negative result.
   *
   * \context         Reactor context.
   *
   * \pre             Receive buffer must consist of kMaxIOBufferArraySize or fewer entries.
   * \pre             Asynchronous receive operation must be in progress.
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void HandleReceive() noexcept;

  /*!
   * \brief Handles receive during asynchronous data transfer.
   *
   * \details Finishes receiving when some data has been received. The asynchronous service completes by calling the
   *          appropriate completion callback with either a positive or negative result.
   *
   * \context         Reactor context.
   *
   * \pre             Receive buffer must consist of kMaxIOBufferArraySize or fewer entries.
   * \pre             Asynchronous receive operation must be in progress.
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_Some_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void HandleReceiveSome() noexcept;

  /*!
   * \brief Outputs if the socket is connected or not.
   *
   * \details This function returning true is necessary but not sufficient for data transfer operations to be possible.
   *
   * \return True if the socket is connected, else false.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto IsConnected() const noexcept -> bool {
    return (connection_state_ == BasicStreamSocketState::kConnected) ||
           (connection_state_ == BasicStreamSocketState::kDormant);
  }

  /*!
   * \brief Outputs if an asynchronous operation is in progress on this socket or not.
   *
   * \return True if an asynchronous operation is in progress on this socket, else false.
   *
   * \context         Move operation.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto AsyncOperationInProgress() const noexcept -> bool {
    bool result{false};
    if (connection_state_ == BasicStreamSocketState::kConnectingAsync) {
      result = true;
    } else if (connection_state_ == BasicStreamSocketState::kConnected) {
      result = ongoing_async_send_operation_ || ongoing_async_receive_operation_;
    } else {
      result = false;
    }
    return result;
  }

  /*!
   * \brief Calls the callback that notifies the user about the end of an asynchronous send operation.
   *
   * \details Also handles the request of a new asynchronous send operation in the callback.
   *
   * \param[in] operation_result The result of the operation that shall be passed to the callback.
   *
   * \context         Reactor context
   *
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Send_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void CallSendNotificationCallback(::amsr::core::Result<void> operation_result) noexcept;

  /*!
   * \brief Calls the callback that notifies the user about the end of an asynchronous receive operation.
   *
   * \details Also handles the request of a new asynchronous receive operation in the callback.
   *
   * \param[in] operation_result The result of the operation that shall be passed to the callback.
   *
   * \context         Reactor context
   *
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void CallReceiveNotificationCallback(::amsr::core::Result<void> operation_result) noexcept;

  /*!
   * \brief Calls the callback that notifies the user about the reception of some data.
   *
   * \details Also handles the request of a new asynchronous receive operation in the callback.
   *
   * \param[in] received_bytes Received bytes that were written into the receive buffer provided by the user.
   *
   * \context         Reactor context
   *
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_Some_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void CallReceiveSomeNotificationCallback(::amsr::core::Result<std::size_t> received_bytes) noexcept;

  /*!
   * \brief Sends data synchronously to the connected endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                             io_buffers_view has to exist for the whole lifetime of the class object.
   *
   * \return Number of bytes that have actually been sent.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to conduct operation.
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable (this includes but is not limited
   *                                                            to that the route to the peer has been configured as
   *                                                            blackhole in the routing table after establishing the
   *                                                            connection).
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Permission to access the socket denied.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Synchronous_Send_NormalCase, DSGN-Osab-TCP_Synchronous_Send_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Synchronous_Send_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto DoSyncSend(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Tries to send data to the connected endpoint synchronously, if not all data can be sent immediately sends
   *        the remaining data asynchronously.
   *
   * \details
   * The operation may be performed asynchronously. That means:
   * - In case the operation is performed asynchronously: The completion callback informs about operation completion,
   *                                                      exclusive access rights of the input buffer are released when
   *                                                      the callback is called.
   * - In case the operation can be completed immediately: The completion callback is not called, exclusive access
   *                                                       rights of the input buffer are released when the function
   *                                                       returns.
   * The return value indicates whether the operation is performed asynchronously or not.
   * Should the operation not complete immediately it is handled as any other asynchronous send operation. Note that
   * this also applies to the precondition of asynchronous send operations that no other asynchronous send operation is
   * allowed to be in progress when an asynchronous send operation is requested.
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The socket
   *                             will take exclusive access rights of the buffers and will hold the exclusive access
   *                             rights till the callback has been executed or till the socket is closed
   *                             (CheckIsClosed() == true).
   *
   * \param[in] callback Callback to notify the completion of the send operation. The socket will take ownership
   *                     of the passed callback until the operation is completed. Should all data be sent immediately
   *                     ownership is released when this function returns.
   *
   * \return    Information if the operation is completed asynchronously or not.
   *
   * \error  osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                 Not enough system resources to conduct operation.
   * \error  osabstraction::OsabErrc::kDisconnected             The connection has been terminated by peer or the peer
   *                                                            is no longer reachable (this includes but is not limited
   *                                                            to that the route to the peer has been configured as
   *                                                            blackhole in the routing table after establishing the
   *                                                            connection).
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges   Permission to access socket denied.
   * \error  osabstraction::OsabErrc::kApiError                 Socket is not open and/or connected and/or there is an
   *                                                            ongoing asynchronous send operation.
   *
   * \context         ANY
   *
   * \pre             this->HasReactor() == true
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE unless asynchronous behavior is required.
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Optimistic_Send_CompletedAtOnce,
   *                  DSGN-Osab-TCP_Optimistic_Send_CompletedAsynchronously,
   *                  DSGN-Osab-TCP_Optimistic_Send_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Optimistic_Send_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto DoOptimisticSend(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                        SendCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<osabstraction::io::SendResult>;

  /*!
   * \brief Sets up the object for an asynchronous send operation.
   *
   * \param[in] io_buffers_view The data that shall be sent in the asynchronous send operation. The io_buffers_view has
   *                            to exist for the whole lifetime of the class object.
   * \param[in] data_offset The number of bytes of the data that shall be sent that already has been sent.
   * \param[in] callback The callback that shall be called when the asynchronous send operation ends.
   *
   * \context         ANY
   *
   * \pre             this->HasReactor() == true
   * \pre             The socket is connected.
   * \pre             No other asynchronous send operation is in progress.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Send_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Send_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Send_WrongStateWhileEventHandling,
   *                  DSGN-Osab-TCP_Optimistic_Send_CompletedAsynchronously
   *
   * \vprivate        Vector component internal API
   */
  void SetupAsyncSend(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view, std::size_t data_offset,
                      SendCompletionCallback&& callback) noexcept;

  /*!
   * \brief Receives data synchronously from the connected endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in, out] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                                  incoming data will be written to the provided buffers. The content of the buffers
   *                                  is undefined if the service reports an error. The number of written bytes will be
   *                                  returned. The io_buffers_view has to exist for the whole lifetime of the class
   *                                  object.
   *
   * \return Number of bytes that have actually been received.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to conduct operation.
   * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer
   *                                                            is no longer reachable or has no more data to send.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Synchronous_Receive_NormalCase,
   *                  DSGN-Osab-TCP_Synchronous_Receive_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Synchronous_Receive_WrongState
   *
   * \vprivate        Vector product internal API
   */
  auto DoSyncReceive(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Sets up the object for an asynchronous receive operation.
   *
   * \param[in] io_buffers_view The buffer that shall be written to in the asynchronous receive operation. The
   *                            io_buffers_view has to exist for the whole lifetime of the class object.
   * \param[in] callback        The callback that shall be called when the asynchronous receive operation ends.
   *
   * \context         ANY
   *
   * \pre             this->HasReactor() == true
   * \pre             The socket is connected.
   * \pre             No other asynchronous receive operation is in progress.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void SetupAsyncReceive(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                         ReceiveCompletionCallback&& callback) noexcept;

  /*!
   * \brief Sets up the object for the asynchronous receive operation of some data.
   *
   * \param[in] io_buffers_view The buffer that shall be written to in the asynchronous receive operation. The
   *                            io_buffers_view has to exist for the whole lifetime of the class object.
   * \param[in] callback        The callback that shall be called when the asynchronous receive operation ends.
   *
   * \context         ANY
   *
   * \pre             this->HasReactor() == true
   * \pre             The socket is connected.
   * \pre             No other asynchronous receive operation is in progress.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Asynchronous_Receive_Some_NormalCase_NoNewOperation,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_NormalCase_NewOperationInCallback,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongState,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_WrongStateWhileEventHandling
   *
   * \vprivate        Vector component internal API
   */
  void SetupAsyncReceiveSome(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                             ReceiveSomeCompletionCallback&& callback) noexcept;

  /*!
   * \brief Changes the object state according to the error that occurred during some operation.
   *
   * \details The object state is changed to match the state of the TCP connection of this socket if the reported error
   *          means a change to the state of the TCP connection. That means the state is only changed in case the
   *          reported error indicates that the connection to the peer has been separated.
   *
   * \param[in] error The error that occurred during the operation.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-TCP_Synchronous_Send_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Synchronous_Receive_ConnectionSeparated,
   *                  DSGN-Osab-TCP_Asynchronous_Send_ConnectionSeparatedDetected,
   *                  DSGN-Osab-TCP_Asynchronous_Receive_ConnectionSeparatedDetected
   *
   * \vprivate        Vector component internal API
   */
  void HandleOperationError(::amsr::core::ErrorCode error) noexcept;

  /*!
   * \brief           Check whether that sending is possible.
   *
   * \return          A value if sending is possible, an error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is not open and/or connected and/or there is an
   *                                                             ongoing asynchronous send operation.
   * \error osabstraction::OsabErrc::kDisconnected               Socket is not connected to peer anymore but was
   *                                                             connected before.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckSendIsPossible() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether receiving is possible.
   *
   * \return          A value if receiving is possible, an error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is not open and/or connected and/or there is an
   *                                                             ongoing asynchronous receive operation.
   * \error osabstraction::OsabErrc::kDisconnected               Socket is not connected to peer anymore but was
   *                                                             connected before.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckReceiveIsPossible() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether the socket is not connected.
   *
   * \return          A value if the socket is not connected, an error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is connected.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckSocketIsDisconnected() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether the socket is not bound to an endpoint.
   *
   * \return          A value if not bound, an error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is bound.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckSocketIsNotBoundToEndpoint() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Stores the connection state.
   */
  BasicStreamSocketState connection_state_{BasicStreamSocketState::kDisconnected};

  /*!
   * \brief IO vector with one element
   *
   * \details
   * IO vector for outgoing single buffer requests.
   */
  ::amsr::core::Array<osabstraction::io::ConstIOBuffer, 1> outgoing_single_buffer_io_vector_{};

  /*!
   * \brief IO vector with one element
   *
   * \details
   * IO vector for incoming single buffer requests.
   */
  ::amsr::core::Array<osabstraction::io::MutableIOBuffer, 1> incoming_single_buffer_io_vector_{};

  /*!
   * \brief Stores the callback to be executed on a completed asynchronous connect operation.
   *
   * \details
   * The callback is only valid during asynchronous connect.
   */
  ConnectCompletionCallback on_connect_completion_callback_{};

  /*!
   * Stores the callback to be executed on a completed asynchronous send operation.
   *
   * \brief
   * The callback is only valid during asynchronous send.
   */
  SendCompletionCallback on_send_completion_callback_{};

  /*!
   * \brief
   * Stores the callback to be executed on a completed asynchronous receive operation
   * The callback is only valid during asynchronous receive.
   */
  ReceiveCompletionCallback on_receive_completion_callback_{};

  /*!
   * \brief
   * Stores the callback to be executed on an asynchronous receive some operation.
   * The callback is only valid during asynchronous receive.
   */
  ReceiveSomeCompletionCallback on_receive_some_completion_callback_{};

  /*!
   * \brief Data to be sent asynchronously.
   */
  osabstraction::io::internal::IoBufferQueue<osabstraction::io::ConstIOBuffer> async_send_buffer_{};

  /*!
   * \brief Buffer to write asynchronously received data to.
   */
  osabstraction::io::internal::IoBufferQueue<osabstraction::io::MutableIOBuffer> async_receive_buffer_{};

  /*!
   * \brief Stores if an asynchronous send operation is in progress (true) or not (false).
   */
  bool ongoing_async_send_operation_{false};

  /*!
   * \brief Stores if an asynchronous receive operation is in progress (true) or not (false).
   */
  bool ongoing_async_receive_operation_{false};

  /*!
   * \brief     Stores whether the received data should be forwarded instantly (true) or not (false).
   *
   * \details   If set to true, the user should be notified instantly when (any amount of) data is filled in the
   *            provided user receive buffer.
   *            If set to false data should be received asynchronously till the provided user receive buffer is filled
   *            completely and then notify the user.
   */
  bool async_receive_forward_data_instantly_{false};

  /*!
   * \brief Stores whether the socket is already bound to an endpoint.
   *
   * \details No protection against concurrent access is required since the variable is not accessed from
   *          reactor context.
   */
  bool is_socket_bound_to_endpoint_{false};

  /*!
   * \brief Number of IO buffer elements that are at most processed at once.
   */
  static constexpr std::size_t kMaxBufferElements{50};
};

/***********************************************************************************************************************
 * Template implementation
 **********************************************************************************************************************/

// VECTOR Next Construct AutosarC++17_10-A12.8.4: MD_OSA_A12.8.4_SocketMoveConstructor
/*!
 * \internal
 * - Initialize this socket with default arguments that lead to an abort
 * - Move other socket into this one.
 * \endinternal
 */
template <class Protocol>
BasicStreamSocket<Protocol>::BasicStreamSocket(BasicStreamSocket&& other) noexcept
    : BasicSocket<Protocol, BasicStreamSocket<Protocol>>{}, connection_state_{BasicStreamSocketState::kConnectError} {
  this->MoveStreamSocket(std::move(other));
}

/*!
 * \internal
 * - If not self assignment:
 *    - Move stream socket.
 * \endinternal
 */
// VECTOR Next Construct AutosarC++17_10-A12.8.3: MD_OSA_A12.8.3_read_moved_from_object_falsepositive
// VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_SocketBase_MoveOperator
template <class Protocol>
auto BasicStreamSocket<Protocol>::operator=(BasicStreamSocket&& other) & noexcept -> BasicStreamSocket& {
  if (&other != this) {
    this->MoveStreamSocket(std::move(other));
  }
  return *this;
}

/*!
 * \internal
 * - Enter critical section.
 * - If the socket is open and not yet bound and uses the same network stack as the passed endpoint
 *   - bind the socket to the endpoint.
 * - Leave critical section.
 * - If the socket was not open or already bound or uses a different network stack as the passed endpoint
 *   - output the respective error.
 * - Otherwise
 *   - output the result of binding.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::Bind(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  ::amsr::core::Result<void> const result{
      this->CheckSocketIsOpen()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckSocketIsNotBoundToEndpoint(); })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(endpoint.GetNetworkStackId());
          })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
            return os_interface::Bind(native_handle, endpoint.GetData());
          })
          .Inspect([this]() { this->is_socket_bound_to_endpoint_ = true; })};

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 *  - Enter critical section.
 *  - If socket state and input are valid and the socket uses the same network stack as the passed endpoint
 *    - release the lock.
 *    - Start connecting the socket to the endpoint.
 *    - Re-acquire the lock.
 *    - If connecting the socket to the endpoint failed
 *      - set socket to connect error state.
 *    - If connecting the socket already finished successfully
 *      - set socket to connected state.
 *    - If connection establishment is in progress
 *      - set socket to connecting sync state.
 *  - Leave critical section.
 *  - If socket state and/or input are invalid and/or the socket uses a different network stack than the passed endpoint
 *    - output the respective error.
 *  - Otherwise
 *    - output the result of starting connection establishment.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::Connect(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  // Precondition checked during CheckSocketIsOpen().
  ::amsr::core::Result<void> const connect_result{
      this->CheckSocketIsOpen()
          .AndThen([&endpoint, this]() -> ::amsr::core::Result<void> {
            return this->CheckEndpointProtocolMatches(endpoint);
          })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(endpoint.GetNetworkStackId());
          })
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckSocketIsDisconnected(); })
          .AndThen([&endpoint, this]() -> ::amsr::core::Result<void> {
            osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
            this->ReleaseLock();
            ::amsr::core::Result<void> const result{os_interface::Connect(native_handle, endpoint.GetData())};
            this->AcquireLock();
            return result;
          })
          .Inspect([this]() {
            this->connection_state_ = BasicStreamSocketState::kConnected;
            this->is_socket_bound_to_endpoint_ = true;
          })
          .InspectError([this](::amsr::core::ErrorCode error) {
            if (error == osabstraction::OsabErrc::kBusy) {
              this->connection_state_ = BasicStreamSocketState::kConnectingSync;
              this->is_socket_bound_to_endpoint_ = true;
            } else {
              this->connection_state_ = BasicStreamSocketState::kConnectError;
            }
          })};

  this->ReleaseLock();

  return connect_result;
}

/*!
 * \internal
 *  - Enter critical section.
 *  - Verify the preconditions.
 *  - If socket state and input are valid and the socket is using the same network stack as the passed endpoint
 *    - start non-blocking connection establishment of the socket to the endpoint.
 *    - If connection establishment was started successfully
 *      - set connection state to Connecting
 *      - Store completion callback.
 *      - Wait in reactor connection operation to succeed. An already succeeded call will also wake the reactor.
 *  - Leave critical section.
 *  - If socket state and/or input are invalid and/or the socket is using a different network stack than the passed
 *    endpoint
 *    - output the respective error.
 *  - Otherwise
 *    - output the result of starting connection establishment.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::ConnectAsync(EndpointType const& endpoint,
                                               ConnectCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  this->AcquireLock();

  this->AssertBlockingModeIsDisabled();  // Either disable the blocking mode or use AsyncConnect
  this->AssertReactorAvailable();

  ::amsr::core::Result<void> const connect_result{
      this->CheckSocketIsOpen()
          .AndThen([&endpoint, this]() -> ::amsr::core::Result<void> {
            return this->CheckEndpointProtocolMatches(endpoint);
          })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(endpoint.GetNetworkStackId());
          })
          .AndThen([this]() -> ::amsr::core::Result<void> {
            // It is important to check here for disconnected, especially no asynchronous connect must be started in
            // case a synchronous non-blocking connect is currently ongoing.
            return this->CheckSocketIsDisconnected();
          })
          .AndThen([&endpoint, &callback, this]() -> ::amsr::core::Result<void> {
            osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
            ::amsr::core::Result<void> result{os_interface::Connect(native_handle, endpoint.GetData())};
            bool const update_connection_state{!result.HasValue() ? (result.Error() == osabstraction::OsabErrc::kBusy)
                                                                  : true};
            if (update_connection_state) {
              this->is_socket_bound_to_endpoint_ = true;

              // There should be no ongoing reactor call. Change the state directly.
              this->connection_state_ = BasicStreamSocketState::kConnectingAsync;

              // Only store the callback after the state has been set to asynchronous connecting to avoid the callback
              // being moved in a concurrent move operation (which is anyway prohibited because the move operation of
              // this class is not thread-safe) after it was stored but before the state was set.
              this->on_connect_completion_callback_ = std::move(callback);

              // No protection of the reactor access required because no other operation that can lead to a change of
              // the reactor state can have been started because all these operations require the socket to be connected
              // which is not the case yet.
              this->ChangeWriteObservation(true);
              result = ::amsr::core::Result<void>{};
            } else {
              this->connection_state_ = BasicStreamSocketState::kConnectError;
            }
            return result;
          })};

  this->ReleaseLock();

  return connect_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check if the state of the connection allows sending.
 * - If the state of the connection does allow sending
 *   - conduct a synchronous send operation.
 * - Leave critical section.
 * - If the state does not allow sending
 *   - output the respective error
 * - If the state allowed sending
 *   - output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::SendSync(
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-SendSync-ConstIOBuffer
  this->AcquireLock();

  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<std::size_t> const send_result{this->CheckSendIsPossible().AndThen(
      [this, &io_buffers_view]() -> ::amsr::core::Result<std::size_t> { return this->DoSyncSend(io_buffers_view); })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-SendSync-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check if the state of the connection allows sending.
 * - If the state of the connection does allow sending
 *   - create IOVector with single entry.
 *   - Conduct a synchronous send operation.
 * - Leave critical section.
 * - If the state does not allow sending
 *   - output the respective error
 * - If the state allowed sending
 *   - output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::SendSync(::amsr::core::Span<std::uint8_t const> buffer_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-SendSync-Span
  this->AcquireLock();

  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<std::size_t> const send_result{
      this->CheckSendIsPossible().AndThen([this, &buffer_view]() -> ::amsr::core::Result<std::size_t> {
        ::amsr::core::Array<osabstraction::io::ConstIOBuffer, 1> outgoing_io_vector{
            {osabstraction::io::ConvertSpanToConstIOBuffer(buffer_view)}};
        return this->DoSyncSend(::amsr::core::Span<osabstraction::io::ConstIOBuffer>{outgoing_io_vector});
      })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-SendSync-Span
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows sending
 *   - set up the object to conduct the asynchronous send operation.
 * - Leave critical section.
 * - If the object state does not allow sending
 *   - output the respective error.
 * - If the object was set up for asynchronous send
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::SendAsync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                            SendCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-SendAsync-ConstIOBuffer
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> const send_result{this->CheckSendIsPossible()};

  if (send_result.HasValue()) {
    this->SetupAsyncSend(io_buffers_view, 0, std::move(callback));
  }

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-SendAsync-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows sending
 *   - set up the object to conduct the asynchronous send operation.
 * - Leave critical section.
 * - If the object state does not allow sending
 *   - output the respective error.
 * - If the object was set up for asynchronous send
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::SendAsync(::amsr::core::Span<std::uint8_t const> buffer_view,
                                            SendCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-SendAsync-Span
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const send_result{this->CheckSendIsPossible()};

  if (send_result.HasValue()) {
    this->outgoing_single_buffer_io_vector_[0] = osabstraction::io::ConvertSpanToConstIOBuffer(buffer_view);

    this->SetupAsyncSend(::amsr::core::Span<osabstraction::io::ConstIOBuffer>{this->outgoing_single_buffer_io_vector_},
                         0, std::move(callback));
  }

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-SendAsync-Span
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the state of the connection does allow sending
 *   - do and optimistic send.
 * - Leave critical section.
 * - If the state does not allow sending
 *   - output the respective error
 * - If the state allowed sending
 *   - output the result of the optimistic send.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::Send(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                       SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<osabstraction::io::SendResult> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-Send-ConstIOBuffer
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<osabstraction::io::SendResult> const send_result{this->CheckSendIsPossible().AndThen(
      [this, &io_buffers_view, &callback]() -> ::amsr::core::Result<osabstraction::io::SendResult> {
        return this->DoOptimisticSend(io_buffers_view, std::move(callback));
      })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-Send-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the state of the connection does allow sending
 *   - Create IOVector with single entry.
 *   - Do optimistic send.
 * - Leave critical section.
 * - If the state does not allow sending
 *   - output the respective error
 * - If the state allowed sending
 *   - output the result of the optimistic send.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::Send(::amsr::core::Span<std::uint8_t const> buffer_view,
                                       SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<osabstraction::io::SendResult> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-Send-Span
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<osabstraction::io::SendResult> const send_result{this->CheckSendIsPossible().AndThen(
      [this, &buffer_view, &callback]() -> ::amsr::core::Result<osabstraction::io::SendResult> {
        this->outgoing_single_buffer_io_vector_[0] = osabstraction::io::ConvertSpanToConstIOBuffer(buffer_view);

        return this->DoOptimisticSend(
            ::amsr::core::Span<osabstraction::io::ConstIOBuffer>{this->outgoing_single_buffer_io_vector_},
            std::move(callback));
      })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-Send-Span
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the state of the connection does allow receiving
 *   - conduct a synchronous receive operation.
 * - Leave critical section.
 * - If the state does not allow receiving
 *   - output the respective error
 * - If the state allowed receiving
 *   - output the result of the receive operation.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::ReceiveSync(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-ReceiveSync-MutableIOBuffer
  this->AcquireLock();

  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<std::size_t> const receive_result{
      this->CheckReceiveIsPossible().AndThen([this, &io_buffers_view]() -> ::amsr::core::Result<std::size_t> {
        return this->DoSyncReceive(io_buffers_view);
      })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-ReceiveSync-MutableIOBuffer
  return receive_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the state of the connection does allow receiving
 *   - Create IOVector with single entry.
 *   - call the operating system service to receive data.
 * - Leave critical section.
 * - If the state does not allow receiving
 *   - output the respective error
 * - If the state allowed receiving
 *   - output the result of the receive operation.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::ReceiveSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-ReceiveSync-Span
  this->AcquireLock();

  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<std::size_t> const receive_result{
      this->CheckReceiveIsPossible().AndThen([this, &buffer_view]() -> ::amsr::core::Result<std::size_t> {
        ::amsr::core::Array<osabstraction::io::MutableIOBuffer, 1> incoming_io_vector{
            {osabstraction::io::ConvertSpanToMutableIOBuffer(buffer_view)}};

        return this->DoSyncReceive(::amsr::core::Span<osabstraction::io::MutableIOBuffer>{incoming_io_vector});
      })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-ReceiveSync-Span
  return receive_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows receiving
 *   - set up the object to conduct the asynchronous receive operation.
 * - Leave critical section.
 * - If the object state does allow not receiving
 *   - output the respective error.
 * - If the object has been set up for receiving
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::ReceiveAsync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                                               ReceiveCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-ReceiveAsync-MutableIOBuffer
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> const receive_result{this->CheckReceiveIsPossible()};

  if (receive_result.HasValue()) {
    this->SetupAsyncReceive(io_buffers_view, std::move(callback));
  }

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-ReceiveAsync-MutableIOBuffer
  return receive_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows receiving
 *   - create IOVector with single entry.
 *   - Set up the object to conduct the asynchronous receive operation.
 * - Leave critical section.
 * - If the object state does allow not receiving
 *   - output the respective error.
 * - If the object has been set up for receiving
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::ReceiveAsync(::amsr::core::Span<std::uint8_t> buffer_view,
                                               ReceiveCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-ReceiveAsync-Span
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const receive_result{this->CheckReceiveIsPossible()};

  if (receive_result.HasValue()) {
    this->incoming_single_buffer_io_vector_[0] = osabstraction::io::ConvertSpanToMutableIOBuffer(buffer_view);

    this->SetupAsyncReceive(
        ::amsr::core::Span<osabstraction::io::MutableIOBuffer>{this->incoming_single_buffer_io_vector_},
        std::move(callback));
  }

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-ReceiveAsync-Span
  return receive_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows receiving
 *   - set up the object to conduct the asynchronous receive some operation.
 * - Leave critical section.
 * - If the object state does allow not receiving
 *   - output the respective error.
 * - If the object has been set up for receiving
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::ReceiveAsyncSome(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
    ReceiveSomeCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-ReceiveAsyncSome-MutableIOBuffer
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> const receive_result{this->CheckReceiveIsPossible()};

  if (receive_result.HasValue()) {
    this->SetupAsyncReceiveSome(io_buffers_view, std::move(callback));
  }

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-ReceiveAsyncSome-MutableIOBuffer
  return receive_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows receiving
 *   - create IOVector with single entry.
 *   - Set up the object to conduct the asynchronous receive some operation.
 * - Leave critical section.
 * - If the object state does allow not receiving
 *   - output the respective error.
 * - If the object has been set up for receiving
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::ReceiveAsyncSome(::amsr::core::Span<std::uint8_t> buffer_view,
                                                   ReceiveSomeCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-TCP-DataTransfer-ReceiveAsyncSome-Span
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const receive_result{this->CheckReceiveIsPossible()};

  if (receive_result.HasValue()) {
    this->incoming_single_buffer_io_vector_[0] = osabstraction::io::ConvertSpanToMutableIOBuffer(buffer_view);

    this->SetupAsyncReceiveSome(
        ::amsr::core::Span<osabstraction::io::MutableIOBuffer>{this->incoming_single_buffer_io_vector_},
        std::move(callback));
  }

  this->ReleaseLock();

  // PTP-E-OsAbstraction-TCP-DataTransfer-ReceiveAsyncSome-Span
  return receive_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Assign native handle.
 * - If assigning the native handle succeeded
 *   - set state to connected.
 * - Leave critical section.
 * - If assigning the native handle failed
 *   - output the error that made it fail.
 * - If assigning the native handle succeeded
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::AssignHandleAndConnectSocket(ProtocolType const& protocol,
                                                               osabstraction::io::NativeHandle handle,
                                                               net_utils::NetworkStackId const& netstack_id) noexcept
    -> ::amsr::core::Result<void> {
  this->AcquireLock();
  ::amsr::core::Result<void> const result{
      this->AssignNativeHandle(protocol, handle, netstack_id).AndThen([this]() -> ::amsr::core::Result<void> {
        // No possible racing condition with a close. This function will be called by the Acceptor.
        this->connection_state_ = BasicStreamSocketState::kConnected;
        this->is_socket_bound_to_endpoint_ = true;
        return ::amsr::core::Result<void>{};
      })};
  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Return basic socket functionality.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::GetLocalEndpoint() noexcept -> ::amsr::core::Result<EndpointType> {
  return BasicSocket<Protocol, BasicStreamSocket<Protocol>>::GetSocketLocalEndpoint();
}

/*!
 * \internal
 * - Return basic socket functionality.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::GetPhysicalAddress() noexcept -> ::amsr::core::Result<amsr::net::EthernetAddress> {
  return BasicSocket<Protocol, BasicStreamSocket<Protocol>>::GetSocketPhysicalAddress();
}

/*!
 * \internal
 * - Enter critical section.
 * - If the socket is connecting synchronously
 *   - try to acquire the remote endpoint.
 *   - If the peer can not be reached
 *      - Check if a socket error has been occurred.
 *      - If a socket error occures set the object state to indicate that establishing a connection failed.
 *      - Else set the object state to indicate that establishing a connection is still ongoing.
 *   - If the peer can be reached
 *       - set the object state to indicate that the socket is connected.
 * - If the socket is connected
 *   - try to acquire the remote endpoint.
 *   - If the peer can not be reached
 *     - set the object state to indicate that the socket is not connected.
 *     - Disable all reactor events.
 * - Leave critical section.
 * - If the state did not allow acquiring the remote endpoint or acquiring it failed
 *   - output the respective error.
 * - If the state allowed to acquire the remote endpoint and it could be acquired
 *   - output the acquired endpoint.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::GetRemoteEndpoint() noexcept -> ::amsr::core::Result<EndpointType> {
  ::amsr::core::Result<EndpointType> result{MakeErrorCode(
      osabstraction::OsabErrc::kApiError, "Socket is not connected or in the process of establishing a connection.")};

  this->AcquireLock();

  if (connection_state_ == BasicStreamSocketState::kConnectingSync) {
    result = this->GetEndpoint(&os_interface::GetPeerName)
                 .Inspect([this](EndpointType) { connection_state_ = BasicStreamSocketState::kConnected; })
                 .OrElse([this](::amsr::core::ErrorCode const& get_peer_name_error) {
                   ::amsr::core::Result<EndpointType> error_to_return{get_peer_name_error};

                   if (get_peer_name_error == osabstraction::OsabErrc::kDisconnected) {
                     // kDisconnected indicates that the connection establishment failed or that the connection
                     // establishment is still ongoing. Therefore, we must check the socket for occurred errors.
                     error_to_return =
                         this->GetOptionInternal(OverloadType<SockOptError>{})
                             // VECTOR NL AutosarC++17_10-A5.1.8: MD_OSA_A5.1.8_nested_lambda
                             .InspectError([this](::amsr::core::ErrorCode const& get_opt_error) {
                               connection_state_ = BasicStreamSocketState::kConnectError;
                               if (get_opt_error == osabstraction::OsabErrc::kSystemEnvironmentError) {
                                 ::amsr::core::Abort("Socket does not support SO_ERROR socket option.");
                               }
                             })
                             // VECTOR NL AutosarC++17_10-A5.1.8: MD_OSA_A5.1.8_nested_lambda
                             .AndThen([this](SockOptError const& socket_error) {
                               ::amsr::core::Result<EndpointType> mapped_socket_error{
                                   MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy Error.")};

                               constexpr std::int32_t kNoSocketError{0};
                               if (socket_error.Value() == kNoSocketError) {  // Connection establishment in progress.
                                 connection_state_ = BasicStreamSocketState::kConnectingSync;
                                 mapped_socket_error =
                                     ::amsr::core::Result<EndpointType>{osabstraction::OsabErrc::kBusy};
                               } else {  // Socket error detected. Connection establishment failed.
                                 connection_state_ = BasicStreamSocketState::kConnectError;
                                 mapped_socket_error =
                                     ::amsr::core::Result<EndpointType>{osabstraction::OsabErrc::kDisconnected};
                               }
                               return mapped_socket_error;
                             });
                   }

                   return error_to_return;
                 });

  } else if (connection_state_ == BasicStreamSocketState::kConnected) {
    result = this->GetEndpoint(&os_interface::GetPeerName);
    if (result.HasValue()) {
      // Output acquired endpoint, no state change required.
    } else if (result.Error() == osabstraction::OsabErrc::kDisconnected) {
      result = ::amsr::core::Result<EndpointType>{
          MakeErrorCode(osabstraction::OsabErrc::kDisconnected, "Connection to peer separated.")};
      connection_state_ = BasicStreamSocketState::kDormant;
      if (this->HasReactor()) {
        this->ChangeWriteObservation(false);
        this->ChangeReadObservation(false);
      }
    } else {
      // Some uninteresting error, just output it.
    }
  } else if (connection_state_ == BasicStreamSocketState::kDormant) {
    // No API error because the connection separation may have been detected by concurrent activity in the reactor
    // context.
    result = ::amsr::core::Result<EndpointType>{
        MakeErrorCode(osabstraction::OsabErrc::kDisconnected, "Connection to peer separated.")};
  } else {
    // Use default-assigned value.
  }

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Verify the precondition.
 * - Dispatch according to connection state.
 *   - Directly return if socket is disconnected.
 *   - Call HandleConnectionEstablishment on asynchronous connection establishment.
 *   - Call HandleDataTransfer on connected state.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::OnReactorEvents(osabstraction::io::reactor1::EventTypes events) noexcept {
  // PTP-B-OsAbstraction-TCP-ReactorTask
  this->AssertLockHeldByThread();

  // VECTOR Next Construct AutosarC++17_10-M6.4.3: MD_OSA_M6.4.3_switch_state_machine_enum_no_default_required
  // VECTOR Next Construct AutosarC++17_10-M6.4.6: MD_OSA_M6.4.6_switch_state_machine_enum_no_default_required
  switch (this->connection_state_) {
    case BasicStreamSocketState::kDisconnected:
      // Close has been called before getting the connection state. Finish the callback and let the reactor release
      // the operating system resources.
      break;
    case BasicStreamSocketState::kConnectingAsync:
      // Lock has been held since before function was called so the precondition of HandleConnectionEstablishment
      // about connection_state_ holds.
      this->HandleConnectionEstablishment(events);
      break;
    case BasicStreamSocketState::kConnected:
      this->HandleDataTransfer(events);
      break;
    case BasicStreamSocketState::kDormant:
      // An error has been detected concurrently before getting the connection state. Do nothing as there was an
      // error.
      break;
    case BasicStreamSocketState::kConnectingSync:
      // This should never be entered, all functions that start enable reactor events should check that no synchronous
      // non-blocking connection establishment is ongoing.
      ::amsr::core::Abort("Reactor event encountered during synchronous non-blocking connection establishment.");
      break;
    case BasicStreamSocketState::kConnectError:
      // Development error. Should never be reached.
      // It is also possible to reach this state during move operations if the thread safety precondition has been
      // violated.
      ::amsr::core::Abort(
          "A socket has to be closed if a connect service fails. It shall not be used for any other operation.");
      break;
  }
  // PTP-E-OsAbstraction-TCP-ReactorTask
}

/*!
 * \internal
 * - Verify the precondition.
 * - Set the state to disconnected.
 * - Set that socket is not bound.
 * - Set that no asynchronous operation is ongoing.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::OnCloseEvent() noexcept {
  this->AssertLockHeldByThread();

  connection_state_ = BasicStreamSocketState::kDisconnected;
  this->is_socket_bound_to_endpoint_ = false;
  this->ongoing_async_send_operation_ = false;
  this->ongoing_async_receive_operation_ = false;
}

/*!
 * \internal
 * - Request the last socket error and parse it.
 * - If not spurious wake:
 *   - Disable reactor write monitoring.
 *   - If connect succeeded:
 *     - Set state to connected.
 *   - If connect failed:
 *     - Set state to disconnected.
 *   - Release the lock.
 *   - Call completion callback with result.
 *   - Re-acquire the lock.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::HandleConnectionEstablishment(osabstraction::io::reactor1::EventTypes) noexcept {
  ::amsr::core::Result<SockOptError> get_error_result{this->GetOptionInternal(OverloadType<SockOptError>{})};
  // Initialize with a default value. Will be overwritten later.
  ::amsr::core::Result<void> result{osabstraction::OsabErrc::kApiError};
  if (!get_error_result.HasValue()) {
    if (get_error_result.Error() == osabstraction::OsabErrc::kSystemEnvironmentError) {
      ::amsr::core::Abort("Socket does not support SO_ERROR socket option.");
    } else {
      // Forward the error to the completion callback.
      result = get_error_result.Drop();
    }
  } else {
    result = os_interface::CompleteNonBlockingConnect(
        osabstraction::internal::OsErrorNumber{get_error_result.Value().Value()});
  }

  bool const spurious_wake{result == osabstraction::OsabErrc::kBusy};

  if (!spurious_wake) {
    // Take callback to local variable before setting the state to avoid race condition with move.
    // Take callback to local variable before resetting the connection state to avoid a race condition with a concurrent
    // move (which is prohibited because the move operation of this class is not thread-safe) that could take place
    // after the state was reset but before the callback was taken to a local variable.
    ConnectCompletionCallback callback{std::move(this->on_connect_completion_callback_)};

    // Disable the write observation. The asynchronous service ends here regardless of the result.
    this->ChangeWriteObservation(false);

    connection_state_ = result.HasValue() ? BasicStreamSocketState::kConnected : BasicStreamSocketState::kConnectError;

    this->ReleaseLock();
    // Pass ownership of result to callback.
    callback(std::move(result));
    this->AcquireLock();
  }
}

/*!
 * \internal
 * - Dispatch to handle the event(s) on hand.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::HandleDataTransfer(osabstraction::io::reactor1::EventTypes events) noexcept {
  if (events.HasWriteEvent()) {
    if (this->ongoing_async_send_operation_) {
      this->HandleSend();
    }
  }
  if (events.HasReadEvent()) {
    // Attributes must be re-checked because the lock has been released and re-acquired in HandleSend().
    if ((connection_state_ == BasicStreamSocketState::kConnected) && this->ongoing_async_receive_operation_) {
      this->HandleReadEvent();
    }
  }
}

/*!
 * \internal
 * - Verify the precondition.
 * - Try to send the remaining data.
 * - If the event was a spurious wake-up
 *   - do nothing and return.
 * - If an error occurred
 *   - adjust the object state accordingly.
 *   - Call the callback with a result that indicates error.
 * - If some but not all data was sent
 *   - adjust the buffer state so the remaining data can be sent later.
 * - If all data was sent
 *   - Call the callback with a result that indicates success.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::HandleSend() noexcept {
  this->AssertBlockingModeIsDisabled();

  if (async_send_buffer_.GetRemainingSize() == 0) {
    this->CallSendNotificationCallback(::amsr::core::Result<void>{});
  } else {
    // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
    ::amsr::core::Array<osabstraction::io::ConstIOBuffer, kMaxBufferElements> current_send_data;
    std::size_t const remaining_parts{
        async_send_buffer_.GetMemory(::amsr::core::Span<osabstraction::io::ConstIOBuffer>{current_send_data})};

    // Lock is held during system call because
    // - this is a non-blocking call which thus does not take long.
    // - not releasing the lock after the verification that the socket is in non-blocking mode ensures that this call
    //   can not block even if the user tried to enable blocking mode with an ongoing asynchronous operation.
    static_cast<void>(os_interface::SendStream(this->GetNativeHandle(),
                                               ::amsr::core::Span<osabstraction::io::ConstIOBuffer>{
                                                   current_send_data.data(), remaining_parts})
                          .Inspect([this](std::size_t sent_bytes) {
                            async_send_buffer_.Consume(sent_bytes);
                            if (async_send_buffer_.CompletelyProcessed()) {
                              this->CallSendNotificationCallback(::amsr::core::Result<void>{});
                            }
                          })
                          .InspectError([this](::amsr::core::ErrorCode error) {
                            if (error != osabstraction::OsabErrc::kBusy) {  // Ignore spurious wakeup.
                              this->HandleOperationError(error);
                              this->CallSendNotificationCallback(::amsr::core::Result<void>{error});
                            }
                          }));
  }
}

/*!
 * \internal
 * If the user wants to be notified directly about received data, call HandleReceiveSome().
 * If the receive buffer must be completely filled before the user is notified, then call HandleReceive().
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::HandleReadEvent() noexcept {
  if (async_receive_forward_data_instantly_) {
    HandleReceiveSome();
  } else {
    HandleReceive();
  }
}

/*!
 * \internal
 * - Verify the precondition.
 * - Try to receive as much data as required.
 * - If the event was a spurious wake-up
 *   - do nothing and return.
 * - If an error occurred
 *   - adjust the object state accordingly.
 *   - Call the callback with an error result.
 * - If some data was received but this is less than required.
 *   - adjust the buffer state so the remaining required data can be received later.
 * - If enough data was received
 *   - Call the callback with a result that indicates success.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::HandleReceive() noexcept {
  this->AssertBlockingModeIsDisabled();

  if (async_receive_buffer_.GetRemainingSize() == 0) {
    this->CallReceiveNotificationCallback(::amsr::core::Result<void>{});
  } else {
    // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
    ::amsr::core::Array<osabstraction::io::MutableIOBuffer, kMaxBufferElements> current_receive_data;
    std::size_t const remaining_parts{
        async_receive_buffer_.GetMemory(::amsr::core::Span<osabstraction::io::MutableIOBuffer>{current_receive_data})};

    // Lock is held during system call because
    // - this is a non-blocking call which thus does not take long.
    // - not releasing the lock after the verification that the socket is in non-blocking mode ensures that this call
    //   can not block even if the user tried to enable blocking mode with an ongoing asynchronous operation.
    static_cast<void>(os_interface::ReceiveStream(this->GetNativeHandle(),
                                                  ::amsr::core::Span<osabstraction::io::MutableIOBuffer>{
                                                      current_receive_data.data(), remaining_parts})
                          .Inspect([this](std::size_t received_bytes) {
                            async_receive_buffer_.Consume(received_bytes);
                            if (async_receive_buffer_.CompletelyProcessed()) {
                              this->CallReceiveNotificationCallback(::amsr::core::Result<void>{});
                            } else {
                              // No error occurred but not enough data has been received yet.
                            }
                          })
                          .InspectError([this](::amsr::core::ErrorCode error) {
                            if (error != osabstraction::OsabErrc::kBusy) {  // Ignore spurious wake-up.
                              this->HandleOperationError(error);
                              this->CallReceiveNotificationCallback(::amsr::core::Result<void>{error});
                            }
                          }));
  }
}

/*!
 * \internal
 * - Verify the precondition.
 * - Try to receive as much data as required.
 * - If the event was a spurious wake-up
 *   - do nothing and return.
 * - If an error occurred
 *   - adjust the object state accordingly.
 *   - Call the callback with an error result.
 * - If some data was received but this is less than required.
 *   - adjust the buffer state so the remaining required data can be received later.
 * - If enough data was received
 *   - Call the callback with a result that indicates success.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::HandleReceiveSome() noexcept {
  this->AssertBlockingModeIsDisabled();

  if (async_receive_buffer_.GetRemainingSize() == 0) {
    this->CallReceiveSomeNotificationCallback(::amsr::core::Result<std::size_t>{0});

  } else {
    // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
    ::amsr::core::Array<osabstraction::io::MutableIOBuffer, kMaxBufferElements> current_receive_data;
    std::size_t const remaining_parts{
        async_receive_buffer_.GetMemory(::amsr::core::Span<osabstraction::io::MutableIOBuffer>{current_receive_data})};

    // Lock is held during system call because
    // - this is a non-blocking call which thus does not take long.
    // - not releasing the lock after the verification that the socket is in non-blocking mode ensures that this call
    //   can not block even if the user tried to enable blocking mode with an ongoing asynchronous operation.
    static_cast<void>(
        os_interface::ReceiveStream(
            this->GetNativeHandle(),
            ::amsr::core::Span<osabstraction::io::MutableIOBuffer>{current_receive_data.data(), remaining_parts})
            .Inspect([this](std::size_t received_bytes) {
              async_receive_buffer_.Consume(received_bytes);
              this->CallReceiveSomeNotificationCallback(::amsr::core::Result<std::size_t>{received_bytes});
            })
            .InspectError([this](::amsr::core::ErrorCode error) {
              if (error != osabstraction::OsabErrc::kBusy) {  // Ignore spurious wake-up.
                this->HandleOperationError(error);
                this->CallReceiveSomeNotificationCallback(::amsr::core::Result<std::size_t>{error});
              }
            }));
  }
}

/*!
 * \internal
 * - Store the callback in a local variable.
 * - Change the object state so an asynchronous send operation can be requested in the callback.
 * - Release the lock.
 * - Call the callback.
 * - Re-acquire the lock.
 * - If no asynchronous send operation has been requested in the callback
 *   - set the reactor so notifications about send being possible do not take place.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::CallSendNotificationCallback(::amsr::core::Result<void> operation_result) noexcept {
  // Stored in local variable on the stack to avoid overwrite in callback.
  // Take callback to local variable before resetting the send state to avoid a race condition with a concurrent
  // move (which is prohibited because the move operation of this class is not thread-safe) that could take place
  // after the state was reset but before the callback was taken to a local variable.
  SendCompletionCallback callback_buffer{std::move(this->on_send_completion_callback_)};

  this->ongoing_async_send_operation_ = false;

  this->ReleaseLock();
  // PTP-B-OsAbstraction-TCP-DataTransfer-SendCompletionCallback
  callback_buffer(std::move(operation_result));
  // PTP-E-OsAbstraction-TCP-DataTransfer-SendCompletionCallback
  this->AcquireLock();

  // As the lock was released, it must be verified if the connection is still up.
  if (IsConnected() && (!ongoing_async_send_operation_)) {
    this->ChangeWriteObservation(false);
  }
}

/*!
 * \internal
 * - Store the callback in a local variable.
 * - Change the object state so an asynchronous receive operation can be requested in the callback.
 * - Release lock.
 * - Call the callback.
 * - Re-acquire lock.
 * - If no asynchronous receive operation has been requested in the callback
 *   - set the reactor so notifications about receive being possible do not take place.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::CallReceiveNotificationCallback(
    ::amsr::core::Result<void> operation_result) noexcept {
  // Stored in local variable on the stack to avoid overwrite in callback.
  // Take callback to local variable before resetting the receive state to avoid a race condition with a concurrent
  // move (which is prohibited because the move operation of this class is not thread-safe) that could take place
  // after the state was reset but before the callback was taken to a local variable.
  ReceiveCompletionCallback callback_buffer{std::move(this->on_receive_completion_callback_)};

  this->ongoing_async_receive_operation_ = false;

  this->ReleaseLock();
  // PTP-B-OsAbstraction-TCP-DataTransfer-ReceiveCompletionCallback
  callback_buffer(std::move(operation_result));
  // PTP-E-OsAbstraction-TCP-DataTransfer-ReceiveCompletionCallback
  this->AcquireLock();

  // As the lock was released, it must be verified if the connection is still up.
  if (IsConnected() && (!ongoing_async_receive_operation_)) {
    this->ChangeReadObservation(false);
  }
}

/*!
 * \internal
 * - Store the callback in a local variable.
 * - Change the object state so an asynchronous receive operation can be requested in the callback.
 * - Release lock.
 * - Call the callback.
 * - Re-acquire lock.
 * - If no asynchronous receive operation has been requested in the callback
 *   - set the reactor so notifications about receive being possible do not take place.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::CallReceiveSomeNotificationCallback(
    ::amsr::core::Result<std::size_t> received_bytes) noexcept {
  // Stored in local variable on the stack to avoid overwrite in callback.
  // Take callback to local variable before resetting the receive state to avoid a race condition with a concurrent
  // move (which is prohibited because the move operation of this class is not thread-safe) that could take place
  // after the state was reset but before the callback was taken to a local variable.
  ReceiveSomeCompletionCallback callback_buffer{std::move(this->on_receive_some_completion_callback_)};

  this->ongoing_async_receive_operation_ = false;

  this->ReleaseLock();
  // PTP-B-OsAbstraction-TCP-DataTransfer-ReceiveSomeCompletionCallback
  callback_buffer(std::move(received_bytes));
  // PTP-E-OsAbstraction-TCP-DataTransfer-ReceiveSomeCompletionCallback
  this->AcquireLock();

  // As the lock was released, it must be verified if the connection is still up.
  if (IsConnected() && (!ongoing_async_receive_operation_)) {
    this->ChangeReadObservation(false);
  }
}

/*!
 * \internal
 * - Release the lock.
 * - Call the operating system service to send the data.
 * - Re-acquire the lock.
 * - If sending failed and the connection has not been closed concurrently while the lock was released
 *   - adjust the state of the object.
 * - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::DoSyncSend(
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};

  this->ReleaseLock();
  ::amsr::core::Result<std::size_t> const send_result{os_interface::SendStream(native_handle, io_buffers_view)};
  this->AcquireLock();

  if ((!send_result.HasValue()) && (connection_state_ == BasicStreamSocketState::kConnected)) {
    this->HandleOperationError(send_result.Error());
  }

  return send_result;
}

/*!
 * \internal
 * - Release the lock.
 * - Call the operating system service to send the data.
 * - Re-acquire the lock.
 * - If the connection was separated concurrently while the lock was released
 *   - output that the connection to the peer has been lost.
 * - If still connected
 *   - If the operating system service reported that it is busy
 *     - check if the object state still allows sending.
 *     - If the object state no longer allows sending
 *       - output the respective error.
 *     - If the object state still allows sending
 *       - set up the object to conduct the send operation asynchronously.
 *       - output that the send operation is being completed asynchronously.
 *   - If the operating system service reported an error
 *     - output that error.
 *   - If the operating system service reported that only a part of the data has been sent
 *     - check if the object state still allows sending.
 *     - If the object state no longer allows sending
 *       - output the respective error.
 *     - If the object state still allows sending
 *       - set up the object to conduct the send operation asynchronously.
 *       - output that the send operation is being completed asynchronously.
 *   - If the operating system service reported that all data has been sent
 *     - output that all data has been sent.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::DoOptimisticSend(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                                   SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<osabstraction::io::SendResult> {
  ::amsr::core::Result<osabstraction::io::SendResult> result{
      MakeErrorCode(osabstraction::OsabErrc::kDisconnected, "Optimistic send failed.")};  // unused dummy value

  osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};

  this->ReleaseLock();
  ::amsr::core::Result<std::size_t> send_result{os_interface::SendStream(native_handle, io_buffers_view)};
  this->AcquireLock();

  // Verify that the connection was not set as lost concurrently while the lock was released. A check of
  // ongoing_async_send_operation_ is not required, because it's value was verified before and a send operation can
  // only be started by an explicit request of the user. As this function is also only called in context of an
  // explicit request of the user, ongoing_async_send_operation_ can only have changed if the threadsafety
  // requirement of the BasicStreamSocket object (at each point in time at most one of the interface functions to be
  // called by the user (product private functions) of the same object can be executing) was ignored.
  if (connection_state_ == BasicStreamSocketState::kConnected) {
    bool sent_all{false};
    bool is_busy{false};

    if (send_result.HasValue()) {
      std::size_t const total_size{osabstraction::io::internal::CalculateAlloverSize(io_buffers_view)};
      if (send_result.Value() == total_size) {
        sent_all = true;
      }
    } else if (send_result.Error() == osabstraction::OsabErrc::kBusy) {
      is_busy = true;
    } else {
      // Intentionally left empty
    }

    if (sent_all) {
      result = ::amsr::core::Result<osabstraction::io::SendResult>{osabstraction::io::SendResult::kSendCompleted};
    } else if (send_result.HasValue() || is_busy) {
      std::size_t already_sent_size{0};
      if (send_result.HasValue()) {
        already_sent_size = send_result.Value();
      }
      // Checking of the state still allows sending is required because the lock had been released since the last check.
      result =
          this->CheckSendIsPossible().AndThen([this, &io_buffers_view, &already_sent_size,
                                               &callback]() -> ::amsr::core::Result<osabstraction::io::SendResult> {
            this->SetupAsyncSend(io_buffers_view, already_sent_size, std::move(callback));
            return ::amsr::core::Result<osabstraction::io::SendResult>{
                osabstraction::io::SendResult::kAsyncProcessingNecessary};
          });
    } else {
      this->HandleOperationError(send_result.Error());
      result = ::amsr::core::Result<osabstraction::io::SendResult>{send_result.Error()};
    }
  } else {
    // The connection was lost while the send operation was ongoing.
    result = ::amsr::core::Result<osabstraction::io::SendResult>{
        MakeErrorCode(osabstraction::OsabErrc::kDisconnected, "Connection to peer separated.")};
  }

  return result;
}

/*!
 * \internal
 * - Set the object state to indicate that an asynchronous send operation is in progress.
 * - Store the data and callback in the object attributes.
 * - Set up the reactor for notifications when data can be sent.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::SetupAsyncSend(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                                 std::size_t data_offset, SendCompletionCallback&& callback) noexcept {
  this->ongoing_async_send_operation_ = true;

  // Only store the callback after the state has been set to sending to avoid the callback being moved in a concurrent
  // move operation (which is anyway prohibited because the move operation of this class is not thread-safe) after it
  // was stored but before the state was set.
  this->on_send_completion_callback_ = std::move(callback);
  this->async_send_buffer_ =
      osabstraction::io::internal::IoBufferQueue<osabstraction::io::ConstIOBuffer>{io_buffers_view};
  this->async_send_buffer_.Consume(data_offset);

  this->ChangeWriteObservation(true);
}

/*!
 * \internal
 * - Release the lock.
 * - Call the operating system service to receive data.
 * - Re-acquire the lock.
 * - If receiving failed and the connection has not been closed concurrently while the lock was released
 *   - adjust the state of the object.
 * - Output the result of receiving.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::DoSyncReceive(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};

  this->ReleaseLock();
  ::amsr::core::Result<std::size_t> const receive_result{os_interface::ReceiveStream(native_handle, io_buffers_view)};
  this->AcquireLock();

  if ((!receive_result.HasValue()) && (connection_state_ == BasicStreamSocketState::kConnected)) {
    this->HandleOperationError(receive_result.Error());
  }

  return receive_result;
}

/*!
 * \internal
 * - Set the object state to indicate that an asynchronous receive operation is in progress.
 * - Set the object state to indicate that the asynchronous receive operation should not notify the user until the
 *   receive buffer capacity is exhausted.
 * - Store the buffer and callback in the object attributes.
 * - Set up the reactor for notifications when data can be received.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::SetupAsyncReceive(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
    ReceiveCompletionCallback&& callback) noexcept {
  this->ongoing_async_receive_operation_ = true;
  this->async_receive_forward_data_instantly_ = false;

  // Only store the callback after the state has been set to receiving to avoid the callback being moved in a
  // concurrent move operation (which is anyway prohibited because the move operation of this class is not
  // thread-safe) after it was stored but before the state was set.
  this->on_receive_completion_callback_ = std::move(callback);
  this->async_receive_buffer_ =
      osabstraction::io::internal::IoBufferQueue<osabstraction::io::MutableIOBuffer>{io_buffers_view};

  this->ChangeReadObservation(true);
}

/*!
 * \internal
 * - Set the object state to indicate that an asynchronous receive operation is in progress.
 * - Set the object state to indicate that the asynchronous receive operation should notify the user if any data is
 *   received.
 * - Store the buffer and callback in the object attributes.
 * - Set up the reactor for notifications when data can be received.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::SetupAsyncReceiveSome(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
    ReceiveSomeCompletionCallback&& callback) noexcept {
  this->ongoing_async_receive_operation_ = true;
  this->async_receive_forward_data_instantly_ = true;

  // Only store the callback after the state has been set to receiving to avoid the callback being moved in a
  // concurrent move operation (which is anyway prohibited because the move operation of this class is not
  // thread-safe) after it was stored but before the state was set.
  this->on_receive_some_completion_callback_ = std::move(callback);
  this->async_receive_buffer_ =
      osabstraction::io::internal::IoBufferQueue<osabstraction::io::MutableIOBuffer>{io_buffers_view};

  this->ChangeReadObservation(true);
}

/*!
 * \internal
 * - If the object is connected and the reported error is that the peer is disconnected
 *   - set the object state so sending and receiving are no longer allowed.
 *   - Deactivate notification on the reactor if required.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::HandleOperationError(::amsr::core::ErrorCode error) noexcept {
  if (IsConnected() && (error == osabstraction::OsabErrc::kDisconnected)) {
    this->connection_state_ = BasicStreamSocketState::kDormant;
    if (this->HasReactor()) {
      this->ChangeWriteObservation(false);
      this->ChangeReadObservation(false);
    }
  }
}

/*!
 * \internal
 * - Assert that no asynchronous operation has been scheduled.
 * - Get other sockets state and set other sockets state to connection error.
 * - Store stream socket context
 * - Let the base class move reactor related attributes from other to this socket.
 * - Restore stream context.
 * \endinternal
 */
template <class Protocol>
void BasicStreamSocket<Protocol>::MoveStreamSocket(BasicStreamSocket&& other) noexcept {
  bool const other_async_operation_in_progress{other.AsyncOperationInProgress()};
  // Abort on ongoing asynchronous operation. The socket is not thread safe. The behavior is undefined.
  if (this->AsyncOperationInProgress() || other_async_operation_in_progress) {
    ::amsr::core::Abort("Tried to move socket with scheduled asynchronous operation.");
  }

  BasicStreamSocketState const others_state{other.connection_state_};
  other.connection_state_ = BasicStreamSocketState::kConnectError;

  this->is_socket_bound_to_endpoint_ = other.is_socket_bound_to_endpoint_;

  // This socket will be closed during the base class move.
  this->MoveSocket(std::move(other));

  this->connection_state_ = others_state;

  // As the precondition states that no asynchronous operation is ongoing, the flags can be set, no copying
  // required.
  this->ongoing_async_send_operation_ = false;
  this->ongoing_async_receive_operation_ = false;
  this->async_receive_forward_data_instantly_ = false;

  // As the precondition states that no asynchronous operation is ongoing and the function is not threadsafe, the
  // mutex members of both objects are not locked so no action to transfer the state of the mutex member is
  // required.

  // No need to move additional attributes. They are only valid during an ongoing asynchronous operation.
}

/*!
 * \internal
 * - Check the following conditions. If all are met, return a value, an error otherwise:
 *   - Socket is open
 *   - The state of the connection allows sending.
 *   - There is no asynchronous send operation in progress
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::CheckSendIsPossible() const noexcept -> ::amsr::core::Result<void> {
  ::amsr::core::Result<void> result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Connection state does not allow sending.")};

  if (this->connection_state_ == BasicStreamSocketState::kConnected) {
    // connection_state_ == BasicStreamSocketState::kConnected means that the socket is open.

    if (this->ongoing_async_send_operation_) {
      result = ::amsr::core::Result<void>{
          MakeErrorCode(osabstraction::OsabErrc::kApiError, "There is an ongoing asynchronous send request")};
    } else {
      result = ::amsr::core::Result<void>{};
    }
  } else if (connection_state_ == BasicStreamSocketState::kDormant) {
    // This is not an API error because the state may only have changed to dormant in the reactor context very
    // recently and the user was not yet notified about this.
    result = ::amsr::core::Result<void>{
        MakeErrorCode(osabstraction::OsabErrc::kDisconnected, "Connection to peer has been separated.")};
  } else {
    // Use default value.
  }

  return result;
}

/*!
 * \internal
 * - Check the following conditions. If all are met, return a value, an error otherwise:
 *   - Socket is open
 *   - The state of the connection allows receiving.
 *   - There is no asynchronous receive operation in progress
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::CheckReceiveIsPossible() const noexcept -> ::amsr::core::Result<void> {
  ::amsr::core::Result<void> result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Connection state does not allow receiving.")};

  if (this->connection_state_ == BasicStreamSocketState::kConnected) {
    // connection_state_ == BasicStreamSocketState::kConnected means that the socket is open.

    if (this->ongoing_async_receive_operation_) {
      result = ::amsr::core::Result<void>{
          MakeErrorCode(osabstraction::OsabErrc::kApiError, "There is an ongoing asynchronous receive request")};
    } else {
      result = ::amsr::core::Result<void>{};
    }
  } else if (connection_state_ == BasicStreamSocketState::kDormant) {
    // This is not an API error because the state may only have changed to dormant in the reactor context very
    // recently and the user was not yet notified about this.
    result = ::amsr::core::Result<void>{
        MakeErrorCode(osabstraction::OsabErrc::kDisconnected, "Connection to peer has been separated.")};
  } else {
    // Use default value.
  }

  return result;
}

/*!
 * \internal
 * - If the socket is disconnected
 *   - return a value.
 * - If the socket is connected
 *   - return an error.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::CheckSocketIsDisconnected() const noexcept -> ::amsr::core::Result<void> {
  return (this->connection_state_ == BasicStreamSocketState::kDisconnected)
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                                        "Connect() has already been called on this socket")};
}

/*!
 * \internal
 * - If the socket is not bound to an endpoint
 *   - return a value.
 * - If the socket is bound to an endpoint:
 *   - return an error.
 * \endinternal
 */
template <class Protocol>
auto BasicStreamSocket<Protocol>::CheckSocketIsNotBoundToEndpoint() const noexcept -> ::amsr::core::Result<void> {
  return !this->is_socket_bound_to_endpoint_ ? ::amsr::core::Result<void>{}
                                             : ::amsr::core::Result<void>{MakeErrorCode(
                                                   osabstraction::OsabErrc::kApiError, "Socket is bound to endpoint")};
}

/*!
 * \exclusivearea ::amsr::net::internal::BasicSocket::lock_::mutex_
 * Ensures atomicity when jointly accessing object state and reactor event settings for asynchronous operations.
 * \protects ::amsr::net::internal::BasicSocket::reactor_,
 *           ::amsr::net::internal::BasicSocket::socket_state_,
 *           ::amsr::net::internal::BasicSocket::cached_blocking_mode_,
 *           ::amsr::net::internal::BasicStreamSocket::connection_state_,
 *           ::amsr::net::internal::BasicStreamSocket::outgoing_single_buffer_io_vector_,
 *           ::amsr::net::internal::BasicStreamSocket::incoming_single_buffer_io_vector_,
 *           ::amsr::net::internal::BasicStreamSocket::on_connect_completion_callback_,
 *           ::amsr::net::internal::BasicStreamSocket::on_send_completion_callback_,
 *           ::amsr::net::internal::BasicStreamSocket::on_receive_completion_callback_,
 *           ::amsr::net::internal::BasicStreamSocket::async_send_buffer_,
 *           ::amsr::net::internal::BasicStreamSocket::async_receive_buffer_,
 *           ::amsr::net::internal::BasicStreamSocket::ongoing_async_send_operation_,
 *           ::amsr::net::internal::BasicStreamSocket::ongoing_async_receive_operation_,
 *           ::amsr::net::internal::BasicStreamSocket::async_receive_forward_data_instantly_,
 *           ::amsr::net::internal::BasicStreamSocket::is_socket_bound_to_endpoint_
 * \usedin   ::amsr::net::internal::BasicStreamSocket::Bind,
 *           ::amsr::net::internal::BasicStreamSocket::Connect,
 *           ::amsr::net::internal::BasicStreamSocket::ConnectAsync,
 *           ::amsr::net::internal::BasicStreamSocket::SendSync,
 *           ::amsr::net::internal::BasicStreamSocket::SendAsync,
 *           ::amsr::net::internal::BasicStreamSocket::Send,
 *           ::amsr::net::internal::BasicStreamSocket::ReceiveSync,
 *           ::amsr::net::internal::BasicStreamSocket::ReceiveAsync,
 *           ::amsr::net::internal::BasicStreamSocket::GetRemoteEndpoint,
 *           ::amsr::net::internal::BasicStreamSocket::OnReactorEvents,
 *           ::amsr::net::internal::BasicStreamSocket::OnCloseEvent,
 *           ::amsr::net::internal::BasicStreamSocket::IsSocketBound,
 *           ::amsr::net::internal::BasicStreamSocket::HandleConnectionEstablishment,
 *           ::amsr::net::internal::BasicStreamSocket::HandleDataTransfer,
 *           ::amsr::net::internal::BasicStreamSocket::HandleSend,
 *           ::amsr::net::internal::BasicStreamSocket::HandleReceive,
 *           ::amsr::net::internal::BasicStreamSocket::IsConnected,
 *           ::amsr::net::internal::BasicStreamSocket::CallSendNotificationCallback,
 *           ::amsr::net::internal::BasicStreamSocket::CallReceiveNotificationCallback,
 *           ::amsr::net::internal::BasicStreamSocket::DoSyncSend,
 *           ::amsr::net::internal::BasicStreamSocket::DoOptimisticSend,
 *           ::amsr::net::internal::BasicStreamSocket::SetupAsyncSend,
 *           ::amsr::net::internal::BasicStreamSocket::DoSyncReceive,
 *           ::amsr::net::internal::BasicStreamSocket::SetupAsyncReceive,
 *           ::amsr::net::internal::BasicStreamSocket::HandleOperationError,
 *           ::amsr::net::internal::BasicStreamSocket::CheckSendIsPossible,
 *           ::amsr::net::internal::BasicStreamSocket::CheckReceiveIsPossible,
 *           ::amsr::net::internal::BasicStreamSocket::CheckSocketIsDisconnected,
 *           ::amsr::net::internal::BasicStreamSocket::CheckSocketIsNotBoundToEndpoint,
 *           ::amsr::net::internal::BasicStreamSocket::AssignHandleAndConnectSocket,
 *           ::amsr::net::internal::BasicStreamSocket::ReceiveAsyncSome,
 *           ::amsr::net::internal::BasicStreamSocket::HandleReadEvent,
 *           ::amsr::net::internal::BasicStreamSocket::HandleReceiveSome,
 *           ::amsr::net::internal::BasicStreamSocket::CallReceiveSomeNotificationCallback,
 *           ::amsr::net::internal::BasicStreamSocket::SetupAsyncReceiveSome
 * \exclude  All other of these methods of the same object.
 * \length   LONG Enabling and disabling events on the reactor.
 *
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_STREAM_SOCKET_H_

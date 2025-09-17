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
 *        \brief  Interface to operating system calls.
 *        \unit   osabstraction::net
 *
 *      \details  The actual implementation of the services may depend on the operating system. This file provide a
 *                unified interface to call a net related operating system service.
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_H_

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/net/interface.h"
#include "amsr/net/internal/os_constants.h"
#include "amsr/net/internal/types.h"
#include "amsr/net_utils/network_stack_id.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/native_types.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace net {
namespace internal {
namespace os_interface {

/*!
 * \brief Function pointer type for GetSocketName / GetPeerName
 */
using OsInterfaceGetEndpointFunction = ::amsr::core::Result<void> (*)(osabstraction::io::NativeHandle,
                                                                      SocketMutableAddrView);

/*!
 * \brief Creates a native handle for a socket.
 *
 * \details
 * On most operating system this service is mapped to the POSIX socket() call.
 *
 * \param[in] family      Socket family (e.g. AF_INET,...)
 * \param[in] type        Socket type   (e.g. SOCK_DGRAM,...)
 * \param[in] protocol    Socket protocol (0 for most address families).
 * \param[in] netstack_id ID of the network stack to open the socket with. Only used on QNX.
 *
 * \return Native handle to a new socket.
 *
 * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to create a
 *                                                            socket.
 * \error osabstraction::OsabErrc::kResource                  Not enough system resources to create a socket.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_Open
 */
::amsr::core::Result<osabstraction::io::NativeHandle> CreateSocket(
    SocketFamily family, SocketType type, SocketProtocol protocol,
    net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief Closes the native handle.
 *
 * \param[in] handle The open native handle.
 *
 * \details Close does not check if the last write operation was successful or not.
 *
 * \context         ANY
 *
 * \pre             Valid native handle.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_Close
 */
void Close(osabstraction::io::NativeHandle handle) noexcept;

/*!
 * \brief           Returns the ephemeral ports range defined by operating system.
 * \details         As no ports from ephemeral ports range should be configured
 *                  by user and should only used by OS, user might need to check this range
 *                  first before configuring ports for applications
 *
 * \param[in]       netstack_id   Network stack to query for the ephemeral ports range.
 *
 * \return          The ephemeral ports range defined by Operating System.
 *
 * \error osabstraction::OsabErrc::kSystemEnvironmentError     Error while reading the values from OS.
 * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 *
 *
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \trace           DSGN-Osab-EphemeralPortsRangeCheck
 * \vprivate        Vector component internal API
 */
::amsr::core::Result<net::internal::EphemeralPortsRange> GetEphemeralPortsRange(
    net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief Binds a socket address to the native handle.
 *
 * \details
 * If port number is 0, ephemeral port is assigned by OS.
 *
 * \param[in] handle   The native handle.
 * \param[in] address  View to a socket address object.
 *
 * \return Result of operation.
 *
 * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to bind address.
 * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
 * \error osabstraction::OsabErrc::kAddressError              The address is already in use on the local machine.
 *
 * \context         ANY
 *
 * \pre             Valid native handle and valid socket address view.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_Bind
 */
::amsr::core::Result<void> Bind(osabstraction::io::NativeHandle handle, SocketConstAddrView address) noexcept;

/*!
 * \brief Sets native handle's blocking mode.
 *
 * \details
 * Sets the file descriptor to the passed blocking state.
 *
 * \param[in] handle The native handle
 * \param[in] enable The blocking mode to be set. True to enable blocking and false to turn blocking off.
 *
 * \context         ANY
 *
 * \pre             Valid native handle.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_BlockingMode
 */
void SetBlockingMode(osabstraction::io::NativeHandle handle, SocketBlockingMode enable) noexcept;

/*!
 * \brief Acquires the address a socket is bound to.
 *
 * \param[in]     handle The native handle of the socket.
 * \param[in,out] address The buffer to write the address to, size_ must contain the size of the buffer in bytes.
 *
 * \return The result of the service.
 *
 * \error osabstraction::OsabErrc::kResource           Not enough system resources to acquire the address.
 * \error osabstraction::OsabErrc::kUnexpected         Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             Valid native handle, accessible buffer, positive buffer size.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> GetSocketName(osabstraction::io::NativeHandle handle,
                                         SocketMutableAddrView address) noexcept;

/*!
 * \brief Acquires the remote address a socket is bound to.
 *
 * \param[in]     handle The native handle of the socket.
 * \param[in,out] address The buffer to write the remote address to, size_ must contain the size of the buffer in bytes.
 *
 * \return The result of the service.
 *
 * \error osabstraction::OsabErrc::kResource           Not enough system resources to acquire the address.
 * \error osabstraction::OsabErrc::kDisconnected       The socket is not connected.
 * \error osabstraction::OsabErrc::kUnexpected         Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             Valid native handle, accessible buffer, positive buffer size.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> GetPeerName(osabstraction::io::NativeHandle handle, SocketMutableAddrView address) noexcept;

/*!
 * \brief Sets the socket option for a native handle.
 *
 * \param[in] handle      The native handle
 * \param[in] level       The socket option level
 * \param[in] name        The socket option name
 * \param[in] value_view  View to socket option value to be set.
 *
 * \return Result of operation
 *
 * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the passed option.
 * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
 * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges    Insufficient privileges to set socket option.
 *
 * \context         ANY
 *
 * \pre             Valid native handle.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_SetOption
 */
::amsr::core::Result<void> SetSocketOption(osabstraction::io::NativeHandle handle, SockOptLevel level, SockOptName name,
                                           SockOptConstValueView value_view) noexcept;

/*!
 * \brief Gets the socket option for a native handle.
 *
 * \param[in] handle      The native handle
 * \param[in] level       The socket option level
 * \param[in] name        The socket option name
 * \param[in] value_view  View to socket option value to be set.
 *
 * \return Result of operation
 *
 * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the requested option.
 * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             Valid native handle.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> GetSocketOption(osabstraction::io::NativeHandle handle, SockOptLevel level, SockOptName name,
                                           SockOptMutableValueView value_view) noexcept;

/*!
 * \brief Translates a string to an IPv4 address.
 *
 * \param[in]    addr_string The string representation of the IPv4 address.
 *
 * \return The IPv4 address.
 *
 * \error osabstraction::OsabErrc::kApiError     The string does not hold the representation of an IPv4 address. Strings
 *                                               of a length greater than kMaxIpv4AddressStringSize are always
 *                                               considered to not hold a representation of an IPv4 address.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE if the input is thread-safe
 *
 * \trace           DSGN-Osab-AddressCreation
 */
::amsr::core::Result<Ipv4Native> ParseIpV4(::amsr::core::StringView addr_string) noexcept;

/*!
 * \brief Translates a string to an IPv6 address.
 *
 * \param[in]    addr_string The string representation of the IPv6 address.
 *
 * \return The IPv6 address.
 *
 * \error osabstraction::OsabErrc::kApiError     The string does not hold the representation of an IPv6 address. Strings
 *                                               of a length greater than kMaxIpv6AddressStringSize are always
 *                                               considered to not hold a representation of an IPv6 address.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE if the input is thread-safe
 *
 * \trace           DSGN-Osab-AddressCreation
 */
::amsr::core::Result<Ipv6Native> ParseIpV6(::amsr::core::StringView addr_string) noexcept;

/*!
 * \brief Translates a string to an Ethernet address.
 *
 * \param[in]    addr_string The string representation of the Ethernet address.
 *
 * \return The Ethernet address.
 *
 * \error osabstraction::OsabErrc::kApiError     The string does not hold the representation of an Ethernet address.
 *                                               Strings of a length other than kMaxEtherAddressString are always
 *                                               considered to not hold a representation of an Ethernet address.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE if the input is thread-safe
 *
 * \trace           DSGN-Osab-AddressCreation
 */
::amsr::core::Result<EthNative> ParseEther(::amsr::core::StringView addr_string) noexcept;

/*!
 * \brief Translates an IPv4 address to a string.
 *
 * \param[in]    addr The IPv4 address.
 * \param[out]   buffer Buffer to write the \0-terminated address string into.
 *
 * \return Number of characters the address string consists of (terminating \0 character not counted).
 *
 * \context         ANY
 *
 * \pre             buffer must be able to hold at least kMaxIpv4AddressStringSize + 1 characters
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
::amsr::core::Span<char>::size_type MakeIpV4String(Ipv4Native addr, ::amsr::core::Span<char> buffer) noexcept;

/*!
 * \brief Translates an IPv6 address to a string.
 *
 * \param[in]    addr The IPv6 address.
 * \param[out]   buffer Buffer to write the \0-terminated address string into.
 *
 * \return Number of characters the address string consists of (terminating \0 character not counted).
 *
 * \context         ANY
 *
 * \pre             buffer must be able to hold at least kMaxIpv6AddressStringSize + 1 characters
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
::amsr::core::Span<char>::size_type MakeIpV6String(Ipv6Native addr, ::amsr::core::Span<char> buffer) noexcept;

/*!
 * \brief Translates an Ethernet address to a string.
 *
 * \param[in]    addr The Ethernet address.
 * \param[out]   buffer The buffer to write the string to.
 *
 * \context         ANY
 *
 * \pre             buffer must be able to hold at least kMaxEtherAddressString characters
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
void MakeEtherString(EthNative addr, ::amsr::core::Span<char> buffer) noexcept;

/*!
 * \brief Connects a socket to a remote endpoint.
 *
 * \param[in] handle   The native handle.
 * \param[in] address  The peers address.
 *
 * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to connect to
 *                                                            address.
 * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kDisconnected              The connection has been terminated by peer or the peer is
 *                                                            not reachable or the route to the peer is configured as
 *                                                            blackhole in the routing table.
 * \error osabstraction::OsabErrc::kResource                  No ephemeral ports or routing cache entries available.
 * \error osabstraction::OsabErrc::kAddressNotAvailable       Cannot establish connection because local endpoint is
 *                                                            already used by another socket.
 * \error osabstraction::OsabErrc::kBusy                      Connection cannot be completed directly. This is a valid
 *                                                            error for non blocking sockets.
 *
 * \context         ANY
 *
 * \pre             Valid native handle that identifies a stream socket and valid socket address view.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> Connect(osabstraction::io::NativeHandle handle, SocketConstAddrView address) noexcept;

/*!
 * \brief Connects a socket to a remote endpoint.
 *
 * \param[in] handle   The native handle.
 * \param[in] address  The peers address.
 *
 * \error osabstraction::OsabErrc::kInsufficientPrivileges     The process has insufficient privileges to set the
 *                                                             address as default destination.
 * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kDisconnected               The the peer is not reachable or the route to the peer is
 *                                                             configured as blackhole in the routing table.
 * \error osabstraction::OsabErrc::kResource                   No ephemeral ports or buffer space or entries in
 *                                                             routing cache available.
 * \error osabstraction::OsabErrc::kAddressNotAvailable        Cannot set default destination because local endpoint is
 *                                                             already used by another socket.
 *
 * \context         ANY
 *
 * \pre             Valid native handle that identifies a datagram socket and valid socket address view.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> SetDefaultPeer(osabstraction::io::NativeHandle handle, SocketConstAddrView address) noexcept;

/*!
 * \brief Completes a non blocking connect call.
 *
 * \details
 * This function takes the error number of a non blocking connect call and evaluates the result. The error codes
 * is usually returned by a getsockopts() operation.
 *
 * \param[in] error    The result of the operating system returned by a preceding getsockopt() call with parameter
 *                     SockOptError
 *
 * \return Result of operation.
 *
 * \error osabstraction::OsabErrc::kUnexpected           Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kDisconnected         The connection has been terminated by peer or the peer is not
 *                                                       reachable.
 * \error osabstraction::OsabErrc::kBusy                 Connection has not completed yet. Try again later.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> CompleteNonBlockingConnect(osabstraction::internal::OsErrorNumber error) noexcept;

/*!
 * \brief Sets native handle into passive mode.
 *
 * \details
 * The native handle will listen for incoming connection requests. A listening socket will request in a backlog queue
 * till they are accepted via the accept call.
 *
 * \param[in] handle   Valid native handle to listen on.
 * \param[in] backlog  Maximum number of queued socket requests.
 *
 * \return Result of operation.
 *
 * \error osabstraction::OsabErrc::kUnexpected           Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kAddressNotAvailable  Cannot establish connection because local endpoint is already
 *                                                        bound by another socket or the operating system used up all
 *                                                        ephemeral ports.
 *
 * \context         ANY
 *
 * \pre             Valid native handle.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> Listen(osabstraction::io::NativeHandle handle, ListenBacklog backlog) noexcept;

/*!
 * \brief Accepts one pending connection request.
 *
 * \details
 * Accepts the connection request and returns a native handle that can be used for the communication. The peer's
 * endpoint address will be stored at the memory location that is specified in the passed view.
 *
 * \param[in]       handle    Native handle in passive mode.
 * \param[in, out]  endpoint  View to an socket address object that is sufficiently large to store the address for this
 *                            address family.
 *
 * \return Native handle to an accepted connection.
 *
 * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kResource                  Not enough system resources to accept connection.
 * \error osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError    Not allowed to accept connection.
 *
 * \context         ANY
 *
 * \pre             Valid native handle that identifies a socket that is listening.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-Linux_AvoidSocketCloseRaceCondition
 */
::amsr::core::Result<osabstraction::io::NativeHandle> Accept(osabstraction::io::NativeHandle handle,
                                                             SocketMutableAddrView endpoint) noexcept;

/*!
 * \brief Sends data over a socket that is connected.
 *
 * \details
 * May not send all supplied data in one call, not sent data can be retried later.
 *
 * \param[in]       handle              Native handle of datagram socket.
 * \param[in]       io_buffers_view     Data to send. The io_buffers_view has to exist
 *                                      for the whole lifetime of the class object.
 *
 * \return Result of operation.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to send datagram.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to send datagram.
 * \error  osabstraction::OsabErrc::kSystemEnvironmentError     The datagram exceeds the maximum allowed size (e.g.
 *                                                              message size > MTU size) or a kernel subsystem is not
 *                                                              configured.
 * \error  osabstraction::OsabErrc::kDisconnected               No route to destination exists. Note that this may be an
 *                                                              asynchronous network error that has been caused by a
 *                                                              previous datagram.
 *
 * \context         ANY
 *
 * \pre             handle identifies an open and connected datagram socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> SendDatagram(
    osabstraction::io::NativeHandle handle,
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> io_buffers_view) noexcept;

/*!
 * \brief Sends data over a stream socket.
 *
 * \details
 * May not send all supplied data in one call, not sent data can be retried later.
 *
 * \param[in]       handle              Native handle of stream socket.
 * \param[in]       io_buffers_view     Data to send. The io_buffers_view has to exist for
 *                                      the whole lifetime of the class object.
 *
 * \return Number of bytes that have actually been sent. 0 if the buffer size is 0.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to conduct operation.
 * \error  osabstraction::OsabErrc::kDisconnected               The connection has been terminated by peer or the peer
 *                                                              is no longer reachable (this includes but is not limited
 *                                                              to that the route to the peer has been configured as
 *                                                              blackhole in the routing table after establishing the
 *                                                              connection).
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Permission to access socket denied.
 *
 * \context         ANY
 *
 * \pre             handle identifies an open and connected stream socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> SendStream(
    osabstraction::io::NativeHandle handle,
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> io_buffers_view) noexcept;

/*!
 * \brief Sends a datagram to the passed address.
 *
 * \details
 * This function sends a datagram via the passed native handle to the destination.
 *
 * \param[in]       handle            Valid native handle.
 * \param[in]       io_buffers_view   View to the datagram buffer to be send. The io_buffers_view has to exist
 *                                    for the whole lifetime of the class object.
 * \param[in]       destination       View to a valid address.
 *
 * \return Result of operation.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to send datagram.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to send datagram.
 * \error  osabstraction::OsabErrc::kSystemEnvironmentError     The datagram exceeds the maximum allowed size (e.g.
 *                                                              message size > MTU size) or a kernel subsystem is not
 *                                                              configured.
 * \error  osabstraction::OsabErrc::kDisconnected               No route to destination exists or is configured as
 *                                                              blackhole in the routing table. Note that this may be an
 *                                                              asynchronous network error that has been caused by a
 *                                                              previous datagram.
 *
 * \context         ANY
 *
 * \pre             handle identifies an open datagram socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> SendTo(osabstraction::io::NativeHandle handle,
                                  ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> io_buffers_view,
                                  SocketConstAddrView destination) noexcept;

/*!
 * \brief Sends a bulk of datagrams.
 *
 * \details
 * This function sends a bulk of datagram to the passed native handle.
 *
 * \param[in]  handle                   Valid native handle.
 * \param[in]  messages_view            View to the buffer with datagrams to send.
 *
 * \return  Number of datagrams that have been sent. 0 if no datagrams have been sent, always greater than 0 if one or
 *          multiple datagrams have been sent.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to send datagram.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to send datagram.
 * \error  osabstraction::OsabErrc::kSystemEnvironmentError     One of the datagrams exceeds the maximum allowed size
 *                                                              (e.g. message size > MTU size) or a kernel subsystem is
 *                                                              not configured.
 * \error  osabstraction::OsabErrc::kDisconnected               No route to destination exists or is configured as
 *                                                              blackhole in the routing table. Note that this may be an
 *                                                              asynchronous network error that has been caused by a
 *                                                              previous datagram.
 *
 * \context         ANY
 *
 * \pre             Valid native handle
 * \pre             The operating system socket object identified by the native handle is a datagram socket object.
 * \pre             The bulk data buffer contains datagrams within the specified limit (kMaxDatagramsBulkSnd).
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Send
 */
::amsr::core::Result<std::size_t> SendToBulk(osabstraction::io::NativeHandle handle,
                                             ::amsr::core::Span<BulkBufferType> messages_view) noexcept;

/*!
 * \brief Receives data over a stream socket.
 *
 * \details
 * May receive less data than the buffer could hold, receiving more data can be retried later.
 *
 * \param[in]       handle    Native handle of stream socket.
 * \param[in, out]  buffer    Buffer to write received data to.
 *
 * \return Number of bytes that have been received. 0 if the buffer size is 0, always greater than 0 if the buffer size
 *         is larger than 0.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to conduct operation.
 * \error  osabstraction::OsabErrc::kDisconnected               The connection has been terminated by peer or the peer
 *                                                              is no longer reachable or an end of stream was detected.
 *
 * \context         ANY
 *
 * \pre             handle identifies an open and connected stream socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> ReceiveStream(osabstraction::io::NativeHandle handle,
                                                ::amsr::core::Span<osabstraction::io::MutableIOBuffer> buffer) noexcept;

/*!
 * \brief Receives a datagram.
 *
 * \details
 * This function receives a datagram from the passed native handle.
 *
 * \param[in]       handle            Valid native handle.
 * \param[in, out]  io_buffers_view   View to the buffer to store the incoming datagram. The io_buffers_view has to
 *                                    exist for the whole lifetime of the class object.
 * \param[in, out]  remote_endpoint   View to an address object. The address object's family must match handle's address
 *                                    family.
 *
 * \return Number of bytes that have been received. 0 if the buffer size is 0, always greater than 0 if the buffer size
 *         is larger than 0.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
 * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
 *                                                              previous datagram.
 * \error  osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
 *                                                              received datagram. The trailing bytes of the datagram
 *                                                              have been dropped.
 *
 * \context         ANY
 *
 * \pre             handle identifies an open datagram socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> ReceiveFrom(osabstraction::io::NativeHandle handle,
                                              ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                                              SocketMutableAddrView remote_endpoint) noexcept;

/*!
 * \brief Receives a bulk of datagrams.
 *
 * \details
 * This function receives a bulk of datagram from the passed native handle.
 *
 * \param[in]       handle                   Valid native handle.
 * \param[in]       is_blocking_mode_enabled true if socket is in blocking mode, false otherwise.
 * \param[in, out]  message_buffers_view     View to the buffer to store the incoming datagrams.
 *
 * \return  Number of datagrams that have been received. 0 if no datagrams have been received, always greater than 0 if
 *          the multiple datagrams have been received.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
 * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
 *                                                              previous datagram.
 * \error  osabstraction::OsabErrc::kSystemEnvironmentError     The kernel subsystem is not configured.
 *
 * \context         ANY
 *
 * \pre             Valid native handle
 * \pre             The operating system socket object identified by the native handle is a datagram socket object.
 * \pre             The bulk data buffer can be written to and is within specified limits.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Receive
 */
::amsr::core::Result<std::size_t> ReceiveFromBulk(osabstraction::io::NativeHandle handle, bool is_blocking_mode_enabled,
                                                  ::amsr::core::Span<BulkBufferType> message_buffers_view) noexcept;

/*!
 * \brief           Setup buffer to store received datagrams.
 *
 * \details         Sets up message_view to store received datagrams' contents in io_buffer.
 *                  Sets up message_view to return the source address of the datagram in case the socket is unconnected.
 *
 * \param[in, out]  message_view        Message buffer.
 * \param[in]       io_buffer           Buffer to store the incoming datagram.
 * \param[in]       endpoint_info       View to an address object. The address object's family must match handle's
 *                                      address family. If the pointer is not null, the source address of the packet
 *                                      will be saved there.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Receive
 */
void SetupBulkBuffer(BulkBufferType& message_view, osabstraction::io::MutableIOBuffer& io_buffer,
                     ::amsr::core::Optional<SocketMutableAddrView> endpoint_info) noexcept;

/*!
 * \brief           Setup buffer to store sent datagrams.
 *
 * \details         Sets up message_view to point to to-be-sent datagrams' contents in io_buffer.
 *                  Sets up message_view to return the destination address of the datagram in case the socket is
 *                  unconnected.
 *
 * \param[in, out]  message_view        Message buffer.
 * \param[in]       io_buffer           Buffer of the outgoing datagrams. Must consist of at most kMaxDatagramsBulkSnd
 *                                      datagrams.
 * \param[in]       endpoint_info       View to an address object. The address object's family must match handle's
 *                                      address family.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Send
 */
void SetupBulkBuffer(BulkBufferType& message_view, ::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffer,
                     ::amsr::core::Optional<SocketMutableAddrView> endpoint_info) noexcept;

/*!
 * \brief           Get received datagram size from message_view.
 *
 * \param[in]       message_view        Message structure.
 *
 * \return          Datagram size.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Receive
 */
std::size_t GetDatagramSize(BulkBufferType message_view) noexcept;

/*!
 * \brief Peeks a datagram.
 *
 * \details
 * This function copies a datagram from the received buffer of passed native handle without removing the data from
 * the receive buffer.
 *
 * \param[in]       handle            Valid native handle.
 * \param[in, out]  io_buffers_view   View to the buffer to store the peeked datagram. The io_buffers_view has to exist
 *                                    for the whole lifetime of the class object.
 *
 * \return Number of bytes that have been copied to the receive buffer. 0 if the buffer size is 0, always greater than 0
 *         if the buffer size is larger than 0.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to peek datagram.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to peek datagram.
 * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
 *                                                              previous datagram.
 *
 * \context         ANY
 *
 * \pre             handle identifies an open and connected datagram socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> PeekDatagram(
    osabstraction::io::NativeHandle handle,
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept;

/*!
 * \brief Receives a datagram and ancillary data.
 *
 * \details
 * This function receives a datagram from the passed native handle. In addition it also checks for ancillary data that
 * has been received together with the datagram. There may be multiple ancillary data packages.
 *
 * \param[in]       handle                Valid native handle.
 * \param[in, out]  io_buffers_view       View to the buffer to store the incoming datagram. The io_buffers_view has to
 *                                        exist for the whole lifetime of the class object.
 * \param[in, out]  remote_endpoint       View to an address object. The address object's family must match handle's
 *                                        address family.
 * \param[in, out]  ancillary_data_view   A view to ancillary data requests. The ancillary data will be written to the
 *                                        provided data view if there are data for the requested socket level and
 *                                        protocol. The used_bytes field will be updated on any write operation.
 *
 * \return Number of bytes that have been received. 0 if the buffer size is 0, always greater than 0 if the buffer size
 *         is larger than 0.
 *
 * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
 * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
 * \error osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
 *                                                             previous datagram.
 * \error osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
 *                                                             received datagram. The trailing bytes of the datagram
 *                                                             have been dropped.
 *
 * \context         ANY
 *
 * \pre             handle identifies an open datagram socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> ReceiveFrom(osabstraction::io::NativeHandle handle,
                                              ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                                              SocketMutableAddrView remote_endpoint,
                                              ::amsr::core::Span<AncillaryDataRequest> ancillary_data_view) noexcept;

/*!
 * \brief Execute an ioctl system call.
 *
 * \details
 * Executes the ioctl command on passed native handle.
 *
 * \param[in]       handle            Valid native handle.
 * \param[in]       command           Numeric representation of the ioctl command.
 * \param[in, out]  data_view         View to the mutable data that is associated to this command.
 *
 * \return Result of operation.
 *
 * \error osabstraction::OsabErrc::kUnexpected                  Unexpected error that is mapped to no category. The
 *                                                              error may be ioctl command specific.
 * \error osabstraction::OsabErrc::kResource                    Not enough system resources to perform operation.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError      Native handle is not associated  with character-special
 *                                                              device or device does not support the operation.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges      (Linux Only) Not allowed to perform the operation.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> CallIoctl(osabstraction::io::NativeHandle handle, IoctlCmdName command,
                                     IoctlMutableValueView data_view) noexcept;

/*!
 * \brief Sets up a socket for sending limited broadcasts.
 *
 * \param[in]       handle            Native handle of the IPv4 datagram socket to be set up for broadcasting.
 * \param[in]       interface         The network interface on which the broadcasts shall be sent.
 *
 * \return The broadcast IPv4 address. In Linux and QNX(io-pkt) it returns limited broadcast address (255.255.255.255)
 *         In QNX(io-sock), it returns directed broadcast address.
 *
 * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the passed option.
 * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
 * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges    Insufficient privileges to prepare broadcast.
 * \error osabstraction::OsabErrc::kResource                  Not enough system resources to prepare broadcast.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<ip::AddressV4> PrepareBroadcast(osabstraction::io::NativeHandle handle,
                                                     Interface const& interface) noexcept;

/*!
 * \brief Acquires a list of socket address information.
 *
 * \details
 * After the list has been processed, the object output by a call to this function must be passed into
 * FreeAddressesInfo, the list the object encapsulates must not be accessed afterwards.
 *
 * \param[in]    host_name     Host name to be resolved.
 * \param[in]    service_name  Service name used in host name resolution.
 * \param[in]    hints         Hints about the type of socket that are supported by the system.
 * \param[in]    netstack_id   Network stack to query for the address info.
 *
 * \return The list of socket addresses.
 *
 * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
 * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
 * \error osabstraction::OsabErrc::kDoesNotExist               The host name does not exist.
 *
 * \context         ANY
 *
 * \pre             The input strings have to be null-terminated.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-DNS_Resolution
 */
::amsr::core::Result<AddrInfoView> GetAddressesInfo(vac::container::CStringView host_name,
                                                    vac::container::CStringView service_name, AddrInfoView hints,
                                                    net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief Deallocates a list of socket address information.
 *
 * \details
 * The list encapsulated by the input object must not be accessed after this function has been called.
 *
 * \param[in]    address_list The list that shall be deallocated.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-DNS_Resolution
 */
void FreeAddressesInfo(AddrInfoView address_list) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_H_

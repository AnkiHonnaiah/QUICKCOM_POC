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
/*!        \file  basic_datagram_socket.h
 *        \brief  Template implementation for datagram based communication.
 *        \unit   osabstraction::net
 *
 *      \details  Contains class declarations/definitions for the BasicDatagramSocket class.
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_DATAGRAM_SOCKET_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_DATAGRAM_SOCKET_H_

#include <iterator>
#include <utility>

#include "amsr/core/array.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/net/interface.h"
#include "amsr/net/internal/basic_socket.h"
#include "amsr/net/internal/os_interface.h"
#include "amsr/net/internal/specific_types.h"
#include "amsr/net/internal/types.h"
#include "amsr/net/ip/address_utils.h"
#include "amsr/net/ip/internal/endpoint.h"
#include "vac/language/unique_function.h"

#include "amsr/core/abort.h"
#include "amsr/core/span.h"
#include "amsr/net/internal/os_specific_constants.h"
#include "osabstraction/io/common_types.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Internal connection (association) state.
 */
enum class BasicDatagramSocketState : std::uint32_t {
  /*!
   * \brief The socket is not connected, i.e. a default remote endpoint has not been associated.
   */
  kDisconnected = 0,

  /*!
   * \brief The socket is connected, i.e. a default remote endpoint has been associated.
   *        Functions like SendSync, SendAsync, Send can be used that do not have an endpoint argument.
   */
  kConnected = 1
};

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_BasicDatagramSocket
/*!
 * \brief Socket for datagram communication.
 *
 * \tparam Protocol Protocol type that fulfills the protocol requirements already defined by BasicSocket
 *
 * A datagram socket takes ownership of operating system resources and provides simple services to send or receive
 * datagrams.
 *
 * Ownership can only be passed but never copied. Thus all copy operators/constructors are deleted.
 *
 * For the behavior of this socket in unconventional circumstances see DSGN-Osab-BasicIP_unconventional_circumstances.
 *
 */
template <class Protocol>
class BasicDatagramSocket final : public BasicSocket<Protocol, BasicDatagramSocket<Protocol>> {
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
   * \brief Alias for endpoint construction token.
   */
  using EndpointToken = ip::internal::EndpointConstructionToken;

  /*!
   * \brief Enables or disables the broadcast socket option.
   *
   * \trace DSGN-Osab-UDP_OptionBroadcast
   */
  using SockOptBroadcast = SocketOptionBoolean<SOL_SOCKET, SO_BROADCAST>;

  /*!
   * \brief Socket option used for joining a multicast group.
   *
   * \trace DSGN-Osab-BasicIP_OptionMulticastJoinGroup
   */
  using SockOptMulticastJoinGroup =
      SocketOptionIPMulticastGroup<IPPROTO_IP, IP_ADD_MEMBERSHIP, IPPROTO_IPV6, IPV6_JOIN_GROUP>;

  /*!
   * \brief Socket option used for leaving a multicast group.
   *
   * \trace DSGN-Osab-BasicIP_OptionMulticastLeaveGroup
   */
  using SockOptMulticastLeaveGroup =
      SocketOptionIPMulticastGroup<IPPROTO_IP, IP_DROP_MEMBERSHIP, IPPROTO_IPV6, IPV6_LEAVE_GROUP>;

  /*!
   * \brief Socket option to set the Time To Live / hop limit of outgoing multicast packages.
   *
   * \details The default value (no option is set) for TTL / hop limit is 1, i.e. the package won't be forwarded
   *          beyond the local network.
   *
   * \trace DSGN-Osab-BasicIP_OptionMulticastHops
   */
  using SockOptMulticastTTL =
      SocketOptionIPSignedInteger<IPPROTO_IP, IP_MULTICAST_TTL, IPPROTO_IPV6, kIPv6MulticastHops>;

  /*!
   * \brief Socket option to set the network interface used for outgoing multicast packages.
   *
   * \trace DSGN-Osab-BasicIP_OptionMulticastSendInterface
   */
  using SockOptMulticastNetworkInterface =
      typename BasicSocket<Protocol, BasicDatagramSocket<Protocol>>::SocketOptionMulticastNetworkInterface;

  /*!
   * \brief Socket option to enable / disable loopback of outgoing multicast packages.
   *
   * \details The default value (no option is set) for loopback is enabled on Linux/QNX, disabled on PikeOs.
   *
   * \trace DSGN-Osab-BasicIP_OptionMulticastLoopback
   */
  using SockOptMulticastLoop =
      SocketOptionIPBoolean<IPPROTO_IP, IP_MULTICAST_LOOP, kIPv6MulticastLoopProtocol, kIPv6MulticastLoop>;

  /*!
   * \brief Notifies about a completed asynchronous send operation.
   *
   * \param[in] result The result of the send operation. Possible errors:
   *
   *  Possible errors:
   *        osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   *        osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   *        osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   *        osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                            message size > MTU size) or a kernel subsystem is not
   *                                                            configured.
   *        osabstraction::OsabErrc::kDisconnected              No route to destination exists or the route is
   *                                                            configured as blackhole in the routing table. Note that
   *                                                            this may be an asynchronous network error that has been
   *                                                            caused by a previous datagram.
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
   * \brief Stores additional information about the received datagram.
   */
  struct DatagramInfo {
    /*!
     * \brief The sender's network endpoint.
     */
    EndpointType remote_endpoint;

    /*!
     * \brief Received datagram's size.
     */
    std::size_t datagram_size{};
  };

  /*!
   * \brief Notifies about a completed asynchronous receive operation.
   *
   * \param[in] result The result of the receive operation. On success it contains additional information about the
   *                   received datagram.
   *
   * Possible errors:
   *         osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   *         osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   *         osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   *         osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by
   *                                                              a previous datagram.
   *         osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                              received datagram. The trailing bytes of the datagram
   *                                                              have been dropped.
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
  using ReceiveCompletionCallback = vac::language::UniqueFunction<void(::amsr::core::Result<DatagramInfo>&& result)>;

  /*!
   * \brief Constructs a socket for synchronous communication.
   *
   * \context         ANY
   *
   * \pre             -
   *
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  BasicDatagramSocket() noexcept = default;

  /*!
   * \brief Moves the other socket into this one.
   *
   * \param[in] other   Other socket used to construct this socket. Other socket must have any ongoing asynchronous
   *                    operation. Its lock must not be locked.
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
  BasicDatagramSocket(BasicDatagramSocket&& other) noexcept;

  /*!
   * \brief Deleted copy constructor
   */
  BasicDatagramSocket(BasicDatagramSocket const& other) = delete;

  /*!
   * \brief Construct a datagram socket for synchronous and asynchronous communication.
   *
   * \param[in] reactor Reactor interface for asynchronous operations. The reactor has to exist for the whole
   *                    lifetime of this BasicDatagramSocket object.
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
  explicit BasicDatagramSocket(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept;

  /*!
   * \brief Destroys a socket.
   *
   * \details         No ongoing asynchronous call is allowed during destruction.
   *                  The user has to avoid destructing the BasicDatagramSocket object while the internal reactor
   *                  callback is executing. This can be done by using a separate software event reactor job, or
   *                  by ensuring per design that BasicDatagramSocket is destroyed in the same thread where
   *                  Reactor::HandleEvents() is called.
   *
   * \context         ANY
   *
   * \pre             No ongoing asynchronous callback.
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
   * \trace           DSGN-Osab-BasicIP_Destruct
   *
   * \vprivate        Vector product internal API
   */
  ~BasicDatagramSocket() noexcept final;

  // VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_SocketBase_MoveOperator
  /*!
   * \brief Closes this socket and moves other socket into this one.
   *
   * \param[in] other Other socket that will be moved into this one. Other socket must have any ongoing asynchronous
   *                        operation. Its lock must not be locked.
   *
   * \return Reference to this object.
   *
   * \context         ANY
   *
   * \pre             This socket has no ongoing asynchronous operation.
   * \pre             The lock of this object is not locked.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto operator=(BasicDatagramSocket&& other) & noexcept -> BasicDatagramSocket&;

  /*!
   * \brief Deleted copy operator.
   */
  auto operator=(BasicDatagramSocket const& other) & noexcept -> BasicDatagramSocket& = delete;

  /*!
   * \brief Prepares the socket to send limited broadcasts.
   *
   * \details  This interface prepares the socket to be able to send limited broadcasts, by:
   *           - Setting SockOptBroadcast option for this socket.
   *           - Proper configuration for the socket based on the underlaying OS and the used network stack.
   *           After this was called:
   *           - Only traffic coming in through the passed interface can be received.
   *           - All sent messages leave through the passed interface.
   *           This interface has to be called before Connect() if the socket is intended to be associated with
   *           a remote endpoint and socket will be used for limited broadcasts.
   *
   * \param[in] interface The network interface on which the broadcasts shall be sent. The interface must be managed by
   *                      the same network stack as this socket (specified during Open() previously). If Bind() has been
   *                      called before, this must be the same interface that was represented by the endpoint passed to
   *                      Bind().
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open or it is not an IPv4 socket or
   *                                                            the socket uses a different network stack than the
   *                                                            passed interface.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support a required socket
   *                                                            option.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Insufficient privileges to prepare broadcast.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to prepare broadcast.
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
   * \trace DSGN-Osab-BasicIP_QNX_LimitedBroadcast
   */
  auto PrepareBroadcast(Interface const& interface) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Binds a socket to a local endpoint.
   *
   * \param[in] endpoint The local network endpoint or a multicast endpoint. The endpoint must use the same network
   *                     stack as this socket (specified during Open() previously) and if PrepareBroadcast() has been
   *                     called before, this endpoint must represent the same interface that was passed there.
   *
   * \details
   * Bind the local network endpoint to the socket. The endpoint will be the destination address for incoming
   * communication and the source address for outgoing communication.
   *
   * Multicast endpoint can also be bound. However, it is not possible to send from a multicast endpoint. A multicast
   * endpoint can only be the destination of a datagram and never the source.
   *
   * \return The result of the service.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open or already bound or
   *                                                            endpoint and socket have different protocol families or
   *                                                            endpoint and prepared for broadcast socket have
   *                                                            different interfaces or the socket uses a different
   *                                                            network stack than the passed endpoint.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to bind address.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kAddressError              The address is already in use on the local machine.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to verify the address.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The address is assigned to multiple interfaces.
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
   * \vprivate        Vector product internal API
   *
   * \trace DSGN-Osab-BasicIP_Bind
   */
  auto Bind(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Associates the socket to a remote endpoint.
   *
   * \details
   * The operation is performed synchronously. An asynchronous Connect() method is not available. If connect is called
   * while an asynchronous Send operation is in progress, the datagram sent by that operation may be sent to either the
   * old or the new default destination.
   * Connecting a socket causes datagrams from all addresses the socket is not connected to to be rejected. Note that it
   * is still possible to receive datagrams from other addresses than the connected one if these datagrams were present
   * in the receive buffer before connect was called.
   * If the provided endpoint is a multicast endpoint, the socket option 'SockOptMulticastNetworkInterface' is set
   * automatically.
   *
   * \attention In case this function outputs an error it is undefined whether the 'SockOptMulticastNetworkInterface'
   *            is set for multicast endpoints or not.
   *
   * \param[in] endpoint Endpoint that matches the protocol family passed during open call (e.g Do not try to connect
   *                     to an IPv6 endpoint if you opened an IPv4 socket). The endpoint must use the same network stack
   *                     as this socket (specified during Open() previously).
   *
   * \error osabstraction::OsabErrc::kApiError                   The socket is not open or already connected or the
   *                                                             protocol of the provided endpoint does not match the
   *                                                             protocol of the socket or the socket uses a different
   *                                                             network stack than the passed endpoint or there is an
   *                                                             asynchronous SentTo operation in progress.
   * \error osabstraction::OsabErrc::kDisconnected               The peer is not reachable or the route to the peer is
   *                                                             configured as blackhole in the routing table.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges     The process has insufficient privileges to set the
   *                                                             address as default destination or IPv4 is used and
   *                                                             the process has insufficient privileges to extract
   *                                                             the assigned IPv4 address of interface.
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                   No ephemeral ports or buffer space or entries in
   *                                                             routing cache available.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        Cannot set default destination because local endpoint
   *                                                             is already used by another socket or when the address
   *                                                             is not available on the local machine.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError     The system settings do not support to set
   *                                                             the multicast option for the socket or the contained IP
   *                                                             address is assigned to multiple interfaces or no
   *                                                             interface with the contained IP address was found.
   * \error osabstraction::OsabErrc::kAddressError               IPv4 is used and the interface does not have an IPv4
   *                                                             address assigned or address is assigned to multiple
   *                                                             interfaces or no interface with the supplied name has
   *                                                             been found.
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
  auto Connect(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends data synchronously to the connected (associated) endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The buffer
   *                             must consist of at most kMaxIOBufferArraySize entries. The io_buffers_view has
   *                             to exist for the whole lifetime of the class object.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or not connected
   *                                                             (associated).
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             send operation.
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
  auto SendSync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends data synchronously to the connected (associated) endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in] buffer_view  View to an array of bytes. Must at most consist of kMaxIOBufferSize bytes. The
   *                         buffer_view has to exist for the whole lifetime of the class object.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or not connected
   *                                                             (associated).
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             send operation.
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
  auto SendSync(::amsr::core::Span<std::uint8_t const> buffer_view) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends multiple datagrams synchronously to the connected (associated) endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * It is strongly recommended to set the socket to unblocking mode. In unblocking mode SendSyncBulk() returns
   * immediately after sending all input datagrams up to the specified datagram number from the socket (limited by
   * the size of buffer_view). If not all datagrams could be sent, the remaining datagrams can be sent with another
   * SendSyncBulk() call.
   * In blocking mode the user is responsible for setting a socket option timeout. Then the SendSyncBulk() will return
   * either after sending the maximum number of datagrams (specified by the size of buffer_view) or after the send
   * timeout runs out. If no send timeout is set, SendSyncBulk() blocks until the maximum number of datagrams is sent.
   * It will not return after sending less datagrams and may therefore block for significant time (e.g. due to high load
   * on the network slowing down sending).
   *
   * Please note that at most as many messages as returned by GetMaxMessagesForBulkSend() can be sent. Trying to
   * send more results in truncation of datagrams array to GetMaxMessagesForBulkSend() entries.
   *
   * \param[in] datagrams        View to arrays of IOBuffers. The IOBuffers must point to valid buffers. Each buffer
   *                             must consist of at most kMaxIOBufferArraySize entries.
   *                             The underlying memory (the actual data and the ConstIOBuffer objects) must exist until
   *                             this function returns.
   *
   * \return A return value of n means that the first n from the input datagrams span have been sent
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    One of the datagrams exceeds the maximum allowed size
   *                                                             (e.g. message size > MTU size) or a kernel subsystem is
   *                                                             not configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or not connected
   *                                                             (associated).
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             send operation.
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
   * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Send
   *
   * \vprivate        Vector product internal API
   */
  auto SendSyncBulk(::amsr::core::Span<::amsr::core::Span<osabstraction::io::ConstIOBuffer>> datagrams) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Sends multiple datagrams synchronously to the connected (associated) endpoint.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * It is strongly recommended to set the socket to unblocking mode. In unblocking mode SendSyncBulk() returns
   * immediately after sending all input datagrams up to the specified datagram number from the socket (limited by
   * the size of buffer_view). If not all datagrams could be sent, the remaining datagrams can be sent with another
   * SendSyncBulk() call.
   * In blocking mode the user is responsible for setting a socket option timeout. Then the SendSyncBulk() will return
   * either after sending the maximum number of datagrams (specified by the size of buffer_view) or after the send
   * timeout runs out. If no send timeout is set, SendSyncBulk() blocks until the maximum number of datagrams is sent.
   * It will not return after sending less datagrams and may therefore block for significant time (e.g. due to high load
   * on the network slowing down sending).
   *
   * Please note that at most as many messages as returned by GetMaxMessagesForBulkSend() can be sent. Trying to
   * sending more results in GetMaxMessagesForBulkSend() or fewer messages being sent.
   *
   * \param[in] datagrams      Views to arrays of bytes. Each entry must at most consist of kMaxIOBufferSize bytes.
   *                           The underlying memory must exist until this function returns.
   *
   * \return A return value of n means that the first n from the input datagrams span have been sent
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagrams.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    One of the datagram exceeds the maximum allowed size
   *                                                             (e.g. message size > MTU size) or a kernel subsystem is
   *                                                             not configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or not connected
   *                                                             (associated).
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             send operation.
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
   * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Send
   *
   * \vprivate        Vector product internal API
   */
  auto SendSyncBulk(::amsr::core::Span<::amsr::core::Span<std::uint8_t const>> datagrams) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Sends data asynchronously to the connected (associated) endpoint.
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
   *                             (CheckIsClosed() == true). Must consist of at most kMaxIOBufferArraySize entries.
   *
   * \param[in] callback Callback to notify the completion of the send operation. The socket will take ownership
   *                     of the passed callback and will hold the ownership till the callback has been
   *                     executed or till the socket is closed (CheckIsClosed() == true).
   *
   * \return A value if the asynchronous send request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous send request was rejected since the
   *                                                             socket is not open and/or not connected (associated)
   *                                                             and/or there is already an ongoing asynchronous send
   *                                                             request.
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
   * \param[in] buffer_view  View to an array of bytes. The socket will take exclusive access rights of the buffers and
   *                         will hold the exclusive access rights till the callback has been executed or till the
   *                         socket closed (CheckIsClosed() == true). Must consist of at most kMaxIOBufferSize bytes.
   *
   * \param[in] callback Callback to notify the completion of the send operation. The socket will take ownership
   *                     of the passed callback and will hold the ownership till the callback has been
   *                     executed or till the socket is closed (CheckIsClosed() == true).
   *
   * \return A value if the asynchronous send request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous send request was rejected since the
   *                                                             socket is not open and/or not connected (associated)
   *                                                             and/or there is already an ongoing asynchronous send
   *                                                             request.
   *
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
   * - In case the operation can be completed immediately:The completion callback is not called, exclusive access rights
   *                                                      of the input buffer is released when the function returns.
   * The return value indicates whether the operation is performed asynchronously or not.
   *
   * Should the operation not complete immediately it is handled as any other asynchronous send operation. Note that
   * this also applies to the precondition of asynchronous send operations that no other asynchronous send operation is
   * allowed to be in progress when an asynchronous send operation is requested.
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The socket
   *                             will take exclusive access rights of the buffers and will hold the exclusive access
   *                             rights till the callback has been executed or till the socket is closed
   *                             (CheckIsClosed() == true). Must consist of at most kMaxIOBufferArraySize entries.
   *
   * \param[in] callback Callback to notify the completion of the send operation. The socket will take ownership
   *                     of the passed callback until the operation is completed. Should all data be sent immediately
   *                     ownership is released when this function returns.
   *
   * \return Information if all data was sent or if asynchronous handling is necessary to complete sending.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or not connected
   *                                                             (associated) and/or there is an ongoing asynchronous
   *                                                             send operation.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE unless asynchronous behavior is required.
   * \threadsafe      FALSE
   * \steady          TRUE
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
   *
   * Should the operation not complete immediately it is handled as any other asynchronous send operation. Note that
   * this also applies to the precondition of asynchronous send operations that no other asynchronous send operation is
   * allowed to be in progress when an asynchronous send operation is requested.
   *
   * \param[in] buffer_view  View to an array of bytes. The socket will take exclusive access rights of the buffers and
   *                         will hold the exclusive access rights till the callback has been executed or till the
   *                         socket closed (CheckIsClosed() == true). Must consist of at most kMaxIOBufferSize bytes.
   *
   * \param[in] callback Callback to notify the completion of the send operation. The socket will take ownership
   *                     of the passed callback until the operation is completed. Should all data be sent immediately
   *                     ownership is released when this function returns.
   *
   * \return Information if all data was sent or if asynchronous handling is necessary to complete sending.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or not connected
   *                                                             (associated) and/or there is an ongoing asynchronous
   *                                                             send operation.
   *
   * \context         ANY
   *
   * \pre             Socket can perform asynchronous operations.
   * \pre             Blocking mode is disabled.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE unless asynchronous behavior is required.
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto Send(::amsr::core::Span<std::uint8_t const> buffer_view, SendCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<osabstraction::io::SendResult>;

  /*!
   * \brief Sends a datagram to the specified destination.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The array's
   *                             size limit is osabstraction::io::kMaxIOBufferArraySize. The io_buffers_view has to
   *                             exist for the whole lifetime of the class object.
   * \param[in] destination      The destination endpoint. The endpoint's protocol family shall match the socket's
   *                             family (e.g. an IPv4 socket cannot send a datagram to an IPv6 endpoint). The endpoint
   *                             must use the same network stack as this socket (specified during Open() previously).
   *
   * \return The result of the service.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists or is configured as
   *                                                             blackhole in the routing table. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open or is connected (associated).
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             send operation or the protocol of the destination
   *                                                             endpoint does not match the protocol of this socket or
   *                                                             the socket uses a different network stack than the
   *                                                             passed endpoint or if the destination is the limited
   *                                                             broadcast address but the socket is not bound and/or
   *                                                             not prepared for broadcast.
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
  auto SendToSync(::amsr::core::Span<osabstraction::io::ConstIOBuffer const> io_buffers_view,
                  EndpointType const& destination) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends a datagram to the specified destination.
   *
   * \param[in] buffer_view     View to an array of bytes. Must consist of at most kMaxIOBufferSize bytes. The
   *                            buffer_view has to exist for the whole lifetime of the class object.
   * \param[in] destination     The destination endpoint. The endpoint's protocol family shall match the socket's
   *                            family (e.g. an IPv4 socket cannot send a datagram to an IPv6 endpoint). The endpoint
   *                            must use the same network stack as this socket (specified during Open() previously).
   *
   * \return The result of the service.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists or is configured as
   *                                                             blackhole in the routing table. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open or is connected (associated).
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             send operation or the protocol of the destination
   *                                                             endpoint does not match the protocol of this socket or
   *                                                             the socket uses a different network stack than the
   *                                                             passed endpoint or if the destination is the limited
   *                                                             broadcast address but the socket is not bound and/or
   *                                                             not prepared for broadcast.
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
  auto SendToSync(::amsr::core::Span<std::uint8_t const> buffer_view, EndpointType const& destination) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends multiple datagrams to the specified destinations.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * It is strongly recommended to set the socket to unblocking mode. In unblocking mode SendToSyncBulk() returns
   * immediately after sending all input datagrams up to the specified datagram number from the socket (limited by
   * the size of buffer_view). If not all datagrams could be sent, the remaining datagrams can be sent with another
   * SendToSyncBulk() call.
   * In blocking mode the user is responsible for setting a socket option timeout. Then the SendToSyncBulk() will return
   * either after sending the maximum number of datagrams (specified by the size of buffer_view) or after the send
   * timeout runs out. If no send timeout is set, SendToSyncBulk() blocks until the maximum number of datagrams is sent.
   * It will not return after sending less datagrams and may therefore block for significant time (e.g. due to high load
   * on the network slowing down sending).
   *
   * Different destinations can be supplied for different datagrams.
   *
   * Please note that at most as many messages as returned by GetMaxMessagesForBulkSend() can be sent. Trying to
   * sending more results in GetMaxMessagesForBulkSend() or fewer messages being sent.
   *
   * \param[in] datagrams        Information about the datagrams to send:
   *                             - Views to arrays of IOBuffers. The IOBuffers must point to valid buffers. Each array's
   *                               size limit is osabstraction::io::kMaxIOBufferArraySize. Underlying memory (actual
   *                               data as well as ConstIOBuffer objects) has to exist until the function returns.
   *                             - The destination endpoint. The endpoint's protocol family shall match the socket's
   *                               family (e.g. an IPv4 socket cannot send a datagram to an IPv6 endpoint). The endpoint
   *                               must use the same network stack as this socket (specified during Open() previously).
   *
   * \return A return value of n means that the first n from the input datagrams span have been sent
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists or is configured as
   *                                                             blackhole in the routing table. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or connected (associated).
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             send operation or the protocol of the destination
   *                                                             endpoint does not match the protocol of this socket or
   *                                                             the socket uses a different network stack than one of
   *                                                             the passed endpoints.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Send
   *
   * \vprivate        Vector product internal API
   */
  auto SendToSyncBulk(::amsr::core::Span<std::pair<::amsr::core::Span<osabstraction::io::ConstIOBuffer>, EndpointType>>
                          datagrams) noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Sends multiple datagrams to the specified destinations.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * It is strongly recommended to set the socket to unblocking mode. In unblocking mode SendToSyncBulk() returns
   * immediately after sending all input datagrams up to the specified datagram number from the socket (limited by
   * the size of buffer_view). If not all datagrams could be sent, the remaining datagrams can be sent with another
   * SendToSyncBulk() call.
   * In blocking mode the user is responsible for setting a socket option timeout. Then the SendToSyncBulk() will return
   * either after sending the maximum number of datagrams (specified by the size of buffer_view) or after the send
   * timeout runs out. If no send timeout is set, SendToSyncBulk() blocks until the maximum number of datagrams is sent.
   * It will not return after sending less datagrams and may therefore block for significant time (e.g. due to high load
   * on the network slowing down sending).
   *
   * Different destinations can be supplied for different datagrams.
   *
   * Please note that at most as many messages as returned by GetMaxMessagesForBulkSend() can be sent. Trying to
   * sending more results in GetMaxMessagesForBulkSend() or fewer messages being sent.
   *
   * \param[in] datagrams       Information about datagrams to send:
   *                            - Views to arrays of bytes. Each view must consist of at most kMaxIOBufferSize bytes.
   *                              Underlying data has to exist until this function has returned.
   *                            - The destination endpoint. The endpoint's protocol family shall match the socket's
   *                              family (e.g. an IPv4 socket cannot send a datagram to an IPv6 endpoint). The endpoint
   *                              must use the same network stack as this socket (specified during Open() previously).
   *
   * \return A return value of n means that the first n from the input datagrams span have been sent
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists or is configured as
   *                                                             blackhole in the routing table. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or connected (associated).
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             send operation or the protocol of the destination
   *                                                             endpoint does not match the protocol of this socket or
   *                                                             the socket uses a different network stack than one of
   *                                                             the passed endpoints.
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
   * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Send
   *
   * \vprivate        Vector product internal API
   */
  auto SendToSyncBulk(
      ::amsr::core::Span<std::pair<::amsr::core::Span<std::uint8_t const>, EndpointType>> datagrams) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Sends a datagram to the specified destination in the background.
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
   *                             (CheckIsClosed() == true). Must consist of at most kMaxIOBufferArraySize entries.
   *
   * \param[in] destination      The destination endpoint. The endpoint's protocol family shall match the socket's
   *                             family (e.g. an IPv4 socket cannot send a datagram to an IPv6 endpoint). The endpoint
   *                             must use the same network stack as this socket (specified during Open() previously).
   *
   * \param[in] callback         Callback to notify the completion of the sendto operations. The socket will take
   *                             ownership of the passed callback and will hold the ownership till the callback has been
   *                             executed or till the socket is closed (CheckIsClosed() == true).
   *
   * \return A value if the asynchronous send request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous send request was rejected since the
   *                                                             socket is not open or is connected (associated)
   *                                                             or there is already an ongoing asynchronous send
   *                                                             request or the protocol of the destination endpoint
   *                                                             does not match the protocol of this socket or the
   *                                                             socket uses a different network stack than the passed
   *                                                             endpoint or if the destination is the limited broadcast
   *                                                             address but the socket is not bound and/or not prepared
   *                                                             for broadcast.
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
   * \vprivate        Vector product internal API
   */
  auto SendToAsync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                   EndpointType const& destination, SendCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends a datagram to the specified destination.
   *
   * \details
   * The socket must be in non-blocking mode and it must stay in that mode till the operation finishes in the
   * completion callback.
   *
   * \param[in] buffer_view      View to an array of bytes. The socket will take exclusive access rights of the buffer
   *                             and will hold the exclusive access rights till the callback has been executed or till
   *                             the socket closed (CheckIsClosed() == true). Must consist of at most kMaxIOBufferSize
   *                             bytes.
   *
   * \param[in] destination      The destination endpoint. The endpoint's protocol family shall match the socket's
   *                             family (e.g. an IPv4 socket cannot send a datagram to an IPv6 endpoint). The endpoint
   *                             must use the same network stack as this socket (specified during Open() previously).
   *
   * \param[in] callback         Callback to notify the completion of the sendto operations. The socket will take
   *                             ownership of the passed callback and will hold the ownership till the callback has been
   *                             executed or till the socket closed  (CheckIsClosed() == true).
   *
   * \return A value if the asynchronous send request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous send request was rejected since the
   *                                                             socket is not open or is connected (associated)
   *                                                             or there is already an ongoing asynchronous send
   *                                                             request or the protocol of the destination endpoint
   *                                                             does not match the protocol of this socket or the
   *                                                             socket uses a different network stack than the passed
   *                                                             endpoint or if the destination is the limited broadcast
   *                                                             address but the socket is not bound and/or not prepared
   *                                                             for broadcast.
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
   * \vprivate        Vector product internal API
   */
  auto SendToAsync(::amsr::core::Span<std::uint8_t const> buffer_view, EndpointType const& destination,
                   SendCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Sends a datagram to the specified destination in the background.
   *
   * \details
   * The socket must be in non-blocking mode and it must stay in that mode till the operation finishes in the
   * completion callback. This function takes the optimistic approach. It will first try to send the message
   * with a non-blocking synchronous service. If this approach fails due to the osabstraction::OsabErrc::kBusy error
   * it will fall back to the SendToAsync service.
   *
   * The service finishes if one of the three conditions evaluates to true.
   *
   *  - This function returns SendResult::kSendCompleted.
   *  - The passed completion callback is called.
   *  - The socket is closed (CheckIsClosed() == true).
   *
   * \param[in] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The socket
   *                             will take exclusive access rights of the buffers till the service finishes. Must
   *                             consist of at most kMaxIOBufferArraySize entries.
   *
   * \param[in] destination      The destination endpoint. The endpoint's protocol family shall match the socket's
   *                             family (e.g. an IPv4 socket cannot send a datagram to an IPv6 endpoint). The endpoint
   *                             must use the same network stack as this socket (specified during Open() previously).
   *
   * \param[in] callback         Callback to notify the completion of the sendto operations. The socket will take
   *                             ownership of it till the service finishes.
   *
   * \return The state of the send operation
   * \retval SendResult::kSendCompleted             The operation has been executed directly. Socket has released
   *                                                ownership over the callback and exclusive access rights of the
   *                                                buffer and the callback will not be called.
   * \retval SendResult::kAsyncProcessingNecessary  The operation could not be executed directly. It will be processed
   *                                                asynchronously in the reactor context. The passed completion
   *                                                callback will notify about a finished send operation.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists or is configured as
   *                                                             blackhole in the routing table. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open or is connected (associated)
   *                                                             or there is an ongoing asynchronous send operation
   *                                                             ongoing or the protocol of the destination endpoint
   *                                                             does not match the protocol of this socket or the
   *                                                             socket uses a different network stack than the passed
   *                                                             endpoint or if the destination is the limited broadcast
   *                                                             address but the socket is not bound and/or not prepared
   *                                                             for broadcast.
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
   * \vprivate        Vector product internal API
   */
  auto SendTo(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view, EndpointType const& destination,
              SendCompletionCallback&& callback) noexcept -> ::amsr::core::Result<osabstraction::io::SendResult>;

  /*!
   * \brief Sends a datagram to the specified destination.
   *
   * \details
   * The socket must be in non-blocking mode and it must stay in that mode till the operation finishes in the
   * completion callback. This function takes the optimistic approach. It will first try to send the message
   * with a non-blocking synchronous service. If this approach fails due to the osabstraction::OsabErrc::kBusy error
   * it will fall back to the SendToAsync service.
   *
   * The service finishes if one of the three conditions evaluates to true.
   *
   *  - This function returns SendResult::kSendCompleted.
   *  - The passed completion callback is called.
   *  - The socket is closed (CheckIsClosed() == true).
   *
   * \param[in] buffer_view      View to an array of bytes. The socket will take exclusive access rights of the buffers
   *                             till the service finishes. Must consist of at most kMaxIOBufferSize bytes.
   *
   * \param[in] destination      The destination endpoint. The endpoint's protocol family shall match the socket's
   *                             family (e.g. an IPv4 socket cannot send a datagram to an IPv6 endpoint). The endpoint
   *                             must use the same network stack as this socket (specified during Open() previously).
   *
   * \param[in] callback         Callback to notify the completion of the sendto operations. The socket will take
   *                             ownership of it till the service finishes.
   *
   * \return The state of the send operation
   * \retval SendResult::kSendCompleted             The operation has been executed directly. Socket has released
   *                                                ownership over the callback and exclusive access rights of the
   *                                                buffer and the callback will not be called.
   * \retval SendResult::kAsyncProcessingNecessary  The operation could not be executed directly. It will be processed
   *                                                asynchronously in the reactor context. The passed completion
   *                                                callback will notify about a finished send operation.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send datagram.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send datagram.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The datagram exceeds the maximum allowed size (e.g.
   *                                                             message size > MTU size) or a kernel subsystem is not
   *                                                             configured.
   * \error  osabstraction::OsabErrc::kDisconnected              No route to destination exists or is configured as
   *                                                             blackhole in the routing table. Note that this may be
   *                                                             an asynchronous network error that has been caused by a
   *                                                             previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open or is connected (associated)
   *                                                             or there is an ongoing asynchronous send operation
   *                                                             ongoing or the protocol of the destination endpoint
   *                                                             does not match the protocol of this socket or the
   *                                                             socket uses a different network stack than the passed
   *                                                             endpoint or if the destination is the limited broadcast
   *                                                             address but the socket is not bound and/or not prepared
   *                                                             for broadcast.
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
   * \vprivate        Vector product internal API
   */
  auto SendTo(::amsr::core::Span<std::uint8_t const> buffer_view, EndpointType const& destination,
              SendCompletionCallback&& callback) noexcept -> ::amsr::core::Result<osabstraction::io::SendResult>;

  /*!
   * \brief Receives data synchronously (typically from the connected endpoint).
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * This method calls ReceiveFromSync and drops the remote endpoint information of the sender of the datagram.
   *
   * Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to are
   * rejected. Datagrams that resided in the receive buffer since before Connect() was called are still output even if
   * they originated from an address other than the connected address.
   *
   * \param[in, out] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                                  incoming datagram will be written to the provided buffers. The content of the
   *                                  buffers is undefined if the service reports an error. The number of written bytes
   *                                  will be returned. Must consist of at most kMaxIOBufferArraySize entries. The
   *                                  io_buffers_view has to exist for the whole lifetime of the class object.
   *
   * \return Number of bytes that have actually been received.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                              previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                   The socket is not open.
   *                                                              Also could happen when there is an ongoing
   *                                                              asynchronous receive operation.
   * \error  osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                              received datagram. The trailing bytes of the datagram
   *                                                              have been dropped.
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
  auto ReceiveSync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Receives data synchronously (typically from the connected endpoint).
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * This method calls ReceiveFromSync and drops the remote endpoint information of the sender of the datagram.
   *
   * Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to are
   * rejected. Datagrams that resided in the receive buffer since before Connect() was called are still output even if
   * they originated from an address other than the connected address.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming datagram will be written to the provided
   *                               buffer. The content of the buffer is undefined if the service reports an error. The
   *                               number of written bytes will be returned. Must consist of at most kMaxIOBufferSize
   *                               bytes. The buffer_view has to exist for the whole lifetime of the class object.
   *
   * \return Number of bytes that have actually been received.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                              previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                   The socket is not open.
   *                                                              Also could happen when there is an ongoing
   *                                                              asynchronous receive operation.
   * \error  osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                              received datagram. The trailing bytes of the datagram
   *                                                              have been dropped.
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
  auto ReceiveSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Receives a bulk of data synchronously (typically from the connected endpoint).
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * It is strongly recommended to set the socket to unblocking mode. In unblocking mode ReceiveSyncBulk() returns
   * immediately after reading all received datagrams up to the specified datagram number from the socket (limited by
   * the size of buffer_view). If not all datagrams could be received, the remaining datagrams are received with the
   * next ReceiveSyncBulk() call.
   * In blocking mode the user is responsible for setting a socket option timeout. Then the ReceiveSyncBulk() will
   * return either after receiving the maximum number of datagrams (specified by the size of buffer_view) or after the
   * receive timeout runs out. If no receive timeout ReceiveSyncBulk() blocks until the maximum number of datagrams
   * is received. It will not return after receiving less datagrams and may therefore block indefinitely.
   *
   * Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to are
   * rejected. Datagrams that resided in the receive buffer since before Connect() was called are still output even if
   * they originated from an address other than the connected address.
   *
   * Please note that at most as many messages as returned by GetMaxMessagesForBulkReceive() can be received. Trying to
   * receive more results in GetMaxMessagesForBulkReceive() or fewer messages being received.
   *
   * \param[in, out] buffers_view  View to an array of pairs of datagram and datagram size. For each incoming datagram
   *                               the pair of data and size will be written to one of the provided buffers.
   *                               In case a received datagrams is truncated (provided buffer did not have enough space
   *                               to store the received datagram) the datagram size is set to kTruncatedDatagram.
   *                               The content of the buffers is undefined if the service reports an error.
   *
   * \return Number of datagrams that have actually been received.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                              previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                   The socket is not open.
   *                                                              Also could happen when there is an ongoing
   *                                                              asynchronous receive operation.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError     The kernel subsystem is not configured.
   *
   * \context         ANY
   *
   * \pre             Each datagram buffer must consist maximum kMaxIOBufferSize bytes.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Receive
   */
  auto ReceiveSyncBulk(
      ::amsr::core::Span<std::pair<::amsr::core::Span<std::uint8_t>, std::size_t>> buffers_view) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Copies a datagram from the receive buffer but does not remove it from the receive buffer.
   *
   * \details The operation can be performed in blocking or unblocking mode.
   *          Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to
   *          are rejected. Datagrams that resided in the receive buffer since before Connect() was called are still
   *          output even if they originated from an address other than the connected address.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming datagram will be written to the provided
   *                               buffer. The content of the buffer is undefined if the service reports an error. The
   *                               number of written bytes will be returned. Must consist of at most kMaxIOBufferSize
   *                               bytes. The buffer_view has to exist for the whole lifetime of the class object.
   *
   * \return Number of bytes that have actually been copied from the receive buffer.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to peek datagram.
   * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to peek datagram.
   * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                              previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                   The socket is not open.
   *                                                              Also could happen when there is an ongoing
   *                                                              asynchronous receive operation.
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
  auto PeekSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Receives a datagram.
   *
   * \details Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to
   *          are rejected. Datagrams that resided in the receive buffer since before Connect() was called are still
   *          ouptut even if they originated from an address other than the connected address.
   *
   * \param[in, out] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                                  incoming datagram will be written to the provided buffers. The content of the
   *                                  buffers is undefined if the service reports an error. The number of written bytes
   *                                  will be stored in the returned DatagramInfo. The socket will take exclusive access
   *                                  rights of the buffer and will hold the exclusive access rights till the callback
   *                                  has been executed or till the socket is closed (CheckIsClosed() == true). Must
   *                                  consist of at most kMaxIOBufferArraySize entries.
   *
   * \param[in] callback              Callback to notify the completion of the ReceiveFrom operations. The socket will
   *                                  take ownership of the passed callback and will hold the ownership till the
   *                                  callback has been executed or till the socket closed  (CheckIsClosed() == true).
   *
   * \return A value if the asynchronous receive request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous receive request was rejected since the
   *                                                             socket is not open and/or there is already an ongoing
   *                                                             asynchronous receive request.
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
   * \vprivate        Vector product internal API
   */
  auto ReceiveAsync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                    ReceiveCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void> {
    // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveAsync-MutableIOBuffer
    // No critical section required because this function does not access object attributes directly and
    // ReceiveFromAsync has a critical section that protects access to the attributes done by it.
    ::amsr::core::Result<void> const result{this->ReceiveFromAsync(io_buffers_view, std::move(callback))};
    // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveAsync-MutableIOBuffer
    return result;
  }

  /*!
   * \brief Receives a datagram.
   *
   * \details Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to
   *          are rejected. Datagrams that resided in the receive buffer since before Connect() was called are still
   *          output even if they originated from an address other than the connected address.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming datagram will be written to the provided
   *                               buffers. The content of the buffer is undefined if the service reports an error. The
   *                               number of written bytes will be stored in the returned DatagramInfo.
   *                               The socket will take exclusive access rights of the buffer and will hold the
   *                               exclusive access rights till the callback has been executed or till the socket is
   *                               closed (CheckIsClosed() == true). Must consist of at most kMaxIOBufferSize bytes.
   *
   * \param[in] callback           Callback to notify the completion of the ReceiveFrom operations. The socket will
   *                               take ownership of the passed callback and will hold the ownership till the
   *                               callback has been executed or till the socket closed  (CheckIsClosed() == true).
   *
   * \return A value if the asynchronous receive request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous receive request was rejected since the
   *                                                             socket is not open and/or there is already an ongoing
   *                                                             asynchronous receive request.
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
   * \vprivate        Vector product internal API
   */
  auto ReceiveAsync(::amsr::core::Span<std::uint8_t> buffer_view, ReceiveCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<void> {
    // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveAsync-Span
    // No critical section required because this function does not access object attributes directly and
    // ReceiveFromAsync has a critical section that protects access to the attributes done by it.
    ::amsr::core::Result<void> const result{this->ReceiveFromAsync(buffer_view, std::move(callback))};
    // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveAsync-Span
    return result;
  }

  /*!
   * \brief Receives a datagram.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to are
   * rejected. Datagrams that resided in the receive buffer since before Connect() was called are still output even if
   * they originated from an address other than the connected address.
   *
   * \param[in, out] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                                  incoming datagram will be written to the provided buffers. The content of the
   *                                  buffers is undefined if the service reports an error. The number of written bytes
   *                                  will be stored in the returned DatagramInfo. Must consist of at most
   *                                  kMaxIOBufferArraySize entries. The io_buffers_view has to exist for the whole
   *                                  lifetime of the class object.
   *
   * \return A struct containing additional information about the received datagram.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                              previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                   The socket is not open.
   *                                                              Also could happen when there is an ongoing
   *                                                              asynchronous receive operation.
   * \error  osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                              received datagram. The trailing bytes of the datagram
   *                                                              have been dropped.
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
  auto ReceiveFromSync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept
      -> ::amsr::core::Result<DatagramInfo>;

  /*!
   * \brief Receives a datagram.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to are
   * rejected. Datagrams that resided in the receive buffer since before Connect() was called are still output even if
   * they originated from an address other than the connected address.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming datagram will be written to the provided
   *                               buffer. The content of the buffer is undefined if the service reports an error. The
   *                               number of written bytes will be stored in the returned DatagramInfo. Must at most
   *                               consist of kMaxIOBufferSize bytes. The buffer_view has to exist for the whole
   *                               lifetime of the class object.
   *
   * \return A struct containing additional information about the received datagram.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                              previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                   The socket is not open.
   *                                                              Also could happen when there is an ongoing
   *                                                              asynchronous receive operation.
   * \error  osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                              received datagram. The trailing bytes of the datagram
   *                                                              have been dropped.
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
  auto ReceiveFromSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept -> ::amsr::core::Result<DatagramInfo>;

  /*!
   * \brief Receives a bulk of datagrams and information about their senders.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   * It is strongly recommended to set the socket to unblocking mode. In unblocking mode ReceiveFromSyncBulk returns
   * immediately after reading all received datagrams up to the specified datagram number from the socket (limited by
   * the size of buffer_view). If not all datagrams could be received, the remaining datagrams are received with the
   * next ReceiveFromSyncBulk() call.
   * In blocking mode the user is responsible for setting a socket option timeout. Then the ReceiveFromSyncBulk() will
   * return either after receiving the maximum number of datagrams (specified by the size of buffer_view) or after the
   * receive timeout runs out. If no receive timeout ReceiveFromSyncBulk() blocks until the maximum number of datagrams
   * is received. It will not return after receiving less datagrams and may therefore block indefinitely.
   *
   * Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to are
   * rejected. Datagrams that resided in the receive buffer since before Connect() was called are still output even if
   * they originated from an address other than the connected address.
   *
   * Please note that at most as many messages as returned by GetMaxMessagesForBulkReceive() can be received. Trying to
   * receive more results in GetMaxMessagesForBulkReceive() or fewer messages being received.
   *
   * \param[in, out] buffers_view  View to an array of pairs of datagram and a struct containing information on the
   *                               sender and the datagram size. For each incoming datagram the pair of data, sender
   *                               and size will be written to one the provided buffers. In case a received datagrams is
   *                               truncated (provided buffer did not have enough space to store the received datagram)
   *                               the datagram size is set to kTruncatedDatagram.
   *                               The content of the buffers is undefined if the service reports an error.
   *
   * \return Number of datagrams that have actually been received.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                              previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                   The socket is not open.
   *                                                              Also could happen when there is an ongoing
   *                                                              asynchronous receive operation.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError     The kernel subsystem is not configured.

   * \context         ANY
   *
   * \pre             Each datagram buffer must consist maximum kMaxIOBufferSize bytes.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Receive
   */
  auto ReceiveFromSyncBulk(
      ::amsr::core::Span<std::pair<::amsr::core::Span<std::uint8_t>, DatagramInfo>> buffers_view) noexcept
      -> ::amsr::core::Result<std::size_t>;

  /*!
   * \brief Receives a datagram.
   *
   * \details Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to
   *          are rejected. Datagrams that resided in the receive buffer since before Connect() was called are still
   *          output even if they originated from an address other than the connected address.
   *
   * \param[in, out] io_buffers_view  View to an array of IOBuffers. The IOBuffers must point to valid buffers. The
   *                                  content of the buffers is undefined if the service reports an error. The number of
   *                                  written bytes will be stored in the returned DatagramInfo. The socket will take
   *                                  exclusive access rights of the buffer and will hold the exclusive access rights
   *                                  till the callback has been executed or till the socket is closed
   *                                  (CheckIsClosed() == true). Must consist of at most kMaxIOBufferArraySize entries.
   *
   * \param[in] callback              Callback to notify the completion of the ReceiveFrom operations. The socket will
   *                                  take ownership of the passed callback and will hold the ownership till the
   *                                  callback has been executed or till the socket closed  (CheckIsClosed() == true).
   *
   * \return A value if the asynchronous receive request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous receive request was rejected since the
   *                                                             socket is not open and/or there is already an ongoing
   *                                                             asynchronous receive request.
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
   * \vprivate        Vector product internal API
   */
  auto ReceiveFromAsync(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                        ReceiveCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Receives a datagram.
   *
   * \details Should the socket be connected using Connect(), datagrams from addresses the socket is not connected to
   *          are rejected. Datagrams that resided in the receive buffer since before Connect() was called are still
   *          output even if they originated from an address other than the connected address.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming datagram will be written to the provided
   *                               buffers. The content of the buffer is undefined if the service reports an error. The
   *                               number of written bytes will be stored in the returned DatagramInfo.
   *                               The socket will take exclusive access rights of the buffer and will hold the
   *                               exclusive access rights till the callback has been executed or till the socket is
   *                               closed (CheckIsClosed() == true). Must consist of at most kMaxIOBufferSize bytes.
   *
   * \param[in] callback           Callback to notify the completion of the ReceiveFrom operations. The socket will
   *                               take ownership of the passed callback and will hold the ownership till the
   *                               callback has been executed or till the socket is closed  (CheckIsClosed() == true).
   *
   * \return A value if the asynchronous receive request was accepted, an error otherwise.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The asynchronous receive request was rejected since the
   *                                                             socket is not open and/or there is already an ongoing
   *                                                             asynchronous receive request.
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
   * \vprivate        Vector product internal API
   */
  auto ReceiveFromAsync(::amsr::core::Span<std::uint8_t> buffer_view, ReceiveCompletionCallback&& callback) noexcept
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
   * \pre             Socket is open.
   * \pre             Blocking mode is disabled.
   * \pre             Socket can perform asynchronous operations.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector component internal API
   */
  void OnReactorEvents(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief Close has been called.
   *
   * \details BasicSocket::Close() has been called by the user and the socket is closing. Reset internal state machine.
   *          This function should not release any resources since reactor is still running at that time and it might
   *          call a callback that uses these resources.
   *
   * \context         Reactor context.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
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
   * \return The remote network endpoint.
   *
   * \error osabstraction::OsabErrc::kResource           Not enough system resources to acquire the address.
   * \error osabstraction::OsabErrc::kApiError           The socket is not open.
   * \error osabstraction::OsabErrc::kDisconnected       The socket is not connected.
   * \error osabstraction::OsabErrc::kUnexpected         Unexpected error that is mapped to no category.
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
   *
   * \vprivate        Vector product internal API
   */
  auto GetRemoteEndpoint() noexcept -> ::amsr::core::Result<EndpointType>;

  /*!
   * \brief Returns the maximum number of datagrams that can be received with one ReceiveSyncBulk() or
   *        ReceiveFromSyncBulk() call.
   *
   * \return The maximum number of datagrams.
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
   *
   * \vprivate        Vector product internal API
   *
   * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Receive
   */
  static constexpr auto GetMaxMessagesForBulkReceive() noexcept -> std::uint32_t { return kMaxDatagramsBulkRcv; }

  /*!
   * \brief Returns the maximum number of datagrams that can be sent with one SendSyncBulk() or SendToSyncBulk() call.
   *
   * \return The maximum number of datagrams.
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
   * \trace           DSGN-Osab-UDP_Synchronous_Bulk_Send
   */
  static constexpr auto GetMaxMessagesForBulkSend() noexcept -> std::uint32_t { return kMaxDatagramsBulkSnd; }

 private:
  /*!
   * \brief Moves the content from other socket into this one.
   *
   * \details Moves other socket into this one. Closes this socket if it is still open.
   *
   * \post Other socket is in state closed. This socket contains now the context of other socket.
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
  void MoveDatagramSocket(BasicDatagramSocket&& other) noexcept;

  /*!
   * \brief Prepares the asynchronous send operation.
   *
   * \param[in] io_buffers_view       View to an array of IOVectors. Must consist of at most kMaxIOBufferArraySize
   *                                  entries. The io_buffers_view has to exist for the whole lifetime of the class
   *                                  object.
   * \param[in] completion_callback   Callback to notify the completion of the sendto operations.
   *
   * \context         Any
   *
   * \pre             The socket is open.
   * \pre             The socket has a reactor.
   * \pre             No ongoing send operation.
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void PrepareAsyncSendOperation(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                 SendCompletionCallback&& completion_callback) noexcept;

  /*!
   * \brief Prepares the asynchronous receive operation.
   *
   * \param[in] io_buffers_view       View to an array of IOVectors. Must consist of at most kMaxIOBufferArraySize
   *                                  entries. The io_buffers_view has to exist for the whole lifetime of the class
   *                                  object.
   * \param[in] completion_callback   Callback to notify the completion of the ReceiveFrom operations.
   *
   * \context         Any
   *
   * \pre             The socket is open.
   * \pre             The socket has a reactor.
   * \pre             No ongoing receive operation.
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  void PrepareAsyncReceiveOperation(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
                                    ReceiveCompletionCallback&& completion_callback) noexcept;

  /*!
   * \brief Complete an asynchronous send request
   *
   * \details The reactor notified that a send operation is possible. This function will now try to send the message in
   *          the reactor context.
   *
   * \context         Reactor context.
   *
   * \pre             The socket is open.
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void HandleSendRequest() noexcept;

  /*!
   * \brief Complete an asynchronous receive request
   *
   * \details The reactor notified that a receive operation is possible. This function will now try to receive the
   *          message in the reactor context.
   *
   * \context         Reactor context.
   *
   * \pre             The socket is open.
   * \pre             Blocking mode is disabled.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void HandleReceiveRequest() noexcept;

  /*!
   * \brief Resets the internal send state for asynchronous send operations to idle.
   *
   * \details It is not possible to release ownership and exclusive access rights in this function because there might
   *          be an ongoing reactor call.
   *
   * \context         Reactor context.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void ResetSendRequestState() noexcept;

  /*!
   * \brief Resets the internal send state for asynchronous receive operations to idle.
   *
   * \details It is not possible to release ownership and exclusive access rights in this function because there might
   *          be an ongoing reactor call.
   *
   * \context         Reactor context.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void ResetReceiveRequestState() noexcept;

  /*!
   * \brief Calls operating system function to receive a datagram.
   *
   * \param[in, out] io_buffers_view  View to an array of IOVectors. The io_buffers_view has to exist for the whole
   *                                  lifetime of the class object.
   *
   * \return A struct containing additional information about the received datagram.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   * \error osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                             previous datagram.
   *
   * \context         ANY
   *
   * \pre             The socket is open.
   * \pre             The lock of this object is not locked.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   * */
  auto CallReceiveFrom(::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) const noexcept
      -> ::amsr::core::Result<DatagramInfo>;

  /*!
   * \brief Checks if this socket has an ongoing asynchronous operation.
   *
   * \return  True if this socket has an ongoing asynchronous operation. Otherwise false.
   *
   * \context         Move operation.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto HasOngoingAsyncOperation() const noexcept -> bool;

  /*!
   * \brief Checks if this socket has not an ongoing asynchronous send operation.
   *
   * \return  A value if this socket has no ongoing asynchronous send operation. Otherwise an error.
   *
   * \error osabstraction::OsabErrc::kApiError                    There is an ongoing asynchronous send operation.
   *
   * \context         Reactor context.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckNoOngoingAsyncSendOperation() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Checks if this socket has not an ongoing asynchronous receive operation.
   *
   * \return  A value if this socket has no ongoing asynchronous receive operation. Otherwise an error.
   *
   * \error osabstraction::OsabErrc::kApiError                    There is an ongoing asynchronous receive operation.
   *
   * \context         Reactor context.
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckNoOngoingAsyncReceiveOperation() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Outputs if the socket is connected or not.
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
  auto IsConnected() const noexcept -> bool { return (connection_state_ == BasicDatagramSocketState::kConnected); }

  /*!
   * \brief           Checks whether a remote endpoint is bound (socket is connected).
   *
   * \return  A value if this socket is connected (remote endpoint associated). Otherwise an error.
   *
   * \error osabstraction::OsabErrc::kApiError                    There is no remote endpoint bound (not connected).
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
  auto CheckIsConnected() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Checks whether a remote endpoint is not bound (socket is not connected).
   *
   * \return          A value if this socket is not connected (remote endpoint associated). Otherwise an error.
   *
   * \error osabstraction::OsabErrc::kApiError                    The remote endpoint is already bound (is connected).
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
  auto CheckIsNotConnected() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether that sending is possible.
   *
   * \return          A value if sending is possible, an error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is not open and/or there is an ongoing
   *                                                             asynchronous send operation.
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
  auto CheckSendIsPossible() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether receiving is possible.
   *
   * \return          A value if receiving is possible, an error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is not open and/or there is an ongoing
   *                                                             asynchronous receive operation.
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
  auto CheckReceiveIsPossible() noexcept -> ::amsr::core::Result<void>;

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
   * \brief           Check whether the socket is not prepared for broadcast.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is prepared for broadcast.
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
  auto CheckSocketIsNotPreparedForBroadcast() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Alter the destination endpoint to set proper broadcast address.
   *
   * \param[in]       original_dest_ep   Endpoint with limited broadcast address.
   *
   * \return An Enpoint construction token to construct the correct destination endpoint.
   *
   * \error osabstraction::OsabErrc::kApiError                   Interfaces do not match.
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
  auto AlterDestinationEndpoint(EndpointType const& original_dest_ep) const noexcept -> EndpointToken;

  /*!
   * \brief           Check whether the interface this socket is related to matches another interface.
   *
   * \details         The socket is considered related to an interface if
   *                  - the socket was bound to a unicast address (the interface this address is assigned to).
   *                  - the socket was set up for broadcast (the interface passed for this setup).
   *                  If the socket is not (yet) related to an interface, this check passes.
   *
   * \param[in]       interface   The interface to check.
   *
   * \error osabstraction::OsabErrc::kApiError                   Interfaces do not match.
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
  auto CheckInterface(Interface const& interface) const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether the socket state is suitable to communicate with some destination.
   *
   * \details         If the input destination does not have specific requirements regarding socket state, the check
   *                  succeeds even if the socket is not in a state that allows sending (e.g. not open).
   *
   * \param[in]       destination The destination to communicate with.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is not in the state required to communicate with
   *                                                             this destination.
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
  auto CheckSocketStateMatchesDestination(EndpointType const& destination) const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Stores the connection state.
   */
  BasicDatagramSocketState connection_state_{BasicDatagramSocketState::kDisconnected};

  /*!
   * \brief IO vector with one element
   *
   * \details
   * IO vector for outgoing single buffer requests.
   */
  ::amsr::core::Array<osabstraction::io::ConstIOBuffer, 1> outgoing_single_buffer_io_vector_{};

  /*!
   * \brief View to IO vector array.
   *
   * \details
   * Stores the view to the currently outgoing IO vector.
   */
  ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> outgoing_io_vector_view_{};

  /*!
   * \brief Stores the destination endpoint of an asynchronous send operation or for a connected socket.
   *
   * \details
   * The value is only valid during an ongoing send operation. It might only a value if SendToAsync() or the optimistic
   * SendTo() is used.
   */
  ::amsr::core::Optional<EndpointType> outgoing_destination_{};

  /*!
   * \brief  Stores the callback to be executed on a completed asynchronous connect operation.
   *
   * \details
   * The callback is only valid during asynchronous connect.
   */
  SendCompletionCallback send_completion_callback_{};

  /*!
   * \brief Stores whether there is an ongoing send operation or not.
   */
  bool ongoing_async_send_operation_{false};

  /*!
   * \brief IO vector with one element
   *
   * \details
   * IO vector for incoming single buffer requests.
   */
  ::amsr::core::Array<osabstraction::io::MutableIOBuffer, 1> incoming_single_buffer_io_vector_{};

  /*!
   * \brief View to IO vector array.
   *
   * \details
   * Stores the view to the currently incoming IO vector.
   */
  ::amsr::core::Span<osabstraction::io::MutableIOBuffer> incoming_io_vector_view_{};

  /*!
   * \brief  Stores the callback to be executed on a completed asynchronous connect operation.
   *
   * \details
   * The callback is only valid during asynchronous connect.
   */
  ReceiveCompletionCallback receive_completion_callback_{};

  /*!
   * \brief Stores whether there is an ongoing send operation or not.
   */
  bool ongoing_async_receive_operation_{false};

  /*!
   * \brief Stores whether the socket is already bound to an endpoint.
   *
   * \details No protection against concurrent access is required since the variable is not accessed from
   *          reactor context.
   */
  bool is_socket_bound_to_endpoint_{false};

  /*!
   * \brief Stores broadcast address after calling PrepareBroadcast().
   */
  ::amsr::core::Optional<ip::AddressV4> broadcast_address_{};

  /*!
   * \brief Stores the interface this socket is bound to.
   *
   * \details No protection against concurrent access is required since the variable is not accessed from
   *          reactor context.
   */
  Interface interface_{};
};

/***********************************************************************************************************************
 * Template implementation
 **********************************************************************************************************************/

/*!
 * \internal
 * - Move other socket into this socket.
 * \endinternal
 */
// VECTOR Next Construct AutosarC++17_10-A12.8.4: MD_OSA_A12.8.4_SocketMoveConstructor
template <class Protocol>
BasicDatagramSocket<Protocol>::BasicDatagramSocket(BasicDatagramSocket&& other) noexcept
    : BasicDatagramSocket<Protocol>() {
  this->MoveDatagramSocket(std::move(other));
}

/*!
 * \internal
 * - Call BasicSocket constructor with passed reactor interface.
 * \endinternal
 */
// VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_SocketDelegateConstructor
// VECTOR Next Construct AutosarC++17_10-A12.1.6: MD_OSA_A12.1.6_SocketInheritingConstructor
template <class Protocol>
BasicDatagramSocket<Protocol>::BasicDatagramSocket(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
    : BasicSocket<Protocol, BasicDatagramSocket<Protocol>>(reactor) {}

/*!
 * \internal
 * - Close the socket if it is not closed yet.
 * - Abort if object is still open to prevent access after delete.
 * \endinternal
 */
template <class Protocol>
BasicDatagramSocket<Protocol>::~BasicDatagramSocket() noexcept {
  // Ignore error. We check after the close if close succeeded. If not we have to abort anyway at this point.
  static_cast<void>(this->Close());

  if (!this->CheckIsClosed()) {
    ::amsr::core::Abort("Failed to close socket in destructor. Object has been destroyed while it was still in use.");
  }
}

// VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_SocketBase_MoveOperator
/*!
 * \internal
 * - Check for self assignment.
 * - Move other socket into this socket.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::operator=(BasicDatagramSocket&& other) & noexcept -> BasicDatagramSocket& {
  if (this != &other) {
    this->MoveDatagramSocket(std::move(other));
  }
  return *this;
}

/*!
 * \internal
 * - Enter critical section.
 * - If the state allows preparing for broadcast, the socket is an IPv4 socket, the interface is managed by the network
 *   stack of this socket and the input interface is valid for this socket
 *   - set up for broadcast.
 *   - Store that setup for broadcast has been done.
 * - Leave critical section.
 * - Output the result of setting up for broadcast.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::PrepareBroadcast(Interface const& interface) noexcept
    -> ::amsr::core::Result<void> {
  this->AcquireLock();

  typename BasicDatagramSocket<Protocol>::SockOptBroadcast option{true};

  osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
  ::amsr::core::Result<void> const result{
      this->CheckSocketIsOpen()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckSocketIsNotPreparedForBroadcast(); })
          .AndThen([this]() -> ::amsr::core::Result<void> {
            ::amsr::core::Result<void> res{MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                                         "Socket to set up for broadcast is not a IPv4 socket.")};
            if (this->GetProtocolUnsafe() == BasicDatagramSocket<Protocol>::ProtocolType::V4()) {
              res = ::amsr::core::Result<void>{};
            }
            return res;
          })
          .AndThen([this, &interface]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(interface.GetNetworkStackId());
          })
          .AndThen([this, &interface]() -> ::amsr::core::Result<void> { return CheckInterface(interface); })
          .Inspect([this, &interface]() -> void { this->interface_ = interface; })
          .AndThen([this, &option]() -> ::amsr::core::Result<void> { return this->SetOptionInternal(option); })
          .AndThen([this, &native_handle]() -> ::amsr::core::Result<void> {
            ::amsr::core::Result<void> ret{};
            ::amsr::core::Result<ip::AddressV4> const prepare_broadcast_result{
                os_interface::PrepareBroadcast(native_handle, this->interface_)};
            if (prepare_broadcast_result.HasValue()) {
              this->broadcast_address_.emplace(prepare_broadcast_result.Value());
            } else {
              ret.EmplaceError(prepare_broadcast_result.Error());
            }
            return ret;
          })};

  this->ReleaseLock();

  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - If the state allows binding, the protocol families of socket and endpoint are the same and the interface of the
 *   endpoint matches the socket
 *   - bind the endpoint.
 * - Leave critical section.
 * - If the state does not allow binding and/or the protocol families of socket and endpoint are different and/or the
 *   interface of the endpoint could not be resolved or does not match the socket
 *   - output the respective error.
 * - If a bind was attempted
 *   - output the result of binding.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::Bind(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  ::amsr::core::Result<void> const bind_result{
      this->CheckSocketIsOpen()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckSocketIsNotBoundToEndpoint(); })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            return this->CheckEndpointProtocolMatches(endpoint);
          })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(endpoint.GetNetworkStackId());
          })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            ::amsr::core::Result<void> result{};
            if (this->broadcast_address_.has_value()) {
              ip::Address const addr{endpoint.GetAddress()};
              ::amsr::core::Result<Interface> interface_result{
                  MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error.")};
              // broadcast_address_ can only have value for IPv4 sockets so the above protocol check ensures
              // that the address in the endpoint is an IPv4 address.
              if (addr.IsMulticast()) {
                // IPv4 multicast address requires that the endpoint also contains an Interface. Use this as the
                // multicast address can not be resolved to an interface.
                interface_result = endpoint.GetInterface();
              } else {
                // Create interface from address because the interface in the endpoint is not guaranteed to be the
                // interface to which the address in the endpoint (which is the thing that will be bound) is assigned.
                interface_result = Interface::MakeInterface(addr, endpoint.GetNetworkStackId());
              }
              if (interface_result.HasValue()) {
                result = this->CheckInterface(interface_result.Value());

                if (result.HasValue()) {
                  interface_ = interface_result.Value();
                }
              } else if (interface_result.Error() == osabstraction::OsabErrc::kSystemEnvironmentError) {
                // System environment error of interface creation/extraction means that the address is not available on
                // the local machine. The address not being available is output by bind with the address not available
                // error.
                result.EmplaceError(
                    MakeErrorCode(osabstraction::OsabErrc::kAddressNotAvailable,
                                  "Address to bind to is not assigned to any network interface of the machine."));
              } else if (interface_result.Error() == osabstraction::OsabErrc::kAddressError) {
                // Address error of interface creation/extraction means that the address is assigned to multiple
                // interfaces of the machine. This is output as system environment error by bind.
                result.EmplaceError(
                    MakeErrorCode(osabstraction::OsabErrc::kSystemEnvironmentError,
                                  "Address to bind to is assigned to multiple network interfaces of the machine."));
              } else {
                result.EmplaceError(interface_result.Error());
              }
            }
            return result;
          })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            // The precondition check "Already bound" will be done by the operating system.
            osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
            return os_interface::Bind(native_handle, endpoint.GetData());
          })
          .Inspect([this]() { this->is_socket_bound_to_endpoint_ = true; })};

  this->ReleaseLock();

  return bind_result;
}

/*!
 * \internal
 *  - Enter critical section.
 *  - If the state allows connecting and the input is valid
 *    - If the endpoint address is a multicast address:
 *      - Set the socket option for multicast.
 *    - Alter destination endpoint if needed.
 *    - Connect the socket to the endpoint.
 *    - If connecting the socket succeeded
 *      - set state accordingly.
 *  - Leave critical section.
 *  - If the state did not allow sending and/or the input was invalid
 *    - output the respective error.
 *  - If setting the socket option for multicast failed:
 *    - output the respective error.
 *  - If connecting was attempted
 *    - output the result of connecting.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::Connect(EndpointType const& endpoint) noexcept -> ::amsr::core::Result<void> {
  this->AcquireLock();

  ::amsr::core::Result<void> const connect_result{
      this->CheckSocketIsOpen()
          .AndThen([this]() -> ::amsr::core::Result<void> {
            bool const is_not_connected{!this->IsConnected()};
            // An asynchronous send operation ongoing if the socket is not connected means this asynchronous send
            // operation is a SendTo operation. SendTo* operations are the only possible send operations on not
            // connected sockets. The socket not being connected means the socket was never connected because
            // un-connecting connected sockets is not supported. This means the operation has been started when the
            // socket was not connected which means it must be a SendTo* operation.
            bool const async_sendto_ongoing{this->ongoing_async_send_operation_ && is_not_connected};
            return async_sendto_ongoing
                       ? ::amsr::core::Result<void>{MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                                                  "An asynchronous SendTo operation is in progress.")}
                       : ::amsr::core::Result<void>{};
          })
          .AndThen([&endpoint, this]() -> ::amsr::core::Result<void> {
            return this->CheckEndpointProtocolMatches(endpoint);
          })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(endpoint.GetNetworkStackId());
          })
          .AndThen([this, &endpoint]() -> ::amsr::core::Result<void> {
            return this->CheckSocketStateMatchesDestination(endpoint);
          })
          .AndThen([&endpoint, this]() -> ::amsr::core::Result<void> {
            ::amsr::core::Result<void> option_result{};
            if (endpoint.GetAddress().IsMulticast()) {
              ::amsr::core::Result<Interface> interface {
                endpoint.GetInterface()
              };
              if (interface.HasValue()) {
                ::amsr::core::Result<SockOptMulticastNetworkInterface> option{
                    SockOptMulticastNetworkInterface::MakeSocketOptionIPNetworkInterface(interface.Value(),
                                                                                         endpoint.GetProtocol())};
                if (option.HasValue()) {
                  option_result = this->SetOptionInternal(option.Value());
                } else {
                  option_result.EmplaceError(option.Error());
                }
              } else {
                option_result.EmplaceError(interface.Error());
              }
            }
            return option_result;
          })
          .AndThen([&endpoint, this]() -> ::amsr::core::Result<void> {
            osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
            EndpointType const dest_endpoint{this->AlterDestinationEndpoint(endpoint)};
            return os_interface::SetDefaultPeer(native_handle, dest_endpoint.GetData());
          })
          .Inspect([this]() {
            this->connection_state_ = BasicDatagramSocketState::kConnected;
            this->is_socket_bound_to_endpoint_ = true;  // Connect does an implicit bind.
          })};

  this->ReleaseLock();

  return connect_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check if the state of the connection allows sending to the default destination.
 * - Leave critical section.
 * - If the state of the connection does not allow sending to the default destination
 *   - output the respective error.
 * - If the state of the connection does allow sending
 *   - call the operating system service to send the data.
 *   - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendSync(
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view) noexcept -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendSync-ConstIOBuffer
  this->AcquireLock();

  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> send_result{
      this->CheckSendIsPossible().AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsConnected(); })};

  this->ReleaseLock();

  if (send_result.HasValue()) {
    send_result = os_interface::SendDatagram(this->GetNativeHandle(), io_buffers_view);
  }

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendSync-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check if the state of the connection allows sending to the default destination.
 * - Leave critical section.
 * - If the state of the connection does not allow sending to the default destination
 *   - output the respective error.
 * - If the state of the connection does allow sending
 *   - call the operating system service to send the data.
 *   - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendSync(::amsr::core::Span<std::uint8_t const> buffer_view) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendSync-Span
  this->AcquireLock();
  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const send_possible{
      this->CheckSendIsPossible().AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsConnected(); })};

  this->ReleaseLock();

  ::amsr::core::Result<void> send_result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error, always overwritten.")};

  if (send_possible.HasValue()) {
    ::amsr::core::Array<osabstraction::io::ConstIOBuffer, 1> const send_buffer{
        {osabstraction::io::ConvertSpanToConstIOBuffer(buffer_view)}};
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> const send_buffer_view{send_buffer};

    osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};

    send_result = os_interface::SendDatagram(native_handle, send_buffer_view);
  } else {
    send_result = send_possible;
  }

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendSync-Span
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Check if the state of the connection allows sending to the default destination.
 * - Leave critical section.
 * - If one of above checks failed
 *   - output the respective error.
 * - Else if the number of supplied datagrams is below the minimum limit, or if the
 *   minimum limit is greater than the maximum limit.
 *   - Send only the first message.
 *   - Output the result of sending.
 * - Else:
 *   - If the number supplied datagrams exceeds the maximum limit truncate it to that limit.
 *   - Set up the bulk send data structures and send prepared messages.
 *   - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendSyncBulk(
    ::amsr::core::Span<::amsr::core::Span<osabstraction::io::ConstIOBuffer>> datagrams) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendSyncBulk-ConstIOBuffer
  this->AcquireLock();

  ::amsr::core::Result<void> const send_possible{
      this->CheckSendIsPossible().AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsConnected(); })};

  this->ReleaseLock();

  ::amsr::core::Result<std::size_t> send_result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error, always overwritten.")};

  if (send_possible.HasValue()) {
    bool use_sendmmsg{kMinDatagramsBulkSnd <= kMaxDatagramsBulkSnd};
    // VECTOR NL Compiler-#186: MD_OSA_Compiler-#186_sendmmsg_limit_configuration_by_macro
    if (datagrams.size() < kMinDatagramsBulkSnd) {
      use_sendmmsg = false;
    } else if (datagrams.size() > kMaxDatagramsBulkSnd) {
      datagrams = datagrams.subspan(0, kMaxDatagramsBulkSnd);
    } else {
      // Default already assigned: use sendmmsg().
    }
    if (datagrams.empty()) {
      send_result = ::amsr::core::Result<std::size_t>{0};
    } else if (use_sendmmsg) {
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<BulkBufferType, kMaxDatagramsBulkSnd> messages;
      // Using size of input span as size for this span to ensure only array elements overwritten in SetupBulkBuffer are
      // read.
      ::amsr::core::Span<BulkBufferType> const messages_view{messages.data(), datagrams.size()};

      for (size_t i{0}; i < datagrams.size(); i++) {
        this->AssertProperIOBufferArraySize(datagrams[i].size());

        BulkBufferType& current_message_view{messages_view[i]};
        ::amsr::core::Span<osabstraction::io::ConstIOBuffer> const current_outgoing_io_buffers_view{datagrams[i]};
        os_interface::SetupBulkBuffer(current_message_view, current_outgoing_io_buffers_view,
                                      ::amsr::core::Optional<SocketMutableAddrView>{});
      }

      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      send_result = os_interface::SendToBulk(native_handle, messages_view);
    } else {
      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      ::amsr::core::Result<void> const single_send_result{os_interface::SendDatagram(native_handle, datagrams[0])};
      send_result = single_send_result.HasValue() ? ::amsr::core::Result<std::size_t>{1}
                                                  : ::amsr::core::Result<std::size_t>{single_send_result.Error()};
    }
  } else {
    send_result = ::amsr::core::Result<std::size_t>{send_possible.Error()};
  }

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendSyncBulk-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Check if the state of the connection allows sending the default destination.
 * - Leave critical section.
 * - If one of above checks failed
 *   - output the respective error.
 * - Else if the number of supplied datagrams is below the minimum limit, or if the
 *   minimum limit is greater than the maximum limit.
 *   - Send only the first message.
 *   - Output the result of sending.
 * - Else:
 *   - If the number supplied datagrams exceeds the maximum limit truncate it to that limit.
 *   - Set up the bulk send data structures and send prepared messages.
 *   - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendSyncBulk(
    ::amsr::core::Span<::amsr::core::Span<std::uint8_t const>> datagrams) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendSyncBulk-Span
  this->AcquireLock();

  ::amsr::core::Result<void> const send_possible{
      this->CheckSendIsPossible().AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsConnected(); })};

  this->ReleaseLock();

  ::amsr::core::Result<std::size_t> send_result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error, always overwritten.")};

  if (send_possible.HasValue()) {
    bool use_sendmmsg{kMinDatagramsBulkSnd <= kMaxDatagramsBulkSnd};
    // VECTOR NL Compiler-#186: MD_OSA_Compiler-#186_sendmmsg_limit_configuration_by_macro
    if (datagrams.size() < kMinDatagramsBulkSnd) {
      use_sendmmsg = false;
    } else if (datagrams.size() > kMaxDatagramsBulkSnd) {
      datagrams = datagrams.subspan(0, kMaxDatagramsBulkSnd);
    } else {
      // Default already assigned: use sendmmsg().
    }
    if (datagrams.empty()) {
      send_result = ::amsr::core::Result<std::size_t>{0};
    } else if (use_sendmmsg) {
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<osabstraction::io::ConstIOBuffer, kMaxDatagramsBulkSnd> outgoing_io_buffers;
      ::amsr::core::Span<osabstraction::io::ConstIOBuffer> const outgoing_io_buffers_view{outgoing_io_buffers.data(),
                                                                                          datagrams.size()};
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<BulkBufferType, kMaxDatagramsBulkSnd> messages;
      // Using size of input span as size for this span to ensure only array elements overwritten in SetupBulkBuffer are
      // read.
      ::amsr::core::Span<BulkBufferType> const messages_view{messages.data(), datagrams.size()};

      for (size_t i{0}; i < datagrams.size(); i++) {
        this->AssertProperIOBufferSize(datagrams[i].size());
        outgoing_io_buffers_view[i].base_pointer = datagrams[i].data();
        outgoing_io_buffers_view[i].size = datagrams[i].size() * sizeof(std::uint8_t);

        BulkBufferType& current_message_view{messages_view[i]};
        ::amsr::core::Span<osabstraction::io::ConstIOBuffer> const current_outgoing_io_buffers_view{
            &(outgoing_io_buffers_view[i]), 1};
        os_interface::SetupBulkBuffer(current_message_view, current_outgoing_io_buffers_view,
                                      ::amsr::core::Optional<SocketMutableAddrView>{});
      }

      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      send_result = os_interface::SendToBulk(native_handle, messages_view);
    } else {
      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      ::amsr::core::Array<osabstraction::io::ConstIOBuffer, 1> const send_buffer{
          {osabstraction::io::ConvertSpanToConstIOBuffer(datagrams[0])}};
      ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> const send_buffer_view{send_buffer};
      ::amsr::core::Result<void> const single_send_result{os_interface::SendDatagram(native_handle, send_buffer_view)};
      send_result = single_send_result.HasValue() ? ::amsr::core::Result<std::size_t>{1}
                                                  : ::amsr::core::Result<std::size_t>{single_send_result.Error()};
    }
  } else {
    send_result = ::amsr::core::Result<std::size_t>{send_possible.Error()};
  }

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendSyncBulk-Span
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows sending to the default destination
 *   - set up the object to conduct the asynchronous send operation to the default destination.
 * - Leave critical section.
 * - If the object state does not allow sending to the default destination
 *   - output the respective error.
 * - If the object state allowed sending to the default destination
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendAsync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                              SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendAsync-ConstIOBuffer
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> const send_result{
      this->CheckSendIsPossible()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsConnected(); })
          .Inspect([this, &io_buffers_view, &callback]() {
            this->outgoing_destination_.reset();  // use default destination
            this->PrepareAsyncSendOperation(io_buffers_view, std::move(callback));
          })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendAsync-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows sending to the default destination
 *   - create IOVector with single entry.
 *   - Set up the object to conduct the asynchronous send operation to the default destination.
 * - Leave critical section.
 * - If the object state does not allow sending to the default destination
 *   - output the respective error.
 * - If the object state allowed sending to the default destination
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendAsync(::amsr::core::Span<std::uint8_t const> buffer_view,
                                              SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendAsync-Span
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const send_result{
      this->CheckSendIsPossible()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsConnected(); })
          .Inspect([this, &buffer_view, &callback]() {
            this->outgoing_single_buffer_io_vector_[0] = osabstraction::io::ConvertSpanToConstIOBuffer(buffer_view);
            this->outgoing_destination_.reset();  // use default destination

            this->PrepareAsyncSendOperation(
                ::amsr::core::Span<osabstraction::io::ConstIOBuffer>{this->outgoing_single_buffer_io_vector_},
                std::move(callback));
          })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendAsync-Span
  return send_result;
}

/*!
 * \internal
 * - Try synchronous send.
 * - If the synchronous send operation succeeded
 *   - output that the send operation is complete.
 * - If the synchronous send operation is busy
 *   - try asynchronous send.
 *   - If the asynchronous send operation could be started
 *     - output that the send operation is ongonig.
 *   - If the asynchronous send operation could not be started
 *     - output the error that made it fail.
 * - If the synchronous send operation failed
 *   - output the error that made it fail.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::Send(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                         SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<osabstraction::io::SendResult> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-Send-ConstIOBuffer
  // No need for critical section here because this function itself does not access the object attributes. The called
  // functions (SendSync, SendAsync) that do access the attributes have critical sections.

  // Preconditions are verified by called SendSync() and SendAsync().

  // Dummy error.
  ::amsr::core::Result<osabstraction::io::SendResult> result{osabstraction::OsabErrc::kApiError};
  ::amsr::core::Result<void> const result_sync_send{this->SendSync(io_buffers_view)};
  if (result_sync_send.HasValue()) {
    result.EmplaceValue(osabstraction::io::SendResult::kSendCompleted);
  } else if (result_sync_send == osabstraction::OsabErrc::kBusy) {
    ::amsr::core::Result<void> const result_send_async{this->SendAsync(io_buffers_view, std::move(callback))};
    if (result_send_async.HasValue()) {
      result.EmplaceValue(osabstraction::io::SendResult::kAsyncProcessingNecessary);
    }
  } else {
    result.EmplaceError(result_sync_send.Error());
  }
  // PTP-E-OsAbstraction-UDP-DataTransfer-Send-ConstIOBuffer
  return result;
}

/*!
 * \internal
 * - Try synchronous send.
 * - If the synchronous send operation succeeded
 *   - output that the send operation is complete.
 * - If the synchronous send operation is busy
 *   - try asynchronous send.
 *   - If the asynchronous send operation could be started
 *     - output that the send operation is ongonig.
 *   - If the asynchronous send operation could not be started
 *     - output the error that made it fail.
 * - If the synchronous send operation failed
 *   - output the error that made it fail.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::Send(::amsr::core::Span<std::uint8_t const> buffer_view,
                                         SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<osabstraction::io::SendResult> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-Send-Span
  // No need for critical section here because this function itself does not access the object attributes. The called
  // functions (SendSync, SendAsync) that do access the attributes have critical sections.

  // Preconditions are verified by called SendSync() and SendAsync().

  // Dummy error.
  ::amsr::core::Result<osabstraction::io::SendResult> result{osabstraction::OsabErrc::kApiError};
  ::amsr::core::Result<void> const result_sync_send{this->SendSync(buffer_view)};
  if (result_sync_send.HasValue()) {
    result.EmplaceValue(osabstraction::io::SendResult::kSendCompleted);
  } else if (result_sync_send == osabstraction::OsabErrc::kBusy) {
    ::amsr::core::Result<void> const result_send_async{this->SendAsync(buffer_view, std::move(callback))};
    if (result_send_async.HasValue()) {
      result.EmplaceValue(osabstraction::io::SendResult::kAsyncProcessingNecessary);
    } else {
      result.EmplaceError(result_send_async.Error());
    }
  } else {
    result.EmplaceError(result_sync_send.Error());
  }
  // PTP-E-OsAbstraction-UDP-DataTransfer-Send-Span
  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check if the state of the connection allows sending to a non-default destination and the input is valid for that
 *   and that the destination endpoint uses the same network stack as this socket and that the socket state allows to
 *   send to the input destination.
 * - Leave critical section.
 * - If the state of the connection does not allow sending to a non-default destination and/or the input is invalid
 *   and/or the destination endpoint uses a different network stack as this socket.
 *   - output the respective error.
 * - Otherwise
 *   - Alter destination endpoint if needed.
 *   - call the operating system service to send the data.
 *   - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendToSync(
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> io_buffers_view,
    EndpointType const& destination) noexcept -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendToSync-ConstIOBuffer
  this->AcquireLock();

  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> const send_possible{
      this->CheckSendIsPossible()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsNotConnected(); })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckEndpointProtocolMatches(destination);
          })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(destination.GetNetworkStackId());
          })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckSocketStateMatchesDestination(destination);
          })};

  this->ReleaseLock();

  ::amsr::core::Result<void> send_result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error, always overwritten.")};

  if (send_possible.HasValue()) {
    EndpointType const dest_endpoint{this->AlterDestinationEndpoint(destination)};
    osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
    send_result = os_interface::SendTo(native_handle, io_buffers_view, dest_endpoint.GetData());
  } else {
    send_result = send_possible;
  }

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendToSync-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check if the state of the connection allows sending to a non-default destination and the input is valid for that
 *   and that the destination endpoint uses the same network stack as this socket and that the socket state allows to
 *   send to the input destination..
 * - Leave critical section.
 * - If the state of the connection does not allow sending to a non-default destination and/or the input is invalid
 *   - output the respective error.
 * - Otherwise
 *   - Alter destination endpoint if needed.
 *   - call the operating system service to send the data.
 *   - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendToSync(::amsr::core::Span<std::uint8_t const> buffer_view,
                                               EndpointType const& destination) noexcept -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendToSync-Span
  this->AcquireLock();

  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const send_possible{
      this->CheckSendIsPossible()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsNotConnected(); })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckEndpointProtocolMatches(destination);
          })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(destination.GetNetworkStackId());
          })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckSocketStateMatchesDestination(destination);
          })};

  this->ReleaseLock();

  ::amsr::core::Result<void> send_result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error, always overwritten.")};

  if (send_possible.HasValue()) {
    ::amsr::core::Array<osabstraction::io::ConstIOBuffer, 1> const send_buffer{
        {osabstraction::io::ConvertSpanToConstIOBuffer(buffer_view)}};
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> const send_buffer_view{send_buffer};

    osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
    EndpointType const dest_endpoint{this->AlterDestinationEndpoint(destination)};
    send_result = os_interface::SendTo(native_handle, send_buffer_view, dest_endpoint.GetData());
  } else {
    send_result = send_possible;
  }

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendToSync-Span
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Check if the state of the connection allows sending to a non-default destination and the input is valid for that.
 * - Alter destination endpoints if needed.
 * - Leave critical section.
 * - If one of above checks failed
 *   - output the respective error.
 * - Else if the number of supplied datagrams is below the minimum limit, or if the
 *   minimum limit is greater than the maximum limit.
 *   - Send only the first message.
 *   - Output the result of sending.
 * - Else:
 *   - If the number supplied datagrams exceeds the maximum limit truncate it to that limit.
 *   - Set up the bulk send data structures and send prepared messages.
 *   - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendToSyncBulk(
    ::amsr::core::Span<std::pair<::amsr::core::Span<osabstraction::io::ConstIOBuffer>, EndpointType>>
        datagrams) noexcept -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendToSyncBulk-ConstIOBuffer
  this->AcquireLock();

  ::amsr::core::Result<void> const send_possible{
      this->CheckSendIsPossible()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsNotConnected(); })
          .AndThen([this, &datagrams]() -> ::amsr::core::Result<void> {
            ::amsr::core::Result<void> protocol_check_result{};
            for (std::size_t i{0}; i < datagrams.size(); i++) {
              protocol_check_result = this->CheckEndpointProtocolMatches(datagrams[i].second);
              if (!protocol_check_result) {
                break;
              }
              protocol_check_result = this->CheckSocketUsesNetworkStack(datagrams[i].second.GetNetworkStackId());
              if (!protocol_check_result) {
                break;
              }
              EndpointType destination{this->AlterDestinationEndpoint(datagrams[i].second)};
              datagrams[i].second = std::move(destination);
            }
            return protocol_check_result;
          })};

  this->ReleaseLock();

  ::amsr::core::Result<std::size_t> send_result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error, always overwritten.")};

  if (send_possible.HasValue()) {
    bool use_sendmmsg{kMinDatagramsBulkSnd <= kMaxDatagramsBulkSnd};
    // VECTOR NL Compiler-#186: MD_OSA_Compiler-#186_sendmmsg_limit_configuration_by_macro
    if (datagrams.size() < kMinDatagramsBulkSnd) {
      use_sendmmsg = false;
    } else if (datagrams.size() > kMaxDatagramsBulkSnd) {
      datagrams = datagrams.subspan(0, kMaxDatagramsBulkSnd);
    } else {
      // Default already assigned: use sendmmsg().
    }
    if (datagrams.empty()) {
      send_result = ::amsr::core::Result<std::size_t>{0};
    } else if (use_sendmmsg) {
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<BulkBufferType, kMaxDatagramsBulkSnd> messages;
      // Using size of input span as size for this span to ensure only array elements overwritten in SetupBulkBuffer are
      // read.
      ::amsr::core::Span<BulkBufferType> const messages_view{messages.data(), datagrams.size()};

      for (size_t i{0}; i < datagrams.size(); i++) {
        this->AssertProperIOBufferArraySize(datagrams[i].first.size());

        BulkBufferType& current_message_view{messages_view[i]};
        ::amsr::core::Span<osabstraction::io::ConstIOBuffer> const current_outgoing_io_buffers_view{datagrams[i].first};
        EndpointType& current_endpoint{datagrams[i].second};
        os_interface::SetupBulkBuffer(current_message_view, current_outgoing_io_buffers_view,
                                      current_endpoint.GetData());
      }

      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      send_result = os_interface::SendToBulk(native_handle, messages_view);
    } else {
      typename Protocol::Endpoint const& ep{datagrams[0].second};
      SocketConstAddrView const socket_address{ep.GetData()};

      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      ::amsr::core::Result<void> const single_send_result{
          os_interface::SendTo(native_handle, datagrams[0].first, socket_address)};
      send_result = single_send_result.HasValue() ? ::amsr::core::Result<std::size_t>{1}
                                                  : ::amsr::core::Result<std::size_t>{single_send_result.Error()};
    }
  } else {
    send_result = ::amsr::core::Result<std::size_t>{send_possible.Error()};
  }

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendToSyncBulk-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Check if the state of the connection allows sending to a non-default destination and the input is valid for that.
 * - Alter destination endpoints if needed.
 * - Leave critical section.
 * - If one of above checks failed
 *   - output the respective error.
 * - Else if the number of supplied datagrams is below the minimum limit, or if the
 *   minimum limit is greater than the maximum limit.
 *   - Send only the first message.
 *   - Output the result of sending.
 * - Else:
 *   - If the number supplied datagrams exceeds the maximum limit truncate it to that limit.
 *   - Set up the bulk send data structures and send prepared messages.
 *   - Output the result of sending.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendToSyncBulk(
    ::amsr::core::Span<std::pair<::amsr::core::Span<std::uint8_t const>, EndpointType>> datagrams) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendToSyncBulk-Span
  this->AcquireLock();

  ::amsr::core::Result<void> const send_possible{
      this->CheckSendIsPossible()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsNotConnected(); })
          .AndThen([this, &datagrams]() -> ::amsr::core::Result<void> {
            ::amsr::core::Result<void> protocol_check_result{};
            for (std::size_t i{0}; i < datagrams.size(); i++) {
              protocol_check_result = this->CheckEndpointProtocolMatches(datagrams[i].second);
              if (!protocol_check_result) {
                break;
              }
              protocol_check_result = this->CheckSocketUsesNetworkStack(datagrams[i].second.GetNetworkStackId());
              if (!protocol_check_result) {
                break;
              }
              EndpointType destination{this->AlterDestinationEndpoint(datagrams[i].second)};
              datagrams[i].second = std::move(destination);
            }
            return protocol_check_result;
          })};

  this->ReleaseLock();

  ::amsr::core::Result<std::size_t> send_result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError, "Dummy error, always overwritten.")};

  if (send_possible.HasValue()) {
    bool use_sendmmsg{kMinDatagramsBulkSnd <= kMaxDatagramsBulkSnd};
    // VECTOR NL Compiler-#186: MD_OSA_Compiler-#186_sendmmsg_limit_configuration_by_macro
    if (datagrams.size() < kMinDatagramsBulkSnd) {
      use_sendmmsg = false;
    } else if (datagrams.size() > kMaxDatagramsBulkSnd) {
      datagrams = datagrams.subspan(0, kMaxDatagramsBulkSnd);
    } else {
      // Default already assigned: use sendmmsg().
    }
    if (datagrams.empty()) {
      send_result = ::amsr::core::Result<std::size_t>{0};
    } else if (use_sendmmsg) {
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<osabstraction::io::ConstIOBuffer, kMaxDatagramsBulkSnd> outgoing_io_buffers;
      ::amsr::core::Span<osabstraction::io::ConstIOBuffer> const outgoing_io_buffers_view{outgoing_io_buffers.data(),
                                                                                          datagrams.size()};
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<BulkBufferType, kMaxDatagramsBulkSnd> messages;
      // Using size of input span as size for this span to ensure only array elements overwritten in SetupBulkBuffer are
      // read.
      ::amsr::core::Span<BulkBufferType> const messages_view{messages.data(), datagrams.size()};

      for (size_t i{0}; i < datagrams.size(); i++) {
        this->AssertProperIOBufferSize(datagrams[i].first.size());
        outgoing_io_buffers_view[i].base_pointer = datagrams[i].first.data();
        outgoing_io_buffers_view[i].size = datagrams[i].first.size() * sizeof(std::uint8_t);

        BulkBufferType& current_message_view{messages_view[i]};
        ::amsr::core::Span<osabstraction::io::ConstIOBuffer> const current_outgoing_io_buffers_view{
            &(outgoing_io_buffers_view[i]), 1};
        EndpointType& current_endpoint{datagrams[i].second};
        os_interface::SetupBulkBuffer(current_message_view, current_outgoing_io_buffers_view,
                                      current_endpoint.GetData());
      }

      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      send_result = os_interface::SendToBulk(native_handle, messages_view);
    } else {
      ::amsr::core::Array<osabstraction::io::ConstIOBuffer, 1> const send_buffer{
          {osabstraction::io::ConvertSpanToConstIOBuffer(datagrams[0].first)}};
      ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> const send_buffer_view{send_buffer};

      typename Protocol::Endpoint const& ep{datagrams[0].second};

      osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
      ::amsr::core::Result<void> const single_send_result{
          os_interface::SendTo(native_handle, send_buffer_view, ep.GetData())};
      send_result = single_send_result.HasValue() ? ::amsr::core::Result<std::size_t>{1}
                                                  : ::amsr::core::Result<std::size_t>{single_send_result.Error()};
    }
  } else {
    send_result = ::amsr::core::Result<std::size_t>{send_possible.Error()};
  }

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendToSyncBulk-Span
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows sending to a non-default destination and the input is valid for that and the destination
 *   endpoint uses the same network stack as this socket and the socket state allows to send to the input destination
 *   - Alter destination endpoint if needed.
 *   - set up the object to conduct the asynchronous send operation to a non-default destination.
 * - Leave critical section.
 * - If the object state does not allow sending to a non-default destination or the input is not valid for that or the
 *   destination endpoint uses a different endpoint as this socket
 *   - output the respective error.
 * - Otherwise
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendToAsync(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                                EndpointType const& destination,
                                                SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendToAsync-ConstIOBuffer
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> const send_result{
      this->CheckSendIsPossible()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsNotConnected(); })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckEndpointProtocolMatches(destination);
          })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(destination.GetNetworkStackId());
          })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckSocketStateMatchesDestination(destination);
          })
          .Inspect([this, &io_buffers_view, &destination, &callback]() {
            EndpointType const dest_endpoint{this->AlterDestinationEndpoint(destination)};
            this->outgoing_destination_.emplace(dest_endpoint);
            this->PrepareAsyncSendOperation(io_buffers_view, std::move(callback));
          })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendToAsync-ConstIOBuffer
  return send_result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - If the object state allows sending to a non-default destination and the input is valid for that
 *   - create IOVector with single entry.
 *   - Alter destination endpoint if needed.
 *   - Set up the object to conduct the asynchronous send operation to a non-default destination.
 * - Leave critical section.
 * - If the object state does not allow sending to a non-default destination or the input is not valid for that or the
 *   destination endpoint uses a different network stack as this socket
 *   - output the respective error.
 * - Otherwise
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendToAsync(::amsr::core::Span<std::uint8_t const> buffer_view,
                                                EndpointType const& destination,
                                                SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendToAsync-Span
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const send_result{
      this->CheckSendIsPossible()
          .AndThen([this]() -> ::amsr::core::Result<void> { return this->CheckIsNotConnected(); })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckEndpointProtocolMatches(destination);
          })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckSocketUsesNetworkStack(destination.GetNetworkStackId());
          })
          .AndThen([this, &destination]() -> ::amsr::core::Result<void> {
            return this->CheckSocketStateMatchesDestination(destination);
          })
          .Inspect([this, &buffer_view, &destination, &callback]() {
            this->outgoing_single_buffer_io_vector_[0] = osabstraction::io::ConvertSpanToConstIOBuffer(buffer_view);
            EndpointType const dest_endpoint{this->AlterDestinationEndpoint(destination)};
            this->outgoing_destination_.emplace(dest_endpoint);

            this->PrepareAsyncSendOperation(
                ::amsr::core::Span<osabstraction::io::ConstIOBuffer>{this->outgoing_single_buffer_io_vector_},
                std::move(callback));
          })};

  this->ReleaseLock();

  // PTP-E-OsAbstraction-UDP-DataTransfer-SendToAsync-Span
  return send_result;
}

/*!
 * \internal
 * - Try synchronous send.
 * - If the synchronous send operation succeeded
 *   - output that the send operation is complete.
 * - If the synchronous send operation is busy
 *   - try asynchronous send.
 *   - If the asynchronous send operation could be started
 *     - output that the send operation is ongoing.
 *   - If the asynchronous send operation could not be started
 *     - output the error that made it fail.
 * - If the synchronous send operation failed
 *   - output the error that made it fail.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendTo(::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
                                           EndpointType const& destination, SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<osabstraction::io::SendResult> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendTo-ConstIOBuffer
  // No need for critical section here because this function itself does not access the object attributes. The called
  // functions (SendSync, SendAsync) that do access the attributes have critical sections.

  // Preconditions are verified by called SendSync() and SendAsync().

  // Dummy error.
  ::amsr::core::Result<osabstraction::io::SendResult> result{osabstraction::OsabErrc::kApiError};
  ::amsr::core::Result<void> const result_sync_send{this->SendToSync(io_buffers_view, destination)};
  if (result_sync_send.HasValue()) {
    result.EmplaceValue(osabstraction::io::SendResult::kSendCompleted);
  } else if (result_sync_send == osabstraction::OsabErrc::kBusy) {
    ::amsr::core::Result<void> const result_send_async{
        this->SendToAsync(io_buffers_view, destination, std::move(callback))};
    if (result_send_async.HasValue()) {
      result.EmplaceValue(osabstraction::io::SendResult::kAsyncProcessingNecessary);
    } else {
      result.EmplaceError(result_send_async.Error());
    }
  } else {
    result.EmplaceError(result_sync_send.Error());
  }
  // PTP-E-OsAbstraction-UDP-DataTransfer-SendTo-ConstIOBuffer
  return result;
}

/*!
 * \internal
 * - Try synchronous send.
 * - If the synchronous send operation succeeded
 *   - output that the send operation is complete.
 * - If the synchronous send operation is busy
 *   - try asynchronous send.
 *   - If the asynchronous send operation could be started
 *     - output that the send operation is ongoing.
 *   - If the asynchronous send operation could not be started
 *     - output the error that made it fail.
 * - If the synchronous send operation failed
 *   - output the error that made it fail.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::SendTo(::amsr::core::Span<std::uint8_t const> buffer_view,
                                           EndpointType const& destination, SendCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<osabstraction::io::SendResult> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-SendTo-Span
  // No need for critical section here because this function itself does not access the object attributes. The called
  // functions (SendSync, SendAsync) that do access the attributes have critical sections.

  // Preconditions are verified by called SendSync() and SendAsync().

  // Dummy error.
  ::amsr::core::Result<osabstraction::io::SendResult> result{osabstraction::OsabErrc::kApiError};
  ::amsr::core::Result<void> const result_sync_send{this->SendToSync(buffer_view, destination)};
  if (result_sync_send.HasValue()) {
    result.EmplaceValue(osabstraction::io::SendResult::kSendCompleted);
  } else if (result_sync_send == osabstraction::OsabErrc::kBusy) {
    ::amsr::core::Result<void> const result_send_async{
        this->SendToAsync(buffer_view, destination, std::move(callback))};
    if (result_send_async.HasValue()) {
      result.EmplaceValue(osabstraction::io::SendResult::kAsyncProcessingNecessary);
    } else {
      result.EmplaceError(result_send_async.Error());
    }
  } else {
    result.EmplaceError(result_sync_send.Error());
  }
  // PTP-E-OsAbstraction-UDP-DataTransfer-SendTo-Span
  return result;
}

/*!
 * \internal
 * - Set state to sending.
 * - Store IOBuffer view, destination and completion callback.
 * - Enable Reactor write supervision.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::PrepareAsyncSendOperation(
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer> io_buffers_view,
    SendCompletionCallback&& completion_callback) noexcept {
  this->ongoing_async_send_operation_ = true;

  this->outgoing_io_vector_view_ = io_buffers_view;

  // Only store the callback after the state has been set to sending to avoid the callback being moved in a concurrent
  // move operation (which is anyway prohibited because the move operation of this class is not thread-safe) after it
  // was stored but before the state was set.
  this->send_completion_callback_ = std::move(completion_callback);

  this->ChangeWriteObservation(true);
}

/*!
 * \internal
 * - Verify the precondition.
 * - Inspect the reactor event.
 * - Call the send sub service on write event.
 * - Call the receive sub service on read event.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::OnReactorEvents(osabstraction::io::reactor1::EventTypes events) noexcept {
  // PTP-B-OsAbstraction-UDP-ReactorTask
  this->AssertLockHeldByThread();

  if (events.HasWriteEvent()) {
    if (ongoing_async_send_operation_) {
      this->HandleSendRequest();
    }
  }
  if (events.HasReadEvent()) {
    // Attributes must be re-checked because the lock has been released and re-acquired in HandleSendRequest().
    if (this->CheckSocketIsOpen().HasValue() && ongoing_async_receive_operation_) {
      this->HandleReceiveRequest();
    }
  }
  // PTP-E-OsAbstraction-UDP-ReactorTask
}

/*!
 * \internal
 * - Verify the precondition.
 * - Reset send state machine.
 * - Reset receive state machine.
 * - Set the state to disconnected.
 * - Set bound socket to false;
 * - Set broadcast socket to false;
 * - Reset interface.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::OnCloseEvent() noexcept {
  this->AssertLockHeldByThread();

  this->ResetSendRequestState();
  this->ResetReceiveRequestState();

  connection_state_ = BasicDatagramSocketState::kDisconnected;
  this->is_socket_bound_to_endpoint_ = false;
  this->broadcast_address_.reset();
  this->interface_ = Interface{};
}

/*!
 * \internal
 * - Return basic socket functionality.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::GetLocalEndpoint() noexcept -> ::amsr::core::Result<EndpointType> {
  return BasicSocket<Protocol, BasicDatagramSocket<Protocol>>::GetSocketLocalEndpoint();
}

/*!
 * \internal
 * - Return basic socket functionality.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::GetPhysicalAddress() noexcept -> ::amsr::core::Result<amsr::net::EthernetAddress> {
  return BasicSocket<Protocol, BasicDatagramSocket<Protocol>>::GetSocketPhysicalAddress();
}

/*!
 * \internal
 * - Enter critical section.
 * - Resolve remote endpoint.
 * - Leave critical section.
 * - Report the result of resolving the endpoint.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::GetRemoteEndpoint() noexcept -> ::amsr::core::Result<EndpointType> {
  this->AcquireLock();
  ::amsr::core::Result<EndpointType> const result{this->GetEndpoint(&os_interface::GetPeerName)};
  this->ReleaseLock();
  return result;
}

/*!
 * \internal
 * - Reset the send operation's internal state.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::ResetSendRequestState() noexcept {
  this->ongoing_async_send_operation_ = false;
}

/*!
 * \internal
 * - Reset the receive operation's internal state.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::ResetReceiveRequestState() noexcept {
  this->ongoing_async_receive_operation_ = false;
}

/*!
 * \internal
 * - Execute send request.
 * - If no spurious wake-up.
 *    - Reset send request state.
 *    - Release the lock.
 *    - Call completion callback.
 *    - Re-acquire the lock.
 *    - If the socket is still open and no new send operation was requested
 *      - disable write monitoring.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::HandleSendRequest() noexcept {
  this->AssertBlockingModeIsDisabled();

  // Lock is held during system call because
  // - this is a non-blocking call which thus does not take long.
  // - not releasing the lock after the verification that the socket is in non-blocking mode ensures that this call can
  //   not block even if the user tried to enable blocking mode with an ongoing asynchronous operation.

  ::amsr::core::Result<void> result{osabstraction::OsabErrc::kApiError};  // dummy value
  if (this->outgoing_destination_.has_value()) {
    EndpointType const& destination{this->outgoing_destination_.value()};
    result = os_interface::SendTo(this->GetNativeHandle(), this->outgoing_io_vector_view_, destination.GetData());
  } else {
    result = os_interface::SendDatagram(this->GetNativeHandle(), this->outgoing_io_vector_view_);
  }

  bool const spurious_wake{result == osabstraction::OsabErrc::kBusy};

  if (!spurious_wake) {
    // Take callback to local variable before resetting the send state to avoid a race condition with a concurrent
    // move (which is prohibited because the move operation of this class is not thread-safe) that could take place
    // after the state was reset but before the callback was taken to a local variable.
    SendCompletionCallback cb{std::move(this->send_completion_callback_)};

    this->ResetSendRequestState();

    this->ReleaseLock();
    // PTP-B-OsAbstraction-UDP-DataTransfer-SendCompletionCallback
    cb(std::move(result));
    // PTP-E-OsAbstraction-UDP-DataTransfer-SendCompletionCallback
    this->AcquireLock();

    // Explicitly check if there is another send request because the lock was released during the callback so another
    // request could have been submitted.
    if (this->CheckSocketIsOpen().HasValue() && (!this->ongoing_async_send_operation_)) {
      // No new request. Disable monitoring.
      this->ChangeWriteObservation(false);
    }
  }
}

/*!
 * \internal
 * - Execute receive request.
 * - If no spurious wake-up.
 *    - Reset receive request state.
 *    - Release the lock.
 *    - Call completion callback.
 *    - Re-acquire the lock.
 *    - If the socket is still open and no new receive operation was requested
 *      - disable read monitoring.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::HandleReceiveRequest() noexcept {
  this->AssertBlockingModeIsDisabled();

  // Lock is held during system call because
  // - this is a non-blocking call which thus does not take long.
  // - not releasing the lock after the verification that the socket is in non-blocking mode ensures that this call can
  //   not block even if the user tried to enable blocking mode with an ongoing asynchronous operation.

  ::amsr::core::Result<DatagramInfo> result{this->CallReceiveFrom(this->incoming_io_vector_view_)};

  bool const spurious_wake{result == osabstraction::OsabErrc::kBusy};

  if (!spurious_wake) {
    // Take callback to local variable before resetting the receive state to avoid a race condition with a concurrent
    // move (which is prohibited because the move operation of this class is not thread-safe) that could take place
    // after the state was reset but before the callback was taken to a local variable.
    ReceiveCompletionCallback cb{std::move(this->receive_completion_callback_)};  // NOLINT(vector-object-not-changed)

    this->ResetReceiveRequestState();

    this->ReleaseLock();
    // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveCompletionCallback
    cb(std::move(result));
    // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveCompletionCallback
    this->AcquireLock();

    // Explicitly check if there is another receive request because the lock was released during the callback so another
    // request could have been submitted.
    if (this->CheckSocketIsOpen().HasValue() && (!this->ongoing_async_receive_operation_)) {
      // No new request. Disable monitoring.
      this->ChangeReadObservation(false);
    }
  }
}

/*!
 * \internal
 * - Call ReceiveFromSync and wrap the result.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::ReceiveSync(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveSync-MutableIOBuffer

  // No critical section required because this function does not access object attributes directly and ReceiveFromSync
  // has a critical section that protects access to the attributes done by it.
  ::amsr::core::Result<DatagramInfo> receive_from_result{this->ReceiveFromSync(io_buffers_view)};

  ::amsr::core::Result<std::size_t> const result{
      ((receive_from_result.HasValue()) ? ::amsr::core::Result<std::size_t>{receive_from_result.Value().datagram_size}
                                        : ::amsr::core::Result<std::size_t>{receive_from_result.Error()})};
  // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveSync-MutableIOBuffer
  return result;
}

/*!
 * \internal
 * - Call ReceiveFromSync and wrap the result.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::ReceiveSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveSync-Span

  // No critical section required because this function does not access object attributes directly and ReceiveFromSync
  // has a critical section that protects access to the attributes done by it.
  ::amsr::core::Result<DatagramInfo> receive_from_result{this->ReceiveFromSync(buffer_view)};

  ::amsr::core::Result<std::size_t> const result{
      ((receive_from_result.HasValue()) ? ::amsr::core::Result<std::size_t>{receive_from_result.Value().datagram_size}
                                        : ::amsr::core::Result<std::size_t>{receive_from_result.Error()})};
  // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveSync-Span
  return result;
}

/*!
 * \internal
 * - Ensure buffers_view's size does not exceed a maximum number of datagrams, truncate it if required.
 * - Enter critical section.
 * - Check if the object state allows receive.
 * - Leave critical section.
 * - If the object state allows receiving:
 *  - If the number of supplied datagrams is below the minimum limit, or if the
 *    minimum limit is greater than the maximum limit.
 *    - Receive only the first message.
 *    - Output the result of reception.
 *  - Else:
 *    - If the number supplied datagrams exceeds the maximum limit truncate it to that limit.
 *    - Zero-initialize messages buffer.
 *    - Prepare buffer structures: incoming_io_buffers (scatter buffer for received datagrams) and
 *      messages (to store incoming datagrams).
 *      For each datagram:
 *        - Assert proper datagram size.
 *        - Prepare a scatter buffer (span of IObuffers) to store received datagrams in buffers_view.
 *        - Setup BulkBufferType message structure with values of the scatter buffer.
 *    - If no error occurred: Save each datagrams' sizes in outgoing buffers_view.
 *    - Output the result of reception.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::ReceiveSyncBulk(
    ::amsr::core::Span<std::pair<::amsr::core::Span<std::uint8_t>, size_t>> buffers_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveSyncBulk
  ::amsr::core::Result<std::size_t> result{osabstraction::OsabErrc::kUnexpected};  // dummy error

  this->AcquireLock();
  ::amsr::core::Result<void> const receive_is_possible_result{this->CheckReceiveIsPossible()};
  this->ReleaseLock();

  bool use_recvmmsg{kMinDatagramsBulkRcv <= kMaxDatagramsBulkRcv};
  // VECTOR NL Compiler-#186: MD_OSA_Compiler-#186_recvmmsg_limit_configuration_by_macro
  if (buffers_view.size() < kMinDatagramsBulkRcv) {
    use_recvmmsg = false;
  } else if (buffers_view.size() > kMaxDatagramsBulkRcv) {
    buffers_view = buffers_view.subspan(0, kMaxDatagramsBulkRcv);
  } else {
    // Intentionally left empty.
  }

  // No critical section required here because accesses are only to attributes that are not protected.
  if (receive_is_possible_result.HasValue()) {
    if (buffers_view.empty()) {
      result = ::amsr::core::Result<std::size_t>{0};
    } else if (use_recvmmsg) {
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<BulkBufferType, kMaxDatagramsBulkRcv> messages;
      // Using size of input span as size for this span to ensure only array elements overwritten in SetupBulkBuffer are
      // read.
      ::amsr::core::Span<BulkBufferType> const messages_view{messages.data(), buffers_view.size()};
      ::amsr::core::Array<osabstraction::io::MutableIOBuffer, kMaxDatagramsBulkRcv> incoming_io_buffers{};
      ::amsr::core::Span<osabstraction::io::MutableIOBuffer> const incoming_io_buffers_view{incoming_io_buffers.data(),
                                                                                            buffers_view.size()};

      for (size_t i{0}; i < buffers_view.size(); i++) {
        this->AssertProperIOBufferSize(buffers_view[i].first.size());
        incoming_io_buffers_view[i].base_pointer = buffers_view[i].first.data();
        incoming_io_buffers_view[i].size = buffers_view[i].first.size() * sizeof(std::uint8_t);
        os_interface::SetupBulkBuffer(messages_view[i], incoming_io_buffers_view[i],
                                      ::amsr::core::Optional<SocketMutableAddrView>{});
      }
      bool const is_blocking_mode_enabled{this->IsBlockingModeEnabled()};
      ::amsr::core::Result<std::size_t> const receive_result{
          os_interface::ReceiveFromBulk(this->GetNativeHandle(), is_blocking_mode_enabled, messages_view)};
      if (receive_result.HasValue()) {
        for (size_t j{0}; j < receive_result.Value(); j++) {
          buffers_view[j].second = os_interface::GetDatagramSize(messages_view[j]);
        }
        result.EmplaceValue(receive_result.Value());
      } else {
        result.EmplaceError(receive_result.Error());
      }
    } else {
      ::amsr::core::Array<osabstraction::io::MutableIOBuffer, 1> receive_buffer{
          {osabstraction::io::ConvertSpanToMutableIOBuffer(buffers_view[0].first)}};
      ::amsr::core::Span<osabstraction::io::MutableIOBuffer> const receive_buffer_view{receive_buffer};
      ::amsr::core::Result<DatagramInfo> const single_receive_result{this->CallReceiveFrom(receive_buffer_view)};
      if (single_receive_result.HasValue()) {
        buffers_view[0].second = single_receive_result.Value().datagram_size;
        result.EmplaceValue(1);
      } else {
        result.EmplaceError(single_receive_result.Error());
      }
    }
  } else {
    result.EmplaceError(receive_is_possible_result.Error());
  }
  // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveSyncBulk
  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the precondition.
 * - Store span in internal IOBuffer.
 * - Check if peeking is possilbe with the current state of the object.
 * - Leave critical section.
 * - If peek is possible
 *   - peek datagram.
 *   - Output the result of peeking.
 * - If peek is not possible
 *   - output the respective error.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::PeekSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-PeekSync
  this->AcquireLock();

  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const receive_is_possible_result{this->CheckReceiveIsPossible()};

  ::amsr::core::Array<osabstraction::io::MutableIOBuffer, 1> incoming_io_vector{
      {osabstraction::io::ConvertSpanToMutableIOBuffer(buffer_view)}};

  osabstraction::io::NativeHandle const native_handle{this->GetNativeHandle()};
  ::amsr::core::Span<osabstraction::io::MutableIOBuffer> const receive_view{incoming_io_vector.data(),
                                                                            incoming_io_vector.size()};

  this->ReleaseLock();

  ::amsr::core::Result<std::size_t> const result{
      (receive_is_possible_result.HasValue()) ? os_interface::PeekDatagram(native_handle, receive_view)
                                              : ::amsr::core::Result<std::size_t>{receive_is_possible_result.Error()}};
  // PTP-E-OsAbstraction-UDP-DataTransfer-PeekSync
  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check if the object state allows receive.
 * - Leave critical section.
 * - If the object state does not allow receive
 *   - output the respective error.
 * - If the object state allows receive
 *   - do a synchronous receive operation.
 *   - Output the result of that operation.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::ReceiveFromSync(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) noexcept
    -> ::amsr::core::Result<DatagramInfo> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveFromSync-MutableIOBuffer
  this->AcquireLock();

  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> const receive_is_possible_result{this->CheckReceiveIsPossible()};

  this->ReleaseLock();

  ::amsr::core::Result<DatagramInfo> const result{
      (receive_is_possible_result.HasValue()) ? this->CallReceiveFrom(io_buffers_view)
                                              : ::amsr::core::Result<DatagramInfo>{receive_is_possible_result.Error()}};
  // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveFromSync-MutableIOBuffer
  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions.
 * - Check if the object state allows receive.
 * - Leave critical section.
 * - If the object state does not allow receive
 *   - output the respective error.
 * - If the object state allows receive
 *   - do a synchronous receive operation.
 *   - Output the result of that operation.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::ReceiveFromSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept
    -> ::amsr::core::Result<DatagramInfo> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveFromSync-Span
  this->AcquireLock();

  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Array<osabstraction::io::MutableIOBuffer, 1> incoming_io_vector{
      {osabstraction::io::ConvertSpanToMutableIOBuffer(buffer_view)}};

  ::amsr::core::Result<DatagramInfo> result{osabstraction::OsabErrc::kUnexpected};  // dummy error
  ::amsr::core::Result<void> const receive_is_possible_result{this->CheckReceiveIsPossible()};

  ::amsr::core::Span<osabstraction::io::MutableIOBuffer> const receive_view{incoming_io_vector};

  this->ReleaseLock();

  if (receive_is_possible_result.HasValue()) {
    result = this->CallReceiveFrom(receive_view);
  } else {
    result.EmplaceError(receive_is_possible_result.Error());
  }
  // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveFromSync-Span
  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Check if the object state allows receive.
 * - Leave critical section.
 * - If the object state allows receiving:
 *  - If the number of supplied datagrams is below the minimum limit, or if the
 *    minimum limit is greater than the maximum limit.
 *    - Receive only the first message.
 *    - Output the result of reception.
 *  - Else:
 *    - If the number supplied datagrams exceeds the maximum limit truncate it to that limit.
 *    - Zero-initialize messages buffer.
 *    - Prepare buffer structures: incoming_io_buffers (scatter buffer for received datagrams) and
 *      messages (to store incoming datagrams).
 *      For each datagram:
 *        - Assert proper datagram size.
 *        - Prepare a scatter buffer (span of IObuffers) to store received datagrams in buffers_view.
 *        - Set socket's protocol to the endpoint of DatagramInfo.
 *        - Setup BulkBufferType message structure with values of the scatter buffer and the endpoint.
 *    - Do a synchronous receive operation to receive a bulk of messages.
 *    - If no error occurred: Save datagrams' sizes in outgoing buffers_view.
 *    - Output the result of reception.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::ReceiveFromSyncBulk(
    ::amsr::core::Span<std::pair<::amsr::core::Span<std::uint8_t>, DatagramInfo>> buffers_view) noexcept
    -> ::amsr::core::Result<std::size_t> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveFromSyncBulk
  ::amsr::core::Result<std::size_t> result{osabstraction::OsabErrc::kUnexpected};  // dummy error

  this->AcquireLock();
  ::amsr::core::Result<void> const receive_is_possible_result{this->CheckReceiveIsPossible()};
  this->ReleaseLock();

  bool use_recvmmsg{kMinDatagramsBulkRcv <= kMaxDatagramsBulkRcv};
  // VECTOR NL Compiler-#186: MD_OSA_Compiler-#186_recvmmsg_limit_configuration_by_macro
  if (buffers_view.size() < kMinDatagramsBulkRcv) {
    use_recvmmsg = false;
  } else if (buffers_view.size() > kMaxDatagramsBulkRcv) {
    buffers_view = buffers_view.subspan(0, kMaxDatagramsBulkRcv);
  } else {
    // Intentionally left empty.
  }

  // No critical section required here because accesses are only to attributes that are not protected.
  if (receive_is_possible_result.HasValue()) {
    if (buffers_view.empty()) {
      result = ::amsr::core::Result<std::size_t>{0};
    } else if (use_recvmmsg) {
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<osabstraction::io::MutableIOBuffer, kMaxDatagramsBulkRcv> incoming_io_buffers;
      ::amsr::core::Span<osabstraction::io::MutableIOBuffer> const incoming_io_buffers_view{incoming_io_buffers.data(),
                                                                                            buffers_view.size()};
      // VECTOR Next Line VectorC++-V8.5.0: MD_OSA_V8.5.0_expensive_initialization_before_overwrite
      ::amsr::core::Array<BulkBufferType, kMaxDatagramsBulkRcv> messages;
      // Using size of input span as size for this span to ensure only array elements overwritten in SetupBulkBuffer are
      // read.
      ::amsr::core::Span<BulkBufferType> const messages_view{messages.data(), buffers_view.size()};

      for (size_t i{0}; i < buffers_view.size(); i++) {
        this->AssertProperIOBufferSize(buffers_view[i].first.size());
        incoming_io_buffers_view[i].base_pointer = buffers_view[i].first.data();
        incoming_io_buffers_view[i].size = buffers_view[i].first.size() * sizeof(std::uint8_t);

        Protocol const prot{this->GetProtocolUnsafe()};
        buffers_view[i].second.remote_endpoint = EndpointType{prot, this->GetNetworkStackId()};

        BulkBufferType& current_message_view{messages_view[i]};
        osabstraction::io::MutableIOBuffer& current_incoming_io_buffers_view{incoming_io_buffers_view[i]};
        EndpointType& current_endpoint{buffers_view[i].second.remote_endpoint};
        os_interface::SetupBulkBuffer(current_message_view, current_incoming_io_buffers_view,
                                      current_endpoint.GetData());
      }
      bool const is_blocking_mode_enabled{this->IsBlockingModeEnabled()};
      ::amsr::core::Result<std::size_t> const receive_result{
          os_interface::ReceiveFromBulk(this->GetNativeHandle(), is_blocking_mode_enabled, messages_view)};

      if (receive_result.HasValue()) {
        for (size_t j{0}; j < receive_result.Value(); j++) {
          std::size_t const datagram_size{os_interface::GetDatagramSize(messages_view[j])};
          buffers_view[j].second.datagram_size = datagram_size;
        }
        result.EmplaceValue(receive_result.Value());
      } else {
        result.EmplaceError(receive_result.Error());
      }
    } else {
      ::amsr::core::Array<osabstraction::io::MutableIOBuffer, 1> receive_buffer{
          {osabstraction::io::ConvertSpanToMutableIOBuffer(buffers_view[0].first)}};
      ::amsr::core::Span<osabstraction::io::MutableIOBuffer> const receive_buffer_view{receive_buffer};
      ::amsr::core::Result<DatagramInfo> const single_receive_result{this->CallReceiveFrom(receive_buffer_view)};
      if (single_receive_result.HasValue()) {
        buffers_view[0].second.remote_endpoint = single_receive_result.Value().remote_endpoint;
        buffers_view[0].second.datagram_size = single_receive_result.Value().datagram_size;
        result.EmplaceValue(1);
      } else {
        result.EmplaceError(single_receive_result.Error());
      }
    }
  } else {
    result.EmplaceError(receive_is_possible_result.Error());
  }
  // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveFromSyncBulk
  return result;
}

/*!
 * \internal
 * - Receive datagram.
 * - If receiving succeeded
 *   - output the source endpoint and amount of received data.
 * - If receiving failed
 *   - output the error that made it fail.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CallReceiveFrom(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view) const noexcept
    -> ::amsr::core::Result<DatagramInfo> {
  // No critical section required here because accesses are only to attributes that are not protected (protocol and
  // native handle).
  DatagramInfo info{EndpointType{this->GetProtocolUnsafe(), this->GetNetworkStackId()}, 0};

  ::amsr::core::Result<std::size_t> receive_result{
      os_interface::ReceiveFrom(this->GetNativeHandle(), io_buffers_view, info.remote_endpoint.GetData())};

  ::amsr::core::Result<DatagramInfo> result{osabstraction::OsabErrc::kUnexpected};
  if (receive_result.HasValue()) {
    info.datagram_size = receive_result.Value();
    // Remote endpoint has been filled during ReceiveFrom call.
    result.EmplaceValue(info);
  } else {
    result.EmplaceError(receive_result.Error());
  }

  return result;
}

/*!
 * \internal
 * - Set state to receiving.
 * - Store IOBuffer view, destination and completion callback.
 * - Enable Reactor read supervision.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::PrepareAsyncReceiveOperation(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
    ReceiveCompletionCallback&& completion_callback) noexcept {
  this->ongoing_async_receive_operation_ = true;

  this->incoming_io_vector_view_ = io_buffers_view;

  // Only store the callback after the state has been set to receiving to avoid the callback being moved in a concurrent
  // move operation (which is anyway prohibited because the move operation of this class is not thread-safe) after it
  // was stored but before the state was set.
  this->receive_completion_callback_ = std::move(completion_callback);

  this->ChangeReadObservation(true);
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions
 * - If the object state allows receiving
 *   - set the object up for an asynchronous receive operation.
 * - Leave critical section.
 * - If the object state does not allow receiving
 *   - output the respective error.
 * - If the object state allows receiving
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::ReceiveFromAsync(
    ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
    ReceiveCompletionCallback&& callback) noexcept -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveFromAsync-MutableIOBuffer
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferArraySize(io_buffers_view.size());

  ::amsr::core::Result<void> const result{this->CheckReceiveIsPossible()};
  if (result.HasValue()) {
    this->PrepareAsyncReceiveOperation(io_buffers_view, std::move(callback));
  }

  this->ReleaseLock();

  // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveFromAsync-MutableIOBuffer
  return result;
}

/*!
 * \internal
 * - Enter critical section.
 * - Verify the preconditions
 * - If the object state allows receiving
 *   - store an IOBuffer view from the passed span.
 *   - Set the object up for an asynchronous receive operation.
 * - Leave critical section.
 * - If the object state does not allow receiving
 *   - output the respective error.
 * - If the object state allows receiving
 *   - output success.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::ReceiveFromAsync(::amsr::core::Span<std::uint8_t> buffer_view,
                                                     ReceiveCompletionCallback&& callback) noexcept
    -> ::amsr::core::Result<void> {
  // PTP-B-OsAbstraction-UDP-DataTransfer-ReceiveFromAsync-Span
  this->AcquireLock();

  this->AssertReactorAvailable();
  this->AssertBlockingModeIsDisabled();
  this->AssertProperIOBufferSize(buffer_view.size());

  ::amsr::core::Result<void> const result{this->CheckReceiveIsPossible()};
  if (result.HasValue()) {
    this->incoming_single_buffer_io_vector_[0] = osabstraction::io::ConvertSpanToMutableIOBuffer(buffer_view);

    this->PrepareAsyncReceiveOperation(
        ::amsr::core::Span<osabstraction::io::MutableIOBuffer>{this->incoming_single_buffer_io_vector_},
        std::move(callback));
  }

  this->ReleaseLock();

  // PTP-E-OsAbstraction-UDP-DataTransfer-ReceiveFromAsync-Span
  return result;
}

/*!
 * \internal
 * - Assert that no asynchronous operation has been scheduled.
 * - Move the socket.
 * \endinternal
 */
template <class Protocol>
void BasicDatagramSocket<Protocol>::MoveDatagramSocket(BasicDatagramSocket&& other) noexcept {
  BasicDatagramSocketState const others_state{other.connection_state_};
  other.connection_state_ = BasicDatagramSocketState::kDisconnected;
  // Abort on ongoing asynchronous operation. The socket is not thread safe. The behavior is undefined.
  bool const this_has_ongoing_async_operation{this->HasOngoingAsyncOperation()};
  bool const other_has_ongoing_async_operation{other.HasOngoingAsyncOperation()};
  if (this_has_ongoing_async_operation || other_has_ongoing_async_operation) {
    ::amsr::core::Abort("Tried to move socket with scheduled asynchronous operation.");
  }

  this->is_socket_bound_to_endpoint_ = other.is_socket_bound_to_endpoint_;
  this->broadcast_address_ = std::move(other.broadcast_address_);
  this->interface_ = std::move(other.interface_);
  // This socket will be closed during the base class move.
  this->MoveSocket(std::move(other));
  this->connection_state_ = others_state;

  // No need to move additional attributes. They are only valid during an ongoing asynchronous operation.
}

/*!
 * \internal
 * - Output whether there is an ongoing send operation or and ongoing receive operation.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::HasOngoingAsyncOperation() const noexcept -> bool {
  return this->ongoing_async_send_operation_ || this->ongoing_async_receive_operation_;
}

/*!
 * \internal
 * - If there is no ongoing asynchronous send operation:
 *   - Return a value.
 * - Otherwise:
 *   - Return an kApiError error.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckNoOngoingAsyncSendOperation() const noexcept -> ::amsr::core::Result<void> {
  return !this->ongoing_async_send_operation_
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{
                   MakeErrorCode(osabstraction::OsabErrc::kApiError, "There is an ongoing asynchronous send request")};
}

/*!
 * \internal
 * - If there is no ongoing asynchronous receive operation:
 *   - Return a value.
 * - Otherwise:
 *   - Return an kApiError error.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckNoOngoingAsyncReceiveOperation() const noexcept -> ::amsr::core::Result<void> {
  return !this->ongoing_async_receive_operation_
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                                        "There is an ongoing asynchronous receive request")};
}

/*!
 * \internal
 * - Check if the socket is connected. If connected:
 *   - Return a value.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckIsConnected() const noexcept -> ::amsr::core::Result<void> {
  return (this->IsConnected()) ? ::amsr::core::Result<void>{}
                               : ::amsr::core::Result<void>{MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                                                          "Tried to send from an unconnected socket.")};
}

/*!
 * \internal
 * - Check if the socket is not connected. If not connected:
 *   - Return a value.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckIsNotConnected() const noexcept -> ::amsr::core::Result<void> {
  return !this->IsConnected()
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{MakeErrorCode(
                   osabstraction::OsabErrc::kApiError,
                   "Tried to send to a certain address but the socket is already connected to an address.")};
}

/*!
 * \internal
 * - Check the following conditions. If all are met, return a value, an error otherwise:
 *   - Socket is open
 *   - There is no asynchronous send operation in progress
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckSendIsPossible() noexcept -> ::amsr::core::Result<void> {
  return this->CheckSocketIsOpen().AndThen(
      [this]() -> ::amsr::core::Result<void> { return this->CheckNoOngoingAsyncSendOperation(); });
}

/*!
 * \internal
 * - Check the following conditions. If all are met, return a value, an error otherwise:
 *   - Socket is open
 *   - There is no asynchronous receive operation in progress
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckReceiveIsPossible() noexcept -> ::amsr::core::Result<void> {
  return this->CheckSocketIsOpen().AndThen(
      [this]() -> ::amsr::core::Result<void> { return this->CheckNoOngoingAsyncReceiveOperation(); });
}

/*!
 * \internal
 * - Check if the socket is not bound to an endpoint. If true:
 *   - Return a value.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckSocketIsNotBoundToEndpoint() const noexcept -> ::amsr::core::Result<void> {
  return !this->is_socket_bound_to_endpoint_ ? ::amsr::core::Result<void>{}
                                             : ::amsr::core::Result<void>{MakeErrorCode(
                                                   osabstraction::OsabErrc::kApiError, "Socket is bound to endpoint")};
}

/*!
 * \internal
 * - Check if the socket is not set up for broadcast. If true:
 *   - Return a value.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckSocketIsNotPreparedForBroadcast() const noexcept
    -> ::amsr::core::Result<void> {
  return !this->broadcast_address_.has_value()
             ? ::amsr::core::Result<void>{}
             : ::amsr::core::Result<void>{
                   MakeErrorCode(osabstraction::OsabErrc::kApiError, "Socket is prepared for broadcast.")};
}

/*!
 * \internal
 * - Check if the input interface matches the interface this socket is related to. If true:
 *   - Return a value.
 * - Otherwise:
 *   - Return an error.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckInterface(Interface const& interface) const noexcept
    -> ::amsr::core::Result<void> {
  ::amsr::core::Result<void> result{
      MakeErrorCode(osabstraction::OsabErrc::kApiError,
                    "Input interface does not match interface the socket is already related to.")};

  bool const is_stored_interface{this->interface_ == interface};
  if ((this->interface_ == Interface{}) || is_stored_interface) {
    result = ::amsr::core::Result<void>{};
  }
  return result;
}

/*!
 * \internal
 * - Extract original destination endpoint token.
 * - Check if broadcast address is set and the original destination endpoint address is limited broadcast.
 *   - If true, assign the broadcast address to endpoint token's address info.
 * - Otherwise:
 *   - Return the original destination endpoint token.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::AlterDestinationEndpoint(EndpointType const& original_dest_ep) const noexcept
    -> EndpointToken {
  EndpointToken token{original_dest_ep.GetToken()};
  ip::Address const adr{original_dest_ep.GetAddress()};

  bool const broadcast_addr_set{this->broadcast_address_.has_value()};
  bool const is_v4_addr{adr.IsV4()};
  bool const is_limited_broadcast{adr.IsLimitedBroadcast()};
  if (broadcast_addr_set && is_v4_addr && is_limited_broadcast) {
    token.ipv4_information.sin_addr.s_addr = this->broadcast_address_.value().GetNative().s_addr;
  }
  return token;
}

/*!
 * \internal
 * - If the destination address is a limited broadcast address
 *   - if the socket is bound and prepared for broadcast
 *     - return a value.
 *   - If the socket is not bound and/or not prepared for broadcast
 *     - return an error.
 * - If the destination address is not a limited broadcast address
 *   - return a value.
 * \endinternal
 */
template <class Protocol>
auto BasicDatagramSocket<Protocol>::CheckSocketStateMatchesDestination(EndpointType const& destination) const noexcept
    -> ::amsr::core::Result<void> {
  return (destination.GetAddress().IsLimitedBroadcast() &&
          ((!this->broadcast_address_.has_value()) || (!this->is_socket_bound_to_endpoint_)))
             ? ::amsr::core::Result<void>{MakeErrorCode(osabstraction::OsabErrc::kApiError,
                                                        "Socket is not bound and/or prepared for broadcast.")}
             : ::amsr::core::Result<void>{};
}

/*!
 * \exclusivearea ::amsr::net::internal::BasicSocket::lock_::mutex_
 * Ensures atomicity when jointly accessing object state and reactor event settings for data transfer operations.
 * \protects ::amsr::net::internal::BasicSocket::reactor_,
 *           ::amsr::net::internal::BasicSocket::socket_state_,
 *           ::amsr::net::internal::BasicSocket::cached_blocking_mode_,
 *           ::amsr::net::internal::BasicDatagramSocket::outgoing_desination_,
 *           ::amsr::net::internal::BasicDatagramSocket::ongoing_async_send_operation_,
 *           ::amsr::net::internal::BasicDatagramSocket::ongoing_async_receive_operation_,
 *           ::amsr::net::internal::BasicDatagramSocket::outgoing_io_vector_view_,
 *           ::amsr::net::internal::BasicDatagramSocket::incoming_io_vector_view_,
 *           ::amsr::net::internal::BasicDatagramSocket::outgoing_single_buffer_io_vector_,
 *           ::amsr::net::internal::BasicDatagramSocket::incoming_single_buffer_io_vector_,
 *           ::amsr::net::internal::BasicDatagramSocket::send_completion_callback_,
 *           ::amsr::net::internal::BasicDatagramSocket::receive_completion_callback_,
 *           ::amsr::net::internal::BasicDatagramSocket::connection_state_,
 *           ::amsr::net::internal::BasicDatagramSocket::broadcast_address_,
 *           ::amsr::net::internal::BasicDatagramSocket::is_socket_bound_to_endpoint_,
 *           ::amsr::net::internal::BasicDatagramSocket::interface_
 * \usedin   ::amsr::net::internal::BasicDatagramSocket::PrepareBroadcast,
 *           ::amsr::net::internal::BasicDatagramSocket::Bind,
 *           ::amsr::net::internal::BasicDatagramSocket::Connect,
 *           ::amsr::net::internal::BasicDatagramSocket::SendSync,
 *           ::amsr::net::internal::BasicDatagramSocket::SendAsync,
 *           ::amsr::net::internal::BasicDatagramSocket::SendToSync,
 *           ::amsr::net::internal::BasicDatagramSocket::SendToAsync,
 *           ::amsr::net::internal::BasicDatagramSocket::PeekSync,
 *           ::amsr::net::internal::BasicDatagramSocket::ReceiveFromSync,
 *           ::amsr::net::internal::BasicDatagramSocket::ReceiveFromAync,
 *           ::amsr::net::internal::BasicDatagramSocket::OnCloseEvent,
 *           ::amsr::net::internal::BasicDatagramSocket::IsSocketBound,
 *           ::amsr::net::internal::BasicDatagramSocket::PrepareAsyncSendOperation,
 *           ::amsr::net::internal::BasicDatagramSocket::PrepareAsyncReceiveOperation,
 *           ::amsr::net::internal::BasicDatagramSocket::ResetSendRequestState,
 *           ::amsr::net::internal::BasicDatagramSocket::ResetReceiveRequestState,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckNoOngoingAsyncSendOperation,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckNoOngoingAsyncReceiveOperation,
 *           ::amsr::net::internal::BasicDatagramSocket::IsConnected,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckIsConnected,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckIsNotConnected,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckSendIsPossible,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckReceiveIsPossible,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckSocketIsNotBoundToEndpoint,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckSocketIsNotPreparedForBroadcast,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckInterface,
 *           ::amsr::net::internal::BasicDatagramSocket::CheckSocketStateMatchesDestination,
 *           ::amsr::net::internal::BasicDatagramSocket::AlterDestinationEndpoint,
 *           ::amsr::net::internal::BasicDatagramSocket::OnReactorEvents,
 *           ::amsr::net::internal::BasicDatagramSocket::HandleSendRequest,
 *           ::amsr::net::internal::BasicDatagramSocket::HandleReceiveRequest
 * \exclude  All other of these methods of the same object.
 * \length   LONG Enabling and disabling events on the reactor.
 *
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_DATAGRAM_SOCKET_H_

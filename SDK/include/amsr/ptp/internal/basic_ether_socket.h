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
 *        \brief  Implements a precision time protocol socket.
 *         \unit  amsr::ptp::Socket
 *   \complexity  amsr::ptp::Socket unit is QM, no action required.
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_BASIC_ETHER_SOCKET_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_BASIC_ETHER_SOCKET_H_

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <utility>
#include "amsr/core/array.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/net_utils/network_stack_id.h"
#include "amsr/ptp/interface.h"
#include "amsr/ptp/internal/ethernet_endpoint.h"
#include "amsr/ptp/types.h"
#include "ara/core/vector.h"
#include "osabstraction/io/common_types.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/native_types.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/osab_error_domain.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ptp {
namespace internal {

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_BasicEtherSocket
/*!
 * \brief Socket that implements the precision time protocol
 *
 */
class BasicEtherSocket final {
 public:
  /*!
   * \brief Maximum supported IOBuffer array size.
   *
   * \details The implementation needs one I/O Buffer entry.
   */
  static constexpr std::size_t kMaxIOBufferArraySize{osabstraction::io::kMaxIOBufferArraySize - 1};

  /*!
   *  \brief Maximum size for one I/O buffer.
   */
  static constexpr std::size_t kMaxIOBufferSize{osabstraction::io::kMaxIOBufferSize};

  /*!
   * \brief Stores additional information about the received Ethernet packet.
   */
  struct ReceivedPacketInfo {
    /*!
     * \brief Stores whether the operation could be completed immediately or will be continued in the background.
     *
     * \details
     * This value is always ReceiveResult::kReceiveCompleted for synchronous services.
     */
    osabstraction::io::ReceiveResult operation_result{};

    /*!
     * \brief The sender's network endpoint.
     */
    Endpoint remote_endpoint;

    /*!
     * \brief Received packet's size.
     */
    std::size_t packet_size{};

    /*!
     * \brief Reception time in nanoseconds.
     *
     * \details
     * Does not contain a value if the Ethernet packet has been received without any timestamp.
     */
    ::amsr::core::Optional<ReceiveTimestamp> receive_timestamp;
  };

  /*!
   * \brief Stores additional information about the sent Ethernet packet.
   */
  struct SentPacketInfo {
    /*!
     * \brief Token that can be used to poll for the time stamp of this sent packet.
     */
    SendTimestampToken timestamp_id{};
  };

  /*!
   * \brief Defines how to generate timestamps.
   */
  using TimestampMode = amsr::ptp::internal::TimestampMode;

  /*!
   * \brief Stores information about a received send timestamp.
   */
  using SendTimestamp = amsr::ptp::internal::SendTimestamp;

  /*!
   * \brief Stores information about a received receive timestamp.
   */
  using ReceiveTimestamp = amsr::ptp::internal::ReceiveTimestamp;

  /*!
   * \brief Token to identify a send time stamp.
   */
  using SendTimestampToken = amsr::ptp::internal::SendTimestampToken;

  /*!
   * \brief Notifies about a completed asynchronous receive operation.
   *
   * \param[in] result The result of the receive operation. On success it contains additional information about the
   *                   received Ethernet packet.
   *
   * Possible errors:
   *        osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   *        osabstraction::OsabErrc::kResource                   Not enough system resources to receive Ethernet packet.
   *        osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive Ethernet packet.
   *        osabstraction::OsabErrc::kDisconnected               Asynchronous network error, e.g. network interface down
   *                                                             or not available anymore.
   *        osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                             received Ethernet packet. Packet has been dropped.
   *        osabstraction::OsabErrc::kSystemEnvironmentError     Device dependent IO error occurred.
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
  using ReceiveCompletionCallback =
      vac::language::UniqueFunction<void(::amsr::core::Result<ReceivedPacketInfo>&& result)>;

  // VECTOR NC AutosarC++17_10-A12.1.6: MD_OSA_A12.1.6_ConstructorInheritanceNotSupportedByQCC
  /*!
   * \brief Construct an Ethernet packet socket for synchronous and asynchronous communication.
   *
   * \param[in] reactor                   Reactor interface for asynchronous operations.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   */
  explicit BasicEtherSocket(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept;

  /*!
   * \brief     Deleted move operator.
   */
  auto operator=(BasicEtherSocket&& other) & noexcept -> BasicEtherSocket& = delete;

  /*!
   * \brief     Deleted copy operator.
   */
  auto operator=(BasicEtherSocket const& other) & noexcept -> BasicEtherSocket& = delete;

  /*!
   * \brief     Deleted move constructor.
   */
  BasicEtherSocket(BasicEtherSocket&& other) = delete;

  /*!
   * \brief Deleted copy constructor
   */
  BasicEtherSocket(BasicEtherSocket const& other) = delete;

  /*!
   * \brief Destroys a socket.
   *
   * \details         No ongoing asynchronous call is allowed during destruction.
   *                  User has to avoid to destruct the BasicEtherSocket object while the internal reactor callback is
   *                  executing. This can be done by using a separate software event reactor job, or by ensuring per
   *                  design that BasicEtherSocket is destroyed in the same thread where Reactor::HandleEvents() is
   *                  called.
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
   * \spec
   *   requires true;
   * \endspec
   */
  ~BasicEtherSocket() noexcept;

  /*!
   * \brief Checks if the socket is in state open or not
   *
   * \details State open means that Open() has been called on the socket but Close() has not yet been called on it.
   *
   * \return True if the socket is in state open. Otherwise false
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
   * \trace           DSGN-Osab-Ptp_Close
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto CheckIsOpen() noexcept -> bool;

  /*!
   * \brief Checks if the socket is in state closed or not.
   *
   * \details
   * Open() has not been called for it since it was created.
   * It does not have a reactor, Close() has been called for it and Open() has not been called for it since the
   * call of Close().
   * Close() has been called for it, it's event handler is not executing and Open() has not been called for it
   * since the call of Close().
   *
   * \return True if the socket is in state closed. Otherwise false
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
   * \trace           DSGN-Osab-Ptp_Close
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto CheckIsClosed() noexcept -> bool;

  /*!
   * \brief Opens a socket
   *
   * \param[in] netstack_id  Specifies the network stack to be used. Defaults to default network stack.
   *
   * \details
   * Socket is opened in non-blocking mode and registered to the reactor if a reactor is available.
   *
   * \return The result of the service.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to create a
   *                                                            socket.
   * \error osabstraction::OsabErrc::kResource                  Not enough resources to open the socket. Either the
   *                                                            reactor has insufficient space left to register a new
   *                                                            handle or the handle cannot be created due to
   *                                                            insufficient system resources.
   * \error osabstraction::OsabErrc::kApiError                  The socket is already open.
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
   * \trace           DSGN-Osab-PtpSocketCreation
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Open(net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief Requests to close the connection.
   *
   * \details
   * Stops all operations and releases the system resources. If any asynchronous operation callback is currently
   * executed then the release of the system resource is delayed to the end of the asynchronous operation. It is safe
   * to delete the socket as soon as it enters the "closed" state (CheckIsClosed() == true). The socket always leaves
   * the "Open" state. Most services are not permitted anymore after a call to Close().
   * It is possible to skip the CheckIsClosed() call if it can be assured that there is no ongoing asynchronous
   * operation. One way would be to call this function in the Reactor thread after a HandleEvents() call.
   *
   * \context         ANY
   *
   * \return Result of the operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  Socket is already closed.
   *
   * \pre             -
   *
   * \post            CheckIsOpen() == false
   *
   * \reentrant       FALSE
   * \synchronous     FALSE if operation has been delayed by a concurrent callback in the reactor context.
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \trace           DSGN-Osab-Ptp_Close
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Close() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Gets the ID of the network stack used by this socket.
   *
   * \return          The network stack ID. Only valid until socket is closed.
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
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetNetworkStackId() const noexcept -> net_utils::NetworkStackId const& { return network_stack_id_; }

  /*!
   * \brief Binds a socket to an endpoint.
   *
   * \details
   * The network interface of the endpoint will be used to receive and send Ethernet frames. In case a multicast
   * endpoint is bound, the Ethernet address of the endpoint is the multicast address from which Ethernet frames will be
   * received.
   *
   * \param[in] endpoint Local network endpoint or a multicast endpoint. The endpoint must use the same network stack as
   *                     this socket (specified during Open() previously).
   *
   * \return The result of the service.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open or an endpoint has already been
   *                                                            bound to this socket or the socket uses a different
   *                                                            network stack than the passed endpoint or the operating
   *                                                            system does not support EthernetSocket.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to bind
   *                                                            address.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kAddressError              The address is already in use on the local machine.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to perform operation.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    QNX:   Native handle is not associated with
   *                                                                   character-special device or device does not
   *                                                                   support the operation.
   *                                                            Linux: Multicast membership or BPF socket option are not
   *                                                                   supported or cannot bind to interface.
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
   * \trace           DSGN-Osab-PtpSocketCreation
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Bind(Endpoint const& endpoint) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Tries to receive an Ethernet package synchronously. Continue the operation asynchronously if receive
   *        would block.
   *
   * \details
   * The operation may be performed asynchronously. That means:
   * - In case the operation is performed asynchronously: The completion callback informs about operation completion,
   *                                                      exclusive access rights of the receive buffer are released
   *                                                      when the callback is called.
   * - In case the operation can be completed immediately: The completion callback is not called, exclusive access
   *                                                       rights of the receive buffer are released when the function
   *                                                       returns.
   *
   * The return value indicates whether the operation is performed asynchronously or not.
   * Should the operation not complete immediately it is continued asynchronously. No new receive operation shall be
   * started till the the ongoing operation finishes in the completion callback.
   *
   * \param[in]  buffer_view     View to an array of bytes. The socket will take exclusive access rights of the buffer
   *                             and will hold the exclusive access rights till the callback has been executed or till
   *                             the socket is closed (CheckIsClosed() == true). The content of the buffer is undefined
   *                             if the service reports an error. The number of written bytes will be stored in the
   *                             returned ReceivedPacketInfo only in case the operation will be completed immediately.
   *
   * \param[in] callback Callback to notify the completion of the receive operation. The socket will take ownership
   *                     of the passed callback until the operation is completed. Should all data be received
   *                     immediately ownership is released when this function returns.
   *
   * \return A struct containing additional information about the received packet.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to receive Ethernet packet.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive Ethernet packet.
   * \error osabstraction::OsabErrc::kDisconnected               Asynchronous network error, e.g. network interface down
   *                                                             or not available anymore.
   * \error osabstraction::OsabErrc::kApiError                   The socket is not open and/or not bound or receiving
   *                                                             Ethernet frames is not supported by the operating
   *                                                             system.
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             receive operation.
   * \error osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                             received Ethernet packet. Packet has been dropped.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError     Device dependent IO error occurred.
   *
   * \context         ANY
   *
   * \pre             No asynchronous receive operation is in progress.
   * \pre             Input buffer must consist of amsr::ptp::Ptp::Socket::kMaxIOBufferSize or fewer entries.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE unless asynchronous behavior is required.
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-PTP_Recv_CompletedSynchronously, DSGN-Osab-PTP_Opt_Recv_CompletedAsynchronously
   * \vprivate        Vector product internal API
   */
  auto ReceiveFrom(::amsr::core::Span<std::uint8_t> buffer_view, ReceiveCompletionCallback&& callback) noexcept
      -> ::amsr::core::Result<ReceivedPacketInfo>;

  /*!
   * \brief Sends a standard Ethernet packet to the specified destination.
   *
   * \details
   * Note that sending a "standard Ethernet packet" will still result in sending a VLAN Ethernet packet if the socket
   * was bound to a VLAN network interface.
   * The timestamp for this sent packet can be polled with PollSendTimestamp() if Tx-Timestamping was enabled.
   *
   * \param[in] io_buffers_view  View to the send buffer. The io_buffers_view has to exist for the whole lifetime of
   *                             the class object.
   * \param[in] destination      The destination endpoint. The endpoint must use the same network stack as this socket
   *                             (specified during Open() previously) and the interface of this endpoint shall match the
   *                             interface of the endpoint that was bound to the socket. The VLAN ID of the endpoint is
   *                             ignored.
   *
   * \return A struct containing additional information about the sent packet.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send Ethernet packet.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send Ethernet packet.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The Ethernet frame exceeds the maximum allowed size
   *                                                             (e.g. message size > MTU size) or a kernel subsystem
   *                                                             is not configured or network interface down or
   *                                                             destination is not reachable.
   * \error  osabstraction::OsabErrc::kDisconnected              Asynchronous network error, e.g. network interface down
   *                                                             or not available anymore.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or bound or the socket is
   *                                                             bound to a different interface than the passed endpoint
   *                                                             or sending Ethernet frames is not supported on the
   *                                                             operating system.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-PTP_Sync_Send
   * \vprivate        Vector product internal API
   */
  auto SendToSync(::amsr::core::Span<std::uint8_t> io_buffers_view, Endpoint const& destination) noexcept
      -> ::amsr::core::Result<SentPacketInfo>;

  /*!
   * \brief Sends a VLAN Ethernet packet to the specified destination.
   *
   * \details
   * This API only works if the socket was bound to a physical network interface, not a VLAN network interface.
   * The timestamp for this sent packet can be polled with PollSendTimestamp() if Tx-Timestamping was enabled.
   *
   * \param[in] io_buffers_view  View to the send buffer. The io_buffers_view has to exist for the whole send operation.
   * \param[in] destination      The destination endpoint. The endpoint must use the same network stack as this socket
   *                             (specified during Open() previously) and the interface of this endpoint shall match the
   *                             interface of the endpoint that was bound to the socket. The VLAN ID of the endpoint is
   *                             used as VLAN ID of the sent packet and does not need to match the VLAN ID of the
   *                             endpoint that was bound to the socket.
   * \param[in] outgoing_packet_vlan_priority  VLAN priority of sent packet. Must be in the range [0, 7].
   *
   * \return A struct containing additional information about the sent packet.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send Ethernet packet.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send Ethernet packet.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The Ethernet frame exceeds the maximum allowed size
   *                                                             (e.g. message size > MTU size) or a kernel subsystem
   *                                                             is not configured or network interface down or
   *                                                             destination is not reachable.
   * \error  osabstraction::OsabErrc::kDisconnected              Asynchronous network error, e.g. network interface down
   *                                                             or not available anymore.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or bound or the socket is
   *                                                             bound to a different interface than the passed endpoint
   *                                                             or the VLAN ID of the endpoint is invalid or the
   *                                                             specified VLAN priority is invalid or sending Ethernet
   *                                                             frames is not supported on the operating system.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \trace           DSGN-Osab-PTP_Sync_Send
   * \vprivate        Vector product internal API
   */
  auto SendVlanFrameToSync(::amsr::core::Span<std::uint8_t> io_buffers_view, Endpoint const& destination,
                           VlanPriority outgoing_packet_vlan_priority) noexcept -> ::amsr::core::Result<SentPacketInfo>;

  /*!
   * \brief Enables time stamping for incoming Ethernet packages.
   *
   * \param[in] mode  The technique to generate the time stamps.
   *
   * \details
   * There might already be Ethernet packets in the network stack before this call that will be received without a
   * timestamp. Enabling receive time stamping might also not become active immediately.
   * Enabling the time stamping may enable it for all incoming packages on the bound endpoint interface. This may
   * have negative impact on the overall network performance.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open and/or not bound to a local
   *                                                            endpoint or the requested receive timestamping is not
   *                                                            supported by OsAbstraction for this operating system.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the selected mode.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto EnableRxTimestamping(TimestampMode mode) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Enables time stamping for outgoing Ethernet packages.
   *
   * \param[in] mode  The technique to generate the time stamps.
   *
   * \details
   * Enabling the time stamping may enable it for all outgoing packages on the bound endpoint interface. This may have
   * negative impact on the overall network performance.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open and/or not bound to a local
   *                                                            endpoint or the requested send timestamping is not
   *                                                            supported by OsAbstraction for this operating system.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The system settings do not support the selected mode.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto EnableTxTimestamping(TimestampMode mode) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Polls for a send time stamp.
   *
   * \param[in] token The token that identifies the desired send time stamp.
   *
   * \return The time stamp.
   *
   * \error osabstraction::OsabErrc::kApiError                  The socket is not open and/or not bound to a local
   *                                                            endpoint and/or selected time stamping mode is not
   *                                                            supported on the current operating system.
   * \error osabstraction::OsabErrc::kBusy                      The requested time stamp is not available right now or
   *                                                            the device to get it is temporarily unavailable.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    Getting send time stamp is not supported by the system
   *                                                            or device to get it does not exist or operation failed
   *                                                            because of a hardware error.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to acquire the time stamp.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto PollSendTimestamp(SendTimestampToken const& token) noexcept -> ::amsr::core::Result<SendTimestamp>;

 private:
  /*!
   * \brief Length (in bytes) of a VLAN Ethernet header.
   */
  static constexpr std::size_t kVlanEthernetHeaderLength{kEtherHeaderLength + kVlanTagLength};

  /*!
   * \brief Internal Basic socket state.
   *
   * \trace           DSGN-Osab-PTP_Socket_States
   */
  enum class BasicSocketState : std::uint32_t {
    /*!
     * \brief The socket is closed.
     *
     * \details
     * Not system resources are allocated and no operation is ongoing.
     */
    kClosed = 0,

    /*!
     * \brief Socket is ready to be used.
     *
     * \details
     * Socket is ready to be used.
     */
    kOpen = 1,

    /*!
     * \brief The socket is currently closing
     *
     * \details
     * Closing has been requested. No new asynchronous callback will be started. The socket will be closed as soon as
     * all ongoing asynchronous callbacks complete.
     */
    kClosing = 2,
  };

  /*!
   * \brief Enum that specifies which types of packets shall be timestamped (received packets, sent packets).
   */
  enum class TimestampedPackets : std::uint32_t { kReceivedPackets, kSentPackets };

  /*!
   * \brief Changes the native handle observation state.
   *
   * \param[in]  single_event Reactor event whose observation shall be changed
   * \param[in]  enable       True to enable observation and false to disable observation.
   *
   * \return          -
   *
   * \context         ANY
   *
   * \pre             CheckIsOpen() == true
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void ChangeReactorObservation(osabstraction::io::reactor1::EventTypes single_event, bool enable) noexcept;

  /*!
   * \brief           Check whether the socket is open.
   *
   * \return          Returns a value if the socket is open, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kApiError    The socket is not open.
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
  auto CheckSocketIsOpen() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether the socket is closed.
   *
   * \return          Returns a value if the socket is open, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kApiError    The socket is not closed.
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
  auto CheckSocketIsClosed() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Checks whether the socket uses a specific network stack.
   *
   * \param[in]       network_stack_id  The ID of the network stack that this socket is expected to use.
   *
   * \return          Value, if the socket uses the passed network stack, error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket uses different network stack.
   *
   * \context         ANY
   *
   * \pre             The socket is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckSocketUsesNetworkStack(net_utils::NetworkStackId const& network_stack_id) const noexcept
      -> ::amsr::core::Result<void>;

  /*!
   * \brief           Checks whether the socket is bound to a specific interface.
   *
   * \param[in]       interface  The interface that the socket is expected to be bound to.
   *
   * \return          Value, if the socket is bound to the expected interface, error otherwise.
   *
   * \error osabstraction::OsabErrc::kApiError                   Socket is bound to different interface.
   *
   * \context         ANY
   *
   * \pre             The socket is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckSocketIsBoundToInterface(Interface const& interface) const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Calls the receive completion callback
   *
   * \param           result   Result of the receive operation
   *
   * \context         Reactor context.
   *
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void CallReceiveCompletionCallback(::amsr::core::Result<ReceivedPacketInfo>&& result) noexcept;

  /*!
   * \brief Registers the native handle to the reactor.
   *
   * \return Result of operation.
   *
   * \error           osabstraction::OsabErrc::kResource      No memory to register another callback or system limit
   *                                                          reached.
   *
   * \context         ANY
   *
   * \pre             Has valid native handle
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto RegisterToReactor() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Checks whether a closing socket can be set to closed.
   *
   * \details The direct transition to closing is not possible if the reactor informs us about an ongoing callback in
   *          another thread. The transition is delay till the next CheckIsClosed() call. Is open calls this function to
   *          update the socket state.
   *
   * \return          -
   *
   * \context         ANY
   *
   * \pre             Socket is in closing state.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void UpdateSocketClosingState() noexcept;

  /*!
   * \brief Implements a reactor callback.
   *
   * \param[in] events Pending events.
   *
   * \details Implement the reactor callback.
   *
   * \return          -
   *
   * \context         ANY
   *
   * \pre             Lock is not held on call.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void OnReactorCallback(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief Checks if the object is in the expected state.
   *
   * \details Updates the state if required.
   *
   * \param[in]  expected_state The expected state.
   *
   * \return True if the object is in the expected state, false if not.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto CheckSocketState(BasicSocketState expected_state) noexcept -> bool;

  /*!
   * \brief           Check whether receiving is possible.
   *
   * \return          A value if receiving is possible, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kApiError   Socket is not open and/or bound and/or there is an ongoing
   *                                                       asynchronous receive operation.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \synchronous     TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckReceiveIsPossible() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether the socket is bound to an endpoint.
   *
   * \return          A value if bound, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kApiError   Socket is not bound.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \synchronous     TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckSocketIsBoundToEndpoint() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Check whether the socket is not bound to an endpoint.
   *
   * \return          A value if not bound, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kApiError    Socket is bound.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \synchronous     TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto CheckSocketIsNotBoundToEndpoint() const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Binds a socket to a local endpoint.
   *
   * \param[in] endpoint The local network endpoint or a multicast endpoint.
   *
   * \details
   * The network interface of the endpoint will be used to receive and send Ethernet frames. In case a multicast
   * endpoint is bound, the Ethernet address of the endpoint is the multicast address from which Ethernet frames will be
   * received.
   * The socket is not allowed to be moved anymore after it was bound to an endpoint.
   *
   * \return The result of the service.
   *
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to bind address.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The multicast membership socket option is not
   *                                                            supported or the provided endpoint interface is not
   *                                                            configured.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kAddressError              The address is already in use on the local machine.
   * \error osabstraction::OsabErrc::kResource                  Not enough system resources to perform operation.
   *
   * \context         ANY
   *
   * \pre             CheckIsOpen() == true
   * \pre             No endpoint has been bound yet.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \trace           DSGN-Osab-PtpSocketCreation
   * \vprivate        Vector component internal API
   */
  auto BindImpl(Endpoint const& endpoint) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Calls function to receive an Ethernet frame.
   *
   * \param[in, out] buffer_view      View to receive buffer. Must at most be amsr::ptp::Ptp::Socket::kMaxIOBufferSize
   *                                  big. The buffer_view has to exist for the whole receive operation.
   *
   * \return A struct containing additional information about the received Ethernet frame.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to receive Ethernet frame.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive Ethernet frame.
   * \error osabstraction::OsabErrc::kDisconnected               Asynchronous network error, e.g. network interface down
   *                                                             or not available anymore.
   * \error osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                             received Ethernet frame. The trailing bytes of the
   *                                                             Ethernet frame have been dropped.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError     Device dependent IO error occurred.
   *
   * \context         ANY
   *
   * \pre             The socket is open.
   * \pre             The socket is bound to an endpoint.
   * \pre             No ongoing receive operation.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * */
  auto ReceiveFromImpl(::amsr::core::Span<std::uint8_t> buffer_view) noexcept
      -> ::amsr::core::Result<ReceivedPacketInfo>;

  /*!
   * \brief Calls function to send a (VLAN) Ethernet frame.
   *
   * \param[in]  io_buffers_view       View to the send buffer. The io_buffers_view has to exist for the whole send
   *                                   operation.
   * \param[in]  destination           The destination endpoint.
   * \param[in]  outgoing_packet_vlan_priority  Optional VLAN priority if a VLAN Ethernet frame shall be sent.
   *
   * \return A struct containing the result of the send operation and the possibly newly allocated timestamp ID.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                  Not enough system resources to send Ethernet frame.
   * \error  osabstraction::OsabErrc::kBusy                      Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to send Ethernet frame.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError    The Ethernet frame exceeds the maximum allowed size
   *                                                             (e.g. message size > MTU size) or a kernel subsystem
   *                                                             is not configured or network interface down or
   *                                                             destination is not reachable.
   * \error  osabstraction::OsabErrc::kDisconnected              Asynchronous network error, e.g. network interface down
   *                                                             or not available anymore.
   * \error  osabstraction::OsabErrc::kApiError                  The socket is not open and/or bound or the socket is
   *                                                             bound to a different interface than the passed endpoint
   *                                                             or sending Ethernet frames is not supported on the
   *                                                             operating system.
   *
   * \context         ANY
   *
   * \pre             The socket is open.
   * \pre             The socket is bound to an endpoint.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   * \pre             All signals are blocked on the calling thread.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto SendToImpl(::amsr::core::Span<std::uint8_t> io_buffers_view, Endpoint const& destination,
                  VlanPriority outgoing_packet_vlan_priority = kInvalidVlanPriority) noexcept
      -> ::amsr::core::Result<SentPacketInfo>;

  /*!
   * \brief      Fills passed buffer with a (VLAN) PTP Ethernet header.
   *
   * \param[in]  destination                    The destination of the Ethernet frame. If this contains a valid VLAN ID,
   *                                            a VLAN Ethernet header is created, otherwise a standard Ethernet header.
   * \param[in]  outgoing_packet_vlan_priority  The VLAN priority of the VLAN Ethernet frame. Ignored if the passed
   *                                            endpoint contains no valid VLAN ID.
   * \param[out] eth_header_output              Buffer where the Ethernet header shall be written.
   *
   * \return     The number of bytes written to the buffer.
   *
   * \context         ANY
   *
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto SetupEthHeader(Endpoint const& destination, VlanPriority outgoing_packet_vlan_priority,
                      ::amsr::core::Span<std::uint8_t, kVlanEthernetHeaderLength> eth_header_output) const noexcept
      -> std::size_t;

  /*!
   * \brief Resets the ring buffer for send timestamps.
   *
   * \context         ANY
   *
   * \pre             The socket is not open (or in the process of closing).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void ResetRingBuffer() noexcept;

  /*!
   * \brief Checks if the interface that the socket is bound to supports the specified timestamping.
   *
   * \param[in]       timestamped_packets   Specifies if receive or send timestamping capabilities shall be checked.
   * \param[in]       timestamping_mode     Specifies if software or hardware timestamping capabilities shall be
   *                                        checked.
   *
   * \return          A value if the interface supports the specified timestamping.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError    The interface does not support the passed
   *                                                                      settings or the supported settings of the
   *                                                                      interface could not be determined.
   * \error           osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no
   *                                                                      category.
   *
   * \context         ANY
   *
   * \pre             The socket is open.
   * \pre             The socket is bound to an endpoint.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ::amsr::core::Result<void> CheckInterfaceTimestampingCapabilities(TimestampedPackets timestamped_packets,
                                                                    TimestampMode timestamping_mode) const;

  /*!
   * \brief Receive an available send timestamp
   *
   * \details The reactor notified that a a send timestamp is available. This function will now try to receive it.
   *
   * \context         Reactor context.
   *
   * \pre             The socket is open.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \trace           DSGN-Osab-PTP
   */
  void HandleTxTimestampReceived() noexcept;

  /*!
   * \brief Sets the object to an internal error state and notifies user about failure to receive a send timestamp.
   *
   * \context         Reactor context.
   *
   * \pre             The socket is open.
   * \pre             The socket is bound to an endpoint.
   * \pre             Lock is held on call (lock is released and re-acquired between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void HandleSendTimeStampError() noexcept;

  /*!
   * \brief Attaches the BPF filter to receive ethernet frames.
   *
   * \param[in] endpoint The ethernet frames destination address.
   *
   * \return Positive void result on success. Otherwise error returned by the operating system interface.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError   The system does not support setting the filter.
   * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
   *
   * \context         ANY
   * \pre             The socket is open.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ::amsr::core::Result<void> SetBpfFilter(Endpoint const& endpoint) noexcept;

  /*!
   * \brief Adds a timestamp to the queue of time stamps for polling.
   *
   * \param[in]       token                 Token that identifies the time stamp when polling for it.
   * \param[in]       timestamp             The time stamp to store.
   *
   * \context         Reactor context.
   *
   * \pre             The socket is open.
   * \pre             The socket is bound to an endpoint.
   * \pre             Lock is held on call (lock is held during whole time between function call and return).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  void AddTimeStampToQueue(SendTimestampToken const& token, SendTimestamp const& timestamp) noexcept;

  /*!
   * \brief Native handle.
   *
   * \details Does not need to be protected by critical sections because it is only set once (during Open/
   *          AssignNativeHandle) and asynchronous operations (against which the exclusive area protects) are only
   *          possible after this has been done.
   */
  osabstraction::io::NativeHandle native_handle_{osabstraction::io::kInvalidNativeHandle};

  /*!
   * \brief Reactor for asynchronous operations.
   *
   * The reactor can only be provided during object construction. If no reactor has been provided then the object
   * can only be used synchronously.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief Stores the reactor's handle to the registered file descriptor.
   *
   * The handle the handle is only valid after registering to the reactor.
   */
  osabstraction::io::reactor1::CallbackHandle reactor_handle_{};

  /*!
   * \brief The state of the internal socket
   */
  BasicSocketState socket_state_{BasicSocketState::kClosed};

  /*!
   * \brief   Lock that protects the socket object attributes.
   *
   * \details For further information, see the description of the exclusive areas for
   *          ::amsr::ptp::internal::BasicEtherSocket.
   */
  std::mutex mutex_{};

  /*!
   * \brief ID of the network stack used by this socket.
   */
  net_utils::NetworkStackId network_stack_id_{};

  /*!
   * \brief Interface that the socket is bound to.
   */
  Interface bound_interface_{};

  /*!
   * \brief VLAN that the socket is bound to.
   * \details kInvalidVlanId if the socket is not bound to any VLAN.
   */
  VlanId vlan_id_{kInvalidVlanId};

  /*!
   * \brief View to buffer, used for receiving.
   */
  ::amsr::core::Span<std::uint8_t> incoming_buffer_view_{};

  /*!
   * \brief  Stores the callback to be executed on a completed asynchronous receive operation.
   *
   * \details
   * The callback is only valid during asynchronous receive.
   */
  ReceiveCompletionCallback receive_completion_callback_{};

  /*!
   * \brief Stores whether there is an ongoing receive operation or not.
   */
  bool ongoing_async_receive_operation_{false};

  /*!
   * \brief Stores whether the socket is already bound to an endpoint.
   *
   * \details No protection against concurrent access (like mutex or atomic) required because this is not accessed from
   *          the socket objects reactor event handling functions which means all functions that access is must be
   *          synchronized in a multi-threaded environment.
   */
  bool is_socket_bound_to_endpoint_{false};

  /*!
   * \brief The receive timestamping mode.
   *
   * The default is software time stamping.
   */
  TimestampMode receive_time_stamping_mode_{TimestampMode::kSoftware};

  /*!
   * \brief The send timestamping mode.
   *
   * The default is software time stamping.
   */
  TimestampMode send_time_stamping_mode_{TimestampMode::kSoftware};

  /*!
   * \brief   Counter that shows the timestamp ID value that the next sent packet will have.
   * \details Each sent Ethernet frame has an associated ID that is returned together with the sent timestamp so that
   *          each received sent timestamp can be mapped to a sent Ethernet packet. The first sent packet has the ID 0,
   *          the ID value increases by 1 for each following packet.
   */
  std::uint32_t next_timestamp_id_{0};

  /*!
   * \brief   Stores the index of the interface that the socket is bound to.
   */
  InterfaceIndex bound_interface_idx_{};

  /*!
   * \brief   Source ethernet address.
   * \details A send call uses this address as source address in the ethernet frame. The address is set during Bind()
   *          call.
   */
  EthernetAddress source_address_{};

  /*!
   * \brief   Alias for the type of the send timestamp ring buffer.
   */
  using RingBufferType = ::amsr::core::Array<std::pair<SendTimestampToken, SendTimestamp>, 16>;

  /*!
   * \brief   Queue of time stamps for polling by user.
   */
  RingBufferType timestamp_ring_buffer_{};

  /*!
   * \brief   Index for next time stamp to insert into queue.
   */
  std::size_t next_timestamp_index_{0};

  /*!
   * \brief   Stores if a failure to receive a send timestamp has occurred (true) or not (false).
   *
   * \details Storing and evaluating can be confined to this class because only synchronous and optimistic operations
   *          can be requested for Ethernet sockets. From this follows that each function to request an operation will
   *          directly call the respective implementation function on this class so the error can be reported on
   *          request.
   */
  bool timestamp_error_{false};

  /*!
   * \brief Stores the internally used io buffer array for sending data.
   *
   * \details Use an ara::core::Array that still fulfills the "continues memory" requirement for the I/O vector array.
   *          Array contains two I/O vectors, first for the ethernet header and second for the buffer to be sent.
   */
  ara::core::Array<osabstraction::io::ConstIOBuffer, 2> tx_io_buffers_{};
};

/*!
 * \exclusivearea ::amsr::ptp::internal::BasicEtherSocket::mutex_
 * Ensures atomicity when jointly accessing object state and reactor event settings for data transfer operations.
 * \protects ::amsr::ptp::internal::BasicEtherSocket::native_handle_,
 *           ::amsr::ptp::internal::BasicEtherSocket::reactor_,
 *           ::amsr::ptp::internal::BasicEtherSocket::reactor_handle_,
 *           ::amsr::ptp::internal::BasicEtherSocket::socket_state_,
 *           ::amsr::ptp::internal::BasicEtherSocket::network_stack_id_,
 *           ::amsr::ptp::internal::BasicEtherSocket::bound_interface_,
 *           ::amsr::ptp::internal::BasicEtherSocket::vlan_id_,
 *           ::amsr::ptp::internal::BasicEtherSocket::incoming_buffer_view_,
 *           ::amsr::ptp::internal::BasicEtherSocket::receive_completion_callback_,
 *           ::amsr::ptp::internal::BasicEtherSocket::ongoing_async_receive_operation_,
 *           ::amsr::ptp::internal::BasicEtherSocket::is_socket_bound_to_endpoint_,
 *           ::amsr::ptp::internal::BasicEtherSocket::receive_time_stamping_mode_,
 *           ::amsr::ptp::internal::BasicEtherSocket::send_time_stamping_mode_,
 *           ::amsr::ptp::internal::BasicEtherSocket::next_timestamp_id_,
 *           ::amsr::ptp::internal::BasicEtherSocket::bound_interface_idx_,
 *           ::amsr::ptp::internal::BasicEtherSocket::source_address_,
 *           ::amsr::ptp::internal::BasicEtherSocket::timestamp_ring_buffer_,
 *           ::amsr::ptp::internal::BasicEtherSocket::next_timestamp_index_,
 *           ::amsr::ptp::internal::BasicEtherSocket::timestamp_error_,
 *           ::amsr::ptp::internal::BasicEtherSocket::tx_io_buffers_,
 * \usedin   ::amsr::ptp::internal::BasicEtherSocket::Close,
 *           ::amsr::ptp::internal::BasicEtherSocket::OnReactorCallback,
 *           ::amsr::ptp::internal::BasicEtherSocket::CheckIsOpen,
 *           ::amsr::ptp::internal::BasicEtherSocket::CheckisClosed,
 *           ::amsr::ptp::internal::BasicEtherSocket::Open,
 *           ::amsr::ptp::internal::BasicEtherSocket::GetNetworkStackId,
 *           ::amsr::ptp::internal::BasicEtherSocket::Bind,
 *           ::amsr::ptp::internal::BasicEtherSocket::ReceiveFrom,
 *           ::amsr::ptp::internal::BasicEtherSocket::SendToSync,
 *           ::amsr::ptp::internal::BasicEtherSocket::SendVlanFrameToSync,
 *           ::amsr::ptp::internal::BasicEtherSocket::EnableRxTimestamping,
 *           ::amsr::ptp::internal::BasicEtherSocket::EnableTxTimestamping,
 *           ::amsr::ptp::internal::BasicEtherSocket::PollSendTimestamp,
 * \exclude  All other of these methods of the same object.
 * \length   LONG Enabling and disabling events on the reactor and performing additional system calls.
 * \endexclusivearea
 */
}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_BASIC_ETHER_SOCKET_H_

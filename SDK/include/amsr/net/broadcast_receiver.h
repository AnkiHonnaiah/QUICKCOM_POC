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
 *        \brief  Receiver for UDP-based IPv4 directed broadcast communication on Linux.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_BROADCAST_RECEIVER_H_
#define LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_BROADCAST_RECEIVER_H_

#include "amsr/core/result.h"

#include "amsr/core/span.h"
#include "amsr/net/ip/udp.h"
#include "amsr/net_utils/network_stack_id.h"
#include "osabstraction/io/common_types.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace net {

/*!
 * \brief Receiver for handling of IPv4 limited broadcasts (messages with destination 255.255.255.255) for UDP.
 *
 * \trace DSGN-Osab-BasicIP_QNX_LimitedBroadcast
 */
class BroadcastReceiver final {
 public:
  /*!
   * \brief Stores additional information about the received broadcast.
   */
  struct ReceivedBroadcastInfo {
    /*!
     * \brief Stores whether the operation could be completed immediately or will be continued in the background.
     *
     * \details
     * This value is always ReceiveResult::kReceiveCompleted for synchronous services.
     */
    osabstraction::io::ReceiveResult operation_result{};

    /*!
     * \brief The sender's network endpoint.
     *
     * \details Only valid if operation_result == ReceiveResult::kReceiveCompleted.
     */
    ip::Udp::Endpoint remote_endpoint;

    /*!
     * \brief   Received payload size.
     *
     * \details Only valid if operation_result == ReceiveResult::kReceiveCompleted.
     */
    std::size_t payload_size{};
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
   *         osabstraction::OsabErrc::kSystemEnvironmentError     QNX: Unexpected output provided by OS.
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
      vac::language::UniqueFunction<void(::amsr::core::Result<ip::Udp::Socket::DatagramInfo>&& result)>;

  /*!
   * \brief Construct a broadcast receiver for synchronous and asynchronous communication over UDP.
   *
   * \param[in] reactor Reactor interface for asynchronous operations. The reactor has to exist for the whole
   *                    lifetime of this BroadcastReceiver object.
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
  explicit BroadcastReceiver(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
      : receiver_socket_{reactor} {}

  /*!
   * \brief Destroy the broadcast receiver.
   *
   * \details         The user has to avoid destructing the BroadcastReceiver object while the internal reactor callback
   *                  is executing. This can be done by using a separate software event reactor job, or by ensuring per
   *                  design that BroadcastReceiver is destroyed in the same thread where Reactor::HandleEvents() is
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
   * \vprivate        Vector product internal API
   */
  ~BroadcastReceiver() noexcept = default;

  // Copying a receiver is not supported.
  BroadcastReceiver(BroadcastReceiver const&) noexcept = delete;
  BroadcastReceiver& operator=(BroadcastReceiver const&) noexcept = delete;

  /*!
   * \brief Moves the other receiver into this one.
   *
   * \param[in] other   Other receiver used to construct this receiver. Other receiver must not have any ongoing
   *                    asynchronous operation. Its lock must not be locked.
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
  BroadcastReceiver(BroadcastReceiver&& other) noexcept = default;

  /*!
   * \brief Closes this receiver and moves other receiver into this one.
   *
   * \param[in] other Other receiver that will be moved into this one. Other receiver must not have any ongoing
   *                  asynchronous operation. Its lock must not be locked.
   *
   * \return Reference to this object.
   *
   * \context         ANY
   *
   * \pre             This receiver has no ongoing asynchronous operation.
   * \pre             The lock of this object is not locked.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  BroadcastReceiver& operator=(BroadcastReceiver&& other) & noexcept = default;

  /*!
   * \brief Opens the receiver.
   *
   * \param[in] netstack_id  Specifies the network stack to be used. Defaults to default network stack.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to create a
   *                                                            receiver.
   * \error osabstraction::OsabErrc::kResource                  Not enough resources to open the receiver. Either the
   *                                                            reactor has insufficient space left to register a new
   *                                                            handle or the handle cannot be created due to
   *                                                            insufficient system resources.
   * \error osabstraction::OsabErrc::kApiError                  The receiver has been opened before.
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
  ::amsr::core::Result<void> Open(net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept;

  /*!
   * \brief Binds a receiver to a local endpoint.
   *
   * \param[in] endpoint The local network endpoint. The endpoint must use the same network stack as this socket
   *                     (specified during Open() previously).
   *
   * \details
   * Bind the local network endpoint to the receiver. The endpoint will be the destination address, port for incoming
   * communication.
   * The network interface which has the given endpoint IPv4 address assigned is used for incoming communication.
   *
   * \return The result of the service.
   *
   * \error osabstraction::OsabErrc::kApiError                  The receiver is not open or already bound or
   *                                                            endpoint and receiver have different protocol families
   *                                                            or the socket uses a different network stack than the
   *                                                            passed endpoint.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to bind address.
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAddressNotAvailable       The address is not available on the local machine.
   * \error osabstraction::OsabErrc::kAddressError              The address is already in use on the local machine.
   * \error osabstraction::OsabErrc::kResource                  QNX: Not enough system resources to set up the receiver.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    Required socket option not supported by OS.
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
   */
  ::amsr::core::Result<void> Bind(ip::Udp::Endpoint const& endpoint) noexcept;

  /*!
   * \brief Requests to close the broadcast receiver.
   *
   * \details
   * Stops all operations and releases the system resources. If any asynchronous operation callback is currently
   * executed then the release of the system resource is delayed to the end of the asynchronous operation. It is safe
   * to delete the receiver as soon as it enters the "closed" state (CheckIsClosed() == true). The receiver always
   * leaves the "Open" state. Most services are not permitted anymore after a call to Close().
   * It is possible to skip the CheckIsClosed() call if it can be assured that there is no ongoing asynchronous
   * operation. One way would be to call this function in the Reactor thread after a HandleEvents() call.
   *
   * \context         ANY
   *
   * \return Result of the operation.
   *
   * \error osabstraction::OsabErrc::kApiError                  Receiver is already closed.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     FALSE if operation has been delayed by a concurrent callback in the reactor context.
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Result<void> Close() noexcept;

  /*!
   * \brief Checks if the receiver is in state closed or not.
   *
   * \details
   * Open() has not been called for it since it was created.
   * It does not have a reactor, Close() has been called for it and Open() has not been called for it since the
   * call of Close().
   * Close() has been called for it, it's event handler is not executing and Open() has not been called for it
   * since the call of Close().
   *
   * \return True if the receiver is in state closed. Otherwise false
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
  bool CheckIsClosed() noexcept;

  /*!
   * \brief Sets the blocking mode.
   *
   * \param[in] enable True to enable blocking mode. False to disable blocking mode.
   *
   * \error  osabstraction::OsabErrc::kApiError                  The receiver is not open.
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
  ::amsr::core::Result<void> SetBlockingMode(bool enable) noexcept;

  /*!
   * \brief Receives a broadcast UDP message synchronously.
   *
   * \details
   * The operation can be performed in blocking or unblocking mode.
   *
   * \param[in, out] buffer_view   View to an array of bytes. The incoming datagram will be written to the provided
   *                               buffer. The content of the buffer is undefined if the service reports an error. The
   *                               number of written bytes will be stored in the returned ReceivedBroadcastInfo. Must at
   * most consist of kMaxIOBufferSize bytes. The buffer_view has to exist until the function returns.
   *
   * \return A struct containing additional information about the received datagram.
   *
   * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
   * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
   * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error that has been caused by a
   *                                                              previous datagram.
   * \error  osabstraction::OsabErrc::kApiError                   The receiver is not open.
   *                                                              Also could happen when there is an ongoing
   *                                                              asynchronous receive operation.
   * \error  osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                              received datagram. The trailing bytes of the datagram
   *                                                              have been dropped.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError      QNX: Unexpected output provided by OS.
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
  ::amsr::core::Result<ReceivedBroadcastInfo> ReceiveFromSync(::amsr::core::Span<std::uint8_t> buffer_view) noexcept;

  /*!
   * \brief Tries to receive a broadcast synchronously. Continue the operation asynchronously if receive would block.
   *
   * \details
   * The operation may be performed asynchronously. That means:
   * - In case the operation is performed asynchronously: The completion callback informs about operation completion,
   *                                                      exclusive access rights of the receive buffer are released
                                                          when the callback is called.
   * - In case the operation can be completed immediately: The completion callback is not called, exclusive access
   *                                                       rights of the receive buffer are released when the function
   *                                                       returns.
   *
   * The return value indicates whether the operation is performed asynchronously or not.
   * Should the operation not complete immediately it is continued asynchronously. No new receive operation shall be
   * started till the the ongoing operation finishes in the completion callback.
   *
   * \param[in]  buffer_view     View to an array of bytes. The receiver will take exclusive access rights of the buffer
   *                             and will hold the exclusive access rights till the callback has been executed or till
   *                             the receiver is closed (CheckIsClosed() == true). The content of the buffer is
   *                             undefined if the service reports an error. The number of written bytes will be stored
   *                             in the returned ReceivedBroadcastInfo only in case the operation will be completed
   *                             immediately. Must at most consist of kMaxIOBufferSize bytes.
   *
   * \param[in] callback Callback to notify the completion of the receive operation. The receiver will take ownership
   *                     of the passed callback until the operation is completed. Should all data be received
   *                     immediately ownership is released when this function returns.
   *
   * \return A struct containing additional information about the received packet.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to receive broadcast.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive broadcast.
   * \error osabstraction::OsabErrc::kDisconnected               Asynchronous network error, e.g. network interface
   *                                                             down or not available anymore.
   * \error osabstraction::OsabErrc::kApiError                   The receiver is not open and/or not bound.
   *                                                             Also could happen when there is an ongoing asynchronous
   *                                                             receive operation.
   * \error osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
   *                                                             received broadcast payload. The trailing bytes of the
   *                                                             datagram have been dropped.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError     QNX: Unexpected output provided by OS.
   *
   * \context         ANY
   *
   * \pre             Blocking mode is disabled.
   * \pre             No asynchronous receive operation is in progress.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE unless asynchronous behavior is required.
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Result<ReceivedBroadcastInfo> ReceiveFrom(::amsr::core::Span<std::uint8_t> buffer_view,
                                                          ReceiveCompletionCallback&& callback) noexcept;

 private:
  /*!
   * \brief   Socket to receive limited broadcasts.
   */
  ip::Udp::Socket receiver_socket_;
};

}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_BROADCAST_RECEIVER_H_

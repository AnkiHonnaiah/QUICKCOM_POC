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
 *        \brief Internal OS interface helper functions.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_INTERNAL_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_INTERNAL_H_

#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/net/internal/os_constants.h"
#include "amsr/net/internal/types.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/native_types.h"

namespace amsr {
namespace net {
namespace internal {
namespace os_interface {

/*!
 * \brief Function pointer alias for a pointer to a amsr::net::internal::os_interface::Map*SocketError()
 * function.
 */
using MapSocketErrorFunction = ::amsr::core::ErrorCode (*)(osabstraction::internal::OsErrorNumber);

/*!
 * \brief Structure for the return type of ReceiveMessage() - a pair of received bytes and the message flags.
 */
struct ReceiveMessageReturnType {
  /*!
   * \brief Received bytes.
   */
  std::size_t received_bytes;

  /*!
   * \brief Message flags returned by the receive system call.
   */
  decltype(msghdr::msg_flags) message_flags;
};

/*!
 * \brief Sends data over a socket.
 *
 * \param[in] handle                    Valid native handle.
 * \param[in] io_buffers_view           View to the buffers to be send. The io_buffers_view has to exist
 *                                      for the whole lifetime of the class object.
 * \param[in] optional_destination      View to an optional address (set pointer and size to 0 if it should be omitted).
 * \param[in] map_socket_error_function Function pointer to the error mapping function to call.
 *
 * \return Number of bytes that have actually been sent. 0 if the buffer size is 0.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to conduct operation.
 * \error  osabstraction::OsabErrc::kDisconnected               The connection has been terminated by peer or the peer
 *                                                              is no longer reachable.
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Permission to access socket denied.
 *
 * \context         ANY
 *
 * \pre             Handle identifies an open and connected socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> SendMessage(
    osabstraction::io::NativeHandle handle, ::amsr::core::Span<osabstraction::io::ConstIOBuffer const> io_buffers_view,
    SocketConstAddrView optional_destination, MapSocketErrorFunction map_socket_error_function) noexcept;

/*!
 * \brief Sends multiple messages over a socket.
 *
 * \param[in]     handle                    Valid native handle.
 * \param[in,out] datagrams                 View to the datagrams to be sent. The number of sent bytes for each datagram
 *                                          is returned in this parameter by this functions.
 * \param[in]     map_socket_error_function Function pointer to the error mapping function to call.
 *
 * \return Number of messages that have actually been sent. 0 if the buffer size is 0.
 *
 * \error  osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error  osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error  osabstraction::OsabErrc::kResource                   Not enough system resources to conduct operation.
 * \error  osabstraction::OsabErrc::kDisconnected               Destination not reachable or asynchronous network
 *                                                              error.
 * \error  osabstraction::OsabErrc::kInsufficientPrivileges     Permission to access socket denied.
 * \error  osabstraction::OsabErrc::kSystemEnvironmentError     The datagram exceeds the maximum allowed size (e.g.
 *                                                              message size > MTU size) or a kernel subsystem is not
 *                                                              configured.
 *
 * \context         ANY
 *
 * \pre             Handle identifies an open datagram socket.
 * \pre             Input data buffer must consist of kMaxDatagramsBulkSnd or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> SendMessagesBulk(osabstraction::io::NativeHandle handle,
                                                   ::amsr::core::Span<BulkBufferType> datagrams,
                                                   MapSocketErrorFunction map_socket_error_function) noexcept;

/*!
 * \brief Receives data from a socket.
 *
 * \param[in]       handle                Valid native handle.
 * \param[in, out]  io_buffers_view       View to the buffer to store the incoming datagram. The io_buffers_view has
 *                                        to exist for the whole lifetime of the class object.
 * \param[in, out]  optional_source       View to an address object. The address object's family must match handle's
 *                                        address family. If the pointer is not null, the source address of the packet
 *                                        will be saved there.
 * \param[in, out]  ancillary_data_view   A view to ancillary data requests. The ancillary data will be written to the
 *                                        provide data view if there is data for the requested socket level and
 *                                        protocol. The used_bytes field will be updated on any write operation.
 *                                        Ancillary data requests are not supported on PikeOS.
 * \param[in]       flags                 Flags to pass to the recvmsg system call.
 *
 * \param[in]       map_socket_error_function Function pointer to the error mapping function to call.
 *
 * \return Number of bytes that have been received. 0 if the buffer size is 0, always greater than 0 if the buffer size
 *         is larger than 0.
 *
 * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
 * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
 * \error osabstraction::OsabErrc::kDisconnected               No route to destination exists. Note that this may be an
 *                                                             asynchronous network error that has been caused by a
 *                                                             previous datagram.
 *                                                             For stream oriented sockets a kDisconnect may be returned
 *                                                             on End of Stream event.
 * \error osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
 *                                                             received datagram. The trailing bytes of the datagram
 *                                                             have been dropped.
 *
 * \context         ANY
 *
 * \pre             flags must be valid for recvmsg.
 * \pre             Handle identifies an open socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<ReceiveMessageReturnType> ReceiveMessage(
    osabstraction::io::NativeHandle handle, ::amsr::core::Span<osabstraction::io::MutableIOBuffer> io_buffers_view,
    SocketMutableAddrView optional_source, ::amsr::core::Span<AncillaryDataRequest> ancillary_data_view,
    MapSocketErrorFunction map_socket_error_function, ReceiveFlags flags) noexcept;

/*!
 * \brief Receives a bulk of datagrams from a socket.
 *
 * \param[in]       handle                      Valid native handle.
 * \param[in, out]  message_buffers_view        View to the message buffers to store the incoming datagrams.
 * \param[in]       map_socket_error_function   Function pointer to the error mapping function to call.
 * \param[in]       flags                       Flags to pass to the recvmsg system call.
 *
 * \return          Number of datagrams that have been received.
 *
 * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
 * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
 * \error osabstraction::OsabErrc::kDisconnected               No route to destination exists. Note that this may be an
 *                                                             asynchronous network error that has been caused by a
 *                                                             previous datagram.
 *                                                             For stream oriented sockets a kDisconnect may be returned
 *                                                             on End of Stream event.
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
::amsr::core::Result<std::size_t> ReceiveMessagesBulk(osabstraction::io::NativeHandle handle,
                                                      ::amsr::core::Span<BulkBufferType> message_buffers_view,
                                                      MapSocketErrorFunction map_socket_error_function,
                                                      std::int32_t flags) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_INTERNAL_H_

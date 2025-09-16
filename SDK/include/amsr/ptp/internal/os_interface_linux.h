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
 *        \brief Interface to Linux specific operating system calls.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_LINUX_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_LINUX_H_

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ptp/internal/types.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/native_types.h"

namespace amsr {
namespace ptp {
namespace internal {
namespace os_interface {

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
 * \steady          FALSE
 */
::amsr::core::Result<void> Bind(osabstraction::io::NativeHandle handle, SocketConstAddrView address) noexcept;

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
 * \brief Receives data from a socket error queue.
 *
 * \param[in]       handle    Native handle of socket.
 * \param[in, out]  buffer    Buffer to write received data to.
 * \param[in, out]  ancillary_data_view   A view to ancillary data requests. The ancillary data will be written to the
 *                                        provided data view if there are data for the requested socket level and
 *                                        protocol. The used_bytes field will be updated on any write operation.
 *
 * \return Number of bytes that have been received. 0 if the buffer size is 0, always greater than 0 if the buffer size
 *         is larger than 0.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kBusy                       No message in socket error queue.
 * \error  osabstraction::OsabErrc::kTruncation                 Provided buffer did not have enough space to store the
 *                                                              received message. The trailing bytes of the message
 *                                                              have been dropped.
 *
 * \context         ANY
 *
 * \pre             handle identifies an open socket.
 * \pre             Input data buffer must consist of kMaxIOBufferArraySize or fewer entries.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> ReceiveFromErrorQueue(
    osabstraction::io::NativeHandle handle, ::amsr::core::Span<osabstraction::io::MutableIOBuffer> buffer,
    ::amsr::core::Span<AncillaryDataRequest> ancillary_data_view) noexcept;

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
 * \error  osabstraction::OsabErrc::kDisconnected               Asynchronous network error, e.g network interface down.
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
 *
 * \context         ANY
 *
 * \pre             Valid native handle.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<void> SetSocketOption(osabstraction::io::NativeHandle handle, SockOptLevel level, SockOptName name,
                                           SockOptConstValueView value_view) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_LINUX_H_

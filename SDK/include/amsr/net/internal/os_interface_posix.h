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
 *        \brief  Interface to posix specific operating system calls.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_POSIX_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_POSIX_H_

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/net/internal/specific_types.h"

namespace amsr {
namespace net {
namespace internal {
namespace os_interface {

/*!
 * \brief           Check flags for each received datagram of mmsghdr structure for errors.
 *
 * \param[in]       received_datagrams       Number of received datagrams.
 * \param[in, out]  message_buffers_view     Messages buffer.
 *
 * \return          Number of received messages.
 *
 * \error           OsabErrc::kUnexpected           Unexpected flags were received.
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
::amsr::core::Result<std::size_t> CheckReceiveBulkFlags(
    std::size_t received_datagrams, ::amsr::core::Span<BulkBufferType> message_buffers_view) noexcept;

/*!
 * \brief           Read data from a socket into the buffer.
 *
 * \param[in]       handle      Valid native handle.
 * \param[out]      buffer      View to the buffer used to store the incoming data.
 *
 * \return          Number of bytes that have been written into the given buffer.
 *
 * \error osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kBusy                     Operation would block. Try again later.
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<std::size_t> ReadFromSocket(osabstraction::io::NativeHandle handle,
                                                 ::amsr::core::Span<std::uint8_t> buffer) noexcept;
}  // namespace os_interface
}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_POSIX_H_

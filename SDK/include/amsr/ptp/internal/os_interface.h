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
 *
 *      \details  The actual implementation of the services may depend on the operating system. This file provide a
 *                unified interface to call a net related operating system service.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_H_

// Force include of qualified socket.h
#include "amsr/ptp/internal/system_header/sys/socket.h"

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/net_utils/network_stack_id.h"
#include "amsr/ptp/internal/types.h"
#include "osabstraction/io/native_types.h"

namespace amsr {
namespace ptp {
namespace internal {
namespace os_interface {

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
 * \spec
 *   requires true;
 * \endspec
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
 * \spec
 *   requires true;
 * \endspec
 */
void Close(osabstraction::io::NativeHandle handle) noexcept;

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
 * \spec
 *   requires true;
 * \endspec
 */
void SetBlockingMode(osabstraction::io::NativeHandle handle, SocketBlockingMode enable) noexcept;

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
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::Result<EthNative> ParseEther(::amsr::core::StringView addr_string) noexcept;

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
 *
 * \spec
 *   requires true;
 * \endspec
 */
void MakeEtherString(EthNative addr, ::amsr::core::Span<char> buffer) noexcept;

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
 *
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::Result<void> CallIoctl(osabstraction::io::NativeHandle handle, IoctlCmdName command,
                                     IoctlMutableValueView data_view) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_OS_INTERFACE_H_

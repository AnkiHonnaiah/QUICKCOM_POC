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
 *        \brief  Maps the internal errors for the basic socket class.
 *        \unit   osabstraction::net
 *
 *      \details  Maps the errors for every basic socket service request.
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_ERRORS_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/net/internal/types.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace net {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps the SetBlockingModeError to an error message and aborts process execution.
 * \param[in] number Error number
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
void MapSetBlockingModeError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   The process has insufficient privileges to create a
 *                                                            socket.
 * \retval osabstraction::OsabErrc::kResource                 Not enough system resources to create a socket.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapCreateSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     The process has insufficient privileges to bind address.
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable        The address is not available on the local machine.
 * \retval osabstraction::OsabErrc::kAddressError               The address is already in use on the local machine.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapBindSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error numbers to the appropriate abort messages.
 *
 * \details This function always aborts on a close error.
 *
 * \param[in] number The error number
 */
void MapCloseSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kResource           Not enough system resources to acquire the address.
 * \retval osabstraction::OsabErrc::kUnexpected         Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kDisconnected       The socket is not connected.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapGetPeerNameError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \details
 * Aborts on fatal errors and maps non-fatal errors. Not all operating system return all errors.
 *
 * \param[in] number Error number
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   The system settings do not support the passed option.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable      The address is not available on the local machine.
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Insufficient privileges to set socket option.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapSetSocketOptionError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \details
 * Aborts on fatal errors and maps non-fatal errors. Not all operating system return all errors.
 *
 * \param[in] number Error number
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   The system settings do not support the passed option.
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapGetSocketOptionError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kResource           Not enough system resources to acquire the address.
 * \retval osabstraction::OsabErrc::kUnexpected         Unexpected error that is mapped to no category.
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapGetSocketNameError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     The process has insufficient privileges to connect to
 *                                                              address.
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kDisconnected               The connection has been terminated by peer or the peer
 *                                                              is not reachable or the route to the peer is configured
 *                                                              as blackhole in the routing table.
 * \retval osabstraction::OsabErrc::kResource                   No ephemeral ports or routing cache entries available.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable        Cannot establish connection because local endpoint is
 *                                                              already used by another socket.
 * \retval osabstraction::OsabErrc::kBusy                       Connection cannot be completed directly. This is a valid
 *                                                              error for non blocking sockets.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_Blackhole
 */
::amsr::core::ErrorCode MapConnectStreamSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     The process has insufficient privileges to connect to
 *                                                              address.
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kDisconnected               The the peer is not reachable or the route to the peer
 *                                                              is configured as blackhole in the routing table.
 * \retval osabstraction::OsabErrc::kResource                   No ephemeral ports or buffer space or entries in
 *                                                              routing cache available.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable        Cannot establish connection because local endpoint is
 *                                                              already used by another socket.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_Blackhole
 */
::amsr::core::ErrorCode MapConnectDatagramSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \details
 * This function is called if the operating system notifies about a finished asynchronous connect call.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected           Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kDisconnected         The connection has been terminated by peer or the peer is not
 *                                                        reachable.
 * \retval osabstraction::OsabErrc::kBusy                 Connection has not completed yet. Try again later.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapCompleteNonBlockingConnectSocketError(
    osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected           Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable  Cannot establish connection because local endpoint is already
 *                                                        bound by another socket or the operating system used up all
 *                                                        ephemeral ports.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapListenSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to accept connection.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError     Not allowed to accept connection.

 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapAcceptSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to send datagram.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to send datagram.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError     The datagram exceeds the maximum allowed size (e.g.
 *                                                              message size > MTU size) or a kernel subsystem is not
 *                                                              configured.
 * \retval osabstraction::OsabErrc::kDisconnected               No route to destination exists or is configured as
 *                                                              blackhole in the routing table. Note that this may be an
 *                                                              asynchronous network error that has been caused by a
 *                                                              previous datagram.
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_Blackhole
 */
::amsr::core::ErrorCode MapSendmsgDatagramSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to send data.
 * \retval osabstraction::OsabErrc::kDisconnected               The connection has been terminated by peer or the peer
 *                                                              is no longer reachable (this includes but is not limited
 *                                                            to that the route to the peer has been configured as
 *                                                            blackhole in the routing table after establishing the
 *                                                            connection)..
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     Permission to access a socket denied.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapSendmsgStreamSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to send datagram.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to send datagram.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError     The datagram exceeds the maximum allowed size (e.g.
 *                                                              message size > MTU size) or a kernel subsystem is not
 *                                                              configured.
 * \retval osabstraction::OsabErrc::kDisconnected               No route to destination exists or is configured as
 *                                                              blackhole in the routing table. Note that this may be an
 *                                                              asynchronous network error that has been caused by a
 *                                                              previous datagram.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIP_Blackhole
 */
::amsr::core::ErrorCode MapSendToSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to receive data.
 * \retval osabstraction::OsabErrc::kDisconnected               The connection has been terminated by peer or the peer
 *                                                              is no longer reachable.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapRecvmsgStreamSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error for recvmsg() call.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
 * \retval osabstraction::OsabErrc::kDisconnected               No route to destination exists. Note that this may be an
 *                                                              asynchronous network error that has been caused by a
 *                                                              previous datagram.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapReceiveFromSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief   Maps error number to the osabstraction error for recvmmsg() call.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to receive datagram.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to receive datagram.
 * \retval osabstraction::OsabErrc::kDisconnected               No route to destination exists. Note that this may be an
 *                                                              asynchronous network error that has been caused by a
 *                                                              previous datagram.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError     The kernel subsystem is not configured.
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
::amsr::core::ErrorCode MapReceiveFromBulkSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category. The
 *                                                              error may be ioctl command specific.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation..
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError     Native handle is not associated  with character-special
 *                                                              device or device does not support the operation.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     (Linux Only) Not allowed to perform the operation.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapIoctlError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps reading from socket error to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapReadFromSocketError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
 * \retval osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
 * \retval osabstraction::OsabErrc::kDoesNotExist               No address is attached to host name.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapGetAddressesInfoError(osabstraction::internal::OsErrorNumber number) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_ERRORS_H_

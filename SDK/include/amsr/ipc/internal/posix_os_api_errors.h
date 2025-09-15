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
 *        \brief  Provides error mappings for POSIX operating system APIs used in Safe IPC.
 *        \unit   osabstraction::ipc::IpcOsInterface_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_POSIX_OS_API_ERRORS_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_POSIX_OS_API_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps Unix Domain Socket creation error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error during creation of unix domain socket.
 * \retval osabstraction::OsabErrc::kResource                 Not enough system resources to create unix domain socket.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Not allowed to create unix domain socket.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapCreateUnixDomainSocketError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps enabling non-blocking mode error to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapEnableNonBlockingModeError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of bind operation to the osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error code during binding of unix domain socket.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to bind unix domain socket to specified path.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable     The specified path is already used or not available.
 * \retval osabstraction::OsabErrc::kResource                Not enough memory to bind unix domain socket.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError  The specified path points to a read-only file system.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapBindSocketError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of listen operation to the osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error during unix domain socket listen
 *                                                           operation.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable     Another socket is listening to the address of this unix
 *                                                           domain socket.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapListenSocketError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of accept operation to the osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error during unix domain socket accept
 *                                                           operation.
 * \retval osabstraction::OsabErrc::kBusy                    Accepting unix domain socket connection would block.
 * \retval osabstraction::OsabErrc::kDisconnected            Connection establishment for unix domain socket
 *                                                           connection was aborted.
 * \retval osabstraction::OsabErrc::kResource                Not enough resources to accept next unix domain socket
 *                                                           connection.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapAcceptSocketError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps connect error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error during unix domain socket connect
 *                                                              operation.
 * \retval osabstraction::OsabErrc::kBusy                       Connection establishment of unix domain socket
 *                                                              connection is in progress.
 * \retval osabstraction::OsabErrc::kAddressNotAvailable        No unix domain socket server is listening on the
 *                                                              specified path or the specified path cannot be resolved.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges     Not allowed to connect to unix domain socket server.
 * \retval osabstraction::OsabErrc::kDisconnected               Connection establishment for unix domain socket
 *                                                              connection was aborted or timed out.
 * \retval osabstraction::OsabErrc::kResource                   Not enough system resources to connect to unix domain
 *                                                              socket server.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapConnectSocketError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps getting socket option to the osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error while getting socket option for unix
 *                                                           domain socket.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapGetSocketOptionError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps sendmsg error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                 Unexpected error while sending over unix domain
 *                                                              socket.
 * \retval osabstraction::OsabErrc::kBusy                       Sending over unix domain socket connection would block.
 * \retval osabstraction::OsabErrc::kDisconnected               The connection has been terminated by peer or the peer
 *                                                              is no longer reachable.
 * \retval osabstraction::OsabErrc::kResource                   Not enough resources to send over unix domain socket
 *                                                              connection or sent file descriptor would exceed the
 *                                                              RLIMIT_NOFILE limit of the IPC peer.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapSendToSocketError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps recvmsg error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected                Unexpected error while receiving from unix domain socket.
 * \retval osabstraction::OsabErrc::kBusy                      Receiving from unix domain socket connection would block.
 * \retval osabstraction::OsabErrc::kDisconnected              Connection to unix domain socket peer has been separated.
 * \retval osabstraction::OsabErrc::kResource                  Not enough resources to receive from unix domain socket
 *                                                             connection.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapReceiveFromSocketError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of file creation operation to the osabstraction error domain.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error occurred during creation of shared memory
 *                                                           file.
 * \retval osabstraction::OsabErrc::kDoesNotExist            Path could not be resolved.
 * \retval osabstraction::OsabErrc::kAlreadyExists           Path already exists.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create shared memory file.
 * \retval osabstraction::OsabErrc::kResource                Not enough system resources to create shared memory file.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError  Cannot create shared memory file on a read-only file
 *                                                           system.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapCreateShmFileError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of file open operation to the osabstraction error domain.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error occurred during opening of shared memory
 *                                                           file.
 * \retval osabstraction::OsabErrc::kDoesNotExist            File does not exist or the path could not be resolved or
 *                                                           the path does not refer to a regular shared memory file.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to open shared memory file.
 * \retval osabstraction::OsabErrc::kResource                Not enough system resources to open shared memory file.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError  Cannot open file for writing as it resides on a read-only
 *                                                           file system or the file is too big for this process.
 * \retval osabstraction::OsabErrc::kBusy                    The shared memory file cannot be opened due to a
 *                                                           conflicting use.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapOpenShmFileError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of chmod operation to the osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error during changing of file mode.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to change mode of requested file.
 * \retval osabstraction::OsabErrc::kDoesNotExist            File specified in chmod call does not exist or the
 *                                                           file path could not be resolved.
 * \retval osabstraction::OsabErrc::kResource                Not enough memory to change mode of file.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError  File system does not allow changing file modes or a file
 *                                                           system I/O error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapChangeFileModeError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of fstat call to the osabstraction error domain.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error while determining the file size.
 * \retval osabstraction::OsabErrc::kResource                Not enough memory to get file size.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError  File size is too big for this process.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapGetFileSizeError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of ftruncate call to the osabstraction error domain.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error during truncation of file.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError  File cannot be truncated because the file (system) does
 *                                                           not support or allow truncation or the specified size is
 *                                                           not valid for the file or a file system I/O error occurred.
 * \retval osabstraction::OsabErrc::kBusy                    File cannot be truncated because it is currently being
 *                                                           executed.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapTruncateFileError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps error of unlink operation to the osabstraction error.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kDoesNotExist            File specified in unlink call does not exist or
 *                                                           the file path could not be resolved.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to unlink requested file.
 * \retval osabstraction::OsabErrc::kResource                Not enough memory to unlink file.
 * \retval osabstraction::OsabErrc::kBusy                    The requested file cannot be unlinked because it is
 *                                                           currently in use.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError  File specified in unlink call resides on read-only file
 *                                                           system or a file system I/O error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto MapUnlinkError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

}  // namespace os_interface
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_POSIX_OS_API_ERRORS_H_

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
 *        \brief  Provides interface for POSIX/UNIX operating system APIs used in Safe IPC.
 *        \unit   osabstraction::ipc::IpcOsInterface_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_POSIX_OS_API_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_POSIX_OS_API_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/socket.h>
#include <sys/un.h>
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/credentials.h"
#include "amsr/ipc/internal/data_types.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/native_types.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace os_interface {

/*!
 * \brief Type for a Unix Domain Socket address.
 */
struct UnixDomainSocketAddress {
  /*!
   * \brief Socket address.
   */
  sockaddr_un value;
};

/*!
 * \brief Stores backlog size for a listening socket.
 */
struct BacklogSize {
  /*!
   * \brief Backlog size.
   */
  std::int32_t value;
};

/*!
 * \brief Information about an amount of bytes that have been or shall be transferred over IPC.
 */
struct TransferredBytes {
  /*!
   * \brief Number of bytes.
   */
  std::size_t value;
};

/*!
 * \brief           Creates a new Unix Domain socket.
 *
 * \details         Non-blocking mode and close-on-exec mode are enabled for the created socket.
 *
 * \return          File descriptor of created Unix Domain Socket.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to create unix domain
 *                                                                    socket.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create unix domain socket.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto CreateUnixDomainSocket() noexcept -> ::amsr::core::Result<osabstraction::io::FileDescriptor>;

/*!
 * \brief           Binds a Unix Domain socket to a Unix Domain socket address.
 *
 * \param[in]       socket_fd   Valid file descriptor of the Unix Domain socket to bind.
 * \param[in]       address     Address to bind the socket to.
 *
 * \return          On success a void value, on failure the error code.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error code during binding of unix
 *                                                                    domain socket.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to bind unix domain socket to
 *                                                                    specified path.
 * \error           osabstraction::OsabErrc::kAddressNotAvailable     The specified path is already used or not
 *                                                                    available.
 * \error           osabstraction::OsabErrc::kResource                Not enough memory to bind unix domain socket.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The specified path points to a read-only file
 *                                                                    system or a file system I/O error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto BindSocket(osabstraction::io::NativeHandle socket_fd, UnixDomainSocketAddress const& address) noexcept
    -> ::amsr::core::Result<void>;

/*!
 * \brief           Converts Unix domain socket into a passively listening endpoint.
 *
 * \param[in]       server_socket_fd    Valid file descriptor of the Unix Domain socket to put into listening mode.
 * \param[in]       backlog_size        Size that the backlog of the listening socket shall have.
 *
 * \return          On success a void value, on failure the error code.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error during unix domain socket listen
 *                                                                    operation.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to perform listen for unix domain
 *                                                                    socket server.
 * \error           osabstraction::OsabErrc::kAddressNotAvailable     Another socket is listening to the address of this
 *                                                                    unix domain socket.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources perform listen for
 *                                                                    unix domain socket server.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto ListenSocket(osabstraction::io::NativeHandle server_socket_fd, BacklogSize backlog_size) noexcept
    -> ::amsr::core::Result<void>;

/*!
 * \brief           Accepts an incoming Unix Domain socket connection request.
 *
 * \param[in]       server_socket_fd    Valid file descriptor of the Unix domain socket server for which a connection
 *                                      shall be accepted.
 *
 * \return          File descriptor of the accepted Unix Domain socket connection. Non-blocking mode and close-on-exec
 *                  mode are enabled for this socket.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to accept connection.
 * \error           osabstraction::OsabErrc::kBusy                    Call would block.
 * \error           osabstraction::OsabErrc::kDisconnected            Connection was disconnected or aborted.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Cannot accept next unix domain socket connection
 *                                                                    because the socket manager could not be found or a
 *                                                                    protocol error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \trace           DSGN-Osab-Linux_AvoidSocketCloseRaceCondition
 */
auto AcceptConnection(osabstraction::io::NativeHandle server_socket_fd) noexcept
    -> ::amsr::core::Result<osabstraction::io::FileDescriptor>;

/*!
 * \brief           Issues a connection request to specified Unix domain socket server address.
 *
 * \param[in]       socket_fd        Valid file descriptor of the Unix Domain socket to connect.
 * \param[in]       server_address   Unix Domain Socket server address to connect to.
 *
 * \return          On success a void value, on failure the error code.
 *
 * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error during unix domain socket
 *                                                                     connect operation.
 * \error           osabstraction::OsabErrc::kBusy                     Connection establishment of unix domain socket
 *                                                                     connection is in progress.
 * \error           osabstraction::OsabErrc::kAddressNotAvailable      No unix domain socket server is listening on the
 *                                                                     specified path or the specified path cannot be
 *                                                                     resolved.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges   Not allowed to connect to unix domain socket
 *                                                                     server.
 * \error           osabstraction::OsabErrc::kDisconnected             Connection establishment for unix domain socket
 *                                                                     connection was aborted or timed out.
 * \error           osabstraction::OsabErrc::kResource                 Not enough system resources to connect to unix
 *                                                                     domain socket server.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError   Failed to connect to unix domain socket server
 *                                                                     because a file system I/O error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto ConnectSocket(osabstraction::io::NativeHandle socket_fd, UnixDomainSocketAddress const& server_address) noexcept
    -> ::amsr::core::Result<void>;

/*!
 * \brief           Checks if an asynchronous connect on a Unix Domain socket completed successfully.
 *
 * \param[in]       socket_fd   Valid file descriptor of the Unix Domain socket for which Connect() was called.
 *
 * \return          On success a void value, on failure the error code.
 *
 * \error           osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kBusy                      Connection establishment of unix domain socket
 *                                                                      connection is in progress.
 * \error           osabstraction::OsabErrc::kAddressNotAvailable       No unix domain socket server is listening on the
 *                                                                      specified path or the specified path cannot be
 *                                                                      resolved.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges    Not allowed to connect to unix domain socket
 *                                                                      server.
 * \error           osabstraction::OsabErrc::kDisconnected              Connection establishment for unix domain socket
 *                                                                      connection was aborted or timed out.
 * \error           osabstraction::OsabErrc::kResource                  Not enough system resources to connect to unix
 *                                                                      domain socket server.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError    Failed to connect to unix domain socket server
 *                                                                      because a file system I/O error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto CompleteNonBlockingConnect(osabstraction::io::NativeHandle socket_fd) noexcept -> ::amsr::core::Result<void>;

/*!
 * \brief           Sends data over a Unix Domain Socket connection.
 *
 * \param[in]       socket_fd    Valid file descriptor of the Unix Domain socket to send the data from.
 * \param[in]       buffer       Buffer view to the send buffer.
 * \param[in]       fd_to_share  Valid file descriptor to share over the Unix Domain socket connection with the peer.
 *                               osabstraction::io::kInvalidNativeHandle if no file descriptor shall be shared.
 *
 * \return          Number of sent bytes (always > 0).
 *
 * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error while sending over unix domain
 *                                                                     socket.
 * \error           osabstraction::OsabErrc::kBusy                     Sending over unix domain socket connection would
 *                                                                     block.
 * \error           osabstraction::OsabErrc::kDisconnected             The connection has been terminated by peer or the
 *                                                                     peer is no longer reachable.
 * \error           osabstraction::OsabErrc::kResource                 Not enough resources to send over unix domain
 *                                                                     socket connection or sent file descriptor would
 *                                                                     exceed the RLIMIT_NOFILE limit of the IPC peer.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto SendToSocket(osabstraction::io::NativeHandle socket_fd,
                  ::amsr::core::Span<osabstraction::io::ConstIOBuffer> buffer,
                  osabstraction::io::NativeHandle fd_to_share = osabstraction::io::kInvalidNativeHandle) noexcept
    -> ::amsr::core::Result<TransferredBytes>;

/*!
 * \brief   Result of a ReceiveFromSocket() call.
 */
struct ReceiveFromSocketResult {
  /*!
   * \brief   Number of received bytes (always larger that 0).
   */
  TransferredBytes num_received_bytes{};
  /*!
   * \brief   File descriptor received in a SCM_RIGHTS control message or an invalid file descriptor if no valid
   *          SCM_RIGHTS control message was received.
   */
  osabstraction::io::FileDescriptor received_fd;
};

/*!
 * \brief           Receives data from a Unix Domain Socket connection.
 *
 * \param[in]       socket_fd   Valid file descriptor of the Unix Domain socket to receive data from.
 * \param[out]      buffer      Buffer view to the receive buffers. The overall size of the receive buffers has to be
 *                              larger than 0.
 *
 * \return          Number of received bytes (always > 0 as kDisconnected is returned if 0 bytes are received) and
 *                  potentially a file descriptor that was received in a SCM_RIGHTS control message (or an invalid
 *                  file descriptor).
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error while receiving from unix domain
 *                                                                    socket.
 * \error           osabstraction::OsabErrc::kBusy                    Receiving from unix domain socket connection would
 *                                                                    block.
 * \error           osabstraction::OsabErrc::kDisconnected            Connection to unix domain socket peer has been
 *                                                                    separated.
 * \error           osabstraction::OsabErrc::kResource                Not enough resources to receive from unix domain
 *                                                                    socket connection.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto ReceiveFromSocket(osabstraction::io::NativeHandle socket_fd,
                       ::amsr::core::Span<osabstraction::io::MutableIOBuffer> buffer) noexcept
    -> ::amsr::core::Result<ReceiveFromSocketResult>;

/*!
 * \brief           Creates a regular file to use as shared memory object.
 *
 * \details         Close-on-exec mode is enabled for the returned file descriptor.
 *
 * \param[in]       file_path     File path name for the shared memory object.
 * \param[in]       access_mode   Access mode to open the file with (read-only / write-only / read-write).
 * \param[in]       file_mode     Unix permissions to create the shared memory object with.
 *
 * \return          File descriptor of the opened file.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error occurred during creation of
 *                                                                    shared memory file.
 * \error           osabstraction::OsabErrc::kDoesNotExist            Path could not be resolved.
 * \error           osabstraction::OsabErrc::kAlreadyExists           Path already exists.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create shared memory file.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to create shared
 *                                                                    memory file.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Cannot create file on a read-only file system or
 *                                                                    the underlying file system is corrupted or the
 *                                                                    underlying file system does not support creating
 *                                                                    files or the media of the underlying file system
 *                                                                    is not available anymore.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto CreateShmFile(FilePath file_path, OpenAccessMode access_mode, FileMode file_mode) noexcept
    -> ::amsr::core::Result<osabstraction::io::FileDescriptor>;

/*!
 * \brief           Opens a regular file that is used as shared memory object.
 *
 * \details         Close-on-exec mode is enabled for the returned file descriptor.
 *
 * \param[in]       file_path     File path name of the shared memory object that shall be opened.
 * \param[in]       access_mode   Access mode to open the file with (read-only / write-only / read-write).
 *
 * \return          File descriptor of the opened file.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error occurred during opening of shared
 *                                                                    memory file.
 * \error           osabstraction::OsabErrc::kDoesNotExist            File does not exist or the path could not be
 *                                                                    resolved or the path does not refer to a regular
 *                                                                    shared memory file.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to open shared memory file.
 * \error           osabstraction::OsabErrc::kResource                Not enough system resources to open shared memory
 *                                                                    file.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Cannot open file for writing as it resides on a
 *                                                                    read-only file system or the file is too big for
 *                                                                    this process or the underlying file system is
 *                                                                    corrupted or the underlying file system does not
 *                                                                    support opening files or the media of the
 *                                                                    underlying file system is not available anymore.
 * \error           osabstraction::OsabErrc::kBusy                    The shared memory file cannot be opened due to a
 *                                                                    conflicting use.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto OpenShmFile(FilePath file_path, OpenAccessMode access_mode) noexcept
    -> ::amsr::core::Result<osabstraction::io::FileDescriptor>;

/*!
 * \brief           Changes the file mode of a file.
 *
 * \details         This can be used to e.g. change the Unix permissions of the file.
 *
 * \param[in]       file_path     File path of the file whose mode shall be changed.
 * \param[in]       file_mode     New mode that shall be set for the file.
 *
 * \return          On success a void value, on failure the error code.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error during changing of file mode.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to change mode of requested file.
 * \error           osabstraction::OsabErrc::kDoesNotExist            File specified in chmod call does not exist or the
 *                                                                    file path could not be resolved.
 * \error           osabstraction::OsabErrc::kResource                Not enough memory to change mode of file.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  File system does not allow changing file modes or
 *                                                                    a file system I/O error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto ChangeFileMode(FilePath file_path, FileMode file_mode) noexcept -> ::amsr::core::Result<void>;

/*!
 * \brief           Get the size of a file.
 *
 * \param[in]       native_handle     Valid file descriptor of the file whose size shall be queried.
 *
 * \return          Size of the file in bytes.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error while determining the file size.
 * \error           osabstraction::OsabErrc::kResource                Not enough memory to get file size.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  File size is too big for this process or the file
 *                                                                    system does not support the fstat call or a file
 *                                                                    system I/O error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto GetFileSize(osabstraction::io::NativeHandle native_handle) noexcept -> ::amsr::core::Result<FileSize>;

/*!
 * \brief           Truncates a file to a specified size.
 *
 * \param[in]       native_handle     Valid file descriptor of the file that shall be truncated.
 * \param[in]       size              File size (in bytes) that the file shall be truncated to, must not exceed the max
 *                                    value that can be represented by the file offset type off_t.
 *
 * \return          On success a void value, on failure the error code.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error during truncation of file.
 * \error           osabstraction::OsabErrc::kResource                Not enough memory to truncate file.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  File cannot be truncated because the file (system)
 *                                                                    does not support or allow truncation or the
 *                                                                    specified size is not valid for the file or a file
 *                                                                    system I/O error occurred.
 * \error           osabstraction::OsabErrc::kBusy                    File cannot be truncated because it is currently
 *                                                                    being executed.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto TruncateFile(osabstraction::io::NativeHandle native_handle, FileSize size) noexcept -> ::amsr::core::Result<void>;

/*!
 * \brief           Unlink a file.
 *
 * \param[in]       file_path     File path name of the file to unlink.
 *
 * \return          On success a void value, on failure the error code.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kDoesNotExist            File specified in unlink call does not exist or
 *                                                                    the file path could not be resolved.
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to unlink requested file.
 * \error           osabstraction::OsabErrc::kResource                Not enough memory to unlink file.
 * \error           osabstraction::OsabErrc::kBusy                    The requested file cannot be unlinked because it
 *                                                                    is currently in use.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  File system does not allow unlinking of files or a
 *                                                                    file system I/O error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto UnlinkFile(FilePath file_path) noexcept -> ::amsr::core::Result<void>;

}  // namespace os_interface
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_POSIX_OS_API_H_

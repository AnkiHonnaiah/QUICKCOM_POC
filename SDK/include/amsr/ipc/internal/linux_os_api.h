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
 *        \brief  Provides interface for Linux operating system APIs used in Safe IPC.
 *        \unit   osabstraction::ipc::IpcOsInterface_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_LINUX_OS_API_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_LINUX_OS_API_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/ipc/credentials.h"
#include "osabstraction/io/native_types.h"
#include "osabstraction/osab_error_domain.h"
#include "osabstraction/process/process_types.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace os_interface {

/*!
 * \brief  Credentials of an Unix Domain socket peer process.
 */
struct SocketPeerCredentials {
  /*!
   * \brief  Process ID of the peer process.
   */
  osabstraction::process::ProcessId peer_pid;

  /*!
   * \brief  User ID of the peer process.
   */
  Credentials peer_uid;
};

/*!
 * \brief           Gets the process ID and user ID of the communication peer of an Unix Domain socket.
 *
 * \details         The Unix Domain socket has to be a stream socket and has to be connected to a peer Unix Domain
 *                  socket. It does not matter if the peer Unix Domain socket has already closed the connection, the
 *                  information about the peer will still be returned successfully.
 *
 * \param[in]       socket_fd   Valid file descriptor of the Unix Domain socket whose peer's PID and UID shall be
 *                              queried.
 *
 * \return          PID and UID of the connected socket peer.
 *
 * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto GetSocketPeerCredentials(osabstraction::io::NativeHandle socket_fd) noexcept
    -> ::amsr::core::Result<SocketPeerCredentials>;

/*!
 * \brief       Maps Reactor1::AddMonitoredEvents() / Reactor1::RemoveMonitoredEvents() / Reactor1::SetMonitoredEvents()
 *              errors.
 *
 * \details     Aborts on unexpected errors. As all errors are unexpected the return value is void and the function does
 *              not return.
 *
 * \param[in]   error   Error code returned by the Reactor1 API.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 */
[[noreturn]] void MapReactorChangeRegistrationErrors(::amsr::core::ErrorCode const& error) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_LINUX_OS_API_H_

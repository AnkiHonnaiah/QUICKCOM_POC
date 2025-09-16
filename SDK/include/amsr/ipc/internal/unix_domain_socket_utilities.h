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
 *        \brief  Provides utilities to work with Unix Domain sockets.
 *        \unit   osabstraction::ipc::IpcOsInterface_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_UNIX_DOMAIN_SOCKET_UTILITIES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_UNIX_DOMAIN_SOCKET_UTILITIES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/ipc/internal/posix_os_api.h"
#include "amsr/ipc/unicast_address.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace os_interface {

/*!
 * \brief           Initializes a UnixDomainSocketAddress structure from a server address.
 *
 * \param[in]       server_address              IPC server address that should be converted into a Unix Domain socket
 *                                              address.
 *
 * \return          Initialized Unix Domain socket address.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto ResolveUnixDomainSocketServerAddress(UnicastAddress const& server_address) noexcept -> UnixDomainSocketAddress;

}  // namespace os_interface
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_UNIX_DOMAIN_SOCKET_UTILITIES_H_

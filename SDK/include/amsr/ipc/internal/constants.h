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
 *        \brief  Provides constants used in Safe IPC.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONSTANTS_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONSTANTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstddef>
#include <cstdint>

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Maximum number of times that IPC peers should try to create a shared memory file if a file name collision
 *          occurred.
 */
constexpr std::size_t kMaxFilenameCollisionRetries{10};

/*!
 * \brief   Server side connection establishment timeout.
 * \details A server will abort the connection establishment with a client if it takes longer than this value (if the
 *          server uses connection establishment timeouts).
 */
constexpr std::chrono::nanoseconds kConnectionEstablishmentTimeout{10'000'000'000};

/*!
 * \brief   Value of the byte that is sent over the Unix Domain socket connection to notify the peer that the connection
 *          was closed by the user.
 */
constexpr std::uint8_t kSafeIpcConnectionClosedByte{253U};

/*!
 * \brief   Value of the byte that is sent over the Unix Domain socket connection when a file descriptor is shared with
 *          the peer process by sending a SCM_RIGHTS control message over the socket.
 */
constexpr std::uint8_t kFileDescriptorExchangeByte{254U};

/*!
 * \brief   Value of the byte that is sent over the Unix Domain socket connection to notify the peer that the shared
 *          memory is readable or writable.
 */
constexpr std::uint8_t kReadWriteNotificationByte{255U};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_CONSTANTS_H_

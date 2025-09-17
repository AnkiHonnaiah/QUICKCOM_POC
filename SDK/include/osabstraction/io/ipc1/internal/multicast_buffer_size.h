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
/*!       \file   multicast_buffer_size.h
 *        \brief  Using custom buffer sizes for multicast.
 *        \unit   osabstraction::ipc1
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_BUFFER_SIZE_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_BUFFER_SIZE_H_

#include <cstdint>

namespace osabstraction {
namespace io {
namespace ipc1 {
namespace internal {

/*!
 * \brief Gets the buffer size configured at compile time for multicast senders and receivers.
 *
 * \return The buffer size to set, or 0 if the OS default shall be used.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto GetConfiguredBufferSize() -> std::int32_t;

}  // namespace internal
}  // namespace ipc1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_BUFFER_SIZE_H_

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
/*!       \file
 *        \brief  Maps the internal errors for MulticastSender, MulticastReceiver classes.
 *        \unit   osabstraction::ipc1
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_ERRORS_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_ERRORS_H_

#include "amsr/core/abort.h"
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/net/ip/udp.h"

namespace osabstraction {
namespace io {
namespace ipc1 {
namespace internal {

/*!
 * \brief           Filters socket errors happened during Init().
 *
 * \param[in]       error Error returned by osabstraction::io::udp::Socket functions during Init()
 *
 * \error           -
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIpcMulticastReceiverPosix,
 *                  DSGN-Osab-BasicIpcMulticastSenderPosix
 *
 * \vprivate        Vector component internal API
 */
void FilterUdpSocketInitErrors(::amsr::core::ErrorCode const& error) noexcept;

/*!
 * \brief           Filters socket errors happened during Send() or passed to the completion callback.
 *
 * \param[in]       error Error returned by osabstraction::io::udp::Socket functions during Send() or passed to
 *                        the completion notification callback.
 *
 * \error           -
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIpcMulticastSenderPosix
 *
 * \vprivate        Vector component internal API
 */
void FilterUdpSocketSendErrors(::amsr::core::ErrorCode const& error) noexcept;

/*!
 * \brief           Filters socket errors happened during ReceiveAsync() or passed to the receive completion callback.
 *
 * \param[in]       error Error returned by osabstraction::io::udp::Socket functions during ReceiveFromAsync() or
 *                        passed to the completion notification callback.
 *
 * \error           -
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-BasicIpcMulticastReceiverPosix
 *
 * \vprivate        Vector component internal API
 */
void FilterUdpSocketReceiveErrors(::amsr::core::ErrorCode const& error) noexcept;

}  // namespace internal
}  // namespace ipc1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_ERRORS_H_

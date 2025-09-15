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
 *        \brief  Collection of OS specify type safe types that are used in in operating system interface.
 *        \unit   osabstraction::net
 *
 *      \details  Most operating system arguments are not type safe. The provided types encapsulate these types.
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_SPECIFIC_TYPES_H_
#define LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_SPECIFIC_TYPES_H_

#include <ifaddrs.h>  // Required for ifaddrs (which is required in the file that includes this file).
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <netinet/in.h>  // Required for ifaddrs (which is required in the file that includes this file).
#include <sys/ioctl.h>
#include <sys/socket.h>  // Required for mmsghdr
#include <cstdint>

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief The ioctl request type.
 *
 * Shall only be used together with the ioctl() call.
 */
using IoctlRequestType = unsigned long;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

/*!
 * \brief Type of the ireq.ifru_flags.
 *
 */
using IfrIfFlags = decltype(ifreq::ifr_flags);

/*!
 * \brief The message structure to store a bulk of datagrams.
 */
using BulkBufferType = mmsghdr;

/*!
 * \brief The message header for netlink messages.
 */
using RouteSocketMessageHdr = struct nlmsghdr;

/*!
 * \brief RTM_NEWADDR and RTM_DELADDR message header.
 */
using RouteAddressMessageHdr = struct ifaddrmsg;

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_SPECIFIC_TYPES_H_

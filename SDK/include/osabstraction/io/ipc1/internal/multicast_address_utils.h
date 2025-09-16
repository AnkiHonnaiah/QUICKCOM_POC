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
/*!        \file  multicast_address_utils.h
 *        \brief  Contains services to manage IPv6 addresses for Multicast BasicIPC.
 *        \unit   osabstraction::ipc1
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_ADDRESS_UTILS_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_ADDRESS_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "osabstraction/io/ipc1/multicast_address.h"

namespace osabstraction {
namespace io {
namespace ipc1 {
namespace internal {

/*!
 * \brief Defines the interface used by multicast BasicIPC.
 *
 * \details
 * The implementation uses the loopback device with following assumptions:
 * - There exists a loopback device with interface index 0x1.
 * - The loopback device is capable to send and receive IPv6 multicast messages.
 *
 * \note
 * On Linux with loopback device named "lo" the user may use the following commands:
 * - sudo ifconfig lo multicast
 * - sudo ip -6 route add ff01::/16 dev lo
 */
static constexpr std::uint16_t kMulticastInterface{1u};

/*!
 *  \brief   Defines the operating system port used by all basic IPC multicast receivers.
 *  \details This value should be outside of the dynamic ports range (https://en.wikipedia.org/wiki/Ephemeral_port)
 */
static constexpr std::uint16_t kMulticastReceiverPort{10000u};

/*!
 *  \brief   Length of an IPv6 address in the format 1234:5678:90AB:CDEF:1234:5678:90AB:CDEF
 *  \details It is assumed that no "::" is used and that the address contains no interface. Null termination is not
 *           considered.
 */
static constexpr std::size_t kIpv6AddressStringLengthWithoutInterface{(8 * 4) + 7};  // 8 * 4 hex numbers + 7 ':'

/*!
 * \brief     Returns a string representation of a IPv6 multicast address to be used for Multicast BasicIPC.
 *
 * \details   The address should look like:
 *            FF01:x:x:0xA000:group_address.Domain[31...16]:group_address.Domain[15...0]:
 *            group_address.Group[31...16]:group_address.Group[15...0]
 *
 * \param[in]       group_address    Multicast group address
 * \param[in,out]   address_buffer   Buffer of characters to be filled out with the address.
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
void GetMulticastAddressString(MulticastAddress const& group_address,
                               ::amsr::core::Array<char, kIpv6AddressStringLengthWithoutInterface + 1>& address_buffer);

}  // namespace internal
}  // namespace ipc1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_IPC1_INTERNAL_MULTICAST_ADDRESS_UTILS_H_

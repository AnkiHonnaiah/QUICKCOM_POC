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
 *        \brief  Linux specific types and constants.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_SPECIFIC_CONSTANTS_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_SPECIFIC_CONSTANTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/ether.h>  // ether_aton()
#include <sys/socket.h>
#include <cstdint>

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief           Maximal length of a network interface name.
 *
 * \details         One character must be subtracted from IF_NAMESIZE because of the terminating \0 character.
 */
static constexpr std::uint8_t const kMaxNetworkInterfaceName{IF_NAMESIZE - 1};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibilityOsDependentConstant
/*!
 * \brief           The type of address to use for ethernet sockets.
 */
using SockaddrEther = sockaddr_ll;

/*!
 * \brief           The socket address family to use for ethernet sockets.
 */
static constexpr sa_family_t const kAddressFamilyEthernet{AF_PACKET};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibilityOsDependentConstant
/*!
 * \brief           The socket type for PTP sockets.
 */
static constexpr std::int32_t const kSocketTypePtp{SOCK_RAW};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibilityOsDependentConstant
/*!
 * \brief           The socket protocol for ptp.
 */
static constexpr std::uint16_t const kSocketProtocolPtp{ETH_P_ALL};

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_OS_SPECIFIC_CONSTANTS_H_

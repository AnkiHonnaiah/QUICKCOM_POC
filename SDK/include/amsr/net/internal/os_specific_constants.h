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
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_OS_SPECIFIC_CONSTANTS_H_
#define LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_OS_SPECIFIC_CONSTANTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/ether.h>  // ether_aton()
#include <netinet/in.h>
#include <netinet/tcp.h>  // TCP_NODELAY socket option
#include <sys/socket.h>

#include <climits>  // IOV_MAX
#include <cstdint>
#include <limits>  // Required for numeric_limits<>

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief           Maximal length of a network interface name.
 *
 * \details         One character must be subtracted from IF_NAMESIZE because of the terminating \0 character.
 */
static constexpr std::uint8_t const kMaxNetworkInterfaceName{IF_NAMESIZE - 1};

/*!
 * \brief           The type of address to use for ethernet sockets.
 */
using SockaddrEther = sockaddr_ll;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibilityOsDependentConstant
/*!
 * \brief           The socket address family to use for ethernet addresses.
 */
static constexpr sa_family_t const kAddressFamilyEthernet{AF_PACKET};

/*!
 * \brief           Number of bytes an Ethernet address consists of.
 */
static constexpr std::uint8_t const kEthernetAddressLength{ETH_ALEN};

/*!
 * \brief           Socket send buffer size.
 */
static constexpr std::int32_t const kSendBufferSize{SO_SNDBUF};

/*!
 * \brief           Unicast hop limit.
 */
static constexpr std::int32_t const kIPv6UnicastHops{IPV6_UNICAST_HOPS};

/*!
 * \brief           Multicast hop limit.
 */
static constexpr std::int32_t const kIPv6MulticastHops{IPV6_MULTICAST_HOPS};

/*!
 * \brief           Interface to use for outgoing multicast packets.
 */
static constexpr std::int32_t const kIPv6MulticastIF{IPV6_MULTICAST_IF};

/*!
 * \brief           Socket option to enable IPv6 multicast packets being delivered back to the local application.
 */
static constexpr std::int32_t const kIPv6MulticastLoop{IPV6_MULTICAST_LOOP};

/*!
 * \brief           Protocol to enable IPv6 multicast packets being delivered back to the local application.
 */
static constexpr std::int32_t const kIPv6MulticastLoopProtocol{IPPROTO_IPV6};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibilityOsDependentConstant
/*!
 * \brief           Option to bind socket to device.
 */
static constexpr std::int32_t const kBindToDevice{SO_BINDTODEVICE};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibilityOsDependentConstant
/*!
 * \brief           Protocol-defined priority for sent packages.
 */
static constexpr std::int32_t const kVLANPriority{SO_PRIORITY};

/*!
 * \brief           Number of maximum probes sent by TCP before dropping the connection.
 */
static constexpr std::int32_t const kTCPKeepCount{TCP_KEEPCNT};

/*!
 * \brief           Idle time interval where no TCP Keep Alive probes shall be sent. If this time expires, Keep Alive
 *                  probes are sent by TCP.
 */
static constexpr std::int32_t const kTCPKeepIdle{TCP_KEEPIDLE};

/*!
 * \brief           Interval time between the transmission of TCP Keep Alive probes.
 */
static constexpr std::int32_t const kTCPKeepInterval{TCP_KEEPINTVL};

// VECTOR Next Line AutosarC++17_10-A16.0.1: MD_OSA_A16.0.1_recvmmsg_configuration
#ifndef AMSR_RECVMMSG_MAXIMUM
/*!
 * \brief           Maximum number of datagrams that can be received with one ReceiveSyncBulk() call.
 */
static constexpr std::uint32_t const kMaxDatagramsBulkRcv{64};
#else
/*!
 * \brief           Maximum number of datagrams that can be received with one ReceiveSyncBulk() call.
 */
static constexpr std::uint32_t const kMaxDatagramsBulkRcv{AMSR_RECVMMSG_MAXIMUM};

static_assert((kMaxDatagramsBulkRcv >= 1) && (kMaxDatagramsBulkRcv <= IOV_MAX),
              "AMSR_RECVMMSG_MAXIMUM outside of possible limit. Valid range is [1; IOV_MAX].");
#endif

// VECTOR Next Line AutosarC++17_10-A16.0.1: MD_OSA_A16.0.1_recvmmsg_configuration
#ifndef AMSR_RECVMMSG_MINIMUM
/*!
 * \brief           Minimum number of datagrams that can be received with one ReceiveSyncBulk() call.
 */
static constexpr std::uint32_t const kMinDatagramsBulkRcv{0};
#else
/*!
 * \brief           Minimum number of datagrams that can be received with one ReceiveSyncBulk() call.
 */
static constexpr std::uint32_t const kMinDatagramsBulkRcv{AMSR_RECVMMSG_MINIMUM};

static_assert((kMinDatagramsBulkRcv >= 0) && (kMinDatagramsBulkRcv <= (kMaxDatagramsBulkRcv + 1)),
              "AMSR_RECVMMSG_MINIMUM outside of possible limit. Valid range is [0; kMaxDatagramsBulkRcv+1].");
#endif

/*!
 * \brief           Datagram length to indicate a truncated datagram.
 */
static constexpr std::uint32_t const kTruncatedDatagram{std::numeric_limits<std::uint32_t>::max()};

// VECTOR Next Line AutosarC++17_10-A16.0.1: MD_OSA_A16.0.1_sendmmsg_configuration
#ifndef AMSR_SENDMMSG_MAXIMUM
/*!
 * \brief           Maximum number of datagrams that can be sent with one SendSyncBulk() call.
 */
static constexpr std::uint32_t const kMaxDatagramsBulkSnd{IOV_MAX};
#else
/*!
 * \brief           Maximum number of datagrams that can be sent with one SendSyncBulk() call.
 */
static constexpr std::uint32_t const kMaxDatagramsBulkSnd{AMSR_SENDMMSG_MAXIMUM};

static_assert((kMaxDatagramsBulkSnd >= 1) && (kMaxDatagramsBulkSnd <= IOV_MAX),
              "AMSR_SENDMMSG_MAXIMUM outside of possible limit. Valid range is [1; IOV_MAX].");
#endif

// VECTOR Next Line AutosarC++17_10-A16.0.1: MD_OSA_A16.0.1_sendmmsg_configuration
#ifndef AMSR_SENDMMSG_MINIMUM
/*!
 * \brief           Minimum number of datagrams that can be sent with one SendSyncBulk() call.
 */
static constexpr std::uint32_t const kMinDatagramsBulkSnd{0};
#else
/*!
 * \brief           Minimum number of datagrams that can be sent with one SendSyncBulk() call.
 */
static constexpr std::uint32_t const kMinDatagramsBulkSnd{AMSR_SENDMMSG_MINIMUM};

static_assert((kMinDatagramsBulkSnd >= 0) && (kMinDatagramsBulkSnd <= (kMaxDatagramsBulkSnd + 1)),
              "AMSR_SENDMMSG_MINIMUM outside of possible limit. Valid range is [0; kMaxDatagramsBulkSnd+1].");
#endif

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_OS_SPECIFIC_CONSTANTS_H_

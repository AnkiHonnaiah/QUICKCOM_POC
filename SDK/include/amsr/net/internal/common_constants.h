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
 *        \brief  Constants for working with the OS common to all OSes.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_COMMON_CONSTANTS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_COMMON_CONSTANTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include "amsr/net/internal/os_constants.h"
#include "amsr/net/internal/os_specific_constants.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief           Maximal length of a string that contains an Ethernet address.
 *
 * \details         The address is represented by hexadecimal characters (which means two characters per byte) and one
 *                  separation character is between each two bytes of the address (which means one character less than
 *                  there are bytes in the address), there is no terminating \0 character.
 */
static constexpr std::uint8_t const kMaxEtherAddressString{(2 * kEthernetAddressLength) + (kEthernetAddressLength - 1)};

/*!
 * \brief           The length of an Ethernet header in bytes.
 */
constexpr std::size_t kEtherHeaderLength{ETHER_HDR_LEN};

/*!
 * \brief           Maximal length of the payload for one Ethernet frame in bytes (Maximum Transmission Unit).
 */
constexpr std::size_t kMaxEtherPayload{ETHERMTU};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief           Offset (in bytes) the EtherType field has from the beginning of the Ethernet header.
 */
constexpr std::size_t kEthHeaderEtherTypeOffset{ETHER_ADDR_LEN * 2};

/*!
 * \brief           The minimum header size of an IPv4 packet (using no options) in bytes.
 * \details         The IPv4 header size is not fixed. It has a minimum length of 20 bytes, but a maximum length of up
 *                  to 60 bytes when IP options are included. Most packets have a header length of 20 bytes.
 */
constexpr std::size_t kIPv4HeaderLengthMin{sizeof(struct ip)};

/*!
 * \brief           Number of bytes one word/step in the IPv4 header length field represents.
 * \details         The IPv4 header length field contains the number of 4-byte-words the header consists of.
 */
constexpr std::size_t kIpV4HeaderSizeByteWord{4};

/*!
 * \brief           The maximum header size of an IPv4 packet in bytes (minimum header size + options).
 * \details         The size of the IPv4 header is not fixed. It has a minimum length of 20 bytes, but a maximum length
 *                  of up to 60 bytes when IP options are included. Most packets have a header length of 20 bytes.
 */
constexpr std::size_t kIPv4HeaderLengthMax{60};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief           Offset (in bytes) the IPv4 version and header length field has from the beginning of the IP header.
 */
constexpr std::size_t kIpV4HeaderVersionHeaderLengthOffset{0};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief           Offset (in bytes) the IPv4 flags and fragment field has from the beginning of the IP header.
 */
constexpr std::size_t kIpV4HeaderFlagFragmentOffset{6};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief           Offset (in bytes) the IPv4 protocol field has from the beginning of the IP header.
 */
constexpr std::size_t kIpV4HeaderProtocolOffset{9};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief           Offset (in bytes) the IPv4 destination address field has from the beginning of the IP header.
 */
constexpr std::size_t kIpV4HeaderDestAddrOffset{16};

/*!
 * \brief           The header size of an IPv6 packet in bytes.
 * \details         In addition to the fixed IPv6 header, none, one or multiple extension headers may follow, providing
 *                  additional protocol-specific information. Extension headers are optional and can include headers
 *                  such as a hop-by-hop options header, a routing header, a fragmentation header, an authentication
 *                  header, or an encapsulating security payload header. These extension headers can increase the
 *                  overall size of the IPv6 header beyond the minimum of 40 bytes.
 */
constexpr std::size_t kIPv6HeaderLengthMin{40};

/*!
 * \brief           The minimum length of a TCP header in bytes.
 * \details         The minimum length of a TCP header is 20 bytes, which includes NO options. The
 *                  actual length of a TCP header depends on the presence of TCP options. The maximum length of a TCP
 *                  header is 60 bytes.
 */
constexpr std::size_t kTcpHeaderLengthMin{sizeof(struct tcphdr)};

/*!
 * \brief           The maximum length of a TCP header in bytes.
 * \details         The maximum length of a TCP header is 60 bytes, which includes options. The actual length of a TCP
 *                  header depends on the presence of TCP options. The minimum length of a TCP header is 20 bytes, but
 *                  it can be up to 60 bytes with options present.
 */
constexpr std::size_t kTcpHeaderLengthMax{60};

/*!
 * \brief           The header size of a UDP header in bytes.
 */
constexpr std::size_t kUdpHeaderLength{sizeof(struct udphdr)};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief           Offset (in bytes) the UDP destination port has from the beginning of the UDP header.
 */
constexpr std::size_t kUdpHeaderDestPortOffset{2};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief IPV4 broadcast address (255.255.255.255)
 */
constexpr std::uint32_t kIpV4BroadcastAddress{0xffffffff};

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_COMMON_CONSTANTS_H_

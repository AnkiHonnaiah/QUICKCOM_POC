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
 *        \brief  Utilities to work with networks.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_NETWORK_PARSER_UTILS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_NETWORK_PARSER_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/net/internal/types.h"
#include "amsr/net/ip/address.h"

#include "amsr/core/span.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace net {
namespace ip {
namespace internal {

/*!
 * \brief           Utilities to extract/provide informations related to Network objects.
 */
class NetworkParserUtils {
 public:
  /*!
   * \brief           Outputs the part of the string containing the IP address.
   *
   * \details         The string being output is not guaranteed to be a valid representation of an IP address.
   *
   * \param[in]       network_string       A string that holds the network representation according to CIDR notation.
   *
   * \return          The part of the input string that holds the IP address, if the separation character '/' has been
   *                  found. This has not been copied to a new string but is a view of the same underlying data
   *                  structure the input is a view of.
   *
   * \error           OsabErrc::kApiError      The string does not contain the '/' separation character.
   *
   * \pre             -
   *
   * \context         ANY
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<::amsr::core::StringView> GetAddress(::amsr::core::StringView network_string);

  /*!
   * \brief           Outputs the part of the string containing the network prefix length.
   *
   * \details         The string being output is not guaranteed to be a valid prefix length.
   *
   * \param[in]       network_string        A string that holds the network representation according to CIDR notation.
   *
   * \return          The part of the input string that holds the network prefix length if operation succeeded. This has
   *                  not been copied to a new string but is a view of the same underlying data structure the input is a
   *                  view of.
   *
   * \error           OsabErrc::kApiError             The string does not contain a prefix length.
   *
   * \pre             -
   *
   * \context         ANY
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<::amsr::core::StringView> GetPrefixLength(::amsr::core::StringView network_string);

  /*!
   * \brief           Checks if given prefix length value matches the given address family requirements.
   *
   * \details         IPv4 allowed range: [0...32], IPv6 allowed range: [0...128].
   *
   * \param[in]       prefix_length     The prefix length value.
   * \param[in]       address           The Address object used to validate the prefix length.
   *
   * \return          true if the check is successfull, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static bool ValidatePrefixLengthToAddressMapping(std::uint32_t prefix_length, Address const& address);

  /*!
   * \brief           Parses the string containing the prefix length and validate it.
   *
   * \param[in]       prefix_length_view    A string that contains the prefix length.
   * \param[in]       address               Address used to check validity of the prefix length.
   *
   * \return          The result containing the prefix length if parsing succeeded, otherwise an error.
   *
   * \error           OsabErrc::kApiError            The string does not hold a decimal integer, or
   *                                                 the integer is out of the ranges defined by the address
   *                                                 family.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<net::internal::PrefixLength> ParsePrefixLength(
      ::amsr::core::StringView prefix_length_view, Address const& address);

  /*!
   * \brief           Converts the prefix length integer into an array of bytes representing the network mask.
   *
   * \details         No checks about length validity are performed.
   *                  The algorithm works as following:
   *                  1. length / 8 returnes the number of (most significant) bytes that must be set to 0xFF.
   *                  2. length % 8 returns the number of missing leading bits to be set to 1 in the next byte to be
   *                     updated. The next byte to be updated is of course the most significant still zero byte.
   *                  Example:             input       prefix=113
   *                                       Step1:      113/8=14 ->  FFFF FFFF FFFF FFFF FFFF FFFF FFFF xx00
   *                                       Step2:      113%8=1  ->  1 bit set to 1 is missing  -> xx = 10000000b (0x80)
   *                                       output:     FFFF FFFF FFFF FFFF FFFF FFFF FFFF 8000
   *
   * \param[in]       length                           IPv6 prefix length integer in range [0...128].
   * \param[out]      netmask_bytes                    span to the array of bytes containing the network mask.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static void FromPrefixLengthToNetworkMaskBytes(net::internal::PrefixLength length,
                                                 ::amsr::core::Span<std::uint8_t> netmask_bytes) noexcept;

  /*!
   * \brief           Converts the network mask array of bytes into a prefix length integer.
   *
   * \details         Syntax checks on the passed parameter are performed. A network mask can only contain
   *                  leading and contiguous bits set to one or none of them.
   *
   * \param[in]       netmask_bytes                    span to the array of bytes containing the network mask.
   *
   * \return          The result containing the prefix length if network mask' syntax is correct, otherwise an error.
   *
   * \error           OsabErrc::kApiError            The network mask is incorrect. Bits set to one that are not leading
   *                                                 and contiguous have been found.
   *
   * \context         ANY
   *
   * \pre             The provided netmask_bytes size is 4(IPv4) or 16(IPv6).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<net::internal::PrefixLength> FromNetworkMaskToPrefixLength(
      ::amsr::core::Span<std::uint8_t> netmask_bytes) noexcept;

  /*!
   * \brief           Checks if two IPv4 addresses have the same network part.
   *
   * \param[in]       left      First Address object to compare.
   * \param[in]       right     Second Address object to compare.
   * \param[in]       length    Prefix length of the network to be applied, with values [0,32].
   *
   * \return          true if the two addresses have the same network part, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static bool CompareIpv4AddressNetworkPart(AddressV4 const& left, AddressV4 const& right,
                                            net::internal::PrefixLength length) noexcept;

  /*!
   * \brief           Checks if two IPv6 addresses have the same network part.
   *
   * \param[in]       left      First Address object to compare.
   * \param[in]       right     Second Address object to compare.
   * \param[in]       length    Prefix length of the network to be applied.
   *
   * \return          true if the two addresses have the same network part, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static bool CompareIpv6AddressNetworkPart(AddressV6 const& left, AddressV6 const& right,
                                            net::internal::PrefixLength length) noexcept;
};

}  // namespace internal
}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_NETWORK_PARSER_UTILS_H_

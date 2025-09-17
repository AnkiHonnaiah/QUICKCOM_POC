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
 *        \brief  Class to represent IP networks.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_NETWORK_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_NETWORK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"

#include "amsr/net/internal/types.h"

#include "osabstraction/osab_error_domain.h"

#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"

#include "amsr/core/abort.h"
#include "amsr/core/span.h"

namespace amsr {
namespace net {
namespace ip {
namespace internal {
/*!
 * \brief           Maximal length of a string that contains an IPv6 network prefix length.
 *
 * \details         Allowed values range is 0 to 128.
 */
static constexpr std::uint8_t const kMaxIpv6PrefixLengthStringSize{3};

}  // namespace internal

/*!
 * \brief           Maximal length of a string that contains an IPv6 network.
 * \details         According to CIDR notation: <IPv6 Address>/<Prefix length>.
 */
static constexpr std::uint8_t const kMaxIpv6NetworkStringSize{ip::internal::kMaxIpv6AddressStringSize + 1 +
                                                              ip::internal::kMaxIpv6PrefixLengthStringSize};

/*!
 * \brief           Forward internal PrefixLength definition to the user.
 */
using PrefixLength = net::internal::PrefixLength;

/*!
 * \brief           Represents either an IPv4 or an IPv6 network
 * \details         The information which version of the IP protocol is used, is already contained in the Address member
 *                  object.
 */
class Network final {
 public:
  /*!
   * \brief           Creates a Network from its string representation.
   * \details         An IPv4 network is represented by a string of the following format:
   *                  "d.d.d.d/p" where each d is a decimal integer number in the range [0, 255] and p is a decimal
   *                  integer in the range [0...32]
   *
   *                  An IPv6 network is represented by a string of the following format:
   *                  "x:x:x:x:x:x:x:x/p" where each x represents up to four hexadecimal digits and p represents a
   *                  decimal integer in the range [0...128].
   *                  One series of contiguous zero values in the address may be replaced by ::
   *                  An IPv6 network may also be represented in the format "x:x:x:x:x:x:d.d.d.d/p" where x, d
   *                  and p have the same meaning as above.
   *                  Any other string will be considered as invalid.
   * \param[in]       network        The string representation of the network.
   * \return          The created object.
   * \error           OsabErrc::kApiError        The string does not follow the above specified format.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-NetworkCreation
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<Network> MakeNetwork(::amsr::core::StringView network) noexcept;

  /*!
   * \brief           Creates a Network from an Address object and prefix length.
   * \param[in]       address                   The Address object.
   * \param[in]       prefix_length             The prefix length of the network.
   *                                            Valid IPv4 range: [0...32], valid IPv6 range: [0...128].
   * \return          The created object.
   * \error           OsabErrc::kApiError       prefix_length does not match address family.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-NetworkCreation
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<Network> MakeNetwork(Address const& address, PrefixLength prefix_length) noexcept;

  /*!
   * \brief           Creates a Network from the host Address object and network mask Address object.
   * \param[in]       address                   The Address object representing the network address.
   * \param[in]       network_mask              The Address object representing the network mask.
   * \return          The created object if no error occurred.
   * \error           OsabErrc::kApiError       network_mask syntax error detected: not leading and contiguous
   *                                            bits found.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-NetworkCreation
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<Network> MakeNetwork(Address const& address, Address const& network_mask) noexcept;

  /*!
   * \brief           Returns the address object used during creation.
   * \return          address member variable.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   */
  Address GetAddress() const noexcept { return this->address_; }

  /*!
   * \brief           Returns the PrefixLength object used during creation.
   * \return          prefix_length member variable.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   */
  PrefixLength GetPrefixLength() const noexcept { return this->prefix_length_; }

  /*!
   * \brief           Check if given address belongs to the current network.
   * \details         The passed address family has to match the network address family. Comparison
   *                  between different IP protocols will fail.
   * \param[in]       other_address              IP address to be checked.
   * \return          true if the address belongs to the current network address range, false otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-NetworkContainsHost
   * \vprivate        Vector product internal API
   */
  bool IsHostOfThisNetwork(Address const& other_address) const noexcept;

  /*!
   * \brief           Outputs a string representation of the network.
   * \param[out]      buffer                    Buffer to write the \0-terminated network string into.
   * \return          Number of characters the network string consists of (terminating \0 character not counted).
   * \context         ANY
   * \pre             buffer must be able to hold at least kMaxIpv6NetworkStringSize + 1 characters.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Span<char>::size_type ToString(::amsr::core::Span<char> buffer) const noexcept;

  /*!
   * \brief           Outputs a string representation of the network.
   * \return          The string representation of the network.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \vprivate        Vector product internal API
   */
  ::amsr::core::String ToString() const noexcept;

  /*!
   * \brief           Network default constructor.
   * \details         The default address is an unspecified IPv4 address with prefix length 0.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   */
  Network() noexcept : Network(Address{}, PrefixLength{0}) {}

 private:
  /*!
   * \brief           Network constructor that accepts an address and a prefix length.
   * \param[in]       address        The IPv4/IPv6 address of the network.
   * \param[in]       prefix_length  The network prefix length.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate        Vector component internal API
   */
  explicit Network(Address address, PrefixLength prefix_length) noexcept
      : address_{address}, prefix_length_{prefix_length} {}

  /*!
   * \brief           Outputs the maximum prefix length string size.
   * \details         The value depends on the address family of this network.
   * \return          The maximum string size representing the prefix length of this network.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate        Vector product internal API
   */
  std::uint8_t GetMaxPrefixLengthStringSize() const noexcept;

  /*!
   * \brief           Member address.
   */
  Address address_{};

  /*!
   * \brief           Member prefix length.
   * \details         Allowed values range is 0 to 128.
   */
  PrefixLength prefix_length_{};
};

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_NETWORK_H_

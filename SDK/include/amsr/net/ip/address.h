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
 *        \brief  Class to represent IP addresses.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address_v4.h"
#include "amsr/net/ip/address_v6.h"

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

/*!
 * \brief           Pull port into a namespace visible to the user.
 *
 * \details         This is done here because Port is expected to be used together with Address.
 */
using Port = net::internal::Port;

/*!
 * \brief           Represents either an IPv4 or an IPv6 address
 *
 * \details         Contains both an IPv4 and an IPv6 object and the information which one is actually represented.
 */
class Address final {
 public:
  /*!
   * \brief           Maximum number of characters the string representation of an IPv4 address may be long.
   *
   * \details         Buffers to hold that address may be required to fit one character more (the \0 character).
   */
  static constexpr std::uint8_t const kMaxIpv4String{AddressV4::kMaxString};

  /*!
   * \brief           Maximum number of characters the string representation of an IPv6 address may be long.
   *
   * \details         Buffers to hold that address may be required to fit one character more (the \0 character).
   */
  static constexpr std::uint8_t const kMaxIpv6String{AddressV6::kMaxString};

  /*!
   * \brief           Creates an Address from its string representation.
   *
   * \details         An IPv4 address is represented by a string of the following format:
   *                  "d.d.d.d" where each d is a decimal integer number in the range [0, 255]
   *
   *                  An IPv6 address is represented by a string of the following format:
   *                  "x:x:x:x:x:x:x:x" where each x represents up to four hexadecimal digits.
   *                  One series of contiguous zero values in the address may be replaced by ::
   *                  An IPv6 address may also be represented in the format "x:x:x:x:x:x:d.d.d.d" where x and d have the
   *                  same meaning as above.
   *
   *                  A valid string as of above criteria may be followed by a '%' character that is followed by an
   *                  identification of a network interface. This has no effect upon the created address.
   *
   *                  Note that these constraints do not allow IPv4 addresses that contain a port number (valid IPv4
   *                  address followed by a ':' character followed by decimal digits).
   *
   * \param[in]       address                   The string representation of the address.
   *
   * \return          The created object.
   *
   * \error           OsabErrc::kApiError                               The string does not represent an address as of
   *                                                                    above criteria.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<Address> MakeAddress(::amsr::core::StringView address) noexcept;

  /*!
   * \brief           Extracts the information if this object represents an IPv4 address.
   *
   * \return          True if the object represents an IPv4 address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  constexpr bool IsV4() const noexcept { return represented_address_ == AddressFamily::kV4; }

  /*!
   * \brief           Extracts the information if this object represents an IPv6 address.
   *
   * \return          True if the object represents an IPv6 address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  constexpr bool IsV6() const noexcept { return represented_address_ == AddressFamily::kV6; }

  /*!
   * \brief           Returns the IPv4 address this object represents.
   *
   * \return          The IPv4 address.
   *
   * \context         ANY
   *
   * \pre             The object represents an IPv4 address (IsV4() == true).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  AddressV4 GetV4Unsafe() const noexcept {
    if (!IsV4()) {
      ::amsr::core::Abort("Tried to extract IPv4 address from Address object that does not contain an IPv4 address.");
    }
    return address_v4_;
  }

  /*!
   * \brief           Returns the IPv6 address this object represents.
   *
   * \return          The IPv6 address.
   *
   * \context         ANY
   *
   * \pre             The object represents an IPv6 address (IsV6() == true).
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  AddressV6 GetV6Unsafe() const noexcept {
    if (!IsV6()) {
      ::amsr::core::Abort("Tried to extract IPv6 address from Address object that does not contain an IPv6 address.");
    }
    return address_v6_;
  }

  /*!
   * \brief           Extracts the information if the contained address is the unspecified address or not.
   *
   * \return          True if the contained address is the unspecified address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool IsUnspecified() const noexcept;

  /*!
   * \brief           Extracts the information if the contained address is the loopback address or not.
   *
   * \return          True if the contained address is the loopback address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool IsLoopback() const noexcept;

  /*!
   * \brief           Extracts the information if the contained address is a multicast address or not.
   *
   * \return          True if the contained address is a multicast address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool IsMulticast() const noexcept;

  /*!
   * \brief           Extracts the information if the contained address is a limited broadcast address or not.
   *
   * \return          True if the contained address is a limited broadcast address, else false. Always false for IPv6.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool IsLimitedBroadcast() const noexcept;

  /*!
   * \brief           Compares this address to another one for equality.
   *
   * \details         Addresses of different families are always considered inequal.
   *
   * \param[in]       other                     The address to compare with.
   *
   * \return          True if the addresses are equal, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool operator==(Address const& other) const noexcept;

  /*!
   * \brief           Compares this address to another one for inequality.
   *
   * \details         Addresses of different families are always considered inequal.
   *
   * \param[in]       other                     The address to compare with.
   *
   * \return          False if the addresses are equal, else true.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool operator!=(Address const& other) const noexcept;

  /*!
   * \brief           Outputs a string representation of the address.
   *
   * \param[out]      buffer                    Buffer to write the \0-terminated address string into.
   *
   * \return          Number of characters the address string consists of (terminating \0 character not counted).
   *
   * \context         ANY
   *
   * \pre             buffer must be able to hold at least kMaxIpv4String + 1 characters if IsV4() == true and at least
   *                  kMaxIpv6String + 1 characters if IsV6() == true.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Span<char>::size_type ToString(::amsr::core::Span<char> buffer) const noexcept;

  /*!
   * \brief           Outputs a string representation of the address.
   *
   * \details         Allocates memory to hold the string representation of the address. Memory is only allocated in
   *                  case the string representation of the address can be acquired.
   *
   * \return          The string representation of the address.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::String ToString() const noexcept;

  /*!
   * \brief           A buffer to hold a string that represents an IP address.
   */
  using StringBuffer = amsr::core::Array<char, std::max(kMaxIpv4String, kMaxIpv6String) + 1>;

  /*!
   * \brief           Outputs a string representation of the address.
   *
   * \details         The string is contained in an owing buffer.
   *
   * \return          The \0-terminated string representation of the address.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  StringBuffer MakeString() const noexcept;

  /*!
   * \brief           Creates a default address.
   *
   * \details         The default address is an unspecified IPv4 address.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector product internal API
   */
  Address() noexcept : Address{AddressV4{}, AddressV6{}, AddressFamily::kV4} {}

  /*!
   * \brief           Creates an address that represents an IPv4 address.
   *
   * \param[in]       address                   The IPv4 address this object shall represent.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector component internal API
   */
  explicit Address(AddressV4 address) noexcept : Address{address, AddressV6{}, AddressFamily::kV4} {}

  /*!
   * \brief           Creates an address that represents an IPv6 address.
   *
   * \param[in]       address                   The IPv6 address this object shall represent.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector component internal API
   */
  explicit Address(AddressV6 address) noexcept : Address{AddressV4{}, address, AddressFamily::kV6} {}

 private:
  /*!
   * \brief           Type that stores which kind of address an object of this class represents.
   */
  enum class AddressFamily : std::uint8_t {
    /*!
     * \brief         The object represents an IPv4 address.
     */
    kV4,

    /*!
     * \brief         The object represents an IPv6 address.
     */
    kV6
  };

  /*!
   * \brief           Creates an address that represents either an IPv4 or an IPv6 address.
   *
   * \details         Required to avoid violation of a Bauhaus rule that constructor delegation shall be used.
   *
   * \param[in]       address_v4                The IPv4 address that shall be contained in the created object.
   * \param[in]       address_v6                The IPv6 address that shall be contained in the created object.
   * \param[in]       address_family            The address family of the address represented by the created object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   *
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector component internal API
   */
  constexpr Address(AddressV4 address_v4, AddressV6 address_v6, AddressFamily address_family) noexcept
      : represented_address_{address_family}, address_v4_{address_v4}, address_v6_{address_v6} {}

  /*!
   * \brief           Information which kind of address the object represents.
   */
  AddressFamily represented_address_;

  /*!
   * \brief           Contained address if the object represents an IPv6 address.
   */
  AddressV4 address_v4_{};

  /*!
   * \brief           Contained address if the object represents an IPv6 address.
   */
  AddressV6 address_v6_{};
};

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_H_

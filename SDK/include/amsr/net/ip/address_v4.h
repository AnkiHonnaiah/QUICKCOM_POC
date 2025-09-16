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
 *        \brief  Class to represent IPv4 addresses.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_V4_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_V4_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/net/internal/os_constants.h"
#include "amsr/net/internal/types.h"

#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

namespace amsr {
namespace net {
namespace ip {

/*!
 * \brief           Represents an IPv4 address.
 */
class AddressV4 final {
 public:
  /*!
   * \brief           Native representation of an IPv4 address in network byte order.
   */
  using Native = net::internal::Ipv4Native;

  /*!
   * \brief           Maximum number of characters the string representation of an IPv4 address may be long.
   *
   * \details         Buffers to hold that address may be required to fit one character more (the \0 character).
   */
  static constexpr std::uint8_t const kMaxString{ip::internal::kMaxIpv4AddressStringSize};

  /*!
   * \brief           Number of bytes an IPv4 address consists of.
   */
  static constexpr std::size_t kAddressBytes{4};

  /*!
   * \brief           Creates an unspecified address.
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
  /*!
   * \internal
   * - Initialize the object to the unspecified address.
   * \endinternal
   */
  constexpr AddressV4() noexcept : AddressV4(Native{0x00000000}) {}

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_AddressV4DelegateConstructor
  /*!
   * \brief           Creates an address.
   *
   * \param[in]       address                   The native representation of the address this object shall represent.
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
  /*!
   * \internal
   * - Initialize the object from the input.
   * \endinternal
   */
  constexpr explicit AddressV4(Native address) noexcept : address_{address} {}

  /*!
   * \brief           Creates an address.
   *
   * \param[in]       address                   The address this object shall represent. Each element of the array
   *                                            should hold the number that corresponds to the respective number in a
   *                                            string that represents the number (so the string would be like this:
   *                                            <address[0]>.<address[1]>.<address[2]>.<address[3]>)
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
  explicit AddressV4(::amsr::core::Array<std::uint8_t, AddressV4::kAddressBytes> address) noexcept;

  /*!
   * \brief           Extracts the native representation of the address.
   *
   * \return          The native representation of the address.
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
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Output the native representation of the address.
   * \endinternal
   */
  constexpr Native GetNative() const noexcept { return address_; }

  /*!
   * \brief           Extracts the binary representation of the address.
   *
   * \return          The binary representation of the address, element [n] corresponds to the n-th (with
   *                  n in [0; kAddressBytes - 1]) number in the string the object was created from.
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
  ::amsr::core::Array<std::uint8_t, kAddressBytes> GetBinary() const noexcept;

  /*!
   * \brief           Extracts the information if the address is the unspecified address or not.
   *
   * \return          True if the address is the unspecified address, else false.
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
   * \brief           Extracts the information if the address is the loopback address or not.
   *
   * \return          True if the address is the loopback address, else false.
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
   * \brief           Extracts the information if the address is a multicast address or not.
   *
   * \return          True if the address is a multicast address, else false.
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
   * \brief           Extracts the information if the address is a limited broadcast address or not.
   *
   * \return          True if the address is a limited broadcast address, else false.
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
   * \brief           Compares this IPv4 address to another one for equality.
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
  /*!
   * \internal
   * - Compare all bytes of both operands for equality.
   * \endinternal
   */
  constexpr bool operator==(AddressV4 const& other) const noexcept { return address_.s_addr == other.address_.s_addr; }

  /*!
   * \brief           Compares this IPv4 address to another one for inequality.
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
  /*!
   * \internal
   * - Compare all bytes of both operands for at least one byte being different at the operands.
   * \endinternal
   */
  constexpr bool operator!=(AddressV4 const& other) const noexcept { return address_.s_addr != other.address_.s_addr; }

  /*!
   * \brief           Outputs a string representation of the address.
   *
   * \param[out]      buffer                    Buffer to write the \0-terminated address string into.
   *
   * \return          Number of characters the address string consists of (terminating \0 character not counted).
   *
   * \context         ANY
   *
   * \pre             buffer must be able to hold at least kMaxString + 1 characters.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Span<char>::size_type ToString(::amsr::core::Span<char> buffer) const noexcept;

 private:
  /*!
   * \brief           The address.
   *
   * \details         The bytes the address consists of are contained in network byte order in this object.
   */
  Native address_;
};

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_V4_H_

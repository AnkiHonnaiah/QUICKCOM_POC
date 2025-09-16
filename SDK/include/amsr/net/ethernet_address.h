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
 *        \brief  Class to represent Ethernet addresses.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_ETHERNET_ADDRESS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_ETHERNET_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/internal/types.h"

#include "osabstraction/osab_error_domain.h"

#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace net {

/*!
 * \brief           Represents an Ethernet address
 */
class EthernetAddress final {
 public:
  /*!
   * \brief           Native representation of an ethernet address in network byte order.
   */
  using Native = internal::EthNative;

  /*!
   * \brief           Number of bytes an Ethernet address consists of.
   */
  static constexpr std::size_t kAddressBytes{net::internal::kEthernetAddressLength};

  /*!
   * \brief           Maximum number of characters the string representation of an ethernet address may be long.
   *
   * \details         Buffers to hold that address may be required to fit one character more (the \0 character).
   */
  static constexpr std::uint8_t const kMaxEtherString{internal::kMaxEtherAddressString};

  /*!
   * \brief           Creates an Ethernet address from its string representation.
   *
   * \details         An Ethernet address is represented by a string of the following format:
   *                  h:h:h:h:h:h where each h is a hexadecimal integer number in the range [0, FF]
   *
   *
   * \param[in]       address                   The string representation of the address.
   *
   * \return          The created object.
   *
   * \error           OsabErrc::kApiError       The string does not represent an address as of above criteria.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<EthernetAddress> MakeAddress(::amsr::core::StringView address) noexcept;

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
   * \threadsafe      TRUE except operator=
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
   * \threadsafe      TRUE except operator=
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  /*!
   * \internal
   * - Output the binary representation of the address.
   * \endinternal
   */
  ::amsr::core::Array<std::uint8_t, kAddressBytes> GetBinary() const noexcept { return address_.data; }

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
   * \threadsafe      TRUE except operator=
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool IsMulticast() const noexcept;

  /*!
   * \brief           Extracts the information if the contained address is a broadcast address or not.
   *
   * \return          True if the contained address is a broadcast address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE except operator=
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool IsBroadcast() const noexcept;

  /*!
   * \brief           Compares this address to another one for equality.
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
   * \threadsafe      TRUE except operator= and if the input is thread-safe
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool operator==(EthernetAddress const& other) const noexcept;

  /*!
   * \brief           Compares this address to another one for inequality.
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
   * \threadsafe      TRUE except operator= and if the input is thread-safe
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  bool operator!=(EthernetAddress const& other) const noexcept;

  /*!
   * \brief           Outputs a string representation of the address.
   *
   * \details         The address is always kMaxEtherString characters long.
   *
   * \param[out]      buffer                    Buffer to write the string to.
   *
   * \context         ANY
   *
   * \pre             buffer must be able to hold at least kMaxEtherString characters
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE except operator=
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   */
  void ToString(::amsr::core::Span<char> buffer) const noexcept;

  /*!
   * \brief           Outputs a string representation of the address.
   *
   * \details         Allocates memory to hold the string representation of the address.
   *
   * \return          The string representation of the address.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE except operator=
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::String ToString() const noexcept;

  /*!
   * \brief           Creates an instance.
   *
   * \param[in]       address                   The Ethernet address this object shall represent.
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
  constexpr explicit EthernetAddress(Native address) noexcept : address_{address} {}

  /*!
   * \brief           Creates an instance.
   *
   * \param[in]       address                   The Ethernet address this object shall represent. Each element of the
   *                                            array should hold the number that corresponds to the respective number
   *                                            in a string that represents the number (so the string would be like
   *                                            this: <address[0]>:<address[1]>:<address[2]>:<address[3]>: ...)
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
  explicit EthernetAddress(::amsr::core::Array<std::uint8_t, EthernetAddress::kAddressBytes> address) noexcept;

  /*!
   * \brief           Creates a default address
   *
   * \details         The default address consists solely of 0x00 bytes.
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
  EthernetAddress() noexcept;

  /*!
   * \brief           Copy constructs an Ethernet address
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input parameter is thread-safe
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector product internal API
   */
  EthernetAddress(EthernetAddress const&) noexcept = default;

  /*!
   * \brief           Move constructs an Ethernet address
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input parameter is thread-safe
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector product internal API
   */
  EthernetAddress(EthernetAddress&&) noexcept = default;

  /*!
   * \brief           Assigns other's Ethernet address
   *
   * \details         This API overwrite object's internal data. So this API shouldn't be called
   *                  in parallel with getters of this class in order to preserve their thread safety.
   *
   * \return          This object.
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
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector product internal API
   */
  EthernetAddress& operator=(EthernetAddress const&) & noexcept = default;

  /*!
   * \brief           Moves other's Ethernet address
   *
   * \details         This API overwrite object's internal data. So this API shouldn't be called
   *                  in parallel with getters of this class in order to preserve their thread safety.
   *
   * \return          This object.
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
   * \trace           DSGN-Osab-AddressCreation
   *
   * \vprivate        Vector product internal API
   */
  EthernetAddress& operator=(EthernetAddress&&) & noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \steady          TRUE
   */
  ~EthernetAddress() noexcept = default;

 private:
  /*!
   * \brief           The address.
   */
  Native address_{};
};

}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_ETHERNET_ADDRESS_H_

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
 *        \brief  Class to represent an address entry returned by Interface::GetAllAddresses().
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_ADDRESS_ENTRY_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_ADDRESS_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ethernet_address.h"
#include "amsr/net/ip/address.h"

#include "osabstraction/osab_error_domain.h"

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/variant.h"

namespace amsr {
namespace net {

/*!
 * \brief Helper class to generate dummy types for function overloading.
 *
 * \tparam Type to generate a dummy type for.
 */
template <typename T>
struct OverloadType {};

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_InterfaceAddressEntry
/*!
 * \brief           Represents the information contained in the ifaddrs struct, which is an entry in the list
 *                  of interface addresses returned by getifaddrs().
 * \details         It contains only one of these possible sets of values:
 *                  1. < EthernetAddress > if HasEthernetAddress() returns true
 *                  2. < IPAddress, Netmask > if HasIPAddressAndNetmask() returns true
 *                  3. < IPAddress, Netmask, BroadcastAddress > if
 *                     HasIPAddressAndNetmask() and HasBroadcastAddress() return true (only IPv4).
 *                  Each value can be retrieved, if the preconditions are met, by the corresponding getter.
 *
 */
class InterfaceAddressEntry final {
 public:
  /*!
   * \brief           Returns the information if the assigned address is an ethernet address or not.
   *
   * \return          True if the assigned address is an ethernet address, else false.
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
  bool HasEthernetAddress() const noexcept;

  /*!
   * \brief           Returns the ethernet address representation.
   *
   * \return          The ethernet address representation.
   *
   * \context         ANY
   *
   * \pre             HasEthernetAddress() returned true.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */

  EthernetAddress GetEthernetAddress() const noexcept;

  /*!
   * \brief           Returns the information if there are IP address and netmask assigned or not.
   *
   * \return          True if there are IP address and mask assigned, else false.
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
  bool HasIPAddressAndNetmask() const noexcept;

  /*!
   * \brief           Returns the assigned IP address representation.
   *
   * \return          The assigned IP address representation.
   *
   * \context         ANY
   *
   * \pre             HasIPAddressAndNetmask() returned true.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */

  ip::Address GetIPAddress() const noexcept;

  /*!
   * \brief           Returns the netmask associated to the assigned IP address.
   *
   * \return          The address representation of the netmask.
   *
   * \context         ANY
   *
   * \pre             HasIPAddressAndNetmask() returned true.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */

  ip::Address GetNetmask() const noexcept;

  /*!
   * \brief           Returns the information if there is a broadcast address assigned (IPv4 only).
   *
   * \return          True if there is a broadcast address assigned, else false.
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

  bool HasBroadcastAddress() const noexcept;

  /*!
   * \brief           Returns the assigned broadcast address.
   *
   * \return          The broadcast address representation.
   *
   * \context         ANY
   *
   * \pre             HasBroadcastAddress() returned true.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */

  ip::Address GetBroadcastAddress() const noexcept;

  /*!
   * \brief           Compares this entry to another one for equality.
   *
   * \param[in]       other                     The entry to compare with.
   *
   * \return          True if the entries are equal, else false.
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
  bool operator==(InterfaceAddressEntry const& other) const noexcept;

  /*!
   * \brief           Compares this entry to another one for inequality.
   *
   * \param[in]       other                     The entry to compare with.
   *
   * \return          False if the entries are equal, else true.
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
  bool operator!=(InterfaceAddressEntry const& other) const noexcept;

  /*!
   * \brief           Copy constructs an InterfaceAddressEntry object.
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
  InterfaceAddressEntry(InterfaceAddressEntry const&) = default;

  /*!
   * \brief           Move constructs an InterfaceAddressEntry object.
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
  InterfaceAddressEntry(InterfaceAddressEntry&&) noexcept = default;

  /*!
   * \brief           Assigns other's InterfaceAddressEntry object.
   *
   * \param[in]       other   InterfaceAddressEntry that shall be copied.
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
   * \vprivate        Vector product internal API
   */
  InterfaceAddressEntry& operator=(InterfaceAddressEntry const& other) & noexcept;

  /*!
   * \brief           Moves other's InterfaceAddressEntry object.
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
   * \vprivate        Vector product internal API
   */
  InterfaceAddressEntry& operator=(InterfaceAddressEntry&&) & noexcept = default;

  /*!
   * \brief Default destructor.
   */
  ~InterfaceAddressEntry() noexcept = default;

  /*!
   * \brief           Creates an instance containing ethernet address.
   *
   * \param[in]       address                   The Ethernet address this object shall contain.
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
  explicit InterfaceAddressEntry(EthernetAddress address) noexcept {
    address_.template emplace<EthernetAddress>(address);
  }

  /*!
   * \brief           Creates an instance containing IP Address and netmask.
   *
   * \details         It is assumed that if an IP address is assigned to the interface also the netmask is set, either
   *                  a default one (chosen by OS) or an user defined one.
   *
   * \param[in]       address                   The IP Address this object contains.
   * \param[in]       netmask                   The Address representation of the netmask.
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
   * \vprivate        Vector component internal API
   */
  explicit InterfaceAddressEntry(ip::Address address, ip::Address netmask) noexcept {
    address_.template emplace<ip::Address>(address);
    netmask_.emplace(netmask);
  }

  /*!
   * \brief           Creates an instance containing IP Address, netmask and broadcast address .
   *
   * \details         Only implemented for IPv4.
   *
   * \param[in]       address                   The IP Address this object contains.
   * \param[in]       netmask                   The Address representation of the netmask.
   * \param[in]       broadcast                 The Address representation of the broadcast address.
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
  explicit InterfaceAddressEntry(ip::Address address, ip::Address netmask, ip::Address broadcast) noexcept
      : InterfaceAddressEntry(address, netmask) {
    broadcast_address_.emplace(broadcast);
  }

  /*!
   * \brief           Creates an instance containing ifaddrs.
   *
   * \param[in]       addr                      The pointer to the OS interface entry to extract the native address from
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   *
   * \vprivate        Vector component internal API
   */
  explicit InterfaceAddressEntry(ifaddrs* addr) : ifaddr_(addr) {}

  /*!
   * \brief           Deleted default constructor.
   */
  InterfaceAddressEntry() noexcept = delete;

  /*!
   * \brief           Extracts the name of the network interface.
   *
   * \return          The name of the interface. Empty string if the object has not been created from an ifaddrs
   *                  instance.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   */
  ::amsr::core::StringView GetName() const noexcept;

  /*!
   * \brief           Checks if broadcast address is set.
   *
   * \return          true if broadcast address is set, false otherwise.
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  bool HasNativeBroadcastAddress() const noexcept;

  /*!
   * \brief           Extracts a native IPv4 broadcast address from a socket address view.
   *
   * \return          The broadcast native address
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true && HasNativeBroadcastAddress() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto ExtractNativeBroadcastAddress() const noexcept -> amsr::net::internal::Ipv4Native;

  /*!
   * \brief           Initializes the object from the ifaddrs instance it has been created from.
   *
   * \error           OsabErrc::kAddressError                     Address family is not supported.
   *
   * \context         ANY
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   */
  auto ProcessIfAddrsEntry() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Checks if an IP address is assigned to this interface.
   *
   * \param[in]       address                   The IP address to check if it is assigned to the interface.
   *
   * \return          True if the address is assigned to the interface, else false.
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   */
  bool AddressAssignedToInterface(ip::Address address) const noexcept;

  /*!
   * \brief           Outputs the next interface entry in the list of interfaces.
   *
   * \details         Output is only useful if HasAddress() == true.
   *
   * \return          Next interface entry if available or empty InterfaceAddressEntry.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  InterfaceAddressEntry GetNextAddress() const noexcept;

  /*!
   * \brief           Checks if the address is valid or not.
   *
   * \details         The address is considered valid if the object has been created from a non-nullptr instance of
   *                  ifaddrs.
   *
   * \return          True if the address is valid, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  bool HasAddress() const noexcept { return ifaddr_ != nullptr; }

  /*!
   * \brief           Searches an interface whose assigned IP address is known.
   *
   * \details         Only the first interface with matching IP address is found. The search is stopped as soon as an
   *                  interface with the searched address assigned to it is found.
   *
   * \param[in]       address                   The IP address to be searched.
   *
   * \return          The interface entry that has been found to have the supplied IP address assigned to it.
   *
   * \error           OsabErrc::kAddressNotAvailable              No interface with the supplied address assigned to it
   *                                                              has been found.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   */
  ::amsr::core::Result<InterfaceAddressEntry> SearchInterfaceFromAddress(ip::Address address) const noexcept;

 private:
  /*!
   * \brief           Checks if some type of address is assigned to a network interface.
   *
   * \details         The parameter is used only to select a type of address that shall be checked.
   *
   * \return          true if an address of the selected type is assigned to the interface.
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   */
  bool HasAddressType(OverloadType<ip::AddressV4>) const noexcept;

  /*!
   * \copydoc HasAddressType()
   */
  bool HasAddressType(OverloadType<ip::AddressV6>) const noexcept;

  /*!
   * \copydoc HasAddressType()
   */
  bool HasAddressType(OverloadType<EthernetAddress>) const noexcept;

  /*!
   * \brief           Extracts a native IPv4 address from a socket address view.
   *
   * \details         Contains an unused parameter to overload the return type for IPv4.
   *
   * \return          The native address
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true && HasAddressType(OverloadType<ip::AddressV4>) == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  amsr::net::internal::Ipv4Native ExtractNativeAddress(OverloadType<amsr::net::internal::Ipv4Native>) const noexcept;

  /*!
   * \brief           Extracts a native IPv6 address from a socket address view.
   *
   * \details         Contains an unused parameter to overload the return type for IPv6.
   *
   * \return          The native address
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true && HasAddressType(OverloadType<ip::AddressV6>) == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  amsr::net::internal::Ipv6Native ExtractNativeAddress(OverloadType<amsr::net::internal::Ipv6Native>) const noexcept;

  /*!
   * \brief           Extracts a native Ethernet address from a socket address view.
   *
   * \details         Contains an unused parameter to overload the return type for Ethernet.
   *
   * \return          The native address
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true && HasAddressType(OverloadType<EthernetAddress>) == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  amsr::net::internal::EthNative ExtractNativeAddress(OverloadType<amsr::net::internal::EthNative>) const noexcept;

  /*!
   * \brief           Extracts an address from a socket address view.
   *
   * \tparam          Address                   Selects if an IPv4 (ip::AddressV4), IPv6 (ip::AddressV6) or Ethernet
   *                                            (EthernetAddress) address shall be extracted.
   *
   * \return          The native address of the respective type.
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true && HasAddressType() of the respective type of address == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  template <typename Address>
  auto ExtractNativeAddress() noexcept -> typename Address::Native;

  /*!
   * \brief           Extracts a native IPv4 netmask from a socket address view.
   *
   * \details         Contains an unused parameter to overload the return type for IPv4.
   *
   * \return          The native netmask
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true && HasAddressType(OverloadType<ip::AddressV4>) == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  amsr::net::internal::Ipv4Native ExtractNativeNetmask(OverloadType<amsr::net::internal::Ipv4Native>) const noexcept;

  /*!
   * \brief           Extracts a native IPv6 netmask from a socket address view.
   *
   * \details         Contains an unused parameter to overload the return type for IPv6.
   *
   * \return          The native netmask
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true && HasAddressType(OverloadType<ip::AddressV6>) == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  amsr::net::internal::Ipv6Native ExtractNativeNetmask(OverloadType<amsr::net::internal::Ipv6Native>) const noexcept;

  /*!
   * \brief           Extracts a netmask from a socket address view.
   *
   * \tparam          Address                   Selects if an IPv4 (ip::AddressV4) or IPv6 (ip::AddressV6) netmask shall
   *                                            be extracted.
   *
   * \return          The native netmask of the respective type.
   *
   * \context         ANY
   *
   * \pre             HasAddress() == true && HasAddressType() of the respective type of address == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  template <typename Address>
  auto ExtractNativeNetmask() noexcept -> typename Address::Native;

  /*!
   * \brief           Pointer to the linked list of interface address structures.
   */
  ifaddrs* ifaddr_{nullptr};

  /*!
   * \brief           The assigned address.
   */
  ::amsr::core::Variant<::amsr::core::monostate, EthernetAddress, ip::Address> address_{};

  /*!
   * \brief           The netmask related to the assigned address.
   */
  ::amsr::core::Optional<amsr::net::ip::Address> netmask_{};

  /*!
   * \brief           The entry broadcast address.
   * \details         Only set in case of IPv4 entries.
   */
  ::amsr::core::Optional<amsr::net::ip::Address> broadcast_address_{};
};

}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_ADDRESS_ENTRY_H_

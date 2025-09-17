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
 *        \brief  OS-specific services to handle network interfaces.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_INTERFACE_SERVICES_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_INTERFACE_SERVICES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/address_v4.h"
#include "amsr/net/ip/address_v6.h"
#include "amsr/net_utils/network_stack_id.h"

#include "amsr/net/interface.h"

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "ara/core/vector.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief           Finds a network interface that has the supplied IP address assigned to it.
 *
 * \param[in]       address                   The IP address to be used.
 * \param[out]      buffer                    The span of characters where the interface name will be eventually
 *                                            written, including the terminating '\0' character.
 * \param[in]       netstack_id               ID of the network stack used to resolve the name to the address. Only used
 *                                            on QNX.
 *
 * \return          The number of characters that have been written to the span, if an interface was found.
 *
 * \error           OsabErrc::kUnexpected                               Some internal state that should not arise was
 *                                                                      detected.
 *
 * \error           OsabErrc::kSystemEnvironmentError                   The address is not assigned to an interface.
 *
 * \error           OsabErrc::kInsufficientPrivileges                   The process has insufficient privileges to
 *                                                                      look up the assigned addresses.
 *
 * \error           OsabErrc::kAddressError                             The address is assigned to multiple interfaces
 *
 * \error           OsabErrc::kResource                                 Not enough system resources to look up the
 *                                                                      addresses.
 *
 * \context         ANY
 *
 * \pre             The provided buffer size should be at least kMaxInterfaceName + 1.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-InterfaceTranslation
 *
 * \vprivate        Vector component internal API
 */
::amsr::core::Result<std::size_t> FindInterfaceNameByAddress(ip::Address address, ::amsr::core::Span<char> buffer,
                                                             net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief           Finds a network interface that is identified by the supplied name.
 *
 * \details         Acquires the index of a network interface from the name of the interface.
 *
 * \param[in]       name                      The name to be used.
 * \param[in]       netstack_id               ID of the network stack used to resolve the name to the index. Only used
 *                                            on QNX.
 *
 * \return          The index of the interface that is identified by that name.
 *
 * \error           OsabErrc::kSystemEnvironmentError                         No interface identified by that name
 *                                                                            found.
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
::amsr::core::Result<Interface::Index> FindInterfaceIndexByName(::amsr::core::StringView name,
                                                                net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief           Extracts the address assigned to a network interface.
 *
 * \tparam          Address                   Selects if an IPv4 (ip::AddressV4) or IPv6 (ip::AddressV6) or an Ethernet
 *                                            (EthernetAddress) address shall be extracted.
 *
 * \param[in]       interface                 The network interface object.
 * \param[in]       netstack_id               ID of the network stack used extract the address. Only used on QNX.
 *
 * \return          The address of the desired type assigned to the network interface.
 *
 * \error           OsabErrc::kUnexpected                               Some internal state that should not arise was
 *                                                                      detected.
 *
 * \error           OsabErrc::kInsufficientPrivileges                   The process has insufficient privileges to
 *                                                                      extract the assigned address.
 *
 * \error           OsabErrc::kAddressError                             The index does not identify a network interface.
 *
 * \error           OsabErrc::kAddressError                             Interface does not have an IP address of the
 *                                                                      desired family assigned.
 *
 * \error           OsabErrc::kResource                                 Not enough system resources to extract the
 *                                                                      address.
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
template <typename Address>
auto ExtractAddress(Interface const& interface, net_utils::NetworkStackId const& netstack_id) noexcept
    -> ::amsr::core::Result<Address>;

/*!
 * \brief           Extracts the netmask from a network interface.
 *
 * \tparam          Address                   Selects if an IPv4 (ip::AddressV4) or IPv6 (ip::AddressV6) netmask shall
 *                                            be extracted.
 *
 * \param[in]       interface                 The network interface object.
 * \param[in]       netstack_id               ID of the network stack used extract the netmask. Only used on QNX.
 *
 * \return          The netmask of the desired type assigned to the network interface.
 *
 * \error           OsabErrc::kUnexpected                               Some internal state that should not arise was
 *                                                                      detected.
 *
 * \error           OsabErrc::kInsufficientPrivileges                   The process has insufficient privileges to
 *                                                                      extract the assigned netmask.
 *
 * \error           OsabErrc::kAddressError                             The index does not identify a network interface.
 *
 * \error           OsabErrc::kAddressError                             Interface does not have an IP address of the
 *                                                                      desired family assigned.
 *
 * \error           OsabErrc::kResource                                 Not enough system resources to extract the
 *                                                                      netmask.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-InterfaceTranslation,
 *                  DSGN-Osab-NetworkDependentAddresses
 *
 * \vprivate        Vector component internal API
 */
template <typename Address>
auto ExtractNetmask(Interface const& interface, net_utils::NetworkStackId const& netstack_id) noexcept
    -> ::amsr::core::Result<Address>;

/*!
 * \brief           Extracts the name of a network interface.
 *
 * \param[in]       index                     Index of the interface whose name shall be extracted.
 * \param[in]       netstack_id               ID of the network stack used to resolve the index to the name. Only used
 *                                            on QNX.
 * \param[out]      buffer                    Buffer to write the \0-terminated name string into.
 *
 * \return          Number of characters the name consists of (terminating \0 character not counted).
 *
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError    No interface identified by the supplied index
 *                                                                      has been found.
 *
 * \context         ANY
 *
 * \pre             buffer must be able to hold at least Interface::kMaxInterfaceName + 1 characters.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-InterfaceTranslation
 *
 * \vprivate        Vector component internal API
 */
::amsr::core::Result<::amsr::core::Span<char>::size_type> ExtractName(Interface::Index index,
                                                                      net_utils::NetworkStackId const& netstack_id,
                                                                      ::amsr::core::Span<char> buffer) noexcept;

/*!
 * \brief           Collects all IPv4, IPv6, Ethernet addresses assigned to the given interface.
 *
 * \param[in]       name                      The name of the interface to be searched, must be at most
 *                                            kMaxNetworkInterfaceName characters long.
 * \param[in]       netstack_id               ID of the network stack used to resolve the index to the name. Only used
 *                                            on QNX.
 * \param[out]      addresses                 List of interface addresses found.
 *
 * \error           OsabErrc::kResource                     Not enough system resources to acquire the list of
 *                                                          addresses.
 * \error           OsabErrc::kInsufficientPrivileges       The process has insufficient privileges to acquire the
 *                                                          network interface addresses.
 * \error           OsabErrc::kUnexpected                   Unexpected error that is mapped to no category.
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
auto CollectInterfaceAddresses(::amsr::core::StringView name, net_utils::NetworkStackId const& netstack_id,
                               ara::core::Vector<InterfaceAddressEntry>& addresses) noexcept
    -> ::amsr::core::Result<void>;

/*!
 * \brief           Reports all IPv4, IPv6, Ethernet addresses assigned to the given interface via the callback
 *                  function.
 *
 * \param[in]       name                      The name of the interface to be searched, must be at most
 *                                            kMaxNetworkInterfaceName characters long.
 * \param[in]       netstack_id               ID of the network stack used to resolve the index to the name. Only used
 *                                            on QNX.
 * \param[in]       callback                  Callback which is invoked for each address entry found. If the callback
 *                                            returns false, search is discontinued.
 *
 * \error           OsabErrc::kResource                     Not enough system resources to acquire the list of
 *                                                          addresses.
 * \error           OsabErrc::kInsufficientPrivileges       The process has insufficient privileges to acquire the
 *                                                          network interface addresses.
 * \error           OsabErrc::kUnexpected                   Unexpected error that is mapped to no category.
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
auto ReportInterfaceAddresses(::amsr::core::StringView name, net_utils::NetworkStackId const& netstack_id,
                              amsr::net::GetAddressCallBack&& callback) noexcept -> ::amsr::core::Result<void>;

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_INTERFACE_SERVICES_H_

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
 *        \brief  Implements a template to resolve domain name to IP address.
 *        \unit   osabstraction::net
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_RESOLVER_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_RESOLVER_H_

#include "amsr/core/result.h"
#include "amsr/endianness/internal/os_interface_endianness.h"
#include "amsr/net/internal/os_interface.h"
#include "amsr/net/internal/resolver_settings.h"
#include "amsr/net/internal/socket_addr_access.h"
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/internal/endpoint.h"
#include "amsr/net_utils/network_stack_id.h"
#include "ara/core/vector.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Template class for BasicResolver.
 *
 * \details
 * This class represents the complete logic for domain name resolution, from text based string to numeric IP address.
 *
 * \tparam  Protocol  The protocol that shall be used.
 */
template <class Protocol>
class BasicResolver {
 public:
  /*!
   * \brief Alias to the template parameter Protocol. Check class description for details.
   *
   * \details
   * The Protocol Type shall satisfy the following requirements:
   * - static constexpr auto GetType() noexcept -> amsr::net::internal::SocketType
   *
   */
  using ProtocolType = Protocol;

  /*!
   * \brief Alias to an Endpoint that can be used with the specified protocol.
   *
   * \details
   * The Endpoint Type shall satisfy the following requirements:
   * - auto MakeEndpoint(Address ip_address, Port port) const noexcept -> amsr::core::Result<Endpoint<ProtocolType>>
   *
   */
  using EndpointType = typename ProtocolType::Endpoint;

  /*!
   * \brief Type for the list that contains entries returned by Resolve().
   */
  using ResolvedEndpoints = ::ara::core::Vector<EndpointType>;

  /*!
   * \brief Resolves the domain name to IP address.
   *
   * \param[in] host            String represents host name.
   * \param[in] netstack_id     Network stack to be used by the endpoint.
   *                            Defaults to default network stack.
   *
   * \return List of all entries returned by resolving the domain name.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
   * \error osabstraction::OsabErrc::kDoesNotExist               The host name does not exist.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        Resolution was successful, but there is no address
   *                                                             associated with the host name.
   * \error osabstraction::OsabErrc::kAddressError               Input numerical address requires an interface to
   *                                                             be supplied as well. So MakeEndpoint() with
   *                                                             interface as an input argument has to be called.
   *
   * \context         -
   *
   * \pre             The input strings have to be null-terminated.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-DNS_Resolution
   *
   * \vprivate        Vector product internal API
   */
  static auto Resolve(vac::container::CStringView const host,
                      net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<ResolvedEndpoints> {
    ResolverSettings<ProtocolType> resolver_settings{};
    return ResolveWithSettings(host, vac::container::CStringView{""}, net::ip::Port{0}, resolver_settings, netstack_id);
  }

  /*!
   * \brief Resolves the domain name to IP address.
   *
   * \param[in] host            String represents host name.
   * \param[in] service         String represents service name.
   * \param[in] netstack_id     Network stack to be used by the endpoint.
   *                            Defaults to default network stack.
   *
   * \return List of all entries returned by resolving the domain name.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
   * \error osabstraction::OsabErrc::kDoesNotExist               The host name does not exist.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        Resolution was successful, but there is no address
   *                                                             associated with the host name.
   * \error osabstraction::OsabErrc::kAddressError               Input numerical address requires an interface to
   *                                                             be supplied as well. So MakeEndpoint() with
   *                                                             interface as an input argument has to be called.
   *
   * \context         -
   *
   * \pre             The input strings have to be null-terminated.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-DNS_Resolution
   *
   * \vprivate        Vector product internal API
   */
  static auto Resolve(vac::container::CStringView const host, vac::container::CStringView const service,
                      net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<ResolvedEndpoints> {
    ResolverSettings<ProtocolType> resolver_settings{};
    return ResolveWithSettings(host, service, net::ip::Port{0}, resolver_settings, netstack_id);
  }

  /*!
   * \brief Resolves the domain name to IP address.
   *
   * \param[in] host                   String represents host name.
   * \param[in] service                String represents service name.
   * \param[in] resolver_settings      Settings for resolving the domain name.
   * \param[in] netstack_id            Network stack to be used by the endpoint.
   *                                   Defaults to default network stack.
   *
   * \return List of all entries returned by resolving the domain name.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
   * \error osabstraction::OsabErrc::kDoesNotExist               The host name does not exist.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        Resolution was successful, but there is no address
   *                                                             associated with the host name.
   * \error osabstraction::OsabErrc::kAddressError               Input numerical address requires an interface to
   *                                                             be supplied as well. So MakeEndpoint() with
   *                                                             interface as an input argument has to be called.
   *
   * \context         -
   *
   * \pre             The input strings have to be null-terminated.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-DNS_Resolution
   *
   * \vprivate        Vector product internal API
   */
  static auto Resolve(vac::container::CStringView const host, vac::container::CStringView const service,
                      ResolverSettings<ProtocolType>& resolver_settings,
                      net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<ResolvedEndpoints> {
    return ResolveWithSettings(host, service, net::ip::Port{0}, resolver_settings, netstack_id);
  }

  /*!
   * \brief Resolves the domain name to IP address.
   *
   * \param[in] host                   String represents host name.
   * \param[in] port                   Port the endpoint shall contain.
   * \param[in] netstack_id            Network stack to be used by the endpoint.
   *                                   Defaults to default network stack.
   *
   * \return List of all entries returned by resolving the domain name.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
   * \error osabstraction::OsabErrc::kDoesNotExist               The host name does not exist.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        Resolution was successful, but there is no address
   *                                                             associated with the host name.
   * \error osabstraction::OsabErrc::kAddressError               Input numerical address requires an interface to
   *                                                             be supplied as well. So MakeEndpoint() with
   *                                                             interface as an input argument has to be called.
   *
   * \context         -
   *
   * \pre             The input strings have to be null-terminated.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-DNS_Resolution
   *
   * \vprivate        Vector product internal API
   */
  static auto Resolve(vac::container::CStringView const host, net::ip::Port const& port,
                      net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<ResolvedEndpoints> {
    ResolverSettings<ProtocolType> resolver_settings{};
    return ResolveWithSettings(host, vac::container::CStringView{""}, port, resolver_settings, netstack_id);
  }

  /*!
   * \brief Resolves the domain name to IP address.
   *
   * \param[in] host                   String represents host name.
   * \param[in] port                   Port the endpoint shall contain.
   * \param[in] resolver_settings      Settings for resolving the domain name.
   * \param[in] netstack_id            Network stack to be used by the endpoint.
   *                                   Defaults to default network stack.
   *
   * \return List of all entries returned by resolving the domain name.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
   * \error osabstraction::OsabErrc::kDoesNotExist               The host name does not exist.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        Resolution was successful, but there is no address
   *                                                             associated with the host name.
   * \error osabstraction::OsabErrc::kAddressError               Input numerical address requires an interface to
   *                                                             be supplied as well. So MakeEndpoint() with
   *                                                             interface as an input argument has to be called.
   *
   * \context         -
   *
   * \pre             The input strings have to be null-terminated.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-DNS_Resolution
   *
   * \vprivate        Vector product internal API
   */
  static auto Resolve(vac::container::CStringView const host, net::ip::Port const& port,
                      ResolverSettings<ProtocolType>& resolver_settings,
                      net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<ResolvedEndpoints> {
    return ResolveWithSettings(host, vac::container::CStringView{""}, port, resolver_settings, netstack_id);
  }

 private:
  /*!
   * \brief Collect addresses information of input host and service.
   *
   * \param[in]  host                String represents host name.
   * \param[in]  service             String represents service name.
   * \param[in]  port                Port the endpoint shall contain.
   * \param[in]  resolver_settings   Settings to help in resolving domain name.
   * \param[in]  netstack_id         Network stack to be used by the endpoint.
   *
   * \return List of all entries returned by resolving the domain name.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
   * \error osabstraction::OsabErrc::kDoesNotExist               The host name does not exist.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        Resolution was successful, but there is no address
   *                                                             associated with the host name.
   *
   * \context         -
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static auto CollectAddressesInformation(vac::container::CStringView host, vac::container::CStringView service,
                                          net::ip::Port const& port, ResolverSettings<ProtocolType>& resolver_settings,
                                          net_utils::NetworkStackId const& netstack_id) noexcept
      -> ::amsr::core::Result<ResolvedEndpoints>;

  /*!
   * \brief Resolves the domain name to IP address.
   *
   * \param[in] host                String represents host name.
   * \param[in] service             String represents service name.
   * \param[in] port                Port the endpoint shall contain.
   * \param[in] resolver_settings   Settings to help in resolving domain name.
   * \param[in] netstack_id         Network stack to be used by the endpoint.
   *
   * \return List of all entries returned by resolving the domain name.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError                   One of the inputs to getaddrinfo() is invalid.
   * \error osabstraction::OsabErrc::kBusy                       Operation would block. Try again later.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to perform operation.
   * \error osabstraction::OsabErrc::kDoesNotExist               The host name does not exist.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        Resolution was successful, but there is no address
   *                                                             associated with the host name.
   * \error osabstraction::OsabErrc::kAddressError               Input numerical address requires an interface to
   *                                                             be supplied as well. So MakeEndpoint() with
   *                                                             interface as an input argument has to be called.
   *
   * \context         -
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  static auto ResolveWithSettings(vac::container::CStringView host, vac::container::CStringView service,
                                  net::ip::Port const& port, ResolverSettings<ProtocolType>& resolver_settings,
                                  net_utils::NetworkStackId const& netstack_id) noexcept
      -> ::amsr::core::Result<ResolvedEndpoints>;
};

/***********************************************************************************************************************
 * Template implementation
 **********************************************************************************************************************/

/*!
 * \internal
 * - Call GetAddressesInfo() to get all addresses information.
 * - If there is no error, iterate through the list of addresses.
 *   - Based on address family, set endpoint token address and status.
 *   - Add an entry to list.
 * - Free memory used by GetAddressesInfo().
 * - Return endpoints entries list if the list is not empty.
 * - If the list is empty, return OsabErrc::kAddressNotAvailable.
 * \endinternal
 */
template <class Protocol>
auto BasicResolver<Protocol>::CollectAddressesInformation(vac::container::CStringView const host,
                                                          vac::container::CStringView const service,
                                                          net::ip::Port const& port,
                                                          ResolverSettings<ProtocolType>& resolver_settings,
                                                          net_utils::NetworkStackId const& netstack_id) noexcept
    -> ::amsr::core::Result<ResolvedEndpoints> {
  ::amsr::core::Result<ResolvedEndpoints> result{osabstraction::OsabErrc::kUnexpected};
  // PTP-B-OsAbstraction-Net-BasicResolver-CollectAddressesInformation
  ::amsr::core::Result<AddrInfoView> const address_info{
      os_interface::GetAddressesInfo(host, service, resolver_settings.GetHints(), netstack_id)};
  if (address_info.HasValue()) {
    AddrInfoView addrinfo{address_info.Value()};
    // Iterate through the list of addresses
    ::ara::core::Vector<EndpointType> entries_list{};
    while (addrinfo.pointer != nullptr) {
      amsr::net::ip::internal::EndpointConstructionToken token{};
      amsr::net::internal::SocketMutableAddrView const addrinfo_view{addrinfo.pointer->ai_addr,
                                                                     addrinfo.pointer->ai_addrlen};
      amsr::net::internal::SocketFamily const addrinfo_family{addrinfo.pointer->ai_family};
      if (internal::SocketAddrAccess::IsV4(addrinfo_family)) {  // IPv4
        std::memcpy(&token.ipv4_information, addrinfo_view.pointer, addrinfo_view.size);
        if (service.empty()) {
          token.ipv4_information.sin_port = amsr::endianness::internal::os_interface::HostToNetworkUInt16(port.port);
        }
        token.status = amsr::net::ip::internal::EndpointConstructionToken::EndpointStatus::kIsV4;
      } else if (internal::SocketAddrAccess::IsV6(addrinfo_family)) {  // IPv6
        std::memcpy(&token.ipv6_information, addrinfo_view.pointer, addrinfo_view.size);
        if (service.empty()) {
          token.ipv6_information.sin6_port = amsr::endianness::internal::os_interface::HostToNetworkUInt16(port.port);
        }
        token.status = amsr::net::ip::internal::EndpointConstructionToken::EndpointStatus::kIsV6;
      } else {
        // PTP-S-OsAbstraction-Net-BasicResolver-IncorrectAddressFamily
      }
      if (token.status != amsr::net::ip::internal::EndpointConstructionToken::EndpointStatus::kEmpty) {
        token.netstack_id = netstack_id;
        entries_list.emplace_back(EndpointType{token});
      }
      addrinfo.pointer = addrinfo.pointer->ai_next;
    }
    os_interface::FreeAddressesInfo(address_info.Value());
    entries_list.empty() ? result.EmplaceError(osabstraction::OsabErrc::kAddressNotAvailable)
                         : result.EmplaceValue(std::move(entries_list));
  } else {
    result.EmplaceError(address_info.Error());
  }
  // PTP-E-OsAbstraction-Net-BasicResolver-CollectAddressesInformation
  return result;
}

/*!
 * \internal
 * - Check if input host is a numerical address using MakeAddress().
 *   - If yes, create a single endpoint using the address and the input port and push to the entries list.
 *   - If no, this means that the the address is textual.
 *   - Call CollectAddressesInformation() to get address information.
 * - Return endpoints entries list.
 * - Otherwise any error happens, return the error.
 * \endinternal
 */
template <class Protocol>
auto BasicResolver<Protocol>::ResolveWithSettings(vac::container::CStringView const host,
                                                  vac::container::CStringView const service, net::ip::Port const& port,
                                                  ResolverSettings<ProtocolType>& resolver_settings,
                                                  net_utils::NetworkStackId const& netstack_id) noexcept
    -> ::amsr::core::Result<ResolvedEndpoints> {
  ::amsr::core::Result<ResolvedEndpoints> result{osabstraction::OsabErrc::kUnexpected};
  ::amsr::core::Result<amsr::net::ip::Address> const address_result{amsr::net::ip::Address::MakeAddress(host)};
  if (address_result.HasValue()) {
    ::amsr::core::Result<EndpointType> endpoint_result{
        EndpointType::MakeEndpoint(address_result.Value(), port, netstack_id)};
    if (endpoint_result.HasValue()) {
      ::ara::core::Vector<EndpointType> entries_list{};
      entries_list.emplace_back(endpoint_result.Value());
      result.EmplaceValue(std::move(entries_list));
    } else {
      result.EmplaceError(endpoint_result.Error());
    }
  } else {
    ::amsr::core::Result<ResolvedEndpoints> const addrinfo_result{
        CollectAddressesInformation(host, service, port, resolver_settings, netstack_id)};
    addrinfo_result.HasValue() ? result.EmplaceValue(addrinfo_result.Value())
                               : result.EmplaceError(addrinfo_result.Error());
  }
  return result;
}

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_RESOLVER_H_

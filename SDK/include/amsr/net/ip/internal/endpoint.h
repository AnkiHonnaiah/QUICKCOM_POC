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
 *        \brief  Class to represent IP network endpoints.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_ENDPOINT_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/socket.h>

#include "amsr/core/optional.h"
#include "amsr/net/interface.h"
#include "amsr/net/ip/address.h"
#include "amsr/net_utils/network_stack_id.h"

#include "amsr/net/internal/types.h"

// Include only required here because doxygen can not find the declaration of MakeEndpoint(endpoint, port) in the class
// if that function is in the cpp file. Should doxygen be fixed and the function be moved back to the cpp file, this
// include can be removed.
#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/net/internal/socket_addr_access.h"

namespace amsr {
namespace net {
namespace ip {
namespace internal {

/*!
 * \brief           Token that can be used to create an endpoint.
 *
 * \details         The token has no template information and can thus be used for all endpoint types.
 *
 * \vprivate        Vector component internal API
 */
struct EndpointConstructionToken final {
  /*!
   * \brief           IPv4 information about the endpoint.
   */
  struct sockaddr_in ipv4_information {};

  /*!
   * \brief           IPv6 information about the endpoint.
   */
  struct sockaddr_in6 ipv6_information {};

  /*!
   * \brief           Information about the interface.
   *
   * \details         If the endpoint shall not have an interface, the optional has no value.
   */
  ::amsr::core::Optional<Interface> interface {};

  /*!
   * \brief           Status an endpoint can have.
   */
  enum class EndpointStatus : std::uint8_t {
    /*!
     * \brief           The endpoint does not contain anything.
     */
    kEmpty = 0,

    /*!
     * \brief           The endpoint is an IPv4 network endpoint.
     */
    kIsV4 = 1,

    /*!
     * \brief           The endpoint is an IPv6 network endpoint.
     */
    kIsV6 = 2
  };

  /*!
   * \brief           The status of the endpoint.
   */
  EndpointStatus status{EndpointStatus::kEmpty};

  /*!
   * \brief           Network stack used by the endpoint.
   */
  net_utils::NetworkStackId netstack_id{};
};

/*!
 * \brief           Represents a communication endpoint.
 *
 * \details         A communication endpoint uniquely identifies an addressable point in a network.
 *
 * \tparam          ProtocolType                     The protocol type refers to a transportation protocol (e.g. TCP or
 *                                                   UDP). A protocol class provides the at least the methods:
 *                                                   - auto V4() noexcept -> Tcp/Udp
 *                                                   - auto V6() noexcept -> Tcp/Udp
 */
template <typename ProtocolType>
class Endpoint final {
 public:
  /*!
   * \brief           The protocol the endpoint is used for.
   */
  using Protocol = ProtocolType;

  /*!
   * \brief           Creates an Endpoint from an IP address, port and Interface.
   *
   * \param[in]       ip_address                IP address the endpoint shall contain.
   * \param[in]       port                      Port the endpoint shall contain.
   *                                            If equals 0, ephemeral port is assigned.
   * \param[in]       interface                 Interface the endpoint shall contain. The created endpoint uses the
   *                                            network stack this interface instance also uses.
   *
   * \return          The created endpoint.
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
   * \trace           DSGN-Osab-EndpointCreation
   *
   * \vprivate        Vector product internal API
   */
  static auto MakeEndpoint(Address ip_address, Port port, Interface interface) noexcept -> Endpoint<ProtocolType>;

  /*!
   * \brief           Creates an Endpoint from an IP address and port.
   *
   * \details         The supplied IP address must either be an IPv4 address or an IPv6 address with global scope.
   *
   * \param[in]       ip_address                IP address the endpoint shall contain.
   * \param[in]       port                      Port the endpoint shall contain.
   *                                            If equals 0, ephemeral port is assigned.
   * \param[in]       netstack_id               Network stack to be used by the endpoint. Defaults to default network
   *                                            stack.
   *
   * \return          The created endpoint.
   *
   * \error           OsabErrc::kAddressError                           The supplied address requires an interface to be
   *                                                                    supplied as well.
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
   * \trace           DSGN-Osab-EndpointCreation
   *
   * \vprivate        Vector product internal API
   */
  static auto MakeEndpoint(Address ip_address, Port port,
                           net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<Endpoint<ProtocolType>>;

  /*!
   * \brief           Creates an Endpoint from an address string and port.
   *
   * \details         Should the address string hold an IPv6 address of non-global scope, it is required to hold an
   *                  interface as well.
   *
   * \param[in]       address                   String that holds IP address and optionally the interface the endpoint
   *                                            shall contain. The IP address part of the string must comply to the
   *                                            same requirements as a string input into
   *                                            Address::MakeAddress(amsr::core::StringView address). The interface is
   *                                            identified by either its name (containing at least one letter) or index
   *                                            (containing only decimals). The address and interface parts of the
   *                                            string are separated by one % character.
   * \param[in]       port                      Port the endpoint shall contain.
   *                                            If equals 0, ephemeral port is assigned.
   * \param[in]       netstack_id               Network stack to be used by the endpoint. Defaults to default network
   *                                            stack.
   *
   * \return          The created endpoint.
   *
   * \error           OsabErrc::kApiError                  The address part of the string does not hold a valid IP
   *                                                       address.
   *
   * \error           OsabErrc::kSystemEnvironmentError    No interface identified by the interface part of the string
   *                                                       has been found.
   *
   * \error           OsabErrc::kAddressError              The supplied string only holds an IP address but this address
   *                                                       requires an interface to be specified as well.
   *
   * \error           OsabErrc::kUnexpected                Some unexpected internal state was detected while processing
   *                                                       the interface part of the string.
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
   * \trace           DSGN-Osab-EndpointCreation
   *
   * \vprivate        Vector product internal API
   */
  static auto MakeEndpoint(::amsr::core::StringView address, Port port,
                           net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<Endpoint<ProtocolType>>;

  /*!
   * \brief           Creates an Endpoint from another endpoint and a port.
   *
   * \details         The created endpoint has all contents of the input endpoint except for the port, which is the
   *                  supplied port, if the input endpoint contains protocol information.
   *
   * \param[in]       endpoint                  The endpoint to create this endpoint from. The created endpoint uses the
   *                                            network stack this endpoint instance also uses.
   * \param[in]       port                      Port the endpoint shall contain. Ignored if endpoint does not have a
   *                                            protocol set. If equals 0, ephemeral port is assigned.
   *
   * \tparam          SourceProtocol            The protocol of the input endpoint.
   *
   * \return          The created endpoint.
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
   * \trace           DSGN-Osab-EndpointCreation
   *
   * \vprivate        Vector product internal API
   */
  template <typename SourceProtocol>
  static auto MakeEndpoint(Endpoint<SourceProtocol> endpoint, Port port) noexcept -> Endpoint<ProtocolType>;

  /*!
   * \brief           Creates an empty endpoint for a given protocol and network stack.
   *
   * \param[in]       protocol                  Protocol the endpoint shall be an endpoint of.
   * \param[in]       netstack_id               Network stack the endpoint shall use. Defaults to default network stack.
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
   * \trace           DSGN-Osab-EndpointCreation
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Create an endpoint with default values.
   * \endinternal
   */
  // VECTOR Next Line AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_DelegateConstructorParameterAreIncompatible
  explicit Endpoint(ProtocolType protocol, net_utils::NetworkStackId netstack_id = net_utils::NetworkStackId{}) noexcept
      : ipv4_information_{},
        ipv6_information_{},
        interface_{},
        network_stack_id_{std::move(netstack_id)},
        protocol_{protocol} {}

  /*!
   * \brief           Creates an empty endpoint with a unspecified protocol and default network stack.
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
   * \trace           DSGN-Osab-EndpointCreation
   *
   * \vprivate        Vector product internal API
   */
  Endpoint() noexcept = default;

  /*!
   * \brief           Outputs operating system socket address information about the endpoint.
   *
   * \return          The operating system socket address information about the endpoint.
   *
   * \context         ANY
   *
   * \pre             The endpoint's protocol must not be unspecified.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto GetData() const noexcept -> net::internal::SocketConstAddrView;

  /*!
   * \brief           Outputs operating system socket address information about the endpoint.
   *
   * \details         This API gives to user write access to object's internal data. So this API shouldn't be called
   *                  in parallel with getters of this class in order to preserve their thread safety.
   *
   * \return          The operating system socket address information about the endpoint.
   *
   * \context         ANY
   *
   * \pre             The endpoint's protocol must not be unspecified.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  auto GetData() noexcept -> net::internal::SocketMutableAddrView;

  /*!
   * \brief           Outputs the contained IP address.
   *
   * \return          The contained IP address.
   *
   * \context         ANY
   *
   * \pre             The endpoint's protocol must not be unspecified.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto GetAddress() const noexcept -> ip::Address;

  /*!
   * \brief           Outputs the contained interface.
   *
   * \return          The contained interface.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  No interface with the contained IP address
   *                                                                    assigned found.
   *
   * \error           osabstraction::OsabErrc::kAddressError            The contained IP address is assigned to
   *                                                                    multiple interfaces.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Some unexpected internal state was detected.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Insufficient privileges to create interface from
   *                                                                    endpoint address.
   *
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources to create interface
   *                                                                    from endpoint address.
   *
   * \context         ANY
   *
   * \pre             The endpoint's protocol must not be unspecified.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   */
  auto GetInterface() const noexcept -> ::amsr::core::Result<Interface>;

  /*!
   * \brief           Outputs the contained port number.
   *
   * \return          The contained port number.
   *
   * \context         ANY
   *
   * \pre             The endpoint's protocol must not be unspecified.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto GetPort() const noexcept -> ip::Port;

  /*!
   * \brief           Gets the ID of the network stack used by this endpoint.
   *
   * \return          The network stack ID. Only valid as long as this instance exists.
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
  auto GetNetworkStackId() const noexcept -> net_utils::NetworkStackId const&;

  /*!
   * \brief           Compares two endpoints for equality.
   *
   * \param[in]       other                     The endpoint to compare with this endpoint.
   *
   * \return          True if the endpoints contain the same IP address and port, else false.
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
  auto operator==(Endpoint const& other) const noexcept -> bool;

  /*!
   * \brief           Compares two endpoints for inequality.
   *
   * \param[in]       other                     The endpoint to compare with this endpoint.
   *
   * \return          False if the endpoints contain the same IP address and port, else true.
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
  auto operator!=(Endpoint const& other) const noexcept -> bool;

  /*!
   * \brief           Returns this endpoint's protocol
   *
   * \return          The endpoint's protocol.
   *
   * \context         ANY
   *
   * \pre             The endpoint's protocol must not be unspecified.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  /*!
   * \internal
   * - Abort program execution if the protocol is not set.
   * - Return the endpoint's protocol.
   * \endinternal
   */
  auto GetProtocol() const noexcept -> ProtocolType {
    if (!protocol_.has_value()) {
      ::amsr::core::Abort("Endpoint::GetProtocol: Protocol is not set.");
    }
    return this->protocol_.value();
  }

  /*!
   * \brief           Outputs a token from which a new endpoint with the same contents can be created.
   *
   * \return          The token from which a new endpoint can be created.
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
  EndpointConstructionToken GetToken() const noexcept;

  /*!
   * \brief           Creates an endpoint from a construction token.
   *
   * \details         There are no checks if the input is valid.
   *
   * \param[in]       token                     The token to construct the endpoint from.
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
  explicit Endpoint(EndpointConstructionToken const& token) noexcept;

 private:
  /*!
   * \brief           Creates an endpoint that holds an IP address, a port and an interface.
   *
   * \details         There are no checks if the input is valid.
   *
   * \param[in]       ip_address                The IP address the endpoint shall hold.
   * \param[in]       port                      The port the endpoint shall hold.
   * \param[in]       interface                 The interface the endpoint shall hold. The created endpoint uses the
   *                                            network stack this interface instance also uses.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   *
   * \trace           DSGN-Osab-EndpointCreation
   *
   * \vprivate        Vector component internal API
   */
  explicit Endpoint(Address ip_address, Port port, Interface interface) noexcept;

  /*!
   * \brief           Creates an endpoint that holds an IP address and a port.
   *
   * \details         There are no checks if the input is valid.
   *
   * \param[in]       ip_address                The IP address the endpoint shall hold.
   * \param[in]       port                      The port the endpoint shall hold.
   * \param[in]       netstack_id               The ID of the network stack used to resolve the IP address to an
   *                                            interface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   *
   * \trace           DSGN-Osab-EndpointCreation
   *
   * \vprivate        Vector component internal API
   */
  explicit Endpoint(Address ip_address, Port port, net_utils::NetworkStackId const& netstack_id) noexcept;

  /*!
   * \brief           Information about the endpoint if it is an IPv4 endpoint.
   */
  struct sockaddr_in ipv4_information_ {};

  /*!
   * \brief           Information about the endpoint if it is an IPv6 endpoint.
   */
  struct sockaddr_in6 ipv6_information_ {};

  /*!
   * \brief           Information about the interface.
   */
  ::amsr::core::Optional<Interface> interface_;

  /*!
   * \brief           ID of the network stack used to interact with the interface assigned to this endpoint.
   */
  net_utils::NetworkStackId network_stack_id_{};

  /*!
   * \brief           Protocol this endpoint is used for.
   */
  ::amsr::core::Optional<ProtocolType> protocol_;
};

// This function implementation is located here and not in the cpp file because doxygen can not find the respective
// declaration in the class. The compiler does find the declaration (as compilation works with doxygen disabled) so
// moving this implementation to the cpp file is possible in principle and only prevented by a tooling issue.
// Refer to INC0197811 support request at component development support for more information.
/*!
 * \internal
 * - Extract the contents of the supplied endpoint.
 * - If the supplied endpoint has a protocol set
 *   - set the supplied port.
 * - Create and output the new endpoint.
 * \endinternal
 */
template <typename ProtocolType>
template <typename SourceProtocol>
auto Endpoint<ProtocolType>::MakeEndpoint(Endpoint<SourceProtocol> endpoint, Port port) noexcept
    -> Endpoint<ProtocolType> {
  amsr::net::ip::internal::EndpointConstructionToken token{endpoint.GetToken()};

  if (token.status == EndpointConstructionToken::EndpointStatus::kIsV4) {
    AddressV4::Native const address{amsr::net::internal::SocketAddrAccess::ExtractAddress(&(token.ipv4_information))};

    token.ipv4_information = amsr::net::internal::SocketAddrAccess::MakeSocketAddress(address, port);
  } else if (token.status == EndpointConstructionToken::EndpointStatus::kIsV6) {
    AddressV6::Native const address{amsr::net::internal::SocketAddrAccess::ExtractAddress(&(token.ipv6_information))};
    amsr::net::internal::InterfaceIndex const interface {
      amsr::net::internal::SocketAddrAccess::ExtractInterfaceIndex(&(token.ipv6_information))
    };

    token.ipv6_information = amsr::net::internal::SocketAddrAccess::MakeSocketAddress(address, port, interface);
  } else {
    // The endpoint has no protocol set. No change of port required.
  }

  return Endpoint<ProtocolType>{token};
}

}  // namespace internal
}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_ENDPOINT_H_

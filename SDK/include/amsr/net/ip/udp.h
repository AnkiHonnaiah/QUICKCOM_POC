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
 *        \brief  Refers the UDP protocol.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_UDP_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_UDP_H_

#include <sys/socket.h>
#include <cstdint>

#include "amsr/net/internal/common_constants.h"
#include "amsr/net/internal/os_specific_constants.h"
#include "amsr/net/internal/types.h"

#include "amsr/net/internal/basic_datagram_socket.h"
#include "amsr/net/internal/basic_resolver.h"
#include "amsr/net/ip/internal/endpoint.h"

namespace amsr {
namespace net {
namespace ip {

/*!
 * \brief           Datagram length to indicate a truncated datagram.
 */
static constexpr std::uint32_t const kTruncatedDatagram{amsr::net::internal::kTruncatedDatagram};

/*!
 * \brief           Class that refers the UDP protocol.
 */
class Udp {
 public:
  /*!
   * \brief           Creates and outputs an Udp object that uses IPv4.
   *
   * \return          The created object.
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
  constexpr static Udp V4() noexcept { return Udp{amsr::net::internal::SocketFamily{AF_INET}}; }

  /*!
   * \brief           Creates and outputs an Udp object that uses IPv6.
   *
   * \return          The created object.
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
  constexpr static Udp V6() noexcept { return Udp{amsr::net::internal::SocketFamily{AF_INET6}}; }

  /*!
   * \brief           The socket used when using UDP.
   */
  using Socket = amsr::net::internal::BasicDatagramSocket<Udp>;

  /*!
   * \brief           The domain name resolver used when using UDP.
   */
  using Resolver = amsr::net::internal::BasicResolver<Udp>;

  /*!
   * \brief           Settings used by BasicResolver.
   */
  using ResolverSettings = amsr::net::internal::ResolverSettings<Udp>;

  /*!
   * \brief           The endpoint used when using UDP.
   */
  using Endpoint = amsr::net::ip::internal::Endpoint<Udp>;

  /*!
   * \brief           List of Endpoints returned by Resolve().
   */
  using ResolvedEndpoints = ara::core::Vector<Endpoint>;

  /*!
   * \brief           Outputs the code that identifies the used IP address family for system calls.
   *
   * \return          AF_INET if IPv4 is used, AF_INET6 if IPv6 is used.
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
  constexpr amsr::net::internal::SocketFamily GetFamily() const noexcept { return family_; }

  /*!
   * \brief           Outputs the code that identifies the kind of protocol (stream or datagram) for system calls.
   *
   * \return          SOCK_DGRAM
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
  static constexpr amsr::net::internal::SocketType GetType() noexcept {
    return amsr::net::internal::SocketType{SOCK_DGRAM};
  }

  /*!
   * \brief           Outputs the protocol specification code for system calls.
   *
   * \return          0
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
  static constexpr amsr::net::internal::SocketProtocol GetProtocol() noexcept {
    return amsr::net::internal::SocketProtocol{0};
  }

  /*!
   * \brief           Compares two protocols for equality.
   *
   * \param[in]       other                     The protocol to compare with.
   *
   * \return          True if the protocols are equal, else false.
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
  constexpr auto operator==(Udp const& other) const noexcept -> bool {
    return GetFamily().value == other.GetFamily().value;
  }

  /*!
   * \brief           Compares two protocols for inequality.
   *
   * \param[in]       other                     The protocol to compare with.
   *
   * \return          True if the protocols are not equal, else false.
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
  constexpr auto operator!=(Udp const& other) const noexcept -> bool {
    return GetFamily().value != other.GetFamily().value;
  }

  /*!
   * \brief            The length of an Ethernet header in bytes.
   */
  static constexpr std::size_t kEtherHeaderLength{amsr::net::internal::kEtherHeaderLength};

  /*!
   * \brief           The minimum header size of an IPv4 packet (using no options) in bytes.
   * \details         The IPv4 header size is not fixed. It has a minimum length of 20 bytes, but a maximum length of up
   *                  to 60 bytes when IP options are included. Most packets have a header length of 20 bytes.
   */
  static constexpr std::size_t kIPv4HeaderLengthMin{amsr::net::internal::kIPv4HeaderLengthMin};

  /*!
   * \brief           The maximum header size of an IPv4 packet in bytes (minimum header size + options).
   * \details         The size of the IPv4 header is not fixed. It has a minimum length of 20 bytes, but a maximum
   *                  length of up to 60 bytes when IP options are included. Most packets have a header length of 20
   *                  bytes.
   */
  static constexpr std::size_t kIPv4HeaderLengthMax{amsr::net::internal::kIPv4HeaderLengthMax};

  /*!
   * \brief           The header size of an IPv6 packet in bytes.
   * \details         In addition to the fixed IPv6 header, none, one or multiple extension headers may follow,
   *                  providing additional protocol-specific information. Extension headers are optional and can include
   *                  headers such as a hop-by-hop options header, a routing header, a fragmentation header, an
   *                  authentication header, or an encapsulating security payload header. These extension headers can
   *                  increase the overall size of the IPv6 header beyond the minimum of 40 bytes.
   */
  static constexpr std::size_t kIPv6HeaderLengthMin{amsr::net::internal::kIPv6HeaderLengthMin};

  /*!
   * \brief           The minimum length of a UDP header in bytes.
   * \details         The minimum length of a UDP header is 20 bytes, which includes NO options. The
   *                  actual length of a UDP header depends on the presence of UDP options. The maximum length of a UDP
   *                  header is 60 bytes.
   */
  static constexpr std::size_t kUdpHeaderLength{amsr::net::internal::kUdpHeaderLength};

 private:
  /*!
   * \brief           Creates an Udp object.
   *
   * \param[in]       family  Code that identifies the used IP address family for system calls.
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
  constexpr explicit Udp(amsr::net::internal::SocketFamily family) noexcept : family_{family} {}

  /*!
   * \brief Code that identifies the used IP address family for system calls.
   */
  amsr::net::internal::SocketFamily family_;
};

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_UDP_H_

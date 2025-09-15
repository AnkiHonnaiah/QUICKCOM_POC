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
 *        \brief  Refers the TCP protocol.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_TCP_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_TCP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/net/internal/basic_acceptor_socket.h"
#include "amsr/net/internal/basic_resolver.h"
#include "amsr/net/internal/basic_stream_socket.h"
#include "amsr/net/internal/common_constants.h"
#include "amsr/net/internal/types.h"
#include "amsr/net/ip/internal/address_family.h"
#include "amsr/net/ip/internal/endpoint.h"

namespace amsr {
namespace net {
namespace ip {

/*!
 * \brief           Class that refers the TCP protocol.
 */
class Tcp {
 public:
  /*!
   * \brief           Creates and outputs a Tcp object that uses IPv4.
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
  constexpr static auto V4() noexcept -> Tcp { return Tcp{amsr::net::internal::SocketFamily{AF_INET}}; }

  /*!
   * \brief           Creates and outputs a Tcp object that uses IPv6.
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
  constexpr static auto V6() noexcept -> Tcp { return Tcp{amsr::net::internal::SocketFamily{AF_INET6}}; }

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
  constexpr auto GetFamily() const noexcept -> amsr::net::internal::SocketFamily { return family_; }

  /*!
   * \brief           Outputs the code that identifies the kind of protocol (stream or datagram) for system calls.
   *
   * \return          SOCK_STREAM
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
  static constexpr auto GetType() noexcept -> amsr::net::internal::SocketType {
    return amsr::net::internal::SocketType{SOCK_STREAM};
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
  static constexpr auto GetProtocol() noexcept -> amsr::net::internal::SocketProtocol {
    return amsr::net::internal::SocketProtocol{0};
  }

  /*!
   * \brief           The socket used when using TCP.
   */
  using Socket = amsr::net::internal::BasicStreamSocket<Tcp>;

  /*!
   * \brief           The acceptor used when using TCP.
   */
  using Acceptor = amsr::net::internal::BasicAcceptorSocket<Tcp>;

  /*!
   * \brief           The domain name resolver used when using TCP.
   */
  using Resolver = amsr::net::internal::BasicResolver<Tcp>;

  /*!
   * \brief           Settings used by BasicResolver.
   */
  using ResolverSettings = amsr::net::internal::ResolverSettings<Tcp>;

  /*!
   * \brief           The endpoint used when using TCP.
   */
  using Endpoint = amsr::net::ip::internal::Endpoint<Tcp>;

  /*!
   * \brief           List of Endpoints returned by Resolve().
   */
  using ResolvedEndpoints = ara::core::Vector<Endpoint>;

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
  constexpr auto operator==(Tcp const& other) const noexcept -> bool {
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
  constexpr auto operator!=(Tcp const& other) const noexcept -> bool {
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
   * \brief           The minimum length of a TCP header in bytes.
   * \details         The minimum length of a TCP header is 20 bytes, which includes NO options. The
   *                  actual length of a TCP header depends on the presence of TCP options. The maximum length of a TCP
   *                  header is 60 bytes.
   */
  static constexpr std::size_t kTcpHeaderLengthMin{amsr::net::internal::kTcpHeaderLengthMin};

  /*!
   * \brief           The maximum length of a TCP header in bytes.
   * \details         The maximum length of a TCP header is 60 bytes, which includes options. The actual length of a TCP
   *                  header depends on the presence of TCP options. The minimum length of a TCP header is 20 bytes, but
   *                  it can be up to 60 bytes with options present.
   */
  static constexpr std::size_t kTcpHeaderLengthMax{amsr::net::internal::kTcpHeaderLengthMax};

 private:
  /*!
   * \brief           Creates a Tcp object.
   *
   * \param[in]       used_address_family       Code that identifies the used IP address family for system calls.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   *
   * \vprivate        Vector product internal API
   */
  explicit constexpr Tcp(amsr::net::internal::SocketFamily used_address_family) noexcept
      : family_{used_address_family} {}

  /*!
   * \brief         Code that identifies the used IP address family for system calls.
   */
  amsr::net::internal::SocketFamily family_;
};

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_TCP_H_

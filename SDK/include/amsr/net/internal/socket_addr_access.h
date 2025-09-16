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
 *        \brief  Wrapper around socket address structs (sockaddr, sockaddr_in, sockaddr_in6, ...)
 *        \unit   osabstraction::net
 *
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_ADDR_ACCESS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_ADDR_ACCESS_H_

#include <sys/socket.h>

#include "amsr/endianness/internal/os_interface_endianness.h"
#include "amsr/net/internal/ether_socket_utils.h"
#include "amsr/net/internal/os_interface.h"
#include "amsr/net/internal/os_specific_constants.h"
#include "amsr/net/internal/types.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Collection of functions for access to parts of a socket address structure.
 */
class SocketAddrAccess final {
 public:
  /*!
   * \brief           Outputs the IP address from a socket address object.
   *
   * \param[in]       sock_addr                                 The socket address object.
   *
   * \return          The IP address from the socket address object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Output the IPv4 address of the socket address object.
   * \endinternal
   */
  static Ipv4Native ExtractAddress(struct sockaddr_in const* sock_addr) noexcept { return sock_addr->sin_addr; }

  /*!
   * \brief           Outputs the IP address from a socket address object.
   *
   * \param[in]       sock_addr                                 The socket address object.
   *
   * \return          The IP address from the socket address object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Output the IPv6 address of the socket address object.
   * \endinternal
   */
  static Ipv6Native ExtractAddress(struct sockaddr_in6 const* sock_addr) noexcept { return sock_addr->sin6_addr; }

  /*!
   * \brief           Outputs the ethernet address from a socket address object.
   *
   * \param[in]       sock_addr                                 The socket address object.
   *
   * \return          The ethernet address from the socket address object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Output the ethernet address of the socket address object.
   * \endinternal
   */
  static EthNative ExtractAddress(SockaddrEther const* sock_addr) noexcept {
    return EthernetSocketAddressExtractAddress(sock_addr);
  }

  /*!
   * \brief           Outputs the port number from a socket address object.
   *
   * \param[in]       sock_addr                                 The socket address object.
   *
   * \return          The port number from the socket address object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Output the port number of the socket address object.
   * \endinternal
   */
  static Port ExtractPort(struct sockaddr_in const* sock_addr) noexcept {
    return {amsr::endianness::internal::os_interface::NetworkToHostUInt16(sock_addr->sin_port)};
  }

  /*!
   * \brief           Outputs the port number from a socket address object.
   *
   * \param[in]       sock_addr                                 The socket address object.
   *
   * \return          The port number from the socket address object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Output the port number of the socket address object.
   * \endinternal
   */
  static Port ExtractPort(struct sockaddr_in6 const* sock_addr) noexcept {
    return {amsr::endianness::internal::os_interface::NetworkToHostUInt16(sock_addr->sin6_port)};
  }

  /*!
   * \brief           Outputs the network interface from the socket address object.
   *
   * \param[in]       sock_addr                                 The socket address object.
   *
   * \return          The interface index from the socket address object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Output the interface index of the socket address object.
   * \endinternal
   */
  static InterfaceIndex ExtractInterfaceIndex(struct sockaddr_in6 const* sock_addr) noexcept {
    return {sock_addr->sin6_scope_id};
  }

  /*!
   * \brief           Outputs an IPv4 socket address object initialized from supplied values.
   *
   * \param[in]       address                                   The IPv4 address to initialize the object with.
   * \param[in]       port                                      The port number to initialize the object with.
   *
   * \return          The socket address object.
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
  /*!
   * \internal
   * - Insert the input values into the socket address object.
   * - Output the socket address object.
   * \endinternal
   */
  static struct sockaddr_in MakeSocketAddress(Ipv4Native address, Port port) noexcept {
    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = amsr::endianness::internal::os_interface::HostToNetworkUInt16(port.port);
    addr.sin_addr = address;
    return addr;
  }

  /*!
   * \brief           Outputs an IPv6 socket address object initialized from supplied values.
   *
   * \param[in]       address                                   The IPv6 address to initialize the object with.
   * \param[in]       port                                      The port number to initialize the object with.
   * \param[in]       interface                                 The interface index to initialize the object with.
   *
   * \return          The socket address object.
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
  /*!
   * \internal
   * - Insert the input values into the socket address object.
   * - Output the socket address object.
   * \endinternal
   */
  static struct sockaddr_in6 MakeSocketAddress(Ipv6Native address, Port port, InterfaceIndex interface) noexcept {
    struct sockaddr_in6 addr {};
    addr.sin6_family = AF_INET6;
    addr.sin6_port = amsr::endianness::internal::os_interface::HostToNetworkUInt16(port.port);
    addr.sin6_addr = address;
    addr.sin6_scope_id = interface.value;
    return addr;
  }

  /*!
   * \brief           Checks if the socket address is an IPv4 address object.
   *
   * \param[in]       sock_addr                                 The socket address.
   *
   * \return          True if the socket address is an IPv4 address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Check and output if the socket address is an IPv4 address.
   * \endinternal
   */
  static bool IsV4(struct sockaddr const* const sock_addr) noexcept {
    return (sock_addr != nullptr) && (sock_addr->sa_family == AF_INET);
  }

  /*!
   * \brief           Checks if the socket address is an IPv6 address object.
   *
   * \param[in]       sock_addr                                 The socket address.
   *
   * \return          True if the socket address is an IPv6 address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Check and output if the socket address is an IPv6 address.
   * \endinternal
   */
  static bool IsV6(struct sockaddr const* const sock_addr) noexcept {
    return (sock_addr != nullptr) && (sock_addr->sa_family == AF_INET6);
  }

  /*!
   * \brief           Checks if the socket address is an Ethernet address object.
   *
   * \param[in]       sock_addr                                 The socket address.
   *
   * \return          True if the socket address is an Ethernet address, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Check and output if the socket address is an Ethernet address.
   * \endinternal
   */
  static bool IsEther(struct sockaddr const* const sock_addr) noexcept {
    return (sock_addr != nullptr) && (sock_addr->sa_family == kAddressFamilyEthernet);
  }

  /*!
   * \brief           Checks if the socket family is an IPv4.
   *
   * \param[in]       sock_family                           The socket family.
   *
   * \return          True if the socket family is an IPv4, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Check and output if the socket family is an IPv4.
   * \endinternal
   */
  static bool IsV4(amsr::net::internal::SocketFamily const& sock_family) noexcept {
    return sock_family.value == AF_INET;
  }

  /*!
   * \brief           Checks if the socket family is an IPv6.
   *
   * \param[in]       sock_family                           The socket family.
   *
   * \return          True if the socket family is an IPv6, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Check and output if the socket family is an IPv6.
   * \endinternal
   */
  static bool IsV6(amsr::net::internal::SocketFamily const& sock_family) noexcept {
    return sock_family.value == AF_INET6;
  }
};

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_ADDR_ACCESS_H_

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
 *        \brief  Collection of type safe types that are used in in operating system interface.
 *        \unit   osabstraction::net
 *
 *      \details  Most operating system arguments are not type safe. The provided types encapsulate these types.
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_TYPES_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_TYPES_H_

#include <netdb.h>
#include <sys/socket.h>
#include <chrono>
#include <cstdint>

#include "amsr/core/array.h"

#include "amsr/net/internal/common_constants.h"
#include "amsr/net/internal/os_constants.h"
#include "amsr/net/internal/os_specific_constants.h"
#include "amsr/net/internal/specific_types.h"  // Required for ifaddrs.

#include "osabstraction/io/io_buffer.h"

namespace amsr {
namespace net {
namespace internal {

/***********************************************************************************************************************
 * socket() arguments
 **********************************************************************************************************************/

/*!
 * \brief Maps to a socket family.
 *
 * \details
 * Check the POSIX socket() call for details.
 */
struct SocketFamily {
  /*! \brief Socket family */
  std::int32_t value;
};

/*!
 * \brief Maps to a socket type.
 *
 * \details
 * Check the POSIX socket() call for details.
 */
struct SocketType {
  /*! \brief Socket type  */
  std::int32_t value;
};

/*!
 * \brief Maps to a socket protocol.
 *
 * \details
 * Check the POSIX socket() call for details.
 */
struct SocketProtocol {
  /*! \brief Socket protocol */
  std::int32_t value;
};

/***********************************************************************************************************************
 * IP address abstraction.
 **********************************************************************************************************************/

/*!
 * \brief IPv4 address
 *
 * \details Contains the bytes the address consists of in the byte order required by the operating system (network
 *          byte order).
 */
using Ipv4Native = in_addr;

/*!
 * \brief IPv6 address
 *
 * \details Contains the bytes the address consists of in the byte order required by the operating system (network
 *          byte order).
 */
using Ipv6Native = in6_addr;

/*!
 * \brief           Strong type for network port.
 */
struct Port {
  /*!
   * \brief         The actual port number.
   *
   * \details       This is the port number in host byte order.
   */
  std::uint16_t port;
};

/*!
 * \brief           Strong type for ephemeral ports range.
 */
struct EphemeralPortsRange {
  /*!
   * \brief         Minimum port value.
   *
   * \details       This is minimum value in range defined as Port type.
   */
  Port min_port;

  /*!
   * \brief         Maximum port value.
   *
   * \details       This is maximum value in range defined as Port type.
   */
  Port max_port;
};

/*!
 * \brief           Strong type for network prefix length.
 */
struct PrefixLength {
  /*!
   * \brief         The actual prefix length.
   */
  std::uint8_t prefix_length;
};

/***********************************************************************************************************************
 * Ethernet address.
 **********************************************************************************************************************/

/*!
 * \brief Ethernet address
 */
struct EthNative {
  /*! \brief Bytes that make up an Ethernet address. */
  ::amsr::core::Array<std::uint8_t, net::internal::kEthernetAddressLength> data{};
};

/***********************************************************************************************************************
 * Network interface index abstraction.
 **********************************************************************************************************************/

/*!
 * \brief Maps to a network interface index.
 */
struct InterfaceIndex {
  /*! \brief Network interface index */
  std::uint32_t value;
};

/***********************************************************************************************************************
 * Socket address abstraction.
 **********************************************************************************************************************/
/*!
 * \brief Size of a socket address.
 */
struct SocketAddressSize {
  /*!
   * \brief Size of the socket address in bytes.
   */
  socklen_t size;
};

/*!
 * \brief View to a mutable socket address.
 */
struct SocketMutableAddrView {
  /*!
   * \brief Pointer to the socket address.
   */
  struct sockaddr* pointer;

  /*!
   * \brief Size of the socket address in bytes.
   */
  socklen_t size;
};

/*!
 * \brief View to a const socket address.
 */
struct SocketConstAddrView {
  /*!
   * \brief Pointer to the socket address.
   */
  struct sockaddr const* pointer;

  /*!
   * \brief Size of the socket address in bytes.
   */
  socklen_t const size;
};

/*!
 * \brief Pointer to a list of socket address information.
 *
 * \details
 * Check the POSIX getaddrinfo() call for details.
 */
struct AddrInfoView {
  /*!
   * \brief Pointer to the linked list of socket address structures.
   */
  struct addrinfo* pointer;
};

/***********************************************************************************************************************
 * setsockopt()/getsockopt() arguments
 **********************************************************************************************************************/

/*!
 * \brief The socket option level.
 *
 * \details
 * Check POSIX getsockopt() call for details
 */
struct SockOptLevel {
  /*!
   * \brief Level
   */
  std::int32_t value;
};

/*!
 * \brief Socket option name
 *
 * \details
 * Check POSIX getsockopt() call for details
 */
struct SockOptName {
  /*!
   * The socket option name.
   */
  std::int32_t value;
};

/*!
 * \brief Stores a mutable view to the value
 *
 * \details
 * Combines the optval and the optlen parameter into one view.
 */
struct SockOptMutableValueView {
  /*!
   * \brief Base pointer to option value.
   */
  void* data;

  /*!
   * \brief Size of option value.
   */
  socklen_t size;
};

/*!
 * \brief Stores a const view to the value
 *
 * \details
 * Combines the optval and the optlen parameter into one view.
 */
struct SockOptConstValueView {
  /*!
   * \brief Base pointer to option value.
   */
  void const* data;

  /*!
   * \brief Size of option value.
   */
  socklen_t const size;
};

/***********************************************************************************************************************
 * ioctl() arguments
 **********************************************************************************************************************/

/*!
 * \brief IOctl command name.
 *
 * \details
 * Check POSIX ioctl() call for details.
 */
struct IoctlCmdName {
  /*!
   * The ioctl command name.
   */
  IoctlRequestType value;
};

/*!
 * \brief Stores a mutable view to the parameter value.
 */
struct IoctlMutableValueView {
  /*!
   * \brief Base pointer to option value.
   */
  void* data;
};

/***********************************************************************************************************************
 * Blocking mode arguments
 **********************************************************************************************************************/
/*!
 * \brief Stores a socket's blocking mode.
 */
struct SocketBlockingMode {
  /*!
   * \brief True if blocking shall be enabled, otherwise false.
   */
  bool value;
};

/***********************************************************************************************************************
 * Listen call arguments.
 **********************************************************************************************************************/
/*!
 * \brief Maximum number of queued connection request.
 *
 * \details
 * This number is passed to the listen() call.
 */
struct ListenBacklog {
  /*!
   * \brief Number of queued connection requests.
   */
  std::int32_t value;
};

/***********************************************************************************************************************
 * Generic ancillary data packages.
 * Used together with sendmsg() / recvmsg()
 **********************************************************************************************************************/

/*!
 * \brief Stores the protocol specific type.
 */
struct ControlMessageType {
  /*!
   * \brief Protocol specific type.
   */
  std::int32_t value;
};

/*!
 * \brief Buffer view for ancillary data.
 *
 * \details
 * The data and size specify the buffer to store the requested data.
 */
struct AncillaryDataView {
  /*!
   * \brief Base pointer to buffer to store the ancillary data.
   */
  void* data;

  /*!
   * \brief Buffer size of ancillary data.
   */
  std::size_t size;
};

/*!
 * \brief Bundles an ancillary data request into one structure.
 *
 * \details
 * It is possible to pass multiple ancillary data request to the operating system layer. This structure bundles
 * a request's data into one object.
 */
struct AncillaryDataRequest {
  /*!
   * \brief protocol level.
   *
   * \details
   * POSIX defines that the cmsg_level in the request has to be a legal level for getsockopt() / setsockopt().
   */
  SockOptLevel level;

  /*!
   * \brief The protocol specific type.
   */
  ControlMessageType type;

  /*!
   * \brief The buffer to store the data.
   */
  AncillaryDataView data_view;

  /*!
   * \brief Stores the actually used buffer size.
   *
   * This variable stores the used buffer space. The operating system interface will update the value
   * to the actually used space. The request is only used if the level and the type matches the received ancillary
   * data request.
   */
  std::size_t* used_bytes;
};

/*!
 * \brief A set of lags that are passed to the recv() and recvfrom() call.
 *
 */
struct ReceiveFlags {
  /*!
   * \brief Receive flags.
   */
  std::int32_t flags{};
};

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_TYPES_H_

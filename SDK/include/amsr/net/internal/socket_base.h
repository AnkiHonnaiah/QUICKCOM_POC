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
/*!        \file  socket_base.h
 *        \brief  A SocketBase class provides services and constants that are common for all sockets.
 *        \unit   osabstraction::net
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_BASE_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_BASE_H_

#include <sys/socket.h>

#include "amsr/net/internal/os_specific_constants.h"
#include "amsr/net/internal/socket_option.h"
#include "osabstraction/io/native_types.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Base class for all network sockets
 *
 * \details
 * This class provides all constants and socket options that are common for all network socket. Special member functions
 * declaration exist to prevent slicing or destruction of base class. All have to be defined because rule of six.
 */
class SocketBase {
 public:
  /*!
   * \brief Enables or disables the reuse address socket option.
   *
   * \trace DSGN-Osab-BasicIP_OptionReuseAddress
   */
  using SockOptReuseAddr = SocketOptionBoolean<SOL_SOCKET, SO_REUSEADDR>;

  /*!
   * \brief Socket option to set the Time To Live / hop limit of outgoing unicast packages.
   *
   * \note The default value (no option is set) for TTL / hop limit is OS dependent.
   *
   * \trace DSGN-Osab-BasicIP_OptionUnicastHops
   */
  using SockOptUnicastTTL = SocketOptionIPSignedInteger<IPPROTO_IP, IP_TTL, IPPROTO_IPV6, kIPv6UnicastHops>;

  /*!
   * \brief Sets the VLAN priority for sent packages.
   *
   * \trace DSGN-Osab-BasicIP_OptionVLANpriority
   */
  using SockOptVlanPriority = SocketOptionVlanPriority;

  /*!
   * \brief Sets the maximum socket send buffer in bytes.
   */
  using SockOptSendBuffer = SocketOptionSignedInteger<SOL_SOCKET, kSendBufferSize>;

  /*!
   * \brief Sets the maximum socket receive buffer in bytes.
   */
  using SockOptReceiveBuffer = SocketOptionSignedInteger<SOL_SOCKET, SO_RCVBUF>;

  /*!
   * \brief Sets the timeout for blocking send operations in microseconds.
   *
   * \details A timeout of 0 means to remove a previously set send timeout.
   */
  using SockOptSendTimeout = SocketOptionTimeMicrosecond<SOL_SOCKET, SO_SNDTIMEO>;

  /*!
   * \brief Sets the timeout for blocking receive operations in microseconds.
   *
   * \details A timeout of 0 means to remove a previously set receive timeout.
   */
  using SockOptReceiveTimeout = SocketOptionTimeMicrosecond<SOL_SOCKET, SO_RCVTIMEO>;

  /*!
   * \brief Binds the socket to a network device.
   *
   * \details The socket will only receive packets that were sent to the bound network device.
   * \trace DSGN-Osab-BasicIP_OptionBindToDevice
   */
  using SockOptBindToDevice = SocketOptionNetworkInterface<SOL_SOCKET, kBindToDevice>;

  /*!
   * \brief Deleted copy constructor
   */
  SocketBase(SocketBase const&) = delete;

  /*!
   * \brief Regular move constructor is not supported.
   */
  SocketBase(SocketBase&& other) = delete;

  /*!
   * \brief Deleted assignment operator
   */
  SocketBase& operator=(SocketBase const& other) = delete;

  /*!
   * \brief Deleted move operator
   */
  SocketBase& operator=(SocketBase&& other) = delete;

  /*!
   * \brief Default destructor.
   *
   * \details
   * Present to protect against up casts and destruction of child objects. Does not provide any additional destruction
   * steps.
   *
   * The destructor is virtual because the ICC/bauhaus cannot resolve a protected destructor the following CRTP
   * inheritance tree.
   */
  virtual ~SocketBase() = default;

 protected:
  /*!
   * \brief Constructs base object.
   *
   * \details
   * Class has no members and thus no initialization is required.
   */
  SocketBase() = default;
};

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_BASE_H_

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
 *        \brief  endpoint utility functions.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ENDPOINT_UTILS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ENDPOINT_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/optional.h"
#include "amsr/net/ip/internal/endpoint.h"
#include "amsr/net/ip/tcp.h"
#include "amsr/net/ip/udp.h"

namespace amsr {
namespace net {
namespace ip {

/*!
 * \brief           The endpoint used when using TCP.
 */
using TcpEndpoint = amsr::net::ip::internal::Endpoint<Tcp>;

/*!
 * \brief           The endpoint used when using UDP.
 */
using UdpEndpoint = amsr::net::ip::internal::Endpoint<Udp>;

/*!
 * \brief           Converts a TCP Endpoint to an UDP Endpoint.
 *
 * \param[in]       tcp_endpoint Endpoint instanciated with TCP Protocol.
 *
 * \return          The created UDP Endpoint
 * \context         ANY
 *
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \vprivate        Vector component internal API
 */
inline auto ConvertTcpToUdpEndpoint(TcpEndpoint const& tcp_endpoint) noexcept -> UdpEndpoint {
  amsr::net::ip::internal::EndpointConstructionToken const token{tcp_endpoint.GetToken()};
  return UdpEndpoint{token};
}

/*!
 * \brief           Converts an UDP Endpoint to a TCP Endpoint.
 *
 * \param[in]       udp_endpoint Endpoint instanciated with UDP Protocol.
 *
 * \return          The created TCP Endpoint
 *
 * \context         ANY
 *
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \vprivate        Vector component internal API
 */
inline auto ConvertUdpToTcpEndpoint(UdpEndpoint const& udp_endpoint) noexcept -> TcpEndpoint {
  amsr::net::ip::internal::EndpointConstructionToken const token{udp_endpoint.GetToken()};
  return TcpEndpoint{token};
}

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ENDPOINT_UTILS_H_

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
 *        \brief  Functions to verify if a protocol object uses an IP address family.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_ADDRESS_FAMILY_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_ADDRESS_FAMILY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/internal/types.h"

namespace amsr {
namespace net {
namespace ip {
namespace internal {

/*!
 * \brief           Outputs if IPv4 is used by the protocol object.
 *
 * \tparam          Protocol  The protocol type refers to a transportation protocol (e.g. TCP or UDP). A protocol class
 *                            provides the at least the methods:
 *                            - auto GetFamily() noexcept -> net::internal::SocketFamily
 *
 * \param[in]       protocol  The protocol to check.
 *
 * \return          True if IPv4 is used, else false.
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
template <typename Protocol>
bool IsV4(Protocol protocol) noexcept {
  return protocol.GetFamily().value == AF_INET;
}

/*!
 * \brief           Outputs if IPv6 is used by the protocol object.
 *
 * \tparam          Protocol  The protocol type refers to a transportation protocol (e.g. TCP or UDP). A protocol class
                              provides the at least the methods:
 *                            - auto GetFamily() noexcept -> net::internal::SocketFamily
 *
 * \param[in]       protocol  The protocol to check.
 *
 * \return          True if IPv6 is used, else false.
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
template <typename Protocol>
bool IsV6(Protocol protocol) noexcept {
  return protocol.GetFamily().value == AF_INET6;
}

}  // namespace internal
}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_INTERNAL_ADDRESS_FAMILY_H_

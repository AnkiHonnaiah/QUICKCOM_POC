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
 *        \brief  Functions for working with ethernet sockets.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_ETHER_SOCKET_UTILS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_ETHER_SOCKET_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/net/internal/os_specific_constants.h"
#include "amsr/net/internal/types.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief           Checks if the length of the address contained in SockaddrEther has been properly set.
 *
 * \param[in]       address                                   The object to extract the native address from.
 *
 * \return          true if contained address length matches the ethernet address length, false otherwise.
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE if the input is thread-safe
 *
 * \vprivate        Vector component internal API
 */
bool EthernetSocketAddressCheckLength(SockaddrEther const* address) noexcept;

/*!
 * \brief           Extracts the native ethernet address from the ethernet socket address object.
 *
 * \param[in]       address                                   The object to extract the native address from.
 *
 * \return          The native ethernet address.
 *
 * \context         ANY
 *
 * \pre             The input address contains a valid ethernet address.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE if the input is thread-safe
 *
 * \vprivate        Vector component internal API
 */
EthNative EthernetSocketAddressExtractAddress(SockaddrEther const* address) noexcept;

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_ETHER_SOCKET_UTILS_H_

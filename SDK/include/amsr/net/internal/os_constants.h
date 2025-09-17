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
 *        \brief  Posix-specific constants for working with the OS.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_CONSTANTS_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_CONSTANTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <arpa/inet.h>
#include <cstdint>

#include "amsr/net/internal/os_specific_constants.h"

namespace amsr {
namespace net {
namespace ip {
namespace internal {

/*!
 * \brief           Maximal length of a string that contains an IPv4 address.
 *
 * \details         One character must be subtracted from INET_ADDRSTRLEN because of the terminating \0 character.
 */
static constexpr std::uint8_t const kMaxIpv4AddressStringSize{INET_ADDRSTRLEN - 1};

/*!
 * \brief           Maximal length of a string that contains an IPv6 address.
 *
 * \details         One character must be subtracted from INET6_ADDRSTRLEN because of the terminating \0 character.
 */
static constexpr std::uint8_t const kMaxIpv6AddressStringSize{INET6_ADDRSTRLEN - 1};

}  // namespace internal
}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_CONSTANTS_H_

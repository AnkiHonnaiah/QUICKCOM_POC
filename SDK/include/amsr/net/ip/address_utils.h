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
 *        \brief  address utility functions.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_UTILS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/net/ip/address_v4.h"

namespace amsr {
namespace net {
namespace ip {

/*!
 * \brief           Creates a directed broadcast IPv4 address.
 *
 * \details         The directed broadcast address consists of the subnet address of the destination subnet with the
 *                  host part of the address filled with 1s.
 *
 * \param[in]       unicast_address       Unicast address in the subnet to which the directed broadcast address belongs.
 * \param[in]       netmask               Netmask of the subnet to which the directed broadcast address belongs.
 *
 * \return          The directed broadcast address.
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
 * \trace           DSGN-Osab-NetworkDependentAddresses
 *
 * \vprivate        Vector product internal API
 */
AddressV4 MakeDirectedBroadcastAddress(AddressV4 unicast_address, AddressV4 netmask) noexcept;

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_ADDRESS_UTILS_H_

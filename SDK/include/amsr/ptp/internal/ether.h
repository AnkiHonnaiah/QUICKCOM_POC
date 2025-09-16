/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  This file implements ethernet address conversion API's.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_ETHER_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_ETHER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/array.h"
#include "amsr/core/string.h"
#include "amsr/ptp/internal/os_specific_constants.h"
#include "amsr/ptp/internal/types.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief           Converts the Ethernet host address ether_addr to a string in standard hex-digits-and-colons
 *                  notation.
 * \param[in]       ether_addr Ethernet address in binary form.
 * \param[out]      address_text Ethernet address in standard hex-digits-and-colons notation.
 * \return          -
 *
 * \context         Any
 *
 * \pre             -
 *
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          FALSE
 *
 * \trace
 *
 * \vprivate        Vector component internal API
 * \spec
 *   requires true;
 * \endspec
 */
void EtherNtoA(ether_addr const* ether_addr, amsr::core::Array<char, kMaxEtherAddressString>& address_text);

/*!
 * \brief           Converts the 48-bit Ethernet address from ASCII representation in the standard hex-digits-and-colons
 *                  notation into binary data.
 * \param[in]       asc         Ethernet address string in standard hex-digits-and-colons notation.
 * \param[in]       ether_addr  Buffer to hold binary data.
 * \return          Converted ether_addr if the input is valid, otherwise nullptr.
 *
 * \context         Any
 *
 * \pre             -
 *
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          FALSE
 *
 * \trace
 *
 * \vprivate        Vector component internal API
 * \spec
 *   requires true;
 * \endspec
 */
ether_addr* EtherAtoN(ara::core::StringView asc, ether_addr* ether_addr);

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_ETHER_H_

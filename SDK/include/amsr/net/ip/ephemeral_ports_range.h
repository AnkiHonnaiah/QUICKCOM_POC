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
 *        \brief  Getter for Ephemeral Ports range defined by Operating System.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_EPHEMERAL_PORTS_RANGE_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_EPHEMERAL_PORTS_RANGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/result.h"

#include "amsr/net/internal/os_interface.h"
#include "amsr/net/internal/types.h"
#include "amsr/net_utils/network_stack_id.h"

namespace amsr {
namespace net {
namespace ip {

/*!
 * \brief           Pull EphemeralPortsRange into a namespace visible to the user.
 *
 */
using EphemeralPortsRange = net::internal::EphemeralPortsRange;

/*!
 * \brief           Returns the ephemeral ports range defined by operating system.
 * \details         As no ports from ephemeral ports range should be configured
 *                  by user and should only used by OS, user might need to check this range
 *                  first before configuring ports for applications
 *
 * \param[in]       netstack_id   Network stack to query for the ephemeral ports range. Defaults to the default network
 *                                stack.
 *
 * \return          The ephemeral ports range defined by Operating System.
 *
 * \error osabstraction::OsabErrc::kSystemEnvironmentError     Error while reading the values from OS.
 * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
 *
 *
 * \context         ANY
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \trace           DSGN-Osab-EphemeralPortsRangeCheck
 * \vprivate        Vector product internal API
 */
inline ::amsr::core::Result<EphemeralPortsRange> GetEphemeralPortsRange(
    net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) {
  return amsr::net::internal::os_interface::GetEphemeralPortsRange(netstack_id);
}

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_EPHEMERAL_PORTS_RANGE_H_

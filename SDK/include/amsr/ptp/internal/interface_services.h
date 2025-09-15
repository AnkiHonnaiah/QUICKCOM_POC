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
 *        \brief  OS-specific services to handle network interfaces.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_INTERFACE_SERVICES_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_INTERFACE_SERVICES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/net_utils/network_stack_id.h"
#include "amsr/ptp/ethernet_address.h"
#include "amsr/ptp/interface.h"
#include "amsr/ptp/internal/types.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief           Finds a network interface that is identified by the supplied name.
 *
 * \details         Acquires the index of a network interface from the name of the interface.
 *
 * \param[in]       name                      The name to be used.
 * \param[in]       netstack_id               ID of the network stack used to resolve the name to the index. Only used
 *                                            on QNX.
 *
 * \return          The index of the interface that is identified by that name.
 *
 * \error           OsabErrc::kSystemEnvironmentError                         No interface identified by that name
 *                                                                            found.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
::amsr::core::Result<InterfaceIndex> FindInterfaceIndexByName(::amsr::core::StringView name,
                                                              net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief           Extracts the MAC address assigned to a network interface.
 *
 * \param[in]       interface                 The network interface object.
 * \param[in]       netstack_id               ID of the network stack used extract the address. Only used on QNX.
 *
 * \return          The address of the desired type assigned to the network interface.
 *
 * \error           OsabErrc::kUnexpected                               Some internal state that should not arise was
 *                                                                      detected.
 * \error           OsabErrc::kInsufficientPrivileges                   The process has insufficient privileges to
 *                                                                      extract the assigned address.
 * \error           OsabErrc::kAddressError                             The index does not identify a network interface.
 * \error           OsabErrc::kAddressError                             Interface does not have an IP address of the
 *                                                                      desired family assigned.
 * \error           OsabErrc::kResource                                 Not enough system resources to extract the
 *                                                                      address.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
auto ExtractAddress(Interface const& interface, net_utils::NetworkStackId const& netstack_id) noexcept
    -> ::amsr::core::Result<EthernetAddress>;

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_INTERFACE_SERVICES_H_

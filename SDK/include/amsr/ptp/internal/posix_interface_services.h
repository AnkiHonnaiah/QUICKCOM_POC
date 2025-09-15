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
 *        \brief  Implements POSIX specific interface services.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_POSIX_INTERFACE_SERVICES_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_POSIX_INTERFACE_SERVICES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/net_utils/network_stack_id.h"
#include "amsr/ptp/internal/types.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief Translates a network interface name to the index of the respective network interface.
 *
 * \param[in]    name The name of the network interface. Only the first kMaxNetworkInterfaceName characters are needed.
 * \param[in]    netstack_id ID of the network stack used to resolve the name to the index. Only used on QNX.
 *
 * \return The index of the network interface.
 *
 * \error osabstraction::OsabErrc::kSystemEnvironmentError   No interface identified by the supplied name has been
 *                                                           found.
 *
 * \context         ANY
 *
 * \pre             name must be at least one character long.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<InterfaceIndex> NameToIndex(::amsr::core::StringView name,
                                                 net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief Acquires a list of interface addresses.
 *
 * \details
 * After the list has been processed, the object output by a call to this function must be passed into
 * FreeInterfaceAddresses, the list the object encapsulates must not be accessed afterwards.
 *
 * \param[in]    netstack_id ID of the network stack used to get the interface information. Only used on QNX.
 *
 * \return The list of interface addresses.
 *
 * \error osabstraction::OsabErrc::kResource                Not enough system resources to acquire the list of
 *                                                          addresses.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges to acquire the
 *                                                          network interface names.
 * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<InterfaceAddrView> GetInterfaceAddresses(net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief Deallocates a list of interface addresses.
 *
 * \details
 * The list encapsulated by the input object must not be accessed after this function has been called.
 *
 * \param[in]    address_list The list that shall be deallocated.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
void FreeInterfaceAddresses(InterfaceAddrView address_list) noexcept;

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_POSIX_INTERFACE_SERVICES_H_

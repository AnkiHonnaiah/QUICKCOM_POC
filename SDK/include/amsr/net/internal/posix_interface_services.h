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
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_POSIX_INTERFACE_SERVICES_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_POSIX_INTERFACE_SERVICES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/net/internal/interface_address_view.h"
#include "amsr/net/internal/types.h"
#include "amsr/net_utils/network_stack_id.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Translates a network interface index to the name of the respective network interface.
 *
 * \param[in]    index       The index of the network interface.
 * \param[in]    netstack_id The ID of the network stack used to resolve the index to the name. Only used on QNX.
 * \param[out]   name_buffer Buffer to write the \0-terminated name string into.
 *
 * \return Number of characters the name consists of (terminating \0 character not counted).
 *
 * \error osabstraction::OsabErrc::kSystemEnvironmentError   No interface identified by the supplied index has been
 *                                                           found.
 *
 * \context         ANY
 *
 * \pre             name_buffer must be able to hold at least kMaxNetworkInterfaceName + 1 characters.
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-InterfaceTranslation
 */
::amsr::core::Result<::amsr::core::Span<char>::size_type> IndexToName(InterfaceIndex index,
                                                                      net_utils::NetworkStackId const& netstack_id,
                                                                      ::amsr::core::Span<char> name_buffer) noexcept;

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
 *
 * \trace           DSGN-Osab-InterfaceTranslation
 */
::amsr::core::Result<InterfaceIndex> NameToIndex(::amsr::core::StringView name,
                                                 net_utils::NetworkStackId const& netstack_id) noexcept;

/*!
 * \brief Acquires a list of interface addresses.
 *
 *
 * \param[in]    netstack_id ID of the network stack used to get the interface information. Only used on QNX.
 *
 * \return The list of interface addresses.
 *
 * \error osabstraction::OsabErrc::kResource                Not enough system resources to acquire the list of
 *                                                          addresses.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges to acquire the
 *                                                          network interface addresses.
 * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-InterfaceTranslation
 */
::amsr::core::Result<InterfaceAddressView> GetInterfaceAddresses(net_utils::NetworkStackId const& netstack_id) noexcept;

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_POSIX_INTERFACE_SERVICES_H_

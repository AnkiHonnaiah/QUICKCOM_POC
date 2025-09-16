/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  APIs to create PTP hardware clocks.
 *
 *      \details  Integrates customer provided PTP hardware clock implementations into OsAbstraction.
 *         \unit  amsr::ptp::PhcExtensionPoint
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_TIME_PTP_HW_CLOCK_EXTENSION_INTEGRATION_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_TIME_PTP_HW_CLOCK_EXTENSION_INTEGRATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/net_utils/network_stack_id.h"
#include "osabstraction/time/clock_interface.h"

namespace amsr {
namespace ptp {
namespace time {

/*!
 * \brief Create a PTP hardware clock that uses the OsAbstraction default implementation.
 *
 * \param[in] clock_id          Identifier of the PHC. On Linux this string is a path to a character device that can be
 *                              used as clock (e.g /dev/ptp0). On QNX this string is the name of the interface whose PHC
 *                              shall be used as clock. This function only checks if the device can be opened. It does
 *                              not check if the opened file descriptor may be used as clock. This check will be done on
 *                              the first Now() call.
 * \param[in] network_stack_id  Identifier of the network stack that manages the interface to which the PHC belongs.
 *                              Only used on QNX, defaults to default network stack ID.
 *
 * \return Created clock (never nullptr)
 *
 * \error  osabstraction::OsabErrc::kApiError               Dynamic clock is not implemented by OsAbstraction for this
 *                                                          operating system.
 * \error  osabstraction::OsabErrc::kSystemEnvironmentError Creating the dynamic clock failed.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          FALSE
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
auto CreateDefaultPtpHwClock(
    ::amsr::core::StringView clock_id,
    amsr::net_utils::NetworkStackId const& network_stack_id = amsr::net_utils::NetworkStackId{}) noexcept
    -> ::amsr::core::Result<std::unique_ptr<::osabstraction::time::ClockInterface>>;

/*!
 * \brief Create a PTP hardware clock that uses a customer provided implementation.
 *
 * \param[in] clock_id          Identifier of the PHC.
 * \param[in] network_stack_id  Identifier of the network stack that manages the interface to which the PHC belongs.
 *
 * \return Created clock (never nullptr)
 *
 * \error  osabstraction::OsabErrc::kApiError               Customer did not provide a PTP hardware clock implementation
 *                                                          or the customer provided PTP hardware clock implementation
 *                                                          does not support the specified PHC.
 * \error  osabstraction::OsabErrc::kSystemEnvironmentError Creating the PTP hardware clock clock failed.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          FALSE
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
auto CreateCustomPtpHwClock(::amsr::core::StringView clock_id, amsr::net_utils::NetworkStackId const& network_stack_id =
                                                                   amsr::net_utils::NetworkStackId{}) noexcept
    -> ::amsr::core::Result<std::unique_ptr<::osabstraction::time::ClockInterface>>;

}  // namespace time
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_TIME_PTP_HW_CLOCK_EXTENSION_INTEGRATION_H_

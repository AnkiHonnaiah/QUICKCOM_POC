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
 *        \brief  Maps the internal errors for the basic socket class.
 *        \unit   osabstraction::net
 *
 *      \details  Maps the errors for every basic socket service request.
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_SOCKET_ERRORS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_SOCKET_ERRORS_H_

#include "amsr/core/error_code.h"
#include "amsr/core/result.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Filters fatal errors for a reactor registration.
 *
 * \param[in] error Error returns by osabstraction::io::reactor1::Register()
 *
 * \return osabstraction::OsabErrc::kResource   No memory to register another callback or system limit reached.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode FilterFatalReactorRegistrationErrors(::amsr::core::ErrorCode const& error) noexcept;

/*!
 * \brief Filters fatal errors for setting a callback for a registered file descriptor.
 *
 * \param[in] error Error returns by osabstraction::io::reactor1::SetCallbackTarget()
 *
 * \note As all errors are considered fatal, this function never returns.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
void FilterFatalReactorSetCallbackTargetErrors(::amsr::core::ErrorCode const& error) noexcept;

/*!
 * \brief Filters fatal errors for adding or removing monitored events.
 *
 * \param[in] error Error returned by osabstraction::io::reactor1::AddMonitoredEvents() or
 *                  osabstraction::io::reactor1::RemoveMonitoredEvents()
 *
 * \note As all errors are considered fatal, this function never returns.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
void FilterFatalReactorChangeMonitoredEventsErrors(::amsr::core::ErrorCode const& error) noexcept;

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_BASIC_SOCKET_ERRORS_H_

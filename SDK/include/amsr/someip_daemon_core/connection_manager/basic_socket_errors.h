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
 *      \details  Maps the errors for every basic socket service request.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_BASIC_SOCKET_ERRORS_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_BASIC_SOCKET_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Filters fatal errors for a reactor registration
 * \param[in]       error
 *                  Error returned by osabstraction::io::reactor1::Register()
 * \param[in]       logger
 *                  A logger to log print a log message in case of valid errors that may be reported
 * \return          osabstraction::OsabErrc::kResource No memory to register another callback or system limit reached.
 * \context         Reactor
 * \pre             -
 * \threadsafe      TRUE
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \steady          FALSE
 */
ara::core::ErrorCode FilterFatalReactorRegistrationErrors(
    ara::core::ErrorCode const& error, someip_daemon_core::logging::AraComLogger const& logger) noexcept;

/*!
 * \brief           Filters fatal errors for setting a callback for a registered file descriptor
 * \param[in]       error
 *                  Error returned by osabstraction::io::reactor1::SetCallbackTarget()
 * \context         Reactor
 * \pre             -
 * \threadsafe      FALSE
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \steady          FALSE
 */
[[noreturn]] void FilterFatalReactorSetCallbackTargetErrors(ara::core::ErrorCode const& error) noexcept;

/*!
 * \brief           Filters fatal errors for adding or removing monitored events
 * \param[in]       error
 *                  Error returned by osabstraction::io::reactor1::AddMonitoredEvents() or
 *                  osabstraction::io::reactor1::RemoveMonitoredEvents()
 * \context         Reactor
 * \pre             -
 * \threadsafe      FALSE
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \steady          FALSE
 */
[[noreturn]] void FilterFatalReactorChangeMonitoredEventsErrors(ara::core::ErrorCode const& error) noexcept;

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_BASIC_SOCKET_ERRORS_H_

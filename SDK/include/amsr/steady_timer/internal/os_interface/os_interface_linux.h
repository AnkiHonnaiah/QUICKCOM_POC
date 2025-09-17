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
 *        \brief  Provides interface for linux specific operating system APIs used in TimerManager.
 *         \unit  amsr::SteadyTimer
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_LINUX_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_LINUX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/steady_timer/internal/timer_os_types.h"

namespace amsr {
namespace steady_timer {
namespace internal {
namespace os_interface {

/*!
 * \brief            Creates a timer.
 * \param[in]        clock_id       Clock to use for time measurements.
 *
 * \return           Id of created timer.
 *
 * \error            osabstraction::OsabErrc::kUnexpected               Unexpected error during creation of timer.
 * \error            osabstraction::OsabErrc::kInsufficientPrivileges   Missing capabilities to create timer with
 *                                                                      specified clock id.
 * \error            osabstraction::OsabErrc::kSystemEnvironmentError   Could not mount (internal) anonymous device
 *                                                                      when creating timer fd (Linux-only).
 * \error            osabstraction::OsabErrc::kApiError                 Invalid clock.
 * \error            osabstraction::OsabErrc::kResource                 Insufficient resources to create timer.
 * \context          Initialization
 * \pre              -
 * \reentrant        FALSE
 * \synchronous      TRUE
 * \threadsafe       FALSE
 * \steady           TRUE
 */
::amsr::core::Result<TimerId> TimerCreate(TimerClock clock_id) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_LINUX_H_

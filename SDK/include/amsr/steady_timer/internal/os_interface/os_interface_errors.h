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
 *        \brief  Provides error mappings for Linux operating system APIs used in TimerManager.
 *         \unit  amsr::SteadyTimer
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_ERRORS_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "osabstraction/internal/errors.h"

namespace amsr {
namespace steady_timer {
namespace internal {
namespace os_interface {
/*!
 * \brief Maps timer creation error to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error during creation of timer.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Missing capabilities to create timer with specified clock
 *                                                            id.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Could not mount (internal) anonymous device when creating
 *                                                            timer fd.
 * \retval osabstraction::OsabErrc::kApiError                 Invalid input parameters provided.
 * \retval osabstraction::OsabErrc::kResource                 Insufficient resources to create timer.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 */
auto MapTimerFdCreateError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Handles an error when arming a timer.
 *
 * \param[in] number  Error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 */
void MapTimerFdSettimeError(osabstraction::internal::OsErrorNumber number) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_ERRORS_H_

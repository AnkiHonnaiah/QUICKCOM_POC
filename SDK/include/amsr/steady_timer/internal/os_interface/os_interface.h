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
 *        \brief  Provides an interface for operating system APIs used in TimerManager.
 *         \unit  amsr::SteadyTimer
 *
 *      \details -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/steady_timer/internal/timer_os_types.h"
#include "amsr/steady_timer/types.h"

namespace amsr {
namespace steady_timer {
namespace internal {
namespace os_interface {

/*!
 * \brief            Arms a timer to expire at the specified time.
 * \param[in]        timer_id   The id of the timer.
 * \param[in]        one_shot   The time point at which the timer shall expire.
 * \context          Initialization
 * \pre              -
 * \reentrant        FALSE
 * \synchronous      TRUE
 * \threadsafe       FALSE
 * \steady           TRUE
 * \spec
 *   requires true;
 * \endspec
 */
void TimerSettime(TimerId timer_id, TimePoint one_shot) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_OS_INTERFACE_OS_INTERFACE_H_

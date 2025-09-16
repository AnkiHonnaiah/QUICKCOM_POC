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
 *        \brief  Defines generic types used for TimerManager.
 *         \unit  amsr::SteadyTimer
 *
 *      \details -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace steady_timer {
namespace internal {

/*!
 * \brief State of the timer.
 */
enum class TimerState : std::uint8_t {
  /*!
   * \brief   Running.
   * \details Timer was added to the TimerManager's TimerHeap and its next_expiry_ != kInfinite.
   */
  kRunning,
  /*!
   * \brief   In callback mode.
   * \details Timer was removed from the TimerManager's TimerHeap.
   *          In the callback, the timer can be stopped or restarted. If nothing is done in the callback, the timer is
   *          stopped/restarted depending on whether it is periodic.
   */
  kCallback,
  /*!
   * \brief   Stopped.
   * \details Timer was removed from the TimerManager's TimerHeap and its next_expiry_ == kInfinite.
   */
  kStopped
};

}  // namespace internal
}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_TYPES_H_

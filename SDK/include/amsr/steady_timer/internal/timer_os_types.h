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
 *        \brief  Defines OS specific types used for TimerManager.
 *         \unit  amsr::SteadyTimer
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_OS_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_OS_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace steady_timer {
namespace internal {

// VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_LinuxApi
/*! \brief Timer Id type. */
struct TimerId {
  /*! \brief Timer Id. */
  int timer_id;
};

// VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_LinuxApi
/*! \brief Clock Id type. */
struct TimerClock {
  /*! \brief Clock Id. */
  int clock_id;
};

}  // namespace internal
}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_OS_TYPES_H_

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
 *        \brief  OS specific time related types.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_TIME_OS_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_TIME_OS_TYPES_H_

#include <ctime>

namespace osabstraction {
namespace time {

/*!
 * \brief Encloses a clock id.
 *
 * The clock ID can be used to construct a clock. Some clock identifier may have a relation to a file descriptor.
 * However, a clock ID can only be used to create a clock. It must not be mixed up with native handles that may be used
 * during construction of dynamic clocks.
 */
struct ClockId {
  /*!
   * \brief Clock id.
   */
  clockid_t clock_id;
};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_library_api
/*!
 * \brief Defines a real time clock.
 *
 * \details
 * The clock returns the "wall" time. The values represents the time passed since Epoch.
 * This clock may be affected by discontinuous jumps (e.g. if the clock is manually changed) and by incremental
 * adjustments.
 * This clock does count the time that the system is suspended.
 */
constexpr ClockId const kClockRealtime{CLOCK_REALTIME};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_library_api
/*!
 * \brief Defines a monotonic clock.
 *
 * \details
 * The clock returns the monotonic time. The values represents the time passed since an unspecified point in the past.
 * The point will be defined during system start-up and cannot be adjusted later. It may be different from the
 * kClockMonotonicRaw start point.
 * This clock is not affected by discontinuous jumps (e.g. if the clock is manually changed) but may be affected by
 * incremental adjustments.
 * This clock does not count the time that the system is suspended.
 */
constexpr ClockId const kClockMonotonic{CLOCK_MONOTONIC};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_library_api
/*!
 * \brief Defines a monotonic clock that provides access to a raw hardware-based time.
 *
 * \details
 * The clock returns the raw hardware-based monotonic time with the highest resolution possible on the operating
 * system. The values represents the time passed since an unspecified point in the past. The point will be defined
 * during system start-up and cannot be adjusted later. It may be different from the kClockMonotonic start point.
 * This clock is not affected by discontinuous jumps (e.g. if the clock is manually changed) and is not affected by
 * incremental adjustments.
 * It is unspecified if the clock counts the time that the system is suspended.
 */
constexpr ClockId const kClockMonotonicRaw{CLOCK_MONOTONIC_RAW};

}  // namespace time
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_TIME_OS_TYPES_H_

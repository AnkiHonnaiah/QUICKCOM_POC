/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file
 *      \brief      Header file providing time information.
 *      \details    -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TIME_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>  // IWYU pragma: export
#include <cstdint>
#include "amsr/log/internal/timestamp_clock.h"

namespace amsr {
namespace log {
namespace internal {

/*!
 * \copydoc amsr::log::internal::clock::TimestampClock
 */
using TimestampClock = clock::TimestampClock;

/*!
 * \brief The time point of a TimestampClock.
 */
using TimestampTimePoint = typename clock::TimestampClock::time_point;

/*! \brief A high resolution clock. */
using HighResClock = std::chrono::high_resolution_clock;

/*! \brief A time point using the high resolution clock. */
using HighResTimePoint = std::chrono::time_point<HighResClock>;

/*! \brief The type used for DLT time stamps. */
using DltTime = std::uint64_t;

/*!
 * \brief Get the DLT timestamp for the current time.
 * \details Is specified as uint32 starting from system startup & with 0.1ms resolution.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 * \return      The current time.
 */
inline auto GetDltTimeStamp() noexcept -> DltTime {
  /*!
   * \brief The internally used clock.
   */
  using Clock = std::chrono::steady_clock;
  /*!
   * \brief The ration describing nanoseconds.
   */
  using DltRatio = std::nano;
  /*!
   * \brief   The internal timestamp type.
   * \details Internal type is 64bit, which is enough to represent 584 years in nanoseconds.
   *          Conversion to either 0.1 ms or [seconds, nanoseconds] in the LogDaemon depending on its runtime config.
   */
  using TimeStamp = std::chrono::duration<DltTime, DltRatio>;

  return std::chrono::duration_cast<TimeStamp>(Clock::now().time_since_epoch()).count();
}

/*!
 * \brief Converts the duration into an integer of microseconds.
 *
 * \param duration The duration to convert.
 * \return         A number signifying the number of microseconds for the duration.
 * \spec
 *   requires true;
 * \endspec
 */
inline auto DurationAsMicroseconds(TimestampClock::duration duration) -> std::uint64_t {
  using Us = std::chrono::microseconds;
  Us::rep const t{std::chrono::duration_cast<Us>(duration).count()};
  return static_cast<std::uint64_t>(t);
}
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TIME_H_

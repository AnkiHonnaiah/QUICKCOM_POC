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
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <chrono>
#include "osabstraction/time/types.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace steady_timer {

/*!
 * \brief Behavior how to handle missed timeouts of an overrun periodic timer that were not already handled by the
 *        callback.
 */
enum class MissedTimeoutBehavior : std::uint8_t {
  /*!
   * \brief   Queue missed timeouts.
   * \details If multiple timeouts are missed, during the next Reactor1::HandleEvents call the callback is
   *          executed once for each missed timeout until the next expiry time of the timer reaches a point in the
   *          future.
   */
  kQueueMissedTimeouts,

  /*!
   * \brief   Discard missed timeouts.
   * \details If multiple timeouts are missed, during the next Reactor1::HandleEvents call the callback is only
   *          executed once. The next expiry time of the timer is set to a point in the future.
   */
  kDiscardMissedTimeouts
};

/*!
 * \brief           Timeout callback to be executed upon timer expiration.
 *
 * \details         All TimerInterface APIs, including destructor are allowed to be called from a TimeoutCallback
 *                  callback function.
 *                  Destroying the TimerManagerInterface that manages this timer will lead to undefined behavior.
 *
 * \context         Reactor1::HandleEvents()
 *
 * \pre             -
 * \post            -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-TimerSupport
 */
using TimeoutCallback = vac::language::UniqueFunction<void(void)>;

/*!
 * \brief Timeout value to represent infinite wait time.
 */
constexpr osabstraction::time::TimeStamp::StdChronoTimeType const kInfinity{
    osabstraction::time::TimeStamp::StdChronoTimeType::max()};

/*! \brief Duration type. */
struct Duration {
  /*! \brief Duration timestamp. */
  osabstraction::time::TimeStamp duration;

  // VECTOR NC VectorC++-V11.0.3: MD_OSA_V11.0.3_TimerManager_TimeStampStruct
  /*!
   * \brief           Constructor.
   * \param[in]       time   Timestamp.
   * \context         ANY
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Duration(osabstraction::time::TimeStamp const time) noexcept : duration{time} {}

  // VECTOR NC VectorC++-V11.0.3: MD_OSA_V11.0.3_TimerManager_TimeStampStruct
  /*!
   * \brief           Constructor.
   * \param[in]       time   Time in nanoseconds.
   * \context         ANY
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Duration(osabstraction::time::TimeStamp::StdChronoTimeType const time) noexcept
      : Duration{osabstraction::time::TimeStamp{time}} {}
};

/*!
 * \brief           Equal operator.
 * \param[in]       lhs   The first duration to compare.
 * \param[in]       rhs The second duration to compare.
 * \return          True if durations are equal, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
bool operator==(Duration const& lhs, Duration const& rhs) noexcept;

/*!
 * \brief           Not equal operator.
 * \param[in]       lhs   The first duration to compare.
 * \param[in]       rhs The second duration to compare.
 * \return          True if durations are not equal, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \vprivate        Vector product internal API
 */
bool operator!=(Duration const& lhs, Duration const& rhs) noexcept;

/*! \brief Timepoint type. */
struct TimePoint {
  /*! \brief Timepoint timestamp. */
  osabstraction::time::TimeStamp time_point;

  // VECTOR NC VectorC++-V11.0.3: MD_OSA_V11.0.3_TimerManager_TimeStampStruct
  /*!
   * \brief           Constructor.
   * \param[in]       time   Timestamp.
   * \context         ANY
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  explicit TimePoint(osabstraction::time::TimeStamp const time) noexcept : time_point{time} {}

  // VECTOR NC VectorC++-V11.0.3: MD_OSA_V11.0.3_TimerManager_TimeStampStruct
  /*!
   * \brief           Constructor.
   * \param[in]       time   Time in nanoseconds.
   * \context         ANY
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  explicit TimePoint(osabstraction::time::TimeStamp::StdChronoTimeType const time) noexcept
      : TimePoint{osabstraction::time::TimeStamp{time}} {}
};

/*!
 * \brief           Equal operator.
 * \param[in]       lhs The first time point to compare.
 * \param[in]       rhs The second time point to compare.
 * \return          True if time points are equal, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
bool operator==(TimePoint const& lhs, TimePoint const& rhs) noexcept;

/*!
 * \brief           Not equal operator.
 * \param[in]       lhs The first time point to compare.
 * \param[in]       rhs The second time point to compare.
 * \return          True if time points are different, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \vprivate        Vector product internal API
 */
bool operator!=(TimePoint const& lhs, TimePoint const& rhs) noexcept;

/*!
 * \brief           Greater equals than operator.
 * \param[in]       lhs The first time point to compare.
 * \param[in]       rhs The second time point to compare.
 * \return          True if rhs time point is greater or equal lhs time point, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \spec
 *   requires true;
 * \endspec
 * \vprivate        Vector product internal API
 */
bool operator>=(TimePoint const& lhs, TimePoint const& rhs) noexcept;

/*!
 * \brief           Greater than operator.
 * \param[in]       lhs The first time point to compare.
 * \param[in]       rhs The second time point to compare.
 * \return          True if rhs time point is greater lhs time point, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \spec
 *   requires true;
 * \endspec
 * \vprivate        Vector product internal API
 */
bool operator>(TimePoint const& lhs, TimePoint const& rhs) noexcept;

/*!
 * \brief           Less equals than operator.
 * \param[in]       lhs The first time point to compare.
 * \param[in]       rhs The second time point to compare.
 * \return          True if rhs time point is smaller or equal lhs time point, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \spec
 *   requires true;
 * \endspec
 * \vprivate        Vector product internal API
 */
bool operator<=(TimePoint const& lhs, TimePoint const& rhs) noexcept;

/*!
 * \brief           Less than operator.
 * \param[in]       lhs The first time point to compare.
 * \param[in]       rhs The second time point to compare.
 * \return          True if rhs time point is smaller lhs time point, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \spec
 *   requires true;
 * \endspec
 * \vprivate        Vector product internal API
 */
bool operator<(TimePoint const& lhs, TimePoint const& rhs) noexcept;

/*!
 * \brief           Plus operator.
 * \param[in]       lhs The first time point to sum up.
 * \param[in]       rhs The second time point to sum up.
 * \return          Sum of: lhs + rhs.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \spec
 *   requires true;
 * \endspec
 * \vprivate        Vector product internal API
 */
TimePoint operator+(TimePoint const& lhs, Duration const& rhs) noexcept;

/*!
 * \brief           Plus assignment operator.
 * \param[in,out]   lhs The first time point to sum up.
 * \param[in]       rhs The second time point to sum up.
 * \return          Sum of: lhs + rhs.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \spec
 *   requires true;
 * \endspec
 * \vprivate        Vector product internal API
 */
TimePoint& operator+=(TimePoint& lhs, Duration const& rhs) noexcept;

/*!
 * \brief           Minus operator.
 * \param[in]       lhs The first time point.
 * \param[in]       rhs The second time point to subtract from lhs.
 * \return          Subtraction of: lhs - rhs.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \spec
 *   requires true;
 * \endspec
 * \vprivate        Vector product internal API
 */
Duration operator-(TimePoint const& lhs, TimePoint const& rhs) noexcept;

/*!
 * \brief           Minus assignment operator.
 * \param[in,out]   lhs The first time point.
 * \param[in]       rhs The second time point to subtract from lhs.
 * \return          Subtraction of: lhs - rhs.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \steady          TRUE
 * \spec
 *   requires true;
 * \endspec
 * \vprivate        Vector product internal API
 */
TimePoint& operator-=(TimePoint& lhs, TimePoint const& rhs) noexcept;

}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TYPES_H_

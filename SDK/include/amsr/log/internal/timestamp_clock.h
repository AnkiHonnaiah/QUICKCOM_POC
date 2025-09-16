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
/**     \file       timestamp_clock.h
 *      \brief      Header file providing clock information.
 *      \details    -
 *
 *      \unit       LogAndTrace::Common::Clock
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TIMSTAMP_CLOCK_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TIMSTAMP_CLOCK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>  // IWYU pragma: export
#include "amsr/log/clock_type.h"
#include "vac/language/switch.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Forward declaration to amsr::log::internal::LoggerBase
 */
class LoggerBase;

namespace clock {
namespace detail {
/*!
 * \brief   Empty struct to be used to map ClockType enum to the C++ standard library clocks.
 *
 * \tparam  kClockType   ClockType enum to be used to map a C++ clock.
 */
template <ClockType kClockType>
struct StdClock {};

/*!
 * \brief   Specialization of StdClock. Maps ClockType::kSystem to std::chrono::system_clock.
 */
template <>
struct StdClock<ClockType::kSystem> {
  using type = std::chrono::system_clock;
};

/*!
 * \brief   Specialization of StdClock. Maps ClockType::kSteady to std::chrono::steady_clock.
 */
template <>
struct StdClock<ClockType::kSteady> {
  using type = std::chrono::steady_clock;
};

/*!
 * \brief   Specialization of StdClock. Maps ClockType::kHighResolution to std::chrono::high_resolution_clock.
 */
template <>
struct StdClock<ClockType::kHighResolution> {
  using type = std::chrono::high_resolution_clock;
};

/*!
 * \brief   Alias to the get the C++ standard library clock type.
 *
 * \tparam  kClockType   ClockType enum to be used to map a C++ clock.
 */
template <ClockType kClockType>
using StdClockT = typename StdClock<kClockType>::type;
}  // namespace detail

/*!
 * \brief   Configurable Clock for log timestamps.
 *
 * \details Can be configured to use either std::chrono::system_clock, std::chrono::steady_clock,
 *          or std::chrono::high_resolution_clock.
 *          Shall be configured once, during initialization of Log.
 *          Meets the C++ Named requirement 'Clock'.
 */
class TimestampClock final {
 private:
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_LOG_AutosarC++17_10-A11.3.1_friend
  /*!
   * \brief   Friend class declaration for LoggerBase.
   *
   * \details Only loggerbase shall be allowed to change TimestampClock::clock_type_ during initialization of Log.
   */
  friend class amsr::log::internal::LoggerBase;

  /*!
   * \brief   Flag for internal clock type.
   *
   * \details Shall only be set during initialization by LoggerBase.
   */
  static ClockType clock_type_;

 public:
  using duration = std::chrono::nanoseconds;
  using rep = duration::rep;
  using period = duration::period;
  using time_point = std::chrono::time_point<TimestampClock, duration>;

  /*!
   * \brief   Flag indicates if the clock is steady.
   *          The value depends on the C++ clocks in the standard library.
   *          Does not indicate if the configured clock is steady. Therefore, shall not be used.
   */
  static bool constexpr is_steady{false};

  /*!
   * \brief   Gets the current time.
   *
   * \return  A time point converted from either std::chrono::system_clock, std::chrono::steady clock or
   *          std::chrono::high_resolution_clock depending on configuration.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static auto now() noexcept -> time_point {
    time_point result{};
    if (clock_type_ == ClockType::kSteady) {
      result = TimePointFrom<ClockType::kSteady>();
    } else if (clock_type_ == ClockType::kSystem) {
      result = TimePointFrom<ClockType::kSystem>();
    } else {
      result = TimePointFrom<ClockType::kDefault>();  // The case for ClockType::kHighResolution
    }
    return result;
  }

  /*!
   * \brief   Gets the configured clock type.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static auto GetClockType() -> ClockType { return clock_type_; }

 private:
  /*!
   * \brief   Returns the current time of a given C++ clock type.
   *
   * \tparam  kClockType   The clock type to use.
   *                       Any enumeration of the amsr::log::internal::ClockType.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  template <ClockType kClockType>
  static auto TimePointFrom() noexcept -> time_point {
    using Clock = detail::StdClockT<kClockType>;
    return time_point(std::chrono::duration_cast<duration>(Clock::now().time_since_epoch()));
  }
};

}  // namespace clock
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TIMESTAMP_CLOCK_H_

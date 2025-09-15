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
 *        \brief  Common time related types.
 *        \unit   osabstraction::time_CommonClocks_Linux
 *        \unit   osabstraction::time_CommonClocks_PikeOS
 *        \unit   osabstraction::time_CommonClocks_QNX
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_TYPES_H_

#include <chrono>
#include <ctime>

#include "amsr/core/abort.h"

namespace osabstraction {
namespace time {

/*!
 * \brief Stores the time passed since an unspecified point in the past.
 *
 * The point in the past depends on the used clock (for example system start-up or Epoch).
 *
 * \vpublic
 */
class TimeStamp final {
 public:
  /*!
   * \brief Used chrono type.
   *
   * \vpublic
   */
  using StdChronoTimeType = std::chrono::nanoseconds;

  /*!
   * \brief Count type underlying the chrono type.
   *
   * \vpublic
   */
  using StdChronoCountType = StdChronoTimeType::rep;

  // VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_PosixApi
  /*!
   * \brief The operating system native implementation of the POSIX type struct timespec.
   *
   * \vpublic
   */
  using OsTimeType = struct timespec;

  /*!
   * \brief Constructs a time stamp with zero time.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  TimeStamp() noexcept = default;

  /*!
   * \brief Constructs a time stamp from std::chrono::nanoseconds.
   *
   * \param[in]       time   Time as std::chrono::nanoseconds.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  explicit TimeStamp(StdChronoTimeType time) noexcept : time_in_ns_{time} {}

  /*!
   * \brief Constructs a time stamp from a OS specific timespec structure.
   *
   * \param[in]       time   Time as OS specific timespec structure.
   *                         The NativeTypeType structure must be valid (tv_sec >= 0, tv_nsec [0, 999999999]).
   *
   * \context         ANY
   *
   * \pre             The resulting time stamp must not overflow the std::chrono::nanoseconds type.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   * - Check if the members of the OsTimeType are valid.
   *    - Valid tv_sec  values are >= 0.
   *    - Valid tv_nsec values are [0, 999999999].
   * - Convert the tv_sec member of OsTimeType into nanoseconds and check if it is smaller than the maximum
   *   representable duration of std::chrono::nanoseconds (No overflow).
   * - Convert the tv_sec member of OsTimeType into nanoseconds and check if it, and the member tv_nsec together
   *   are smaller than the maximum representable duration of std::chrono::nanoseconds (No overflow).
   * - If any of the checks fail Abort.
   * - Else perform the conversion.
   *   \endinternal
   */
  explicit TimeStamp(OsTimeType time) noexcept : TimeStamp() {
    constexpr std::chrono::nanoseconds kNsecPerSec{std::chrono::seconds(1)};

    if (((time.tv_sec < 0) || (time.tv_nsec < 0) || (time.tv_nsec >= kNsecPerSec.count()))) {
      ::amsr::core::Abort(
          "Invalid timespec. The members of the timespec structure must be greater than zero and the member "
          "tv_nsec must be smaller than 999999999.");
    } else if (((std::chrono::nanoseconds::max() / kNsecPerSec) < time.tv_sec)) {
      ::amsr::core::Abort("Timestamp conversion would overflow.");
    } else if (((std::chrono::nanoseconds::max() - std::chrono::nanoseconds{time.tv_nsec}) <
                std::chrono::seconds{time.tv_sec})) {
      ::amsr::core::Abort("Timestamp conversion would overflow.");
    } else {  // If the members of NativeTimeTyp are valid and no overflow occurs, perform the conversion.
      time_in_ns_ = std::chrono::nanoseconds{time.tv_nsec} + std::chrono::seconds{time.tv_sec};
    }
  }

  /*!
   * \brief Copy constructor
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  TimeStamp(TimeStamp const&) noexcept = default;

  /*!
   * \brief Move constructor
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  TimeStamp(TimeStamp&&) noexcept = default;

  /*!
   * \brief Assigns a time stamp.
   *
   * \return Reference to this object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(TimeStamp const&) & noexcept -> TimeStamp& = default;

  /*!
   * \brief Move operator.
   *
   * \return Reference to this object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(TimeStamp&&) & noexcept -> TimeStamp& = default;

  // VECTOR Next Construct AutosarC++17_10-M0.1.8:  MD_OSA_M0.1.8_DefaultedDestructor
  /*!
   * \brief Defaulted destructor
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ~TimeStamp() noexcept = default;

  /*!
   * \brief Returns the passed time in nanoseconds.
   *
   * \return          The passed time in nanoseconds.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       TRUE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-TimeStamp
   *
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetNanoseconds() const noexcept -> StdChronoTimeType { return time_in_ns_; }

 private:
  /*!
   * \brief Stores the time passed since a point in the past.
   */
  StdChronoTimeType time_in_ns_{};
};

}  // namespace time
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_TYPES_H_

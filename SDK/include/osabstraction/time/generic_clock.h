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
 *        \brief  Defines generic clocks that are present on all operating systems.
 *        \unit   osabstraction::time_CommonClocks_Linux
 *        \unit   osabstraction::time_CommonClocks_QNX
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_TIME_GENERIC_CLOCK_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_TIME_GENERIC_CLOCK_H_

#include "amsr/core/result.h"
#include "osabstraction/time/clock_interface.h"
#include "osabstraction/time/os_types.h"
#include "osabstraction/time/types.h"

namespace osabstraction {
namespace time {

/*!
 * \brief A generic clock takes a clock id and creates the clock.
 *
 * The generic clock can be used for a monotonic, raw monotonic or a real time clock.
 */
class GenericClock : public ClockInterface {
 public:
  /*!
   * \brief Constructs a generic clock from a passed clock ID.
   *
   * \param[in] id Valid clock ID.
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
   * \trace           DSGN-Osab-GenericClock
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  explicit GenericClock(ClockId id) noexcept : ClockInterface{}, clock_id_{id} {}

  /*!
   * \brief Returns the passed time.
   *
   * \details
   * Returns the time passed since an unspecified point in the past.  The point in the past depends on the used clock
   * (for example system start-up or Epoch).
   *
   * \return          The passed time.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The clock does not exist (anymore).
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-GenericClock
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_OSA_CALL_OBJ_PASS_VALUE
  auto Now() const noexcept -> ::amsr::core::Result<TimeStamp> override { return Now(clock_id_); }

  /*!
   * \brief Returns the passed time.
   *
   * \details
   * Returns the time passed since an unspecified point in the past.  The point in the past depends on the used clock
   * (for example system start-up or Epoch).
   *
   * \param[in] clock_id Valid clock ID (kClockRealtime, kClockMonotonic or kClockMonotonicRaw).
   *
   * \return          The passed time.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The clock does not exist (anymore).
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-GenericClock
   *
   * \vprivate        Vector product internal API
   */
  static auto Now(ClockId clock_id) noexcept -> ::amsr::core::Result<TimeStamp>;

  /*!
   * \brief Copy constructs a generic clock.
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
   * \trace           DSGN-Osab-GenericClock
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  GenericClock(GenericClock const&) noexcept = default;

  /*!
   * \brief Move constructs a generic clock.
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
   * \trace           DSGN-Osab-GenericClock
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  GenericClock(GenericClock&&) noexcept = default;

  /*!
   * \brief Copy assigns a generic clock.
   *
   * \return
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
   * \trace           DSGN-Osab-GenericClock
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  GenericClock& operator=(GenericClock const&) & noexcept = default;

  /*!
   * \brief Move assigns a generic clock.
   *
   * \return
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
   * \trace           DSGN-Osab-GenericClock
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(GenericClock&&) & noexcept -> GenericClock& = default;

  /*!
   * \brief Destroys the generic clock.
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
   * \trace           DSGN-Osab-GenericClock
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_OSA_MOLE_1298
  ~GenericClock() noexcept override = default;

 private:
  /*!
   * \brief Stores the clock id.
   */
  ClockId clock_id_;
};

}  // namespace time
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_TIME_GENERIC_CLOCK_H_

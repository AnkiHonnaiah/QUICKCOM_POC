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
 *        \brief  Header file for the TimerInterface class.
 *         \unit  amsr::SteadyTimer
 *
 *         \complexity The complexity is increased by many inter-module calls and high call tree complexity.
 *                     The inter-unit calls are different from the initialization and execution phase and are necessary
 *                     for the correct functionality of this unit. Splitting the classes depending on the execution
 *                     state only for the purpose of metrics optimization does not add any benefit and would make the
 *                     design worse. These interdependencies are by design and thoroughly tested.
 *                     Even some functions occur deep in the call tree, they remain easily testable due to their
 *                     inherent simplicity.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/steady_timer/types.h"

namespace amsr {
namespace steady_timer {

/*!
 * \brief         TimerInterface class.
 * \details       Timers can be periodic and/or one-shot.
 */
class TimerInterface {
 public:
  /*!
   * \brief           Destructor.
   * \details         Upon destruction the timer is stopped and deregistered from the TimerManager.
   *                  Destroying the TimerInterface object outside of the callback while the callback is executing is
   *                  not allowed. The TimerInterface object can be safely destroyed in one of the following ways:
   *                  - Using a separate software event reactor job to destroy the TimerInterface object.
   *                  - Ensuring per design that TimerInterface object is destroyed in the same thread where
   *                    Reactor1::HandleEvents() is called.
   *                  - Calling Stop() and then waiting until IsCallbackExecuting() returns false to destroy the
   *                    TimerInterface object.
   *                  - Destroying the TimerInterface object in the TimeoutCallback function.
   * \context         Shutdown
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerMultithreadedSafeDestruction
   * \vprivate        Vector product internal API
   */
  virtual ~TimerInterface() noexcept = default;

  /*!
   * \brief           Returns the time_point at which this timer expires next.
   * \return          The next expiry point if timer was started, kInfinity otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerNextExpiry
   * \vprivate        Vector product internal API
   */
  virtual auto GetNextExpiry() const noexcept -> TimePoint = 0;

  /*!
   * \brief           Starts the timer. The timer can be both one-shot and periodic, purely periodic or just
   *                  one-shot.
   * \details         If the timer has already been scheduled, it will be rescheduled.
   * \param[in]       one_shot One-shot timeout. Value is optional, if not set the timer is only using periodic mode
   *                           (implicit offset is GetClock::Now()).
   *                           For correct timing calculation of absolute timeout values, the user has to use as the
   *                           base time TimerManagerInterface::GetClock::Now() of the TimerManagerInterface that
   *                           this timer object.
   *                           If the one-shot timeout value is in the past, the timeout is automatically normalized to
   *                           the current point in time. This affects also the next periodic expiry time points,
   *                           which are calculated as (one_shot + period).
   * \param[in]       period   Periodic timeout. Value is optional, if not set the timer is only using one-shot mode.
   * \context         ANY
   * \pre             - At least one of the parameters 'one_shot' and 'period' must be set.
   *                  - One-shot timeout must not be 0 or kInfinity.
   *                  - Periodic timeout must not be 0, kInfinity or negative.
   *                  - User must ensure: (TimerManagerInterface::GetClock::Now() + period) do not overflow the
   *                    std::chrono::nanoseconds type (as long as periodic timer is running).
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerStart
   * \vprivate        Vector product internal API
   */
  virtual void Start(amsr::core::Optional<TimePoint> const& one_shot,
                     amsr::core::Optional<Duration> const& period) noexcept = 0;

  /*!
   * \brief           Starts the timer. The timer can be both one-shot and periodic, purely periodic or just
   *                  one-shot.
   * \details         If the timer has already been scheduled, it will be rescheduled.
   * \param[in]       one_shot One-shot timeout. Value is optional, if not set the timer is only using periodic mode
   *                           (implicit offset is GetClock::Now()).
   *                           If the one-shot timeout value is negative (in the past), the timeout is automatically
   *                           normalized to the current point in time. This affects also the next periodic expiry
   *                           time points, which are calculated as (one_shot + period).
   * \param[in]       period   Periodic timeout. Value is optional, if not set the timer is only using one-shot mode.
   * \context         ANY
   * \pre             - At least one of the parameters 'one_shot' and 'period' must be set.
   *                  - One-shot timeout must not be kInfinity.
   *                  - Periodic timeout must not be 0, kInfinity or negative.
   *                  - User must ensure: (TimerManagerInterface::GetClock::Now() + period) and
   *                    (TimerManagerInterface::GetClock::Now() + one_shot) do not overflow the std::chrono::nanoseconds
   *                    type (as long as periodic timer is running).
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerStart
   * \vprivate        Vector product internal API
   */
  virtual void Start(amsr::core::Optional<Duration> const& one_shot,
                     amsr::core::Optional<Duration> const& period) noexcept = 0;

  /*!
   * \brief           Stops the timer.
   * \details         If the TimeoutCallback is currently executing, callback execution will finish. Use
   *                  IsCallbackExecuting() to check when the callback execution finished.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerStop
   * \vprivate        Vector product internal API
   */
  virtual void Stop() noexcept = 0;

  /*!
   * \brief           Returns whether the expiry point of this timer is greater than the current time.
   * \details         If the timer is currently stopped, it will return FALSE. If this API is called from a timeout
   *                  callback, it will compare to the next timer expiration:
   *                   - kInfinity for a one-shot timer
   *                   - current timer expiry time + period for a periodic timer
   * \context         ANY
   * \return          TRUE if the next_expiry_ point is elapsed, FALSE otherwise.
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerIsExpired
   * \vprivate        Vector product internal API
   */
  virtual auto IsExpired() const noexcept -> bool = 0;

  /*!
   * \brief           Returns whether the timer has expired at the given time point.
   * \details         If the timer is currently stopped, it will return FALSE.  If this API is called from a timeout
   *                  callback, it will compare to the next timer expiration:
   *                   - kInfinity for a one-shot timer
   *                   - current timer expiry time + period for a periodic timer
   * \param[in]       at   The time point at which the expiry check is made.
   * \return          TRUE if the next_expiry_ point is elapsed, FALSE otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerIsExpired
   * \vprivate        Vector product internal API
   */
  virtual auto IsExpired(TimePoint const& at) const noexcept -> bool = 0;

  /*!
   * \brief           Convert the next_expiry_ of this timer to a nanoseconds duration.
   * \return          The current next_expiry_ converted to a nanoseconds duration relative to the current time.
   *                  If next_expiry_ is greater than the current time, the result is an empty optional.
   *                  If the timer is currently stopped, the result is the difference between the current time and
   *                  kInfinity.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerToNanoseconds
   * \vprivate        Vector product internal API
   */
  virtual auto ToNanoseconds() const noexcept -> amsr::core::Optional<std::chrono::nanoseconds> = 0;

  /*!
   * \brief           Returns whether the timer callback is currently executing.
   * \details         This API is needed to check that no callback is executing before destructing the timer, Stop()
   *                  must be called first before calling this API. See notes on Destructor for more details.
   * \return          TRUE if the callback is executing, FALSE otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerIsCallbackExecuting
   * \vprivate        Vector product internal API
   */
  virtual auto IsCallbackExecuting() const noexcept -> bool = 0;

 protected:
  /*!
   * \brief Constructor.
   */
  TimerInterface() noexcept = default;
  /*!
   * \brief Default copy constructor.
   */
  TimerInterface(TimerInterface const&) = default;

  /*!
   * \brief Default copy assignment.
   */
  TimerInterface& operator=(TimerInterface const&) & = default;

  /*!
   * \brief Default move constructor.
   */
  TimerInterface(TimerInterface&&) noexcept = default;

  /*!
   * \brief Default move assignment.
   */
  TimerInterface& operator=(TimerInterface&&) & noexcept = default;
};

}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_INTERFACE_H_

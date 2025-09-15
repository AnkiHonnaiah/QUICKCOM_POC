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
 *        \brief  Header file for the Timer class.
 *         \unit  osabstraction::Timer
 *
 *   \complexity  Only minor violation, no action required.
 *
 *      \details  An abstract timer, one-shot or periodic. Users should subclass Timer
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/time.h>
#include <chrono>

#include "amsr/timer/timer_manager.h"

namespace amsr {
namespace timer {

/*!
 * \brief         Base Class representing a timer.
 * \details       Timers can be periodic or one-shot. Create a subclass of Timer to implement a time-based callback.
 *
 * \vprivate      pes internal API
 */
class Timer {
 public:
  /*!
   * \brief    Typedef representing the clock type this timer operates on.
   *           We use steady_clock to avoid forward and backward leaps, causing unwanted expiration by suspend-to-RAM or
   *           by system clock adjustment.
   *
   * \vprivate pes internal API
   */
  using Clock = std::chrono::steady_clock;

  /*!
   * \brief           Constructor taking a timer_manager this Timer is associated to.
   * \param           timer_manager The pointer to the TimerManager to be connected to. The TimerManager has to exist
   *                                for the whole lifetime of this Timer object.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  explicit Timer(TimerManager* timer_manager) noexcept;

  /*!
   * \brief Deleted copy constructor.
   */
  Timer(Timer const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   */
  Timer& operator=(Timer const&) & = delete;

  /*!
   * \brief Deleted move constructor.
   */
  Timer(Timer&&) = delete;

  /*!
   * \brief Deleted move assignment.
   */
  Timer& operator=(Timer&&) & = delete;

  /*!
   * \brief Destructor.
   * \pre  Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *       this TimerManager.
   * \steady          TRUE
   *
   * \vprivate        pes internal API
   */
  virtual ~Timer() noexcept;

  /*!
   * \brief           Returns the time_point at which this timer expires next.
   * \details         The returned value is only valid if the Timer is started.
   * \return          The next expiry point.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  Clock::time_point const& GetNextExpiry() const noexcept { return next_expiry_; }

  /*!
   * \brief           Sets the timer to periodic mode with the given period.
   * \details         The running state of the timer is not modified.
   * \param           period The period at which this timer should fire.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  void SetPeriod(Clock::duration period) noexcept;

  /*!
   * \brief           Sets the timer to one-shot mode with the expiry point.
   * \details         The running state of the timer is not modified.
   * \param           time_point The Time Point at which this timer should fire.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  void SetOneShot(Clock::time_point time_point) noexcept;

  /*!
   * \brief           Sets the timer to one-shot mode with the expiry point relative to the current time.
   * \details         The running state of the timer is not modified.
   * \param           timeout The Time until this timer should fire.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  void SetOneShot(Clock::duration timeout) noexcept;

  /*!
   * \brief           Starts the timer.
   * \details         If the timer has already been scheduled, it will be rescheduled.
   * \context         ANY
   * \pre             Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *                  this TimerManager.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  void Start() noexcept;

  /*!
   * \brief           Stops the timer.
   * \context         ANY
   * \pre             Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *                  this TimerManager.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  void Stop() noexcept;

  /*!
   * \brief           Callback that is activated when the timer expires.
   * \details         This callback can indicate whether the timer should be scheduled again.
   *                  In a multithreaded context, the callback must not start another thread
   *                  that can call timer manager APIs before this callback completes.
   * \return          TRUE if the timer shall be scheduled again, FALSE otherwise.
   *                  Returning true has no effect when the timer is not periodic.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  virtual bool HandleTimer() = 0;

  /*!
   * \brief           Handler method called by the TimerManager.
   * \context         ANY
   * \pre             Must not be executed in context of HandleTimer().
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  void DoHandleTimer() noexcept;

  /*!
   * \brief           Returns whether the expiry point of this timer is greater than the current time.
   * \context         ANY
   * \return          TRUE if the next_expiry_ point is elapsed, FALSE otherwise.
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsExpired() const noexcept;

  /*!
   * \brief           Returns whether the timer has expired at the given time point.
   * \param           at The time point at which the expiry check is made.
   * \return          TRUE if the next_expiry_ point is elapsed, FALSE otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsExpired(Clock::time_point const& at) const noexcept;

  /*!
   * \brief           Convert the next_expiry_ of this timer to a nanoseconds duration.
   * \return          The current next_expiry_ converted to a nanoseconds duration relative to the current time.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  std::chrono::nanoseconds ToNanoseconds() const noexcept;

 private:
  /*!
   * \brief           Reschedules a periodic timer.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        Vector product internal API
   */
  void RestartPeriodic() noexcept;

  /*!
   * \brief Timer managing this timer.
   */
  TimerManager* timer_manager_;
  /*!
   * \brief Flag indicating this is a one time action.
   */
  bool one_shot_{true};
  /*!
   * \brief Interval for periodic timers.
   */
  Clock::duration period_;
  /*!
   * \brief Time for the next expiry.
   */
  Clock::time_point next_expiry_;
};

}  // namespace timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_H_

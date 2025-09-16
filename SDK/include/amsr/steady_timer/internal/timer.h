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
 *         \unit  amsr::SteadyTimer
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/optional.h"
#include "amsr/steady_timer/internal/timer_manager.h"
#include "amsr/steady_timer/internal/timer_types.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/types.h"

namespace amsr {
namespace steady_timer {
namespace internal {

/*!
 * \brief         Timer class.
 * \details       Timers can be periodic and/or one-shot.
 */
class Timer final : public TimerInterface {
 public:
  /*!
   * \brief           Constructor taking a timer_manager this Timer is associated to.
   * \param[in]       timer_manager  The pointer to the TimerManager to be connected to. The TimerManager has to exist
   *                                 for the whole lifetime of this Timer object.
   * \param[in]       callback       The callback function that is to be called when the timer expires.
   *                                 It is possible to restart/stop or destroy the timer in this callback function.
   *                                 The timer_manager must not be destroyed in this callback.
   *                                 If any non-steady API is called in this callback function, then
   *                                 calls to Reactor1::HandleEvents are not steady.
   * \param[in]       missed_timeouts_behavior How missed not handled timeouts of a periodic timer shall be handled.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerSupport
   */
  explicit Timer(std::shared_ptr<TimerManager> timer_manager, TimeoutCallback&& callback,
                 MissedTimeoutBehavior missed_timeouts_behavior) noexcept;

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
   */
  ~Timer() noexcept final;

  /*!
   * \copydoc ::amsr::steady_timer::TimerInterface::GetNextExpiry()
   */
  TimePoint GetNextExpiry() const noexcept final;

  /*!
   * \copydoc ::amsr::steady_timer::TimerInterface::GetNextExpiry()
   * \spec
   *   requires true;
   * \endspec
   */
  TimePoint GetNextExpiryInternal() const noexcept;

  /*!
   * \copydoc ::amsr::steady_timer::TimerInterface::Start()
   */
  void Start(amsr::core::Optional<TimePoint> const& one_shot,
             amsr::core::Optional<Duration> const& period) noexcept final;

  /*!
   * \copydoc ::amsr::steady_timer::TimerInterface::Start()
   */
  void Start(amsr::core::Optional<Duration> const& one_shot,
             amsr::core::Optional<Duration> const& period) noexcept final;

  /*!
   * \copydoc ::amsr::steady_timer::TimerInterface::Stop()
   */
  void Stop() noexcept final;

  /*!
   * \copydoc ::amsr::steady_timer::TimerInterface::IsExpired()
   */
  bool IsExpired() const noexcept final;

  /*!
   * \brief           Returns whether the timer has expired at the given time point.
   * \details         If the timer is currently stopped, it will return FALSE.
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
  bool IsExpired(TimePoint const& at) const noexcept final;

  /*!
   * \copydoc ::amsr::steady_timer::TimerInterface::ToNanoseconds()
   */
  amsr::core::Optional<std::chrono::nanoseconds> ToNanoseconds() const noexcept final;

  /*!
   * \brief           Handler method called by the TimerManager.
   * \details         Checks internally if the timer has expired. If that is the case, calls the callback function.
   *                  Before and after calling the callback, the TimerManager mutex is locked.
   * \param[in]       now The current time.
   * \pre             -
   * \post            -
   * \return          True if the timer has expired and callback was called, false otherwise.
   * \context         ANY
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE if timeout_callback_ is steady.
   * \trace           DSGN-Osab-TimerTimeoutHandling
   * \spec
   *   requires true;
   * \endspec
   */
  bool OnTimeout(TimePoint const& now) noexcept;

  /*!
   * \copydoc ::amsr::steady_timer::TimerInterface::IsCallbackExecuting()
   */
  auto IsCallbackExecuting() const noexcept -> bool final;

 private:
  /*!
   * \brief           Starts the timer. The timer can be both one-shot and periodic, purely periodic or just
   *                  one-shot. At least one of the parameters 'one_shot' and 'period' must be set.
   * \details         If the timer has already been scheduled, it will be rescheduled.
   * \param[in]       one_shot  One-shot timeout. Value is optional, if not set the timer is only using periodic mode.
   *                            Value must not be 0 or kInfinity.
   * \param[in]       period    Periodic timeout. Value is optional, if not set the timer is only using one-shot mode.
   *                            Value must not be 0 or kInfinity.
   *                            User must ensure: (GetClock::Now() + period) does not overflow the
   *                            std::chrono::nanoseconds type. (as long as periodic timer is running)
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   */
  void StartInternal(amsr::core::Optional<TimePoint> const& one_shot,
                     amsr::core::Optional<Duration> const& period) noexcept;

  /*!
   * \brief           Stops the timer.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   */

  void StopInternal() noexcept;

  /*!
   * \brief           Returns whether the timer has expired at the given time point.
   * \details         If the timer is currently stopped, it will return FALSE.
   * \param           at The time point at which the expiry check is made.
   * \return          TRUE if the next_expiry_ point is elapsed, FALSE otherwise.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   */
  bool IsExpiredInternal(TimePoint const& at) const noexcept;

  /*!
   * \brief           Assert that the provided timeouts are valid.
   * \param[in]       one_shot  One-shot timeout. Value is optional, if not set the timer is only using periodic mode.
   *                            Value must not be 0 or kInfinity.
   * \param[in]       period    Periodic timeout. Value is optional, if not set the timer is only using one-shot mode.
   *                            Value must not be 0 or kInfinity.
   * \context         ANY
   * \pre             -
   * \reentrant       TRUE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   */
  static void AssertValidTimeout(amsr::core::Optional<TimePoint> const& one_shot,
                                 amsr::core::Optional<Duration> const& period) noexcept;

  /*!
   * \brief Timer manager managing this timer.
   */
  std::shared_ptr<TimerManager> timer_manager_;

  /*!
   * \brief Shared ptr to keep track of timer object lifetime.
   */
  std::shared_ptr<bool> lifetime_ptr_;

  /*!
   * \brief Interval for periodic timers.
   */
  ::amsr::core::Optional<Duration> period_;

  /*!
   * \brief Time point of the next expiry.
   */
  TimePoint next_expiry_;

  /*!
   * \brief Store the current state of the timer.
   */
  TimerState state_{TimerState::kStopped};

  /*!
   * \brief Function to call when the timer expires.
   */
  TimeoutCallback timeout_callback_;

  /*!
   * \brief   Store how to handle missed timeouts.
   */
  MissedTimeoutBehavior missed_timeouts_behavior_;

  /*!
   * \brief   Boolean indicating whether there is currently a callback in progress.
   */
  bool user_callback_executing_{false};
};

/*!
 * \brief           Checks if two timers are equal.
 * \param[in]       a Timer to be checked for equality.
 * \param[in]       b Timer to be checked for equality.
 * \return          True if both timers are equal, false otherwise.
 * \context         ANY
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 * \trace           DSGN-Osab-TimerSupport
 */
bool operator==(Timer const& a, Timer const& b) noexcept;

}  // namespace internal
}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_H_

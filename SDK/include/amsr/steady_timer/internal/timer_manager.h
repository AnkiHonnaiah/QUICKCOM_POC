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
 *        \brief  Header file for TimerManager.
 *         \unit  amsr::SteadyTimer
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_MANAGER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>  // std::shared_ptr, std::unique_ptr
#include <mutex>   // std::mutex
#include "amsr/steady_timer/internal/timer_heap.h"
#include "amsr/steady_timer/internal/timer_os_types.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/steady_timer/types.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/io/reactor1/types.h"
#include "osabstraction/time/generic_clock.h"
#include "osabstraction/time/os_types.h"
#include "osabstraction/time/types.h"

namespace amsr {
namespace steady_timer {
namespace internal {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_OSA_A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief   An event manager for Timer objects.
 */
class TimerManager final : public TimerManagerInterface, public std::enable_shared_from_this<TimerManager> {
 public:
  /*!
   * \brief           Constructor for a TimerManager linked to a given reactor.
   * \param[in]       reactor The reactor's lifetime should fully cover the lifetime period of the TimerManager
   *                          object. If the reactor object is not thread-safe then it should not be changed throughout
   *                          all the lifetime of the TimerManager object.
   * \param[in]       clock   The clock id to use. Must be one of the following clocks: monotonic, raw monotonic or a
   *                          real-time clock.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerSupport
   */
  explicit TimerManager(osabstraction::io::reactor1::Reactor1Interface& reactor,
                        osabstraction::time::ClockId clock) noexcept;

  /*!
   * \brief Deleted copy constructor.
   */
  TimerManager(TimerManager const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   */
  TimerManager& operator=(TimerManager const&) & = delete;

  /*!
   * \brief Deleted move constructor.
   */
  TimerManager(TimerManager&&) = delete;

  /*!
   * \brief Deleted move assignment.
   */
  TimerManager&& operator=(TimerManager&&) & = delete;

  /*!
   * \brief           Destructor.
   * \context         Shutdown
   * \pre             Deinitialize() has been called.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerDestroyTimerManager
   */
  ~TimerManager() noexcept final;

  /*!
   * \copydoc ::amsr::steady_timer::TimerManagerInterface::GetClock()
   * \spec
   *   requires true;
   * \endspec
   */
  osabstraction::time::GenericClock& GetClock() noexcept final { return clock_; }

  /*!
   * \copydoc ::amsr::steady_timer::TimerManagerInterface::CreateTimer()
   */
  auto CreateTimer(TimeoutCallback&& callback, MissedTimeoutBehavior missed_timeouts_behavior) noexcept
      -> std::unique_ptr<TimerInterface> final;

  /*!
   * \brief           Initializes the TimerManager.
   * \details         Creates a timer and registers the timer to the reactor.
   * \return          Valid result on success.
   *
   * \error            osabstraction::OsabErrc::kUnexpected               Unexpected error during creation of timer.
   * \error            osabstraction::OsabErrc::kInsufficientPrivileges   Missing privileges to create a timer.
   * \error            osabstraction::OsabErrc::kSystemEnvironmentError   Could not mount (internal) anonymous device
   *                                                                      when creating timer fd (Linux-only).
   * \error            osabstraction::OsabErrc::kApiError                 Invalid clock id.
   * \error            osabstraction::OsabErrc::kResource                 Insufficient resources to create timer or no
   *                                                                      memory to register another reactor callback.
   *
   * \context         Initialization
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> Initialize() noexcept;

  /*!
   * \copydoc ::amsr::steady_timer::TimerManagerInterface::Deinitialize()
   */
  void Deinitialize() noexcept final;

  /*!
   * \brief           Removes a slot from the Timer container.
   * \details         Before and after calling this function the mutex must be locked.
   * \context         ANY
   * \pre             -
   * \post            -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \spec
   *   requires true;
   * \endspec
   */
  void DeallocateTimer() noexcept;

  /*!
   * \brief           Add a timer to be considered when computing the next expiry.
   * \details         Must not be called multiple times for the same timer.
   *                  Before and after calling this function the mutex must be locked.
   * \param           timer   Timer object.
   * \context         ANY
   * \pre             -
   * \post            -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \spec
   *   requires true;
   * \endspec
   */
  void AddTimer(Timer& timer) noexcept;

  /*!
   * \brief           Remove a timer to be managed.
   * \details         Before and after calling this function the mutex must be locked.
   * \param           timer   Timer object.
   * \context         ANY
   * \pre             -
   * \post            -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \spec
   *   requires true;
   * \endspec
   */
  void RemoveTimer(Timer& timer) noexcept;

  /*!
   * \brief           Remove the first timer from the timers.
   * \details         Before and after calling this function the mutex must be locked.
   * \context         ANY
   * \pre             -
   * \post            -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \spec
   *   requires true;
   * \endspec
   */
  void PopTimer() noexcept;

  /*!
   * \brief           Reevaluates all currently managed timers.
   * \details         If the next expiry is changed, the OS timer is rearmed.
   *                  Before and after calling this function the mutex must be locked.
   * \pre             -
   * \post            -
   * \context         ANY
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \spec
   *   requires true;
   * \endspec
   */
  void UpdateTimers() noexcept;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_nonconst_ptr
  /*!
   * \brief           Get the mutex.
   * \details         The mutex is shared between the TimerManager and its managed Timers. This method allows the
   *                  Timers to (un)lock the mutex.
   * \return          The mutex.
   * \context         ANY
   * \reentrant       TRUE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   */
  std::mutex& GetMutex() noexcept { return mutex_; }

  /*!
   * \brief           Get the current time.
   * \return          Current time point in nanoseconds.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  TimePoint Now() const noexcept;

 private:
  /*!
   * \brief           Checks if the OS timer needs to be rearmed and if so, rearms it.
   * \context         ANY
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   */
  void CheckRearmOsTimer() noexcept;

  /*!
   * \brief           Handles all expired timers.
   * \details         Invokes callbacks of expired timers and rearms the OS timer if necessary.
   * \param[in]       callback_handle   Handle of the registered Reactor callback that was called.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE if all expired timers' timeout_callback_ are steady.
   * \trace           DSGN-Osab-TimerSupport
   * \spec
   *   requires true;
   * \endspec
   */
  void HandleTimerExpiry(osabstraction::io::reactor1::CallbackHandle callback_handle) noexcept;

  /*!
   * \brief The reactor that is linked to the timer manager.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief The set of timers to consider for firing.
   */
  TimerHeap timers_;

  /*!
   * \brief The configured clock used by this timer manager.
   */
  TimerClock clock_id_;

  /*!
   * \brief The configured clock used by this timer manager.
   */
  osabstraction::time::GenericClock clock_;

  /*!
   * \brief The callback handle of the timer manager reactor event handler.
   */
  osabstraction::io::reactor1::CallbackHandle timer_callback_handle_;

  /*!
   * \brief Point in time the first timer will expire.
   */
  TimePoint next_expiry_;

  /*!
   * \brief Timer id.
   */
  amsr::core::Optional<TimerId> timer_id_{};

  /*!
   * \brief   The mutex of the lock.
   */
  mutable std::mutex mutex_{};

  /*!
   * \brief   Flag whether callback is currently executing or not.
   */
  bool in_callback_{false};
};

/*!
 * \exclusivearea ::amsr::steady_timer::internal::TimerManager::mutex_
 * This mutex is shared for TimerManager and all of its managed Timer objects.
 * Serializes all TimerManager and Timer API calls from different threads to ensure consistent (re)arming of the system
 * timer with the correct expiry time.
 * \protects ::amsr::steady_timer::internal::TimerManager::timers_
 *           ::amsr::steady_timer::internal::TimerManager::next_expiry_
 *           ::amsr::steady_timer::internal::TimerManager::in_callback_
 *           ::amsr::steady_timer::internal::Timer::state_
 *           ::amsr::steady_timer::internal::Timer::next_expiry_
 *           ::amsr::steady_timer::internal::Timer::period_
 *           ::amsr::steady_timer::internal::Timer::lifetime_ptr_
 *           ::amsr::steady_timer::internal::Timer::user_callback_executing_
 * \usedin   ::amsr::steady_timer::internal::TimerManager::CreateTimer
 *           ::amsr::steady_timer::internal::TimerManager::Deinitialize
 *           ::amsr::steady_timer::internal::TimerManager::HandleTimerExpiry
 *           ::amsr::steady_timer::internal::TimerManager::~TimerManager
 *           ::amsr::steady_timer::internal::Timer::Start
 *           ::amsr::steady_timer::internal::Timer::Stop
 *           ::amsr::steady_timer::internal::Timer::GetNextExpiry
 *           ::amsr::steady_timer::internal::Timer::IsExpired
 *           ::amsr::steady_timer::internal::Timer::ToNanoseconds
 *           ::amsr::steady_timer::internal::Timer::OnTimeout
 *           ::amsr::steady_timer::internal::Timer::IsCallbackExecuting
 *           ::amsr::steady_timer::internal::Timer::~Timer
 * \exclude  ::amsr::steady_timer::internal::TimerManager::GetClock
 *           ::amsr::steady_timer::internal::TimerManager::Initialize
 *           ::amsr::steady_timer::internal::TimerManager::DeallocateTimer
 *           ::amsr::steady_timer::internal::TimerManager::AddTimer
 *           ::amsr::steady_timer::internal::TimerManager::RemoveTimer
 *           ::amsr::steady_timer::internal::TimerManager::PopTimer
 *           ::amsr::steady_timer::internal::TimerManager::UpdateTimers
 *           ::amsr::steady_timer::internal::TimerManager::GetMutex
 *           ::amsr::steady_timer::internal::TimerManager::CheckRearmOsTimer
 *           ::amsr::steady_timer::internal::TimerManager::Now
 *           ::amsr::steady_timer::internal::Timer::StartInternal
 *           ::amsr::steady_timer::internal::Timer::StopInternal
 *           ::amsr::steady_timer::internal::Timer::IsExpiredInternal
 *           ::amsr::steady_timer::internal::Timer::GetNextExpiryInternal
 * \length   LONG The lock is held when accessing the timers_ and when reading/updating the next_expiry_.
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_MANAGER_H_

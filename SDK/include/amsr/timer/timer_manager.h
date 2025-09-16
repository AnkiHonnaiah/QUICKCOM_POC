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
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_MANAGER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/time.h>
#include <chrono>
#include <cstddef>
#include <queue>
#include <utility>

#include "ara/core/vector.h"

#include "amsr/timer/timer_reactor_interface.h"

namespace amsr {
namespace timer {

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Timeout value to repsent infinite wait time.
 */
constexpr std::chrono::nanoseconds const kInfinity{std::chrono::nanoseconds::max()};

class Timer;

/*!
 * \brief  Comparison operator that compares Timer pointers based on their expiry timestamps.
 * \param  a Left timer.
 * \param  b Right timer.
 * \return True, in case a expires after b. False otherwise.
 * \steady          TRUE
 * \trace  DSGN-Osab-TimerSupport
 * \spec
 *   requires true;
 * \endspec
 */
bool TimerExpiryCompare(Timer const* a, Timer const* b) noexcept;

/*!
 * \brief    An event manager for Timer objects.
 * \details  Not threadsafe. Use ThreadSafeTimerManager if multiple threads can interact with timers.
 *
 * \vprivate pes internal API
 */
class TimerManager {
 public:
  /*!
   * \brief           Constructor for a TimerManager linked to a given reactor.
   * \details         If the TimerManager is created with a valid ReactorInterface pointer, TimerManager will wake up
   *                  the Reactor by calling its API Unblock() whenever the following condition applies:
   *                  - a new timer is added with an expiry time earlier than the earliest timer or
   *                  - a first timer is added to the TimerManager.
   *                  It is done to ensure that the Reactor::HandleEvents() returns and can be called again with the
   *                  updated timeout.
   * \param           reactor The pointer to the reactor for constructing the TimerManager.May be nullptr.
   *                          The reactor's lifetime should fully cover the lifetime period of the TimerManager object.
   *                          If the reactor object is not thread-safe then it should not be changed throughout
   *                          all the lifetime of the TimerManager object.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  explicit TimerManager(TimerReactorInterface* reactor) noexcept;

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
   * \brief Default destructor.
   *
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate        pes internal API
   */
  virtual ~TimerManager() noexcept = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief           Add a timer to be considered when computing the next expiry.
   * \details         Adding the same timer more than once will not create duplicates.
   * \param           timer Pointer to a Timer object.
   *                        Given pointer has to be a valid timer object.
   *                        nullptr has special semantics in GetNextTimer().
   *                        The Timer object's lifetime should fully cover the period till its timeout expiration.
   *                        If the Timer object is not thread-safe then it should not be changed till its handling is
   *                        done.
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
  virtual void AddTimer(Timer* timer) noexcept;

  /*!
   * \brief           Remove a timer to be manged.
   * \param           timer Pointer to a Timer object.
   * \context         ANY
   * \pre             Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *                  this TimerManager.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void RemoveTimer(Timer const* timer) noexcept;

  /*!
   * \brief           Return timer expiry duration in nanoseconds for the next expiring timer.
   * \return          Expriry duration in nanoseconds (relative to the current time) or std::chrono::nanoseconds::max()
   *                  if no timer has been added.
   * \context         ANY
   * \pre             Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *                  this TimerManager.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  virtual std::chrono::nanoseconds GetNextExpiry() const noexcept;

  /*!
   * \brief           Trigger handling of expired timers.
   * \details         This function, if not implemented by the user, may call the user callback HandleTimer().
   * \context         ANY
   * \pre             Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *                  this TimerManager.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  virtual void HandleTimerExpiry() noexcept;

  /*!
   * \brief           Heapify and update the order (smallest expiry time first) of the Timer objects in the queue.
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
  void Update() noexcept;

  /*!
   * \brief           Determine whether there are any timers currently running on this TimerManager.
   * \context         ANY
   * \return          TRUE if there is any timer running, FALSE otherwise.
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
  bool empty() const noexcept { return timers_.empty(); }

  /*!
   * \brief           Returns the number of timers currently running on this TimerManager.
   * \context         ANY
   * \return          number of timers currently running on this TimerManager.
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
  std::size_t size() const noexcept { return timers_.size(); }

 private:
  /*!
   * \brief           Returns the next Timer that will fire.
   * \context         ANY
   * \return          pointer to the next Timer that will fire.
   *                  nullptr if no timer registered.
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        Vector product internal API
   */
  Timer* GetNextTimer() const noexcept;

  /*!
   * \brief Container for storing active timers.
   */
  using TimerContainer = ara::core::Vector<Timer*>;

  /*!
   * \brief The reactor which is linked to the timer manager.
   */
  TimerReactorInterface* reactor_;

  /*!
   * \brief The set of timers to consider for firing.
   */
  TimerContainer timers_;
};

}  // namespace timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_MANAGER_H_

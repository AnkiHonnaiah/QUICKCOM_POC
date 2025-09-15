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
/*!        \file  threadsafe_timer_manager.h
 *        \brief  Header file for the ThreadSafeTimerManager class.
 *        \unit   osabstraction::Timer
 *
 *      \details  This is a thread-safe version of the TimerManager class.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_THREADSAFE_TIMER_MANAGER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_THREADSAFE_TIMER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>
#include "amsr/timer/timer_manager.h"

namespace amsr {
namespace timer {

class Timer;

/*!
 * \brief    A thread-safe timer manager.
 *
 * \vprivate pes internal API
 */
class ThreadSafeTimerManager : public TimerManager {
 public:
  /*!
   * \brief           Constructor for a ThreadSafeTimerManager linked to a given reactor.
   * \param           reactor The pointer to the reactor for constructing the TimerManager. May be nullptr.
   *                          The reactor's lifetime should fully cover the lifetime period of the TimerManager object.
   *                          If the reactor object is not thread-safe then it should not be changed throughout
   *                          all the lifetime of the TimerManager object.
   * \details         If the TimerManager is created with a valid ReactorInterface pointer, TimerManager will wake up
   *                  the Reactor by calling its API Unblock() whenever the following condition applies:
   *                  - a new timer is added with an expiry time earlier than the earliest timer or
   *                  - a first timer is added to the TimerManager.
   *                  It is done to ensure that the Reactor::HandleEvents() returns and can be called again with the
   *                  updated timeout.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  explicit ThreadSafeTimerManager(TimerReactorInterface* reactor) noexcept;

  /*!
   * \brief Deleted copy constructor.
   */
  ThreadSafeTimerManager(ThreadSafeTimerManager const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return
   */
  ThreadSafeTimerManager& operator=(ThreadSafeTimerManager const&) & = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ThreadSafeTimerManager(ThreadSafeTimerManager&&) = delete;

  /*!
   * \brief Deleted move assignment.
   * \return
   */
  ThreadSafeTimerManager&& operator=(ThreadSafeTimerManager&&) & = delete;

  /*!
   * \brief     Destructor.
   *
   * \steady    FALSE
   * \vprivate  pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ~ThreadSafeTimerManager() noexcept override = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief           Add a timer to be considered when computing the next expiry.
   * \details         This function may block and may call Unblock() of the reactor passed during construction.
   *                  If the user implements its own Unblock(), he must not call the HandleTimerExpiry() there.
   * \param           timer Pointer to a Timer object.
   *                        The Timer object's lifetime should fully cover the period till its timeout expiration.
   *                        If the Timer object is not thread-safe then it should not be changed till its handling is
   *                        done. Given pointer has to be a valid timer object. nullptr has
   *                        special semantics in GetNextTimer().
   * \context         ANY
   * \pre             Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *                  this TimerManager.
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE if Unblock() does not call HandleTimerExpiry().
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  void AddTimer(Timer* timer) noexcept override;

  /*!
   * \brief           No longer consider a timer when computing the next expiry.
   * \details         This function may block.
   * \param           timer Pointer to a Timer object.
   * \context         ANY
   * \pre             Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *                  this TimerManager.
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  void RemoveTimer(Timer const* timer) noexcept override;

  /*!
   * \brief           Return timer expiry duration in nanoseconds for the next expiring timer.
   * \details         This function may block.
   * \return          Expriry duration in nanoseconds (relative to the current time) or std::chrono::nanoseconds::max()
   *                  if no timer has been added.
   * \context         ANY
   * \pre             Must not be called with a lock held that is also acquired during HandleTimer() of any Timer of
   *                  this TimerManager.
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  std::chrono::nanoseconds GetNextExpiry() const noexcept override;

  /*!
   * \brief           Trigger handling of expired timers.
   * \details         This function may block and may call the user callback HandleTimer(). The user must not create any
   *                  other thread that is calling ThreadSafeTimerManager APIs in the callback.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \threadsafe      TRUE if the user does create threads calling blocking APIs in the callback.
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerSupport
   * \vprivate        pes internal API
   */
  void HandleTimerExpiry() noexcept override;

 private:
  /*!
   * \brief Mutex to protect operations on the list of timers.
   */
  mutable std::recursive_mutex timer_list_mutex_;
};

}  // namespace timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_THREADSAFE_TIMER_MANAGER_H_

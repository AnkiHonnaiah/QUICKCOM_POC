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
 *        \brief  Header file for TimerManagerInterface.
 *         \unit  amsr::SteadyTimer
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_MANAGER_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/steady_timer/timer_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/time/generic_clock.h"
#include "osabstraction/time/os_types.h"

namespace amsr {
namespace steady_timer {

/*!
 * \brief   An event manager for Timer objects.
 */
class TimerManagerInterface {
 public:
  /*!
   * \brief           Default destructor.
   * \pre             Deinitialize() has been called.
   * \steady          FALSE
   * \context         Shutdown
   * \trace           DSGN-Osab-TimerDestroyTimerManager
   * \vprivate        Vector product internal API
   */
  virtual ~TimerManagerInterface() noexcept = default;

  /*!
   * \brief           Creates a timer.
   * \param[in]       callback       The callback function that is to be called when the timer expires.
   *                                 It is possible to restart/stop or destroy the timer in this callback function.
   *                                 This timer manager object must not be destroyed in this callback.
   *                                 If any non-steady API is called in this callback function, then
   *                                 calls to Reactor1::HandleEvents are not steady.
   * \param[in]       missed_timeouts_behavior How missed not handled timeouts of a periodic timer shall be handled.
   *
   * \return          A unique pointer to the created timer.
   * \context         Initialization
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerCreateTimer
   * \vprivate        Vector product internal API
   */
  virtual auto CreateTimer(TimeoutCallback&& callback, MissedTimeoutBehavior missed_timeouts_behavior) noexcept
      -> std::unique_ptr<TimerInterface> = 0;

  /*!
   * \brief           Returns the used clock.
   * \return          The used clock.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \trace           DSGN-Osab-TimerClock
   * \vprivate        Vector product internal API
   */
  virtual auto GetClock() noexcept -> osabstraction::time::GenericClock& = 0;

  /*!
   * \brief           Deinitializes the TimerManager.
   * \details         Deregisters the timer from the reactor and deletes the timer.
   *
   * \context         Shutdown
   * \pre             All timers managed by this timer manager have been stopped or destroyed.
   * \post            No timer managed by this timer manager can be started or stopped anymore.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerDeinitializeTimerManager
   */
  virtual void Deinitialize() noexcept = 0;

 protected:
  /*!
   * \brief Default copy constructor.
   */
  TimerManagerInterface(TimerManagerInterface const&) noexcept = default;

  /*!
   * \brief Default copy assignment.
   */
  TimerManagerInterface& operator=(TimerManagerInterface const&) & noexcept = default;

  /*!
   * \brief Default move constructor.
   */
  TimerManagerInterface(TimerManagerInterface&&) noexcept = default;

  /*!
   * \brief Default move assignment.
   */
  TimerManagerInterface& operator=(TimerManagerInterface&&) & noexcept = default;
  /*!
   * \brief Default constructor.
   */
  TimerManagerInterface() = default;
};

}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_MANAGER_INTERFACE_H_

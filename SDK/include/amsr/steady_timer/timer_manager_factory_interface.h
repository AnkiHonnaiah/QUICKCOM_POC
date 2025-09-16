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
 *        \brief  Factory interface for TimerManagerInterface.
 *         \unit  amsr::SteadyTimer
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_MANAGER_FACTORY_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_MANAGER_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/steady_timer/timer_manager_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/time/os_types.h"

namespace amsr {
namespace steady_timer {

/*!
 * \brief   Factory for TimerManagerInterface objects.
 */
class TimerManagerFactoryInterface {
 public:
  /*!
   * \brief           Function to create objects which implements TimerManagerInterface.
   * \param[in]       reactor The reactor's lifetime should fully cover the lifetime period of the
   *                          TimerManagerInterface object.
   * \param[in]       clock   The clock id to use. Must be one of the following clocks: kClockRealtime, kClockMonotonic.
   * \return          A shared pointer to an object which implements TimerManagerInterface on success. Otherwise, an
   *                  error is returned.
   *
   * \error           osabstraction::OsabErrc::kUnexpected               Unexpected error occurred.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges   Missing privileges to create a timer.
   * \error            osabstraction::OsabErrc::kSystemEnvironmentError  Could not mount (internal) anonymous device
   *                                                                     when creating timer fd (Linux-only).
   * \error           osabstraction::OsabErrc::kApiError                 Invalid clock id.
   * \error           osabstraction::OsabErrc::kResource                 Insufficient resources to create a timer or no
   *                                                                     memory to register another reactor callback.
   *
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-TimerCreateTimerManager
   * \vprivate        Vector product internal API
   */
  virtual auto CreateTimerManager(osabstraction::io::reactor1::Reactor1Interface& reactor,
                                  osabstraction::time::ClockId clock) const noexcept
      -> ::amsr::core::Result<std::shared_ptr<TimerManagerInterface>> = 0;

  /*!
   * \brief Default destructor.
   * \steady          FALSE
   * \vprivate        Vector product internal API
   */
  virtual ~TimerManagerFactoryInterface() noexcept = default;

 protected:
  /*!
   * \brief Default copy constructor.
   */
  TimerManagerFactoryInterface(TimerManagerFactoryInterface const&) = default;

  /*!
   * \brief Default move constructor.
   */
  TimerManagerFactoryInterface(TimerManagerFactoryInterface&&) noexcept = default;

  /*!
   * \brief Default copy assignment.
   */
  TimerManagerFactoryInterface& operator=(TimerManagerFactoryInterface const&) & noexcept = default;

  /*!
   * \brief Default move assignment.
   */
  TimerManagerFactoryInterface& operator=(TimerManagerFactoryInterface&&) & noexcept = default;

  /*!
   * \brief Default constructor.
   */
  TimerManagerFactoryInterface() = default;
};

}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_TIMER_MANAGER_FACTORY_INTERFACE_H_

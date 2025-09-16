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
/*!        \file  timer_reactor_interface.h
 *        \brief  Defines the interface between the timer manager and the reactor.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_REACTOR_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_REACTOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace amsr {
namespace timer {

/*!
 * \brief    Interface for a class which wants to be informed about timer expiry events.
 *
 * \vprivate pes internal API
 */
class TimerReactorInterface {
 public:
  /*!
   * \brief       Default Constructor.
   *
   * \steady      TRUE
   * \vprivate    pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  TimerReactorInterface() noexcept = default;

  /*!
   * \brief       Destructor.
   *
   * \steady      FALSE
   * \vprivate    pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~TimerReactorInterface() noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_OSA_M9.3.3_logical_const_reactor
  /*!
   * \brief           Unblocks the reactor.
   * \details         For details see \ref osabstraction::io::reactor1::Reactor1Interface::Unblock()
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        pes internal API
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void Unblock() noexcept = 0;

 protected:
  /*!
   * \brief Default Copy Constructor.
   */
  TimerReactorInterface(TimerReactorInterface const&) noexcept = default;

  /*!
   * \brief Default Copy Assignment.
   */
  TimerReactorInterface& operator=(TimerReactorInterface const&) & noexcept = default;

  /*!
   * \brief Default Move Constructor.
   */
  TimerReactorInterface(TimerReactorInterface&&) noexcept = default;

  /*!
   * \brief Default Move Assignment.
   */
  TimerReactorInterface& operator=(TimerReactorInterface&&) & noexcept = default;
};

}  // namespace timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_TIMER_TIMER_REACTOR_INTERFACE_H_

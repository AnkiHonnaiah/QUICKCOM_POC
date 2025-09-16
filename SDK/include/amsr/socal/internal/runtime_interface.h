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
/*!        \file  runtime_interface.h
 *        \brief  Runtime interface definition.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_RUNTIME_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_RUNTIME_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/steady_timer/timer_manager_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Interface to access the Socal runtime APIs.
 *
 * \unit Socal::Runtime::Runtime
 */
class RuntimeInterface {
 public:
  /*!
   * \brief Default constructor.
   * \steady FALSE
   */
  RuntimeInterface() noexcept = default;

  RuntimeInterface(RuntimeInterface const&) = delete;
  RuntimeInterface(RuntimeInterface&&) = delete;
  RuntimeInterface& operator=(RuntimeInterface const&) & = delete;
  RuntimeInterface& operator=(RuntimeInterface&&) & = delete;

  /*!
   * \brief Get the steady timer manager.
   * \return Reference to the steady timer manager.
   *         The returned reference is valid as long as Socal is still initialized.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual ::amsr::steady_timer::TimerManagerInterface& GetSteadyTimerManager() const noexcept = 0;

  /*!
   * \brief Get a reference to the reactor.
   * \return Reference to the reactor.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual ::osabstraction::io::reactor1::Reactor1Interface& GetReactor() noexcept = 0;

 protected:
  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RuntimeInterface() noexcept = default;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_RUNTIME_INTERFACE_H_

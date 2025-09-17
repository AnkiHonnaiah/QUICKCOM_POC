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
/*!        \file  required_service_instance_registry.h
 *        \brief  This file contains the definition of the RequiredServiceInstanceRegistry class.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TIMER_MANAGER_TIMER_MANAGER_WRAPPER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TIMER_MANAGER_TIMER_MANAGER_WRAPPER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/steady_timer/timer_manager_factory_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace timer_manager {

/*!
 * \brief A wrapper unit around the steady timer manager
 * \details Manages the lifecycle of the timer manager, as well as its correct deinitialization
 */
class TimerManagerWrapper final {
 public:
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_SomeIpDaemon_AutosarC++17_10-M7.1.2_Parameter_can_be_declared_as_const
  /*!
   * \brief Constructs a timer manager. Aborts in case of construction error.
   * \param[in] reactor                The reactor, used to construct the timer manager.
   * \param[in] timer_manager_factory  The factory used to create a timer manager.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   */
  TimerManagerWrapper(osabstraction::io::reactor1::Reactor1Interface& reactor,
                      amsr::steady_timer::TimerManagerFactoryInterface&& timer_manager_factory) noexcept;

  /*!
   * \brief Destructor.
   * \details Takes care of deinitializing the timer manager.
   *          Note: the destruction of this object leads to the deinitialization and destruction of the timer manager,
   *          so it must be destroyed after all objects holding a pointer to the timer manager are destroyed.
   *
   * \pre       All timers belonging to the wrapped timer manager must have been destroyed before calling this API.
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~TimerManagerWrapper() noexcept;

  TimerManagerWrapper(TimerManagerWrapper const&) noexcept = delete;
  TimerManagerWrapper(TimerManagerWrapper&&) noexcept = delete;
  TimerManagerWrapper& operator=(TimerManagerWrapper const&) noexcept = delete;
  TimerManagerWrapper& operator=(TimerManagerWrapper&&) noexcept = delete;

  /*!
   * \brief Returns a raw pointer to the created timer manager
   *
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   * \return The instance ID or no value if the key does not map to any existing entry.
   */
  amsr::steady_timer::TimerManagerInterface* GetTimerManager() const noexcept;

 private:
  /*!
   * The created timer manager.
   */
  std::shared_ptr<amsr::steady_timer::TimerManagerInterface> timer_manager_{nullptr};
};

}  // namespace timer_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TIMER_MANAGER_TIMER_MANAGER_WRAPPER_H_

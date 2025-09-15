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
/**        \file  oneshot_multicast_timer_observer_interface.h
 *        \brief  Interface for ServiceDiscovery one shot multicast timer observer
 *
 *      \details  Allows an observer to be notified when the oneshot multicast timer has been fired
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_MULTICAST_TIMER_OBSERVER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_MULTICAST_TIMER_OBSERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace scheduler {

/*!
 * \brief OneshotMulticastTimerObserverInterface
 */
class OneshotMulticastTimerObserverInterface {
 public:
  /*!
   * \brief Constructor
   * \context Init
   * \steady  FALSE
   */
  OneshotMulticastTimerObserverInterface() noexcept = default;
  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady  FALSE
   */
  virtual ~OneshotMulticastTimerObserverInterface() noexcept = default;

  OneshotMulticastTimerObserverInterface(OneshotMulticastTimerObserverInterface const&) = delete;
  OneshotMulticastTimerObserverInterface(OneshotMulticastTimerObserverInterface&&) = delete;
  OneshotMulticastTimerObserverInterface& operator=(OneshotMulticastTimerObserverInterface const&) & = delete;
  OneshotMulticastTimerObserverInterface& operator=(OneshotMulticastTimerObserverInterface&&) & = delete;

  /*!
   * \brief Called at the end of the HandleTimer function of the observer timer
   * \pre -
   * \context   Timer
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void OnMulticastTimerExpired() = 0;
};

}  // namespace scheduler
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_MULTICAST_TIMER_OBSERVER_INTERFACE_H_

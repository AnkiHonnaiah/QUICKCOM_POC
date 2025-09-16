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
/**        \file  oneshot_unicast_timer_observer_interface.h
 *        \brief  Interface for ServiceDiscovery Message UnicastOneshotTimer Observer
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_UNICAST_TIMER_OBSERVER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_UNICAST_TIMER_OBSERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace scheduler {

/*!
 * \brief OneshotUnicastTimerObserverInterface
 */
class OneshotUnicastTimerObserverInterface {
 public:
  /*!
   * \brief Default constructor.
   * \context Init
   * \steady  FALSE
   */
  OneshotUnicastTimerObserverInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady  FALSE
   */
  virtual ~OneshotUnicastTimerObserverInterface() noexcept = default;

  OneshotUnicastTimerObserverInterface(OneshotUnicastTimerObserverInterface const&) = delete;
  OneshotUnicastTimerObserverInterface& operator=(OneshotUnicastTimerObserverInterface const&) & = delete;
  OneshotUnicastTimerObserverInterface(OneshotUnicastTimerObserverInterface&&) = delete;
  OneshotUnicastTimerObserverInterface& operator=(OneshotUnicastTimerObserverInterface&&) & = delete;

  /*!
   * \brief Called at the end of the HandleTimer function of the observer timer
   * \param[in] address_pair Pair of IP-Address and port of the unicast message receiver
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   */
  virtual void OnUnicastTimerExpired(amsr::someip_protocol::internal::AddressPair const& address_pair) = 0;
};

}  // namespace scheduler
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_SCHEDULER_ONESHOT_UNICAST_TIMER_OBSERVER_INTERFACE_H_

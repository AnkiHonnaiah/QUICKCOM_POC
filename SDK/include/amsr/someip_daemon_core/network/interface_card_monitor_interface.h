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
/**        \file  interface_card_monitor_interface.h
 *        \brief  A InterfaceMonitor Interface which used to allow registration and unregistration of observers.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/network/interface_card_state_observer_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace network {

/*!
 * \brief Interface for NetworkCardMonitor class.
 */
class InterfaceCardMonitorInterface {
 public:
  /*!
   * \brief Default constructor.
   *
   * \steady  FALSE
   */
  InterfaceCardMonitorInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \steady  FALSE
   */
  virtual ~InterfaceCardMonitorInterface() noexcept = default;

  InterfaceCardMonitorInterface(InterfaceCardMonitorInterface const&) = delete;
  InterfaceCardMonitorInterface(InterfaceCardMonitorInterface&&) = delete;
  InterfaceCardMonitorInterface& operator=(InterfaceCardMonitorInterface const&) & = delete;
  InterfaceCardMonitorInterface& operator=(InterfaceCardMonitorInterface&&) & = delete;

  /*!
   * \brief         Register the Network State Observer.
   *
   * \param[in]     observer The Network state observer.
   * \context       App
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   * \steady        FALSE
   *
   */
  virtual void RegisterInterfaceCardStateObserver(InterfaceCardStateObserverInterface* observer) noexcept = 0;
};

}  // namespace network
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_INTERFACE_H_

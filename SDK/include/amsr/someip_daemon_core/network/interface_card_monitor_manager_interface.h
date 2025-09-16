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
/**        \file  interface_card_monitor_manager_interface.h
 *        \brief  A Interface Card Monitor Manager Interface class
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/network/interface_card_monitor_interface.h"
#include "amsr/someip_daemon_core/network/interface_card_state_observer_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace network {

/*!
 * \brief InterfaceCardMonitorManagerInterface class.
 */
class InterfaceCardMonitorManagerInterface {
 public:
  /*!
   * \brief  Default constructor.
   *
   * \context   Init
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  InterfaceCardMonitorManagerInterface() = default;

  /*!
   * \brief Default destructor.
   *
   * \context   Shutdown
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~InterfaceCardMonitorManagerInterface() noexcept = default;

  InterfaceCardMonitorManagerInterface(InterfaceCardMonitorManagerInterface const&) = delete;
  InterfaceCardMonitorManagerInterface(InterfaceCardMonitorManagerInterface&&) = delete;
  InterfaceCardMonitorManagerInterface& operator=(InterfaceCardMonitorManagerInterface const&) & = delete;
  InterfaceCardMonitorManagerInterface& operator=(InterfaceCardMonitorManagerInterface&&) & = delete;

  /*!
   * \brief        Add the Interface Monitor.
   *
   * \param[in]    unicast_sd_address        The Unicast service discovery address.
   * \param[in]    interface_card_monitor    The interface card monitor.
   * \context      Init
   * \pre          no interface monitor has already been registered for the provided unicast sd address.
   *
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \threadsafe   FALSE
   * \steady       FALSE
   */
  virtual void AddInterfaceCardMonitor(
      someip_daemon_core::IpAddress const& unicast_sd_address,
      amsr::UniquePtr<InterfaceCardMonitorInterface> interface_card_monitor) noexcept = 0;

  /*!
   * \brief        Register a Interface Card State Observer.
   *
   * \param[in]    unicast_sd_address   The Unicast service discovery address.
   * \param[in]    observer             The Interface Card state observer.
   * \context      Init
   *
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \threadsafe   FALSE
   * \steady       FALSE
   */
  virtual void RegisterInterfaceCardStateObserver(someip_daemon_core::IpAddress const& unicast_sd_address,
                                                  InterfaceCardStateObserverInterface* observer) noexcept = 0;

  /*!
   * \brief        Erase all the contents from the Interface Card Monitor container.
   *
   * \context      Shutdown
   *
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \threadsafe   FALSE
   * \steady       FALSE
   */
  virtual void Deinitialize() noexcept = 0;
};

}  // namespace network
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_MANAGER_INTERFACE_H_

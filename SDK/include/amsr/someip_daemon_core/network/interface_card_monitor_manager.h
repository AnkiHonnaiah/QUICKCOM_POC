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
/**        \file  interface_card_monitor_manager.h
 *        \brief  A Interface Card Monitor Manager class
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/network/interface_card_monitor_interface.h"
#include "amsr/someip_daemon_core/network/interface_card_monitor_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace network {

/*!
 * \brief InterfaceCardMonitorManager class.
 */
class InterfaceCardMonitorManager final : public InterfaceCardMonitorManagerInterface {
 public:
  /*!
   * \brief  Default constructor.
   *
   * \context   Init
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  InterfaceCardMonitorManager() = default;

  /*!
   * \brief Default destructor.
   *
   * \context   Shutdown
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~InterfaceCardMonitorManager() noexcept override = default;

  InterfaceCardMonitorManager(InterfaceCardMonitorManager const&) = delete;
  InterfaceCardMonitorManager(InterfaceCardMonitorManager&&) = delete;
  InterfaceCardMonitorManager& operator=(InterfaceCardMonitorManager const&) & = delete;
  InterfaceCardMonitorManager& operator=(InterfaceCardMonitorManager&&) & = delete;

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
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void AddInterfaceCardMonitor(someip_daemon_core::IpAddress const& unicast_sd_address,
                               amsr::UniquePtr<InterfaceCardMonitorInterface> interface_card_monitor) noexcept override;

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
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void RegisterInterfaceCardStateObserver(someip_daemon_core::IpAddress const& unicast_sd_address,
                                          InterfaceCardStateObserverInterface* observer) noexcept override;

  /*!
   * \brief        Erase all the contents from the Interface Card Monitor container.
   *
   * \context      Shutdown
   *
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \threadsafe   FALSE
   * \steady        FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void Deinitialize() noexcept override;

 private:
  /*!
   * \brief   A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kNetworkLoggerContextId,
                                                    someip_daemon_core::logging::kNetworkLoggerContextDescription,
                                                    ara::core::StringView{"InterfaceCardMonitorManager"}};

  /*!
   * \brief   The Network Interface Card Monitor Container.
   */
  ara::core::Map<amsr::someip_protocol::internal::IpAddress, amsr::UniquePtr<InterfaceCardMonitorInterface>>
      interface_card_monitors_map_{};
};

}  // namespace network
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_MANAGER_H_

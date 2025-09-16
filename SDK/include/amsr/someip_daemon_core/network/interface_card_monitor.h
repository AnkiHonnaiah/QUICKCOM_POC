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
/**        \file  interface_card_monitor.h
 *        \brief  A Interface Card Monitor class
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/interface.h"
#include "amsr/net/interface_monitor.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/network/interface_card.h"
#include "amsr/someip_daemon_core/network/interface_card_monitor_impl.h"
#include "amsr/someip_daemon_core/network/interface_card_monitor_interface.h"
#include "ara/core/optional.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace network {

/*!
 * \brief InterfaceCardMonitor class.
 */
class InterfaceCardMonitor final : public InterfaceCardMonitorInterface {
 public:
  /*!
   * \brief  Default constructor.
   *
   * \param[in] address   A remote IP address.
   * \param[in] reactor   A reactor.
   *
   * \context   Init
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  InterfaceCardMonitor(someip_daemon_core::IpAddress const address,
                       osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \context   Shutdown
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~InterfaceCardMonitor() noexcept override = default;

  InterfaceCardMonitor() = delete;
  InterfaceCardMonitor(InterfaceCardMonitor const&) = delete;
  InterfaceCardMonitor(InterfaceCardMonitor&&) = delete;
  InterfaceCardMonitor& operator=(InterfaceCardMonitor const&) & = delete;
  InterfaceCardMonitor& operator=(InterfaceCardMonitor&&) & = delete;

 private:
  /*!
   * \brief        Register a Network State Observer.
   *
   * \param[in]    observer The Network state observer.
   * \context      App
   *
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \threadsafe   FALSE
   * \steady       FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void RegisterInterfaceCardStateObserver(InterfaceCardStateObserverInterface* observer) noexcept override;

  /*!
   * \brief   A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kNetworkLoggerContextId,
                                                    someip_daemon_core::logging::kNetworkLoggerContextDescription,
                                                    ara::core::StringView{"InterfaceCardMonitor"}};

  /*!
   * \brief   The Interface.
   */
  net::Interface interface_;

  /*!
   * \brief   The Interface Card.
   */
  InterfaceCard interface_card_{interface_};

  /*!
   * \brief   The Interface card Monitor Impl.
   */
  InterfaceCardMonitorImpl interface_card_monitor_impl_{interface_card_, interface_.GetName()};

  /*!
   * \brief   The Interface Monitor.
   */
  ara::core::Optional<amsr::net::InterfaceMonitor> interface_monitor_{};
};

}  // namespace network
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_H_

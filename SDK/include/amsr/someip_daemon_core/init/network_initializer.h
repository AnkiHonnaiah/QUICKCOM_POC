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
/**        \file  network_initializer.h
 *         \brief  Network Initializer.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_NETWORK_INITIALIZER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_NETWORK_INITIALIZER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/connection_manager/connection_manager.h"
#include "amsr/someip_daemon_core/connection_manager/connection_manager_initializer_interface.h"
#include "amsr/someip_daemon_core/network/interface_card_monitor_manager_interface.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace init {

/*!
 * \brief The NetworkInitializer class used to create the network interface monitors
 *        for the unique unicast address taken from network endpoint service discovery and
 *        emplace them into the interface card monitor manager.
 */
class NetworkInitializer final {
 public:
  /*!
   * \brief default constructor.
   *
   * \steady   FALSE
   */
  NetworkInitializer() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \steady   FALSE
   */
  ~NetworkInitializer() noexcept = default;

  NetworkInitializer(NetworkInitializer const&) = delete;
  NetworkInitializer(NetworkInitializer&&) = delete;
  NetworkInitializer& operator=(NetworkInitializer const&) & = delete;
  NetworkInitializer& operator=(NetworkInitializer&&) & = delete;

  /*!
   * \brief       For each configured network endpoint, an actual network endpoint is created and emplaced in
   *              the connection manager.
   * \details     Lifecycle (and destruction) of those endpoints is handled by the ConnectionManager object.
   *              Depending on the configuration The created endpoints could either be created as TCP/UDP or with
   *              TLS configuration. The Endpoint will listen for TLS traffic on exposed ports,
   *              and transfer it as TCP/UDP traffic to the application over the same port.
   *
   * \param[in]   configuration       The configuration.
   * \param[in]   connection_manager  The connection manager.
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \trace SPEC-10144432, SPEC-10144433
   */
  static void InitializeConnectionManager(
      configuration::Configuration const& configuration,
      connection_manager::ConnectionManagerInitializerInterface& connection_manager) noexcept;

  /*!
   * \brief       In the InitializeNetworkMonitoring method, for each service discovery unicast address, an interface
   *              card monitor instance will be created and emplaced into the interface card monitor manager.
   *
   * \param[in]   configuration                   The configuration.
   * \param[in]   interface_card_monitor_manager  The Network Interface Monitor Manager
   * \param[in]   reactor                         A reactor.
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static void InitializeNetworkMonitoring(configuration::Configuration const& configuration,
                                          network::InterfaceCardMonitorManagerInterface& interface_card_monitor_manager,
                                          osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept;
};

}  // namespace init
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_INIT_NETWORK_INITIALIZER_H_

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
/**        \file  service_discovery_server_state_pool.h
 *        \brief  Service Discovery Server State Pool
 *
 *      \details  This class initializes and contains all states of the SD Server State Machine
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_POOL_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_state_down.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_state_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_state_main.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_state_repetition.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_state_wait.h"
#include "vac/statemachine/state_pool.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * \brief ServiceDiscoveryServerStatePool.
 */
class ServiceDiscoveryServerStatePool final
    : public vac::statemachine::StatePool<ServiceDiscoveryServerStateInterface> {
 public:
  /*!
   * \brief Default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryServerStatePool() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ServiceDiscoveryServerStatePool() noexcept override = default;

  ServiceDiscoveryServerStatePool(ServiceDiscoveryServerStatePool const&) = delete;
  ServiceDiscoveryServerStatePool(ServiceDiscoveryServerStatePool&&) = delete;
  ServiceDiscoveryServerStatePool& operator=(ServiceDiscoveryServerStatePool const&) & = delete;
  ServiceDiscoveryServerStatePool& operator=(ServiceDiscoveryServerStatePool&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Returns State of given handle.
   *
   * \param[in] state_handle The state handle.
   *
   * \return The State with given handle.
   *
   * \pre          -
   * \context      Init, App, Network, Timer
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  ServiceDiscoveryServerStateInterface* GetState(Handle const state_handle) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Checks if handle refers to an existing state.
   *
   * \param[in] state_handle The state handle.
   *
   * \return true if state exists with given handle, otherwise returns false.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  bool IsValid(Handle const state_handle) const override;

 private:
  /*!
   * \brief Down phase of the SD server state machine.
   */
  ServiceDiscoveryServerStateDown down_state_;

  /*!
   * \brief Initial Wait phase of the SD server state machine.
   */
  ServiceDiscoveryServerStateWait initial_wait_state_;

  /*!
   * \brief Repetition phase of the SD server state machine.
   */
  ServiceDiscoveryServerStateRepetition repetition_state_;

  /*!
   * \brief Main phase of the SD server state machine.
   */
  ServiceDiscoveryServerStateMain main_state_;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_POOL_H_

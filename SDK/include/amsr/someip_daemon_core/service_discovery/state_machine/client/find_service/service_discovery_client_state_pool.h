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
/**        \file  service_discovery_client_state_pool.h
 *        \brief  A service discovery client state pool
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_POOL_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/initial_sd_delay_config.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_down_phase.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_initial_wait_phase.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_main_phase.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_repetition_phase.h"
#include "vac/statemachine/state_pool.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/*!
 * \brief A Service Discovery Client state pool.
 */
class ServiceDiscoveryClientStatePool final : public vac::statemachine::StatePool<ServiceDiscoveryClientState> {
 public:
  /*!
   * \brief     constructor
   * \param[in] initial_sd_delay_config Initial SD delay configuration.
   *
   * \context ANY (Init or during runtime for Instance ID = ALL)
   * \steady FALSE
   */
  explicit ServiceDiscoveryClientStatePool(
      configuration::types::InitialSdDelayConfig const& initial_sd_delay_config) noexcept;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ServiceDiscoveryClientStatePool() noexcept override = default;

  ServiceDiscoveryClientStatePool() = delete;
  ServiceDiscoveryClientStatePool(ServiceDiscoveryClientStatePool const&) = delete;
  ServiceDiscoveryClientStatePool(ServiceDiscoveryClientStatePool&&) = delete;
  ServiceDiscoveryClientStatePool& operator=(ServiceDiscoveryClientStatePool const&) & = delete;
  ServiceDiscoveryClientStatePool& operator=(ServiceDiscoveryClientStatePool&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Returns the instance of a state identified by the given state handle.
   *
   * \param[in] state_handle A state handle.
   * \return An instance of a state.
   * \context App
   * \steady FALSE
   */
  ServiceDiscoveryClientState* GetState(ServiceDiscoveryClientStateHandle const state_handle) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether there is a state identified by the given handle.
   *
   * \param[in] state_handle A state handle.
   * \return true if the state is valid and false otherwise.
   * \context App
   * \steady FALSE
   */
  bool IsValid(ServiceDiscoveryClientStateHandle const state_handle) const override;

 private:
  /*!
   * \brief An instance of state 'DOWN_PHASE'.
   */
  ServiceDiscoveryClientStateDownPhase state_down_phase_{};
  /*!
   * \brief An instance of state 'INITIAL_WAIT_PHASE'.
   */
  ServiceDiscoveryClientStateInitialWaitPhase state_initial_wait_phase_;
  /*!
   * \brief An instance of state 'REPETITION_PHASE'.
   *        Repetition phase exists only of InitialRepetitionsMax is defined and set to a value > 0
   */
  ara::core::Optional<ServiceDiscoveryClientStateRepetitionPhase> state_repetition_phase_{};
  /*!
   * \brief An instance of state 'MAIN_PHASE'.
   */
  ServiceDiscoveryClientStateMainPhase state_main_phase_{};
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_POOL_H_

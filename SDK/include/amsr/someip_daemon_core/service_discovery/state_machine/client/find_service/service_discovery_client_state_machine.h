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
/**        \file  service_discovery_client_state_machine.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>  // unique_ptr
#include "amsr/someip_daemon_core/configuration/types/initial_sd_delay_config.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine_context.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_owner.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_pool.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/container/string_literals.h"
#include "vac/statemachine/state_owner.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals.
 */
using vac::container::literals::operator""_sv;

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief A Service Discovery Client state machine.
 * \trace SPEC-4981670
 */
class ServiceDiscoveryClientStateMachine : public ServiceDiscoveryClientStateContext {
 public:
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryClientStateMachine() noexcept override = default;

  ServiceDiscoveryClientStateMachine() = delete;
  ServiceDiscoveryClientStateMachine(ServiceDiscoveryClientStateMachine const &) = delete;
  ServiceDiscoveryClientStateMachine(ServiceDiscoveryClientStateMachine &&) = delete;
  ServiceDiscoveryClientStateMachine &operator=(ServiceDiscoveryClientStateMachine const &) & = delete;
  ServiceDiscoveryClientStateMachine &operator=(ServiceDiscoveryClientStateMachine &&) & = delete;

  /*!
   * \brief Constructor of ServiceDiscoveryClientStateMachine.
   *
   * \param[in] context                                  A state machine context.
   * \param[in] timer_manager                            A timer manager.
   * \param[in] initial_sd_delay_config                  Initial delay configuration (initial delay min/max, repetition
   *                                                     count/delay) for this SM.
   * \param[in] required_service_instance_deployment_id  The service deployment id.
   * \param[in] instance_id                              A SOME/IP service instance identifier.
   *
   * \context ANY (Init or during runtime for Instance ID = ALL)
   * \steady FALSE
   */
  ServiceDiscoveryClientStateMachine(
      ServiceDiscoveryClientStateMachineContext *context, amsr::steady_timer::TimerManagerInterface *timer_manager,
      configuration::types::InitialSdDelayConfig const &initial_sd_delay_config,
      configuration::types::RequiredSomeIpServiceInstanceDeploymentId const &required_service_instance_deployment_id,
      amsr::someip_protocol::internal::InstanceId const instance_id);

  /*!
   * \brief Called when network has been activated.
   * \context Network
   * \steady FALSE
   * \trace SPEC-4981693
   */
  void OnNetworkUp();

  /*!
   * \brief Called when network has been deactivated.
   * \context Network
   * \steady FALSE
   * \trace SPEC-4981693
   */
  void OnNetworkDown();

  /*!
   * \brief Called when the service has been requested.
   * \context App
   * \steady FALSE
   */
  void OnServiceRequested();

  /*!
   * \brief Called when the service has been released.
   * \context App
   * \steady FALSE
   */
  void OnServiceReleased();

  /*!
   * \brief Called when a matching OfferService entry has been received.
   * \context App
   * \steady TRUE
   * \trace SPEC-4981693
   */
  void OnOfferService();

  /*!
   * \brief Called when a previously scheduled delay has expired.
   * \context Timer
   * \steady FALSE
   */
  void OnTimeout();

  /*!
   * \brief Called when the TTL of the current offer has expired.
   * \context Timer
   * \steady FALSE
   */
  void OnOfferTtlExpired();

 protected:
  /*!
   * \brief Indicates whether the service is available or not.
   *
   * \return true if the service is available and false otherwise.
   * \context App
   * \steady FALSE
   */
  bool IsServiceAvailable() const override;

  /*!
   * \brief Indicates whether network is active or not.
   *
   * \return true if network is active and false otherwise.
   * \context App
   * \steady FALSE
   */
  bool IsNetworkUp() const override;

  /*!
   * \brief Indicates whether the service is requested or not.
   *
   * \return true if the service is requested and false otherwise.
   * \context App
   * \steady FALSE
   */
  bool IsServiceRequested() const override;

  /*!
   *
   * \brief Transmits a FindService entry.
   * \context App
   * \steady FALSE
   */
  void SendFindService() override;

  /*!
   * \brief Initiates a state change.
   *
   * \param[in] handle The handle of a new state.
   * \pre The new state shall be valid.
   *
   * \context App
   * \steady FALSE
   */
  void ChangeState(ServiceDiscoveryClientStateHandle const handle) override;

  /*!
   * \brief Starts the timer with a given delay.
   *
   * \param[in] delay A timer delay.
   * \pre The delay shall not be zero.
   *
   * \context App | Timer
   * \steady FALSE
   */
  void StartTimer(std::chrono::nanoseconds const delay) noexcept override;

  /*!
   * \brief Stops the timer.
   * \context Timer
   * \steady FALSE
   */
  void StopTimer() override;

  /*!
   * \brief     Return current state.
   * \return    Current state.
   * \pre       -
   * \reentrant False
   * \context   Testing
   * \steady FALSE
   */
  ServiceDiscoveryClientStateHandle GetCurrentState();

 private:
  /*!
   * \brief A state machine context.
   */
  ServiceDiscoveryClientStateMachineContext *context_;
  /*!
   * \brief A state pool.
   */
  ServiceDiscoveryClientStatePool state_pool_;

  /*!
   * \brief A timer for states.
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> state_timer_;

  /*!
   * \brief A state owner.
   */
  ServiceDiscoveryClientStateOwner state_owner_{state_pool_, *this};

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_H_

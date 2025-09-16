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
/**        \file  service_discovery_client_state_initial_wait_phase.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_INITIAL_WAIT_PHASE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_INITIAL_WAIT_PHASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/initial_sd_delay_config.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_context.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {
/*!
 * \brief Service Discovery Client state 'INITIAL_WAIT_PHASE'.
 * \trace SPEC-10144722
 * \trace SPEC-4980066
 */
class ServiceDiscoveryClientStateInitialWaitPhase final : public ServiceDiscoveryClientState {
 public:
  /*!
   * \brief Constructor of ServiceDiscoveryClientStateInitialWaitPhase.
   * \context ANY (Init or during runtime for Instance ID = ALL)
   * \steady FALSE
   */
  explicit ServiceDiscoveryClientStateInitialWaitPhase(
      configuration::types::InitialSdDelayConfig const& initial_sd_delay_config) noexcept;

  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ServiceDiscoveryClientStateInitialWaitPhase() noexcept override = default;

  ServiceDiscoveryClientStateInitialWaitPhase() = delete;
  ServiceDiscoveryClientStateInitialWaitPhase(ServiceDiscoveryClientStateInitialWaitPhase const&) = delete;
  ServiceDiscoveryClientStateInitialWaitPhase(ServiceDiscoveryClientStateInitialWaitPhase&&) = delete;
  ServiceDiscoveryClientStateInitialWaitPhase& operator=(ServiceDiscoveryClientStateInitialWaitPhase const&) & = delete;
  ServiceDiscoveryClientStateInitialWaitPhase& operator=(ServiceDiscoveryClientStateInitialWaitPhase&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether a state change is valid or not.
   *
   * \param[in] handle A state handle.
   * \return true if the state change is valid and false otherwise.
   * \context App
   * \steady FALSE
   */
  bool IsValidChange(ServiceDiscoveryClientStateHandle const handle) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the state is entered.
   *
   * \param[in,out] context A state context.
   * \context App
   * \steady FALSE
   *
   * \trace SPEC-4981638
   * \trace SPEC-4981648
   */
  void OnEnter(ServiceDiscoveryClientStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the state is left.
   *
   * \param[in,out] context A state context.
   * \context App
   * \steady FALSE
   */
  void OnLeave(ServiceDiscoveryClientStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the OS notifies about the network being up.
   * \details The OS may not guarantee consistent network UP/DOWN notifications, so we must allow the case of network up
   *          notification when the network was already up from our perspective.
   *
   * \param[in,out] context A state context.
   * \context Network
   * \steady FALSE
   */
  void OnNetworkUp(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the OS notifies about the network being down.
   *
   * \param[in,out] context A state context.
   * \context Network
   * \steady FALSE
   * \trace SPEC-4981664
   * \trace SPEC-10144744
   */
  void OnNetworkDown(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when service has been requested.
   *
   * \param[in,out] context A state context.
   * \pre Service shall not be requested previously.
   * \context App
   * \steady FALSE
   */
  void OnServiceRequested(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when service has been released.
   *
   * \param[in,out] context A state context.
   * \context App
   * \steady FALSE
   * \trace SPEC-10144743
   */
  void OnServiceReleased(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param[in,out] context A state context.
   * \context App
   * \steady FALSE
   *
   * \trace SPEC-4981647
   * \trace SPEC-10144737
   * \trace SPEC-10144738
   */
  void OnOfferService(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a previously scheduled delay has expired.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady FALSE
   *
   * \trace SPEC-4981648
   * \trace SPEC-10144740
   * \trace SPEC-10144741
   */
  void OnTimeout(ServiceDiscoveryClientStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the TTL of the current offer is expired.
   *
   * \param[in,out] context A state context.
   * \pre Service shall be offered already.
   * \context Timer
   * \steady FALSE
   */
  void OnOfferTtlExpired(ServiceDiscoveryClientStateContext& context) const override;

 private:
  /*!
   * \brief Returns a random initial delay.
   *
   * \return A random initial delay in nanoseconds.
   * \context App
   * \steady FALSE
   * \trace SPEC-4981639
   * \trace SPEC-4981640
   * \trace SPEC-9646125
   */
  std::chrono::nanoseconds GetRandomInitialDelay() const noexcept;

  /*!
   * \brief Initial delay configurations for this SM.
   */
  configuration::types::InitialSdDelayConfig const initial_sd_delay_config_;

  /*!
   * \brief true if repetition phase is configured, false otherwise.
   */
  bool const repetition_phase_configured_{(initial_sd_delay_config_.initial_repetitions_max.has_value()) &&
                                          (initial_sd_delay_config_.initial_repetitions_max.value() > 0U)};

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"ServiceDiscoveryClientStateInitialWaitPhase"}};
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_INITIAL_WAIT_PHASE_H_

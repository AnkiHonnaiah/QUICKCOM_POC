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
/**        \file  service_discovery_client_state_repetition_phase.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_REPETITION_PHASE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_REPETITION_PHASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include "amsr/someip_daemon_core/configuration/types/initial_sd_delay_config.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_context.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/*!
 * \brief Service Discovery Client state 'REPETITION_PHASE'.
 * \trace SPEC-10144722
 */
class ServiceDiscoveryClientStateRepetitionPhase final : public ServiceDiscoveryClientState {
 public:
  /*!
   * \brief Constructor of ServiceDiscoveryClientStateRepetitionPhase.
   * \param[in] initial_repetitions_max         max count of repetitions to send FindService messages (Must be > 0).
   * \param[in] initial_repetitions_base_delay  Base delay that is used to calculate the delay between sending
   *                                            FindService messages during repetition phase.
   * \steady FALSE
   */
  ServiceDiscoveryClientStateRepetitionPhase(configuration::types::InitialRepetitionsMax const initial_repetitions_max,
                                             std::chrono::nanoseconds const& initial_repetitions_base_delay) noexcept;

  /*!
   * \brief Default destructor.
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ServiceDiscoveryClientStateRepetitionPhase() noexcept override = default;

  ServiceDiscoveryClientStateRepetitionPhase(ServiceDiscoveryClientStateRepetitionPhase const&) = delete;
  ServiceDiscoveryClientStateRepetitionPhase(ServiceDiscoveryClientStateRepetitionPhase&&) = delete;
  ServiceDiscoveryClientStateRepetitionPhase& operator=(ServiceDiscoveryClientStateRepetitionPhase const&) & = delete;
  ServiceDiscoveryClientStateRepetitionPhase& operator=(ServiceDiscoveryClientStateRepetitionPhase&&) & = delete;

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
   * \trace SPEC-4981644
   * \trace SPEC-10144745
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
   * \trace SPEC-10144754
   */
  void OnNetworkDown(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when service has been requested.
   *
   * \param[in,out] context A state context.
   * \pre The service shall be requested already.
   *
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
   * \trace SPEC-10144753
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
   * \trace SPEC-4981653
   * \trace SPEC-10144751
   * \trace SPEC-10144752
   */
  void OnOfferService(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a previously scheduled delay has expired.
   *
   * \param[in,out] context A state context.
   * \context Timer
   * \steady FALSE
   *
   * \trace SPEC-4981645, SPEC-4981646, SPEC-4981649, SPEC-10144746, SPEC-10144747
   */
  void OnTimeout(ServiceDiscoveryClientStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the TTL of the current offer is expired.
   *
   * \param[in,out] context A state context.
   * \pre The service shall be offered already.
   *
   * \context Timer
   * \steady FALSE
   */
  void OnOfferTtlExpired(ServiceDiscoveryClientStateContext& context) const override;

 private:
  /*!
   * \brief The maximum number of repetitions in the repetition phase.
   *        Must be > 0.
   */
  configuration::types::InitialRepetitionsMax const initial_repetitions_max_;

  /*!
   * \brief The base delay in the FindService entry during repetition phase.
   */
  std::chrono::nanoseconds const initial_repetitions_base_delay_;

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"ServiceDiscoveryClientStateRepetitionPhase"}};

  /*!
   * \brief Counts number of repetitions so far.
   */
  std::size_t repetition_counter_{0U};

  /*!
   * \brief A time value to delay the next transmission of a FindService entry.
   */
  std::chrono::nanoseconds timer_delay_{std::chrono::nanoseconds{0U}};
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_REPETITION_PHASE_H_

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
/**        \file  service_discovery_client_state_down_phase.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_DOWN_PHASE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_DOWN_PHASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_context.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/*!
 * \brief Service Discovery Client state 'DOWN_PHASE'.
 * \trace SPEC-10144722
 */
class ServiceDiscoveryClientStateDownPhase final : public ServiceDiscoveryClientState {
 public:
  /*!
   * \brief Constructor of ServiceDiscoveryClientStateDownPhase.
   * \context ANY (Init or during runtime for Instance ID = ALL)
   * \steady FALSE
   */
  ServiceDiscoveryClientStateDownPhase() noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryClientStateDownPhase() noexcept override = default;

  ServiceDiscoveryClientStateDownPhase(ServiceDiscoveryClientStateDownPhase const&) = delete;
  ServiceDiscoveryClientStateDownPhase(ServiceDiscoveryClientStateDownPhase&&) = delete;
  ServiceDiscoveryClientStateDownPhase& operator=(ServiceDiscoveryClientStateDownPhase const&) & = delete;
  ServiceDiscoveryClientStateDownPhase& operator=(ServiceDiscoveryClientStateDownPhase&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether a state change is valid or not.
   *
   * \param[in] handle A state handle.
   * \return true if the state change is valid and false otherwise.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  bool IsValidChange(ServiceDiscoveryClientStateHandle const handle) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the state is entered.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady FALSE
   * \trace SPEC-10144724
   * \trace SPEC-10144732
   */
  void OnEnter(ServiceDiscoveryClientStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the state is left.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  void OnLeave(ServiceDiscoveryClientStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the OS notifies about the network being up.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context Network
   * \steady FALSE
   * \trace SPEC-4981636
   */
  void OnNetworkUp(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the OS notifies about the network being down.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context Network
   * \steady FALSE
   */
  void OnNetworkDown(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when service has been requested.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady FALSE
   *
   * \trace SPEC-4980066
   * \trace SPEC-4981636
   * \trace SPEC-4981647
   * \trace SPEC-10144728
   * \trace SPEC-10144729
   */
  void OnServiceRequested(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when service has been released.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  void OnServiceReleased(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param[in] context A state context.
   * \pre     -
   * \context App
   * \steady FALSE
   * \trace SPEC-10144725
   * \trace SPEC-10144733
   */
  void OnOfferService(ServiceDiscoveryClientStateContext& context) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a previously scheduled delay has expired.
   *
   * \param[in,out] context A state context.
   *
   * \pre Timer shall be running.
   *
   * \context Timer
   * \steady FALSE
   */
  void OnTimeout(ServiceDiscoveryClientStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the TTL of the current offer is expired.
   *
   * \param[in,out] context A state context.
   *
   * \context Timer
   * \steady FALSE
   */
  void OnOfferTtlExpired(ServiceDiscoveryClientStateContext& context) const override;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_DOWN_PHASE_H_

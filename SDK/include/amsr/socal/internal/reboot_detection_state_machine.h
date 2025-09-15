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
/**        \file  reboot_detection_state_machine.h
 *        \brief  This class implements a pollable state machine reflecting the state of the server.
 *
 *      \details  The RebootDetectionStateMachine provides a pollable state reflecting:
 *                - if the service connected to is up,
 *                - if the service connected to is down or
 *                - if a reboot was detected.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REBOOT_DETECTION_STATE_MACHINE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REBOOT_DETECTION_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <mutex>
#include <utility>

#include "amsr/socal/internal/service_discovery/proxy_service_discovery_listener.h"
#include "amsr/socal/service_state.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Class modelling a state machine for reboot detection.
 *
 * \unit Socal::Proxy::RebootDetectionStateMachine
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class RebootDetectionStateMachine final
    : public ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryListener {
 public:
  /*!
   * \brief Polls the current state.
   * \return The state of the server as monitored by the state machine.
   *         - If the server goes down, the returned state is kDown.
   *         - If the server goes up again, kRestarted is returned, indicating a detected reboot.
   *         - After a detected reboot and a call to PollState, all subsequent calls return kUp as long as the server is
   *           available.
   * \pre        -
   * \context    App | Callback.
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ServiceState PollState() noexcept;

 private:
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SOCAL_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Signals a 'service up' to the state machine.
   * \pre         -
   * \context     Backend upon arrival of new updates | App when providing the initial snapshot.
   * \threadsafe  FALSE against calls to OnServiceStopped, TRUE otherwise.
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void OnServiceOffered() noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SOCAL_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Signals a 'service down' to the state machine.
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE against calls to OnServiceOffered, TRUE otherwise.
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void OnServiceStopped() noexcept override;

  /*!
   * \brief Server state with additional polling distinction in down state.
   */
  enum class InternalServiceState : uint8_t { kDownNotPolled, kDown, kRestarted, kUp };

  /*!
   * \brief Trigger signals used to progress state machine.
   */
  enum class ServiceSignal : uint8_t { kServiceUp, kServiceDown, kStatePolled };

  /*!
   * \brief Transit to the next state with the given signal.
   * \param[in] signal  The trigger signal for the state transitions.
   * \pre        -
   * \context    ANY
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void MakeTransition(ServiceSignal const signal) noexcept;

  /*!
   * \brief   Transition table for the internal states.
   * \details A std::pair is used as a Key, consisting of the current state and the signal triggering the transition.
   *          The value represents the next state.
   */
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_ConstantStaticObject
  static std::map<std::pair<InternalServiceState, ServiceSignal>, InternalServiceState> const transitions_;

  /*!
   * \brief Translation table for InternalServiceState to ServiceState.
   */
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_ConstantStaticObject
  static std::map<InternalServiceState, ServiceState> const to_service_state_;

  /*!
   * \brief Internal state representing the current server state with polling distinction.
   */
  InternalServiceState state_{InternalServiceState::kDown};

  /*!
   * \brief Mutex to protect state machine transition.
   */
  mutable std::mutex state_lock_{};
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REBOOT_DETECTION_STATE_MACHINE_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::internal::RebootDetectionMachine::state_lock_
 * Used for protecting concurrent state transitions and access.
 *
 * \protects ::amsr::socal::internal::RebootDetectionMachine::state_
 * \usedin ::amsr::socal::internal::RebootDetectionMachine::OnServiceOffered
 * \usedin ::amsr::socal::internal::RebootDetectionMachine::OnServiceStopped
 * \usedin ::amsr::socal::internal::RebootDetectionMachine::PollState
 * \exclude All other methods of a RebootDetectionMachine object.
 * \length SHORT Accessing or modifying state.
 * \endexclusivearea
 */
// clang-format on

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
/**        \file  service_discovery_client_state.h
 *        \brief  A class for service discovery client states.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/find_service/service_discovery_client_state_context.h"
#include "ara/core/string.h"
#include "vac/statemachine/state.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/*!
 * \brief Service Discovery Client states.
 * \trace SPEC-4981635
 */
enum class ServiceDiscoveryClientStateHandle : std::uint8_t {
  kDownPhase = 0,        /*! DOWN_PHASE state */
  kInitialWaitPhase = 1, /*! INITIAL_WAIT_PHASE state */
  kRepetitionPhase = 2,  /*! REPETITION_PHASE state */
  kMainPhase = 3         /*! MAIN_PHASE state */
};

/*!
 * \brief To convert ServiceDiscoveryClientState To String.
 *
 * \param[in] state Service Discovery Client State.
 *
 * \return the string representation of the state.
 *
 * \pre         -
 * \context     App, Network
 * \reentrant   FALSE
 * \steady      FALSE
 */
ara::core::String ServiceDiscoveryClientStateToString(ServiceDiscoveryClientStateHandle const& state) noexcept;

/*!
 * \brief A Service Discovery Client state.
 */
class ServiceDiscoveryClientState
    : public vac::statemachine::State<ServiceDiscoveryClientStateHandle, ServiceDiscoveryClientStateContext> {
 public:
  /*!
   * \brief Delete default constructor.
   * \steady FALSE
   */
  ServiceDiscoveryClientState() = delete;

  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady  FALSE
   */
  ~ServiceDiscoveryClientState() noexcept override = default;

  ServiceDiscoveryClientState(ServiceDiscoveryClientState const&) = delete;
  ServiceDiscoveryClientState(ServiceDiscoveryClientState&&) = delete;
  ServiceDiscoveryClientState& operator=(ServiceDiscoveryClientState const&) & = delete;
  ServiceDiscoveryClientState& operator=(ServiceDiscoveryClientState&&) & = delete;
  /*!
   * \brief shorthand for vac::statemachine::State<ServiceDiscoveryClientStateHandle,
   * ServiceDiscoveryClientStateContext>::State
   */
  using vac::statemachine::State<ServiceDiscoveryClientStateHandle, ServiceDiscoveryClientStateContext>::State;

  /*!
   * \brief Called when network becomes active.
   *
   * \param[in,out] context A state context.
   * \context Network
   * \steady FALSE
   */
  virtual void OnNetworkUp(ServiceDiscoveryClientStateContext& context) const = 0;

  /*!
   * \brief Called when network becomes inactive.
   *
   * \param[in,out] context A state context.
   * \context Network
   * \steady FALSE
   */
  virtual void OnNetworkDown(ServiceDiscoveryClientStateContext& context) const = 0;

  /*!
   * \brief Called when service has been requested.
   *
   * \param[in,out] context A state context.
   *
   * \context App
   * \steady FALSE
   */
  virtual void OnServiceRequested(ServiceDiscoveryClientStateContext& context) const = 0;

  /*!
   * \brief Called when service has been released.
   *
   * \param[in,out] context A state context.
   *
   * \context App
   * \steady FALSE
   */
  virtual void OnServiceReleased(ServiceDiscoveryClientStateContext& context) const = 0;

  /*!
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param[in,out] context A state context.
   *
   * \context App
   * \steady FALSE
   */
  virtual void OnOfferService(ServiceDiscoveryClientStateContext& context) const = 0;

  /*!
   * \brief Called when a previously scheduled delay has elapsed.
   *
   * \param[in,out] context A state context.
   *
   * \context Timer
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void OnTimeout(ServiceDiscoveryClientStateContext& context) = 0;

  /*!
   * \brief Called when the TTL of the current offer has expired.
   *
   * \param[in,out] context A state context.
   *
   * \context App
   * \steady FALSE
   */
  virtual void OnOfferTtlExpired(ServiceDiscoveryClientStateContext& context) const = 0;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_H_

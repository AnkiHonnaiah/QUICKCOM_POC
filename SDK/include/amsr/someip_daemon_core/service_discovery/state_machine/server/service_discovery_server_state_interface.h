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
/**        \file  service_discovery_server_state_interface.h
 *        \brief  ServiceDiscoveryServerStateInterface
 *
 *      \details  Super class for all states within SD Server State Machine
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_context_interface.h"
#include "vac/statemachine/state.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * \brief The handles representing the States defined for the StateMachine.
 * \trace SPEC-4981635
 */
enum class ServiceDiscoveryServerStateHandle : std::int32_t {
  kDownPhase = 0,
  kWaitPhase = 1,
  kRepetitionPhase = 2,
  kMainPhase = 3
};

/*!
 * \brief To convert ServiceDiscoveryServerState To String.
 *
 * \param[in] state Service Discovery Server State.
 *
 * \return the string representation of the state.
 *
 * \pre         -
 * \context     App, Network, Timer
 * \reentrant   FALSE
 * \steady      FALSE
 */
ara::core::String ServiceDiscoveryServerStateToString(ServiceDiscoveryServerStateHandle const& state) noexcept;

/*!
 * \brief ServiceDiscoveryServerStateInterface
 */
class ServiceDiscoveryServerStateInterface
    : public vac::statemachine::State<ServiceDiscoveryServerStateHandle, ServiceDiscoveryServerContextInterface> {
 public:
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryServerStateInterface() noexcept override = default;

  ServiceDiscoveryServerStateInterface() = delete;
  ServiceDiscoveryServerStateInterface(ServiceDiscoveryServerStateInterface const&) = delete;
  ServiceDiscoveryServerStateInterface(ServiceDiscoveryServerStateInterface&&) = delete;
  ServiceDiscoveryServerStateInterface& operator=(ServiceDiscoveryServerStateInterface const&) & = delete;
  ServiceDiscoveryServerStateInterface& operator=(ServiceDiscoveryServerStateInterface&&) & = delete;

 private:
  /*!
   * \brief Definition of the base class.
   */
  using Base = vac::statemachine::State<ServiceDiscoveryServerStateHandle, ServiceDiscoveryServerContextInterface>;

 public:
  /*!
   * copydoc vac::statemachine::State
   */
  using Base::Base;

  /*!
   * \brief Called when the provided service instance is brought up.
   *
   * \param context The server state machine context.
   *
   * \context App
   * \steady FALSE
   */
  virtual void OnServiceUp(ServiceDiscoveryServerContextInterface& context) const = 0;

  /*!
   * \brief Called when the provided service instance is brought down.
   *
   * \param context The server state machine context.
   * \steady FALSE
   */
  virtual void OnServiceDown(ServiceDiscoveryServerContextInterface& context) const = 0;

  /*!
   * \brief Called when network is brought up.
   *
   * \param context The server state machine context.
   *
   * \context App
   * \steady FALSE
   */
  virtual void OnNetworkUp(ServiceDiscoveryServerContextInterface& context) const = 0;

  /*!
   * \brief Called when network is brought down.
   *
   * \param context The server state machine context.
   *
   * \context App
   * \steady FALSE
   */
  virtual void OnNetworkDown(ServiceDiscoveryServerContextInterface& context) const = 0;

  /*!
   * \brief Called once a multicast offer service message is sent.
   *
   * \param[in] context The server state machine context.
   *
   * \context App
   * \steady TRUE
   */
  virtual void OnOfferSent(ServiceDiscoveryServerContextInterface& context) const = 0;

  /*!
   * \brief Called on reception of a SOME/IP SD FindService entry.
   *
   * \param[in] context The server state machine context.
   * \param[in] from_address An IP address identifying sender's IP address.
   * \param[in] from_port A UDP port identifying sender's port.
   *
   * \context App
   * \steady FALSE
   */
  virtual void OnFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                  someip_daemon_core::IpAddress const& from_address,
                                  amsr::net::ip::Port const& from_port) const noexcept = 0;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_INTERFACE_H_

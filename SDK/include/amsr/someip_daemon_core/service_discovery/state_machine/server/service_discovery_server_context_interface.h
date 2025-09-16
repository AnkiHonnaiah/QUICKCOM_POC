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
/**        \file  service_discovery_server_context_interface.h
 *        \brief  Service Discovery Server State Machine Context Interface
 *
 *      \details  Service Discovery Server State Machine Context Interface
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "ara/core/vector.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

enum class ServiceDiscoveryServerStateHandle;

/*!
 * \brief ServiceDiscoveryServerContextInterface interface for SD server state machine context.
 * The state machine context implementation must provide all functionalities required by different state machine states.
 */
// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
class ServiceDiscoveryServerContextInterface {
 public:
  /*!
   * \brief The signature for an action to be executed when an OfferService entry has been sent.
   */
  using TimerSendActionType = vac::language::UniqueFunction<void()>;
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  ServiceDiscoveryServerContextInterface() noexcept = default;
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryServerContextInterface() noexcept = default;

  ServiceDiscoveryServerContextInterface(ServiceDiscoveryServerContextInterface const&) = delete;
  ServiceDiscoveryServerContextInterface(ServiceDiscoveryServerContextInterface&&) = delete;
  ServiceDiscoveryServerContextInterface& operator=(ServiceDiscoveryServerContextInterface const&) & = delete;
  ServiceDiscoveryServerContextInterface& operator=(ServiceDiscoveryServerContextInterface&&) & = delete;

  /*!
   * \brief Checks the current network state
   *
   * \return true of network is up, false otherwise
   *
   * \pre          -
   * \context      ANY
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual bool IsNetworkUp() const = 0;

  /*!
   * \brief Checks the current service state
   *
   * \return true of service is requested, false if released
   *
   * \pre          -
   * \context      Init, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual bool IsServiceUp() const = 0;

  /*!
   * \brief request a state change
   *
   * \param[in] handle The handle for the requested next state.
   *
   * \pre          -
   * \context      Timer, Network, App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual void RequestStateChange(ServiceDiscoveryServerStateHandle const handle) = 0;

  /*!
   * \brief returns the requested next state set by \see RequestStateChange
   *
   * \return the requested next state
   *
   * \pre          -
   * \context      Timer, Network, App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual ServiceDiscoveryServerStateHandle GetRequestedNextState() const = 0;

  /*!
   * \brief Gets the configuration maximum number of repetitions in Repetition Phase.
   *
   * \return The configuration maximum number of repetitions.
   *
   * \pre          -
   * \context      Timer
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual std::uint32_t GetInitialRepetitionMaxCount() const = 0;

  /*!
   * \brief Schedule a multicast OfferService message from initial wait phase.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   */
  virtual void ScheduleInitialMulticastOfferServiceMessage() = 0;

  /*!
   * \brief Schedules sending a unicast OfferService SOME/IP SD message after the specified delay.
   *
   * \param[in] to_address  An IP address identifying the destination.
   * \param[in] to_port     A UDP port identifying the destination.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   *
   */
  virtual void SendUnicastOfferServiceMessage(someip_daemon_core::IpAddress const& to_address,
                                              amsr::net::ip::Port const& to_port) noexcept = 0;

  /*!
   * \brief Schedules sending a periodic OfferService SOME/IP SD message.
   *
   * \pre          -
   * \context      Timer
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   */
  virtual void SchedulePeriodicOffer() = 0;

  /*!
   * \brief Removes a previously scheduled periodic OfferService SOME/IP SD message from being sent.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual void UnschedulePeriodicOffer() = 0;

  /*!
   * \brief Removes a previously scheduled multicast OfferService SOME/IP SD message from being sent.
   * \details The OfferService was scheduled in the initial wait phase.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  virtual void UnscheduleInitialMulticastOffer() = 0;

  /*!
   * \brief Schedules sending a multicast StopOfferService SOME/IP SD message.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   */
  virtual void SendMulticastStopOfferServiceMessage() = 0;

  /*!
   * \brief      Schedule repetition phase Offer Service messages.
   *
   * \pre        -
   * \context    Timer
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   * \vprivate
   */
  virtual void ScheduleOfferService() = 0;

  /*!
   * \brief      Unschedule repetition phase Offer Service messages.
   *
   * \pre        -
   * \context    Timer, App, Network
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   * \vprivate
   */
  virtual void UnscheduleOfferService() = 0;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_INTERFACE_H_

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
/**        \file  service_discovery_server_context.h
 *        \brief  Service Discovery Server State Machine Context
 *
 *      \details  Context class contains all required interfaces to be called by any state machine state
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_context_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_state_owner.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace server {

/*!
 * \brief ServiceDiscoveryServerContext Implementation of ServiceDiscoveryServerContextInterface
 */
class ServiceDiscoveryServerContext final : public ServiceDiscoveryServerContextInterface {
 public:
  /*!
   * \brief Constructor
   *
   * \param[in] state_owner               A StateOwner.
   * \param[in] address                   The IP address of this server service instance.
   * \param[in] service_discovery_config  Provided Service service discovery configuration.
   * \param[in] message_builder           An SD Server message builder.
   * \param[in] message_scheduler         A message scheduler.
   *
   * \pre          -
   * \context      Init
   * \steady       FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  ServiceDiscoveryServerContext(
      ServiceDiscoveryServerStateOwner& state_owner,
      configuration::ConfigurationTypesAndDefs::ProvidedServiceInstanceServiceDiscovery const& service_discovery_config,
      ServiceDiscoveryServerMessageBuilder& message_builder,
      message::scheduler::SchedulerInterface* message_scheduler) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryServerContext() noexcept override = default;

  ServiceDiscoveryServerContext() = delete;
  ServiceDiscoveryServerContext(ServiceDiscoveryServerContext const&) = delete;
  ServiceDiscoveryServerContext(ServiceDiscoveryServerContext&&) = delete;
  ServiceDiscoveryServerContext& operator=(ServiceDiscoveryServerContext const&) & = delete;
  ServiceDiscoveryServerContext& operator=(ServiceDiscoveryServerContext&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Checks the current network state
   *
   * \return true if network is up, false otherwise
   *
   * \pre          -
   * \context      ANY
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  bool IsNetworkUp() const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Checks the current service state
   *
   * \return true if service is requested, false if released
   *
   * \pre          -
   * \context      Init, Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  bool IsServiceUp() const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
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
  void RequestStateChange(ServiceDiscoveryServerStateHandle const handle) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief return the requested next state set by \see RequestStateChange
   *
   * \return the requested next state
   *
   * \pre          -
   * \context      Timer, Network, App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  ServiceDiscoveryServerStateHandle GetRequestedNextState() const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
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
  std::uint32_t GetInitialRepetitionMaxCount() const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Schedule a multicast OfferService message from initial wait phase.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \steady       FALSE
   * \synchronous  FALSE
   */
  void ScheduleInitialMulticastOfferServiceMessage() override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
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
   * \steady       TRUE
   *
   * \trace SPEC-4981656, SPEC-4981658
   */
  void SendUnicastOfferServiceMessage(someip_daemon_core::IpAddress const& to_address,
                                      amsr::net::ip::Port const& to_port) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Schedules sending a periodic OfferService SOME/IP SD message.
   *
   * \pre          -
   * \context      Timer
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   *
   * \trace SPEC-4981651, SPEC-4981652, SPEC-10144706, SPEC-10144707
   */
  void SchedulePeriodicOffer() override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Removes a previously scheduled periodic OfferService SOME/IP SD message from being sent.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \steady       FALSE
   * \synchronous  TRUE
   */
  void UnschedulePeriodicOffer() override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Removes a previously scheduled multicast OfferService SOME/IP SD message from being sent.
   *        The OfferService was scheduled in the initial wait phase.
   *
   * \pre          -
   * \context      App, Network
   * \reentrant    FALSE
   * \steady       FALSE
   * \synchronous  TRUE
   */
  void UnscheduleInitialMulticastOffer() override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief      Schedule repetition phase Offer Service messages.
   * \pre        -
   * \context    Timer
   * \threadsafe FALSE
   * \steady     FALSE
   * \reentrant  FALSE
   * \vprivate
   * \trace SPEC-10144693, SPEC-10144694
   */
  void ScheduleOfferService() override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief      Unschedule repetition phase Offer Service messages.
   * \pre        -
   * \context    Timer, App, Network
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   * \vprivate
   */
  void UnscheduleOfferService() override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Schedules sending a multicast StopOfferService SOME/IP SD message.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   */
  void SendMulticastStopOfferServiceMessage() override;

 private:
  /*!
   * \brief state owner
   */
  ServiceDiscoveryServerStateOwner& state_owner_;
  /*!
   * \brief The corresponding service discovery configuration.
   */
  configuration::ConfigurationTypesAndDefs::ProvidedServiceInstanceServiceDiscovery const service_discovery_config_;
  /*!
   * \brief SD Server message builder
   */
  ServiceDiscoveryServerMessageBuilder& message_builder_;
  /*!
   * \brief A message scheduler.
   */
  message::scheduler::SchedulerInterface* message_scheduler_;
  /*!
   * \brief Requested next state.
   */
  ServiceDiscoveryServerStateHandle requested_next_state_{ServiceDiscoveryServerStateHandle::kDownPhase};

  /*!
   * \brief The minimum delay of a response to a multicast request.
   */
  std::chrono::nanoseconds const request_response_delay_min_{service_discovery_config_.request_response_delay_min_};
  /*!
   * \brief The maximum delay of a response to a multicast request
   */
  std::chrono::nanoseconds const request_response_delay_max_{service_discovery_config_.request_response_delay_max_};
  /*!
   * \brief The minimum delay for sending an offer message within the wait phase.
   */
  std::chrono::nanoseconds const initial_wait_delay_min_{service_discovery_config_.initial_delay_min_};
  /*!
   * \brief The maximum delay for sending an offer message within the wait phase.
   */
  std::chrono::nanoseconds const initial_wait_delay_max_{service_discovery_config_.initial_delay_max_};
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_H_

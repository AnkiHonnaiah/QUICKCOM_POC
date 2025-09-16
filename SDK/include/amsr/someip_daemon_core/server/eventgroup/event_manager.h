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
/**        \file  event_manager.h
 *        \brief  Event Manager class for someip daemon.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <unordered_map>
#include <utility>

#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_daemon_core/address_validator/address_validator.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state_change_handler.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/server/eventgroup/event_manager_interface.h"
#include "amsr/someip_daemon_core/server/eventgroup/eventgroup_subscription.h"
#include "amsr/someip_daemon_core/server/server_event_handler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/ip_endpoint_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/optional.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {
namespace eventgroup {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief Internal class within the server statemachine to manage un/subscription for eventgroups.
 * \trace SPEC-4981695
 */
class EventManager final : public EventgroupSubscriptionExpiryHandler, public EventManagerInterface {
 public:
  /*!
   * \brief Message Builder type for SD server messages.
   */
  using ServiceDiscoveryServerMessageBuilder =
      amsr::someip_daemon_core::service_discovery::state_machine::server::ServiceDiscoveryServerMessageBuilder;

  /*!
   * \brief Scheduler type used to plan outgoing SD messages.
   */
  using SchedulerInterface = amsr::someip_daemon_core::service_discovery::message::scheduler::SchedulerInterface;

  /*!
   * \brief Type alias for the service instance communication type.
   */
  using ServiceInstanceCommunicationType = configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType;

  /*!
   * \brief A container of eventgroup subscriptions.
   */
  using EventgroupSubscriptionContainer = ara::core::Vector<amsr::UniquePtr<EventgroupSubscription>>;

  /*!
   * \brief A map of EventgroupSubscriptionContainer to Eventgroup IDs.
   */
  using EventgroupSubscriptionMap = std::unordered_map<
      someip_protocol::internal::EventgroupId, EventgroupSubscriptionContainer,
      std::hash<someip_protocol::internal::EventgroupId>, std::equal_to<someip_protocol::internal::EventgroupId>,
      ara::core::PolymorphicAllocator<
          std::pair<someip_protocol::internal::EventgroupId const, EventgroupSubscriptionContainer>>>;

  /*!
   * \brief Constructor.
   *
   * \param[in] timer_manager         A timer manager.
   * \param[in] local_network         Local network.
   * \param[in] config                A configuration.
   * \param[in] instance_config       Provided Service configuration.
   * \param[in] communication_type    The communication type.
   * \param[in] message_builder       An SD Server message builder.
   * \param[in] message_scheduler     A message scheduler.
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  EventManager(amsr::steady_timer::TimerManagerInterface* timer_manager, ::amsr::net::ip::Network const& local_network,
               configuration::Configuration const* config,
               configuration::Configuration::IpcChannel::ProvidedServiceInstance const* instance_config,
               ServiceInstanceCommunicationType communication_type,
               ServiceDiscoveryServerMessageBuilder const& message_builder,
               SchedulerInterface* message_scheduler) noexcept;

  EventManager() = delete;
  EventManager(EventManager const&) = delete;
  EventManager& operator=(EventManager const&) = delete;
  EventManager(EventManager&&) = delete;
  EventManager& operator=(EventManager&&) & = delete;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Destructor.
   *
   * \pre          -
   * \context      Shutdown
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  ~EventManager() noexcept override;

  /*!
   * \brief Adds a new subscriber for a given eventgroup.
   *
   * \param[in] from_address  An IP address identifying the subscriber.
   * \param[in] from_port     A port identifying the subscriber.
   * \param[in] entry         A container of entries which will be sent in the SOME/IP SD message.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   * \trace SPEC-10144822
   * \trace SPEC-10144823
   * \trace SPEC-10144825
   * \trace SPEC-10144826
   */
  void SubscribeEventgroupEntry(
      someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry) noexcept;

  /*!
   * \brief Removes a subscriber for a given eventgroup.
   *
   * \param[in] from_address  An IP address identifying the subscriber.
   * \param[in] from_port     A port identifying the subscriber.
   * \param[in] entry         A container of entries which will be sent in the SOME/IP SD message.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void UnSubscribeEventgroupEntry(
      someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
      amsr::someip_daemon_core::service_discovery::message::entries::StopSubscribeEventgroupEntry const&
          entry) noexcept;

  /*!
   * \brief Removes all eventgroup subscribers.
   *
   * \pre          -
   * \context      Any
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void UnsubscribeAllEventgroupEntries() noexcept;

  /*!
   * \brief Removes all event group subscription sent from from_address.
   * \param[in] from_address  An IP address identifying sender's IP address
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void UnsubscribeAllEventgroupsOnReboot(someip_daemon_core::IpAddress const& from_address) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Cancels eventgroup subscription if the TCP connection broke unexpectedly.
   *
   * \param[in] tcp_address           TCP address of the subscription.
   * \param[in] tcp_port              TCP port of the subscription.
   * \param[in] eventgroup_id         Eventgroup Id of the new subscription
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void CancelEventgroupSubscriptionUponTcpDisconnection(
      someip_daemon_core::IpAddress const& tcp_address, amsr::net::ip::Port const tcp_port,
      amsr::someip_protocol::internal::EventgroupId const eventgroup_id) noexcept override;

  /*!
   * \brief Register remote client communication.
   *
   * \param[in] remote_client_communication  Remote Client Communication.
   *
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  void RegisterRemoteClientCommunication(std::shared_ptr<someip_daemon_core::server::ServerEventHandlerInterface> const&
                                             remote_client_communication) noexcept;

 private:
  /*!
   * \brief Type alias for configuration types
   */
  using ConfigurationTypesAndDefs = configuration::ConfigurationTypesAndDefs;

  /*!
   * \brief Type alias for a tuple storing a pointer to a provided service instance event group configuration and a
   * boolean flag indicating if the configuration is valid.
   */
  using EventGroupConfigResult = std::pair<ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroup const*, bool>;

  /*!
   * \brief Handles a multicast-only eventgroup subscription request.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port A UDP port identifying sender's port.
   * \param[in] entry A SOME/IP SD SubscribeEventgroup entry.
   * \param[in] event_group_config The configuration of the eventgroup.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  void HandleMulticastOnlySubscribeEventgroupEntry(
      someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry,
      ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroup const* event_group_config) noexcept;

  /*!
   * \brief Handles a unicast eventgroup subscription request.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port A UDP port identifying sender's port.
   * \param[in] entry A SOME/IP SD SubscribeEventgroup entry.
   * \param[in] event_group_config The configuration of the eventgroup.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  void HandleUnicastSubscribeEventgroupEntry(
      someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry,
      ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroup const* event_group_config) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called on eventgroup subscription expiration.
   *
   * \param[in] egs An eventgroup subscription which expired.
   *
   * \return Always false (for OneShot timers, return value is ignored).
   *
   * \pre          -
   * \context      Timer
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void OnSubscribeEventgroupExpired(EventgroupSubscription* egs) override;

  /*!
   * \brief Handles a new eventgroup subscription.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port A UDP port identifying sender's port.
   * \param[in] entry A SOME/IP SD SubscribeEventgroup entry.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   *
   * \trace SPEC-4980076, SPEC-4980111, SPEC-4981627, SPEC-4981699, SPEC-4981709, SPEC-10144838, SPEC-10144842
   */
  void NewEventgroupSubscription(
      someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry) noexcept;

  /*!
   * \brief Registers a new eventgroup subscription.
   *
   * \param[in] from_address An IP address identifying sender's IP address.
   * \param[in] from_port A UDP port identifying sender's port.
   * \param[in] entry The received SubscribeEventgroup entry.
   *
   * \pre          -
   * \context      Timer
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   * \trace SPEC-4980076, SPEC-4980111, SPEC-4981627, SPEC-4981699, SPEC-10144828, SPEC-10144836
   */
  void RegisterNewSubscription(
      someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const from_port,
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry) noexcept;

  /*!
   * \brief Handles a renewal of an eventgroup subscription.
   *
   * \param[in] from_address An IP address identifying sender's IP address.
   * \param[in] from_port A UDP port identifying sender's port.
   * \param[in] entry A SOME/IP SD SubscribeEventgroup entry.
   * \param[in] eventgroup_subscription The eventgroup subscription to be renewed.
   * \param[in] event_group_config The configuration of the eventgroup.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       TRUE
   *
   * \trace SPEC-4981656, SPEC-4981699, SPEC-10144838, SPEC-10144842
   */
  void RenewEventgroupSubscription(
      someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry,
      EventgroupSubscription* eventgroup_subscription,
      ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroup const* event_group_config) const noexcept;

  /*!
   * \brief Send a NACK to the given Eventgroup subscription entry.
   *
   * \param[in] from_address  An IP address identifying sender's IP address.
   * \param[in] from_port     A UDP port identifying sender's port.
   * \param[in] entry_id         A SOME/IP SD SubscribeEventgroup entry.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  FALSE
   * \steady       FALSE
   *
   * \trace SPEC-4981630
   * \trace SPEC-10144598
   */
  void RejectEventgroupSubscription(
      someip_daemon_core::IpAddress const& from_address, amsr::net::ip::Port const& from_port,
      amsr::someip_daemon_core::service_discovery::message::entries::EventgroupEntryId const& entry_id) const noexcept;

  /*!
   * \brief Cancels an eventgroup subscription.
   *
   * \param[in] entry         A SOME/IP SD SubscribeEventgroup entry.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   * \trace SPEC-10144843
   */
  void CancelEventgroupSubscription(
      someip_daemon_core::IpAddress const&, amsr::net::ip::Port const&,
      amsr::someip_daemon_core::service_discovery::message::entries::StopSubscribeEventgroupEntry const&
          entry) noexcept;

  /*!
   * \brief Check if the given SOME/IP SD message is compatible with the configuration:
   * - Communication type is kSdAndCommunication
   * - eventgroup id exists.
   * - TCP/UDP options exist as required for the given eventgroup.
   *
   * \param[in] entry    The Subscribe Eventgroup SD message entry.
   *
   * \return Tuple of
   * - a pointer to the related event group configuration structure. Only valid if the second tuple value is true.
   * - a boolean flag indicating if the entry/options are consistent with the configuration. Otherwise false.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   *
   * \trace SPEC-4981632
   * \trace SPEC-10144600
   */
  // VECTOR NC Metric-HIS.LEVEL: MD_SomeIpDaemon_Metric-HIS.LEVEL_his_level
  EventGroupConfigResult CheckAgainstConfiguration(
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry)
      const noexcept;

  /*!
   * \brief Check if the given TCP endpoint is compatible with the configuration
   *
   * \param[in] entry    The Subscribe Eventgroup SD message entry.
   *
   * \return true if the endpoint is compatible with the configuration
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   *
   * \trace SPEC-4981632
   * \trace SPEC-10144600
   */
  bool ValidateTcpEndpoint(
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry)
      const noexcept;

  /*!
   * \brief Check if the given UDP endpoint is compatible with the configuration
   *
   * \param[in] entry    The Subscribe Eventgroup SD message entry.
   *
   * \return true if the endpoint is compatible with the configuration
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   *
   * \trace SPEC-4981632
   * \trace SPEC-10144600
   */
  bool ValidateUdpEndpoint(
      amsr::someip_daemon_core::service_discovery::message::entries::SubscribeEventgroupEntry const& entry)
      const noexcept;

  /*!
   * \brief Return the number of all eventgroup subscriptions
   *
   * \return The the number of all eventgroup subscriptions
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  std::size_t GetNumberOfAllEventgroupSubscriptions() const noexcept;

  /*!
   * \brief Checks whether the endpoint options represent a multicast-only subscription
   * \details The multicast-only eventgroup subscriptions are the ones with empty udp and tcp endpoint options.
   *
   * \return True if the endpoint options represent a multicast-only subscription, False otherwise
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  static bool IsMulticastOnlyEventgroupSubscription(
      ara::core::Optional<service_discovery::message::options::IpEndpointOption> const udp_endpoint,
      ara::core::Optional<service_discovery::message::options::IpEndpointOption> const tcp_endpoint) noexcept;

  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service configuration.
   *
   * \param[in] instance_config The instance configuration
   *
   * \return A string representing the logger context identifier for given SOME/IP service configuration.
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  static ara::core::String GetLoggerContextId(
      configuration::Configuration::IpcChannel::ProvidedServiceInstance const* instance_config) noexcept;

  /*!
   * \brief A map of active unicast eventgroup subscriptions to Eventgroup IDs.
   */
  EventgroupSubscriptionMap unicast_eventgroup_subscriptions_{};

  /*!
   * \brief A map of active multicast-only eventgroup subscriptions to Eventgroup IDs.
   */
  EventgroupSubscriptionMap multicast_only_eventgroup_subscriptions_{};
  /*!
   * \brief A shared pointer to a remote client communication for event subscription handling.
   */
  std::shared_ptr<someip_daemon_core::server::ServerEventHandlerInterface> remote_client_communication_{nullptr};

  /*!
   * \brief A timer manager.
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;
  /*!
   * \brief A configuration.
   */
  configuration::Configuration const* config_;
  /*!
   * \brief The corresponding configuration of this server service instance.
   */
  configuration::Configuration::IpcChannel::ProvidedServiceInstance const* instance_config_;
  /*!
   * \brief The communication type of this server service instance.
   */
  ServiceInstanceCommunicationType const communication_type_;
  /*!
   * \brief SD Server message builder
   */
  ServiceDiscoveryServerMessageBuilder const& message_builder_;
  /*!
   * \brief A message scheduler.
   */
  SchedulerInterface* message_scheduler_;
  /*!
   * \brief Network endpoint address validator.
   */
  address_validator::AddressValidator const address_validator_;
  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription, GetLoggerContextId(instance_config_)};
};

}  // namespace eventgroup
}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MANAGER_H_

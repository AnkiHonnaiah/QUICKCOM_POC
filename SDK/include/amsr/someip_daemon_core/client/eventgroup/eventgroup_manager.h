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
/**        \file  eventgroup_manager.h
 *        \brief  Client eventgroup manager
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_manager_context.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_sd_message_builder.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_machine.h"
#include "amsr/someip_daemon_core/client/eventgroup/multicast_eventgroup_listener_interface.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/active_offer_service_entry.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/abort.h"
#include "ara/core/map.h"
#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief Client eventgroup manager.
 * \details Class that handles all events/eventgroups subscriptions for client side.
 *
 * \tparam ConnectionManagerType Type of Connection Manager class.
 */
template <typename ConnectionManagerType>
class EventgroupManager final : public EventgroupStateMachineContext {
 public:
  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;

  /*!
   * \brief A container of SOME/IP eventgroup identifiers.
   */
  using EventgroupIdContainer = ara::core::Vector<someip_protocol::internal::EventgroupId>;

  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

  /*!
   * \brief Constructor of EventgroupManager.
   *
   * \param[in] service_deployment             Service deployment configuration.
   * \param[in] instance_id                    A SOME/IP service instance identifier.
   * \param[in] udp_endpoint                   An optional UDP endpoint. Only for forwarding to the
   *                                           message builder.
   * \param[in] custom_subscription_endpoints  An optional Service Address. Only for forwarding to the
   *                                           message builder.
   * \param[in] required_eventgroups           Required eventgroups timing configuration
   * \param[in] remote_server                  An eventgroup manager context (the remote server)
   * \param[in] timer_manager                  Timer Manager instance
   * \param[in] message_scheduler              SD Message scheduler (to schedule sending out Subscribe/StopSubscribe
   *                                           messages)
   * \param[in] multicast_eventgroup_listener  A listener to multicast eventgroups
   *
   * \context Init
   * \pre     -
   * \steady  FALSE
   * \trace SPEC-4981695
   * \internal
   * - Reserve the eventgroup state machine container size to the size of passed container.
   * - For every eventgroup ID from the eventgroups container,
   *   - Register a new state machine instance to eventgroup state machine container.
   *   - If eventgroup resubscription is configured, enable eventgroup resubscription in the
   *     state machine.
   * \endinternal
   */
  EventgroupManager(
      configuration::types::SomeIpServiceInterfaceDeployment const& service_deployment,
      amsr::someip_protocol::internal::InstanceId const instance_id,
      ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> const& udp_endpoint,
      configuration::types::ServiceAddress const& custom_subscription_endpoints,
      RequiredEventgroupsContainer const& required_eventgroups, EventgroupManagerContext* remote_server,
      amsr::steady_timer::TimerManagerInterface* timer_manager,
      service_discovery::message::scheduler::SchedulerInterface& message_scheduler,
      MulticastEventgroupListenerInterface& multicast_eventgroup_listener) noexcept
      : EventgroupStateMachineContext(),
        remote_server_{remote_server},
        message_scheduler_{message_scheduler},
        message_builder_{service_deployment, instance_id, udp_endpoint, required_eventgroups,
                         custom_subscription_endpoints},
        multicast_eventgroup_listener_{multicast_eventgroup_listener},
        logger_{someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
                someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
                logging::LoggerPrefixGenerator::GetLoggerPrefix("EventgroupManager", service_deployment.deployment_id,
                                                                instance_id)} {
    assert(timer_manager != nullptr);
    logger_.LogVerbose(
        [&required_eventgroups](ara::log::LogStream& s) {
          s << "Number of eventgroups ";
          s << required_eventgroups.size();
        },
        static_cast<char const*>(__func__), __LINE__);

    for (RequiredEventgroupsContainer::const_reference eventgroup : required_eventgroups) {
      eventgroup_state_machines_.emplace_back(
          memory::MemoryUtilities::CreateUniquePtr<EventgroupStateMachine>(eventgroup.first, this));

      // If configured, enable eventgroup re-subscription mechanism
      if (eventgroup.second.has_value() && eventgroup.second.value().subscribe_eventgroup_retry_config.has_value()) {
        eventgroup_state_machines_.back()->EnableEventgroupReSubscription(
            eventgroup.second.value().subscribe_eventgroup_retry_config.value(), timer_manager);
      }
    }
  }

  /*!
   * \brief Default destructor.
   *
   * \context Shutdown
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  virtual ~EventgroupManager() noexcept = default;

  EventgroupManager(EventgroupManager const&) = delete;
  EventgroupManager(EventgroupManager&&) = delete;
  EventgroupManager& operator=(EventgroupManager const&) & = delete;
  EventgroupManager& operator=(EventgroupManager&&) & = delete;

  /*!
   * \brief Gets the current subscription state of the given eventgroup.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return Current eventgroup subscription state.
   * \context App
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - Search for the eventgroup state machine matching to the given eventgroup ID.
   * - Return the subscription state of a found eventgroup ID.
   * \endinternal
   */
  EventSubscriptionState GetEventgroupSubscriptionState(
      someip_protocol::internal::EventgroupId const eventgroup_id) const {
    EventgroupManager::EventgroupStateMachineContainer::const_iterator const kIt{GetEventgroup(eventgroup_id)};
    assert(kIt != eventgroup_state_machines_.end());
    return (*kIt)->GetSubscriptionState();
  }

  /*!
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param[in] entry        The offer service SD entry.
   * \param[in] is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   *
   * \context App
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - Store locally the offer entry
   * - Store locally the multicast flag
   * - Send OfferService entry to all eventgroup state machines with/without multicast.
   * \endinternal
   */
  void OnOfferService(service_discovery::state_machine::client::ActiveOfferServiceEntry const& entry,
                      bool const is_multicast) {
    active_offer_entry_.emplace(entry);

    is_active_offer_multicast_ = is_multicast;
    // PTP-B-SomeIpDaemon-EventgroupManager_OnOfferService
    // VECTOR NC AutosarC++17_10-A18.1.5: MD_SomeIpDaemon_AutosarC++17_10-A18.1.5_const_ref_unique_ptr
    static_cast<void>(
        std::for_each(eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
                      [is_multicast](EventgroupStateMachinePtr const& sm) { sm->OnOfferService(is_multicast); }));
    // PTP-E-SomeIpDaemon-EventgroupManager_OnOfferService
  }

  /*!
   * \brief Called when a matching StopOfferService entry has been received.
   * \context App
   * \pre     -
   * \steady  FALSE
   * \trace SPEC-4981666, SPEC-10144837
   *
   * \internal
   * - Reset the stored offer entry
   * - Send StopOfferService entry to all eventgroup state machine with/without multicast.
   * \endinternal
   */
  void OnStopOfferService() {
    active_offer_entry_.reset();

    // VECTOR NC AutosarC++17_10-A18.1.5: MD_SomeIpDaemon_AutosarC++17_10-A18.1.5_const_ref_unique_ptr
    static_cast<void>(std::for_each(eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
                                    [](EventgroupStateMachinePtr const& sm) { sm->OnStopOfferService(); }));
  }

  /*!
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \context App
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - Search for the eventgroup state machine matching to the given eventgroup ID.
   * - Send Subscribe Eventgroup entry to the state machine found.
   * \endinternal
   */
  void OnSubscribe(someip_protocol::internal::EventgroupId const eventgroup_id) {
    EventgroupManager::EventgroupStateMachineContainer::const_iterator const kIt{GetEventgroup(eventgroup_id)};
    assert(kIt != eventgroup_state_machines_.end());
    (*kIt)->OnSubscribe();
  }

  /*!
   * \brief Called when a SOME/IP eventgroup has been unsubscribed.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \context App
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - Search for the eventgroup state machine matching to the given eventgroup ID.
   * - Send Stop Subscribe Eventgroup entry to the state machine found.
   * \endinternal
   */
  void OnUnsubscribe(someip_protocol::internal::EventgroupId const eventgroup_id) {
    EventgroupManager::EventgroupStateMachineContainer::const_iterator const kIt{GetEventgroup(eventgroup_id)};
    assert(kIt != eventgroup_state_machines_.end());
    (*kIt)->OnUnsubscribe();
  }

  /*!
   * \brief Called when a matching SubscribeEventgroupAck entry has been received.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \param[in] multicast_address A multicast IP address assigned to the SOME/IP eventgroup by server.
   * \param[in] multicast_port A multicast UDP port assigned to the SOME/IP eventgroup by server.
   * \context   App
   * \pre       -
   * \steady    TRUE
   *
   * \internal
   * - If the service is available
   *   - If an UDP endpoint is present in the active offer or no multicast endpoint option is
   *     present in the ACK entry
   *     - Find the state machine for the received eventgroup id
   *     - Send Subscribe Eventgroup Acknowledgement entry to the state machine found.
   * - Otherwise log an error and discard the entry
   * \endinternal
   */
  void OnSubscribeEventgroupAck(someip_protocol::internal::EventgroupId const eventgroup_id,
                                ara::core::Optional<someip_daemon_core::IpAddress> const& multicast_address,
                                someip_protocol::internal::Port const multicast_port) {
    // Forward the Ack to the state machine if either:
    //  - There is an active offer available
    //  - An UDP endpoint is present in the active offer (otherwise no multicast messages must be received)
    //  - No multicast endpoint is provided in the ACK
    if (IsServiceAvailable()) {
      bool const multicast_endpoint_available{(multicast_address.has_value()) &&
                                              (multicast_port != configuration::Configuration::kInvalidPort)};

      if ((active_offer_entry_.value().offer_entry.udp_endpoint.has_value()) || (!multicast_endpoint_available)) {
        EventgroupManager::EventgroupStateMachineContainer::const_iterator const kIt{GetEventgroup(eventgroup_id)};
        assert(kIt != eventgroup_state_machines_.end());

        (*kIt)->OnSubscribeEventgroupAck(multicast_address, multicast_port);
      } else {
        // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "Received SubscribeEventgroupAck with multicast address but no UDP endpoint was present in the "
                   "active OfferService. Please check the configuration of the server for consistency.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
      logger_.LogError([](ara::log::LogStream& s) { s << "Received SubscribeEventgroupAck with no active offer"; },
                       static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Called when a matching StopSubscribeEventgroupAck entry has been received.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \context App
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - Search for the eventgroup state machine matching to the given eventgroup ID.
   * - Send Subscribe Eventgroup Negative Acknowledgement entry to the state machine found.
   * \endinternal
   */
  void OnSubscribeEventgroupNack(someip_protocol::internal::EventgroupId const eventgroup_id) {
    EventgroupManager::EventgroupStateMachineContainer::const_iterator const kIt{GetEventgroup(eventgroup_id)};
    assert(kIt != eventgroup_state_machines_.end());
    (*kIt)->OnSubscribeEventgroupNack();
  }

  /*!
   * \brief Called when a connection to the server has been established.
   * \context Network
   * \pre      -
   * \steady  FALSE
   *
   * \internal
   * - Forward connection establishment event to all eventgroup state machine.
   * \endinternal
   */
  void OnConnectionEstablished() {
    // PTP-B-SomeIpDaemon-EventgroupManager_OnConnectionEstablished
    // VECTOR NC AutosarC++17_10-A18.1.5: MD_SomeIpDaemon_AutosarC++17_10-A18.1.5_const_ref_unique_ptr
    static_cast<void>(std::for_each(eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
                                    [](EventgroupStateMachinePtr const& sm) { sm->OnConnectionEstablished(); }));
    // PTP-E-SomeIpDaemon-EventgroupManager_OnConnectionEstablished
  }

  /*!
   * \brief Called when a connection to the server has been closed.
   * \context Network
   * \pre      -
   * \steady  FALSE
   *
   * \internal
   * - Call OnConnectionClosed on all event group state machines
   * \endinternal
   */
  void OnConnectionClosed() {
    // Notify the state machines about connection being closed
    // VECTOR NC AutosarC++17_10-A18.1.5: MD_SomeIpDaemon_AutosarC++17_10-A18.1.5_const_ref_unique_ptr
    static_cast<void>(std::for_each(eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
                                    [](EventgroupStateMachinePtr const& sm) { sm->OnConnectionClosed(); }));
  }

  /*!
   * \brief Registers a local TCP endpoint, used for sending subscription requests
   *
   * \param[in] local_tcp_network_endpoint The local tcp endpoint
   *
   * \pre     -
   * \context App, Network
   * \steady  FALSE
   *
   * \trace SPEC-6334968
   */
  void RegisterLocalTcpNetworkEndpoint(
      configuration::types::NetworkEndpoint const& local_tcp_network_endpoint) noexcept {
    message_builder_.RegisterLocalTcpNetworkEndpoint(local_tcp_network_endpoint);
  }

 protected:
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether the service is available or not.
   *
   * \return true if the service is available and false otherwise.
   * \context App
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - Return service availability status.
   * \endinternal
   */
  bool IsServiceAvailable() const override { return active_offer_entry_.has_value(); }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether the client is connected to the server or not.
   *
   * \return true if the connection to the server is available, false otherwise.
   * \pre     -
   * \context     ANY
   * \synchronous true
   * \steady      FALSE
   */
  bool IsConnected() const override { return remote_server_->IsConnected(); }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Initiates the transmission of a SubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \pre     -
   * \context App
   * \steady  FALSE
   *
   * \internal
   * - Build SubscribeEventgroup Entry from the active offer (use exact offered instance ID).
   * - Schedule the SD entry to be sent.
   * \endinternal
   */
  void SendSubscribeEventgroup(someip_protocol::internal::EventgroupId const eventgroup_id) override {
    assert(active_offer_entry_.has_value());

    someip_daemon_core::IpAddress const sd_address{active_offer_entry_.value().sd_address};
    amsr::net::ip::Port const sd_port{active_offer_entry_.value().sd_port};

    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
    logger_.LogVerbose(
        [&sd_address, &sd_port, &eventgroup_id](ara::log::LogStream& s) {
          ara::core::String const sd_address_str{sd_address.ToString()};
          ara::core::StringView const sd_address_str_view{sd_address_str};
          s << "To <" << sd_address_str_view << "," << sd_port.port << ">"
            << " eventgroup 0x" << ara::log::HexFormat(eventgroup_id);
        },
        static_cast<char const*>(__func__), __LINE__);

    // Outgoing subscribe eventgroup messages shall use the exact eventgroup identifier as given in the offer message.
    service_discovery::message::entries::SubscribeEventgroupEntry const entry{
        message_builder_.MakeSubscribeEventgroupEntry(eventgroup_id)};
    std::chrono::nanoseconds const min_delay{std::chrono::nanoseconds::zero()};
    std::chrono::nanoseconds const max_delay{std::chrono::nanoseconds::zero()};

    someip_protocol::internal::IpAddress const to_address{sd_address.ToString()};
    message_scheduler_.ScheduleSubscribeEventgroupEntry(entry, min_delay, max_delay, {to_address, sd_port.port});
  }
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \pre     -
   * \context App
   * \steady  FALSE
   *
   * \internal
   * - Build Stop SubscribeEventgroup Entry from the active offer (use exact offered instance ID).
   * - Schedule the SD entry to be sent.
   * \endinternal
   */
  void SendStopSubscribeEventgroup(someip_protocol::internal::EventgroupId const eventgroup_id) override {
    assert(active_offer_entry_.has_value());

    someip_daemon_core::IpAddress const sd_address{active_offer_entry_.value().sd_address};
    amsr::net::ip::Port const sd_port{active_offer_entry_.value().sd_port};

    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
    logger_.LogVerbose(
        [&sd_address, &sd_port, &eventgroup_id](ara::log::LogStream& s) {
          ara::core::String const sd_address_str{sd_address.ToString()};
          ara::core::StringView const sd_address_str_view{sd_address_str};
          s << "To <" << sd_address_str_view << "," << sd_port.port << ">"
            << " eventgroup 0x" << ara::log::HexFormat(eventgroup_id);
        },
        static_cast<char const*>(__func__), __LINE__);

    // Outgoing stop subscribe eventgroup messages shall use the exact eventgroup identifier as given in the offer
    service_discovery::message::entries::StopSubscribeEventgroupEntry const entry{
        message_builder_.MakeStopSubscribeEventgroupEntry(eventgroup_id)};
    someip_protocol::internal::IpAddress const to_address{sd_address.ToString()};
    message_scheduler_.ScheduleStopSubscribeEventgroupEntry(entry, {to_address, sd_port.port});
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Starts listening for multicast UDP events of a given eventgroup.
   *
   * \param[in] multicast_address  A multicast IP address to which events of the given eventgroup will be sent.
   * \param[in] multicast_port     A UDP port number.
   *
   * \return true if we started listening to the given multicast address and false otherwise.
   * \context Network
   * \pre     The service must be available
   * \steady  FALSE
   *
   * \internal
   * - Create and initialize a listening status to true.
   * - Start listening multicast UDP events only for first subcription request.
   * - If multicast listening status is true.
   *   - Increment the eventgroup subscription count.
   * - Return the listening status.
   * \endinternal
   */
  bool StartListenForMulticastEventgroup(someip_daemon_core::IpAddress const& multicast_address,
                                         someip_protocol::internal::Port const multicast_port) override {
    bool result{true};
    // Only start listening to the given address only for the first ACK.
    if (eventgroup_subscriptions_ == 0U) {
      // Service must be available
      assert(IsServiceAvailable());

      // The active offer must contain a UDP endpoint, that will be used as sender address to the multicast channel
      assert(active_offer_entry_.value().offer_entry.udp_endpoint.has_value());

      // Get event sender address and port
      someip_daemon_core::IpAddress const sender_address{
          active_offer_entry_.value().offer_entry.udp_endpoint.value().address};
      someip_protocol::internal::Port const sender_port{
          active_offer_entry_.value().offer_entry.udp_endpoint.value().port.port};
      result = multicast_eventgroup_listener_.StartListenForMulticastEventgroup(multicast_address, multicast_port,
                                                                                sender_address, sender_port);
    }
    if (result) {
      ++eventgroup_subscriptions_;
    }
    return result;
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Stops listening for multicast UDP events of a given eventgroup.
   * \context Network
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - Decrement the eventgroup subscription count if there are subscription left.
   * - If all the eventgroup subscription are stopped.
   *   - Stop listing for multicast UDP events.
   * \endinternal
   */
  void StopListenForMulticastEventgroup() override {
    if (eventgroup_subscriptions_ > 0U) {
      --eventgroup_subscriptions_;
    }
    // If no more subsriptions available, stop listening for multicast (Only if we are already listening)
    if (eventgroup_subscriptions_ == 0U) {
      multicast_eventgroup_listener_.StopListenForMulticastEventgroup();
    }
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \context App | Network
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - Forward the trigger to the remote server.
   * \endinternal
   */
  void OnEventgroupSubscribed(someip_protocol::internal::EventgroupId const eventgroup_id) override {
    remote_server_->OnEventgroupSubscribed(eventgroup_id);
  }

  /*!
   * \brief An eventgroup state machine pointer.
   */
  using EventgroupStateMachinePtr = amsr::UniquePtr<EventgroupStateMachine>;

  /*!
   * \brief A container of eventgroup state machine pointers.
   */
  using EventgroupStateMachineContainer = ara::core::Vector<EventgroupStateMachinePtr>;

  /*!
   * \brief Returns an eventgroup state machine for the given SOME/IP eventgroup.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \return A const iterator pointing to an eventgroup state machine.
   * \context App
   * \pre     -
   * \steady  FALSE
   *
   * \internal
   * - For every state machine in the eventgroup state machine container.
   * - Assert that state machine is valid.
   * - Otherwise, return state machine matching to the given eventgroup ID.
   * \endinternal
   */
  EventgroupStateMachineContainer::const_iterator GetEventgroup(
      someip_protocol::internal::EventgroupId const eventgroup_id) const {
    // VECTOR NC AutosarC++17_10-A18.1.5: MD_SomeIpDaemon_AutosarC++17_10-A18.1.5_const_ref_unique_ptr
    EventgroupManager::EventgroupStateMachineContainer::const_iterator const kIt{
        std::find_if(eventgroup_state_machines_.cbegin(), eventgroup_state_machines_.cend(),
                     [eventgroup_id](EventgroupStateMachinePtr const& sm) {
                       assert(sm);
                       return sm->GetEventgroupId() == eventgroup_id;
                     })};
    return kIt;
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Checks the type of the current active offer
   * \return true if the current active offer is multicast
   * \context App
   * \pre     -
   * \steady  FALSE
   */
  bool IsActiveOfferMulticast() const override { return is_active_offer_multicast_; }

 private:
  /*!
   * \brief Remote Server.
   */
  EventgroupManagerContext* remote_server_;

  /*!
   * \brief A message scheduler.
   */
  amsr::someip_daemon_core::service_discovery::message::scheduler::SchedulerInterface& message_scheduler_;

  /*!
   * \brief A container of all eventgroup state machines.
   */
  EventgroupStateMachineContainer eventgroup_state_machines_{};

  /*!
   * \brief Counts how many eventgroup subscriptions are active.
   */
  std::size_t eventgroup_subscriptions_{0};

  /*!
   * \brief Indicates whether the offer has been received via multicast.
   */
  bool is_active_offer_multicast_{false};

  /*!
   * \brief Currently active service offer.
   */
  ara::core::Optional<amsr::someip_daemon_core::service_discovery::state_machine::client::ActiveOfferServiceEntry>
      active_offer_entry_{};

  /*!
   * \brief A builder for SOME/IP SD messages.
   */
  EventgroupSdMessageBuilder message_builder_;

  /*!
   * \brief A listener for multicast eventgroups
   */
  MulticastEventgroupListenerInterface& multicast_eventgroup_listener_;

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_MANAGER_H_

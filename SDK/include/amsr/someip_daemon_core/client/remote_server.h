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
/**        \file  remote_server.h
 *        \brief  Manages the event communication between local client and remote service.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <unordered_map>
#include <utility>
#include "amsr/someip_daemon_core/client/client_event_dispatcher.h"
#include "amsr/someip_daemon_core/client/eventgroup/event_multicast_listener.h"
#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_manager.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_manager_context.h"
#include "amsr/someip_daemon_core/client/eventgroup/remote_server_eventgroup.h"
#include "amsr/someip_daemon_core/client/remote_server_connection_manager.h"
#include "amsr/someip_daemon_core/client/remote_server_connection_manager_factory_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/port.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_event_group_timing_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_sd_client_service_instance_config.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_version.h"
#include "amsr/someip_daemon_core/configuration/util/configuration_types_utilities.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/logging/logging_utilities.h"
#include "amsr/someip_daemon_core/packet_handler/validators/local_incoming_packet_validator_error.h"
#include "amsr/someip_daemon_core/packet_router/application_packet_router_interface.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler_interface.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/active_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/service_discovery_client_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "ara/core/memory_resource.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/container/string_literals.h"
#include "vac/language/location.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals.
 */
using vac::container::literals::operator""_sv;

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief RemoteServer.
 * \details When adding or removing required definitions in TemplateConfiguration,
 * remember to also update documentation of TemplateConfiguration in class template
 * RemoteServerManager!
 *
 * \tparam ConnectionManagerType Type of ConnectionManager
 */
template <typename ConnectionManagerType>
class RemoteServer : public amsr::someip_daemon_core::client::eventgroup::EventgroupManagerContext,
                     public RemoteServerInterface {
 public:
  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;

  /*!
   * \brief Alias for the underlying type of EventSubscriptionState.
   */
  using EventSubscriptionStateUnderlyingType = std::underlying_type<eventgroup::EventSubscriptionState>::type;

  /*!
   * \brief Alias for SomeIpEventHandlerInterface shared pointer.
   */
  using SomeIpEventObserverPtr = std::shared_ptr<client::SomeIpEventHandlerInterface>;

  /*!
   * \brief Alias for PduEventHandlerInterface shared pointer.
   */
  using PduEventObserverPtr = std::shared_ptr<client::PduEventHandlerInterface>;

  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

  /*!
   * \brief  A timeout list for UDP Method message accumulation purposes.
   */
  using MethodsMessageAccumulationTimeoutMap = std::unordered_map<
      someip_protocol::internal::MethodId, std::chrono::nanoseconds, std::hash<someip_protocol::internal::MethodId>,
      std::equal_to<someip_protocol::internal::MethodId>,
      ara::core::PolymorphicAllocator<std::pair<someip_protocol::internal::MethodId const, std::chrono::nanoseconds>>>;

  RemoteServer() = delete;
  RemoteServer(RemoteServer const&) = delete;
  RemoteServer(RemoteServer&&) = delete;
  RemoteServer& operator=(RemoteServer const&) & = delete;
  RemoteServer& operator=(RemoteServer&&) & = delete;

  /*!
   * \brief Constructor of RemoteServer.
   *
   * \param[in] service_deployment                       The service deployment configuration.
   * \param[in] instance_id                              A SOME/IP service instance identifier.
   * \param[in] network_config                           The network configuration.
   * \param[in] required_eventgroups                     Required eventgroups timing configuration
   * \param[in] required_pdus                            List of required PDUs, needed for the multicast event listener.
   * \param[in] is_communication_only                    Flag to indicate if static SD is enabled.
   * \param[in] conman                                   A connection manager.
   * \param[in] packet_router                            The packet router.
   * \param[in] message_scheduler                        Message scheduler (used to schedule Subscribe / Unsubscribe SD
   *                                                     messages).
   * \param[in] timer_manager                            Time manager.
   * \param[in] reactor                                  Reactor, used to trigger SW events for connection cleaup.
   * \param[in] method_message_accumulation_timeouts     UDP message accumulation timeouts for methods
   * \param[in] custom_subscription_endpoints            Custom (optional) endpoints for eventgroup subscription
   * \param[in] remote_server_connection_manager_factory Remote server connection manager factory
   * \param[in] sec_com_config                           The SecCom configuration for the required service instance.
   * \param[in] service_discovery_client                 The service discovery client (SM) assigned to this RemoteServer
   *
   * \context Init
   * \steady  FALSE
   *
   * \note If a RemoteServer is constructed with SD configured, this construction may initiate connection establishment
   *       if there is an active OfferService registered in the SD Client.
   *       If the RemoteServer is constructed with SD disabled, the API InitializeStaticSD must be called from
   *       outside to trigger this connection establishment.
   *
   * \internal
   * - Create RemoteServerConnectionManager if either TCP or UDP connections are required
   * - Initialize Eventgroup Map
   * - Register the remote server in the packet router.
   * - If dynamic SD is available
   *   - Register into SD class to get the SD updates (offers, acks, nacks).
   *   - Initialize Eventgroup Manager.
   *   - Register this remote server in the service discovery client (SM).
   * - Register a SW event for cleanup after disconnection detection.
   * - In case the SW event registration in the reactor fails.
   *   - Log fatal and abort.
   * \endinternal
   */
  RemoteServer(
      configuration::types::SomeIpServiceInterfaceDeployment const& service_deployment,
      someip_protocol::internal::InstanceId const instance_id,
      configuration::types::RequiredNetworkEndpoint const& network_config,
      RequiredEventgroupsContainer const& required_eventgroups,
      configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const& required_pdus,
      bool const is_communication_only, ConnectionManager& conman,
      std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
      service_discovery::message::scheduler::SchedulerInterface* message_scheduler,
      amsr::steady_timer::TimerManagerInterface* timer_manager, osabstraction::io::reactor1::Reactor1Interface& reactor,
      MethodsMessageAccumulationTimeoutMap const& method_message_accumulation_timeouts,
      configuration::types::ServiceAddress const& custom_subscription_endpoints,
      amsr::UniquePtr<RemoteServerConnectionManagerFactoryInterface>&& remote_server_connection_manager_factory,
      configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config,
      service_discovery::state_machine::client::ServiceDiscoveryClientInterface* service_discovery_client) noexcept
      : amsr::someip_daemon_core::client::eventgroup::EventgroupManagerContext(),
        RemoteServerInterface(),
        service_deployment_{service_deployment},
        instance_id_{instance_id},
        packet_router_{packet_router},
        reactor_{reactor},
        event_multicast_listener_{service_deployment_.deployment_id, instance_id, required_pdus, network_config.address,
                                  conman},
        required_eventgroups_{required_eventgroups},
        communication_only_{is_communication_only},
        method_message_accumulation_timeouts_{method_message_accumulation_timeouts},
        network_config_{network_config},
        remote_server_connection_manager_factory_{std::move(remote_server_connection_manager_factory)},
        sec_com_config_{sec_com_config},
        service_discovery_client_{service_discovery_client} {
    assert(packet_router != nullptr);
    InitializeEventgroupMap();
    packet_router_->RegisterRemoteServer(comm_service_instance_id_, *this);

    if (!communication_only_) {
      ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> udp_endpoint{};

      if (network_config.udp_port.has_value()) {
        configuration::types::Port const udp_port{network_config.udp_port.value().Value()};
        amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress const udp_endpoint_value{
            network_config.address, udp_port};
        udp_endpoint.emplace(udp_endpoint_value);
      }

      assert(message_scheduler != nullptr);
      eventgroup_manager_.emplace(service_deployment, instance_id, udp_endpoint, custom_subscription_endpoints,
                                  required_eventgroups, this, timer_manager, *message_scheduler,
                                  event_multicast_listener_);

      // Register this remote server in the service discovery client
      // Note, this action will trigger the connection establishment if the ServiceDiscoveryClient has an active
      // OfferService registered. It will call OnOfferRemoteService() which will start the connection process
      assert(service_discovery_client_ != nullptr);
      service_discovery_client_->RegisterRemoteServer(instance_id, this);
    }

    // Register a SW event for closed connection cleanup
    ::ara::core::Result<osabstraction::io::reactor1::CallbackHandle> register_software_event_result{
        reactor_.RegisterSoftwareEvent(
            [this](osabstraction::io::reactor1::CallbackHandle, ::osabstraction::io::reactor1::EventTypes) {
              // Disconnect
              Disconnect();
            })};
    if (register_software_event_result) {
      sw_event_handle_cleanup_ = register_software_event_result.Value();
    } else {
      logger_.LogFatalAndAbort(
          [&register_software_event_result](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Failed to register the reactor software event. Detailed error: ");
            abort_msg.append(register_software_event_result.Error().Message());
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief   Destructor of RemoteServer.
   *
   * \context Shutdown
   * \steady  FALSE
   *
   * \internal
   * - Disconnect from the server.
   * - Unregister the remote server from the packet router.
   * - If SD is enabled
   *   - Unregister the remote server from the service discovery client.
   * - Unregister the SW event.
   * - In case the SW event unregistration in the reactor fails.
   *   - Log fatal and abort.
   * \endinternal
   */
  ~RemoteServer() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);
    // Disconnect from the server
    Disconnect();
    // Unregister this remote server from the packet router
    packet_router_->UnregisterRemoteServer(comm_service_instance_id_);

    // Unregister this remote server from the service discovery client
    if (!communication_only_) {
      service_discovery_client_->UnregisterRemoteServer(instance_id_);
    }

    // Unregister the SW event
    ::ara::core::Result<void> const unregister_result{reactor_.UnregisterSoftwareEvent(sw_event_handle_cleanup_)};
    if (!unregister_result) {
      // Log fatal and abort
      logger_.LogFatalAndAbort(
          [&unregister_result](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Failed to unregister the reactor software event. Detailed error: ");
            abort_msg.append(unregister_result.Error().Message());
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Triggers a SOME/IP event subscription.
   *
   * \param[in] event_id  The event id.
   * \param[in] observer  A pointer to the subscribed application.
   * \param[in] client_id The client id.
   *
   * \return The state of the event subscription.
   *
   * \context App
   * \steady  FALSE
   * \trace SPEC-5951482
   *
   * \internal
   *  - Find the eventgroup mapped to the given event id.
   *  - If the eventgroup is found,
   *  - Register an observer for event subscription state and forward the subscription to the eventgroup.
   *  	- If Dynamic SD is used,
   *  	  - Forward the subscription to the eventgroup manager and notify all subscribers about the subscription
   * state.
   *  	- Else Static SD is used,
   *      - Assert service is offered,
   *      - Set the subscription state as subscribed and notify all the subscribers about the new subscription state.
   * - Else Log an error if eventgroup is not found.
   * \endinternal
   */
  eventgroup::EventSubscriptionState SubscribeSomeIpEvent(
      someip_protocol::internal::EventId const event_id, SomeIpEventObserverPtr const observer,
      someip_protocol::internal::ClientId const client_id) noexcept override {
    eventgroup::EventSubscriptionState result{eventgroup::EventSubscriptionState::kNotSubscribed};

    // Get corresponding eventgroup state
    typename EventgroupMap::iterator const eventgroup_it{FindEventgroup(event_id)};

    if (eventgroup_it != eventgroup_map_.end()) {
      // Register observer
      assert(client_event_dispatcher_ != nullptr);
      client_event_dispatcher_->SubscribeSomeIpEvent(event_id, instance_id_, observer, client_id);

      eventgroup_it->second.Subscribe(event_id);
      result = eventgroup_it->second.GetSubscriptionState();

      if (!communication_only_) {  // Dynamic SD
        // Creation of service instance Id.
        configuration::types::SomeIpServiceInstanceId const service_instance_id{service_deployment_.deployment_id,
                                                                                instance_id_};
        eventgroup_manager_->OnSubscribe(eventgroup_it->first);
        // Notify observers about the dynamic subscription result.
        observer->OnSomeIpSubscriptionStateChange(service_instance_id, event_id, result);
      } else {  // Static SD
        // For Static SD the service is considered offered during initialization and never stopped until shutdown.
        assert(IsOffered());
        logger_.LogDebug(
            [](ara::log::LogStream& s) {
              s << "Static SD subscription request. Notifying static subscribers about successful subscription for "
                   "all subscribed eventgroups.";
            },
            static_cast<char const*>(__func__), __LINE__);

        // Re-subscription is considered always successful for Static SD.
        result = eventgroup::EventSubscriptionState::kSubscribed;
        for (EventgroupMapEntry const& eventgroup : eventgroup_map_) {
          OnSubscriptionCompleted(eventgroup.first);
        }
      }
    } else {
      logger_.LogError(
          [event_id](ara::log::LogStream& s) {
            s << "No eventgroup found for event id 0x";
            // Append the event Id in the correct format
            logging::LoggingUtilities::LogEventId(s, event_id);
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief   Register the client event dispatcher.
   * \details This API must be called only once by the remote server factory if there are events configured within
   * this remote server.
   *
   * \param[in] client_event_dispatcher
   *
   * \context Init
   *
   * \internal
   *  - If the injected client dispatcher is not a nullptr:
   *    - Assign the client event dispatcher to the local member variables by passing the unique pointer ownership.
   *  - Else:
   *    - Abort
   * \endinternal
   * \steady FALSE
   */
  void RegisterClientEventDispatcher(
      amsr::UniquePtr<ClientEventDispatcherInterface>&& client_event_dispatcher) noexcept {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    // There should be configured events when calling this API
    assert(!service_deployment_.communication_deployment.events.empty());
    assert(!eventgroup_map_.empty());
    // The provided client event dispatcher must not be nullptr and cannot be a double registration
    if ((client_event_dispatcher != nullptr) && (client_event_dispatcher_ == nullptr)) {
      client_event_dispatcher_ = std::move(client_event_dispatcher);
    } else {
      logger_.LogFatalAndAbort(
          [](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Violation of API preconditions. Invalid or double registration.");
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
  };

  /*!
   * \brief Triggers a PDU event subscription.
   *
   * \param[in] event_id The event id.
   * \param[in] observer A pointer to the subscribed application.
   *
   * \return The state of the event subscription.
   *
   * \context App
   * \steady  FALSE
   *
   * \internal
   *  - Find the eventgroup mapped to the given event id.
   *  - If the eventgroup is found,
   *  - Register an observer for event subscription state and forward the subscription to the eventgroup.
   *  	- If Dynamic SD is used,
   *  	  - Forward the subscription to the eventgroup manager and notify all subscribers about the subscription
   * state.
   *  	- Else Static SD is used,
   *      - Assert service is offered,
   *      - Set the subscription state as subscribed and notify all the subscribers about the new subscription state.
   * - Else Log an error if eventgroup is not found.
   * \endinternal
   */
  eventgroup::EventSubscriptionState SubscribePduEvent(someip_protocol::internal::EventId const event_id,
                                                       PduEventObserverPtr const observer) noexcept override {
    eventgroup::EventSubscriptionState result{eventgroup::EventSubscriptionState::kNotSubscribed};

    // Get corresponding eventgroup state
    typename EventgroupMap::iterator const eventgroup_it{FindEventgroup(event_id)};

    if (eventgroup_it != eventgroup_map_.end()) {
      // Register observer
      assert(client_event_dispatcher_ != nullptr);
      client_event_dispatcher_->SubscribePduEvent(event_id, observer);

      eventgroup_it->second.Subscribe(event_id);
      result = eventgroup_it->second.GetSubscriptionState();

      if (!communication_only_) {  // Dynamic SD
        // Creation of service instance Id.
        configuration::types::SomeIpServiceInstanceId const service_instance_id{service_deployment_.deployment_id,
                                                                                instance_id_};
        eventgroup_manager_->OnSubscribe(eventgroup_it->first);
        // Notify observers about the dynamic subscription result.
        observer->OnPduSubscriptionStateChange(service_instance_id, event_id, result);
      } else {  // Static SD
        // For Static SD the service is considered offered during initialization and never stopped until shutdown.
        assert(IsOffered());
        logger_.LogDebug(
            [](ara::log::LogStream& s) {
              s << "Static SD subscription request. Notifying static subscribers about successful subscription for "
                   "all subscribed eventgroups.";
            },
            static_cast<char const*>(__func__), __LINE__);

        // Re-subscription is considered always successful for Static SD.
        result = eventgroup::EventSubscriptionState::kSubscribed;
        for (EventgroupMapEntry const& eventgroup : eventgroup_map_) {
          OnSubscriptionCompleted(eventgroup.first);
        }
      }
    } else {
      logger_.LogError(
          [event_id](ara::log::LogStream& s) {
            s << "No eventgroup found for event id 0x";
            // Append the event Id in the correct format
            logging::LoggingUtilities::LogEventId(s, event_id);
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Triggers a SOME/IP event unsubscription.
   *
   * \param[in] event_id  The event id.
   * \param[in] observer  A pointer to the subscribed application.
   *
   * \pre The event must be subscribed by the application before calling this API.
   *
   * \context App
   * \steady  FALSE
   *
   * \internal
   *  - Find the eventgroup mapped to the given event.
   *  - Forward the unsubscription to the eventgroup.
   *  - If Dynamic SD is used, forward the unsubscription to the eventgroup manager.
   *  - If no more subscribers for this eventgroup exist, invalidate the corresponding field cache table.
   * \endinternal
   */
  void UnsubscribeSomeIpEvent(someip_protocol::internal::EventId const event_id,
                              SomeIpEventObserverPtr const observer) noexcept override {
    // Find corresponding eventgroup
    typename EventgroupMap::iterator const eventgroup_it{FindEventgroup(event_id)};

    if (eventgroup_it != eventgroup_map_.end()) {
      // Unregister observer
      assert(client_event_dispatcher_ != nullptr);
      client_event_dispatcher_->UnsubscribeSomeIpEvent(event_id, observer);

      eventgroup_it->second.Unsubscribe(event_id);
      if (!communication_only_) {
        someip_protocol::internal::EventgroupId const eventgroup_id{eventgroup_it->first};
        eventgroup_manager_->OnUnsubscribe(eventgroup_id);
      }
    } else {
      logger_.LogError(
          [event_id](ara::log::LogStream& s) {
            s << "No eventgroup found for event id 0x";
            // Append the event Id in the correct format
            logging::LoggingUtilities::LogEventId(s, event_id);
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Triggers a PDU event unsubscription.
   *
   * \param[in] event_id  The event id.
   * \param[in] observer  A pointer to the subscribed application.
   *
   * \pre The event must be subscribed by the application before calling this API.
   *
   * \context App
   * \steady  FALSE
   *
   * \internal
   *  - Find the eventgroup mapped to the given event.
   *  - Forward the unsubscription to the eventgroup.
   *  - If Dynamic SD is used, forward the unsubscription to the eventgroup manager.
   *  - If no more subscribers for this eventgroup exist, invalidate the corresponding field cache table.
   * \endinternal
   */
  void UnsubscribePduEvent(someip_protocol::internal::EventId const event_id,
                           PduEventObserverPtr const observer) noexcept override {
    // Find corresponding eventgroup
    typename EventgroupMap::iterator const eventgroup_it{FindEventgroup(event_id)};

    if (eventgroup_it != eventgroup_map_.end()) {
      // Unregister observer
      assert(client_event_dispatcher_ != nullptr);
      client_event_dispatcher_->UnsubscribePduEvent(event_id, observer);

      eventgroup_it->second.Unsubscribe(event_id);
      if (!communication_only_) {
        someip_protocol::internal::EventgroupId const eventgroup_id{eventgroup_it->first};
        eventgroup_manager_->OnUnsubscribe(eventgroup_id);
      }
    } else {
      logger_.LogError(
          [event_id](ara::log::LogStream& s) {
            s << "No eventgroup found for event id 0x";
            // Append the event Id in the correct format
            logging::LoggingUtilities::LogEventId(s, event_id);
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Called upon reception of an ACK entry.
   *
   * \param[in] eventgroup_id The event group identifier.
   * \param[in] multicast_address Multicast address.
   * \param[in] multicast_port Multicast port.
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Forward the call to the eventgroup manager.
   * \endinternal
   */
  void OnSubscribeEventgroupAck(someip_protocol::internal::EventgroupId const eventgroup_id,
                                ara::core::Optional<someip_daemon_core::IpAddress> const& multicast_address,
                                someip_protocol::internal::Port const multicast_port) override {
    assert(!communication_only_);
    eventgroup_manager_->OnSubscribeEventgroupAck(eventgroup_id, multicast_address, multicast_port);
  }

  /*!
   * \brief Called upon reception of an NACK entry.
   *
   * \param[in] eventgroup_id The event group identifier.
   *
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Invalidate related Field cache.
   * - Forward the call to the eventgroup manager.
   * - If the event group contains at least one TCP event, close the connection.
   * - Notify subscribers with state Pending.
   * \endinternal
   */
  void OnSubscribeEventgroupNack(someip_protocol::internal::EventgroupId const eventgroup_id) override {
    assert(!communication_only_);
    if (eventgroup_manager_->GetEventgroupSubscriptionState(eventgroup_id) !=
        eventgroup::EventSubscriptionState::kNotSubscribed) {
      eventgroup_manager_->OnSubscribeEventgroupNack(eventgroup_id);
      // Only disconnect if the event group contains at least one TCP event
      if (configuration::util::ConfigurationTypesUtilities::HasRequiredEventWithProto(
              service_deployment_.communication_deployment, eventgroup_id,
              configuration::types::TransportProtocol::kTCP)) {
        Disconnect();
      }

      // Notify subscribers that subscription is pending.
      NotifyEventgroupSubscriptionObservers(eventgroup_id, eventgroup::EventSubscriptionState::kSubscriptionPending);

      // Invalidate the field cache of the corresponding eventgroup
      ara::core::Map<amsr::someip_protocol::internal::EventgroupId,
                     amsr::someip_daemon_core::configuration::types::SomeIpEventGroup>::const_iterator const eg_it{
          service_deployment_.communication_deployment.event_groups.find(eventgroup_id)};
      for (amsr::someip_protocol::internal::EventId const event_id : eg_it->second.events) {
        assert(client_event_dispatcher_ != nullptr);
        client_event_dispatcher_->InvalidateFieldCacheEntry(event_id);
      }
    }
  }

  /*!
   * \brief Called when a service has been offered.
   *
   * \param[in] remote_server_address The remote server address.
   *
   * \pre -
   * \context Network
   * \steady  FALSE
   *
   * \internal
   * - Set the remote server address.
   * - Establish the connection.
   * \endinternal
   */
  void OnServiceOffered(ServiceAddress const& remote_server_address) noexcept override {
    remote_server_address_.emplace(remote_server_address);

    // Establish the connection
    Connect();
  }

  /*!
   * \brief Called when a service has been stopped being offered.
   *
   * \pre -
   * \context Network
   * \steady  FALSE
   *
   * \internal
   * - Reset the remote server address.
   * - Reset the entry content.
   * - Initiate disconnection.
   * \endinternal
   */
  void OnServiceStopped() noexcept override {
    remote_server_address_.reset();
    entry_.reset();
    Disconnect();
  }

  /*!
   * \brief     Called when a service has been offered via dynamic SD.
   *
   * \param[in] entry        The offer service SD entry.
   * \param[in] is_multicast indicates whether the entry is received via multicast or unicast.
   *
   * \pre       The service instance must be known in the configuration.
   * \context   Network
   * \steady    FALSE
   *
   * \internal
   * - Get TCP and UDP endpoint of a current active offer.
   * - Assign TCP and UDP endpoint of current active offer to tcp_endpoint_ and udp_endpoint_.
   * - Forward the call to the event manager (to send the relevant subscriptions).
   * \endinternal
   */
  void OnOfferRemoteService(service_discovery::state_machine::client::ActiveOfferServiceEntry const& entry) override {
    assert(!communication_only_);
    entry_.emplace(entry);
    amsr::someip_daemon_core::service_discovery::message::entries::OfferServiceEntry const& active_offer{
        entry.offer_entry};

    ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> tcp_endpoint{};
    ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> udp_endpoint{};

    if (active_offer.tcp_endpoint.has_value()) {
      configuration::types::Port const tcp_port{active_offer.tcp_endpoint.value().port.port};
      amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress const tcp_endpoint_value{
          active_offer.tcp_endpoint.value().address, tcp_port};
      tcp_endpoint.emplace(tcp_endpoint_value);
    }
    if (active_offer.udp_endpoint.has_value()) {
      configuration::types::Port const udp_port{active_offer.udp_endpoint.value().port.port};
      amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress const udp_endpoint_value{
          active_offer.udp_endpoint.value().address, udp_port};
      udp_endpoint.emplace(udp_endpoint_value);
    }
    ServiceAddress const remote_server_address{tcp_endpoint, udp_endpoint};
    OnServiceOffered(remote_server_address);

    eventgroup_manager_->OnOfferService(entry, entry.is_multicast);
  }

  /*!
   * \brief Called when an offer renewal entry has been received (necessary to trigger Subscription messages).
   *
   * \param[in] is_multicast indicates whether the entry is received via multicast or unicast.
   *
   * \context Network
   * \steady  FALSE
   *
   * \internal
   * - If there is at least one client requesting the service, initiate connection establishment.
   * - Forward the call to the event manager (to send the relevant subscriptions).
   * \endinternal
   */
  void OnOfferRenewal(bool const is_multicast) override {
    // Renewal must only happen if the service has been offered already
    assert(entry_.has_value() && remote_server_address_.has_value());
    assert(!communication_only_);
    // Attempt to re-establish connection (in case connection got broken)
    Connect();
    eventgroup_manager_->OnOfferService(entry_.value(), is_multicast);
  }

  /*!
   * \brief Called when a service is no longer offered.
   *
   * \context Network
   * \steady  FALSE
   *
   * \internal
   * - Reset remote_server_address_.
   * - Invalidate related Field cache.
   * - Forward the call to the event manager (to update subscription state).
   * - If there are eventgroups configured in the required service instance.
   *  - Notify subscribers with state Pending.
   * \endinternal
   */
  void OnStopOfferRemoteService() override {
    assert(!communication_only_);

    // Reset remote_server_address_.
    OnServiceStopped();

    eventgroup_manager_->OnStopOfferService();

    // If there are eventgroups configured in the required service instance.
    if (!service_deployment_.communication_deployment.event_groups.empty()) {
      // Notify observers
      for (configuration::types::SomeIpEventDeployment const& event :
           service_deployment_.communication_deployment.events) {
        configuration::types::SomeIpServiceInstanceId const service_instance_id{service_deployment_.deployment_id,
                                                                                instance_id_};
        // The client event dispatcher must exist if there are events configured
        assert(client_event_dispatcher_ != nullptr);
        if (event.is_signal_based) {
          client_event_dispatcher_->OnPduSubscriptionStateChange(
              service_instance_id, event.event_id, eventgroup::EventSubscriptionState::kSubscriptionPending);

        } else {
          client_event_dispatcher_->OnSomeIpSubscriptionStateChange(
              service_instance_id, event.event_id, eventgroup::EventSubscriptionState::kSubscriptionPending);
        }
        // Invalidate the cache in case it is a field event
        if (event.is_field) {
          client_event_dispatcher_->InvalidateFieldCacheEntry(event.event_id);
        }
      }
    }
  }

  /*!
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \details Called from SD Client Eventgroup state machine.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \pre     -
   * \context App
   * \steady  FALSE
   *
   * \internal
   * - Notify all subscribers (new Subscription state Subscribed).
   * \endinternal
   */
  void OnEventgroupSubscribed(someip_protocol::internal::EventgroupId const eventgroup_id) override {
    logger_.LogVerbose(
        [&eventgroup_id](ara::log::LogStream& s) { s << "Eventgroup 0x" << ara::log::HexFormat(eventgroup_id); },
        static_cast<char const*>(__func__), __LINE__);

    // Get corresponding eventgroup state
    typename EventgroupMap::iterator const eventgroup_it{eventgroup_map_.find(eventgroup_id)};

    if (eventgroup_it != eventgroup_map_.end()) {
      eventgroup_it->second.OnSubscriptionAccepted();
    }
    NotifyEventgroupSubscriptionObservers(eventgroup_id, eventgroup::EventSubscriptionState::kSubscribed);
  }

  /*!
   * \brief Forwards a local SOME/IP request message.
   * \param[in] packet A SOME/IP request message.
   *
   * \return void if the message was sent successfully, and error otherwise.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ServiceNotOffered
   * Runtime check failed: service is not offered.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kConfigurationError_UnknownMethod
   * Configuration check failed: method not configured.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable
   * Runtime check failed: connection is not established.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionTransmissionFailed
   * Runtime check failed: connection transmission failed.
   *
   * \pre         The remote server connection manager must have been emplaced first.
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \internal
   * - Check if the connection is established
   *   - Check if methods are configured
   *     - Check forward packet and return void if succeed.
   *     - Return an error otherwise.
   * - If connection is not established, check for the reason
   *   - If the service is not offered, return not-offered error
   *   - If the service is offered but connection is not established, return non-connected error
   * \endinternal
   */
  ara::core::Result<void> SendMethodRequest(Packet& packet) noexcept override {
    someip_protocol::internal::SomeIpMessageHeader const& header{packet->GetHeader()};
    ara::core::Result<void> validation_result{
        packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable};

    logger_.LogVerbose(
        [this, &header](ara::log::LogStream& s) {
          s << "Send request (ServiceId: 0x";
          s << ara::log::HexFormat(header.service_id_);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(header.interface_version_);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(instance_id_);
          s << ", ClientId: 0x";
          s << ara::log::HexFormat(header.client_id_);
          s << ", SessionId: 0x";
          s << ara::log::HexFormat(header.session_id_);
          s << ", MethodId: 0x";
          s << ara::log::HexFormat(header.method_id_);
          s << ")";
        },
        static_cast<char const*>(__func__), __LINE__);

    // Transmission can happen only if the service is offered and if the connection is established.
    // As an active offer is a pre-requisite for a connection establishment, checking for connection is enough to
    // ensure both
    if (IsConnected()) {
      // Find the method id in the method deployment container
      ara::core::Vector<configuration::types::SomeIpMethodDeployment>::const_iterator const methods_deployment_begin{
          service_deployment_.communication_deployment.methods.cbegin()};
      ara::core::Vector<configuration::types::SomeIpMethodDeployment>::const_iterator const methods_deployment_end{
          service_deployment_.communication_deployment.methods.cend()};
      ara::core::Vector<configuration::types::SomeIpMethodDeployment>::const_iterator const method_deployment_it{
          std::find_if(methods_deployment_begin, methods_deployment_end,
                       [&header](configuration::types::SomeIpMethodDeployment const method) {
                         return method.method_id == header.method_id_;
                       })};
      if (method_deployment_it != methods_deployment_end) {
        // Find out the accumulation timeout, if any
        using Iter = typename decltype(method_message_accumulation_timeouts_)::const_iterator;
        Iter const it{method_message_accumulation_timeouts_.find(header.method_id_)};

        if (it != method_message_accumulation_timeouts_.cend()) {
          // Add timeout to the packet metadata
          packet->SetAccumulationTimeout(ara::core::Optional<std::chrono::nanoseconds>{it->second});
        }

        assert(remote_server_connection_manager_ != nullptr);
        validation_result =
            remote_server_connection_manager_->SendRequest(method_deployment_it->transport_protocol, packet);
      } else {
        // Emplace error
        validation_result.EmplaceError(
            packet_handler::validators::LocalIncomingPacketValidatorError::kConfigurationError_UnknownMethod);
      }
    } else if (!IsOffered()) {  // The service is not offered
      // Emplace error
      validation_result.EmplaceError(
          packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ServiceNotOffered);
    } else {
      // Stick with default error kRuntimeError_ConnectionNotAvailable
    }

    return validation_result;
  }

  /*!
   * \brief Initialize the Static Service Discovery.
   *
   * \param[in] remote_server_address     Unicast IP address and port of the remote server
   * \param[in] event_multicast_endpoint  Optional multicast endpoint for multicast event reception with Static SD
   *
   * \pre       Static SD is configured.
   * \context   Init
   * \steady    FALSE
   *
   * \internal
   * - Check if Static SD is configured (Service Discovery disabled)
   * - Call OnServiceOffered with the remote server address.
   * - If multicast address configured,
   *   - Start listening for multicast events.
   * - If Dynamic SD is configured (not allowed in this case), abort is executed.
   * \endinternal
   */
  void InitializeStaticSD(
      amsr::someip_daemon_core::configuration::types::ServiceAddress const& remote_server_address,
      ara::core::Optional<amsr::someip_daemon_core::configuration::types::NetworkEndpointAddress> const&
          event_multicast_endpoint) noexcept override {
    // This API shall only be called when service discovery is disabled
    if (communication_only_) {  // Static SD
      // Trigger OfferService
      OnServiceOffered(remote_server_address);

      // Start listening for multicast events if multicast address configured
      if (event_multicast_endpoint.has_value()) {
        someip_daemon_core::IpAddress const event_multicast_address{event_multicast_endpoint.value().address};

        someip_daemon_core::IpAddress const remote_server_ipaddress{remote_server_address.udp_endpoint.value().address};

        // In case of an error, this is reported inside the method StartListenForMulticastEventgroup()
        static_cast<void>(event_multicast_listener_.StartListenForMulticastEventgroup(
            event_multicast_address, event_multicast_endpoint.value().port.Value(), remote_server_ipaddress,
            remote_server_address.udp_endpoint.value().port.Value()));
      }
    } else {
      logger_.LogFatalAndAbort(
          [](ara::core::String& abort_msg) noexcept { abort_msg.append("Only Static SD allowed."); }, AMSR_FILE,
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Get the connection state.
   *
   * \context  App, Network
   * \steady   TRUE
   *
   * \return True if connection is successfully established. False otherwise.
   *
   * \internal
   *  - Assume the connection status is kDisconnected.
   *  - If a remote server connection manager object exists,
   *   - Poll the connection state from the remote server connection manager.
   *     Note that this check could return kConnected or kConnecting.
   *  - Else if this rsi is configured with multicast only, assume the connection is established,
   *  - Return true if the connection is established and false otherwise.
   * \endinternal
   */
  bool IsConnected() const noexcept override {
    connection_manager::ConnectionState connection_state{connection_manager::ConnectionState::kDisconnected};

    // We have a remote server connection manager, poll the connection state.
    if (remote_server_connection_manager_ != nullptr) {
      connection_state = remote_server_connection_manager_->GetConnectionState();
    } else if (is_multicast_only_) {
      // We do not have a RemoteServerConnectionManager but only multicast communication is required, where the
      // connection state is always assumed to be connected
      connection_state = connection_manager::ConnectionState::kConnected;
    } else {
      // Connections are required (no multicast only) but no remote server connection manager is created, so the
      // connection must be disconnected.
      // Make Bauhaus happy (M6.4.2)
    }

    return connection_state == connection_manager::ConnectionState::kConnected;
  }

  /*!
   * \brief Called by the packet router when a SomeIp event is received.
   *
   * \param[in] instance_id The instance id
   * \param[in] packet      A SOME/IP notification message
   *
   * \pre     -
   * \steady  TRUE
   * \context Network
   *
   * \internal
   *  - Forward he event to the client dispatcher.
   * \endinternal
   */
  void OnSomeIpEvent(someip_protocol::internal::InstanceId const instance_id,
                     client::Packet const& packet) noexcept override {
    // The client event dispatcher must exist if there are events configured
    assert(client_event_dispatcher_ != nullptr);
    client_event_dispatcher_->OnSomeIpEvent(instance_id, packet);
  }

  /*!
   * \brief Called by the packet router when an UDP event is received.
   *
   * \param[in] instance_id The instance id
   * \param[in] packet      A PDU notification message
   *
   * \pre     -
   * \steady  TRUE
   * \context Network
   *
   * \internal
   *  - Forward he event to the client dispatcher.
   * \endinternal
   */
  void OnPduEvent(someip_protocol::internal::InstanceId const instance_id,
                  client::PduPacket const& packet) noexcept override {
    // The client event dispatcher must exist if there are events configured
    assert(client_event_dispatcher_ != nullptr);
    client_event_dispatcher_->OnPduEvent(instance_id, packet);
  }

 protected:
  /*!
   * \brief Handle "connection established".
   * \details The object calling this API only exists if the service is offered, so it is impossible for this
   * API to be called if this is not the case. If it is, then there are some implementation/architectural issues.
   *
   * \context Network
   * \steady  FALSE
   *
   * \trace SPEC-8053475
   *
   * \internal
   * - Notify about the connection established.
   * \endinternal
   */
  void HandleConnectionEstablished() noexcept override {
    logger_.LogDebug([](ara::log::LogStream& s) { s << "Connection with the server is established."; },
                     static_cast<char const*>(__func__), __LINE__);

    // This API is called only from within the object remote_server_connection_manager_, so it must have been
    // constructed
    assert(remote_server_connection_manager_ != nullptr);

    // A service being offered and requested is a pre-condition for creating the RemoteServerConnectionManager which
    // triggers this API, so ath this point the service must be offered.
    assert(IsOffered());
    OnConnected();
  }

  /*!
   * \brief Handle "connection closed"
   * \details Schedule call to Disconnect by triggering a SW event. This must
   *          be done asynchronously because calling Disconnect will reset remote_server_connection_manager_.
   *
   * \context Network
   * \steady  FALSE
   *
   * \internal
   * - Trigger the SW event, to handle the disconnection cleanup in the next reactor call
   * - If trigger fails
   *   - Log fatal and abort
   * \endinternal
   */
  void HandleConnectionClosed() noexcept override {
    logger_.LogDebug([](ara::log::LogStream& s) { s << "Connection with the server has been closed."; },
                     static_cast<char const*>(__func__), __LINE__);

    // Trigger the SW event to do the cleanup because the disconnected connections cannot be destroyed instantly.
    ::ara::core::Result<void> const trigger_reactor_result{reactor_.TriggerSoftwareEvent(sw_event_handle_cleanup_)};
    if (!trigger_reactor_result) {
      // Log fatal and abort
      logger_.LogFatalAndAbort(
          [&trigger_reactor_result](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Failed to trigger the reactor software event. Detailed error: ");
            abort_msg.append(trigger_reactor_result.Error().Message());
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
  }

 private:
  /*!
   * \brief Alias for eventgroup map.
   */
  using EventgroupMap = ara::core::Map<someip_protocol::internal::EventgroupId, eventgroup::RemoteServerEventgroup>;

  /*!
   * \brief Alias for entry type of eventgroup map.
   */
  using EventgroupMapEntry =
      std::pair<someip_protocol::internal::EventgroupId const, eventgroup::RemoteServerEventgroup>;

  /*!
   * \brief Handle "connection established"
   *
   * \context Network
   * \steady  FALSE
   *
   * \trace SPEC-8053475
   *
   * \internal
   * - If we are doing Service Discovery
   *   - Notify the event group manager
   * - else (we are doing communication only)
   *   - Notify every event group of subscription completion
   * \endinternal
   */
  void OnConnected() noexcept {
    if (!communication_only_) {  // {Dynamic SD}
      eventgroup_manager_->OnConnectionEstablished();
    } else {  // {Static}
      // Update subscription state and notify about subscription completed
      for (EventgroupMapEntry const& eventgroup : eventgroup_map_) {
        OnSubscriptionCompleted(eventgroup.first);
      }
    }
  }

  /*!
   * \brief Connects to a remote provided service instance identified by its IP address and port numbers.
   *
   * \context App, Network
   * \steady  FALSE
   *
   * \trace SPEC-4980085
   * \trace SPEC-4980120
   * \trace SPEC-8053475
   *
   * \internal
   * - If the connection state is not already set to "established"
   *   - If we have no remote server connection manager but at least UDP or TCP is configured
   *     - Obtain a remote server connection manager
   *     - If (creation of a remote server connection manager was successful) and (TCP is configured) and (we are also
   * doing service discovery)
   *       - Register the local TCP network endpoint with the event group manager
   * - If connection is already established (could happen in case other services are already using this same
   * connection)
   *   - Call OnConnected
   * \endinternal
   */
  // VECTOR NC Metric-HIS.LEVEL: MD_SomeIpDaemon_Metric-HIS.LEVEL_his_level
  void Connect() noexcept {
    // We shall only try to connect if there is an active offer
    assert(remote_server_address_.has_value());

    // Create RemoteServerConnectionManager only if connections are required and no connection is already established
    if ((remote_server_connection_manager_ == nullptr) && (tcp_connection_required_ || udp_connection_required_)) {
      remote_server_connection_manager_ =
          remote_server_connection_manager_factory_->CreateRemoteServerConnectionManager(
              service_deployment_.deployment_id, instance_id_, network_config_, remote_server_address_.value(), this,
              tcp_connection_required_, udp_connection_required_, sec_com_config_);

      // If creation succeeded, TCP connection is required and SD is enabled, extract the assigned local TCP endpoint
      // and register it in the eventgroup manager
      if ((remote_server_connection_manager_ != nullptr) && tcp_connection_required_ && (!communication_only_)) {
        eventgroup_manager_->RegisterLocalTcpNetworkEndpoint(
            remote_server_connection_manager_->GetLocalTcpNetworkEndpoint());
      }
    }

    // If other service instances have already requested a connection we might have missed the connection state change
    // event and thus need to check if they are already established we can start offering immediately.
    if (IsConnected()) {
      OnConnected();
    }
  }

  /*!
   * \brief Disconnects from the connected remote provided service instance.
   *
   * \context ANY
   * \steady  FALSE
   *
   * \internal
   * - If we are doing Service Discovery
   *   - Notify the event group manager of connection closure
   * - If there are eventgroups configured in the required service instance.
   *   - Reset the event group subscription states
   * - Destroy the remote server connection manager to trigger the disconnection
   * \endinternal
   */
  void Disconnect() noexcept {
    logger_.LogDebug([](ara::log::LogStream& s) { s << "Closing connection with the server."; },
                     static_cast<char const*>(__func__), __LINE__);

    if (!communication_only_) {
      eventgroup_manager_->OnConnectionClosed();
    }

    // If there are eventgroups configured in the required service instance.
    if (!service_deployment_.communication_deployment.event_groups.empty()) {
      // Reset eventgroup subscriptions
      ResetEventgroupsSubscriptionStates();
    }
    // Destroy the remote server connection manager, actively closing the connection
    remote_server_connection_manager_ = nullptr;
  }

  /*!
   * \brief Indicates whether the service is offered.
   * \return true if the service is offered.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  bool IsOffered() const noexcept {
    return (remote_server_address_.has_value() &&
            (remote_server_address_->tcp_endpoint.has_value() || remote_server_address_->udp_endpoint.has_value()));
  }

  /*!
   * \brief Notifies all observers about an eventgroup id subscription
   *
   * \param[in] eventgroup_id  The eventgroup id
   *
   * \context Network
   * \steady  FALSE
   *
   * \internal
   * - Get eventgroup subscription status
   * - Set the subscription state to kSubscribed
   * - Notify all observers
   * \endinternal
   */
  void OnSubscriptionCompleted(someip_protocol::internal::EventgroupId const eventgroup_id) {
    // Get eventgroup subscription status
    typename ara::core::Map<someip_protocol::internal::EventgroupId, eventgroup::RemoteServerEventgroup>::iterator const
        it{eventgroup_map_.find(eventgroup_id)};

    assert(it != eventgroup_map_.end());

    // Set subscription state to kSubscribed
    it->second.OnSubscriptionAccepted();

    RequiredEventgroupsContainer::const_iterator const required_eventgroups_it{
        required_eventgroups_.find(eventgroup_id)};

    if (required_eventgroups_it != required_eventgroups_.cend()) {
      // Find all events mapped to this eventgroup
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     configuration::types::SomeIpEventGroup>::const_iterator const eventgroup_deployment{
          service_deployment_.communication_deployment.event_groups.find(eventgroup_id)};

      assert(eventgroup_deployment != service_deployment_.communication_deployment.event_groups.cend());

      configuration::types::SomeIpServiceInstanceId const service_instance_id{service_deployment_.deployment_id,
                                                                              instance_id_};

      for (someip_protocol::internal::EventId const event_id : eventgroup_deployment->second.events) {
        ara::core::Vector<configuration::types::SomeIpEventDeployment>::const_iterator const event_it{
            std::find_if(service_deployment_.communication_deployment.events.cbegin(),
                         service_deployment_.communication_deployment.events.cend(),
                         [event_id](configuration::types::SomeIpEventDeployment const& e) noexcept {
                           return (event_id == e.event_id);
                         })};

        if (event_it != service_deployment_.communication_deployment.events.cend()) {
          // The client event dispatcher must exist if there are events configured
          assert(client_event_dispatcher_ != nullptr);
          if (event_it->is_signal_based) {
            client_event_dispatcher_->OnPduSubscriptionStateChange(service_instance_id, event_it->event_id,
                                                                   eventgroup::EventSubscriptionState::kSubscribed);

          } else {
            client_event_dispatcher_->OnSomeIpSubscriptionStateChange(service_instance_id, event_it->event_id,
                                                                      eventgroup::EventSubscriptionState::kSubscribed);
          }
        }
      }
    }
  }

  /*!
   * \brief Reset subscription state for all events within specific service instance to kSubscriptionPending.
   * \details This implementation notifies each subscription observer for each event within the given eventgroup.
   *
   * \context Network, Shutdown
   * \steady  FALSE
   *
   * \internal
   * - Get the service interface configuration from the static configuration struct.
   * - If the service interface configuration is found
   *   - Update event subscription state of all service events to kSubscriptionPending.
   *
   * \endinternal
   */
  void ResetEventgroupsSubscriptionStates() {
    logger_.LogVerbose(
        [](ara::log::LogStream& s) {
          s << "Updating event subscription state for all events - New state: kSubscriptionPending";
        },
        static_cast<char const*>(__func__), __LINE__);
    assert(!eventgroup_map_.empty());
    // Reset subscription state for all eventgroups
    EventgroupMap::iterator const eventgroup_map_begin{eventgroup_map_.begin()};
    EventgroupMap::iterator const eventgroup_map_end{eventgroup_map_.end()};
    // Return value of method for_each() not needed to be managed
    static_cast<void>(std::for_each(
        eventgroup_map_begin, eventgroup_map_end,
        [](std::pair<someip_protocol::internal::EventgroupId const, eventgroup::RemoteServerEventgroup>& eventgroup) {
          eventgroup.second.OnSubscriptionCancelled();
        }));
    configuration::types::SomeIpServiceInstanceId const service_instance_id{service_deployment_.deployment_id,
                                                                            instance_id_};
    // Notify observers
    for (configuration::types::SomeIpEventDeployment const& event :
         service_deployment_.communication_deployment.events) {
      // The client event dispatcher must exist if there are events configured
      assert(client_event_dispatcher_ != nullptr);
      if (event.is_signal_based) {
        client_event_dispatcher_->OnPduSubscriptionStateChange(
            service_instance_id, event.event_id, eventgroup::EventSubscriptionState::kSubscriptionPending);

      } else {
        client_event_dispatcher_->OnSomeIpSubscriptionStateChange(
            service_instance_id, event.event_id, eventgroup::EventSubscriptionState::kSubscriptionPending);
      }
    }
  }

  /*!
   * \brief Call when an event group's subscription state is changed.
   *
   * \param[in] eventgroup_id The event group identifier of the event group with the subscription change.
   * \param[in] new_state The new subscription state.
   *
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Log the service Id, instance Id, eventgroup Id and the new state.
   * - Update event subscription state for each event within the given event group with the new state.
   * \endinternal
   */
  void NotifyEventgroupSubscriptionObservers(
      someip_protocol::internal::EventgroupId const eventgroup_id,
      client::eventgroup::EventSubscriptionState const new_state) const noexcept {
    logger_.LogDebug(
        [&eventgroup_id, &new_state](ara::log::LogStream& s) {
          ara::core::String const new_state_string{
              amsr::someip_daemon_core::client::eventgroup::EventSubscriptionStateToString(new_state)};
          s << "EventgroupId: 0x";
          s << ara::log::HexFormat(eventgroup_id);
          s << " - Eventgroup subscription state updated to : ";
          s << new_state_string;
        },
        static_cast<char const*>(__func__), __LINE__);

    // Notify all event observers
    RequiredEventgroupsContainer::const_iterator const required_eventgroups_it{
        required_eventgroups_.find(eventgroup_id)};

    if (required_eventgroups_it != required_eventgroups_.cend()) {
      // Find all events mapped to this eventgroup
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     configuration::types::SomeIpEventGroup>::const_iterator const eventgroup_deployment{
          service_deployment_.communication_deployment.event_groups.find(eventgroup_id)};

      assert(eventgroup_deployment != service_deployment_.communication_deployment.event_groups.cend());

      configuration::types::SomeIpServiceInstanceId const service_instance_id{service_deployment_.deployment_id,
                                                                              instance_id_};

      for (someip_protocol::internal::EventId const event_id : eventgroup_deployment->second.events) {
        ara::core::Vector<configuration::types::SomeIpEventDeployment>::const_iterator const event_it{
            std::find_if(service_deployment_.communication_deployment.events.cbegin(),
                         service_deployment_.communication_deployment.events.cend(),
                         [event_id](configuration::types::SomeIpEventDeployment const& e) noexcept {
                           return (event_id == e.event_id);
                         })};
        if (event_it != service_deployment_.communication_deployment.events.cend()) {
          // The client event dispatcher must exist if there are events configured
          assert(client_event_dispatcher_ != nullptr);
          if (event_it->is_signal_based) {
            client_event_dispatcher_->OnPduSubscriptionStateChange(service_instance_id, event_it->event_id, new_state);

          } else {
            client_event_dispatcher_->OnSomeIpSubscriptionStateChange(service_instance_id, event_it->event_id,
                                                                      new_state);
          }
        }
      }
    }
  }

  /*!
   * \brief Find the eventgroup subscription state of a given event id.
   *
   * \param[in] event_id The event id.
   *
   * \context App
   * \steady  FALSE
   *
   * \return An iterator to the subscription state found in the eventgroup_map_. It may return an end iterator
   *        if either the eventgroup is not found in configuration or the found eventgroup has no entry in the
   *        eventgroup_map_
   */
  EventgroupMap::iterator FindEventgroup(someip_protocol::internal::EventId const event_id) noexcept {
    EventgroupMap::iterator it{eventgroup_map_.end()};

    for (ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                        configuration::types::SomeIpEventGroup>::const_reference eventgroup_deployment :
         service_deployment_.communication_deployment.event_groups) {
      // Check if the event is found in the container for this eventgroup
      ara::core::Vector<someip_protocol::internal::EventId>::const_iterator const events_begin{
          eventgroup_deployment.second.events.cbegin()};
      ara::core::Vector<someip_protocol::internal::EventId>::const_iterator const events_end{
          eventgroup_deployment.second.events.cend()};

      ara::core::Vector<someip_protocol::internal::EventId>::const_iterator const event_it{
          std::find_if(events_begin, events_end,
                       [event_id](someip_protocol::internal::EventId const& id) { return id == event_id; })};

      if (event_it != events_end) {
        // Event found in this eventgroup. Get iterator to eventgroup subscription map and break loop
        it = eventgroup_map_.find(eventgroup_deployment.first);
        break;
      }
    }

    return it;
  }

  /*!
   * \brief Initializes the eventgroup map.
   *
   * \context Init
   * \steady  FALSE
   *
   * \internal
   * - Get a container with all required eventgroups
   * - For each configured eventgroup
   *   - Create a new RemoteServerEventgroup and insert it in the map
   * \endinternal
   */
  void InitializeEventgroupMap() noexcept {
    for (RequiredEventgroupsContainer::const_reference eventgroup : required_eventgroups_) {
      // Return value of emplace method not managed. Assumed no errors when inserted in the map
      static_cast<void>(eventgroup_map_.emplace(std::piecewise_construct, std::forward_as_tuple(eventgroup.first),
                                                std::forward_as_tuple(eventgroup.first)));
    }
  }

  /*!
   * \brief Service deployment configuration.
   * \details Used in this class to access the deployment id and the list of configured
   *          events and eventgroups.
   */
  configuration::types::SomeIpServiceInterfaceDeployment const service_deployment_;

  /*!
   * \brief The service instance id.
   */
  someip_protocol::internal::InstanceId const instance_id_;

  // Instances referenced by RemoteServer

  /*!
   * \brief A packet router.
   */
  std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router_;

  /*!
   * \brief The reactor, used to trigger SW events for cleanup after disconnection detection.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief Service Instance Id used in communication containing Service Id, Instance Id and Major Version.
   */
  configuration::types::SomeIpServiceInstanceIdCommunication const comm_service_instance_id_{
      {service_deployment_.deployment_id.service_interface_id,
       service_deployment_.deployment_id.service_interface_version.major_version},
      instance_id_};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix("RemoteServer"_sv, service_deployment_.deployment_id,
                                                      instance_id_)};

  // Members managed by RemoteServer

  /*!
   * \brief A map tracking the subscription state of every eventgroup.
   */
  EventgroupMap eventgroup_map_{};

  /*!
   * \brief Active offer entry.
   * \details Set upon first offer reception. Reset upon stop offer reception.
   */
  ara::core::Optional<service_discovery::state_machine::client::ActiveOfferServiceEntry> entry_{ara::core::nullopt};

  /*!
   * \brief   A client event dispatcher for SomeIp or PDU events.
   * \details This object must exists whenever a PDU or SomeIp event is configured within this remote server. It must
   * be injected by the remote server factory when the remote server is created.
   */
  amsr::UniquePtr<ClientEventDispatcherInterface> client_event_dispatcher_{nullptr};
  /*!
   * \brief A service address.
   * \details Set upon first offer reception. Reset upon stop offer reception.
   */
  ara::core::Optional<ServiceAddress> remote_server_address_{ara::core::nullopt};

  /*!
   * \brief A state machine responsible for SOME/IP eventgroup subscriptions.
   */
  ara::core::Optional<amsr::someip_daemon_core::client::eventgroup::EventgroupManager<ConnectionManager>>
      eventgroup_manager_{};

  /*!
   * \brief The connection manager for this remote server.
   */
  amsr::UniquePtr<RemoteServerConnectionManagerInterface> remote_server_connection_manager_{nullptr};

  /*!
   * \brief An event multicast listener.
   */
  eventgroup::EventMulticastListener<ConnectionManager> event_multicast_listener_;

  /*!
   * \brief The required eventgroups for this required service instance.
   */
  RequiredEventgroupsContainer const required_eventgroups_;

  /*!
   * \brief The communication type.
   */
  bool const communication_only_;

  /*!
   * \brief A timeout list for UDP message accumulation purposes.
   */
  MethodsMessageAccumulationTimeoutMap const method_message_accumulation_timeouts_;

  /*!
   * \brief The network configuration.
   */
  configuration::types::RequiredNetworkEndpoint const network_config_;

  /*!
   * \brief Defines if a remote TCP connection is required or not.
   * \details This variable is true if for this required SOME/IP service instance, any required event or method is
   *          configured to use TCP protocol, and a TCP endpoint has been configured.
   *          In case no endpoint is configured, it is still possible that events using TCP are configured but
   * deployed to only use multicast communication for this specific required service instance.
   */
  bool const tcp_connection_required_{configuration::util::ConfigurationTypesUtilities::IsTransportProtocolRequired(
                                          service_deployment_.communication_deployment, required_eventgroups_,
                                          configuration::types::TransportProtocol::kTCP) &&
                                      network_config_.tcp_port.has_value()};

  /*!
   * \brief Defines if a remote UDP connection is required or not.
   * \details This variable is true if for this required SOME/IP service instance, any required event or method is
   *          configured to use UDP protocol, and a UDP endpoint has been configured.
   *          In case no endpoint is configured, it is still possible that events using UDP are configured but
   * deployed to only use multicast communication for this specific required service instance.
   */
  bool const udp_connection_required_{configuration::util::ConfigurationTypesUtilities::IsTransportProtocolRequired(
                                          service_deployment_.communication_deployment, required_eventgroups_,
                                          configuration::types::TransportProtocol::kUDP) &&
                                      network_config_.udp_port.has_value()};

  /*!
   * \brief Defines if this remote server is configured to use multicast only communication
   * \details A RemoteServer that has neither UDP nor TCP endpoints configured, it can only process event multicast
   *          communication. In this scenario, no connections are needed (no RemoteServerConnectionManager) and its
   *          connection state is assumed to be always "Connected" as UDP multicast communication requires no
   *          connection.
   */
  bool const is_multicast_only_{(!network_config_.tcp_port.has_value()) && (!network_config_.udp_port.has_value())};

  /*!
   * \brief The Remote Server Connection Manager Factory.
   */
  amsr::UniquePtr<RemoteServerConnectionManagerFactoryInterface> remote_server_connection_manager_factory_;

  /*!
   * \brief The SecCom configuration.
   */
  configuration::ConfigurationTypesAndDefs::SecComConfig const sec_com_config_;

  /*!
   * \brief   A pointer to the service discovery client.
   * \details A remote server must register/unregister into a service discovery client to be notified about the state
   *          of the remote service.
   *          The lifecycle of this service discovery client is guaranteed to be larger than the lifecycle of any
   *          RemoteServer instance.
   * \note    This may be nullptr for communication only remote servers.
   */
  service_discovery::state_machine::client::ServiceDiscoveryClientInterface* service_discovery_client_;

  /*!
   * \brief Handle for Reactor software event for cleanup after disconnection detection.
   */
  osabstraction::io::reactor1::CallbackHandle sw_event_handle_cleanup_{};
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_H_

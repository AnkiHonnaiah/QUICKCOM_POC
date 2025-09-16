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
/**        \file  remote_client_communication.h
 *        \brief  Remote Client Communication.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_COMMUNICATION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_COMMUNICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include "amsr/generic/generic_error_domain.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/provided_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/provided_someip_eventgroup.h"
#include "amsr/someip_daemon_core/configuration/types/someip_event_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state_change_handler.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/logging/logging_utilities.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "amsr/someip_daemon_core/server/eventgroup/event_manager_interface.h"
#include "amsr/someip_daemon_core/server/eventgroup/event_message_dispatcher.h"
#include "amsr/someip_daemon_core/server/method_request_handler_interface.h"
#include "amsr/someip_daemon_core/server/remote_client_interface.h"
#include "amsr/someip_daemon_core/server/server_event_handler_interface.h"
#include "amsr/someip_daemon_core/server/server_field_cache_interface.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/ip_endpoint_option.h"
#include "ara/core/error_code.h"
#include "ara/core/map.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/language/location.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief RemoteClientCommunication.
 * \details When adding or removing required definitions in TemplateConfiguration,
 * remember to also update documentation of TemplateConfiguration in class template
 * RemoteClientManager!
 *
 * \tparam ConnectionManagerType Type of ConnectionManager
 */
template <typename ConnectionManagerType>
// VECTOR NC AutosarC++17_10-A10.1.1: MD_SomeIpDaemon_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
class RemoteClientCommunication : public connection_manager::ConnectionStateChangeHandler,
                                  public RemoteClientInterface,
                                  public ServerEventHandlerInterface {
 public:
  /*!
   * \brief Delete Default constructor.
   * \steady FALSE
   */
  RemoteClientCommunication() = delete;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief Default destructor.
   * \steady FALSE
   *
   * \internal
   * - Destroy the TCP receiver
   * - Destroy the UDP receiver
   * - Destroy the UDP multicast sender
   * - Unregister the SW event.
   * - In case the SW event unregistration in the reactor fails.
   *   - Log fatal and abort.
   * \endinternal
   */
  ~RemoteClientCommunication() noexcept override {
    tcp_receiver_.reset(nullptr);
    udp_receiver_.reset(nullptr);
    udp_mcast_sender_.reset(nullptr);

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

  RemoteClientCommunication(RemoteClientCommunication const&) = delete;
  RemoteClientCommunication(RemoteClientCommunication&&) = delete;
  RemoteClientCommunication& operator=(RemoteClientCommunication const&) & = delete;
  RemoteClientCommunication& operator=(RemoteClientCommunication&&) & = delete;

  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;

  /*!
   * \brief Alias for "configuration::Configuration".
   */
  using Config = configuration::Configuration;

  /*!
   * \brief Alias for map of ProvidedSomeIpEventgroup by Eventgroup ID.
   */
  using ProvidedEventgroupMap =
      ara::core::Map<someip_protocol::internal::EventgroupId, configuration::types::ProvidedSomeIpEventgroup>;

  /*!
   * \brief  A timeout list for Events message accumulation purposes.
   */
  using EventsMessageAccumulationTimeoutMap = std::unordered_map<
      someip_protocol::internal::EventId, std::chrono::nanoseconds, std::hash<someip_protocol::internal::EventId>,
      std::equal_to<someip_protocol::internal::EventId>,
      ara::core::PolymorphicAllocator<std::pair<someip_protocol::internal::EventId const, std::chrono::nanoseconds>>>;

  /*!
   * \brief Constructor.
   *
   * \param[in] someip_service_instance_id          The SOME/IP service instance information
   *                                                (service id, instance id, and  version).
   * \param[in] conman                              A connection manager.
   * \param[in] provided_endpoint                   A provided endpoint with the local network configuration and the
   *                                                multicast endpoint if exists.
   * \param[in] event_message_accumulation_timeouts Accumulation timeouts.
   * \param[in] provided_eventgroup_map             A map with the service instance provided eventgroups.
   * \param[in] reactor                             Reactor, used to trigger SW events for connection cleaup.
   *
   * \context Network
   * \steady FALSE
   *
   *
   * \internal
   * - Get the machine mapping container from the configuration
   * - If a TCP endpoint is configured
   *   - Register this provided service instance in the TCP endpoint
   * - If a UDP endpoint is configured
   *   - Register this provided service instance in the UDP endpoint
   * - Register a SW event for cleanup after disconnection detection.
   * - In case the SW event registration in the reactor fails.
   *   - Log fatal and abort.
   * \endinternal
   */
  RemoteClientCommunication(configuration::types::SomeIpServiceInstanceId const& someip_service_instance_id,
                            ConnectionManager& conman,
                            configuration::types::ProvidedNetworkEndpoint const& provided_endpoint,
                            EventsMessageAccumulationTimeoutMap const& event_message_accumulation_timeouts,
                            ProvidedEventgroupMap const& provided_eventgroup_map,
                            osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
      : connection_manager::ConnectionStateChangeHandler{},
        RemoteClientInterface{},
        ServerEventHandlerInterface{},
        conman_{conman},
        provided_endpoint_{provided_endpoint},
        someip_service_instance_id_{someip_service_instance_id},
        provided_eventgroup_map_{provided_eventgroup_map},
        reactor_{reactor},
        logger_{someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
                someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
                logging::LoggerPrefixGenerator::GetLoggerPrefix(
                    "RemoteClientCommunication", someip_service_instance_id_.deployment_id,
                    someip_service_instance_id_.instance_id,
                    someip_protocol::internal::IpAddress{provided_endpoint_.GetLocalEndpoint().address.ToString()})},
        event_message_accumulation_timeouts_{event_message_accumulation_timeouts} {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id{
        {someip_service_instance_id_.deployment_id.service_interface_id,
         someip_service_instance_id_.deployment_id.service_interface_version.major_version},
        someip_service_instance_id_.instance_id};

    if (provided_endpoint_.GetLocalEndpoint().tcp_port.has_value()) {
      TcpEndpointPtr const tcp_endpoint{conman.GetPassiveTcpEndpoint(
          amsr::someip_protocol::internal::IpAddress{provided_endpoint_.GetLocalEndpoint().address.ToString()},
          provided_endpoint_.GetLocalEndpoint().tcp_port->Value())};
      tcp_endpoint->RegisterProvidedServiceInstance(service_instance_id);
    }

    if (provided_endpoint_.GetLocalEndpoint().udp_port.has_value()) {
      UdpEndpointPtr const udp_endpoint{conman.GetUdpEndpoint(
          amsr::someip_protocol::internal::IpAddress{provided_endpoint_.GetLocalEndpoint().address.ToString()},
          provided_endpoint_.GetLocalEndpoint().udp_port->Value())};
      udp_endpoint->RegisterProvidedServiceInstance(service_instance_id);
    }

    // Register a SW event for disconnected client cleanup
    ::ara::core::Result<osabstraction::io::reactor1::CallbackHandle> register_software_event_result{
        reactor_.RegisterSoftwareEvent([this](osabstraction::io::reactor1::CallbackHandle,
                                              ::osabstraction::io::reactor1::EventTypes) { CleanupSubscriptions(); })};
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
   * \brief Offer a provided service instance.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - If the service is not offered
   *   - Reset any open connections to make sure no old clients still use the old connection.
   *   - Set as true is_service_offered flag
   *   - If port mapping configuration's TCP port is valid,
   *    - Get reference to the TCP receiver from the connection manager.
   *  - If port mapping configuration's UDP port is valid,
   *    - Get reference to the UDP receiver from the connection manager.
   *  - If port mapping configuration's event multicast address is valid,
   *    - Get the UDP sender based on port mapping configuration and set it for multi-casting.
   * - Otherwise, log error.
   * \endinternal
   */
  void OfferService() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    if (!is_service_offered_) {
      // Reset any open connections
      tcp_receiver_.reset(nullptr);
      udp_receiver_.reset(nullptr);
      udp_mcast_sender_.reset(nullptr);

      // Set as true service offered
      is_service_offered_ = true;

      if (provided_endpoint_.GetLocalEndpoint().tcp_port.has_value()) {
        tcp_receiver_ = conman_.GetPassiveTcpReceiver(
            amsr::someip_protocol::internal::IpAddress{provided_endpoint_.GetLocalEndpoint().address.ToString()},
            provided_endpoint_.GetLocalEndpoint().tcp_port->Value());
      }
      if (provided_endpoint_.GetLocalEndpoint().udp_port.has_value()) {
        static_cast<void>(
            conman_
                .GetUdpUnicastReceiver(
                    amsr::someip_protocol::internal::IpAddress{
                        provided_endpoint_.GetLocalEndpoint().address.ToString()},
                    provided_endpoint_.GetLocalEndpoint().udp_port->Value())
                .AndThen([this](typename ConnectionManager::UdpReceiverPtr&& udp_receiver_ptr) {
                  udp_receiver_ = std::move(udp_receiver_ptr);
                  return ara::core::Result<void>();
                })
                .InspectError([this](ara::core::ErrorCode error_code) {
                  logger_.LogFatalAndAbort(
                      [this, &error_code](ara::core::String& abort_msg) noexcept {
                        ara::core::StringView const error_msg{error_code.Message()};
                        ara::core::StringView const user_msg{error_code.UserMessage()};
                        ara::core::ErrorDomain::SupportDataType const support_data{error_code.SupportData()};
                        abort_msg.append("Failed to assign UDP address (address: ");
                        abort_msg.append(provided_endpoint_.GetLocalEndpoint().address.ToString());
                        abort_msg.append(", port: ");
                        // clang-format off
                        // VECTOR NC AutosarC++17_10-M5.0.4: MD_SomeIpDaemon_AutosarC++17_10-M5.0.4_implicit_integral_conversion_changes_signedness
                        // clang-format on
                        abort_msg.append(std::to_string(provided_endpoint_.GetLocalEndpoint().udp_port->Value()));
                        abort_msg.append(", error message: ");
                        abort_msg.append(error_msg);
                        abort_msg.append(", user message: ");
                        abort_msg.append(user_msg);
                        abort_msg.append(", support data: ");
                        abort_msg.append(std::to_string(support_data));
                        abort_msg.append(").");
                      },
                      AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
                }));

        if (provided_endpoint_.HasMulticastEndpoint()) {
          // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
          // VECTOR NC VectorC++-V5.0.1: MD_SomeIpDaemon_V5.0.1_falsepositive
          static_cast<void>(
              conman_
                  .GetUdpSender(
                      amsr::someip_protocol::internal::IpAddress{
                          provided_endpoint_.GetLocalEndpoint().address.ToString()},
                      provided_endpoint_.GetLocalEndpoint().udp_port->Value(),
                      amsr::someip_protocol::internal::IpAddress{
                          provided_endpoint_.GetMulticastEndpoint().address.ToString()},
                      provided_endpoint_.GetMulticastEndpoint().port.Value(), nullptr)
                  .AndThen([this](typename ConnectionManager::UdpSenderPtr&& udp_sender_ptr) {
                    udp_mcast_sender_ = std::move(udp_sender_ptr);
                    return ara::core::Result<void>();
                  })
                  .InspectError([this](ara::core::ErrorCode error_code) {
                    logger_.LogFatalAndAbort(
                        [this, &error_code](ara::core::String& abort_msg) noexcept {
                          ara::core::StringView const error_msg{error_code.Message()};
                          ara::core::StringView const user_msg{error_code.UserMessage()};
                          ara::core::ErrorDomain::SupportDataType const support_data{error_code.SupportData()};
                          abort_msg.append("Failed to assign UDP address (address: ");
                          abort_msg.append(provided_endpoint_.GetLocalEndpoint().address.ToString());
                          abort_msg.append(", port: ");
                          // clang-format off
                          // VECTOR NC AutosarC++17_10-M5.0.4: MD_SomeIpDaemon_AutosarC++17_10-M5.0.4_implicit_integral_conversion_changes_signedness
                          // clang-format on
                          abort_msg.append(std::to_string(provided_endpoint_.GetLocalEndpoint().udp_port->Value()));
                          abort_msg.append(", event multicast port: ");
                          // clang-format off
                          // VECTOR NC AutosarC++17_10-M5.0.4: MD_SomeIpDaemon_AutosarC++17_10-M5.0.4_implicit_integral_conversion_changes_signedness
                          // clang-format on
                          abort_msg.append(std::to_string(provided_endpoint_.GetMulticastEndpoint().port.Value()));
                          abort_msg.append(", error message: ");
                          abort_msg.append(error_msg);
                          abort_msg.append(", user message: ");
                          abort_msg.append(user_msg);
                          abort_msg.append(", support data: ");
                          abort_msg.append(std::to_string(support_data));
                          abort_msg.append(").");
                        },
                        AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
                  }));
        }
      }
    } else {
      logger_.LogDebug([](ara::log::LogStream& s) noexcept { s << "Service already offered"; },
                       static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Stop offering a provided service instance.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Clear event group subscriptions.
   * - Set as false is_service_offered flag.
   * \endinternal
   */
  void StopOfferService() noexcept override {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    // Release UDP resources
    udp_receiver_.reset(nullptr);
    udp_mcast_sender_.reset(nullptr);
    // Set as false service offered
    is_service_offered_ = false;

    eventgroup_subscriptions_.clear();
  }

  /*!
   * \brief Add a new subscriber for a given event group.
   *
   * \param[in] udp_endpoint An optional containing IP address and port using UDP from a provided service instance.
   * \param[in] tcp_endpoint An optional containing IP address and port using TCP from a provided service instance.
   * \param[in] eventgroup_id A SOME/IP event group identifier.
   * \return A result with the value void if successful, with an error otherwise.
   * \error  amsr::generic::GenErrc::kSystemConfiguration if the corresponding
   *         TCP connection was not found.
   *
   * \pre A TCP connection must have been opened by the client before calling this function if tcp_endpoint is not
   *      empty.
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \trace SPEC-4980075, SPEC-4980110, SPEC-4981705, SPEC-4981706, SPEC-4981708, SPEC-4981712
   *
   * \internal
   * - Try to find the event within the provided eventgroup map.
   * - If found:
   *  - If TCP is required (configuration contains TCP event),
   *    get TCP sender from the connection manager.
   *  - If UDP is required (configuration contains UDP event),
   *    get UDP sender from the connection manager.
   *  - If service has been already offered, send initial field events for every new subscriber.
   *  - Register the subscriber in the container of subscriptions.
   * - Else:
   *  - Log an error.
   * \endinternal
   */
  // VECTOR NC Metric-HIS.PATH: MD_SomeIpDaemon_Metric-HIS.PATH
  ara::core::Result<void> SubscribeEventgroup(
      ara::core::Optional<amsr::someip_daemon_core::service_discovery ::message ::options::IpEndpointOption> const&
          udp_endpoint,
      ara::core::Optional<amsr::someip_daemon_core::service_discovery ::message ::options::IpEndpointOption> const&
          tcp_endpoint,
      someip_protocol::internal::EventgroupId const eventgroup_id) noexcept override {
    ara::core::Result<void> result{amsr::generic::GenErrc::kSystemConfiguration};

    ProvidedEventgroupMap::const_iterator const eg{provided_eventgroup_map_.find(eventgroup_id)};
    if (eg != provided_eventgroup_map_.cend()) {
      // Create new subscriber
      EventgroupSubscriberType subscriber{};

      bool const tcp_required{eg->second.contains_tcp_event};
      bool has_tcp_sender_error{false};
      // Get source of SOME/IP event message from the configuration
      if (tcp_required && tcp_endpoint.has_value()) {
        // Note: A string must be used because the connection manager APIs requires it. It must be replaced whenever it
        // is steady
        // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
        // VECTOR NC VectorC++-V5.0.1: MD_SomeIpDaemon_V5.0.1_falsepositive
        ara::core::Result<TcpSenderPtr> tcp_sender_result{conman_.GetPassiveTcpSender(
            amsr::someip_protocol::internal::IpAddress{provided_endpoint_.GetLocalEndpoint().address.ToString()},
            provided_endpoint_.GetLocalEndpoint().tcp_port->Value(),
            amsr::someip_protocol::internal::IpAddress{tcp_endpoint->address.ToString()}, tcp_endpoint->port.port)};
        if (tcp_sender_result.HasValue()) {
          subscriber.tcp_sender = std::move(tcp_sender_result.Value());
          subscriber.tcp_sender->SetConnectionStateChangeHandler(this);
        } else {
          logger_.LogError(
              [this, &tcp_endpoint, &eventgroup_id](ara::log::LogStream& s) {
                s << "Tcp sender could not be created. Received new eventgroup subscription from: Tcp ";
                s << tcp_endpoint->address.ToString();
                s << ",";
                s << tcp_endpoint->port.port;
                s << ". Local TCP: address ";
                s << provided_endpoint_.GetLocalEndpoint().address.ToString();
                s << ", port ";
                s << provided_endpoint_.GetLocalEndpoint().tcp_port->Value();
                s << " eventgroup id 0x";
                s << ara::log::HexFormat(eventgroup_id);
              },
              static_cast<char const*>(__func__), __LINE__);
          has_tcp_sender_error = true;
        }
      }

      if (!has_tcp_sender_error) {
        bool const udp_required{eg->second.contains_udp_event};
        if (udp_required && udp_endpoint.has_value() && (provided_endpoint_.GetLocalEndpoint().udp_port)) {
          // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
          // VECTOR NC VectorC++-V5.0.1: MD_SomeIpDaemon_V5.0.1_falsepositive
          static_cast<void>(
              conman_
                  .GetUdpSender(
                      amsr::someip_protocol::internal::IpAddress{
                          provided_endpoint_.GetLocalEndpoint().address.ToString()},
                      provided_endpoint_.GetLocalEndpoint().udp_port->Value(),
                      // Note: A string must be used because the connection manager APIs requires it. It must be
                      // replaced whenever it is steady
                      amsr::someip_protocol::internal::IpAddress{udp_endpoint->address.ToString()},
                      udp_endpoint->port.port, nullptr)
                  .AndThen([&subscriber, this](typename ConnectionManager::UdpSenderPtr&& udp_sender_ptr) {
                    subscriber.udp_sender = std::move(udp_sender_ptr);
                    subscriber.udp_sender->SetConnectionStateChangeHandler(this);
                    return ara::core::Result<void>();
                  }));
        }
        result.EmplaceValue();

        if (is_service_offered_ && eg->second.contains_field) {
          // Send initial field event for every new subscriber.
          SendInitialEvents(eg->second.events, subscriber);
        }

        // Register the subscriber in the container of subscriptions
        eventgroup_subscriptions_[eventgroup_id].emplace_back(std::move(subscriber));
        logger_.LogDebug(
            [this, &tcp_endpoint, &udp_endpoint, &eventgroup_id](ara::log::LogStream& s) {
              std::size_t const subscriptions_count{eventgroup_subscriptions_.at(eventgroup_id).size()};
              s << "Received new eventgroup subscription from: ";
              if (tcp_endpoint.has_value()) {
                s << "tcp address: ";
                s << tcp_endpoint->address.ToString();
                s << ", tcp port: ";
                s << tcp_endpoint->port.port;
              } else {
                s << "empty TCP endpoint";
              }
              if (udp_endpoint.has_value()) {
                s << ", udp address: ";
                s << udp_endpoint->address.ToString();
                s << ", udp port: ";
                s << udp_endpoint->port.port;
              } else {
                s << "empty UDP endpoint";
              }
              s << ", eventgroup id 0x";
              s << ara::log::HexFormat(eventgroup_id);
              s << ". Number of eventgroup subscribers: ";
              s << subscriptions_count;
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError(
          [&eventgroup_id](ara::log::LogStream& s) {
            s << "(EventgroupId: 0x";
            s << ara::log::HexFormat(eventgroup_id);
            s << ") is not configured for this provided instance.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

  /*!
   * \brief Remove a subscriber for a given event group.
   *
   * \param[in] udp_endpoint An optional containing IP address and port using UDP from a provided service instance.
   * \param[in] tcp_endpoint An optional containing IP address and port using TCP from a provided service instance.
   * \param[in] eventgroup_id A SOME/IP event group identifier.
   *
   * \context Network
   * \reentrant FALSE
   * \steady FALSE
   *
   * \trace SPEC-4980073
   * \trace SPEC-4980108
   * \trace SPEC-4981708
   *
   * \internal
   * - Find a matching subscription in the subscription container
   *   - Check if the TCP entry of the subscription matches the given address and port
   *   - Check if the UDP entry of the subscription matches the given address and port
   *   - If both TCP and UDP addresses are matching: subscription found
   * - If subscription found, delete it from the subscription container
   * \endinternal
   */
  void UnsubscribeEventgroup(
      ara::core::Optional<service_discovery::message::options::IpEndpointOption> const& udp_endpoint,
      ara::core::Optional<service_discovery::message::options::IpEndpointOption> const& tcp_endpoint,
      someip_protocol::internal::EventgroupId const eventgroup_id) noexcept override {
    // Look for the subscription in the map
    typename EventgroupSubscriberContainer::const_iterator const iter_begin{
        eventgroup_subscriptions_.at(eventgroup_id).cbegin()};
    typename EventgroupSubscriberContainer::const_iterator const iter_end{
        eventgroup_subscriptions_.at(eventgroup_id).cend()};

    typename EventgroupSubscriberContainer::const_iterator const it{
        std::find_if(iter_begin, iter_end,
                     [this, eventgroup_id, &udp_endpoint, &tcp_endpoint](EventgroupSubscriberType const& subscriber) {
                       bool const matching_tcp{IsMatchingTcpSubscription(tcp_endpoint, eventgroup_id, subscriber)};

                       bool const matching_udp{IsMatchingUdpSubscription(udp_endpoint, eventgroup_id, subscriber)};

                       // Both must be found for a match
                       return matching_tcp && matching_udp;
                     })};
    assert(it != iter_end);

    // Delete the entry
    EventgroupSubscriberContainer& eventgroup_subscriptions{eventgroup_subscriptions_[eventgroup_id]};

    static_cast<void>(eventgroup_subscriptions.erase(it));

    std::size_t const subscriptions_count{eventgroup_subscriptions_.at(eventgroup_id).size()};
    logger_.LogDebug(
        [&tcp_endpoint, &udp_endpoint, &eventgroup_id, &subscriptions_count](ara::log::LogStream& s) {
          s << "Received eventgroup unsubscription from: ";
          if (tcp_endpoint.has_value()) {
            s << "tcp address: ";
            s << tcp_endpoint->address.ToString();
            s << ", tcp port: ";
            s << tcp_endpoint->port.port;
          } else {
            s << "empty TCP endpoint";
          }
          if (udp_endpoint.has_value()) {
            s << ", udp address: ";
            s << udp_endpoint->address.ToString();
            s << ", udp port: ";
            s << udp_endpoint->port.port;
          } else {
            s << "empty UDP endpoint";
          }
          s << " eventgroup id 0x";
          s << ara::log::HexFormat(eventgroup_id);
          s << ". Remaining subscribers: ";
          s << subscriptions_count;
        },
        static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief Test whether a TCP connection with the specified remote IP address and remote port exist or not.
   *
   * \param[in] address A remote IP address.
   * \param[in] port    A remote port number.
   *
   * \return "true" if the specified connection exists and "false" otherwise.
   *
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Return the status of TCP passive connection.
   * \endinternal
   */
  bool HasTcpConnection(someip_daemon_core::IpAddress const& address,
                        amsr::net::ip::Port const port) const noexcept override {
    bool result{false};
    if (provided_endpoint_.GetLocalEndpoint().tcp_port.has_value()) {
      // VECTOR NC VectorC++-V5.0.1: MD_SomeIpDaemon_V5.0.1_falsepositive
      // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
      result = conman_.HasTcpPassiveConnection(
          amsr::someip_protocol::internal::IpAddress{provided_endpoint_.GetLocalEndpoint().address.ToString()},
          provided_endpoint_.GetLocalEndpoint().tcp_port->Value(),
          amsr::someip_protocol::internal::IpAddress{address.ToString()}, port.port);
    }
    return result;
  }

  /*!
   * \brief       Set the event manager.
   * \param[in]   event_manager A pointer to the associated EventManager Instance.
   *
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void SetEventManager(eventgroup::EventManagerInterface* event_manager) noexcept { event_manager_ = event_manager; }

  /*!
   * \brief Send a SOME/IP event message to a remote peer.
   *
   * \param[in] packet A SOME/IP message.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kSystemConfiguration If the event is not found
   *        in any provided eventgroup.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the event couldn't be
   *        forwarded.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   * \pre       Service must be offered.
   *
   * \internal
   * - If the service is not offered, Log fatal and abort.
   * - Set the return value to indicate failure.
   * - Log the event ID.
   * - Look for the event in the configuration
   * - If a valid event is found,
   *   - Use the event message dispatcher to forward the event.
   * - Return the return value.
   * \endinternal
   */

  ara::core::Result<void> SendSomeIpEvent(SomeIpPacketSharedPtr packet) noexcept override {
    if (!is_service_offered_) {
      // The Service must be offered.
      logger_.LogFatalAndAbort(
          [](ara::core::String& abort_msg) noexcept { abort_msg.append("The Service is not offered"); }, AMSR_FILE,
          static_cast<char const*>(__func__), __LINE__);
    }

    ara::core::Result<void> result{ara::core::ErrorCode{amsr::generic::GenErrc::kSystemConfiguration}};
    someip_protocol::internal::SomeIpMessageHeader const& header{packet->GetHeader()};
    someip_protocol::internal::EventId const event_id{header.method_id_};

    logger_.LogVerbose(
        [&event_id](ara::log::LogStream& s) {
          s << "(EventId: 0x";
          // Append the event Id in the correct format
          logging::LoggingUtilities::LogEventId(s, event_id);
          s << ")";
        },
        static_cast<char const*>(__func__), __LINE__);

    ara::core::Result<configuration::types::SomeIpEventDeployment> const event_config{FindEventConfiguration(event_id)};

    if (event_config.HasValue()) {
      // Add the accumulation timeout
      packet->SetAccumulationTimeout(GetMessageAccumulationTimeout(event_id, event_message_accumulation_timeouts_));
      // In case the service is offered and transmission succeeded, we must return success
      result = event_message_dispatcher_.DispatchSomeIpMessage(
          packet, eventgroup_subscriptions_, provided_eventgroup_map_, event_config.Value(), udp_mcast_sender_.get());
    }
    return result;
  }

  /*!
   * \brief Send a PDU event message to a remote peer.
   *
   * \param[in] packet A PDU message.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kSystemConfiguration If the event is not found
   *        in any provided eventgroup.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the event couldn't be
   *        forwarded.
   *
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   * \pre       Service must be offered.
   *
   * \internal
   * - If the service is not offered, Log fatal and abort.
   * - Set the return value to indicate failure.
   * - Log the event ID.
   * - Try getting event from the configuration.
   * - If a valid event is found,
   *   - Use the event message dispatcher to forward the event.
   * - Return the return value.
   * \endinternal
   */
  ara::core::Result<void> SendPduEvent(PduPacketSharedPtr packet) noexcept override {
    if (!is_service_offered_) {
      // The Service must be offered.
      logger_.LogFatalAndAbort(
          [](ara::core::String& abort_msg) noexcept { abort_msg.append("The Service is not offered."); }, AMSR_FILE,
          static_cast<char const*>(__func__), __LINE__);
    }

    ara::core::Result<void> result{amsr::generic::GenErrc::kSystemConfiguration};
    someip_protocol::internal::PduMessageHeader const& header{packet->GetHeader()};
    someip_protocol::internal::EventId const event_id{
        someip_protocol::internal::PduMessageHeaderUtility::GetMethodId(header)};

    logger_.LogDebug(
        [&event_id](ara::log::LogStream& s) {
          s << "(EventId: 0x";
          // Append the event Id in the correct format
          logging::LoggingUtilities::LogEventId(s, event_id);
          s << ")";
        },
        static_cast<char const*>(__func__), __LINE__);

    ara::core::Result<configuration::types::SomeIpEventDeployment> const event_config{FindEventConfiguration(event_id)};

    if (event_config.HasValue()) {
      // Add the accumulation timeout
      packet->SetAccumulationTimeout(GetMessageAccumulationTimeout(event_id, event_message_accumulation_timeouts_));
      result = event_message_dispatcher_.DispatchPduMessage(packet, eventgroup_subscriptions_, provided_eventgroup_map_,
                                                            event_config.Value(), udp_mcast_sender_.get());
    }

    return result;
  }

  /*!
   * \brief Sends a SOME/IP response message to a remote source.
   *
   * \param[in] packet           A SOME/IP response message.
   * \param[in] recipient        A remote source identifier.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the method response couldn't be forwarded.
   *
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Use the local endpoint which provides the service.
   * - If the response shall be sent over UDP protocol.
   *   - Send the method response using the configured local UdpEndpoint.
   *   - If sending fails, log an error.
   * - Else
   *   - Get a TcpSender and forward the method response to the remote application.
   *   - If sending fails, log an error.
   * \endinternal
   */
  ara::core::Result<void> SendMethodResponse(SomeIpPacketSharedPtr packet,
                                             packet_router::RemoteSourceIdentifier const& recipient) const noexcept {
    someip_protocol::internal::SomeIpMessageHeader const& header{packet->GetHeader()};
    someip_protocol::internal::ServiceId const service_id{header.service_id_};
    someip_protocol::internal::MethodId const method_id{header.method_id_};
    someip_protocol::internal::ClientId const client_id{header.client_id_};
    someip_protocol::internal::SessionId const remote_session_id{header.session_id_};
    someip_protocol::internal::InterfaceVersion const interface_version{header.interface_version_};
    someip_protocol::internal::InstanceId const instance_id{someip_service_instance_id_.instance_id};

    ara::core::Result<void> result{amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed};
    bool response_is_sent{false};

    auto const local_endpoint = provided_endpoint_.GetLocalEndpoint();

    if (recipient.protocol_ == configuration::ConfigurationTypesAndDefs::Protocol::kUDP) {
      UdpEndpointPtr const udp_endpoint{
          conman_.GetUdpEndpoint(amsr::someip_protocol::internal::IpAddress{local_endpoint.address.ToString()},
                                 local_endpoint.udp_port.value().Value())};

      if (udp_endpoint != nullptr) {
        response_is_sent = udp_endpoint->SendMethodResponse(packet, recipient);
        if (response_is_sent) {
          result.EmplaceValue();
        }
      }
    } else {
      auto const tcp_sender = conman_.GetPassiveTcpSender(
          amsr::someip_protocol::internal::IpAddress{local_endpoint.address.ToString()},
          local_endpoint.tcp_port.value().Value(),
          amsr::someip_protocol::internal::IpAddress{recipient.address_.ToString()}, {recipient.port_.port});
      if (tcp_sender.HasValue()) {
        response_is_sent = tcp_sender.Value()->ForwardFromLocal(packet);
      } else {
        logger_.LogError(
            [&local_endpoint, &recipient](ara::log::LogStream& s) {
              s << "Tcp sender could not be created to: ";
              s << recipient.address_.ToString();
              s << ",";
              s << recipient.port_.port;
              s << ". From local TCP: address ";
              s << local_endpoint.address.ToString();
              s << ", port ";
              s << local_endpoint.tcp_port->Value();
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      if (response_is_sent) {
        result.EmplaceValue();
      }
    }
    // Log error if failed to send the method response.
    if (!response_is_sent) {
      logger_.LogError(
          [&service_id, &interface_version, &instance_id, &method_id, &client_id,
           &remote_session_id](ara::log::LogStream& s) {
            s << "Response (ServiceId: 0x";
            s << ara::log::HexFormat(service_id);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(interface_version);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(instance_id);
            s << ", MethodId: 0x";
            s << ara::log::HexFormat(method_id);
            s << ", ClientId: 0x";
            s << ara::log::HexFormat(client_id);
            s << ", SessionId: 0x";
            s << ara::log::HexFormat(remote_session_id);
            s << ") could not be sent";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

  /*!
   * \brief Register the field cache handler.
   *
   * \param[in] server_field_cache    The field cache handler.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  void RegisterFieldCacheHandler(ServerCacheInterface* server_field_cache) noexcept override {
    assert(server_field_cache != nullptr);
    assert(server_field_cache_ == nullptr);
    server_field_cache_ = server_field_cache;
  }

 protected:
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SomeIpDaemon_AutosarC++17_10-M0.1.8_functions_with_no_side_effects
  /*!
   * \brief             Called when a TCP connection state is changed.
   * \param[in]         sender The TCP sender associated to the connection that changed its state.
   * \context           Network
   * \reentrant         FALSE
   * \steady            FALSE
   *
   * \internal
   * - Trigger the SW event, to handle the disconnection cleanup in the next reactor call
   * - If trigger fails
   *   - Log fatal and abort
   * \endinternal
   */
  void OnConnectionStateChange(TcpSenderPtr sender) noexcept override {
    static_cast<void>(sender);

    logger_.LogDebug([](ara::log::LogStream& s) { s << "Client disconnection detected."; },
                     static_cast<char const*>(__func__), __LINE__);

    // Trigger the SW event to do the cleanup to avoid deleting the connection object from within
    // itself (Network call context)
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

  /*!
   * \brief             Called when a TCP connection is disconnected.
   * \context           Timer
   * \reentrant         FALSE
   * \steady            FALSE
   *
   * \internal
   * - For each eventgroup
   *   - For each eventgroup subscription
   *     - If the TCP connection is disconnected
   *       - Trigger the event manager to cancel the subscription
   * - Delete all subscriptions with a broken connection
   * \endinternal
   */
  void CleanupSubscriptions() noexcept {
    for (typename SomeIpEventGroupSubscriptionMap::value_type& subscription : eventgroup_subscriptions_) {
      typename EventgroupSubscriberContainer::iterator const iter_begin{subscription.second.begin()};
      typename EventgroupSubscriberContainer::iterator const iter_end{subscription.second.end()};
      if (iter_begin != iter_end) {
        typename EventgroupSubscriberContainer::iterator const new_begin_to_erase{std::remove_if(
            iter_begin, iter_end, [this, &subscription](EventgroupSubscriberType const& subscriber) noexcept {
              bool do_cleanup{false};
              // Check there is a TCP subscriber and it is connected
              if ((subscriber.tcp_sender != nullptr) && (!subscriber.tcp_sender->IsConnected())) {
                // Notify the event manager
                if (event_manager_ != nullptr) {
                  event_manager_->CancelEventgroupSubscriptionUponTcpDisconnection(
                      subscriber.tcp_sender->GetRemoteAddress(), subscriber.tcp_sender->GetRemotePort(),
                      subscription.first);
                }
                do_cleanup = true;
              }

              return do_cleanup;
            })};
        someip_protocol::internal::EventgroupId const eventgroup_id{subscription.first};
        static_cast<void>(eventgroup_subscriptions_[eventgroup_id].erase(new_begin_to_erase, iter_end));
      }
    }
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SomeIpDaemon_AutosarC++17_10-M0.1.8_functions_with_no_side_effects
  /*!
   * \brief             Called when a UDP connection state is changed.
   * \param[in]         sender The UDP sender associated to the connection that changed its state.
   * \context           Network
   * \reentrant         FALSE
   * \steady            FALSE
   */
  void OnConnectionStateChange(UdpSenderPtr sender) noexcept override {
    static_cast<void>(sender);

    logger_.LogDebug(
        [this](ara::log::LogStream& s) {
          typename SomeIpEventGroupSubscriptionMap::iterator map_it{eventgroup_subscriptions_.begin()};
          typename SomeIpEventGroupSubscriptionMap::iterator const map_end{eventgroup_subscriptions_.end()};
          for (; map_it != map_end; map_it++) {
            typename EventgroupSubscriberContainer::const_iterator const iter_begin{map_it->second.cbegin()};
            typename EventgroupSubscriberContainer::const_iterator const iter_end{map_it->second.cend()};

            s << "Event group ID " << map_it->first << ": ";
            static_cast<void>(std::for_each(iter_begin, iter_end, [&s](EventgroupSubscriberType const& subscriber) {
              if (subscriber.udp_sender != nullptr) {
                ara::core::String const remote_address_string{subscriber.udp_sender->GetRemoteAddress().ToString()};
                std::uint16_t const port{subscriber.udp_sender->GetRemotePort().port};

                ara::core::String connected_state_string{};
                if (subscriber.udp_sender->IsConnected()) {
                  connected_state_string = "connected";
                } else {
                  connected_state_string = "NOT connected";
                }
                s << "remote address: " << remote_address_string << ", remote port: " << port << ", "
                  << connected_state_string << "\n";
              }
            }));
          }
        },
        static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief The TCP sender pointer type.
   */
  using TcpSenderPtr = typename ConnectionManager::TcpSenderPtr;

  /*!
   * \brief The UDP sender pointer type.
   */
  using UdpSenderPtr = typename ConnectionManager::UdpSenderPtr;

  /*!
   * \brief Alias for EventgroupSubscriber type.
   */
  using EventgroupSubscriberType = eventgroup::EventgroupSubscriber<UdpSenderPtr, TcpSenderPtr>;

  /*!
   * \brief The TCP receiver pointer type.
   */
  using TcpReceiverPtr = typename ConnectionManager::TcpReceiverPtr;

  /*!
   * \brief The UDP receiver pointer type.
   */
  using UdpReceiverPtr = typename ConnectionManager::UdpReceiverPtr;

  /*!
   * \brief A container of eventgroup subscriptions.
   */
  using EventgroupSubscriberContainer = ara::core::Vector<EventgroupSubscriberType>;

  /*!
   * \brief Type alias for address/port pair.
   */
  using AddressPair = std::pair<someip_protocol::internal::IpAddress, someip_protocol::internal::Port>;

  /*!
   * \brief A map from a SOME/IP event-group subscriptions.
   */
  using SomeIpEventGroupSubscriptionMap =
      std::unordered_map<someip_protocol::internal::EventgroupId, EventgroupSubscriberContainer,
                         std::hash<someip_protocol::internal::EventgroupId>,
                         std::equal_to<someip_protocol::internal::EventgroupId>,
                         ara::core::PolymorphicAllocator<
                             std::pair<someip_protocol::internal::EventgroupId const, EventgroupSubscriberContainer>>>;

 private:
  /*!
   * \brief       Sends all event-mapped field cached values of the given event map to the passed TCP and UDP sinks.
   * \details     If the pointer to the protocol packet sink for this eventgroup is invalid, nothing is sent and the
   *              method fails silently.
   * \param[in]   events      A provided events map by one eventgroup.
   * \param[in]   subscriber  Event group subscriber type.

   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \trace SPEC-4981711
   *
   * \internal
   *  - For every event in event map:
   *    - If the event is a field:
   *      - Forward the cached field value to the subscriber.
   * \endinternal
   */
  void SendInitialEvents(configuration::types::ProvidedSomeIpEventMap const& events,
                         EventgroupSubscriberType const& subscriber) const noexcept {
    logger_.LogVerbose(
        [this](ara::log::LogStream& s) {
          s << "ServiceId: ";
          s << ara::log::HexFormat(someip_service_instance_id_.deployment_id.service_interface_id);
          s << ", MajorVersion: 0x";
          s << ara::log::HexFormat(someip_service_instance_id_.deployment_id.service_interface_version.major_version);
          s << ", InstanceId: 0x";
          s << ara::log::HexFormat(someip_service_instance_id_.instance_id);
        },
        static_cast<char const*>(__func__), __LINE__);
    configuration::types::ProvidedSomeIpEventMap::const_iterator event;
    for (event = events.cbegin(); event != events.cend(); event++) {
      if (event->second.is_field) {
        // The field cache must already contain an entry for this event ID, as it is initialized during construction
        assert(server_field_cache_ != nullptr);
        SomeIpPacketSharedPtr const packet{server_field_cache_->GetCacheField(event->second.event_id)};

        // Initial field notifications must always be cached when a subscription is received and the service is offered
        // so if the packet is not available at this point, it is considered an implementation issue
        assert(packet != nullptr);

        bool send_success{true};
        if (event->second.transport_protocol == configuration::types::TransportProtocol::kTCP) {
          if (subscriber.tcp_sender != nullptr) {
            send_success = subscriber.tcp_sender->ForwardFromLocal(packet);
          }
        } else if (subscriber.udp_sender != nullptr) {
          // Add the accumulation timeout
          packet->SetAccumulationTimeout(
              GetMessageAccumulationTimeout(event->second.event_id, event_message_accumulation_timeouts_));

          // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
          send_success = subscriber.udp_sender->ForwardFromLocal(packet);
        } else {
          // empty on purpose (Bauhaus).
        }
        if (!send_success) {
          logger_.LogError(
              [&event](ara::log::LogStream& s) {
                s << "Failed to send initial field event (EventId: 0x";
                // Append the event Id in the correct format
                logging::LoggingUtilities::LogEventId(s, event->second.event_id);
                s << ").";
              },
              static_cast<char const*>(__func__), __LINE__);
        }
      }
    }
  }

  /*!
   * \brief Tries to find an event within the provided eventgroup map.
   *
   * \param[in] event_id   An event to look for its configuration.
   * \return  A Result containing the event deployment if the event id is found in a provided eventgroup
   * \error   amsr::generic::GenErrc::kSystemConfiguration  If the event is not found
   * in any provided eventgroup.
   *
   * \steady TRUE
   *
   * \internal
   * - For every eventgroup within the deployed eventgroups:
   *  - Try to find the event id in the eventgroup.
   * - If the event is found:
   *  - Return a Result containing the event deployment.
   * - Else:
   *  - Return an empty Result containing a amsr::generic::GenErrc::kSystemConfiguration error.
   * \endinternal
   */
  ara::core::Result<configuration::types::SomeIpEventDeployment> FindEventConfiguration(
      someip_protocol::internal::EventId const event_id) const noexcept {
    ara::core::Result<configuration::types::SomeIpEventDeployment> result{amsr::generic::GenErrc::kSystemConfiguration};
    configuration::types::ProvidedSomeIpEventMap::const_iterator event_config;
    ProvidedEventgroupMap::const_iterator eg;
    for (eg = provided_eventgroup_map_.cbegin(); eg != provided_eventgroup_map_.cend(); ++eg) {
      event_config = eg->second.events.find(event_id);
      if (event_config != eg->second.events.cend()) {
        result.EmplaceValue(event_config->second);
        break;
      }
    }
    return result;
  }

  /*!
   * \brief Check if the TCP entry of the subscription matches the given address and port
   *
   * \param[in] tcp_endpoint An optional containing IP address and port using TCP from a provided service instance.
   * \param[in] eventgroup_id A SOME/IP event group identifier.
   * \param[in] subscriber    The eventgroup subscriber containing the UDP endpoint
   * \return "true" if TCP subscription matches and "false" otherwise.
   *
   * \context Network
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Check if the TCP entry of the subscription matches the given address and port
   *   - If TCP is required
   *     - If a valid TCP address is provided and it matches with the subscription TCP address --> TCP matching
   *     - If no valid TCP address is provided and no TCP address is registered for this subscription --> TCP matching
   *   - If TCP is not required --> TCP matching
   * \endinternal
   */
  bool IsMatchingTcpSubscription(
      ara::core::Optional<service_discovery::message::options::IpEndpointOption> const& tcp_endpoint,
      someip_protocol::internal::EventgroupId const eventgroup_id,
      EventgroupSubscriberType const& subscriber) const noexcept {
    bool matching_tcp{false};

    // Check if TCP subscription matches
    bool tcp_required{false};
    ProvidedEventgroupMap::const_iterator const eg{provided_eventgroup_map_.find(eventgroup_id)};

    if (eg != provided_eventgroup_map_.cend()) {
      tcp_required = eg->second.contains_tcp_event;
    }

    if (tcp_required) {
      if (tcp_endpoint.has_value()) {
        if (subscriber.tcp_sender != nullptr) {
          if ((subscriber.tcp_sender->GetRemoteAddress() == tcp_endpoint->address) &&
              (subscriber.tcp_sender->GetRemotePort().port == tcp_endpoint->port.port)) {
            matching_tcp = true;
          }
        }
      } else if (subscriber.tcp_sender == nullptr) {  // No TCP subscription
        matching_tcp = true;
      } else {
        // empty on purpose (Bauhaus).
      }
    } else {
      // If no TCP is required, there must not be a TCP sender
      assert(subscriber.tcp_sender == nullptr);
      // This subscriber is considered a hit from the TCP sender point of view
      matching_tcp = true;
    }

    return matching_tcp;
  }

  /*!
   * \brief Check if the UDP entry of the subscription matches the given address and port
   *
   * \param[in] udp_endpoint An optional containing IP address and port using UDP from a provided service instance.
   * \param[in] eventgroup_id A SOME/IP event group identifier.
   * \param[in] subscriber    The eventgroup subscriber containing the UDP endpoint
   * \return "true" if UDP subscription matches and "false" otherwise.
   *
   * \context Network
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Check if the UDP entry of the subscription matches the given address and port
   *  - If UDP is required
   *    - If a valid UDP address is provided and it matches with the subscription UDP address --> UDP matching
   *    - If no valid UDP address is provided and no UDP address is registered for this subscription --> UDP matching
   *  - If UDP is not required --> UDP matching
   * \endinternal
   */
  bool IsMatchingUdpSubscription(
      ara::core::Optional<service_discovery::message::options::IpEndpointOption> const& udp_endpoint,
      someip_protocol::internal::EventgroupId const eventgroup_id,
      EventgroupSubscriberType const& subscriber) const noexcept {
    bool matching_udp{false};
    // Check if UDP subscription matches
    bool udp_required{false};
    ProvidedEventgroupMap::const_iterator const eg{provided_eventgroup_map_.find(eventgroup_id)};

    if (eg != provided_eventgroup_map_.cend()) {
      udp_required = eg->second.contains_udp_event;
    }
    if (udp_required) {
      if (udp_endpoint.has_value()) {
        if (subscriber.udp_sender != nullptr) {
          if ((subscriber.udp_sender->GetRemoteAddress() == udp_endpoint->address) &&
              (subscriber.udp_sender->GetRemotePort().port == udp_endpoint->port.port)) {
            matching_udp = true;
          }
        }
      } else if (subscriber.udp_sender == nullptr) {  // No UDP subscription
        matching_udp = true;
      } else {
        // empty on purpose (Bauhaus).
      }

    } else {
      // If no UDP is required, there must not be a UDP sender
      assert(subscriber.udp_sender == nullptr);
      // This subscriber is considered a hit from the UDP sender point of view
      matching_udp = true;
    }

    return matching_udp;
  }

  /*!
   * \brief Type alias for ConnectionManager::TcpEndpointPtr
   */
  using TcpEndpointPtr = typename ConnectionManager::TcpEndpointPtr;

  /*!
   * \brief Type alias for ConnectionManager::UdpEndpointPtr
   */
  using UdpEndpointPtr = typename ConnectionManager::UdpEndpointPtr;

  /*!
   * \brief Get the message accumulation timeout for a given event
   *
   * \param[in] id       The Event id
   * \param[in] timeouts List on which to search for the event id
   * \return An optional containing the UDP message accumulation timeout or empty if not found
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  static ara::core::Optional<std::chrono::nanoseconds> GetMessageAccumulationTimeout(
      someip_protocol::internal::EventId const id, EventsMessageAccumulationTimeoutMap const& timeouts) noexcept {
    std::decay_t<decltype(timeouts)>::const_iterator const it{timeouts.find(id)};
    return (it == timeouts.cend()) ? ara::core::Optional<std::chrono::nanoseconds>{ara::core::nullopt}
                                   : ara::core::Optional<std::chrono::nanoseconds>{it->second};
  }

  /*!
   * \brief A connection manager reference.
   */
  ConnectionManager& conman_;

  /*!
   * \brief A network of the provided endpoint, contains the multicast and unicast endpoints.
   */
  configuration::types::ProvidedNetworkEndpoint const provided_endpoint_;

  /*!
   * \brief The someip service instance id.
   */
  configuration::types::SomeIpServiceInstanceId const someip_service_instance_id_;

  /*!
   * \brief A TCP receiver.
   */
  TcpReceiverPtr tcp_receiver_{nullptr};

  /*!
   * \brief A UDP receiver.
   */
  UdpReceiverPtr udp_receiver_{nullptr};

  /*!
   * \brief A UDP multicast sender.
   */
  UdpSenderPtr udp_mcast_sender_{nullptr};

  /*!
   * \brief A provided SOME IP eventgroup map by eventgroup id.
   */
  ProvidedEventgroupMap const provided_eventgroup_map_;

  /*!
   * \brief The reactor, used to trigger SW events for cleanup after disconnection detection.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief A map from a SOME/IP event-group subscriptions.
   */
  SomeIpEventGroupSubscriptionMap eventgroup_subscriptions_{};

  /*!
   * \brief The associated EventManager.
   */
  eventgroup::EventManagerInterface* event_manager_{nullptr};

  /*!
   * \brief Used to dispatch event messages to subscribers via TCP and UDP senders or UDP multicast.
   */
  eventgroup::EventMessageDispatcher<TcpSenderPtr, UdpSenderPtr> const event_message_dispatcher_{};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Event message accumulation timeouts.
   */
  EventsMessageAccumulationTimeoutMap const event_message_accumulation_timeouts_;

  /*!
   * \brief Indicates whether the remote service has been offered.
   */
  bool is_service_offered_{false};

  /*!
   * \brief Handle for Reactor software event for cleanup after disconnection detection.
   */
  osabstraction::io::reactor1::CallbackHandle sw_event_handle_cleanup_{};

  /*!
   * \brief Field Cache Hanler.
   */
  ServerCacheInterface* server_field_cache_{nullptr};

  /*!
   * \brief Friend test to access the protected virtual functions
   * - OnConnectionStateChange(TcpSenderPtr sender)
   * - OnConnectionStateChange(UdpSenderPtr sender)
   * - CleanupSubscriptions()
   */

  FRIEND_TEST(UT__RemoteClientCommunication, CleanupSubscriptions);
  FRIEND_TEST(UT__RemoteClientCommunication, UDP_OnConnectionStateChange);
  FRIEND_TEST(UT__RemoteClientCommunication, TCP_OnConnectionStateChange);
  FRIEND_TEST(UT__RemoteClientCommunication, TCP_OnConnectionStateChange_SwEventFail);
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_COMMUNICATION_H_

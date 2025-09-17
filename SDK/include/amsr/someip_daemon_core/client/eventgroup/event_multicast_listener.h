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
/**        \file  event_multicast_listener.h
 *        \brief  Manages listening to a multicast group for multicast event reception
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENT_MULTICAST_LISTENER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENT_MULTICAST_LISTENER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <utility>
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/net/interface.h"
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon_core/client/eventgroup/multicast_eventgroup_listener_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "ara/core/string_view.h"
#include "vac/container/string_literals.h"
#include "vac/language/location.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals.
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief EventMulticastListener.
 * \details Allows listening and stop listening for multicast events
 *
 * \tparam ConnectionManagerType Type of ConnectionManager
 */
template <typename ConnectionManagerType>
class EventMulticastListener final : public eventgroup::MulticastEventgroupListenerInterface {
 public:
  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;

  /*!
   * \brief Constructor of EventMulticastListener.
   *
   * \param[in] service_deployment_id  The service deployment id.
   * \param[in] instance_id            A SOME/IP service instance identifier.
   * \param[in] required_pdus          List of required PDUs, to map PDUs to events at the multicast UDP endpoint.
   * \param[in] local_ip_address       The local IP address to listen for multicast.
   * \param[in] conman                 The connection manager.
   * \context Init
   * \steady  FALSE
   */
  EventMulticastListener(configuration::types::SomeIpServiceInterfaceDeploymentId const& service_deployment_id,
                         someip_protocol::internal::InstanceId const instance_id,
                         configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const& required_pdus,
                         someip_daemon_core::IpAddress const& local_ip_address, ConnectionManager& conman) noexcept
      : eventgroup::MulticastEventgroupListenerInterface(),
        service_deployment_id_{service_deployment_id},
        instance_id_{instance_id},
        required_pdus_{required_pdus},
        conman_{conman},
        ip_address_{local_ip_address} {}

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief   Define default destructor.
   * \steady  FALSE
   */
  ~EventMulticastListener() noexcept override = default;

  EventMulticastListener() = delete;
  EventMulticastListener(EventMulticastListener const&) = delete;
  EventMulticastListener(EventMulticastListener&&) = delete;
  EventMulticastListener& operator=(EventMulticastListener const&) & = delete;
  EventMulticastListener& operator=(EventMulticastListener&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Starts listening for multicast UDP events.
   *
   * \details Will not start listening to given address if already listening to multicast UDP events.
   *
   * \param[in] multicast_address  A multicast IP address to which events of the given eventgroup will be sent.
   * \param[in] multicast_port     A multicast UDP port number.
   * \param[in] sender_address     The unicast IP address of the sender of the event.
   * \param[in] sender_port        The unicast UDP port of the sender of the event.
   *
   * \context Network
   *
   * \return "true" if listening to multicast UDP events and "false" otherwise.
   * \steady FALSE
   *
   * \internal
   * - If not already listening to multicast UDP events,
   *   - Create the given IP address.
   *   - Create a UDP endpoint with with the new address.
   *   - Get a UDP multicast receiver.
   *   - Start listening to multicast events on the given address.
   *   - If any error occurred during the setup, stop the setup and log the error.
   * \endinternal
   */
  bool StartListenForMulticastEventgroup(someip_daemon_core::IpAddress const& multicast_address,
                                         someip_protocol::internal::Port const multicast_port,
                                         someip_daemon_core::IpAddress const& sender_address,
                                         someip_protocol::internal::Port const sender_port) noexcept override {
    logger_.LogVerbose(
        [&multicast_address, &multicast_port, &sender_address, &sender_port](ara::log::LogStream& s) {
          s << "Multicast address <";
          s << multicast_address.ToString();
          s << ", ";
          s << multicast_port;
          s << ">. Sender address <";
          s << sender_address.ToString();
          s << ", ";
          s << sender_port;
          s << ">.";
        },
        static_cast<char const*>(__func__), __LINE__);

    if (!listening_to_multicast_) {
      ara::core::Optional<amsr::net::ip::Udp::Endpoint> sender_endpoint{ara::core::nullopt};
      static_cast<void>(
          amsr::net::ip::Udp::Endpoint::MakeEndpoint(ara::core::StringView{sender_address.ToString()},
                                                     amsr::net::ip::Port{sender_port})
              .AndThen([&sender_endpoint, &multicast_address](amsr::net::ip::Udp::Endpoint&& endpoint) noexcept {
                sender_endpoint.emplace(std::move(endpoint));
                return ara::core::Result<someip_daemon_core::IpAddress>(multicast_address);
              })
              .AndThen([&multicast_address, &multicast_port,
                        this](someip_daemon_core::IpAddress const&& multicast_address_net) {
                amsr::net::ip::Udp::Endpoint const udp_endpoint{amsr::net::ip::Udp::Endpoint::MakeEndpoint(
                    multicast_address_net, amsr::net::ip::Port{multicast_port}, interface_)};
                return conman_.GetUdpMulticastReceiver(ip_address_, multicast_address, multicast_port, udp_endpoint);
              })
              .AndThen([&multicast_address, &multicast_port, &sender_endpoint,
                        this](typename ConnectionManager::UdpReceiverPtr&& udp_reveiver_ptr) {
                udp_receiver_ = std::move(udp_reveiver_ptr);

                // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
                // VECTOR NL VectorC++-V5.0.1: MD_SomeIpDaemon_V5.0.1_falsepositive
                udp_receiver_->RegisterRequiredServiceInstance(
                    {{service_deployment_id_.service_interface_id,
                      service_deployment_id_.service_interface_version.major_version},
                     instance_id_},
                    sender_endpoint.value());

                // Build the PDU reception mappings
                udp_receiver_->BuildPduReceptionMappings(service_deployment_id_.service_interface_id, required_pdus_);
                listening_to_multicast_ = true;
                logger_.LogDebug(
                    [&multicast_address, &multicast_port](ara::log::LogStream& s) {
                      s << "Started listening to multicast events on address <";
                      s << multicast_address.ToString();
                      s << ", ";
                      s << multicast_port;
                      s << ">";
                    },
                    static_cast<char const*>(__func__), __LINE__);
                return ara::core::Result<void>();
              })
              .InspectError([this, &multicast_address, &multicast_port](ara::core::ErrorCode const& error_code) {
                ara::core::StringView const error_msg{error_code.Message()};
                logger_.LogError(
                    [&multicast_address, &multicast_port, &error_msg](ara::log::LogStream& s) {
                      s << "Cannot listen to multicast events on address <";
                      s << multicast_address.ToString();
                      s << ", ";
                      s << multicast_port;
                      s << ">. Error: ";
                      s << error_msg;
                    },
                    static_cast<char const*>(__func__), __LINE__);
              }));
    }

    return listening_to_multicast_;
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Stop listening for multicast UDP events.
   *
   * \context ANY
   * \steady  FALSE
   */
  void StopListenForMulticastEventgroup() noexcept override {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    if (listening_to_multicast_) {
      udp_receiver_.reset(nullptr);
      listening_to_multicast_ = false;
      logger_.LogDebug([](ara::log::LogStream& s) { s << "Stopped listening for multicast events."; },
                       static_cast<char const*>(__func__), __LINE__);
    }
  }

 private:
  /*!
   * \brief Make network interface instance from the given IP address.
   * \details IP address must be assigned to exactly one interface, otherwise it is handled as a violation.
   *
   * \param[in] address The IP address.
   *
   * \return The network interface.
   * \steady FALSE
   *
   * \context Init
   */
  amsr::net::Interface MakeInterface(someip_daemon_core::IpAddress const& address) const {
    return amsr::net::Interface::MakeInterface(address)
        .InspectError([this, &address](ara::core::ErrorCode error_code) {
          logger_.LogFatalAndAbort(
              [&error_code, &address](ara::core::String& abort_msg) noexcept {
                abort_msg.append("Missing network interface for the configured IP address (address: ");
                abort_msg.append(address.ToString());
                abort_msg.append("( error message: ");
                abort_msg.append(error_code.Message());
                abort_msg.append(", user message: ");
                abort_msg.append(error_code.UserMessage());
                abort_msg.append(", support data: ");
                abort_msg.append(std::to_string(error_code.SupportData()));
                abort_msg.append(").");
              },
              AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
        })
        .Value();
  }

  /*!
   * \brief The service deployment id
   */
  configuration::types::SomeIpServiceInterfaceDeploymentId const service_deployment_id_;

  /*!
   * \brief The service instance id
   */
  someip_protocol::internal::InstanceId const instance_id_;

  /*!
   * \brief A list of required pdus. Needed to map PDU ids to Event ids in the UDP multicast endpoint.
   */
  configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const required_pdus_;

  /*!
   * \brief A connection manager.
   */
  ConnectionManager& conman_;

  /*!
   * \brief The IP address for this client.
   */
  someip_daemon_core::IpAddress const ip_address_;

  /*!
   * \brief A UDP receiver.
   */
  typename ConnectionManager::UdpReceiverPtr udp_receiver_{nullptr};

  /*!
   * \brief Defines if currently listening to eventgroup multicast address or not.
   */
  bool listening_to_multicast_{false};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix("EventMulticastListener"_sv, service_deployment_id_,
                                                      instance_id_)};

  /*!
   * \brief The Network interface for this client.
   * \details Must be constructed last, as it uses the logger in case of error
   */
  amsr::net::Interface const interface_{MakeInterface(ip_address_)};
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENT_MULTICAST_LISTENER_H_

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
/**        \file  remote_server_connection_manager.h
 *        \brief  Manages the TCP/UDP connections between local client and remote service.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/net/interface.h"
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon_core/client/remote_server_connection_manager_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/port.h"
#include "amsr/someip_daemon_core/configuration/types/required_network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_event_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_method_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/configuration/types/transport_protocol.h"
#include "amsr/someip_daemon_core/configuration/util/configuration_types_utilities.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state_change_handler.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/packet_handler/validators/local_incoming_packet_validator_error.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals.
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief RemoteServer.
 * \details When adding or removing required definitions in TemplateConfiguration,
 * remember to also update documentation of TemplateConfiguration in class template
 * RemoteServerManager!
 *
 * \tparam ConnectionManagerType Type of ConnectionManager
 */
template <typename ConnectionManagerType>
class RemoteServerConnectionManager final : public RemoteServerConnectionManagerInterface,
                                            public connection_manager::ConnectionStateChangeHandler {
 public:
  /*!
   * \brief The type of a class responsible for data communication.
   */
  using ConnectionManager = ConnectionManagerType;

  /*!
   * \brief Represents a SOME/IP message.
   */
  using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief Container of required eventgroups for this required service instance
   */
  using RequiredEventgroupsContainer =
      ara::core::Map<::amsr::someip_protocol::internal::EventgroupId,
                     ara::core::Optional<configuration::types::SomeipSdClientEventGroupTimingConfig>>;

  /*!
   * \brief Construct a RemoteServerConnectionManager and connect to the peer.
   *
   * \param[in] service_deployment_id   The service deployment id
   * \param[in] instance_id             The service instance id
   * \param[in] network_config          The network configuration.
   * \param[in] conman                  A connection manager.
   * \param[in] remote_server           A remote server.
   * \param[in] remote_server_address   The remote server address to connect to.
   * \param[in] tcp_connection_required Defines if a remote TCP connection is required or not.
   * \param[in] udp_connection_required Defines if a remote UDP connection is required or not.
   * \param[in] sec_com_config          The SecCom configuration.
   *
   * \return The RemoteServerConnectionManager.
   *
   * \context Network | App
   * \steady  FALSE
   * \internal
   * - Create the result object by calling the constructor
   * - Initiate the asynchronous connection process.
   * - On connection success, return the result object, otherwise return null
   * \endinternal
   */
  static amsr::UniquePtr<RemoteServerConnectionManager<ConnectionManagerType>> Create(
      configuration::types::SomeIpServiceInterfaceDeploymentId const& service_deployment_id,
      someip_protocol::internal::InstanceId const instance_id,
      configuration::types::RequiredNetworkEndpoint const& network_config, ConnectionManager& conman,
      configuration::types::ServiceAddress remote_server_address,
      RemoteServerConnectionStateChangeHandler* remote_server, bool const tcp_connection_required,
      bool const udp_connection_required,
      configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config) noexcept {
    // Create a RemoteServerConnectionManager
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
    amsr::UniquePtr<RemoteServerConnectionManager<ConnectionManagerType>> result{
        memory::MemoryUtilities::CreateUniquePtr<RemoteServerConnectionManager<ConnectionManagerType>>(
            service_deployment_id, instance_id, network_config, conman, *remote_server, tcp_connection_required,
            udp_connection_required, sec_com_config)};

    // Initiate connection establishment
    bool const success{result->Connect(remote_server_address)};

    // If connection establishment request did not succeed, delete the created object and return nullptr
    if (!success) {
      result = nullptr;
    }

    return std::move(result);
  }

  /*!
   * \brief Constructor of RemoteServerConnectionManager.
   *
   * \param[in] service_deployment_id   The service deployment id
   * \param[in] instance_id             The service instance id
   * \param[in] network_config          The network configuration.
   * \param[in] conman                  A connection manager.
   * \param[in] remote_server           A remote server.
   * \param[in] tcp_connection_required Defines if a remote TCP connection is required or not.
   * \param[in] udp_connection_required Defines if a remote UDP connection is required or not.
   * \param[in] sec_com_config          The SecCom configuration.
   *
   * \context ANY
   * \steady  FALSE
   */
  RemoteServerConnectionManager(configuration::types::SomeIpServiceInterfaceDeploymentId const& service_deployment_id,
                                someip_protocol::internal::InstanceId const instance_id,
                                configuration::types::RequiredNetworkEndpoint const& network_config,
                                ConnectionManager& conman, RemoteServerConnectionStateChangeHandler& remote_server,
                                bool const tcp_connection_required, bool const udp_connection_required,
                                configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config)
      : RemoteServerConnectionManagerInterface(),
        connection_manager::ConnectionStateChangeHandler(),
        service_deployment_id_{service_deployment_id},
        instance_id_{instance_id},
        conman_{conman},
        remote_server_{remote_server},
        network_config_{network_config},
        tcp_connection_required_{tcp_connection_required},
        udp_connection_required_{udp_connection_required},
        sec_com_config_{sec_com_config} {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief Define default destructor.
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Destructor of RemoteServerConnectionManager.
   *
   * \context Shutdown | Network | App
   * \steady  FALSE
   *
   * \internal
   * - Disconnect
   * \endinternal
   */
  ~RemoteServerConnectionManager() noexcept override { Disconnect(); }

  RemoteServerConnectionManager() = delete;
  RemoteServerConnectionManager(RemoteServerConnectionManager const&) = delete;
  RemoteServerConnectionManager(RemoteServerConnectionManager&&) = delete;
  RemoteServerConnectionManager& operator=(RemoteServerConnectionManager const&) & = delete;
  RemoteServerConnectionManager& operator=(RemoteServerConnectionManager&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Return the local TCP network endpoint if it exists.
   * \details This API must only be called if a local network endpoint has been assigned to this object.
   *          This is ensured by the usage of the factory API "Create" which won't return a valid pointer to
   *          this object unless a local network endpoint has been assigned to this object when starting the connection
   *          establishment.
   *
   * \return The local TCP network endpoint if it exists
   *
   * \context ANY
   * \steady  FALSE
   */
  configuration::types::NetworkEndpoint const& GetLocalTcpNetworkEndpoint() const noexcept override {
    assert(network_config_.tcp_port.has_value());

    return local_network_endpoint_.value();
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a TCP connection state is changed.
   *
   * \context Network
   * \steady  FALSE
   *
   * \internal
   * - If the TCP sender is connected
   *  - Set the TCP connection state to "connected"
   * - else (the TCP sender is not connected)
   *  - Set the TCP connection state to "disconnected"
   * - if the TCP connection state is set to connected
   *  - If no (UDP connection is required) or the (UDP connection state is set to connected)
   *   - Notify the remote server of successful connection establishment
   * - else (the TCP connection state is not set to connected)
   *  - Notify the remote server of connection closure
   * \endinternal
   */
  void OnConnectionStateChange(TcpSenderPtr) noexcept override {
    // This function is called from within TcpSender, which passes itself
    // as an argument, and this is the same instance as tcp_sender_
    assert(tcp_sender_ != nullptr);
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    if (tcp_sender_->IsConnected()) {
      tcp_connection_state_ = connection_manager::ConnectionState::kConnected;
    } else {
      tcp_connection_state_ = connection_manager::ConnectionState::kDisconnected;
    }

    if (tcp_connection_state_ == connection_manager::ConnectionState::kConnected) {
      if ((!udp_connection_required_) || (udp_connection_state_ == connection_manager::ConnectionState::kConnected)) {
        // If TCP is now connected and UDP is not required, then we are connected
        remote_server_.HandleConnectionEstablished();
      }
    } else {
      // If TCP is disconnected, we are now disconnected, no matter what UDP is
      remote_server_.HandleConnectionClosed();
    }
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a UDP connection state is changed.
   *
   * \context Network
   * \steady  FALSE
   *
   * \internal
   * - If the UDP sender is connected
   *  - Set the UDP connection state to "connected"
   * - else (the UDP sender is not connected)
   *  - Set the UDP connection state to "disconnected"
   * - if the UDP connection state is set to connected
   *  - If no (TCP connection is required) or the (TCP connection state is set to connected)
   *   - Notify the remote server of successful connection establishment
   * - else (the UDP connection state is not set to connected)
   *  - Notify the remote server of connection closure
   * \endinternal
   */
  void OnConnectionStateChange(UdpSenderPtr) noexcept override {
    // This function is called from within UdpSender, which passes itself
    // as an argument, and this is the same instance as udp_sender_
    assert(udp_sender_ != nullptr);
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    if (udp_sender_->IsConnected()) {  // Connected.
      udp_connection_state_ = connection_manager::ConnectionState::kConnected;
    } else {  // Disconnected.
      udp_connection_state_ = connection_manager::ConnectionState::kDisconnected;
    }

    if (udp_connection_state_ == connection_manager::ConnectionState::kConnected) {
      if ((!tcp_connection_required_) || (tcp_connection_state_ == connection_manager::ConnectionState::kConnected)) {
        // If UDP is now connected and TCP is not required, then we are connected
        remote_server_.HandleConnectionEstablished();
      }
    } else {
      // If UDP is disconnected, we are now disconnected, no matter what TCP is
      remote_server_.HandleConnectionClosed();
    }
  }

  /*!
   * \brief Sends a SOME/IP message to remote peer.
   *
   * \param[in] protocol The SOME/IP protocol (TCP/UDP) that shall be used to transmit the method request.
   * \param[in] packet   SOME/IP message containing Method Request header + payload.
   * \param[in] message_accumulation_timeout UDP message accumulation timeout
   *
   * \return void if the message was sent successfully, and error otherwise.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable
   * Runtime check failed: connection is not established.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionTransmissionFailed
   * Runtime check failed: connection transmission failed.
   *
   * \context ANY
   * \steady  TRUE
   *
   * \internal
   * - If the message is a TCP message,
   *   - If a TCP sender has been set up,
   *     - Forward the message to a TCP sender.
   *   - Else, return RuntimeError: connection not available.
   * - If the message is a UDP message,
   *   - If a UDP sender has been set up,
   *     - Forward the message to a UDP sender and return void if succeed.
   *   - Else, return RuntimeError: connection not available.
   * - Else, return RuntimeError: connection transmission failed.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ara::core::Result<void> SendRequest(configuration::types::TransportProtocol const protocol,
                                      Packet const packet) noexcept override {
    ara::core::Result<void> validation_result{
        packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionTransmissionFailed};
    // Check protocol
    if (protocol == configuration::types::TransportProtocol::kTCP) {
      if (tcp_sender_ != nullptr) {
        bool const send_result{tcp_sender_->ForwardFromLocal(packet)};
        if (send_result) {
          validation_result.EmplaceValue();
        }
      } else {
        // Emplace error
        validation_result.EmplaceError(
            packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable);
      }
    } else if (udp_sender_ != nullptr) {
      bool const send_result{udp_sender_->ForwardFromLocal(packet)};
      if (send_result) {
        validation_result.EmplaceValue();
      }
    } else {
      // Emplace error
      validation_result.EmplaceError(
          packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable);
    }
    return validation_result;
  }

  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Get the connection state.
   *
   * \context ANY
   * \steady  TRUE
   *
   * \return The state of the connection to the server.
   *
   * \internal
   * - If UDP is required and TCP is not
   *   - return UDP connection state
   * - else if TCP is required and UDP is not
   *   - return TCP connection state
   * - else
   *   - assert both TCP and UDP are required
   *   - If both are connected
   *     - return connected
   *   - else if both are disconnected
   *     - return disconnected
   *   - else
   *     - return connecting
   * \endinternal
   *
   */
  connection_manager::ConnectionState GetConnectionState() const noexcept override {
    connection_manager::ConnectionState result{connection_manager::ConnectionState::kDisconnected};

    if (udp_connection_required_ && (!tcp_connection_required_)) {
      result = udp_connection_state_;
    } else if ((!udp_connection_required_) && tcp_connection_required_) {
      result = tcp_connection_state_;
    } else {
      assert(udp_connection_required_ && tcp_connection_required_);
      if ((udp_connection_state_ == connection_manager::ConnectionState::kConnected) &&
          (tcp_connection_state_ == connection_manager::ConnectionState::kConnected)) {
        result = connection_manager::ConnectionState::kConnected;
      } else if ((udp_connection_state_ == connection_manager::ConnectionState::kDisconnected) ||
                 (tcp_connection_state_ == connection_manager::ConnectionState::kDisconnected)) {
        result = connection_manager::ConnectionState::kDisconnected;
      } else {
        // At least one of them must be in kConnecting and the other one in kConnected
        result = connection_manager::ConnectionState::kConnecting;
      }
    }

    return result;
  }

 private:
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Close connections.
   *
   * \context ANY
   * \steady  FALSE
   *
   * * \internal
   * - Disconnect from the TCP connection.
   * - Disconnect from the UDP connection.
   * \endinternal
   */
  void Disconnect() noexcept {
    logger_.LogDebug([](ara::log::LogStream& s) { s << "Disconnecting"; }, static_cast<char const*>(__func__),
                     __LINE__);

    if (tcp_sender_ != nullptr) {
      // The next line is what triggers disconnection (destructor if TcpSender), only
      // if no more TCP sender require the connection will it be closed.
      tcp_sender_.reset(nullptr);
      tcp_connection_state_ = connection_manager::ConnectionState::kDisconnected;
    }
    if (network_config_.udp_port.has_value()) {
      if (udp_sender_ != nullptr) {  // indicates that we have registered an instance ID before
        static_cast<void>(
            amsr::net::ip::Udp::Endpoint::MakeEndpoint(udp_sender_->GetRemoteAddress(), udp_sender_->GetRemotePort())
                .AndThen([this](amsr::net::ip::Udp::Endpoint const& remote_endpoint) noexcept {
                  someip_protocol::internal::IpAddress const ip_address_str{network_config_.address.ToString()};

                  // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
                  // VECTOR NC VectorC++-V5.0.1: MD_SomeIpDaemon_V5.0.1_falsepositive
                  conman_.GetUdpEndpoint(ip_address_str, network_config_.udp_port.value().Value())
                      ->UnregisterRequiredServiceInstance(
                          {{service_deployment_id_.service_interface_id,
                            service_deployment_id_.service_interface_version.major_version},
                           instance_id_},
                          remote_endpoint);
                  return ara::core::Result<void>{};
                }));
      }
      udp_sender_.reset(nullptr);
      udp_connection_state_ = connection_manager::ConnectionState::kDisconnected;
    }
  }

  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Connects to a remote provided service instance identified by its IP address and port numbers.
   *
   * \pre udp_address and udp_port are valid.
   *
   * \param[in] udp_address An IP address of a provided service instance.
   * \param[in] udp_port A UDP port number.
   * \return A void result on success, an error otherwise
   *
   * \context ANY
   * \steady  FALSE
   *
   * \trace SPEC-4980085
   * \trace SPEC-4980120
   *
   * \internal
   * - If the connection is not already connected or connecting.
   *   - Try to connect to the given address and port.
   *   - If connection fails, log an error.
   * \endinternal
   */
  ara::core::Result<void> ConnectUdp(someip_daemon_core::IpAddress const& udp_address,
                                     someip_protocol::internal::Port const udp_port) noexcept {
    // Start assuming connection can't be established
    ara::core::Result<void> result{ara::core::ErrorCode{osabstraction::OsabErrc::kDisconnected}};
    someip_protocol::internal::IpAddress const udp_address_str{udp_address.ToString()};
    logger_.LogVerbose(
        [&udp_address_str, &udp_port](ara::log::LogStream& s) {
          s << "<";
          s << udp_address_str;
          s << ", ";
          s << udp_port;
          s << ">";
        },
        static_cast<char const*>(__func__), __LINE__);

    // ConnectUdp must not be called if no UDP configuration has been passed to the RemoteServerConnectionManager
    assert(network_config_.udp_port.has_value());

    logger_.LogVerbose(
        [&udp_address_str, &udp_port](ara::log::LogStream& s) {
          s << "Connecting to <";
          s << udp_address_str;
          s << ", ";
          s << udp_port;
          s << ">";
        },
        static_cast<char const*>(__func__), __LINE__);

    someip_protocol::internal::IpAddress const ip_address_str{network_config_.address.ToString()};

    ara::core::Result<void> const connection_result{
        conman_
            .GetUdpSender(ip_address_str, network_config_.udp_port.value().Value(), udp_address_str, udp_port,
                          &sec_com_config_)
            .AndThen([this, &ip_address_str, &udp_address,
                      &udp_port](typename ConnectionManager::UdpSenderPtr&& udp_sender_ptr) {
              udp_sender_ = std::move(udp_sender_ptr);
              udp_sender_->SetConnectionStateChangeHandler(this);

              if (udp_sender_->IsConnected()) {
                udp_connection_state_ = connection_manager::ConnectionState::kConnected;
              } else {
                udp_connection_state_ = connection_manager::ConnectionState::kConnecting;
              }
              // MakeEndpoint cannot fail here since it is checked during GetUdpSender that udp_address is valid.
              amsr::net::ip::Udp::Endpoint const remote_endpoint{
                  amsr::net::ip::Udp::Endpoint::MakeEndpoint(udp_address, {udp_port}).Value()};
              assert(!network_config_.udp_port.value().IsDynamic());
              configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id{
                  {service_deployment_id_.service_interface_id,
                   service_deployment_id_.service_interface_version.major_version},
                  instance_id_};
              // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_falsepositive
              // VECTOR NC VectorC++-V5.0.1: MD_SomeIpDaemon_V5.0.1_falsepositive
              conman_.GetUdpEndpoint(ip_address_str, network_config_.udp_port.value().Value())
                  ->RegisterRequiredServiceInstance(service_instance_id, remote_endpoint);
              return ara::core::Result<void>{};
            })
            .InspectError([this, &udp_address_str, &udp_port](ara::core::ErrorCode) {
              logger_.LogError(
                  [&udp_address_str, &udp_port](ara::log::LogStream& s) {
                    s << "Failed to connect to remote UDP address <";
                    s << udp_address_str;
                    s << ", ";
                    s << udp_port;
                    s << ">.";
                  },
                  static_cast<char const*>(__func__), __LINE__);
            })};
    if (connection_result.HasValue()) {
      // Connection properly established. TCP Sender can be used
      result.EmplaceValue();
    } else {
      result.EmplaceError(connection_result.Error());
    }

    return result;
  }

  /*!
   * \brief Connects to a remote provided service instance identified by its IP address and port numbers.
   *
   * \pre tcp_address and tcp_port are valid
   *
   * \param[in] tcp_address An IP address of a provided service instance.
   * \param[in] tcp_port A TCP port number.
   *
   * \return A result with the local TCP port assigned to the connection or an OsAbstraction error
   *         in case connection can't be established
   *
   * \context ANY
   * \steady  FALSE
   *
   * \trace SPEC-4980085
   * \trace SPEC-4980120
   *
   * \internal
   * - If the connection is not already connected or connecting,
   *   - Try to connect to the given address and port.
   *   - If connection fails, log an error.
   *   - Get the TCP port assigned to the connection and return it.
   * - Else, get the TCP port assigned to the connection and return it.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ara::core::Result<configuration::types::NetworkEndpoint> ConnectTcp(
      someip_daemon_core::IpAddress const& tcp_address, someip_protocol::internal::Port const tcp_port) noexcept {
    someip_protocol::internal::IpAddress const tcp_address_str{tcp_address.ToString()};
    logger_.LogVerbose(
        [&tcp_address_str, &tcp_port](ara::log::LogStream& s) {
          s << "<";
          s << tcp_address_str;
          s << ", ";
          s << tcp_port;
          s << ">";
        },
        static_cast<char const*>(__func__), __LINE__);

    // ConnectTcp must not be called if no TCP configuration has been passed to the RemoteServerConnectionManager
    assert(network_config_.tcp_port.has_value());

    // Start assuming connection can't be established
    ara::core::Result<configuration::types::NetworkEndpoint> result{
        ara::core::ErrorCode{osabstraction::OsabErrc::kDisconnected}};

    logger_.LogVerbose(
        [&tcp_address_str, &tcp_port](ara::log::LogStream& s) {
          s << "Connecting to <";
          s << tcp_address_str;
          s << ", ";
          s << tcp_port;
          s << ">";
        },
        static_cast<char const*>(__func__), __LINE__);

    someip_protocol::internal::IpAddress const ip_address_str{network_config_.address.ToString()};
    someip_protocol::internal::Port required_tcp_port{0};
    if (!network_config_.tcp_port.value().IsDynamic()) {
      required_tcp_port = network_config_.tcp_port.value().Value();
    }

    ara::core::Result<void> const connection_result{
        conman_.GetActiveTcpSender(ip_address_str, required_tcp_port, tcp_address_str, tcp_port, sec_com_config_)
            .AndThen([this, &ip_address_str, &tcp_address_str,
                      &tcp_port](typename ConnectionManager::TcpSenderPtr&& tcp_sender_ptr) {
              tcp_sender_ = std::move(tcp_sender_ptr);
              tcp_sender_->SetConnectionStateChangeHandler(this);
              tcp_sender_->RegisterRequiredServiceInstance(
                  {{service_deployment_id_.service_interface_id,
                    service_deployment_id_.service_interface_version.major_version},
                   instance_id_});

              if (tcp_sender_->IsConnected()) {
                tcp_connection_state_ = connection_manager::ConnectionState::kConnected;

                logger_.LogDebug(
                    [this, &ip_address_str, &tcp_address_str, &tcp_port](ara::log::LogStream& s) {
                      someip_protocol::internal::Port const tcp_local_port{tcp_sender_->GetLocalPort()};
                      s << "Connection from <";
                      s << ip_address_str;
                      s << ", ";
                      s << tcp_local_port;
                      s << "> to <";
                      s << tcp_address_str;
                      s << ", ";
                      s << tcp_port;
                      s << ">  ESTABLISHED";
                    },
                    static_cast<char const*>(__func__), __LINE__);
              } else {
                tcp_connection_state_ = connection_manager::ConnectionState::kConnecting;
                logger_.LogDebug(
                    [this, &ip_address_str, &tcp_address_str, &tcp_port](ara::log::LogStream& s) {
                      someip_protocol::internal::Port const tcp_local_port{tcp_sender_->GetLocalPort()};
                      s << "Connection from <";
                      s << ip_address_str;
                      s << ", ";
                      s << tcp_local_port;
                      s << "> to <";
                      s << tcp_address_str;
                      s << ", ";
                      s << tcp_port;
                      s << ">  PENDING";
                    },
                    static_cast<char const*>(__func__), __LINE__);
              }
              return ara::core::Result<void>();
            })
            .InspectError([this, &tcp_address_str, &tcp_port](ara::core::ErrorCode error_code) {
              ara::core::StringView const error_msg{error_code.Message()};
              logger_.LogError(
                  [&tcp_address_str, &tcp_port, &error_msg](ara::log::LogStream& s) {
                    s << "Failed to connect to remote TCP address <";
                    s << tcp_address_str;
                    s << ", ";
                    s << tcp_port;
                    s << ">. ";
                    s << error_msg;
                  },
                  static_cast<char const*>(__func__), __LINE__);
            })};

    if (connection_result.HasValue()) {
      // Connection properly established. TCP Sender can be used
      result.EmplaceValue(configuration::types::NetworkEndpoint{
          network_config_.address, network_config_.network, {}, {tcp_sender_->GetLocalPort()}});
    } else {
      result.EmplaceError(connection_result.Error());
    }

    return result;
  }

  /*!
   * \brief Set up required connections.
   *
   * \param[in] remote_server_address  The remote server address.
   *
   * \return True if connection establishment has successfully been initiated. False otherwise.
   *
   * \context Network | App
   * \steady  FALSE
   *
   * \internal
   * - If UDP is required, set up the UDP connection.
   * - If TCP is required, set up the TCP connection.
   *   - Store the assigned local network endpoint in case of successful TCP connection establishment attempt.
   * - Return true when connection establishment has successfully been initiated
   * \endinternal
   */
  bool Connect(configuration::types::ServiceAddress& remote_server_address) noexcept {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    // Assume by default that connection establishment failed
    bool success{false};
    bool connect_tcp{tcp_connection_required_};

    // Establish UDP connection if requested
    if (udp_connection_required_) {
      // Udp connection establishment is always expected to succeed
      ara::core::Result<void> const udp_connection_result{ConnectUdp(
          remote_server_address.udp_endpoint.value().address, remote_server_address.udp_endpoint.value().port.Value())};
      if (udp_connection_result.HasValue()) {
        // Connection is successful, save the assigned local endpoint address and port
        success = !connect_tcp;
      } else {
        connect_tcp = false;
      }
    }

    // Establish TCP connection if requested
    if (connect_tcp) {
      ara::core::Result<configuration::types::NetworkEndpoint> const tcp_connection_result{ConnectTcp(
          remote_server_address.tcp_endpoint.value().address, remote_server_address.tcp_endpoint.value().port.Value())};

      if (tcp_connection_result.HasValue()) {
        // Connection is successful, save the assigned local endpoint address and port
        local_network_endpoint_.emplace(tcp_connection_result.Value());
        success = true;
      }
    }

    return success;
  }

  /*!
   * \brief The service deployment configuration
   * \details Used in this class to access the deployment id and the list of configured
   *          methods
   */
  configuration::types::SomeIpServiceInterfaceDeploymentId const service_deployment_id_;

  /*!
   * \brief The service instance id
   */
  someip_protocol::internal::InstanceId const instance_id_;

  /*!
   * \brief A connection manager.
   */
  ConnectionManager& conman_;

  /*!
   * \brief A reference to remote server
   */
  client::RemoteServerConnectionStateChangeHandler& remote_server_;

  /*!
   * \brief The local network endpoint of the TCP connection.
   */
  ara::core::Optional<configuration::types::NetworkEndpoint> local_network_endpoint_{ara::core::nullopt};

  /*!
   * \brief State of the connection to the server.
   */
  connection_manager::ConnectionState tcp_connection_state_{connection_manager::ConnectionState::kDisconnected};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix("RemoteServerConnectionManager"_sv, service_deployment_id_,
                                                      instance_id_)};

  // Constants

  /*!
   * \brief The network configuration for this remote server connection manager
   */
  configuration::types::RequiredNetworkEndpoint const network_config_;

  /*!
   * \brief A TCP sender.
   */
  typename ConnectionManager::TcpSenderPtr tcp_sender_{nullptr};

  /*!
   * \brief A UDP sender.
   */
  typename ConnectionManager::UdpSenderPtr udp_sender_{nullptr};

  /*!
   * \brief Connection (to server) state.
   */
  connection_manager::ConnectionState udp_connection_state_{connection_manager::ConnectionState::kDisconnected};

  /*!
   * \brief Defines if a remote TCP connection is required or not.
   * \details If for this required SOME/IP service instance, any required event or method is configured to use TCP
   * protocol, then this variable shall be true, otherwise set to false.
   */
  bool const tcp_connection_required_;

  /*!
   * \brief Defines if a remote UDP connection is required or not.
   * \details If for this required SOME/IP service instance, any required event or method is configured to use UDP
   * protocol, then this variable shall be true, otherwise set to false.
   */
  bool const udp_connection_required_;

  /*!
   * \brief The SecCom configuration.
   */
  configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config_;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_H_

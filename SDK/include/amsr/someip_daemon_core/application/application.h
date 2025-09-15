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
/**        \file  application.h
 *        \brief  Represent a connected instance of a local application.
 *
 *      \details  Handles the communication for a local application.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include "ara/core/string.h"

#include "amsr/iostream/io_format.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_daemon_core/application/application_client_handler.h"
#include "amsr/someip_daemon_core/application/application_server_handler.h"
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/application/command_controller.h"
#include "amsr/someip_daemon_core/application/receiving_routing_controller.h"
#include "amsr/someip_daemon_core/application/sending_routing_controller.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/packet_handler/local_incoming_packet_validator.h"
#include "amsr/someip_daemon_core/server/local_server_manager.h"
#include "amsr/someip_daemon_core/tracing/communication_tracing_interface.h"
#include "amsr/someip_daemon_core/tracing/sd_tracing_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/log/logging.h"
#include "osabstraction/io/io_buffer.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/language/unique_function.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Handles the connection of a local application.
 *
 * \tparam ApplicationConnectionType Type used to manage connection between SOME/IP Daemon and the application.
 */
template <typename ApplicationConnectionType>
class Application {
 public:
  /*!
   * \brief A type alias for "ApplicationConnection".
   */
  using ApplicationConnection = ApplicationConnectionType;

  /*!
   * \brief A type alias for the target LocalIncomingPacketValidator.
   */
  using PacketValidatorType = packet_handler::LocalIncomingPacketValidator;

  /*!
   * \brief A type alias for "CommandController".
   */
  using CommandControllerType = CommandController<ApplicationConnection>;

  /*!
   * \brief A type alias for "ReceivingRoutingController".
   */
  using ReceivingRoutingControllerType = ReceivingRoutingController<ApplicationConnection>;

  /*!
   * \brief A type alias for "SendingRoutingController".
   */
  using SendingRoutingControllerType = SendingRoutingController<ApplicationConnection>;

  /*!
   * \brief A type alias for "ApplicationServerHandler".
   */
  using ApplicationServerHandlerType = ApplicationServerHandler<ApplicationConnectionType>;

  /*!
   * \brief A type alias for "ApplicationClientHandler".
   */
  using ApplicationClientHandlerType = ApplicationClientHandler<ApplicationConnectionType>;

  /*!
   * \brief Type for the callback function that is called when application disconnects.
   */
  using DisconnectionFunction = vac::language::UniqueFunction<void(Application*)>;

  /*!
   * \brief Type alias for the service instance communication type.
   */
  using ServiceInstanceCommunicationType = configuration::ConfigurationTypesAndDefs::ServiceInstanceCommunicationType;

  /*!
   * \brief Constructor.
   *
   * \param[in] packet_validator                   A validator for incoming packets.
   * \param[in] connection                         Represents a new incoming IPC connection to the application.
   * \param[in] identity_access_manager            A reference to the identity access manager.
   * \param[in] application_id                     A unique application ID.
   * \param[in] config                             Configuration.
   * \param[in] required_service_instance_manager  A reference to the RequiredServiceInstanceManager.
   * \param[in] local_server_manager               A reference to the Local Server Manager.
   * \param[in] communication_tracing              A reference to the communication tracing implementation.
   * \param[in] sd_tracing                         A reference to the SD tracing implementation.
   *
   * \pre -
   * \context App
   * \steady  FALSE
   */
  Application(PacketValidatorType const& packet_validator, amsr::UniquePtr<ApplicationConnection> connection,
              iam::IamInterface& identity_access_manager, amsr::ipc::Credentials const application_id,
              configuration::Configuration const& config,
              client::RequiredServiceInstanceManager& required_service_instance_manager,
              server::LocalServerManager& local_server_manager,
              tracing::CommunicationTracingInterface& communication_tracing,
              tracing::SdTracingInterface& sd_tracing) noexcept
      : connection_{std::move(connection)},
        application_identifier_{application_id},
        sending_routing_controller_{
            someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<SendingRoutingControllerType>(
                *connection_, communication_tracing, sd_tracing)},
        receiving_routing_controller_{application_id,
                                      connection_->GetPeerProcessId(),
                                      sending_routing_controller_,
                                      packet_validator,
                                      local_servers_,
                                      local_clients_,
                                      communication_tracing},
        application_server_handler_{identity_access_manager,     application_id,       config,
                                    sending_routing_controller_, local_server_manager, local_servers_},
        application_client_handler_{identity_access_manager, application_id, sending_routing_controller_,
                                    required_service_instance_manager, local_clients_} {}

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief Destructor.
   *
   * \pre -
   * \context Shutdown, Timer
   * \steady  FALSE
   */
  virtual ~Application() noexcept = default;

  Application(Application const&) = delete;
  Application(Application const&&) = delete;
  auto operator=(Application const&) -> Application& = delete;
  auto operator=(Application const&&) -> Application& = delete;

  /*!
   * \brief Register a function to call upon disconnection and start the data reception from the connection.
   *
   * \param[in] disconnection_function The disconnect function to register
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Start receiving the data using the IPC connection.
   * \endinternal
   */
  void StartReceive(DisconnectionFunction&& disconnection_function) {
    disconnection_function_ = std::move(disconnection_function);
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_lambda_parameter_false_positive
    connection_->StartReceive(
        [this]() { this->ReleaseApplication(); },
        [this](amsr::someipd_app_protocol::internal::MessageType const& message_type,
               SpecificHeaderView const& specific_header_view,
               vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer) {
          this->OnMessage(message_type, specific_header_view, std::move(memory_buffer));
        });
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief Return the application's connection state.
   *
   * \pre -
   * \return Application's connection.
   * \context   Timer
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Get the connection state of the IPC connection and return it.
   * \endinternal
   */
  ConnectionState GetConnectionState() const noexcept { return connection_->GetConnectionState(); }

  /*!
   * \brief Release the application's resources when the connection terminates.
   *
   * \details The notification comes from "ApplicationConnection" upon connection termination.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Log the function call.
   * - Cleanup client resources.
   * - Stop all service discoveries.
   * - Cleanup server resources.
   * - Notify the application manager about the disconnection.
   * \endinternal
   */
  void ReleaseApplication() noexcept {
    logger_.LogInfo(
        [this](ara::log::LogStream& s) {
          // VECTOR NC AutosarC++17_10-A5.2.4, AutosarC++17_10-M5.2.9: MD_SomeIpDaemon_A5.2.4_reinterpret_cast_logging
          s << "User id 0x" << ara::log::HexFormat(reinterpret_cast<std::uint64_t>(this));
        },
        static_cast<char const*>(__func__), __LINE__);

    // Clean up resources immediately to avoid further calls to this application.
    application_client_handler_.CleanUpClientResources();
    application_client_handler_.StopAllServiceDiscoveries();
    application_server_handler_.CleanUpServerResources();

    // Indicate application termination to the application manager as the last step.
    disconnection_function_(this);
  }

  /*!
   * \brief Handle a message from the connected application.
   *
   * \param[in] message_type The message type of the received message.
   * \param[in] specific_header_view A view into the received specific header buffer.
   * \param[in] memory_buffer A memory buffer containing the payload of the received message
   *                          without a specific header.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Log the message type and size.
   * - If the message is of routing type,
   *   - Forward the message to the routing controller.
   * - Else
   *   - Forward the message to the command controller.
   * \endinternal
   */
  void OnMessage(amsr::someipd_app_protocol::internal::MessageType const& message_type,
                 SpecificHeaderView const& specific_header_view,
                 vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer) {
    logger_.LogVerbose(
        [&message_type, &memory_buffer](ara::log::LogStream& s) {
          // VECTOR NL AutosarC++17_10-M0.3.1: MD_SomeIpDaemon_M0.3.1_FalsePositive
          vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer>::size_type const size{memory_buffer->size()};
          ara::log::LogHex32 const message_type_hex{
              ara::log::HexFormat(static_cast<MessageTypeUnderlyingType>(message_type))};

          s << "Received a new message of type 0x" << message_type_hex << " and of payload length " << size;
        },
        static_cast<char const*>(__func__), __LINE__);

    if (amsr::someipd_app_protocol::internal::IsRoutingSomeIpMessage(message_type)) {
      // PTP-B-SomeIpDaemon-Application_OnRoutingSomeIpMessage
      receiving_routing_controller_.OnRoutingSomeIpMessage(specific_header_view, std::move(memory_buffer));
      // PTP-E-SomeIpDaemon-Application_OnRoutingSomeIpMessage
    } else if (amsr::someipd_app_protocol::internal::IsRoutingPduMessage(message_type)) {
      // PTP-B-SomeIpDaemon-Application_OnRoutingPduMessage
      receiving_routing_controller_.OnRoutingPduMessage(specific_header_view, std::move(memory_buffer));
      // PTP-E-SomeIpDaemon-Application_OnRoutingPduMessage
    } else {  // Control message
      // PTP-B-SomeIpDaemon-Application_OnControlMessage
      command_controller_.OnControlMessage(message_type, specific_header_view, std::move(memory_buffer));
      // PTP-E-SomeIpDaemon-Application_OnControlMessage
    }
  }

 protected:
  /*!
   * \brief Obtain a reference to the connection (for testing purposes)
   *
   * \steady  FALSE
   */
  ApplicationConnection& GetConnection() noexcept { return *connection_; }

 private:
  /*!
   * \brief Underlying type of "MessageType".
   */
  using MessageTypeUnderlyingType = std::underlying_type<amsr::someipd_app_protocol::internal::MessageType>::type;

  /*!
   * \brief Generates a unique logger context identifier for a given application
   *
   * \param[in] application_identifier An application identifier
   * \return    A string representing the logger context identifier for the given application.
   *
   * \pre       -
   * \context   App
   * \steady    FALSE
   */
  static ara::core::String GetLoggerContextId(amsr::ipc::Credentials const application_identifier) noexcept {
    constexpr std::size_t kBufferSize{50};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};
    prefix_builder.Write(ara::core::StringView{"Application: "});
    prefix_builder.Write(ara::core::StringView{"User ID: "});
    // VECTOR NL AutosarC++17_10-M8.4.4: MD_SomeIpDaemon_M8.4.4_FalsePositive
    prefix_builder << ::amsr::stream::SetShowBase;
    prefix_builder.Write(application_identifier, ::amsr::stream::Base::kHex);
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief The connection to an application.
   */
  amsr::UniquePtr<ApplicationConnection> connection_;

  /*!
   * \brief The unique identity of the application.
   */
  amsr::ipc::Credentials application_identifier_;

  /*!
   * \brief The sending routing controller.
   * \details It must be a shared pointer because the packet router accepts only shared pointers.
   */
  std::shared_ptr<SendingRoutingControllerType> sending_routing_controller_;

  /*!
   * \brief The receiving routing controller.
   */
  ReceivingRoutingControllerType receiving_routing_controller_;

  /*!
   * \brief The application server handler.
   */
  ApplicationServerHandlerType application_server_handler_;

  /*!
   * \brief The shared pointer of application client handler.
   */
  ApplicationClientHandlerType application_client_handler_;

  /*!
   * \brief The command controller.
   */
  CommandControllerType command_controller_{application_client_handler_, application_server_handler_, *connection_};

  /*!
   * \brief Registered callback that is triggered when this application disconnects from the daemon.
   */
  DisconnectionFunction disconnection_function_{};

  /*!
   * \brief Map of offered local servers.
   * \details This map manages the lifecycle of the offered local servers locally, this container is used to access
   * these objects in the ReceivingRouterController and to read/write them in the ApplicationServerHandler
   */
  LocalServerMap local_servers_{};

  /*!
   * \brief Map of local clients.
   * \details This map manages the lifecycle of the local clients locally, this container is used to read/write
   * these objects in the ApplicationClientHandler for adding/removing elements, and Handle subscription/unsubscription,
   * and in the ReceivingRoutingController for route method requests.
   */
  LocalClientMap local_clients_{};

  // VECTOR NC AutosarC++17_10-A8.5.2: MD_SomeIpDaemon_AutosarC++17_10-A8.5.2_falsepositive
  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger const logger_{
      someip_daemon_core::logging::kApplicationLoggerContextId,
      someip_daemon_core::logging::kApplicationHandlerLoggerContextDescription,
      GetLoggerContextId(application_identifier_)};
};

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_H_

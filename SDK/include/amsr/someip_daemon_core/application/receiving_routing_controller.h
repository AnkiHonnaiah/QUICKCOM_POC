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
/**        \file  receiving_routing_controller.h
 *        \brief  Application receiving routing controller.
 *
 *      \details  Routing controller for receive path from application.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_RECEIVING_ROUTING_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_RECEIVING_ROUTING_CONTROLLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/application/sending_routing_controller.h"
#include "amsr/someip_daemon_core/configuration/types/local_client_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/someip_daemon_core/packet_handler/local_incoming_packet_validator.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
// Note: The below header is included due to an compiler error in clang format 11. The issue reported here is
// packet_router::RemoteSourceIdentifier structure is defined in the network packet router interface file and the
// forward declaration in packet sink interface is not accepted.
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "amsr/someip_daemon_core/tracing/application_tracing_interface.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/message_builder.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"
#include "someipd_app_protocol/internal/deser_message_headers.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/container/string_literals.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief The StringView operator.
 */
using ::vac::container::literals::operator""_sv;

/*!
 * \brief IPC receiving routing controller.
 *
 * \tparam ApplicationConnectionType Type used to manage connection between SOME/IP daemon and the application.
 */
template <typename ApplicationConnectionType>
class ReceivingRoutingController final {
 public:
  /*!
   * \brief A type alias for "ApplicationConnection".
   */
  using ApplicationConnection = ApplicationConnectionType;

  /*!
   * \brief A type alias for "SendingRoutingController".
   */
  using SendingRoutingControllerType = SendingRoutingController<ApplicationConnection>;

  /*!
   * \brief Packet validator type.
   */
  using PacketValidatorType = packet_handler::LocalIncomingPacketValidator;

  /*!
   * \brief Constructor of "ReceivingRoutingController".
   *
   * \param[in] application_identifier      A unique application identifier.
   * \param[in] peer_process_id             The process id of the connected application.
   * \param[in] sending_routing_controller  A share pointer of sending_routing_controller.
   * \param[in] packet_validator            A validator for incoming packets.
   * \param[in] local_servers               A reference to the offered Local Server instances.
   * \param[in] local_clients               A reference to the Local Clients instances.
   * \param[in] application_tracing         A reference to the application tracing implementation.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  ReceivingRoutingController(amsr::ipc::Credentials const application_identifier,
                             osabstraction::process::ProcessId const peer_process_id,
                             std::shared_ptr<SendingRoutingControllerType> const& sending_routing_controller,
                             PacketValidatorType const& packet_validator, LocalServerMap const& local_servers,
                             LocalClientMap const& local_clients,
                             tracing::ApplicationTracingInterface& application_tracing) noexcept
      : application_identifier_{application_identifier},
        peer_process_id_{peer_process_id},
        sending_routing_controller_{sending_routing_controller},
        packet_validator_{packet_validator},
        local_servers_{local_servers},
        local_clients_{local_clients},
        application_tracing_{application_tracing} {}

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief   Destructor of ReceivingRoutingController.
   * \steady  FALSE
   */
  ~ReceivingRoutingController() noexcept = default;

  ReceivingRoutingController(ReceivingRoutingController const&) = delete;
  ReceivingRoutingController(ReceivingRoutingController const&&) = delete;
  auto operator=(ReceivingRoutingController const&) -> ReceivingRoutingController& = delete;
  auto operator=(ReceivingRoutingController const&&) -> ReceivingRoutingController& = delete;

  /*!
   * \brief Processes a SOME/IP message received from the connected application.
   *
   * \param[in] specific_header_view A view containing the specific message header of the given routing message.
   * \param[in] memory_buffer A memory buffer containing a routing message.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Deserialize the message header.
   * - Create someip message header.
   * - Create someip packet.
   * - If the validation of the sourceId or the header failed, log an error and send error response.
   * - Forward the message to the local server/client depending on the message type.
   * \endinternal
   */
  void OnRoutingSomeIpMessage(
      SpecificHeaderView const& specific_header_view,
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer) noexcept {
    // Deserialize routing message header.
    amsr::someipd_app_protocol::internal::Reader reader{specific_header_view};
    ara::core::Optional<amsr::someipd_app_protocol::internal::RoutingSomeIpMessageHeader> const routing_header{
        amsr::someipd_app_protocol::internal::DeserMessageHeaders::
            DeserializeSpecificIpcPacketHeaderRoutingSomeIpMessage(reader)};

    if (routing_header.has_value()) {
      // Get SOME/IP instance identifier.
      // VECTOR NL AutosarC++17_10-M8.5.1: MD_SomeIpDaemon_AutosarC++17_10_M8.5.1_false_positive
      someip_protocol::internal::InstanceId const instance_id{routing_header->instance_id};

      // Create SOME/IP message from the given memory buffer.
      ara::core::Result<someip_protocol::internal::SomeIpMessage> someip_message{
          someip_protocol::internal::SomeIpMessage::CreateSomeIpMessage(std::move(memory_buffer))};

      if (someip_message.HasValue()) {
        // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
        std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet{
            someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<someip_protocol::internal::SomeIpMessage>(
                std::move(someip_message.Value()))};

        someip_protocol::internal::SomeIpMessageHeader const& header{packet->GetHeader()};

        ara::core::Result<void> const validation_result{
            packet_validator_.Validate(application_identifier_, header, instance_id)};

        if (validation_result.HasValue()) {
          if (header.message_type_ == someip_protocol::internal::SomeIpMessageType::kNotification) {
            OnEvent(instance_id, packet);
          } else if ((header.message_type_ == someip_protocol::internal::SomeIpMessageType::kResponse) ||
                     (header.message_type_ == someip_protocol::internal::SomeIpMessageType::kError)) {
            // TODO SVT-3253: create a dummy RemoteSourceIdentifier for now; will be replaced with a real on in an
            // upcoming change
            packet_router::RemoteSourceIdentifier const recipient{};
            OnMethodResponse(instance_id, recipient, packet);
          } else {
            assert((header.message_type_ == someip_protocol::internal::SomeIpMessageType::kRequest) ||
                   (header.message_type_ == someip_protocol::internal::SomeIpMessageType::kRequestNoReturn));
            OnMethodRequest(instance_id, packet);
          }

        } else {  // Validation Failed
          logger_.LogError(
              [&validation_result, &header, &instance_id](ara::log::LogStream& s) noexcept {
                ara::core::ErrorCode const error_code{validation_result.Error()};
                s << error_code.Message();
                s << " - (ServiceId: 0x";
                s << ara::log::HexFormat(header.service_id_);
                s << ", MajorVersion: 0x";
                s << ara::log::HexFormat(header.interface_version_);
                s << ", InstanceId: 0x";
                s << ara::log::HexFormat(instance_id);
                s << "): MethodID: 0x";
                s << ara::log::HexFormat(header.method_id_);
                s << ", SessionID: 0x";
                s << ara::log::HexFormat(header.session_id_);
                s << ". No forwarding.";
              },
              static_cast<char const*>(__func__), __LINE__);
          if (header.message_type_ == someip_protocol::internal::SomeIpMessageType::kRequest) {
            someip_protocol::internal::SomeIpReturnCode const someip_return_code{
                TranslateErrorCode(validation_result.Error())};
            logger_.LogDebug([](ara::log::LogStream& s) noexcept { s << "Sending error response."; },
                             static_cast<char const*>(__func__), __LINE__);

            SendErrorResponse(header, someip_return_code, instance_id);
          }
        }
      } else {
        logger_.LogError([](ara::log::LogStream& s) noexcept { s << "Cannot create SOME/IP message from the buffer"; },
                         static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError(
          [&specific_header_view](ara::log::LogStream& s) noexcept {
            SpecificHeaderView::size_type const size{specific_header_view.size()};
            s << "Deserialization of RoutingSomeIpMessageHeader has failed. Received length: " << size;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Processes a PDU message received from the connected application.
   *
   * \param[in] specific_header_view A view containing the specific message header of the given routing message.
   * \param[in] memory_buffer A memory buffer containing a routing message.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Deserialize the message header.
   * - Create pdu message header.
   * - Create pdu packet.
   * - If the validation of the sourceId or the header failed, log an error and exit the function.
   * - Forward the message to the local server.
   * \endinternal
   */
  void OnRoutingPduMessage(
      SpecificHeaderView const& specific_header_view,
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer) const noexcept {
    // Deserialize routing message header.
    amsr::someipd_app_protocol::internal::Reader reader{specific_header_view};
    ara::core::Optional<amsr::someipd_app_protocol::internal::RoutingPduMessageHeader> const routing_header{
        amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeSpecificIpcPacketHeaderRoutingPduMessage(
            reader)};

    if (routing_header.has_value()) {
      // Get SOME/IP instance identifier.
      // VECTOR NL AutosarC++17_10-M8.5.1: MD_SomeIpDaemon_AutosarC++17_10_M8.5.1_false_positive
      someip_protocol::internal::InstanceId const instance_id{routing_header->instance_id};

      // Create SOME/IP message from the given memory buffer.
      ara::core::Result<someip_protocol::internal::PduMessage> pdu_message{
          someip_protocol::internal::PduMessage::CreatePduMessage(std::move(memory_buffer))};

      if (pdu_message.HasValue()) {
        // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
        std::shared_ptr<someip_protocol::internal::PduMessage> const packet{
            someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<someip_protocol::internal::PduMessage>(
                std::move(pdu_message.Value()))};

        someip_protocol::internal::PduMessageHeader const& header{packet->GetHeader()};
        ara::core::Result<void> const validation_result{packet_validator_.Validate(header)};

        if (validation_result.HasValue()) {
          OnPduEvent(instance_id, packet);
        } else {
          logger_.LogError(
              [&validation_result, &header, &instance_id](ara::log::LogStream& s) noexcept {
                ara::core::ErrorCode const error_code{validation_result.Error()};
                s << error_code.Message() << " - (InstanceId: 0x" << ara::log::HexFormat(instance_id) << ", PduId: 0x "
                  << ara::log::HexFormat(header.pdu_id_) << ") . No forwarding.";
              },
              static_cast<char const*>(__func__), __LINE__);
        }
      } else {
        logger_.LogError([](ara::log::LogStream& s) noexcept { s << "Cannot create PDU message from the buffer"; },
                         static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError(
          [&specific_header_view](ara::log::LogStream& s) noexcept {
            SpecificHeaderView::size_type const size{specific_header_view.size()};
            s << "Deserialization of RoutingPduMessageHeader has failed. Received length: " << size;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

 private:
  /*!
   * \brief handle Event responses received from the connected application in case of message type is response.
   *
   * \param[in] instance_id   SOME/IP instance id..
   * \param[in] packet        SOME/IP request message.
   *
   * \steady TRUE
   *
   * \internal
   * - Trace the event transmission
   * - Get the corresponding local server.
   * - Find the localServer mapped to service_instance_id.
   * - If local server not found, log an error and send error response.
   * - Send event to the local server.
   * \endinternal
   */
  void OnEvent(someip_protocol::internal::InstanceId const instance_id,
               std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet) const {
    someip_protocol::internal::SomeIpMessageHeader const& header{packet->GetHeader()};
    // Get SOME/IP service identifier.
    someip_protocol::internal::ServiceId const service_id{header.service_id_};
    someip_protocol::internal::MajorVersion const major_version{header.interface_version_};

    // Trace event transmission
    application_tracing_.TraceSomeIpEventSendSync(peer_process_id_, service_id, major_version, instance_id,
                                                  header.method_id_, packet->GetTotalSize());

    LocalServerId const local_server_id{{service_id, {major_version, 0 /* Multiple MinorVersions not yet supported*/}},
                                        instance_id};
    LocalServerMap::const_iterator const it{local_servers_.find(local_server_id)};

    if (it != local_servers_.cend()) {
      it->second->SendEvent(packet);
    } else {
      logger_.LogError(
          [&header, &instance_id](ara::log::LogStream& s) noexcept {
            s << "Service has not been offered before for event (ServiceId: ";
            s << ara::log::HexFormat(header.service_id_);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(header.interface_version_);
            s << ", InstanceId: ";
            s << ara::log::HexFormat(instance_id);
            s << ", EventId: ";
            s << ara::log::HexFormat(header.method_id_);
            s << ", SessionId: ";
            s << ara::log::HexFormat(header.session_id_);
            s << ")";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief handle PDU Event responses received from the connected application in case of message type is response.
   *
   * \param[in] instance_id   Pdu instance id..
   * \param[in] packet        Pdu request message.
   *
   * \steady TRUE
   *
   * \internal
   * - Trace the PDU event transmission.
   * - Get the corresponding local server.
   * - Find the localServer mapped to service_instance_id.
   * - If local server not found, log an error and send error response.
   * - Send Pdu event to the local server.
   * \endinternal
   */
  void OnPduEvent(someip_protocol::internal::InstanceId const instance_id,
                  std::shared_ptr<someip_protocol::internal::PduMessage> const& packet) const {
    someip_protocol::internal::PduMessageHeader const& header{packet->GetHeader()};
    // Get Pdu service identifier.
    someip_protocol::internal::ServiceId const service_id{
        someip_protocol::internal::PduMessageHeaderUtility::GetServiceId(header)};
    someip_protocol::internal::EventId const event_id{
        someip_protocol::internal::PduMessageHeaderUtility::GetMethodId(header)};

    // Trace PDU transmission
    application_tracing_.TracePduEventSendSync(peer_process_id_, service_id, instance_id, event_id,
                                               packet->GetTotalSize());

    bool found_local_server{false};
    for (LocalServerMap::const_iterator it{local_servers_.cbegin()}; it != local_servers_.cend(); ++it) {
      if ((it->first.deployment_id.service_interface_id == service_id) && (it->first.instance_id == instance_id)) {
        it->second->SendPduEvent(packet);
        found_local_server = true;
        break;
      }
    }

    if (!found_local_server) {
      logger_.LogError(
          [&service_id, &instance_id](ara::log::LogStream& s) noexcept {
            s << "Service has not been offered before for S2S event (ServiceId: ";
            s << ara::log::HexFormat(service_id);
            s << ", InstanceId: ";
            s << ara::log::HexFormat(instance_id);
            s << ")";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief handle method responses received from the connected application in case of message type is response.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] recipient     The response recipient.
   * \param[in] packet        SOME/IP request message.
   *
   * \steady TRUE
   *
   * \internal
   * - Trace method response transmission.
   * - Get the corresponding local server.
   * - Find the localServer mapped to service_instance_id.
   * - If local server not found, log an error and send error response.
   * - Send method response to the local server.
   * \endinternal
   */
  void OnMethodResponse(someip_protocol::internal::InstanceId const instance_id,
                        packet_router::RemoteSourceIdentifier const& recipient,
                        std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet) const {
    someip_protocol::internal::SomeIpMessageHeader const& header{packet->GetHeader()};
    // Get SOME/IP service identifier and major version.
    someip_protocol::internal::ServiceId const service_id{header.service_id_};
    someip_protocol::internal::MajorVersion const major_version{header.interface_version_};

    // Trace response transmission
    application_tracing_.TraceMethodResponseSendSync(peer_process_id_, service_id, major_version, instance_id,
                                                     header.method_id_, packet->GetTotalSize());

    LocalServerId const local_server_id{{service_id, {major_version, 0 /* Multiple MinorVersions not yet supported*/}},
                                        instance_id};
    LocalServerMap::const_iterator const it{local_servers_.find(local_server_id)};

    if (it != local_servers_.cend()) {
      it->second->SendMethodResponse(packet, recipient);
    } else {
      logger_.LogError(
          [&header, &instance_id](ara::log::LogStream& s) noexcept {
            s << "Service has not been offered before for method response (ServiceId: 0x";
            s << ara::log::HexFormat(header.service_id_);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(header.interface_version_);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(instance_id);
            s << ", MethodId: 0x";
            s << ara::log::HexFormat(header.method_id_);
            s << ", ClientId: 0x";
            s << ara::log::HexFormat(header.client_id_);
            s << ", SessionId: 0x";
            s << ara::log::HexFormat(header.session_id_);
            s << ")";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Send method requests received from the connected application in case of message type is request.
   *
   * \param[in] instance_id   SOME/IP instance id..
   * \param[in] packet        SOME/IP request message.
   *
   * \steady TRUE
   *
   * \internal
   * - Trace method request transmission.
   * - Get the corresponding local client.
   *   - If local client found, forward method request to the client client
   *     - If method request can't be forwarded, log specific error.
   *   - If local client not found, log an error
   * - If local client nor found or transmission failed, and the message type is kRequest
   *   - Send back an error response to the application
   * \endinternal
   */
  void OnMethodRequest(someip_protocol::internal::InstanceId const instance_id,
                       std::shared_ptr<someip_protocol::internal::SomeIpMessage>& packet) {
    ara::core::Result<void> send_method_result{
        packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ServiceNotOffered};
    someip_protocol::internal::SomeIpMessageHeader const& header{packet->GetHeader()};

    // Trace request transmission
    application_tracing_.TraceMethodRequestSendSync(peer_process_id_, header.service_id_, header.interface_version_,
                                                    instance_id, header.method_id_, packet->GetTotalSize());

    configuration::types::LocalClientId const local_client_id{
        configuration::types::SomeIpServiceInstanceId{
            configuration::types::SomeIpServiceInterfaceDeploymentId{
                header.service_id_, configuration::types::SomeIpServiceVersion{header.interface_version_, 0U}},
            instance_id},
        header.client_id_};

    typename LocalClientMap::const_iterator const local_client_it{local_clients_.find(local_client_id)};
    if (local_client_it != local_clients_.cend()) {
      send_method_result = local_client_it->second->SendMethodRequest(packet);

      if (!send_method_result.HasValue()) {
        ara::core::StringView const error_message{send_method_result.Error().Message()};
        ara::core::StringView const user_msg{send_method_result.Error().UserMessage()};
        ara::core::ErrorDomain::SupportDataType const support_data{send_method_result.Error().SupportData()};
        logger_.LogError(
            [&instance_id, &header, &error_message, &user_msg, &support_data](ara::log::LogStream& s) noexcept {
              s << "Could not forward method request (ServiceId: ";
              s << ara::log::HexFormat(header.service_id_);
              s << ", MajorVersion: 0x";
              s << ara::log::HexFormat(header.interface_version_);
              s << ", InstanceId: ";
              s << ara::log::HexFormat(instance_id);
              s << ", MethodId: ";
              s << ara::log::HexFormat(header.method_id_);
              s << ", ClientId: ";
              s << ara::log::HexFormat(header.client_id_);
              s << ", SessionId: ";
              s << ara::log::HexFormat(header.session_id_);
              s << "). ";
              s << "Error message: ";
              s << error_message;
              s << ", user message: ";
              s << user_msg;
              s << ", support data: ";
              s << support_data;
              s << ".";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError(
          [&instance_id, &header](ara::log::LogStream& s) noexcept {
            s << "Could not forward method request (ServiceId: ";
            s << ara::log::HexFormat(header.service_id_);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(header.interface_version_);
            s << ", InstanceId: ";
            s << ara::log::HexFormat(instance_id);
            s << ", MethodId: ";
            s << ara::log::HexFormat(header.method_id_);
            s << ", ClientId: ";
            s << ara::log::HexFormat(header.client_id_);
            s << ", SessionId: ";
            s << ara::log::HexFormat(header.session_id_);
            s << "). Local client not found.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    // Send an error method response in case of transmission failure
    if ((!send_method_result.HasValue()) &&
        (header.message_type_ == someip_protocol::internal::SomeIpMessageType::kRequest)) {
      logger_.LogDebug([](ara::log::LogStream& s) noexcept { s << "Sending error response."; },
                       static_cast<char const*>(__func__), __LINE__);

      someip_protocol::internal::SomeIpReturnCode const someip_return_code{
          TranslateErrorCode(send_method_result.Error())};

      SendErrorResponse(header, someip_return_code, instance_id);
    }
  }

  /*!
   * \brief Send an error response for a failed method request.
   *
   * \param[in] header SOME/IP header.
   * \param[in] error_code Return code to set in the SOME/IP header.
   * \param[in] instance_id SOME/IP service instance identifier.
   *
   * \steady  TRUE
   *
   * \internal
   * - Log an error with the passed in error code
   * - Try creating SOME/IP error response packet
   * - Set the passed in packet as the error response packet
   * - Try forwarding the newly set packet
   * - If forwarding fails
   *   - Log an error about failed forwarding
   * \endinternal
   */
  void SendErrorResponse(someip_protocol::internal::SomeIpMessageHeader const& header,
                         someip_protocol::internal::SomeIpReturnCode const error_code,
                         someip_protocol::internal::InstanceId const instance_id) {
    logger_.LogVerbose(
        [&error_code](ara::log::LogStream& s) noexcept {
          ara::log::LogHex8 const error_code_hex{ara::log::HexFormat(static_cast<std::uint8_t>(error_code))};

          s << "Sending error response with SOME/IP return code (0x" << error_code_hex << ").";
        },
        static_cast<char const*>(__func__), __LINE__);

    vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> error_res{
        someip_protocol::internal::MessageBuilder::CreateSomeIpErrorHeader(error_code, header,
                                                                           error_message_memory_buffer_allocator_)};

    // Use the packet for the error message - swap it with the original request.
    ara::core::Result<someip_protocol::internal::SomeIpMessage> someip_message{
        someip_protocol::internal::SomeIpMessage::CreateSomeIpMessage(std::move(error_res))};
    if (someip_message.HasValue()) {
      // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
      std::shared_ptr<someip_protocol::internal::SomeIpMessage> const packet{
          someip_daemon_core::memory::MemoryUtilities::CreateSharedPtr<someip_protocol::internal::SomeIpMessage>(
              std::move(someip_message.Value()))};
      sending_routing_controller_->OnMethodResponse(instance_id, packet);
    } else {
      logger_.LogError([](ara::log::LogStream& s) noexcept { s << "Buffer too small for SOME/IP header."; },
                       static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Translation between error code types
   *
   * \param[in] error_code The error code to translate
   *
   * \return The validation error in SomeIpReturnCode format
   *
   * \context App
   * \steady  TRUE
   *
   * \internal
   * - Check input error code and translate it to SomeIpReturnCode format
   * \endinternal
   */
  static someip_protocol::internal::SomeIpReturnCode TranslateErrorCode(ara::core::ErrorCode const& error_code) {
    someip_protocol::internal::SomeIpReturnCode translated_error{someip_protocol::internal::SomeIpReturnCode::kNotOk};

    switch (static_cast<packet_handler::validators::LocalIncomingPacketValidatorError>(error_code.Value())) {
      case packet_handler::validators::LocalIncomingPacketValidatorError::kHeaderValidationError_WrongProtocolVersion: {
        translated_error = someip_protocol::internal::SomeIpReturnCode::kWrongProtocolVersion;
        break;
      }
      case packet_handler::validators::LocalIncomingPacketValidatorError::kHeaderValidationError_WrongMessageType: {
        translated_error = someip_protocol::internal::SomeIpReturnCode::kWrongMessageType;
        break;
      }
      case packet_handler::validators::LocalIncomingPacketValidatorError::kConfigurationError_UnknownMethod: {
        translated_error = someip_protocol::internal::SomeIpReturnCode::kUnknownMethod;
        break;
      }
      case packet_handler::validators::LocalIncomingPacketValidatorError::kConfigurationError_WrongInterfaceVersion: {
        translated_error = someip_protocol::internal::SomeIpReturnCode::kWrongInterfaceVersion;
        break;
      }
      case packet_handler::validators::LocalIncomingPacketValidatorError::kConfigurationError_UnknownService: {
        translated_error = someip_protocol::internal::SomeIpReturnCode::kUnknownService;
        break;
      }
      case packet_handler::validators::LocalIncomingPacketValidatorError::kSecurityValidationError: {
        translated_error = someip_protocol::internal::SomeIpReturnCode::kNotReachable;
        break;
      }
      case packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionTransmissionFailed:
      case packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable: {
        translated_error = someip_protocol::internal::SomeIpReturnCode::kNotReachable;
        break;
      }
      case packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ServiceNotOffered: {
        translated_error = someip_protocol::internal::SomeIpReturnCode::kNotReady;
        break;
      }
      default:
        // Nothing to do.
        break;
    }

    return translated_error;
  }

  /*!
   * \brief Unique application identifier
   */
  amsr::ipc::Credentials const application_identifier_;

  /*!
   * \brief Process id of the connected application
   */
  osabstraction::process::ProcessId const peer_process_id_;

  /*!
   * \brief The sending routing controller.
   * \details It must be a shared pointer because the packet router accepts only shared pointers.
   */
  std::shared_ptr<SendingRoutingControllerType> sending_routing_controller_;

  /*!
   * \brief The reference to the packet validator.
   */
  PacketValidatorType const& packet_validator_;

  /*!
   * \brief Map of offered local servers.
   * \details This map is managed by the Application class (memory owner) and written/read in ApplicationServerHandler
   */
  LocalServerMap const& local_servers_;

  /*!
   * \brief Map of local clients.
   * \details This map is managed by the Application class (memory owner) and written/read in ApplicationClientHandler
   * by emplacing local clients during request the service and erasing it during release the service.
   */
  LocalClientMap const& local_clients_;

  /*!
   * \brief Reference to application tracing implementation
   */
  tracing::ApplicationTracingInterface& application_tracing_;

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger const logger_{
      someip_daemon_core::logging::kApplicationLoggerContextId,
      someip_daemon_core::logging::kApplicationLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix("ReceivingRoutingController"_sv, application_identifier_)};

  /*!
   * \brief Allocator for creating error messages as response to invalid incoming request messages.
   */
  someip_daemon_core::MemoryBufferAllocator error_message_memory_buffer_allocator_{};
};

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_RECEIVING_ROUTING_CONTROLLER_H_

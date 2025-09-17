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
/**        \file  command_controller.h
 *        \brief  Application command controller
 *
 *      \details  Receives commands sent from ApplicationConnection, deserializes them, prepare serialized responses
 *                and sends them back.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_COMMAND_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_COMMAND_CONTROLLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <utility>

#include "amsr/someip_daemon_core/application/application_client_handler_interface.h"
#include "amsr/someip_daemon_core/application/application_commands_wrapper.h"
#include "amsr/someip_daemon_core/application/application_server_handler_interface.h"
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/someip_daemon_core/service_discovery/ser_deser_service_discovery.h"
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "ara/log/logger.h"
#include "ara/log/logstream.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/deser_message_headers.h"
#include "someipd_app_protocol/internal/ipc_packet_serializer.h"
#include "someipd_app_protocol/internal/message.h"
#include "someipd_app_protocol/internal/protocol_types.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Shorting for namespace amsr::someipd_app_protocol::internal.
 */
namespace someipd_app_protocol = ::amsr::someipd_app_protocol::internal;

/*!
 * \brief Command Controller.
 */
template <typename ApplicationConnectionType>
class CommandController final {
 public:
  /*!
   * \brief Type alias for "ApplicationConnection".
   */
  using ApplicationConnection = ApplicationConnectionType;

  /*!
   * \brief Type alias for "ApplicationServerHandler".
   */
  using ApplicationServerHandlerType = ApplicationServerHandlerInterface;
  /*!
   * \brief Type alias for "ApplicationClientHandler".
   */
  using ApplicationClientHandlerType = ApplicationClientHandlerInterface;
  /*!
   * \brief Type alias for "ControlHeaderView".
   */
  using ControlHeaderView = SpecificHeaderView;

  /*!
   * \brief Type alias for "ControlMessageReturnCode".
   */
  using ControlMessageReturnCode = ::amsr::someipd_app_protocol::internal::ControlMessageReturnCode;

  /*!
   * \brief Underlying type of "MessageType".
   */
  using MessageTypeUnderlyingType = std::underlying_type_t<amsr::someipd_app_protocol::internal::MessageType>;

  /*!
   * \brief Underlying type of "ControlMessageReturnCode".
   */
  using ControlMessageReturnCodeUnderlyingType =
      std::underlying_type_t<amsr::someipd_app_protocol::internal::ControlMessageReturnCode>;

  /*!
   * \brief A type alias for "ApplicationCommandsWrapper".
   */
  using ApplicationCommandsWrapperType = ApplicationCommandsWrapper;

  /*!
   * \brief Type alias for "MemoryBufferView".
   */
  using MemoryBufferView = ::vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer>::MemoryBufferView;

  /*!
   * \brief Type alias for "Reader".
   */
  using Reader = ::amsr::someipd_app_protocol::internal::Reader;

  /*!
   * \brief Type-alias for general payload for command messages
   */
  using ServiceDiscoveryMessagePayload = amsr::someipd_app_protocol::internal::ServiceInstanceIdentificationData;

  /*!
   * \brief Error message in case "nullptr" request payload is received while a payload is expected.
   */
  static constexpr char const* kErrorMsgRequestCommandNullPayload{
      "Failed to deserialize received command control request packet. NULL payload received while payload is "
      "expected."};

  /*!
   * \brief Error message in case request command deserialization failed (unexpected payload length received).
   */
  static constexpr char const* kErrorMsgRequestCommandUnexpectedPayloadLength{
      "Failed to deserialize received command control request packet. Unexpected payload length."};

  /*!
   * \brief Constructor.
   *
   * \param[in] application_client_handler A reference to the application client handler.
   * \param[in] application_server_handler A reference to the application server handler.
   * \param[in] application_connection A reference to the object responsible for IPC connection for this application.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  CommandController(ApplicationClientHandlerType& application_client_handler,
                    ApplicationServerHandlerType& application_server_handler,
                    ApplicationConnection& application_connection)
      : application_commands_wrapper_{application_client_handler, application_server_handler},
        application_connection_(application_connection) {}

  /*!
   * \brief   Default destructor.
   * \steady  FALSE
   */
  ~CommandController() = default;

  CommandController(CommandController const&) = delete;
  CommandController(CommandController const&&) = delete;
  auto operator=(CommandController const&) -> CommandController& = delete;
  auto operator=(CommandController const&&) -> CommandController& = delete;

  /*!
   * \brief Called once a control message is received.
   *
   * \param[in] message_type Received control message type.
   * \param[in] specific_header_view Control message header view.
   * \param[in] payload_buffer A memory buffer containing the received control message.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Deserialize request control header.
   * - Validate control header
   * - Send message response to SomeIpDaemonClient
   * - For message types where SomeIpDaemonClient expects an answer,
   *   - Serialize response control header.
   *   - Determine response message type.
   *   - Send response.
   * \endinternal
   */
  void OnControlMessage(amsr::someipd_app_protocol::internal::MessageType const& message_type,
                        ControlHeaderView const& specific_header_view,
                        vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) {
    // PTP-B-SomeIpDaemon-CommandController_OnControlMessage
    ControlMessageReturnCode validate_control_header_return_code{ControlMessageReturnCode::kNotOk};
    response_packet_.reset(nullptr);

    // Deserialize request control header.
    Reader reader{specific_header_view};
    ara::core::Optional<amsr::someipd_app_protocol::internal::ControlMessageHeader> const control_header{
        deserializer_.DeserializeSpecificIpcPacketHeaderCommandMessage(reader)};

    validate_control_header_return_code =
        ValidateControlHeaderAndForwardToHandler(message_type, control_header, payload_buffer);

    SendResponseToSomeIpDaemonClient(message_type, validate_control_header_return_code);
    // PTP-E-SomeIpDaemon-CommandController_OnControlMessage
  }

 private:
  // ---- Handlers (Payload Deserializer/Serializer) ------------------------------------------------------

  /*!
   * \brief "ReleaseService" control message handler.
   *
   * \param[in] payload_buffer Memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return return code: "kOk" if deserialization succeeds and "ClientId" is released, otherwise an error code.
   *
   * \internal
   * - If provided payload is verified,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Release the client ID.
   *   - Else
   *     - Log an error message.
   *     - Set the return code to indicate a malformed message.
   * - Else
   *   - Log an error message.
   *   - Set the return code to indicate a malformed message.
   * - Return the result.
   * \endinternal
   */
  auto ReleaseServiceHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept
      -> ControlMessageReturnCode {
    ControlMessageReturnCode result{ControlMessageReturnCode::kNotOk};
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    // Verify payload.
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Prepare "Reader".
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      Reader reader{buffer_view};

      ara::core::Optional<amsr::someipd_app_protocol::internal::ControlMessageReleaseServicePayload> request{
          service_discovery::DeserializeControlMessageReleaseServiceRequestPayload(reader)};

      // Verify deserialized payload.
      if (request.has_value()) {
        result = application_commands_wrapper_.ReleaseService(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << " Received length: " << received_length
                << ", Expected length: "
                << amsr::someipd_app_protocol::internal::kControlMessageReleaseServiceRequestPayloadLength;
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ControlMessageReturnCode::kMalformedMessage;
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
      result = ControlMessageReturnCode::kMalformedMessage;
    }
    // No response payload.

    return result;
  }

  /*!
   * \brief "RequestLocalServer" control message handler.
   *
   * \param[in] payload_buffer Memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return return code: "kOk" if deserialization succeeds and "Service" is configured and available.
   *
   * \internal
   * - If the provided payload is valid,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Request the local server.
   *   - Else
   *     - Log an error message.
   *     - Set the return code to indicate a malformed message.
   * - Else
   *   - Log an error message.
   *   - Set the return code to indicate a malformed message.
   * - Return the result.
   * \endinternal
   *
   * \trace SPEC-10144770
   */
  auto RequestLocalServerHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept
      -> ControlMessageReturnCode {
    ControlMessageReturnCode result{ControlMessageReturnCode::kNotOk};

    // Verify payload.
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Deserialize payload.
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      Reader reader{buffer_view};

      ara::core::Optional<ServiceDiscoveryMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeServiceInstanceIdentificationData(
              reader)};

      // Verify deserialized payload.
      if (request.has_value()) {
        result = application_commands_wrapper_.RequestLocalServer(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ControlMessageReturnCode::kMalformedMessage;
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
      result = ControlMessageReturnCode::kMalformedMessage;
    }

    // No response payload.

    return result;
  }

  /*!
   * \brief "ReleaseLocalServer" control message handler.
   *
   * \param[in] payload_buffer Memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \internal
   * - If provided payload is verified,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Release the local server.
   *   - Else
   *     - Log an error message.
   * - Else
   *   - Log an error message.
   * \endinternal
   *
   * \trace SPEC-10144770
   */
  void ReleaseLocalServerHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept {
    // Verify payload
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Deserialize Payload
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      Reader reader{buffer_view};

      ara::core::Optional<ServiceDiscoveryMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeServiceInstanceIdentificationData(
              reader)};

      // Verify deserialized payload
      if (request.has_value()) {
        application_commands_wrapper_.ReleaseLocalServer(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief "OfferService" control message handler.
   *
   * \param[in] payload_buffer Memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - If the provided payload is valid,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Offer the service.
   *   - Else
   *     - Log an error message.
   * - Else
   *   - Log an error message.
   * \endinternal
   *
   * \trace SPEC-10144770
   */
  void OfferServiceHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept {
    // Verify payload.
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Deserialize payload.
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      Reader reader{buffer_view};

      ara::core::Optional<ServiceDiscoveryMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeServiceInstanceIdentificationData(
              reader)};

      // Verify deserialized payload.
      if (request.has_value()) {
        application_commands_wrapper_.OfferService(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief "StopOfferService" control message handler.
   *
   * \param[in] payload_buffer Memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - If provided payload is verified,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Stop offering the service.
   *   - Else
   *     - Log an error message.
   * - Else
   *   - Log an error message.
   * \endinternal
   *
   * \trace SPEC-10144770
   */
  void StopOfferServiceHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept {
    // Verify payload
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Deserialize Payload
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      Reader reader{buffer_view};

      ara::core::Optional<ServiceDiscoveryMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeServiceInstanceIdentificationData(
              reader)};

      // Verify deserialized payload
      if (request.has_value()) {
        application_commands_wrapper_.StopOfferService(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief "SubscribeEvent" control message handler.
   *
   * \param[in] payload_buffer Memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return return code: "kOk" if deserialization succeeds and the event is successfully subscribed, otherwise an
   *         error code.
   *
   * \internal
   * - If provided payload is verified,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Subscribe to the requested event.
   *   - Else
   *     - Log an error message.
   *     - Set the return code to indicate a malformed message.
   * - Else
   *   - Log an error message.
   *   - Set the return code to indicate a malformed message.
   * - Return the result.
   * \endinternal
   *
   * \trace SPEC-10144770
   */
  auto SubscribeEventHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept
      -> ControlMessageReturnCode {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};

    // Verify payload.
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Deserialize payload.
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      Reader reader{buffer_view};

      ara::core::Optional<amsr::someipd_app_protocol::internal::EventControlMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::
              DeserializeStructWithServiceIdInstanceIdMajorVersionMinorVersionEventIdClientId(reader)};

      // Verify deserialized payload.
      if (request.has_value()) {
        result = application_commands_wrapper_.SubscribeEvent(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kControlMessageSubscribeEventRequestPayloadLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ControlMessageReturnCode::kMalformedMessage;
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
      result = ControlMessageReturnCode::kMalformedMessage;
    }

    return result;
  }

  /*!
   * \brief "UnsubscribeEvent" control message handler.
   *
   * \param[in] payload_buffer memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return return code: "kOk" if deserialization succeeds and the event is successfully unsubscribed, otherwise an
   *         error code.
   *
   * \internal
   * - If provided payload is verified,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Unsubscribe to the requested event.
   *   - Else
   *     - Log an error message.
   *     - Set the return code to indicate a malformed message.
   * - Else
   *   - Log an error message.
   *   - Set the return code to indicate a malformed message.
   * - Return the result.
   * \endinternal
   *
   * \trace SPEC-10144770
   */
  auto UnsubscribeEventHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept
      -> ControlMessageReturnCode {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};

    // Verify payload
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Deserialize Payload
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      Reader reader{buffer_view};

      ara::core::Optional<amsr::someipd_app_protocol::internal::EventControlMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::
              DeserializeStructWithServiceIdInstanceIdMajorVersionMinorVersionEventIdClientId(reader)};

      // Verify deserialized payload
      if (request.has_value()) {
        result = application_commands_wrapper_.UnsubscribeEvent(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kControlMessageUnsubscribeEventRequestPayloadLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ControlMessageReturnCode::kMalformedMessage;
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
      result = ControlMessageReturnCode::kMalformedMessage;
    }

    return result;
  }

  /*!
   * \brief "RequestService" control message handler.
   *
   * \param[in] payload_buffer memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return return code: "kOk" if deserialization succeeds and the service is successfully requested, otherwise an
   *         error code.
   *
   * \internal
   * - If provided payload is verified,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Request the service.
   *     - If the client ID could be retrieved,
   *      - Allocate memory for the response packet.
   *      - If allocation is successful,
   *       - Serialize control message.
   *     - Else if the identity access manager is denied the access,
   *       - Log an error message.
   *       - Set the return code to access_denied to indicate failure.
   *     - Else if the client Id generator reaches the maximum number of simultaneous clients,
   *       - Log an error message.
   *       - Set the return code to client_ids_overflow to indicate failure.
   *     - Else if the remote server is not found,
   *       - Log an error message.
   *       - Set the return code to remote_server_not_found to indicate failure.
   *     - Else if the required service instance is not found,
   *       - Log an error message.
   *       - Set the return code to required_service_instance_not_found to indicate failure.
   *     - Else, nothing to do.
   *   - Else
   *     - Log an error message.
   *     - Set the return code to indicate malformed message.
   * - Else
   *     - Log an error message.
   *     - Set the return code to indicate a malformed message.
   * - Return the result.
   * \endinternal
   */
  auto RequestServiceHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept
      -> ControlMessageReturnCode {
    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};

    // Verify payload.
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Prepare reader.
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const deser_buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                                 packet_view[0U].size};
      ::amsr::someip_protocol::internal::deserialization::Reader reader{deser_buffer_view};

      ara::core::Optional<ServiceDiscoveryMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeServiceInstanceIdentificationData(
              reader)};

      // Verify deserialized payload.
      if (request.has_value()) {
        ara::core::Result<someip_protocol::internal::ClientId> const request_service_result{
            application_commands_wrapper_.RequestService(request.value())};
        if (request_service_result.HasValue()) {
          amsr::someipd_app_protocol::internal::ControlMessageClientId const client_id{request_service_result.Value()};
          result = ControlMessageReturnCode::kOk;
          // Serialize response payload.
          std::size_t const response_size{
              amsr::someipd_app_protocol::internal::kControlMessageRequestServiceResponsePayloadLength};

          // Allocate memory for our packet
          ara::core::Result<vac::memory::allocator::MemoryBufferPtr> alloc_result{
              tx_buffer_allocator_.Allocate(response_size)};
          // Memory allocation is not expected to fail. In case of lack of memory, it would be handled by the allocator
          assert(alloc_result.HasValue());

          // Wrap the memory into the appropriate data structure (for compatibility reasons)
          response_packet_ = memory::MemoryUtilities::CreateUniquePtr<
              vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>(std::move(alloc_result.Value()));

          std::uint8_t* const response_packet_pointer{
              // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
              static_cast<std::uint8_t*>(response_packet_->GetView(0U)[0U].base_pointer)};
          std::size_t const response_packet_size{response_packet_->size()};
          someipd_app_protocol::someip_protocol::BufferView const buffer_view{
              // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
              response_packet_pointer, response_packet_size};
          someipd_app_protocol::someip_protocol::Writer writer{buffer_view};
          service_discovery::SerializeControlMessageRequestServiceResponsePayload(writer, client_id);
        } else if (request_service_result.CheckError(SomeIpDaemonErrc::access_denied)) {
          logger_.LogError([](ara::log::LogStream& s) { s << "Failed to request the service. IAM access denied."; },
                           static_cast<char const*>(__func__), __LINE__);
          result = ControlMessageReturnCode::kRequestServiceAccessDenied;
        } else if (request_service_result.CheckError(SomeIpDaemonErrc::client_ids_overflow)) {
          logger_.LogError(
              [](ara::log::LogStream& s) {
                s << "Failed to request the service. Client Id generator reaches the maximum number of simultaneous "
                     "clients.";
              },
              static_cast<char const*>(__func__), __LINE__);
          result = ControlMessageReturnCode::kRequestServiceClientIdsOverflow;
        } else if (request_service_result.CheckError(SomeIpDaemonErrc::remote_server_not_found)) {
          logger_.LogError(
              [](ara::log::LogStream& s) { s << "Failed to request the service. No remote server is found."; },
              static_cast<char const*>(__func__), __LINE__);
          result = ControlMessageReturnCode::kRequestServiceRemoteServerNotFound;
        } else if (request_service_result.CheckError(SomeIpDaemonErrc::required_service_instance_not_found)) {
          logger_.LogError(
              [](ara::log::LogStream& s) {
                s << "Failed to request the service. No required service instance is found.";
              },
              static_cast<char const*>(__func__), __LINE__);
          result = ControlMessageReturnCode::kRequestServiceRequiredServiceInstanceNotFound;
        } else {
          // Nothing to do (avoid bauhaus violation).
          static_assert(true, "");
        }
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ControlMessageReturnCode::kMalformedMessage;
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
      result = ControlMessageReturnCode::kMalformedMessage;
    }
    return result;
  }

  /*!
   * \brief "StartServiceDiscovery" control message handler.
   *
   * \param[in] payload_buffer memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return return code: "kOk" if deserialization succeeds and the service is successfully requested, otherwise an
   *         error code.
   *
   * \internal
   * - If provided payload is verified,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Request the service.
   *   - Else
   *     - Log an error message.
   *     - Set the return code to indicate a malformed message.
   * - Else
   *   - Log an error message.
   *   - Set the return code to indicate a malformed message.
   * - Return the result.
   * \endinternal
   */
  auto StartServiceDiscoveryHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept
      -> ControlMessageReturnCode {
    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};

    // Verify payload.
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Prepare reader.
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      ::amsr::someip_protocol::internal::deserialization::Reader reader{buffer_view};

      ara::core::Optional<ServiceDiscoveryMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeServiceInstanceIdentificationData(
              reader)};

      // Verify deserialized payload.
      if (request.has_value()) {
        result = application_commands_wrapper_.StartServiceDiscovery(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ControlMessageReturnCode::kMalformedMessage;
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
      result = ControlMessageReturnCode::kMalformedMessage;
    }

    return result;
  }

  /*!
   * \brief StopServiceDiscovery control message handler.
   *
   * \param[in] payload_buffer memory buffer containing control message request payload.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return return code: "kOk" if deserialization succeeds and the service is successfully released, otherwise an
   *         error code.
   *
   * \internal
   * - If provided payload is verified,
   *   - Deserialize the control message.
   *   - If the deserialized payload is verified,
   *     - Release the service.
   *   - Else
   *     - Log an error message.
   *     - Set the return code to indicate a malformed message.
   * - Else
   *   - Log an error message.
   *   - Set the return code to indicate a malformed message.
   * - Return the result.
   * \endinternal
   */
  auto StopServiceDiscoveryHandler(
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> payload_buffer) noexcept
      -> ControlMessageReturnCode {
    ControlMessageReturnCode result{ControlMessageReturnCode::kOk};
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    // Verify payload
    if ((payload_buffer != nullptr) && (payload_buffer->size() > 0)) {
      // Deserialize Payload
      MemoryBufferView packet_view{payload_buffer->GetView(0U)};
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      Reader::BufferView const buffer_view{static_cast<Reader::BufferView::pointer>(packet_view[0U].base_pointer),
                                           packet_view[0U].size};
      Reader reader{buffer_view};
      ara::core::Optional<ServiceDiscoveryMessagePayload> request{
          amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeServiceInstanceIdentificationData(
              reader)};

      // Verify deserialized payload
      if (request.has_value()) {
        result = application_commands_wrapper_.StopServiceDiscovery(request.value());
      } else {
        logger_.LogError(
            [received_length = payload_buffer->size()](ara::log::LogStream& s) {
              s << kErrorMsgRequestCommandUnexpectedPayloadLength << ". Expected length: "
                << amsr::someipd_app_protocol::internal::kServiceInstanceIdentificationDataLength
                << ", received length: " << received_length;
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ControlMessageReturnCode::kMalformedMessage;
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << kErrorMsgRequestCommandNullPayload; },
                       static_cast<char const*>(__func__), __LINE__);
      result = ControlMessageReturnCode::kMalformedMessage;
    }

    return result;
  }

  /*!
   * \brief Called once a control message is received.
   *
   * \param[in] message_type Received control message type.
   * \param[in] control_header Control header to be validated.
   * \param[in] payload_buffer A memory buffer containing the received control message.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return return code: "kOk" if the control message header is valid and the handler deserialization is ok, otherwise
   * an error code.
   *
   * \internal
   * - If valid control header,
   *   - Forward the message to the corresponding handler.
   * - Else
   *   - Log an error message.
   * \endinternal
   */
  auto ValidateControlHeaderAndForwardToHandler(
      amsr::someipd_app_protocol::internal::MessageType const& message_type,
      ara::core::Optional<amsr::someipd_app_protocol::internal::ControlMessageHeader> control_header,
      vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer>& payload_buffer)
      -> ControlMessageReturnCode {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    ControlMessageReturnCode return_code{ControlMessageReturnCode::kNotOk};

    if (control_header.has_value() && (control_header->return_code == ControlMessageReturnCode::kOk)) {
      // Forward the message to the corresponding handler.
      // The handler will deserialize the payload, do the required logic, serialize the response payload and return the
      // return code.
      using amsr::someipd_app_protocol::internal::MessageType;
      ara::core::Map<MessageType, vac::language::UniqueFunction<void()>> function_map{};
      // Add kReleaseService action
      static_cast<void>(function_map.emplace(MessageType::kReleaseService, [this, &return_code, &payload_buffer] {
        return_code = ReleaseServiceHandler(std::move(payload_buffer));
      }));
      // Add kRequestLocalServer action
      static_cast<void>(function_map.emplace(MessageType::kRequestLocalServer, [this, &return_code, &payload_buffer] {
        return_code = RequestLocalServerHandler(std::move(payload_buffer));
      }));

      // Add kReleaseLocalServer action
      static_cast<void>(function_map.emplace(MessageType::kReleaseLocalServer, [this, &payload_buffer] {
        ReleaseLocalServerHandler(std::move(payload_buffer));
      }));
      // Add kOfferService action
      static_cast<void>(function_map.emplace(
          MessageType::kOfferService, [this, &payload_buffer] { OfferServiceHandler(std::move(payload_buffer)); }));
      // Add kStopOfferService action
      static_cast<void>(function_map.emplace(MessageType::kStopOfferService, [this, &payload_buffer] {
        StopOfferServiceHandler(std::move(payload_buffer));
      }));
      // Add kSubscribeEvent action
      static_cast<void>(function_map.emplace(MessageType::kSubscribeEvent, [this, &return_code, &payload_buffer] {
        return_code = SubscribeEventHandler(std::move(payload_buffer));
      }));
      // Add kUnsubscribeEvent action
      static_cast<void>(function_map.emplace(MessageType::kUnsubscribeEvent, [this, &return_code, &payload_buffer] {
        return_code = UnsubscribeEventHandler(std::move(payload_buffer));
      }));
      // Add kRequestService action
      static_cast<void>(function_map.emplace(MessageType::kRequestService, [this, &return_code, &payload_buffer] {
        return_code = RequestServiceHandler(std::move(payload_buffer));
      }));
      // Add kStartServiceDiscovery action
      static_cast<void>(
          function_map.emplace(MessageType::kStartServiceDiscovery, [this, &return_code, &payload_buffer] {
            return_code = StartServiceDiscoveryHandler(std::move(payload_buffer));
          }));
      // Add kStopServiceDiscovery action
      static_cast<void>(function_map.emplace(MessageType::kStopServiceDiscovery, [this, &return_code, &payload_buffer] {
        return_code = StopServiceDiscoveryHandler(std::move(payload_buffer));
      }));

      auto const iter = function_map.find(message_type);

      if (iter != function_map.end()) {
        iter->second();
      } else {
        logger_.LogError(
            [&message_type](ara::log::LogStream& s) {
              s << "Unknown control message type 0x"
                << ara::log::HexFormat(static_cast<MessageTypeUnderlyingType>(message_type));
            },
            static_cast<char const*>(__func__), __LINE__);
        return_code = ControlMessageReturnCode::kUnknownMessageType;
      }

    } else {
      if (control_header.has_value()) {
        logger_.LogError(
            [unknown_ret_code = control_header.value().return_code](ara::log::LogStream& s) {
              s << "Unexpected return code for command request: 0x"
                << ara::log::HexFormat(static_cast<ControlMessageReturnCodeUnderlyingType>(unknown_ret_code));
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "Unexpected return code for command request: Header deserialization failed.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      return_code = ControlMessageReturnCode::kMalformedMessage;
    }
    return return_code;
  }

  /*!
   * \brief Sends message response to SomeIpDaemonClient after ValidateControlHeaderAndForwardToHandler has been called.
   *
   * \param[in] message_type Received control message type.
   * \param[in] validate_control_header_return_code Return code from ValidateControlHeaderAndForwardToHandler.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - For message types where SomeIpDaemonClient expects an answer,
   *   - Serialize response control header.
   *   - Determine response message type.
   *   - Send response.
   * \endinternal
   */
  auto SendResponseToSomeIpDaemonClient(amsr::someipd_app_protocol::internal::MessageType const& message_type,
                                        ControlMessageReturnCode const& validate_control_header_return_code) -> void {
    // Send response when someipdaemonclient expects an answer. (Skip when fire and forget)
    if ((message_type != amsr::someipd_app_protocol::internal::MessageType::kReleaseService) &&
        (message_type != amsr::someipd_app_protocol::internal::MessageType::kReleaseLocalServer) &&
        (message_type != amsr::someipd_app_protocol::internal::MessageType::kOfferService) &&
        (message_type != amsr::someipd_app_protocol::internal::MessageType::kStopOfferService) &&
        (message_type != amsr::someipd_app_protocol::internal::MessageType::kSubscribeEvent) &&
        (message_type != amsr::someipd_app_protocol::internal::MessageType::kUnsubscribeEvent) &&
        (message_type != amsr::someipd_app_protocol::internal::MessageType::kStartServiceDiscovery) &&
        (message_type != amsr::someipd_app_protocol::internal::MessageType::kStopServiceDiscovery)) {
      someipd_app_protocol::someip_protocol::BufferView const buffer_view{transmit_control_header_};
      someipd_app_protocol::someip_protocol::Writer writer{buffer_view};

      someipd_app_protocol::SerializeSpecificIpcPacketHeaderCommandMessage(writer, validate_control_header_return_code);

      ControlHeaderView const control_header_view{transmit_control_header_.data(), transmit_control_header_.size()};

      // Determine response message type.
      amsr::someipd_app_protocol::internal::MessageType response_message_type{};
      if (validate_control_header_return_code == ControlMessageReturnCode::kOk) {
        response_message_type =
            amsr::someipd_app_protocol::internal::GetPositiveResponseMessageTypeFromRequestMessageType(message_type);
      } else {
        response_message_type =
            amsr::someipd_app_protocol::internal::GetNegativeResponseMessageTypeFromRequestMessageType(message_type);
      }

      // Send response.
      static_cast<void>(
          application_connection_.SendMessage(response_message_type, control_header_view, std::move(response_packet_)));
    }
  }

  // ---- Control logic ------------------------------------------------------------------------------------

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kApplicationLoggerContextId,
                                                    someip_daemon_core::logging::kApplicationLoggerContextDescription,
                                                    ara::core::StringView{"CommandController"}};

  /*!
   * \brief Application commands wrapper class.
   */
  ApplicationCommandsWrapperType application_commands_wrapper_;

  /*!
   * \brief Reference to the application connection.
   */
  ApplicationConnection& application_connection_;

  /*!
   * \brief Allocator for transmitted control packets.
   */
  someip_daemon_core::MemoryBufferAllocator tx_buffer_allocator_{};

  /*!
   * \brief A buffer for the control message header of transmitted control messages.
   */
  std::array<std::uint8_t, amsr::someipd_app_protocol::internal::kControlMessageHeaderLength>
      transmit_control_header_{};

  // VECTOR NC AutosarC++17_10-A8.5.2: MD_SomeIpDaemon_AutosarC++17_10-A8.5.2_falsepositive
  /*!
   * \brief A buffer for the control message header of response messages.
   */
  vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> response_packet_{};

  /*!
   * \brief An instance of the DeserMessageHeaders class.
   */
  someipd_app_protocol::DeserMessageHeaders deserializer_{};
};

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_COMMAND_CONTROLLER_H_

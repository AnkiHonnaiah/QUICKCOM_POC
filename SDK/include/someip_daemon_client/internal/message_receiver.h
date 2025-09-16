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
/*!        \file  someip_daemon_client/internal/message_receiver.h
 *        \brief  Message receiver operations for the Ipc connections.
 *      \details  This file contains the declarations for
 *                receiving SOME/IP messages and control commands.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_MESSAGE_RECEIVER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_MESSAGE_RECEIVER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ios>
#include <type_traits>
#include <utility>
#include "someip-protocol/internal/marshalling.h"
#include "someip_daemon_client/internal/common_types.h"
#include "someip_daemon_client/internal/controller_interface.h"
#include "someipd_app_protocol/internal/deser_message_headers.h"
#include "someipd_app_protocol/internal/pdu_message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

namespace details {
/*!
 * \brief Enumeration describing possible deserialization errors.
 */
enum class HeaderDeserializationError : std::uint8_t {
  /*!
   * \brief Wrong protocol version detected.
   */
  kWrongProtocolVersion,

  /*!
   * \brief An unexpected message type received.
   */
  kUnexpectedMessageType
};
}  // namespace details

/*!
 * \brief A message receiver class that makes use of the IPC wrapper to provide functionalities for receiving all
 * different types of messages between SOME/IP daemon and application.
 * \tparam TemplateConfiguration The type of a class used for message forwarding and event notification.
 *         Contains the following required type definition:
 *         - typename TemplateConfiguration::RoutingControllerType
 */
// VECTOR NC Metric-OO.WMC.One: MD_SOMEIPDAEMONCLIENT_Metric-OO.WMC.One_methods_per_class
template <typename TemplateConfiguration>
class MessageReceiver {
 public:
  /*!
   * \brief Type alias for IPC protocol specific header view.
   */
  using SpecificHeaderView = amsr::someipd_app_protocol::internal::SpecificHeaderView;

  /*!
   * \brief Type alias for Routing Controller.
   */
  using ReceiveRoutingController = typename TemplateConfiguration::RoutingControllerType;

  /*!
   * \brief     Initialize all the resources of MessageReceiver.
   * \param[in] receive_message_allocator Memory buffer allocator for received messages.
   * \param[in] receive_routing_controller Routing controller for received messages.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  MessageReceiver(vac::memory::allocator::MemoryBufferAllocator& receive_message_allocator,
                  ReceiveRoutingController& receive_routing_controller)
      : receive_message_allocator_{receive_message_allocator},
        logger_{logging::kSomeIpLoggerContextId, logging::kSomeIpLoggerContextDescription,
                amsr::core::StringView{"MessageReceiver"}},
        reception_buffer_{},
        receive_routing_controller_{receive_routing_controller} {}

  MessageReceiver() = delete;

  MessageReceiver(MessageReceiver const&) = delete;

  MessageReceiver(MessageReceiver&&) = delete;

  auto operator=(MessageReceiver const&) & -> MessageReceiver& = delete;

  auto operator=(MessageReceiver&&) & -> MessageReceiver& = delete;

  /*!
   * \brief Destructor.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SDCL_SLC23_DEFAULT_DTOR_MOLE_1298
  virtual ~MessageReceiver() noexcept = default;

  /*!
   * \brief     Process the message just received after making sure it is big enough to accommodate the generic and
   *            the specific header.
   *
   * \pre       The message has been already received and saved in reception_buffer_.
   * \pre       The message is big enough to accommodate the generic and the specific header.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Read generic header.
   * - #10 Check if the generic header reading succeeded.
   *   - #20 Check if the message type is control response,
   *     - Call ProcessReceivedControlMessageResponse for further processing.
   *   - #21 Otherwise, if the message type is SOME/IP Routing or SOME/IP with meta data Routing or Initial field
   * notification.
   *     - Call ProcessReceivedSomeIpMessage() for further processing.
   *   - #22 Otherwise, if the message type is PDU Routing or the message type is PDU with meta data Routing,
   *     - Call ProcessReceivedPduMessage() for further processing.
   *   - #23 Otherwise, if valid initial field notification routing message is received,
   *     - Call ProcessReceivedInitialFieldNotificationSomeIpMessage() for further processing.
   *   - #24 Otherwise, if valid Non SOME/IP routing message is received,
   *       - Call ProcessNonSomeIpMessage() to process the received message.
   *   - #25 Otherwise, log that the message is inconsistent.
   * - #11 Otherwise, log an error that the generic header is inconsistent.
   * - Reset the buffer for received message body for the next reception.
   * \endinternal
   */
  void ProcessReceivedMessage() noexcept {
    // PTP-B-SomeipDaemonClient-MessageReceiver_ProcessReceivedMessage
    // Read generic header
    PacketBufferView generic_header_view{reception_buffer_.receive_generic_header};
    amsr::core::Result<someipd_app_protocol::internal::GenericMessageHeader, HeaderDeserializationError> const
        generic_header_result{ReadGenericIpcPacketHeader(generic_header_view)};

    if (generic_header_result.HasValue()) {  // #10
      someipd_app_protocol::internal::GenericMessageHeader const& generic_header{generic_header_result.Value()};

      if (someipd_app_protocol::internal::IsControlMessageResponse(generic_header.message_type)) {  // #20
        // Control Response
        SpecificHeaderView const specific_header_view{reception_buffer_.receive_specific_header};
        ProcessReceivedControlMessageResponse(generic_header, specific_header_view, reception_buffer_);
      } else if (someipd_app_protocol::internal::IsRoutingSomeIpMessage(generic_header.message_type) ||
                 someipd_app_protocol::internal::IsRoutingSomeIpMessageWithMetaData(
                     generic_header.message_type)) {  // #21
        ProcessReceivedSomeIpMessage(generic_header);
      } else if (someipd_app_protocol::internal::IsRoutingPduMessage(generic_header.message_type) ||
                 someipd_app_protocol::internal::IsRoutingPduMessageWithMetaData(generic_header.message_type)) {  // #22
        ProcessReceivedPduMessage(generic_header);
      } else if (reception_buffer_.receive_message_body) {
        if (someipd_app_protocol::internal::IsRoutingInitialFieldNotificationMessage(
                generic_header.message_type)) {  // #23
          // Routing message: InitialFieldNotification
          ProcessReceivedInitialFieldNotificationSomeIpMessage();
        } else {  // # 24
          // Other routing messages: {Event Subscription State Update, Service Update)
          // clang-format off
          // NOLINTNEXTLINE(whitespace/line_length)
          SpecificHeaderView const specific_header_view{reception_buffer_.receive_message_body->GetView(0U)};  // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
          // clang-format on
          // VCA_SDCL_PROCESS_NON_SOMEIP_MESSAGE
          receive_routing_controller_.ProcessNonSomeIpMessage(generic_header.message_type, specific_header_view);
        }
      } else {  // #25
        // Inconsistent Message, drop.
        logger_.LogVerbose([](ara::log::LogStream& s) { s << "Inconsistent Message received. Message dropped."; },
                           {__func__}, {__LINE__});
      }
    } else {  // #11
      // Inconsistent Generic Header, drop.
      logger_.LogError([](ara::log::LogStream& s) { s << "Inconsistent Generic Header received. Message dropped."; },
                       {__func__}, {__LINE__});
    }

    // Clear previously received data.
    // VCA_SDCL_LNG03_OBJECT_IS_VALID
    ResetReceptionBuffer();

    // PTP-E-SomeipDaemonClient-MessageReceiver_ProcessReceivedMessage
  }

  /*!
   * \brief       Sets up a new memory buffer for an incoming IPC header message.
   *
   * \param[in]   message_length The total length of the message expected next.
   * \return      A view to the I/O vector container pointing to the memory region(s) for incoming IPC header message.
   * \pre         message_length >= kHeaderLength
   * \context     Reactor
   * \reentrant   FALSE
   * \steady TRUE
   *
   * \internal
   *  - Reset the current reception buffer, update reception state.
   *  - Return a view that fits an IPC header, the generic and specific header.
   * \endinternal
   */
  auto PrepareReceiveHeaderMemoryBuffer(std::size_t const message_length) noexcept -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_PrepareReceiveHeaderMemoryBuffer
    assert(message_length >= kHeaderLength);

    // Clear previously received data.
    ResetReceptionBuffer();

    // Update reception state.
    chunk_reception_state_ = ChunkReceptionState::kIpcHeader;

    // Setup view that match the size of the header.
    MutableIOBufferContainerView const view{MutableIOBufferContainerView{
        reception_buffer_.receive_container_ipc_header.data(), reception_buffer_.receive_container_ipc_header.size()}};
    // PTP-E-SomeipDaemonClient-MessageReceiver_PrepareReceiveHeaderMemoryBuffer
    return view;
  }

  /*!
   * \brief       Process received message chunk and return a view for next message chunk.
   *
   * \param[in]   remaining_message_length The remaining length of the message.
   * \return      A view to the I/O vector container pointing to the memory region(s) for incoming message chunk.
   *              When remaining_message_length is 0 or an error occurs, an empty view is returned.
   * \pre
   * \context     Reactor
   * \reentrant   FALSE
   * \steady TRUE
   *
   *
   * \internal
   * - Process received chunks, and return view for next chunk when needed:
   *   - If IPC header received: Parse header and prepare view for next chunk.
   *   - Else if SOME/IP header received: Parse and prepare view for SOME/IP payload if needed.
   *   - Else if PDU header received: Parse and prepare view for PDU payload if needed.
   *   - Else if chunk received before "OnMessageAvailable", log error and reset buffers.
   *   - Else, verify that chunk reception is complete. On error log and reset buffers.
   * - Return view for next chunk when more data is expected.
   * \endinternal
   */
  auto ProcessChunkAndPrepareReceiveMemoryBuffer(std::size_t const remaining_message_length) noexcept
      -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_ProcessChunkAndPrepareReceiveMemoryBuffer
    MutableIOBufferContainerView view;
    logger_.LogVerbose(
        [&remaining_message_length](ara::log::LogStream& s) {
          s << "Process received chunk. Remaining length: " << remaining_message_length;
        },
        {__func__}, {__LINE__});

    // Processing received chunks:
    if (chunk_reception_state_ == ChunkReceptionState::kIpcHeader) {
      // Handle IPC header chunk, prepare view for next expected chunk.
      view = ProcessReceivedHeader(remaining_message_length);
    } else if (chunk_reception_state_ == ChunkReceptionState::kSomeIpHeader) {
      // Handle received chunk and prepare view for expected SOME/IP chunk.
      view = PrepareReceiveSomeIpPayload(remaining_message_length);
    } else if (chunk_reception_state_ == ChunkReceptionState::kPduHeader) {
      // Handle received chunk and prepare view for expected PDU chunk.
      view = PrepareReceivePduPayload(remaining_message_length);
    } else if (chunk_reception_state_ == ChunkReceptionState::kReceptionStart) {
      // Handle when chunk received before on message available have been signalled from IPC.
      logger_.LogError([](ara::log::LogStream& s) { s << "Chunk received in unexpected state."; }, {__func__},
                       {__LINE__});
      ResetReceptionBuffer();
    } else {
      // Verify chunk receive completed, empty view will be returned.
      if (remaining_message_length != 0U) {
        logger_.LogError(
            [&remaining_message_length](ara::log::LogStream& s) {
              s << "Unexpected remaining chunk, reset reception. Size =  " << remaining_message_length;
            },
            {__func__}, {__LINE__});
        ResetReceptionBuffer();
      } else {
        chunk_reception_state_ = ChunkReceptionState::kReceptionComplete;
      }
    }

    // PTP-E-SomeipDaemonClient-MessageReceiver_ProcessChunkAndPrepareReceiveMemoryBuffer
    return view;
  }

  /*!
   * \brief     Register the controllers in message receiver to handle the message responses.
   *
   * \param     message_type MessageType of the control messages.
   * \param     controller_handle Controllers of the control messages.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Check if the controller to register is of type kRequestService or kRequestLocalServer.
   *  - If no controller of the message type has been registered, register controller handle.
   *  - If a controller of the message type has been registered, abort execution.
   * - If the message type is neither kRequestService nor kRequestLocalServer, abort execution.
   * \endinternal
   */
  void RegisterControllerHandle(someipd_app_protocol::internal::MessageType const message_type,
                                ControllerInterface* controller_handle) noexcept {
    if ((message_type == someipd_app_protocol::internal::MessageType::kRequestService) ||
        (message_type == someipd_app_protocol::internal::MessageType::kRequestLocalServer)) {
      if ((message_type == someipd_app_protocol::internal::MessageType::kRequestService) &&
          (request_service_controller_ == nullptr)) {
        request_service_controller_ = controller_handle;
      } else if ((message_type == someipd_app_protocol::internal::MessageType::kRequestLocalServer) &&
                 (request_local_server_controller_ == nullptr)) {
        request_local_server_controller_ = controller_handle;
      } else {
        logger_.LogFatalAndAbort(
            [&message_type](logging::AraComLogger::StringStream& s) {
              s << "Registering two controller handles of same message type is not allowed (Message Type: 0x";  // VCA_SDCL_STRING_STREAM_USAGE
              s << std::hex                                              // VCA_SDCL_STRING_STREAM_USAGE
                << static_cast<MessageTypeUnderlyingType>(message_type)  // VCA_SDCL_STRING_STREAM_USAGE
                << ").";                                                 // VCA_SDCL_STRING_STREAM_USAGE
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogFatalAndAbort(
          [&message_type](logging::AraComLogger::StringStream& s) {
            s << "Registering controller handle of this message type is not allowed (Message Type: 0x";  // VCA_SDCL_STRING_STREAM_USAGE
            s << std::hex                                              // VCA_SDCL_STRING_STREAM_USAGE
              << static_cast<MessageTypeUnderlyingType>(message_type)  // VCA_SDCL_STRING_STREAM_USAGE
              << ").";                                                 // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

 protected:
  /*!
   * \brief Type for Error codes from Header deserialization.
   */
  using HeaderDeserializationError = details::HeaderDeserializationError;

  /*!
   * \brief The header length is the sum of the generic header length and the specific header length.
   */
  static std::size_t constexpr kHeaderLength{someipd_app_protocol::internal::kGenericMessageHeaderLength +
                                             someipd_app_protocol::internal::kSpecificMessageHeaderLength};

  /*!
   * \brief Packet buffer view type.
   */
  using PacketBufferView = ::amsr::someip_protocol::internal::PacketBufferView;

  // ---- Utilities for message reception ---------------------------------------------------------

  /*!
   * \brief     Deserialize received generic header and validates its contents.
   * \param[in] generic_header_view View of generic header buffer.
   * \return    Result containing the deserialized generic header or an error if validation fails.
   *
   * \pre       The passed buffer has at least the size of the generic header.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Initialize the result to HeaderDeserializationError::kWrongProtocolVersion.
   * - Deserialize generic header.
   *   - Check if the protocol version is supported.
   *     - Check if the message type is control message or routing message,
   *       - Emplace the generic header to the result.
   *     - Otherwise, Set the result to kUnexpectedMessageType, and log an error that receiving message with
   *       unexpected type.
   *   - Otherwise, log an error that receiving message with invalid protocol version.
   * - Return the result.
   * \endinternal
   */
  auto ReadGenericIpcPacketHeader(PacketBufferView const& generic_header_view) const
      -> amsr::core::Result<someipd_app_protocol::internal::GenericMessageHeader, HeaderDeserializationError> {
    amsr::core::Result<someipd_app_protocol::internal::GenericMessageHeader, HeaderDeserializationError> result{
        HeaderDeserializationError::kWrongProtocolVersion};

    // Deserialize generic header
    amsr::someipd_app_protocol::internal::Reader reader{generic_header_view};

    amsr::core::Optional<amsr::someipd_app_protocol::internal::GenericMessageHeader> const generic_header{
        deserializer_.DeserializeGenericIpcPacketHeader(reader)};
    // Deserialization should never fail here as the input buffer is an array of
    // the required size.
    assert(generic_header.has_value());
    // Validate all fields
    if (generic_header->protocol_version == amsr::someipd_app_protocol::internal::kProtocolVersion) {
      if (someipd_app_protocol::internal::IsControlMessageResponse(generic_header->message_type) ||
          someipd_app_protocol::internal::IsRoutingMessage(generic_header->message_type)) {
        result.EmplaceValue(generic_header.value());
      } else {
        result.EmplaceError(HeaderDeserializationError::kUnexpectedMessageType);
        logger_.LogError(
            [msg_type = generic_header.value().message_type](ara::log::LogStream& s) {
              s << "Received message with unexpected type " << static_cast<std::size_t>(msg_type);
            },
            {__func__}, {__LINE__});
      }
    } else {
      logger_.LogError(
          [protocol_version = generic_header.value().protocol_version](ara::log::LogStream& s) {
            s << "Received message with invalid protocol version " << protocol_version << ", expected "
              << someipd_app_protocol::internal::kProtocolVersion;
          },
          {__func__}, {__LINE__});
    }
    return result;
  }

  /*!
   * \brief     Deserialize routing SOME/IP specific Header (Contains instance ID).
   * \param[in] specific_header_view View of specific header buffer.
   * \return    The deserialized routing SOME/IP specific Header.
   * \pre       specific_header_view length is enough to hold routing SOME/IP Header.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Create the deserializer with the passed specific header view.
   * - Deserialize and return the routing SOME/IP specific header.
   * \endinternal
   */
  static auto ReadRoutingSomeIpSpecificIpcPacketHeader(SpecificHeaderView const& specific_header_view) noexcept
      -> amsr::someipd_app_protocol::internal::RoutingSomeIpMessageHeader {
    amsr::someipd_app_protocol::internal::Reader reader{specific_header_view};
    amsr::someipd_app_protocol::internal::RoutingSomeIpMessageHeader routing_someip_header{};

    amsr::core::Optional<amsr::someipd_app_protocol::internal::RoutingSomeIpMessageHeader> result{
        amsr::someipd_app_protocol::internal::DeserMessageHeaders::
            DeserializeSpecificIpcPacketHeaderRoutingSomeIpMessage(reader)};
    // Deserialization should never fail here as the input buffer is an array of
    // the required size.
    assert(result.has_value());
    routing_someip_header = result.value();

    return routing_someip_header;
  }

  /*!
   * \brief     Deserialize routing PDU specific Header (Contains instance ID).
   * \param[in] specific_header_view View of specific header buffer.
   * \return    The deserialized routing PDU specific Header.
   * \pre       specific_header_view length is enough to hold routing PDU specific Header.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Create the deserializer with the passed specific header view.
   * - Deserialize and return the routing PDU specific header.
   * \endinternal
   */
  static auto ReadRoutingPduSpecificIpcPacketHeader(SpecificHeaderView const& specific_header_view) noexcept
      -> amsr::someipd_app_protocol::internal::RoutingPduMessageHeader {
    amsr::someipd_app_protocol::internal::Reader reader{specific_header_view};
    amsr::someipd_app_protocol::internal::RoutingPduMessageHeader routing_pdu_header{};

    amsr::core::Optional<amsr::someipd_app_protocol::internal::RoutingPduMessageHeader> result{
        amsr::someipd_app_protocol::internal::DeserMessageHeaders::DeserializeSpecificIpcPacketHeaderRoutingPduMessage(
            reader)};
    // Deserialization should never fail here as the input buffer is an array of
    // the required size.
    assert(result.has_value());
    routing_pdu_header = result.value();

    return routing_pdu_header;
  }

  /*!
   * \brief     Deserialize routing notification specific Header (Contains instance and client IDs).
   * \param[in] specific_header_view View of specific header buffer.
   * \return    The deserialized routing notification specific Header.
   * \pre       specific_header_view length is enough to hold routing notification header.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Create the deserializer with the passed specific header view.
   * - Deserialize and return the routing notification specific header.
   * \endinternal
   */
  static auto ReadRoutingNotificationIpcPacketHeader(SpecificHeaderView const& specific_header_view)
      -> amsr::someipd_app_protocol::internal::RoutingNotificationMessageHeader {
    amsr::someipd_app_protocol::internal::Reader reader{specific_header_view};
    amsr::someipd_app_protocol::internal::RoutingNotificationMessageHeader routing_notification_header{};

    amsr::core::Optional<amsr::someipd_app_protocol::internal::RoutingNotificationMessageHeader> result{
        amsr::someipd_app_protocol::internal::DeserMessageHeaders::
            DeserializeSpecificIpcPacketHeaderRoutingNotificationMessage(reader)};
    // Deserialization should never fail here as the input buffer is an array of the required size.
    assert(result.has_value());
    routing_notification_header = result.value();

    return routing_notification_header;
  }

 private:
  /*!
   * \brief     Prepare reception payload views for the message body.
   *
   * \param[in] headersize The size of the header.
   * \return    A view to the I/O vector container pointing to the memory region(s) for reception of the rest of the
   *            message payload.
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Setup view for message body payload, return view.
   * \endinternal
   */
  auto PrepareReceivePayloadView(std::size_t const headersize) -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_PrepareReceivePayloadView
    vac::memory::allocator::MemoryBuffer::MemoryBufferView const receive_message_body_view{
        // VCA_SDCL_SLC10_SLC22_OBJECT_REF_VALIDITY_IS_A_FUNCTION_PRECONDITION
        reception_buffer_.receive_message_body->GetView(0U)};
    vac::memory::allocator::MemoryBuffer::MemoryBufferView const
        // VCA_SDCL_SLC10_SLC22_OBJECT_REF_VALIDITY_IS_A_FUNCTION_PRECONDITION
        receive_message_body_payload_view{reception_buffer_.receive_message_body->GetView(headersize)};

    osabstraction::io::MutableIOBuffer const receive_message_body_buffer{receive_message_body_view.data(),
                                                                         receive_message_body_view.size()};
    osabstraction::io::MutableIOBuffer const receive_message_body_payload_buffer{
        receive_message_body_payload_view.data(), receive_message_body_payload_view.size()};

    reception_buffer_.receive_buffer_container_body[0U] = {receive_message_body_buffer.base_pointer,
                                                           receive_message_body_buffer.size};
    reception_buffer_.receive_buffer_container_body_payload[0U] = {receive_message_body_payload_buffer.base_pointer,
                                                                   receive_message_body_payload_buffer.size};

    MutableIOBufferContainerView const view{
        MutableIOBufferContainerView{reception_buffer_.receive_buffer_container_body_payload.data(),
                                     reception_buffer_.receive_buffer_container_body_payload.size()}};
    // PTP-E-SomeipDaemonClient-MessageReceiver_PrepareReceivePayloadView
    return view;
  }

  /*!
   * \brief     Prepare reception buffers for SOME/IP payload data if any.
   *
   * \param[in] remaining_message_length The remaining length of the message.
   * \return    A view to the I/O vector container pointing to the memory region(s) for reception of the rest of the
   *            SOME/IP payload. Otherwise return an empty view.
   * \pre       The message header has been already received and is saved in reception_buffer_.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Allocate memory for message body, SOME/IP header and payload size.
   * - Copy the already received SOME/IP header into allocated message body.
   *   - If more data to receive, setup view for SOME/IP message body payload, return view.
   * - Otherwise return empty buffer.
   * \endinternal
   */
  auto PrepareReceiveSomeIpPayload(std::size_t const remaining_message_length) -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_PrepareReceiveSomeIpPayload
    MutableIOBufferContainerView view{};
    std::size_t const headersize{amsr::someip_protocol::internal::kHeaderSize};

    // Try to allocate required size, with room for already received header
    amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> buffer_result{
        // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
        receive_message_allocator_.Allocate(remaining_message_length + headersize)};

    if (buffer_result.HasValue()) {
      // VCA_SDCL_MOVE_ASSIGNMENT
      reception_buffer_.receive_message_body = std::move(buffer_result.Value());
    } else {
      logger_.LogFatal([](ara::log::LogStream& s) { s << "Memory allocation failed for SOME/IP payload"; }, {__func__},
                       {__LINE__});
      amsr::core::Abort("Memory allocation failed for SOME/IP payload.");
    }

    // Copy in already received SOME/IP header into body buffer.
    // VCA_SDCL_SLC10_SLC22_SLC11_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
    std::memcpy(reception_buffer_.receive_message_body->GetView(0U).data(),
                reception_buffer_.receive_someip_header.data(), headersize);

    // If any SOME/IP payload to be received, setup view for payload body.
    if (remaining_message_length > 0U) {
      view = PrepareReceivePayloadView(headersize);

      // Update chunk reception state.
      chunk_reception_state_ = ChunkReceptionState::kSomeIpPayload;
    } else {
      // Update chunk reception state.
      chunk_reception_state_ = ChunkReceptionState::kReceptionComplete;
    }

    // PTP-E-SomeipDaemonClient-MessageReceiver_PrepareReceiveSomeIpPayload
    return view;
  }

  /*!
   * \brief     Prepare reception buffers for PDU payload data if any.
   *
   * \param[in] remaining_message_length The remaining length of the message.
   * \return    A view to the I/O vector container pointing to the memory region(s) for reception of the rest of the
   *            PDU payload. Otherwise return an empty view.
   * \pre       The message header has been already received and is saved in reception_buffer_.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Allocate memory for message body, PDU header and payload size.
   * - Copy the already received PDU header into allocated message body.
   *   - If more data to receive, setup view for PDU message body payload, return view.
   * - Otherwise return empty buffer.
   * \endinternal
   */
  auto PrepareReceivePduPayload(std::size_t const remaining_message_length) -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_PrepareReceivePduPayload
    MutableIOBufferContainerView view{};
    std::size_t const headersize{amsr::someip_protocol::internal::kPduHeaderSize};
    // Try to allocate required size, with room for already received header
    amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> buffer_result{
        // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
        receive_message_allocator_.Allocate(remaining_message_length + headersize)};

    if (buffer_result.HasValue()) {
      // VCA_SDCL_MOVE_ASSIGNMENT
      reception_buffer_.receive_message_body = std::move(buffer_result.Value());
    } else {
      logger_.LogFatal([](ara::log::LogStream& s) { s << "Memory allocation failed for PDU payload."; }, {__func__},
                       {__LINE__});
      amsr::core::Abort("Memory allocation failed for PDU payload..");
    }

    // Copy in already received SOME/IP header into body buffer.
    // VCA_SDCL_SLC10_SLC22_SLC11_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
    std::memcpy(reception_buffer_.receive_message_body->GetView(0U).data(), reception_buffer_.receive_pdu_header.data(),
                headersize);

    // If any PDU payload to be received, setup view for payload body.
    if (remaining_message_length > 0U) {
      view = PrepareReceivePayloadView(headersize);
      // Update chunk reception state.
      chunk_reception_state_ = ChunkReceptionState::kPduPayload;
    } else {
      // Update chunk reception state.
      chunk_reception_state_ = ChunkReceptionState::kReceptionComplete;
    }

    // PTP-E-SomeipDaemonClient-MessageReceiver_PrepareReceivePduPayload
    return view;
  }

  /*!
   * \brief     Clear received data.
   *
   * \context   Any
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Reset chunk reception state.
   * - Clear received data.
   * \endinternal
   */
  void ResetReceptionBuffer() {
    // PTP-B-SomeipDaemonClient-MessageReceiver_ResetReceptionBuffer
    chunk_reception_state_ = ChunkReceptionState::kReceptionStart;
    reception_buffer_.receive_buffer_container_body[0U] = {};
    reception_buffer_.receive_buffer_container_body_payload[0U] = {};
    if (reception_buffer_.receive_message_body) {
      reception_buffer_.receive_message_body.reset();  // VCA_SDCL_FUNCTION_NO_PRECONDITION
    }
    // PTP-E-SomeipDaemonClient-MessageReceiver_ResetReceptionBuffer
  }

  /*!
   * \brief     Process the received message header.
   *
   * \param[in] remaining_message_length The remaining length of the message.
   * \return    A view to the I/O vector container pointing to the memory region(s) for.
   *            On error an empty view is returned.
   * \pre       The message header has been already received and is saved in reception_buffer_.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Read generic header.
   * - Check if the generic header reading succeeded.
   *   - Check if incoming message is SOME/IP,
   *     - Prepare view for incoming SOME/IP chunks.
   *   - Check if incoming message is PDU,
   *     - Prepare view for incoming SOME/IP or PDU message chunks.
   *   - Otherwise,
   *     - Prepare view for incoming data.
   * - Otherwise, log inconsistent header error, reset reception.
   * \endinternal
   */
  auto ProcessReceivedHeader(std::size_t const remaining_message_length) -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_ProcessReceivedHeader
    MutableIOBufferContainerView view{};

    // Read generic header
    PacketBufferView generic_header_view{reception_buffer_.receive_generic_header};
    amsr::core::Result<someipd_app_protocol::internal::GenericMessageHeader, HeaderDeserializationError> const
        generic_header_result{ReadGenericIpcPacketHeader(generic_header_view)};
    if (generic_header_result.HasValue()) {
      someipd_app_protocol::internal::GenericMessageHeader const& generic_header{generic_header_result.Value()};
      if ((someipd_app_protocol::internal::IsRoutingSomeIpMessage(generic_header.message_type)) ||
          (someipd_app_protocol::internal::IsRoutingSomeIpMessageWithMetaData(generic_header.message_type))) {
        view = PrepareReceiveSomeIp(
            someipd_app_protocol::internal::IsRoutingSomeIpMessageWithMetaData(generic_header.message_type),
            remaining_message_length);
      } else if ((someipd_app_protocol::internal::IsRoutingPduMessage(generic_header.message_type)) ||
                 (someipd_app_protocol::internal::IsRoutingPduMessageWithMetaData(generic_header.message_type))) {
        view = PrepareReceivePdu(
            someipd_app_protocol::internal::IsRoutingPduMessageWithMetaData(generic_header.message_type),
            remaining_message_length);
      } else {
        view = PrepareReceiveData(remaining_message_length);
      }
    } else {
      // Inconsistent Generic Header, drop.
      logger_.LogError([](ara::log::LogStream& s) { s << "Inconsistent Generic Header received. Message dropped."; },
                       {__func__}, {__LINE__});
      // Receive reset.
      ResetReceptionBuffer();
    }
    // PTP-E-SomeipDaemonClient-MessageReceiver_ProcessReceivedHeader
    return view;
  }

  /*!
   * \brief     Prepare reception buffers for data.
   *
   * \param[in] remaining_message_length The remaining length of the message.
   * \return    A view to the I/O vector container pointing to the memory region(s) for reception of the rest of the
   *            data payload. Otherwise return an empty view.
   * \pre       The message header has been already received and is saved in reception_buffer_.
   * \context   Reactor
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - When more data is available,
   *   - Clear previously received data.
   *   - Allocate memory for data message body.
   *   - Return view for allocated data payload.
   * - Otherwise return an empty view.
   * \endinternal
   */
  auto PrepareReceiveData(std::size_t const remaining_message_length) -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_PrepareReceiveData
    MutableIOBufferContainerView view{};
    logger_.LogDebug(
        [&remaining_message_length](ara::log::LogStream& s) {
          s << "Setup buffers for data/command, remaining length: " << remaining_message_length;
        },
        {__func__}, {__LINE__});

    // Check if more data is available.
    if (remaining_message_length > 0U) {
      // Clear previously received data.
      ResetReceptionBuffer();

      // Try to allocate required size
      amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> buffer_result{
          // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
          receive_message_allocator_.Allocate(remaining_message_length)};

      if (buffer_result.HasValue()) {
        // VCA_SDCL_MOVE_ASSIGNMENT
        reception_buffer_.receive_message_body = std::move(buffer_result.Value());
      } else {
        logger_.LogFatal([](ara::log::LogStream& s) { s << "Memory allocation failed for data message body."; },
                         {__func__}, {__LINE__});
        amsr::core::Abort("Memory allocation failed for data message body.");
      }

      // Instantiate a I/O vector container with the entry for the message body/payload.
      vac::memory::allocator::MemoryBuffer::MemoryBufferView const receive_message_body_view{
          // VCA_SDCL_SLC10_SLC22_OBJECT_REF_VALIDITY_IS_A_FUNCTION_PRECONDITION
          reception_buffer_.receive_message_body->GetView(0U)};

      osabstraction::io::MutableIOBuffer const receive_message_body_buffer{receive_message_body_view.data(),
                                                                           receive_message_body_view.size()};
      reception_buffer_.receive_buffer_container_body[0U] = {receive_message_body_buffer.base_pointer,
                                                             receive_message_body_buffer.size};

      view = MutableIOBufferContainerView{reception_buffer_.receive_buffer_container_body.data(),
                                          reception_buffer_.receive_buffer_container_body.size()};

      // Update chunk reception state.
      chunk_reception_state_ = ChunkReceptionState::kCommandOrData;
    } else {
      // Update chunk reception state, reception complete.
      chunk_reception_state_ = ChunkReceptionState::kReceptionComplete;
    }
    // PTP-E-SomeipDaemonClient-MessageReceiver_PrepareReceiveData
    return view;
  }

  /*!
   * \brief     Prepare reception buffers for SOME/IP header and metadata, if expected.
   *
   * \param[in] has_metadata              True if message has metadata.
   * \param[in] remaining_message_length  The remaining length of the message.
   * \return    A view to the I/O vector container pointing to the memory region(s).
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Calculate minimal expected message length.
   * - If message length requirements are fulfilled,
   *   - Return view room for SOME/IP header information, and room for metadata if expected.
   * - Otherwise, log error and return empty view.
   * \endinternal
   */
  auto PrepareReceiveSomeIp(bool const has_metadata, std::size_t const remaining_message_length)
      -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_PrepareReceiveSomeIp
    MutableIOBufferContainerView view{};
    logger_.LogDebug(
        [&remaining_message_length](ara::log::LogStream& s) {
          s << "Setup buffers for SOME/IP header, remaining length: " << remaining_message_length;
        },
        {__func__}, {__LINE__});

    // Verify remaining message length
    std::size_t const metadata_length{has_metadata ? amsr::someip_protocol::internal::kTimeStampSize : 0U};
    std::size_t const header_length{amsr::someip_protocol::internal::kHeaderSize};
    std::size_t const minimal_message_length{metadata_length + header_length};
    if (remaining_message_length >= minimal_message_length) {
      if (has_metadata) {
        view = MutableIOBufferContainerView{reception_buffer_.receive_container_metadata_and_someip_header.data(),
                                            reception_buffer_.receive_container_metadata_and_someip_header.size()};

      } else {
        view = MutableIOBufferContainerView{reception_buffer_.receive_container_someip_header.data(),
                                            reception_buffer_.receive_container_someip_header.size()};
      }
      // Update chunk reception state.
      chunk_reception_state_ = ChunkReceptionState::kSomeIpHeader;
    } else {
      if (has_metadata) {
        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "Invalid size SOME/IP message with meta data received. SOME/IP message with meta data dropped.";
            },
            {__func__}, {__LINE__});
      } else {
        logger_.LogError(
            [](ara::log::LogStream& s) { s << "Invalid size SOME/IP message received. SOME/IP message dropped."; },
            {__func__}, {__LINE__});
      }

      // Clear previously received data.
      ResetReceptionBuffer();
    }
    // PTP-E-SomeipDaemonClient-MessageReceiver_PrepareReceiveSomeIp
    return view;
  }

  /*!
   * \brief     Prepare reception buffers for PDU header and metadata, if expected.
   *
   * \param[in] has_metadata True if message has metadata.
   * \param[in] remaining_message_length The remaining length of the message.
   * \return    A view to the I/O vector container pointing to the memory region(s).
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Calculate minimal expected message length.
   * - If message length requirements are fulfilled,
   *   - Return view room for PDU header information, and room for metadata if expected.
   * - Otherwise, log error and return empty view.
   * \endinternal
   */
  auto PrepareReceivePdu(bool const has_metadata, std::size_t const remaining_message_length)
      -> MutableIOBufferContainerView {
    // PTP-B-SomeipDaemonClient-MessageReceiver_PrepareReceivePdu
    MutableIOBufferContainerView view{};
    logger_.LogDebug(
        [&remaining_message_length](ara::log::LogStream& s) {
          s << "Setup buffers for PDU Header, remaining length: " << remaining_message_length;
        },
        {__func__}, {__LINE__});

    // Verify remaining message length
    std::size_t const metadata_length{has_metadata ? amsr::someip_protocol::internal::kTimeStampSize : 0U};
    std::size_t const header_length{amsr::someip_protocol::internal::kPduHeaderSize};
    std::size_t const minimal_message_length{metadata_length + header_length};
    if (remaining_message_length >= minimal_message_length) {
      if (has_metadata) {
        view = MutableIOBufferContainerView{reception_buffer_.receive_container_metadata_and_pdu_header.data(),
                                            reception_buffer_.receive_container_metadata_and_pdu_header.size()};
      } else {
        view = MutableIOBufferContainerView{reception_buffer_.receive_container_pdu_header.data(),
                                            reception_buffer_.receive_container_pdu_header.size()};
      }
      // Update chunk reception state.
      chunk_reception_state_ = ChunkReceptionState::kPduHeader;
    } else {
      if (has_metadata) {
        logger_.LogError(
            [&remaining_message_length](ara::log::LogStream& s) {
              s << "Too small PDU message with meta data received. Message size: " << remaining_message_length;
              s << ". PDU message dropped.";
            },
            {__func__}, {__LINE__});
      } else {
        logger_.LogError(
            [&remaining_message_length](ara::log::LogStream& s) {
              s << "Too small PDU message received. Message size: " << remaining_message_length;
              s << ". PDU message dropped.";
            },
            {__func__}, {__LINE__});
      }

      // Clear previously received data.
      ResetReceptionBuffer();
    }

    // PTP-E-SomeipDaemonClient-MessageReceiver_PrepareReceivePdu
    return view;
  }

  /*!
   * \brief Process the message just received after making sure it uses the correct protocol version.
   * \param[in] generic_header The message's generic header.
   * \param[in] specific_header_view View of the specific header.
   * \param[in] reception_buffer the message's reception buffer.
   * \context   App
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Deserialize the control message header.
   * - Get the controller depending on the message type.
   * - If the required controller is available:
   *   - Set the value for the promise.
   * - Otherwise, if the required controller is not set:
   *   - Drop the message and abort execution.
   * \endinternal
   */
  void ProcessReceivedControlMessageResponse(someipd_app_protocol::internal::GenericMessageHeader const& generic_header,
                                             SpecificHeaderView const& specific_header_view,
                                             ReceptionBuffer const& reception_buffer) noexcept {
    // Deserialize control message header
    amsr::someipd_app_protocol::internal::Reader reader{specific_header_view};

    amsr::core::Optional<amsr::someipd_app_protocol::internal::ControlMessageHeader> const control_header{
        deserializer_.DeserializeSpecificIpcPacketHeaderCommandMessage(reader)};
    // Deserialization should never fail here as the input buffer is an array of
    // the required size.
    assert(control_header.has_value());
    amsr::someipd_app_protocol::internal::MessageType const message_type{
        GetMessageTypeWithoutFlags(generic_header.message_type, logger_)};

    ControllerInterface* controller{nullptr};
    switch (message_type) {
      case amsr::someipd_app_protocol::internal::MessageType::kRequestService:
        controller = request_service_controller_;
        break;
      case amsr::someipd_app_protocol::internal::MessageType::kRequestLocalServer:
        controller = request_local_server_controller_;
        break;
      default:  // COV_SOMEIPDAEMON_CLIENT_UNSUPPORTED_MESSAGE_TYPE
        // This will never occur as GetControlPositiveResponseMessageType will abort earlier.
        break;
    }
    if (controller != nullptr) {
      // VCA_SDCL_SLC10_SLC22_OBJECT_REF_VALIDITY_IS_A_FUNCTION_PRECONDITION
      controller->SetPromiseValue(reception_buffer, control_header.value());
    } else {
      // VCA_SDCL_LNG03_OBJECT_IS_VALID
      logger_.LogFatalAndAbort(
          [&message_type](logging::AraComLogger::StringStream& s) {
            s << "Controller not set, dropping message and aborting (Message Type: 0x";  // VCA_SDCL_STRING_STREAM_USAGE
            s << std::hex << std::uppercase                                              // VCA_SDCL_STRING_STREAM_USAGE
              << static_cast<MessageTypeUnderlyingType>(message_type)                    // VCA_SDCL_STRING_STREAM_USAGE
              << ").";                                                                   // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Call ProcessSomeIpMessage with the received message after determining whether the SomeIP message contains
   *        meta data or not.
   * \param[in] generic_header The message's generic header.
   * \context Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Deserialize routing SOME/IP message header.
   * - If the message type is SOME/IP Routing
   *   - Call ProcessSomeIpMessage() to process the received SOME/IP message.
   * - Otherwise, if the message type is SOME/IP with meta data Routing,
   *   - Call ProcessSomeIpMessage() to process the received SOME/IP message with meta data.
   * \endinternal
   */
  void ProcessReceivedSomeIpMessage(
      someipd_app_protocol::internal::GenericMessageHeader const& generic_header) noexcept {
    // PTP-B-SomeipDaemonClient-MessageReceiver_ProcessReceivedSomeIpMessage_SomeIpMessageHandling
    // Deserialize routing SOME/IP message header
    SpecificHeaderView const specific_header_view{reception_buffer_.receive_specific_header};
    amsr::someipd_app_protocol::internal::RoutingSomeIpMessageHeader const routing_someip_header{
        ReadRoutingSomeIpSpecificIpcPacketHeader(specific_header_view)};
    if (someipd_app_protocol::internal::IsRoutingSomeIpMessage(generic_header.message_type)) {
      // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
      amsr::core::Result<amsr::someipd_app_protocol::internal::SomeIpMessage> someip_message_result{
          amsr::someipd_app_protocol::internal::SomeIpMessage::CreateSomeIpMessage(
              std::move(reception_buffer_.receive_message_body))};

      if (someip_message_result.HasValue()) {
        // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION, VCA_SDCL_PROCESS_SOMEIP_MESSAGE
        receive_routing_controller_.ProcessSomeIpMessage(routing_someip_header.instance_id,
                                                         std::move(someip_message_result.Value()));
      } else {
        logger_.LogError(
            [](ara::log::LogStream& s) { s << "Invalid size SOME/IP message received. SOME/IP message dropped."; },
            {__func__}, {__LINE__});
      }
      // PTP-E-SomeipDaemonClient-MessageReceiver_ProcessReceivedSomeIpMessage_SomeIpMessageHandling
    } else {
      // PTP-B-SomeipDaemonClient-MessageReceiver_ProcessReceivedSomeIpMessage_SomeIpMetaDataHandling
      ara::core::Array<amsr::someip_protocol::internal::TimeStamp, 1U> time_stamp_buffer{};
      std::memcpy(time_stamp_buffer.data(), reception_buffer_.receive_metadata.data(),
                  amsr::someip_protocol::internal::kTimeStampSize);
      amsr::core::Result<amsr::someipd_app_protocol::internal::SomeIpMessage> someip_message_result{
          amsr::someipd_app_protocol::internal::SomeIpMessage::CreateSomeIpMessage(
              std::move(reception_buffer_.receive_message_body), *time_stamp_buffer.data())};
      if (someip_message_result.HasValue()) {
        // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION, VCA_SDCL_PROCESS_SOMEIP_MESSAGE
        receive_routing_controller_.ProcessSomeIpMessage(routing_someip_header.instance_id,
                                                         std::move(someip_message_result.Value()));
      } else {
        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "Invalid size SOME/IP message with meta data received. SOME/IP message with meta data dropped.";
            },
            {__func__}, {__LINE__});
      }
      // PTP-E-SomeipDaemonClient-MessageReceiver_ProcessReceivedSomeIpMessage_SomeIpMetaDataHandling
      // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
    }
  }

  /*!
   * \brief Call ProcessPduMessage with the received message after determining whether the PDU message contains meta
   *         data or not.
   * \param[in] generic_header The message's generic header.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Deserialize routing PDU message header.
   * - If the message type is PDU Routing
   *   - Call ProcessPduMessage() to process the received PDU message.
   * - Otherwise, if the message type is PDU with meta data Routing,
   *   - Call ProcessPduMessage() to process the received PDU message with meta data.
   * \endinternal
   */
  void ProcessReceivedPduMessage(someipd_app_protocol::internal::GenericMessageHeader const& generic_header) noexcept {
    // PTP-B-SomeipDaemonClient-MessageReceiver_ProcessReceivedPduMessage_PduMessageHandling
    // Deserialize routing Pdu message header
    SpecificHeaderView const specific_header_view{reception_buffer_.receive_specific_header};
    amsr::someipd_app_protocol::internal::RoutingPduMessageHeader const routing_pdu_header{
        ReadRoutingPduSpecificIpcPacketHeader(specific_header_view)};
    if (someipd_app_protocol::internal::IsRoutingPduMessage(generic_header.message_type)) {
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPDAEMONCLIENT_M0.3.1_Previously_checked_pointer
      // VCA_SDCL_SLC10_SLC22_OBJECT_REF_VALIDITY_IS_A_FUNCTION_PRECONDITION
      std::size_t const pdu_message_size{reception_buffer_.receive_message_body->GetView(0U).size()};
      // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
      amsr::core::Result<amsr::someipd_app_protocol::internal::PduMessage> pdu_message_result{
          amsr::someipd_app_protocol::internal::PduMessage::CreatePduMessage(
              std::move(reception_buffer_.receive_message_body))};
      if (pdu_message_result.HasValue()) {
        // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION, VCA_SDCL_PROCESS_PDU_MESSAGE
        receive_routing_controller_.ProcessPduMessage(routing_pdu_header.instance_id,
                                                      std::move(pdu_message_result.Value()));
      } else {
        // Inconsistent PDU Message, drop.
        logger_.LogError(
            [&pdu_message_size](ara::log::LogStream& s) {
              s << "Too small PDU message received. Message size: " << pdu_message_size;
              s << ". PDU message dropped.";
            },
            {__func__}, {__LINE__});
      }
      // PTP-E-SomeipDaemonClient-MessageReceiver_ProcessReceivedPduMessage_PduMessageHandling
    } else {
      // PTP-B-SomeipDaemonClient-MessageReceiver_ProcessReceivedPduMessage_PduMetaDataHandling

      // First copy the PDU message time_stamp to a temporary continuous buffer
      ara::core::Array<amsr::someip_protocol::internal::TimeStamp, 1U> time_stamp_buffer{};
      std::memcpy(time_stamp_buffer.data(), reception_buffer_.receive_metadata.data(),
                  amsr::someip_protocol::internal::kTimeStampSize);
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPDAEMONCLIENT_M0.3.1_Previously_checked_pointer
      // VCA_SDCL_SLC10_SLC22_OBJECT_REF_VALIDITY_IS_A_FUNCTION_PRECONDITION
      std::size_t const pdu_message_size{reception_buffer_.receive_message_body->GetView(0U).size()};
      // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
      amsr::core::Result<amsr::someipd_app_protocol::internal::PduMessage> pdu_message_result{
          amsr::someipd_app_protocol::internal::PduMessage::CreatePduMessage(
              std::move(reception_buffer_.receive_message_body), *time_stamp_buffer.data())};
      if (pdu_message_result.HasValue()) {
        // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION, VCA_SDCL_PROCESS_PDU_MESSAGE
        receive_routing_controller_.ProcessPduMessage(routing_pdu_header.instance_id,
                                                      std::move(pdu_message_result.Value()));
      } else {
        // Inconsistent PDU Message, drop.
        logger_.LogError(
            [&pdu_message_size](ara::log::LogStream& s) {
              s << "Too small PDU message received. Message size: " << pdu_message_size;
              s << ". PDU message dropped.";
            },
            {__func__}, {__LINE__});
      }
      // PTP-E-SomeipDaemonClient-MessageReceiver_ProcessReceivedPduMessage_PduMetaDataHandling
      // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
    }
  }

  /*!
   * \brief Call ProcessInitialFieldNotificationSomeIpMessage with the received message.
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Call ProcessInitialFieldNotificationSomeIpMessage() to process the received initial notification message.
   * \endinternal
   */
  void ProcessReceivedInitialFieldNotificationSomeIpMessage() noexcept {
    // PTP-B-SomeipDaemonClient-MessageReceiver_ProcessReceivedInitialFieldNotificationSomeIpMessage
    // Deserialize routing initial field notification message header
    SpecificHeaderView const specific_header_view{reception_buffer_.receive_specific_header};
    amsr::someipd_app_protocol::internal::RoutingNotificationMessageHeader const routing_notification_header{
        ReadRoutingNotificationIpcPacketHeader(specific_header_view)};

    // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
    amsr::core::Result<amsr::someipd_app_protocol::internal::SomeIpMessage> someip_message_result{
        amsr::someipd_app_protocol::internal::SomeIpMessage::CreateSomeIpMessage(
            std::move(reception_buffer_.receive_message_body))};

    if (someip_message_result.HasValue()) {
      // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION, VCA_SDCL_PROCESS_SOMEIP_MESSAGE
      receive_routing_controller_.ProcessInitialFieldNotificationSomeIpMessage(routing_notification_header.instance_id,
                                                                               std::move(someip_message_result.Value()),
                                                                               routing_notification_header.client_id);
    } else {
      logger_.LogError(
          [](ara::log::LogStream& s) { s << "Invalid size initial notification message received, message dropped."; },
          {__func__}, {__LINE__});
    }
    // PTP-E-SomeipDaemonClient-MessageReceiver_ProcessReceivedInitialFieldNotificationSomeIpMessage
  }

  /*!
   * \brief Underlying type of "MessageType".
   */
  using MessageTypeUnderlyingType = std::underlying_type_t<amsr::someipd_app_protocol::internal::MessageType>;

  /*!
   * \brief A reference to the allocator for received messages.
   */
  vac::memory::allocator::MemoryBufferAllocator& receive_message_allocator_;

  /*!
   * \brief Our logger.
   */
  logging::AraComLogger logger_;

  /*!
   * \brief Reception buffer instance.
   */
  ReceptionBuffer reception_buffer_;

  /*!
   * \brief RoutingController handling routing messages.
   */
  ReceiveRoutingController& receive_routing_controller_;

  /*!
   * \brief Controller for request service control messages.
   */
  ControllerInterface* request_service_controller_{nullptr};

  /*!
   * \brief Controller for request local server control messages.
   */
  ControllerInterface* request_local_server_controller_{nullptr};

  /*!
   * \brief An instance of the DeserMessageHeaders class.
   */
  amsr::someipd_app_protocol::internal::DeserMessageHeaders deserializer_{};

  /*!
   * \brief Chunk reception states.
   */
  enum class ChunkReceptionState : std::uint8_t {
    kReceptionStart,   /*!<  Start state.                           */
    kIpcHeader,        /*!<  IPC header, first chunk.               */
    kSomeIpHeader,     /*!<  Second chunk, for SOME/IP header etc.  */
    kSomeIpPayload,    /*!<  Third chunk, for SOME/IP payload.      */
    kPduHeader,        /*!<  Second chunk, for PDU header etc.      */
    kPduPayload,       /*!<  Third chunk, for PDU payload.          */
    kCommandOrData,    /*!<  Second chunk, for commands and data.   */
    kReceptionComplete /*!<  Last chunk, expected to be empty.      */
  };

  /*!
   * \brief The chunk reception state.
   */
  ChunkReceptionState chunk_reception_state_{ChunkReceptionState::kReceptionStart};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_SOMEIPDAEMON_CLIENT_UNSUPPORTED_MESSAGE_TYPE
//   \ACCEPT XX
//   \REASON This will never occur as GetControlPositiveResponseMessageType will abort earlier.
//
// COV_JUSTIFICATION_END

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_MESSAGE_RECEIVER_H_

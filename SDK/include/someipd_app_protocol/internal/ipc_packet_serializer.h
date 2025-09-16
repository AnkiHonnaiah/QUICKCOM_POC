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
/*!        \file  someipd_app_protocol/internal/ipc_packet_serializer.h
 *        \brief  Provides serializers for the message headers of SOME/IP daemon IPC protocol.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_IPC_PACKET_SERIALIZER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_IPC_PACKET_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <memory>
#include <utility>

#include "amsr/someip_daemon_client/external/libvac/flexible_unique_memory_buffer_allocator.h"
#include "ara/core/string.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/types.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"
#include "vac/memory/non_zeroing_allocator.h"

namespace amsr {
namespace someipd_app_protocol {
namespace internal {

/*!
 * \brief Shorting for amsr::someip_protocol::internal::serialization.
 */
namespace someip_protocol = ::amsr::someip_protocol::internal::serialization;

/*!
 * \brief SerializeServiceInstanceIdClientIdData Structure.
 */
struct SerializeServiceInstanceIdClientIdData {
  /*!
   * \brief Writer holding the buffer to which the data is serialized.
   */
  someip_protocol::Writer& writer;

  /*!
   * \brief Control message service ID.
   */
  ControlMessageServiceId service_id;

  /*!
   * \brief Control message instance ID.
   */
  ControlMessageInstanceId instance_id;

  /*!
   * \brief Control message major version.
   */
  ControlMessageMajorVersion major_version;

  /*!
   * \brief Control message minor version.
   */
  ControlMessageMinorVersion minor_version;

  /*!
   * \brief The control message client ID.
   */
  ControlMessageClientId client_id;
};

/*!
 * \brief SerializeServiceInstanceIdData Structure.
 */
struct SerializeServiceInstanceIdData {
  /*!
   * \brief Writer holding the buffer to which the data is serialized.
   */
  someip_protocol::Writer& writer;

  /*!
   * \brief Control message service ID.
   */
  ControlMessageServiceId service_id;

  /*!
   * \brief Control message instance ID.
   */
  ControlMessageInstanceId instance_id;

  /*!
   * \brief Control message major version.
   */
  ControlMessageMajorVersion major_version;

  /*!
   * \brief Control message minor version.
   */
  ControlMessageMinorVersion minor_version;
};

/*!
 * \brief SerializePayloadWithServiceInstanceIdEventClientIdData Structure.
 */
struct SerializePayloadWithServiceInstanceIdEventClientIdData {
  /*!
   * \brief Writer holding the buffer to which the data is serialized.
   */
  someip_protocol::Writer& writer;

  /*!
   * \brief Control message service ID.
   */
  ControlMessageServiceId service_id;

  /*!
   * \brief Control message instance ID.
   */
  ControlMessageInstanceId instance_id;

  /*!
   * \brief Control message major version.
   */
  ControlMessageMajorVersion major_version;

  /*!
   * \brief Control message minor version.
   */
  ControlMessageMinorVersion minor_version;

  /*!
   * \brief Control message event ID.
   */
  ControlMessageEventId event_id;

  /*!
   * \brief Control message client ID.
   */
  ControlMessageClientId client_id;
};

/*!
 * \brief Serialization helper function to serialize the generic ipc packet header.
 * \tparam Endian Endianness of serialization.
 * \param[in, out] writer Writer holding the buffer to which the data is serialized.
 * \param[in] message_type The message types of SOME/IP daemon IPC protocol.
 * \param[in] message_length Message length.
 * \pre message_length must be within the maximum numeric limit.
 * \context App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 */
template <typename Endian = ::amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializeGenericIpcPacketHeader(someip_protocol::Writer& writer, MessageType const message_type,
                                     std::size_t const message_length) noexcept {
  // PTP-B-SomeipDaemonClient-SerializeGenericIpcPacketHeader
  if (static_cast<std::size_t>(std::numeric_limits<MessageLength>::max()) < message_length) {
    // VCA_SDCL_CALLING_CONSTRUCTOR_METHOD
    ::amsr::someip_daemon_client::internal::logging::AraComLogger const logger{
        // VCA_SDCL_CALLING_CONSTRUCTOR_METHOD
        ::amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
        // VCA_SDCL_CALLING_CONSTRUCTOR_METHOD
        ::amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
        amsr::core::StringView{"SerializeGenericIpcPacketHeader()"}};
    logger.LogFatalAndAbort(
        [&message_length](amsr::someip_daemon_client::internal::logging::AraComLogger::StringStream& s) {
          s << "[";                                                                      // VCA_SDCL_STRING_STREAM_USAGE
          s << "Violation: Length field overflow during serialization. Size to push =";  // VCA_SDCL_STRING_STREAM_USAGE
          s << std::to_string(message_length);                                           // VCA_SDCL_STRING_STREAM_USAGE
          s << ", maximum size = ";                                                      // VCA_SDCL_STRING_STREAM_USAGE
          s << std::to_string(std::numeric_limits<MessageLength>::max());                // VCA_SDCL_STRING_STREAM_USAGE
          s << "]";                                                                      // VCA_SDCL_STRING_STREAM_USAGE
                                                                                         // VCA_SDCL_STRING_STREAM_USAGE
        },
        static_cast<char const*>(__func__), __LINE__);
  }

  writer.writePrimitive<ProtocolVersion, Endian>(kProtocolVersion);  // VCA_SDCL_OBJECT_ALLOCATED_ON_CALLERS_STACK
  // VCA_SDCL_OBJECT_ALLOCATED_ON_CALLERS_STACK
  writer.writePrimitive<std::underlying_type_t<MessageType>, Endian>(
      static_cast<std::underlying_type_t<MessageType>>(message_type));
  // VCA_SDCL_OBJECT_ALLOCATED_ON_CALLERS_STACK
  writer.writePrimitive<MessageLength, Endian>(static_cast<MessageLength>(message_length));
  // PTP-E-SomeipDaemonClient-SerializeGenericIpcPacketHeader
}

/*!
 * \brief Serialization helper function to serialize the specific ipc packet header for a command message.
 * \tparam Endian Endianness of serialization.
 * \param[in, out] writer Writer holding the buffer to which the data is serialized.
 * \param[in] return_code The control message return code.
 * \pre -
 * \context Init
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Endian = ::amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializeSpecificIpcPacketHeaderCommandMessage(someip_protocol::Writer& writer,
                                                    ControlMessageReturnCode const return_code) noexcept {
  // PTP-B-SomeipDaemonClient-SerializeSpecificIpcPacketHeaderCommandMessage
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  writer.writePrimitive<std::underlying_type_t<ControlMessageReturnCode>, Endian>(
      static_cast<std::underlying_type_t<ControlMessageReturnCode>>(return_code));
  // PTP-E-SomeipDaemonClient-SerializeSpecificIpcPacketHeaderCommandMessage
}

/*!
 * \brief Serialization helper function to serialize the spcific ipc packet header for a routing message.
 * \tparam Endian Endianness of serialization.
 * \param[in, out] writer Writer holding the buffer to which the data is serialized.
 * \param[in] instance_id routing message instance ID.
 * \param[in] client_id routing message client ID, defaulted to 0U.
 * \pre -
 * \context App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Endian = ::amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializeSpecificIpcPacketHeaderRoutingMessage(someip_protocol::Writer& writer,
                                                    RoutingMessageInstanceId const instance_id,
                                                    RoutingMessageClientId const client_id = 0U) noexcept {
  // PTP-B-SomeipDaemonClient-SerializeSpecificIpcPacketHeaderRoutingMessage
  writer.writePrimitive<RoutingMessageInstanceId, Endian>(instance_id);  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  writer.writePrimitive<RoutingMessageClientId, Endian>(client_id);      // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  // PTP-E-SomeipDaemonClient-SerializeSpecificIpcPacketHeaderRoutingMessage
}

/*!
 * \brief Serialization helper function to serialize the control message request payload
 *        containing service id, instance id, major version, minor version, event id and client id.
 * \tparam Endian Endianness of serialization.
 * \param[in] data Object of structure SerializePayloadWithServiceInstanceIdEventClientIdData containing data members
 *            writer, service_id, instance_id, minor_version, major_version, event_id and client_id.
 * \pre -
 * \context App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Endian = ::amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializePayloadWithServiceInstanceIdEventClientId(
    SerializePayloadWithServiceInstanceIdEventClientIdData const& data) noexcept {
  // PTP-B-SomeipDaemonClient-SerializePayloadWithServiceInstanceIdEventClientId
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageServiceId, Endian>(data.service_id);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageInstanceId, Endian>(data.instance_id);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageMajorVersion, Endian>(data.major_version);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageMinorVersion, Endian>(data.minor_version);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageEventId, Endian>(data.event_id);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageClientId, Endian>(data.client_id);
  // PTP-E-SomeipDaemonClient-SerializePayloadWithServiceInstanceIdEventClientId
}

/*!
 * \brief Serialization helper function to serialize the control message request payload
 *        containing service id, instance id, major version and minor version.
 * \tparam Endian Endianness of serialization.
 * \param[in] data Object of structure SerializePayloadWithServiceInstanceIdEventClientIdData containing data members
 *                 writer, service_id, instance_id, minor_version and major_version.
 * \pre -
 * \context App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Endian = ::amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializePayloadWithServiceInstanceId(SerializeServiceInstanceIdData const& data) noexcept {
  // PTP-B-SomeipDaemonClient-SerializePayloadWithServiceInstanceId
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageServiceId, Endian>(data.service_id);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageInstanceId, Endian>(data.instance_id);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageMajorVersion, Endian>(data.major_version);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageMinorVersion, Endian>(data.minor_version);
  // PTP-E-SomeipDaemonClient-SerializePayloadWithServiceInstanceId
}

/*!
 * \brief Serialization helper function to serialize the control message request payload
 *        containing service id, instance id, major version, minor version and client id.
 * \tparam Endian Endianness of serialization.
 * \param[in] data Object of structure SerializePayloadWithServiceInstanceIdEventClientIdData containing data members
 *                 writer, service_id, instance_id, minor_version, major_version and client_id.
 * \pre -
 * \context App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Endian = ::amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializePayloadWithServiceInstanceIdClientId(SerializeServiceInstanceIdClientIdData const& data) noexcept {
  // PTP-B-SomeipDaemonClient-SerializePayloadWithServiceInstanceIdClientId
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageServiceId, Endian>(data.service_id);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageInstanceId, Endian>(data.instance_id);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageMajorVersion, Endian>(data.major_version);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageMinorVersion, Endian>(data.minor_version);
  // VCA_SDCL_PARAMETER_PASSED_BY_VALUE
  data.writer.writePrimitive<ControlMessageClientId, Endian>(data.client_id);
  // PTP-E-SomeipDaemonClient-SerializePayloadWithServiceInstanceIdClientId
}

/*!
 * \brief   IpcPacketSerializer Class.
 * \details Instantiated once in each controller/packet sender. Provides serialization logic.
 */
class IpcPacketSerializer final {
 public:
  /*!
   * \brief     Constructor for IPC packet serializer.
   * \param[in] message_type The type of the messages that will be serialized.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A12.6.1: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-A12.6.1_Member_not_explicitly_initialized
  // VECTOR NC VectorC++-V12.1.1: MD_SOMEIPDAEMONCLIENT_VectorC++-V12.1.1_Member_not_explicitly_initialized
  explicit IpcPacketSerializer(MessageType message_type) noexcept : ipc_packet_type_(message_type) {
    amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> allocated_packet_result{
        // VCA_SDCL_SPC_15_OTHER_COMPONENT_FUNCTION
        allocator_.Allocate(kGenericMessageHeaderLength + kSpecificMessageHeaderLength)};
    if (!allocated_packet_result.HasValue()) {
      logger_.LogFatalAndAbort(
          [](amsr::someip_daemon_client::internal::logging::AraComLogger::StringStream& s) {
            s << "Failed to allocate memory buffer.";  // VCA_SDCL_STRING_STREAM_USAGE
            // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
    ipc_header_ = std::move(allocated_packet_result.Value());
  }

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   */
  ~IpcPacketSerializer() noexcept = default;

  IpcPacketSerializer() = delete;
  IpcPacketSerializer(IpcPacketSerializer const& other) = delete;
  IpcPacketSerializer(IpcPacketSerializer const&& other) = delete;
  auto operator=(IpcPacketSerializer const& other) -> IpcPacketSerializer& = delete;
  auto operator=(IpcPacketSerializer&& other) -> IpcPacketSerializer& = delete;

  /*!
   * \brief      Serializer for control messages.
   * \tparam     T             The type of control message to serialize.
   * \param[in]  payload       The payload to be serialized.
   * \param[in]  payload_size  The size needed for the memory buffer to fit the serialized payload.
   * \return     A MemoryBufferPtr to the serialized control message payload.
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   */
  template <
      class T,
      typename = std::enable_if_t<
          std::is_same<::amsr::someip_daemon_client::internal::ProvidedServiceInstanceType, T>::value ||
          std::is_same<::amsr::someip_daemon_client::internal::ProxyCreationRequiredServiceInstanceType, T>::value ||
          std::is_same<::amsr::someip_daemon_client::internal::ProxySDRequiredServiceInstanceType, T>::value ||
          std::is_same<::amsr::someip_daemon_client::internal::RequiredEventType, T>::value ||
          std::is_same<::amsr::someip_daemon_client::internal::RequestedServiceType, T>::value>>
  auto Serialize(T const& payload, std::size_t payload_size) noexcept -> vac::memory::allocator::MemoryBufferPtr {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPDAEMONCLIENT_M0.3.1_Previously_checked_pointer
    // Get a view of the complete memory_buffer.
    someip_protocol::BufferView const ipc_header_view{
        // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
        ipc_header_->GetView(0U)};

    // Serialize the generic IPC message header.
    MessageLength message_length{static_cast<MessageLength>(payload_size) + kSpecificMessageHeaderLength};
    someip_protocol::BufferView const generic_header_view{ipc_header_view.first(kGenericMessageHeaderLength)};
    SerializeGenericHeader(generic_header_view, message_length);

    // Serialize the specific IPC message header.
    someip_protocol::BufferView const specific_header_view{ipc_header_view.last(kSpecificMessageHeaderLength)};
    SerializeSpecificHeader(specific_header_view);

    // Allocate memory to hold the serialized control message payload.
    amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> allocated_packet_result{
        // VCA_SDCL_SPC_15_OTHER_COMPONENT_FUNCTION
        allocator_.Allocate(payload_size)};
    if (!allocated_packet_result.HasValue()) {
      logger_.LogFatalAndAbort(
          [](amsr::someip_daemon_client::internal::logging::AraComLogger::StringStream& s) {
            s << "Failed to allocate memory buffer.";  // VCA_SDCL_STRING_STREAM_USAGE
            // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    vac::memory::allocator::MemoryBufferPtr packet{std::move(allocated_packet_result.Value())};

    // Serialize the control message payload.
    // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
    someip_protocol::BufferView const packet_view{packet->GetView(0U)};
    SerializeControlMessagePayload(payload, packet_view);

    return packet;
  }

  /*!
   * \brief      Serializer for IPC headers of type routing.
   * \param[in]  instance_id Service instance identifier.
   * \param[in]  packet_size The size of the routing message to be sent.
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady TRUE
   */
  void Serialize(amsr::someip_protocol::internal::InstanceId instance_id, std::size_t packet_size) const noexcept {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPDAEMONCLIENT_M0.3.1_Previously_checked_pointer
    // Get a view of the complete memory_buffer.
    someip_protocol::BufferView const ipc_header_view{
        // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
        ipc_header_->GetView(0U)};

    // Serialize the generic IPC message header.
    MessageLength message_length{static_cast<MessageLength>(packet_size) + kSpecificMessageHeaderLength};
    someip_protocol::BufferView const generic_header_view{ipc_header_view.first(kGenericMessageHeaderLength)};
    SerializeGenericHeader(generic_header_view, message_length);

    // Serialize the specific IPC message header.
    someip_protocol::BufferView const specific_header_view{ipc_header_view.last(kSpecificMessageHeaderLength)};
    SerializeSpecificHeader(specific_header_view, instance_id);
  }

  /*!
   * \brief      Get a reference to the memory buffer holding the ipc message header.
   * \return     A reference to the memory buffer holding the ipc message header.
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady TRUE
   */
  auto GetIpcMessageHeader() const noexcept -> vac::memory::allocator::MemoryBuffer const& { return *ipc_header_; }

 private:
  /*!
   * \brief      Sets up the generic IPC message header for IPC packets.
   * \param[in]  generic_header_view A view of the buffer where to put the serialized generic IPC message header.
   * \param[in]  message_length      The length of the specific IPC message header + payload.
   * \pre        -
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady TRUE
   */
  void SerializeGenericHeader(someip_protocol::BufferView const& generic_header_view,
                              MessageLength& message_length) const noexcept {
    logger_.LogVerbose(
        [&message_length](ara::log::LogStream& s) {
          s << "Message length without generic IPC message header (specific IPC message header + payload): "
            << message_length;
        },
        {static_cast<char const*>(__func__)}, {__LINE__});

    someip_protocol::Writer writer{generic_header_view};
    SerializeGenericIpcPacketHeader(writer, ipc_packet_type_, message_length);
  }

  /*!
   * \brief      Sets up the specific IPC message header for control command messages.
   * \param[in]  specific_header_view A view of the buffer where to put the serialized specific IPC message header.
   * \pre        -
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   */
  static void SerializeSpecificHeader(someip_protocol::BufferView const& specific_header_view) noexcept {
    someip_protocol::Writer writer{specific_header_view};
    SerializeSpecificIpcPacketHeaderCommandMessage(writer, ControlMessageReturnCode::kOk);
  }

  /*!
   * \brief      Sets up the specific IPC message header for routing (someip/pdu) messages.
   * \param[in]  specific_header_view A view of the buffer where to put the serialized specific IPC message header.
   * \param[in]  instance_id          The instance id of the routing message.
   * \pre        -
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady TRUE
   */
  static void SerializeSpecificHeader(someip_protocol::BufferView const& specific_header_view,
                                      amsr::someip_protocol::internal::InstanceId const instance_id) noexcept {
    someip_protocol::Writer writer{specific_header_view};
    SerializeSpecificIpcPacketHeaderRoutingMessage(writer, instance_id);
  }

  /*!
   * \brief      Serialize payload for control command messages of type ServiceInstanceType.
   * \details    This request payload is required for the OfferService, ReleaseLocalServer, RequestLocalServer,
   *             RequestService, StartServiceDiscovery, StopOfferService and StopServiceDiscovery commands.
   * \param[in]  payload      The payload to be serialized.
   * \param[in]  payload_view A view of the buffer where to put the serialized payload.
   * \pre        -
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   */
  template <class T>
  static void SerializeControlMessagePayload(T const& payload,
                                             someip_protocol::BufferView const& payload_view) noexcept {
    someip_protocol::Writer writer{payload_view};
    SerializeServiceInstanceIdData const data{writer, payload.service_id, payload.instance_id, payload.major_version,
                                              payload.minor_version};
    SerializePayloadWithServiceInstanceId(data);
  }

  /*!
   * \brief      Serialize payload for control command messages of type RequestedServiceType.
   * \details    This request payload is required for the ReleaseService command.
   * \param[in]  payload      The payload to be serialized.
   * \param[in]  payload_view A view of the buffer where to put the serialized payload.
   * \pre        -
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   */
  static void SerializeControlMessagePayload(amsr::someip_daemon_client::internal::RequestedServiceType const& payload,
                                             someip_protocol::BufferView const& payload_view) noexcept {
    someip_protocol::Writer writer{payload_view};
    SerializeServiceInstanceIdClientIdData const data{
        writer,           payload.service_id, payload.instance_id, payload.major_version, payload.minor_version,
        payload.client_id};
    SerializePayloadWithServiceInstanceIdClientId(data);
  }

  /*!
   * \brief      Serialize payload for control command messages of type RequiredEventType.
   * \details    This request payload is required for the SubscribeEvent and UnsubscribeEvent commands.
   * \param[in]  payload      The payload to be serialized.
   * \param[in]  payload_view A view of the buffer where to put the serialized payload.
   * \pre        -
   * \context    APP
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   */
  static void SerializeControlMessagePayload(amsr::someip_daemon_client::internal::RequiredEventType const& payload,
                                             someip_protocol::BufferView const& payload_view) noexcept {
    someip_protocol::Writer writer{payload_view};
    SerializePayloadWithServiceInstanceIdEventClientIdData const data{
        writer,           payload.service_id, payload.instance_id, payload.major_version, payload.minor_version,
        payload.event_id, payload.client_id};
    SerializePayloadWithServiceInstanceIdEventClientId(data);
  }

  /*!
   * \brief IpcPacketSerializer logger.
   */
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"IpcPacketSerializer"}};

  /*!
   * \brief Allocator for transmitted control packets to SOME/IP daemon.
   */
  amsr::someip_daemon_client::external::libvac::FlexibleUniqueMemoryBufferAllocator<
      vac::memory::NonZeroingAllocator<std::uint8_t>>
      allocator_{};

  /*!
   * \brief Memory buffer for the ipc headers.
   */
  vac::memory::allocator::MemoryBufferPtr ipc_header_;

  /*!
   * \brief The type of the ipc packet.
   */
  MessageType ipc_packet_type_;
};

}  // namespace internal
}  // namespace someipd_app_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_IPC_PACKET_SERIALIZER_H_

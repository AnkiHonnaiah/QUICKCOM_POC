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
/*!        \file  someipd_app_protocol/internal/deser_message_headers.h
 *        \brief  Provides deserializers for the message headers of SOME/IP daemon IPC protocol.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_DESER_MESSAGE_HEADERS_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_DESER_MESSAGE_HEADERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <type_traits>
#include <utility>

#include "amsr/core/optional.h"
#include "ara/log/logging.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someipd_app_protocol/internal/message.h"
#include "someipd_app_protocol/internal/protocol_types.h"

namespace amsr {
namespace someipd_app_protocol {
namespace internal {

/*!
 * \brief Reads out primitive data types from a buffer view.
 */
using Reader = ::amsr::someip_protocol::internal::deserialization::Reader;

/*!
 * \brief Type-alias for MachineSpecificEndianness.
 */
using MachineSpecificEndianness = ::amsr::someip_protocol::internal::MachineSpecificEndianness;

/*!
 * \brief   Message header deserializer class.
 * \details Provides deserialization logic.
 */
class DeserMessageHeaders final {
  /*!
   * \brief Reads out primitive data types from a buffer view.
   */
  using Reader = ::amsr::someip_protocol::internal::deserialization::Reader;

  /*!
   * \brief Type-alias for MachineSpecificEndianness.
   */
  using MachineSpecificEndianness = ::amsr::someip_protocol::internal::MachineSpecificEndianness;

 public:
  /*!
   * \brief Constructor of DeserMessageHeaders.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  // NOLINTNEXTLINE (hicpp-use-equals-default)
  DeserMessageHeaders() noexcept {};

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   */
  ~DeserMessageHeaders() noexcept = default;

  DeserMessageHeaders(DeserMessageHeaders const& other) = delete;
  DeserMessageHeaders(DeserMessageHeaders const&& other) = delete;
  auto operator=(DeserMessageHeaders const& other) -> DeserMessageHeaders& = delete;
  auto operator=(DeserMessageHeaders&& other) -> DeserMessageHeaders& = delete;

  /*!
   * \brief Deserializes payload for routing messages of type ServiceDiscoveryServiceInstanceUpdate from a given
   *        serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader  Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize the service ID.
   *   - Deserialize the instance ID.
   *   - Deserialize the major version.
   *   - Deserialize the minor version.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  static auto DeserializePayloadForRoutingServiceDiscoveryServiceInstanceUpdateMessage(Reader& reader) noexcept
      -> amsr::core::Optional<RoutingServiceDiscoveryServiceInstanceUpdateMessagePayload> {
    // PTP-B-SomeipDaemonClient-DeserializePayloadForRoutingServiceDiscoveryServiceInstanceUpdateMessage
    amsr::core::Optional<RoutingServiceDiscoveryServiceInstanceUpdateMessagePayload> result{amsr::core::nullopt};

    if (reader.VerifySize(kServiceDiscoveryServiceInstanceUpdateMessageHeaderLength)) {
      RoutingServiceDiscoveryServiceInstanceUpdateMessagePayload data{};
      reader.ReadPrimitive<RoutingMessageServiceId, Endian>(data.service_id);
      reader.ReadPrimitive<RoutingMessageInstanceId, Endian>(data.instance_id);
      reader.ReadPrimitive<RoutingMessageMajorVersion, Endian>(data.major_version);
      reader.ReadPrimitive<RoutingMessageMinorVersion, Endian>(data.minor_version);
      result.emplace(std::move(data));
    }
    // PTP-E-SomeipDaemonClient-DeserializePayloadForRoutingServiceDiscoveryServiceInstanceUpdateMessage
    return result;
  }

  /*!
   * \brief Deserializes payload for routing messages of type ServiceDiscoveryEventSubscriptionState from a given
   *        serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader  Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize the service ID.
   *   - Deserialize the instance ID.
   *   - Deserialize the major version.
   *   - Deserialize the minor version.
   *   - Deserialize the event ID.
   *   - Check if the subscription state is valid
   *     - Deserialize the subscription state of an event.
   *   - Otherwise, log an error that the received event subscription state is invalid.
   * - Otherwise, log an error that the received message is too small.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  auto DeserializePayloadForRoutingServiceDiscoveryEventSubscriptionStateMessage(Reader& reader) const noexcept
      -> amsr::core::Optional<RoutingServiceDiscoveryEventSubscriptionStateMessagePayload> {
    // PTP-B-SomeipDaemonClient-DeserializePayloadForRoutingServiceDiscoveryEventSubscriptionStateMessage
    amsr::core::Optional<RoutingServiceDiscoveryEventSubscriptionStateMessagePayload> result{amsr::core::nullopt};

    if (reader.VerifySize(kServiceDiscoveryEventSubscriptionStateMessageHeaderLength)) {
      RoutingServiceDiscoveryEventSubscriptionStateMessagePayload data{};
      reader.ReadPrimitive<RoutingMessageServiceId, Endian>(data.service_id);
      reader.ReadPrimitive<RoutingMessageInstanceId, Endian>(data.instance_id);
      reader.ReadPrimitive<RoutingMessageMajorVersion, Endian>(data.major_version);
      reader.ReadPrimitive<RoutingMessageMinorVersion, Endian>(data.minor_version);
      reader.ReadPrimitive<RoutingMessageEventId, Endian>(data.event_id);
      using SubscriptionStateType = std::underlying_type_t<amsr::someip_protocol::internal::SubscriptionState>;
      SubscriptionStateType temp_state{};
      reader.ReadPrimitive<SubscriptionStateType, Endian>(temp_state);
      namespace someip_protocal = ::amsr::someip_protocol::internal;
      if ((temp_state == static_cast<SubscriptionStateType>(someip_protocal::SubscriptionState::kSubscribed)) ||
          (temp_state == static_cast<SubscriptionStateType>(someip_protocal::SubscriptionState::kNotSubscribed)) ||
          (temp_state ==
           static_cast<SubscriptionStateType>(someip_protocal::SubscriptionState::kSubscriptionPending))) {
        data.state = static_cast<someip_protocal::SubscriptionState>(temp_state);
        result.emplace(std::move(data));
      } else {
        logger_.LogError(
            [&temp_state, &data](ara::log::LogStream& s) noexcept {
              s << "Invalid event subscription state received 0x" << ara::log::HexFormat(temp_state) << " for event 0x"
                << ara::log::HexFormat(data.event_id);
            },
            {__func__}, {__LINE__});
      }
    } else {
      logger_.LogError(
          [received_size = reader.Size()](ara::log::LogStream& s) noexcept {
            s << "Deserialization of RoutingServiceDiscoveryEventSubscriptionStateMessagePayload failed. Received size:"
              << received_size << ". Expected size: "
              << amsr::someipd_app_protocol::internal::kServiceDiscoveryEventSubscriptionStateMessageHeaderLength;
          },
          {__func__}, {__LINE__});
    }

    // PTP-E-SomeipDaemonClient-DeserializePayloadForRoutingServiceDiscoveryEventSubscriptionStateMessage
    return result;
  }

  /*!
   * \brief Deserializes the payload for 'SubscribeEvent' and 'UnsubscribeEvent' control messages from a given
   *        serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader  Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize the service ID.
   *   - Deserialize the instance ID.
   *   - Deserialize the major version.
   *   - Deserialize the minor version.
   *   - Deserialize the event ID.
   *   - Deserialize the client ID.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  static auto DeserializeStructWithServiceIdInstanceIdMajorVersionMinorVersionEventIdClientId(Reader& reader) noexcept
      -> amsr::core::Optional<EventControlMessagePayload> {
    // PTP-B-SomeipDaemonClient-DeserializeStructWithServiceIdInstanceIdMajorVersionMinorVersionEventId
    amsr::core::Optional<EventControlMessagePayload> result{amsr::core::nullopt};

    if (reader.VerifySize(kControlMessageSubscribeEventRequestPayloadLength)) {
      EventControlMessagePayload data{};
      reader.ReadPrimitive<amsr::someip_protocol::internal::ServiceId, Endian>(data.service_id);
      reader.ReadPrimitive<amsr::someip_protocol::internal::InstanceId, Endian>(data.instance_id);
      reader.ReadPrimitive<amsr::someip_protocol::internal::MajorVersion, Endian>(data.major_version);
      reader.ReadPrimitive<amsr::someip_protocol::internal::MinorVersion, Endian>(data.minor_version);
      reader.ReadPrimitive<amsr::someip_protocol::internal::EventId, Endian>(data.event_id);
      reader.ReadPrimitive<amsr::someip_protocol::internal::ClientId, Endian>(data.client_id);
      result.emplace(std::move(data));
    }

    // PTP-E-SomeipDaemonClient-DeserializeStructWithServiceIdInstanceIdMajorVersionMinorVersionEventId
    return result;
  }

  /*!
   * \brief Deserializes the payload for control messages containing service id, instance id, major version and minor
   *        version from a given serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize service ID.
   *   - Deserialize instance ID.
   *   - Deserialize major version.
   *   - Deserialize minor version.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  static auto DeserializeServiceInstanceIdentificationData(Reader& reader) noexcept
      -> amsr::core::Optional<ServiceInstanceIdentificationData> {
    // PTP-B-SomeipDaemonClient-DeserializeServiceInstanceIdentificationData
    amsr::core::Optional<ServiceInstanceIdentificationData> result{amsr::core::nullopt};

    if (reader.VerifySize(kServiceInstanceIdentificationDataLength)) {
      ServiceInstanceIdentificationData data{};
      reader.ReadPrimitive<amsr::someip_protocol::internal::ServiceId, Endian>(data.service_id);
      reader.ReadPrimitive<amsr::someip_protocol::internal::InstanceId, Endian>(data.instance_id);
      reader.ReadPrimitive<amsr::someip_protocol::internal::MajorVersion, Endian>(data.major_version);
      reader.ReadPrimitive<amsr::someip_protocol::internal::MinorVersion, Endian>(data.minor_version);
      result.emplace(data);
    }
    // PTP-E-SomeipDaemonClient-DeserializeServiceInstanceIdentificationData

    return result;
  }

  /*!
   * \brief Deserializes the generic IPC packet header from a given serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader  Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize the protocol version.
   *   - Deserialize the message type value.
   *   - Get the message type according to the value.
   *   - Deserialize the message length.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  auto DeserializeGenericIpcPacketHeader(Reader& reader) const noexcept -> amsr::core::Optional<GenericMessageHeader> {
    // PTP-B-SomeipDaemonClient-DeserializeGenericIpcPacketHeader
    amsr::core::Optional<GenericMessageHeader> result{amsr::core::nullopt};

    if (reader.VerifySize(kGenericMessageHeaderLength)) {
      GenericMessageHeader data{};
      reader.ReadPrimitive<ProtocolVersion, Endian>(data.protocol_version);
      std::underlying_type_t<MessageType> temp{};
      reader.ReadPrimitive<std::underlying_type_t<MessageType>, Endian>(temp);
      data.message_type = GetIpcPacketType(temp, logger_);
      reader.ReadPrimitive<MessageLength, Endian>(data.message_length);
      result.emplace(std::move(data));
    }
    // PTP-E-SomeipDaemonClient-DeserializeGenericIpcPacketHeader
    return result;
  }

  /*!
   * \brief Deserializes the specific IPC packet header for control message protocol from a given serialized byte
   *        stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader  Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Check if the deserialized control message return code type belongs to kOk, kNotOk, kMalformedMessage,
   *     kUnknownMessageType, kUnsupportedVersion, kInvalidParam, kTimeOut, kDisconnected, kNotConnected,
   *     kLocalServerNotAvailable, kOfferServiceAccessDenied or kRequestServiceAccessDenied.
   *     - Deserialize the control message return code.
   *   - Otherwise, log an error that the deserialized control message return code type is invalid.
   * - Otherwise, log an error that the serialized byte stream does not have enough size.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  auto DeserializeSpecificIpcPacketHeaderCommandMessage(Reader& reader) const noexcept(true)
      -> amsr::core::Optional<ControlMessageHeader> {
    // PTP-B-SomeipDaemonClient-DeserializeSpecificIpcPacketHeaderCommandMessage
    amsr::core::Optional<ControlMessageHeader> result{amsr::core::nullopt};

    if (reader.VerifySize(kControlMessageHeaderLength)) {
      using ControlMessageReturnCodeType = std::underlying_type_t<ControlMessageReturnCode>;
      ControlMessageReturnCodeType temp{};
      reader.ReadPrimitive<ControlMessageReturnCodeType, Endian>(temp);

      if ((temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kOk)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kNotOk)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kMalformedMessage)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kUnknownMessageType)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kUnsupportedVersion)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kInvalidParam)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kTimeOut)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kLocalServerNotAvailable)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kOfferServiceAccessDenied)) ||
          (temp == static_cast<ControlMessageReturnCodeType>(
                       amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kRequestServiceAccessDenied)) ||
          (temp ==
           static_cast<ControlMessageReturnCodeType>(
               amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kRequestServiceClientIdsOverflow)) ||
          (temp ==
           static_cast<ControlMessageReturnCodeType>(
               amsr::someipd_app_protocol::internal::ControlMessageReturnCode::kRequestServiceRemoteServerNotFound)) ||
          (temp ==
           static_cast<ControlMessageReturnCodeType>(amsr::someipd_app_protocol::internal::ControlMessageReturnCode::
                                                         kRequestServiceRequiredServiceInstanceNotFound))) {
        ControlMessageHeader data{};
        data.return_code = static_cast<ControlMessageReturnCode>(temp);
        result.emplace(std::move(data));
      } else {
        result.reset();
        logger_.LogError(
            [&temp](ara::log::LogStream& s) noexcept {
              s << "Invalid return code: 0x" << ara::log::HexFormat(temp) << " outside of the range of the enumerators";
            },
            {__func__}, {__LINE__});
      }
    } else {
      logger_.LogError([](ara::log::LogStream& s) noexcept { s << "Invalid control message header."; }, {__func__},
                       {__LINE__});
    }

    // PTP-E-SomeipDaemonClient-DeserializeSpecificIpcPacketHeaderCommandMessage
    return result;
  }

  /*!
   * \brief Deserializes the specific header for routing SOME/IP message protocol from a given serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader  Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize the instance ID.
   *   - Deserialize the reserved bytes.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  static auto DeserializeSpecificIpcPacketHeaderRoutingSomeIpMessage(Reader& reader) noexcept
      -> amsr::core::Optional<RoutingSomeIpMessageHeader> {
    // PTP-B-SomeipDaemonClient-DeserializeSpecificIpcPacketHeaderRoutingSomeIpMessage
    amsr::core::Optional<RoutingSomeIpMessageHeader> result{amsr::core::nullopt};

    if (reader.VerifySize(kRoutingSomeIpMessageHeaderLength)) {
      RoutingSomeIpMessageHeader data{};
      reader.ReadPrimitive<RoutingMessageInstanceId, Endian>(data.instance_id);
      reader.ReadPrimitive<std::uint16_t, Endian>(data.reserved);
      result.emplace(data);
    }
    // PTP-E-SomeipDaemonClient-DeserializeSpecificIpcPacketHeaderRoutingSomeIpMessage
    return result;
  }

  /*!
   * \brief Deserializes the payload for 'RequestService' control message response from a given serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader  Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize the reserved bytes.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  static auto DeserializePayloadForControlMessageRequestServiceResponse(Reader& reader) noexcept
      -> amsr::core::Optional<ControlMessageRequestServiceResponsePayload> {
    // PTP-B-SomeipDaemonClient-DeserializePayloadForControlMessageRequestServiceResponse
    amsr::core::Optional<ControlMessageRequestServiceResponsePayload> result{amsr::core::nullopt};

    if (reader.VerifySize(kControlMessageRequestServiceResponsePayloadLength)) {
      ControlMessageRequestServiceResponsePayload data{};
      reader.ReadPrimitive<ControlMessageClientId, Endian>(data.client_id);
      result.emplace(std::move(data));
    }
    // PTP-E-SomeipDaemonClient-DeserializePayloadForControlMessageRequestServiceResponse
    return result;
  }

  /*!
   * \brief Deserializes the routing PDU message specific header from a given serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize the instance ID.
   *   - Deserialize the reserved bytes.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  static auto DeserializeSpecificIpcPacketHeaderRoutingPduMessage(Reader& reader) noexcept
      -> amsr::core::Optional<RoutingPduMessageHeader> {
    // PTP-B-SomeipDaemonClient-DeserializeSpecificIpcPacketHeaderRoutingPduMessage
    amsr::core::Optional<RoutingPduMessageHeader> result{amsr::core::nullopt};

    if (reader.VerifySize(kRoutingPduMessageHeaderLength)) {
      RoutingPduMessageHeader data{};
      reader.ReadPrimitive<RoutingMessageInstanceId, Endian>(data.instance_id);
      reader.ReadPrimitive<std::uint16_t, Endian>(data.reserved);
      result.emplace(std::move(data));
    }
    // PTP-E-SomeipDaemonClient-DeserializeSpecificIpcPacketHeaderRoutingPduMessage
    return result;
  }

  /*!
   * \brief Deserializes the routing notification message specific header from a given serialized byte stream.
   * \tparam Endian Endianness of deserialization.
   * \param[in, out] reader Reader object holding the serialized data.
   * \return  An amsr::core::Optional with deserialized data, or empty on deserialization failure.
   * \pre -
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if the serialized byte stream has enough size to deserialize the given data type.
   *   - Deserialize the instance ID.
   *   - Deserialize the client ID.
   * - Return the deserialization result.
   * \endinternal
   */
  template <typename Endian = MachineSpecificEndianness>
  static auto DeserializeSpecificIpcPacketHeaderRoutingNotificationMessage(Reader& reader) noexcept
      -> amsr::core::Optional<RoutingNotificationMessageHeader> {
    // PTP-B-SomeipDaemonClient-DeserializeSpecificIpcPacketHeaderRoutingNotificationMessage
    amsr::core::Optional<RoutingNotificationMessageHeader> result{amsr::core::nullopt};

    if (reader.VerifySize(kRoutingNotificationMessageHeaderLength)) {
      RoutingNotificationMessageHeader data{};
      reader.ReadPrimitive<RoutingMessageInstanceId, Endian>(data.instance_id);
      reader.ReadPrimitive<std::uint16_t, Endian>(data.client_id);
      result.emplace(std::move(data));
    }
    // PTP-E-SomeipDaemonClient-DeserializeSpecificIpcPacketHeaderRoutingNotificationMessage
    return result;
  }

 private:
  /*!
   * \brief IpcPacketSerializer logger.
   */
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"DeserMessageHeaders"}};
};

}  // namespace internal
}  // namespace someipd_app_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_DESER_MESSAGE_HEADERS_H_

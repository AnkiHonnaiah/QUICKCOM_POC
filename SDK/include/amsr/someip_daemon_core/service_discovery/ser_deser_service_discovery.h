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
/*!        \file  ser_deser_service_discovery.h
 *        \brief  Provides serializers for the message headers of SOME/IP service discovery protocol.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SER_DESER_SERVICE_DISCOVERY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SER_DESER_SERVICE_DISCOVERY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <limits>
#include <utility>
#include "ara/core/vector.h"

#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someipd_app_protocol/internal/deser_message_headers.h"
#include "someipd_app_protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

/*!
 * \brief Shorting for namespace amsr::someip_protocol::internal::serialization
 */
namespace someip_protocol = ::amsr::someip_protocol::internal::serialization;

/*!
 *\brief SerializeEventData Structure.
 */
struct SerializeEventData {
  /*!
   * \brief Writer holding the buffer to which data is serialized.
   */
  someip_protocol::Writer& writer;

  /*!
   * \brief A SOME/IP service identifier.
   */
  someipd_app_protocol::internal::RoutingMessageServiceId const service_id;

  /*!
   * \brief A SOME/IP instance identifier
   */
  someipd_app_protocol::internal::RoutingMessageInstanceId const instance_id;

  /*!
   * \brief A SOME/IP Major version
   */
  someipd_app_protocol::internal::RoutingMessageMajorVersion const major_version;

  /*!
   * \brief A SOME/IP Minor version
   */
  someipd_app_protocol::internal::RoutingMessageMinorVersion const minor_version;

  /*!
   * \brief A SOME/IP event identifier.
   */
  someipd_app_protocol::internal::RoutingMessageEventId event_id;

  /*!
   * \brief An event subscription state.
   */
  someipd_app_protocol::internal::RoutingMessageSubscriptionState state;
};

/*!
 *\brief SerializeRoutingData Structure.
 */
struct SerializeRoutingData {
  /*!
   * \brief Writer holding the buffer to which data is serialized.
   */
  someip_protocol::Writer& writer;

  /*!
   * \brief A SOME/IP service identifier.
   */
  someipd_app_protocol::internal::RoutingMessageServiceId const service_id;

  /*!
   * \brief A SOME/IP instance identifier.
   */
  someipd_app_protocol::internal::RoutingMessageInstanceId const instance_id;

  /*!
   * \brief A SOME/IP Major version
   */
  someipd_app_protocol::internal::RoutingMessageMajorVersion const major_version;

  /*!
   * \brief A SOME/IP Minor version
   */
  someipd_app_protocol::internal::RoutingMessageMinorVersion const minor_version;
};

/*!
 * \brief Serializes Payload for routing messages of type kServiceDiscoveryServiceInstanceUp or
 * kServiceDiscoveryServiceInstanceDown.
 * \tparam Endian Endianness of deserialization.
 * \param[in] routing_data Object of structure SerializeRoutingData having data members writer, service_id,
 *            instance_id, major version and minor version.
 * \pre -
 * \context Network, App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 *
 * \internal
 * - Write the service ID to the buffer.
 * - Write the instance ID to the buffer.
 * - Write the major version to the buffer.
 * - Write the minor version to the buffer.
 * \endinternal
 */

template <typename Endian = amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializeRoutingServiceDiscoveryServiceInstanceUpdateMessagePayload(SerializeRoutingData routing_data) noexcept {
  // PTP-B-SomeipDaemon_SerializeRoutingServiceDiscoveryServiceInstanceUpdateMessagePayload
  routing_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageServiceId, Endian>(
      routing_data.service_id);
  routing_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageInstanceId, Endian>(
      routing_data.instance_id);
  routing_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageMajorVersion, Endian>(
      routing_data.major_version);
  routing_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageMinorVersion, Endian>(
      routing_data.minor_version);
  // PTP-E-SomeipDaemon_SerializeRoutingServiceDiscoveryServiceInstanceUpdateMessagePayload
}

/*!
 * \brief Serializes Payload for routing messages of type kServiceDiscoveryEventSubscriptionState.
 * \tparam Endian Endianness of deserialization.
 * \param[in] event_data Object of structure SerializeEventData having data members
 *            writer, service_id, instance_id, major version, minor version, event_id, state.
 * \pre -
 * \context Network, App, Shutdown
 * \threadsafe FALSE
 * \reentrant	FALSE
 * \synchronous TRUE
 * \steady FALSE
 *
 * \internal
 * - Write the service ID to the buffer.
 * - Write the instance ID to the buffer.
 * - Write the major version to the buffer.
 * - Write the minor version to the buffer.
 * - Write the event ID to the buffer.
 * - Write the routing message subscription state to the buffer.
 * \endinternal
 */

template <typename Endian = amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializeRoutingServiceDiscoveryEventSubscriptionStateMessagePayload(SerializeEventData event_data) noexcept {
  // PTP-B-SomeipDaemon_SerializeRoutingServiceDiscoveryEventSubscriptionStateMessagePayload
  event_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageServiceId, Endian>(
      event_data.service_id);
  event_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageInstanceId, Endian>(
      event_data.instance_id);
  event_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageMajorVersion, Endian>(
      event_data.major_version);
  event_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageMinorVersion, Endian>(
      event_data.minor_version);
  event_data.writer.writePrimitive<someipd_app_protocol::internal::RoutingMessageEventId, Endian>(event_data.event_id);
  event_data.writer.writePrimitive<std::underlying_type<amsr::someip_protocol::internal::SubscriptionState>::type,
                                   Endian>(
      static_cast<std::underlying_type<amsr::someip_protocol::internal::SubscriptionState>::type>(event_data.state));
  // PTP-E-SomeipDaemon_SerializeRoutingServiceDiscoveryEventSubscriptionStateMessagePayload
}

/*!
 * \brief Serializes 4 reserved bytes for the routing channel message header.
 * \tparam Endian Endianness of deserialization.
 * \param[in, out] writer Writer holding the buffer to which data is serialized.
 * \pre -
 * \context Network
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 *
 * \internal
 * - Write four reserved bytes.
 * \endinternal
 */
template <typename Endian = amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializeRoutingServiceDiscoveryHeader(someip_protocol::Writer& writer) noexcept {
  // PTP-B-SomeipDaemon_SerializeRoutingServiceDiscoveryHeader
  std::uint32_t const reserved{0U};
  writer.writePrimitive<std::uint32_t, Endian>(reserved);
  // PTP-E-SomeipDaemon_SerializeRoutingServiceDiscoveryHeader
}

/*!
 * \brief Deserializes the 'ReleaseService' control message request payload from a given serialized byte stream.
 * \tparam Endian Endianness of deserialization.
 * \param[in,out] reader  Reader object holding the serialized data.
 * \return  true if deserialization was successful, false otherwise.
 *
 * \pre         -
 * \context     App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 *
 * \internal
 * - Check if the serialized byte stream has enough size to deserialize the given data type.
 *   - Deserialize the reserved bytes.
 * - Return the deserialization result.
 * \endinternal
 */
template <typename Endian = amsr::someip_protocol::internal::MachineSpecificEndianness>
ara::core::Optional<someipd_app_protocol::internal::ControlMessageReleaseServicePayload>
DeserializeControlMessageReleaseServiceRequestPayload(
    amsr::someip_protocol::internal::deserialization::Reader& reader) noexcept {
  // PTP-B-SomeipDaemon_DeserializeControlMessageReleaseServiceRequestPayload
  ara::core::Optional<someipd_app_protocol::internal::ControlMessageReleaseServicePayload> result{ara::core::nullopt};

  if (reader.VerifySize(someipd_app_protocol::internal::kControlMessageReleaseServiceRequestPayloadLength)) {
    someipd_app_protocol::internal::ControlMessageReleaseServicePayload data{};
    reader.ReadPrimitive<amsr::someip_protocol::internal::ServiceId, Endian>(data.service_id);
    reader.ReadPrimitive<amsr::someip_protocol::internal::InstanceId, Endian>(data.instance_id);
    reader.ReadPrimitive<amsr::someip_protocol::internal::MajorVersion, Endian>(data.major_version);
    reader.ReadPrimitive<amsr::someip_protocol::internal::MinorVersion, Endian>(data.minor_version);
    reader.ReadPrimitive<someipd_app_protocol::internal::ControlMessageClientId, Endian>(data.client_id);
    result.emplace(std::move(data));
  }
  // PTP-E-SomeipDaemon_DeserializeControlMessageReleaseServiceRequestPayload
  return result;
}

/*!
 * \brief Serialization helper function to serialize the control message Release Service payload.
 * \tparam Endian Endianness of serialization.
 * \param[in,out] writer Writer holding the buffer to which the data is serialized.
 * \param[in] client_id The control message client ID.
 * \pre -
 * \context App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 */
template <typename Endian = ::amsr::someip_protocol::internal::MachineSpecificEndianness>
void SerializeControlMessageRequestServiceResponsePayload(
    someip_protocol::Writer& writer, someipd_app_protocol::internal::ControlMessageClientId const client_id) noexcept {
  // PTP-B-SomeipDaemon_SerializeControlMessageRequestServiceResponsePayload
  writer.writePrimitive<someipd_app_protocol::internal::ControlMessageClientId, Endian>(client_id);
  // PTP-E-SomeipDaemon_SerializeControlMessageRequestServiceResponsePayload
}

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SER_DESER_SERVICE_DISCOVERY_H_

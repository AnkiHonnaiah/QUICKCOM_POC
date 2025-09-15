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
/*!        \file  someipd_app_protocol/internal/message.h
 *        \brief  SOME/IP daemon IPC protocol definition
 *
 *      \details  This file specifies the communication protocol for messages exchanged between an application
 *                and the SOME/IP daemon via IPC.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_MESSAGE_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/span.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/protocol_types.h"

namespace amsr {
namespace someipd_app_protocol {
namespace internal {

/*!
 * An IPC message always looks as follows:
 *    +-----------------------------+
 *    |   Generic header            |
 *    +-----------------------------+
 *    |   Specific message header   |
 *    +-----------------------------+
 *    |   Payload                   |
 *    +-----------------------------+
 */

// ---- Generic message header -------------------------------------------------------------------------------------

/*!
 * \brief Protocol version.
 */
static constexpr ProtocolVersion kProtocolVersion{2U};

/*!
 * \brief Generic IPC protocol message header
 */
struct GenericMessageHeader {
  /*!
   * \brief Protocol version of the message.
   */
  ProtocolVersion protocol_version{};
  /*!
   * \brief Message type for processing.
   */
  MessageType message_type{};
  /*!
   * \brief The length of a message in bytes.
   */
  MessageLength message_length{};
};

/*!
 * \brief Length (in bytes) of the generic IPC protocol header.
 */
static constexpr MessageLength kGenericMessageHeaderLength{12U};

/*!
 * \brief Length of a 'SpecificMessageHeader' header. This is the common length of all control and routing channel
 * message headers. Size of all control and routing channel message headers shall be equal. This will
 * simplify SOME/IP daemon implementation consuming the IPC message headers.
 */
static constexpr MessageLength kSpecificMessageHeaderLength{4U};

// ----Command or Service Discovery Message Payload-----------------------------------------------------------------

/*!
 * \brief Service instance identification data.
 */
struct ServiceInstanceIdentificationData {
  /*!
   * \brief Service ID
   */
  ::amsr::someip_protocol::internal::ServiceId service_id;
  /*!
   * \brief Service instance ID
   */
  ::amsr::someip_protocol::internal::InstanceId instance_id;
  /*!
   * \brief Major Version
   */
  ::amsr::someip_protocol::internal::MajorVersion major_version;
  /*!
   * \brief Minor Version
   */
  ::amsr::someip_protocol::internal::MinorVersion minor_version;
};

/*!
 * \brief Length of service instance identification data consisting of service id, instance id, major version and
 * minor version.
 */
static constexpr MessageLength kServiceInstanceIdentificationDataLength{9U};

// ---- Control channel message headers ----------------------------------------------------------------------------

// ---- ControlMessageHeader ----------------------------------

/*!
 * \brief Specific message header for control channel messages.
 */
struct ControlMessageHeader {
  /*!
   * \brief Return code
   */
  ControlMessageReturnCode return_code;
};

/*!
 * \brief Length of a 'ControlMessageHeader' header
 */
static constexpr MessageLength kControlMessageHeaderLength{4U};

// ---- ControlMessage payloads ----

/*!
 * \brief Message payload of 'RequestService' control message.
 * \details This message payload is used for
 * - response from SOME/IP daemon of 'RequestService'.
 */
struct ControlMessageRequestServiceResponsePayload {
  /*!
   * \brief client ID
   */
  ControlMessageClientId client_id;
};

/*!
 * \brief Message payload of 'ReleaseService' control message.
 * \details This message payload is used for
 * - request to SOME/IP daemon of 'ReleaseService'.
 * Related response payload of this command is empty.
 */
struct ControlMessageReleaseServicePayload {
  /*!
   * \brief Service ID
   */
  ControlMessageServiceId service_id;
  /*!
   * \brief Service instance ID
   */
  ControlMessageInstanceId instance_id;
  /*!
   * \brief Major Version
   */
  ControlMessageMajorVersion major_version;
  /*!
   * \brief Minor Version
   */
  ControlMessageMinorVersion minor_version;
  /*!
   * \brief client ID
   */
  ControlMessageClientId client_id;
};

/*!
 * \brief Length of a 'RequestService' response control message.
 */
static constexpr MessageLength kControlMessageRequestServiceResponsePayloadLength{2U};

/*!
 * \brief Length of a 'ReleaseService' request control message.
 */
static constexpr MessageLength kControlMessageReleaseServiceRequestPayloadLength{11U};

/*!
 * \brief 'SubscribeEvent' and 'UnsubscribeEvent' request control message.
 */
struct EventControlMessagePayload {
  /*!
   * \brief Service ID
   */
  ControlMessageServiceId service_id;
  /*!
   * \brief Service instance ID
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
   * \brief Service event ID
   */
  ControlMessageEventId event_id;
  /*!
   * \brief Service event ID
   */
  ControlMessageClientId client_id;
};
/*!
 * \brief Length of a 'SubscribeEvent' request control message.
 */
static constexpr MessageLength kControlMessageSubscribeEventRequestPayloadLength{13U};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10_M3.4.1_global
/*!
 * \brief Length of a 'UnsubscribeEvent' request control message.
 */
static constexpr MessageLength kControlMessageUnsubscribeEventRequestPayloadLength{
    kControlMessageSubscribeEventRequestPayloadLength};

// ---- Routing channel message headers ----------------------------------------------------------------------------

// ---- RoutingSomeIpMessageHeader ----------------------------------

/*!
 * \brief Specific message header for SOME/IP routing messages.
 */
struct RoutingSomeIpMessageHeader {
  /*!
   * \brief SOME/IP instance ID
   */
  RoutingMessageInstanceId instance_id;
  /*!
   * \brief reserved 2 bytes. Introduced for consistent header length of all control and routing channel message
   * headers.
   */
  std::uint16_t reserved;
};

/*!
 * \brief Specific message header for PDU routing messages.
 */
struct RoutingPduMessageHeader {
  /*!
   * \brief SOME/IP instance ID
   */
  RoutingMessageInstanceId instance_id;

  /*!
   * \brief reserved 2 bytes. Introduced for consistent header length of all control and routing channel message
   * headers.
   */
  std::uint16_t reserved;
};

/*!
 * \brief Specific message header for notification routing messages.
 */
struct RoutingNotificationMessageHeader {
  /*!
   * \brief SOME/IP instance ID
   */
  RoutingMessageInstanceId instance_id;

  /*!
   * \brief client id.
   */
  RoutingMessageClientId client_id;
};

/*!
 * \brief Length of a 'RoutingSomeIpMessageHeader' header in bytes.
 */
static constexpr MessageLength kRoutingSomeIpMessageHeaderLength{4U};

/*!
 * \brief Length of a 'RoutingPduMessageHeader' header in bytes.
 */
static constexpr MessageLength kRoutingPduMessageHeaderLength{4U};

/*!
 * \brief Length of a 'RoutingNotificationMessageHeader' header in bytes.
 */
static constexpr MessageLength kRoutingNotificationMessageHeaderLength{4U};

// ---- RoutingServiceDiscoveryHeader -------------------------------

/*!
 * \brief Specific message header for the service discovery routing messages:
 * - ServiceDiscoveryServiceInstanceUp
 * - ServiceDiscoveryServiceInstanceDown
 * - ServiceDiscoveryEventSubscriptionState
 */
struct RoutingServiceDiscoveryHeader {
  /*!
   * \brief reserved 4 bytes. Introduced for consistent header length of all control and routing channel message
   * headers.
   */
  std::uint32_t reserved;
};

/*!
 * \brief Length of a 'RoutingServiceDiscoveryMessageHeader' header in bytes.
 */
static constexpr MessageLength kRoutingServiceDiscoveryHeaderLength{4U};

// ---- ServiceDiscoveryServiceInstanceUpdate payload ---------------

/*!
 * \brief Type-alias for routing messages payload of type kServiceDiscoveryServiceInstanceUpdate.
 */
using RoutingServiceDiscoveryServiceInstanceUpdateMessagePayload = ServiceInstanceIdentificationData;

/*!
 * \brief Length of a 'ServiceDiscoveryServiceInstanceUpdate' header in bytes.
 */
static constexpr MessageLength kServiceDiscoveryServiceInstanceUpdateMessageHeaderLength{9U};

// ---- ServiceDiscoveryEventSubscriptionState payload --------------

/*!
 * \brief Payload for routing messages of type kServiceDiscoveryEventSubscriptionState.
 */
struct RoutingServiceDiscoveryEventSubscriptionStateMessagePayload {
  /*!
   * \brief SOME/IP service identifier
   */
  RoutingMessageServiceId service_id;
  /*!
   * \brief Instance ID
   */
  RoutingMessageInstanceId instance_id;
  /*!
   * \brief Major version
   */
  RoutingMessageMajorVersion major_version;
  /*!
   * \brief Minor version
   */
  RoutingMessageMinorVersion minor_version;
  /*!
   * \brief SOME/IP event identifier
   */
  RoutingMessageEventId event_id;
  /*!
   * \brief Event subscription state (\see ServiceDiscoveryEventSubscriptionState)
   */
  RoutingMessageSubscriptionState state;
};

/*!
 * \brief Length of a 'ServiceDiscoveryEventSubscriptionState' header in bytes.
 */
static constexpr MessageLength kServiceDiscoveryEventSubscriptionStateMessageHeaderLength{12U};

// ---- Consistency checks  ----------------------------------
/*!
 * \brief Make sure that size of ControlMessageHeader is equal to kSpecificMessageHeaderLength.
 * \details Size of all control and routing channel message headers shall be equal. This will
 * simplify SOME/IP daemon implementation consuming the IPC message headers.
 */
static_assert(kControlMessageHeaderLength == kSpecificMessageHeaderLength,
              "sizeof ControlMessageHeader is not as specified.");
/*!
 * \brief Make sure that kSpecificMessageHeaderLength is sufficient for RoutingSomeIpMessageHeader.
 * \details Size of all control and routing channel message headers shall be equal. This will
 * simplify SOME/IP daemon implementation consuming the IPC message headers.
 */
static_assert(kRoutingSomeIpMessageHeaderLength == kSpecificMessageHeaderLength,
              "sizeof RoutingSomeIpMessageHeader is not as specified.");
/*!
 * \brief Make sure that kSpecificMessageHeaderLength is sufficient for RoutingServiceDiscoveryMessageHeader.
 * \details Size of all control and routing channel message headers shall be equal. This will
 * simplify SOME/IP daemon implementation consuming the IPC message headers.
 */
static_assert(kRoutingServiceDiscoveryHeaderLength == kSpecificMessageHeaderLength,
              "sizeof RoutingServiceDiscoveryMessageHeader is not as specified.");
/*!
 * \brief Make sure that kRoutingNotificationMessageHeaderLength is sufficient for RoutingNotificationMessageHeader.
 * \details Size of all control and routing channel message headers shall be equal. This will
 * simplify SOME/IP daemon implementation consuming the IPC message headers.
 */
static_assert(kRoutingNotificationMessageHeaderLength == kSpecificMessageHeaderLength,
              "sizeof RoutingNotificationMessageHeader is not as specified.");

/*!
 * \brief Represents an array view of a specific routing or control channel message header.
 */
using SpecificHeaderView = amsr::core::Span<std::uint8_t const>;

}  // namespace internal
}  // namespace someipd_app_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_MESSAGE_H_

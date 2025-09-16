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
/*!        \file  someipd_app_protocol/internal/protocol_types.h
 *        \brief  Types of SOME/IP daemon IPC protocol
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_PROTOCOL_TYPES_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_PROTOCOL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <type_traits>

#include "amsr/unique_ptr.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"

namespace amsr {
namespace someipd_app_protocol {
namespace internal {

// ---- Packet / Memory ----
/*!
 * \brief Represents an IPC message.
 */
using PacketBuffer = ara::core::Vector<std::uint8_t>;

/*!
 * \brief Represents a unique pointer to an IPC message.
 */
using Packet = amsr::UniquePtr<PacketBuffer>;

// ---- Generic header ----
/*!
 * \brief Protocol version
 */
using ProtocolVersion = std::uint32_t;

/*!
 * \brief Offset for where the MessageType of the control messages starts
 * \details For control messages, kControlMessageTypesOffset <= MessageType < kRoutingMessageTypesOffset.
 */
static constexpr std::uint32_t kControlMessageTypesOffset{0x00000000U};

/*!
 * \brief Offset for where the MessageType of the routing messages starts
 * \details For routing messages, MessageTypes >= kRoutingMessageTypesOffset.
 */
static constexpr std::uint32_t kRoutingMessageTypesOffset{0x20000000U};

/*!
 * \brief On a positive response from the SOME/IP daemon, this offset is added to the message type from the command.
 */
static constexpr std::uint32_t kMessageTypePositiveResponseOffset{0x80000000U};

/*!
 * \brief On a negative response from the SOME/IP daemon, this offset is added to the message type from the command.
 */
static constexpr std::uint32_t kMessageTypeNegativeResponseOffset{0x40000000U};

/*!
 * \brief Message types of SOME/IP daemon IPC protocol
 */
enum class MessageType : std::uint32_t {
  // ---- Control Channel Message Types ----
  /*!
   * \brief A request sent by an application to the SOME/IP daemon to free a previously allocated client identifier.
   */
  kReleaseService = kControlMessageTypesOffset + 0U,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon informing it about the availability of a SOME/IP
   * service instance offered by this application.
   */
  kOfferService = kControlMessageTypesOffset + 1U,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon informing it that previously offered SOME/IP
   * service instance is no longer available.
   */
  kStopOfferService = kControlMessageTypesOffset + 2U,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon instructing it to subscribe a SOME/IP event
   * on behalf of the application and to forward it to the application.
   */
  kSubscribeEvent = kControlMessageTypesOffset + 3U,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon to cancel a subscription of a SOME/IP event.
   */
  kUnsubscribeEvent = kControlMessageTypesOffset + 4U,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon to start service discovery of a SOME/IP service
   * instance.
   */
  kStartServiceDiscovery = kControlMessageTypesOffset + 5U,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon to stop service discovery of a SOME/IP service
   * instance.
   */
  kStopServiceDiscovery = kControlMessageTypesOffset + 6U,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon informing it about the Requested SOME/IP required
   * service instance by this application.
   */
  kRequestService = kControlMessageTypesOffset + 7U,
  /*!
   * \brief Positive response to RequestService.
   */
  kRequestServicePositiveResponse = kControlMessageTypesOffset + 7U + kMessageTypePositiveResponseOffset,
  /*!
   * \brief Negative response to RequestService.
   */
  kRequestServiceNegativeResponse = kControlMessageTypesOffset + 7U + kMessageTypeNegativeResponseOffset,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon informing it about the Requested SOME/IP provided
   * service instance by this application.
   */
  kRequestLocalServer = kControlMessageTypesOffset + 8U,
  /*!
   * \brief Positive response to RequestLocalServer.
   */
  kRequestLocalServerPositiveResponse = kControlMessageTypesOffset + 8U + kMessageTypePositiveResponseOffset,
  /*!
   * \brief Negative response to RequestLocalServer.
   */
  kRequestLocalServerNegativeResponse = kControlMessageTypesOffset + 8U + kMessageTypeNegativeResponseOffset,
  /*!
   * \brief A request sent by an application to the SOME/IP daemon informing it about the Released SOME/IP provided
   * service instance by this application.
   */
  kReleaseLocalServer = kControlMessageTypesOffset + 9U,

  // ---- Routing Channel Message Types ----

  /*!
   * \brief SOME/IP request, response, event notification in the payload contained. These messages are bidirectional
   * from application to SOME/IP daemon and back from SOME/IP daemon to application.
   */
  kSomeIp = kRoutingMessageTypesOffset + 0U,

  /*!
   * \brief SOME/IP with metadata event notification in the payload contained. These messages are
   *  from SOME/IP daemon to the application.
   */
  kSomeIpWithMetaData = kRoutingMessageTypesOffset + 1U,

  /*!
   * \brief An event subscription requested by a client has changed and the application client gets notified about the
   * state change.
   */
  kServiceDiscoveryEventSubscriptionState = kRoutingMessageTypesOffset + 2U,

  /*!
   * \brief PDU message (signal-based) event notification contained in the payload.
   * These messages are bidirectional from application to SOME/IP daemon and back from SOME/IP daemon to application.
   */
  kPdu = kRoutingMessageTypesOffset + 3U,

  /*!
   * \brief A service instance has been offered. The application gets notified about this update.
   */
  kServiceDiscoveryServiceInstanceUp = kRoutingMessageTypesOffset + 4U,

  /*!
   * \brief A service instance has been stopped. The application gets notified about this update.
   */
  kServiceDiscoveryServiceInstanceDown = kRoutingMessageTypesOffset + 5U,

  /*!
   * \brief PDU message with metadata (signal-based) event notification contained in the payload.
   * From SOME/IP daemon to the application.
   */
  kPduWithMetaData = kRoutingMessageTypesOffset + 6U,

  /*!
   * \brief SOME/IP initial field notification contained in the payload.
   * From SOME/IP daemon to application.
   */
  kSomeIpInitialFieldNotification = kRoutingMessageTypesOffset + 7U,
};

/*!
 * \brief Return the control message type from the given value.
 * \details Abort if the given value does not have a corresponding control message type.
 * \param[in] type_value A value represents a message type.
 * \param[in] logger Reference to the logger to be used.
 * \return The corresponding message type.
 * \pre -
 * \context App | Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 */
auto GetControlMessageType(std::underlying_type_t<MessageType> type_value,
                           amsr::someip_daemon_client::internal::logging::AraComLogger const& logger) -> MessageType;

/*!
 * \brief Return the routing message type from the given value.
 * \details Abort if the given value does not have a corresponding routing message type.
 * \param[in] type_value A value represents a message type.
 * \param[in] logger Reference to the logger to be used.
 * \return The corresponding message type.
 * \pre -
 * \context App | Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 */
auto GetRoutingMessageType(std::underlying_type_t<MessageType> type_value,
                           amsr::someip_daemon_client::internal::logging::AraComLogger const& logger) -> MessageType;

/*!
 * \brief Return the negative control response message type from the given value.
 * \details Abort if the given value does not have a corresponding negative control response message type.
 * \param[in] type_value A value represents a message type.
 * \param[in] logger Reference to the logger to be used.
 * \return The corresponding message type.
 * \pre -
 * \context App | Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 */
auto GetControlNegativeResponseMessageType(std::underlying_type_t<MessageType> type_value,
                                           amsr::someip_daemon_client::internal::logging::AraComLogger const& logger)
    -> MessageType;

/*!
 * \brief Return the positive control response message type from the given value.
 * \details Abort if the given value does not have a corresponding positive control response message type.
 * \param[in] type_value A value represents a message type.
 * \param[in] logger Reference to the logger to be used.
 * \return The corresponding message type.
 * \pre -
 * \context App | Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 */
auto GetControlPositiveResponseMessageType(std::underlying_type_t<MessageType> type_value,
                                           amsr::someip_daemon_client::internal::logging::AraComLogger const& logger)
    -> MessageType;

/*!
 * \brief Return the message type without flags from the given message type with flags.
 * \details Abort if the given value does not have a corresponding message type without flags.
 * \param[in] message_type_with_flags A message type with flags.
 * \param[in] logger Reference to the logger to be used.
 * \return The corresponding message type w/o flags.
 * \pre -
 * \context App | Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 *
 */
auto GetMessageTypeWithoutFlags(MessageType message_type_with_flags,
                                amsr::someip_daemon_client::internal::logging::AraComLogger const& logger)
    -> MessageType;

/*!
 * \brief Return the message type from the given value.
 * \param[in] type_value A value represents a message type.
 * \param[in] logger Reference to the logger to be used.
 * \return The corresponding message type.
 * \pre -
 * \context App | Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 */
auto GetIpcPacketType(std::underlying_type_t<MessageType> type_value,
                      amsr::someip_daemon_client::internal::logging::AraComLogger const& logger) -> MessageType;

/*!
 * \brief Tests whether the given message type is a response to one of the control message types.
 * \param[in] message_type A message type.
 * \return True if the given message type is a response to one of the control message types and false otherwise.
 * \pre -
 * \context App | Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 */
auto IsControlMessageResponse(MessageType message_type) -> bool;

/*!
 * \brief Tests whether the given message type is a routing message.
 * \param[in] message_type A message type.
 * \return True if the given message type is Routing and false otherwise.
 * \pre -
 * \context Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 */
auto IsRoutingMessage(MessageType message_type) -> bool;

/*!
 * \brief Tests whether the given message type is a routing SOME/IP message.
 * \param[in] message_type A message type.
 * \return True if the given message type is SOME/IP and false otherwise.
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
 * - Verify if the passed message type is routing SOME/IP message.
 * \endinternal
 */
inline auto IsRoutingSomeIpMessage(MessageType message_type) -> bool { return (message_type == MessageType::kSomeIp); }

/*!
 * \brief Tests whether the given message type is a routing SOME/IP message with metadata.
 * \param[in] message_type A message type.
 * \return True if the given message type is SOME/IP with metadata and false otherwise.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Verify if the passed message type is routing SOME/IP message with metadata.
 * \endinternal
 */
inline auto IsRoutingSomeIpMessageWithMetaData(MessageType message_type) -> bool {
  return (message_type == MessageType::kSomeIpWithMetaData);
}

/*!
 * \brief Tests whether the given message type is a routing PDU message.
 * \param[in] message_type A message type.
 * \return True if the given message type is PDU and false otherwise.
 * \pre -
 * \context App | Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Verify if the passed message type is routing PDU message.
 * \endinternal
 */
inline auto IsRoutingPduMessage(MessageType message_type) -> bool { return (message_type == MessageType::kPdu); }

/*!
 * \brief Tests whether the given message type is a routing PDU message with metadata.
 * \param[in] message_type A message type.
 * \return True if the given message type is PDU message with metadata and false otherwise.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Verify if the passed message type is routing PDU message with metadata.
 * \endinternal
 */
inline auto IsRoutingPduMessageWithMetaData(MessageType message_type) -> bool {
  return (message_type == MessageType::kPduWithMetaData);
}

/*!
 * \brief Tests whether the given message type is a initial field notification message.
 * \param[in] message_type A message type.
 * \return True if the given message type is initial field notification and false otherwise.
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
 * - Verify if the passed message type is initial field notification message.
 * \endinternal
 */
inline auto IsRoutingInitialFieldNotificationMessage(MessageType message_type) -> bool {
  return (message_type == MessageType::kSomeIpInitialFieldNotification);
}

/*!
 * \brief Return the positive control response message type corresponding to the given request message type.
 * \param[in] message_type A request message type.
 * \return Positive response message type.
 * \pre -
 * \context Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 */
auto GetPositiveResponseMessageTypeFromRequestMessageType(MessageType message_type) -> MessageType;

/*!
 * \brief Return the negative control response message type corresponding to the given request message type.
 * \param[in] message_type A request message type.
 * \return Negative response message type.
 * \pre -
 * \context Reactor
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady FALSE
 * \spec
 *   requires true;
 * \endspec
 */
auto GetNegativeResponseMessageTypeFromRequestMessageType(MessageType message_type) -> MessageType;

/*!
 * \brief Message length
 */
using MessageLength = std::uint32_t;

// ---- Control message header ----

/*!
 * \brief Offset to command specific return codes.
 */
static constexpr std::uint32_t kCommandSpecificReturnCodesOffset{0x80000000U};

/*!
 * \brief Type definition for the control message return code
 */
enum class ControlMessageReturnCode : std::uint32_t {
  kOk = 0x00000000U,                  //< Request / positive response received
  kNotOk = 0x00000001U,               //< Generic error occurred
  kMalformedMessage = 0x00000002U,    //< Malformed message detected
  kUnsupportedVersion = 0x00000003U,  //< Unsupported protocol version detected
  kUnknownMessageType = 0x00000004U,  //< Unknown message type in generic header detected
  kInvalidParam = 0x00000005U,        //< Invalid protocol parameter(s) used
  kTimeOut = 0x00000006U,             //< Timeout with no response

  // ---- Command specific return codes ----
  kLocalServerNotAvailable = kCommandSpecificReturnCodesOffset +
                             0U,  //< RequestLocalServer command: Local server is already owned by another application
  kOfferServiceAccessDenied = kCommandSpecificReturnCodesOffset +
                              1U,  //< OfferService command: application has no right to offer a specific service
  kRequestServiceAccessDenied = kCommandSpecificReturnCodesOffset +
                                2U,  //< RequestService command: application has no right to request a specific service
  kRequestServiceClientIdsOverflow =
      kCommandSpecificReturnCodesOffset +
      3U,  //< RequestService command: application reaches the maximum number of simultaneous client Ids
  kRequestServiceRemoteServerNotFound =
      kCommandSpecificReturnCodesOffset + 4U,  //< RequestService command: Remote Server not found
  kRequestServiceRequiredServiceInstanceNotFound =
      kCommandSpecificReturnCodesOffset + 5U  //< RequestService command: required service instance not found
};

// ---- Control message payload ----
/*!
 * \brief Type definition of the control message client ID.
 */
using ControlMessageClientId = amsr::someip_protocol::internal::ClientId;
/*!
 * \brief Type definition of the control message service ID.
 */
using ControlMessageServiceId = amsr::someip_protocol::internal::ServiceId;
/*!
 * \brief Type definition of the control message instance ID.
 */
using ControlMessageInstanceId = amsr::someip_protocol::internal::InstanceId;
/*!
 * \brief Type definition of the control message major version.
 */
using ControlMessageMajorVersion = amsr::someip_protocol::internal::MajorVersion;
/*!
 * \brief Type definition of the control message minor version.
 */
using ControlMessageMinorVersion = amsr::someip_protocol::internal::MinorVersion;
/*!
 * \brief Type definition of the control message event ID.
 */
using ControlMessageEventId = amsr::someip_protocol::internal::EventId;

// ---- Routing message header ----

/*!
 * \brief Type definition of the routing message service ID.
 */
using RoutingMessageServiceId = amsr::someip_protocol::internal::ServiceId;
/*!
 * \brief Type definition of the routing message instance ID.
 */
using RoutingMessageInstanceId = amsr::someip_protocol::internal::InstanceId;
/*!
 * \brief Type definition of the routing message major version.
 */
using RoutingMessageMajorVersion = amsr::someip_protocol::internal::MajorVersion;
/*!
 * \brief Type definition of the routing message minor version.
 */
using RoutingMessageMinorVersion = amsr::someip_protocol::internal::MinorVersion;
/*!
 * \brief Type definition of the routing message event ID.
 */
using RoutingMessageEventId = amsr::someip_protocol::internal::EventId;
/*!
 * \brief Type definition of the routing message subscription state.
 */
using RoutingMessageSubscriptionState = amsr::someip_protocol::internal::SubscriptionState;
/*!
 * \brief Type definition of the control message client ID.
 */
using RoutingMessageClientId = amsr::someip_protocol::internal::ClientId;

}  // namespace internal
}  // namespace someipd_app_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_PROTOCOL_TYPES_H_

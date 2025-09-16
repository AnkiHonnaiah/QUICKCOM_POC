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
/**        \file someip-protocol/internal/someip_posix_types.h
 *        \brief General SOME/IP types which can be used across component boundaries.
 *        \unit  SomeIpProtocol::Messages::SomeipPosixTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_POSIX_TYPES_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_POSIX_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include <memory>
#include <utility>
#include "ara/core/array.h"
#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_protocol {
namespace internal {

/*!
 * \brief Type definition for the PDU ID.
 */
using PduId = std::uint32_t;

/*!
 * \brief Type definition for the SOME/IP Service ID
 * \trace SPEC-4981282
 * \vprivate Product private
 */
using ServiceId = std::uint16_t;

/*!
 * \brief Type definition for the SOME/IP Instance ID
 * \vprivate Product private
 */
using InstanceId = std::uint16_t;

/*!
 * \brief Type definition for the SOME/IP Client ID
 * \trace SPEC-4981287
 * \trace SPEC-4981288
 * \vprivate Product private
 */
using ClientId = std::uint16_t;

/*!
 * \brief Type definition for the SOME/IP Session ID
 * \trace SPEC-4981287
 * \vprivate Product private
 */
using SessionId = std::uint16_t;

/*!
 * \brief Type definition for the SOME/IP Event ID
 * \trace SPEC-4981307
 * \vprivate Product private
 */
using EventId = std::uint16_t;

/*!
 * \brief Type definition for the SOME/IP Eventgroup ID
 * \vprivate Product private
 */
using EventgroupId = std::uint16_t;

/*!
 * \brief Type definition for the SOME/IP Method ID
 * \trace SPEC-4981281
 * \vprivate Product private
 */
using MethodId = std::uint16_t;

/*!
 * \brief Type definition for the SOME/IP MessageType
 * \trace SPEC-4981303
 * \vprivate Product private
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
using MessageType = std::uint8_t;

/*!
 * \brief Type definition for the SOME/IP ReturnCode
 * \trace SPEC-4981469
 * \vprivate Product private
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
using ReturnCode = std::uint8_t;

/*!
 * \brief Type definition for the SOME/IP ProtocolVersion
 * \trace SPEC-4981299
 * \vprivate Product private
 */
using ProtocolVersion = std::uint8_t;

/*!
 * \brief Type definition for the SOME/IP InterfaceVersion
 * \trace SPEC-4981302
 * \vprivate Product private
 */
using InterfaceVersion = std::uint8_t;

/*!
 * \brief Major version used for the SOME/IP deployment.
 * \vprivate Product private
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
using MajorVersion = std::uint8_t;

/*!
 * \brief Minor version used for the SOME/IP messages.
 * \vprivate Product private
 */
using MinorVersion = std::uint32_t;

/*!
 * \brief Length definition for the SOME/IP header
 * \trace SPEC-4981283
 * \vprivate Product private
 */
using LengthField = std::uint32_t;

/*!
 * \brief Type-alias for an IP address.
 *        Can be later exchanged with a specific static / safe string type.
 * \vprivate Product private
 */
using IpAddress = ara::core::String;

/*!
 * \brief Type-alias for an IP address prefix length.
 * \vprivate Product private
 */
using IpAddressPrefixLength = std::uint32_t;

/*!
 * \brief Type definition for the SOME/IP Port
 * \vprivate Product private
 */
using Port = std::uint16_t;

/*!
 * \brief pair of IP address and port.
 * \vprivate Product private
 */
using AddressPair = std::pair<IpAddress, Port>;

/*!
 * \brief Type-alias for the SOME/IP delay of retrying subscription of an eventgroup in milliseconds.
 * \vprivate Product private
 */
using SubscribeEventgroupRetryDelay = std::chrono::milliseconds;

/*!
 * \brief Type-alias for the SOME/IP subscribe eventgroup maximum retries.
 * \vprivate Product private
 */
using SubscribeEventgroupRetryMax = std::uint8_t;

/*!
 * \brief Type-alias for the SOME/IP TTL
 * \vprivate Product private
 */
using Ttl = std::uint32_t;

/*!
 * \brief Alias for type within service discovery entry.
 * \vprivate Product private
 */
using SdEntryType = std::uint8_t;

/*!
 * \brief Alias for option indexing within service discovery entry.
 * \vprivate Product private
 */
using SdEntryOptionIndex = std::uint8_t;

/*!
 * \brief Alias for option count within service discovery entry.
 * \vprivate Product private
 */
using SdEntryOptionCount = std::uint8_t;

/*!
 * \brief Type-alias for the SOME/IP SD Entry Counter type (uint4)
 * \vprivate Product private
 */
using SdEntryCounter = std::uint8_t;

/*!
 * \brief Type-alias for the SOME/IP SD Flag type
 * \vprivate Product private
 */
using SdFlag = std::uint8_t;

/*!
 * \brief Alias for SD entries length field.
 * \trace SPEC-4981517
 * \vprivate Product private
 */
using SdEntriesLength = std::uint32_t;

/*!
 * \brief Alias for SD options length field.
 * \trace SPEC-4981517
 * \vprivate Product private
 */
using SdOptionsLength = std::uint32_t;

/*!
 * \brief Alias for SD entry index option field.
 * \vprivate Product private
 */
using SdEntryIndexOption = std::uint8_t;

/*!
 * \brief Alias for SD entry number option field.
 * \vprivate Product private
 */
using SdEntryNumberOption = std::uint8_t;

/*!
 * \brief Alias for Socket Channel Id
 * \vprivate Product private
 */
using ChannelId = std::int32_t;

/*!
 * \brief Value type for the QoS priority.
 * \details QoS is an int variable in the socket API. But in the IP packet 'TOS' is only 8-bit.
 * \vprivate Product private
 */
using QoSPriority = std::uint8_t;

/*!
 * \brief QoSOption configuration.
 * \vprivate Product private
 */
using QoSOption = ara::core::Optional<QoSPriority>;

/*!
 * \brief Type-alias for KeepAlive retry count socket option.
 * \details int used by the socket API. Logic value range is >=1. Maximum value depends on OS.
 * \vprivate Product private
 */
using KeepAliveRetryCount = std::int32_t;

/*!
 * \brief Time stamp Type definition.
 * \vprivate Product private
 */
using TimeStampClock = std::chrono::system_clock;

/*!
 * \brief Time stamp Type definition.
 * \vprivate Product private
 */
using TimeStamp = TimeStampClock::time_point;

/*!
 * \brief Size of time stamp type.
 * \vprivate Product private
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
constexpr std::size_t kTimeStampSize{sizeof(TimeStampClock::time_point)};

/*!
 * \brief KeepAlive parameters for TCP sockets. These parameters can be set via setsocketopt() POSIX API.
 * \vprivate Product private
 */
struct KeepAliveParameters {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPPROTOCOL_V11.0.3_publicDataMembers
  /*!
   * \brief Compares two KeepAliveParameters objects.
   * \param[in] other The instance to compare with.
   * \return true if all parameters are the same, false if one of them differ.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */
  inline bool operator==(KeepAliveParameters const& other) const noexcept {
    return (keep_alive_time_ == other.keep_alive_time_) && (keep_alive_interval_ == other.keep_alive_interval_) &&
           (keep_alive_retry_count_ == other.keep_alive_retry_count_);
  }

  /*!
   * \brief The duration between two keepalive transmissions in idle condition in seconds.
   * By default this is set to 2 hours.
   */
  ara::core::Optional<std::chrono::seconds> keep_alive_time_;

  /*!
   * \brief The time (in seconds) between individual keepalive probes.
   */
  ara::core::Optional<std::chrono::seconds> keep_alive_interval_;

  /*! \brief Number of retransmissions to be carried out before declaring that remote end is not available. */
  ara::core::Optional<KeepAliveRetryCount> keep_alive_retry_count_;
};

/*!
 * \brief Flag to enable Socket Option Linger.
 * \vprivate Product private
 */
using EnableSocketOptionLinger = bool;

/*!
 * \brief An optional of TCP socket Ligner Option setting.
 * \vprivate Product private
 */
using EnableSocketOptionLingerOptional = ara::core::Optional<EnableSocketOptionLinger>;

/*!
 * \brief TcpKeepAliveOption configuration.
 * \vprivate Product private
 */
using TcpKeepAliveOption = ara::core::Optional<KeepAliveParameters>;

/*!
 * \brief Flag for TCP Nagle socket setting.
 * \vprivate Product private
 */
using SocketOptionTcpNaglesAlgorithm = bool;

/*!
 * \brief An optional of TCP Nagle socket setting.
 * \vprivate Product private
 */
using SocketOptionTcpNaglesAlgorithmOptional = ara::core::Optional<SocketOptionTcpNaglesAlgorithm>;

/*!
 * \brief Type definition for Bulk read receive period [us].
 * \vprivate Product private
 */
using BulkReadReceivePeriodUsType = std::uint32_t;

/*!
 * \brief Bulk read receive period [us] type.
 * \vprivate Product private
 */
using BulkReadReceivePeriodUs = ara::core::Optional<BulkReadReceivePeriodUsType>;

/*!
 * \brief Value type of "Bulk read max number of consecutive receive calls"
 * \vprivate Product private
 */
using BulkReadMaxNoOfConsecutiveReceiveCalls = std::uint16_t;

/*!
 * \brief Optional for "Bulk read max number of consecutive receive calls"
 * \vprivate Product private
 */
using BulkReadMaxNoOfConsecutiveReceiveCallsOptional = ara::core::Optional<BulkReadMaxNoOfConsecutiveReceiveCalls>;

/*!
 * \brief Value type of "Bulk read min number to continue consecutive receive calls"
 * \vprivate Product private
 */
using BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls = std::uint16_t;

/*!
 * \brief Optional for "Bulk read min number to continue consecutive receive calls"
 * \vprivate Product private
 */
using BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsOptional =
    ara::core::Optional<BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls>;

/*!
 * \brief Type definition for the counter thresholds to decide if a TCP connection must be re-established.
 * \vprivate Product private
 */
using TcpMessageCounterThresholdType = std::uint16_t;

/*!
 * \brief     Options of a socket combined in a structure.
 * \details   Possible options are
 *            - QoS
 *            - KeepAlive
 *            - Socket Option Linger
 *            - Socket Option Tcp Nagle
 *            - Socket Option Bulk read receive period [us]
 * \vprivate  Product private
 */
struct SocketOptions {
  /*!
   * \brief Socket QoS option
   */
  QoSOption qos_;
  /*!
   * \brief KeepAlive option for TCP sockets.
   */
  TcpKeepAliveOption keep_alive_;
  /*!
   * \brief Enables Socket option lingering for TCP sockets.
   * \details Is is disenabled by default.
   */
  EnableSocketOptionLingerOptional enable_so_linger_{ara::core::nullopt};
  /*!
   * \brief Enables Socket Option Nagle's algorithm TCP sockets.
   * \details It is not present on port level by default.
   */
  SocketOptionTcpNaglesAlgorithmOptional enable_tcp_nagle_{ara::core::nullopt};
  /*!
   * \brief Socket bulk read receive period us option
   * \details It is not present on port level by default.
   */
  BulkReadReceivePeriodUs bulk_read_receive_period_us_{ara::core::nullopt};
  /*!
   * \brief Threshold for invalid messages received before disconnecting the TCP connection.
   */
  TcpMessageCounterThresholdType no_of_invalid_tcp_messages_before_disconnection_{0U};
  /*!
   * \brief Number of consecutive valid messages that must be received to reset the error counter.
   */
  TcpMessageCounterThresholdType no_of_consecutive_valid_tcp_messages_before_error_reset_{0U};
};

/*!
 * \brief Type definition for subscription state of an event
 * \vprivate Product private
 */
enum class SubscriptionState : uint8_t {
  kSubscribed = 0U,         /*! \brief < Subscribed to event */
  kNotSubscribed = 1U,      /*! \brief < Not subscribed to event */
  kSubscriptionPending = 2U /*! \brief < Event subscription is pending */
};

/*!
 * \brief Service instance.
 * \vprivate Product private
 */
struct ServiceInstance {
  /*!
   * \brief Service ID of a SOME/IP service
   */
  ServiceId service_id_;
  /*!
   * \brief Instance ID of a SOME/IP service
   */
  InstanceId instance_id_;

  /*!
   * \brief Major version of a SOME/IP service.
   */
  MajorVersion major_version_;

  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPPROTOCOL_V11.0.3_publicDataMembers
  /*!
   * \brief Compares two ServiceInstance objects.
   * \param[in] other The instance to compare with.
   * \return true if if condition is met, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */
  inline bool operator<(ServiceInstance const& other) const noexcept {
    return (std::tie(service_id_, instance_id_, major_version_) <
            std::tie(other.service_id_, other.instance_id_, other.major_version_));
  }
};

/*!
 * \brief Container for service instances.
 * \vprivate Product private
 */
using ServiceInstanceContainer = ara::core::Vector<ServiceInstance>;

/*!
 * \brief Service discovery event subscription state entry.
 * \vprivate Product private
 */
struct EventSubscriptionState {
  /*!
   * \brief SOME/IP service identifier
   */
  ServiceId service_id_;

  /*!
   * \brief SOME/IP service instance identifier
   */
  InstanceId instance_id_;

  /*!
   * \brief SOME/IP service major version
   */
  MajorVersion major_version_;

  /*!
   * \brief SOME/IP event identifier
   */
  EventId event_id_;

  /*!
   * \brief An event subscription state
   */
  SubscriptionState state_;
};

/*!
 * \brief Represents a SOME/IP message.
 * \vprivate Product private
 */
using PacketBuffer = ara::core::Vector<std::uint8_t>;

/*!
 * \brief Represents a unique pointer to a SOME/IP message.
 * \vprivate Product private
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
using SomeIpPacket = std::unique_ptr<PacketBuffer>;

/*!
 * \brief Represents a unique pointer to a PDU message.
 * \vprivate Product private
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
using PduPacket = std::unique_ptr<PacketBuffer>;

/*!
 * \brief Maximum transmission unit.
 * \vprivate Product private
 */
using MTU = std::uint32_t;

}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_POSIX_TYPES_H_

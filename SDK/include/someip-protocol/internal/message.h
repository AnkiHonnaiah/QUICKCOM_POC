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
/**        \file someip-protocol/internal/message.h
 *        \brief Provides the SOME/IP protocol type and constant definitions.
 *      \unit    SomeIpProtocol::Messages::Message
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstddef>
#include <limits>
#include "ara/core/array.h"
#include "ara/core/optional.h"
#include "ara/log/logger.h"
#include "ara/log/logstream.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip-protocol/internal/someip_protocol_error_code.h"

namespace amsr {
namespace someip_protocol {
namespace internal {

/*!
 * SOME/IP SD message looks as follows:
 *    +-----------------------------+
 *    |   SOME/IP Header            |
 *    +-----------------------------+
 *    |   SOME/IP SD Header         |
 *    +-----------------------------+
 *    |   Entries Length Field      |
 *    +-----------------------------+
 *    |   List of Entries           |
 *    +-----------------------------+
 *    |   Options Length Field      |
 *    +-----------------------------+
 *    |   List of Options           |
 *    +-----------------------------+
 */

/*!
 * SOME/IP SD Header looks as follows:
 *    +-----------------------------+
 *    |   flags                     |
 *    +-----------------------------+
 *    |   reserved                  |
 *    +-----------------------------+
 */

/*!
 * SOME/IP SD Entry looks as follows:
 *    +-----------------------------+
 *    |   Generic Entry Header      |
 *    +-----------------------------+
 *    |   Payload                   |
 *    +-----------------------------+
 */

/*!
 * SOME/IP SD Option looks as follows:
 *    +-----------------------------+
 *    |   Generic Option Header     |
 *    +-----------------------------+
 *    |   Payload                   |
 *    +-----------------------------+
 */

/*!
 * \brief SOME/IP message types.
 * \vprivate Product private
 *
 * \trace SPEC-4981303
 * \trace SPEC-4981305
 * \trace SPEC-4981306
 * \trace SPEC-10144460
 * \trace SPEC-10144530
 */
enum class SomeIpMessageType : std::uint8_t {
  kRequest = 0x00U,           /*!< A request expecting a response */
  kRequestNoReturn = 0x01U,   /*!< A 'fire & forget' request */
  kNotification = 0x02U,      /*!< An event expecting no response */
  kResponse = 0x80U,          /*!< A response message */
  kError = 0x81U,             /*!< A response containing an error */
  kTpRequest = 0x20U,         /*!< A SOME/IP-TP request expecting a response */
  kTpRequestNoReturn = 0x21U, /*!< A SOME/IP-TP 'fire & forget' request */
  kTpNotification = 0x22U,    /*!< A SOME/IP-TP event expecting no response */
  kTpResponse = 0xA0U,        /*!< A SOME/IP-TP response message */
  kTpError = 0xA1U            /*!< A SOME/IP-TP response containing an error */
};

/*!
 * \brief SOME/IP return codes.
 * \vprivate Product private
 *
 * \trace SPEC-4981470
 * \trace SPEC-4981306
 * \trace SPEC-10144532
 */
enum class SomeIpReturnCode : std::uint8_t {
  kOk = 0x00U,                    /*!< No error occurred. */
  kNotOk = 0x01U,                 /*!< An unspecified error occurred, or client id is not 0 for event notification. */
  kUnknownService = 0x02U,        /*!< The requested service id is unknown. */
  kUnknownMethod = 0x03U,         /*!< The requested method id is unknown. */
  kNotReady = 0x04U,              /*!< Service id and method id are known. Application not running. */
  kNotReachable = 0x05U,          /*!< IAM denied sending a method request. */
  kWrongProtocolVersion = 0x07U,  /*!< Version of SOME/IP protocol not supported. */
  kWrongInterfaceVersion = 0x08U, /*!< Wrong interface version. */
  kMalformedMessage = 0x09U,      /*!< Deserialization error, so that payload cannot be deserialized. */
  kWrongMessageType =
      0x0AU, /*!< An unexpected message type was received (e.g. REQUEST_NO_RETURN for a method defined as REQUEST). */
  kRangeServiceErrorsStart = 0x20U, /*!< Reserved range start for service and method specific errors. */
  kRangeServiceErrorsEnd = 0x5EU,   /*!< Reserved range end for service and method specific errors. */
};

/*!
 * \brief SOME/IP message header.
 * \vprivate Product private
 * \trace SPEC-4981278
 * \trace SPEC-4981279
 * \trace SPEC-4981280
 * \trace SPEC-4981282
 * \trace SPEC-4981283
 * \trace SPEC-4981287
 * \trace SPEC-4981288
 * \trace SPEC-4981486
 * \trace SPEC-4981487
 * \trace SPEC-4981488
 * \trace SPEC-4981489
 * \trace SPEC-4981490
 * \trace SPEC-4981492
 * \trace SPEC-4981497
 * \trace SPEC-4981498
 * \trace SPEC-4981499
 * \trace SPEC-4981500
 * \trace SPEC-10144436
 * \trace SPEC-10144490
 * \trace SPEC-10144513
 */
struct SomeIpMessageHeader {
  ServiceId service_id_; /*!< A SOME/IP service identifier */
  MethodId method_id_;   /*!< A SOME/IP method or event identifier */
  LengthField length_;   /*!< Contains the length of a SOME/IP message in bytes starting from Client ID */
  ClientId client_id_;   /*!< A unique identifier which allows differentiating multiple clients of the same ECU */
  SessionId session_id_; /*!< A unique identifier which allows differentiating multiple calls to the same method */
  ProtocolVersion protocol_version_;   /*!< A SOME/IP protocol version */
  InterfaceVersion interface_version_; /*!< The major version of a SOME/IP service interface */
  SomeIpMessageType message_type_;     /*!< A SOME/IP message type */
  SomeIpReturnCode return_code_;       /*!< A return code which signal whether a request was successful */
};

/*!
 * \brief           Support for logging a SomeIpMessageHeader.
 * \param[in, out]  s The log stream written to.
 * \param           header A SOME/IP message header.
 * \pre             -
 * \context         ANY
 * \vprivate        Product private
 * \spec
 *   requires true;
 * \endspec
 */
static inline void LogSomeIpMessageHeader(ara::log::LogStream& s, SomeIpMessageHeader const& header) noexcept {
  s << "SOME/IP Message header [";
  s << "Service ID: 0x";
  s << ara::log::HexFormat(header.service_id_);
  s << ", ";
  s << "Method ID: 0x";
  s << ara::log::HexFormat(header.method_id_);
  s << ", ";
  s << "Length: ";
  s << header.length_;
  s << " bytes, ";
  s << "Client ID: 0x";
  s << ara::log::HexFormat(header.client_id_);
  s << ", ";
  s << "Session ID: 0x";
  s << ara::log::HexFormat(header.session_id_);
  s << ", ";
  s << "Protocol version: 0x";
  s << ara::log::HexFormat(header.protocol_version_);
  s << ", ";
  s << "Interface version: 0x";
  s << ara::log::HexFormat(header.interface_version_);
  s << ", ";
  s << "Message type: 0x";
  s << ara::log::HexFormat(static_cast<std::uint8_t>(header.message_type_));
  s << ", ";
  s << "Return code: 0x";
  s << ara::log::HexFormat(static_cast<std::uint8_t>(header.return_code_));
  s << "] ";
}

/*!
 * \brief Meta data of the message.
 * \vprivate Product private
 */
struct MetaData {
  ara::core::Optional<TimeStamp> rx_time_stamp; /*!< A time stamp for when the message was received */
  ara::core::Optional<std::chrono::nanoseconds>
      tx_message_accumulation_timeout; /*!< Transmission accumulation timeout for a message */
};

/*!
 * \brief PDU message header.
 * \vprivate Product private
 */
struct PduMessageHeader {
  PduId pdu_id_;       /*!< A PDU identifier */
  LengthField length_; /*!< Contains the length of a PDU message payload in bytes */
};

/*!
 * \brief The utility functions for PduMessageHeader.
 * \vprivate Product private
 * \unit SomeIpProtocol::Messages::PduMessage
 */
class PduMessageHeaderUtility final {
 public:
  /*!
   * \brief     Get the service id.
   * \param[in] header The PDU message header.
   * \return    The service id.
   * \pre       SetServiceId must have been called correctly beforehand.
   * \context   App
   * \spec
   *   requires true;
   * \endspec
   */
  static inline ServiceId GetServiceId(PduMessageHeader const& header) noexcept {
    return static_cast<std::uint16_t>((header.pdu_id_ >> 16U) & 0xFFFFu);
  }

  /*!
   * \brief     Get the method id.
   * \param[in] header The PDU message header.
   * \return    The method id.
   * \pre       SetMethodId must have been called correctly beforehand.
   * \context   App
   * \spec
   *   requires true;
   * \endspec
   */
  static inline MethodId GetMethodId(PduMessageHeader const& header) noexcept {
    return static_cast<std::uint16_t>(header.pdu_id_ & 0xFFFFu);
  }

  /*!
   * \brief       Set the service id.
   * \param[in]   serviceId The service id for setting.
   * \param[out]  header The header object to be set.
   * \pre         -
   * \context     App
   * \spec
   *   requires true;
   * \endspec
   */
  static inline void SetServiceId(ServiceId const serviceId, PduMessageHeader& header) noexcept {
    header.pdu_id_ = ((static_cast<std::uint32_t>(serviceId) << 16U) & 0xFFFF0000u) | (header.pdu_id_ & 0x0000FFFFu);
  }

  /*!
   * \brief       Set the method id.
   * \param[in]   methodId The method id for setting.
   * \param[out]  header The header object to be set.
   * \pre         -
   * \context     App
   * \spec
   *   requires true;
   * \endspec
   */
  static inline void SetMethodId(MethodId const methodId, PduMessageHeader& header) noexcept {
    header.pdu_id_ = (header.pdu_id_ & 0xFFFF0000u) | (static_cast<std::uint32_t>(methodId) & 0x0000FFFFu);
  }
};

/*!
 * \brief PDU header size - represents the full size including the PDU ID and the length field.
 * \vprivate Product private
 */
static constexpr std::size_t kPduHeaderSize{sizeof(ServiceId) + sizeof(MethodId) + sizeof(LengthField)};

/*!
 * \brief     SOME/IP header size - represents the full size including the service ID,
 *            method ID and the length field.
 * \vprivate  Product private
 * \trace     SPEC-4981279
 */
static constexpr std::size_t kHeaderSize{sizeof(SomeIpMessageHeader)};

/*!
 * \brief Slightly pedantic, but to prevent from unwanted alignment -
 *        even if the SOME/IP header is made exactly in a size no alignment takes place,
 *        we never know what a specific compiler does.
 * \trace SPEC-4981279
 */
static_assert(kHeaderSize == 16U, "sizeof(SomeIpMessageHeader) does not meet specification.");

/*!
 * \brief Default SOME/IP protocol version.
 * \vprivate Product private
 * \trace SPEC-4981301
 * \trace SPEC-10144311
 */
static ProtocolVersion const kProtocolVersion{0x01U};

/*!
 * \brief Minimum SOME/IP length value from the SOME/IP header (for a SOME/IP message without payload).
 * \vprivate Product private
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static constexpr LengthField kMinimumPayloadLength{8U};

/*!
 * \brief SOME/IP SD service identifier.
 * \vprivate Product private
 * \trace SPEC-4981487
 * \trace SPEC-10144518
 */
static ServiceId const kSdServiceId{0xFFFFU};

/*!
 * \brief SOME/IP SD method identifier.
 * \vprivate Product private
 * \trace SPEC-4981488
 * \trace SPEC-10144518
 */
static MethodId const kSdMethodId{0x8100U};

/*!
 * \brief SOME/IP SD client identifier.
 * \vprivate Product private
 * \trace SPEC-4981490
 * \trace SPEC-10144520
 * */
static ClientId const kSdClientId{0x0000U};
/*!
 * \brief Minimum length of an (empty) SOME/IP-SD message.
 * \vprivate Product private
 * \trace SPEC-4981485
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static constexpr LengthField kSdMinimumMessageLength{12U};

/*!
 * \brief SOME/IP SD protocol version.
 * \vprivate Product private
 * \trace SPEC-4981485
 * \trace SPEC-4981497
 * \trace SPEC-10144528
 */
static ProtocolVersion const kSdProtocolVersion{0x01U};

/*!
 * \brief SOME/IP SD interface version.
 * \vprivate Product private
 * \trace SPEC-4981485
 * \trace SPEC-4981498
 * \trace SPEC-10144529
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static InterfaceVersion const kSdInterfaceVersion{0x01U};

/*!
 * \brief Wildcard for service instance identifier.
 * \vprivate Product private
 * \trace SPEC-4981719
 */
static InstanceId const kInstanceIdAll{0xFFFFU};

/*!
 * \brief Total length of SOME/IP header fields message identifier and length.
 * \vprivate Product private
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static constexpr std::size_t kHeaderLength{8U};

/*!
 * \brief The offset of the length field in the SOME/IP header.
 * \vprivate Product private
 */
static std::size_t const kLengthFieldOffset{4U};

/*!
 * \brief Wildcard for the major version of a SOME/IP service.
 * \vprivate Product private
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static std::uint8_t const kMajorVersionAny{0xFFU};
/*!
 * \brief Wildcard for the minor version of a SOME/IP service.
 * \vprivate Product private
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static std::uint32_t const kMinorVersionAny{0xFFFFFFFFU};

/*!
 * \brief SOME/IP SD flags.
 * \vprivate Product private
 * \trace SPEC-4981502
 * \trace SPEC-4981503
 * \trace SPEC-4981504
 * \trace SPEC-4981508
 */
enum class SomeIpSdFlag : std::uint8_t {
  kReboot = 0x80U, /*! \brief < Reboot flag */
  kUnicast = 0x40U /*! \brief < Unicast flag */
};

/*!
 * \brief SOME/IP SD entry types.
 * \vprivate Product private
 *
 * \trace SPEC-4981519
 * \trace SPEC-4981591
 * \trace SPEC-10144775
 * \trace SPEC-10144619
 * \trace SPEC-10144638
 */
enum class SomeIpSdEntryType : std::uint8_t {
  kFindService = 0x00U,            /*! \brief < FindServiceEntry */
  kOfferService = 0x01U,           /*! \brief < OfferServiceEntry or StopOfferServiceEntry */
  kSubscribeEventgroup = 0x06U,    /*! \brief < SubscriveEventgroupEntry or StopSubscribeEventgroupEntry */
  kSubscribeEventgroupAck = 0x07U, /*! \brief < SubscriveEventgroupAckEntry or StopSubscribeEventgroupNackEntry */
};

/*!
 * \brief Shift value for number of 1st options in SOME/IP SD entry.
 * \vprivate Product private
 */
static std::size_t const kSomeIpSdEntryNumberFirstOptsShift{4};
/*!
 * \brief Mask value for number of options in SOME/IP SD entry.
 * \vprivate Product private
 */
static std::size_t const kSomeIpSdEntryNumberOptsMask{0xf};
/*!
 * \brief Shift value for major version in SOME/IP SD entry.
 * \vprivate Product private
 */
static std::size_t const kSomeIpSdEntryMajorVersionShift{24};
/*!
 * \brief Mask value for major version in SOME/IP SD entry.
 * \vprivate Product private
 */
static std::size_t const kSomeIpSdEntryMajorVersionMask{0xff};
/*!
 * \brief Mask value for TTL in SOME/IP SD entry.
 * \vprivate Product private
 */
static std::size_t const kSomeIpSdEntryTtlMask{0xffffff};

/*!
 * \brief SOME/IP SD FindService entry.
 * \vprivate Product private
 * \trace SPEC-4981520
 * \trace SPEC-4981521
 * \trace SPEC-4981522
 * \trace SPEC-10144513
 * \trace SPEC-10144618
 * \trace SPEC-10144637
 */
struct SomeIpSdEntryHeader {
  /*!
   * \brief Entry type
   */
  std::uint8_t type_;

  /*!
   * \brief First options index
   */
  std::uint8_t index_1st_opts_;

  /*!
   * \brief Second options index
   */
  std::uint8_t index_2nd_opts_;

  /*!
   * \brief Number of first options in most significant four bits, number of second options in least significan four
   *        bits
   */
  std::uint8_t number_of_opts_;
};

/*!
 * \brief SOME/IP SD findService/OfferService/StopOfferService entry payload.
 * \vprivate Product private
 * \trace SPEC-4981514
 * \trace SPEC-4981520
 * \trace SPEC-4981521
 * \trace SPEC-4663355
 * \trace SPEC-4663356
 * \trace SPEC-4663357
 * \trace SPEC-4663367
 * \trace SPEC-4663481
 * \trace SPEC-10144618
 * \trace SPEC-10144637
 */
struct SomeIpSdServiceEntryPayload {
  /*!
   * \brief SOME/IP service identifier
   */
  ServiceId service_id_;

  /*!
   * \brief SOME/IP instance identifier
   */
  InstanceId instance_id_;

  /*!
   * \brief Major version and TTL
   */
  std::uint32_t major_version_ttl_;

  /*!
   * \brief Minor version of SOME/IP service
   */
  std::uint32_t minor_version_;
};

/*!
 * \brief     SOME/IP SD SubscribeEventgroup/StopSubscribeEventgroup/SubscribeEventgroupAck/StopSubscribeEventgroupNack
 *            entry payload.
 * \vprivate  Product private
 * \trace     SPEC-4981522
 */
struct SomeIpSdEventgroupEntryPayload {
  /*!
   * \brief SOME/IP service identifier
   */
  ServiceId service_id_;

  /*!
   * \brief SOME/IP instance identifier
   */
  InstanceId instance_id_;

  /*!
   * \brief Major version and TTL
   */
  std::uint32_t major_version_ttl_;

  /*!
   * \brief Reserved
   */
  std::uint8_t reserved_;

  /*!
   * \brief Counter that allows for telling apart simultaneous subscriptions to the same event group.
   */
  std::uint8_t counter_;

  /*!
   * \brief Event group identifier
   */
  std::uint16_t eventgroup_id_;
};

/*!
 * \brief Discardable option flag.
 * \vprivate Product private
 * \trace SPEC-4981527
 */
using OptionFlags = std::uint8_t;

/*!
 * \brief Discardable option flag.
 * \vprivate Product private
 * \trace SPEC-4981527
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static OptionFlags const kSomeIpSdOptionFlagDiscardable{0x1U};

/*!
 * \brief Size of individual sd option header.
 * \vprivate Product private
 * \trace SPEC-4981527
 */
constexpr std::size_t kSdOptionHeaderSize{4U};

/*!
 * \brief Size of a SOME/IP SD IPv4 unicast/multicast endpoint option's payload.
 * \vprivate Product private
 */
constexpr std::size_t kSomeIpSdIPv4EndpointOptionPayloadSize{0x0008U};

/*!
 * \brief Value of length field in a SOME/IP SD IPv4 unicast/multicast endpoint option.
 * \vprivate Product private
 */
constexpr std::size_t kSomeIpSdIPv4EndpointOptionLength{kSomeIpSdIPv4EndpointOptionPayloadSize + sizeof(OptionFlags)};

/*!
 * \brief Size of a SOME/IP SD IPv6 unicast/multicast endpoint option's payload.
 * \vprivate Product private
 */
constexpr std::size_t kSomeIpSdIPv6EndpointOptionPayloadSize{0x0014};

/*!
 * \brief Value of length field in a SOME/IP SD IPv6 unicast/multicast endpoint option.
 * \vprivate Product private
 * \trace SPEC-10144558
 * \trace SPEC-10144573
 */
constexpr std::size_t kSomeIpSdIPv6EndpointOptionLength{kSomeIpSdIPv6EndpointOptionPayloadSize + sizeof(OptionFlags)};

/*!
 * \brief SOME/IP SD endpoint option header.
 * \vprivate Product private
 * \trace SPEC-4981516
 * \trace SPEC-4981527
 * \trace SPEC-4981528
 */
struct SomeIpSdEndpointOptionHeader {
  /*!
   * \brief Length in bytes of endpoint option
   */
  std::uint16_t length_;

  /*!
   * \brief Type of endpoint option
   */
  std::uint8_t type_;

  /*!
   * \brief   Option header flags.
   * \details Bit 0: Option is discardable.
   *          Bits 1-7: reserved bits - set and expect them to (be) zero!
   * \trace SPEC-4981527
   */
  OptionFlags flags_;
};

/*!
 * \brief Determine if an option is discardable.
 * \trace SPEC-8054086
 *
 * \param header The header of the option.
 * \return True if the option is discardable, otherwise: false.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Product private
 * \spec
 *   requires true;
 * \endspec
 */
inline bool IsSomeIpSdEndpointOptionDiscardable(SomeIpSdEndpointOptionHeader const& header) {
  return ((header.flags_ & kSomeIpSdOptionFlagDiscardable) != 0U);
}

/*!
 * \brief SOME/IP SD IPv4 unicast/multicast endpoint option payload.
 * \vprivate Product private
 * \trace SPEC-4981550
 * \trace SPEC-4981564
 */
struct SomeIpSdIPv4EndpointOptionPayload {
  /*!
   * \brief The IPv4 endpoint address.
   */
  ara::core::Array<std::uint8_t, 4> address_;
  /*!
   * \brief Reserved.
   */
  std::uint8_t reserved_;
  /*!
   * \brief The L4 protocol.
   */
  std::uint8_t proto_;
  /*!
   * \brief The L4 port number.
   */
  std::uint16_t port_;
};

/*!
 * \brief SOME/IP SD IPv6 unicast/multicast endpoint option payload.
 *
 * \trace SPEC-4981556
 * \trace SPEC-4981569
 * \trace SPEC-4981570
 */
struct SomeIpSdIPv6EndpointOptionPayload {
  /*!
   * \brief The IPv6 endpoint address.
   */
  ara::core::Array<std::uint8_t, 16> address_;

  /*!
   * \brief Reserved.
   */
  std::uint8_t reserved_;

  /*!
   * \brief The L4 protocol.
   */
  std::uint8_t proto_;

  /*!
   * \brief The L4 port number.
   */
  std::uint16_t port_;
};

/*!
 * \brief Calculates the value of the length field in the SOME/IP message header.
 * \param body_length Length of body.
 * \return The value of the length field.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Product private
 * \spec
 *   requires true;
 * \endspec
 */
inline std::size_t CalculateSomeIpMessageHeaderLengthField(std::size_t body_length) {
  return ((body_length + sizeof(SomeIpMessageHeader)) - kHeaderLength);
}

/*!
 * \brief     Maximum value of SOME/IP SD session ID.After reaching this value, the session ID must set back to
 *            kSomeIpSdMinimumSessionId.
 * \vprivate  Product private
 * \trace     SPEC-10144526
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static constexpr SessionId kSomeIpSdMaximumSessionId{0xFFFFU};

/*!
 * \brief Starting (Minimum) value of SOME/IP SD session identifier.
 * \vprivate Product private
 * \trace SPEC-10144526
 * \trace SPEC-10144521
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.4.1_CanBeDeclaredLocallyInPrimaryFile
static constexpr SessionId kSomeIpSdMinimumSessionId{0x0001U};

/*!
 * \brief Invalid value of SOME/IP SD session identifier.
 * \vprivate Product private
 */
static constexpr SessionId kSomeIpSdInvalidSessionId{0U};

/*!
 * \brief Size of flags (8 bit) and reserved bits (24 bit).
 * \vprivate Product private
 */
constexpr std::size_t kSdHeaderSize{4U};

/*!
 * \brief Size of SD entries length field.
 * \vprivate Product private
 */
constexpr std::size_t kSdLengthOfEntriesFieldSize{sizeof(SdEntriesLength)};

/*!
 * \brief Size of SD options length field.
 * \vprivate Product private
 */
constexpr std::size_t kSdLengthOfOptionsFieldSize{sizeof(SdOptionsLength)};

/*!
 * \brief Size of individual sd entry header (type, index_1st_opts_, index_2nd_opts_ and number_of_opts_).
 * \vprivate Product private
 */
constexpr std::size_t kSdEntryHeaderSize{4U};

/*!
 * \brief Size of individual sd entry payload (ServiceEntryPayload / ventgroupEntryPayload).
 * \vprivate Product private
 */
constexpr std::size_t kSdEntryPayloadSize{12U};

/*!
 * \brief The maximum size of a SD message (header + payload). Corresponds to the UDP MTU.
 * \vprivate Product private
 */
constexpr std::size_t kSdMessageMaxSize{12U};

/*!
 * \brief Maximum SOMEIP SD message size allowed (to be sent in a single UDP PDU)
 * \vprivate Product private
 * \trace SPEC-4981424
 */
constexpr std::size_t kMaxSdMessageSize{1392U};

/*!
 * \brief Maximum SOMEIP SD payload size allowed (to be sent in a single UDP PDU)
 * \vprivate Product private
 * \trace SPEC-4981424
 */
constexpr std::size_t kMaxSdPayloadSize{
    kMaxSdMessageSize - (kHeaderSize + kSdHeaderSize + kSdLengthOfEntriesFieldSize + kSdLengthOfOptionsFieldSize)};

}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_H_

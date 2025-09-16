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
/*!        \file  someip_sd_message_deserializer.h
 *        \brief  Deserializer for SOME/IP service discovery message.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_MESSAGE_DESERIALIZER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_MESSAGE_DESERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/someip_daemon_core/marshalling/deserialization/someip_sd_entries_deserializer.h"
#include "amsr/someip_daemon_core/marshalling/deserialization/someip_sd_options_deserializer.h"
#include "amsr/someip_daemon_core/marshalling/marshalling.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message.h"
#include "someip-protocol/internal/deserialization/deser_someip_header.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace marshalling {

/*!
 * \brief Shorting for amsr::someip_protocol::internal::deserialization.
 */
namespace someip_deserialization = amsr::someip_protocol::internal::deserialization;

/*!
 * \brief ServiceDiscoveryMessage deserialization result, pair of a boolean which specifies whether the deserialization
 * has been successful or not and deserialized ServiceDiscoveryMessage.
 */
using DeserializeServiceDiscoveryMessageResult = std::pair<bool, service_discovery::message::ServiceDiscoveryMessage>;

/*!
 * \brief Deserializer for ServiceDiscoveryMessage.
 */
class SomeIpSdMessageDeserializer {
 public:
  /*!
   * \brief Result of DeserializeSomeIpSdFlags method.
   */
  using DeserializeSomeIpSdFlagsResult = std::pair<bool, someip_protocol::internal::SdFlag>;

  /*!
   * \brief Type alias for serialization buffer size which can easily be exchanged.
   */
  using size_type = std::size_t;

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  /*!
   * \brief Function to perform the deserialization.
   * \details Initial Data Requested Flag inside of Eventgroup Entries is not supported. Bit is always 0.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \param[in] header The someip message header
   * \return A pair of a booleans which specifies whether the deserialization has been successful or not and
   *         deserialized ServiceDiscoveryMessage
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981501
   *
   * \internal
   * - Deserialize the SomeIp SD flags and check if the message is valid.
   *   - Update the current flag.
   * - Otherwise,
   *   - Log an error that the flag field is invalid.
   * - Check if the message is valid.
   *   - Deserialize SomeIp SD entries.
   *   - Update the current message validity status.
   *   - Check if the new message status is valid.
   *     - Update the entries payload.
   *   - Otherwise,
   *     - Log an error indicates failure at parsing the entries.
   * - Check if the message is valid.
   *   - Deserialize the SomeIp SD options.
   *   - Update the current message validity status.
   *   - Check if the new message status is valid.
   *     - Update the options payload.
   *   - Otherwise,
   *     - Log an error indicates failure at parsing the options.
   * - Check if the message is valid.
   *   - Serialize SD message.
   * - Return a pair of the message validity status and the SD message.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeServiceDiscoveryMessageResult DeserializeSomeIpSdMessage(
      someip_deserialization::Reader& reader, someip_protocol::internal::SomeIpMessageHeader const& header) noexcept {
    bool is_message_valid{false};
    someip_protocol::internal::SdFlag flags{};
    service_discovery::message::options::ServiceDiscoveryOptionContainer options{};
    service_discovery::message::entries::ServiceDiscoveryEntryContainer entries{};

    // Parse flags
    DeserializeSomeIpSdFlagsResult const kFlagPair{DeserializeSomeIpSdFlags<Endian>(reader)};
    is_message_valid = kFlagPair.first;
    if (is_message_valid) {
      flags = kFlagPair.second;
    } else {
      someip_daemon_core::logging::AraComLogger const kLogger{
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
          ara::core::StringView{"ServiceDiscoveryMarshalling"}};
      kLogger.LogError(
          [&header](ara::log::LogStream& s) {
            s << "(ClientId: 0x" << ara::log::HexFormat(header.client_id_) << ", SessionId: 0x"
              << ara::log::HexFormat(header.session_id_) << "), flags field invalid, expected at least "
              << someip_protocol::internal::kSdHeaderSize;
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    if (is_message_valid) {
      // Parse entries
      SomeIpSdEntriesDeserializer const kSdEntriesDeserializer{};
      DeserializeSdEntryContainerResult const kEntriesPair{
          kSdEntriesDeserializer.DeserializeSomeIpSdEntries<Endian>(reader)};
      is_message_valid = kEntriesPair.first;
      if (is_message_valid) {
        entries = kEntriesPair.second;
      } else {
        someip_daemon_core::logging::AraComLogger const kLogger{
            someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
            someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
            ara::core::StringView{"ServiceDiscoveryMarshalling"}};
        kLogger.LogError(
            [&header](ara::log::LogStream& s) {
              s << "(ClientId: 0x" << ara::log::HexFormat(header.client_id_) << ", SessionId: 0x"
                << ara::log::HexFormat(header.session_id_) << "), failed to parse entries";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }

    if (is_message_valid) {
      // Parse options
      SomeIpSdOptionsDeserializer const kSdOptionsDeserializer{};
      DeserializeSdOptionContainerResult const kOptionsPair{
          kSdOptionsDeserializer.DeserializeSomeIpSdOptions<Endian>(reader)};
      is_message_valid = kOptionsPair.first;
      if (is_message_valid) {
        options = kOptionsPair.second;
      } else {
        someip_daemon_core::logging::AraComLogger const kLogger{
            someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
            someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
            ara::core::StringView{"ServiceDiscoveryMarshalling"}};
        kLogger.LogError(
            [&header](ara::log::LogStream& s) {
              s << " (ClientId: 0x" << ara::log::HexFormat(header.client_id_) << ", SessionId: 0x"
                << ara::log::HexFormat(header.session_id_) << "), failed to parse options";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }

    service_discovery::message::ServiceDiscoveryMessage sd_message{
        {0U, 0U, 0U, 0U, 0U, 0U, 0U, someip_protocol::internal::SomeIpMessageType::kRequest,
         someip_protocol::internal::SomeIpReturnCode::kOk},
        0U,
        service_discovery::message::entries::ServiceDiscoveryEntryContainer(),
        service_discovery::message::options::ServiceDiscoveryOptionContainer(),
        {service_discovery::message::entries::ServiceDiscoveryEntryContainer(),
         service_discovery::message::options::ServiceDiscoveryOptionContainer()}};
    if (is_message_valid) {
      sd_message.someip_header_.service_id_ = header.service_id_;
      sd_message.someip_header_.method_id_ = header.method_id_;
      sd_message.someip_header_.length_ = header.length_;
      sd_message.someip_header_.client_id_ = header.client_id_;
      sd_message.someip_header_.session_id_ = header.session_id_;
      sd_message.someip_header_.protocol_version_ = header.protocol_version_;
      sd_message.someip_header_.interface_version_ = header.interface_version_;
      sd_message.someip_header_.message_type_ = header.message_type_;
      sd_message.someip_header_.return_code_ = header.return_code_;
      sd_message.flags_ = flags;

      sd_message.entries_ = entries;
      sd_message.options_ = options;
    }

    return std::make_pair(is_message_valid, sd_message);
  }

  /*!
   * \brief Deserializes the SomeIp Sd Flags.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * flag.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981502
   * \trace SPEC-4981503
   * \trace SPEC-4981504
   * \trace SPEC-4981508
   * \trace SPEC-4981513
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Read the SdFlag and the 24 reserved bytes the buffer view considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the flag was valid.
   * - Return a pair of the status of the flag and the flag payload.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdFlagsResult DeserializeSomeIpSdFlags(someip_deserialization::Reader& reader) noexcept {
    someip_protocol::internal::SdFlag flags{};
    size_type const size_before{reader.Size()};
    if (reader.VerifySize(someip_protocol::internal::kSdHeaderSize)) {
      std::uint8_t reserved{};
      reader.ReadPrimitive<someip_protocol::internal::SdFlag, Endian>(flags);
      reader.ReadPrimitive<std::uint8_t, Endian>(reserved);
      reader.ReadPrimitive<std::uint8_t, Endian>(reserved);
      reader.ReadPrimitive<std::uint8_t, Endian>(reserved);
    }

    size_type const size_after{reader.Size()};
    bool const is_flag_field_valid{(size_before - size_after) == someip_protocol::internal::kSdHeaderSize};
    return std::make_pair(is_flag_field_valid, flags);
  }
};

}  // namespace marshalling
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_MESSAGE_DESERIALIZER_H_

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
/*!        \file  someip_sd_entries_deserializer.h
 *        \brief  Deserializer for SOME/IP service discovery entries.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_ENTRIES_DESERIALIZER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_ENTRIES_DESERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/marshalling/marshalling.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace marshalling {

/*!
 * \brief Shorting for amsr::someip_protocol::internal::deserialization.
 */
namespace someip_deserialization = amsr::someip_protocol::internal::deserialization;

/*!
 * \brief Sd entries deserialization result, pair of a boolean which specifies whether the deserialization has been
 * successful or not and deserialized SD entries container.
 */
using DeserializeSdEntryContainerResult =
    std::pair<bool, service_discovery::message::entries::ServiceDiscoveryEntryContainer>;

/*!
 * \brief Deserializer for the list of entries.
 */
class SomeIpSdEntriesDeserializer {
 public:
  /*!
   * \brief Type alias for serialization buffer size which can easily be exchanged.
   */
  using size_type = std::size_t;

  /*!
   * \brief Sd entry deserialization result, pair of a boolean which specifies whether the deserialization has been
   * successful or not and deserialized SD entry.
   */
  using DeserializeSdEntryResult = std::pair<bool, service_discovery::message::entries::ServiceDiscoveryEntry>;

  /*!
   * \brief Result of DeserializeSomeIpSdLengthOfEntries method.
   */
  using DeserializeSomeIpSdLengthOfEntriesResult = std::pair<bool, someip_protocol::internal::SdEntriesLength>;

  /*!
   * \brief Result of DeserializeSomeIpSdEntryHeader method.
   */
  using DeserializeSomeIpSdEntryHeaderResult = std::pair<bool, someip_protocol::internal::SomeIpSdEntryHeader>;

  /*!
   * \brief Result of DeserializeSomeIpSdServiceEntryPayload method.
   */
  using DeserializeSomeIpSdServiceEntryPayloadResult =
      std::pair<bool, someip_protocol::internal::SomeIpSdServiceEntryPayload>;

  /*!
   * \brief Result of DeserializeSomeIpSdEventgroupEntryPayload method.
   */
  using DeserializeSomeIpSdEventgroupEntryPayloadResult =
      std::pair<bool, someip_protocol::internal::SomeIpSdEventgroupEntryPayload>;

  /*!
   * \brief Function to perform the deserialization.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * data.
   * \trace SPEC-10144594
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Deserialize the length of the entries and check if the length field is valid.
   *   - Calculate the expected length of the entries and check if the length of the entries is valid.
   *     - Calculate the number of entries.
   *     - For all the entries:
   *       - Get the state for deserialization of the entry.
   *       - Get the deserialized data.
   *       - Check if the deserialization was successful:
   *         - Push the deserialized data into entries::ServiceDiscoveryEntryContainer.
   *       - Otherwise, break.
   *   - Otherwise,
   *     - Log an error that the length of entries is invalid.
   * - Otherwise,
   *   - Log an error that entry length field invalid.
   * - Return a pair of deserialization result and the deserialized data.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSdEntryContainerResult DeserializeSomeIpSdEntries(someip_deserialization::Reader& reader) noexcept {
    bool is_valid_entry{false};
    service_discovery::message::entries::ServiceDiscoveryEntryContainer entries{};

    // Deserialize length
    DeserializeSomeIpSdLengthOfEntriesResult const deserialized_length_pair{
        DeserializeSomeIpSdLengthOfEntries<Endian>(reader)};

    bool const is_length_field_valid{deserialized_length_pair.first};
    if (is_length_field_valid) {
      someip_protocol::internal::SdEntriesLength length_of_entries{0U};
      someip_protocol::internal::SdEntriesLength const expected_length_of_entries{
          static_cast<someip_protocol::internal::SdEntriesLength>(
              sizeof(someip_protocol::internal::SomeIpSdServiceEntryPayload) +
              sizeof(someip_protocol::internal::SomeIpSdEntryHeader))};
      length_of_entries = deserialized_length_pair.second;

      // validate that length_of_entries is valid.
      bool const is_length_of_entries_valid{(length_of_entries % expected_length_of_entries) == 0U};

      if (is_length_of_entries_valid) {
        // calculate number of entries
        size_type const entries_count{static_cast<size_type>(length_of_entries) / expected_length_of_entries};

        for (size_type i{0U}; i < entries_count; ++i) {
          static_cast<void>(i);
          DeserializeSdEntryResult const entry_result{DeserializeEntry<Endian>(reader)};
          is_valid_entry = entry_result.first;
          service_discovery::message::entries::ServiceDiscoveryEntry const entry{entry_result.second};
          if (is_valid_entry) {
            entries.push_back(entry);
          } else {
            break;
          }
        }
      } else {
        someip_daemon_core::logging::AraComLogger const kLogger{
            someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
            someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
            ara::core::StringView{"ServiceDiscoveryMarshalling"}};
        kLogger.LogError(
            [&length_of_entries](ara::log::LogStream& s) {
              s << "Length of entries is invalid " << length_of_entries << ". This message is discarded.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      someip_daemon_core::logging::AraComLogger const kLogger{
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
          ara::core::StringView{"ServiceDiscoveryMarshalling"}};
      kLogger.LogError(
          [](ara::log::LogStream& s) {
            s << "Entry length field invalid, expected at least "
              << someip_protocol::internal::kSdLengthOfEntriesFieldSize;
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return std::make_pair(is_valid_entry, entries);
  }

 private:
  /*!
   * \brief Function to deserialize entry.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and
   * deserialized data.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981519
   * \internal
   * - Deserializes the SomeIpSdEntryHeader from the given serialized byte stream.
   * - Check if the header is valid.
   *   - Retrieve the header.
   *     - Check if the SomeIpSdEntryType is "kFindService" or "kOfferService".
   *       - Deserializes the SomeIpSdEntryPayload from the given serialized byte stream.
   *       - Check if the entry is valid.
   *         - Get SD entry.
   *       - Otherwise,
   *         - Log an error indicates insufficient deserialized entry bytes.
   *     - Check if the SomeIpSdEntryType is "kSubscribeEventgroup" or "kSubscribeEventgroupAck".
   *       - Deserializes the SomeIpSdEventgroupEntry from the given serialized byte stream.
   *       - Check if the entry is valid.
   *         - Get  entries::ServiceDiscoveryEntry from a header and an eventgroup entry.
   *       - Otherwise,
   *         - Log an error indicates insufficient deserialized entry bytes.
   *     - As default,
   *       - Log an error for unknown entry type.
   *   - Otherwise,
   *     - Log an error indicates that header field invalid.
   * - Return a pair of entry status and the entry payload.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSdEntryResult DeserializeEntry(someip_deserialization::Reader& reader) noexcept {
    bool is_valid_entry{false};
    service_discovery::message::entries::ServiceDiscoveryEntry entry{};
    DeserializeSomeIpSdEntryHeaderResult const result{DeserializeSomeIpSdEntryHeader<Endian>(reader)};
    bool const is_header_valid{result.first};
    if (is_header_valid) {
      someip_protocol::internal::SomeIpSdEntryHeader const header{result.second};
      switch (static_cast<someip_protocol::internal::SomeIpSdEntryType>(header.type_)) {
        case someip_protocol::internal::SomeIpSdEntryType::kFindService:
        case someip_protocol::internal::SomeIpSdEntryType::kOfferService: {
          DeserializeSomeIpSdServiceEntryPayloadResult const entry_result{
              DeserializeSomeIpSdServiceEntryPayload<Endian>(reader)};
          is_valid_entry = entry_result.first;
          if (is_valid_entry) {
            entry = GetServiceDiscoveryEntry(header, entry_result.second);
          } else {
            someip_daemon_core::logging::AraComLogger const kLogger{
                someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
                someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
                ara::core::StringView{"ServiceDiscoveryMarshalling"}};
            kLogger.LogError(
                [](ara::log::LogStream& s) {
                  s << "Insufficient deserialized entry bytes, expected at least "
                    << sizeof(someip_protocol::internal::SomeIpSdServiceEntryPayload)
                    << ".  This message is discarded.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
          break;
        }
        case someip_protocol::internal::SomeIpSdEntryType::kSubscribeEventgroup:
        case someip_protocol::internal::SomeIpSdEntryType::kSubscribeEventgroupAck: {
          DeserializeSomeIpSdEventgroupEntryPayloadResult const entry_result{
              DeserializeSomeIpSdEventgroupEntryPayload(reader)};
          is_valid_entry = entry_result.first;
          if (is_valid_entry) {
            entry = GetServiceDiscoveryEntry(header, entry_result.second);
          } else {
            someip_daemon_core::logging::AraComLogger const kLogger{
                someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
                someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
                ara::core::StringView{"ServiceDiscoveryMarshalling"}};
            kLogger.LogError(
                [](ara::log::LogStream& s) {
                  s << "Insufficient deserialized entry bytes, expected at least "
                    << sizeof(someip_protocol::internal::SomeIpSdServiceEntryPayload)
                    << ".  This message is discarded.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
          break;
        }
        default: {
          someip_daemon_core::logging::AraComLogger const kLogger{
              someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
              someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
              ara::core::StringView{"ServiceDiscoveryMarshalling"}};
          kLogger.LogError(
              [&header](ara::log::LogStream& s) {
                s << "Unknown entry type 0x"
                  << ara::log::HexFormat(
                         static_cast<std::underlying_type<someip_protocol::internal::SomeIpSdEntryType>::type>(
                             header.type_));
              },
              static_cast<char const*>(__func__), __LINE__);
          break;
        }
      }
    } else {
      // Options length field is invalid
      someip_daemon_core::logging::AraComLogger const kLogger{
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
          ara::core::StringView{"ServiceDiscoveryMarshalling"}};
      kLogger.LogError(
          [](ara::log::LogStream& s) {
            s << "Header field invalid, expected at least " << someip_protocol::internal::kSdEntryHeaderSize;
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return std::make_pair(is_valid_entry, entry);
  }

  /*!
   * \brief Function to get  entries::ServiceDiscoveryEntry.
   * \param[in] header The SomeIpSdEntryHeader
   * \param[in] payload The payload for the service entry (SomeIpSdServiceEntryPayload)
   * \return A  entries::ServiceDiscoveryEntry
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Create  entries::ServiceDiscoveryEntry instance using supplied SomeIpSdEntryHeader and
   * SomeIpSdServiceEntryPayload.
   * - Return the entry instance.
   * \endinternal
   */
  static service_discovery::message::entries::ServiceDiscoveryEntry GetServiceDiscoveryEntry(
      someip_protocol::internal::SomeIpSdEntryHeader const& header,
      someip_protocol::internal::SomeIpSdServiceEntryPayload const& payload) {
    service_discovery::message::entries::ServiceDiscoveryEntry entry{};
    entry.type_ = header.type_;
    entry.service_id_ = payload.service_id_;
    entry.instance_id_ = payload.instance_id_;
    std::uint32_t const kMajorVersionTtl{payload.major_version_ttl_};
    entry.major_version_ = static_cast<std::uint8_t>(
        static_cast<std::uint32_t>(kMajorVersionTtl >> someip_protocol::internal::kSomeIpSdEntryMajorVersionShift) &
        static_cast<std::uint32_t>(someip_protocol::internal::kSomeIpSdEntryMajorVersionMask));
    entry.ttl_ = static_cast<amsr::someip_protocol::internal::Ttl>(
        kMajorVersionTtl & static_cast<std::uint32_t>(someip_protocol::internal::kSomeIpSdEntryTtlMask));
    entry.index_1st_opts_ = header.index_1st_opts_;
    entry.index_2nd_opts_ = header.index_2nd_opts_;
    entry.number_1st_opts_ = static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
        static_cast<std::uint8_t>(header.number_of_opts_ >>
                                  someip_protocol::internal::kSomeIpSdEntryNumberFirstOptsShift) &
        static_cast<std::uint8_t>(someip_protocol::internal::kSomeIpSdEntryNumberOptsMask));
    entry.number_2nd_opts_ = static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
        header.number_of_opts_ & static_cast<std::uint8_t>(someip_protocol::internal::kSomeIpSdEntryNumberOptsMask));
    entry.minor_version_ = payload.minor_version_;
    return entry;
  }

  /*!
   * \brief Function to get  entries::ServiceDiscoveryEntry from a header and an eventgroup entry.
   * \param[in] header The SomeIpSdEntryHeader
   * \param[in] payload The payload for the event group entry (SomeIpSdEventgroupEntryPayload)
   * \return A  entries::ServiceDiscoveryEntry
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Create  entries::ServiceDiscoveryEntry instance using supplied SomeIpSdEntryHeader and
   * SomeIpSdEventgroupEntryPayload.
   * - Return the entry instance.
   * \endinternal
   */
  static service_discovery::message::entries::ServiceDiscoveryEntry GetServiceDiscoveryEntry(
      someip_protocol::internal::SomeIpSdEntryHeader const& header,
      someip_protocol::internal::SomeIpSdEventgroupEntryPayload const& payload) {
    service_discovery::message::entries::ServiceDiscoveryEntry entry{};
    entry.type_ = header.type_;
    entry.service_id_ = payload.service_id_;
    entry.instance_id_ = payload.instance_id_;
    std::uint32_t const kMajorVersionTtl{payload.major_version_ttl_};
    entry.major_version_ = static_cast<std::uint8_t>(
        static_cast<std::uint32_t>(kMajorVersionTtl >> someip_protocol::internal::kSomeIpSdEntryMajorVersionShift) &
        static_cast<std::uint32_t>(someip_protocol::internal::kSomeIpSdEntryMajorVersionMask));
    entry.ttl_ = static_cast<amsr::someip_protocol::internal::Ttl>(
        kMajorVersionTtl & static_cast<std::uint32_t>(someip_protocol::internal::kSomeIpSdEntryTtlMask));
    entry.index_1st_opts_ = header.index_1st_opts_;
    entry.index_2nd_opts_ = header.index_2nd_opts_;
    entry.number_1st_opts_ = static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
        static_cast<std::uint8_t>(header.number_of_opts_ >>
                                  someip_protocol::internal::kSomeIpSdEntryNumberFirstOptsShift) &
        static_cast<std::uint8_t>(someip_protocol::internal::kSomeIpSdEntryNumberOptsMask));
    entry.number_2nd_opts_ = static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
        header.number_of_opts_ & static_cast<std::uint8_t>(someip_protocol::internal::kSomeIpSdEntryNumberOptsMask));
    entry.counter_ = payload.counter_;
    entry.eventgroup_id_ = payload.eventgroup_id_;
    return entry;
  }

  /*!
   * \brief Deserializes the LengthOfEntries serialized from a given byte stream.
   *

   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * length.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981517
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Read SdEntriesLength considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the length field was valid.
   * - Return a pair of the status of the length field and the length of entry.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdLengthOfEntriesResult DeserializeSomeIpSdLengthOfEntries(
      someip_deserialization::Reader& reader) noexcept {
    someip_protocol::internal::SdEntriesLength length_of_entries{};
    size_type const size_before{reader.Size()};

    if (reader.VerifySize(someip_protocol::internal::kSdLengthOfEntriesFieldSize)) {
      reader.ReadPrimitive<someip_protocol::internal::SdEntriesLength, Endian>(length_of_entries);
    }

    size_type const size_after{reader.Size()};
    bool const is_length_field_valid{(size_before - size_after) ==
                                     someip_protocol::internal::kSdLengthOfEntriesFieldSize};
    return std::make_pair(is_length_field_valid, length_of_entries);
  }

  /*!
   * \brief Deserializes the SomeIpSdEntryHeader from a given serialized byte stream.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * header.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981520
   * \trace SPEC-4981521
   * \trace SPEC-4981522
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Read type, first option index, second option index, and number of options considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the header was valid.
   * - Return a pair of the status of the header and the header payload.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdEntryHeaderResult DeserializeSomeIpSdEntryHeader(
      someip_deserialization::Reader& reader) noexcept {
    someip_protocol::internal::SomeIpSdEntryHeader header{};
    size_type const size_before{reader.Size()};

    if (reader.VerifySize(someip_protocol::internal::kSdEntryHeaderSize)) {
      reader.ReadPrimitive<std::uint8_t, Endian>(header.type_);
      reader.ReadPrimitive<std::uint8_t, Endian>(header.index_1st_opts_);
      reader.ReadPrimitive<std::uint8_t, Endian>(header.index_2nd_opts_);
      reader.ReadPrimitive<std::uint8_t, Endian>(header.number_of_opts_);
    }

    size_type const size_after{reader.Size()};
    bool const is_header_valid{(size_before - size_after) == someip_protocol::internal::kSdEntryHeaderSize};
    return std::make_pair(is_header_valid, header);
  }

  /*!
   * \brief Deserializes the SomeIpSdServiceEntryPayload from a given serialized byte stream.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * data.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981520
   * \trace SPEC-4981590
   * \trace SPEC-4981595
   * \trace SPEC-4981600
   * \trace SPEC-4981601
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Read ServiceId, InstanceId, major version, and minor version considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the entry was valid.
   * - Return a pair of the status of the entry and the entry payload.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdServiceEntryPayloadResult DeserializeSomeIpSdServiceEntryPayload(
      someip_deserialization::Reader& reader) noexcept {
    someip_protocol::internal::SomeIpSdServiceEntryPayload entry{};
    size_type const size_before{reader.Size()};

    if (reader.VerifySize(sizeof(someip_protocol::internal::SomeIpSdServiceEntryPayload))) {
      reader.ReadPrimitive<someip_protocol::internal::ServiceId, Endian>(entry.service_id_);
      reader.ReadPrimitive<someip_protocol::internal::InstanceId, Endian>(entry.instance_id_);
      reader.ReadPrimitive<std::uint32_t, Endian>(entry.major_version_ttl_);
      reader.ReadPrimitive<std::uint32_t, Endian>(entry.minor_version_);
    }

    size_type const size_after{reader.Size()};
    bool const is_valid_entry{(size_before - size_after) ==
                              sizeof(someip_protocol::internal::SomeIpSdServiceEntryPayload)};
    return std::make_pair(is_valid_entry, entry);
  }

  /*!
   * \brief Deserializes the SomeIpSdEventgroupEntry from a given serialized byte stream.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * data.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981522
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Read ServiceId, InstanceId, major version, reserved byte, counter, and eventgroup to the buffer view
   *     considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the entry was valid.
   * - Return a pair of the status of the entry and the entry payload.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdEventgroupEntryPayloadResult DeserializeSomeIpSdEventgroupEntryPayload(
      someip_deserialization::Reader& reader) noexcept {
    someip_protocol::internal::SomeIpSdEventgroupEntryPayload entry{};
    size_type const size_before{reader.Size()};

    if (reader.VerifySize(sizeof(someip_protocol::internal::SomeIpSdServiceEntryPayload))) {
      reader.ReadPrimitive<someip_protocol::internal::ServiceId, Endian>(entry.service_id_);
      reader.ReadPrimitive<someip_protocol::internal::InstanceId, Endian>(entry.instance_id_);
      reader.ReadPrimitive<std::uint32_t, Endian>(entry.major_version_ttl_);
      reader.ReadPrimitive<std::uint8_t, Endian>(entry.reserved_);
      reader.ReadPrimitive<std::uint8_t, Endian>(entry.counter_);
      reader.ReadPrimitive<std::uint16_t, Endian>(entry.eventgroup_id_);
    }

    size_type const size_after{reader.Size()};
    bool const is_valid_entry{(size_before - size_after) ==
                              sizeof(someip_protocol::internal::SomeIpSdServiceEntryPayload)};
    return std::make_pair(is_valid_entry, entry);
  }
};

}  // namespace marshalling
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_ENTRIES_DESERIALIZER_H_

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
/*!        \file  ser_sd_message.h
 *        \brief  Serializer for service discovery message
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_SER_SD_MESSAGE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_SER_SD_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <string>
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/string.h"

#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/logging/someip_daemon_log_builder.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message.h"
#include "marshalling.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "vac/language/location.h"

namespace amsr {
namespace someip_daemon_core {
namespace marshalling {

namespace someip_serialization = amsr::someip_protocol::internal::serialization;

/*!
 * \brief Obtain the wire protocol from a SomeIpSdEndpointOptionProto.
 * \param[in] from The SomeIpSdEndpointOptionProto.
 * \return The associated wire protocol.
 * \pre -
 * \context   ANY
 * \reentrant FALSE
 * \steady    TRUE
 * \trace SPEC-10144563
 * \trace SPEC-10144564
 * \trace SPEC-10144578
 *
 * \internal
 * - Return the underlying protocol used for this endpoint.
 * \endinternal
 */
inline std::uint8_t SerializeSdEndpointOptionProto(
    service_discovery::message::options::SomeIpSdEndpointOptionProto from) {
  std::uint8_t result{0U};

  switch (from) {
    case service_discovery::message::options::SomeIpSdEndpointOptionProto::kTcp: {
      result = kProtoTcp;
      break;
    }
    case service_discovery::message::options::SomeIpSdEndpointOptionProto::kUdp: {
      result = kProtoUdp;
      break;
    }
    default: {
      assert(false);
      break;
    }
  }
  return result;
}

/*!
 * \brief Serializer for SD flags.
 * \tparam Endian The endianness.
 * \param[in,out]  writer      Writer holding the buffer to which data is serialized.
 * \param[in]       flags       Flags of the SOME/IP-SD Header.
 * \pre -
 * \context   ANY
 * \reentrant FALSE
 * \steady    TRUE
 *
 * \internal
 * - Write the flag primitive to the buffer considering the specified endian.
 * \endinternal
 */
template <typename Endian>
void SerializeSomeIpSdFlags(someip_serialization::Writer& writer,
                            someip_protocol::internal::SdFlag const flags) noexcept {
  writer.writePrimitive<someip_protocol::internal::SdFlag, Endian>(flags);
}

/*!
 * \brief Serializer for SD reserved bytes.
 * \tparam Endian The endianness.
 * \param[in,out]  writer Writer holding the buffer to which data is serialized.
 * \pre -
 * \context   ANY
 * \reentrant FALSE
 * \steady    TRUE
 *
 * \internal
 * - Write the reserved bytes to the buffer considering the specified endianness.
 * \endinternal
 */
template <typename Endian>
void SerializeSomeIpSdReserved(someip_serialization::Writer& writer) noexcept {
  // PRS_SOMEIPSD_00386 specifies that 3 reserved bytes with 0 value shall be serialized after the SD flags.
  writer.writePrimitive<std::uint8_t, Endian>(0U);
  writer.writePrimitive<std::uint8_t, Endian>(0U);
  writer.writePrimitive<std::uint8_t, Endian>(0U);
}

/*!
 * \brief Serializer for service entry.
 * \tparam Endian The endianness.
 * \param[in,out]  writer      Writer holding the buffer to which data is serialized.
 * \param[in]       entry       SOME/IP SD entry.
 * \pre -
 * \context   ANY
 * \reentrant FALSE
 * \steady    TRUE
 *
 * \internal
 * - Write the SdEntryType, SdEntryOptionIndex, ServiceId, InstanceId, MajorVersion, and MinorVersion primitive to the
 *   buffer considering the specified endian.
 * \endinternal
 * \trace SPEC-10144774
 * \trace SPEC-10144793
 * \trace SPEC-10144801
 * \trace SPEC-10144816
 */
template <typename Endian>
void SerializeSomeIpSdServiceEntry(someip_serialization::Writer& writer,
                                   service_discovery::message::entries::ServiceDiscoveryEntry const& entry) noexcept {
  writer.writePrimitive<amsr::someip_protocol::internal::SdEntryType, Endian>(entry.type_);
  writer.writePrimitive<amsr::someip_protocol::internal::SdEntryOptionIndex, Endian>(entry.index_1st_opts_);
  writer.writePrimitive<amsr::someip_protocol::internal::SdEntryOptionIndex, Endian>(entry.index_2nd_opts_);
  writer.writePrimitive<std::uint8_t, Endian>(static_cast<std::uint8_t>(
      static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
          static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
              entry.number_1st_opts_ & static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
                                           someip_protocol::internal::kSomeIpSdEntryNumberOptsMask))
          << someip_protocol::internal::kSomeIpSdEntryNumberFirstOptsShift) |
      static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
          entry.number_2nd_opts_ & static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
                                       someip_protocol::internal::kSomeIpSdEntryNumberOptsMask))));
  writer.writePrimitive<amsr::someip_protocol::internal::ServiceId, Endian>(entry.service_id_);
  writer.writePrimitive<amsr::someip_protocol::internal::InstanceId, Endian>(entry.instance_id_);
  writer.writePrimitive<std::uint32_t, Endian>(static_cast<std::uint32_t>(
      ((static_cast<std::uint32_t>(entry.major_version_) &
        static_cast<std::uint32_t>(someip_protocol::internal::kSomeIpSdEntryMajorVersionMask))
       << static_cast<std::uint32_t>(someip_protocol::internal::kSomeIpSdEntryMajorVersionShift)) |
      static_cast<amsr::someip_protocol::internal::Ttl>(
          entry.ttl_ &
          static_cast<amsr::someip_protocol::internal::Ttl>(someip_protocol::internal::kSomeIpSdEntryTtlMask))));
  writer.writePrimitive<amsr::someip_protocol::internal::MinorVersion, Endian>(entry.minor_version_);
}

/*!
 * \brief Serializer for SomeIpSdEventGroup event group entry.
 * \tparam Endian The endianness.
 * \param[in,out]  writer      Writer holding the buffer to which data is serialized.
 * \param[in]       entry       SOME/IP SD entry.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady    TRUE
 *
 * \internal
 * - Write the SdEntryType, SdEntryOptionIndex, ServiceId, InstanceId, MajorVersion, SdEntryCounter, and EventgroupId
 *   primitive to the buffer considering the specified endian.
 * \endinternal
 * \trace SPEC-10144774
 * \trace SPEC-10144801
 * \trace SPEC-4981690
 */
template <typename Endian>
void SerializeSomeIpSdEventGroupEntry(
    someip_serialization::Writer& writer,
    service_discovery::message::entries::ServiceDiscoveryEntry const& entry) noexcept {
  writer.writePrimitive<amsr::someip_protocol::internal::SdEntryType, Endian>(entry.type_);
  writer.writePrimitive<amsr::someip_protocol::internal::SdEntryOptionIndex, Endian>(entry.index_1st_opts_);
  writer.writePrimitive<amsr::someip_protocol::internal::SdEntryOptionIndex, Endian>(entry.index_2nd_opts_);
  writer.writePrimitive<std::uint8_t, Endian>(static_cast<std::uint8_t>(
      static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
          static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
              entry.number_1st_opts_ & static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
                                           someip_protocol::internal::kSomeIpSdEntryNumberOptsMask))
          << someip_protocol::internal::kSomeIpSdEntryNumberFirstOptsShift) |
      static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
          entry.number_2nd_opts_ & static_cast<amsr::someip_protocol::internal::SdEntryOptionCount>(
                                       someip_protocol::internal::kSomeIpSdEntryNumberOptsMask))));
  writer.writePrimitive<amsr::someip_protocol::internal::ServiceId, Endian>(entry.service_id_);
  writer.writePrimitive<amsr::someip_protocol::internal::InstanceId, Endian>(entry.instance_id_);
  writer.writePrimitive<std::uint32_t, Endian>(static_cast<std::uint32_t>(
      ((static_cast<std::uint32_t>(entry.major_version_) &
        static_cast<std::uint32_t>(someip_protocol::internal::kSomeIpSdEntryMajorVersionMask))
       << static_cast<std::uint32_t>(someip_protocol::internal::kSomeIpSdEntryMajorVersionShift)) |
      static_cast<amsr::someip_protocol::internal::Ttl>(
          entry.ttl_ &
          static_cast<amsr::someip_protocol::internal::Ttl>(someip_protocol::internal::kSomeIpSdEntryTtlMask))));
  writer.writePrimitive<std::uint8_t, Endian>(static_cast<std::uint8_t>(0x00U));
  writer.writePrimitive<amsr::someip_protocol::internal::SdEntryCounter, Endian>(entry.counter_);
  writer.writePrimitive<amsr::someip_protocol::internal::EventgroupId, Endian>(entry.eventgroup_id_);
}

/*!
 * \brief Serializes the SomeIpSdIPv4EndpointOption.
 * \tparam Endian The endianness.
 * \param[in,out]  writer    Writer holding the buffer to which data is serialized.
 * \param[in]      option    SOME/IP SD option.
 * \pre -
 * \context   ANY
 * \reentrant FALSE
 * \steady    TRUE
 *
 * \internal
 * - Assert that the address is valid and is IPv4 address.
 * - Retrieve the binary representation of the address.
 * - Write the kSomeIpSdIPv4EndpointOptionLength, options::SomeIpSdEndpointOptionType, Reserved byte, IPv4-Address,
 * Reserved byte , SomeIpSdEndpointOptionProto, and Port primitive to the buffer considering the specified endian.
 * \endinternal
 */
template <typename Endian>
void SerializeSomeIpSdIPv4EndpointOption(
    someip_serialization::Writer& writer,
    service_discovery::message::options::ServiceDiscoveryOption const& option) noexcept {
  assert(option.address_.IsV4());
  ara::core::Array<std::uint8_t, amsr::net::ip::AddressV4::kAddressBytes> const s_addr{
      option.address_.GetV4Unsafe().GetBinary()};

  writer.writePrimitive<std::uint16_t, Endian>(
      static_cast<std::uint16_t>(someip_protocol::internal::kSomeIpSdIPv4EndpointOptionLength));  // Length
  writer.writePrimitive<std::uint8_t, Endian>(
      static_cast<typename std::underlying_type<decltype(option.type_)>::type>(option.type_));  // Type
  writer.writePrimitive<std::uint8_t, Endian>(0x00U);                                           // Reserved
  writer.writePrimitive<std::uint8_t, Endian>(s_addr[0]);                                       // IPv4-Address
  writer.writePrimitive<std::uint8_t, Endian>(s_addr[1]);
  writer.writePrimitive<std::uint8_t, Endian>(s_addr[2]);
  writer.writePrimitive<std::uint8_t, Endian>(s_addr[3]);
  writer.writePrimitive<std::uint8_t, Endian>(0x00U);                                          // Reserved
  writer.writePrimitive<std::uint8_t, Endian>(SerializeSdEndpointOptionProto(option.proto_));  // Transport protocol
  writer.writePrimitive<std::uint16_t, Endian>(option.port_.port);                             // Port
}

/*!
 * \brief Serializes the SomeIpSdIPv6EndpointOption.
 * \tparam Endian The endianness.
 * \param[in,out]  writer    Writer holding the buffer to which data is serialized.
 * \param[in]      option    SOME/IP SD option.
 * \pre -
 * \context   ANY
 * \reentrant FALSE
 * \steady    TRUE
 * \trace SPEC-10144557
 * \trace SPEC-10144560
 * \trace SPEC-10144572
 * \trace SPEC-10144573
 * \trace SPEC-10144575
 *
 * \internal
 * - Assert that the address is valid and is IPv6 address.
 * - Retrieve the native representation of the address.
 * - Write the SomeIpSdIPv4EndpointOptionLength, options::SomeIpSdEndpointOptionType, Reserved byte, IPv6-Address,
 * Reserved byte , SomeIpSdEndpointOptionProto, and Port primitive to the buffer considering the specified endian.
 * \endinternal
 */
template <typename Endian>
void SerializeSomeIpSdIPv6EndpointOption(
    someip_serialization::Writer& writer,
    service_discovery::message::options::ServiceDiscoveryOption const& option) noexcept {
  assert(option.address_.IsV6());
  struct in6_addr const sin6_addr{option.address_.GetV6Unsafe().GetNative()};

  writer.writePrimitive<std::uint16_t, Endian>(
      static_cast<std::uint16_t>(someip_protocol::internal::kSomeIpSdIPv6EndpointOptionLength));  // Length
  writer.writePrimitive<std::uint8_t, Endian>(
      static_cast<typename std::underlying_type<decltype(option.type_)>::type>(option.type_));  // Type
  writer.writePrimitive<std::uint8_t, Endian>(0x00U);                                           // Reserved
  for (std::size_t address_byte_index{0U}; address_byte_index < 16U; ++address_byte_index) {    // IPv6-Address
    writer.writePrimitive<std::uint8_t, Endian>(static_cast<std::uint8_t>(sin6_addr.s6_addr[address_byte_index]));
  }
  writer.writePrimitive<std::uint8_t, Endian>(0x00U);                                          // Reserved
  writer.writePrimitive<std::uint8_t, Endian>(SerializeSdEndpointOptionProto(option.proto_));  // Transport protocol
  writer.writePrimitive<std::uint16_t, Endian>(option.port_.port);                             // Port
}

/*!
 * \brief Serializer for SOME/IP SD entries.
 */
class SomeIpSdEntriesSerializer {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  /*!
   * \brief Serializer for a SomeIpSdEntries.
   * \tparam          Endian      The endianness.
   * \param[in,out]   writer      Writer holding the buffer to which data is serialized.
   * \param[in]       entries     SOME/IP SD entries.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   * \trace SPEC-10144541
   *
   * \internal
   * - Get SOME/IP SD entries size.
   * - Check if the size of entries is greater than SdEntriesLength.
   *   - Log fatal that length field overflows during serialization and abort.
   * - Write the SdEntriesLength primitive considering the endian.
   * - Iterate over the entries and serialize them accordingly.
   *   - If any entry is not of SomeIpSdEntryType,
   *     - Log fatal that the entry is invalid and abort.
   * \endinternal
   */
  template <typename Endian>
  static void SerializeSomeIpSdEntries(
      someip_serialization::Writer& writer,
      service_discovery::message::entries::ServiceDiscoveryEntryContainer const& entries) noexcept {
    namespace someip = someip_protocol::internal;
    someip_daemon_core::logging::AraComLogger const logger{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                           someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                           ara::core::StringView{"SomeIpSdEntriesSerializer"}};

    std::size_t const kEntriesSize{GetSomeIpSdEntriesSize(entries)};
    if (kEntriesSize > std::numeric_limits<someip::SdEntriesLength>::max()) {
      logger.LogFatalAndAbort(
          [](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Violation: Length field overflow during serialization.");
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
    writer.writePrimitive<someip::SdEntriesLength, Endian>(static_cast<someip::SdEntriesLength>(kEntriesSize));

    for (service_discovery::message::entries::ServiceDiscoveryEntry const& entry : entries) {
      switch (static_cast<someip::SomeIpSdEntryType>(entry.type_)) {
        case someip::SomeIpSdEntryType::kFindService:
        case someip::SomeIpSdEntryType::kOfferService: {
          SerializeSomeIpSdServiceEntry<Endian>(writer, entry);
          break;
        }
        case someip::SomeIpSdEntryType::kSubscribeEventgroup:
        case someip::SomeIpSdEntryType::kSubscribeEventgroupAck: {
          SerializeSomeIpSdEventGroupEntry<Endian>(writer, entry);
          break;
        }
        default: {
          logger.LogFatalAndAbort(
              [&entry](ara::core::String& abort_msg) noexcept {
                abort_msg.append("Violation: Invalid entry type ");
                abort_msg.append(std::to_string(static_cast<std::uint32_t>(
                    static_cast<std::underlying_type<someip::SomeIpSdEntryType>::type>(entry.type_))));
              },
              AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
          break;
        }
      }
    }
  }

  /*!
   * \brief Calculate the size of the SOME/IP SD entries (NOT including Length field for the entries array).
   * \param[in] entries   SOME/IP SD options.
   * \return    Size of entries in bytes.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   */
  static std::size_t GetSomeIpSdEntriesSize(
      service_discovery::message::entries::ServiceDiscoveryEntryContainer const& entries) noexcept {
    std::size_t const kSize{entries.size() * (someip_protocol::internal::kSdEntryHeaderSize +
                                              someip_protocol::internal::kSdEntryPayloadSize)};
    return kSize;
  }
};

/*!
 * \brief Serializer for SomeIp SD options.
 */
class SomeIpSdOptionsSerializer {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  /*!
   * \brief Serializes SomeIpSdOptions.
   * \tparam          Endian    The endianness.
   * \param[in,out]   writer    Writer holding the buffer to which data is serialized.
   * \param[in]       options   SOME/IP SD options.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   * \trace SPEC-10144545
   *
   * \internal
   * - Get SOME/IP SD options size.
   * - Check if the options size is greater than OptionsLength.
   *   - Log fatal that length field overflows during serialization and abort.
   * - Write the OptionsLength primitive considering the endian.
   * - Iterate over the options and serialize them accordingly.
   *   - If any entry is not of options::SomeIpSdEndpointOptionType,
   *     - Log fatal that the option is invalid and abort.
   * \endinternal
   */
  template <typename Endian>
  static void SerializeSomeIpSdOptions(
      someip_serialization::Writer& writer,
      service_discovery::message::options::ServiceDiscoveryOptionContainer const& options) noexcept {
    using OptionsLength = someip_protocol::internal::SdOptionsLength;
    someip_daemon_core::logging::AraComLogger const logger{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                           someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                           ara::core::StringView{"SerializeSomeIpSdOptions"}};
    std::size_t const kOptionsSize{GetSomeIpSdOptionsSize(options)};
    if (kOptionsSize > std::numeric_limits<OptionsLength>::max()) {
      logger.LogFatalAndAbort(
          [](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Violation: Length field overflow during serialization.");
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
    writer.writePrimitive<OptionsLength, Endian>(static_cast<OptionsLength>(kOptionsSize));

    // Options
    for (service_discovery::message::options::ServiceDiscoveryOption const& option : options) {
      switch (option.type_) {
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4MulticastEndpointOption: {
          SerializeSomeIpSdIPv4EndpointOption<Endian>(writer, option);
          break;
        }
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6MulticastEndpointOption: {
          SerializeSomeIpSdIPv6EndpointOption<Endian>(writer, option);
          break;
        }
        default: {
          logger.LogFatalAndAbort(
              [&option](ara::core::String& abort_msg) noexcept {
                abort_msg.append("Violation: Invalid option type ");
                abort_msg.append(std::to_string(static_cast<std::uint32_t>(
                    static_cast<
                        std::underlying_type<service_discovery::message::options::SomeIpSdEndpointOptionType>::type>(
                        option.type_))));
              },
              AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
          break;
        }
      }
    }
  }

  /*!
   * \brief Calculate the size of the SOME/IP SD options (NOT including Length field for the options array).
   * \param[in] options   SOME/IP SD options.
   * \return Size of Options array in bytes.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Iterate over options::ServiceDiscoveryOption.
   *   - Check if options::SomeIpSdEndpointOptionType of type IPv4.
   *     - Add kSdOptionHeaderSize and kSomeIpSdIPv4EndpointOptionPayloadSize to the option size.
   *   - Otherwise, if options::SomeIpSdEndpointOptionType of type IPv6.
   *     - Add kSdOptionHeaderSize and kSomeIpSdIPv6EndpointOptionPayloadSize to the option size.
   *   - Otherwise as default,
   *     - Log fatal that the option type is invalid and abort.
   * - Return the option size.
   * \endinternal
   */
  static std::size_t GetSomeIpSdOptionsSize(
      service_discovery::message::options::ServiceDiscoveryOptionContainer const& options) {
    namespace someip = someip_protocol::internal;
    someip_daemon_core::logging::AraComLogger const logger{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                           someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                           ara::core::StringView{"SerializeSomeIpSdOptions"}};
    std::size_t options_size{0U};

    for (service_discovery::message::options::ServiceDiscoveryOption const& option : options) {
      switch (option.type_) {
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4MulticastEndpointOption: {
          options_size += someip::kSdOptionHeaderSize + someip::kSomeIpSdIPv4EndpointOptionPayloadSize;
          break;
        }
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6MulticastEndpointOption: {
          options_size += someip::kSdOptionHeaderSize + someip::kSomeIpSdIPv6EndpointOptionPayloadSize;
          break;
        }
        default: {
          logger.LogFatalAndAbort(
              [&option](ara::core::String& abort_msg) noexcept {
                abort_msg.append("Violation: Invalid option type ");
                abort_msg.append(std::to_string(static_cast<std::uint32_t>(
                    static_cast<
                        std::underlying_type<service_discovery::message::options::SomeIpSdEndpointOptionType>::type>(
                        option.type_))));
              },
              AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
          break;
        }
      }
    }

    return options_size;
  }
};

/*!
 * \brief Serializer for SomeIp SD message.
 */
class SomeIpSdMessageSerializer {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  /*!
   * \brief Serializes SomeIp SD message.
   * \tparam          Endian     The endianness.
   * \param[in,out]   writer     Writer holding the buffer to which data is serialized.
   * \param[in]       sd_message SOME/IP SD message.
   * \return          void.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981514
   * \trace SPEC-4981516
   * \trace SPEC-10144513
   * \trace SPEC-10144515
   * \trace SPEC-10144820
   *
   * \internal
   * - Get SOME/IP SD message size.
   * - Check if the message size is greater than complete SOME/IP message.
   *   - Log fatal that length field overflows during serialization and abort.
   * - Serialize SOME/IP header, entries, and options.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static void SerializeSomeIpSdMessage(someip_serialization::Writer& writer,
                                       service_discovery::message::ServiceDiscoveryMessage const& sd_message) noexcept {
    using LengthField = amsr::someip_protocol::internal::LengthField;
    someip_daemon_core::logging::AraComLogger const logger{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                           someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                           ara::core::StringView{"SomeIpSdMessageSerializer"}};
    std::size_t const kSdMessageSize{GetSomeIpSdMessageSize(sd_message)};
    if (kSdMessageSize > ((std::numeric_limits<LengthField>::max() + someip_protocol::internal::kHeaderSize) -
                          amsr::someip_protocol::internal::kMinimumPayloadLength)) {
      logger.LogFatalAndAbort(
          [](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Violation: Length field overflow during serialization.");
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }

    // SOME/IP header
    LengthField const kSdPayloadSize{static_cast<LengthField>(kSdMessageSize - someip_protocol::internal::kHeaderSize)};
    someip_serialization::SerializeSomeIpMessageHeader(writer, sd_message.someip_header_, kSdPayloadSize);

    // SD header
    SerializeSomeIpSdFlags<Endian>(writer, sd_message.flags_);
    SerializeSomeIpSdReserved<Endian>(writer);

    // List of Entries
    SomeIpSdEntriesSerializer::SerializeSomeIpSdEntries<Endian>(writer, sd_message.entries_);

    // List of Options
    SomeIpSdOptionsSerializer::SerializeSomeIpSdOptions<Endian>(writer, sd_message.options_);
  }

  /*!
   * \brief Calculate the size of the complete SOME/IP SD message.
   * \param[in] message   SOME/IP SD options.
   * \return Size of Options array in bytes.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Return the addition of kHeaderSize, kSdHeaderSize, kSdLengthOfEntriesFieldSize, entries size,
   *   kSdLengthOfOptionsFieldSize, and options size.
   * \endinternal
   */
  static std::size_t GetSomeIpSdMessageSize(service_discovery::message::ServiceDiscoveryMessage const& message) {
    namespace someip = someip_protocol::internal;

    std::size_t const kEntriesSize{SomeIpSdEntriesSerializer::GetSomeIpSdEntriesSize(message.entries_)};
    std::size_t const kOptionsSize{SomeIpSdOptionsSerializer::GetSomeIpSdOptionsSize(message.options_)};

    std::size_t const sd_message_size{someip::kHeaderSize + someip::kSdHeaderSize +
                                      someip::kSdLengthOfEntriesFieldSize + kEntriesSize +
                                      someip::kSdLengthOfOptionsFieldSize + kOptionsSize};
    return sd_message_size;
  }
};

}  // namespace marshalling
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_SER_SD_MESSAGE_H_

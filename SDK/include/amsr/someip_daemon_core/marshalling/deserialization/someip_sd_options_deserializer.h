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
/*!        \file  someip_sd_options_deserializer.h
 *        \brief  Deserializer for SOME/IP service discovery options.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_OPTIONS_DESERIALIZER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_OPTIONS_DESERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <utility>
#include "amsr/net/ip/address_v4.h"
#include "amsr/net/ip/address_v6.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/marshalling/marshalling.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message.h"
#include "ara/core/array.h"
#include "ara/core/string.h"
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
 * \brief Sd options deserialization result, pair of a boolean which specifies whether the deserialization has been
 * successful or not and deserialized SD option container.
 */
using DeserializeSdOptionContainerResult =
    std::pair<bool, service_discovery::message::options::ServiceDiscoveryOptionContainer>;

/*!
 * \brief Deserializer for list of Options.
 */
class SomeIpSdOptionsDeserializer {
 public:
  /*!
   * \brief Type alias for serialization buffer size which can easily be exchanged.
   */
  using size_type = std::size_t;

  /*!
   * \brief Result of DeserializeSomeIpSdLengthOfOptions method.
   */
  using DeserializeSomeIpSdLengthOfOptionsResult = std::pair<bool, someip_protocol::internal::SdOptionsLength>;

  /*!
   * \brief Result of DeserializeSomeIpSdEndpointOptionHeader method.
   */
  using DeserializeSomeIpSdEndpointOptionHeaderResult =
      std::pair<bool, someip_protocol::internal::SomeIpSdEndpointOptionHeader>;

  /*!
   * \brief Result of DeserializeSomeIpSdIPv4EndpointOptionPayload method.
   */
  using DeserializeSomeIpSdIPv4EndpointOptionPayloadResult =
      std::pair<bool, someip_protocol::internal::SomeIpSdIPv4EndpointOptionPayload>;

  /*!
   * \brief Result of DeserializeSomeIpSdIPv6EndpointOptionPayload method.
   */
  using DeserializeSomeIpSdIPv6EndpointOptionPayloadResult =
      std::pair<bool, someip_protocol::internal::SomeIpSdIPv6EndpointOptionPayload>;

  /*!
   * \brief Result of DeserializeSomeIpSdUnknownOptionPayload method.
   */
  using DeserializeSomeIpSdUnknownOptionPayloadResult = bool;

  /*!
   * \brief Result of DeserializeOption method.
   */
  using DeserializeOptionResult = std::pair<bool, service_discovery::message::options::ServiceDiscoveryOption>;

  /*!
   * \brief Function to perform the deserialization.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and
   * deserialized data.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Deserializes the LengthOfOptions serialized from a given byte stream.
   * - Check if the length field is valid.
   *   - Set the validation option flag to true.
   *   - Get the SD options length field.
   *   - Check if the SD length of options is equal to the remaining bytes to be read.
   *     - If the options length field is consistent, deserialize the options.
   *   - Else, log an error that options length field is invalid and set the validity status for options to false.
   * - Otherwise,
   *   - Log an error that options length field is invalid.
   * - Return a pair of validity status of the options and the options payload.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSdOptionContainerResult DeserializeSomeIpSdOptions(
      someip_deserialization::Reader& reader) noexcept {
    bool deserialization_success{false};
    service_discovery::message::options::ServiceDiscoveryOptionContainer options{};

    // Deserialize length
    DeserializeSomeIpSdLengthOfOptionsResult const deserialized_length_pair{
        DeserializeSomeIpSdLengthOfOptions<Endian>(reader)};
    bool const is_length_field_valid{deserialized_length_pair.first};
    if (is_length_field_valid) {
      someip_protocol::internal::SdOptionsLength const length_of_options{deserialized_length_pair.second};
      if (length_of_options == reader.Size()) {
        deserialization_success = true;
        //  If there are options in the message and the length of options field is consistent, then we interpret the
        //  remaining bytes in the message as options.
        while (deserialization_success && (reader.Size() > 0U)) {
          DeserializeOptionResult const option_result{DeserializeOption<Endian>(reader)};
          deserialization_success = option_result.first;
          if (deserialization_success) {
            options.push_back(option_result.second);
          }
        }
      } else {
        // If the length of the options array doesn't match the remaining bytes to be read from the datagram.
        // Log an error and set the valid option to false.
        someip_daemon_core::logging::AraComLogger const kLogger{
            someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
            someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
            ara::core::StringView{"ServiceDiscoveryMarshalling"}};
        kLogger.LogError(
            [](ara::log::LogStream& s) { s << "Options length field invalid. This message is discarded."; },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      someip_daemon_core::logging::AraComLogger const kLogger{
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
          ara::core::StringView{"ServiceDiscoveryMarshalling"}};
      kLogger.LogError(
          [](ara::log::LogStream& s) {
            s << "Options length field invalid, expected at least "
              << someip_protocol::internal::kSdLengthOfOptionsFieldSize;
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return std::make_pair(deserialization_success, options);
  }

 private:
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  /*!
   * \brief Function to deserialize option.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and
   * deserialized data.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Deserializes the SomeIpSdEndpointOptionHeader from a given byte stream.
   * - Check if the header is valid.
   *   - Retrieve the header from the deserialized data.
   *   - Calculate the length of the payload from the header.
   *   - Check if options::SomeIpSdEndpointOptionType of type "kIPv4UnicastEndpointOption" or
   *     "kIPv4MulticastEndpointOption".
   *     - Deserializes the SomeIpSdIPv4EndpointOption from a given serialized byte stream.
   *     - Check if the option is valid.
   *       - Get the SD options from an IPv4 payload.
   *     - Otherwise,
   *       - Log an error that insufficient deserialized option bytes.
   *   - Check if options::SomeIpSdEndpointOptionType of type "kIPv6UnicastEndpointOption" or
   *     "kIPv6MulticastEndpointOption".
   *     - Deserializes the SomeIpSdIPv6EndpointOption from a given serialized byte stream.
   *     - Check if the option is valid.
   *       - Get the SD options from an IPv6 payload.
   *     - Otherwise,
   *       - Log an error that insufficient deserialized option bytes.
   *   - As default:
   *     - Deserializes an unknown SomeIpSdIP option's payload from a byte stream.
   *     - Check if the option is valid.
   *       - Get the ServiceDiscoveryOption.
   *     - Otherwise,
   *       - Log an error that insufficient deserialized option bytes.
   * - Otherwise,
   *   - Log an error that options length field is invalid.
   * - Return a pair of validity status of the options and the options payload.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeOptionResult DeserializeOption(someip_deserialization::Reader& reader) noexcept {
    bool is_valid_option{false};
    DeserializeSomeIpSdEndpointOptionHeaderResult const result{DeserializeSomeIpSdEndpointOptionHeader<Endian>(reader)};
    bool const is_header_valid{result.first};
    service_discovery::message::options::ServiceDiscoveryOption option{
        service_discovery::message::options::SomeIpSdEndpointOptionType::kUnknownOption,
        someip_daemon_core::IpAddress(),
        service_discovery::message::options::SomeIpSdEndpointOptionProto::kUnknown,
        {0U},
        true};
    someip_daemon_core::logging::AraComLogger const logger{
        someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
        someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
        ara::core::StringView{"ServiceDiscoveryMarshalling"}};
    if (is_header_valid) {
      someip_protocol::internal::SomeIpSdEndpointOptionHeader const header{result.second};
      std::uint16_t const payload_length{
          static_cast<std::uint16_t>(header.length_ - sizeof(someip_protocol::internal::OptionFlags))};
      switch (static_cast<service_discovery::message::options::SomeIpSdEndpointOptionType>(header.type_)) {
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4MulticastEndpointOption: {
          DeserializeSomeIpSdIPv4EndpointOptionPayloadResult const option_result{
              DeserializeSomeIpSdIPv4EndpointOptionPayload<Endian>(reader, payload_length)};
          is_valid_option = option_result.first;
          if (is_valid_option) {
            option = GetServiceDiscoveryOption(header, option_result.second);
          } else {
            logger.LogError(
                [](ara::log::LogStream& s) {
                  s << "Insufficient deserialized option bytes, expected at least "
                    << someip_protocol::internal::kSomeIpSdIPv4EndpointOptionPayloadSize
                    << ". This message is discarded.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
          break;
        }
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6MulticastEndpointOption: {
          DeserializeSomeIpSdIPv6EndpointOptionPayloadResult const option_result{
              DeserializeSomeIpSdIPv6EndpointOptionPayload<Endian>(reader, payload_length)};
          is_valid_option = option_result.first;
          if (is_valid_option) {
            option = GetServiceDiscoveryOption(header, option_result.second);
          } else {
            logger.LogError(
                [](ara::log::LogStream& s) {
                  s << "Insufficient deserialized option bytes, expected at least "
                    << someip_protocol::internal::kSomeIpSdIPv6EndpointOptionPayloadSize
                    << ". This message is discarded.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
          break;
        }
        // Non-supported options
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kConfigurationOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kLoadBalancingOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4SDEndpointOption:
        case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6SDEndpointOption: {
          logger.LogVerbose(
              [&header](ara::log::LogStream& s) {
                s << "Not supported option type 0x"
                  << ara::log::HexFormat(
                         static_cast<std::underlying_type<
                             service_discovery::message::options::SomeIpSdEndpointOptionType>::type>(header.type_));
              },
              static_cast<char const*>(__func__), __LINE__);
          DeserializeSomeIpSdUnknownOptionPayloadResult const option_result{
              DeserializeSomeIpSdUnknownOptionPayload(reader, payload_length)};
          is_valid_option = option_result;
          if (is_valid_option) {
            option = GetServiceDiscoveryOption(header);
          } else {
            // Logger is created above on entering default path
            logger.LogError(
                [&payload_length](ara::log::LogStream& s) {
                  s << "Insufficient deserialized option bytes, expected at least " << payload_length
                    << ". This message is discarded.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
          break;
        }
        default: {
          logger.LogWarn(
              [&header](ara::log::LogStream& s) {
                s << "Unknown option type 0x"
                  << ara::log::HexFormat(
                         static_cast<std::underlying_type<
                             service_discovery::message::options::SomeIpSdEndpointOptionType>::type>(header.type_));
              },
              static_cast<char const*>(__func__), __LINE__);
          DeserializeSomeIpSdUnknownOptionPayloadResult const option_result{
              DeserializeSomeIpSdUnknownOptionPayload(reader, payload_length)};
          is_valid_option = option_result;
          if (is_valid_option) {
            option = GetServiceDiscoveryOption(header);
          } else {
            // Logger is created above on entering default path
            logger.LogError(
                [&payload_length](ara::log::LogStream& s) {
                  s << "Insufficient deserialized option bytes, expected at least " << payload_length
                    << ". This message is discarded.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
          break;
        }
      }
    } else {
      // Options length field is invalid
      someip_daemon_core::logging::AraComLogger const kLogger{
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
          someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
          ara::core::StringView{"ServiceDiscoveryMarshalling"}};
      logger.LogError(
          [](ara::log::LogStream& s) {
            s << "Options header field invalid, expected at least " << someip_protocol::internal::kSdOptionHeaderSize;
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return std::make_pair(is_valid_option, option);
  }

  /*!
   * \brief Function to convert V4 bytestream address to IpAddress.
   * \param[in] payload The option payload containing the address.
   * \return An IpAddress.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Copy the IPv4 address from the supplied payload to a local array.
   * - Create an AddressV4 instance with current address.
   * - Return the net::ip::Address.
   * \endinternal
   */
  static someip_daemon_core::IpAddress CreateIpV4Address(
      someip_protocol::internal::SomeIpSdIPv4EndpointOptionPayload const& payload) {
    // VECTOR NC VectorC++-V8.5.0: MD_SomeIpDaemon_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    ara::core::Array<std::uint8_t, amsr::net::ip::AddressV4::kAddressBytes> address;
    static_cast<void>(std::copy(std::begin(payload.address_), std::end(payload.address_), std::begin(address)));
    amsr::net::ip::AddressV4 const address_v4{address};

    return someip_daemon_core::IpAddress(address_v4);
  }

  /*!
   * \brief Function to convert V6 bytestream address to IpAddress.
   * \param[in] payload The option payload containing the address.
   * \return An IpAddress.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Copy the IPv6 address from the supplied payload to a local array.
   * - Create an AddressV6 instance with current address.
   * - Return the net::ip::Address.
   * \endinternal
   */
  static someip_daemon_core::IpAddress CreateIpV6Address(
      someip_protocol::internal::SomeIpSdIPv6EndpointOptionPayload const& payload) {
    // VECTOR NC VectorC++-V8.5.0: MD_SomeIpDaemon_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    ara::core::Array<std::uint8_t, amsr::net::ip::AddressV6::kAddressBytes> address;
    static_cast<void>(std::copy(std::begin(payload.address_), std::end(payload.address_), std::begin(address)));
    amsr::net::ip::AddressV6 const address_v6{address};

    return someip_daemon_core::IpAddress(address_v6);
  }

  /*!
   * \brief Function to get options::ServiceDiscoveryOption from an IPv4 payload.
   * \param[in] header The SomeIpSdEndpointOptionHeader
   * \param[in] payload The payload for the IPv4Endpoint option (SomeIpSdIPv4EndpointOptionPayload)
   * \return A options::ServiceDiscoveryOption
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-8054086
   *
   * \internal
   * - Create a options::ServiceDiscoveryOption instance,serialize it from the supplied header, and
   *   SomeIpSdIPv4EndpointOptionPayload payload.
   * - Return the option.
   * \endinternal
   */
  static service_discovery::message::options::ServiceDiscoveryOption GetServiceDiscoveryOption(
      someip_protocol::internal::SomeIpSdEndpointOptionHeader const& header,
      someip_protocol::internal::SomeIpSdIPv4EndpointOptionPayload const& payload) {
    service_discovery::message::options::ServiceDiscoveryOption const option{
        static_cast<service_discovery::message::options::SomeIpSdEndpointOptionType>(header.type_),
        CreateIpV4Address(payload),
        DeserializeSdEndpointOptionProto(payload.proto_),
        {payload.port_},
        someip_protocol::internal::IsSomeIpSdEndpointOptionDiscardable(header)};
    return option;
  }

  /*!
   * \brief Function to get options::ServiceDiscoveryOption from an IPv6 payload.
   * \param[in] header The SomeIpSdEndpointOptionHeader
   * \param[in] payload The payload for the IPv6Endpoint option (SomeIpSdIPv6EndpointOptionPayload)
   * \return A options::ServiceDiscoveryOption
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-8054086
   *
   * \internal
   * - Create a options::ServiceDiscoveryOption instance, serialize it from the supplied header, and
   *   SomeIpSdIPv6EndpointOptionPayload payload.
   * - Return the option.
   * \endinternal
   */
  static service_discovery::message::options::ServiceDiscoveryOption GetServiceDiscoveryOption(
      someip_protocol::internal::SomeIpSdEndpointOptionHeader const& header,
      someip_protocol::internal::SomeIpSdIPv6EndpointOptionPayload const& payload) {
    service_discovery::message::options::ServiceDiscoveryOption const option{
        static_cast<service_discovery::message::options::SomeIpSdEndpointOptionType>(header.type_),
        CreateIpV6Address(payload),
        DeserializeSdEndpointOptionProto(payload.proto_),
        {payload.port_},
        someip_protocol::internal::IsSomeIpSdEndpointOptionDiscardable(header)};
    return option;
  }

  /*!
   * \brief Function to get options::ServiceDiscoveryOption.
   * \param header The SomeIpSdEndpointOptionHeader
   * \return A options::ServiceDiscoveryOption of unknown type.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-8054086
   *
   * \internal
   * - Create a options::ServiceDiscoveryOption instance and serialize it from the supplied header.
   * - Return the option.
   * \endinternal
   */
  static service_discovery::message::options::ServiceDiscoveryOption GetServiceDiscoveryOption(
      someip_protocol::internal::SomeIpSdEndpointOptionHeader const& header) {
    service_discovery::message::options::SomeIpSdEndpointOptionType type{};
    switch (static_cast<service_discovery::message::options::SomeIpSdEndpointOptionType>(header.type_)) {
      case service_discovery::message::options::SomeIpSdEndpointOptionType::kConfigurationOption: {
        type = service_discovery::message::options::SomeIpSdEndpointOptionType::kConfigurationOption;
        break;
      }
      case service_discovery::message::options::SomeIpSdEndpointOptionType::kLoadBalancingOption: {
        type = service_discovery::message::options::SomeIpSdEndpointOptionType::kLoadBalancingOption;
        break;
      }
      case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4SDEndpointOption: {
        type = service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv4SDEndpointOption;
        break;
      }
      case service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6SDEndpointOption: {
        type = service_discovery::message::options::SomeIpSdEndpointOptionType::kIPv6SDEndpointOption;
        break;
      }
      default: {
        type = service_discovery::message::options::SomeIpSdEndpointOptionType::kUnknownOption;
        break;
      }
    }

    service_discovery::message::options::ServiceDiscoveryOption const option{
        type,
        someip_daemon_core::IpAddress(),
        service_discovery::message::options::SomeIpSdEndpointOptionProto::kUnknown,
        {0U},
        someip_protocol::internal::IsSomeIpSdEndpointOptionDiscardable(header)};
    return option;
  }

  /*!
   * \brief Obtain a SomeIpSdEndpointOptionProto from the wire protocol.
   * \param[in] from The wire protocol.
   * \return The associated SomeIpSdEndpointOptionProto value.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Return SomeIpSdEndpointOptionProto Deserialized from the supplied wire protocol.
   * \endinternal
   */
  static service_discovery::message::options::SomeIpSdEndpointOptionProto DeserializeSdEndpointOptionProto(
      std::uint8_t const from) {
    service_discovery::message::options::SomeIpSdEndpointOptionProto result{
        service_discovery::message::options::SomeIpSdEndpointOptionProto::kUnknown};

    switch (from) {
      case kProtoTcp: {
        result = service_discovery::message::options::SomeIpSdEndpointOptionProto::kTcp;
        break;
      }
      case kProtoUdp: {
        result = service_discovery::message::options::SomeIpSdEndpointOptionProto::kUdp;
        break;
      }
      default: {
        // any other proto type is not allowed
        break;
      }
    }
    return result;
  }

  /*!
   * \brief Deserializes the LengthOfOptions serialized from a given byte stream.
   *
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * length.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   * \trace SPEC-10144595
   * \trace SPEC-4981517
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Read the SdOptionsLength considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the length field was valid.
   * - Return a pair of the status of the length field and the length of options.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdLengthOfOptionsResult DeserializeSomeIpSdLengthOfOptions(
      someip_deserialization::Reader& reader) noexcept {
    someip_protocol::internal::SdOptionsLength length_of_options{};
    size_type const size_before{reader.Size()};

    if (reader.VerifySize(someip_protocol::internal::kSdLengthOfOptionsFieldSize)) {
      reader.ReadPrimitive<someip_protocol::internal::SdOptionsLength, Endian>(length_of_options);
    }

    size_type const size_after{reader.Size()};
    bool const is_length_field_valid{(size_before - size_after) ==
                                     someip_protocol::internal::kSdLengthOfOptionsFieldSize};
    return std::make_pair(is_length_field_valid, length_of_options);
  }

  /*!
   * \brief Deserializes the SomeIpSdEndpointOptionHeader from a given serialized byte stream.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * data.
   * \trace SPEC-10144612
   * \trace SPEC-10144605
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Read the length, type, and OptionFlags considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the header was valid.
   * - Return a pair of the status of the header and the header data.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdEndpointOptionHeaderResult DeserializeSomeIpSdEndpointOptionHeader(
      someip_deserialization::Reader& reader) noexcept {
    someip_protocol::internal::SomeIpSdEndpointOptionHeader header{};
    size_type const size_before{reader.Size()};

    if (reader.VerifySize(someip_protocol::internal::kSdOptionHeaderSize)) {
      reader.ReadPrimitive<std::uint16_t, Endian>(header.length_);
      reader.ReadPrimitive<std::uint8_t, Endian>(header.type_);
      reader.ReadPrimitive<someip_protocol::internal::OptionFlags, Endian>(header.flags_);
    }

    size_type const size_after{reader.Size()};
    bool const is_header_valid{((size_before - size_after) == someip_protocol::internal::kSdOptionHeaderSize) &&
                               (header.length_ >= sizeof(someip_protocol::internal::OptionFlags))};
    return std::make_pair(is_header_valid, header);
  }

  /*!
   * \brief Deserializes the SomeIpSdIPv4EndpointOption from a given serialized byte stream.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \param[in] length The length of the option payload.
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * data.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981548
   * \trace SPEC-4981549
   * \trace SPEC-4981550
   * \trace SPEC-4981552
   * \trace SPEC-4981554
   * \trace SPEC-4981563
   * \trace SPEC-4981564
   * \trace SPEC-10144605
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Check if the supplied length is equal to kSomeIpSdIPv4EndpointOptionPayloadSize.
   *     - Read the 4-byte address considering the endian.
   *     - Read the reserved byte, protocol and port considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the options was valid.
   * - Return a pair of the status of the options and the options data.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdIPv4EndpointOptionPayloadResult DeserializeSomeIpSdIPv4EndpointOptionPayload(
      someip_deserialization::Reader& reader, std::uint16_t const length) noexcept {
    someip_protocol::internal::SomeIpSdIPv4EndpointOptionPayload option{};
    size_type const size_before{reader.Size()};

    if (reader.VerifySize(someip_protocol::internal::kSomeIpSdIPv4EndpointOptionPayloadSize)) {
      if (length == someip_protocol::internal::kSomeIpSdIPv4EndpointOptionPayloadSize) {
        constexpr size_type length_of_address{4U};
        for (size_type address_index{0U}; address_index < length_of_address; ++address_index) {
          reader.ReadPrimitive<std::uint8_t, Endian>(option.address_[address_index]);
        }
        reader.ReadPrimitive<std::uint8_t, Endian>(option.reserved_);
        reader.ReadPrimitive<std::uint8_t, Endian>(option.proto_);
        reader.ReadPrimitive<std::uint16_t, Endian>(option.port_);
      }
    }

    size_type const size_after{reader.Size()};
    bool const is_valid_option{(size_before - size_after) ==
                               someip_protocol::internal::kSomeIpSdIPv4EndpointOptionPayloadSize};
    return std::make_pair(is_valid_option, option);
  }

  /*!
   * \brief Deserializes the SomeIpSdIPv6EndpointPayloadOption from a given serialized byte stream.
   * \tparam Endian The endianness.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \param[in] length The length of the payload as declared in the option header
   * \return A pair of a boolean which specifies whether the deserialization has been successful or not and deserialized
   * data.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981555
   * \trace SPEC-4981556
   * \trace SPEC-4981558
   * \trace SPEC-4981560
   * \trace SPEC-4981568
   * \trace SPEC-4981569
   * \trace SPEC-10144605
   *
   * \internal
   * - Get the size of read buffer view.
   * - Check if the given amount of bytes can be read from the buffer view.
   *   - Check if the supplied length is equal to kSomeIpSdIPv6EndpointOptionPayloadSize.
   *     - Read the 16-byte address considering the endian.
   *     - Read the reserved byte, protocol and port considering the endian.
   * - Get the size of buffer view after reading.
   * - Check whether the options was valid.
   * - Return a pair of the status of the options and the options data.
   * \endinternal
   */
  template <typename Endian = NetworkByteOrder>
  static DeserializeSomeIpSdIPv6EndpointOptionPayloadResult DeserializeSomeIpSdIPv6EndpointOptionPayload(
      someip_deserialization::Reader& reader, std::uint16_t const length) noexcept {
    someip_protocol::internal::SomeIpSdIPv6EndpointOptionPayload options{};
    size_type const size_before{reader.Size()};

    if (reader.VerifySize(someip_protocol::internal::kSomeIpSdIPv6EndpointOptionPayloadSize)) {
      if (length == someip_protocol::internal::kSomeIpSdIPv6EndpointOptionPayloadSize) {
        constexpr size_type length_of_address{16U};
        for (size_type address_index{0U}; address_index < length_of_address; ++address_index) {
          reader.ReadPrimitive<std::uint8_t, Endian>(options.address_[address_index]);
        }
        reader.ReadPrimitive<std::uint8_t, Endian>(options.reserved_);
        reader.ReadPrimitive<std::uint8_t, Endian>(options.proto_);
        reader.ReadPrimitive<std::uint16_t, Endian>(options.port_);
      }
    }

    size_type const size_after{reader.Size()};
    bool const is_valid_option{(size_before - size_after) ==
                               someip_protocol::internal::kSomeIpSdIPv6EndpointOptionPayloadSize};
    return std::make_pair(is_valid_option, options);
  }

  /*!
   * \brief Deserializes an unknown SomeIpSdIPv6 option's payload from a byte stream.
   * \param[in,out] reader The reader that contains the serialized payload.
   * \param[in] length The length of the payload as declared in the option header
   * \return If deserialization was successful, true is returned. Otherwise, false is returned.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   */
  static DeserializeSomeIpSdUnknownOptionPayloadResult DeserializeSomeIpSdUnknownOptionPayload(
      someip_deserialization::Reader& reader, std::uint16_t const length) {
    DeserializeSomeIpSdUnknownOptionPayloadResult const is_valid_option{reader.Skip(static_cast<std::size_t>(length))};
    return is_valid_option;
  }
};

}  // namespace marshalling
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MARSHALLING_DESERIALIZATION_SOMEIP_SD_OPTIONS_DESERIALIZER_H_

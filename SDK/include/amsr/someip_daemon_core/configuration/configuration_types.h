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
/*!        \file  configuration_types.h
 *        \brief  Common type definitions used within the SOME/IP daemon.
 *      \details  This module is used to create type aliases to avoid using raw built-in types.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CONFIGURATION_TYPES_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CONFIGURATION_TYPES_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

#include "amsr/ipc/unicast_address.h"
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/network.h"
#include "amsr/someip_daemon_core/configuration/types/initial_sd_delay_config.h"
#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/variant.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/guid.h"
#include "ara/log/logstream.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_tp/someip_tp_types.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/tls_comm_general_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_daemon_core {

/*!
 * \brief Alias for Address type.
 */
using IpAddress = amsr::net::ip::Address;

namespace configuration {

/*!
 * \brief Generic function that checks two containers for equal elements
 * \tparam ElementT Explicitly provided type of the elements in the containers
 * \tparam ContainerT Deduced type of the container
 * \param[in] container1 The first container that is compared to the second one.
 * \param[in] container2 The second container that is compared to the first one.
 * \param[in] all Return "true" only if all elements are equal else return "true" if at least one match is found.
 * \return "true" if at least one or all elements ( based on \p all ) match, "false" otherwise
 *         (also "false" if one of the containers is empty).
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
template <typename ContainerT>
auto CompareContainers(ContainerT const& container1, ContainerT const& container2, bool all) noexcept -> bool {
  using ElementT = typename ContainerT::value_type;

  bool result{all};

  // VECTOR NL AutosarC++17_10-M0.1.2: MD_SomeIpDaemon_AutosarC++17_10-M0.1.2_dead_branch_false_positive
  if (all) {
    result = container1.size() == container2.size();
  }

  if (result == all) {
    for (ElementT const& element : container1) {
      typename ContainerT::const_iterator const element_it{std::find(container2.cbegin(), container2.cend(), element)};
      result = element_it != container2.cend();
      if (result != all) {
        break;
      }
    }
  }

  return result;
}

/*!
 * \brief Generic function that checks two containers if their elements are the same
 * and they both have the same size.
 * \tparam ElementT Explicitly provided type of the elements in the containers
 * \tparam ContainerT Deduced type of the container
 * \param[in] container1 The first container that is compared to the second one.
 * \param[in] container2 The second container that is compared to the first one.
 * \return "true" if both containers are of the same size and the elements contained
 *         are equal; "false" if the containers are not of the same size or one of the elements differ.
 * \pre -
 * \context ANY
 * \reentrant FALSE
 * \steady FALSE
 */
template <typename ContainerT>
auto Compare(ContainerT const& container1, ContainerT const& container2) noexcept -> bool {
  return CompareContainers(container1, container2, true);
}

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals.
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief Configuration declarations other configuration-related classes may derive from for usage.
 */
class ConfigurationTypesAndDefs {
 public:
  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  ConfigurationTypesAndDefs() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ConfigurationTypesAndDefs(ConfigurationTypesAndDefs const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ConfigurationTypesAndDefs(ConfigurationTypesAndDefs&&) = delete;

  /*!
   * \brief Deleted copy assignment.
   */
  auto operator=(ConfigurationTypesAndDefs const&) & -> ConfigurationTypesAndDefs& = delete;

  /*!
   * \brief Deleted move assignment.
   */
  auto operator=(ConfigurationTypesAndDefs&&) & -> ConfigurationTypesAndDefs& = delete;

  /*!
   * \brief Represents a dynamic port.
   */
  static constexpr someip_protocol::internal::Port kInvalidPort{0U};

  /*!
   * \brief Represents a default Socket Option Linger.
   */
  static constexpr someip_protocol::internal::EnableSocketOptionLinger kDefaultSocketOptionLinger{false};

  /*!
   * \brief Represents a default network mask.
   */
  static constexpr char const* kDefaultNetworkMask{"0.0.0.0"};

  /*!
   * \brief Represents a default prefix length.
   */
  static constexpr someip_protocol::internal::IpAddressPrefixLength kDefaultPrefixLength{0U};

  /*!
   * \brief Represents any service.
   * \trace SPEC-10144629
   */
  static constexpr someip_protocol::internal::ServiceId kServiceIdAny{0xFFFFU};

  /*!
   * \brief Represents ALL service instances (used for finding all service instances).
   * \trace SPEC-10144629
   */
  static constexpr someip_protocol::internal::InstanceId kInstanceIdAll{0xFFFFU};

  /*!
   * \brief Represents any major version of a service.
   * \details It shall be used only within these contexts:
   *         - PDU handling, as PDU IDs do not contain MajorVersion.
   *         - IAM, as MajorVersion is irrelevant for the moment.
   *         - Reception of FindService messages with kMajorVersionAny is allowed (and must be answered).
   */
  static constexpr someip_protocol::internal::MajorVersion kMajorVersionAny{0xFFU};

  /*!
   * \brief Represents any minor version of a service.
   */
  static constexpr someip_protocol::internal::MinorVersion kMinorVersionAny{0xFFFFFFFFU};

  /*!
   * \brief Enum describing the two possible mechanisms to consider minor version when searching
   *        for services.
   */
  enum class VersionDrivenFindBehavior : std::uint8_t {
    /*!
     * \brief Either a specific minor version or ANY (if minor version is configured with kMinorVersionAny)
     */
    kExactOrAnyMinorVersion = 0U,
    /*!
     * \brief Minimum minor version search (kMinorVersionAny not allowed)
     */
    kMinimumMinorVersion = 1U
  };

  /*!
   * \brief Default value of VersionDrivenFindBehavior enum.
   */
  static constexpr VersionDrivenFindBehavior kDefaultVersionDrivenFindBehavior{
      VersionDrivenFindBehavior::kExactOrAnyMinorVersion};

  /*!
   * \brief Type-alias for maximum initial repetitions
   */
  using InitialRepetitionsMax = types::InitialRepetitionsMax;

  /*!
   * \brief Represents SubscribeEventgroupRetry Infinite.
   */
  static constexpr someip_protocol::internal::SubscribeEventgroupRetryMax kSubscribeEventgroupRetryInfinite{0xFFU};

  /*!
   * \brief Represents TTL Infinite.
   */
  static constexpr someip_protocol::internal::Ttl kTtlInfinite{0xFFFFFF};

  /*!
   * \brief Default minimum interval for overload protection filter.
   */
  static constexpr std::chrono::nanoseconds kDefaultOverloadProtectionMinInterval{std::chrono::nanoseconds::zero()};

  /*!
   * \brief Default RequestResponseDelay min/max time interval for responses to multicast service discovery requests.
   */
  static constexpr std::chrono::nanoseconds kDefaultRequestResponseDelayInterval{std::chrono::nanoseconds::zero()};

  /*!
   * \brief Default OfferCyclicDelay.
   */
  static constexpr std::chrono::nanoseconds kDefaultOfferCyclicDelay{std::chrono::nanoseconds::zero()};

  /*!
   * \brief Default InitialRepetitionsMax for service discovery.
   */
  static constexpr InitialRepetitionsMax kDefaultInitialRepetitionsMax{0};

  /*!
   * \brief Default InitialRepetitionsBaseDelay for service discovery.
   */
  static constexpr std::chrono::nanoseconds kDefaultInitialRepetitionsBaseDelay{std::chrono::nanoseconds::zero()};

  /*!
   * \brief Default InitialDelay min/max time interval for offer and find service messages.
   */
  static constexpr std::chrono::nanoseconds kDefaultInitialDelayInterval{std::chrono::nanoseconds::zero()};

  /*!
   * \brief Layer 4 protocols.
   */
  enum class Protocol : std::uint8_t {
    /*!
     * \brief TCP protocol
     */
    kTCP = 0U,

    /*!
     * \brief UDP protocol
     */
    kUDP
  };

  /*!
   * \brief String representations of available protocols.
   */
  static constexpr std::array<ara::core::StringView, 2U> kProtocolString{"TCP"_sv, "UDP"_sv};

  /*!
   * \brief Convert the protocol to String.
   * \details Used for logging.
   *
   * \param[in] protocol The protocol to convert
   *
   * \return The string representation of the given protocol.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static auto GetProtocolAsString(Protocol const protocol) noexcept -> ara::core::String {
    std::uint8_t const proto{static_cast<std::underlying_type_t<Protocol>>(protocol)};
    return ara::core::String{kProtocolString.at(proto)};
  }

  /*!
   * \brief Type-alias for event multicast threshold
   */
  using EventMulticastThreshold = std::uint32_t;

  /*!
   * \brief Alias for UDP collection buffer size threshold
   */
  using UdpCollectionBufferSizeThreshold = std::uint32_t;

  /*!
   * \brief The value of event multicast threshold that disables multicast event communication.
   * \trace SPEC-5759758
   */
  static constexpr EventMulticastThreshold kEventMulticastThresholdUseOnlyUnicast{0U};

  /*!
   * \brief The value of SOME/IP-TP segment length that disables segmentation of the corresponding SOME/IP message.
   */
  static constexpr someip_tp::SomeIpTpSegmentSize kSomeIpTpSegmentSizeNoSegmentation{0U};

  /*!
   * \brief The value of SOME/IP-TP separation time that disables waiting between segments.
   */
  static constexpr std::chrono::nanoseconds kSomeIpTpSeparationTimeNoWait{std::chrono::nanoseconds::zero()};

  /*!
   * \brief The default number of SOME/IP-TP rx preallocation buffers.
   */
  static constexpr someip_tp::SomeIpTpRxBufferCount kSomeIpTpRxBufferCountDefault{0U};

  /*!
   * \brief The value of SOME/IP-TP segment length that disables segmentation of the corresponding SOME/IP message.
   */
  static constexpr someip_tp::SomeIpTpSegmentSize kSomeIpTpBurstSizeDefault{1U};

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief Represents SOME/IP-TP configurations for method.
   */
  class MethodSomeIpTp {
   public:
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare two objects of Event SomeIpTp.
     * \param[in] other The other object to compare to this object.
     * \return true if both are inactive, or both are active and have the same parameters; false otherwise.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(MethodSomeIpTp const& other) const noexcept {
      return (is_active_ == other.is_active_) &&
             ((!is_active_) ||
              ((separation_time_request_ == other.separation_time_request_) &&
               (separation_time_response_ == other.separation_time_response_) &&
               (maximum_segment_length_request_ == other.maximum_segment_length_request_) &&
               (maximum_segment_length_response_ == other.maximum_segment_length_response_) &&
               (maximum_rx_message_length_ == other.maximum_rx_message_length_) &&
               (rx_buffer_count_ == other.rx_buffer_count_) && (burst_size_request_ == other.burst_size_request_) &&
               (burst_size_response_ == other.burst_size_response_)));
    }

    /*!
     * \brief Indicate whether SomeIpTp is active for this method.
     */
    bool is_active_{};

    /*!
     * \brief Minimum separation time between sending consecutive request segments.
     */
    std::chrono::nanoseconds separation_time_request_{};

    /*!
     * \brief Minimum separation time between sending consecutive response segments.
     */
    std::chrono::nanoseconds separation_time_response_{};

    /*!
     * \brief Maximum length of a method request segment.
     */
    someip_tp::SomeIpTpSegmentSize maximum_segment_length_request_{};

    /*!
     * \brief Maximum length of a method response segment
     */
    someip_tp::SomeIpTpSegmentSize maximum_segment_length_response_{};

    /*!
     * \brief Maximum length of the received message.
     */
    someip_tp::SomeIpTpMessageSize maximum_rx_message_length_{};

    /*!
     * \brief Number of buffers to pre-allocate for message reception.
     */
    someip_tp::SomeIpTpRxBufferCount rx_buffer_count_{kSomeIpTpRxBufferCountDefault};

    /*!
     * \brief Burst size for requests
     */
    someip_tp::SomeIpTpBurstSize burst_size_request_{kSomeIpTpBurstSizeDefault};

    /*!
     * \brief Burst size for responses
     */
    someip_tp::SomeIpTpBurstSize burst_size_response_{kSomeIpTpBurstSizeDefault};
  };

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief Represents SOME/IP-TP configurations for event.
   */
  class EventSomeIpTp {
   public:
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare two objects of Event SomeIpTp.
     * \param[in] other The other object to compare to this object.
     * \return true if both are inactive, or both are active and have the same parameters; false otherwise.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(EventSomeIpTp const& other) const noexcept {
      return (is_active_ == other.is_active_) &&
             ((!is_active_) || ((separation_time_ == other.separation_time_) &&
                                (maximum_segment_length_ == other.maximum_segment_length_) &&
                                (maximum_rx_message_length_ == other.maximum_rx_message_length_) &&
                                (rx_buffer_count_ == other.rx_buffer_count_) && (burst_size_ == other.burst_size_)));
    }

    /*!
     * \brief Indicate whether SomeIpTp is active for this event.
     */
    bool is_active_{};

    /*!
     * \brief Minimum separation time between sending consecutive segments.
     */
    std::chrono::nanoseconds separation_time_{};

    /*!
     * \brief Maximum length of an event segment.
     */
    someip_tp::SomeIpTpSegmentSize maximum_segment_length_{};

    /*!
     * \brief Maximum length of the received message.
     */
    someip_tp::SomeIpTpMessageSize maximum_rx_message_length_{};

    /*!
     * \brief Number of buffers to pre-allocate for message reception.
     */
    someip_tp::SomeIpTpRxBufferCount rx_buffer_count_{kSomeIpTpRxBufferCountDefault};

    /*!
     * \brief Burst size
     */
    someip_tp::SomeIpTpBurstSize burst_size_{kSomeIpTpBurstSizeDefault};
  };

  /*!
   * \brief Represents a SOME/IP method.
   */
  struct Method {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare two objects of type Method based on their ID and their transport protocol in use.
     * \param[in] other The other method to compare to this object.
     * \return true if method ID, proto and someip_tp parameters  are the same; false if not.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(Method const& other) const noexcept {
      return (id_ == other.id_) && (proto_ == other.proto_) && (someip_tp_ == other.someip_tp_);
    }

    /*!
     * \brief The method identifier.
     */
    someip_protocol::internal::MethodId id_{};

    /*!
     * \brief The protocol used for this method.
     */
    Protocol proto_{};

    /*!
     * \brief The SOME/IP-TP parameters.
     */
    MethodSomeIpTp someip_tp_{};
  };

  /*!
   * \brief Method container.
   */
  using MethodContainer = ara::core::Vector<Method>;

  /*!
   * \brief Represents an overload protection filter.
   */
  struct OverloadProtectionFilter {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare two objects of type OverloadProtectionFilter based on min_intervals.
     *
     * \param[in] other The OverloadProtectionFilter to compare with.
     * \return true if objects are same; false if they differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(OverloadProtectionFilter const& other) const noexcept {
      return (min_interval_ns == other.min_interval_ns);
    }

    /*!
     * \brief The minimum interval for filter (For events, any event received in this interval is dropped).
     */
    std::chrono::nanoseconds min_interval_ns{kDefaultOverloadProtectionMinInterval};
  };

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief Represents a SOME/IP event or field notifier configuration.
   */
  class Event {
   public:
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare two objects of type Event based on their ID and their transport protocol in use.
     *
     * \param[in] other The event to compare with.
     * \return true if event ID, is_field option, proto and someip_tp parameters are the same; false if one of them
     * differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(Event const& other) const noexcept {
      return (id_ == other.id_) && (is_field_ == other.is_field_) && (proto_ == other.proto_) &&
             (is_signal_based_ == other.is_signal_based_) && (someip_tp_ == other.someip_tp_) &&
             (overload_protection_filter_ == other.overload_protection_filter_) &&
             (timestamp_flag_ == other.timestamp_flag_);
    }

    /*!
     * \brief The event identifier.
     */
    someip_protocol::internal::EventId id_{};

    /*!
     * \brief Is this a field event?
     */
    bool is_field_{};

    /*!
     * \brief The protocol used for the event.
     */
    Protocol proto_{};

    /*!
     * \brief Is this signal based event?
     */
    bool is_signal_based_{false};

    /*!
     * \brief The SOME/IP-TP parameters.
     */
    EventSomeIpTp someip_tp_{};

    /*!
     * \brief The event filter.
     */
    OverloadProtectionFilter overload_protection_filter_{};

    /*!
     * \brief The timestamp_flag.
     */
    bool timestamp_flag_{};
  };

  /*!
   * \brief Event container.
   */
  using EventContainer = ara::core::Vector<Event>;

  /*!
   * \brief Event identifier container.
   */
  using EventIdContainer = ara::core::Vector<someip_protocol::internal::EventId>;

  /*!
   * \brief Represents a SOME/IP eventgroup.
   */
  struct Eventgroup {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare two objects of type Eventgroup based on their ID and the event IDs contained.
     * \param[in] other The Eventgroup to compare with.
     * \return true if event IDs and EventIdContainer size are equal, otherwise false.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(Eventgroup const& other) const noexcept {
      bool equal{true};

      if (events_.size() != other.events_.size()) {
        equal = false;
      } else {
        for (someip_protocol::internal::EventId const& event_ref_id : events_) {
          EventIdContainer::const_iterator const it{
              std::find(other.events_.cbegin(), other.events_.cend(), event_ref_id)};
          if (it == other.events_.cend()) {
            equal = false;
          }
        }
      }

      return (id_ == other.id_) && equal;
    }

    /*!
     * \brief The event group identifier.
     */
    someip_protocol::internal::EventgroupId id_{};

    /*!
     * \brief The events that belong to this group.
     */
    EventIdContainer events_{};
  };

  /*!
   * \brief Eventgroup container.
   */
  using EventgroupContainer = ara::core::Vector<Eventgroup>;

  /*!
   * \brief Eventgroup identifier container.
   */
  using EventgroupIdContainer = ara::core::Vector<someip_protocol::internal::EventgroupId>;

  /*!
   * \brief Represents a SOME/IP service.
   */
  struct Service {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Comparing two services by their SOME/IP identifiers, the major and minor version.
     *
     * \param[in] rhs The service to compare with this instance.
     * \return true if all parameters (id, major, minor version) of one service match; false if not.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(Service const& rhs) const noexcept {
      return (id_ == rhs.id_) && (major_version_ == rhs.major_version_) && (minor_version_ == rhs.minor_version_);
    }

    /*!
     * \brief The service identifier.
     */
    someip_protocol::internal::ServiceId id_{};

    /*!
     * \brief The service major version.
     */
    someip_protocol::internal::MajorVersion major_version_{};

    /*!
     * \brief The service minor version.
     */
    someip_protocol::internal::MinorVersion minor_version_{};

    /*!
     * \brief The methods that belong to this service.
     */
    MethodContainer methods_{};

    /*!
     * \brief The events that belong to this service.
     */
    EventContainer events_{};

    /*!
     * \brief The event groups that belong to this service.
     */
    EventgroupContainer eventgroups_{};
  };

  /*!
   * \brief Service container.
   */
  using ServiceContainer = ara::core::Vector<Service>;

  /*!
   * \brief Represents a signal serialized Pdu.
   */
  struct SignalSerializedPdu {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare this signal serialized pdu with another one.
     *
     * \param[in] other The service instance to compare with.
     * \return true if pdu ID and event ID are the same;
     * false if one of these IDs differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(SignalSerializedPdu const& other) const noexcept {
      bool const equal{std::tie(pdu_id_, event_id_) == std::tie(other.pdu_id_, other.event_id_)};
      return equal;
    }

    /*!
     * \brief The Pdu identifier.
     */
    someip_protocol::internal::PduId pdu_id_{};

    /*!
     * \brief The Event identifier.
     */
    someip_protocol::internal::EventId event_id_{};
  };

  /*!
   * \brief Container of SignalSerializedPdu.
   */
  using SignalSerializedPduContainer = ara::core::Vector<SignalSerializedPdu>;

  /*!
   * \brief IP address / port pair.
   */
  struct SocketAddress {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief SocketAddress are compared by their addresses and ports.
     *
     * \param[in] other the SocketAddress object to compare with.
     * \return true if all the parameters are the same; false if one of them differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    inline bool operator<(SocketAddress const& other) const noexcept {
      bool result{false};

      ara::core::String const local_address_string{address_.ToString()};
      ara::core::String const other_address_string{other.address_.ToString()};
      result = local_address_string < other_address_string;

      if (address_ == other.address_) {
        result = port_.port < other.port_.port;
      }

      return result;
    }

    /*!
     * \brief IP Address
     */
    someip_daemon_core::IpAddress address_{};

    /*!
     * \brief Port
     */
    amsr::net::ip::Port port_{};
  };

  /*!
   * \brief Represents the type of communication that a provided service instance implements
   */
  enum class ServiceInstanceCommunicationType : std::uint8_t {
    /*!
     * \brief  Service discovery will be done for this service as well as communication management
     *         (sockets will be opened and communication packets will be routed)
     */
    kSdAndCommunication,

    /*!
     * \brief Only service discovery will be done for this service instance. No communication sockets will be
     *        opened and no communication packets will be routed.
     */
    kSdOnly,

    /*!
     * \brief Only communication will be done (sockets will be opened and communication packets will be
     *        routed). No service discovery will be done for this service instance
     */
    kCommunicationOnly
  };

  /*!
   * \brief Represents a subscriber to a service.
   */
  struct ServiceSubscriberConfiguration {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare two objects of ServiceSubscriberConfiguration.
     * \param[in] other The other object to compare to this object.
     * \return true if both have the same parameters; false otherwise.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(ServiceSubscriberConfiguration const& other) const noexcept {
      return (address_ == other.address_) && (udp_port_ == other.udp_port_) && (eventgroup_ == other.eventgroup_);
    }

    /*!
     * \brief The IP address.
     */
    someip_daemon_core::IpAddress address_{};

    /*!
     * \brief The UDP port number.
     */
    someip_protocol::internal::Port udp_port_{};

    /*!
     * \brief The event group container.
     */
    EventgroupIdContainer eventgroup_{};
  };

  /*!
   * \brief Container of ServiceSubscriberConfiguration.
   */
  using ServiceSubscriberConfigurationContainer = ara::core::Vector<ServiceSubscriberConfiguration>;

  /*!
   * \brief Represents a service provider.
   */
  struct ServiceProviderConfiguration {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare two objects of ServiceProviderConfiguration.
     * \param[in] other The other object to compare to this object.
     * \return true if both have the same parameters; false otherwise.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(ServiceProviderConfiguration const& other) const noexcept {
      // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
      return (address_ == other.address_) && (udp_port_ == other.udp_port_) && (tcp_port_ == other.tcp_port_) &&
             (event_multicast_address_ == other.event_multicast_address_) &&
             (event_multicast_port_ == other.event_multicast_port_);
    }

    /*!
     * \brief The IP address.
     */
    ara::core::Optional<someip_daemon_core::IpAddress> address_{};

    /*!
     * \brief The UDP port number.
     * \details Configuring UDP port is optional, but either UDP or TCP port shall be configured.
     */
    ara::core::Optional<someip_protocol::internal::Port> udp_port_{};

    /*!
     * \brief The TCP port number.
     * \details Configuring TCP port is optional, but either UDP or TCP port shall be configured.
     */
    ara::core::Optional<someip_protocol::internal::Port> tcp_port_{};

    /*!
     * \brief The event multicast IP address.
     */
    ara::core::Optional<someip_daemon_core::IpAddress> event_multicast_address_{};

    /*!
     * \brief The event multicast port number.
     */
    ara::core::Optional<someip_protocol::internal::Port> event_multicast_port_{};
  };

  /*!
   * \brief Type definition for the secure id of a secure connection.
   */
  using SecureId = std::uint32_t;

  /*!
   * \brief A list of secure connection IDs.
   */
  using SecureIdContainer = ara::core::Vector<SecureId>;

  /*!
   * \brief Represents a mapping to a network endpoint port.
   */
  struct MachineMapping {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Network endpoints are compared by their addresses and ports.
     *
     * \param[in] other the MachineMapping object to compare with.
     * \return true if all the parameters are the same; false if one of them differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(MachineMapping const& other) const noexcept {
      // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
      return (address_ == other.address_) && (udp_port_ == other.udp_port_) && (tcp_port_ == other.tcp_port_) &&
             (event_multicast_address_ == other.event_multicast_address_) &&
             (event_multicast_port_ == other.event_multicast_port_) && (network_mask_ == other.network_mask_) &&
             (prefix_length_ == other.prefix_length_) && (communication_type_ == other.communication_type_) &&
             (communication_enabled_ == other.communication_enabled_) &&
             (service_discovery_enabled_ == other.service_discovery_enabled_) && (provider_ == other.provider_) &&
             (subscribers_ == other.subscribers_) && (provided_pdus_ == other.provided_pdus_) &&
             (udp_collection_buffer_size_threshold_ == other.udp_collection_buffer_size_threshold_) &&
             (eventgroup_udp_endpoint_address_ == other.eventgroup_udp_endpoint_address_) &&
             (eventgroup_tcp_endpoint_address_ == other.eventgroup_tcp_endpoint_address_) &&
             (eventgroup_udp_endpoint_port_ == other.eventgroup_udp_endpoint_port_) &&
             (eventgroup_tcp_endpoint_port_ == other.eventgroup_tcp_endpoint_port_);
    }

    /*!
     * \brief The network endpoint IP address.
     */
    ara::core::Optional<someip_daemon_core::IpAddress> address_{};

    /*!
     * \brief The UDP port.
     */
    ara::core::Optional<someip_protocol::internal::Port> udp_port_{};

    /*!
     * \brief The TCP port.
     */
    ara::core::Optional<someip_protocol::internal::Port> tcp_port_{};

    /*!
     * \brief The event multicast IP address.
     */
    ara::core::Optional<someip_daemon_core::IpAddress> event_multicast_address_{};

    /*!
     * \brief The event multicast port.
     */
    someip_protocol::internal::Port event_multicast_port_{};

    /*!
     * \brief The network mask.
     */
    someip_daemon_core::IpAddress network_mask_{};

    /*!
     * \brief The IP address prefix length.
     */
    someip_protocol::internal::IpAddressPrefixLength prefix_length_{0U};

    /*!
     * \brief Network object representing IP address and network mask (IPv4) or IP address and prefix length (IPv6).
     */
    amsr::net::ip::Network network_{};

    /*!
     * \brief The communication type this service instance implements
     *        SD and Communication is the default communication type.
     */
    ServiceInstanceCommunicationType communication_type_{ServiceInstanceCommunicationType::kSdAndCommunication};

    /*!
     * \brief This value represent the usage of communication management.
     */
    bool communication_enabled_{true};

    /*!
     * \brief This value represent the usage of service discovery.
     */
    bool service_discovery_enabled_{true};

    /*!
     * \brief The subscriber of the provided service.
     */
    ara::core::Optional<ServiceSubscriberConfigurationContainer> subscribers_{};

    /*!
     * \brief provider of the required service.
     */
    ara::core::Optional<ServiceProviderConfiguration> provider_{};

    /*!
     * \brief Provided signal serialized PDUs.
     */
    SignalSerializedPduContainer provided_pdus_{};

    /*!
     * \brief UDP collection buffer size threshold
     */
    UdpCollectionBufferSizeThreshold udp_collection_buffer_size_threshold_{};

    /*!
     * \brief IP Address to be written in the UDP endpoint option of the subscribe / stop subscribe SD messages.
     */
    ara::core::Optional<someip_daemon_core::IpAddress> eventgroup_udp_endpoint_address_{};

    /*!
     * \brief IP Address to be written in the TCP endpoint option of the subscribe / stop subscribe SD messages.
     */
    ara::core::Optional<someip_daemon_core::IpAddress> eventgroup_tcp_endpoint_address_{};

    /*!
     * \brief The Port to be written in the UDP endpoint option of the subscribe / stop subscribe SD messages.
     */
    ara::core::Optional<someip_protocol::internal::Port> eventgroup_udp_endpoint_port_{};

    /*!
     * \brief The Port to be written in the TCP endpoint option of the subscribe / stop subscribe SD messages.
     */
    ara::core::Optional<someip_protocol::internal::Port> eventgroup_tcp_endpoint_port_{};

    /*!
     * \brief The link to the secure connection configuration used in the machine mapping. This attribute is only
     * used for machine mappings configured for a required service instance and not for provided service instances.
     */
    ara::core::Optional<SecureIdContainer> secure_connection_ids;
  };

  /*!
   * \brief Container for machine mappings.
   */
  using MachineMappingContainer = ara::core::Vector<MachineMapping>;

  /*!
   * \brief Type alias for SubscribeEventgroupRetryDelay.
   */
  using SubscribeEventgroupRetryDelay = someip_protocol::internal::SubscribeEventgroupRetryDelay;

  /*!
   * \brief Type alias for SubscribeEventgroupRetryMax.
   */
  using SubscribeEventgroupRetryMax = someip_protocol::internal::SubscribeEventgroupRetryMax;

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief SD eventgroup parameters for a required service instance.
   */
  class RequiredServiceInstanceSdEventgroup {
   public:
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compares two RequiredServiceInstanceSdEventgroup objects.
     *
     * \param[in] other The instance to compare with.
     * \return true if all parameters are the same; false if one of them differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(RequiredServiceInstanceSdEventgroup const& other) const noexcept {
      return (id_ == other.id_) &&
             (subscribe_eventgroup_retry_delay_ms_ == other.subscribe_eventgroup_retry_delay_ms_) &&
             (subscribe_eventgroup_retry_max_ == other.subscribe_eventgroup_retry_max_) && (ttl_ == other.ttl_) &&
             (request_response_delay_min_ == other.request_response_delay_min_) &&
             (request_response_delay_max_ == other.request_response_delay_max_);
    }

    /*!
     * \brief The event group identifier.
     */
    someip_protocol::internal::EventgroupId id_{};

    /*!
     * \brief The SubscribeEventgroupRetryDelay in the SubscribeEventgroup entry.
     */
    ara::core::Optional<SubscribeEventgroupRetryDelay> subscribe_eventgroup_retry_delay_ms_{ara::core::nullopt};

    /*!
     * \brief The SubscribeEventgroupRetryMax in the SubscribeEventgroup entry.
     */
    ara::core::Optional<SubscribeEventgroupRetryMax> subscribe_eventgroup_retry_max_{ara::core::nullopt};

    /*!
     * \brief The time to live (in seconds) in the SubscribeEventgroup entry.
     */
    someip_protocol::internal::Ttl ttl_{};

    /*!
     * \brief The minimum delay in the SubscribeEventgroup entry.
     */
    std::chrono::nanoseconds request_response_delay_min_{};

    /*!
     * \brief The maximum delay in the SubscribeEventgroup entry.
     */
    std::chrono::nanoseconds request_response_delay_max_{};
  };

  /*!
   * \brief Container of SD eventgroup parameters for a required service.
   */
  using RequiredServiceInstanceSdEventgroupContainer = ara::core::Vector<RequiredServiceInstanceSdEventgroup>;

  /*!
   * \brief Internal representation of both SomeipMethodProps and SomeipEventProps.
   *
   */
  struct SomeipMethodEventProps {
    /*!
     * \brief Timeout element of this someip daemon posix json object.
     */
    std::chrono::nanoseconds timeout;
    /*!
     * \brief Id for the method or event.
     */
    someip_protocol::internal::MethodId id;
    /*!
     * \brief Trigger element of this someip daemon posix json object.
     */
    bool is_trigger;

    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare based on the id for sorting purposes.
     * \steady FALSE
     */
    constexpr bool operator<(SomeipMethodEventProps const& other) const { return id < other.id; }
  };

  /*!
   * \brief Container of SomeipMethodEventProps
   */
  using SomeipMethodEventPropsContainer = ara::core::Vector<SomeipMethodEventProps>;

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief SD parameters for a required service.
   */
  class RequiredServiceInstanceServiceDiscovery {
   public:
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compare the service discovery parameters from two required service instances.
     *
     * \param[in] other The RequiredServiceInstanceServiceDiscovery to compare this instance with.
     * \return true if all attributes of two instances match; false if one of them are not equal.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(RequiredServiceInstanceServiceDiscovery const& other) const noexcept {
      return (ttl_ == other.ttl_) && (initial_repetitions_max_ == other.initial_repetitions_max_) &&
             (initial_delay_min_ == other.initial_delay_min_) && (initial_delay_max_ == other.initial_delay_max_) &&
             (initial_repetitions_base_delay_ == other.initial_repetitions_base_delay_) &&
             (Compare(eventgroups_, other.eventgroups_));
    }

    /*!
     * \brief The time to live (in seconds) in the FindService entry.
     */
    someip_protocol::internal::Ttl ttl_{};

    /*!
     * \brief The maximum number of repetitions in the repetition phase.
     */
    InitialRepetitionsMax initial_repetitions_max_{};

    /*!
     * \brief The minimum delay in the FindService entry.
     */
    std::chrono::nanoseconds initial_delay_min_{};

    /*!
     * \brief The maximum delay in the FindService entry.
     */
    std::chrono::nanoseconds initial_delay_max_{};

    /*!
     * \brief The base delay in the FindService entry during repetition phase.
     */
    std::chrono::nanoseconds initial_repetitions_base_delay_{};

    /*!
     * \brief The service discovery event group parameters.
     */
    RequiredServiceInstanceSdEventgroupContainer eventgroups_{};
  };

  /*!
   * \brief SD eventgroup parameters for a provided service instance.
   */
  struct ProvidedServiceInstanceSdEventgroup {
    /*!
     * \brief The event group identifier.
     */
    someip_protocol::internal::EventgroupId id_{};

    /*!
     * \brief The number of multicast subscription events sent out.
     */
    EventMulticastThreshold event_multicast_threshold_{};

    /*!
     * \brief The minimum delay of a SubscribeEventGroupAck answer to a received SubscribeEventGroup trigger by a
     * multicast ServiceOffer.
     */
    std::chrono::nanoseconds request_response_delay_min_{};

    /*!
     * \brief The maximum delay of a SubscribeEventGroupAck answer to a received SubscribeEventGroup trigger by a
     * multicast ServiceOffer.
     */
    std::chrono::nanoseconds request_response_delay_max_{};
  };

  /*!
   * \brief Container of SD eventgroup parameters for a provided service.
   */
  using ProvidedServiceInstanceSdEventgroupContainer = ara::core::Vector<ProvidedServiceInstanceSdEventgroup>;

  /*!
   * \brief SD parameters for a provided service.
   */
  struct ProvidedServiceInstanceServiceDiscovery {
    /*!
     * \brief The time to live (in seconds) in the OfferService entry.
     */
    someip_protocol::internal::Ttl ttl_{}; /*! TTL in OfferService entry in seconds */

    /*!
     * \brief The maximum number of repetitions during repetition phase.
     */
    InitialRepetitionsMax initial_repetitions_max_{};

    /*!
     * \brief The minimum delay in the FindService entry.
     */
    std::chrono::nanoseconds initial_delay_min_{};

    /*!
     * \brief The maximum delay in the initial FindService entry.
     */
    std::chrono::nanoseconds initial_delay_max_{};

    /*!
     * \brief The base delay in the FindService during repetition phase.
     */
    std::chrono::nanoseconds initial_repetitions_base_delay_{};

    /*!
     * \brief The delay between consecutive OfferService entries during the main phase.
     */
    std::chrono::nanoseconds cyclic_offer_delay_{};

    /*!
     * \brief The minimum delay of a response to a multicast request.
     */
    std::chrono::nanoseconds request_response_delay_min_{};

    /*!
     * \brief The maximum delay of a response to a multicast request
     */
    std::chrono::nanoseconds request_response_delay_max_{};

    /*!
     * \brief The service discovery event group parameters.
     */
    ProvidedServiceInstanceSdEventgroupContainer eventgroups_{};
  };

  /*!
   * \brief Contains service discovery parameters for a network endpoint.
   */
  struct NetworkEndpointServiceDiscovery {
    /*!
     * \brief The normalized multicast IP address for multicast service discovery messages.
     */
    someip_daemon_core::IpAddress multicast_address_{};

    /*!
     * \brief The UDP multicast port.
     */
    someip_protocol::internal::Port port_{};
  };

  /*!
   * \brief Indicates if an endpoint is tagged to use communication or service discovery only. kCommunication indicates
   *        that an endpoint should be created.
   */
  enum class ComRequestedType : std::uint8_t {
    kNotRequested,  /*! Communication is not requested for this endpoint. */
    kCommunication, /*! Communication is requested for this endpoint. */
    kSdOnly         /*! Only service discovery requested for this endpoint. */
  };

  /*!
   * \brief Represents a network endpoint port.
   */
  struct NetworkEndpointPort {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compares two NetworkEndpointPort objects.
     * \param[in] other The instance to compare with.
     * \return true if all parameters are the same, false if one of them differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    inline bool IsEqual(NetworkEndpointPort const& other) const noexcept {
      return (port_ == other.port_) && (proto_ == other.proto_) && (options_.qos_ == other.options_.qos_) &&
             (options_.keep_alive_ == other.options_.keep_alive_) &&
             (options_.enable_so_linger_ == other.options_.enable_so_linger_) &&
             (options_.enable_tcp_nagle_ == other.options_.enable_tcp_nagle_) &&
             (options_.bulk_read_receive_period_us_ == other.options_.bulk_read_receive_period_us_) &&
             (options_.no_of_invalid_tcp_messages_before_disconnection_ ==
              other.options_.no_of_invalid_tcp_messages_before_disconnection_) &&
             (options_.no_of_consecutive_valid_tcp_messages_before_error_reset_ ==
              other.options_.no_of_consecutive_valid_tcp_messages_before_error_reset_);
    }

    // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
    // VECTOR NC Metric-HIS.PATH: MD_SomeIpDaemon_Metric-HIS.PATH
    // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Appends the values of the members of this object to a LogStream.
     * \param[in] s The reference to the LogStream.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    void AppendToLogstream(ara::log::LogStream& s) const noexcept {
      s << "Port number: ";
      s << port_;
      s << "\nProtocol: ";
      s << GetProtocolAsString(proto_);
      if (options_.qos_.has_value()) {
        s << "\nQuality of Service (QoS, vlanPriority) value: ";
        s << options_.qos_.value();
      } else {
        s << "\nQuality of Service (QoS, vlanPriority) not configured";
      }
      if (options_.keep_alive_.has_value()) {
        amsr::someip_protocol::internal::KeepAliveParameters keep_alive_params{options_.keep_alive_.value()};
        if (keep_alive_params.keep_alive_interval_.has_value()) {
          s << "\nTcp keep alive interval (only for TCP ports): ";
          s << static_cast<std::int64_t>(keep_alive_params.keep_alive_interval_.value().count());
        } else {
          s << "\nTcp keep alive interval not configured.";
        }
        if (keep_alive_params.keep_alive_retry_count_.has_value()) {
          s << "\nTcp keep alive retry count (only for TCP ports): ";
          s << keep_alive_params.keep_alive_retry_count_.value();
        } else {
          s << "\nTcp keep alive retry count not configured.";
        }
        if (keep_alive_params.keep_alive_time_.has_value()) {
          s << "\nTcp keep alive time (only for TCP ports): ";
          s << static_cast<std::int64_t>(keep_alive_params.keep_alive_time_.value().count());
        } else {
          s << "\nTcp keep alive time not configured.";
        }
      } else {
        s << "\nKeep alive options not configured.";
      }
      if (options_.enable_so_linger_.has_value()) {
        s << "\nSocket linger (only for TCP ports) value: ";
        s << options_.enable_so_linger_.value();
      } else {
        s << "\nSocket linger option not configured";
      }
      if (options_.enable_tcp_nagle_.has_value()) {
        s << "\nNagles Algorithm (only for TCP ports) value: ";
        s << options_.enable_tcp_nagle_.value();
      } else {
        s << "\nNagles algorithm option not configured.";
      }
      if (options_.bulk_read_receive_period_us_.has_value()) {
        s << "\nBulk read receive period [us] (only for UDP ports): ";
        s << options_.bulk_read_receive_period_us_.value();
      } else {
        s << "\nBulk read receive period [us] has not been configured.";
      }
      if (options_.no_of_invalid_tcp_messages_before_disconnection_ != 0U) {
        s << "\nInvalid messages before disconnect (only for TCP ports) value: ";
        s << options_.no_of_invalid_tcp_messages_before_disconnection_;
      } else {
        s << "\nInvalid messages before disconnect has not been configured.";
      }
      if (options_.no_of_consecutive_valid_tcp_messages_before_error_reset_ != 0U) {
        s << "\nConsecutive valid messages before error reset (only for TCP ports) value: ";
        s << options_.no_of_consecutive_valid_tcp_messages_before_error_reset_;
      } else {
        s << "\nConsecutive valid messages before error reset has not been configured.";
      }
      if (secure_connection_ids_.has_value()) {
        s << "\nSecure IDs: ";
        ConfigurationTypesAndDefs::SecureIdContainer::const_iterator const secure_ids_begin{
            secure_connection_ids_.value().begin()};
        ConfigurationTypesAndDefs::SecureIdContainer::const_iterator const secure_ids_end{
            secure_connection_ids_.value().end()};
        for (ConfigurationTypesAndDefs::SecureIdContainer::const_iterator secure_ids_iter{secure_ids_begin};
             secure_ids_iter != secure_ids_end; ++secure_ids_iter) {
          s << *secure_ids_iter;
          if (secure_ids_iter != secure_ids_end) {
            s << ", ";
          }
        }
      }
    }

    /*!
     * \brief The port number.
     */
    someip_protocol::internal::Port port_{}; /*! Port number */

    /*!
     * \brief The L4 protocol.
     */
    Protocol proto_{};

    /*!
     * \brief The quality of service options, nagle algorithm configuration for sockets etc.
     */
    someip_protocol::internal::SocketOptions options_{};

    /*!
     * \brief Indicates if this endpoint is tagged to use communication or service discovery only.
     */
    ComRequestedType com_requested_{ComRequestedType::kNotRequested};
    /*!
     * \brief The UDP message accumulation threshold.
     */
    std::size_t udp_message_accumulation_threshold_{};

    /*!
     * \brief The secure connection ids linking to secure connections. This is used only on provided side.
     */
    ara::core::Optional<SecureIdContainer> secure_connection_ids_{ara::core::nullopt};
  };

  /*!
   * \brief Container of network endpoint ports.
   */
  using NetworkEndpointPortContainer = ara::core::Vector<NetworkEndpointPort>;

  /*!
   * \brief Represents a network endpoint.
   */
  struct NetworkEndpoint {
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compares two NetworkEndpoint objects.
     * \param[in] other The instance to compare with.
     * \return true if all parameters except ports and socket options for a port are the same, false if one of them
     * differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    inline bool IsEqualExceptPorts(NetworkEndpoint const& other) const noexcept {
      // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
      return (address_ == other.address_) && (mtu_ == other.mtu_) && (mss_ == other.mss_) &&
             (service_discovery_.multicast_address_ == other.service_discovery_.multicast_address_) &&
             (service_discovery_.port_ == other.service_discovery_.port_) &&
             (bulk_read_count_ == other.bulk_read_count_) &&
             (bulk_read_max_no_of_consecutive_receive_calls == other.bulk_read_max_no_of_consecutive_receive_calls) &&
             (bulk_read_min_no_of_datagrams_to_continue_consecutive_receive_calls ==
              other.bulk_read_min_no_of_datagrams_to_continue_consecutive_receive_calls);
    }

    // VECTOR NC Metric-HIS.PATH: MD_SomeIpDaemon_Metric-HIS.PATH
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Appends the values of the members of this object to a LogStream.
     * \param[in] s The reference to the LogStream.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    void AppendToLogstream(ara::log::LogStream& s) const noexcept {
      s << "Ip address: ";
      s << address_.ToString();
      s << "\nMtu: ";
      s << mtu_;
      s << "\nService discovery multicast address: ";
      s << service_discovery_.multicast_address_.ToString();
      s << "\nService discovery port: ";
      s << service_discovery_.port_;
      s << "\nBulk read count: ";
      s << bulk_read_count_;
      if (bulk_read_receive_period_us_.has_value()) {
        s << "\nBulk read receive period [us]: ";
        s << bulk_read_receive_period_us_.value();
      } else {
        s << "\nBulk read receive period [us] has not been configured for this network endpoint";
      }
      if (bulk_read_max_no_of_consecutive_receive_calls.has_value()) {
        s << "\nBulk read maximum number of consecutive receive calls: ";
        s << bulk_read_max_no_of_consecutive_receive_calls.value();
      } else {
        s << "\nBulk read maximum number of consecutive receive calls has not been configured for this network "
             "endpoint";
      }
      if (bulk_read_min_no_of_datagrams_to_continue_consecutive_receive_calls.has_value()) {
        s << "\nBulk read minimum number of datagrams to continue consecutive receive calls: ";
        s << bulk_read_min_no_of_datagrams_to_continue_consecutive_receive_calls.value();
      } else {
        s << "\nBulk read minimum number of datagrams to continue consecutive receive calls has not been configured "
             "for this network endpoint";
      }
      s << "\nTcp nagle enabled: ";
      s << tcp_nagle_enabled_;
      if (tcp_keep_alive_.has_value()) {
        amsr::someip_protocol::internal::KeepAliveParameters keep_alive_params{tcp_keep_alive_.value()};
        if (keep_alive_params.keep_alive_interval_.has_value()) {
          s << "\nTcp keep alive interval: ";
          s << static_cast<std::int64_t>(keep_alive_params.keep_alive_interval_.value().count());
        } else {
          s << "\nTcp keep alive interval not configured for this network endpoint";
        }
        if (keep_alive_params.keep_alive_retry_count_.has_value()) {
          s << "\nTcp keep alive retry count: ";
          s << keep_alive_params.keep_alive_retry_count_.value();
        } else {
          s << "\nTcp keep alive retry count not configured for this network endpoint";
        }
        if (keep_alive_params.keep_alive_time_.has_value()) {
          s << "\nTcp keep alive time: ";
          s << static_cast<std::int64_t>(keep_alive_params.keep_alive_time_.value().count());
        } else {
          s << "\nTcp keep alive time not configured for this network endpoint";
        }
      } else {
        s << "\nTcp keep alive options not configured for this network endpoint";
      }
      if (qos_.has_value()) {
        s << "\nQuality of Service (Qos, vlanPriority) value: ";
        s << qos_.value();
      } else {
        s << "\nQuality of Service (Qos, vlanPriority) not configured for this network endpoint";
      }
      s << "\nNumber of invalid Tcp messages before disconnection threshold value: ";
      s << no_of_invalid_tcp_messages_before_disconnection_;
      s << "\nNumber of consecutive valid Tcp messages before error reset value: ";
      s << no_of_consecutive_valid_tcp_messages_before_error_reset_;
      s << "\n";
    }

    /*!
     * \brief The normalized IP address.
     */
    someip_daemon_core::IpAddress address_{};

    /*!
     * \brief The Maximum Transmission Unit.
     */
    someip_protocol::internal::MTU mtu_{};

    /*!
     * \brief The Maximum Segment Size = mtu - (ip + udp header overhead)
     */
    someip_protocol::internal::MTU mss_{};

    /*!
     * \brief The service discovery parameters.
     */
    NetworkEndpointServiceDiscovery service_discovery_{};

    /*!
     * \brief The ports.
     */
    NetworkEndpointPortContainer ports_{};

    /*!
     * \brief Number of datagrams to request in a single attempt.
     */
    std::uint32_t bulk_read_count_{1U};

    /*!
     * \brief The receive period for bulk reads in micro seconds.
     */
    someip_protocol::internal::BulkReadReceivePeriodUs bulk_read_receive_period_us_{ara::core::nullopt};

    /*!
     * \brief Tcp Nagle enable setting.
     */
    someip_protocol::internal::SocketOptionTcpNaglesAlgorithm tcp_nagle_enabled_{true};

    /*!
     * \brief Tcp Keep Alive setting.
     */
    someip_protocol::internal::TcpKeepAliveOption tcp_keep_alive_{ara::core::nullopt};

    /*!
     * \brief Quality of Service (Qos, vlanPriority) setting
     */
    someip_protocol::internal::QoSOption qos_{ara::core::nullopt};

    /*!
     * \brief Threshold for invalid messages received before disconnecting the TCP connection.
     */
    someip_protocol::internal::TcpMessageCounterThresholdType no_of_invalid_tcp_messages_before_disconnection_{0U};

    /*!
     * \brief Number of consecutive valid messages that must be received to reset the invalid message counter.
     */
    someip_protocol::internal::TcpMessageCounterThresholdType no_of_consecutive_valid_tcp_messages_before_error_reset_{
        0U};

    /*!
     * \brief The maximum number of consecutive receive calls within one polling iteration.
     */
    someip_protocol::internal::BulkReadMaxNoOfConsecutiveReceiveCallsOptional
        bulk_read_max_no_of_consecutive_receive_calls{ara::core::nullopt};

    /*!
     * \brief The minimum number of datagrams to continue consecutive receive calls within one polling
     * iteration.
     */
    someip_protocol::internal::BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsOptional
        bulk_read_min_no_of_datagrams_to_continue_consecutive_receive_calls{ara::core::nullopt};
  };

  using QoSPriority = someip_protocol::internal::QoSPriority;

  /*!
   * \brief Container of network endpoints.
   */
  using NetworkEndpointContainer = ara::core::Vector<NetworkEndpoint>;

  /*!
   * \brief Type definition for the crypto provider UUID.
   */
  using CryptoProviderUuid = ara::crypto::Uuid;

  /*!
   * \brief Type definition for the key slot UUID.
   */
  using KeySlotUuid = ara::crypto::Uuid;

  /*!
   * \brief Type definition for the cipher suite identifier.
   */
  using CipherSuiteId = amsr::tls::CipherSuiteId;

  /*!
   * \brief Type definition for the PSK identity string.
   */
  using PskIdentity = tls::config::PskConfig::PskId;

  /*!
   * \brief Type definition for the PSK identity hint string.
   */
  using PskIdentityHint = tls::config::PskConfig::PskHint;

  /*!
   * \brief Type definition for the PSK UUID.
   */
  using PskUuid = ara::crypto::Uuid;

  /*!
   * \brief Type definition for the private key UUID.
   */
  using PrivateKeyUuid = ara::crypto::Uuid;

  /*!
   * \brief Type definition for the TLS version string.
   */
  using TlsVersion = ara::core::String;

  /*!
   * \brief Type definition for the priority of a secure connection.
   */
  using SecureConnectionPriority = std::uint32_t;

  /*!
   * \brief Container type for collections of cipher suite ids.
   */
  using CipherSuiteIdContainer = amsr::tls::ListOfCipherSuites;

  /*!
   * \brief PSK identity map element.
   */
  struct PskIdentityMapElement {
    /*!
     * \brief The psk identity hint used on client-side. If not set, the element refers to the server-side. If set, the
     * element refers to the client-side.
     */
    ara::core::Optional<PskIdentityHint> psk_identity_hint{};

    /*!
     * \brief The psk identity
     */
    PskIdentity psk_identity{};

    /*!
     * \brief The psk uuid.
     */
    PskUuid psk_uuid{};
  };

  /*!
   * \brief PSK identity map elements container
   */
  using PskIdentityMapElementContainer = ara::core::Vector<PskIdentityMapElement>;

  /*!
   * \brief The certificate label type.
   */
  using CertLabel = ara::core::String;

  /*!
   * \brief The certificate label container.
   */
  using CertChainLabelContainer = ara::core::Vector<CertLabel>;

  /*!
   * \brief The psk configuration.
   */
  struct PskConfig {
    /*!
     * \brief The psk identity hint used on server side. If not set, the config refers to a client-side config. If set,
     * the config refers to a server-side config.
     */
    ara::core::Optional<PskIdentityHint> psk_identity_hint{};

    /*!
     * \brief A list of all configured psk config elements.
     */
    PskIdentityMapElementContainer psk_identity_map{};
  };

  /*!
   * \brief The certification configuration element.
   */
  struct CertConfig {
    /*!
     * \brief The private key uuid.
     */
    PrivateKeyUuid private_key_uuid{};

    /*!
     * \brief A list of the certificate chain.
     */
    CertChainLabelContainer certificate_chain_label_list{};
  };

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief The secure connection configuration element.
   */
  struct SecureConnection {
    /*!
     * \brief Appends the values of the members of this object to a LogStream.
     * \param[in] s The reference to the LogStream.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    void AppendToLogstream(ara::log::LogStream& s) const noexcept {
      s << "TLS version: " << tls_version << "\n";
      s << "Cipher suite ID: " << cipher_suite_id << "\n";
      s << "Priority: " << priority << "\n";

      if (psk_config.has_value()) {
        if (psk_config.value().psk_identity_hint.has_value()) {
          s << "Server PSK identity hint: " << psk_config.value().psk_identity_hint.value() << "\n";
        }
        s << "PSK identity map entries: \n";
        for (PskIdentityMapElement map_element : psk_config.value().psk_identity_map) {
          s << "{";
          if (map_element.psk_identity_hint.has_value()) {
            s << "PSK identity hint: " << map_element.psk_identity_hint.value() << ", ";
          }
          ara::core::String const psk_uuid{map_element.psk_uuid.ToString()};
          s << "PSK identity: " << map_element.psk_identity << ", "
            << "PSK UUID: " << psk_uuid << "}\n";
        }
      }

      if (dtls_cookie_verification_enabled.has_value()) {
        s << "DTLS cookie verification enabled: " << dtls_cookie_verification_enabled.value() << "\n";
      }
    }

    /*!
     * \brief The secure connection identified. It is used as a link to machine mappings.
     */
    SecureId secure_id{};

    /*!
     * \brief The TLS version.
     */
    TlsVersion tls_version{};

    /*!
     * \brief The cipher suite identifier.
     */
    CipherSuiteId cipher_suite_id{};

    /*!
     * \brief The secure connection priority.
     */
    SecureConnectionPriority priority{};

    /*!
     * \brief A client-side or server-side psk configuration. If psk_config has a value, cert_config must be empty.
     */
    ara::core::Optional<PskConfig> psk_config{};

    /*!
     * \brief A certificate configuration. If cert_config has a value, psk_config must be empty.
     */
    ara::core::Optional<CertConfig> cert_config{};

    /*!
     * \brief The flag to indicate if the cookie verification is enabled in DTLS mode.
     * \details This flag is only valid for udp endpoints. Hence it's optional.
     */
    ara::core::Optional<bool> dtls_cookie_verification_enabled{};
  };

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief Mapping between a secure channel and a network port.
   */
  class SecureEndpoint {
   public:
    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief Compares two SecureEndpoint objects.
     *
     * \param[in] other The instance to compare with.
     * \return true if all parameters are the same; false if one of them differ.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    bool operator==(SecureEndpoint const& other) const noexcept {
      // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
      return (port == other.port) && (transport_proto == other.transport_proto) && (address == other.address) &&
             (is_server == other.is_server) && (cipher_suite_ids == other.cipher_suite_ids) &&
             (psk_uuid == other.psk_uuid) && (psk_identity == other.psk_identity) &&
             (psk_identity_hint == other.psk_identity_hint) &&
             (dtls_cookie_verification_enabled == other.dtls_cookie_verification_enabled);
    }

    /*!
     * \brief The normalized IP address.
     */
    someip_daemon_core::IpAddress address{};

    /*!
     * \brief The network port.
     */
    someip_protocol::internal::Port port{};

    /*!
     * \brief The underlying transport protocol.
     */
    Protocol transport_proto{};

    /*!
     * \brief Is this a server?
     */
    bool is_server{};

    /*!
     * \brief The allowed cipher suites.
     */
    CipherSuiteIdContainer cipher_suite_ids{};

    /*!
     * \brief The PSK UUID.
     */
    PskUuid psk_uuid{};

    /*!
     * \brief The PSK identity.
     */
    PskIdentity psk_identity{};

    /*!
     * \brief The PSK identity hint corresponding to the PSK identity (client only).
     */
    PskIdentityHint psk_identity_hint{};

    /*!
     * \brief The flag to indicate if the cookie verification is enabled in DTLS mode.
     * \details This flag is only valid for udp endpoints. Hence it's optional.
     */
    ara::core::Optional<bool> dtls_cookie_verification_enabled{};
  };

  /*!
   * \brief Container of secure endpoint definitions.
   */
  using SecureEndpointContainer = ara::core::Vector<SecureEndpoint>;

  /*!
   * \brief Container of secure connection definitions.
   */
  using SecureConnectionContainer = ara::core::Vector<SecureConnection>;

  /*!
   * \brief Secure com properties used for JSON parsing.
   */
  struct SecureCom {
    /*!
     * \brief The machine's PSK identity hint.
     */
    PskIdentityHint psk_identity_hint{};

    /*!
     * \brief The secure endpoints.
     */
    SecureEndpointContainer secure_endpoints{};

    /*!
     * \brief The secure connections.
     */
    ara::core::Optional<SecureConnectionContainer> secure_connections{};
  };

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief The SecCom configuration.
   */
  class SecComConfig final {
   public:
    /*!
     * \brief Add the secure endpoint configuration of an application to the global configuration.
     * \details This merging sequence will add secure endpoints which are not part of the global configuration yet.
     * If a secure endpoint matches an already existing ones, it will not be added to the global configuration. If
     * there is an inconsistency, the merge process will be aborted.
     * \param[in] parsed_secure_endpoints Parsed secure endpoints configuration to be added to the global config.
     * \return True if the merge process was successful, otherwise false.
     * \pre -
     * \context Init
     * \reentrant FALSE
     * \threadsafe FALSE
     * \steady FALSE
     */
    bool AddSecureEndpoints(SecureEndpointContainer const& parsed_secure_endpoints) noexcept;

    /*!
     * \brief     Indicates whether a connection should be secured given the address, port, and protocol of an endpoint.
     *
     * \note      This function will automatically select between the "old" and "new" approach to secure connections as
     *            appropriate.
     *
     * \param[in] address  A local endpoint address.
     * \param[in] port     A local endpoint port.
     * \param[in] protocol The used communication protocol.
     * \return
     *
     * \pre       -
     * \context   ANY
     * \reentrant FALSE
     * \steady TRUE
     */
    auto ShouldUseSecureCommunication(someip_protocol::internal::IpAddress const& address,
                                      someip_protocol::internal::Port port, Protocol protocol) const noexcept -> bool;

    /*!
     * \brief Determine if an endpoint is secure or not.
     * \param[in] address The endpoint's address.
     * \param[in] port The endpoint's port.
     * \param[in] protocol The endpoint's protocol.
     * \return True if the endpoint is secure, false otherwise.
     * \context ANY
     * \pre -
     * \threadsafe TRUE
     * \reentrant TRUE
     * \steady TRUE
     */
    bool IsSecureEndpoint(someip_protocol::internal::IpAddress const& address, someip_protocol::internal::Port port,
                          Protocol protocol) const noexcept;

    /*!
     * \brief Return the secure communication configuration needed by a service instance.
     * \param[in] opt_secure_ids The IDs of the secure connections that are referenced by the service instance.
     * \return A secure communication configuration with: only the referenced secure connections if the new
     *         configuration is used, all the available secure endpoints otherwise.
     * \context Init
     * \pre -
     * \threadsafe TRUE
     * \reentrant TRUE
     * \steady FALSE
     */
    SecComConfig CreateSecComConfig(ara::core::Optional<SecureIdContainer> const& opt_secure_ids) const noexcept;

    /*!
     * \brief Return whether the secure communication configuration has been set or not.
     * \return True if the secure communication configuration has been set, false otherwise.
     * \context Any
     * \pre -
     * \threadsafe TRUE
     * \reentrant TRUE
     * \steady FALSE
     */
    auto IsEmpty() const noexcept -> bool;

    /*!
     * \brief The secure endpoints.
     */
    SecureEndpointContainer secure_endpoints{};

    /*!
     * \brief TLS secure connections.
     */
    SecureConnectionContainer secure_connections{};
  };

  /*!
   * \brief SOME/IP daemon address domain.
   */
  using IpcDomain = amsr::ipc::Domain;

  /*!
   * \brief SOME/IP daemon address port.
   */
  using IpcPort = amsr::ipc::Port;

  /*!
   * \brief Default IPC domain used for communication with SOME/IP daemon.
   */
  static constexpr IpcDomain kDefaultSomeIpdIpcDomain{42U};

  /*!
   * \brief System config path to IAM json file.
   */
  using SystemConfigPath = ara::core::String;

  /*!
   * \brief Application container.
   */
  using ApplicationContainer = ara::core::Vector<ara::core::String>;

  /*!
   * \brief Parameters for generator version information.
   */
  struct GeneratorVersion {
    /*!
     * \brief Name of the generator.
     */
    ara::core::String name{};

    /*!
     * \brief Generated commit Id.
     */
    ara::core::String commit_id{};
  };

  /*!
   * \brief Represents a SOME/IP daemon posix info struct.
   */
  struct SomeIpdPosixJson {
    /*!
     * \brief Applications belonging to someipd posix json info
     */
    ApplicationContainer applications_{};

    /*!
     * \brief TrustZone crypto provider UUID
     */
    CryptoProviderUuid trustzone_crypto_provider_uuid{};

    /*!
     * \brief Flag for enabling the TLS crypto mode remote behavior.
     */
    bool tls_crypto_mode_remote_{};

    /*!
     * \brief Flag for merging different required service instances that differ in minor version together or not.
     */
    bool merge_rsi_with_different_minor_versions{};
  };

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief Represents an IPC channel.
   */
  class IpcChannel final {
   public:
    /*!
     * \brief Represents a provided SOME/IP service instance.
     */
    struct ProvidedServiceInstance {
      // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
      /*!
       * \brief Compare two objects of type Event based on their ID and their transport protocol in use.
       *
       * \param[in] other The event to compare with.
       * \return true if event ID, is_field option, proto and someip_tp parameters are the same; false if one of them
       * differ.
       * \pre -
       * \context ANY
       * \reentrant FALSE
       */
      bool operator==(ProvidedServiceInstance const& other) const noexcept {
        return (service_id_ == other.service_id_) && (instance_id_ == other.instance_id_) &&
               (major_version_ == other.major_version_) && (minor_version_ == other.minor_version_);
      }
      /*!
       * \brief The service identifier.
       */
      someip_protocol::internal::ServiceId service_id_{};

      /*!
       * \brief The service instance identifier.
       */
      someip_protocol::internal::InstanceId instance_id_{};

      /*!
       * \brief The service instance major version.
       */
      someip_protocol::internal::MajorVersion major_version_{};

      /*!
       * \brief The service instance minor version.
       */
      someip_protocol::internal::MinorVersion minor_version_{};

      /*!
       * \brief The network endpoint port mappings.
       */
      MachineMappingContainer port_mappings_{};

      /*!
       * \brief The service discovery parameters.
       */
      ProvidedServiceInstanceServiceDiscovery service_discovery_{};

      /*!
       * \brief The service discovery parameters.
       */
      SomeipMethodEventPropsContainer event_props_{};

      /*!
       * \brief The service discovery parameters.
       */
      SomeipMethodEventPropsContainer method_props_{};
    };

    /*!
     * \brief Container of provided service instances.
     */
    using ProvidedServiceInstanceContainer = ara::core::Vector<ProvidedServiceInstance>;

    /*!
     * \brief Represents a required SOME/IP service instance.
     */
    struct RequiredServiceInstance {
      // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
      /*!
       * \brief Compare two objects of type Event based on their ID and their transport protocol in use.
       *
       * \param[in] other The event to compare with.
       * \return true if event ID, is_field option, proto and someip_tp parameters are the same; false if one of them
       * differ.
       * \pre -
       * \context ANY
       * \reentrant FALSE
       * \steady FALSE
       */
      bool operator==(RequiredServiceInstance const& other) const noexcept {
        return (service_id_ == other.service_id_) && (instance_id_ == other.instance_id_) &&
               (major_version_ == other.major_version_) && (minor_version_ == other.minor_version_) &&
               (version_driven_find_behavior_ == other.version_driven_find_behavior_);
      }
      /*!
       * \brief The service identifier.
       */
      someip_protocol::internal::ServiceId service_id_{};

      /*!
       * \brief The service instance identifier.
       */
      someip_protocol::internal::InstanceId instance_id_{};

      /*!
       * \brief The service instance major version.
       */
      someip_protocol::internal::MajorVersion major_version_{};

      /*!
       * \brief The service instance minor version.
       */
      someip_protocol::internal::MinorVersion minor_version_{};

      /*!
       * \brief The version driven find behavior.
       */
      VersionDrivenFindBehavior version_driven_find_behavior_{};

      /*!
       * \brief Network endpoint port mapping.
       */
      MachineMapping port_mapping_{};

      /*!
       * \brief Service discovery parameters.
       */
      RequiredServiceInstanceServiceDiscovery service_discovery_{};

      /*!
       * \brief Required signal serialized PDUs.
       */
      SignalSerializedPduContainer required_pdus_{};

      /*!
       * \brief The method event props.
       */
      SomeipMethodEventPropsContainer method_props_{};
    };

    /*!
     * \brief Container of required service instances.
     */
    using RequiredServiceInstanceContainer = ara::core::Vector<RequiredServiceInstance>;

    /*!
     * \brief Comparing two ipc channels by their domain and port.
     *
     * \param[in] rhs The ipc channel to compare with this instance.
     * \return true if domain and port of other ipc channel match; false if not.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    auto operator==(IpcChannel const& rhs) const noexcept -> bool {
      return (domain == rhs.domain) && (port == rhs.port);
    }

    /*!
     * \brief The ipc channel domain.
     */
    IpcDomain domain{kDefaultSomeIpdIpcDomain};

    /*!
     * \brief The ipc channel port.
     */
    IpcPort port{};

    /*!
     * \brief The IPC channel's required service instances.
     */
    RequiredServiceInstanceContainer required_service_instances{};

    /*!
     * \brief The IPC channel's provided service instances.
     */
    ProvidedServiceInstanceContainer provided_service_instances{};

    /*!
     * \brief Add a required service instance to the IPC channel.
     *
     * \param[in] required_service_instance The required service instances to add.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     */
    void AddRequiredServiceInstance(RequiredServiceInstance const& required_service_instance) noexcept;

    /*!
     * \brief Add a provided service instance to the IPC channel.
     *
     * \param[in] provided_service_instance The provided service instances to add.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     */
    void AddProvidedServiceInstance(ProvidedServiceInstance const& provided_service_instance) noexcept;

    /*!
     * \brief Returns a container of all required service instances.
     *
     * \return A container of required services.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady TRUE
     */
    auto GetRequiredServiceInstances() const noexcept -> RequiredServiceInstanceContainer const&;

    /*!
     * \brief Returns a container of all provided service instances.
     *
     * \return A container of provided services.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady TRUE
     */
    auto GetProvidedServiceInstances() const noexcept -> ProvidedServiceInstanceContainer const&;

    /*!
     * \brief Searches for a required service instance based on service ID, instance ID, major and minor versions.
     *
     * \param[in] service_id A SOME/IP service identifier.
     * \param[in] instance_id A SOME/IP service instance identifier.
     * \param[in] major_version A major version of a SOME/IP service.
     * \param[in] minor_version A minor version of a SOME/IP service.
     * \return A required service instance or nullptr.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    auto GetRequiredServiceInstance(someip_protocol::internal::ServiceId service_id,
                                    someip_protocol::internal::InstanceId instance_id,
                                    someip_protocol::internal::MajorVersion major_version,
                                    someip_protocol::internal::MinorVersion minor_version) const noexcept
        -> RequiredServiceInstance const*;

    /*!
     * \brief Searches for a provided service instance based on service ID, instance ID, major and minor versions.
     *
     * \param[in] service_id A SOME/IP service identifier.
     * \param[in] instance_id A SOME/IP service instance identifier.
     * \param[in] major_version A major version of a SOME/IP service.
     * \param[in] minor_version A minor version of a SOME/IP service.
     * \return A pointer to a provided service instance if a provided service instance with the given tuple of service
     * ID, instance ID, major and minor version is present; a nullptr otherwise.
     * \pre -
     * \context ANY
     * \reentrant FALSE
     * \steady FALSE
     */
    auto GetProvidedServiceInstance(someip_protocol::internal::ServiceId service_id,
                                    someip_protocol::internal::InstanceId instance_id,
                                    someip_protocol::internal::MajorVersion major_version,
                                    someip_protocol::internal::MinorVersion minor_version) const noexcept
        -> ProvidedServiceInstance const*;

    /*!
     * \brief Searches for a provided service instance matching only the service_id and instance_id.
     * \details This API is used in case no information about Major and Minor versions is available,
     *          as configuring services with same service instance but different major or minor versions
     *          is currently not supported.
     *
     * \param[in] service_id   A SOME/IP service identifier.
     * \param[in] instance_id  A SOME/IP service instance identifier.
     * \return A pointer to a provided service instance if a provided service instance with the given tuple of service
     * ID and instance ID was found, otherwise a nullptr.
     *
     * \pre -
     * \context   App
     * \reentrant FALSE
     * \steady FALSE
     */
    auto GetProvidedServiceInstance(someip_protocol::internal::ServiceId service_id,
                                    someip_protocol::internal::InstanceId instance_id) const noexcept
        -> ProvidedServiceInstance const*;
  };

  /*!
   * \brief IPC channels container.
   */
  using IpcChannelsContainer = ara::core::Vector<IpcChannel>;

  /*!
   * \brief Configuration for a cyclic timer.
   */
  struct CyclicTimerConfig {
    /*!
     * \brief The network endpoint IP address.
     */
    someip_protocol::internal::IpAddress address{};

    /*!
     * \brief The delay between consecutive OfferService entries during the main phase.
     */
    std::chrono::nanoseconds cyclic_offer_delay{};
  };

  /*!
   * \brief CyclicTimerConfig container.
   */
  using CyclicTimerConfigContainer = ara::core::Vector<CyclicTimerConfig>;

  /*!
   * \brief Configuration for a cyclic timer.
   */
  struct RepetitionOfferTimerConfig {
    /*!
     * \brief The network endpoint IP address.
     */
    someip_protocol::internal::IpAddress address{};

    /*!
     * \brief The initial delay between consecutive OfferService entries during the repetition phase.
     */
    std::chrono::nanoseconds initial_repetitions_base_delay{};
  };

  /*!
   * \brief RepetitionOfferTimerConfig container.
   */
  using RepetitionOfferTimerConfigContainer = ara::core::Vector<RepetitionOfferTimerConfig>;

  /*!
   * \brief Type for bulk read count.
   */
  using BulkReadCountType = std::uint32_t;

  /*!
   * \brief Type for bulk read receive period [us].
   */
  using BulkReadReceivePeriodUsType = someip_protocol::internal::BulkReadReceivePeriodUsType;

  /*!
   * \brief Type for bulk read receive period [us].
   */
  using BulkReadReceivePeriodUs = someip_protocol::internal::BulkReadReceivePeriodUs;

  /*!
   * \brief Type for bulk read maximum number of consecutive receive calls.
   */
  using BulkReadMaxNoOfConsecutiveReceiveCalls =
      someip_protocol::internal::BulkReadMaxNoOfConsecutiveReceiveCallsOptional;

  /*!
   * \brief Type for bulk read minimum number of datagrams to continue consecutive receive calls.
   */
  using BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls =
      someip_protocol::internal::BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsOptional;

  /*!
   * \brief Type for TCP Nagle enabled.
   */
  using TcpNagleEnabledType = someip_protocol::internal::SocketOptionTcpNaglesAlgorithm;

  /*!
   * \brief Type for TCP KeepAlive configuration.
   */
  using TcpKeepAliveType = someip_protocol::internal::TcpKeepAliveOption;

  /*!
   * \brief Type for Quality of Service (Qos, vlanPriority) setting.
   */
  using QosType = someip_protocol::internal::QoSOption;

  /*!
   * \brief Max value for the Quality of Service (Qos, vlanPriority) option.
   */
  static constexpr someip_protocol::internal::QoSPriority const kQosMax{7U};

 protected:
  /*!
   * \brief Destructor is declared as protected to prevent any object of this class
   *        from being deleted directly through its base class
   * \see AutosarC++14-A12.4.1
   * \steady FALSE
   */
  ~ConfigurationTypesAndDefs() noexcept = default;
};

/*!
 * \brief Default value to use for bulk read count if it is not in the configuration.
 */
static constexpr ConfigurationTypesAndDefs::BulkReadCountType const kBulkReadCountDefault{1U};

/*!
 * \brief Default value to use for bulk read receive period if it is not in the configuration.
 */
static constexpr ConfigurationTypesAndDefs::BulkReadReceivePeriodUs const kBulkReadReceivePeriodUsDefault{
    ara::core::nullopt};

/*!
 * \brief Default value to use for bulk read maximum number of consecutive receive calls if it is not in the
 * configuration.
 */
static constexpr ConfigurationTypesAndDefs::BulkReadMaxNoOfConsecutiveReceiveCalls const
    kBulkReadMaxNoOfConsecutiveReceiveCallsDefault{ara::core::nullopt};

/*!
 * \brief Default value to use for bulk read minimum number of datagrams to continue consecutive receive calls if it
 * is not in the configuration.
 */
static constexpr ConfigurationTypesAndDefs::BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls const
    kBulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCallsDefault{ara::core::nullopt};

/*!
 * \brief Default value to use for TcpNagleEnabled if it is not in the configuration.
 */
static constexpr ConfigurationTypesAndDefs::TcpNagleEnabledType const kTcpNagleEnabledDefault{true};

/*!
 * \brief Default value to use for TcpKeepAlive if it is not in the configuration.
 */
static constexpr ConfigurationTypesAndDefs::TcpKeepAliveType const kTcpKeepAliveDefault{ara::core::nullopt};

/*!
 * \brief Default value to use for Quality of Service setting if it is not in the configuration.
 */
static constexpr ConfigurationTypesAndDefs::QosType const kQosDefault{ara::core::nullopt};

/*!
 * \brief Default value to use for number of invalid Tcp messages before disconnection setting if it is not in the
 * configuration.
 */
static constexpr someip_protocol::internal::TcpMessageCounterThresholdType const
    kNoOfInvalidTcpMessagesBeforeDisconnectionDefault{0U};

/*!
 * \brief Default value to use for number of consecutive valid Tcp messages before error reset setting if it is not in
 * the configuration.
 */
static constexpr someip_protocol::internal::TcpMessageCounterThresholdType const
    NoOfConsecutiveValidTcpMessagesBeforeErrorResetDefault{0U};

}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CONFIGURATION_TYPES_H_

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
/*!        \file  event_message_dispatcher.h
 *        \brief  This file contains the EventMessageDispatcher which provides an API to send event messages to
 *                subscribers via TCP or UDP.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MESSAGE_DISPATCHER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MESSAGE_DISPATCHER_H_

#include <memory>
#include <set>
#include <unordered_map>
#include "ara/core/vector.h"

#include "amsr/generic/generic_error_domain.h"
#include "amsr/someip_daemon_core/server/eventgroup/eventgroup_subscriber.h"
#include "ara/core/memory_resource.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {
namespace eventgroup {

/*!
 * \brief The EventMessageDispatcher provides an API to dispatch an event to subscribers via UDP or TCP senders.
 * \tparam TcpSender TCP Sender type
 * \tparam UdpSender UDP Sender type
 */
template <typename TcpSender, typename UdpSender>
class EventMessageDispatcher final {
 private:
  /*!
   * \brief Container of unique IP addresses to which an event has already been sent.
   * \details std::set container is used to avoid sending duplicate events to the same destination.
   */
  using SentToSet =
      std::set<configuration::Configuration::SocketAddress, std::less<configuration::Configuration::SocketAddress>,
               ara::core::PolymorphicAllocator<configuration::Configuration::SocketAddress>>;

 public:
  /*!
   * \brief Alias for EventgroupSubscriber type.
   */
  using EventgroupSubscriberType = eventgroup::EventgroupSubscriber<UdpSender, TcpSender>;
  /*!
   * \brief Type used to reference a map of Senders.
   */
  using SendersMap = std::unordered_map<
      someip_protocol::internal::EventgroupId, ara::core::Vector<EventgroupSubscriberType>,
      std::hash<someip_protocol::internal::EventgroupId>, std::equal_to<someip_protocol::internal::EventgroupId>,
      ara::core::PolymorphicAllocator<
          std::pair<someip_protocol::internal::EventgroupId const, ara::core::Vector<EventgroupSubscriberType>>>>;

  /*!
   * \brief Type used to represent an immutable pointer to UdpSender.
   */
  using UdpSenderImmutablePtr = typename UdpSender::pointer const;

  /*!
   * \brief Alias for a map of provided eventgroups by eventgroup Id.
   */
  using ProvidedEventgroupMap =
      ara::core::Map<someip_protocol::internal::EventgroupId, configuration::types::ProvidedSomeIpEventgroup>;

  /*!
   * \brief Default constructible.
   * \steady FALSE
   */
  EventMessageDispatcher() noexcept = default;
  /*!
   * \brief Default destructible.
   * \steady FALSE
   */
  ~EventMessageDispatcher() noexcept = default;

  EventMessageDispatcher(EventMessageDispatcher const&) noexcept = delete;
  EventMessageDispatcher(EventMessageDispatcher&&) noexcept = delete;
  EventMessageDispatcher& operator=(EventMessageDispatcher const&) noexcept = delete;
  EventMessageDispatcher& operator=(EventMessageDispatcher&&) noexcept = delete;

  /*!
   * \brief Dispatches a SOME/IP message via TCP to subscribers.
   * \param[in] message The SOME/IP message.
   * \param[in] senders A reference to a map with the event subscribers.
   * \param[in] provided_eventgroup_map A provided eventgroup map.
   * \param[in] event A reference to the current event.
   * \param[in] udp_mcast_sender The UDP multicast sender.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the event couldn't be
   *        forwarded.
   *
   * \context Network
   * \reentrant FALSE
   * \steady TRUE
   * \trace SPEC-4981715
   *
   * \internal
   * - For each eventgroup to which the event belongs
   *  - Get the multicast configuration for the event
   *  - If the event exist within the senders.
   *    - If the event must be sent using multicast (it has a multicast configuration, threshold is valid, threshold is
   *      lesser than the number of eventgroup's senders and udp multicast sender is not nullptr):
   *      - If Dispatch the event using the multicast sender returns success,
   *        - Emplace the value to the result.
   *    - Else: send the event using unicast
   *      - If the event is configured to use TCP protocol:
   *        - If Dispatch the event using the TCP subscription returns success,
   *        - Emplace the value to the result.
   *      - Else: the event uses udp protocol
   *        - If Dispatch the event using the UDP subscription (unicast) returns success,
   *        - Emplace the value to the result.
   *  - Else: Emplace the value to the result, if no subscribers are found.
   * \endinternal
   */
  static ara::core::Result<void> DispatchSomeIpMessage(
      std::shared_ptr<someip_protocol::internal::SomeIpMessage> message, SendersMap const& senders,
      ProvidedEventgroupMap const& provided_eventgroup_map, configuration::types::SomeIpEventDeployment const& event,
      UdpSenderImmutablePtr udp_mcast_sender) noexcept {
    ara::core::Result<void> result{ara::core::ErrorCode{amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed}};
    SentToSet sent_to_list{};

    for (std::pair<someip_protocol::internal::EventgroupId,
                   ara::core::Optional<configuration::types::MulticastEventgroupConfig>> const& eg_config :
         EventToEventgroups(provided_eventgroup_map, event.event_id)) {
      // Multicast threshold.
      // Check availability to avoid std::out_of_range exception.
      if (senders.count(eg_config.first) != 0) {
        bool const send_via_multicast{(eg_config.second.has_value()) &&
                                      (eg_config.second->multicast_threshold !=
                                       configuration::Configuration::kEventMulticastThresholdUseOnlyUnicast) &&
                                      (eg_config.second->multicast_threshold <= senders.at(eg_config.first).size()) &&
                                      (udp_mcast_sender != nullptr)};

        if (send_via_multicast) {
          // Send via multicast
          if (udp_mcast_sender->ForwardFromLocal(message)) {
            result.EmplaceValue();
          }
        } else {
          // Send via unicast
          if (event.transport_protocol == configuration::types::TransportProtocol::kTCP) {
            if (DispatchSomeIpMessageWithTcp(message, senders, eg_config.first, sent_to_list)) {
              result.EmplaceValue();
            }
          } else {
            if (DispatchSomeIpMessageWithUdp(message, senders, eg_config.first, sent_to_list)) {
              result.EmplaceValue();
            } else {
              // Added a else block to make Bauhaus happy (M6.4.2)
            }
          }
        }
      } else {
        // If no subscribers are found, still it is a valid usecase.
        // Hence, no error is returned.
        result.EmplaceValue();
      }
    }
    return result;
  }

  /*!
   * \brief Dispatches a PDU message via TCP to subscribers.
   * \param[in] message                  The PDU message.
   * \param[in] senders                  A reference to a map with the event subscribers.
   * \param[in] provided_eventgroup_map  A reference to eventgroup map configuration.
   * \param[in] event                    A reference to the current event.
   * \param[in] udp_mcast_sender         The UDP multicast sender.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the event couldn't be
   *        forwarded.
   *
   * \context Network
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - For each eventgroup to which the event belongs
   *  - Get the multicast configuration for th event
   *  - If the event exist within the senders.
   *    - If the event must be sent using multicast (it has a multicast configuration, threshold is valid, threshold is
   *      lesser than the number of eventgroup's senders and udp multicast sender is not nullptr):
   *      - If Dispatch the event using the multicast sender returns success,
   *        - Emplace the value to the result.
   *    - Else: send the event using unicast
   *      - If the event is configured to use TCP protocol:
   *        - If Dispatch the event using the TCP subscription returns success,
   *        - Emplace the value to the result.
   *      - Else: the event uses udp protocol
   *        - If Dispatch the event using the UDP subscription (unicast) returns success,
   *        - Emplace the value to the result.
   *  - Else: Emplace the value to the result, if no subscribers are found.
   * \endinternal
   */
  static ara::core::Result<void> DispatchPduMessage(std::shared_ptr<someip_protocol::internal::PduMessage> message,
                                                    SendersMap const& senders,
                                                    ProvidedEventgroupMap const& provided_eventgroup_map,
                                                    configuration::types::SomeIpEventDeployment const& event,
                                                    UdpSenderImmutablePtr udp_mcast_sender) noexcept {
    ara::core::Result<void> result{ara::core::ErrorCode{amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed}};
    SentToSet sent_to_list{};

    for (std::pair<someip_protocol::internal::EventgroupId,
                   ara::core::Optional<configuration::types::MulticastEventgroupConfig>> const& eg_config :
         EventToEventgroups(provided_eventgroup_map, event.event_id)) {
      // Multicast threshold.
      // Check availability to avoid std::out_of_range exception.
      if (senders.count(eg_config.first) != 0) {
        bool const send_via_multicast{(eg_config.second.has_value()) &&
                                      (eg_config.second->multicast_threshold !=
                                       configuration::Configuration::kEventMulticastThresholdUseOnlyUnicast) &&
                                      (eg_config.second->multicast_threshold <= senders.at(eg_config.first).size()) &&
                                      (udp_mcast_sender != nullptr)};

        if (send_via_multicast) {
          // Send via multicast
          if (udp_mcast_sender->ForwardPduFromLocal(message)) {
            result.EmplaceValue();
          }
        } else {
          // Send via unicast
          if (event.transport_protocol == configuration::types::TransportProtocol::kTCP) {
            if (DispatchPduMessageWithTcp(message, senders, eg_config.first, sent_to_list)) {
              result.EmplaceValue();
            }
          } else {
            if (DispatchPduMessageWithUdp(message, senders, eg_config.first, sent_to_list)) {
              result.EmplaceValue();
            } else {
              // Added a else block to make Bauhaus happy (M6.4.2)
            }
          }
        }
      } else {
        // If no subscribers are found, still it is a valid usecase.
        // Hence, no error is returned.
        result.EmplaceValue();
      }
    }
    return result;
  }

 private:
  /*!
   * \brief Search for all the eventgroups an event belongs to and return a container with their multicast
   * configuration.
   * \param[in] provided_eventgroup_map A map containing all the eventgroups.
   * \param[in] event                   An event to look for in the eventgroup map.
   *
   * \return A vector of pairs, each containing the multicast configuration of the eventgroups where the event
   * belongs to and its eventgroup id
   *
   * \steady TRUE
   *
   * \internal
   *  - Create a vector of pairs, each containing eventgroup multicast configuration and eventgroup id.
   *  - For every eventgroup:
   *    - If the event exists within the eventgroup:
   *      - Add the eventgroup multicast configuration and eventgroup id to the vector.
   * \endinternal
   */
  static ara::core::Vector<std::pair<someip_protocol::internal::EventgroupId,
                                     ara::core::Optional<configuration::types::MulticastEventgroupConfig>>>
  EventToEventgroups(ProvidedEventgroupMap const& provided_eventgroup_map,
                     someip_protocol::internal::EventId const& event_id) noexcept {
    // PTP-B-Configuration_EventToEventgroups
    configuration::types::ProvidedSomeIpEventMap::const_iterator event_config;
    ProvidedEventgroupMap::const_iterator eg;
    ara::core::Vector<std::pair<someip_protocol::internal::EventgroupId,
                                ara::core::Optional<configuration::types::MulticastEventgroupConfig>>>
        eventgroup_config{};

    for (eg = provided_eventgroup_map.cbegin(); eg != provided_eventgroup_map.cend(); ++eg) {
      event_config = eg->second.events.find(event_id);
      if (event_config != eg->second.events.cend()) {
        eventgroup_config.emplace_back(eg->first, eg->second.multicast_eventgroup_config);
      }
    }

    // PTP-E-Configuration_EventToEventgroups
    return eventgroup_config;
  }

  /*!
   * \brief Dispatches a SOME/IP message via TCP to subscribers.
   * \param[in] message The SOME/IP message.
   * \param[in] senders A reference to a map with the event subscribers.
   * \param[in] egid The eventgroup id of the event to send.
   * \param[in, out] sent_to_list A set containing all the IP addresses to which this event has already
   *                         been sent, to avoid send duplication.
   *
   * \return false if a failure occurred while forwarding to one of the receivers,
   *                   true otherwise.
   *
   * \context Network
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - For each subscriber for the given eventgroup id
   *   - If the subscriber has a registered TCP sender
   *     - If the TCP sender has not been used yet to send this event
   *       - Forward the event
   *       - Add this sender to the list of used addresses
   * \endinternal
   */
  static bool DispatchSomeIpMessageWithTcp(std::shared_ptr<someip_protocol::internal::SomeIpMessage> message,
                                           SendersMap const& senders, someip_protocol::internal::EventgroupId egid,
                                           SentToSet& sent_to_list) noexcept {
    bool result{true};
    // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
    // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
    for (EventgroupSubscriberType const& s : senders.at(egid)) {
      if (s.tcp_sender != nullptr) {
        // Skip sending same event multiple times to the same target
        someip_daemon_core::IpAddress const& address{s.tcp_sender->GetRemoteAddress()};
        amsr::net::ip::Port const port{s.tcp_sender->GetRemotePort()};
        configuration::Configuration::SocketAddress const remote_socket_address{address, port};
        if (sent_to_list.insert(remote_socket_address).second) {
          bool const dispatch_outcome{s.tcp_sender->ForwardFromLocal(message)};
          result = result && dispatch_outcome;
        }
      }
    }

    return result;
  }

  /*!
   * \brief Dispatches a SOME/IP message via UDP to subscribers.
   * \param[in] message The SOME/IP message.
   * \param[in] senders A reference to a map with the event subscribers.
   * \param[in] egid The eventgroup id of the event to send.
   * \param[in, out] sent_to_list A set containing all the IP addresses to which this event has already
   *                         been sent, to avoid send duplication.
   *
   * \return false if a failure occurred while forwarding to one of the receivers,
   *                   true otherwise.
   *
   * \context Network
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - For each subscriber for the given eventgroup id
   *   - If the subscriber has a registered UDP sender
   *     - If the UDP sender has not been used yet to send this event
   *       - Forward the event
   *       - Add this sender to the list of used addresses
   * \endinternal
   */
  static bool DispatchSomeIpMessageWithUdp(std::shared_ptr<someip_protocol::internal::SomeIpMessage> message,
                                           SendersMap const& senders, someip_protocol::internal::EventgroupId egid,
                                           SentToSet& sent_to_list) noexcept {
    bool result{true};
    // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
    // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
    for (EventgroupSubscriberType const& s : senders.at(egid)) {
      if (s.udp_sender != nullptr) {
        // Skip sending same event multiple times to the same target
        someip_daemon_core::IpAddress const& address{s.udp_sender->GetRemoteAddress()};
        amsr::net::ip::Port const port{s.udp_sender->GetRemotePort()};
        configuration::Configuration::SocketAddress const socket_address{address, port};
        if (sent_to_list.insert(socket_address).second) {
          bool const dispatch_outcome{s.udp_sender->ForwardFromLocal(message)};
          result = result && dispatch_outcome;
        }
      }
    }
    return result;
  }

  /*!
   * \brief Dispatches a PDU message via TCP to subscribers.
   * \param[in] message The PDU message.
   * \param[in] senders A reference to a map with the event subscribers.
   * \param[in] egid The eventgroup id of the event to send.
   * \param[in, out] sent_to_list A set containing all the IP addresses to which this event has already
   *                         been sent, to avoid send duplication.
   *
   * \return false if a failure occurred while forwarding to one of the receivers,
   *                   true otherwise.
   *
   * \context Network
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - For each subscriber for the given eventgroup id
   *   - If the subscriber has a registered TCP sender
   *     - If the TCP sender has not been used yet to send this event
   *       - Forward the event
   *       - Add this sender to the list of used addresses
   * \endinternal
   */
  static bool DispatchPduMessageWithTcp(std::shared_ptr<someip_protocol::internal::PduMessage> message,
                                        SendersMap const& senders, someip_protocol::internal::EventgroupId egid,
                                        SentToSet& sent_to_list) noexcept {
    bool result{true};
    // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
    // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
    for (EventgroupSubscriberType const& s : senders.at(egid)) {
      if (s.tcp_sender != nullptr) {
        // Skip sending same event multiple times to the same target
        someip_daemon_core::IpAddress const& address{s.tcp_sender->GetRemoteAddress()};
        amsr::net::ip::Port const port{s.tcp_sender->GetRemotePort()};
        configuration::Configuration::SocketAddress const remote_socket_address{address, port};
        if (sent_to_list.insert(remote_socket_address).second) {
          bool const dispatch_outcome{s.tcp_sender->ForwardPduFromLocal(message)};
          result = result && dispatch_outcome;
        }
      }
    }

    return result;
  }

  /*!
   * \brief Dispatches a PDU message via UDP to subscribers.
   * \param[in] message The PDU message.
   * \param[in] senders A reference to a map with the event subscribers.
   * \param[in] egid The eventgroup id of the event to send.
   * \param[in, out] sent_to_list A set containing all the IP addresses to which this event has already
   *                         been sent, to avoid send duplication.
   *
   * \return false if a failure occurred while forwarding to one of the receivers,
   *                   true otherwise.
   *
   * \context Network
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - For each subscriber for the given eventgroup id
   *   - If the subscriber has a registered UDP sender
   *     - If the TCP sender has not been used yet to send this event
   *       - Forward the event
   *       - Add this sender to the list of used addresses
   * \endinternal
   */
  static bool DispatchPduMessageWithUdp(std::shared_ptr<someip_protocol::internal::PduMessage> message,
                                        SendersMap const& senders, someip_protocol::internal::EventgroupId egid,
                                        SentToSet& sent_to_list) noexcept {
    bool result{true};
    // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
    // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
    for (EventgroupSubscriberType const& s : senders.at(egid)) {
      if (s.udp_sender != nullptr) {
        // Skip sending same event multiple times to the same target
        someip_daemon_core::IpAddress const& address{s.udp_sender->GetRemoteAddress()};
        amsr::net::ip::Port const port{s.udp_sender->GetRemotePort()};
        configuration::Configuration::SocketAddress const socket_address{address, port};
        if (sent_to_list.insert(socket_address).second) {
          bool const dispatch_outcome{s.udp_sender->ForwardPduFromLocal(message)};
          result = result && dispatch_outcome;
        }
      }
    }
    return result;
  }
};

}  // namespace eventgroup
}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENT_MESSAGE_DISPATCHER_H_

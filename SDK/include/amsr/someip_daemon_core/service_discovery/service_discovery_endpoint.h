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
/**        \file  service_discovery_endpoint.h
 *        \brief  SD Network Endpoint.
 *
 *      \details  The network endpoint responsible for sending/receiving service discovery messages (via
 *                unicast/multicast).
 *                Reception: The endpoint receives packets from remote partner, deserializes it and prepares
 *                ServiceDiscoveryMessage object to be sent to the ServiceDiscoveryMessageHandler.
 *                Sending: The endpoint expects ServiceDiscoveryMessage objects from ServiceDiscoveryMessageHandler,
 *                serializes it and sends it to the remote partner.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <utility>

#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/marshalling/deserialization/someip_sd_message_deserializer.h"
#include "amsr/someip_daemon_core/marshalling/ser_sd_message.h"
#include "amsr/someip_daemon_core/message_reader/datagram_message_reader.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_find_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_offer_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_stop_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_ack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_nack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/service_discovery_option.h"
#include "amsr/someip_daemon_core/service_discovery/message/scheduler/scheduler.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_builder.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_header_builder.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_message_processor.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_message_sender.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_socket.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief ServiceDiscoveryEndpoint.
 */
class ServiceDiscoveryEndpoint : public ServiceDiscoveryMessageSenderInterface,
                                 public message::scheduler::SchedulerInterface {
 public:
  /*!
   * \brief Type alias for Identity Access Manager.
   */
  using IdentityAccessManager = iam::IamInterface;
  /*!
   * \brief A type alias for the Reactor1
   */
  using Reactor = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief Default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~ServiceDiscoveryEndpoint() noexcept override = default;

  ServiceDiscoveryEndpoint() = delete;
  ServiceDiscoveryEndpoint(ServiceDiscoveryEndpoint const&) = delete;
  ServiceDiscoveryEndpoint(ServiceDiscoveryEndpoint&&) = delete;
  ServiceDiscoveryEndpoint& operator=(ServiceDiscoveryEndpoint const&) & = delete;
  ServiceDiscoveryEndpoint& operator=(ServiceDiscoveryEndpoint&&) & = delete;

  /*!
   * \brief Constructor of ServiceDiscoveryEndpoint.
   * \param[in,out] reactor                 A reactor for asynchronous event notification on socket handles.
   * \param[in,out] timer_manager           Timer manager.
   * \param[in]     unicast_address         A unicast IP address used for sending and receiving of SD messages.
   * \param[in]     multicast_address       A multicast IP address used for receiving of SD messages.
   * \param[in]     port                    The UDP port used for sending/receiving messages (the same for
   *                                        unicast/multicast).
   * \param[in]     max_datagram_length     The maximum length of a UDP datagram received or sent.
   * \param[in,out] identity_access_manager The Identity Access Manager to check access rights.
   * \param[in]     statistics_handler      The statistics handler
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  ServiceDiscoveryEndpoint(
      Reactor* reactor, amsr::steady_timer::TimerManagerInterface* timer_manager,
      someip_daemon_core::IpAddress const& unicast_address, someip_daemon_core::IpAddress const& multicast_address,
      amsr::net::ip::Port const port, std::size_t const max_datagram_length,
      IdentityAccessManager& identity_access_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler);

  /*!
   * \brief Call to invoke to signal that an enclosed instance of type UDPSocket requires its underlying socket to be
   * created and opened.
   * \pre -
   * \context Init, Network, Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void OnSocketRequired() noexcept;

  /*!
   * \brief Create a new cyclic timer.
   * \param[in] cyclic_offer_delay Cyclic delay of the timer.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void CreateCyclicTimer(std::chrono::nanoseconds const& cyclic_offer_delay) noexcept;

  /*!
   * \brief      Create a new repetition offer timer.
   * \param[in]  initial_repetitions_base_delay initial repetitions base delay of the timer.
   * \pre        -
   * \context    Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   * \vprivate
   */
  void CreateRepetitionOfferTimer(std::chrono::nanoseconds const& initial_repetitions_base_delay) noexcept;

  /*!
   * \brief Schedule a FindService Entry to be sent via multicast.
   * \param[in] entry FindService Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void ScheduleFindServiceEntry(message::entries::FindServiceEntry const& entry,
                                std::chrono::nanoseconds const& min_delay,
                                std::chrono::nanoseconds const& max_delay) noexcept override;

  /*!
   * \brief Schedule a Stop OfferService Entry to be sent via multicast.
   * \param[in] entry StopOffer Entry to be scheduled.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void ScheduleStopOfferServiceEntry(message::entries::StopOfferServiceEntry const& entry) noexcept override;

  /*!
   * \brief Schedule a SubscribeEventgroup Entry to be sent to a specific receiver.
   * \param[in] entry SubscribeEventgroup Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context App, Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady TRUE
   */
  void ScheduleSubscribeEventgroupEntry(
      message::entries::SubscribeEventgroupEntry const& entry, std::chrono::nanoseconds const& min_delay,
      std::chrono::nanoseconds const& max_delay,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) noexcept override;

  /*!
   * \brief Schedule a StopSubscribeEventgroup Entry to be sent to a specific receiver.
   * \param[in] entry StopSubscribeEventgroup Entry to be scheduled.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void ScheduleStopSubscribeEventgroupEntry(
      message::entries::StopSubscribeEventgroupEntry const& entry,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) noexcept override;

  /*!
   * \brief Schedule a SubscribeEventgroupAck Entry to be sent to a specific receiver.
   * \details A post-send action will be executed after the Ack has been sent.
   * \param[in] entry SubscribeEventgroupAck Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \param[in] receiver_address The receiver address.
   * \param[in] post_action An action that will be executed once the entry has been sent.
   * \pre -
   * \context Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void ScheduleSubscribeEventgroupAckEntry(message::entries::SubscribeEventgroupAckEntry const& entry,
                                           std::chrono::nanoseconds const& min_delay,
                                           std::chrono::nanoseconds const& max_delay,
                                           amsr::someip_protocol::internal::AddressPair const& receiver_address,
                                           TimerSendActionType&& post_action) noexcept override;
  /*!
   * \brief Schedule a SubscribeEventgroupAck Entry to be sent to a specific receiver.
   * \param[in] entry SubscribeEventgroupAck Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady TRUE
   */
  void ScheduleSubscribeEventgroupAckEntry(
      message::entries::SubscribeEventgroupAckEntry const& entry, std::chrono::nanoseconds const& min_delay,
      std::chrono::nanoseconds const& max_delay,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) noexcept override;

  /*!
   * \brief Schedule a SubscribeEventgroupNack Entry to be sent to a specific receiver.
   * \param[in] entry SubscribeEventgroupNack Entry to be scheduled.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady TRUE
   */
  void ScheduleSubscribeEventgroupNAckEntry(
      message::entries::SubscribeEventgroupNAckEntry const& entry,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) noexcept override;

  /*!
   * \brief Schedule a unicast OfferService Entry.
   * \param[in] entry OfferService Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \param[in] receiver_address The receiver address.
   * \pre -
   * \context Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void ScheduleOfferServiceEntry(
      message::entries::OfferServiceEntry const& entry, std::chrono::nanoseconds const& min_delay,
      std::chrono::nanoseconds const& max_delay,
      amsr::someip_protocol::internal::AddressPair const& receiver_address) noexcept override;

  /*!
   * \brief Schedule a multicast OfferService Entry.
   * \param[in] entry OfferService Entry to be scheduled.
   * \param[in] min_delay Minimum delay to wait before sending this entry.
   * \param[in] max_delay Maximum delay to wait before sending this entry.
   * \param[in] post_action An action that will be executed once the entry has been sent.
   * \pre -
   * \context Timer, App, Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void ScheduleOfferServiceEntry(message::entries::OfferServiceEntry const& entry,
                                 std::chrono::nanoseconds const& min_delay, std::chrono::nanoseconds const& max_delay,
                                 TimerSendActionType&& post_action) noexcept override;

  /*!
   * \brief Schedule a cyclic OfferService Entry.
   * \param[in] entry OfferService Entry to be scheduled.
   * \param[in] cyclic_offer_delay Timer cyclic delay.
   * \pre -
   * \context Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void ScheduleOfferServiceEntry(message::entries::OfferServiceEntry const& entry,
                                 std::chrono::nanoseconds const& cyclic_offer_delay) noexcept override;

  /*!
   * \brief Remove a OfferService Entry from the cyclic timer.
   * \param[in] entry_id Entry id of the OfferService Entry to be unscheduled.
   * \param[in] cyclic_offer_delay Timer cyclic delay.
   * \pre -
   * \context App, Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnscheduleOfferServiceEntry(message::entries::ServiceEntryId const& entry_id,
                                   std::chrono::nanoseconds const& cyclic_offer_delay) noexcept override;

  /*!
   * \brief Remove a OfferService Entry from the multicast timer.
   * \param[in] entry_id Entry id of the OfferService Entry to be unscheduled.
   * \pre -
   * \context Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnscheduleOfferServiceEntry(message::entries::ServiceEntryId const& entry_id) noexcept override;

  /*!
   * \brief Handler for asynchronous event notification on UDP sockets.
   * \param[in] handle A handle.
   * \pre -
   * \context Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady TRUE
   */
  void HandleRead(osabstraction::io::NativeHandle const& handle);

  /*!
   * \brief Register a server endpoint observer.
   *
   * \param[in] key   The lookup key.
   * \param[in] value The server endpoint observer to register.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void RegisterServer(
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const& key,
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryServerEndpointObserverSharedPtr value) noexcept;

  /*!
   * \brief Register a client endpoint observer.
   *
   * \param[in] key   The lookup key.
   * \param[in] value The client endpoint observer to register.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void RegisterClient(
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const& key,
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryClientEndpointObserverSharedPtr value) noexcept;

  /*!
   * \brief Un-register the client endpoint observer.
   *
   * \param[in] key   The lookup key.
   * \pre -
   * \context Shutdown
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnregisterClient(
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const& key) noexcept;

  /*!
   * \brief Un-register the server endpoint observer.
   *
   * \param[in] key   The lookup key.
   * \pre -
   * \context Shutdown
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnregisterServer(
      typename ServiceDiscoveryMessageProcessor::ServiceDiscoveryEndpointObserverMapKey const& key) noexcept;

  /*!
   * \brief A SOME/IP message type.
   */
  using SomeIpMessage = amsr::someip_protocol::internal::SomeIpMessage;

  /*!
   * \brief Parses a single received SOME/IP-SD message.
   * \param[in] packet A SOME/IP-SD packet.
   * \param[in] from_endpoint The communication endpoint of the SOME/IP SD message's sender.
   * \param[in] is_multicast Specifies whether a SOME/IP-SD message is a multicast one.
   * \pre -
   * \context Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   *
   * \trace SPEC-4981674
   * \trace SPEC-4981679
   */
  void ProcessSdMessage(SomeIpMessage const& packet, amsr::net::ip::Udp::Endpoint const& from_endpoint,
                        bool const is_multicast);

 protected:
  /*!
   * \brief Create and open the sockets.
   * \pre -
   * \context Init, Network, Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady TRUE
   */
  void OpenSockets() noexcept;
  /*!
   * \brief Represents a single UDP datagram.
   */
  using DatagramBuffer = ara::core::Vector<std::uint8_t>;

  /*!
   * \brief An iterator of a datagram buffer.
   */
  using DatagramBufferIterator = DatagramBuffer::iterator;

  /*!
   * \brief A pair of bool and next free position in the datagram buffer.
   */
  using SerializeSdMessageResultPair = std::pair<bool, ServiceDiscoveryEndpoint::DatagramBufferIterator>;

  /*!
   * \brief A pointer to the const buffer.
   */
  using ConstBufferPtr = std::uint8_t const*;

  /*!
   * \brief Schedule repetition phase Offer Service messages.
   * \param[in] initial_repetitions_base_delay The base delay.
   * \param[in] initial_repetitions_max The number of repetitions.
   * \param[in] post_action The post action to perform.
   * \param[in] offer_service_entry The Offer Service Entry.
   * \pre -
   * \context Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void ScheduleOfferService(std::chrono::nanoseconds const& initial_repetitions_base_delay,
                            configuration::ConfigurationTypesAndDefs::InitialRepetitionsMax initial_repetitions_max,
                            message::scheduler::RepetitionOfferTimer::TimerSendActionType&& post_action,
                            message::entries::OfferServiceEntry&& offer_service_entry) noexcept override;

  /*!
   * \brief Unschedule repetition phase Offer Service messages.
   * \param[in] initial_repetitions_base_delay The base delay.
   * \param[in] service_entry_id The service entry ID.
   * \pre -
   * \context Timer, App, Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   */
  void UnscheduleOfferService(std::chrono::nanoseconds const& initial_repetitions_base_delay,
                              message::entries::ServiceEntryId const& service_entry_id) noexcept override;

 private:
  /*!
   * \brief Sends a unicast SOME/IP SD message to the provided IP address and port.
   * \param[in] receiver_address Pair of IP address and port identifying the destination.
   * \param[in] sd_message The SD message to be sent.
   * \return True If the message was sent successfully, and false otherwise.
   * \pre -
   * \context Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  bool SendUnicastMessage(amsr::someip_protocol::internal::AddressPair const& receiver_address,
                          message::ServiceDiscoveryMessage const& sd_message) noexcept override;

  /*!
   * \brief Sends a multicast SOME/IP SD message to the provided IP address and port.
   * \param[in] sd_message The SD message to be sent.
   * \return True If the message was sent successfully, and false otherwise.
   * \pre -
   * \context Timer, Network, App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  bool SendMulticastMessage(message::ServiceDiscoveryMessage const& sd_message) noexcept override;

  /*!
   * \brief Serializes a SD message into the pre-allocated memory buffer.
   * \param[in] sd_message The SD message to be sent.
   * \return A span pointing to the serialized message and its size.
   * \pre -
   * \context Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  ara::core::Span<std::uint8_t> SerializeSdMessage(message::ServiceDiscoveryMessage const& sd_message) noexcept;

  /*!
   * \brief Processes a single received SOME/IP-SD message.
   * \param[in] from_endpoint The communication endpoint of the SOME/IP SD message's sender.
   * \param[in] is_multicast Specifies whether a SOME/IP-SD message is a multicast one.
   * \param[in] sd_message The received SD message.
   * \pre -
   * \context Network
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  void InvokeReceiveHandler(amsr::net::ip::Udp::Endpoint const& from_endpoint, bool const is_multicast,
                            message::ServiceDiscoveryMessage const& sd_message) noexcept;

  /*!
   * \brief Validates SD message header.
   * \param[in] header SOME/IP message header.
   * \return Validation result true or false.
   * \pre -
   * \context Init, Network, Timer
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \trace SPEC-10144587
   * \trace SPEC-10144588
   * \trace SPEC-10144589
   * \trace SPEC-10144590
   * \trace SPEC-10144591
   * \trace SPEC-10144592
   * \trace SPEC-10144585
   */
  static bool ValidateHeader(amsr::someip_protocol::internal::SomeIpMessageHeader const& header) noexcept;

  /*!
   * \brief Generates a unique logger prefix for a given SD endpoint
   *
   * \return    A string representing the logger prefix for the given SD endpoint.
   *
   * \pre       -
   * \context   Init
   * \steady FALSE
   */
  ara::core::String GetLoggerPrefix() const noexcept;

  /*!
   * \brief A unicast IP address used for sending and receiving of SD messages.
   */
  someip_daemon_core::IpAddress const ucast_address_;

  /*!
   * \brief A multicast IP address used for sending and receiving of SD messages.
   */
  someip_daemon_core::IpAddress const mcast_address_;

  /*!
   * \brief   A reusable memory buffer for sending SD messages.
   * \details The buffer is part of the static storage of this object and reused to serialize the SD.
   */
  ara::core::Array<std::uint8_t, amsr::someip_protocol::internal::kMaxSdMessageSize> outgoing_sd_message_buffer_{};

  /*!
   * \brief A unicast UDP socket (wrapper).
   */
  UnicastUDPSocket ucast_socket_;

  /*!
   * \brief A multicast UDP socket (wrapper).
   */
  MulticastUDPSocket mcast_socket_;

  /*!
   * \brief A SOME/IP message reader.
   */
  ServiceDiscoveryMessageSender message_sender_{ucast_socket_, mcast_socket_};

  /*!
   * \brief A UdpSocketReader for reading datagrams from a UDP socket.
   */
  connection_manager::UdpSocketReader udp_socket_reader_;

  /*!
   * \brief A SOME/IP message reader.
   */
  message_reader::DatagramMessageReader message_reader_{};

  /*!
   * \brief State variable that becomes true after the first call to OpenSockets().
   */
  bool sockets_opened_{false};

  /*!
   * \brief Service Discovery Message Builder.
   * \details For each SD endpoint, one separate builder must exist.
   *          Because it manages session id and reboot flag.
   * \trace SPEC-4981506
   */
  message::ServiceDiscoveryMessageBuilder sd_message_builder_{};

  /*!
   * \brief Identity and access management.
   */
  IdentityAccessManager& identity_access_manager_;

  /*!
   * \brief A statistics handler.
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  /*!
   * \brief SD Message processor.
   */
  ServiceDiscoveryMessageProcessor message_processor_;

  /*!
   * \brief SD Message Send Scheduler
   */
  message::scheduler::Scheduler send_scheduler_;

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription, GetLoggerPrefix()};
};

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_H_

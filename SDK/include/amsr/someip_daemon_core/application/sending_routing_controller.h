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
/**        \file  sending_routing_controller.h
 *        \brief  Application sending routing controller.
 *
 *      \details  Routing controller for send path to application.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_SENDING_ROUTING_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_SENDING_ROUTING_CONTROLLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <cstddef>
#include <memory>
#include <sstream>
#include <utility>

#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/client/event_handler_interface.h"
#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "amsr/someip_daemon_core/client/method_response_handler_interface.h"
#include "amsr/someip_daemon_core/client/pdu_event_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logging_utilities.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "amsr/someip_daemon_core/service_discovery/ser_deser_service_discovery.h"
#include "amsr/someip_daemon_core/tracing/network_tracing_interface.h"
#include "amsr/someip_daemon_core/tracing/sd_tracing_interface.h"
#include "ara/core/array.h"
#include "ara/core/string_view.h"
#include "ara/log/logger.h"
#include "ara/log/logstream.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/ipc_packet_serializer.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/language/location.h"
#include "vac/memory/memory_buffer_wrapper.h"

#include "someipd_app_protocol/internal/protocol_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

namespace someipd_app_protocol = ::amsr::someipd_app_protocol::internal;

/*!
 * \brief IPC sending routing controller.
 *
 * \tparam ApplicationConnectionType Type used to manage connection between SOME/IP daemon and the application.
 */
template <typename ApplicationConnectionType>
class SendingRoutingController final : public packet_sink::LocalPacketSink,
                                       public client::EventHandlerInterface,
                                       public client::MethodResponseHandlerInterface {
 public:
  /*!
   * \brief A type alias for "ApplicationConnection".
   */
  using ApplicationConnection = ApplicationConnectionType;

  /*!
   * \brief Constructor of "SendingRoutingController".
   *
   * \param[in] connection       A reference to the IPC connection for message transmission.
   * \param[in] network_tracing  A reference to the network tracing implementation.
   * \param[in] sd_tracing       A reference to the SD tracing implementation.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  explicit SendingRoutingController(ApplicationConnection& connection,
                                    tracing::NetworkTracingInterface& network_tracing,
                                    tracing::SdTracingInterface& sd_tracing) noexcept
      : packet_sink::LocalPacketSink(),
        client::EventHandlerInterface(),
        client::MethodResponseHandlerInterface(),
        connection_{connection},
        network_tracing_{network_tracing},
        sd_tracing_{sd_tracing} {}

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief   Destructor of SendingRoutingController.
   * \steady  FALSE
   */
  ~SendingRoutingController() noexcept override = default;

  SendingRoutingController(SendingRoutingController const&) = delete;
  SendingRoutingController(SendingRoutingController const&&) = delete;
  auto operator=(SendingRoutingController const&) -> SendingRoutingController& = delete;
  auto operator=(SendingRoutingController const&&) -> SendingRoutingController& = delete;

  /*!
   * \brief To be called on a service instance offer of a specific service instance.
   *
   * \param[in] service_instance_id The SOME/IP service instance id of the offered service.
   *
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Trace the service instance up notification.
   * - Update the connected application about the state of the "ServiceInstance".
   * \endinternal
   */
  void OnStartOfferServiceInstance(configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept {
    // Trace instance state update
    sd_tracing_.TraceServiceInstanceUpSync(
        connection_.GetPeerProcessId(), service_instance_id.deployment_id.service_interface_id,
        service_instance_id.deployment_id.service_interface_version.major_version, service_instance_id.instance_id);

    // Send message to the connected application
    SendServiceDiscoveryServiceInstanceUpMessage(service_instance_id);
  }

  /*!
   * \brief To be called on a stopped offer of a specific service instance.
   *
   * \param[in] service_instance_id The SOME/IP service instance id of the stopped service.
   *
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Trace service instance down notification.
   * - Update the connected application about the state of the "ServiceInstance".
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  void OnStopOfferServiceInstance(configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept {
    // Trace instance state update
    sd_tracing_.TraceServiceInstanceDownSync(
        connection_.GetPeerProcessId(), service_instance_id.deployment_id.service_interface_id,
        service_instance_id.deployment_id.service_interface_version.major_version, service_instance_id.instance_id);

    // Send message to the connected application
    SendServiceDiscoveryServiceInstanceDownMessage(service_instance_id);
  }

  /*!
   * \brief Handle PDU Event responses sent to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \return -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Trace the reception of a PDU message.
   * - Serialize the header of the service instance.
   * - Transmit PDU message to application.
   * - If transmission is unsuccessful, log an error message.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnPduEvent(someip_protocol::internal::InstanceId const instance_id, client::PduPacket packet) noexcept override {
    // PTP-B-SomeIpDaemon-SendingRoutingController_OnPduEvent

    // Get message header
    someip_protocol::internal::PduMessageHeader const header{packet->GetHeader()};
    someip_protocol::internal::ServiceId const service_id{
        someip_protocol::internal::PduMessageHeaderUtility::GetServiceId(header)};
    someip_protocol::internal::EventId const event_id{
        someip_protocol::internal::PduMessageHeaderUtility::GetMethodId(header)};

    // Trace PDU reception
    network_tracing_.TracePduEventReceiveSync(connection_.GetPeerProcessId(), service_id, instance_id, event_id,
                                              packet->GetTotalSize());

    // Serialize specific header.
    SpecificHeaderView const specific_header_view{SerializeSpecificHeaderForPduMessage(instance_id)};

    // Transmit PDU message to application.
    bool const result{connection_.SendPduMessage(specific_header_view, packet)};

    if (!result) {
      logger_.LogError(
          [&service_id, &event_id, &instance_id](ara::log::LogStream& s) noexcept {
            s << "Failed to forward a PDU message to application with (ServiceId: 0x";
            s << ara::log::HexFormat(service_id);
            s << "InstanceId: 0x";
            s << ara::log::HexFormat(instance_id);
            s << ", EventId: 0x";
            // Append the event Id in the correct format
            logging::LoggingUtilities::LogEventId(s, event_id);
            s << ").";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // PTP-E-SomeIpDaemon-SendingRoutingController_OnPduEvent
  }

  /*!
   * \brief Handle SOME/IP Event responses sent to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \return -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Trace reception of SOME/IP message.
   * - Serialize the header of the service instance.
   * - Transmit SOME/IP message to application.
   * - If transmission is unsuccessful, log an error message.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnSomeIpEvent(amsr::someip_protocol::internal::InstanceId const instance_id,
                     client::Packet const packet) noexcept override {
    // PTP-B-SomeIpDaemon-SendingRoutingController_OnSomeIpEvent
    someip_protocol::internal::SomeIpMessageHeader const header{packet->GetHeader()};

    // Trace event reception
    network_tracing_.TraceSomeIpEventReceiveSync(connection_.GetPeerProcessId(), header.service_id_,
                                                 header.interface_version_, instance_id, header.method_id_,
                                                 packet->GetTotalSize());

    // Serialize specific header.
    SpecificHeaderView const specific_header_view{SerializeSpecificHeaderForSomeIpMessage(instance_id)};
    // Transmit SOME/IP message to application.
    bool const result{connection_.SendSomeIpMessage(specific_header_view, packet)};

    if (!result) {
      logger_.LogError(
          [&header, &instance_id](ara::log::LogStream& s) noexcept {
            s << "Failed to forward a SOME/IP event to application with (ServiceId: 0x";
            s << ara::log::HexFormat(header.service_id_);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(header.interface_version_);
            s << "InstanceId: 0x";
            s << ara::log::HexFormat(instance_id);
            s << "EventId: ";
            s << header.method_id_;
            s << "ClientId:: ";
            s << header.client_id_;
            s << ").";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // PTP-E-SomeIpDaemon-SendingRoutingController_OnSomeIpEvent
  }

  /*!
   * \brief Handle Initial field notifications sending to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP field notification.
   * \param[in] client_id     SOME/IP client id.
   *
   * \return -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Serialize the header for initial field notification.
   * - Transmit the initial field notification message to application.
   * - If transmission is unsuccessful, log an error message.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnSomeIpInitialFieldNotification(amsr::someip_protocol::internal::InstanceId const instance_id,
                                        client::Packet const packet,
                                        amsr::someip_protocol::internal::ClientId const client_id) noexcept override {
    // PTP-B-SomeIpDaemon-SendingRoutingController_OnSomeIpInitialFieldNotification
    // Serialize specific header with client id.
    SpecificHeaderView const specific_header_view{
        SerializeSpecificHeaderForInitialFieldNotificationSomeIpMessage(instance_id, client_id)};

    // Transmit initial field notification SOME/IP message to application.
    bool const result{connection_.SendInitialFieldNotificationMessage(specific_header_view, packet)};

    if (!result) {
      someip_protocol::internal::SomeIpMessageHeader const header{packet->GetHeader()};
      logger_.LogError(
          [&header, &instance_id](ara::log::LogStream& s) noexcept {
            s << "Failed to send an initial field notification SOME/IP message to application with (ServiceId: 0x";
            s << ara::log::HexFormat(header.service_id_);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(header.interface_version_);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(instance_id);
            s << ", EventId: ";
            s << header.method_id_;
            s << ", ClientId: ";
            s << header.client_id_;
            s << ").";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // PTP-E-SomeIpDaemon-SendingRoutingController_OnSomeIpInitialFieldNotification
  }

  /*!
   * \brief Inform the application about a PDU event subscription state change.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] event_id The subscribed event identifier.
   * \param[in] new_state Current event subscription state.
   *
   * \return -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnPduSubscriptionStateChange(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                                    someip_protocol::internal::EventId const event_id,
                                    client::eventgroup::EventSubscriptionState const new_state) noexcept override {
    SendEventSubscriptionStateMessage(service_instance_id, event_id,
                                      static_cast<amsr::someip_protocol::internal::SubscriptionState>(new_state));
  }

  /*!
   * \brief Handle SOME/IP event subscription state change.
   *
   * \param[in] service_instance_id The service instance id.
   * \param[in] event_id The subscribed event identifier.
   * \param[in] new_state Current event subscription state.
   *
   * \return -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnSomeIpSubscriptionStateChange(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                                       someip_protocol::internal::EventId const event_id,
                                       client::eventgroup::EventSubscriptionState const new_state) noexcept override {
    SendEventSubscriptionStateMessage(service_instance_id, event_id,
                                      static_cast<amsr::someip_protocol::internal::SubscriptionState>(new_state));
  }

  /*!
   * \brief Forward method response to the connected application.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \return    -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \internal
   * - Trace method response reception.
   * - Serialize the header of the service instance.
   * - Transmit SOME/IP message to application.
   * - If transmission is unsuccessful, log an error message.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnMethodResponse(someip_protocol::internal::InstanceId const instance_id,
                        client::Packet packet) noexcept override {
    // PTP-B-SomeIpDaemon-SendingRoutingController_OnMethodResponse
    someip_protocol::internal::SomeIpMessageHeader const header{packet->GetHeader()};

    // Trace event reception
    network_tracing_.TraceMethodResponseReceiveSync(connection_.GetPeerProcessId(), header.service_id_,
                                                    header.interface_version_, instance_id, header.method_id_,
                                                    packet->GetTotalSize());

    // Serialize specific header.
    SpecificHeaderView const specific_header_view{SerializeSpecificHeaderForSomeIpMessage(instance_id)};
    // Transmit SOME/IP message to application.
    bool const result{connection_.SendSomeIpMessage(specific_header_view, packet)};

    if (!result) {
      logger_.LogError(
          [&header, &instance_id](ara::log::LogStream& s) noexcept {
            s << "Failed to forward a SOME/IP method response to application with (ServiceId: 0x";
            s << ara::log::HexFormat(header.service_id_);
            s << ", MajorVersion: 0x";
            s << ara::log::HexFormat(header.interface_version_);
            s << ", InstanceId: 0x";
            s << ara::log::HexFormat(instance_id);
            s << ", MethodId: ";
            s << header.method_id_;
            s << ", ClientId:: ";
            s << header.client_id_;
            s << ").";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // PTP-E-SomeIpDaemon-SendingRoutingController_OnMethodResponse
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a SOME/IP method request to the connected application.
   *
   * \param[in] instance_id   SOME/IP service instance identifier.
   * \param[in] sender        The request sender.
   * \param[in] packet        A pointer to a SOME/IP message.
   *
   * \pre -
   * \context Network, Timer
   * \reentrant FALSE
   * \steady    TRUE
   * \return "true" if the message is sent successfully, and "false" otherwise.
   *
   * \internal
   * - Trace method request reception.
   * - Serialize the header of the service instance.
   * - Transmit SOME/IP message to application.
   * - Return the result value.
   * \endinternal
   */
  auto HandleMethodRequest(amsr::someip_protocol::internal::InstanceId const instance_id,
                           packet_router::RemoteSourceIdentifier const& sender,
                           std::shared_ptr<someip_protocol::internal::SomeIpMessage> const packet) noexcept
      -> bool override {
    // Get message header
    someip_protocol::internal::SomeIpMessageHeader const header{packet->GetHeader()};
    assert(header.message_type_ == someip_protocol::internal::SomeIpMessageType::kRequest);

    static_cast<void>(sender);  // TODO SVT-3253: parameter will be used in an upcoming change

    // Trace event reception
    network_tracing_.TraceMethodRequestReceiveSync(connection_.GetPeerProcessId(), header.service_id_,
                                                   header.interface_version_, instance_id, header.method_id_,
                                                   packet->GetTotalSize());

    // Serialize specific header.
    SpecificHeaderView const specific_header_view{SerializeSpecificHeaderForSomeIpMessage(instance_id)};
    // Transmit SOME/IP message to application.
    return connection_.SendSomeIpMessage(specific_header_view, packet);
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a SOME/IP method request no return to the connected application.
   *
   * \param[in] instance_id   SOME/IP service instance identifier.
   * \param[in] packet        A pointer to a SOME/IP message.
   *
   * \pre -
   * \context Network, Timer
   * \reentrant FALSE
   * \steady    TRUE
   * \return "true" if the message is sent successfully, and "false" otherwise.
   *
   * \internal
   * - Trace method request no return reception (as normal method request reception)
   * - Serialize the header of the service instance.
   * - Transmit SOME/IP message to application.
   * - Return the result value.
   * \endinternal
   */
  auto HandleMethodRequestNoReturn(amsr::someip_protocol::internal::InstanceId const instance_id,
                                   std::shared_ptr<someip_protocol::internal::SomeIpMessage> const packet) noexcept
      -> bool override {
    // Get message header
    someip_protocol::internal::SomeIpMessageHeader const header{packet->GetHeader()};
    assert(header.message_type_ == someip_protocol::internal::SomeIpMessageType::kRequestNoReturn);

    // Trace event reception
    network_tracing_.TraceMethodRequestReceiveSync(connection_.GetPeerProcessId(), header.service_id_,
                                                   header.interface_version_, instance_id, header.method_id_,
                                                   packet->GetTotalSize());

    // Serialize specific header.
    SpecificHeaderView const specific_header_view{SerializeSpecificHeaderForSomeIpMessage(instance_id)};
    // Transmit SOME/IP message to application.
    return connection_.SendSomeIpMessage(specific_header_view, packet);
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Forwards a PDU message to the connected application.
   *
   * \param[in] instance_id   SOME/IP service instance identifier.
   * \param[in] packet        A pointer to a PDU message.
   *
   * \steady  FALSE
   *
   * \internal
   * - Must not be called, if happened log an error message and abort.
   * \endinternal
   */
  auto ForwardPdu(amsr::someip_protocol::internal::InstanceId const instance_id,
                  std::shared_ptr<someip_protocol::internal::PduMessage> const packet) noexcept -> bool override {
    static_cast<void>(instance_id);
    static_cast<void>(packet);
    logger_.LogFatalAndAbort(
        [](ara::core::String& abort_msg) noexcept {
          abort_msg.append("Wrong path. Forwards PDU messages shall done through LocalClient");
        },
        AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    return false;
  }

 private:
  /*!
   * \brief Sends a "ServiceDiscoveryServiceInstanceUp" routing message to the connected application.
   *
   * \param[in] service_instance_id The SOME/IP service instance id of the offered service.
   *
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Allocate memory for the memory buffer.
   * - If memory allocation succeeds,
   *   - Serialize an empty header.
   *   - Serialize the payload.
   *   - Transmit the message to the connected application.
   *   - If transmission fails, log an error message.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  void SendServiceDiscoveryServiceInstanceUpMessage(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept {
    constexpr std::size_t kAllocSize{someipd_app_protocol::kServiceDiscoveryServiceInstanceUpdateMessageHeaderLength};

    // Allocate memory for our packet
    ara::core::Result<vac::memory::allocator::MemoryBufferPtr> alloc_result{
        transmit_message_allocator_.Allocate(kAllocSize)};
    // Memory allocation is not expected to fail. In case of lack of memory, it would be handled by the allocator
    assert(alloc_result.HasValue());

    // Wrap the memory into the appropriate data structure (for compatibility reasons)
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_local_object_not_be_allocated_on_heap
    vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer{
        memory::MemoryUtilities::CreateUniquePtr<vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>(
            std::move(alloc_result.Value()))};

    vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer>::MemoryBufferView packet_view{
        memory_buffer->GetView(0U)};
    someip_protocol::internal::serialization::BufferView const buffer_view{
        // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
        static_cast<std::uint8_t*>(packet_view[0U].base_pointer), memory_buffer->size()};
    someip_protocol::internal::serialization::Writer writer{buffer_view};

    service_discovery::SerializeRoutingData const routing_data{
        writer, service_instance_id.deployment_id.service_interface_id, service_instance_id.instance_id,
        service_instance_id.deployment_id.service_interface_version.major_version,
        service_instance_id.deployment_id.service_interface_version.minor_version};

    service_discovery::SerializeRoutingServiceDiscoveryServiceInstanceUpdateMessagePayload(routing_data);

    // Serialize specific header.
    SpecificHeaderView const specific_header_view{SerializeEmptySpecificHeader()};
    // Transmit message to application.
    bool const result{connection_.SendMessage(someipd_app_protocol::MessageType::kServiceDiscoveryServiceInstanceUp,
                                              specific_header_view, std::move(memory_buffer))};
    if (!result) {
      logger_.LogError(
          [](ara::log::LogStream& s) noexcept {
            s << "Failed to send a ServiceDiscoveryServiceInstanceUp message to application";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Sends a "ServiceDiscoveryServiceInstanceDown" routing message to the connected application.
   *
   * \param[in] service_instance_id The SOME/IP service instance id of the stopped service.
   *
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Allocate memory for the memory buffer.
   * - If memory allocation succeeds,
   *   - Serialize an empty header.
   *   - Serialize the payload.
   *   - Transmit the message to the connected application.
   *   - If transmission fails, log an error message.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  void SendServiceDiscoveryServiceInstanceDownMessage(
      configuration::types::SomeIpServiceInstanceId const& service_instance_id) noexcept {
    constexpr std::size_t kAllocSize{someipd_app_protocol::kServiceDiscoveryServiceInstanceUpdateMessageHeaderLength};

    // Allocate memory for our packet
    ara::core::Result<vac::memory::allocator::MemoryBufferPtr> alloc_result{
        transmit_message_allocator_.Allocate(kAllocSize)};
    // Memory allocation is not expected to fail. In case of lack of memory, it would be handled by the allocator
    assert(alloc_result.HasValue());

    // Wrap the memory into the appropriate data structure (for compatibility reasons)
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_local_object_not_be_allocated_on_heap
    vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer{
        memory::MemoryUtilities::CreateUniquePtr<vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>(
            std::move(alloc_result.Value()))};

    // Serialize payload
    vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer>::MemoryBufferView packet_view{
        memory_buffer->GetView(0U)};
    someip_protocol::internal::serialization::BufferView const buffer_view{
        // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
        static_cast<std::uint8_t*>(packet_view[0U].base_pointer), memory_buffer->size()};
    someip_protocol::internal::serialization::Writer writer{buffer_view};

    service_discovery::SerializeRoutingData const routing_data{
        writer, service_instance_id.deployment_id.service_interface_id, service_instance_id.instance_id,
        service_instance_id.deployment_id.service_interface_version.major_version,
        service_instance_id.deployment_id.service_interface_version.minor_version};

    service_discovery::SerializeRoutingServiceDiscoveryServiceInstanceUpdateMessagePayload(routing_data);
    // Serialize specific header.
    SpecificHeaderView const specific_header_view{SerializeEmptySpecificHeader()};
    // Transmit message to application.
    bool const result{connection_.SendMessage(someipd_app_protocol::MessageType::kServiceDiscoveryServiceInstanceDown,
                                              specific_header_view, std::move(memory_buffer))};
    if (!result) {
      logger_.LogError(
          [](ara::log::LogStream& s) noexcept {
            s << "Failed to send a ServiceDiscoveryServiceInstanceDown message to application";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Sends a "EventSubscriptionState" routing message to the connected application.
   *
   * \param[in] service_instance_id The SOME/IP service instance id of the stopped service.
   * \param[in] event_id            A SOME/IP event identifier.
   * \param[in] state               An event subscription state.
   *
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Allocate memory for the memory buffer.
   * - If memory allocation succeeds,
   *   - Serialize an empty header.
   *   - Serialize the payload.
   *   - Transmit the message to the connected application.
   *   - If transmission fails, log an error message.
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_SomeIpDaemon_V3.9.2_PreferUserDefinedTypes
  void SendEventSubscriptionStateMessage(configuration::types::SomeIpServiceInstanceId const& service_instance_id,
                                         amsr::someip_protocol::internal::EventId const event_id,
                                         amsr::someip_protocol::internal::SubscriptionState const state) noexcept {
    constexpr std::size_t kAllocSize{someipd_app_protocol::kServiceDiscoveryEventSubscriptionStateMessageHeaderLength};

    // Allocate memory for our packet
    ara::core::Result<vac::memory::allocator::MemoryBufferPtr> alloc_result{
        transmit_message_allocator_.Allocate(kAllocSize)};
    // Memory allocation is not expected to fail. In case of lack of memory, it would be handled by the allocator
    assert(alloc_result.HasValue());

    // Wrap the memory into the appropriate data structure (for compatibility reasons)
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_local_object_not_be_allocated_on_heap
    vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> memory_buffer{
        memory::MemoryUtilities::CreateUniquePtr<vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>(
            std::move(alloc_result.Value()))};

    vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer>::MemoryBufferView packet_view{
        memory_buffer->GetView(0U)};
    someip_protocol::internal::serialization::BufferView const buffer_view{
        // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
        static_cast<std::uint8_t*>(packet_view[0U].base_pointer), memory_buffer->size()};
    someip_protocol::internal::serialization::Writer writer{buffer_view};

    service_discovery::SerializeEventData const event_data{
        writer,
        service_instance_id.deployment_id.service_interface_id,
        service_instance_id.instance_id,
        service_instance_id.deployment_id.service_interface_version.major_version,
        service_instance_id.deployment_id.service_interface_version.minor_version,
        event_id,
        state};

    service_discovery::SerializeRoutingServiceDiscoveryEventSubscriptionStateMessagePayload(event_data);
    // Serialize specific header.
    SpecificHeaderView const specific_header_view{SerializeEmptySpecificHeader()};
    // Transmit message to application.
    bool const result{
        connection_.SendMessage(someipd_app_protocol::MessageType::kServiceDiscoveryEventSubscriptionState,
                                specific_header_view, std::move(memory_buffer))};
    if (!result) {
      logger_.LogError(
          [](ara::log::LogStream& s) noexcept {
            s << "Failed to send a EventSubscriptionState message to application";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Creates the empty specific header.
   *
   * \details Some routing messages sent to application do not use specific header but it still needs to be.
   *         sent according to the IPC protocol between the SOME/IP daemon and application.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return The created Buffer view based on someip message header.
   *
   * \internal
   * - Serialize the routing service discovery header.
   * \endinternal
   */
  someipd_app_protocol::someip_protocol::BufferView SerializeEmptySpecificHeader() noexcept {
    // Fill specific header with '\0' bytes
    amsr::someip_protocol::internal::serialization::BufferView const buffer_view{transmit_routing_sd_message_header_};
    amsr::someip_protocol::internal::serialization::Writer writer{buffer_view};

    service_discovery::SerializeRoutingServiceDiscoveryHeader(writer);

    return buffer_view;
  }

  /*!
   * \brief Creates a specific header for the given SOME/IP service instance identifier.
   *
   * \param[in] instance_id SOME/IP service instance identifier.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   * \return The created Buffer view based on someip message header.
   *
   * \internal
   * - Serialize the routing SOME/IP message header.
   * \endinternal
   */
  someipd_app_protocol::someip_protocol::BufferView SerializeSpecificHeaderForSomeIpMessage(
      amsr::someip_protocol::internal::InstanceId const instance_id) noexcept {
    // Serialize routing message header
    someipd_app_protocol::someip_protocol::BufferView const buffer_view{transmit_routing_someip_message_header_};
    someipd_app_protocol::someip_protocol::Writer writer{buffer_view};

    someipd_app_protocol::SerializeSpecificIpcPacketHeaderRoutingMessage(writer, instance_id);

    return buffer_view;
  }

  /*!
   * \brief Creates a specific header for the given SOME/IP service instance identifier.
   *
   * \param[in] instance_id SOME/IP service instance identifier.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   * \return The created Buffer view based on pdu message header.
   *
   * \internal
   * - Serialize the routing SOME/IP message header.
   * \endinternal
   */
  someipd_app_protocol::someip_protocol::BufferView SerializeSpecificHeaderForPduMessage(
      amsr::someip_protocol::internal::InstanceId const instance_id) noexcept {
    // Serialize routing message header
    someipd_app_protocol::someip_protocol::BufferView const buffer_view{transmit_routing_pdu_message_header_};
    someipd_app_protocol::someip_protocol::Writer writer{buffer_view};

    someipd_app_protocol::SerializeSpecificIpcPacketHeaderRoutingMessage(writer, instance_id);

    return buffer_view;
  }

  /*!
   * \brief Creates a specific header for the given initial field notification message.
   *
   * \param[in] instance_id SOME/IP service instance identifier.
   * \param[in] client_id SOME/IP client identifier.
   *
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   * \return The created Buffer view based on someip message header.
   *
   * \internal
   * - Clear the routing message header.
   * - Resize it to the correct length.
   * - Serialize the routing initial field notification SOME/IP message header.
   * \endinternal
   */
  someipd_app_protocol::someip_protocol::BufferView SerializeSpecificHeaderForInitialFieldNotificationSomeIpMessage(
      amsr::someip_protocol::internal::InstanceId const instance_id,
      amsr::someip_protocol::internal::ClientId const client_id) noexcept {
    // Serialize routing message header
    someipd_app_protocol::someip_protocol::BufferView const buffer_view{transmit_routing_someip_message_header_};
    someipd_app_protocol::someip_protocol::Writer writer{buffer_view};

    someipd_app_protocol::SerializeSpecificIpcPacketHeaderRoutingMessage(writer, instance_id, client_id);

    return buffer_view;
  }

  /*!
   * \brief The allocator for messages to be sent to the connected application.
   */
  someip_daemon_core::MemoryBufferAllocator transmit_message_allocator_{};

  /*!
   * \brief The reference to the IPC connection for message transmission.
   */
  ApplicationConnection& connection_;

  /*!
   * \brief Reference to network tracing implementation
   */
  tracing::NetworkTracingInterface& network_tracing_;

  /*!
   * \brief Reference to service discovery tracing implementation
   */
  tracing::SdTracingInterface& sd_tracing_;

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger const logger_{
      someip_daemon_core::logging::kApplicationLoggerContextId,
      someip_daemon_core::logging::kApplicationLoggerContextDescription,
      ara::core::StringView{"SendingRoutingController"}};

  /*!
   * \brief A buffer for the routing someip message header.
   */
  ara::core::Array<std::uint8_t, amsr::someipd_app_protocol::internal::kRoutingSomeIpMessageHeaderLength>
      transmit_routing_someip_message_header_{};
  /*!
   * \brief A buffer for the routing pdu message header.
   */
  ara::core::Array<std::uint8_t, amsr::someipd_app_protocol::internal::kRoutingPduMessageHeaderLength>
      transmit_routing_pdu_message_header_{};
  /*!
   * \brief A buffer for the routing service discovery message header.
   */
  ara::core::Array<std::uint8_t, amsr::someipd_app_protocol::internal::kRoutingServiceDiscoveryHeaderLength>
      transmit_routing_sd_message_header_{};
};

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_SENDING_ROUTING_CONTROLLER_H_

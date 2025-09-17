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
/*!        \file  someip_daemon_client/internal/receive_routing_controller.h
 *         \brief  Receive routing controller of SomeIpDaemonClient
 *        \details  Setting handlers for routing messages and processing routing messages
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_RECEIVE_ROUTING_CONTROLLER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_RECEIVE_ROUTING_CONTROLLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include <utility>

#include "amsr/core/optional.h"
#include "ara/core/string.h"
#include "ara/log/logging.h"
#include "ara/log/logstream.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_daemon_client/internal/client_interface.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/server_interface.h"
#include "someipd_app_protocol/internal/deser_message_headers.h"
#include "someipd_app_protocol/internal/message.h"
#include "someipd_app_protocol/internal/pdu_message.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Groups together functionality for all routing messages sent from application to SOME/IP daemon.
 */
class ReceiveRoutingController final {
 public:
  /*!
   * \brief Alias for IPC protocol message type
   */
  using MessageType = amsr::someipd_app_protocol::internal::MessageType;

  /*!
   * \brief Alias for IPC protocol instance ID
   */
  using RoutingMessageInstanceId = amsr::someipd_app_protocol::internal::RoutingMessageInstanceId;

  /*!
   * \brief Alias for IPC protocol client ID
   */
  using RoutingMessageClientId = amsr::someipd_app_protocol::internal::RoutingMessageClientId;

  /*!
   * \brief Constructor of ReceiveRoutingController.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // NOLINTNEXTLINE (hicpp-use-equals-default)
  ReceiveRoutingController() noexcept {};

  /*!
   * \brief Destructor of ReceiveRoutingController.
   * \pre -
   * \context Shutdown
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ~ReceiveRoutingController() noexcept = default;

  /*!
   * \brief Delete copy constructor.
   */
  ReceiveRoutingController(ReceiveRoutingController const&) = delete;

  /*!
   * \brief Delete copy assignment operator.
   */
  auto operator=(ReceiveRoutingController const&) -> ReceiveRoutingController& = delete;

  /*!
   * \brief Delete move constructor.
   */
  ReceiveRoutingController(ReceiveRoutingController const&&) = delete;

  /*!
   * \brief Delete move assignment operator.
   */
  auto operator=(ReceiveRoutingController const&&) -> ReceiveRoutingController& = delete;

  /*!
   * \brief Sets a receive handler for server-side incoming SOME/IP messages.
   * \param[in] server_manager A pointer to the server-side handler object to which received SOME/IP messages
   * will be passed.
   * \pre server_manager is not a nullptr.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetServerManager(ServerInterface* server_manager) { server_manager_ = server_manager; }

  /*!
   * \brief Sets a receive handler for client-side incoming SOME/IP messages, asynchronous notifications of offered
   * services and event subscription state changes.
   * \param[in] client_manager A pointer to the client-side handler object to which received SOME/IP messages,
   * offered service instances and event subscription state changes will be passed.
   * \pre client_manager is not a nullptr.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetClientManager(ClientInterface* client_manager) { client_manager_ = client_manager; }

  /*!
   * \brief Receive and process a SOME/IP message
   * \param[in] instance_id SOME/IP Instance ID.
   * \param[in] packet The SOME/IP message.
   * \pre client_manager_ is set
   * \pre server_manager_ is set
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Create SOME/IP message with the passed packet payload
   * - Check if the message created successfully
   *   - Fetch the SOME/IP header and message buffer
   *   - Check the message type of the header
   *     - If the message type belongs to skeleton/server specific message
   *       - Handle the received request from the server side
   *     - Otherwise, if the message type belongs to proxy/client specific message
   *       - Handle the received request from the client side
   *     - Otherwise, log an error that the SOME/IP message type is unsupported
   * - Otherwise, log an error that the received SOME/IP message has invalid size
   * \endinternal
   */
  void ProcessSomeIpMessage(RoutingMessageInstanceId const instance_id,
                            amsr::someipd_app_protocol::internal::SomeIpMessage packet) {
    SomeIpMessageHeader const& someip_header{packet.GetHeader()};
    switch (someip_header.message_type_) {
      // Skeleton / server specific message types
      case amsr::someip_protocol::internal::SomeIpMessageType::kRequest:
      case amsr::someip_protocol::internal::SomeIpMessageType::kRequestNoReturn:
      case amsr::someip_protocol::internal::SomeIpMessageType::kTpRequest:
      case amsr::someip_protocol::internal::SomeIpMessageType::kTpRequestNoReturn: {
        if (server_manager_ != nullptr) {
          // VCA_SDCL_NULL_POINTER_CHECK_FOR_RECEIVE_HANDLER, VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
          server_manager_->HandleReceive(instance_id, std::move(packet));
        } else {
          logger_.LogFatalAndAbort(
              [](logging::AraComLogger::StringStream& s) {
                s << "server_manager_ not set, dropping message and aborting.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                                                 // VCA_SDCL_STRING_STREAM_USAGE
              },
              static_cast<char const*>(__func__), __LINE__);
        }
        break;
      }
        // Proxy / client specific message types
      case amsr::someip_protocol::internal::SomeIpMessageType::kNotification:
      case amsr::someip_protocol::internal::SomeIpMessageType::kTpNotification:
      case amsr::someip_protocol::internal::SomeIpMessageType::kResponse:
      case amsr::someip_protocol::internal::SomeIpMessageType::kError:
      case amsr::someip_protocol::internal::SomeIpMessageType::kTpResponse:
      case amsr::someip_protocol::internal::SomeIpMessageType::kTpError: {
        if (client_manager_ != nullptr) {
          // VCA_SDCL_NULL_POINTER_CHECK_FOR_RECEIVE_HANDLER
          client_manager_->HandleReceive(instance_id, std::move(packet));
        } else {
          logger_.LogFatalAndAbort(
              [](logging::AraComLogger::StringStream& s) {
                s << "client_manager_ not set, dropping message and aborting.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                                                 // VCA_SDCL_STRING_STREAM_USAGE
              },
              static_cast<char const*>(__func__), __LINE__);
        }
        break;
      }

      default: {
        logger_.LogError(
            [&someip_header](ara::log::LogStream& s) {
              s << "Unsupported SOME/IP message type: 0x"
                << ara::log::HexFormat(static_cast<std::uint16_t>(someip_header.message_type_))
                << ". Dropping message.";
            },
            {static_cast<char const*>(__func__)}, {__LINE__});
        break;
      }
    }
  }

  /*!
   * \brief Receive and process a PDU message
   * \param[in] instance_id PDU Instance ID.
   * \param[in] packet The PDU message.
   * \pre client_manager_ is set
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Check if the client manager is set
   *   - Handle the PDU packet with client manager
   * - Otherwise, log a fatal error and abort
   * \endinternal
   */
  void ProcessPduMessage(RoutingMessageInstanceId const instance_id,
                         amsr::someipd_app_protocol::internal::PduMessage packet) const {
    if (client_manager_ != nullptr) {
      // VCA_SDCL_NULL_POINTER_CHECK_FOR_RECEIVE_HANDLER, VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
      client_manager_->HandleReceivePdu(instance_id, std::move(packet));
    } else {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "client_manager_ not set, dropping message and aborting.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                                             // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Process a routing message not of type SOME/IP.
   * \param[in] message_type The type of the received message.
   * \param[in] body_view A view of the message's body.
   * \pre       client_manager_ is set
   * \pre       body_view is not empty
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Check if the client manager is set
   *   - Create the root deserializer with the passed body view
   *   - Check if the message type is kServiceDiscoveryServiceInstanceUp/Down
   *     - Receive and process a ServiceInstanceUp/Down packet with the created root deserializer
   *   - Check if the message type is kServiceDiscoveryEventSubscriptionState
   *     - receive and process an EventSubscriptionState update packet with the created root deserializer
   *   - Otherwise, log an error that the SOME/IP header message type is unsupported
   * - Otherwise, log a fatal error and abort
   * \endinternal
   */
  void ProcessNonSomeIpMessage(someipd_app_protocol::internal::MessageType const message_type,
                               ::amsr::someip_protocol::internal::PacketBufferView const& body_view) const {
    if (client_manager_ != nullptr) {
      switch (message_type) {
        case MessageType::kServiceDiscoveryServiceInstanceUp:
          ReceiveServiceDiscoveryServiceInstanceUp(body_view);
          break;
        case MessageType::kServiceDiscoveryServiceInstanceDown:
          ReceiveServiceDiscoveryServiceInstanceDown(body_view);
          break;
        case MessageType::kServiceDiscoveryEventSubscriptionState:
          ReceiveServiceDiscoveryEventSubscriptionState(body_view);
          break;
        default:
          logger_.LogError(
              [&message_type](ara::log::LogStream& s) {
                s << "Unsupported generic header message type 0x"
                  << ara::log::HexFormat(static_cast<std::underlying_type_t<MessageType>>(message_type));
              },
              {static_cast<char const*>(__func__)}, {__LINE__});
          break;
      }
    } else {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "client_manager_ not set, dropping message and aborting.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                                             // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Receive and process an initial field notification message
   * \param[in] instance_id SomeIp Instance ID.
   * \param[in] packet      The SomeIp message.
   * \param[in] client_id   SomeIp Client ID.
   * \pre client_manager_ is set
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady TRUE
   *
   * \internal
   * - Check if the client manager is set
   *   - Handle the initial field notification packet with client manager
   * - Otherwise, log a fatal error and abort
   * \endinternal
   */
  void ProcessInitialFieldNotificationSomeIpMessage(RoutingMessageInstanceId const instance_id,
                                                    amsr::someipd_app_protocol::internal::SomeIpMessage packet,
                                                    RoutingMessageClientId const client_id) const {
    if (client_manager_ != nullptr) {
      // VCA_SDCL_NULL_POINTER_CHECK_FOR_RECEIVE_HANDLER, VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
      client_manager_->HandleReceiveInitialFieldNotification(instance_id, std::move(packet), client_id);
    } else {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "client_manager_ not set, dropping message and aborting.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                                             // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

 private:
  /*!
   * \brief Type alias for SOME/IP message header
   */
  using SomeIpMessageHeader = amsr::someip_protocol::internal::SomeIpMessageHeader;

  /*!
   * \brief Receive and process a EventSubscriptionState update packet.
   * \param[in] body_view View of payload buffer.
   * \pre  body_view is not empty
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Deserialize the SOME/IP header from the passed message body view.
   * - If the deserialization is successful
   *   - Create event subscription state with the deserialization result.
   *     - Handle the update of the event subscription state from the client side.
   * \endinternal
   */
  void ReceiveServiceDiscoveryEventSubscriptionState(
      ::amsr::someip_protocol::internal::PacketBufferView const& body_view) const {
    // PTP-B-SomeipDaemonClient-ReceiveRoutingController_ReceiveServiceDiscoveryEventSubscriptionState
    // Deserialize EventSubscriptionState
    amsr::someipd_app_protocol::internal::Reader reader{body_view};
    amsr::core::Optional<someipd_app_protocol::internal::RoutingServiceDiscoveryEventSubscriptionStateMessagePayload>
        // VCA_SDCL_VALID_OPTIONAL_TYPE
        const payload{deserializer_.DeserializePayloadForRoutingServiceDiscoveryEventSubscriptionStateMessage(reader)};

    if (payload.has_value()) {
      // VCA_SDCL_VALID_OPTIONAL_TYPE
      amsr::someip_protocol::internal::EventSubscriptionState const event_subscription_state{
          payload->service_id, payload->instance_id, payload->major_version, payload->event_id,
          static_cast<amsr::someipd_app_protocol::internal::RoutingMessageSubscriptionState>(payload->state)};
      // Forwards the event details to the vector-specific ARA stack.
      // It is then translated into the type specified by ara::com.
      // VCA_SDCL_NULL_POINTER_CHECK_FOR_RECEIVE_HANDLER, VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
      client_manager_->HandleEventSubscriptionStateUpdate(event_subscription_state);
    }
    // PTP-E-SomeipDaemonClient-ReceiveRoutingController_ReceiveServiceDiscoveryEventSubscriptionState
  }

  /*!
   * \brief Receive and process a ServiceInstanceUp packet.
   * \param[in] body_view View of payload buffer.
   * \pre  body_view is not empty
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Deserialize the SOME/IP header from the passed message body view.
   * - If the deserialization is successful
   *   - Set the service instance from the deserialized data.
   *   - Handle the service instance from the client side.
   * - Otherwise, log an error that the received SOME/IP message is too small.
   * \endinternal
   */
  void ReceiveServiceDiscoveryServiceInstanceUp(
      ::amsr::someip_protocol::internal::PacketBufferView const& body_view) const {
    // PTP-B-SomeipDaemonClient-ReceiveRoutingController_ReceiveServiceDiscoveryServiceInstanceUp
    // Deserialize service instance container
    amsr::someipd_app_protocol::internal::Reader reader{body_view};
    amsr::core::Optional<
        someipd_app_protocol::internal::RoutingServiceDiscoveryServiceInstanceUpdateMessagePayload> const payload{
        amsr::someipd_app_protocol::internal::DeserMessageHeaders::
            DeserializePayloadForRoutingServiceDiscoveryServiceInstanceUpdateMessage(reader)};
    if (payload.has_value()) {
      amsr::someip_protocol::internal::ServiceInstance const service_instance{payload->service_id, payload->instance_id,
                                                                              payload->major_version};

      // Forward to client handler
      // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
      client_manager_->OnServiceInstanceUp(service_instance);
    } else {
      logger_.LogError(
          [received_size = reader.Size()](ara::log::LogStream& s) {
            s << "Deserialization of ServiceDiscoveryServiceInstanceUp message payload failed. Received size:"
              << received_size << ". Expected size: "
              << amsr::someipd_app_protocol::internal::kServiceDiscoveryEventSubscriptionStateMessageHeaderLength;
          },
          {static_cast<char const*>(__func__)}, {__LINE__});
    }
    // PTP-E-SomeipDaemonClient-ReceiveRoutingController_ReceiveServiceDiscoveryServiceInstanceUp
  }

  /*!
   * \brief Receive and process a ServiceInstanceDown packet.
   * \param[in] body_view View of payload buffer.
   * \pre  body_view is not empty
   * \context Reactor
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Deserialize the SOME/IP header from the passed message body view.
   * - If the deserialization is successful
   *   - Set the service instance from the deserialized data.
   *   - Handle the service instance from the client side.
   * - Otherwise, log an error that the received SOME/IP message is too small.
   * \endinternal
   */
  void ReceiveServiceDiscoveryServiceInstanceDown(
      ::amsr::someip_protocol::internal::PacketBufferView const& body_view) const {
    // PTP-B-SomeipDaemonClient-ReceiveRoutingController_ReceiveServiceDiscoveryServiceInstanceDown
    // Deserialize service instance container
    amsr::someipd_app_protocol::internal::Reader reader{body_view};

    amsr::core::Optional<
        someipd_app_protocol::internal::RoutingServiceDiscoveryServiceInstanceUpdateMessagePayload> const result{
        amsr::someipd_app_protocol::internal::DeserMessageHeaders::
            DeserializePayloadForRoutingServiceDiscoveryServiceInstanceUpdateMessage(reader)};

    if (result.has_value()) {
      amsr::someip_protocol::internal::ServiceInstance const service_instance{result->service_id, result->instance_id,
                                                                              result->major_version};

      // Forward to client handler
      // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
      client_manager_->OnServiceInstanceDown(service_instance);
    } else {
      logger_.LogError(
          [received_size = reader.Size()](ara::log::LogStream& s) {
            s << "Deserialization of ServiceDiscoveryServiceInstanceDown message payload failed. Received size:"
              << received_size << ". Expected size: "
              << amsr::someipd_app_protocol::internal::kServiceDiscoveryEventSubscriptionStateMessageHeaderLength;
          },
          {static_cast<char const*>(__func__)}, {__LINE__});
    }
    // PTP-E-SomeipDaemonClient-ReceiveRoutingController_ReceiveServiceDiscoveryServiceInstanceDown
  }

  /*!
   * \brief Logger.
   */
  // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
  amsr::someip_daemon_client::internal::logging::AraComLogger logger_{
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextId,
      // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
      amsr::someip_daemon_client::internal::logging::kSomeIpLoggerContextDescription,
      amsr::core::StringView{"ReceiveRoutingController"}};

  /*!
   * \brief A pointer to a receive handler for incoming SOME/IP messages and incoming Service Discovery messages on
   * Server side.
   */
  ServerInterface* server_manager_{nullptr};

  /*!
   * \brief A pointer to a receive handler for incoming SOME/IP messages and incoming Service Discovery messages on
   * Client side.
   */
  ClientInterface* client_manager_{nullptr};

  /*!
   * \brief An instance of the DeserMessageHeaders class.
   */
  amsr::someipd_app_protocol::internal::DeserMessageHeaders deserializer_{};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_RECEIVE_ROUTING_CONTROLLER_H_

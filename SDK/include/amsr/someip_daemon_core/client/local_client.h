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
/**        \file  local_client.h
 *        \brief  Used for all client-related communication.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_LOCAL_CLIENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_LOCAL_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara/core/map.h"
#include "ara/core/vector.h"

#include "amsr/someip_daemon_core/client/client_id_generator_interface.h"
#include "amsr/someip_daemon_core/client/event_handler_interface.h"
#include "amsr/someip_daemon_core/client/eventgroup/event_subscription_state.h"
#include "amsr/someip_daemon_core/client/local_client_interface.h"
#include "amsr/someip_daemon_core/client/method_response_handler_interface.h"
#include "amsr/someip_daemon_core/client/pdu_event_handler_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_manager_interface.h"
#include "amsr/someip_daemon_core/client/someip_event_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/types/local_client_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/logger_prefix_generator.h"
#include "amsr/someip_daemon_core/logging/logging_utilities.h"
#include "amsr/someip_daemon_core/packet_handler/validators/local_incoming_packet_validator_error.h"
#include "amsr/someip_daemon_core/packet_router/application_packet_router_interface.h"
#include "ara/core/result.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief LocalClient class used for all client-related communication.
 */
class LocalClient final : public LocalClientInterface, public MethodResponseHandlerInterface {
 private:
  /*!
   * \brief Alias for ClientId type.
   */
  using ClientId = configuration::types::LocalClientId;
  /*!
   * \brief Alias for EventContainer type.
   */
  using EventContainer = ara::core::Vector<configuration::types::SomeIpEventDeployment>;
  /*!
   * \brief Event observer.
   */
  using EventObserver = std::shared_ptr<EventHandlerInterface>;
  /*!
   * \brief SOME/IP event observer.
   */
  using SomeIpEventObserver = std::shared_ptr<SomeIpEventHandlerInterface>;
  /*!
   * \brief PDU event observer.
   */
  using PduEventObserver = std::shared_ptr<PduEventHandlerInterface>;
  /*!
   * \brief Map of SOME/IP event subscribers.
   */
  using SomeIpEventSubscriptionMap = ara::core::Map<someip_protocol::internal::EventId, SomeIpEventObserver>;
  /*!
   * \brief Map of PDU event subscribers.
   */
  using PduEventSubscriptionMap = ara::core::Map<someip_protocol::internal::EventId, PduEventObserver>;

 public:
  /*!
   * \brief Constructor of LocalClient.
   *
   * \param[in] service_deployment    The service deployment.
   * \param[in] local_client_id       The local client id.
   * \param[in] remote_server_manager Reference to the remote server manager.
   * \param[in] packet_router         The packet router.
   * \param[in] client_id_generator   Reference to the client Id generator.
   *
   * \context App
   * \steady  FALSE
   */
  LocalClient(configuration::types::SomeIpServiceInterfaceDeployment const& service_deployment,
              ClientId const& local_client_id, client::RemoteServerManagerInterface& remote_server_manager,
              std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router,
              ClientIdGeneratorInterface& client_id_generator) noexcept;

  /*!
   * \brief Define destructor.
   * \details Handle automatic unsubscription in case a proxy (client) is destroyed without unsubscribing to all its
   *          subscribed events.
   *
   * \context App, Shutdown
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~LocalClient() noexcept;

  LocalClient() = delete;
  LocalClient(LocalClient const&) = delete;
  LocalClient(LocalClient&&) = delete;
  LocalClient& operator=(LocalClient const&) & = delete;
  LocalClient& operator=(LocalClient&&) & = delete;

  /*!
   * \brief Register response handler.
   *
   * \param[in] response_handler    A pointer to the response handler.
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void RegisterResponseHandler(MethodResponseHandlerInterface* response_handler) noexcept override;

  /*!
   * \brief Subscribe to SOME/IP or PDU event.
   *
   * \param[in] event_id    A SOME/IP event identifier.
   * \param[in] observer    A pointer to the subscribed application.
   *
   * \return    The current event subscription state.
   *
   * \error     SomeIpDaemonErrc::event_id_not_found       If the event is not found in the configuration
   * \error     SomeIpDaemonErrc::eventgroup_id_not_found  If the event does not map to a required eventgroup
   * \error     SomeIpDaemonErrc::event_already_subscribed If the event has already been subscribed
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \trace SPEC-4981687
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ara::core::Result<void> SubscribeEvent(someip_protocol::internal::EventId const event_id,
                                         EventObserver const observer) noexcept override;

  /*!
   * \brief Unsubscribe to SOME/IP or PDU event.
   *
   * \param[in] event_id   A SOME/IP event identifier.
   * \param[in] observer   A pointer to the subscribed application.
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void UnsubscribeEvent(someip_protocol::internal::EventId const event_id,
                        EventObserver const observer) noexcept override;

  /*!
   * \brief       Forwards a local SOME/IP request message.
   * \param[in]   packet      A SOME/IP request message.
   *
   * \return void if the message was sent successfully, and error otherwise.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ServiceNotOffered
   * Runtime check failed: service is not offered.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kConfigurationError_UnknownMethod
   * Configuration check failed: method not configured.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable
   * Runtime check failed: connection is not established.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionTransmissionFailed
   * Runtime check failed: connection transmission failed.
   *
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ara::core::Result<void> SendMethodRequest(Packet& packet) const noexcept override;

  /*!
   * \brief Subscribe to SOME/IP event.
   *
   * \param[in] event_id    A SOME/IP event identifier.
   * \param[in] observer    A pointer to the subscribed application.
   *
   * \return    Empty ara::core::Result in case of successful subscription or an error.
   *
   * \error     SomeIpDaemonErrc::event_id_not_found       If the event is not found in the configuration
   *                                                       (or if it is not configured with serialization = SomeIp)
   * \error     SomeIpDaemonErrc::eventgroup_id_not_found  If the event does not map to a required eventgroup
   * \error     SomeIpDaemonErrc::event_already_subscribed If the event has already been subscribed
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ara::core::Result<void> SubscribeSomeIpEvent(someip_protocol::internal::EventId const event_id,
                                               SomeIpEventObserver const observer) noexcept override;

  /*!
   * \brief Unsubscribe to SOME/IP event.
   *
   * \param[in] event_id   A SOME/IP event identifier.
   * \param[in] observer   A pointer to the subscribed application.
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void UnsubscribeSomeIpEvent(someip_protocol::internal::EventId const event_id,
                              SomeIpEventObserver const observer) noexcept override;

  /*!
   * \brief Subscribe to PDU event.
   *
   * \param[in] event_id    A PDU event identifier.
   * \param[in] observer    A pointer to the subscribed application.
   *
   * \return    Empty ara::core::Result in case of successful subscription or an error.
   *
   * \error     SomeIpDaemonErrc::event_id_not_found       If the event is not found in the configuration
   *                                                       (or if it is not configured with serialization = SignalBased)
   * \error     SomeIpDaemonErrc::eventgroup_id_not_found  If the event does not map to a required eventgroup
   * \error     SomeIpDaemonErrc::event_already_subscribed If the event has already been subscribed
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ara::core::Result<void> SubscribePduEvent(someip_protocol::internal::EventId const event_id,
                                            PduEventObserver const observer) noexcept override;

  /*!
   * \brief Unsubscribe to PDU event.
   *
   * \param[in] event_id   A PDU event identifier.
   * \param[in] observer   A pointer to the subscribed application.
   *
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void UnsubscribePduEvent(someip_protocol::internal::EventId const event_id,
                           PduEventObserver const observer) noexcept override;

  /*!
   * \brief Get the clint Id.
   *
   * \return    The client Id.
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  someip_protocol::internal::ClientId GetClientId() const noexcept override;

 private:
  /*!
   * \brief Forward method response to the registered handler.
   *
   * \param[in] instance_id   SOME/IP instance id.
   * \param[in] packet        SOME/IP request message.
   *
   * \return    -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  void OnMethodResponse(someip_protocol::internal::InstanceId const instance_id,
                        client::Packet packet) noexcept override;
  /*!
   * \brief Unique identifier for the local client.
   */
  ClientId const local_client_id_;

  /*!
   * \brief A reference to the remote server manager.
   * \details This is needed to request and release our RemoteServer upon creation and destruction of this object.
   *          The lifecycle of the RemoteServerManager is guaranteed to be longer than the lifecycle of any instance
   *          of this object
   */
  client::RemoteServerManagerInterface& remote_server_manager_;

  /*!
   * \brief A shared pointer to the remote server.
   */
  std::shared_ptr<client::RemoteServerInterface> remote_server_;

  /*!
   * \brief A reference to the application packet router to forward SOME/IP messages to.
   */
  std::shared_ptr<packet_router::ApplicationPacketRouterInterface> packet_router_;

  /*!
   * \brief A reference to the client Id generator.
   */
  ClientIdGeneratorInterface& client_id_generator_;

  /*!
   * \brief Method Response handler.
   */
  MethodResponseHandlerInterface* response_handler_{nullptr};

  /*!
   * \brief List of SOME/IP event subscribers.
   * \details Register the events' observers to avoid double subscription.
   *          Handle automatic unsubscription in case a proxy (client) is destroyed without unsubscribing to all its
   *          subscribed events.
   */
  SomeIpEventSubscriptionMap someip_event_subscriptions_{};

  /*!
   * \brief List of PDU event subscribers.
   * \details Register the events' observers to avoid double subscription.
   *          Handle automatic unsubscription in case a proxy (client) is destroyed without unsubscribing to all its
   *          subscribed events.
   */
  PduEventSubscriptionMap pdu_event_subscriptions_{};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "LocalClient",
          configuration::types::SomeIpServiceInterfaceDeploymentId{
              local_client_id_.service_instance_id.deployment_id.service_interface_id,
              configuration::types::SomeIpServiceVersion{
                  local_client_id_.service_instance_id.deployment_id.service_interface_version.major_version,
                  local_client_id_.service_instance_id.deployment_id.service_interface_version.minor_version}},
          local_client_id_.service_instance_id.instance_id, local_client_id_.client_id)};
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_LOCAL_CLIENT_H_

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
/*!        \file  someip_daemon_client/internal/proxy_someip_daemon_client.h
 *        \brief  Access library to SOME/IP daemon functionality
 *
 *      \details  This file provides an API for proxy communication with the SOME/IP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_PROXY_SOMEIP_DAEMON_CLIENT_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_PROXY_SOMEIP_DAEMON_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "someip_daemon_client/internal/client_interface.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someipd_app_protocol/internal/protocol_types.h"

/*!
 * \brief Literals namespace is needed for "_sv literal.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-M7.3.6_using-directives_in_header_file
using vac::container::literals::operator""_sv;

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief ProxySomeIpDaemonClient API providing access to the SOME/IP daemon functionality.
 * \details An IPC communication channel is used for communication with the SOME/IP daemon.
 * \tparam TemplateConfiguration The configuration to set the type of ipc connection wrapper, message receiver, message
 * sender, receive routing controller, someip packet sender, request service
 * controller, release service controller, start service discovery controller, stop service discovery controller,
 * subscribe event controller and unsubscribe event controller. Contains the following required type definition:
 *         - typename TemplateConfiguration::IpcConnectionWrapperType
 *         - typename TemplateConfiguration::MessageReceiverType
 *         - typename TemplateConfiguration::MessageSenderType
 *         - typename TemplateConfiguration::RoutingControllerType
 *         - typename TemplateConfiguration::SomeIpPacketSenderType
 *         - typename TemplateConfiguration::RequestServiceControllerType
 *         - typename TemplateConfiguration::ReleaseServiceControllerType
 *         - typename TemplateConfiguration::StartServiceDiscoveryControllerType
 *         - typename TemplateConfiguration::StopServiceDiscoveryControllerType
 *         - typename TemplateConfiguration::SubscribeEventControllerType
 *         - typename TemplateConfiguration::UnsubscribeEventControllerType
 */
template <typename TemplateConfiguration>
class ProxySomeIpDaemonClient {
 public:
  /*!
   * \brief Type alias for IpcConnectionWrapper.
   */
  using IpcConnectionWrapper = typename TemplateConfiguration::IpcConnectionWrapperType;

  /*!
   * \brief Type alias for MessageReceiver.
   */
  using MessageReceiver = typename TemplateConfiguration::MessageReceiverType;

  /*!
   * \brief Type alias for MessageSender.
   */
  using MessageSender = typename TemplateConfiguration::MessageSenderType;

  /*!
   * \brief Type alias for Routing Controller.
   */
  using ReceiveRoutingController = typename TemplateConfiguration::RoutingControllerType;

  /*!
   * \brief Type alias for SomeIpPacketSender.
   */
  using SomeIpPacketSender = typename TemplateConfiguration::SomeIpPacketSenderType;

  /*!
   * \brief Type alias for Request Service Controller.
   */
  using RequestServiceController = typename TemplateConfiguration::RequestServiceControllerType;

  /*!
   * \brief Type alias for Release Service Controller.
   */
  using ReleaseServiceController = typename TemplateConfiguration::ReleaseServiceControllerType;

  /*!
   * \brief Type alias for Start Service Discovery Controller.
   */
  using StartServiceDiscoveryController = typename TemplateConfiguration::StartServiceDiscoveryControllerType;

  /*!
   * \brief Type alias for Stop ServiceDiscovery Controller.
   */
  using StopServiceDiscoveryController = typename TemplateConfiguration::StopServiceDiscoveryControllerType;

  /*!
   * \brief Type alias for Subscribe Event Controller.
   */
  using SubscribeEventController = typename TemplateConfiguration::SubscribeEventControllerType;

  /*!
   * \brief Type alias for Unsubscribe Event Controller.
   */
  using UnsubscribeEventController = typename TemplateConfiguration::UnsubscribeEventControllerType;

  /*!
   * \brief Type alias for control message return code.
   */
  using ControlMessageReturnCode = amsr::someipd_app_protocol::internal::ControlMessageReturnCode;

  /*!
   * \brief Type alias for control message client ID.
   */
  using ClientId = amsr::someipd_app_protocol::internal::ControlMessageClientId;

  ProxySomeIpDaemonClient() = delete;

  ProxySomeIpDaemonClient(ProxySomeIpDaemonClient const&) = delete;

  ProxySomeIpDaemonClient(ProxySomeIpDaemonClient&&) = delete;

  auto operator=(ProxySomeIpDaemonClient const&) & -> ProxySomeIpDaemonClient& = delete;

  auto operator=(ProxySomeIpDaemonClient&&) & -> ProxySomeIpDaemonClient& = delete;

  /*!
   * \brief Constructor of ProxySomeIpDaemonClient.
   * \param[in] ipc_connection_wrapper     IpcConnectionWrapper used to register callbacks for the controllers.
   * \param[in] message_receiver           MessageReceiver used to register control handlers for each controller.
   * \param[in] message_sender             MessageSender used to construct the controllers.
   * \param[in] receive_routing_controller ReceiveRoutingController handling routing messages.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  ProxySomeIpDaemonClient(IpcConnectionWrapper& ipc_connection_wrapper, MessageReceiver& message_receiver,
                          MessageSender& message_sender, ReceiveRoutingController* receive_routing_controller)
      : ipc_connection_wrapper_{ipc_connection_wrapper},
        message_receiver_{message_receiver},
        message_sender_{message_sender},
        receive_routing_controller_{receive_routing_controller},
        // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
        logger_{logging::kSomeIpLoggerContextId, logging::kSomeIpLoggerContextDescription,
                // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
                amsr::core::StringView{"ProxySomeIpDaemonClient"}},
        someip_packet_sender_{message_sender_},                // VCA_SDCL_FUNCTION_NO_PRECONDITION
        request_service_controller_{message_sender_},          // VCA_SDCL_FUNCTION_NO_PRECONDITION
        release_service_controller_{message_sender_},          // VCA_SDCL_FUNCTION_NO_PRECONDITION
        start_service_discovery_controller_{message_sender_},  // VCA_SDCL_FUNCTION_NO_PRECONDITION
        stop_service_discovery_controller_{message_sender_},   // VCA_SDCL_FUNCTION_NO_PRECONDITION
        subscribe_event_controller_{message_sender_},          // VCA_SDCL_FUNCTION_NO_PRECONDITION
        unsubscribe_event_controller_{message_sender_}         // VCA_SDCL_FUNCTION_NO_PRECONDITION
  {
    // VCA_SDCL_FUNCTION_NO_PRECONDITION
    message_receiver_.RegisterControllerHandle(someipd_app_protocol::internal::MessageType::kRequestService,
                                               &request_service_controller_);

    // clang-format off
    // VCA_SDCL_UNIQUE_FUNCTION
    vac::language::UniqueFunction<void()> setup_request_service_controller_promise{ [this]() { request_service_controller_.SetUpPromise(); }};  // COV_SOMEIPDAEMON_CLIENT_LAMBDA  // NOLINT(whitespace/line_length)
    // clang-format on
    // VCA_SDCL_UNIQUE_FUNCTION
    ipc_connection_wrapper_.RegisterRequestServiceOnDisconnectCallback(
        std::move(setup_request_service_controller_promise));
  }

  /*!
   * \brief Destructor.
   *
   * \pre       The related reactor thread must be stopped.
   * \context   Shutdown
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  // VCA_SDCL_SLC23_DEFAULT_DTOR_MOLE_1298
  virtual ~ProxySomeIpDaemonClient() noexcept = default;

  // ---- Routing channel API --------------------------------------------------------------------------------------

  /*!
   * \copydoc amsr::someip_daemon_client::internal::ReceiveRoutingController::SetClientManager
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  void SetClientManager(ClientInterface* client_manager) noexcept {
    if ((receive_routing_controller_ != nullptr) &&  // COV_SOMEIPDAEMON_CLIENT_RECEIVING_ROUTING_CONTROLLER_ALWAYS_TRUE
        (client_manager != nullptr)) {
      // VCA_SDCL_MEMBER_OBJECT_IS_VALID, VCA_SDCL_CLIENT_MANAGER_PARAMETER_IS_VALID
      receive_routing_controller_->SetClientManager(client_manager);
    }
  }

  // ---- Controller objects --------------------------------------------------------------------------------------

  /*!
   * \brief  Someip Packet sender for IPC communication with SOME/IP daemon.
   */
  SomeIpPacketSender* const SendSomeIpMessage{&someip_packet_sender_};

  /*!
   * \brief RequestServiceController handling request service command message.
   */
  RequestServiceController* const RequestService{&request_service_controller_};

  /*!
   * \brief ReleaseServiceController handling release service command message.
   */
  ReleaseServiceController* const ReleaseService{&release_service_controller_};

  /*!
   * \brief StartServiceDiscoveryController handling start service discovery command message.
   */
  StartServiceDiscoveryController* const StartServiceDiscovery{&start_service_discovery_controller_};

  /*!
   * \brief StopServiceDiscoveryController handling stop service discovery command message.
   */
  StopServiceDiscoveryController* const StopServiceDiscovery{&stop_service_discovery_controller_};

  /*!
   * \brief SubscribeEventController handling subscribe event command message.
   */
  SubscribeEventController* const SubscribeEvent{&subscribe_event_controller_};

  /*!
   * \brief UnsubscribeEventController handling unsubscribe event command message.
   */
  UnsubscribeEventController* const UnsubscribeEvent{&unsubscribe_event_controller_};

 protected:
  /*!
   * \brief Get a reference for the receive routing controller.
   * \details This function is used for testing.
   * \return Reference for the routing controller.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetReceiveRoutingController() const noexcept -> ReceiveRoutingController& {
    return *receive_routing_controller_;
  }

 private:
  /*!
   * \brief IpcConnectionWrapper used to register callbacks for the controllers.
   */
  IpcConnectionWrapper& ipc_connection_wrapper_;

  /*!
   * \brief MessageReceiver used to register control handlers for each controller.
   */
  MessageReceiver& message_receiver_;

  /*!
   * \brief MessageSender used to construct the controllers.
   */
  MessageSender& message_sender_;

  /*!
   * \brief ReceiveRoutingController handling routing messages.
   */
  ReceiveRoutingController* receive_routing_controller_;

  /*!
   * \brief Logger.
   */
  logging::AraComLogger logger_;

  /*!
   * \brief  Someip Packet sender for IPC communication with SOME/IP daemon.
   */
  SomeIpPacketSender someip_packet_sender_;

  /*!
   * \brief RequestServiceController handling request service command message.
   */
  RequestServiceController request_service_controller_;

  /*!
   * \brief ReleaseServiceController handling release service command message.
   */
  ReleaseServiceController release_service_controller_;

  /*!
   * \brief StartServiceDiscoveryController handling start service discovery command message.
   */
  StartServiceDiscoveryController start_service_discovery_controller_;

  /*!
   * \brief StopServiceDiscoveryController handling stop service discovery command message.
   */
  StopServiceDiscoveryController stop_service_discovery_controller_;

  /*!
   * \brief SubscribeEventController handling subscribe event command message.
   */
  SubscribeEventController subscribe_event_controller_;

  /*!
   * \brief UnsubscribeEventController handling unsubscribe event command message.
   */
  UnsubscribeEventController unsubscribe_event_controller_;
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_SOMEIPDAEMON_CLIENT_LAMBDA
//   \ACCEPT XX
//   \REASON The callback functions are called and covered using SomeipdaemonclientMock class hence real object for
//           Someipdaemonclient not created.
//
// \ID COV_SOMEIPDAEMON_CLIENT_RECEIVING_ROUTING_CONTROLLER_ALWAYS_TRUE
//   \ACCEPT TF tx tf
//   \REASON The condition checks the receiving_reouting_controller is not a nullptr. The check is needed to ensure that
//           the nullptr is not accessed, but it cannot be covered as it is created as a member of the
//           Someipdaemonclient class, which will never be a nullptr.
//
// COV_JUSTIFICATION_END

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_PROXY_SOMEIP_DAEMON_CLIENT_H_

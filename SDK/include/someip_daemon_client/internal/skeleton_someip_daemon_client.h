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
/*!        \file  someip_daemon_client/internal/skeleton_someip_daemon_client.h
 *        \brief  Access library to SOME/IP daemon functionality
 *      \details  This file provides an API for skeleton communication with the SOME/IP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SKELETON_SOMEIP_DAEMON_CLIENT_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SKELETON_SOMEIP_DAEMON_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/server_interface.h"
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
 * \brief SkeletonSomeIpDaemonClient API providing access to the SOME/IP daemon functionality.
 * \details An IPC communication channel is used for communication with the SOME/IP daemon.
 * \tparam TemplateConfiguration The configuration to set the type of ipc connection wrapper, message receiver, message
 * sender, receive routing controller, someip packet sender, pdu packet sender, offer service, stop offer
 * service, request local server and release local server controllers. Contains the following required type definition:
 *         - typename TemplateConfiguration::IpcConnectionWrapperType
 *         - typename TemplateConfiguration::MessageReceiverType
 *         - typename TemplateConfiguration::MessageSenderType
 *         - typename TemplateConfiguration::RoutingControllerType
 *         - typename TemplateConfiguration::SomeIpPacketSenderType
 *         - typename TemplateConfiguration::PduPacketSenderType
 *         - typename TemplateConfiguration::RequestLocalServerControllerType
 *         - typename TemplateConfiguration::ReleaseLocalServerControllerType
 *         - typename TemplateConfiguration::OfferServiceControllerType
 *         - typename TemplateConfiguration::StopOfferServiceControllerType
 */
template <typename TemplateConfiguration>
class SkeletonSomeIpDaemonClient {
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
   * \brief Type alias for PduPacketSender.
   */
  using PduPacketSender = typename TemplateConfiguration::PduPacketSenderType;

  /*!
   * \brief Type alias for Request Local Server Controller.
   */
  using RequestLocalServerController = typename TemplateConfiguration::RequestLocalServerControllerType;

  /*!
   * \brief Type alias for Release Local Server Controller.
   */
  using ReleaseLocalServerController = typename TemplateConfiguration::ReleaseLocalServerControllerType;

  /*!
   * \brief Type alias for Offer Service Controller.
   */
  using OfferServiceController = typename TemplateConfiguration::OfferServiceControllerType;

  /*!
   * \brief Type alias for Stop Offer Service Controller.
   */
  using StopOfferServiceController = typename TemplateConfiguration::StopOfferServiceControllerType;

  SkeletonSomeIpDaemonClient() = delete;
  SkeletonSomeIpDaemonClient(SkeletonSomeIpDaemonClient const&) = delete;
  SkeletonSomeIpDaemonClient(SkeletonSomeIpDaemonClient&&) = delete;
  auto operator=(SkeletonSomeIpDaemonClient const&) & -> SkeletonSomeIpDaemonClient& = delete;
  auto operator=(SkeletonSomeIpDaemonClient&&) & -> SkeletonSomeIpDaemonClient& = delete;

  /*!
   * \brief Constructor of SkeletonSomeIpDaemonClient.
   * \param[in] ipc_connection_wrapper     IpcConnectionWrapper used to register callbacks for the controllers.
   * \param[in] message_receiver           MessageReceiver used to register control handlers for each controller.
   * \param[in] message_sender             MessageSender used to construct the controllers.
   * \param[in] receive_routing_controller ReceiveRoutingController handling routing messages.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  SkeletonSomeIpDaemonClient(IpcConnectionWrapper& ipc_connection_wrapper, MessageReceiver& message_receiver,
                             MessageSender& message_sender, ReceiveRoutingController* receive_routing_controller)
      : ipc_connection_wrapper_{ipc_connection_wrapper},
        message_receiver_{message_receiver},
        message_sender_{message_sender},
        receive_routing_controller_{receive_routing_controller},
        // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
        logger_{logging::kSomeIpLoggerContextId, logging::kSomeIpLoggerContextDescription,
                // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
                amsr::core::StringView{"SkeletonSomeIpDaemonClient"}},
        someip_packet_sender_{message_sender_},             // VCA_SDCL_FUNCTION_NO_PRECONDITION
        pdu_packet_sender_{message_sender_},                // VCA_SDCL_FUNCTION_NO_PRECONDITION
        request_local_server_controller_{message_sender_},  // VCA_SDCL_FUNCTION_NO_PRECONDITION
        release_local_server_controller_{message_sender_},  // VCA_SDCL_FUNCTION_NO_PRECONDITION
        offer_service_controller_{message_sender_},         // VCA_SDCL_FUNCTION_NO_PRECONDITION
        stop_offer_service_controller_{message_sender_}     // VCA_SDCL_FUNCTION_NO_PRECONDITION
  {
    // VCA_SDCL_FUNCTION_NO_PRECONDITION
    message_receiver_.RegisterControllerHandle(someipd_app_protocol::internal::MessageType::kRequestLocalServer,
                                               &request_local_server_controller_);
    // clang-format off
    // VCA_SDCL_UNIQUE_FUNCTION
    vac::language::UniqueFunction<void()> setup_request_local_server_controller_promise{ [this]() { request_local_server_controller_.SetUpPromise(); }};  // COV_SOMEIPDAEMON_CLIENT_LAMBDA  // NOLINT(whitespace/line_length)
    // clang-format on
    // VCA_SDCL_UNIQUE_FUNCTION
    ipc_connection_wrapper_.RegisterRequestLocalServerOnDisconnectCallback(
        std::move(setup_request_local_server_controller_promise));
  }

  /*!
   * \brief Destructor.
   *
   * \pre       The related reactor thread must be stopped.
   * \context   Shutdown
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \internal
   * - Disconnect from the SOME/IP daemon.
   * \endinternal
   */
  // VCA_SDCL_SLC23_DEFAULT_DTOR_MOLE_1298
  virtual ~SkeletonSomeIpDaemonClient() noexcept = default;

  // ---- Client / Server management API ---------------------------------------------------------------------------

  /*!
   * \copydoc amsr::someip_daemon_client::internal::ReceiveRoutingController::SetServerManager
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetServerManager(ServerInterface* server_manager) noexcept {
    if ((receive_routing_controller_ != nullptr) &&  // COV_SOMEIPDAEMON_CLIENT_RECEIVING_ROUTING_CONTROLLER_ALWAYS_TRUE
        (server_manager != nullptr)) {
      // VCA_SDCL_MEMBER_OBJECT_IS_VALID, VCA_SDCL_CLIENT_MANAGER_PARAMETER_IS_VALID
      receive_routing_controller_->SetServerManager(server_manager);
    }
  }

  // ---- Controller objects --------------------------------------------------------------------------------------

  /*!
   * \brief  SomeIp Packet sender handling the sending of someip messages.
   */
  SomeIpPacketSender* const SendSomeIpMessage{&someip_packet_sender_};

  /*!
   * \brief  Pdu Packet sender for PDU communication with SOME/IP daemon.
   */
  PduPacketSender* const SendPduMessage{&pdu_packet_sender_};

  /*!
   * \brief RequestLocalServerController handling request local server command message.
   */
  RequestLocalServerController* const RequestLocalServer{&request_local_server_controller_};

  /*!
   * \brief ReleaseLocalServerController handling release local server command message.
   */
  ReleaseLocalServerController* const ReleaseLocalServer{&release_local_server_controller_};

  /*!
   * \brief OfferServiceController handling offer service command message.
   */
  OfferServiceController* const OfferService{&offer_service_controller_};

  /*!
   * \brief StopOfferServiceController handling stop offer service command message.
   */
  StopOfferServiceController* const StopOfferService{&stop_offer_service_controller_};

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
   * \brief A class responsible for handling the sending of IPC messages
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
   * \brief  SomeIp Packet sender handling the sending of someip messages.
   */
  SomeIpPacketSender someip_packet_sender_;

  /*!
   * \brief  Someip Packet sender for IPC communication with SOME/IP daemon.
   */
  PduPacketSender pdu_packet_sender_;

  /*!
   * \brief RequestLocalServerController handling request local server command message.
   */
  RequestLocalServerController request_local_server_controller_;

  /*!
   * \brief ReleaseLocalServerController handling release local server command message.
   */
  ReleaseLocalServerController release_local_server_controller_;

  /*!
   * \brief OfferServiceController handling offer service command message.
   */
  OfferServiceController offer_service_controller_;

  /*!
   * \brief StopOfferServiceController handling stop offer service command message.
   */
  StopOfferServiceController stop_offer_service_controller_;
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

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SKELETON_SOMEIP_DAEMON_CLIENT_H_

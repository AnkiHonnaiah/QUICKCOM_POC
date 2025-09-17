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
/**        \file  packet_router.h
 *        \brief  Packet router
 *
 *      \details  The packet router is responsible for forwarding SOME/IP method requests, method responses, and events
 *                to corresponding service consumers or providers.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_PACKET_ROUTER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_PACKET_ROUTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/client/method_response_handler_interface.h"
#include "amsr/someip_daemon_core/client/remote_server.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/packet_router/application_packet_router_interface.h"
#include "amsr/someip_daemon_core/packet_router/client_event_packet_router.h"
#include "amsr/someip_daemon_core/packet_router/client_method_response_packet_router.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "amsr/someip_daemon_core/packet_router/server_method_packet_router.h"
#include "amsr/steady_timer/timer_manager_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief PacketRouter.
 */
class PacketRouter : public ApplicationPacketRouterInterface, public NetworkPacketRouterInterface {
 public:
  /*!
   * \brief     Default destructor.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~PacketRouter() override = default;

  PacketRouter() = delete;
  PacketRouter(PacketRouter const&) = delete;
  PacketRouter(PacketRouter&&) = delete;
  PacketRouter& operator=(PacketRouter const&) & = delete;
  PacketRouter& operator=(PacketRouter&&) & = delete;

  /*!
   * \brief     Constructs the PacketRouter.
   * \param[in] config                  Const pointer to a configuration.
   * \param[in] timer_manager           Timer manager, used for creation of overload protection schedulers
   * \param[in] identity_access_manager IAM Interface, used for access checks for incoming remote messages.
   * \param[in] statistics_handler      The statistics handler
   * \param[in] someip_message_creator  SomeIp Message Creator.
   * \pre       Pointer to configuration shall not be nullptr.
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  PacketRouter(configuration::Configuration const* config, amsr::steady_timer::TimerManagerInterface* timer_manager,
               iam::IamInterface& identity_access_manager,
               someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
               someip_message_creator::SomeIpMessageCreatorInterface const& someip_message_creator);
  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::ApplicationPacketRouterInterface::AddRequestProviderRoute
   * \steady  FALSE
   */
  void AddRequestProviderRoute(SomeIpServiceInstanceIdCommunication const& service_instance_id,
                               server::MethodRequestHandler& request_handler) override;

  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::ApplicationPacketRouterInterface::AddResponseConsumerRoute
   * \steady  FALSE
   */
  void AddResponseConsumerRoute(SomeIpServiceInstanceIdCommunication const& service_instance_id,
                                someip_protocol::internal::ClientId const client_id,
                                client::MethodResponseHandlerInterface& to) override;

  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::ApplicationPacketRouterInterface::DeleteResponseConsumerRoute
   * \steady  FALSE
   */
  void DeleteResponseConsumerRoute(SomeIpServiceInstanceIdCommunication const& service_instance_id,
                                   someip_protocol::internal::ClientId const client_id) override;
  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::ApplicationPacketRouterInterface::DeleteRequestProviderRoute
   * \steady  FALSE
   */
  void DeleteRequestProviderRoute(SomeIpServiceInstanceIdCommunication const& service_instance_id) override;

  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::ApplicationPacketRouterInterface::Forward
   * \steady  TRUE
   */
  void Forward(someip_protocol::internal::InstanceId const instance_id,
               std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet) override;

  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::NetworkPacketRouterInterface::OnPacketReceived
   * \steady  TRUE
   */
  void OnPacketReceived(someip_protocol::internal::InstanceId const instance_id,
                        std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet,
                        RemotePacketSinkSharedPtr reply_to, RemoteSourceIdentifier const& source_id) override;

  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::NetworkPacketRouterInterface::OnPduPacketReceived
   * \steady  TRUE
   */
  void OnPduPacketReceived(someip_protocol::internal::InstanceId const instance_id,
                           std::shared_ptr<someip_protocol::internal::PduMessage> const& packet,
                           RemoteSourceIdentifier const& source_id) override;

  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::ApplicationPacketRouterInterface::RegisterRemoteServer
   * \steady  FALSE
   */
  void RegisterRemoteServer(SomeIpServiceInstanceIdCommunication const& instance_id_communication,
                            client::RemoteServerInterface& remote_server) noexcept override;

  /*!
   * \copydoc amsr::someip_daemon_core::packet_router::ApplicationPacketRouterInterface::UnregisterRemoteServer
   * \steady  FALSE
   */
  void UnregisterRemoteServer(SomeIpServiceInstanceIdCommunication const& instance_id_communication) noexcept override;

 private:
  /*!
   * \brief Ara com logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kPacketRouterLoggerContextId,
                                                    someip_daemon_core::logging::kPacketRouterLoggerContextDescription,
                                                    ara::core::StringView{"PacketRouter"}};

  /*!
   * \brief   Server method packet router
   * \details Handles reception of method requests and transmission of method responses
   */
  ServerMethodPacketRouter server_method_packet_router_;

  /*!
   * \brief   Client method packet router
   * \details Handles reception of method responses
   */
  ClientMethodResponsePacketRouter client_method_response_packet_router_;

  /*!
   * \brief   Client event packet router
   * \details Handles reception of events
   */
  ClientEventPacketRouter client_event_packet_router_;
};

}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_PACKET_ROUTER_H_

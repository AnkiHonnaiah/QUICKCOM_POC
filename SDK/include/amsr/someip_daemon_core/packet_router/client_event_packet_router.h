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
/**        \file  client_event_packet_router.h
 *        \brief  Packet router
 *
 *      \details  The client event packet router is responsible for receiving SOME/IP events and forwarding them to
 *                the corresponding subscribers.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_CLIENT_EVENT_PACKET_ROUTER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_CLIENT_EVENT_PACKET_ROUTER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include <utility>
#include "ara/core/map.h"
#include "ara/core/vector.h"

#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/client/datatypes/client_pdu_event.h"
#include "amsr/someip_daemon_core/client/datatypes/client_someip_event.h"
#include "amsr/someip_daemon_core/client/event_handler_interface.h"
#include "amsr/someip_daemon_core/client/remote_server_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/packet_handler/remote_event_packet_validator.h"
#include "amsr/someip_daemon_core/packet_router/filters/overload_protection_scheduler.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {

/*!
 * \brief ClientEventPacketRouter.
 */
class ClientEventPacketRouter {
 public:
  /*!
   * \brief Represents a SOME/IP message.
   */
  using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief Represents a PDU message.
   */
  using PduPacket = std::shared_ptr<someip_protocol::internal::PduMessage>;
  /*!
   * \brief     Default destructor.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~ClientEventPacketRouter() = default;

  ClientEventPacketRouter() = delete;
  ClientEventPacketRouter(ClientEventPacketRouter const&) = delete;
  ClientEventPacketRouter(ClientEventPacketRouter&&) = delete;
  ClientEventPacketRouter& operator=(ClientEventPacketRouter const&) & = delete;
  ClientEventPacketRouter& operator=(ClientEventPacketRouter&&) & = delete;

  /*!
   * \brief     Construct the ClientEventPacketRouter.
   * \param[in] config                  Const pointer to a configuration.
   * \param[in] timer_manager           Timer manager used for creation of overload protection schedulers.
   * \param[in] statistics_handler      The statistics handler.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  ClientEventPacketRouter(
      configuration::Configuration const* config, amsr::steady_timer::TimerManagerInterface* timer_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler);

  /*!
   * \brief       Handles the reception of a remote SOME/IP event.
   * \param[in]   service_instance_id SOME/IP service instance id.
   * \param[in]   packet              A SOME/IP event message.
   * \param[in]   source_id           Identifier of the remote packet source.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace SPEC-5951451
   */
  void OnEventReceived(someip_protocol::internal::InstanceId const instance_id, Packet const& packet,
                       RemoteSourceIdentifier const& source_id);

  /*!
   * \brief       Handles the reception of a remote PDU event.
   * \param[in]   service_instance_id SOME/IP instance id.
   * \param[in]   packet              A PDU event message.
   * \param[in]   source_id           Identifier of the remote packet source.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void OnPduEventReceived(someip_protocol::internal::InstanceId const instance_id, PduPacket const& packet,
                          RemoteSourceIdentifier const& source_id);

  /*!
   * \brief       Register a remote server.
   * \param[in]   instance_id_communication The service instance id to identify the server
   * \param[in]   event_handler             Remote server that will handle the events.
   *
   * \details     Every remote server must be registered once during its initialization.
   * \pre         The remote server can not be already registered with the same instance id. The API must be
   *              called only once per instance id.
   *
   * \context     Init, Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RegisterRemoteServer(configuration::types::SomeIpServiceInstanceIdCommunication const& instance_id_communication,
                            client::RemoteServerInterface& remote_server) noexcept;

  /*!
   * \brief       Unregister a remote server.
   * \param[in]   instance_id_communication The service instance id to identify the server
   *
   * \details     Every remote server must be unregistered once during its deinitialization.
   * \pre         The remote server must be registered before unregistration. The API must be
   *              called only once per instance id.
   *
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void UnregisterRemoteServer(
      configuration::types::SomeIpServiceInstanceIdCommunication const& instance_id_communication) noexcept;

 protected:
  /*!
   * \brief Type alias for SomeIp event routing table.
   */
  using RemoteServerRoutingTable =
      ara::core::Map<configuration::types::SomeIpServiceInstanceIdCommunication, client::RemoteServerInterface&>;

  /*!
   * \brief Overload protection scheduler table
   */
  using OverloadProtectionSchedulerTable =
      ara::core::Map<configuration::Configuration::Event const* const, filters::OverloadProtectionScheduler>;

  /*!
   * \brief Type alias for event count.
   */
  using NotificationCount = std::size_t;

  /*!
   * \brief Type alias for delayed events statistics map.
   */
  using EventDelayedStatisticsMap = ara::core::Map<configuration::Configuration::Event const*, NotificationCount>;

  /*!
   * \brief Type alias for event.
   */
  using Event = configuration::ConfigurationTypesAndDefs::Event;

  /*!
   * \brief       Initializes the overload protection feature.
   * \details     Creates all overload protection schedulers during initialization.
   * \param[in]   timer_manager  Timer manager, used for creation of overload protection schedulers
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void InitializeOverloadProtection(amsr::steady_timer::TimerManagerInterface* timer_manager);

  /*!
   * \brief         Forwards a remote SOME/IP event message configured to use the overload protection feature.
   * \details       The event may be forwarded immediately to the application, scheduled for later transmission or
   *                dropped, depending on the status of the overload protection.
   * \param[in,out] scheduler   The overload protection scheduler assigned to the received event.
   * \param[in]     instance_id SOME/IP instance id.
   * \param[in]     packet      A SOME/IP event message.
   * \param[in]     event_ptr   Configuration of the received event type.
   * \pre           -
   * \context       Network
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   */
  void ForwardEventOverloadProtection(filters::OverloadProtectionScheduler& scheduler,
                                      someip_protocol::internal::InstanceId const instance_id, Packet packet,
                                      configuration::Configuration::Event const* const event_ptr);

  /*!
   * \brief         Forwards a remote PDU event message configured to use the overload protection feature.
   * \details       The event may be forwarded immediately to the application, scheduled for later transmission or
   *                dropped, depending on the status of the overload protection.
   * \param[in,out] scheduler   The overload protection scheduler assigned to the received event.
   * \param[in]     instance_id SOME/IP instance id.
   * \param[in]     packet      A PDU event message.
   * \param[in]     event_ptr   Configuration of the received event type.
   * \pre           -
   * \context       Network
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   */
  void ForwardPduEventOverloadProtection(filters::OverloadProtectionScheduler& scheduler,
                                         someip_protocol::internal::InstanceId const instance_id, PduPacket packet,
                                         configuration::Configuration::Event const* const event_ptr);

  /*!
   * \brief       Forwards a remote SOME/IP event message to a local event subscriber.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   packet A SOME/IP event message.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       SPEC-5951451
   * \steady      TRUE
   */
  void ForwardEvent(someip_protocol::internal::InstanceId const instance_id, Packet const& packet) const noexcept;

  /*!
   * \brief       Forwards a remote PDU event message to a local event subscriber.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   packet A PDU event message.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void ForwardPduEvent(someip_protocol::internal::InstanceId const instance_id, PduPacket const& packet) const noexcept;

  /*!
   * \brief     Update the statistics of the given event for a notification due to overload protection.
   * \details   The statistics refer to events of the same type that have not been forwarded immediately. The statistics
   *            don't hold information about whether those events were eventually forwarded or dropped.
   * \param[in] event_ptr Event to update.
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  void UpdateDelayedStatistics(configuration::Configuration::Event const* event_ptr);

 private:
  /*!
   * \brief Const pointer to a configuration.
   */
  configuration::Configuration const* config_;

  /*!
   * \brief Ara com logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kPacketRouterLoggerContextId,
                                                    someip_daemon_core::logging::kPacketRouterLoggerContextDescription,
                                                    ara::core::StringView{"ClientEventPacketRouter"}};

  /*!
   * \brief   Overload protection scheduler table.
   * \details Stores an OverloadProtectionScheduler for each received event type with the feature
   *          enabled in the configuration.
   *          Creation and insertion of the schedulers into the table happens at runtime upon first
   *          reception of the event type.
   */
  OverloadProtectionSchedulerTable overload_protection_scheduler_table_{};

  /*!
   * \brief Map contains the event pointer mapped to the count of notifications that have been delayed
   *        due to overload protection.
   */
  EventDelayedStatisticsMap delayed_notifications_statistics_map_{};

  /*!
   * \brief Remote server routing table.
   */
  RemoteServerRoutingTable remote_server_event_routing_table_{};

  /*!
   * \brief   Remote packet validator.
   * \details All remote incoming SOME/IP packets shall be passed through this validator to validate the following
   *          points:
   *          - SOME/IP Header (shall be consistent and valid).
   *          - Configuration (the received message is consistent with our configuration).
   *          - Transmission protocol (The used transmission protocol is same as expected).
   */
  amsr::someip_daemon_core::packet_handler::RemoteEventPacketValidator remote_event_validator_;
};

}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_CLIENT_EVENT_PACKET_ROUTER_H_

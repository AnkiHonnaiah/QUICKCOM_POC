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
/**        \file  client_method_response_packet_router.h
 *        \brief  Packet router
 *
 *      \details  The packet router is responsible for receiving SOME/IP method responses
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_CLIENT_METHOD_RESPONSE_PACKET_ROUTER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_CLIENT_METHOD_RESPONSE_PACKET_ROUTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>
#include <tuple>
#include <utility>
#include "ara/core/map.h"
#include "ara/core/vector.h"

#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/client/method_response_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/packet_handler/remote_incoming_packet_validator.h"
#include "amsr/someip_daemon_core/packet_router/remote_session_id_generator.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/message_builder.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {

/*!
 * \brief ClientMethodResponsePacketRouter.
 */
class ClientMethodResponsePacketRouter {
 public:
  /*!
   * \brief Key for looking up the destination of a SOME/IP response message.
   */
  struct ResponseRouteKey {
    /*!
     * \brief A SOME/IP service instance identifier containing InstanceId, ServiceId, MajorVersion and MinorVersion.
     */
    configuration::types::SomeIpServiceInstanceIdCommunication instance_id{};

    /*!
     * \brief SOME/IP client identifier.
     */
    someip_protocol::internal::ClientId client_id{};

    // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
    /*!
     * \brief       Defines a strict weak ordering relationship for response route keys.
     * \param[in]   other A key to compare against.
     * \return      True if this key is less than the other one, false otherwise.
     * \pre         -
     * \context     ANY
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      TRUE
     */
    bool operator<(ResponseRouteKey const& other) const {
      return std::make_tuple(this->instance_id, this->client_id) < std::make_tuple(other.instance_id, other.client_id);
    }
  };

  /*!
   * \brief     Default destructor.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ~ClientMethodResponsePacketRouter() = default;

  ClientMethodResponsePacketRouter() = delete;
  ClientMethodResponsePacketRouter(ClientMethodResponsePacketRouter const&) = delete;
  ClientMethodResponsePacketRouter(ClientMethodResponsePacketRouter&&) = delete;
  ClientMethodResponsePacketRouter& operator=(ClientMethodResponsePacketRouter const&) & = delete;
  ClientMethodResponsePacketRouter& operator=(ClientMethodResponsePacketRouter&&) & = delete;

  /*!
   * \brief Construct the ClientMethodResponsePacketRouter.
   * \param[in] config                  Const pointer to a configuration.
   * \param[in] identity_access_manager IAM Interface used for access checks for incoming remote messages.
   *                                    It shall not be a "nullptr".
   * \param[in] statistics_handler      The statistics handler.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  ClientMethodResponsePacketRouter(
      configuration::Configuration const* config, iam::IamInterface& identity_access_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler);

  /*!
   * \brief       Adds a new routing entry for consumers of SOME/IP method responses.
   * \param[in]   service_instance_id   SOME/IP service instance id.
   * \param[in]   client_id             SOME/IP client id.
   * \param[in]   to                    A reference to the method response handler.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void AddResponseRoute(configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id,
                        someip_protocol::internal::ClientId const client_id,
                        client::MethodResponseHandlerInterface& to);

  /*!
   * \brief        Deletes a previously added routing entry for consumers of SOME/IP method responses.
   * \param[in]    service_instance_id  SOME/IP service instance ID.
   * \param[in]    client_id   SOME/IP client ID.
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void DeleteResponseRoute(configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id,
                           someip_protocol::internal::ClientId const client_id);

  /*!
   * \brief       Forwards a remote SOME/IP response message.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   packet      A SOME/IP response message.
   * \param[in]   source_id   Identifier of the remote packet source.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void OnResponseReceived(someip_protocol::internal::InstanceId const instance_id,
                          std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet,
                          RemoteSourceIdentifier const& source_id);

 protected:
  /*!
   * \brief Response routing table for service consumers.
   */
  using ResponseRoutingTable = ara::core::Map<ResponseRouteKey, client::MethodResponseHandlerInterface&>;

  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes

  /*!
   * \brief   Response consumer routing table (client side).
   * \details Associates ResponseRouteKey with the method response handler of the local application that consumes the
   *          response.
   */
  ResponseRoutingTable response_routing_table_{};

  // VECTOR Enable VectorC++-V11-0-2

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
                                                    ara::core::StringView{"ClientMethodResponsePacketRouter"}};

  /*!
   * \brief   Remote packet validator.
   * \details All remote incoming SOME/IP packets shall be passed through this validator to validate the following
   *          points:
   *          - SOME/IP Header (shall be consistent and valid).
   *          - Configuration (the received message is consistent with our configuration).
   *          - Security (the sending source is granted to send the message).
   *          - Transmission protocol (The used transmission protocol is same as expected).
   *          - Overload Protection Filter (Events shall be filtered according to the configured filters).
   */
  amsr::someip_daemon_core::packet_handler::RemoteIncomingPacketValidator remote_packet_validator_;
};

}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_CLIENT_METHOD_RESPONSE_PACKET_ROUTER_H_

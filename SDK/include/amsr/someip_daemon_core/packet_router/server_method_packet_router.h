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
/**        \file  server_method_packet_router.h
 *        \brief  Packet router for server methods
 *      \details  The packet router is responsible for forwarding SOME/IP server method requests and responses.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SERVER_METHOD_PACKET_ROUTER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SERVER_METHOD_PACKET_ROUTER_H_
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
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/someip_daemon_core/packet_handler/remote_incoming_packet_validator.h"
#include "amsr/someip_daemon_core/packet_router/remote_session_id_generator.h"
#include "amsr/someip_daemon_core/packet_router/someip_message_creator/someip_message_creator_interface.h"
#include "amsr/someip_daemon_core/server/method_request_handler_interface.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {

/*!
 * \brief Key for looking up the destination of a SOME/IP response message.
 */
struct ResponseRouteKey {
  /*!
   * \brief SOME/IP service instance identifier.
   */
  configuration::types::SomeIpServiceInstanceIdCommunication service_instance_id{};

  /*!
   * \brief SOME/IP client identifier.
   */
  someip_protocol::internal::ClientId client_id{};

  /*!
   * \brief SOME/IP method identifier.
   */
  someip_protocol::internal::MethodId method_id{};

  /*!
   * \brief SOME/IP session identifier.
   */
  someip_protocol::internal::SessionId session_id{};

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
    return std::make_tuple(this->service_instance_id, this->client_id, this->method_id, this->session_id) <
           std::make_tuple(other.service_instance_id, other.client_id, other.method_id, other.session_id);
  }
};

/*!
 * \brief Wrapper of properties of a SOME/IP response.
 */
struct RemoteResponseRoute {
  /*!
   * \brief SOME/IP session identifier of the original request.
   */
  someip_protocol::internal::SessionId original_session_id{};

  /*!
   * \brief Packet sink of the remote client.
   */
  RemotePacketSinkSharedPtr remote_client_sink{};
};

/*!
 * \brief ServerMethodPacketRouter.
 */
class ServerMethodPacketRouter {
 public:
  /*!
   * \brief     Default destructor.
   * \pre       -
   * \context   Shutdown
   * \steady    FALSE
   */
  virtual ~ServerMethodPacketRouter() = default;

  ServerMethodPacketRouter() = delete;
  ServerMethodPacketRouter(ServerMethodPacketRouter const&) = delete;
  ServerMethodPacketRouter(ServerMethodPacketRouter&&) = delete;
  ServerMethodPacketRouter& operator=(ServerMethodPacketRouter const&) & = delete;
  ServerMethodPacketRouter& operator=(ServerMethodPacketRouter&&) & = delete;

  /*!
   * \brief     Construct the PacketRouter.
   * \param[in] config                  Const pointer to a configuration.
   * \param[in] identity_access_manager IAM Interface, used for access checks for incoming remote messages.
   * \param[in] statistics_handler      The statistics handler.
   * \param[in] someip_message_creator  SomeIp Message Creator.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  ServerMethodPacketRouter(
      configuration::Configuration const* const config, iam::IamInterface& identity_access_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
      someip_message_creator::SomeIpMessageCreatorInterface const& someip_message_creator);

  /*!
   * \brief       Adds a new routing entry for providers of SOME/IP method requests.
   * \param[in]   service_instance_id SOME/IP service instance id.
   * \param[in]   source_id           The source id of the request.
   * \param[in]   request_handler     A request handler application.
   * \pre         The shared pointer to the destination packet sink shall not be a "nullptr".
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void AddRequestRoute(configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id,
                       server::MethodRequestHandler& request_handler);

  /*!
   * \brief       Deletes a previously added routing entry for providers of SOME/IP method requests.
   * \param[in]   service_instance_id SOME/IP service instance ID.
   * \param[in]   source_id    Identifier of the remote packet source.
   * \pre         -
   * \context     App | Timer | Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void DeleteRequestRoute(configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id);

  /*!
   * \brief       Forwards a remote SOME/IP request message.
   * \param[in]   instance_id  SOME/IP instance id.
   * \param[in]   from         A packet sink from which the passed SOME/IP request message comes.
   * \param[in]   packet       A SOME/IP request message.
   * \param[in]   source_id    Identifier of the remote packet source.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace SPEC-10144523
   */
  void OnRequestReceived(someip_protocol::internal::InstanceId const instance_id, RemotePacketSinkSharedPtr const& from,
                         std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet,
                         RemoteSourceIdentifier const& source_id);

  /*!
   * \brief       Forwards a remote SOME/IP fire & forget request message.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   packet      A SOME/IP request message.
   * \param[in]   source_id   Identifier of the remote packet source.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void OnRequestNoReturnReceived(someip_protocol::internal::InstanceId const instance_id,
                                 std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet,
                                 RemoteSourceIdentifier const& source_id);

  /*!
   * \brief       Forwards a local SOME/IP response message.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   packet      A SOME/IP response message.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void ForwardResponse(someip_protocol::internal::InstanceId const instance_id,
                       std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet);

 private:
  /*!
   * \brief Request routing table.
   */
  using RequestRoutingTable =
      ara::core::Map<configuration::types::SomeIpServiceInstanceIdCommunication, server::MethodRequestHandler const&>;

  /*!
   * \brief Response routing table.
   */
  using ResponseRoutingTable = ara::core::Map<ResponseRouteKey, RemoteResponseRoute>;

  /*!
   * \brief       Deletes a response route
   * \param[in]   service_instance_id  SOME/IP service instance id.
   * \param[in]   instance_id SOME/IP instance id.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void DeleteResponseRoute(configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id);

  /*!
   * \brief       Handle request cannot be forwarded.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   from        A packet sink from which the passed SOME/IP request message comes.
   * \param[in]   header      A SOME/IP request header.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void OnForwardRequestFailed(someip_protocol::internal::InstanceId const instance_id, RemotePacketSinkSharedPtr from,
                              someip_protocol::internal::SomeIpMessageHeader const& header);

  /*!
   * \brief       Handle not found request route.
   * \param[in]   instance_id SOME/IP instance id.
   * \param[in]   from        A packet sink from which the passed SOME/IP request message comes.
   * \param[in]   header      A SOME/IP request message header.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void OnRequestRouteNotFound(someip_protocol::internal::InstanceId const instance_id, RemotePacketSinkSharedPtr from,
                              someip_protocol::internal::SomeIpMessageHeader const& header);

  /*!
   * \brief       Creates and sends an error response for a failed method request.
   * \param[in]   header      SOME/IP error header.
   * \param[in]   error_code  Return code to set in the SOME/IP header.
   * \param[in]   to          A packet sink to which the SOME/IP error response shall be sent.
   * \pre         -
   * \context     Network | App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void SendErrorResponse(someip_protocol::internal::SomeIpMessageHeader const& header,
                         someip_protocol::internal::SomeIpReturnCode const error_code,
                         RemotePacketSinkSharedPtr const& to);

  /*!
   * \brief       Creates a new packet with the updated session_id in the header.
   * \details     The given packet will lose ownership of the SOME/IP message buffer. The return value gives ownership
   *              of this SOME/IP message buffer to the caller of the function.
   * \param[in]   packet     A SOME/IP message.
   * \param[in]   session_id SOME/IP session_id to update in the new packet header.
   * \return      A new packet with the updated session id.
   * \pre         -
   * \context     App | Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  static someip_protocol::internal::SomeIpMessage::DataBufferUniquePtr CreateNewPacketWithSessionId(
      std::shared_ptr<someip_protocol::internal::SomeIpMessage> const& packet,
      someip_protocol::internal::SessionId const session_id);

  /*!
   * \brief Const pointer to a configuration.
   */
  configuration::Configuration const* config_;

  /*!
   * \brief Ara com logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kPacketRouterLoggerContextId,
                                                    someip_daemon_core::logging::kPacketRouterLoggerContextDescription,
                                                    ara::core::StringView{"ServerMethodPacketRouter"}};

  /*!
   * \brief   Remote session id generator.
   * \details Generates unique session ids for requests with the same combination
   *          of {service id, instance id, method id, interface version}.
   *          This is used to guarantee unique session ids for remote incoming method requests from different ECUs.
   */
  RemoteSessionIdGenerator remote_session_id_generator_{};

  /*!
   * \brief SomeIpMessage Creator.
   */
  someip_message_creator::SomeIpMessageCreatorInterface const& someip_message_creator_;

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

  /*!
   * \brief Allocator for creating error messages as response to invalid incoming request messages.
   */
  someip_daemon_core::MemoryBufferAllocator error_message_memory_buffer_allocator_{};

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief Request routing table for service providers (server side).
   */
  RequestRoutingTable request_routing_table_{};

  /*!
   * \brief   Response provider routing table (server side).
   * \details Associates ResponseRouteKey with a pair of the remote sink that will consume the response and the session
   *          id of the corresponding request.
   */
  ResponseRoutingTable response_routing_table_{};
  // VECTOR Enable VectorC++-V11-0-2
};

}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_SERVER_METHOD_PACKET_ROUTER_H_

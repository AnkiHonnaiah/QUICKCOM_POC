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
/**        \file  remote_server_connection_manager_interface.h
 *        \brief  Class declaration of RemoteServerConnectionManagerInterface
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/configuration/types/network_endpoint.h"
#include "amsr/someip_daemon_core/configuration/types/transport_protocol.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief Class declaration of RemoteServerConnectionManagerInterface
 */
class RemoteServerConnectionManagerInterface {
 public:
  /*!
   * \brief Represents a SOME/IP message.
   */
  using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief             Constructs a default instance of Connection State Change Handler.
   * \pre               -
   * \context           Init
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            FALSE
   */
  RemoteServerConnectionManagerInterface() noexcept = default;

  /*!
   * \brief             Destructs instance
   * \pre               -
   * \reentrant         FALSE
   * \steady            FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  virtual ~RemoteServerConnectionManagerInterface() noexcept = default;

  RemoteServerConnectionManagerInterface(RemoteServerConnectionManagerInterface const &) = delete;
  RemoteServerConnectionManagerInterface(RemoteServerConnectionManagerInterface &&) = delete;
  RemoteServerConnectionManagerInterface &operator=(RemoteServerConnectionManagerInterface const &) & = delete;
  RemoteServerConnectionManagerInterface &operator=(RemoteServerConnectionManagerInterface &&) & = delete;

  /*!
   * \brief Sends a SOME/IP message to remote peer.
   *
   * \param[in] protocol The SOME/IP protocol (TCP/UDP) that shall be used to transmit the method request.
   * \param[in] packet   SOME/IP message containing Method Request header + payload.
   *
   * \return void if the message was sent successfully, and error otherwise.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionNotAvailable
   * Runtime check failed: connection is not established.
   *
   * \error packet_handler::validators::LocalIncomingPacketValidatorError::kRuntimeError_ConnectionTransmissionFailed
   * Runtime check failed: connection transmission failed.
   *
   * \context ANY
   * \steady  TRUE
   */
  virtual ara::core::Result<void> SendRequest(configuration::types::TransportProtocol const protocol,
                                              Packet const packet) noexcept = 0;

  /*!
   * \brief Get the connection state.
   *
   * \context ANY
   * \steady  TRUE
   *
   * \return The state of the connection to the server.
   *
   */
  virtual connection_manager::ConnectionState GetConnectionState() const noexcept = 0;

  /*!
   * \brief Return the local TCP network endpoint if it exists.
   * \details This API must only be called if a local network endpoint has been assigned to this object.
   *          This is ensured by the usage of the factory API "Create" which won't return a valid pointer to
   *          this object unless a local network endpoint has been assigned to this object when starting the connection
   *          establishment.
   *
   * \return The local TCP network endpoint if it exists
   *
   * \context ANY
   * \steady  FALSE
   */
  virtual configuration::types::NetworkEndpoint const &GetLocalTcpNetworkEndpoint() const noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REMOTE_SERVER_CONNECTION_MANAGER_INTERFACE_H_

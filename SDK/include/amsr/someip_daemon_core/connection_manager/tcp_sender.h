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
/*!        \file
 *        \brief  Class decleration for TcpSender
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_SENDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state_change_handler.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class TcpEndpoint;
class TcpConnection;

/*!
 * \brief           Class for TcpSender
 */
class TcpSender final : public packet_sink::RemotePacketSink {
 public:
  /*!
   * \brief           A TCP connection pointer type
   */
  using TcpConnectionPtr = TcpConnection*;

  /*!
   * \brief           A TCP connection state change handler pointer type
   */
  using ConnectionStateChangeHandlerPtr = ConnectionStateChangeHandler*;

  /*!
   * \brief           Constructor of TcpSender
   * \param[in]       endpoint
   *                  A reference to the TCP endpoint this sender belongs to.
   * \param[in]       connection
   *                  A TCP connection this sender belongs to.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpSender(TcpEndpoint& endpoint, TcpConnectionPtr connection) noexcept;

  TcpSender() = delete;
  TcpSender(TcpSender const&) = delete;
  TcpSender& operator=(TcpSender const&) = delete;
  TcpSender(TcpSender&&) = delete;
  TcpSender& operator=(TcpSender&&) = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief           Destructs instance TcpSender
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~TcpSender() noexcept override;

  /*!
   * \brief           Getter function to return the remote IP address
   * \return          Returns the remote IP address.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  someip_daemon_core::IpAddress const& GetRemoteAddress() const noexcept;

  /*!
   * \brief           Returns the remote port number
   * \return          A remote port number.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  amsr::net::ip::Port GetRemotePort() const noexcept;

  /*!
   * \brief           Sets a connection state change handler
   * \param[in]       handler
   *                  A connection state change handler.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void SetConnectionStateChangeHandler(ConnectionStateChangeHandlerPtr handler) noexcept;

  /*!
   * \brief           Registers a required SOME/IP service instance
   * \param[in]       service_instance_id
   *                  A SOME/IP service instance identifier.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void RegisterRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id) noexcept;

  /*!
   * \brief           Unregisters a required SOME/IP service instance
   * \param[in]       service_instance_id
   *                  A SOME/IP service instance identifier.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void UnregisterRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief           Sends a SOME/IP message to remote peer
   * \param[in]       packet
   *                  A SOME/IP message.
   * \return          true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool ForwardFromLocal(packet_sink::Packet packet) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief           Sends a PDU message to remote peer
   * \param[in]       packet
   *                  A PDU message.
   * \return          true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool ForwardPduFromLocal(packet_sink::PduPacket packet) noexcept override;

  /*!
   * \brief           Notifies a TCP connection state change handler about a state change
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void Notify() noexcept;

  /*!
   * \brief           Determines whether the underlying connection is already established
   * \return          True if the connection is established, otherwise false.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool IsConnected() const noexcept;

  /*!
   * \brief           Get the local port number (set during Connection establishment)
   * \return          The local port of this connection In case the configured port has value 0 (dynamic port), the
   *                  retrieved local port will contain the exact port number. Otherwise, it will contain the same value
   *                  as the configured one.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  someip_protocol::internal::Port GetLocalPort() const noexcept;

 private:
  /*!
   * \brief           Represents a container of required SOME/IP service instances
   */
  using RequiredServiceInstanceContainer =
      ara::core::Vector<configuration::types::SomeIpServiceInstanceIdCommunication>;

  /*!
   * \brief           A reference to the TCP endpoint this sender belongs to
   */
  TcpEndpoint& endpoint_;

  /*!
   * \brief           A TCP connection this sender belongs to
   */
  TcpConnectionPtr connection_;

  /*!
   * \brief           A TCP connection state change handler
   */
  ConnectionStateChangeHandlerPtr handler_{nullptr};

  /*!
   * \brief           Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief           A container of registered required SOME/IP service instances
   */
  RequiredServiceInstanceContainer required_service_instances_;

  /*!
   * \brief           Friend test to access the private member required_service_instances_
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpSender_Parametrized,
              RegisterAndUnregisterDifferentRequiredServiceInstance);
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_SENDER_H_

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
 *        \brief  Class for handling TLS connections.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_TLS_CONNECTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_TLS_CONNECTION_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <memory>
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/connection_manager/buffer_data_source.h"
#include "amsr/someip_daemon_core/connection_manager/connection.h"
#include "amsr/someip_daemon_core/connection_manager/secure_communication/secure_communication_manager.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_connection.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "tls/public/tls_comm_callback_interfaces.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace secure_communication {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_SomeIpDaemon_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief           TlsConnection class
 */
class TlsConnection final : public connection_manager::TcpConnection, public tls::TlsCommCallbackInterface {
 public:
  /*!
   * \brief           The type of a class responsible reading a SOMEIP stream message
   */
  using StreamMessageReader = message_reader::SomeIpStreamMessageReader;

  /*!
   * \brief           Constructs an instance of TlsConnection
   *
   * \param[in]       local_endpoint
   *                  A reference to the TLS-secured TCP endpoint this connection belongs to.
   * \param[in]       packet_router
   *                  The packet router.
   * \param[in]       remote_endpoint
   *                  A reference to the remote endpoint.
   * \param[in]       options
   *                  Specific socket options to apply for the underlying TCP socket.
   * \param[in]       secure_communication_manager
   *                  A secure communication manager.
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \param[in]       sec_com_config
   *                  The SecCom configuration.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TlsConnection(TcpEndpoint& local_endpoint, std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router,
                Endpoint&& remote_endpoint, someip_protocol::internal::SocketOptions const& options,
                SecureCommunicationManager const& secure_communication_manager,
                someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
                configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config) noexcept;

  /*!
   * \brief           Constructs an instance of TlsConnection
   *
   * \param[in]       local_endpoint
   *                  A reference to the TLS-secured TCP endpoint this connection belongs to.
   * \param[in]       remote_endpoint
   *                  A reference to the remote endpoint.
   * \param[in]       packet_router
   *                  The packet router.
   * \param[in]       socket
   *                  A connected TCP socket that was accepted by TCP server.
   * \param[in]       options
   *                  Specific socket options to apply for for the underlying TCP socket.
   * \param[in]       secure_communication_manager
   *                  A secure communication manager.
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \param[in]       sec_com_config
   *                  The SecCom configuration.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TlsConnection(TcpEndpoint& local_endpoint, Endpoint&& remote_endpoint,
                std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router, Socket&& socket,
                someip_protocol::internal::SocketOptions const& options,
                SecureCommunicationManager const& secure_communication_manager,
                someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
                configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config) noexcept;

  /*!
   * \brief           Destructor
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~TlsConnection() noexcept final = default;

  TlsConnection() = delete;
  TlsConnection(TlsConnection const&) = delete;
  TlsConnection(TlsConnection&&) = delete;
  TlsConnection& operator=(TlsConnection const&) = delete;
  TlsConnection& operator=(TlsConnection&&) = delete;

 private:
  /*!
   * \copydoc         connection_manager::TcpConnection::Forward()
   * \steady          FALSE
   */
  bool Forward(std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet) noexcept final;

  /*!
   * \copydoc         connection_manager::TcpConnection::HandleRead()
   * \steady          FALSE
   */
  bool HandleRead() noexcept final;

  /*!
   * \brief           Called by the (D)TLS provider after a successful handshake
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void OnConnected() noexcept final;

  /*!
   * \brief           Called by a (D)TLS provider when the connection is disconnected
   * \details         The connection can be disconnected as a result of several different events. The most common are:
   *                  - Call to TlsCommBase::CloseConnection() from the user
   *                  - An unsuccessful handshake
   *                  - Any kind of failure
   * \param[in]       err_string
   *                  An error message.
   * \param[in]       err_code
   *                  An error code.
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void OnDisconnected(ara::core::StringView const, tls::TlsCommErrorCode const) noexcept final;

  /*!
   * \brief           Called internally by a (D)TLS provider when data needs to be sent over the transport layer
   * \details         The data buffer contains serialized and possibly encrypted messages. The most common messages are
   *                  Handshake messages and user application data messages.
   * \param[in]       buffer
   *                  A read only view of a raw data buffer.
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void WriteToTransport(tls::ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \brief           Called internally by a (D)TLS provider when data needs to be sent to the user application
   * \details         The data buffer contains user application messages received from the connected peer.
   * \param           buffer
   *                  A read only view of a raw data buffer.
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void WriteToCommParty(tls::ReadOnlyBufferView buffer) noexcept final;

  /*!
   * \copydoc         connection_manager::TcpConnection::Connected()
   * \steady          FALSE
   */
  void Connected() noexcept final;

  /*!
   * \copydoc         connection_manager::TcpConnection::Disconnected()
   * \steady          FALSE
   */
  void Disconnected() noexcept final;

  /*!
   * \brief           Called upon socket errors
   *
   * \param[in]       error_code
   *                  The Error code.
   * \context         API
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   *
   * \steady          FALSE
   */
  void OnSocketError(ara::core::ErrorCode const& error_code) noexcept;

  /*!
   * \brief           The network packet router
   */
  std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router_;

  /*!
   * \brief           A TLS communication provider
   */
  TlsCommunicationProviderPtr const tls_provider_;

  /*!
   * \brief           TLS ciphertext memory buffer type
   */
  using TlsCipherTextBuffer = std::array<std::uint8_t, secure_communication::kReadBufferSize>;

  /*!
   * \brief           A memory buffer
   */
  TlsCipherTextBuffer read_buffer_;

  /*!
   * \brief           Current state of the TLS connection
   */
  amsr::tls::EnTlsCommRoleState connection_state_{amsr::tls::EnTlsCommRoleState::kRoleStateNotConnected};

  /*!
   * \brief           Flag to keep track if the remote peer has disconnected from the TLS connection
   */
  bool peer_has_disconnected_{false};

  /*!
   * \brief           Friend test to test call to HandleRead() in Handshake state
   */
  FRIEND_TEST(Cdd__SomeIpd__ConnectionManagerSecureCom, TlsConnection_CallHandleReadInHandshakeState);
};

}  // namespace secure_communication
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_TLS_CONNECTION_H_

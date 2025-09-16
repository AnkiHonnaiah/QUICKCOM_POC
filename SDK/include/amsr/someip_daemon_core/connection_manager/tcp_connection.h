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
 *        \brief  Manage the SOME/IP TCP connection.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_CONNECTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_CONNECTION_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <utility>
#include "amsr/net/ip/tcp.h"
#include "amsr/someip_daemon/extension_points/message_authentication/mac_interface.h"
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/connection_manager/connection.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_connection_error_handler.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_response_sender.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/someip_daemon_core/message_reader/someip_stream_message_reader.h"
#include "amsr/someip_daemon_core/message_reader/someip_stream_packet_limits.h"
#include "amsr/someip_daemon_core/message_writer/someip_stream_message_writer.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/message_builder.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class TcpEndpoint;
class TcpSender;

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief           Class Declaration of TcpConnection
 * \trace           SPEC-10144475, SPEC-10144476, SPEC-10144481
 */
class TcpConnection : public Connection<TcpSender> {
 public:
  /*!
   * \brief           Maximum amount of memory (in bytes) that can be allocated for a TCP packet
   * \details         This constant represents the maximum allowed size for a TCP packet (header + payload). Any
   *                  received SOME-IP packet that requires more memory than this maximum value will be rejected. This
   *                  value MUST NEVER BE MODIFIED.
   */
  static std::uint32_t constexpr kMaximumTcpPacketSize{
      message_reader::kMaximumAllowedPayloadLength +
      static_cast<std::uint32_t>(someip_protocol::internal::kHeaderLength)};

  /*!
   * \brief           Delete default constructor
   * \steady          FALSE
   */
  TcpConnection() = delete;

  TcpConnection(TcpConnection const&) = delete;
  TcpConnection(TcpConnection&&) = delete;
  TcpConnection& operator=(TcpConnection const&) = delete;
  TcpConnection& operator=(TcpConnection&&) = delete;

  /*!
   * \brief           Destructs the instance of TcpConnection. Shuts down TCP connection if it is still alive
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~TcpConnection() noexcept override;

  /*!
   * \brief           A TCP sender pointer type
   */
  using TcpSenderPtr = typename std::add_pointer<TcpSender>::type;

  /*!
   * \brief           A TCP socket type
   */
  using Socket = amsr::net::ip::Tcp::Socket;

  /*!
   * \brief           An IP address
   */
  using Address = someip_daemon_core::IpAddress;

  /*!
   * \brief           The type of a class responsible for reading a message from the stream
   */
  using StreamMessageReader = message_reader::SomeIpStreamMessageReader;

  /*!
   * \brief           The type of a class responsible for writing a message to the stream
   */
  using StreamMessageWriter = message_writer::SomeIpStreamMessageWriter;

  /*!
   * \brief           Type alias for osabstraction endpoint
   */
  using Endpoint = amsr::net::ip::Tcp::Endpoint;

  /*!
   * \brief           Constructs an active TcpConnection
   * \param[in]       local_endpoint
   *                  A reference to the TCP endpoint this connection belongs to.
   * \param[in]       packet_router
   *                  The packet router.
   * \param[in]       remote_endpoint
   *                  A remote endpoint.
   * \param[in]       options
   *                  Specific socket options to apply for this TCP socket.
   * \param[in]       mac_manager
   *                  MacManager used to create and verify secure messages.
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \context         Init
   * \pre             Connection creation shall be possible.
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpConnection(
      TcpEndpoint& local_endpoint, std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router,
      Endpoint&& remote_endpoint, someip_protocol::internal::SocketOptions const& options,
      someip_daemon::extension_points::message_authentication::MacInterface* mac_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept;

  /*!
   * \brief           Constructs a passive TcpConnection
   * \param[in]       local_endpoint
   *                  A reference to the TCP endpoint this connection belongs to.
   * \param[in]       remote_endpoint
   *                  A reference to the remote endpoint.
   * \param[in]       packet_router
   *                  The packet router.
   * \param[in]       socket
   *                  A connected TCP socket that was accepted by TCP server.
   * \param[in]       options
   *                  Specific socket options to apply for this TCP socket of this TCP connection.
   * \param[in]       mac_manager
   *                  MacManager used to create and verify secure messages.
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \context         Timer (Connection is being created once server state machine leaves Initial Wait Phase)
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpConnection(
      TcpEndpoint& local_endpoint, Endpoint&& remote_endpoint,
      std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router, Socket&& socket,
      someip_protocol::internal::SocketOptions const& options,
      someip_daemon::extension_points::message_authentication::MacInterface* mac_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept;

  /*!
   * \brief           Indicates whether this connection was established actively or passively
   * \return          "true" if TCP connection is active and "false" otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool IsActive() const noexcept;

  /*!
   * \brief           Indicates whether this connection is still alive
   * \return          True if TCP connection is alive and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool IsConnected() const noexcept override;

  /*!
   * \brief           Connects actively to the remote peer if TCP connection is dead
   * \return          void "Result" indicating whether a new active TCP connection is created or not.
   * \context         ANY
   * \pre             Constructor for active TcpConnection shall be called.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ara::core::Result<void> Connect() noexcept;

  /*!
   * \brief           Shuts down TCP connection if it is still alive
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void Disconnect() noexcept;

  /*!
   * \brief           Sends a SOME/IP message to remote peer
   * \param[in]       packet
   *                  A SOME/IP message.
   * \return          "true" if the message is sent successfully and "false" otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  virtual bool Forward(std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet) noexcept;

  /*!
   * \brief           Increments the number of users of this TCP connection
   * \return          The current number of users.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  std::size_t Acquire() noexcept;

  /*!
   * \brief           Decrements the number of users of this TCP connection
   * \return          The current number of users.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  std::size_t Release() noexcept;

  /*!
   * \brief           Returns the number of users of this TCP connection
   * \return          The current number of users.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  std::size_t GetUsers() const noexcept;

  /*!
   * \brief           Registers a required SOME/IP service instance
   * \param[in]       service_instance_id
   *                  A SOME/IP service instance id containing instance id, service id, and major version.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \trace           SPEC-8053471
   * \steady          FALSE
   */
  void RegisterRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id) noexcept;

  /*!
   * \brief           Unregisters a required SOME/IP service instance
   * \param[in]       service_instance_id
   *                  A SOME/IP service instance id containing instance id, service id, and major version.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void UnregisterRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id) noexcept;

  /*!
   * \brief           Called when a TCP socket becomes readable
   * \return          "true" if read operation is completed without errors and "false" otherwise.
   * \context         Reactor
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  virtual bool HandleRead() noexcept;

  /*!
   * \brief           Called when a TCP socket becomes writable
   * \context         Reactor
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  void HandleWrite() noexcept;

  /*!
   * \brief           Get the socket from TcpConnection object
   * \details         It has to be used alongwith ReturnSocket so that socket will be returned when used.
   * \return          The socket object.
   * \context         Reactor
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  Socket& GetSocketRef() noexcept;

  /*!
   * \brief           Get the local port number (set during Connection establishment)
   * \return          The local port of this connection. In case the configured port has value 0 (dynamic port), the
   *                  retrieved local port will contain the exact port number. Otherwise, it will contain the same value
   *                  as the configured one. Note: if Connect() has not been called, or has failed, the return value
   *                  will be the same as the one given to the constructor.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  someip_protocol::internal::Port GetLocalPort() const noexcept;

 protected:
  /*!
   * \brief           A TCP sender pointer type
   */
  using TcpSenderContainer = ara::core::Vector<TcpSenderPtr>;

  /*!
   * \brief           Represents a container of required SOME/IP service instances
   */
  using RequiredServiceInstanceContainer =
      ara::core::Vector<configuration::types::SomeIpServiceInstanceIdCommunication>;

  /*!
   * \brief           A SOME/IP message type
   */
  using SomeIpMessage = someip_protocol::internal::SomeIpMessage;

  /*!
   * \brief           Called when a TCP connection is established
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  virtual void Connected() noexcept;

  /*!
   * \brief           Called when a TCP connection dies or is closed
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  virtual void Disconnected() noexcept;

  /*!
   * \brief           Processes a single received SOME/IP message
   * \param[in]       message
   *                  A received SOME/IP message.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void ProcessMessage(SomeIpMessage&& message) noexcept;

  /*!
   * \brief           Update/Sets whether this connection is still alive and connected or not
   * \param[in]       is_connected
   *                  Value for "is_connected_" flag.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void SetIsConnected(bool const is_connected) noexcept;

  /*!
   * \brief           Finds the instance id associated with the received SOME/IP message
   *
   * \param[in]       header
   *                  SOME/IP header.
   * \return          An ara::core::result containing SOME/IP service instance id.
   * \error           SomeIpReturnCode::kWrongInterface
   *                  ServiceId matches but the MajorVersion does not.
   * \error           SomeIpReturnCode::kUnknownService
   *                  Neither ServiceId nor MajorVersion matches.
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ara::core::Result<someip_protocol::internal::InstanceId, someip_protocol::internal::SomeIpReturnCode> FindInstanceId(
      someip_protocol::internal::SomeIpMessageHeader const& header) const noexcept;

  /*!
   * \brief           Sends an error response for a failed method request
   * \param[in]       header
   *                  SOME/IP header.
   * \param[in]       error_code
   *                  Return code to set in the SOME/IP header.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void SendErrorResponse(someip_protocol::internal::SomeIpMessageHeader const& header,
                         someip_protocol::internal::SomeIpReturnCode const error_code) noexcept;

  /*!
   * \brief           Looks up the SOME/IP service instance identifier belonging to a registered SOME/IP service
   *                  instance
   * \param[in]       service_deployment_id
   *                  The service deployment id, containing service id and major version.
   *
   * \return          An ara::core::result containing SOME/IP service instance id.
   * \error           SomeIpReturnCode::kWrongInterface
   *                  ServiceId matches but the MajorVersion does not.
   * \error           SomeIpReturnCode::kUnknownService
   *                  Neither ServiceId nor MajorVersion matches.
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ara::core::Result<someip_protocol::internal::InstanceId, someip_protocol::internal::SomeIpReturnCode>
  GetRequiredServiceInstanceId(configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication const&
                                   service_deployment_id) const noexcept;

  /*!
   * \brief           Applies the socket options from the configuration on an established connection
   * \return          "true" if sockets options are set and "false" otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool ApplySocketOptions() noexcept;

  /*!
   * \brief           Generates a unique logger prefix
   * \param[in]       local_address
   *                  A local IP address.
   * \param[in]       local_port
   *                  A local port number.
   * \param[in]       remote_address
   *                  A remote IP address.
   * \param[in]       remote_port
   *                  A remote port number.
   * \return          A string representing the logger prefix.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  static ara::core::String GetLoggerPrefix(someip_protocol::internal::IpAddress const& local_address,
                                           someip_protocol::internal::Port const local_port,
                                           someip_protocol::internal::IpAddress const& remote_address,
                                           someip_protocol::internal::Port const remote_port) noexcept;

  /*!
   * \brief           Helper function for handling of the result of enqueing messages
   * \param[in]       status
   *                  The status of the enqueue operation.
   * \return          True if the operation was successful, otherwise false.
   * \pre             This handler shall not be called with status 'kOk'.
   * \steady          FALSE
   */
  bool HandleEnqueueStatus(message_writer::SomeIpStreamMessageWriter::ReturnCode status) const noexcept;

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief           A reference to the TCP endpoint this connection belongs to
   */
  TcpEndpoint& endpoint_;

  /*!
   * \brief           An instance of the remote os endpoint with which this connection communicates
   */
  Endpoint remote_endpoint_;

  /*!
   * \brief           A TCP socket
   */
  Socket socket_;

  /*!
   * \brief           The local port number (set during Connection establishment)
   */
  someip_protocol::internal::Port local_port_;

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_mutableMemberShallBePrivate
  /*!
   * \brief           Data buffer allocator, used to allocate memory for the received payload
   */
  someip_daemon_core::MemoryBufferAllocator allocator_{kMaximumTcpPacketSize};

  /*!
   * \brief           A message reader for stream sockets
   */
  StreamMessageReader message_reader_{allocator_};

  /*!
   * \brief           A message writer for stream sockets
   */
  StreamMessageWriter message_writer_{this};

  /*!
   * \brief           Reactor handle
   */
  osabstraction::io::reactor1::CallbackHandle reactor_handle_{};
  // VECTOR Enable VectorC++-V11-0-2

 private:
  /*!
   * \brief           A MAC manager
   */
  someip_daemon::extension_points::message_authentication::MacInterface* mac_manager_;

  /*!
   * \brief           The packet router (to which the received packets will be forwarded)
   */
  std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router_;

  /*!
   * \brief           Indicates whether this connection was established actively or passively
   */
  bool is_active_;

  /*!
   * \brief           Indicates whether this connection is still alive
   */
  bool is_connected_;

  /*!
   * \brief           Counts how many times TCP connection is in use
   */
  std::atomic<std::size_t> users_;

  /*!
   * \brief           Allocator for creating error messages as response to invalid incoming request messages
   */
  someip_daemon_core::MemoryBufferAllocator error_message_memory_buffer_allocator_{};

  /*!
   * \brief           TCP socket options for this connection handed over from the TCP endpoint
   */
  someip_protocol::internal::SocketOptions const socket_options_;

  /*!
   * \brief           A container of registered required SOME/IP service instances
   */
  RequiredServiceInstanceContainer required_service_instances_{};

  /*!
   * \brief           A statistics handler
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  /*!
   * \brief           Used to track SOME/IP header errors over TCP and to trigger disconnection
   */
  TcpConnectionErrorHandler tcp_connection_error_handler_;

 protected:
  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief           Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_;

 private:
  /*!
   * \brief           Friend test, to test TcpConnectionErrorHandler member variable
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpConnection, TcpConnection_ErrorHandler);
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_CONNECTION_H_

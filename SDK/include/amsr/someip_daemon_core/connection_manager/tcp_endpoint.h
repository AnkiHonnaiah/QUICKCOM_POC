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
 *        \brief  Class declaration of TcpEndpoint.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_ENDPOINT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/connection_manager/secure_communication/secure_communication_manager.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_connection.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_receiver.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_response_sender.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_sender.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_server.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class TcpEndpoint;

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief           Class declaration of TcpEndpoint
 */
class TcpEndpoint final {
 public:
  /*!
   * \brief           Destructs instance
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~TcpEndpoint() noexcept = default;

  TcpEndpoint() = delete;
  TcpEndpoint(TcpEndpoint const&) = delete;
  TcpEndpoint(TcpEndpoint&&) = delete;
  TcpEndpoint& operator=(TcpEndpoint const&) = delete;
  TcpEndpoint& operator=(TcpEndpoint&&) = delete;

  /*!
   * \brief           A TCP sender pointer type
   */
  using TcpSenderPtr = amsr::UniquePtr<TcpSender>;

  /*!
   * \brief           A TCP receiver pointer type
   */
  using TcpReceiverPtr = amsr::UniquePtr<TcpReceiver>;

  /*!
   * \brief           A SOME/IP message type
   */
  using SomeIpMessage = someip_protocol::internal::SomeIpMessage;

  /*!
   * \brief           A type alias for the Reactor1
   */
  using Reactor = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief           Type alias for the endpoint from libosabstraction
   */
  using Endpoint = amsr::net::ip::Tcp::Endpoint;

  /*!
   * \brief           Type alias for server accepted connection from libosabstraction
   */
  using AcceptedConnection = amsr::net::ip::Tcp::Acceptor::AcceptedConnection;

  /*!
   * \brief           Enum that determines if the TcpEndpoint is active (both secured and not) or passive (either
   *                  secured or not)
   */
  enum class TcpEndpointType : std::uint8_t { kTlsSecuredPassive, kNotSecuredPassive, kActive };

  /*!
   * \brief           Constructs an instance of a passive TcpEndpoint not secured with TLS
   * \param[in]       reactor
   *                  A reactor.
   * \param[in]       packet_router
   *                  The packet router (to which the received packets will be forwarded)
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \param[in]       endpoint
   *                  The local OS layer endpoint.
   * \param[in]       options
   *                  Additional options for TCP sockets (QoS, KeepAlive).
   * \param[in]       mac_manager
   *                  MacManager
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \context         Init
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  explicit TcpEndpoint(
      Reactor* reactor, std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router,
      someip_daemon_core::IpAddress const& address, someip_protocol::internal::Port const port, Endpoint&& endpoint,
      someip_protocol::internal::SocketOptions const& options,
      someip_daemon::extension_points::message_authentication::MacInterface* mac_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept;

  /*!
   * \brief           Constructs an instance of a passive TcpEndpoint secured with TLS
   * \param[in]       reactor
   *                  A reactor.
   * \param[in]       packet_router
   *                  The packet router (to which the received packets will be forwarded).
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \param[in]       endpoint
   *                  The local OS layer endpoint.
   * \param[in]       options
   *                  Additional options for TCP sockets (QoS, KeepAlive).
   * \param[in]       secure_communication_manager
   *                  A secure communication manager.
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \param[in]       sec_com_config
   *                  The SecCom configuration.
   * \context         Init
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  explicit TcpEndpoint(
      Reactor* reactor, std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router,
      someip_daemon_core::IpAddress const& address, someip_protocol::internal::Port const port, Endpoint&& endpoint,
      someip_protocol::internal::SocketOptions const& options,
      secure_communication::SecureCommunicationManager& secure_communication_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
      configuration::ConfigurationTypesAndDefs::SecComConfig sec_com_config) noexcept;

  /*!
   * \brief           Constructs an instance of an active TCP endpoint
   * \param[in]       reactor
   *                  A reactor.
   * \param[in]       packet_router
   *                  The packet router (to which the received packets will be forwarded)
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \param[in]       endpoint
   *                  The local OS layer endpoint.
   * \param[in]       options
   *                  Additional options for TCP sockets (QoS, KeepAlive).
   * \param[in]       mac_manager
   *                  MacManager
   * \param[in]       secure_communication_manager
   *                  A secure communication manager.
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \context         Init
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  explicit TcpEndpoint(
      Reactor* reactor, std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router,
      someip_daemon_core::IpAddress const& address, someip_protocol::internal::Port const port, Endpoint&& endpoint,
      someip_protocol::internal::SocketOptions const& options,
      someip_daemon::extension_points::message_authentication::MacInterface* mac_manager,
      secure_communication::SecureCommunicationManager& secure_communication_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler) noexcept;

  /*!
   * \brief           Getter for the type of TCP endpoint
   * \return          An enum determining if the endpoint is active (both secured and not) or passive (either secured or
   *                  not).
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       TRUE
   * \steady          FALSE
   */
  TcpEndpointType GetTcpEndpointType() const noexcept;

  /*!
   * \brief           Returns the local IP address
   * \return          A local IP address.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  someip_daemon_core::IpAddress const& GetAddress() const noexcept;

  /*!
   * \brief           Returns the local port number
   * \return          A local port number.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  someip_protocol::internal::Port GetPort() const noexcept;

  /*!
   * \brief           Indicates whether a passive TCP connection exists and if one is found if it is connected
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \return          true if the specified passive TCP connection exists and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool HasPassiveConnectionAndIsConnected(someip_protocol::internal::IpAddress const& address,
                                          someip_protocol::internal::Port const port) noexcept;

  /*!
   * \brief           Returns a new TCP sender for an active TCP connection
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \param[in]       sec_com_config
   *                  The SecCom configuration.
   * \return          A TCP sender.
   * \context         ANY
   * \pre             TCP connection shall be possible.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \trace           SPEC-10144555
   * \trace           SPEC-10144565
   * \trace           SPEC-4980226, SPEC-4980227, SPEC-4980228, SPEC-4980230, SPEC-4980232, SPEC-10144555, SPEC-10144565
   * \steady          FALSE
   */
  ara::core::Result<TcpSenderPtr> GetActiveSender(
      someip_protocol::internal::IpAddress const& address, someip_protocol::internal::Port const port,
      configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config) noexcept;

  /*!
   * \brief           Returns a new TCP sender for a passive TCP connection
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \return          A TCP sender.
   * \context         ANY
   * \pre             TCP connection shall be creatable.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ara::core::Result<TcpSenderPtr> GetPassiveSender(someip_protocol::internal::IpAddress const& address,
                                                   someip_protocol::internal::Port const port) noexcept;

  /*!
   * \brief           Returns a new TCP receiver
   * \return          A TCP receiver.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpReceiverPtr GetReceiver() noexcept;

  /*!
   * \brief           Registers the given TCP connection for event notifications
   * \details         This function only registers a callback. It does not start monitoring any read/write
   *                  notifications. They must be enabled/disabled explicitly with the
   *                  MonitorWriteEvents/MonitorReadEvents API
   * \param[in]       connection
   *                  A TCP connection.
   * \param[in]       io_source
   *                  A native handle for TCP socket.
   * \return          A handle for the registered callback.
   * \error           osabstraction::OsabErrc::kApiError
   *                  A callback is already registered for NativeHandle or waiting for the specified events is not
   *                  possible for NativeHandle.
   * \error           osabstraction::OsabErrc::kResource
   *                  No memory to register another callback or system limit
   * \context         Reactor
   * \pre             TCP connection is not null
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  ara::core::Result<osabstraction::io::reactor1::CallbackHandle> RegisterEventHandler(
      TcpConnection* connection, osabstraction::io::NativeHandle const& io_source) noexcept;

  /*!
   * \brief           Unregisters the given callback handle at the reactor
   * \details         After this function call, no more event notifications will be received
   * \param[in]       reactor_handle
   *                  A reactor handle
   * \context         Reactor
   * \pre             reactor_handle is a valid and registered callback handle
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  void UnregisterEventHandler(osabstraction::io::reactor1::CallbackHandle const reactor_handle) const noexcept;

  /*!
   * \brief           Enables or disables monitoring write events
   * \param[in]       reactor_handle
   *                  The handle to enable/disable the event monitoring
   * \param[in]       enable
   *                  A flag to indicate enable/disable
   * \context         Reactor
   * \pre             reactor_handle is a valid and registered callback handle
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  void MonitorWriteEvents(osabstraction::io::reactor1::CallbackHandle const reactor_handle,
                          bool const enable) const noexcept;

  /*!
   * \brief           Enables or disables monitoring read events
   * \param[in]       reactor_handle
   *                  The handle to enable/disable the event monitoring
   * \param[in]       enable
   *                  A flag to indicate enable/disable
   * \context         Reactor
   * \pre             reactor_handle is a valid and registered callback handle
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  void MonitorReadEvents(osabstraction::io::reactor1::CallbackHandle const reactor_handle,
                         bool const enable) const noexcept;

  /*!
   * \brief           Acquires a new reference to the given TCP connection
   * \param[in]       connection
   *                  A TCP connection.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void AcquireConnection(TcpConnection* connection) const noexcept;

  /*!
   * \brief           Releases a reference to the given TCP connection
   * \param[in]       connection
   *                  A TCP connection.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void ReleaseConnection(TcpConnection* connection) noexcept;

  /*!
   * \brief           Increments the number of server users
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void AcquireServer() noexcept;

  /*!
   * \brief           Decreases the number of server users
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \trace           SPEC-4981407
   * \steady          FALSE
   */
  void ReleaseServer() noexcept;

  /*!
   * \brief           Registers a provided SOME/IP service instance
   * \param[in]       service_instance_id
   *                  The service instance id
   * \context         Init
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \trace           SPEC-8053472
   * \steady          FALSE
   */
  void RegisterProvidedServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id) noexcept;

  /*!
   * \brief           Unregisters a provided SOME/IP service instance
   * \param[in]       service_instance_id
   *                  The service instance id
   * \context         Shutdown
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void UnregisterProvidedServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id) noexcept;

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
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ara::core::Result<someip_protocol::internal::InstanceId, someip_protocol::internal::SomeIpReturnCode>
  GetProvidedServiceInstanceId(configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication const&
                                   service_deployment_id) const noexcept;

  /*!
   * \brief           Returns the internal endpoint object
   * \return          the internal osabstraction endpoint object
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  Endpoint const& GetEndpoint() const noexcept;

 private:
  /*!
   * \brief           A TCP connection pointer type
   */
  using TcpConnectionPtr = amsr::UniquePtr<TcpConnection>;

  /*!
   * \brief           A TCP connection container type
   */
  using TcpConnectionContainer = ara::core::Vector<TcpConnectionPtr>;

  /*!
   * \brief           TCP connection container iterator type
   */
  using TcpConnectionContainerIter = TcpConnectionContainer::iterator;

  /*!
   * \brief           A TCP server pointer type
   */
  using TcpServerPtr = amsr::UniquePtr<TcpServer>;

  /*!
   * \brief           A map from a socket handle to a TCP connection
   */
  using EventHandlerMap = std::unordered_map<
      osabstraction::io::reactor1::CallbackHandle, TcpConnection*,
      std::hash<osabstraction::io::reactor1::CallbackHandle>,
      std::equal_to<osabstraction::io::reactor1::CallbackHandle>,
      ara::core::PolymorphicAllocator<std::pair<osabstraction::io::reactor1::CallbackHandle const, TcpConnection*>>>;

  /*!
   * \brief           Represents a container of provided SOME/IP service instances
   */
  using ProvidedServiceInstanceContainer =
      ara::core::Vector<configuration::types::SomeIpServiceInstanceIdCommunication>;

  /*!
   * \brief           Delegating constructor
   * \param[in]       reactor
   *                  A reactor.
   * \param[in]       packet_router
   *                  The packet router (to which the received packets will be forwarded)
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \param[in]       endpoint
   *                  The local OS layer endpoint.
   * \param[in]       options
   *                  Additional options for TCP sockets (QoS, KeepAlive).
   * \param[in]       mac_manager
   *                  MacManager
   * \param[in]       opt_sec_comm_mng
   *                  A secure communication manager.
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \param[in]       tcp_endpoint_type
   *                  Whether the endpoint is active or secured/not secured passive.
   * \param[in]       sec_com_config
   *                  The SecCom configuration if connection is passive and secured, empty otherwise.
   * \context         Init
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  explicit TcpEndpoint(
      Reactor* reactor, std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router,
      someip_daemon_core::IpAddress const& address, someip_protocol::internal::Port const port, Endpoint&& endpoint,
      someip_protocol::internal::SocketOptions const& options,
      someip_daemon::extension_points::message_authentication::MacInterface* mac_manager,
      ara::core::Optional<std::reference_wrapper<secure_communication::SecureCommunicationManager>> opt_sec_comm_mng,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
      TcpEndpointType const tcp_endpoint_type,
      configuration::ConfigurationTypesAndDefs::SecComConfig sec_com_config) noexcept;

  /*!
   * \brief           Called from the reactor when any of the monitored events is triggered. Can be called when
   *                  connection state changes from connecting to connected
   * \param[in]       reactor_handle
   *                  The registered handle that receives the notification
   * \param[in]       events
   *                  The type of received event(s)
   * \context         Reactor
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  void OnReactorCallback(osabstraction::io::reactor1::CallbackHandle const reactor_handle,
                         osabstraction::io::reactor1::EventTypes const events) noexcept;

  /*!
   * \brief           Closes a TCP connection
   * \brief           Called by OnReactorCallback if a HandleRead operation returns false. Checks current state of the
   *                  connection and does clean up if needed
   * \param[in]       connection
   *                  A TCP connection.
   * \param[in,out]   connection
   *                  A pointer to a TcpConnection
   * \context         Reactor
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  void OnHandleReadFailure(TcpConnection* connection) noexcept;

  /*!
   * \brief           Closes an active TCP connection
   * \param[in]       connection
   *                  An active TCP connection.
   * \context         ANY
   * \pre             Connection cannot be a passive connection.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void CloseActiveConnection(TcpConnection* connection) noexcept;

  /*!
   * \brief           Closes a passive TCP connection
   * \param[in]       connection
   *                  A TCP connection.
   * \context         ANY
   * \pre             Connection cannot be an active connection.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void ClosePassiveConnection(TcpConnection* connection) noexcept;

  /*!
   * \brief           Unregisters all TCP socket file descriptors belonging to the given TCP connection from event
   *                  notification
   * \param[in]       connection
   *                  A TCP connection.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void CleanUpConnectionEventHandlers(TcpConnection const* connection) noexcept;

  /*!
   * \brief           Looks up a TCP connection by its remote IP address and port number
   * \brief           Looks up an active TCP connection by its remote IP address and port number
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \return          An iterator pointing into TCP connection container.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpConnectionContainerIter GetActiveConnection(someip_protocol::internal::IpAddress const& address,
                                                 someip_protocol::internal::Port const port) noexcept;

  /*!
   * \brief           Looks up a passive TCP connection by its remote IP address and port number
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \return          An iterator pointing into TCP connection container.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpConnectionContainerIter GetPassiveConnection(someip_protocol::internal::IpAddress const& address,
                                                  someip_protocol::internal::Port const port) noexcept;

  /*!
   * \brief           Looks up a TCP connection by its remote IP address and port number
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \return          An iterator pointing into TCP connection container.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpConnectionContainerIter GetConnection(someip_protocol::internal::IpAddress const& address,
                                           someip_protocol::internal::Port const port) noexcept;

  /*!
   * \brief           Closes all passively established TCP connections
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void CloseAcceptedConnections() noexcept;

  /*!
   * \brief           Generates a unique logger prefix
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          a string representing the logger prefix.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  static ara::core::String GetLoggerPrefix(someip_protocol::internal::IpAddress const& address,
                                           someip_protocol::internal::Port const port) noexcept;

  /*!
   * \brief           The packet router (to which the received packets will be forwarded)
   */
  std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router_;

  /*!
   * \brief           A container of active TCP connections
   */
  TcpConnectionContainer active_connections_{};

  /*!
   * \brief           A container of passive TCP connections
   */
  TcpConnectionContainer passive_connections_{};

  /*!
   * \brief           Quality of service setting for this TCP endpoint
   */
  someip_protocol::internal::SocketOptions const socket_options_;
  // VECTOR Enable VectorC++-V11-0-2

  /*!
   * \brief           Releases a reference to the given active TCP connection
   * \param[in]       connection
   *                  A TCP connection.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void ReleaseActiveConnection(TcpConnection* connection) noexcept;

  /*!
   * \brief           Releases a reference to the given passive TCP connection
   * \param[in]       connection
   *                  A TCP connection.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void ReleasePassiveConnection(TcpConnection* connection) noexcept;

  /*!
   * \brief           Called upon a new passive connection accepted
   * \details         This function handles a possible error while accepting the connection. In case of success, calls
   *                  RegisterConnection()
   * \param[in]       accept_result
   *                  The result of accepting the connection
   * \context         Reactor
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void OnConnectionAccepted(ara::core::Result<AcceptedConnection>&& accept_result) noexcept;

  /*!
   * \brief           Creates a new TCP connection from the given connected TCP socket
   * \param[in]       accepted_connection
   *                  The object for the accepted connection.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \trace           SPEC-4980226, SPEC-4980227, SPEC-4980228, SPEC-4980230, SPEC-4980232
   * \steady          FALSE
   */
  void RegisterConnection(AcceptedConnection&& accepted_connection) noexcept;

  /*!
   * \brief           Reactor1 interface for Linux
   */
  Reactor* reactor_;

  /*!
   * \brief           Os layer endpoint
   */
  Endpoint endpoint_;

  /*!
   * \brief           A local IP address
   */
  someip_daemon_core::IpAddress const address_;

  /*!
   * \brief           A local port number
   */
  someip_protocol::internal::Port const port_;

  /*!
   * \brief           A map from a callback handle registered for notifications to a TCP connection
   */
  EventHandlerMap event_handlers_{};

  /*!
   * \brief           A container of registered provided SOME/IP service instances
   */
  ProvidedServiceInstanceContainer provided_service_instances_{};

  /*!
   * \brief           A TCP server
   */
  TcpServerPtr server_{nullptr};

  /*!
   * \brief           Counts how many times TCP server is in use
   */
  std::atomic<std::size_t> server_users_{0U};

  /*!
   * \brief           A MAC manager
   */
  someip_daemon::extension_points::message_authentication::MacInterface* mac_manager_;

  /*!
   * \brief           Friend test, to test OnReactorCallback()
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpEndpoint, OnReactorCallback_without_existing_event_handlers);

  /*!
   * \brief           Friend test, to test the ForwardPduFromLocal() function on TcpResponseSender
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpResponseSender, ForwardPduFromLocal);

  /*!
   * \brief           Friend test to test recoverable error handling for TcpEndpoint::OnConnectionAccepted
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpEndpoint, OnConnectionAccepted__RecoverableErrors);

  /*!
   * \brief           Friend test to test non-recoverable error handling for TcpEndpoint::OnConnectionAccepted
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpEndpoint, OnConnectionAccepted__NonRecoverableError);

  /*!
   * \brief           Friend test for OnHandleReadFailure(), OnReactorCallback(), and RegisterEventHandler()
   * \details         Used to access active_connections_
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpEndpoint, OnReactorCallback_OnHandleReadFailure_ActiveConnection);

  /*!
   * \brief           A statistics handler
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  /*!
   * \brief           Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief           The secure communication manager
   * \details         The TCP endpoint is not secure if and only if it is empty.
   */
  ara::core::Optional<std::reference_wrapper<secure_communication::SecureCommunicationManager>> const
      opt_secure_communication_manager_;

  /*!
   * \brief           Flag that determines if the endpoint is active or passive, and if the latter, if secured or not
   */
  TcpEndpointType const tcp_endpoint_type_;

  /*!
   * \brief           A SecCom configuration
   * \details         Only used for passive secured connections.
   */
  configuration::ConfigurationTypesAndDefs::SecComConfig const sec_com_config_;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_ENDPOINT_H_

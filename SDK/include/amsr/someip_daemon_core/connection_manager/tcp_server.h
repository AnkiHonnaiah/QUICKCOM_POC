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
 *        \brief  Class declaration for TCP server.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_SERVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/tcp.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/unique_ptr.h"
#include "ara/core/string.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class TcpEndpoint;

/*!
 * \brief           Class declaration of TcpServer
 */
class TcpServer final {
 public:
  /*!
   * \brief           A type alias for the Reactor1
   */
  using Reactor = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief           Type alias for server accepted connection from "libosabstraction"
   */
  using AcceptedConnection = amsr::net::ip::Tcp::Acceptor::AcceptedConnection;

  /*!
   * \brief           Function signature of action to be executed after a connection has been accepted
   */
  using ConnectionAcceptedActionType = vac::language::UniqueFunction<void(ara::core::Result<AcceptedConnection>&&)>;

  /*!
   * \brief           Constructor of TcpServer
   * \param[in]       reactor
   *                  A reactor.
   * \param[in]       endpoint
   *                  A reference to the TCP endpoint this server belongs to.
   * \context         Reactor
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TcpServer(Reactor* reactor, TcpEndpoint& endpoint) noexcept;

  TcpServer(TcpServer const&) = delete;
  TcpServer& operator=(TcpServer const&) = delete;
  TcpServer(TcpServer&&) = delete;
  TcpServer& operator=(TcpServer&&) = delete;

  /*!
   * \brief           Default destructor
   * \context         Reactor
   * \pre             The TCP server must be stopped
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~TcpServer() noexcept = default;

  /*!
   * \brief           Starts the TCP Server
   * \details         Opens an acceptor socket, binds to the local endpoint and starts listening. This function may
   *                  abort in case of development errors only.
   * \error           osabstraction::OsabErrc::kResource
   *                  No memory or resources to perform operation.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError
   *                  The Interface the socket is bound to cannot be identified uniquely.
   * \error           osabstraction::OsabErrc::kAddressError
   *                  The network interface does not have a physical address.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges
   *                  The process has insufficient privileges to create a socket.
   * \error           osabstraction::OsabErrc::kAddressNotAvailable
   *                  Cannot establish connection because local endpoint is already bound by another socket or the
   *                  operating system used up all ephemeral ports.
   *
   * \context         Reactor
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ara::core::Result<void> Initialize() noexcept;

  /*!
   * \brief           Starts accepting TCP connections
   * \details         This function may abort in case of development errors only.
   * \param[in]       on_connection_accepted
   *                  Callback to be triggered when a connection is accepted
   *
   * \error           osabstraction::OsabErrc::kResource
   *                  No memory to register another callback or system limit reached.
   *
   * \context         Reactor
   * \pre             Initialize() has been called
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \trace           SPEC-10144480
   * \steady          FALSE
   */
  ara::core::Result<void> StartAcceptingConnections(ConnectionAcceptedActionType&& on_connection_accepted) noexcept;

  /*!
   * \brief           Cleans up resources and closes acceptor socket
   * \context         Reactor
   * \pre             The TCP Server has been started
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void Stop() noexcept;

 private:
  /*!
   * \brief           A TCP server socket type
   */
  using ServerSocketAcceptor = amsr::net::ip::Tcp::Acceptor;

  /*!
   * \brief           Opens and configures the socket acceptor
   *
   * \error           osabstraction::OsabErrc::OsabErrc::kResource
   *                  No memory to register another callback or system limit reached.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError
   *                  The Interface the socket is bound to cannot be identified uniquely.
   * \error           osabstraction::OsabErrc::kAddressError
   *                  The network interface does not have a physical address.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges
   *                  The process has insufficient privileges to create a socket.
   * \error           osabstraction::OsabErrc::kAddressNotAvailable
   *                  Cannot establish connection because local endpoint is already bound by another socket or the
   *                  operating system used up all ephemeral ports.
   *
   * \context         Reactor
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ara::core::Result<void> InitializeSocket() noexcept;

  /*!
   * \brief           Generates a unique logger prefix
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          Returns a string representing the logger prefix.
   * \context         Reactor
   * \pre             -
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  static ara::core::String GetLoggerPrefix(someip_protocol::internal::IpAddress const& address,
                                           someip_protocol::internal::Port const port) noexcept;

  /*!
   * \brief           Registers the TCP server at the reactor to monitor read events
   *
   * \param[in]       on_connection_accepted
   *                  Callback to be triggered when a connection is accepted
   *
   * \error           osabstraction::OsabErrc::kApiError
   *                  A callback is already registered for NativeHandle or waiting for the specified events is not
   *                  possible for NativeHandle.
   * \error           osabstraction::OsabErrc::kResource
   *                  No memory to register another callback or system limit reached.
   *
   * \context         Reactor
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  ara::core::Result<void> RegisterReadEvents(ConnectionAcceptedActionType&& on_connection_accepted) noexcept;

  /*!
   * \brief           Starts accepting a connection and forward the server socket to OnAccept
   * \return          An ara::core::Result containing the status of connection acceptance.
   * \context         Reactor
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  ara::core::Result<amsr::net::ip::Tcp::Acceptor::AcceptedConnection> StartAcceptingConnection() noexcept;

  /*!
   * \brief           Aborts in case of development errors
   *
   * \param[in]       error
   *                  The Error code.
   *
   * \context         Reactor
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          FALSE
   */
  void FilterFatalDevelopmentErrors(ara::core::ErrorCode const error) noexcept;

  /*!
   * \brief           A reactor
   */
  Reactor* reactor_;

  /*!
   * \brief           A reference to the TCP endpoint this server belongs to
   */
  TcpEndpoint& endpoint_;

  /*!
   * \brief           A TCP server socket acceptor for incoming connections
   */
  amsr::UniquePtr<ServerSocketAcceptor> server_socket_unique_ptr_{
      memory::MemoryUtilities::CreateUniquePtr<ServerSocketAcceptor>()};

  /*!
   * \brief           A handle provided by the reactor when registering for notifications Needed to unregister for
   *                  notifications
   */
  osabstraction::io::reactor1::CallbackHandle reactor_handle_{};

  /*!
   * \brief           Callback to be triggered when a connection is accepted
   */
  ConnectionAcceptedActionType on_connection_accepted_{};

  /*!
   * \brief           The logger
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kPacketRouterLoggerContextId,
                                                    someip_daemon_core::logging::kPacketRouterLoggerContextDescription,
                                                    ara::core::StringView{"TCPServer"}};
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_SERVER_H_

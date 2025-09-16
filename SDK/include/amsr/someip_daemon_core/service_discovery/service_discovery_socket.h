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
/**        \file  service_discovery_socket.h
 *        \brief  SD Socket.
 *
 *      \details  This file contains Wrapper classes for UDP Sockets used for Service Discovery.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_SOCKET_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <utility>
#include "amsr/net/interface.h"
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/span.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

/*!
 * \brief Forward declaration ServiceDiscoveryEndpoint to avoid cyclic dependency.
 */
class ServiceDiscoveryEndpoint;

/*!
 * \brief Wrapper class for ara::core::Optional<amsr::net::ip::Udp::Socket>.
 * \trace SPEC-4981484
 */
class UDPSocket {
 public:
  /*!
   * \brief A type alias for the Reactor1
   */
  using Reactor = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief     Constructor of UDPSocket.
   * \param[in] address                     The socket's address.
   * \param[in] port                        The socket's port.
   * \param[in] service_discovery_endpoint  The instance of ServiceDiscoveryEndpoint this object belongs to.
   * \param[in] reactor                     The reactor to register with.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  UDPSocket(someip_daemon_core::IpAddress const& address, amsr::net::ip::Port const port,
            ServiceDiscoveryEndpoint& service_discovery_endpoint, Reactor* reactor);

  /*!
   * \brief     Destructor of UDPSocket. Unregisters socket from reactor thread and closes it.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual ~UDPSocket() noexcept;

  UDPSocket(UDPSocket const&) = delete;
  UDPSocket(UDPSocket&&) = delete;
  UDPSocket& operator=(UDPSocket const&) & = delete;
  UDPSocket& operator=(UDPSocket&&) & = delete;

  /*!
   * \brief     Obtain a reference to the wrapped instance of
   *            ara::core::Optional<amsr::net::ip::Udp::Socket>.
   * \pre       -
   * \return    A reference to the wrapped instance of ara::core::Optional<amsr::net::ip::Udp::Socket>.
   *
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  amsr::net::ip::Udp::Socket& GetDelegate() noexcept;

  /*!
   * \brief     Create, open and register with reactor thread the wrapped instance of
   *            ara::core::Optional<amsr::net::ip::Udp::Socket>.
   * \pre       -
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  void OpenSocket();

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief The wrapped instance of ara::core::Optional<amsr::net::ip::Udp::Socket>.
   */
  ara::core::Optional<amsr::net::ip::Udp::Socket> delegate_;

  /*!
   * \brief Communication endpoint needed to open socket.
   */
  ara::core::Optional<amsr::net::ip::Udp::Endpoint> unicast_endpoint_;

  /*!
   * \brief Network interface needed to open socket.
   */
  ara::core::Optional<amsr::net::Interface> network_interface_;

  /*!
   * \brief The socket's address.
   */
  someip_daemon_core::IpAddress const address_;
  /*!
   * \brief The socket's port.
   */
  amsr::net::ip::Port const port_;
  /*!
   * \brief The instance of ServiceDiscoveryEndpoint this object belongs to.
   */
  ServiceDiscoveryEndpoint& service_discovery_endpoint_;

  /*!
   * \brief Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"ServiceDiscoverySocket"}};

  // VECTOR Enable VectorC++-V11-0-2

  /*!
   * \brief     Callback to invoke just after the wrapped socket is created and opened.
   * \pre       Socket has been created and opened.
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual void OnSocketOpened() = 0;

  /*!
   * \brief     Create unicast endpoint and interface needed to open the socket.
   * \param[in] address The address for the created endpoint.
   * \param[in] port    The port for the created endpoint.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  void CreateEndpointAndInterface(someip_daemon_core::IpAddress const& address, amsr::net::ip::Port const port);

  /*!
   * \brief     Return a reference to the communication endpoint that holds the address and port information of the
   *            socket.
   * \pre       -
   * \return    A reference to the wrapped instance of ara::core::Optional<amsr::net::ip::Udp::Endpoint>.
   *
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual ara::core::Optional<amsr::net::ip::Udp::Endpoint>& GetEndpoint() = 0;

 private:
  /*!
   * \brief     Open the UDP socket.
   * \pre       -
   * \return    True if the UDP was opened successfully, false otherwise.
   *
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  bool OpenUDPSocket();

  /*!
   * \brief The reactor to register with.
   */
  Reactor* reactor_;

  /*!
   * \brief A handle provided by the reactor when registering for notifications
   *        Needed to unregister for notifications.
   *        Set when the UDP socket is opened.
   *        Reset when UDP socket is destroyed.
   */
  ara::core::Optional<osabstraction::io::reactor1::CallbackHandle> reactor_handle_{};
};

/*!
 * \brief Wrapper used to transmit unicast or multicast messages and receive unicast messages.
 */
class UnicastUDPSocket : public UDPSocket {
 public:
  /*!
   * \brief     Constructor of UnicastUDPSocket.
   *
   * \param[in] address                     The socket's address.
   * \param[in] port                        The socket's port.
   * \param[in] service_discovery_endpoint  The instance of ServiceDiscoveryEndpoint this object belongs to.
   * \param[in] reactor                     The reactor to register with.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  UnicastUDPSocket(someip_daemon_core::IpAddress const& address, amsr::net::ip::Port const port,
                   ServiceDiscoveryEndpoint& service_discovery_endpoint, Reactor* reactor);

  /*!
   * \brief     Set the network interface used for outgoing multicast packages as a socket option.
   * \details   Callback to invoke just after the wrapped socket is created and opened.
   * \pre       Socket has been created and opened.
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  void OnSocketOpened() override;

  /*!
   * \brief     Delegate to the SendToSync() method of the wrapped instance of amsr::net::ip::Udp::Socket.
   *
   * \param[in] buffer_view         A view of the buffer where the data to be sent is stored.
   * \param[in] remote_endpoint     The remote communication (unicast or multicast) endpoint to which
   *                                the passed data should be sent.
   * \pre       -
   * \return    True if sending was successful, false otherwise.
   *
   * \context   Timer
   * \reentrant FALSE
   * \steady TRUE
   */
  virtual ara::core::Result<void> Send(ara::core::Span<std::uint8_t> const& buffer_view,
                                       amsr::net::ip::Udp::Endpoint const& remote_endpoint);

  /*!
   * \brief     Return the native handle to use with a reactor (GetRawFiledescriptor).
   * \pre       -
   * \return    The native handle to use with a reactor.
   *
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  osabstraction::io::NativeHandle GetNativeHandle() noexcept;

  /*!
   * \brief     Return a reference to the communication endpoint that holds the address and port information of the
   *            socket.
   * \pre       -
   * \return    A reference to the wrapped instance of ara::core::Optional<amsr::net::ip::Udp::Endpoint>.
   *
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  ara::core::Optional<amsr::net::ip::Udp::Endpoint>& GetEndpoint() override;
};

/*!
 * \brief Wrapper used for receiving multicast messages.
 */
class MulticastUDPSocket : public UDPSocket {
 public:
  /*!
   * \brief Creates the unicast and multicast endpoints.
   * \param[in] multicast_address           The socket's multicast address.
   * \param[in] port                        The socket's port.
   * \param[in] unicast_address             Socket's unicast address.
   * \param[in] service_discovery_endpoint  The instance of ServiceDiscoveryEndpoint this object belongs to.
   * \param[in] reactor                     The reactor to register with.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  MulticastUDPSocket(someip_daemon_core::IpAddress const& multicast_address, amsr::net::ip::Port const port,
                     someip_daemon_core::IpAddress const& unicast_address,
                     ServiceDiscoveryEndpoint& service_discovery_endpoint, Reactor* reactor);

  /*!
   * \brief     Leaves the multicast group and destructs the MulticastUDPSocket.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady FALSE
   */
  ~MulticastUDPSocket() noexcept override;

  MulticastUDPSocket(MulticastUDPSocket const&) = delete;
  MulticastUDPSocket(MulticastUDPSocket&&) = delete;
  MulticastUDPSocket& operator=(MulticastUDPSocket const&) & = delete;
  MulticastUDPSocket& operator=(MulticastUDPSocket&&) & = delete;

  /*!
   * \brief     Joins the multicast group.
   * \details   Callback to invoke just after the wrapped socket is created and opened.
   * \pre       Socket has been created and opened.
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  void OnSocketOpened() override;

  /*!
   * \brief     Return a reference to the communication endpoint that holds the address and port information of the
   *            socket.
   * \pre       -
   * \return    A reference to the wrapped instance of ara::core::Optional<amsr::net::ip::Udp::Endpoint>.
   *
   * \context Init, Network, Timer
   * \reentrant FALSE
   * \steady FALSE
   */
  ara::core::Optional<amsr::net::ip::Udp::Endpoint>& GetEndpoint() override;

 private:
  /*!
   * \brief     Create the multicast endpoint containing the multicast information.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   */
  void CreateMulticastEndpoint();

  /*!
   * \brief The socket's multicast address.
   */
  ara::core::Optional<someip_daemon_core::IpAddress> multicast_address_{};

  /*!
   * \brief Communication endpoint needed to send to multicast address.
   */
  ara::core::Optional<amsr::net::ip::Udp::Endpoint> multicast_endpoint_{};
};

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_SOCKET_H_

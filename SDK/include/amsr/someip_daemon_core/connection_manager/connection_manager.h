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
 *        \brief  ConnectionManager class responsible for managing TCP / UDP connections.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>  // size_t
#include <memory>   // unique_ptr, shared_ptr
#include "amsr/net/ip/address.h"
#include "amsr/pdu-aggregation/internal/pdu_aggregator_interface.h"
#include "amsr/someip_daemon/extension_points/message_authentication/mac_interface.h"
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon/extension_points/tls/tls_extensions_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/connection_manager/connection_manager_initializer_interface.h"
#include "amsr/someip_daemon_core/connection_manager/meta_data_manager/meta_data_manager.h"
#include "amsr/someip_daemon_core/connection_manager/secure_communication/secure_communication_manager.h"
#include "amsr/someip_daemon_core/connection_manager/tcp_endpoint.h"
#include "amsr/someip_daemon_core/connection_manager/udp_endpoint.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/message_reader/datagram_message_reader.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief           Responsible for managing TCP/UDP connections
 * \vprivate        Vector component internal API
 */
class ConnectionManager : public ConnectionManagerInitializerInterface {
 public:
  /*!
   * \brief           Represents an invalid port number
   */
  static constexpr someip_protocol::internal::Port kInvalidPort{configuration::ConfigurationTypesAndDefs::kInvalidPort};

  /*!
   * \brief           A TCP sender pointer type
   */
  using TcpSenderPtr = amsr::UniquePtr<TcpSender>;

  /*!
   * \brief           A TCP receiver pointer type
   */
  using TcpReceiverPtr = amsr::UniquePtr<TcpReceiver>;

  /*!
   * \brief           A UDP sender pointer type
   */
  using UdpSenderPtr = amsr::UniquePtr<UdpSender>;

  /*!
   * \brief           A UDP receiver pointer type
   */
  using UdpReceiverPtr = amsr::UniquePtr<UdpReceiver>;

  /*!
   * \brief           A type alias for the Reactor1
   */
  using Reactor = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief           A TCP endpoint pointer type
   */
  using TcpEndpointPtr = std::shared_ptr<TcpEndpoint>;

  /*!
   * \brief           A UDP endpoint pointer type
   */
  using UdpEndpointPtr = std::shared_ptr<UdpEndpoint>;

  ConnectionManager() = delete;

  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief           Destructs the ConnectionManager
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  ~ConnectionManager() noexcept override = default;

  ConnectionManager(ConnectionManager const&) = delete;
  ConnectionManager(ConnectionManager&&) = delete;

  auto operator=(ConnectionManager const&) -> ConnectionManager& = delete;
  auto operator=(ConnectionManager&&) -> ConnectionManager& = delete;

  /*!
   * \brief           Constructs an instance of ConnectionManager
   * \param[in]       config
   *                  A configuration.
   * \param[in]       reactor
   *                  A reactor.
   * \param[in]       packet_router
   *                  A reference to the packet router(to which the received packets will be forwarded).
   * \param[in]       timer_manager
   *                  A timer manager.
   * \param[in]       mac_manager
   *                  A MAC manager.
   * \param[in]       statistics_handler
   *                  A reference to the statistics handler.
   * \param[in]       meta_data_manager
   *                  A meta data manager.
   * \param[in]       pdu_aggregator
   *                  A PDU aggregator.
   * \param[in]       tls_extensions_handlers
   *                  User implemented TLS extensions handlers.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  ConnectionManager(
      configuration::Configuration const* config, Reactor* reactor,
      std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router,
      amsr::steady_timer::TimerManagerInterface* timer_manager,
      someip_daemon::extension_points::message_authentication::MacInterface& mac_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
      meta_data_manager::MetaDataManager* meta_data_manager,
      amsr::pdu_aggregation::internal::PduAggregatorInterface* pdu_aggregator,
      ara::core::Vector<std::unique_ptr<someip_daemon::extension_points::tls::TlsExtensionsHandlerInterface>> const&
          tls_extensions_handlers) noexcept;

  /*!
   * \brief           Creates a communication network endpoint for use with TCP
   *
   * \param[in]       network_endpoint
   *                  The configuration network endpoint.
   * \param[in]       network_endpoint_port
   *                  The configuration network endpoint port.
   * \param[in]       is_secured
   *                  True if the endpoint should support using TLS; false for plain TCP.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void CreateTcpCommunicationNetworkEndpoint(ConfigNetworkEndpoint const& network_endpoint,
                                             ConfigNetworkEndpointPort const& network_endpoint_port,
                                             bool is_secured) noexcept override;

  /*!
   * \brief           Creates a communication network endpoint for use with UDP
   *
   * \param[in]       network_endpoint
   *                  The configuration network endpoint.
   * \param[in]       network_endpoint_port
   *                  The configuration network endpoint port.
   * \param[in]       is_secured
   *                  True if the endpoint should support using DTLS; false for plain UDP.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void CreateUdpCommunicationNetworkEndpoint(ConfigNetworkEndpoint const& network_endpoint,
                                             ConfigNetworkEndpointPort const& network_endpoint_port,
                                             bool is_secured) noexcept override;

  /*!
   * \brief           Returns a new TCP sender for an active TCP connection
   * \param[in]       from_address
   *                  A local IP address.
   * \param[in]       from_port
   *                  A local port number.
   * \param[in]       to_address
   *                  A remote IP address.
   * \param[in]       to_port
   *                  A remote port number.
   * \param[in]       sec_com_config
   *                  The SecCom configuration.
   * \return          Returns a result object containing TCP sender or error_code.
   * \error           osabstraction::OsabErrc::kApiError
   *                  if TCP connection cannot be created.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto GetActiveTcpSender(someip_protocol::internal::IpAddress const& from_address,
                          someip_protocol::internal::Port from_port,
                          someip_protocol::internal::IpAddress const& to_address,
                          someip_protocol::internal::Port to_port,
                          configuration::ConfigurationTypesAndDefs::SecComConfig const& sec_com_config) noexcept
      -> ara::core::Result<TcpSenderPtr>;

  /*!
   * \brief           Returns a new TCP sender for a passive TCP connection
   * \param[in]       from_address
   *                  A local IP address.
   * \param[in]       from_port
   *                  A local port number.
   * \param[in]       to_address
   *                  A remote IP address.
   * \param[in]       to_port
   *                  A remote port number.
   * \return          Returns a result object containing TCP sender or error_code.
   * \error           osabstraction::OsabErrc::kApiError
   *                  if TCP connection cannot be created.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto GetPassiveTcpSender(someip_protocol::internal::IpAddress const& from_address,
                           someip_protocol::internal::Port from_port,
                           someip_protocol::internal::IpAddress const& to_address,
                           someip_protocol::internal::Port to_port) noexcept -> ara::core::Result<TcpSenderPtr>;

  /*!
   * \brief           Returns a new passive TCP receiver
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          A unique pointer with a TCP receiver.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto GetPassiveTcpReceiver(someip_protocol::internal::IpAddress const& address,
                             someip_protocol::internal::Port port) noexcept -> TcpReceiverPtr;

  /*!
   * \brief           Indicates whether a passive TCP connection exist
   * \param[in]       from_address
   *                  A local IP address.
   * \param[in]       from_port
   *                  A local port number.
   * \param[in]       to_address
   *                  A remote IP address.
   * \param[in]       to_port
   *                  A remote port number.
   * \return          true if the specified passive TCP connection exists and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto HasTcpPassiveConnection(someip_protocol::internal::IpAddress const& from_address,
                               someip_protocol::internal::Port from_port,
                               someip_protocol::internal::IpAddress const& to_address,
                               someip_protocol::internal::Port to_port) noexcept -> bool;

  /*!
   * \brief           Returns a new UDP sender
   *
   * \param[in]       from_address
   *                  A local IP address.
   * \param[in]       from_port
   *                  A local port number.
   * \param[in]       to_address
   *                  A remote IP address.
   * \param[in]       to_port
   *                  A remote port number.
   * \param[in]       sec_com_config
   *                  A SecCom configuration.
   * \return          A new UDP sender or an error.
   *
   * \error           SomeIpDaemonErrc::error_not_ok
   *                  if the server was not successfully acquired.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetUdpSender(someip_protocol::internal::IpAddress const& from_address, someip_protocol::internal::Port from_port,
                    someip_protocol::internal::IpAddress const& to_address, someip_protocol::internal::Port to_port,
                    configuration::ConfigurationTypesAndDefs::SecComConfig const* sec_com_config) noexcept
      -> ara::core::Result<UdpSenderPtr>;

  /*!
   * \brief           Returns a new UDP receiver
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          Result containing a UDP receiver or an error.
   * \error           SomeIpDaemonErrc::error_not_ok
   *                  if the server was not successfully acquired.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetUdpUnicastReceiver(someip_protocol::internal::IpAddress const& address,
                             someip_protocol::internal::Port port) noexcept -> ara::core::Result<UdpReceiverPtr>;

  /*!
   * \brief           Getter function a new multicast UDP receiver
   * \param[in]       local_address
   *                  A local IP address.
   * \param[in]       multicast_address
   *                  A multicast IP address.
   * \param[in]       multicast_port
   *                  A multicast port.
   * \param[in]       endpoint
   *                  Network endpoint containing 'multicast_address' and 'multicast_port'.
   * \return          Result containing a UDP receiver or an error.
   * \error           SomeIpDaemonErrc::error_not_ok
   *                  if the server was not successfully acquired.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetUdpMulticastReceiver(someip_daemon_core::IpAddress const& local_address,
                               someip_daemon_core::IpAddress const& multicast_address,
                               someip_protocol::internal::Port const multicast_port,
                               amsr::net::ip::Udp::Endpoint endpoint) noexcept -> ara::core::Result<UdpReceiverPtr>;

  /*!
   * \brief           Looks up a passive TCP endpoint by its IP address and port number
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          An iterator pointing into TCP endpoint container.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto GetPassiveTcpEndpoint(someip_protocol::internal::IpAddress const& address,
                             someip_protocol::internal::Port const port) noexcept -> TcpEndpointPtr;

  /*!
   * \brief           Looks up a UDP endpoint by its IP address and port number
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          An iterator pointing into UDP endpoint container.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetUdpEndpoint(someip_protocol::internal::IpAddress const& address,
                      someip_protocol::internal::Port const port) noexcept -> UdpEndpointPtr;

 protected:
  /*!
   * \brief           A TCP endpoint container type
   */
  using TcpEndpointContainer = ara::core::Vector<TcpEndpointPtr>;

  /*!
   * \brief           A TCP endpoint container iterator type
   */
  using TcpEndpointContainerIter = TcpEndpointContainer::iterator;

  /*!
   * \brief           A UDP endpoint container type
   */
  using UdpEndpointContainer = ara::core::Vector<UdpEndpointPtr>;

  /*!
   * \brief           A UDP endpoint container iterator type
   */
  using UdpEndpointContainerIter = UdpEndpointContainer::iterator;

  /*!
   * \brief           Looks up an active TCP endpoint by its IP address and port number
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          An iterator pointing into TCP endpoint container or a nullopt.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto FindActiveTcpEndpoint(someip_protocol::internal::IpAddress const& address,
                             someip_protocol::internal::Port port) noexcept
      -> ara::core::Optional<TcpEndpointContainerIter>;

  /*!
   * \brief           Looks up a passive TCP endpoint by its IP address and port number
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          An iterator pointing into TCP endpoint container or a nullopt.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto FindPassiveTcpEndpoint(someip_protocol::internal::IpAddress const& address,
                              someip_protocol::internal::Port port) noexcept
      -> ara::core::Optional<TcpEndpointContainerIter>;

  /*!
   * \brief           Looks up a UDP endpoint by its IP address and port number
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          An iterator pointing into UDP endpoint container.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto FindUdpEndpoint(someip_protocol::internal::IpAddress const& address,
                       someip_protocol::internal::Port port) noexcept -> UdpEndpointContainerIter;

 private:
  /*!
   * \brief           Looks up all configured RequiredServiceInstance for a TCP endpoint by IP address and port
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          boolean indicates whether the endpoint is part of any RequiredServiceInstance.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto IsTcpEndpointActive(someip_daemon_core::IpAddress const& address,
                           someip_protocol::internal::Port port) const noexcept -> bool;

  /*!
   * \brief           Indicates whether a UDP endpoint is active
   *
   * \param[in]       address
   *                  The endpoint IP address.
   * \param[in]       port
   *                  The endpoint port number.
   * \return          True if the endpoint should be active; false otherwise.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto IsUdpEndpointActive(someip_daemon_core::IpAddress const& address,
                           someip_protocol::internal::Port port) const noexcept -> bool;

  /*!
   * \brief           Looks up a TCP endpoint by its IP address and port number
   * \param[in]       tcp_endpoints
   *                  A container of TCP endpoints.
   * \param[in]       address
   *                  A local IP address.
   * \param[in]       port
   *                  A local port number.
   * \return          An iterator pointing into TCP endpoint container or a nullopt.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto FindTcpEndpoint(TcpEndpointContainer& tcp_endpoints, someip_protocol::internal::IpAddress const& address,
                       someip_protocol::internal::Port port) noexcept -> ara::core::Optional<TcpEndpointContainerIter>;

  /*!
   * \brief           Determines the UDP message accumulation threshold for a network endpoint and port
   *
   * \param[in]       network_endpoint
   *                  A network endpoint.
   * \param[in]       multicast_port
   *                  A multicast port number.
   * \return          A UDP message accumulation threshold value.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  static auto GetUdpMessageAccumulationThreshold(ConfigNetworkEndpoint const& network_endpoint,
                                                 someip_protocol::internal::Port multicast_port) noexcept
      -> UdpMessageAccumulationThreshold;

  /*!
   * \brief           SomeIpd configuration
   */
  configuration::Configuration const* config_;

  /*!
   * \brief           SomeIpd reactor object
   */
  Reactor* reactor_;

  /*!
   * \brief           The reference to the packet router (to which the received packets will be forwarded)
   */
  std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router_;

  /*!
   * \brief           Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief           A timer manager
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief           A secure communication manager
   */
  secure_communication::SecureCommunicationManager secure_communication_manager_;

  /*!
   * \brief           A MAC manager
   */
  someip_daemon::extension_points::message_authentication::MacInterface& mac_manager_;

  /*!
   * \brief           A statistics handler
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;

  /*!
   * \brief           A meta data manager
   */
  meta_data_manager::MetaDataManager* meta_data_manager_;

  /*!
   * \brief           A PDU aggregator
   */
  amsr::pdu_aggregation::internal::PduAggregatorInterface* pdu_aggregator_;

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief           A container of active TCP endpoints
   */
  TcpEndpointContainer active_tcp_endpoints_{};
  /*!
   * \brief           A container of passive TCP endpoints
   */
  TcpEndpointContainer passive_tcp_endpoints_{};

  /*!
   * \brief           A container of UDP endpoints
   */
  UdpEndpointContainer udp_endpoints_{};
  // VECTOR Enable VectorC++-V11.0.2
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_MANAGER_H_

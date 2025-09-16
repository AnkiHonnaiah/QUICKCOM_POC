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
 *        \brief  Class declaration for UdpEndpoint
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_ENDPOINT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>   // atomic
#include <cstddef>  // size_t
#include <cstdint>  // uint64_t, uint32_t, int8_t
#include <memory>   // shared_from_this, shared_ptr
#include <utility>  // pair
#include "amsr/net/interface.h"
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/udp.h"
#include "amsr/pdu-aggregation/internal/pdu_aggregator_interface.h"
#include "amsr/someip_daemon/extension_points/message_authentication/mac_interface.h"
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_version.h"
#include "amsr/someip_daemon_core/connection_manager/meta_data_manager/meta_data_manager.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_receive_remapper.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_transmit_remapper_factory.h"
#include "amsr/someip_daemon_core/connection_manager/secure_communication/secure_communication_manager.h"
#include "amsr/someip_daemon_core/connection_manager/udp_connection.h"
#include "amsr/someip_daemon_core/connection_manager/udp_message_reception_handler.h"
#include "amsr/someip_daemon_core/connection_manager/udp_receiver.h"
#include "amsr/someip_daemon_core/connection_manager/udp_response_sender.h"
#include "amsr/someip_daemon_core/connection_manager/udp_sender.h"
#include "amsr/someip_daemon_core/connection_manager/udp_socket_reader.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/someip_daemon_core/message_reader/datagram_message_reader.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "amsr/someip_daemon_core/service_registry/required_service_instance_registry.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "someip-protocol/internal/message_builder.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_tp/someip_tp_reception_controller.h"
#include "someip_tp/someip_tp_segmentation_manager.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

namespace secure_communication {
class DtlsConnection;
}  // namespace secure_communication

/*!
 * \brief           Represents the different possible UDP endpoint variations
 */
enum class UdpEndpointType : std::int8_t {
  /*!
   * \brief           Corresponds to an "active" endpoint
   */
  kUnicastProvidesNothingRequiresAny,

  /*!
   * \brief           Corresponds to an unsecured "passive" endpoint
   */
  kUnicastProvidesUdpRequiresAny,

  /*!
   * \brief           Corresponds to a secured "passive" endpoint
   */
  kUnicastProvidesDtlsRequiresAny,

  /*!
   * \brief           A multicast endpoint
   */
  kMulticast,
};

/*!
 * \brief           Strong type for UDP message accumulation threshold values
 */
struct UdpMessageAccumulationThreshold final {
  /*!
   * \brief           The threshold value
   */
  std::size_t value;
};

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief           Represents a single UDP endpoint
 */
class UdpEndpoint final : public std::enable_shared_from_this<UdpEndpoint> {
 public:
  UdpEndpoint() = delete;

  /*!
   * \brief           Default destructor
   * \steady          UNKNOWN
   */
  ~UdpEndpoint() noexcept = default;

  UdpEndpoint(UdpEndpoint const&) = delete;
  UdpEndpoint(UdpEndpoint&&) = delete;

  auto operator=(UdpEndpoint const&) -> UdpEndpoint& = delete;
  auto operator=(UdpEndpoint&&) -> UdpEndpoint& = delete;

  /*!
   * \brief           Statistics counter
   */
  using StatisticsCounter = std::uint64_t;

  /*!
   * \brief           Various UDP endpoint statistics
   */
  struct Statistics {
    /*!
     * \brief           Received datagrams total
     */
    StatisticsCounter rx_datagrams_{0U};

    /*!
     * \brief           Received SOME/IP messages
     */
    StatisticsCounter rx_messages_{0U};

    /*!
     * \brief           Transmitted datagrams total
     */
    StatisticsCounter tx_datagrams_{0U};
  };

  /*!
   * \brief           A UDP sender pointer type
   */
  using UdpSenderPtr = amsr::UniquePtr<UdpSender>;

  /*!
   * \brief           A UDP receiver pointer type
   */
  using UdpReceiverPtr = amsr::UniquePtr<UdpReceiver>;

  /*!
   * \brief           A SOME/IP TP segmentation manager for UDP connections
   */
  using TransportProtocolSegmentationManager = UdpConnection::TransportProtocolSegmentationManager;

  /*!
   * \brief           A SOME/IP-TP re-assembler controller for UDP connections
   */
  using TransportProtocolReceptionController = someip_tp::TransportProtocolReceptionController;

  /*!
   * \brief           A type alias for the Reactor1
   */
  using Reactor = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief           A single datagram represented by multiple buffers (scatter-gather) and an endpoint
   */
  using Datagram = std::pair<ara::core::Span<osabstraction::io::ConstIOBuffer>, amsr::net::ip::Udp::Endpoint>;

  /*!
   * \brief           Shorthand alias for the primary configuration type
   */
  using Configuration = configuration::Configuration;

  /*!
   * \brief           Shorthand alias for the SecCom configuration type
   */
  using SecComConfig = configuration::ConfigurationTypesAndDefs::SecComConfig;

  /*!
   * \brief           Shorthand alias for the statistics handler interface
   */
  using StatisticsHandlerInterface = someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface;

  /*!
   * \brief           Shorthand alias for the MAC interface
   */
  using MacInterface = someip_daemon::extension_points::message_authentication::MacInterface;

  /*!
   * \brief           A SOME/IP message type
   */
  using SomeIpMessage = someip_protocol::internal::SomeIpMessage;

  /*!
   * \brief           A PDU message type
   */
  using PduMessage = someip_protocol::internal::PduMessage;

  /*!
   * \brief           Convenience alias for a bulk read receive period
   */
  using BulkReadReceivePeriod = someip_protocol::internal::BulkReadReceivePeriodUsType;

  /*!
   * \brief           Convenience alias for maximum consecutive bulk read attempt counts
   */
  using BulkReadMaxAttemptCount = someip_protocol::internal::BulkReadMaxNoOfConsecutiveReceiveCalls;

  /*!
   * \brief           Convenience alias for minimum bulk read datagram counts
   */
  using BulkReadMinDatagramCount = someip_protocol::internal::BulkReadMinNoOfDatagramsToContinueConsecutiveReceiveCalls;

  /*!
   * \brief           Creates a UDP endpoint
   *
   * \details         The provided combination of parameters is validated given the specified endpoint type.
   * \param[in]       endpoint_type
   *                  The endpoint variation type.
   * \param[in]       ip_address
   *                  A local or group IP address, depending on the endpoint type.
   * \param[in]       port
   *                  A local or multicast port number.
   * \param[in]       mtu
   *                  The maximum transmission unit (MTU) value.
   * \param[in]       mss
   *                  The maximum segment size (MSS) value.
   * \param[in]       configuration
   *                  The associated configuration, cannot be null.
   * \param[in]       reactor
   *                  The associated reactor, cannot be null.
   * \param[in]       packet_router
   *                  The associated packet router, cannot be null.
   * \param[in]       timer_manager
   *                  The associated timer manager, cannot be null.
   * \param[in]       statistics_handler
   *                  The associated statistics handler, cannot be null.
   * \param[in]       meta_data_manager
   *                  The associated meta data manager, cannot be null.
   * \param[in]       message_reader
   *                  The associated message reader, cannot be null.
   * \param[in]       secure_communication_manager
   *                  The associated secure communication manager, cannot be null.
   * \param[in]       sec_com_config
   *                  The associated secure communication configuration, can be empty.
   * \param[in]       pdu_aggregator
   *                  The associated PDU aggregator, can be null.
   * \param[in]       mac_manager
   *                  The associated MAC manager, can be null.
   * \param[in]       socket_options
   *                  The socket options.
   * \param[in]       network_endpoint
   *                  The associated network endpoint. If absent, this is initialized using the given IP address and
   *                  port.
   * \param[in]       accumulation_threshold
   *                  The UDP message accumulation threshold.
   * \param[in]       bulk_read_count
   *                  The bulk read limit.
   * \param[in]       bulk_read_receive_period
   *                  The bulk read receive period, if any.
   * \param[in]       bulk_read_max_attempts
   *                  The maximum number of consecutive bulk read attempts, if any.
   * \param[in]       bulk_read_min_datagrams
   *                  The minimum fetched datagrams in a bulk read attempt. Reading fewer datagrams than this value with
   *                  prevent further calls to the associated OS function.
   *
   * \context         Init
   * \pre             None of the pointers documented as being non-nullable can be null.
   * \pre             If type is kProvidesDtlsRequiresAny, then the SecCom configuration cannot be null.
   *
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  UdpEndpoint(UdpEndpointType endpoint_type, someip_daemon_core::IpAddress ip_address,
              someip_protocol::internal::Port port, MTU mtu, MSS mss, Configuration const* configuration,
              Reactor* reactor, std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router,
              amsr::steady_timer::TimerManagerInterface* timer_manager, StatisticsHandlerInterface* statistics_handler,
              meta_data_manager::MetaDataManager* meta_data_manager,
              amsr::UniquePtr<message_reader::DatagramMessageReader> message_reader,
              secure_communication::SecureCommunicationManager* secure_communication_manager,
              SecComConfig sec_com_config = SecComConfig{},
              amsr::pdu_aggregation::internal::PduAggregatorInterface* pdu_aggregator = nullptr,
              MacInterface* mac_manager = nullptr, someip_protocol::internal::SocketOptions socket_options = {},
              ara::core::Optional<amsr::net::ip::Udp::Endpoint> network_endpoint = {},
              UdpMessageAccumulationThreshold accumulation_threshold = {0U}, BulkReadCount bulk_read_count = {1U},
              ara::core::Optional<BulkReadReceivePeriod> bulk_read_receive_period = {},
              ara::core::Optional<BulkReadMaxAttemptCount> bulk_read_max_attempts = {},
              ara::core::Optional<BulkReadMinDatagramCount> bulk_read_min_datagrams = {}) noexcept;

  /*!
   * \brief           Returns the associated IP address
   *
   * \return          An IP address.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetAddress() const noexcept -> someip_daemon_core::IpAddress const&;

  /*!
   * \brief           Returns the associated message accumulation threshold
   *
   * \return          A message accumulation threshold value.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto GetMessageAccumulationThreshold() const noexcept -> std::size_t;

  /*!
   * \brief           Returns the local port number
   *
   * \return          A local port number.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetPort() const noexcept -> someip_protocol::internal::Port;

  /*!
   * \brief           Get the maximum allowed UDP payload (MTU - header overhead)
   *
   * \return          The payload.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetMaxAllowedUdpPayload() const noexcept -> MSS;

  /*!
   * \brief           Creates a new UDP sender
   *
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port_number
   *                  A remote port number.
   * \param[in]       sec_com_config
   *                  A SecCom configuration.
   * \return          The created UDP sender or an error.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   *
   * \trace           SPEC-10144565
   * \trace           SPEC-10144579
   * \steady          TRUE
   */
  auto GetSender(someip_protocol::internal::IpAddress const& address, someip_protocol::internal::Port port_number,
                 SecComConfig const* sec_com_config) noexcept -> ara::core::Result<UdpSenderPtr>;

  /*!
   * \brief           Returns a new UDP receiver
   *
   * \return          A UDP receiver or an error.
   *
   * \context         ANY
   * \pre             Must be called on a shared pointer UDP endpoint.
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetReceiver() noexcept -> ara::core::Result<UdpReceiverPtr>;

  /*!
   * \brief           Acquires a new reference to the given UDP connection
   *
   * \param[in]       connection
   *                  A UDP connection.
   * \return          True if the connection was acquired; false otherwise.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto AcquireConnection(UdpConnection* connection) noexcept -> bool;

  /*!
   * \brief           Releases a reference to the given UDP connection
   *
   * \param[in]       connection
   *                  A UDP connection.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ReleaseConnection(UdpConnection* connection) noexcept;

  /*!
   * \brief           Increments the number of server users
   *
   * \return          True if the server was acquired; false otherwise.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  bool AcquireServer() noexcept;

  /*!
   * \brief           Decreases the number of server users
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ReleaseServer() noexcept;

  /*!
   * \brief           Registers a required SOME/IP service instance
   *
   * \details         The endpoint argument is only used if this is unicast.
   * \param[in]       service_instance_id
   *                  The service instance id.
   * \param[in]       sender_endpoint
   *                  The sender's endpoint used as a key to index a registry.
   *
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void RegisterRequiredServiceInstance(configuration::types::SomeIpServiceInstanceIdCommunication service_instance_id,
                                       amsr::net::ip::Udp::Endpoint const& sender_endpoint) noexcept;

  /*!
   * \brief           Unregisters a required SOME/IP service instance
   *
   * \details         The endpoint argument is only used if this is unicast.
   * \param[in]       service_instance_id
   *                  The service instance id.
   * \param[in]       sender_endpoint
   *                  A sender's endpoint used as a key to index a registry.
   *
   * \context         Shutdown
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void UnregisterRequiredServiceInstance(configuration::types::SomeIpServiceInstanceIdCommunication service_instance_id,
                                         amsr::net::ip::Udp::Endpoint const& sender_endpoint) noexcept;

  /*!
   * \brief           Registers a provided SOME/IP service instance
   *
   * \param[in]       service_instance_id
   *                  The service instance id.
   *
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void RegisterProvidedServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication service_instance_id) noexcept;

  /*!
   * \brief           Unregisters a provided SOME/IP service instance
   *
   * \param[in]       service_instance_id
   *                  The service instance id.
   *
   * \context         Shutdown
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void UnregisterProvidedServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication service_instance_id) noexcept;

  /*!
   * \brief           Sends data to specified remote peer(s)
   *
   * \param[in]       datagrams
   *                  The datagrams bundled with the addresses to send to.
   * \return          True if the send operation was successful; false otherwise.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto Send(ara::core::Span<Datagram> datagrams) noexcept -> bool;

  /*!
   * \brief           Sends a method response to the remote source
   *
   * \param[in]       packet
   *                  A SOME/IP packet.
   * \param[in]       remote_source
   *                  A packet source which the SOME/IP method response shall be sent to.
   * \return          True if the response is forwarded to the remote application, otherwise false.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto SendMethodResponse(std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet,
                          packet_router::RemoteSourceIdentifier const& remote_source) noexcept -> bool;

  /*!
   * \brief           Processes a single received SOME/IP message
   *
   * \param[in]       message
   *                  A SOME/IP message.
   * \param[in]       from_endpoint
   *                  The communication endpoint of the SOME/IP message's sender.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ProcessMessage(SomeIpMessage&& message, amsr::net::ip::Udp::Endpoint const& from_endpoint) noexcept;

  /*!
   * \brief           Processes a single received PDU message
   *
   * \param[in]       message
   *                  A PDU message.
   * \param[in]       from_endpoint
   *                  The communication endpoint of the SOME/IP message's sender.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ProcessPduMessage(PduMessage&& message, amsr::net::ip::Udp::Endpoint const& from_endpoint) noexcept;

  /*!
   * \brief           Processes a received UdpSocketReader::Datagram
   *
   * \param[in]       datagram
   *                  The datagram to process.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ProcessDatagram(UdpSocketReader::Datagram& datagram) noexcept;

  /*!
   * \brief           Finds the instance id associated with the received SOME/IP message
   *
   * \param[in]       endpoint
   *                  The endpoint used as a key to index a registry.
   * \param[in]       header
   *                  SOME/IP header.
   * \return          An ara::core::result containing SOME/IP service instance id.
   *
   * \error           SomeIpReturnCode::kWrongInterface
   *                  ServiceId matches but the MajorVersion does not.
   * \error           SomeIpReturnCode::kUnknownService
   *                  Neither ServiceId nor MajorVersion matches.
   *
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto FindInstanceId(amsr::net::ip::Udp::Endpoint const& endpoint,
                      someip_protocol::internal::SomeIpMessageHeader const& header) const noexcept
      -> ara::core::Result<someip_protocol::internal::InstanceId, someip_protocol::internal::SomeIpReturnCode>;

  /*!
   * \brief           Finds the instance id associated with the received PDU message
   *
   * \param[in]       endpoint
   *                  The endpoint used as a key to index a registry.
   * \param[in]       header
   *                  A PDU header.
   * \return          The instance id for the provided header
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto FindPduInstanceId(amsr::net::ip::Udp::Endpoint const& endpoint,
                         someip_protocol::internal::PduMessageHeader header) const noexcept
      -> ara::core::Optional<someip_protocol::internal::InstanceId>;

  /*!
   * \brief           Performs actual processing of the input data source
   *
   * \param[in]       data_source
   *                  The data source containing the data received from the transport layer.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ProcessDataSource(UdpDataSource& data_source) noexcept;

  /*!
   * \brief           Returns the set of segmentation parameter for the specified SOME/IP message
   *
   * \param[in]       message_header
   *                  A SOME/IP message header.
   * \return          A set of segmentation parameters.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  auto GetSegmentationParametersFromConfig(someip_protocol::internal::SomeIpMessageHeader const& message_header)
      const noexcept -> someip_tp::SegmentationParameters;

  /*!
   * \brief           Build PDU receiver remapper
   *
   * \param[in]       service_id
   *                  A SOME/IP service id.
   * \param[in]       required_pdus
   *                  A list of required PDUs.
   *
   * \context         Init, Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void BuildPduReceptionMappings(
      someip_protocol::internal::ServiceId service_id,
      configuration::ConfigurationTypesAndDefs::SignalSerializedPduContainer const& required_pdus) const noexcept;

 private:
  /*!
   * \brief           Type alias for RemotePacketSinkSharedPtr
   */
  using RemotePacketSinkSharedPtr = packet_router::RemotePacketSinkSharedPtr;

  /*!
   * \brief           Maximum number of SOME/IP TP message segmentizers per endpoint
   */
  static constexpr std::size_t kMaxTpSegmentizers{100U};

  /*!
   * \brief           A UDP connection pointer type
   */
  using UdpConnectionPtr = amsr::UniquePtr<UdpConnection>;

  /*!
   * \brief           A UDP socket type
   */
  using Socket = amsr::net::ip::Udp::Socket;

  /*!
   * \brief           Represents a container of provided SOME/IP service instances
   */
  using ProvidedServiceInstanceContainer =
      ara::core::Map<configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication,
                     someip_protocol::internal::InstanceId>;

  /*!
   * \brief           Validates the endpoint according to the endpoint type, aborting if an invalid configuration is
   *                  detected
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ValidateConfiguration() const noexcept;

  /*!
   * \brief           Attempts to initialize the stored network endpoint
   *
   * \details         If no network endpoint is provided, then the local IP address and port will be used to initialize
   *                  the associated network endpoint.
   * \param[in]       network_endpoint
   *                  The requested network endpoint to use, if any.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void TryInitializeNetworkEndpoint(ara::core::Optional<amsr::net::ip::Udp::Endpoint> network_endpoint) noexcept;

  /*!
   * \brief           Attempts to initialize the network interface and multicast group address
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void TryInitializeNetworkInterface() noexcept;

  /*!
   * \brief           Opens UDP socket and registers it with the reactor
   *
   * \details         Socket is opened when UDP communication is required.
   * \return          True if the socket was successfully opened; false otherwise.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto OpenSocket() noexcept -> bool;

  /*!
   * \brief           Unregister UDP socket from the reactor thread and close it
   *
   * \details         Socket is closed when the connection is released.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void CloseSocket() noexcept;

  /*!
   * \brief           Set Multicast Network Interface option for the open socket
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void SetSocketOptMulticastNetworkInterface() const noexcept;

  /*!
   * \brief           Set Multicast Join Group for the open socket
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void SetSocketOptMulticastJoinGroup() const noexcept;

  /*!
   * \brief           Registers the endpoint for read notifications
   *
   * \details         This function registers a callback and enables monitoring or read notifications.
   * \param[in]       io_source
   *                  A UDP socket native handle.
   *
   * \error           osabstraction::OsabErrc::kApiError
   *                  A callback is already registered for NativeHandle or waiting for the specified events is not
   *                  possible for NativeHandle.
   * \error           osabstraction::OsabErrc::kResource
   *                  No memory to register another callback or system limit reached.
   *
   * \context         App
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          TRUE
   */
  void RegisterReadEventHandler(osabstraction::io::NativeHandle const& io_source) noexcept;

  /*!
   * \brief           Called from the reactor when any of the monitored events is triggered
   *
   * \param[in]       events
   *                  The type of received event(s).
   *
   * \context         App
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          TRUE
   */
  void OnReactorCallback(osabstraction::io::reactor1::EventTypes events) noexcept;

  /*!
   * \brief           Handles read events
   *
   * \context         App
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \steady          TRUE
   */
  void HandleRead() noexcept;

  /*!
   * \brief           Looks up a UDP connection by its remote IP address and port number
   *
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \return          A potentially null pointer to the found UDP connection.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetConnection(someip_daemon_core::IpAddress const& address, amsr::net::ip::Port port) const noexcept
      -> UdpConnection*;

  /*!
   * \brief           Creates a response sender from the endpoint and the connection defined by its remote IP address
   *                  and port number
   *
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \return          A UdpResponseSender shared pointer.
   *
   * \context         ANY
   * \pre             Must be called on a shared pointer UDP endpoint.
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto CreateResponseSender(someip_daemon_core::IpAddress const& address, amsr::net::ip::Port port) noexcept
      -> std::shared_ptr<UdpResponseSender>;

  /*!
   * \brief           Creates a multicast network endpoint
   *
   * \param[in]       address
   *                  The network endpoint multicast IP address.
   * \param[in]       port
   *                  The network endpoint port number.
   * \param[in]       endpoint
   *                  The local network endpoint that will provide the network interface.
   * \return          An ara::core::Result containing the network communication endpoint or an error.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  static auto CreateMulticastNetworkEndpoint(someip_daemon_core::IpAddress const& address, amsr::net::ip::Port port,
                                             amsr::net::ip::Udp::Endpoint const& endpoint) noexcept
      -> ara::core::Result<amsr::net::ip::Udp::Endpoint>;

  /*!
   * \brief           Sends an error response for a failed method request
   *
   * \param[in]       header
   *                  A SOME/IP header.
   * \param[in]       error_code
   *                  A return code to set in the SOME/IP header.
   * \param[in]       to
   *                  A packet sink to which the SOME/IP error response shall be sent.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void SendErrorResponse(someip_protocol::internal::SomeIpMessageHeader const& header,
                         someip_protocol::internal::SomeIpReturnCode error_code, RemotePacketSinkSharedPtr to) noexcept;

  /*!
   * \brief           Looks up the SOME/IP service instance identifier belonging to a registered SOME/IP service
   *                  instance
   *
   * \param[in]       service_deployment_id
   *                  The service deployment id, containing service id and major version.
   * \return          An ara::core::result containing SOME/IP service instance id.
   *
   * \error           SomeIpReturnCode::kWrongInterface
   *                  ServiceId matches but the MajorVersion does not.
   * \error           SomeIpReturnCode::kUnknownService
   *                  Neither ServiceId nor MajorVersion matches.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto GetProvidedServiceInstanceId(
      configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication const& service_deployment_id) const noexcept
      -> ara::core::Result<someip_protocol::internal::InstanceId, someip_protocol::internal::SomeIpReturnCode>;

  /*!
   * \brief           Performs actual processing of an input SOME/IP packet
   *
   * \param[in]       message
   *                  The SOME/IP message to be processed.
   * \param[in]       from_endpoint
   *                  The endpoint used when using UDP.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ProcessSomeIpPacket(message_reader::ProtocolMessage&& message,
                           amsr::net::ip::Udp::Endpoint const& from_endpoint) noexcept;

  /*!
   * \brief           Performs actual processing of an input PDU packet
   *
   * \param[in]       message
   *                  The PDU message to be processed.
   * \param[in]       from_endpoint
   *                  The endpoint used when using UDP.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ProcessPduPacket(message_reader::ProtocolMessage&& message,
                        amsr::net::ip::Udp::Endpoint const& from_endpoint) noexcept;

  /*!
   * \brief           Creates a new UDP or DTLS connection, stores it, and returns a pointer to it
   *
   * \param[in]       endpoint
   *                  The remote endpoint.
   * \param[in]       ip_address
   *                  The remote IP address.
   * \param[in]       port
   *                  The remote port.
   * \param[in]       sec_com_config
   *                  The SecCom configuration to use.
   * \param[in]       is_server
   *                  The role of the connection (server or client).
   * \return          A pointer to the new connection.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto CreateConnectionTo(amsr::net::ip::Udp::Endpoint const& endpoint, someip_daemon_core::IpAddress const& ip_address,
                          amsr::net::ip::Port port, SecComConfig const* sec_com_config, bool const is_server) noexcept
      -> UdpConnection*;

  /*!
   * \brief           Returns an existing DTLS connection for a given remote address
   *
   * \param[in]       from_endpoint
   *                  The remote communication endpoint.
   * \return          The associated DTLS connection.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   *
   * \trace           SPEC-4980226, SPEC-4980227, SPEC-4980229, SPEC-4980231, SPEC-4980232
   * \steady          FALSE
   */
  auto GetSecuredConnection(amsr::net::ip::Udp::Endpoint const& from_endpoint) const noexcept
      -> secure_communication::DtlsConnection&;

  /*!
   * \brief           Creates a UDP sender for a given connection
   *
   * \details         The sender will differ based on the presence of a secure communication manager.
   * \param[in]       connection
   *                  A UDP connection.
   * \return          The created UDP sender.
   *
   * \context         ANY
   * \pre             Must be called on a shared pointer UDP endpoint.
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  auto CreateUdpSender(UdpConnection* connection) noexcept -> amsr::UniquePtr<UdpSender>;

  /*!
   * \brief           Processes a datagram secured with DTLS
   *
   * \param[in]       datagram
   *                  A datagram secured using DTLS.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void ProcessSecuredDatagram(UdpSocketReader::Datagram const& datagram) const noexcept;

  /*!
   * \brief           Processes a single received SOME/IP-TP message
   *
   * \param[in]       msg
   *                  A SOME/IP-TP message.
   * \param[in]       from_endpoint
   *                  The communication endpoint of the SOME/IP-TP message's sender.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void ProcessSomeIpTpMessage(SomeIpMessage&& msg, amsr::net::ip::Udp::Endpoint const& from_endpoint) noexcept;

  /*!
   * \brief           The type of the endpoint
   */
  UdpEndpointType endpoint_type_;

  /*!
   * \brief           A local or group IP address
   */
  someip_daemon_core::IpAddress ip_address_;

  /*!
   * \brief           A port number
   */
  someip_protocol::internal::Port const port_;

  /*!
   * \brief           Maximum Segment Size: Allowed UDP payload
   */
  MSS const mss_;

  /*!
   * \brief           The associated logger
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief           The associated configuration, cannot be null
   */
  Configuration const* const config_;

  /*!
   * \brief           The associated reactor, cannot be null
   */
  Reactor* reactor_;

  /*!
   * \brief           The associated packet router (to which the received packets will be forwarded). Cannot be null
   */
  std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router_;

  /*!
   * \brief           The associated timer manager, cannot be null
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief           The associated statistics handler, cannot be null
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface* statistics_handler_;

  /*!
   * \brief           The associated meta data manager, cannot be null
   */
  meta_data_manager::MetaDataManager* meta_data_manager_;

  /*!
   * \brief           A SOME/IP message reader for datagram sockets, cannot be null
   */
  message_reader::DatagramMessageReader::Uptr message_reader_;

  /*!
   * \brief           The associated secure communication manager, may be null
   */
  secure_communication::SecureCommunicationManager* secure_communication_manager_;

  /*!
   * \brief           The associated secure communication configuration, if any
   */
  SecComConfig const sec_com_config_;

  /*!
   * \brief           The associated PDU aggregator, may be null
   */
  amsr::pdu_aggregation::internal::PduAggregatorInterface* pdu_aggregator_;

  /*!
   * \brief           The associated MAC manager, may be null
   */
  someip_daemon::extension_points::message_authentication::MacInterface* mac_manager_;

  /*!
   * \brief           The local network, if any
   */
  ara::core::Optional<amsr::net::Interface> network_interface_;

  /*!
   * \brief           An optional multicast group address, if any
   */
  ara::core::Optional<someip_daemon_core::IpAddress> multicast_group_address_;

  /*!
   * \brief           Allocator for creating error messages as response to invalid incoming request messages
   */
  someip_daemon_core::MemoryBufferAllocator error_message_memory_buffer_allocator_;

  /*!
   * \brief           Tracks the number of times UDP socket is in use
   */
  std::atomic<std::size_t> users_;

  /*!
   * \brief           A registry for required service instances
   */
  service_registry::RequiredServiceInstanceRegistry rsi_registry_;

  /*!
   * \brief           A container of registered provided SOME/IP service instances
   */
  ProvidedServiceInstanceContainer provided_service_instances_;

  /*!
   * \brief           Map containing instance id and SOME/IP-TP reception controller
   */
  ara::core::Map<amsr::someip_protocol::internal::InstanceId, TransportProtocolReceptionController> controllers_;

  /*!
   * \brief           A handle provided by the reactor when registering for notifications, if any
   * \details         Needed to unregister from notifications. Set when the UDP socket is opened, and reset when the UDP
   *                  socket is closed.
   */
  ara::core::Optional<osabstraction::io::reactor1::CallbackHandle> reactor_handle_;

  /*!
   * \brief           The factory used to build the remapper
   */
  amsr::UniquePtr<pdu_id_mappers::PduIdTransmitRemapperFactory> pdu_id_transmit_remapper_factory_;

  /*!
   * \brief           A PDU receive remapper
   * \details         This is used for remapping the PDU ID of the received message in case that it is a PDU to service
   *                  ID and event ID.
   */
  amsr::UniquePtr<connection_manager::pdu_id_mappers::PduIdReceiveRemapperInterface> pdu_id_receive_remapper_;

  /*!
   * \brief           The local network communication endpoint
   * \note            This is only used by multicast endpoints.
   */
  amsr::net::ip::Udp::Endpoint network_endpoint_;

  /*!
   * \brief           A SOME/IP TP segmentation manager
   */
  TransportProtocolSegmentationManager tp_segmentation_manager_;

  /*!
   * \brief           Used for reading datagrams from a data source (UDP socket)
   */
  UdpSocketReader socket_reader_;

  /*!
   * \brief           Used for processing received protocol messages
   */
  UdpMessageReceptionHandler message_reception_handler_;

  /*!
   * \brief           A container of UDP connections
   */
  ara::core::Vector<UdpConnectionPtr> connections_;

  /*!
   * \brief           A UDP socket
   */
  amsr::UniquePtr<Socket> socket_;

  /*!
   * \brief           A UPD socket data source, used for reading in datagrams from a UDP socket
   */
  amsr::UniquePtr<UdpDataSource> data_source_;

  /*!
   * \brief           Quality of service setting
   */
  someip_protocol::internal::SocketOptions socket_options_;

  /*!
   * \brief           Timer for handling periodic bulk reads
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> bulk_read_timer_;

  /*!
   * \brief           Socket configuration of bulk read receive period, if any
   */
  ara::core::Optional<BulkReadReceivePeriod> bulk_read_receive_period_option_;

  /*!
   * \brief           Cached message accumulation threshold
   */
  UdpMessageAccumulationThreshold const message_accumulation_threshold_;

  /*!
   * \brief           Max number of bulk reading attempts
   */
  ara::core::Optional<BulkReadMaxAttemptCount> bulk_read_max_no_of_consecutive_receive_calls_;

  /*!
   * \brief           Minimum number of fetched datagrams to stop bulk reading attempts
   */
  ara::core::Optional<BulkReadMinDatagramCount> bulk_read_min_no_of_datagrams_to_continue_consecutive_receive_calls_;

  /*!
   * \brief           A friend test to test ForwardPdu() for UdpResponseSender
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpResponseSender, ForwardPduFromLocalNotSupported);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, CloseSocket_ErrorHandling);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, ProcessPduPacket_ErrorHandling);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, ProcessPduPacket_AggregatePdu);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, ProcessPduPacket_AggregatePdu_Negative);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, ProcessSomeIpPacket_ErrorHandling);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, ProcessSomeIpPacket_ActiveSomeipTp);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, ProcessSomeIpPacket_MutlipleSender_SameMessageId);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, Send_ErrorHandling);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint, ProcessSomeIpPacket_MultipleSenders);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint__DeathTest, GetSecuredConnection_MissingConnection);

  FRIEND_TEST(UT__SomeIpd__ConnectionManager__UdpEndpoint__DeathTest, GetSecuredConnection_InsecureConnection);
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_ENDPOINT_H_

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
 *        \brief  Declaration of UdpConnection.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_CONNECTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <utility>
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon/extension_points/message_authentication/mac_interface.h"
#include "amsr/someip_daemon_core/connection_manager/connection.h"
#include "amsr/someip_daemon_core/connection_manager/message_accumulation.h"
#include "amsr/someip_daemon_core/connection_manager/udp_message_reception_handler_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/message_reader/datagram_message_reader.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "someip_tp/someip_tp_segmentation_manager.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class UdpEndpoint;
class UdpSender;

/*!
 * \brief           Declaration of UdpConnection
 */
class UdpConnection : public Connection<UdpSender> {
 public:
  /*!
   * \brief           Delete default constructor
   * \steady          TRUE
   */
  UdpConnection() = delete;

  /*!
   * \brief           Destructs instance UdpConnection
   * \context         Shutdown
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  ~UdpConnection() noexcept override;

  UdpConnection(UdpConnection const&) = delete;
  UdpConnection(UdpConnection&&) = delete;
  UdpConnection& operator=(UdpConnection const&) = delete;
  UdpConnection& operator=(UdpConnection&&) = delete;

  /*!
   * \brief           A UDP endpoint pointer type
   */
  using UdpEndpointPtr = typename std::add_pointer<UdpEndpoint>::type;

  /*!
   * \brief           A SOME/IP TP segmentation manager for UDP connections
   */
  using TransportProtocolSegmentationManager = someip_tp::TransportProtocolSegmentationManager;

  /*!
   * \brief           Constructor of UdpConnection
   * \param[in]       endpoint
   *                  A UDP endpoint this connection belongs to.
   * \param[in]       timer_manager
   *                  Timer manager to forward to the Accumulation unit.
   * \param[in]       reactor
   *                  Reactor used for triggering SW events for connection establishment.
   * \param[in]       tp_segmentation_manager
   *                  A SOME/IP TP segmentation manager or nullptr.
   * \param[in]       remote_endpoint
   *                  A remote network communication endpoint.
   * \param[in]       address
   *                  A remote IP address.
   * \param[in]       port
   *                  A remote port number.
   * \param[in]       mac_manager
   *                  A MAC manager.
   * \param[in]       reader
   *                  A datagram message reader.
   * \param[in]       message_reception_handler
   *                  A protocol message reception handler.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  UdpConnection(UdpEndpointPtr endpoint, amsr::steady_timer::TimerManagerInterface* timer_manager,
                osabstraction::io::reactor1::Reactor1Interface* reactor,
                TransportProtocolSegmentationManager* tp_segmentation_manager,
                amsr::net::ip::Udp::Endpoint remote_endpoint, someip_daemon_core::IpAddress const& address,
                amsr::net::ip::Port const port,
                someip_daemon::extension_points::message_authentication::MacInterface* mac_manager,
                message_reader::DatagramMessageReader& reader,
                UdpMessageReceptionHandlerInterface& message_reception_handler) noexcept;

  /*!
   * \brief           Indicates whether this connection is still alive
   * \return          Returns true indicating the UDP is connected.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  bool IsConnected() const noexcept override;

  /*!
   * \brief           Sends a SOME/IP message to remote peer, allowing for segmentation
   * \param[in]       packet
   *                  A SOME/IP message.
   * \return          Returns true If the message was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  bool Forward(std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet) noexcept;

  /*!
   * \brief           Send a PDU message to remote peer
   * \param[in]       packet
   *                  The PDU message.
   * \return          "true" if the message is sent successfully and "false" otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  bool ForwardPdu(std::shared_ptr<someip_protocol::internal::PduMessage> packet) noexcept;

  /*!
   * \brief           Increments the number of users of this UDP connection
   * \return          Returns the incremented current number of users.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  std::size_t Acquire() noexcept;

  /*!
   * \brief           Decrements the number of users of this UDP connection
   * \return          Returns the current number of users.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  std::size_t Release() noexcept;

  /*!
   * \brief           Notifies about closure of the underlying socket
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  virtual void OnSocketClosure() noexcept;

  /*!
   * \brief           Indicates whether the connection is secured
   *
   * \return          True if the connection uses secured communication; false otherwise.
   *
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  virtual auto IsSecure() const noexcept -> bool;

 protected:
  /*!
   * \brief           Callback for a SW event notification
   *
   * \context         Timer
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  virtual void HandleSwEvent() noexcept;

  /*!
   * \brief           An I/O vector container
   */
  using IovecContainer = ara::core::Vector<osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief           Generates a unique logger prefix
   *
   * \param[in]       local_address
   *                  A local IP address.
   * \param[in]       local_port
   *                  A local port number.
   * \param[in]       remote_address
   *                  A remote IP address.
   * \param[in]       remote_port
   *                  A remote port number.
   * \return          Returns a string representing the logger prefix.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  static ara::core::String GetLoggerPrefix(someip_protocol::internal::IpAddress const& local_address,
                                           someip_protocol::internal::Port local_port,
                                           someip_protocol::internal::IpAddress const& remote_address,
                                           someip_protocol::internal::Port remote_port) noexcept;

  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief           A UDP endpoint this connection belongs to
   */
  UdpEndpointPtr local_endpoint_;

  /*!
   * \brief           A network communication endpoint
   */
  amsr::net::ip::Udp::Endpoint remote_endpoint_;

 private:
  /*!
   * \brief           Generate MAC data and call SOME/IP-TP to segment the message
   * \param[in]       seg_params
   *                  Segmentation parameters configured for this message.
   * \param[out]      seg_result
   *                  Result of segmenting the message.
   * \param[in]       packet
   *                  SomeIpMessage to be secured with MAC Interface.
   * \context         ANY
   * \pre             MacInterface shall be configured.
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  void GenerateMacAndSegmentize(someip_tp::SegmentationParameters const& seg_params,
                                TransportProtocolSegmentationManager::ProcessMessageResult& seg_result,
                                std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet) noexcept;

  /*!
   * \brief           Call SOME/IP-TP to segment the message
   * \param[in]       seg_params
   *                  Segmentation parameters configured for this message.
   * \param[in]       message
   *                  SomeIpMessage to be segmented.
   * \return          ProcessMessageResult The result of segmentation.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  TransportProtocolSegmentationManager::ProcessMessageResult SegmentSomeIpTpMessage(
      someip_tp::SegmentationParameters const& seg_params,
      std::shared_ptr<someip_protocol::internal::SomeIpMessage> message) noexcept;

  /*!
   * \brief           Handle a packet before message accumulation
   * \param[in]       packet
   *                  The router message.
   * \return          true If the datagram was sent successfully, and false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  bool HandlePacketAccumulation(std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet) noexcept;

  /*!
   * \brief           Send datagrams to the wire
   * \param[in]       datagrams
   *                  Data to send. The 1st span level are datagrams. The 2nd level a scatter-gather view of a datagram.
   * \return          true If all the datagrams ware sent successfully. False otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  virtual bool HandleDatagrams(
      ara::core::Span<ara::core::Span<osabstraction::io::ConstIOBuffer>> const datagrams) noexcept;
  /*!
   * \brief           Send a single packet to the remote endpoint
   *
   * \param[in]       packet
   *                  Datagram to send.
   * \return          true if the whole datagram was sent correctly. Otherwise false.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  bool HandlePacket(std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet) noexcept;

  /*!
   * \brief           Counts how many times UDP connection is in use
   */
  std::atomic<std::size_t> users_;

  /*!
   * \brief           A reactor pointer used to trigger SW events for connection establishment
   */
  osabstraction::io::reactor1::Reactor1Interface* reactor_;

  /*!
   * \brief           A SOME/IP TP segmentation manager
   */
  TransportProtocolSegmentationManager* tp_segmentation_manager_;

  /*!
   * \brief           Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief           A MAC manager
   */
  someip_daemon::extension_points::message_authentication::MacInterface* mac_manager_;

  /*!
   * \brief           DatagramMessageReader used to read protocol messages from datagrams
   * \details         TODO(ARCTOR-5477): Implements usage for this variable.
   */
  message_reader::DatagramMessageReader& reader_;

  /*!
   * \brief           UdpMessageReceptionHandler used to process received protocol messages
   * \details         TODO(ARCTOR-5477): Implements usage for this variable.
   */
  UdpMessageReceptionHandlerInterface& message_reception_handler_;

  /*!
   * \brief           UDP message accumulation feature wrapper class
   */
  MessageAccumulation accumulation_;

  /*!
   * \brief           A vector to cache dynamic memory (io buffers)
   */
  ara::core::Vector<osabstraction::io::ConstIOBuffer> io_buffers_{};

  /*!
   * \brief           A vector to cache dynamic memory for calling the Datagram API
   */
  ara::core::Vector<std::pair<ara::core::Span<osabstraction::io::ConstIOBuffer>, amsr::net::ip::Udp::Endpoint>>
      datagram_api_buffers_{};

  /*!
   * \brief           A vector to cache dynamic memory for spans
   */
  ara::core::Vector<ara::core::Span<osabstraction::io::ConstIOBuffer>> datagrams_{};

  /*!
   * \brief           Handle for Reactor software event for connection establishment
   */
  osabstraction::io::reactor1::CallbackHandle sw_event_handle_connect_{};
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_CONNECTION_H_

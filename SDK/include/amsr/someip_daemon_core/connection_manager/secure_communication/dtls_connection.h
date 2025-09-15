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
 *        \brief  Represents a DTLS connection.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_DTLS_CONNECTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_DTLS_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/connection_manager/secure_communication/secure_communication_manager.h"
#include "amsr/someip_daemon_core/connection_manager/udp_connection.h"
#include "amsr/someip_daemon_core/connection_manager/udp_message_reception_handler_interface.h"
#include "amsr/someip_daemon_core/message_reader/datagram_message_reader.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "tls/public/tls_comm_callback_interfaces.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class UdpEndpoint;

namespace secure_communication {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_SomeIpDaemon_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief           DtlsConnection class
 */
class DtlsConnection final : public connection_manager::UdpConnection, public tls::TlsCommCallbackInterface {
 public:
  /*!
   * \brief           Raw data buffer view type
   */
  using BufferView = tls::ReadOnlyBufferView;

  /*!
   * \brief           Constructs an instance of DtlsConnection
   *
   * \param[in]       endpoint
   *                  A DTLS endpoint this connections belongs to.
   * \param[in]       timer_manager
   *                  A timer manager.
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
   * \param[in]       sec_com_config
   *                  A SecCom configuration.
   * \param[in]       secure_communication_manager
   *                  A secure communication manager.
   * \param[in]       reader
   *                  A datagram message reader.
   * \param[in]       message_reception_handler
   *                  A protocol message reception handler.
   *
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  DtlsConnection(UdpEndpoint* endpoint, amsr::steady_timer::TimerManagerInterface* timer_manager,
                 osabstraction::io::reactor1::Reactor1Interface* reactor,
                 TransportProtocolSegmentationManager* tp_segmentation_manager,
                 amsr::net::ip::Udp::Endpoint remote_endpoint, someip_daemon_core::IpAddress const& address,
                 amsr::net::ip::Port const port, configuration::Configuration::SecComConfig const* sec_com_config,
                 SecureCommunicationManager const& secure_communication_manager,
                 message_reader::DatagramMessageReader& reader,
                 UdpMessageReceptionHandlerInterface& message_reception_handler, bool const is_server) noexcept;

  /*!
   * \brief           Destructs an instance of DtlsConnection
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~DtlsConnection() noexcept final = default;

  DtlsConnection() = delete;
  DtlsConnection(DtlsConnection const&) = delete;
  DtlsConnection(DtlsConnection&&) = delete;
  DtlsConnection& operator=(DtlsConnection const&) = delete;
  DtlsConnection& operator=(DtlsConnection&&) = delete;

  /*!
   * \copydoc         connection_manager::UdpConnection::IsConnected
   * \steady          FALSE
   */
  bool IsConnected() const noexcept final;

  /*!
   * \copydoc         connection_manager::UdpConnection::HandleSwEvent
   * \steady          FALSE
   */
  void HandleSwEvent() noexcept final;

  /*!
   * \copydoc         connection_manager::UdpConnection::OnSocketClosure
   * \steady          FALSE
   */
  void OnSocketClosure() noexcept final;

  /*!
   * \brief           Handles data read from transport
   * \param[in]       datagram
   *                  A datagram to process.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void HandleRead(UdpSocketReader::Datagram const& datagram) noexcept;

  /*!
   * \copydoc         amsr::someip_daemon_core::connection_manager::UdpConnection::IsSecure
   * \steady          FALSE
   */
  auto IsSecure() const noexcept -> bool final;

 private:
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
  void WriteToCommParty(tls::ReadOnlyBufferView buffer) final;

  /*!
   * \copydoc         connection_manager::UdpConnection::HandleDatagrams()
   * \steady          FALSE
   */
  bool HandleDatagrams(
      ara::core::Span<ara::core::Span<osabstraction::io::ConstIOBuffer>> const datagrams) noexcept final;

  /*!
   * \brief           Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief           The associated SecCom configuration
   */
  configuration::Configuration::SecComConfig const* sec_com_config_;

  /*!
   * \brief           Determines whether the connection was established
   */
  bool is_connected_{false};

  /*!
   * \brief           Save the error state for communication providers
   */
  bool provider_has_error_{false};

  /*!
   * \brief           Reference to the underlying DTLS communication provider
   */
  TlsCommunicationProviderPtr const dtls_provider_;

  /*!
   * \brief           This keeps track of the sender of the currently processed datagram
   */
  amsr::net::ip::Udp::Endpoint currently_processing_from_{};

  /*!
   * \brief           A cached memory buffer
   */
  ara::core::Vector<uint8_t> datagram_buffer_{};
};

}  // namespace secure_communication
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_DTLS_CONNECTION_H_

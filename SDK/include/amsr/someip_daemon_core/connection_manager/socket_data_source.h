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
 *        \brief  Data source for UDP sockets.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SOCKET_DATA_SOURCE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SOCKET_DATA_SOURCE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include <utility>
#include "amsr/net/ip/tcp.h"
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon_core/connection_manager/data_source.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/string.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Data source for UDP sockets
 */
class UdpSocketDataSource final : public UdpDataSource {
 public:
  /*!
   * \brief           Constructor
   * \param[in]       socket
   *                  Reference to underlying socket for this data source.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  explicit UdpSocketDataSource(amsr::net::ip::Udp::Socket& socket) noexcept : UdpDataSource{}, socket_{socket} {}

  /*!
   * \copydoc         UdpDataSource::Receive
   * \internal
   * - Receive data from the bound-to socket synchronously.
   * - Check if the reception was successful, then copy out the datagram information.
   * - If the reception failed, log the error.
   * \endinternal
   * \steady          TRUE
   */
  amsr::net::ip::Udp::Socket::DatagramInfo Receive(
      ara::core::Span<std::uint8_t> const& target_message_buffer_view) noexcept final {
    amsr::net::ip::Udp::Socket::DatagramInfo datagram_info{};

    ara::core::Result<amsr::net::ip::Udp::Socket::DatagramInfo> receive_result{
        socket_.ReceiveFromSync(target_message_buffer_view)};

    if (receive_result.HasValue()) {
      datagram_info = receive_result.Value();
    } else {
      datagram_info.datagram_size = 0U;

      logger_.LogWarn(
          [&receive_result](ara::log::LogStream& s) noexcept {
            s << "Failed to receive message from remote peer: ";
            s << receive_result.Error().Value();
            s << ", User message: ";
            s << receive_result.Error().UserMessage();
            s << ", Support Data: ";
            s << receive_result.Error().SupportData();
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return datagram_info;
  }

  /*!
   * \brief           Receive one or multiple datagrams at once
   * \param[in,out]   target_datagram_ranges
   *                  Writable ranges for writing received datagram to and additional datagram information.
   * \return          The number of datagram received from the socket.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \internal
   * - Use the socket to receive multiple datagrams at once.
   * - Inspect for any error and log.
   * - Return the number of received datagrams or 0 in case of any error.
   * \endinternal
   * \steady          TRUE
   */
  std::size_t Receive(
      ara::core::Span<std::pair<ara::core::Span<std::uint8_t>, amsr::net::ip::Udp::Socket::DatagramInfo>> const&
          target_datagram_ranges) noexcept final {
    return socket_.ReceiveFromSyncBulk(target_datagram_ranges)
        .InspectError([this](ara::core::ErrorCode const error) noexcept {
          logger_.LogInfo(
              [&error, this](ara::log::LogStream& s) noexcept {
                s << "Failed to receive datagram(s) from remote peer(s): " << error << "\n";
                ara::core::Result<amsr::net::ip::Udp::Endpoint> const res_local_endpoint{
                    this->socket_.GetLocalEndpoint()};
                if (res_local_endpoint.HasValue()) {
                  ara::core::String const local_address{res_local_endpoint.Value().GetAddress().ToString()};
                  s << "Address: " << local_address;
                  std::uint16_t const local_port{res_local_endpoint.Value().GetPort().port};
                  s << " -- Port: " << local_port;
                } else {
                  s << "No local endpoint found: " << res_local_endpoint.Error();
                }
              },
              static_cast<char const*>(__func__), __LINE__);
        })
        .ValueOr(0U);
  }

 private:
  /*!
   * \brief           Socket representing the actual data source
   */
  amsr::net::ip::Udp::Socket& socket_;

  /*!
   * \brief           Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"SocketDataSource"}};
};

/*!
 * \brief           Data source for TCP sockets
 */
class TcpSocketDataSource final : public TcpDataSource {
 public:
  /*!
   * \brief           Constructs an instance of TcpSocketDataSource
   * \param[in]       socket
   *                  Reference to underlying socket for this data source.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  explicit TcpSocketDataSource(amsr::net::ip::Tcp::Socket& socket) noexcept : TcpDataSource{}, socket_{socket} {}

  /*!
   * \copydoc         TcpDataSource::ReceiveSync(ara::core::Span<std::uint8_t> const& buffer_view)
   * \steady          FALSE
   */
  ara::core::Result<std::size_t> ReceiveSync(ara::core::Span<std::uint8_t> const& buffer_view) noexcept final {
    return socket_.ReceiveSync(buffer_view);
  }

  /*!
   * \copydoc         TcpDataSource::ReceiveSync(ara::core::Span<MutableIOBuffer> const& io_buffers_view)
   * \steady          FALSE
   */
  ara::core::Result<std::size_t> ReceiveSync(ara::core::Span<MutableIOBuffer> const& io_buffers_view) noexcept final {
    return socket_.ReceiveSync(io_buffers_view);
  }

 private:
  /*!
   * \brief           Socket representing the actual data source
   */
  amsr::net::ip::Tcp::Socket& socket_;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SOCKET_DATA_SOURCE_H_

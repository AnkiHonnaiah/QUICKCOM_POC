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
 *        \brief  Abstraction for UDP data sources.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_DATA_SOURCE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_DATA_SOURCE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include <utility>
#include "amsr/net/ip/udp.h"
#include "ara/core/span.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Abstraction for Udp data sources
 */
class UdpDataSource {
 public:
  /*!
   * \brief           Default constructor
   * \steady          TRUE
   */
  UdpDataSource() noexcept = default;

  /*!
   * \brief           Define destructor
   * \steady          TRUE
   */
  virtual ~UdpDataSource() noexcept = default;

  UdpDataSource(UdpDataSource const &) = delete;
  UdpDataSource(UdpDataSource &&) = delete;
  UdpDataSource &operator=(UdpDataSource const &) = delete;
  UdpDataSource &operator=(UdpDataSource &&) = delete;

  /*!
   * \brief           Type alias for osabstraction::io::MutableIOBuffer
   */
  using MutableIOBuffer = osabstraction::io::MutableIOBuffer;

  /*!
   * \brief           Receives a datagram from a UDP socket
   *
   * \param[out]      target_message_buffer_view
   *                  A buffer view where the received data will be stored.
   *
   * \return          The length of the received datagram.
   * \context         Reactor
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  virtual amsr::net::ip::Udp::Socket::DatagramInfo Receive(
      ara::core::Span<std::uint8_t> const &target_message_buffer_view) noexcept = 0;

  /*!
   * \brief           Receive one or multiple datagrams at once
   * \param[in,out]   target_datagram_ranges
   *                  Writable ranges for writing received datagram to and additional datagram information.
   * \return          The number of datagram received from the socket.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  virtual std::size_t Receive(
      ara::core::Span<std::pair<ara::core::Span<std::uint8_t>, amsr::net::ip::Udp::Socket::DatagramInfo>> const
          &target_datagram_ranges) noexcept = 0;
};

/*!
 * \brief           Abstraction for Tcp data sources
 */
class TcpDataSource {
 public:
  /*!
   * \brief           Default constructor
   * \steady          FALSE
   */
  TcpDataSource() noexcept = default;

  /*!
   * \brief           Define destructor
   * \steady          FALSE
   */
  virtual ~TcpDataSource() noexcept = default;

  /*!
   * \brief           Delete copy constructor
   * \steady          FALSE
   */
  TcpDataSource(TcpDataSource const &) = delete;

  /*!
   * \brief           Delete move constructor
   * \steady          FALSE
   */
  TcpDataSource(TcpDataSource &&) = delete;

  /*!
   * \brief           Default copy assignment
   * \steady          FALSE
   */
  TcpDataSource &operator=(TcpDataSource const &) = delete;

  /*!
   * \brief           Default move assignment
   * \steady          FALSE
   */
  TcpDataSource &operator=(TcpDataSource &&) = delete;

  /*!
   * \brief           Type alias for osabstraction::io::MutableIOBuffer
   */
  using MutableIOBuffer = osabstraction::io::MutableIOBuffer;

  /*!
   * \brief           Returns received data from a TCP socket synchronously
   *
   * \param[out]      buffer_view
   *                  A buffer view where the received data will be stored.
   * \return          The length of the received data.
   * \context         Reactor
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          FALSE
   */
  virtual ara::core::Result<std::size_t> ReceiveSync(ara::core::Span<std::uint8_t> const &buffer_view) noexcept = 0;

  /*!
   * \brief           Returns received data from a TCP socket synchronously
   *
   * \param[out]      io_buffers_view
   *                  A mutable IO buffer where the received data will be stored.
   * \return          The length of the received data.
   * \context         Reactor
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          FALSE
   */
  virtual ara::core::Result<std::size_t> ReceiveSync(
      ara::core::Span<MutableIOBuffer> const &io_buffers_view) noexcept = 0;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_DATA_SOURCE_H_

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
 *        \brief  Class declaration for BufferDataSource
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_BUFFER_DATA_SOURCE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_BUFFER_DATA_SOURCE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <tuple>
#include <utility>
#include "amsr/someip_daemon_core/connection_manager/data_source.h"
#include "ara/core/span.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Non-owning consumable UDP data source based on a buffer view
 */
class UdpBufferDataSource final : public UdpDataSource {
 public:
  /*!
   * \brief           Buffer view type
   */
  using BufferView = ara::core::Span<std::uint8_t const>;

  /*!
   * \brief           Constructs instance of BufferDataSource
   * \param[in]       buffer_view
   *                  View on the underlying data buffer for this source.
   * \param[in]       remote_endpoint
   *                  Communication endpoint reference of the remote peer from which the data has been received.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  UdpBufferDataSource(BufferView buffer_view, amsr::net::ip::Udp::Endpoint const& remote_endpoint) noexcept
      : UdpDataSource{}, buffer_view_{buffer_view}, remote_endpoint_{remote_endpoint} {}

  /*!
   * \copydoc         UdpDataSource::Receive
   * \steady          TRUE
   */
  amsr::net::ip::Udp::Socket::DatagramInfo Receive(
      ara::core::Span<std::uint8_t> const& target_message_buffer_view) noexcept final {
    std::size_t const received_bytes{std::min(target_message_buffer_view.size(), buffer_view_.size())};

    amsr::net::ip::Udp::Socket::DatagramInfo datagram_info{};

    static_cast<void>(std::copy(buffer_view_.begin(),
                                std::next(buffer_view_.begin(), static_cast<ptrdiff_t>(received_bytes)),
                                target_message_buffer_view.begin()));

    // Update internal buffer view to represent amount of data consumed.
    buffer_view_ = buffer_view_.subspan(received_bytes);

    datagram_info.remote_endpoint = remote_endpoint_;
    datagram_info.datagram_size = received_bytes;

    return datagram_info;
  }

  /*!
   * \brief           Receive one or multiple datagrams at once
   * \param[in,out]   target_datagram_ranges
   *                  Writable ranges for writing received datagram to and additional datagram information.
   * \return          The number of datagrams received from the socket.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  std::size_t Receive(
      ara::core::Span<std::pair<ara::core::Span<std::uint8_t>, amsr::net::ip::Udp::Socket::DatagramInfo>> const&
          target_datagram_ranges) noexcept final {
    return target_datagram_ranges.size();
  }

  /*!
   * \brief           Checks if this data source is empty
   * \return          "true" if all byte have been consumed and "false" otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  bool Empty() const noexcept { return buffer_view_.empty(); }

 private:
  /*!
   * \brief           Buffer view onto the actual data source
   */
  BufferView buffer_view_;

  /*!
   * \brief           Remote endpoint encapsulating address and port
   */
  amsr::net::ip::Udp::Endpoint const& remote_endpoint_;
};

/*!
 * \brief           Non-owning consumable TCP data source based on a buffer view
 */
class TcpBufferDataSource final : public TcpDataSource {
 public:
  /*!
   * \brief           Buffer view type
   */
  using BufferView = ara::core::Span<std::uint8_t const>;

  /*!
   * \brief           Constructor
   * \param[in]       buffer_view
   *                  View on the underlying data buffer for this source.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          FALSE
   */
  explicit TcpBufferDataSource(BufferView buffer_view) noexcept : TcpDataSource{}, buffer_view_{buffer_view} {}

  /*!
   * \brief           Returns received data
   *
   * \param[out]      buffer
   *                  A buffer where the received data is stored.
   * \return          The length of the received data.
   * \context         Reactor
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          FALSE
   */
  std::size_t Receive(ara::core::Span<std::uint8_t> buffer) noexcept {
    std::size_t const received_bytes{std::min(buffer.size(), buffer_view_.size())};

    static_cast<void>(std::copy(
        buffer_view_.begin(), std::next(buffer_view_.begin(), static_cast<ptrdiff_t>(received_bytes)), buffer.begin()));

    // Update internal buffer view to represent amount of data consumed.
    buffer_view_ = buffer_view_.subspan(received_bytes);

    return received_bytes;
  }

  /*!
   * \copydoc         TcpDataSource::ReceiveSync(ara::core::Span<std::uint8_t> const& buffer_view)
   * \steady          FALSE
   */
  ara::core::Result<std::size_t> ReceiveSync(ara::core::Span<std::uint8_t> const& buffer_view) noexcept final {
    std::size_t n{std::min(buffer_view.size(), buffer_view_.size())};
    std::memcpy(buffer_view.begin(), buffer_view_.data(), n);

    // Update internal buffer view to represent amount of data consumed.
    buffer_view_ = buffer_view_.subspan(n);

    return ara::core::Result<std::size_t>::FromValue(n);
  }

  /*!
   * \copydoc         TcpDataSource::ReceiveSync(ara::core::Span<MutableIOBuffer> const& io_buffers_view)
   * \steady          FALSE
   */
  ara::core::Result<std::size_t> ReceiveSync(ara::core::Span<MutableIOBuffer> const& io_buffers_view) noexcept final {
    using const_iterator = ara::core::Span<MutableIOBuffer>::const_iterator;
    std::size_t n{0U};

    // forward to other receive overload to fill each scattered buffer.
    for (const_iterator it{io_buffers_view.begin()}; it < io_buffers_view.end(); ++it) {
      // VECTOR NL AutosarC++17_10-M5.2.8: MD_SomeIpDaemon_AutosarC++17_10-M5.2.8_conversion_void_int_prt
      n += Receive(ara::core::Span<std::uint8_t>{static_cast<std::uint8_t*>(it->base_pointer), it->size});
    }

    return ara::core::Result<std::size_t>::FromValue(n);
  }

  /*!
   * \brief           Checks if this data source is empty
   * \return          True if all byte have been consumed, otherwise false.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  bool Empty() const noexcept { return buffer_view_.empty(); }

 private:
  /*!
   * \brief           Buffer view onto the actual data source
   */
  BufferView buffer_view_;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_BUFFER_DATA_SOURCE_H_

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
 *        \brief  Reader for reading UDP datagrams from a UDP data source.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_SOCKET_READER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_SOCKET_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <limits>
#include <memory>
#include <utility>
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/connection_manager/data_source.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/message.h"
#include "vac/container/static_vector.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Strong type for MTU values
 */
struct MTU final {
  /*!
   * \brief           Maximum transmission unit value
   */
  std::size_t value;
};

/*!
 * \brief           Strong type for MSS values
 */
struct MSS final {
  /*!
   * \brief           The maximum segment size (MSS) value, i.e., the maximum allowed UDP payload
   */
  std::size_t value;
};

/*!
 * \brief           Strong type for bulk read count values
 */
struct BulkReadCount final {
  /*!
   * \brief           The bulk read count value
   */
  std::uint32_t value;
};

/*!
 * \brief           The UdpSocketReader provides an API to read datagrams from a data source in order to provide a range
 *                  of received datagrams which can be processed by the API user
 */
class UdpSocketReader final {
 public:
  /*!
   * \brief           Constructor of UdpSocketReader
   * \vprivate        Vector component private API
   * \param[in]       mtu
   *                  Maximum length of a datagram, a.k.a. maximum transmission unit.
   * \param[in]       bulk_read_count
   *                  The number of datagrams the buffer may contain at once. Default is 1.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \steady          TRUE
   */
  explicit UdpSocketReader(MTU const mtu, BulkReadCount const bulk_read_count = {1U}) noexcept;

  /*!
   * \brief           Represents a Datagram which contains one or more protocol messages
   */
  class Datagram final {
   private:
    /*!
     * \brief           The buffer range where this datagram is written to
     */
    ara::core::Span<std::uint8_t> datagram_buffer_{};
    /*!
     * \brief           Contains context information about a datagram, the actual length and from where it was
     *                  originally sent
     */
    amsr::net::ip::Udp::Socket::DatagramInfo datagram_info_{};
    /*!
     * \brief           The number of already processed bytes of this datagram
     */
    std::size_t processed_bytes_offset_{0U};

   public:
    /*!
     * \brief           Constructs a Datagram for the given buffer range
     * \param[in]       buffer
     *                  A view on the memory buffer where a single datagram is written to.
     * \context         ANY
     * \pre             -
     * \threadsafe      TRUE, for different this pointer
     * \steady          TRUE
     */
    explicit Datagram(ara::core::Span<std::uint8_t> buffer) noexcept : datagram_buffer_{buffer} {}

    /*!
     * \brief           Updates this datagram with information about a newly received datagram which was written to the
     *                  buffer
     * \param[in]       datagramInfo
     *                  Information about the new datagram
     * \context         ANY
     * \pre             -
     * \threadsafe      TRUE, for different this pointer
     * \steady          TRUE
     */
    void OnNewDatagramReceived(amsr::net::ip::Udp::Socket::DatagramInfo const& datagramInfo) noexcept;

    /*!
     * \brief           Get a data range for processing from the datagram buffer
     * \details         Calling this method will increment the offset of processed bytes by the requested number of
     *                  bytes.
     * \param[in]       bytes
     *                  The number of bytes request for processing.
     * \return          A span starting from the current offset with a size of the requested number of bytes.
     * \context         ANY
     * \pre             -
     * \threadsafe      TRUE, for different this pointer
     * \steady          TRUE
     */
    ara::core::Span<std::uint8_t> ProcessBytes(std::size_t const bytes) noexcept;

    /*!
     * \brief           Get a span of this datagram starting from the current offset with the specified number of bytes
     * \param[in]       bytes
     *                  The number of bytes this span shall have.
     * \return          A read only buffer view.
     * \context         ANY
     * \pre             -
     * \threadsafe      TRUE, for different this pointer
     * \steady          TRUE
     */
    ara::core::Span<std::uint8_t> const GetViewFromCurrentOffset(std::size_t const bytes) const noexcept;

    /*!
     * \brief           Returns the number of bytes still available for processing. If the current offset exceeds the
     *                  actual length of the stored datagram 0 is returned
     * \return          0 or the remaining number of bytes left for processing.
     * \context         ANY
     * \pre             -
     * \threadsafe      TRUE, for different this pointer
     * \steady          TRUE
     */
    std::size_t BytesStillRemaining() const noexcept;

    /*!
     * \brief           Invalidates the datagram by moving the current offset to the end of the actual datagram
     * \steady          TRUE
     */
    void Invalidate() noexcept;

    /*!
     * \brief           Get the original senders endpoint of this datagram
     * \return          The remote endpoint which sent the datagram to us.
     * \context         ANY
     * \pre             -
     * \threadsafe      TRUE, for different this pointer
     * \steady          TRUE
     */
    amsr::net::ip::Udp::Endpoint From() const noexcept;

    /*!
     * \brief           Get the size of this datagram
     * \return          The size of the datagram which is sent to us.
     * \context         ANY
     * \pre             -
     * \threadsafe      TRUE, for different this pointer
     * \steady          TRUE
     */
    std::size_t GetDatagramSize() const noexcept;
  };

  /*!
   * \brief           Not copy constructible
   * \steady          TRUE
   */
  UdpSocketReader(UdpSocketReader const&) = delete;

  /*!
   * \brief           Not move constructible
   * \steady          TRUE
   */
  UdpSocketReader(UdpSocketReader&&) = delete;

  /*!
   * \brief           Not copy assignable
   * \steady          TRUE
   */
  UdpSocketReader& operator=(UdpSocketReader const&) = delete;

  /*!
   * \brief           Not move assignable
   * \steady          TRUE
   */
  UdpSocketReader& operator=(UdpSocketReader&&) = delete;

  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  ~UdpSocketReader() noexcept = default;

  /*!
   * \brief           Reads datagrams from the provided data source and provides a range object to read datagrams
   * \vprivate        Vector component private API
   * \param[in,out]   data_source
   *                  A reference to a UDP data source from which zero or more datagrams will be read.
   * \return          Container with references for all read datagrams.
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \steady          TRUE
   */
  ara::core::Vector<std::reference_wrapper<UdpSocketReader::Datagram>> Read(
      connection_manager::UdpDataSource& data_source) noexcept;

 private:
  /*!
   * \brief           A helper function for reading zero or one datagram from the provided data source
   * \param[in,out]   data_source
   *                  A reference to a UDP data source.
   * \return          Container with references for all read datagrams.
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \steady          TRUE
   */
  ara::core::Vector<std::reference_wrapper<UdpSocketReader::Datagram>> ReadSingleDatagramFromDataSource(
      connection_manager::UdpDataSource& data_source) noexcept;

  /*!
   * \brief           A helper function for reading zero or more datagrams from the provided data source
   * \param[in,out]   data_source
   *                  A reference to a UDP data source.
   * \return          Container with references for all read datagrams.
   * \context         Network
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \synchronous     FALSE
   * \steady          TRUE
   */
  ara::core::Vector<std::reference_wrapper<UdpSocketReader::Datagram>> ReadMultipleDatagramFromDataSource(
      connection_manager::UdpDataSource& data_source) noexcept;

  /*!
   * \brief           MTU of a datagram
   */
  std::size_t const mtu_;

  /*!
   * \brief           The number of datagrams the allocated buffer can contain. Defaulted to 1
   */
  std::size_t const no_of_datagrams_;

  /*!
   * \brief           Pair consisting of a writable span for datagram storage and context information for the stored
   *                  datagram
   */
  using DatagramBufferAndInfo = std::pair<ara::core::Span<std::uint8_t>, amsr::net::ip::Udp::Socket::DatagramInfo>;

  /*!
   * \brief           Vector storing the writable memory spans of the continuous buffer used when reading in multiple
   *                  datagrams
   * \details         Memory is reserved once during construction.
   */
  vac::container::StaticVector<DatagramBufferAndInfo> datagrams_ranges_{};

  /*!
   * \brief           Vector storing one or multiple Datagram instances
   * \details         Memory is reserved once during construction.
   */
  vac::container::StaticVector<Datagram> datagrams_{};

  /*!
   * \brief           Static continuous buffer used for storing one or multiple UDP datagrams
   * \details         Memory is reserved once during construction.
   */
  vac::container::StaticVector<std::uint8_t> datagram_buffer_{};
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_SOCKET_READER_H_

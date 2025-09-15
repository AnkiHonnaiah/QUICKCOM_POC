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
/**        \file  someip_stream_message_writer.h
 *        \brief  SOME/IP stream message writer.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_WRITER_SOMEIP_STREAM_MESSAGE_WRITER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_WRITER_SOMEIP_STREAM_MESSAGE_WRITER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <array>
#include <deque>
#include <limits>
#include <memory>
#include <utility>

#include "amsr/net/ip/tcp.h"
#include "amsr/someip_daemon_core/connection_manager/data_source.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/memory_resource.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_message.h"
#include "vac/memory/memory_buffer.h"

/*!
 * \brief Forward declaration of "TcpConnection" class.
 */
namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
class TcpConnection;
}
}  // namespace someip_daemon_core
}  // namespace amsr

namespace amsr {
namespace someip_daemon_core {
namespace message_writer {

/*!
 * \brief Maximum number of packets to accept when socket buffer cannot accept
          them anymore.
 * \details The maximum queue size and this count match in case of TCP
 *          but can vary in the case of TLS if extra packets are needed
 *          by TLS to transmit the packet.
 */
static constexpr std::size_t kMaxEnqueuedPacketsCount{1U};

/*!
 * \brief SOME/IP stream message reader.
 */
class SomeIpStreamMessageWriter final {
 public:
  /*!
   * \brief             A TCP socket type.
   */
  using Socket = amsr::net::ip::Tcp::Socket;

  /*!
   * \brief Return codes for the status when the socket is written to.
   */
  enum class ReturnCode : std::uint8_t {
    kOk,         /*! Message successfully sent fully or partially. */
    kSocketBusy, /*! Socket is busy and cannot be written to. */
    kError,      /*! Error in the transmission. */
    kQueueEmpty, /*! Queue is empty. */
    kQueueFull   /*! Queue is full. */
  };

  /*!
   * \brief The constructor.
   *
   * \steady FALSE
   * \param[in] tcp_connection Pointer to the related TCP connection.
   */
  explicit SomeIpStreamMessageWriter(connection_manager::TcpConnection* tcp_connection) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \steady FALSE
   * \pre -
   * \context ANY
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~SomeIpStreamMessageWriter() noexcept = default;

  SomeIpStreamMessageWriter(SomeIpStreamMessageWriter const&) = delete;
  SomeIpStreamMessageWriter(SomeIpStreamMessageWriter&&) = delete;
  SomeIpStreamMessageWriter& operator=(SomeIpStreamMessageWriter const&) = delete;
  SomeIpStreamMessageWriter& operator=(SomeIpStreamMessageWriter&&) = delete;

  /*!
   * \brief Enqueue a packet.
   *
   * \param[in] uptr_packet Packet to enqueue.
   * \param[in] is_TLS To differentiate between TCP and TLS.
   *
   * \steady TRUE
   * \return Status of queuing the packet.
   */
  ReturnCode Enqueue(someip_protocol::internal::SomeIpMessage::DataBufferUniquePtr&& uptr_packet, bool is_TLS);

  /*!
   * \brief Called by the TCP connection when the socket is writable.
   *
   * \steady TRUE
   * \return Status of write to the socket.
   */
  ReturnCode OnSocketWritable();

  /*!
   * \brief Get number of packets in the queue.
   *
   * \steady TRUE
   * \return Queue size.
   */
  std::size_t GetQueueSize() const;

  /*!
   * \brief Get maximum allowed count of packets.
   *
   * \steady TRUE
   * \return Maximum allowed count.
   */
  static std::size_t GetMaxEnqueuedPacketsCount();

 private:
  /*!
   * \brief Try to transmit a message from the queue.
   *
   * \steady TRUE
   * \return A value that indicates the status of the operation.
   */
  ReturnCode WriteFromQueue();

  /*!
   * \brief The container type to buffer outgoing messages.
   */
  using TransmitQueue =
      std::deque<someip_protocol::internal::SomeIpMessage::DataBufferUniquePtr,
                 ara::core::PolymorphicAllocator<someip_protocol::internal::SomeIpMessage::DataBufferUniquePtr>>;

  /*!
   * \brief Contains all messages pending for transmission.
   */
  TransmitQueue queue_{};

  /*!
   * \brief Sum of all the bytes pointed to by the pointers in the queue.
   */
  std::size_t queue_memory_used_{0U};

  /*!
   * \brief Current message's sent bytes.
   */
  std::size_t bytes_sent_current_message_{0U};

  /*!
   * \brief Pointer to the TCP connection (to access socket and some other members).
   */
  connection_manager::TcpConnection* tcp_connection_;

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                    someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                    ara::core::StringView{"SomeIpStreamMessageWriter"}};
};

}  // namespace message_writer
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_WRITER_SOMEIP_STREAM_MESSAGE_WRITER_H_

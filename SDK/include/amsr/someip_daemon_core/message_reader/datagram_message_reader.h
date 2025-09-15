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
/*!        \file  datagram_message_reader.h
 *        \brief  Reader for reading PDU and SOME/IP messages from UDP datagrams
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_DATAGRAM_MESSAGE_READER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_DATAGRAM_MESSAGE_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <utility>
#include "ara/core/vector.h"
#include "someip-protocol/internal/marshalling.h"

#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_receive_remapper.h"
#include "amsr/someip_daemon_core/connection_manager/udp_socket_reader.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/unique_ptr.h"
#include "someip-protocol/internal/deserialization/deser_pdu_header.h"
#include "someip-protocol/internal/deserialization/deser_someip_header.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/ser_pdu_header.h"
#include "vac/container/static_vector.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_daemon_core {
namespace message_reader {

/*!
 * \brief Enum for message protocols
 * \vprivate Vector component private API
 */
enum class Protocol : std::uint8_t {
  kSomeIp,  // Message is SomeIp message
  kPdu      // Message is PDU message
};

/*!
 * \brief A protocol message.
 * \vprivate Vector component private API
 */
struct ProtocolMessage {
  /*!
   * \brief Buffer containing the message.
   */
  vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> message_buffer_{nullptr};
  /*!
   * \brief The message protocol.
   */
  Protocol protocol_{};
  /*!
   * \brief Optional PDU ID value if the message is a PDU
   */
  ara::core::Optional<someip_protocol::internal::PduId> pdu_id_{ara::core::nullopt};
};

/*!
 * \brief DatagramMessageReader.
 * \vprivate Vector component private API
 */
class DatagramMessageReader final {
 public:
  /*!
   * \brief A SOME/IP datagram message reader pointer type.
   */
  using Uptr = amsr::UniquePtr<DatagramMessageReader>;

  /*!
   * \brief Default constructor.
   *
   * \steady  TRUE
   */
  DatagramMessageReader() = default;

  /*!
   * \brief Default destructor
   *
   * \steady  FALSE
   */
  ~DatagramMessageReader() noexcept = default;

  DatagramMessageReader(DatagramMessageReader const&) = delete;
  DatagramMessageReader(DatagramMessageReader&&) = delete;
  DatagramMessageReader& operator=(DatagramMessageReader const&) = delete;
  DatagramMessageReader& operator=(DatagramMessageReader&&) = delete;

  /*!
   * \brief Reads a provided datagram and parses protocol messages from the datagram.
   * \param[in, out] datagram The datagram to read protocol messages from.
   * \param[in] pdu_id_receive_remapper a pointer to pdu remapper to identify whether the message is a generic PDU
   * message or a SOME/IP message.
   * \return A container with protocol messages.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   * \trace SPEC-4981401
   * \trace SPEC-10144326
   */
  ara::core::Vector<ProtocolMessage> Read(
      connection_manager::UdpSocketReader::Datagram& datagram,
      connection_manager::pdu_id_mappers::PduIdReceiveRemapperInterface* pdu_id_receive_remapper = nullptr) noexcept;

 private:
  /*!
   * \brief Get the payload's length in the header of a message in the passed-in datagram.
   * \param[in] datagram The datagram containing the message.
   * \return Payload's length.
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  static std::size_t GetPayloadLength(connection_manager::UdpSocketReader::Datagram const& datagram) noexcept;

  /*!
   * \brief Checks if the current datagram has enough data available for a complete PDU header.
   * \details The datagram is invalidated if there is not enough data for a header.
   * \param[in,out] datagram The current datagram.
   * \return true if there is enough data, otherwise false.
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  bool HasEnoughDataForCompletePduHeader(connection_manager::UdpSocketReader::Datagram const& datagram) const noexcept;

  /*!
   * \brief Determine the protocol of the next message.
   * \param[in] datagram The current datagram.
   * \param[in] pdu_id_receive_remapper a reference to pdu remapper to identify whether the message is a generic PDU
   * message or a SOME/IP message.
   * \return A pair containing the message protocol and PDU id.
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  static std::pair<Protocol, ara::core::Optional<someip_protocol::internal::PduId>> DetermineProtocolAndPduID(
      connection_manager::UdpSocketReader::Datagram const& datagram,
      connection_manager::pdu_id_mappers::PduIdReceiveRemapperInterface& pdu_id_receive_remapper) noexcept;

  /*!
   * \brief Checks if the current datagram has enough data available for a complete message with the specified length.
   * \details The datagram is invalidated if there is not enough data for a complete message.
   * \param[in] actual_message_size The size of the complete message.
   * \param[in,out] datagram The current datagram.
   * \return true if there is enough data, otherwise false.
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  bool DatagramContainsCompleteMessage(std::size_t const actual_message_size,
                                       connection_manager::UdpSocketReader::Datagram const& datagram) const noexcept;

  /*!
   * \brief Copies the complete message from the datagram into a newly allocated buffer.
   * \details Consumes the specified number of bytes of the datagram for processing.
   * \param[in] actual_message_size The size of the complete message.
   * \param[in,out] datagram The datagram to copy the message from. Consumes bytes equal to the message size.
   * \return The message with senders endpoint for the specified protocol.
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> GetMessageFromDatagram(
      std::size_t const actual_message_size, connection_manager::UdpSocketReader::Datagram& datagram) noexcept;

  /*!
   * \brief Increments the counter for bad datagrams and logs the new value.
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  void IncrementBadDatagramCounter() noexcept;

  /*!
   * \brief A counter for bad datagrams.
   */
  std::size_t bad_datagram_counter_{0U};
  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                    someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                    ara::core::StringView{"DatagramMessageReader"}};
  /*!
   * \brief A data buffer allocator.
   */
  someip_daemon_core::MemoryBufferAllocator allocator_{};
};

}  // namespace message_reader
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_DATAGRAM_MESSAGE_READER_H_

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
/*!        \file  someip_daemon_client/internal/common_types.h
 *        \brief  SOME/IP Daemon Client common Types
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_COMMON_TYPES_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_COMMON_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/span.h"
#include "ara/core/array.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief The connection state.
 */
enum class ConnectionState : std::uint8_t {
  kDisconnected, /*!< No connection established */
  kConnected     /*!< Connection established */
};

/*!
 * \brief Connection states used in logging messages.
 */
static ara::core::Array<char const*, 2U> const kConnectionStateEnumStr{"Disconnected", "Connected"};

/*!
 * \brief Type-alias for the mutable I/O memory buffer.
 */
using MutableIOBuffer = osabstraction::io::MutableIOBuffer;

/*!
 * \brief Type-alias for a container of mutable I/O memory buffers.
 */
using MutableIOBufferContainerSingle = ara::core::Array<MutableIOBuffer, 1U>;

/*!
 * \brief Type-alias for a container of mutable I/O memory buffers.
 */
using MutableIOBufferContainerDouble = ara::core::Array<MutableIOBuffer, 2U>;

/*!
 * \brief Type-alias for the mutable I/O memory buffer view.
 */
using MutableIOBufferContainerView = amsr::core::Span<MutableIOBuffer>;

/*!
 * \brief Struct used for Data Reception.
 * \details Buffers for Reception:
 *          GenericMessageHeader:  receive_generic_header
 *          SpecificMessageHeader: receive_specific_header
 *          MessageBody:           receive_message_body
 */
struct ReceptionBuffer {  // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
  /*!
   * \brief A buffer for the generic message header of received messages.
   */
  ara::core::Array<std::uint8_t, amsr::someipd_app_protocol::internal::kGenericMessageHeaderLength>
      receive_generic_header{};

  /*!
   * \brief A buffer for the specific message header of received messages.
   */
  ara::core::Array<std::uint8_t, amsr::someipd_app_protocol::internal::kSpecificMessageHeaderLength>
      receive_specific_header{};

  /*!
   * \brief A buffer for the metadata.
   */
  ara::core::Array<std::uint8_t, amsr::someip_protocol::internal::kTimeStampSize> receive_metadata{};

  /*!
   * \brief A buffer for the SomeIp header.
   */
  ara::core::Array<std::uint8_t, amsr::someip_protocol::internal::kHeaderSize> receive_someip_header{};

  /*!
   * \brief A buffer for the Pdu header.
   */
  ara::core::Array<std::uint8_t, amsr::someip_protocol::internal::kPduHeaderSize> receive_pdu_header{};

  /*!
   * \brief Contains the next received message.
   */
  vac::memory::allocator::MemoryBufferPtr receive_message_body{};

  /*!
   * \brief A container of I/O buffers for reception.
   * \details Buffer contents:
   *          MessageBody:           receive_message_body
   */
  MutableIOBufferContainerSingle receive_buffer_container_body{{MutableIOBuffer{}}};

  /*!
   * \brief A container of I/O buffers for reception.
   * \details Buffer contents:
   *          MessageBody:           receive_message_body
   */
  MutableIOBufferContainerSingle receive_buffer_container_body_payload{{MutableIOBuffer{}}};

  /*!
   * \brief A container of I/O buffers for reception of header.
   * \details Buffer contents:
   *          GenericMessageHeader:  receive_generic_header
   *          SpecificMessageHeader: receive_specific_header
   */
  MutableIOBufferContainerDouble receive_container_ipc_header{
      {{receive_generic_header.data(), receive_generic_header.size()},
       {receive_specific_header.data(), receive_specific_header.size()}}};

  /*!
   * \brief A container of I/O buffers for reception of SOME/IP header.
   * \details Buffer contents:
   *          SOME/IP header
   */
  MutableIOBufferContainerSingle receive_container_someip_header{
      {{receive_someip_header.data(), receive_someip_header.size()}}};

  /*!
   * \brief A container of I/O buffers for reception of PDU header.
   * \details Buffer contents:
   *          PDU header
   */
  MutableIOBufferContainerSingle receive_container_pdu_header{{{receive_pdu_header.data(), receive_pdu_header.size()}}};

  /*!
   * \brief A container of I/O buffers for reception of Metadata and SOME/IP header.
   * \details Buffer contents:
   *          Metadata
   *          SOME/IP header
   */
  MutableIOBufferContainerDouble receive_container_metadata_and_someip_header{
      {{receive_metadata.data(), receive_metadata.size()},
       {receive_someip_header.data(), receive_someip_header.size()}}};

  /*!
   * \brief A container of I/O buffers for reception of Metadata and PDU header.
   * \details Buffer contents:
   *          Metadata
   *          PDU header
   */
  MutableIOBufferContainerDouble receive_container_metadata_and_pdu_header{
      {{receive_metadata.data(), receive_metadata.size()}, {receive_pdu_header.data(), receive_pdu_header.size()}}};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_COMMON_TYPES_H_

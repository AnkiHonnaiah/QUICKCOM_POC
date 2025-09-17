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
/*!        \file someipd_app_protocol/internal/message_builder.h
 *        \brief Defines the MessageBuilder class for creataing SOME/IP error header.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_MESSAGE_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"
#include "vac/memory/memory_buffer.h"
#include "vac/memory/memory_buffer_allocator.h"

namespace amsr {
namespace someipd_app_protocol {
namespace internal {
/*!
 * \brief Assembles messages for SOME/IP.
 * \vprivate Product private
 */
class MessageBuilder final {
 public:
  /*!
   * \brief Assemble a SOME/IP error response message.
   * \details Assemble message in case of:
   * - malformed messages
   * - unknown service (instance)
   * - method not implemented
   *
   * \param allocator A memory buffer allocator.
   * \param return_code SOME/IP return code to write to the SOME/IP header of
   * the error response.
   * \param request_header The unmodified SOME/IP header from the request.
   * This SOME/IP header will be reused and slightly modified for the error
   * response.
   * \return Serialized error response packet.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Allocate memory for the SOME/IP header.
   * - Create a SomeIpMessageHeader, copied from 'request_header'.
   * - Set the message type of the new header to kError.
   * - set the return code of the new header to 'return_code'.
   * - If the return code is equal to kWrongProtocolVersion
   *   - set the protocol version of the new header to the Default SOME/IP protocol version.
   * - Create a Writer from the allocated MemoryBuffer.
   * - Serialize the new header.
   * \endinternal
   */
  static auto CreateSomeIpErrorHeader(vac::memory::allocator::MemoryBufferAllocator& allocator,
                                      amsr::someip_protocol::internal::SomeIpReturnCode const return_code,
                                      amsr::someip_protocol::internal::SomeIpMessageHeader const& request_header)
      -> vac::memory::allocator::MemoryBufferPtr {
    amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> allocated_packet_result{
        allocator.Allocate(amsr::someip_protocol::internal::kHeaderSize)};  // VCA_SDCL_SOMEIP_HEADER

    if (!allocated_packet_result.HasValue()) {
      amsr::core::Abort("Failed to allocate memory buffer.");
    }

    vac::memory::allocator::MemoryBufferPtr packet{std::move(allocated_packet_result.Value())};
    // Header from the failed request is used to get the same service, instance, client and session ID.
    amsr::someip_protocol::internal::SomeIpMessageHeader header_response{request_header};

    // The message type of the SOME/IP header must be set to kError
    header_response.message_type_ = amsr::someip_protocol::internal::SomeIpMessageType::kError;
    header_response.return_code_ = return_code;
    if (return_code == amsr::someip_protocol::internal::SomeIpReturnCode::kWrongProtocolVersion) {
      header_response.protocol_version_ = amsr::someip_protocol::internal::kProtocolVersion;
    }

    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPDAEMONCLIENT_M0.3.1_Previously_checked_pointer
    // Get buffer view and create writer
    vac::memory::allocator::MemoryBuffer::MemoryBufferView const buffer_view{
        packet->GetView(0U)};  // VCA_SDCL_SOMEIP_HEADER

    amsr::someip_protocol::internal::serialization::Writer writer{buffer_view};
    amsr::someip_protocol::internal::serialization::SerializeSomeIpMessageHeader(writer, header_response, 0U);

    return packet;
  }
};
}  // namespace internal
}  // namespace someipd_app_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_MESSAGE_BUILDER_H_

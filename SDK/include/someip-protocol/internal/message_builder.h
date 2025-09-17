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
/**        \file message_builder.h
 *        \brief Defines the MessageBuilder class for creataing SOME/IP error header.
 *        \unit SomeIpProtocol::Messages::MessageBuilder
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_BUILDER_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/io/io_buffer.h>
#include <memory>
#include <utility>
#include "amsr/core/span.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"
#include "vac/memory/memory_buffer_wrapper.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
/*!
 * \brief Assembles messages for SOME/IP.
 * \vprivate Product private
 * \trace SPEC-4981310
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
   * \trace SPEC-10144311
   */
  static ::vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer> CreateSomeIpErrorHeader(
      amsr::someip_protocol::internal::SomeIpReturnCode const return_code,
      amsr::someip_protocol::internal::SomeIpMessageHeader const& request_header,
      ::vac::memory::allocator::MemoryBufferAllocator& allocator) {
    amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> alloc_result{
        allocator.Allocate(kHeaderSize)};  // VCA_SOMEIPPROTOCOL_POSSIBLY_NULL
    assert(alloc_result.HasValue());
    ara::core::Result<amsr::UniquePtr<vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>>
        packet_result{
            amsr::AllocateUnique<vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>(
                ara::core::PolymorphicAllocator<vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>{},
                std::move(alloc_result.Value()))
                .Value()};  // VCA_SOMEIPPROTOCOL_POSSIBLY_NULL
    assert(packet_result.HasValue());
    // Header from the failed request is used to get the same service, instance, client and session ID.
    amsr::someip_protocol::internal::SomeIpMessageHeader header_response{request_header};

    // The message type of the SOME/IP header must be set to kError
    header_response.message_type_ = amsr::someip_protocol::internal::SomeIpMessageType::kError;
    header_response.return_code_ = return_code;
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (return_code == amsr::someip_protocol::internal::SomeIpReturnCode::kWrongProtocolVersion) {
      header_response.protocol_version_ = amsr::someip_protocol::internal::kProtocolVersion;
    }

    // Get buffer view and create writer
    vac::memory::MemoryBuffer<osabstraction::io::MutableIOBuffer>::MemoryBufferView packet_view{
        packet_result.Value()->GetView(0U)};  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC, VCA_SOMEIPPROTOCOL_SOMEIP_HEADER
    // clang-format off
    // VECTOR NC AutosarC++17_10-M5.2.8:MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.2.8_conv_from_voidp
    // VECTOR NC AutosarC++17_10-M0.3.1:MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.3.1_NullAtDereference
    someip_protocol::internal::serialization::BufferView const body_view{
        static_cast<std::uint8_t*>(packet_view[0U].base_pointer), packet_result.Value()->size()}; // VCA_SOMEIPPROTOCOL_SOMEIP_HEADER
    // clang-format on
    someip_protocol::internal::serialization::Writer writer{body_view};
    someip_protocol::internal::serialization::SerializeSomeIpMessageHeader(writer, header_response, 0U);

    return std::move(packet_result.Value());
  }  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED

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
  static ::vac::memory::allocator::MemoryBufferPtr CreateSomeIpErrorHeader(
      ::vac::memory::allocator::MemoryBufferAllocator& allocator,
      amsr::someip_protocol::internal::SomeIpReturnCode const return_code,
      amsr::someip_protocol::internal::SomeIpMessageHeader const& request_header) {
    // clang-format off
    // VCA_SOMEIPPROTOCOL_WITHIN_SPEC, VCA_SOMEIPPROTOCOL_SOMEIP_HEADER
    ::vac::memory::allocator::MemoryBufferPtr packet{allocator.Allocate(kHeaderSize).Value()};
    // clang-format on
    // Header from the failed request is used to get the same service, instance, client and session ID.
    amsr::someip_protocol::internal::SomeIpMessageHeader header_response{request_header};

    // The message type of the SOME/IP header must be set to kError
    header_response.message_type_ = amsr::someip_protocol::internal::SomeIpMessageType::kError;
    header_response.return_code_ = return_code;
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (return_code == amsr::someip_protocol::internal::SomeIpReturnCode::kWrongProtocolVersion) {
      header_response.protocol_version_ = amsr::someip_protocol::internal::kProtocolVersion;
    }

    assert(packet != nullptr);
    // Get buffer view and create writer
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.3.1_NullAtDereference
    // VCA_SOMEIPPROTOCOL_WITHIN_SPEC, VCA_SOMEIPPROTOCOL_SOMEIP_HEADER
    vac::memory::allocator::MemoryBuffer::MemoryBufferView const buffer_view{packet->GetView(0U)};

    someip_protocol::internal::serialization::Writer writer{buffer_view};
    someip_protocol::internal::serialization::SerializeSomeIpMessageHeader(writer, header_response, 0U);

    return packet;
  }
};
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_MESSAGE_BUILDER_H_

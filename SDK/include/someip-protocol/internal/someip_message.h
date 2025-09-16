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
/*!        \file someip_message.h
 *        \brief This class represents a single complete SOME/IP message.
 *        \unit SomeIpProtocol::Messages::SomeIpMessage
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_MESSAGE_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_MESSAGE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <utility>

#include "ara/core/array.h"
#include "ara/core/memory_resource.h"
#include "ara/core/optional.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/deserialization/deser_someip_header.h"
#include "someip-protocol/internal/deserialization/types.h"
#include "someip-protocol/internal/logging/someip_protocol_log_builder.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip-protocol/internal/someip_protocol_error_code.h"
#include "vac/language/location.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"
#include "vac/memory/memory_buffer_wrapper.h"

namespace amsr {
namespace someip_protocol {
namespace internal {

/*!
 * \brief SOME/IP message class.
 * \trace SPEC-4980062
 * \vprivate Product private
 */
class SomeIpMessage final {  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
 public:
  /*!
   * \brief Data buffer unique pointer type.
   */
  using DataBufferUniquePtr = ::vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief Data buffer pointer type.
   */
  using DataBufferPtr = ::vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer>::pointer;

  /*!
   * \brief     Named constructor of SomeIpMessage.
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \return    SomeIpMessage object containing the complete SOME/IP message,
   *            or an error if the buffer size is too small for the SOME/IP header.
   * \pre       SOME/IP message header must be correctly serialized.
   * \context   Reactor
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::Result<SomeIpMessage> CreateSomeIpMessage(SomeIpMessage::DataBufferUniquePtr buffer) {
    amsr::core::Result<SomeIpMessage> result{amsr::core::Result<SomeIpMessage>::FromError(
        amsr::someip_protocol::MakeErrorCode(SomeIpProtocolErrc::buffer_too_small, 0, ""))};
    // clang-format off
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if ((buffer != nullptr) && (buffer->size() >= kHeaderSize)) { // VCA_SOMEIPPROTOCOL_MSG_BUFFER
      SomeIpMessageHeader const header{DeserializeMessageHeader(buffer.get())};
      if (buffer->size() >= (static_cast<std::size_t>(header.length_) + kHeaderLength)) { // VCA_SOMEIPPROTOCOL_MSG_BUFFER
        // clang-format off
        result.EmplaceValue(SomeIpMessage(std::move(buffer), header)); // VCA_SOMEIPPROTOCOL_SOMEIP_MSG_REFERENCE
        // clang-format on
      }
    }
    // clang-format on
    return result;
  }

  /*!
   * \brief     Named constructor of SomeIpMessage.
   * \param[in] buffer A data buffer containing a complete SOME/IP message.
   * \param[in] time_stamp A time stamp for when the message was received.
   * \return    "SomeIpMessage" object containing the complete SOME/IP message
   *            or an error if the buffer size is too small for the SOME/IP header.
   * \pre       SOME/IP message header must be correctly serialized.
   * \context   Reactor
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::Result<SomeIpMessage> CreateSomeIpMessage(SomeIpMessage::DataBufferUniquePtr buffer,
                                                               TimeStamp time_stamp) {
    using ResultSomeIp = amsr::core::Result<SomeIpMessage>;
    ResultSomeIp result{
        ResultSomeIp::FromError(amsr::someip_protocol::MakeErrorCode(SomeIpProtocolErrc::buffer_too_small, 0, ""))};
    // clang-format off
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if ((buffer != nullptr) && (buffer->size() >= kHeaderSize)) { // VCA_SOMEIPPROTOCOL_MSG_BUFFER
      // clang-format on
      SomeIpMessageHeader const header{DeserializeMessageHeader(buffer.get())};
      // clang-format off
      if (buffer->size() >= (static_cast<std::size_t>(header.length_) + kHeaderLength)) {  // VCA_SOMEIPPROTOCOL_MSG_BUFFER
        result.EmplaceValue(SomeIpMessage(std::move(buffer), header, time_stamp)); // VCA_SOMEIPPROTOCOL_SOMEIP_MSG_REFERENCE
        // clang-format on
      }
      // clang-format on
    }

    return result;
  }

  /*!
   * \brief   Returns the SOME/IP message header of the contained SOME/IP message.
   * \return  SOME/IP message header.
   * \pre     SomeIpMessage must be initialized with a valid header.
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  SomeIpMessageHeader const& GetHeader() const { return message_header_; }

  /*!
   * \brief   Returns the optional time stamp value of the SOME/IP message's meta data.
   * \return  Reference to the optional time stamp for when the SOMEIP message was received.
   * \pre     SomeIpMessage must be initialized with a valid TimeStamp.
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  ara::core::Optional<TimeStamp> const& GetTimeStamp() const noexcept { return meta_data_.rx_time_stamp; }

  /*!
   * \brief   Returns the optional tx accumulation timeout of the SOME/IP message's meta data.
   * \return  Reference to the optional time stamp for when the SOMEIP message was received.
   * \pre     SetAccumulationTimeout must have been called previously.
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  ara::core::Optional<std::chrono::nanoseconds> const GetAccumulationTimeout() const noexcept {
    return meta_data_.tx_message_accumulation_timeout;
  }

  /*!
   * \brief   Returns the total size of the contained SOME/IP message with header.
   * \return  Total SOME/IP message size.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetTotalSize() const {
    return (buffer_ != nullptr) ? buffer_->size() : 0;  // VCA_SOMEIPPROTOCOL_MSG_BUFFER
  }

  /*!
   * \brief   Returns the total size of the contained SOME/IP message body without header.
   * \return  Size of SOME/IP message body.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetBodySize() const {
    // clang-format off
    return (buffer_ != nullptr) ? (buffer_->size() - kHeaderSize) : 0; // VCA_SOMEIPPROTOCOL_MSG_BUFFER
    // clang-format on
  }

  /*!
   * \brief   Get memory buffer containing the SOME/IP message.
   * \return  Memory buffer containing the SOME/IP message.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  DataBufferPtr GetBuffer() const { return buffer_.get(); }

  /*!
   * \brief   Releases the ownership of the memory buffer
   * \return  Memory buffer containing the SOME/IP message.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  DataBufferUniquePtr ReleasePacket() { return std::move(buffer_); }

  /*!
   * \brief     Create a copy of the internal buffer.
   * \param[in] allocator An allocator for allocating memory for the copy.
   * \return    A copy of the internal buffer.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Use the allocator to allocate memory based on the size of the internal buffer.
   * - Copy the internal buffer to the allocated memory.
   * - Return a reference to the memory whose ownership can be transferred.
   * \endinternal
   */
  DataBufferUniquePtr GetBufferCopy(vac::memory::allocator::MemoryBufferAllocator& allocator) const {
    DataBufferUniquePtr buffer_copy{nullptr};

    if (buffer_ != nullptr) {
      std::size_t const buffer_size{buffer_->size()};  // VCA_SOMEIPPROTOCOL_POSSIBLY_NULL_FP
      ara::core::Result<vac::memory::allocator::MemoryBufferPtr> memory_buffer_ptr_res{
          allocator.Allocate(buffer_size)};  // VCA_SOMEIPPROTOCOL_POSSIBLY_NULL_FP
      if (memory_buffer_ptr_res.HasValue()) {
        // VCA_SOMEIPPROTOCOL_VALID_MAKE_UNIQUE
        buffer_copy =
            amsr::AllocateUnique<vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>(
                ara::core::PolymorphicAllocator<vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>>{},
                std::move(memory_buffer_ptr_res.Value()))
                .Value();
        void* const buffer_view{buffer_copy.get()->GetView(0)[0].base_pointer};  // VCA_SOMEIPPROTOCOL_MSG_BUFFER
        static_cast<void>(buffer_->CopyOut(0, buffer_size, buffer_view));        // VCA_SOMEIPPROTOCOL_MSG_BUFFER
      }
    }

    return buffer_copy;
  }  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED

  /*!
   * \brief   Verify if the time stamp is emplaced or not.
   * \return  "true" if the time stamp is emplaced and "false" otherwise.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  bool HasTimeStamp() const { return (meta_data_.rx_time_stamp.has_value()); }

  /*!
   * \brief     Set the time stamp of the SOME/IP message's meta data.
   * \param[in] time_stamp The time stamp when SOME/IP message was received.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  void SetTimeStamp(TimeStamp& time_stamp) noexcept { meta_data_.rx_time_stamp.emplace(time_stamp); }

  /*!
   * \brief     Set the accumulation timeout of the SOME/IP message's meta data.
   * \param[in] accumulation_timeout The accumulation timeout.
   * \pre       -
   * \context   Reactor|App
   */
  void SetAccumulationTimeout(ara::core::Optional<std::chrono::nanoseconds> const accumulation_timeout) noexcept {
    meta_data_.tx_message_accumulation_timeout = accumulation_timeout;
  }

 private:
  /*!
   * \brief Constructor of SomeIpMessage.
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \param[in] header A deserialized SOME/IP message header
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SomeIpMessage(DataBufferUniquePtr buffer, SomeIpMessageHeader const& header)
      : buffer_{std::move(buffer)}, message_header_{header}, meta_data_{} {}

  /*!
   * \brief Constructor of SomeIpMessage.
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \param[in] header A deserialized SOME/IP message header
   * \param[in] time_stamp A time stamp for when the SOME/IP message was received.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SomeIpMessage(DataBufferUniquePtr buffer, SomeIpMessageHeader const& header, TimeStamp time_stamp)
      : SomeIpMessage(std::move(buffer), header) {  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    meta_data_.rx_time_stamp.emplace(time_stamp);
  }

  /*!
   * \brief Deserializes the SOME/IP message header.
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \return SOME/IP message header.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Copy the message header from buffer to a temporary buffer.
   * - Create a reader with the temporary buffer.
   * - Deserialize the SOME/IP header.
   * - Return the deserialized SOME/IP header.
   * \endinternal
   */
  static inline SomeIpMessageHeader DeserializeMessageHeader(DataBufferPtr const buffer) {
    // First copy the SOME/IP message header to a temporary continuous buffer
    ara::core::Array<std::uint8_t, kHeaderSize> header_buffer{};
    std::size_t const header_offset{0U};
    // Extract the SOME/IP message header from the possibly fragmented buffer
    // clang-format off
    static_cast<void>(buffer->CopyOut(header_offset, header_buffer.size(), header_buffer.data())); // VCA_SOMEIPPROTOCOL_MSG_BUFFER
    // clang-format on
    // Deserialize the SOME/IP message header
    someip_protocol::internal::SomeIpMessageHeader header{
        0U, 0U, 0U, 0U, 0U, 0U, 0U, SomeIpMessageType::kRequest, SomeIpReturnCode::kOk};
    // Get the buffer view containing SOME/IP header
    PacketBufferView packet_buffer{header_buffer.data(), header_buffer.size()};
    amsr::someip_protocol::internal::deserialization::BufferView const buffer_view{packet_buffer};

    amsr::someip_protocol::internal::deserialization::Reader reader{buffer_view};

    static_cast<void>(amsr::someip_protocol::internal::deserialization::DeserializeSomeIpMessageHeader(reader, header));

    return header;
  }

  /*!
   * \brief A data buffer containing a complete SOME/IP message with a header.
   */
  DataBufferUniquePtr buffer_;

  /*!
   * \brief The SOME/IP message header.
   */
  SomeIpMessageHeader const message_header_;

  /*!
   * \brief The meta data of the SOME/IP message.
   */
  MetaData meta_data_;
};

}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SOMEIP_MESSAGE_H_

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
/*!        \file someipd_app_protocol/internal/someip_message.h
 *        \brief This class represents a single complete SOME/IP message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_SOMEIP_MESSAGE_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_SOMEIP_MESSAGE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/optional.h"
#include "ara/core/array.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/deserialization/deser_someip_header.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip-protocol/internal/someip_protocol_error_code.h"
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer_allocator.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someipd_app_protocol {
namespace internal {

/*!
 * \brief Meta data of the SomeIp message.
 */
struct MetaData {
  amsr::core::Optional<amsr::someip_protocol::internal::TimeStamp>
      rx_time_stamp; /*!< A time stamp for when the message was received */
  amsr::core::Optional<std::chrono::nanoseconds>
      tx_message_accumulation_timeout; /*!< Transmission accumulation timeout for a message */
};

/*!
 * \brief SOME/IP message class.
 * \vprivate Product private
 */
class SomeIpMessage final {
 public:
  /*!
   * \brief Data buffer unique pointer type.
   */
  using DataBufferUniquePtr = vac::memory::allocator::MemoryBufferPtr;

  /*!
   * \brief Data buffer pointer type.
   */
  using DataBufferPtr = vac::memory::allocator::MemoryBufferPtr::pointer;

  /*!
   * \brief Named constructor of SomeIpMessage.
   *
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \return SomeIpMessage object containing the complete SOME/IP message,
   *        or an error if the buffer size is too small for the SOME/IP header.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto CreateSomeIpMessage(SomeIpMessage::DataBufferUniquePtr buffer) -> amsr::core::Result<SomeIpMessage> {
    amsr::core::Result<SomeIpMessage> result{amsr::core::Result<SomeIpMessage>::FromError(
        amsr::someip_protocol::MakeErrorCode(amsr::someip_protocol::SomeIpProtocolErrc::buffer_too_small, 0, ""))};
    // clang-format off
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-M0.1.2_subexpression_always_evaluates_to_false
    if ((buffer != nullptr) && (buffer->GetView(0U).size() >= amsr::someip_protocol::internal::kHeaderSize)) {  // VCA_SDCL_SOMEIP_HEADER
      amsr::someip_protocol::internal::SomeIpMessageHeader const header{DeserializeMessageHeader(buffer.get())};
      if (buffer.get()->GetView(0).size() >= (static_cast<std::size_t>(header.length_) + amsr::someip_protocol::internal::kHeaderLength)) {  // VCA_SDCL_SOMEIP_HEADER
        // clang-format on
        result.EmplaceValue(SomeIpMessage(std::move(buffer), header));  // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
      }
    }
    return result;
  }

  /*!
   * \brief Named constructor of SomeIpMessage.
   * \param[in] buffer A data buffer containing a complete SOME/IP message.
   * \param[in] time_stamp A time stamp for when the message was received.
   * \return "SomeIpMessage" object containing the complete SOME/IP message
   * or an error if the buffer size is too small for the SOME/IP header.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto CreateSomeIpMessage(SomeIpMessage::DataBufferUniquePtr buffer,
                                  amsr::someip_protocol::internal::TimeStamp time_stamp)
      -> amsr::core::Result<SomeIpMessage> {
    using ResultSomeIp = amsr::core::Result<SomeIpMessage>;
    ResultSomeIp result{ResultSomeIp::FromError(
        amsr::someip_protocol::MakeErrorCode(amsr::someip_protocol::SomeIpProtocolErrc::buffer_too_small, 0, ""))};
    // clang-format off
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-M0.1.2_subexpression_always_evaluates_to_false
    if ((buffer != nullptr) && (buffer->GetView(0).size() >= amsr::someip_protocol::internal::kHeaderSize)) {  // VCA_SDCL_SOMEIP_HEADER
      amsr::someip_protocol::internal::SomeIpMessageHeader const header{DeserializeMessageHeader(buffer.get())};
      if (buffer->GetView(0).size() >= (static_cast<std::size_t>(header.length_) + amsr::someip_protocol::internal::kHeaderLength)) {  // VCA_SDCL_SOMEIP_HEADER
        // clang-format on
        result.EmplaceValue(
            SomeIpMessage(std::move(buffer), header, time_stamp));  // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
      }
    }

    return result;
  }

  /*!
   * \brief Returns the SOME/IP message header of the contained SOME/IP message.
   *
   * \return SOME/IP message header.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetHeader() const -> amsr::someip_protocol::internal::SomeIpMessageHeader const& { return message_header_; }

  /*!
   * \brief Returns the optional time stamp value of the SOME/IP message's meta data.
   *
   * \return Reference to the optional time stamp for when the SOMEIP message was received.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetTimeStamp() const noexcept -> amsr::core::Optional<amsr::someip_protocol::internal::TimeStamp> const& {
    return meta_data_.rx_time_stamp;
  }

  /*!
   * \brief Returns the optional tx accumulation timeout of the SOME/IP message's meta data.
   *
   * \return Reference to the optional time stamp for when the SOMEIP message was received.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetAccumulationTimeout() const noexcept -> amsr::core::Optional<std::chrono::nanoseconds> {
    return meta_data_.tx_message_accumulation_timeout;
  }

  /*!
   * \brief Returns the total size of the contained SOME/IP message with header.
   *
   * \return Total SOME/IP message size.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetTotalSize() const -> std::size_t {
    return (buffer_ != nullptr) ? buffer_->GetView(0).size() : 0;  // VCA_SDCL_SOMEIP_HEADER
  }

  /*!
   * \brief Returns the total size of the contained SOME/IP message body without header.
   *
   * \return Size of SOME/IP message body.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetBodySize() const -> std::size_t {
    // clang-format off
    return (buffer_ != nullptr) ? (buffer_->GetView(0).size() - amsr::someip_protocol::internal::kHeaderSize) : 0;  // VCA_SDCL_SOMEIP_HEADER
    // clang-format on
  }

  /*!
   * \brief Get memory buffer containing the SOME/IP message.
   *
   * \return Memory buffer containing the SOME/IP message.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetBuffer() const -> DataBufferPtr { return buffer_.get(); }

  /*!
   * \brief Releases the ownership of the memory buffer
   *
   * \return Memory buffer containing the SOME/IP message.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto ReleasePacket() -> DataBufferUniquePtr { return std::move(buffer_); }

  /*!
   * \brief Create a copy of the internal buffer.
   *
   * \param[in] allocator An allocator for allocating memory for the copy.
   *
   * \return A copy of the internal buffer.
   * \steady TRUE
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
  auto GetBufferCopy(vac::memory::allocator::MemoryBufferAllocator& allocator) const -> DataBufferUniquePtr {
    std::size_t const buffer_size{(buffer_ != nullptr) ? buffer_->GetView(0U).size() : 0};  // VCA_SDCL_SOMEIP_HEADER
    DataBufferUniquePtr buffer_copy{allocator.Allocate(buffer_size).Value()};               // VCA_SDCL_SOMEIP_HEADER
    if (buffer_ != nullptr) {
      void* const buffer_view{buffer_copy.get()->GetView(0).data()};       // VCA_SDCL_SOMEIP_HEADER
      std::memcpy(buffer_view, buffer_->GetView(0U).data(), buffer_size);  // VCA_SDCL_SOMEIP_HEADER
    }
    return buffer_copy;
  }

  /*!
   * \brief Verify if the time stamp is emplaced or not.
   * \return "true" if the time stamp is emplaced and "false" otherwise.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto HasTimeStamp() const -> bool { return (meta_data_.rx_time_stamp.has_value()); }

  /*!
   * \brief Set the time stamp of the SOME/IP message's meta data.
   * \param[in] time_stamp The time stamp when SOME/IP message was received.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetTimeStamp(amsr::someip_protocol::internal::TimeStamp const& time_stamp) noexcept {
    meta_data_.rx_time_stamp.emplace(time_stamp);
  }

  /*!
   * \brief Set the accumulation timeout of the SOME/IP message's meta data.
   * \param[in] accumulation_timeout The accumulation timeout.
   * \steady TRUE
   */
  void SetAccumulationTimeout(amsr::core::Optional<std::chrono::nanoseconds> const accumulation_timeout) noexcept {
    meta_data_.tx_message_accumulation_timeout = accumulation_timeout;
  }

 private:
  /*!
   * \brief Constructor of SomeIpMessage.
   *
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \param[in] header A deserialized SOME/IP message header
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SomeIpMessage(DataBufferUniquePtr buffer, amsr::someip_protocol::internal::SomeIpMessageHeader const& header)
      : buffer_{std::move(buffer)}, message_header_{header}, meta_data_{} {}

  /*!
   * \brief Constructor of SomeIpMessage.
   *
   * \param[in] buffer A data buffer containing a complete and valid SOME/IP message.
   * \param[in] header A deserialized SOME/IP message header
   * \param[in] time_stamp A time stamp for when the SOME/IP message was received.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SomeIpMessage(DataBufferUniquePtr buffer, amsr::someip_protocol::internal::SomeIpMessageHeader const& header,
                         amsr::someip_protocol::internal::TimeStamp time_stamp)
      : SomeIpMessage(std::move(buffer), header) {
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
   * \steady TRUE
   *
   * \internal
   * - Copy the message header from buffer to a temporary buffer.
   * - Create a reader with the temporary buffer.
   * - Deserialize the SOME/IP header.
   * - Return the deserialized SOME/IP header.
   * \endinternal
   */
  static auto DeserializeMessageHeader(DataBufferPtr const buffer)
      -> amsr::someip_protocol::internal::SomeIpMessageHeader {
    // First copy the SOME/IP message header to a temporary continuous buffer
    ara::core::Array<std::uint8_t, amsr::someip_protocol::internal::kHeaderSize> header_buffer{};
    // Extract the SOME/IP message header from the possibly fragmented buffer
    // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
    std::memcpy(header_buffer.data(), buffer->GetView(0U).data(), header_buffer.size());
    // Deserialize the SOME/IP message header
    amsr::someip_protocol::internal::SomeIpMessageHeader header{
        0U,
        0U,
        0U,
        0U,
        0U,
        0U,
        0U,
        amsr::someip_protocol::internal::SomeIpMessageType::kRequest,
        amsr::someip_protocol::internal::SomeIpReturnCode::kOk};
    // Get the buffer view containing SOME/IP header
    amsr::someip_protocol::internal::PacketBufferView packet_buffer{header_buffer.data(), header_buffer.size()};
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
  amsr::someip_protocol::internal::SomeIpMessageHeader const message_header_;

  /*!
   * \brief The meta data of the SOME/IP message.
   */
  MetaData meta_data_;
};

}  // namespace internal
}  // namespace someipd_app_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_SOMEIP_MESSAGE_H_

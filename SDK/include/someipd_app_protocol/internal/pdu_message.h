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
/*!        \file someipd_app_protocol/internal/pdu_message.h
 *        \brief This class represents a single complete PDU message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_PDU_MESSAGE_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_PDU_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/optional.h"
#include "ara/core/array.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/deserialization/deser_pdu_header.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/ser_pdu_header.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip-protocol/internal/someip_protocol_error_code.h"
#include "someip_message.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someipd_app_protocol {
namespace internal {

/*!
 * \brief PDU message class.
 * \vprivate Product private
 */
class PduMessage final {
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
   * \brief Named constructor of PduMessage.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \return PduMessage object containing the complete PDU message, or an error if the buffer size is too small
   *         for the PDU message header.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto CreatePduMessage(PduMessage::DataBufferUniquePtr buffer) -> amsr::core::Result<PduMessage> {
    amsr::core::Result<PduMessage> result{amsr::core::Result<PduMessage>::FromError(
        amsr::someip_protocol::MakeErrorCode(amsr::someip_protocol::SomeIpProtocolErrc::buffer_too_small, 0, ""))};
    // clang-format off
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-M0.1.2_subexpression_always_evaluates_to_false
    if ((buffer != nullptr) && (buffer->GetView(0U).size() >= amsr::someip_protocol::internal::kPduHeaderSize)) {  // VCA_SDCL_SOMEIP_HEADER
      amsr::someip_protocol::internal::PduMessageHeader const header{DeserializePduMessageHeader(buffer.get())};
      if (buffer->GetView(0U).size() >= (static_cast<std::size_t>(header.length_) + amsr::someip_protocol::internal::kPduHeaderSize)) {  // VCA_SDCL_SOMEIP_HEADER
        // clang-format on
        result.EmplaceValue(PduMessage(std::move(buffer), header));  // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
      }
    }
    return result;
  }

  /*!
   * \brief Named constructor of PduMessage.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \param[in] time_stamp A time stamp for when the PDU message was received.
   * \return PduMessage object containing the complete PDU message, or an error if the buffer size is too small
   * for the PDU message header.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static auto CreatePduMessage(PduMessage::DataBufferUniquePtr buffer,
                               amsr::someip_protocol::internal::TimeStamp time_stamp)
      -> amsr::core::Result<PduMessage> {
    using Result = amsr::core::Result<PduMessage>;
    Result result{Result::FromError(
        amsr::someip_protocol::MakeErrorCode(amsr::someip_protocol::SomeIpProtocolErrc::buffer_too_small, 0, ""))};
    // clang-format off
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-M0.1.2_subexpression_always_evaluates_to_false
    if ((buffer != nullptr) && (buffer->GetView(0U).size() >= amsr::someip_protocol::internal::kPduHeaderSize)) {  // VCA_SDCL_SOMEIP_HEADER
      amsr::someip_protocol::internal::PduMessageHeader const header{DeserializePduMessageHeader(buffer.get())};
      if (buffer->GetView(0U).size() >= (static_cast<std::size_t>(header.length_) + amsr::someip_protocol::internal::kPduHeaderSize)) {  // VCA_SDCL_SOMEIP_HEADER
        // clang-format on
        result.EmplaceValue(
            PduMessage(std::move(buffer), header, time_stamp));  // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
      }
    }
    return result;
  }

  /*!
   * \brief Returns the PDU message header of the contained PDU message.
   * \return PDU message header.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetHeader() -> amsr::someip_protocol::internal::PduMessageHeader& { return message_header_; }

  /*!
   * \brief Returns the optional time stamp value of the PDU message's meta data.
   *
   * \return Reference to the optional Time stamp for when the PDU message was received.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetTimeStamp() const -> amsr::core::Optional<amsr::someip_protocol::internal::TimeStamp> const& {
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
   * \brief Returns the total size of the contained PDU message with header.
   * \return Total PDU message size.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetTotalSize() const -> std::size_t {
    return (buffer_ != nullptr) ? buffer_->GetView(0U).size() : 0;  // VCA_SDCL_SOMEIP_HEADER
  }
  /*!
   * \brief Returns the total size of the contained PDU message body w/o header.
   * \return Size of PDU message body.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetBodySize() const -> std::size_t {
    // clang-format off
    return (buffer_ != nullptr) ? (buffer_->GetView(0U).size() - amsr::someip_protocol::internal::kPduHeaderSize) : 0; // VCA_SDCL_SOMEIP_HEADER
    // clang-format on
  }
  /*!
   * \brief Get memory buffer containing the PDU message.
   * \return Memory buffer containing the PDU message.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetBuffer() const -> DataBufferPtr { return buffer_.get(); }
  /*!
   * \brief Releases the ownership of the memory buffer.
   * \return Memory buffer containing the PDU message.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto ReleasePacket() -> DataBufferUniquePtr { return std::move(buffer_); }

  /*!
   * \brief Verify if the time stamp is emplaced or not.
   * \return True if the time stamp is emplaced.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto HasTimeStamp() const -> bool { return (meta_data_.rx_time_stamp.has_value()); }
  /*!
   * \brief Set the time stamp for the PDU messages' meta data.
   * \param[in] time_stamp The time stamp for when the PDU message was received.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetTimeStamp(amsr::someip_protocol::internal::TimeStamp const& time_stamp) {
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

  /*!
   * \brief Set the PDU identifier inside the message header.
   * \param[in] id New identifier value.
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
   * - Verify that the buffer holds a message.
   * - Create a writer with the temporary header buffer.
   * - Serialize the PDU header.
   * - Copy the temporary header into the original message.
   * \endinternal
   */
  void SetPduId(amsr::someip_protocol::internal::PduId const id) {
    if (buffer_) {
      ara::core::Vector<std::uint8_t> data(amsr::someip_protocol::internal::kPduHeaderSize);

      amsr::someip_protocol::internal::serialization::Writer writer{
          amsr::someip_protocol::internal::serialization::BufferView{data}};
      message_header_.pdu_id_ = id;
      SerializePduMessageHeader(writer, message_header_, message_header_.length_);
      // Ignore return value. Size already verified.
      // VCA_SDCL_SLC10_SLC22_SLC11_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
      std::memcpy(buffer_->GetView(0U).data(), data.data(), data.size());
    }
  }

 private:
  /*!
   * \brief Constructor of PduMessage.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \param[in] header A deserialized PDU message header
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit PduMessage(DataBufferUniquePtr buffer, amsr::someip_protocol::internal::PduMessageHeader const& header)
      : buffer_{std::move(buffer)}, message_header_{header}, meta_data_{} {}

  /*!
   * \brief Constructor of PduMessage.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \param[in] header A deserialized PDU message header
   * \param[in] time_stamp A time stamp for when the PDU message was received.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit PduMessage(DataBufferUniquePtr buffer, amsr::someip_protocol::internal::PduMessageHeader const& header,
                      amsr::someip_protocol::internal::TimeStamp time_stamp)
      : PduMessage(std::move(buffer), header) {
    meta_data_.rx_time_stamp.emplace(time_stamp);
  }

  /*!
   * \brief Deserializes the PDU message header.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \return PDU message header.
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
   * - Deserialize the PDU header.
   * - Return the deserialized PDU header.
   * \endinternal
   */
  static auto DeserializePduMessageHeader(DataBufferPtr const buffer)
      -> amsr::someip_protocol::internal::PduMessageHeader {
    // First copy the PDU message header to a temporary continuous buffer
    ara::core::Array<std::uint8_t, amsr::someip_protocol::internal::kPduHeaderSize> header_buffer{};
    // Extract the PDU message header from the possibly fragmented buffer
    // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
    std::memcpy(header_buffer.data(), buffer->GetView(0).data(), header_buffer.size());
    // Deserialize the PDU message header
    amsr::someip_protocol::internal::PduMessageHeader header{0U, 0U};
    // Get the buffer view containing PDU header
    amsr::someip_protocol::internal::PacketBufferView packet_buffer{header_buffer.data(), header_buffer.size()};
    amsr::someip_protocol::internal::deserialization::BufferView const buffer_view{packet_buffer};

    amsr::someip_protocol::internal::deserialization::Reader reader{buffer_view};
    static_cast<void>(amsr::someip_protocol::internal::deserialization::DeserializePduMessageHeader(reader, header));
    return header;
  }
  /*!
   * \brief A data buffer containing a complete PDU message with header.
   */
  DataBufferUniquePtr buffer_;
  /*!
   * \brief The PDU message header.
   */
  amsr::someip_protocol::internal::PduMessageHeader message_header_;

  /*!
   * \brief The Meta data of the message.
   */
  MetaData meta_data_;
};

}  // namespace internal
}  // namespace someipd_app_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIPD_APP_PROTOCOL_INTERNAL_PDU_MESSAGE_H_

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
/*!        \file pdu_message.h
 *        \brief This class represents a single complete PDU message.
 *        \unit SomeIpProtocol::Messages::PduMessage
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_PDU_MESSAGE_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_PDU_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <utility>

#include "ara/core/array.h"
#include "ara/core/optional.h"
#include "ara/core/vector.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/deserialization/deser_pdu_header.h"
#include "someip-protocol/internal/deserialization/types.h"
#include "someip-protocol/internal/logging/someip_protocol_log_builder.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/ser_pdu_header.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip-protocol/internal/someip_protocol_error_code.h"
#include "vac/language/location.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {

/*!
 * \brief PDU message class.
 * \vprivate Product private
 */
class PduMessage final {  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
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
   * \brief     Named constructor of PduMessage.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \return    PduMessage object containing the complete PDU message, or an error if the buffer size is too small
   *            for the PDU message header.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::Result<PduMessage> CreatePduMessage(PduMessage::DataBufferUniquePtr buffer) {
    amsr::core::Result<PduMessage> result{amsr::core::Result<PduMessage>::FromError(
        amsr::someip_protocol::MakeErrorCode(SomeIpProtocolErrc::buffer_too_small, 0, ""))};
    // clang-format off
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if ((buffer != nullptr) && (buffer->size() >= kPduHeaderSize)) {  // VCA_SOMEIPPROTOCOL_MSG_BUFFER
      PduMessageHeader const header{DeserializePduMessageHeader(buffer.get())};
      // clang-format off
      if (buffer->size() >= (static_cast<std::size_t>(header.length_) + kPduHeaderSize)) {  // VCA_SOMEIPPROTOCOL_MSG_BUFFER
        result.EmplaceValue(PduMessage(std::move(buffer), header)); // VCA_SOMEIPPROTOCOL_PDU_MSG_REFERENCE, VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED, VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
        // clang-format on
      }
    }
    return result;
  }

  /*!
   * \brief     Named constructor of PduMessage.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \param[in] time_stamp A time stamp for when the PDU message was received.
   * \return    PduMessage object containing the complete PDU message, or an error if the buffer size is too small
   *            for the PDU message header.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::Result<PduMessage> CreatePduMessage(PduMessage::DataBufferUniquePtr buffer, TimeStamp time_stamp) {
    using Result = amsr::core::Result<PduMessage>;
    Result result{Result::FromError(amsr::someip_protocol::MakeErrorCode(SomeIpProtocolErrc::buffer_too_small, 0, ""))};
    // clang-format off
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if ((buffer != nullptr) && (buffer->size() >= kPduHeaderSize)) { // VCA_SOMEIPPROTOCOL_MSG_BUFFER
      PduMessageHeader const header{DeserializePduMessageHeader(buffer.get())};
      if (buffer->size() >= (static_cast<std::size_t>(header.length_) + kPduHeaderSize)) {  // VCA_SOMEIPPROTOCOL_MSG_BUFFER
        result.EmplaceValue(PduMessage(std::move(buffer), header, time_stamp)); // VCA_SOMEIPPROTOCOL_PDU_MSG_REFERENCE, VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED, VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
      }
      // clang-format on
    }
    return result;
  }

  /*!
   * \brief   Returns the PDU message header of the contained PDU message.
   * \return  PDU message header.
   * \pre     PduMessage must be initialized with a valid header
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  PduMessageHeader& GetHeader() { return message_header_; }

  /*!
   * \brief   Returns the optional time stamp value of the PDU message's meta data.
   * \return  Reference to the optional Time stamp for when the PDU message was received.
   * \pre     PduMessage must be initialized with a valid TimeStamp.
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  ara::core::Optional<TimeStamp> const& GetTimeStamp() const { return meta_data_.rx_time_stamp; }

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
   * \brief   Returns the total size of the contained PDU message with header.
   * \return  Total PDU message size.
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
   * \brief   Returns the total size of the contained PDU message body w/o header.
   * \return  Size of PDU message body.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetBodySize() const {
    // clang-format off
    return (buffer_ != nullptr) ? (buffer_->size() - kPduHeaderSize) : 0; // VCA_SOMEIPPROTOCOL_MSG_BUFFER
    // clang-format on
  }
  /*!
   * \brief   Get memory buffer containing the PDU message.
   * \return  Memory buffer containing the PDU message.
   * \pre     PduMessage must be initialized with a valid buffer
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  DataBufferPtr GetBuffer() const { return buffer_.get(); }
  /*!
   * \brief   Releases the ownership of the memory buffer.
   * \return  Memory buffer containing the PDU message.
   * \pre     PduMessage must be initialized with a valid buffer
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  DataBufferUniquePtr ReleasePacket() { return std::move(buffer_); }

  /*!
   * \brief   Verify if the time stamp is emplaced or not.
   * \return  True if the time stamp is emplaced, false otherwise.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  bool HasTimeStamp() const { return (meta_data_.rx_time_stamp.has_value()); }
  /*!
   * \brief     Set the time stamp for the PDU messages' meata data.
   * \param[in] time_stamp The time stamp for when the PDU message was received.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  void SetTimeStamp(TimeStamp& time_stamp) { meta_data_.rx_time_stamp.emplace(time_stamp); }

  /*!
   * \brief     Set the accumulation timeout of the SOME/IP message's meta data.
   * \param[in] accumulation_timeout The accumulation timeout.
   * \pre       -
   * \context   Reactor|App
   */
  void SetAccumulationTimeout(ara::core::Optional<std::chrono::nanoseconds> const accumulation_timeout) noexcept {
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
  void SetPduId(PduId id) {
    // clang-format off
    if (buffer_) {  // VCA_SOMEIPPROTOCOL_MSG_BUFFER
      // clang-format on
      ara::core::Vector<std::uint8_t> data(
          someip_protocol::internal::kPduHeaderSize);  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC

      serialization::Writer writer{serialization::BufferView{data}};
      message_header_.pdu_id_ = id;
      SerializePduMessageHeader(writer, message_header_, message_header_.length_);
      // Ignore return value. Size already verified.

      // clang-format off
      static_cast<void>(buffer_->CopyIn(0U, data.size(), data.data()));  // VCA_SOMEIPPROTOCOL_MSG_BUFFER
      // clang-format on
    }  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
  }

 private:
  /*!
   * \brief     Constructor of PduMessage.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \param[in] header A deserialized PDU message header
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  explicit PduMessage(DataBufferUniquePtr buffer, PduMessageHeader const& header)
      : buffer_{std::move(buffer)}, message_header_{header}, meta_data_{} {}

  /*!
   * \brief     Constructor of PduMessage.
   * \param[in] buffer A data buffer containing a complete and valid PDU message.
   * \param[in] header A deserialized PDU message header
   * \param[in] time_stamp A time stamp for when the PDU message was received.
   * \pre       -
   * \context   Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  explicit PduMessage(DataBufferUniquePtr buffer, PduMessageHeader const& header, TimeStamp time_stamp)
      : PduMessage(std::move(buffer), header) {  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
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
   *
   * \internal
   * - Copy the message header from buffer to a temporary buffer.
   * - Create a reader with the temporary buffer.
   * - Deserialize the PDU header.
   * - Return the deserialized PDU header.
   * \endinternal
   */
  static PduMessageHeader DeserializePduMessageHeader(DataBufferPtr const buffer) {
    // First copy the PDU message header to a temporary continuous buffer
    ara::core::Array<std::uint8_t, kPduHeaderSize> header_buffer{};
    std::size_t const header_offset{0U};
    // Extract the PDU message header from the possibly fragmented buffer
    // clang-format off
    static_cast<void>(buffer->CopyOut(header_offset, header_buffer.size(), header_buffer.data())); // VCA_SOMEIPPROTOCOL_MSG_BUFFER
    // clang-format on
    // Deserialize the PDU message header
    someip_protocol::internal::PduMessageHeader header{0U, 0U};
    // Get the buffer view containing PDU header
    ::amsr::someip_protocol::internal::PacketBufferView packet_buffer{header_buffer.data(), header_buffer.size()};
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
  PduMessageHeader message_header_;

  /*!
   * \brief The Meta data of the message.
   */
  MetaData meta_data_;
};

}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_PDU_MESSAGE_H_

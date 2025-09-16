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
/**        \file  someip_tp_segmented_iteration.h
 *        \brief  SOME/IP-TP
 *      \details  Transporting large SOME/IP messages via UDP/IP
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENTED_ITERATION_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENTED_ITERATION_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/vector.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"
#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_tp/someip_tp_header.h"
#include "someip_tp/someip_tp_validation.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief Iterates an unsegmented SOMEIP message in bursts of SOMEIP/TP segments.
 * \trace SPEC-4981397
 * \trace SPEC-10144451
 */
class SegmentedIteration final {
 public:
  /*!
   * \brief   Reset the object for an iteration on a new packet.
   * \param   packet                  SOMEIP message
   * \param   maximum_segment_length  Maximum segment length in bytes. As seen by the SOMEIP header's length field.
   * \return  true                    Correctly set.
   * \return  false                   Validation failure. GetNext can not be called.
   * \steady  TRUE
   */
  bool Reset(someip_protocol::internal::SomeIpMessage const& packet,
             someip_protocol::internal::MTU const maximum_segment_length) {
    bool const valid{someip_tp::ValidateParameters(packet.GetBodySize(), maximum_segment_length)};
    payload_ = {};
    current_tp_offset_ = 0U;
    segment_length_ = 0u;

    if (valid) {
      segment_length_ = ToEffectiveSegmentLength(maximum_segment_length);
      assert(segment_length_ > 0);
      std::size_t const payload_size{packet.GetBodySize()};
      // if separation time is zero we prepare all datagrams in a single burst
      std::size_t const max_datagrams_in_one_burst{(payload_size / segment_length_) +
                                                   static_cast<std::size_t>((payload_size % segment_length_) != 0U)};
      // Extend storage only if we need more than we already have.
      if (max_datagrams_in_one_burst > headers_.size()) {
        headers_.resize(max_datagrams_in_one_burst);
        burst_storage.resize(max_datagrams_in_one_burst);
        burst.reserve(max_datagrams_in_one_burst);
      }
      payload_ = {
          std::addressof(*packet.GetBuffer()->GetIterator(sizeof(someip_protocol::internal::SomeIpMessageHeader))),
          payload_size};
      header_template_ = packet.GetHeader();
      header_template_.message_type_ = ToSomeIpTpMessageType(header_template_.message_type_);
    }
    return valid;
  }

  /*!
   * \brief Gets a burst of segmented messages.
   *
   * \param   max_n_segments maximum number of segments to advance the iteration
   * \return  ara::core::Span<ara::core::Span<osabstraction::io::ConstIOBuffer>> view of the messages to be sent
   * -  // VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_SomeIpDaemon_AutosarC++17_10-M7.1.2_paramCanBePtrRefToConst
   * \steady  TRUE
   * \trace SPEC-4981414
   * \trace SPEC-4981415
   * \trace SPEC-4981424
   * \trace SPEC-4981425
   * \trace SPEC-4981427
   * \trace SPEC-4981428
   * \trace SPEC-4981429
   * \trace SPEC-10144456
   * \trace SPEC-10144457
   * \trace SPEC-10144460
   * \trace SPEC-10144467
   * \trace SPEC-10144468
   * \trace SPEC-9644222
   * \trace SPEC-9644223
   * \trace SPEC-9644224
   *
   * \internal
   * - Update internal storage for a new burst.
   * - Prepare at most 'max_n_segments' segments for this burst.
   * - Return a view for the burst.
   * \endinternal
   */
  ara::core::Span<ara::core::Span<osabstraction::io::ConstIOBuffer>> GetNext(std::size_t const max_n_segments) {
    assert(!HasEnded());         // Bug
    assert(max_n_segments > 0);  // Bug

    burst.clear();
    // Prepare this burst.
    for (std::size_t i{0U}; i < max_n_segments; i++) {
      std::size_t const segment_size{std::min(payload_.size(), segment_length_)};
      bool const more_segments{payload_.size() > segment_length_};
      HeaderArray& header{headers_[i]};
      someip_protocol::internal::serialization::Writer writer{{header}};
      SerializeSomeIpMessageHeader(writer, header_template_, kTpHeaderLength + segment_size);
      SomeIpTpHeader const tp_header{SomeIpTpOffset{static_cast<SomeIpTpOffsetField>(current_tp_offset_)},
                                     more_segments};
      writer.writePrimitive<SomeIpTpHeaderField, someip_protocol::internal::BigEndian>(tp_header.GetTpHeader());
      burst_storage[i][0U] = osabstraction::io::ConstIOBuffer{static_cast<void*>(&header), header.size()};
      burst_storage[i][1U] = osabstraction::io::ConstIOBuffer{static_cast<void*>(payload_.data()), segment_size};
      burst.push_back(ara::core::MakeSpan(burst_storage[i]));
      if (more_segments) {
        current_tp_offset_ += segment_size;
        payload_ = payload_.subspan(segment_size);  // Advance by one segment size
      } else {
        payload_ = {};
        break;  // Done!
      }
    }

    // Return a view for this burst.
    return ara::core::MakeSpan(burst);
  }

  /*!
   * \brief   Gets if the iteration is complete
   * \return  true  The whole message was iterated.
   * \return  false There are still segments pending.
   * \steady  TRUE
   */
  bool HasEnded() const noexcept { return payload_.empty(); }

 private:
  /*!
   * \brief Memory chunk to contain all headers to be redone by SOMEIP/TP before sending
   */
  using HeaderArray = std::array<uint8_t, someip_protocol::internal::kHeaderSize + kTpHeaderLength>;
  /*!
   * \brief Storage for prepared SOME/IP-TP headers. Intended to be reused.
   */
  ara::core::Vector<HeaderArray> headers_{};
  /*!
   * \brief Storage for prepared TP segments ([1] -> Header, [2] -> Payload segment). Intended to be reused.
   */
  ara::core::Vector<ara::core::Array<osabstraction::io::ConstIOBuffer, 2U>> burst_storage{};

  /*!
   * \brief Storage for views of prepared TP segments. Intended to be reused.
   */
  ara::core::Vector<ara::core::Span<osabstraction::io::ConstIOBuffer>> burst{};

  /*!
   * \brief A view for the payload of the current message which shall be fragmented. Intended to be reused.
   * \details Is overwritten with an updated span offset by a segment length for each segmentation step.
   */
  ara::core::Span<std::uint8_t> payload_{};
  /*!
   * \brief Original header
   */
  amsr::someip_protocol::internal::SomeIpMessageHeader header_template_{};
  /*!
   * \brief Maximum number of bytes on each segment
   */
  std::size_t segment_length_{0u};
  /*!
   * \brief The current TP offset to set in a TP header. Intended to be reused.
   */
  std::size_t current_tp_offset_{0U};
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENTED_ITERATION_H_

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
/**        \file  someip_tp_reception_assembler.h
 *        \brief  SOME/IP-TP
 *      \details  Transporting large SOME/IP messages via UDP/IP.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_ASSEMBLY_ASSEMBLER_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_ASSEMBLY_ASSEMBLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <array>
#include <limits>
#include <memory>
#include <utility>
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_tp/logging/ara_com_logger.h"
#include "someip_tp/someip_tp_header.h"
#include "someip_tp/someip_tp_segment.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief SOME/IP-TP segment assembler on the receiver-side.
 */
class Assembler final {
 public:
  using Message = ara::core::Vector<std::uint8_t, ara::core::PolymorphicAllocator<std::uint8_t>>;
  /*!
   * \brief Constructor.
   *
   * \param[in] max_message_size        The maximum size of the assembled message, including headers.
   * \param[in, out] allocator          The allocator used to create message buffers.
   * \steady    TRUE
   * \trace     SPEC-4981431
   * \trace     SPEC-4981431
   */
  Assembler(std::size_t const max_message_size, ara::core::PolymorphicAllocator<std::uint8_t>& allocator) noexcept
      : max_msg_size_{max_message_size},
        allocator_{allocator},
        session_{std::numeric_limits<someip_protocol::internal::SessionId>::max()},
        msg_{allocator_},
        beg_{std::numeric_limits<std::size_t>::max()},
        end_{0U},
        cancelled_{false},
        logger_{"someip_tp::Assembler"} {}

  /*!
   * \brief Cancels the assembly process.
   * \steady FALSE
   * \details A new assembly process is started when a different session ID is received for a SOME/IP-TP segment.
   */
  void Cancel() noexcept { cancelled_ = true; }

  /*!
   * \brief       Assemble a SOME/IP message from the provided SOME/IP-TP segment.
   * \details     Segments which arrive out-of-order are not supported.
   * \param[in]   tp_segment The SOME/IP-TP segment to assemble from.
   * \return      The re-assembled message, or an empty optional.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace       SPEC-4981437
   * \trace       SPEC-4981438
   * \trace       SPEC-4981439
   *
   * \internal
   *  - If this is a new assembly store state information from the SOME/IP header
   *  - Check if the assembly process has been cancelled previosuly. Log a message if it has been cancelled.
   *   - Check the segment's validity
   *     - Process the segment
   *  - Return with the assembled message when it's complete
   * \endinternal
   */
  ara::core::Optional<Message> Assemble(SomeIpTpSegment const& tp_segment) noexcept {
    ara::core::Optional<Message> result{ara::core::nullopt};
    Update(tp_segment.SomeIpHeader());
    if (!cancelled_) {
      if (Validate(tp_segment)) {  // All checks passed.
        result = Process(tp_segment);
      }
    } else {
      logger_.LogVerbose(
          [this, &tp_segment](ara::log::LogStream& s) noexcept {
            s << "Re-assembly has been cancelled due to a previous error. Dropping segment. ";
            LogCurrentStatus(s);
            s << " Received ";
            amsr::someip_protocol::internal::LogSomeIpMessageHeader(s, tp_segment.SomeIpHeader());
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

 private:
  /*!
   * \brief Check if the received segments offset is within the internal buffer's boundaries.
   * \param[in] segment Segment to check.
   * \return True if inside, otherwise false.
   */
  bool CheckBoundaries(SomeIpTpSegment const& segment) const noexcept {
    std::size_t const beg{segment.TpHeader().GetOffset().GetOffsetValue() + someip_protocol::internal::kHeaderSize};
    std::size_t const end{beg + segment.Payload().size()};
    return msg_.size() >= end;
  }
  /*!
   * \brief Check if the received segment is the last one and all expected segments were already received.
   * \param[in] segment Segment to check.
   * \return True if more segments are expected or no segments are missing, otherwise false.
   */
  bool CheckMissingSegments(SomeIpTpSegment const& segment) const noexcept {
    std::size_t const offset{segment.TpHeader().GetOffset().GetOffsetValue()};
    std::size_t const end{offset + segment.Payload().size()};
    bool const continuous{offset <= end_ && end >= beg_};  // overlaps allowed
    bool const first_msg{end_ == 0U};
    bool const more{segment.TpHeader().HasMoreSegments()};
    // either this is the last segment in descending order, or we have the last segment received in out-of-order.
    bool const descending_order_complete{!more && ((beg_ == 0U) || (offset == 0U))};
    bool const check{(continuous || first_msg) && descending_order_complete};
    return more ? more : check;
  }

  /*!
   * \brief Check if the received segment has a valid TP header and payload length.
   * \param[in] segment Segment to check.
   * \return True if the TP offset and segment length is valid, otherwise false.
   */
  bool CheckOffsetAndLength(SomeIpTpSegment const& segment) const noexcept {
    bool const more{segment.TpHeader().HasMoreSegments()};
    std::size_t const offset_valid{(segment.TpHeader().GetOffset().GetOffsetValue() % 16U) == 0U};
    // only the last segment can have a length which is not a multiple of 16.
    std::size_t const payload_valid{more ? ((segment.Payload().size() % 16U)) == 0U : true};
    return offset_valid && payload_valid;
  }

  /*!
   * \brief Validate the segment priot to processing.
   * \param[in] tp_segment The segment to validate.
   * \return true if the segment is valid.
   * \return false if the segment is not ok.
   */
  bool Validate(SomeIpTpSegment const& tp_segment) noexcept {
    // [PRS_SOMEIP_00743] The receiver should only reassemble up to its configured
    // buffer size and skip the rest of the message. Fulfilled
    // [PRS_SOMEIP_00749] When a missing segment is detected during assembly of a
    // SOME/IP message, the current assembly process shall be canceled. Fulfilled.
    // [PRS_SOMEIP_00753] The receiver may cancel reassembly, if overlapping or duplicated segments change already
    // written bytes in the buffer, if this feature can be turned off by configuration. Not fulfilled.
    // [PRS_SOMEIP_00754] The receiver shall be able to detect and handle obvious errors gracefully. E.g. cancel
    // reassembly if segment length of a segment with MS=1 is not a multiple of 16. Fulfilled.
    someip_protocol::internal::SomeIpMessageHeader const& someip_header{tp_segment.SomeIpHeader()};
    bool is_valid_segment{true};
    if (!CheckBoundaries(tp_segment)) {  // [PRS_SOMEIP_00743]
      cancelled_ = true;                 // [PRS_SOMEIP_00744]
      is_valid_segment = false;
      logger_.LogVerbose(
          [this, &someip_header](ara::log::LogStream& s) noexcept {
            s << "SOME/IP-TP re-assembly error. Out-of-bounds! Received TP segment specifies a writing range outside "
                 "of the "
                 "internal buffer's boundaries. Re-assembly process cancelled. Dropping segment. ";
            LogCurrentStatus(s);
            s << " Received ";
            amsr::someip_protocol::internal::LogSomeIpMessageHeader(s, someip_header);
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    if (!CheckMissingSegments(tp_segment)) {
      cancelled_ = true;  // [PRS_SOMEIP_00749]
      is_valid_segment = false;
      logger_.LogVerbose(
          [this, &someip_header](ara::log::LogStream& s) noexcept {
            s << "SOME/IP-TP re-assembly error. Missing segments! The last segment was received before all remaining "
                 "segments were received. Re-assembly process cancelled. Dropping segment. ";
            LogCurrentStatus(s);
            s << " Received ";
            amsr::someip_protocol::internal::LogSomeIpMessageHeader(s, someip_header);
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    if (!CheckOffsetAndLength(tp_segment)) {
      cancelled_ = true;  // [PRS_SOMEIP_00754]
      is_valid_segment = false;
      logger_.LogVerbose(
          [this, &someip_header](ara::log::LogStream& s) noexcept {
            s << "SOME/IP-TP re-assembly error. The received segment is not according to specification. Re-assembly "
                 "process cancelled. Dropping segment. ";
            LogCurrentStatus(s);
            s << " Received ";
            amsr::someip_protocol::internal::LogSomeIpMessageHeader(s, someip_header);
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return is_valid_segment;
  }

  /*!
   * \brief Updates the assemblers state.
   * \steady TRUE
   * \param[in] someip_header A new SOME/IP header to check for session ID.
   */
  void Update(someip_protocol::internal::SomeIpMessageHeader const& someip_header) noexcept {
    // [PRS_SOMEIP_00741] The Session ID shall be used to detect the next original message to be reassembled. Fulfilled.
    // [PRS_SOMEIP_00742] The receiver shall start a new reassembly (and may throw
    // away old segments that were not successfully reassembled), if a new segment with a
    // different Session-ID is received. Fulfilled.
    // [PRS_SOMEIP_00750] Interleaving of different segmented messages using the
    // same buffer (e.g. only the Session-ID and payload are different) is not supported. Fulfilled.
    if (someip_header.session_id_ != session_) {  // [PRS_SOMEIP_00741], [PRS_SOMEIP_00742]
      if (!msg_.empty()) {
        logger_.LogVerbose(
            [this, &someip_header](ara::log::LogStream& s) noexcept {
              s << "A different session ID was received while there is an ongoing assembly process. ";
              LogCurrentStatus(s);
              s << " Received ";
              amsr::someip_protocol::internal::LogSomeIpMessageHeader(s, someip_header);
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      cancelled_ = false;
      session_ = someip_header.session_id_;
      beg_ = std::numeric_limits<std::size_t>::max();
      end_ = 0U;
    }
    // Check state of buffer to see if a new one is needed after a successful re-assembly moved the message out
    // OR if this is the very first call to assembler. In both cases we need a new buffer.
    if (msg_.empty()) {
      msg_ = Message(max_msg_size_, allocator_);
    }
  }

  /*!
   * \brief Stores the segments payload in the message at the correct offset and updates the state of the assembler.
   * \param[in] segment Segment to store.
   * \steady FALSE
   * \return Optional with Message if the message is complete, otherwise nullopt.
   */
  ara::core::Optional<Message> Process(SomeIpTpSegment const& segment) noexcept {
    // [PRS_SOMEIP_00744] Only correctly reassembled message of up to the configured
    // size shall be passed to an application. Fulfilled.
    // [PRS_SOMEIP_00745] The Return Code of the last segment used for reassembly
    // shall be used for the reassembled message. Fulfilled.
    // [PRS_SOMEIP_00746] During reassembling the SOME/IP TP segments into a large
    // unsegmented message, the Message Type shall be adapted, the TP Flag shall be reset to 0. Fulfilled.
    // [PRS_SOMEIP_00747] The receiver shall support reassembly of segments that are
    // received in ascending and descending order. Fulfilled.
    // [PRS_SOMEIP_00752] The receiver shall correctly reassemble overlapping and duplicated segments by overwriting
    // based on the last received segment. Fulfilled.
    ara::core::Optional<Message> result{ara::core::nullopt};
    std::size_t const beg{segment.TpHeader().GetOffset().GetOffsetValue()};
    std::size_t const end{beg + segment.Payload().size()};
    ara::core::Span<std::uint8_t> const payload{segment.Payload()};
    // [PRS_SOMEIP_00747], [PRS_SOMEIP_00752]
    std::copy(payload.begin(), payload.end(), std::next(msg_.begin(), someip_protocol::internal::kHeaderSize + beg));
    beg_ = std::min(beg, beg_);
    end_ = std::max(end, end_);
    if (!segment.TpHeader().HasMoreSegments()) {
      someip_protocol::internal::SomeIpMessageHeader final_header{segment.SomeIpHeader()};  // [PRS_SOMEIP_00745]
      final_header.length_ =
          static_cast<someip_protocol::internal::LengthField>(someip_protocol::internal::kHeaderLength + end_);
      final_header.message_type_ = FromSomeIpTpMessageType(final_header.message_type_);  // [PRS_SOMEIP_00746]
      someip_protocol::internal::serialization::Writer writer{msg_};
      // Serialize the header before the payload.
      someip_protocol::internal::serialization::SerializeSomeIpMessageHeaderObject(writer, final_header);
      msg_.resize(someip_protocol::internal::kHeaderSize + end_);  // shrink to fit actual message size
      result.emplace(std::move(msg_));
    }
    return result;
  }

  /*!
   * \brief Logs the current state if the assembler.
   * \steady FALSE
   * \param[in, out] s Logstream to write to.
   */
  void LogCurrentStatus(ara::log::LogStream& s) const noexcept {
    s << "Current assembler status: Cancelled? ";
    s << cancelled_;
    s << " SessionID [";
    s << session_;
    s << "] Begin:End [";
    s << beg_;
    s << ":";
    s << end_;
    s << "].";
  }

  /*!
   * \brief The configured maximum size of the SOME/IP message.
   */
  std::size_t const max_msg_size_;

  /*!
   * \brief The allocator used for re-assembly.
   */
  ara::core::PolymorphicAllocator<std::uint8_t>& allocator_;

  /*!/
   * \brief The current SessionID
   */
  someip_protocol::internal::SessionId session_;

  /*!
   * \brief Container for the re-assembled message.
   */
  Message msg_;

  /*!
   * \brief Beginning position of the current continuous written memory chunk.
   */
  std::size_t beg_;

  /*!
   * \brief End position of the current continuous written memory chunk. The end position is unwritten.
     size = end_ - beg_;
   */
  std::size_t end_;

  /*!
   * \brief Flag to keep track of if the current assembly has been cancelled.
   */
  bool cancelled_;

  /*!
   * \brief Logger.
   */
  logging::AraComLogger logger_;
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_ASSEMBLY_ASSEMBLER_H_

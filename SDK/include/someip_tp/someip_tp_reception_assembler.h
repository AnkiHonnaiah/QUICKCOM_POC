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
#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_RECEPTION_ASSEMBLER_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_RECEPTION_ASSEMBLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <array>
#include <limits>
#include <memory>
#include <utility>

#include "amsr/unique_ptr.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_tp/someip_tp_copy_operation.h"
#include "someip_tp_header.h"

#include "amsr/unique_ptr.h"
#include "vac/memory/allocator/deterministic_memory_buffer.h"
#include "vac/memory/allocator/deterministic_memory_buffer_allocator.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief Current state of one assembler on receiver-side. This state signalizes if all segments have been assembled to
 * the original SOME/IP message or if there are segments received at all.
 */
enum class AssemblerState : std::uint8_t {
  /*!
   * \brief No segments have been received
   */
  kBlank,
  /*!
   * \brief Reception has started
   */
  kOngoing,
  /*!
   * \brief A full message is ready
   */
  kDone,
  /*!
   * \brief Failure state. Reassembling will not be continued.
   */
  kError
};

/*!
 * \brief Error Detection for the SOME/IP segments.
 */
enum class SegmentStatus : std::uint8_t {
  /*!
   * \brief Segment valid
   */
  kOk,
  /*!
   * \brief Allocation failure
   */
  kAllocFailure,
  /*!
   * \brief The SOME/IP-TP offset from this segment points to an offset in the
   *        overall original message to assemble that will exceed the provided
   *        buffer size for re-assembling.
   */
  kConfiguredBufferSizeOverflow,
  /*!
   * \brief The received protocol versions, interface versions or message types are inconsistent within each segment.
   */
  kHeaderInfoMismatch,
  /*!
   * \brief One or more Segments are missed while receiving.
   */
  kMissingSegments,
  /*!
   * \brief Aborted, there was an error on previoos segments.
   */
  kAborted,
};

/*!
 * \brief SOME/IP-TP segment assembler on the receiver-side.
 */
class SomeIpTpMessageReceptionAssembler final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in] n_preallocated_messages The number of message for the internal allocator.
   * \param[in] max_message_size        The maximum size of the assembled message, including headers.
   * \param[in] session                 Current session of this segmented method / event to re-assemble.
   *                                    The defragmentation could be reset externally from a controller that holds
   *                                    this assembler, if a new session ID is transmitted within a segment.
   * \steady    TRUE
   * \trace     SPEC-4981431
   * \trace     SPEC-4981431
   */
  SomeIpTpMessageReceptionAssembler(std::size_t const n_preallocated_messages, std::size_t const max_message_size,
                                    someip_protocol::internal::SessionId const session) noexcept
      : assembler_state_{AssemblerState::kBlank},
        session_{session},
        protocol_version_{someip_protocol::internal::kProtocolVersion},
        interface_version_{someip_protocol::internal::kMajorVersionAny},
        message_type_{someip_protocol::internal::SomeIpMessageType::kError},
        alloc_{MakeUniqueOrAbort<vac::memory::allocator::deterministic::DeterministicMemoryBufferAllocator>(
            n_preallocated_messages, max_message_size)},
        msg_{nullptr},
        beg_{std::numeric_limits<std::size_t>::max()},
        end_{std::numeric_limits<std::size_t>::min()},
        end_max_{max_message_size - someip_protocol::internal::kHeaderSize} {
    assert(end_max_ < max_message_size);  // overflow check
    assert(n_preallocated_messages >= 1U);
  }

  /*!
   * \brief   Get the current session being re-assembled.
   * \return  Current session of this assembler.
   * \steady  TRUE
   */
  someip_protocol::internal::SessionId GetCurrentSession() const noexcept { return session_; }

  /*!
   * \brief   Get the current assembler.
   * \return  Current state of this assembler.
   * \steady  TRUE
   */
  AssemblerState GetAssemblerState() const noexcept { return assembler_state_; }

  /*!
   * \brief   Set a new session.
   * \param   session The new session ID.
   * \steady  TRUE
   */
  void SetCurrentSession(someip_protocol::internal::SessionId const session) noexcept { session_ = session; }

  /*!
   * \brief       This method is invoked whenever one segment is received for re-assembling.
   * \details     This implementation is currently not capable of handling send orders that leave unwritten holes.
   * \tparam      OpType Type of CopyOperation
   * \param[in]   someip_header The SOME/IP header of this segment already deserialized.
   * \param[in]   tp_header     The SOME/IP-TP header of this segment already deserialized.
   * \param[in]   payload       The SOME/IP-TP payload segment which is appended to the existing buffer.
   * \return      The current state of this assembler and the status of this segment received.
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
   * - Check state:
   *   - If there are no segments received allocate save the header data and try to allocate a buffer if there is none
   *     already.
   *     - If allocation succeeds set the state to kOngoing
   *     - Else set the state to kError and the segment state to kBufferAllocationFailed
   *   - If there are segments received match the header data agains the saved header data from the first message.
   *     - If there is a mismatch set the state to kError and the segment state to kHeaderInfoMismatch.
   *   - For any other state ignore the passed segment.
   *  - If the state is kOngoing:
   *    - If the chunk is contiguous and under doesn't exceed the maximum size copy it to the payload.
   *    - Else set the state and error code depending on which constraint was violated and the state to kError.
   *    - If the message has the more flag to false, check that the first offset is zero. If not set the error to
   *      kMissingSegments and the state to kError.
   *  - If the state is kOngoing and the message has the more flag to false:
   *    - Copy the header into the buffer with no tp information.
   *    - Set the state to kDone.
   * \endinternal
   */
  // VECTOR NC Metric-HIS.PATH: MD_SomeIpDaemon_Metric-HIS.PATH
  // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
  template <typename OpType>
  std::pair<AssemblerState, SegmentStatus> Assemble(someip_protocol::internal::SomeIpMessageHeader const& someip_header,
                                                    SomeIpTpHeader const& tp_header,
                                                    CopyOperation<OpType> const& payload_copy) noexcept {
    SegmentStatus segment_status{SegmentStatus::kOk};

    // state handling
    switch (assembler_state_) {
      case AssemblerState::kBlank: {
        interface_version_ = someip_header.interface_version_;
        message_type_ = someip_header.message_type_;
        assembler_state_ = AssemblerState::kOngoing;
        if (!msg_) {
          using BufferResult =
              ara::core::Result<amsr::UniquePtr<vac::memory::allocator::deterministic::DeterministicMemoryBuffer>>;
          BufferResult buffer{alloc_->AllocateDeterministic(end_max_ + someip_protocol::internal::kHeaderSize)};
          if (buffer.HasValue()) {
            msg_ = std::move(buffer.Value());
          } else {
            segment_status = SegmentStatus::kAllocFailure;
            assembler_state_ = AssemblerState::kError;
          }
        }
        break;
      }
      case AssemblerState::kOngoing: {
        if ((protocol_version_ != someip_header.protocol_version_) ||
            (interface_version_ != someip_header.interface_version_) ||
            (message_type_ != someip_header.message_type_)) {
          assembler_state_ = AssemblerState::kError;
          segment_status = SegmentStatus::kHeaderInfoMismatch;
        }
        break;
      }
      case AssemblerState::kDone: {
        break;
      }
      case AssemblerState::kError: {
        segment_status = SegmentStatus::kAborted;
        break;
      }
      default: {
        break;
      }
    }

    // payload writing
    if (assembler_state_ == AssemblerState::kOngoing) {
      std::size_t const beg{tp_header.GetOffset().GetOffsetValue()};
      std::size_t const end{beg + payload_copy.Size()};

      bool const contiguous{(beg <= end_) && (end >= beg_)};  // overlaps allowed
      bool const first_msg{end_ == 0};
      bool const in_bounds{end <= end_max_};
      bool const ok{in_bounds && (contiguous || first_msg)};

      if (ok) {
        assert(!!msg_);
        payload_copy(msg_->GetView(someip_protocol::internal::kHeaderSize + beg));
        beg_ = std::min(beg, beg_);
        end_ = std::max(end, end_);
      } else {
        assembler_state_ = AssemblerState::kError;
        segment_status = in_bounds ? SegmentStatus::kMissingSegments : SegmentStatus::kConfiguredBufferSizeOverflow;
      }
      if ((ok) && (!tp_header.HasMoreSegments()) && (beg_ != 0)) {
        assembler_state_ = AssemblerState::kError;
        segment_status = SegmentStatus::kMissingSegments;
      }
    }
    // message finishing. header writing.
    if ((assembler_state_ == AssemblerState::kOngoing) && (!tp_header.HasMoreSegments())) {
      using BE = someip_protocol::internal::BigEndian;
      namespace ipproto = someip_protocol::internal;

      assembler_state_ = AssemblerState::kDone;
      // trim the size of the probably too big buffer
      static_cast<void>(msg_->ReduceSize(end_ + someip_protocol::internal::kHeaderSize));
      someip_protocol::internal::serialization::Writer ser{msg_->GetView(0)};

      ipproto::SomeIpMessageHeader header{someip_header};
      header.length_ = static_cast<ipproto::LengthField>(ipproto::kHeaderLength + end_);
      header.message_type_ = FromSomeIpTpMessageType(header.message_type_);  // drop TP flag
      ser.writePrimitive<ipproto::ServiceId, BE>(header.service_id_);
      ser.writePrimitive<ipproto::MethodId, BE>(header.method_id_);
      ser.writePrimitive<ipproto::LengthField, BE>(header.length_);
      ser.writePrimitive<ipproto::ClientId, BE>(header.client_id_);
      ser.writePrimitive<ipproto::SessionId, BE>(header.session_id_);
      ser.writePrimitive<ipproto::ProtocolVersion, BE>(header.protocol_version_);
      ser.writePrimitive<ipproto::InterfaceVersion, BE>(header.interface_version_);
      ser.writePrimitive<ipproto::MessageType, BE>(static_cast<ipproto::MessageType>(header.message_type_));
      ser.writePrimitive<ipproto::ReturnCode, BE>(static_cast<ipproto::ReturnCode>(header.return_code_));
    }
    return std::make_pair(assembler_state_, segment_status);
  }

  /*!
   * \brief       Reset the state of this assembler to be able to start with another segmentation session, also payload
   *              length is reset. The internal buffer for message storage is kept.
   * \return      The assembler state of this object
   *              that has been reset. Should return AssemblerState::kBlank.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \internal
   * - Reset beg_ and end_ to 0.
   * - Reset assembler_state_ to kBlank.
   * - Return assembler state.
   * \endinternal
   */
  AssemblerState PrepareForNewAssembly() {
    beg_ = std::numeric_limits<decltype(beg_)>::max();
    end_ = 0u;
    assembler_state_ = AssemblerState::kBlank;
    return assembler_state_;
  }

  /*!
   * \brief   Transfer the memory ownership of the current message.
   * \steady  TRUE
   */
  vac::memory::allocator::MemoryBufferPtr ReleaseMessage() {
    vac::memory::allocator::MemoryBufferPtr ret{};
    if (assembler_state_ == AssemblerState::kDone) {
      ret = vac::memory::allocator::MemoryBufferPtr{msg_.release()};
    }
    return ret;
  }

 private:
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__SomeIpTp, SomeIpTpMessageReceptionAssembler_AllocFailure);
  /*!
   * \brief Track the state of re-assembling.
   */
  AssemblerState assembler_state_;
  /*!/
   * \brief session
   */
  someip_protocol::internal::SessionId session_;
  /*!
   * \brief Track the protocol version of each segment.
   */
  someip_protocol::internal::ProtocolVersion const protocol_version_;

  /*!
   * \brief Track the interface version of each segment.
   */
  someip_protocol::internal::InterfaceVersion interface_version_;

  /*!
   * \brief Track the message type of each segment.
   */
  someip_protocol::internal::SomeIpMessageType message_type_;

  /*!
   * \brief Allocator.
   */
  amsr::UniquePtr<vac::memory::allocator::deterministic::DeterministicMemoryBufferAllocator> alloc_;

  /*!
   * \brief Message memory buffer.
   */
  amsr::UniquePtr<vac::memory::allocator::deterministic::DeterministicMemoryBuffer> msg_{};

  /*!
   * \brief Beginning position of the current contiguous written memory chunk.
   */
  std::size_t beg_;

  /*!
   * \brief End position of the current contiguous written memory chunk. The end position is unwritten.
     size = end_ - beg_;
   */
  std::size_t end_;

  /*!
   * \brief Maximum value that end_ can take. Equal to the maximum allowed size.
   */
  std::size_t end_max_;
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_RECEPTION_ASSEMBLER_H_

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
/*!        \file  someip_tp_reception_controller.h
 *        \brief  SOME/IP-TP Reception Controller manages the incoming SOME/IP messages.
 *      \details  Transporting large SOME/IP messages via UDP/IP
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_RECEPTION_CONTROLLER_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_RECEPTION_CONTROLLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <limits>
#include <tuple>
#include <utility>
#include "ara/core/vector.h"

#include "logging/ara_com_logger.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip_tp.h"
#include "someip_tp_assembler_slot.h"
#include "vac/memory/memory_buffer_wrapper.h"
namespace amsr {
namespace someip_tp {
/*!
 * \brief       Declaration of SOME/IP-TP Reception Controller and management of  incoming SOME/IP messages.
 * |details     The TP-controller is attached to a certain UDP endpoint and forwards SOME/IP segments
 *              to the re-assembling units. On reception of a SOME/IP message an object of type
 *              SomeIpTpReceptionController checks if it's a regular SOME/IP message or a SOME/IP-TP segment.
 *              - If it's a regular SOME/IP message it will be forwarded immediately for processing.
 *              - In case the controller detects that this is a SOME/IP-TP segment, the segment
 *              will be forwarded to a re-assembler object.
 */
class TransportProtocolReceptionController final {
 public:
  /*!
   * \brief         Type-alias for the assembler slot collection of this controller.
   */
  using AssemblerSlots = ara::core::Vector<TransportProtocolAssemblerSlot>;

  /*!
   * \brief             Current reception state for reception.
   */
  enum class ReceptionState : std::uint32_t {
    /*!
     * \brief           No assembler slot available in the reception controller.
     *                  This appears when there is no configuration for an incoming SOME/IP-TP segment.
     */
    kMessageDropped,
    /*!
     * \brief           A regular SOME/IP message has been forwarded to its processor.
     */
    kNonTpMessageForwarded,
    /*!
     * \brief           A SOME/IP-TP segment has been received and forwarded to an assembler.
     */
    kSegmentForwardedToAssembler,
    /*!
     * \brief           The last SOME/IP-TP segment has been received and the complete SOME/IP message
     *                  has been re-assembled successfully by an assembler. It will be forwarded as
     *                  it would be a conventional, non-segmented SOME/IP message.
     */
    kTpMessageForwarded
  };

  /*!
   * \brief         Type-alias for returning reception message to the caller along with reception state.
   */
  using ReceptionResult = std::pair<ReceptionState, ara::core::Optional<someip_protocol::internal::SomeIpMessage>>;

  /*!
   * \brief             Constructs an instance of TransportProtocolReceptionController
   * \context           Init
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            FALSE
   */
  TransportProtocolReceptionController();

  /*!
   * \brief             Construction of a controller using the assembler slots.
   * \param[in]         slots Assembler slots collection for this controller.
   * \context           Init
   * \steady            FALSE
   */
  explicit TransportProtocolReceptionController(AssemblerSlots&& slots);

  /*!
   * \brief             Adds an assembler-slot to this controller based on the configuration.
   *                    For each method / event of a service this method is called to add an assembler slot.
   * \param[in]         slot The assembler slot to add to the collection.
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            FALSE
   */
  void AddAssemblerSlot(TransportProtocolAssemblerSlot&& slot) { slots_.push_back(std::move(slot)); }

  /*!
   * \brief             Evaluates a SOME/IP-TP packet on reception.
   * \tparam            BufferType Type of CopyOperation.
   * \param[in]         assembler The assembler to pass the segment to.
   * \param[in]         header Received SOME/IP header of this segment.
   * \param[in]         tp_header Received and deserialized SOME/IP-TP header of this segment.
   * \param[in]         payload_view A buffer view over the payload received which will be appended.
   * \return            The current reception status for this SOME/IP-TP segment.
   *                    Either the segment is added to an assembler or a successive SOME/IP-TP segments have been
   *                    re-assembled to a complete SOME/IP message.
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   *
   * \internal
   * - Create reception state for current reception with value kSegmentForwardedToAssembler.
   * - If the current session being re-assembled is not the current header session id,
   *   - Reset the state of this assembler to be able to start with another segmentation session.
   *   - Set a new session with id to differentiate multiple calls to the same method.
   * - If the segment is not the last one while the payload size is not multiples of 16,
   *   - Set the reception state to kMessageDropped and log an error.
   * - Else,
   *   - If the assembler state is not set to kError,
   *     - Assemble the segment, get the current state of this assembler and the status of segments.
   *       - If the segments is valid and the message is completely assembled,
   *         - The segments can be forwarded, update reception state to kTpMessageForwarded.
   *       - If the segment status returned as kConfiguredBufferSizeOverflow, kHeaderInfoMismatch or
   *   kIncorrectPayloadLength,
   *         - The segments cannot be forwarded, update reception state to kMessageDropped.
   *   - Else,
   *     - Set the reception state to kMessageDropped since the re-assembly is cancelled.
   * \endinternal
   */
  template <typename BufferType>
  ReceptionState EvaluateTpSegment(SomeIpTpMessageReceptionAssembler& assembler,
                                   someip_protocol::internal::SomeIpMessageHeader const& header,
                                   SomeIpTpHeader const& tp_header,
                                   CopyOperation<BufferType> const payload_copy) const noexcept {
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
    logger_.LogVerbose(
        [&header, &tp_header](ara::log::LogStream& s) {
          s << "Evaluating TP segment for SOME/IP message with ";
          SomeIpTpHeader::LogSomeIpTpMessageHeader(s, header, tp_header);
        },
        __func__, __LINE__);

    ReceptionState reception_state{ReceptionState::kSegmentForwardedToAssembler};

    if (assembler.GetCurrentSession() != header.session_id_) {
      if (assembler.GetAssemblerState() == AssemblerState::kOngoing) {
        // The previous segmentation did not end well, log a warning
        // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
        logger_.LogWarn(
            [&assembler, &header](ara::log::LogStream& s) {
              someip_protocol::internal::SessionId const session_id{assembler.GetCurrentSession()};
              s << "ServiceId: 0x";
              s << ara::log::HexFormat(header.service_id_);
              s << "Current session ID: 0x";
              s << ara::log::HexFormat(session_id);
              s << ", received session ID: 0x";
              s << ara::log::HexFormat(header.session_id_);
              s << ". Previous assembly ended unsuccessfully. Resetting assembler and session.";
            },
            __func__, __LINE__);
      } else {
        // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
        logger_.LogVerbose(
            [&assembler, &header](ara::log::LogStream& s) {
              someip_protocol::internal::SessionId const session_id{assembler.GetCurrentSession()};
              s << "ServiceId: 0x";
              s << ara::log::HexFormat(header.service_id_);
              s << "Current session ID: 0x";
              s << ara::log::HexFormat(session_id);
              s << ", received session ID: 0x";
              s << ara::log::HexFormat(header.session_id_);
              s << ". Previous assembly ended successfully. Starting new assembly.";
            },
            __func__, __LINE__);
      }

      static_cast<void>(assembler.PrepareForNewAssembly());
      assembler.SetCurrentSession(header.session_id_);
    }

    if (assembler.GetAssemblerState() != AssemblerState::kError) {
      std::pair<AssemblerState, SegmentStatus> const ret{assembler.Assemble(header, tp_header, payload_copy)};

      bool const all_segments_received{ret.first == AssemblerState::kDone};
      bool const segment_status_ok{ret.second == SegmentStatus::kOk};

      // If the message is completely assembled, it can be forwarded.
      if (segment_status_ok) {
        if (all_segments_received) {
          reception_state = ReceptionState::kTpMessageForwarded;
        }
      } else if (ret.second == SegmentStatus::kAllocFailure) {
        reception_state = ReceptionState::kMessageDropped;
        // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
        logger_.LogError(
            [&header, &tp_header](ara::log::LogStream& s) {
              s << "No memory available. Dropping message with ";
              SomeIpTpHeader::LogSomeIpTpMessageHeader(s, header, tp_header);
            },
            __func__, __LINE__);
      } else if (ret.second == SegmentStatus::kConfiguredBufferSizeOverflow) {
        reception_state = ReceptionState::kMessageDropped;
        // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
        logger_.LogError(
            [&header, &tp_header](ara::log::LogStream& s) {
              s << "Configured buffer size overflow. Dropping message with ";
              SomeIpTpHeader::LogSomeIpTpMessageHeader(s, header, tp_header);
            },
            __func__, __LINE__);
      } else if (ret.second == SegmentStatus::kHeaderInfoMismatch) {
        reception_state = ReceptionState::kMessageDropped;
        // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
        logger_.LogError(
            [&header, &tp_header](ara::log::LogStream& s) {
              s << "Received inconsistent header information. Dropping message with ";
              SomeIpTpHeader::LogSomeIpTpMessageHeader(s, header, tp_header);
            },
            __func__, __LINE__);
      } else if (ret.second == SegmentStatus::kMissingSegments) {
        reception_state = ReceptionState::kMessageDropped;
        // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
        logger_.LogError(
            [&header, &tp_header](ara::log::LogStream& s) {
              s << "One or more segments are missing. Dropping message with ";
              SomeIpTpHeader::LogSomeIpTpMessageHeader(s, header, tp_header);
            },
            __func__, __LINE__);
      } else {
        // No change in value of reception_state
      }
    } else {
      reception_state = ReceptionState::kMessageDropped;
      logger_.LogError(
          [&assembler, &header, &tp_header](ara::log::LogStream& s) {
            s << "The reassembly has been cancelled for the current session ID: 0x";
            s << ara::log::HexFormat(assembler.GetCurrentSession());
            s << ". Dropping message with ";
            SomeIpTpHeader::LogSomeIpTpMessageHeader(s, header, tp_header);
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return reception_state;
  }

  // VECTOR NC Metric-HIS.LEVEL: MD_SomeIpDaemon_Metric-HIS.LEVEL_his_level
  /*!
   * \brief             This method is invoked on reception of a SOME/IP message from the socket.
   * \param[in]         message The received SOME/IP message which is either directly returned because
   *                    of no SOME/IP-TP usage or will be re-assembled and returned as it could be a TP segment.
   * \return            The state of processing the message along with reassembled message or original message based on
   *                    TP-flag.
   * \pre               -
   * \context           ANY
   * \steady            TRUE
   * \trace             SPEC-4981435
   */
  ReceptionResult OnReception(someip_protocol::internal::SomeIpMessage&& message);

  /*!
   * \brief             Evaluate and forward SOME/IP segment
   * \param[in]         message The received SOME/IP message
   * \param[in]         msg_body_size The SOME/IP message payload size
   * \param[in]         header The SomeIp message header
   * \param[in,out]     result The state of processing the message along with reassembled message or original message
   *                    based on TP-flag.
   * \pre               -
   * \context           ANY
   * \steady            TRUE
   */
  void EvaluateAndForwardTpSegment(someip_protocol::internal::SomeIpMessage const& message,
                                   std::size_t const& msg_body_size,
                                   someip_protocol::internal::SomeIpMessageHeader const& header,
                                   TransportProtocolReceptionController::ReceptionResult& result);

 private:
  /*!
   * \brief         Our logger.
   */
  someip_tp::logging::AraComLogger logger_;
  /*!
   * \brief         Assembler slot collection.
   */
  AssemblerSlots slots_;

  FRIEND_TEST(MUT_SomeIpd__SomeIpTp, TransportProtocolReceptionController_ConstructorWithConfigurations);
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_RECEPTION_CONTROLLER_H_

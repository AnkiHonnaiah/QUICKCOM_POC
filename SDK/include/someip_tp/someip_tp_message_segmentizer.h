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
/**        \file  someip_tp_message_segmentizer.h
 *        \brief  SOME/IP message segmentizer; segment messages to be transmitted.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_MESSAGE_SEGMENTIZER_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_MESSAGE_SEGMENTIZER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <utility>

#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/optional.h"
#include "ara/core/span.h"
#include "logging/ara_com_logger.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/message.h"
#include "someip_tp/someip_tp.h"
#include "someip_tp/someip_tp_segmented_iteration.h"
#include "vac/language/unique_function.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief            SOME/IP Message Segmentizer.
 * \trace            SPEC-10144470
 * \trace            SPEC-10144473
 */
class TransportProtocolMessageSegmentizer final {
 public:
  /*!
   * \brief Typedef for a someip message
   */
  using SomeIpMsg = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief A callback to send multiple datagrams. Returns a boolean indicating success.
   */
  using SendCallback =
      vac::language::UniqueFunction<bool(ara::core::Span<ara::core::Span<osabstraction::io::ConstIOBuffer>> msg)>;
  /*!
   * \brief          Constructor of TransportProtocolMessageSegmentizer.
   * \param[in]      timer_manager A timer manager.
   * \context        Init
   * \pre            -
   * \threadsafe     TRUE
   * \reentrant      FALSE
   * \steady         FALSE
   */
  explicit TransportProtocolMessageSegmentizer(amsr::steady_timer::TimerManagerInterface* timer_manager) noexcept
      : logger_{ara::core::StringView{"TransportProtocolMessageSegmentizer"}},
        timer_{timer_manager->CreateTimer([this]() { TimerExpired(); },
                                          amsr::steady_timer::MissedTimeoutBehavior::kDiscardMissedTimeouts)} {}

  /*!
   * \brief             Indicates whether this message segmentizer is busy or not.
   * \return            Returns true if the message segmnetizer is busy and false otherwise.
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  bool IsBusy() const { return (packet_ != nullptr); }

  /*!
   * \param[in]         connection          A connection to which segments will be forwarded.
   * \param[in]         packet              A SOME/IP message.
   * \param[in]         separation_time     A delay between two consecutive segments.
   * \param[in]         max_segment_length  Maximum segment length. As seen by the SOMEIP header's length field.
   * \param[in]         burst_size          A maximum amount of segments to send between separation times. (0 is
   *                                        interpreted as 1)
   * \param[in]         send                A callback to which segments will be forwarded.
   * \return            if the message wasn't handled due to wrong settings.
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   * \trace             SPEC-10144451
   * \trace             SPEC-10144471
   * \trace             SPEC-10144472
   *
   * \internal
   *  - Assert that the supplied Connection and Packet are not "nullptr".
   *  - Check if the local Packet is nullptr.
   *    - Indicate that the segmentation of the previous message is complete and stop the next segment timer.
   *  - Create a segmentizer object from the supplied Packet.
   *  - Check if the segmentizer has valid settings.
   *    - Check if separation time between segments is zero.
   *      - Send all segments at once.
   *    - Otherwise,
   *      - Set the burst size and separation time.
   *      - Check if separation time is < 1ms (best resolution we have).
   *        - if so, adjust the burst size for sending at a similar data msg rate (it is rounded to the ceiling)
   *          while using an interval of 1ms.
   *      - Send the first burst with the passed burst size.
   *      - Check if there is more data to send:
   *        - if so Start the timer with the separation time as period, then on each timer tick:
   *          - Send the next bust of messages.
   *          - Check if more segments are left to send. If not conclude.
   * - Otherwise,
   *   - Return false.
   *   - Clear the internal state.
   * \endinternal
   */
  bool Segment(SomeIpMsg packet, std::chrono::nanoseconds const separation_time,
               SomeIpTpSegmentSize const max_segment_length, SomeIpTpBurstSize const burst_size, SendCallback send) {
    assert(send);
    assert(packet != nullptr);

    someip_protocol::internal::SomeIpMessageHeader message_header{packet->GetHeader()};

    if (packet_ != nullptr) {
      someip_protocol::internal::SomeIpMessageHeader old_message_header{packet_->GetHeader()};
      // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
      logger_.LogWarn(
          [&message_header, &old_message_header, &packet](ara::log::LogStream& s) {
            std::size_t const total_size{packet->GetTotalSize()};
            s << "Initiating a new segmentation. Received new ";
            someip_protocol::internal::LogSomeIpMessageHeader(s, message_header);
            s << " with total packet size of ";
            s << total_size;
            s << " bytes before the previous one with ";
            someip_protocol::internal::LogSomeIpMessageHeader(s, old_message_header);
            s << " was completed.";
          },
          static_cast<char const*>(__func__), __LINE__);
      Reset(true);
    } else {
      // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
      logger_.LogVerbose(
          [&message_header, &packet](ara::log::LogStream& s) {
            std::size_t const total_size{packet->GetTotalSize()};
            s << "Initiating a new segmentation. Received new ";
            someip_protocol::internal::LogSomeIpMessageHeader(s, message_header);
            s << " with total packet size of ";
            s << total_size;
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    send_ = std::move(send);
    packet_ = packet;
    separation_time_ = separation_time;
    max_segment_length_ = max_segment_length;

    bool const valid{bursts_.Reset(*packet, max_segment_length)};
    if (valid) {
      if (separation_time_ == std::chrono::nanoseconds::zero()) {
        static_cast<void>(SendSegments(std::numeric_limits<std::size_t>::max(), false));
      } else {
        assert(burst_size > 0);
        burst_size_ = std::max(burst_size, SomeIpTpBurstSize{1u});
        StartSendingViaTimer();
      }
    } else {
      Reset(false);
    }
    return valid;
  }

 private:
  /*!
   * \brief Segments receiving order.
   */
  enum class SegmentSendStatus : uint8_t { kFinished, kPending, kSendError };

  /*!
   * \brief             Start sending the first burst of segments. Then set the timer for the next send.
   * \pre               Segment() called to setup member variables.
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  void StartSendingViaTimer() {
    namespace ch = std::chrono;
    constexpr ch::nanoseconds k1ms_in_ns{ch::duration_cast<ch::nanoseconds>(ch::milliseconds{1})};
    constexpr ch::milliseconds k1ms{1};

    SomeIpTpBurstSize const first_batch_size{burst_size_};
    if (separation_time_ < k1ms) {
      // keeping the timer at most at 1ms due to QNX issues (documented in techref)
      double const rate_msgs_msec{(static_cast<double>(burst_size_) * k1ms_in_ns) / ch::nanoseconds(separation_time_)};
      burst_size_ = static_cast<SomeIpTpBurstSize>(std::ceil(rate_msgs_msec));
      separation_time_ = k1ms;
    }
    switch (SendSegments(first_batch_size, false)) {
      case SegmentSendStatus::kPending: {
        timer_->Start(ara::core::Optional<amsr::steady_timer::TimePoint>{},
                      amsr::steady_timer::Duration{osabstraction::time::TimeStamp{separation_time_}});

        break;
      }
      case SegmentSendStatus::kFinished: {
        // All segments sent due to a large enough burst size.
        break;
      }
      default: {
        // SegmentSendStatus::kSendError
        break;
      }
    }
  }
  /*!
   * \brief Send the next batch of segments
   * \param   burst_size        Burst size
   * \param   stop_timers       If the timer have to be stopped on completion or failure
   * \return  SegmentSendStatus
   * \steady  TRUE
   */
  SegmentSendStatus SendSegments(std::size_t const burst_size, bool const stop_timers) {
    assert(burst_size > 0);
    bool const success{send_(bursts_.GetNext(burst_size))};
    SegmentSendStatus ret{SegmentSendStatus::kSendError};
    if (success) {
      if (bursts_.HasEnded()) {
        Reset(stop_timers);
        ret = SegmentSendStatus::kFinished;
      } else {
        ret = SegmentSendStatus::kPending;
      }
    } else {
      OnSendSegmentFailed(stop_timers);
    }
    return ret;
  }
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  // VECTOR NC AutosarC++17_10-A0.1.3: MD_SomeIpDaemon_AutosarC++17_10-A0.1.3_not_called_function
  /*!
   * \brief             Next segment timeout handler.
   * \pre               -
   * \context           Timer
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            FALSE
   */
  void TimerExpired() noexcept {
    SegmentSendStatus const status{SendSegments(burst_size_, true)};

    // Stop the timer if all segments have been transmitted
    if (status != SegmentSendStatus::kPending) {
      timer_->Stop();
    }
  }
  /*!
   * \brief             Resets the state of the segmentizer.
   * \detials           Usually called when the processing of the given unfragmented SOME/IP message is finished.
   * \param[in]         stop_timer Indicates whether to stop the next-segment timer or not.
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  void Reset(bool stop_timer) {
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
    logger_.LogVerbose([&stop_timer](ara::log::LogStream& s) { s << "Stop timer " << (stop_timer ? "true" : "false"); },
                       static_cast<char const*>(__func__), __LINE__);
    if (stop_timer) {
      timer_->Stop();
    }
    send_ = nullptr;
    packet_ = nullptr;
  }
  /*!
   * \brief             Called when sending a SOME/IP message segment has failed.
   * \param[in]         stop_timer Indicates whether to stop the next-segment timer or not.
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  void OnSendSegmentFailed(bool stop_timer) {
    someip_protocol::internal::SomeIpMessageHeader const& message_header{packet_->GetHeader()};
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_SomeIpDaemon_AutosarC++17_10-M5.2.12_array_to_pointer_decay
    logger_.LogError(
        [&message_header](ara::log::LogStream& s) {
          s << "Sending segment failed for SOME/IP packet with ";
          someip_protocol::internal::LogSomeIpMessageHeader(s, message_header);
        },
        static_cast<char const*>(__func__), __LINE__);

    Reset(stop_timer);
  }

  /*!
   * \brief            Our logger.
   */
  someip_tp::logging::AraComLogger logger_;

  /*!
   * \brief            A callback to which segments will be forwarded.
   */
  SendCallback send_{};

  /*!
   * \brief            An unsegmented SOME/IP message which shall be fragmented.
   */
  SomeIpMsg packet_{nullptr};

  /*!
   * \brief            The length of time period which must pass between transmission of 2 consecutive segments.
   * \trace            SPEC-10144472
   */
  std::chrono::nanoseconds separation_time_{};

  /*!
   * \brief            Number of segments to send between separation times.
   */
  SomeIpTpBurstSize burst_size_{1u};

  /*!
   * \brief            Maximum length of a single segment.
   * \trace            SPEC-10144471
   */
  SomeIpTpSegmentSize max_segment_length_{};

  /*!
   * \brief            Burst helper
   */
  SegmentedIteration bursts_{};

  /*!
   * \brief Timer
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> timer_;
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_MESSAGE_SEGMENTIZER_H_

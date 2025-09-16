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
/**        \file  someip_tp_segmentation_manager.h
 *        \brief  segmentation and the transmission of the given SOME/IP message.
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENTATION_MANAGER_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENTATION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <memory>
#include <utility>
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/span.h"
#include "someip_tp_message_segmentizer.h"
#include "vac/container/intrusive_list.h"
#include "vac/container/static_vector.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief             A set of segmentation parameters.
 * \trace SPEC-10144471
 */
struct SegmentationParameters {
  bool segmentize_;                          /*! Indicates whether segmentation shall be done */
  std::chrono::nanoseconds separation_time_; /*! Delay between two consecutive segments */
  SomeIpTpSegmentSize max_segment_length_;   /*! Maximum length of a segment */
  SomeIpTpBurstSize burst_size_;             /*! Number of segments to transmit between separation times*/
};

/*!
 * \brief        SOME/IP TP Segmentation Manager.
 * \trace        SPEC-10144451
 */

class TransportProtocolSegmentationManager final {
 public:
  /*!
   * \brief Typedef for a someip message
   */
  using SomeIpMsg = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;
  /*!
   * \brief A callback to send full someip messages. Returns a boolean indicating success.
   */
  using SendCallback = vac::language::UniqueFunction<bool(SomeIpMsg msg)>;
  /*!
   * \brief              Constructs an instance of TransportProtocolSegmentationManager.
   * \param[in]          config A configuration.
   * \param[in]          timer_manager A timer manager.
   * \param[in]          max_message_segmentizer_count Maximum number of message segmentizers to support.
   * \context            Init
   * \pre                -
   * \threadsafe         TRUE
   * \reentrant          FALSE
   * \steady             FALSE
   * \private
   */
  TransportProtocolSegmentationManager(amsr::steady_timer::TimerManagerInterface* timer_manager,
                                       std::size_t const max_message_segmentizer_count)
      : logger_(someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
                someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
                ara::core::StringView{"TransportProtocolSegmentationManager"}) {
    segmentizers_.reserve(max_message_segmentizer_count);
    // VECTOR NC AutosarC++17_10-A6.5.1: MD_SomeIpDaemon_AutosarC++17_10-A6.5.1_false_positive
    for (std::size_t i{0u}; i < max_message_segmentizer_count; ++i) {
      segmentizers_.emplace_back(timer_manager);
      free_.push_back(segmentizers_.back());
    }
  }

  /*!
   * \brief Result of a process message call
   */
  enum class ProcessMessageResult : std::uint_fast8_t { Ok, NoSegmentizerFound, WrongSegmentationParameters };

  /*!
   * \param[in]         address IP address. For indexing purposes only.
   * \param[in]         port IP port. For indexing purposes only.
   * \brief             Initiates the segmentation and the transmission of the given SOME/IP message.
   * \param[in]         instance_id A SOME/IP instance identifier.
   * \param[in]         packet A SOME/IP message.
   * \param[in]         segment_params Segmentation parameters for the SOME/IP message.
   * \param[in]         send_burst A callback to send bursts of segmented messages.
   * \param[in]         send_burst A callback to send a single message.
   * \param[in]         A send callback over which the segments shall be transmitted.
   * \return            ProcessMessageResult.
   * \pre               -
   * \context           ANY
   * \steady            TRUE
   * \trace             SPEC-10144453
   *
   * \internal
   *  - Create a default false result, indicating the message has been processed unsuccessfully.
   *  - Get the SOME/IP message header of the contained SOME/IP message.
   *  - If the message is allowed to be segmented.
   *    - If the message segmentizer that shall be used for the segmentation of the specified SOME/IP message is found.
   *      - Segment the message and update the result to true.
   *    - Otherwise, log an Error.
   *  - Otherwise, forward the message unsegmented to the transmitter.
   * \endinternal
   */
  ProcessMessageResult ProcessMessage(amsr::net::ip::Address const& address, amsr::net::ip::Port const port,
                                      std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet,
                                      SegmentationParameters const& segment_params,
                                      TransportProtocolMessageSegmentizer::SendCallback send) {
    assert(segment_params.segmentize_);

    ProcessMessageResult result{ProcessMessageResult::NoSegmentizerFound};
    TransportProtocolMessageSegmentizer* const segmentizer{GetMessageSegmentizer(packet->GetHeader(), address, port)};
    if (segmentizer != nullptr) {
      bool const accepted{segmentizer->Segment(packet, segment_params.separation_time_,
                                               segment_params.max_segment_length_, segment_params.burst_size_,
                                               std::move(send))};
      result = accepted ? ProcessMessageResult::Ok : ProcessMessageResult::WrongSegmentationParameters;
    }
    return result;
  }

 private:
  /*!
   * \brief An Id to identify to which transmission a segmentizer belongs
   */
  struct SegmentizerKey {
    /*!
     * \cond
     */
    amsr::net::ip::Address address{};
    amsr::net::ip::Port port{};
    someip_protocol::internal::ServiceId service_id{};
    someip_protocol::internal::MethodId method_id{};
    someip_protocol::internal::ClientId client_id{};
    /*!
     * \endcond
     */
  };

  /*!
   * \brief Pairing a segmentizer with a key
   */
  class Segmentizer : public vac::container::IntrusiveListNode<Segmentizer> {
   public:
    /*!
     * \brief   Segmentizer constructor
     * \param   timer_manager Pointer to a amsr::timer::TimerManager. Used to initialize "value" of the Segmentizer
     *                        object.
     * \steady  FALSE
     */
    explicit Segmentizer(amsr::steady_timer::TimerManagerInterface* timer_manager)
        : vac::container::IntrusiveListNode<Segmentizer>(), key{}, value{timer_manager} {}

    /*!
     * \brief   Segmentizer default destuctor
     * \steady  FALSE
     */
    ~Segmentizer() override = default;

    /*!
     * \brief   Segmentizer default copy constructor
     * \steady  FALSE
     */
    Segmentizer(Segmentizer const&) = default;

    /*!
     * \brief   Segmentizer default copy assignment
     * \steady  TRUE
     */
    Segmentizer& operator=(Segmentizer const&) = default;

    /*!
     * \brief   Segmentizer default move constructor
     * \steady  FALSE
     */
    Segmentizer(Segmentizer&&) = default;

    /*!
     * \brief   Segmentizer default move assignment
     * \steady  FALSE
     */
    Segmentizer& operator=(Segmentizer&&) = default;

    /*!
     * \brief   Segmentizer default move assignment
     * \steady  TRUE
     */
    SegmentizerKey& Key() { return key; }

    /*!
     * \brief   Segmentizer default move assignment
     * \steady  TRUE
     */
    TransportProtocolMessageSegmentizer& Value() { return value; }

   private:
    SegmentizerKey key;
    TransportProtocolMessageSegmentizer value;
    /*!
     * \endcond
     */
  };

  /*!
   * \brief Compare Segmentizer keys for equality.
   * \param lhs
   * \param rhs
   * \return true equal
   * \return false  different
   * \steady TRUE
   */
  static constexpr bool Equals(SegmentizerKey const& lhs, SegmentizerKey const& rhs) {
    // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
    return (rhs.port.port == lhs.port.port) && (lhs.service_id == rhs.service_id) && (lhs.method_id == rhs.method_id) &&
           (lhs.client_id == rhs.client_id) && (rhs.address == lhs.address);
  }

  /*!
   * \brief             Finds a message segmentizer that shall be used for the segmentation of
   *                    the specified SOME/IP message.
   * \param[in]         header A SOME/IP message header.
   * \param[in]         address IP address. For indexing purposes only.
   * \param[in]         port IP port. For indexing purposes only.
   * \return            A pointer to a message segmentizer to be used or a nullptr in case no segmentizer is available.
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   *
   * \internal
   *  - Iterate the list of maybe busy segmentizers.
   *    - If the iterated segmentizer is busy and the key matches return this segmentizer.
   *    - If the iterated segmentizer is not busy move to the free list.
   *  - If there is no segmentizer found try to fetch one from the free list. Swap it to
   *    the list of maybe busy segmentizers.
   * \endinternal
   */
  TransportProtocolMessageSegmentizer* GetMessageSegmentizer(
      someip_protocol::internal::SomeIpMessageHeader const& header, amsr::net::ip::Address const& address,
      amsr::net::ip::Port const port) {
    SegmentizerKey const key{address, port, header.service_id_, header.method_id_, header.client_id_};

    Segmentizer* selected{nullptr};
    decltype(maybe_busy_)::iterator it{maybe_busy_.begin()};
    while (it != maybe_busy_.end()) {
      Segmentizer& seg{*it};
      ++it;
      if (seg.Value().IsBusy()) {
        if (Equals(key, seg.Key())) {
          selected = &seg;
          break;
        }
      } else {
        // not busy anymore
        maybe_busy_.erase(seg);
        free_.push_front(seg);
      }
    }
    if ((selected == nullptr) && (!free_.empty())) {
      Segmentizer& seg{static_cast<Segmentizer&>(*free_.front().GetSelf())};
      selected = &seg;
      free_.erase(seg);
      seg.Key() = key;
      maybe_busy_.push_front(seg);
    }
    return (selected != nullptr) ? &selected->Value() : nullptr;
  }
  /*!
   * \brief         Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief         Backing array with message segmentizers.
   */
  vac::container::StaticVector<Segmentizer> segmentizers_{};

  /*!
   * \brief         List of message segmentizers that might be busy.
   */
  vac::container::IntrusiveList<Segmentizer> maybe_busy_{};

  /*!
   * \brief         List of message segmentizers that are surely free.
   */
  vac::container::IntrusiveList<Segmentizer> free_{};
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENTATION_MANAGER_H_

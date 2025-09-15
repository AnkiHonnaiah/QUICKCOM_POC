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
/**        \file  someip_tp_segment.h
 *        \brief  SOME/IP-TP
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENT_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <utility>

#include "ara/core/span.h"
#include "someip-protocol/internal/deserialization/deser_someip_header.h"
#include "someip-protocol/internal/message.h"
#include "someip_tp_header.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief The SOME/IP-TP segment.
 * \details The segment is comprised of a SomeIpHeader, TP-header, and a view for the payload data.
 */
class SomeIpTpSegment final {
 public:
  /*!
   * \brief Creates a new SomeIpTpSegment.
   * \param[in] someip_header The SomeIpHeader
   * \param[in] segment View for the complete segment, used to extract the TP-header and payload view.
   * \return A new SomeIpTpSegment
   * \error someip_protocol::SomeIpProtocolErrc::buffer_too_small if 'segment' is not enough for a proper SOME/IP-TP
   * segment.
   */
  static ara::core::Result<SomeIpTpSegment> Create(
      amsr::someip_protocol::internal::SomeIpMessageHeader const& someip_header,
      ara::core::Span<std::uint8_t> const segment) noexcept {
    ara::core::Result<SomeIpTpSegment> result{
        someip_protocol::MakeErrorCode(someip_protocol::SomeIpProtocolErrc::buffer_too_small, {},
                                       "The buffer for a SOME/IP-TP segment is not large enough to contain a SOME/IP "
                                       "header, TP-header and minimum segment payload.")};
    constexpr std::size_t kMinSize{someip_protocol::internal::kHeaderSize + someip_tp::kTpHeaderLength};
    SomeIpTpSegment new_segment{someip_header, segment};
    if (segment.size() > kMinSize) {
      result.EmplaceValue(std::move(new_segment));
    }
    return result;
  }

  /*!
   * \brief Get a reference to the SOME/IP header
   */
  amsr::someip_protocol::internal::SomeIpMessageHeader const& SomeIpHeader() const noexcept { return someip_header_; }
  /*!
   * \brief Get a reference to the SOME/IP-TP header
   */
  SomeIpTpHeader const& TpHeader() const noexcept { return tp_header_; }
  /*!
   * \brief Get a view to payload data
   */
  ara::core::Span<std::uint8_t> const Payload() const noexcept { return payload_; }

 private:
  /*!
   * \brief Constructor.
   * \param[in] someip_header The SomeIpHeader
   * \param[in] segment View for the complete segment, used to extract the TP-header and payload view.
   */
  SomeIpTpSegment(amsr::someip_protocol::internal::SomeIpMessageHeader const& someip_header,
                  ara::core::Span<std::uint8_t> const segment) noexcept
      : someip_header_{someip_header},
        tp_header_{segment.subspan(sizeof(amsr::someip_protocol::internal::SomeIpMessageHeader), kTpHeaderLength)},
        payload_{segment.subspan(sizeof(amsr::someip_protocol::internal::SomeIpMessageHeader) + kTpHeaderLength)} {}

  /*!
   * \brief SOME/IP header
   */
  amsr::someip_protocol::internal::SomeIpMessageHeader const someip_header_;
  /*!
   * \brief SOME/IP-TP header
   */
  SomeIpTpHeader const tp_header_;
  /*!
   * \brief Payload view
   */
  ara::core::Span<std::uint8_t> const payload_;
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_SEGMENT_H_

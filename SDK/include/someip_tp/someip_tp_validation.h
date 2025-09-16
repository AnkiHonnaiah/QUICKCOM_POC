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
/*!        \file  someip_tp_validation.h
 *        \brief
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_VALIDATION_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_VALIDATION_H_

#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_tp/someip_tp_types.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief       Check if the message size is valid.
 * \param       payload_size The size of the message, headers excluded.
 * \return      true if the SOME/IP message has the correct size, false if the SOME/IP message
 *              can not be segmented.
 * \pre         -
 * \context     Reactor
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \steady      TRUE
 */
static constexpr bool ValidatePayloadSize(std::size_t payload_size) noexcept {
  // VECTOR NL AutosarC++17_10-M0.1.2: MD_SomeIpDaemon_AutosarC++17_10-M0.1.2_dead_branch_false_positive
  return (payload_size >= std::size_t{kMinTpMessagePayload}) && (payload_size <= std::size_t{kMaxTpMessagePayload});
}

/*!
 * \brief       Check if the TP segmentation settings are valid and the original message is valid.
 * \note        This must be called before any segmentation happens!
 *              Otherwise the segmented payload length may not be valid! This can occur if
 *              the maximum_segment_length given is smaller than a specified value.
 * \param       payload_size The size of the message, all headers excluded.
 * \param       maximum_segment_length The maximum length of a segment (as seen by the SOMEIP length field).
 * \return      true if all conditions for a successful SOME/IP-TP segmentation are met; false
 *              if any of the constraints do differ (e.g. giving a maximum TP length of zero).
 * \pre         -
 * \context     Reactor
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \steady      TRUE
 * \trace       SPEC-4981424
 *
 * \internal
 * - Check if the unsegmented message is valid.
 * - Create a bool, valid, to hold the result in this function. Initialize it to false.
 * - If the unsegmented message is valid
 *   - Verify that that maximum_segment_length_ is greater or equal than the Minimum SOME/IP message length.
 *   - Verify that maximum_segment_payload_length_ is smaller or equal than the Maximum SOME/IP message length.
 *   - Verify that effective maximum_segment_payload_length_ is smaller than the size of someip_payload_.
 *   - If the all three verifications are valid
 *     - set valid to true.
 * - Return valid.
 * \endinternal
 */
static constexpr bool ValidateParameters(std::size_t const payload_size,
                                         someip_protocol::internal::LengthField const maximum_segment_length) noexcept {
  bool valid{ValidatePayloadSize(payload_size)};
  // This must first be checked, otherwise there is a nullptr-dereference possible.
  if (valid) {
    // It shall be verified that the segment length is greater than both the SOME/IP header and
    // the TP header.
    // Notice that effective (no headers included) segment lengths under 16bytes can't be represented
    // by the Tp header's offset value, as the offsets (and by extension the effective (no headers) segment lengths) are
    // rounded to the floor to 16 byte multiples.
    bool const min_segment_ok{maximum_segment_length >= kMinTpSegmentLength};
    // It shall be verified that the segment length is smaller than the MTU of UDP, as SOME/IP-TP
    // is exclusively used for SOME/IP over UDP.
    bool const max_segment_ok{maximum_segment_length <= kMaxTpSegmentLength};
    // check that the payload is bigger than the effective segment.
    decltype(maximum_segment_length) const effective_max_segment_length{
        (min_segment_ok) ? static_cast<decltype(maximum_segment_length)>(maximum_segment_length -
                                                                         kSomeipTpHeaderBytesComputedInLengthField)
                         : std::numeric_limits<decltype(maximum_segment_length)>::max()};
    bool const requires_segmentation{std::size_t{effective_max_segment_length} < payload_size};
    valid = min_segment_ok && max_segment_ok && requires_segmentation;
  }
  return valid;
}

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_VALIDATION_H_

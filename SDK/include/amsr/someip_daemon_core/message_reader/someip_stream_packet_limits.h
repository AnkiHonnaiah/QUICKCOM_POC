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
/**        \file  someip_stream_packet_limits.h
 *        \brief  Definition of limits for SOME-IP stream reception
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_SOMEIP_STREAM_PACKET_LIMITS_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_SOMEIP_STREAM_PACKET_LIMITS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace message_reader {

/*!
 * \brief   Upper size limit of header length field.
 * \details This constant represents the maximum allowed size of the length field
 *          within the SOME/IP header to avoid a potential overflow in the total message size computation.
 *          This value MUST NEVER BE MODIFIED.
 */
static std::uint32_t constexpr kMaximumHeaderLengthField{
    std::numeric_limits<std::uint32_t>::max() - static_cast<std::uint32_t>(someip_protocol::internal::kHeaderLength)};

/*!
 * \brief   Maximum allowed size (in bytes) for the payload of a TCP packet.
 * \details This constant represents the maximum allowed size for a TCP packet payload.
 *          Modifying this value will limit the maximum amount of memory that can be allocated by the reception
 *          of a SOME-IP message.
 *          This value must not be larger than kMaximumHeaderLengthField.
 */
static std::uint32_t constexpr kMaximumAllowedPayloadLength{kMaximumHeaderLengthField};

static_assert(
    kMaximumAllowedPayloadLength <= kMaximumHeaderLengthField,
    "The maximum allowed TCP body size can't be larger than the maximum size allowed by the SOME/IP protocol");

}  // namespace message_reader
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_SOMEIP_STREAM_PACKET_LIMITS_H_

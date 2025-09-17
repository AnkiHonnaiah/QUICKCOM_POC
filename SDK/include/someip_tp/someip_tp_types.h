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
/*!        \file  someip_tp_types.h
 *        \brief  SOME/IP-TP
 *
 *      \details  Transporting large SOME/IP messages via UDP/IP
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_TYPES_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "amsr/unique_ptr.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_tp {

// VECTOR Disable AutosarC++17_10-M3.4.1:MD_SomeIpDaemon_AutosarC++17_10_M3.4.1_global
/*!
 * \brief SOME/IP TP segment size in bytes.
 */
using SomeIpTpSegmentSize = std::uint32_t;

/*!
 * \brief SOME/IP TP message size in bytes.
 */
using SomeIpTpMessageSize = std::uint32_t;

/*!
 * \brief Number of message buffers used to handle on SOME/IP message type.
 */
using SomeIpTpNumMessageBuffers = std::uint32_t;

/*!
 * \brief Consists of the offset in bytes (multiple of 16) and a more flag (1 bit)
 * that signalizes additional TP segments. The TP header field is four bytes wide.
 */
using SomeIpTpHeaderField = std::uint32_t;

/*!
 * \brief Type for offset of the SOME/IP-TP header.
 */
using SomeIpTpOffsetField = std::uint32_t;

/*!
 * \brief Number of pre-allocated rx buffers
 */
using SomeIpTpRxBufferCount = std::uint32_t;

/*!
 * \brief Number of segments to transmit between separation times
 */
using SomeIpTpBurstSize = std::uint32_t;

using MessageType = someip_protocol::internal::SomeIpMessageType;

/*!
 * \brief Length of the SOME/IP-TP header in bytes.
 */
static constexpr someip_protocol::internal::LengthField kTpHeaderLength{4U};

/*!
 * \brief SOME/IP-TP more flag for bit-wise operations.
 */
static constexpr SomeIpTpHeaderField kTpMoreFlagBitMask{0x00000001U};
/*!
 * \brief Active SOME/IP-TP offset bits for bit-wise operations.
 * \trace SPEC-10144463
 * \trace SPEC-10144465
 */
static constexpr SomeIpTpHeaderField kTpOffsetBitsMask{0xFFFFFFF0U};

/*!
 * \brief As the offset bytes are masked, every offset is multiple of 16.
 */
static constexpr SomeIpTpHeaderField kTpOffsetMultiplicty{~kTpOffsetBitsMask + 1U};

/*!
 * \brief The SOMEIP Header's length field value can't be zero. It always includes this overhead.
 */
static constexpr someip_protocol::internal::LengthField kSomeipHeaderBytesComputedInLengthField{
    someip_protocol::internal::LengthField{
        sizeof(someip_protocol::internal::SessionId) + sizeof(someip_protocol::internal::ClientId) +
        sizeof(someip_protocol::internal::ProtocolVersion) + sizeof(someip_protocol::internal::InterfaceVersion) +
        sizeof(someip_protocol::internal::SomeIpMessageType) + sizeof(someip_protocol::internal::SomeIpReturnCode)}};

/*!
 * \brief As kSomeipHeaderBytesComputedInLengthField, but including the Tp header
 */
static constexpr someip_protocol::internal::LengthField kSomeipTpHeaderBytesComputedInLengthField{
    kSomeipHeaderBytesComputedInLengthField + kTpHeaderLength};

/*!
 * \brief Maximum Payload that a segment can transport. Aligned to 16 bytes.
 * \trace SPEC-4981424
 */
static constexpr someip_protocol::internal::LengthField kMaxTpSegmentPayload{1392U};

static_assert((kMaxTpSegmentPayload % kTpOffsetMultiplicty) == 0, "This is just documentation");

/*!
 * \brief Maximum SOME/IP message segment length (UDP is implied). As seen by the SOMEIP header's length field.
 * \trace SPEC-4981424
 */
static constexpr someip_protocol::internal::LengthField kMaxTpSegmentLength{kSomeipTpHeaderBytesComputedInLengthField +
                                                                            kMaxTpSegmentPayload};

/*!
 * \brief Minimum SOME/IP message segment length. As the offsets are multiples of 16bytes, effective (with no headers
 * accounted) segments have to be at mimimum 16 bytes. As seen by the SOMEIP header's length field.
 */
static constexpr someip_protocol::internal::LengthField kMinTpSegmentLength{kSomeipTpHeaderBytesComputedInLengthField +
                                                                            kTpOffsetMultiplicty};

/*!
 * \brief kMinTpSegmentLength excluding all headers (SOMEIP + SOMEIP/TP). Payload is understood here as effective data
 * stripped of all protocol-related information.
 */
static constexpr someip_protocol::internal::LengthField kMinTpSegmentPayload{kMinTpSegmentLength -
                                                                             kSomeipTpHeaderBytesComputedInLengthField};

/*!
 * \brief Minimum SOME/IP message payload length which can be transported via SOME/IP TP protocol and results in two
 * segments. Payload is understood here as effective data stripped of all protocol-related information.
 */
static constexpr someip_protocol::internal::LengthField kMinTpMessagePayload{kMinTpSegmentPayload + 1u};

/*!
 * \brief Maximum SOME/IP message payload length which can be transported via SOME/IP TP protocol. Payload is
 * understood as effective data stripped of all protocol-related information.
 */
static constexpr someip_protocol::internal::LengthField kMaxTpMessagePayload{
    std::numeric_limits<someip_protocol::internal::MTU>::max()};
// VECTOR Enable AutosarC++17_10-M3.4.1

/*!
 * \brief       Checks whether the given message type belongs to one of the SOME/IP-TP message types.
 * \param[in]   message_type A SOME/IP message type.
 * \return      true if the given message type belongs to one of the SOME/IP-TP message types and false otherwise.
 * \pre         -
 * \context     Reactor|App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 */
inline bool IsSomeIpTpMessage(MessageType message_type) {
  return (message_type == MessageType::kTpRequest) || (message_type == MessageType::kTpRequestNoReturn) ||
         (message_type == MessageType::kTpNotification) || (message_type == MessageType::kTpResponse) ||
         (message_type == MessageType::kTpError);
}

/*!
 * \brief       Converts the given SOME/IP message type to the corresponding SOME/IP-TP message type.
 * \param[in]   message_type A SOME/IP message type.
 * \return      the corresponding SOME/IP-TP message type.
 * \pre         -
 * \context     Reactor|App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 */
inline MessageType ToSomeIpTpMessageType(MessageType message_type) {
  MessageType result{};
  switch (message_type) {
    case MessageType::kRequest: {
      result = MessageType::kTpRequest;
      break;
    }
    case MessageType::kRequestNoReturn: {
      result = MessageType::kTpRequestNoReturn;
      break;
    }
    case MessageType::kResponse: {
      result = MessageType::kTpResponse;
      break;
    }
    case MessageType::kNotification: {
      result = MessageType::kTpNotification;
      break;
    }
    case MessageType::kError: {
      result = MessageType::kTpError;
      break;
    }
    default: {
      result = message_type;
      break;
    }
  }
  return result;
}

/*!
 * \brief       Converts the given SOME/IP-TP message type to the corresponding SOME/IP message type.
 * \param[in]   message_type A SOME/IP-TP message type.
 * \return      the corresponding SOME/IP message type for this TP message type.
 * \pre         -
 * \context     Reactor|App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      TRUE
 */
inline MessageType FromSomeIpTpMessageType(MessageType message_type) {
  MessageType result{};
  switch (message_type) {
    case MessageType::kTpRequest: {
      result = MessageType::kRequest;
      break;
    }
    case MessageType::kTpRequestNoReturn: {
      result = MessageType::kRequestNoReturn;
      break;
    }
    case MessageType::kTpResponse: {
      result = MessageType::kResponse;
      break;
    }
    case MessageType::kTpNotification: {
      result = MessageType::kNotification;
      break;
    }
    case MessageType::kTpError: {
      result = MessageType::kError;
      break;
    }
    default: {
      result = message_type;
      break;
    }
  }
  return result;
}

/*!
 * \brief   Convert from segment_length, which includes some header data, to an
 *          effective segment length, which does not include header data.
 * \details MaximumSegmentLength on the spec
 * \param   segment_length Segment length as specified on the model: with header overheads.
 * \return  constexpr SomeIpTpSegmentSize segmented length that can be used for segmentation
 * \steady  TRUE
 */
inline constexpr SomeIpTpSegmentSize ToEffectiveSegmentLength(SomeIpTpSegmentSize segment_length) {
  assert((segment_length <= kMaxTpSegmentLength) && (segment_length >= kMinTpSegmentLength));
  return (segment_length - SomeIpTpSegmentSize{kSomeipTpHeaderBytesComputedInLengthField}) & kTpOffsetBitsMask;
}

/*!
 * \brief Helper function which aborts on a failed allocation for a new unique ptr.
 * \tparam T Type to instantiate.
 * \tparam Args Argument list for construction.
 * \param[in] args Arguments for construction.
 * \return amsr::UniquePtr<T>
 */
template <typename T, typename... Args>
auto MakeUniqueOrAbort(Args&&... args) noexcept -> amsr::UniquePtr<T> {
  ara::core::Result<amsr::UniquePtr<T>> result{amsr::AllocateUnique<T>({}, std::forward<Args>(args)...)};
  if (!result.HasValue()) {
    ara::core::Abort(result.Error().Message().data());
  }
  return std::move(result.Value());
}

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_TYPES_H_

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
/**        \file  someip_tp_header.h
 *        \brief  SOME/IP-TP
 *
 *      \details  Transporting large SOME/IP messages via UDP/IP
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_HEADER_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_HEADER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <utility>

#include "ara/core/span.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someip_tp_offset.h"
#include "someip_tp_types.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief Buffer view to wrap the data to inspect.
 */
// VECTOR NC AutosarC++17_10-M3.4.1:MD_SomeIpDaemon_AutosarC++17_10_M3.4.1_global_declaration_in_header
using BufferView = ara::core::Span<std::uint8_t>;

/*!
 * \brief Buffer view to wrap the data to inspect.
 */
// VECTOR NC AutosarC++17_10-M3.4.1:MD_SomeIpDaemon_AutosarC++17_10_M3.4.1_global_declaration_in_header
using ImmutableBufferView = ara::core::Span<std::uint8_t const>;

/*!
 * \brief The SOME/IP-TP header consists of an offset of the original,
 * fragmented SOME/IP message in bytes and a "more flag" (additional segments).
 * \trace SPEC-4981415
 * \trace SPEC-4981416
 * \trace SPEC-10144453
 * \trace SPEC-10144454
 * \trace SPEC-10144465
 */
class SomeIpTpHeader final {
 public:
  /*!
   * \brief   Set the SOME/IP-TP header information with this constructor.
   * \param   byte_offset Offset in bytes of this segment from the original SOME/IP message.
   * \param   more Signalize if there comes an additional segment after this one.
   * \steady  TRUE
   */
  SomeIpTpHeader(SomeIpTpOffset byte_offset, bool more) noexcept : offset_{byte_offset}, more_{more} {}

  /*!
   * \brief       Construct a SomeIpTpHeader from a given bytestream.
   * \param       serialized_header SOME/IP-TP header in network-byte-order.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace SPEC-10144462, SPEC-10144463, SPEC-10144465, SPEC-10144466, SPEC-10144468
   *
   * \internal
   * - Create a SomeIpTpHeaderField to hold the deserialized SomeIptTpHeader.
   * - If the size of serialized_header is equal to the length of the SOME/IP-TP header.
   *   - create a Reader with the serialized_header.
   *   - read the header from the bufferView into the created SomeIpTpHeaderField.
   * - Set offset_ from the offset bits in the header.
   * - set more_ from the more flag bit in the header.
   * \endinternal
   */
  explicit SomeIpTpHeader(ImmutableBufferView serialized_header) : SomeIpTpHeader(SomeIpTpOffset{0U}, false) {
    SomeIpTpHeaderField tp_header{};

    if (serialized_header.size() == kTpHeaderLength) {
      someip_protocol::internal::deserialization::Reader reader{serialized_header};
      reader.ReadPrimitive<SomeIpTpHeaderField, someip_protocol::internal::BigEndian>(tp_header);
    }

    offset_ = SomeIpTpOffset{tp_header & kTpOffsetBitsMask};
    more_ = static_cast<bool>(tp_header & kTpMoreFlagBitMask);
  }

  /*!
   * \brief   Getter for the offset of this SOME/IP-TP header.
   * \steady  TRUE
   */
  SomeIpTpOffset GetOffset() const noexcept { return offset_; }

  /*!
   * \brief   Getter for the more flag of this SOME/IP-TP header.
   * \steady  TRUE
   * \trace   SPEC-10144467
   */
  bool HasMoreSegments() const noexcept { return more_; }

  /*!
   * \brief   Get the offset and the more flag at once.
   * \return  The logical OR-ed SOME/IP-TP header in host-byte-order of offset value and more flag.
   * \remark  Must be converted into big-endian.
   * \steady  TRUE
   */
  SomeIpTpHeaderField GetTpHeader() const noexcept {
    SomeIpTpHeaderField const more{static_cast<SomeIpTpHeaderField>(more_) & kTpMoreFlagBitMask};
    return offset_.GetOffsetValue() | static_cast<SomeIpTpHeaderField>(more);
  }
  /*!
   * \brief   Support for logging a SomeIpTp header.
   * \param   [in, out] s The log stream written to.
   * \param   someIpMessageHeader  A SOME/IP message header.
   * \param   tp_header SOME/IP-TP header.
   * \steady  TRUE
   */
  static inline void LogSomeIpTpMessageHeader(
      ara::log::LogStream& s, someip_protocol::internal::SomeIpMessageHeader const& someip_message_header,
      SomeIpTpHeader const& someip_tp_header) noexcept {
    s << "SOME/IP-TP header [Offset: 0x";
    s << ara::log::HexFormat(someip_tp_header.GetOffset().GetOffsetValue());
    s << ", More-flag: ";
    s << ara::log::HexFormat(someip_tp_header.more_);
    s << "]. ";
    LogSomeIpMessageHeader(s, someip_message_header);
  }

 private:
  /*!
   * \brief Offset in bytes in the original non-segmented SOME/IP message
   */
  SomeIpTpOffset offset_;

  /*!
   * \brief More flag signalizes if there comes an additional segment of the
   * original non-fragmented SOME/IP message.
   */
  bool more_;
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_HEADER_H_

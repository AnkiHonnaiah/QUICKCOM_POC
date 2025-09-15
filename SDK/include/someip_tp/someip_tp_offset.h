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
/**        \file  someip_tp_offset.h
 *        \brief  SOME/IP-TP
 *
 *      \details  Transporting large SOME/IP messages via UDP/IP
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_OFFSET_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_OFFSET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "someip_tp_types.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief Offset of a TP segment in bytes modeled as class.
 * \trace SPEC-10144462
 * \trace SPEC-10144463
 */
class SomeIpTpOffset final {
 public:
  /*!
   * \brief   Pass in the offset in bytes.
   * \param   byte_offset The offset in bytes (not shifted).
   * \steady  TRUE
   */
  explicit SomeIpTpOffset(SomeIpTpOffsetField byte_offset) noexcept : offset_value_{byte_offset} {}

  /*!
   * \brief   Change the offset value by copy assignment.
   * \param   other The offset to assign to the current object.
   * \return  A reference to the object with copied attributes.
   * \steady  FALSE
   */
  SomeIpTpOffset& operator=(SomeIpTpOffset const& other) & = default;
  /*!
   * \brief   Set default move assignment.
   * \steady  FALSE
   */
  SomeIpTpOffset& operator=(SomeIpTpOffset&&) & noexcept = default;
  /*!
   * \brief   Set default copy constructor.
   * \steady  FALSE
   */
  SomeIpTpOffset(SomeIpTpOffset const&) = default;
  /*!
   * \brief   Set default move constructor.
   * \steady  FALSE
   */
  SomeIpTpOffset(SomeIpTpOffset&&) noexcept = default;
  /*!
   * \brief   Set default destructor.
   * \steady  TRUE
   */
  ~SomeIpTpOffset() = default;

  /*!
   * \brief       Check if the offset value in bytes is valid.
   * \details     It can not take the full 32 bits as the lower 4 bits of the SOME/IP-TP header are reserved
   *              or used by the more flag. Also check if the offset is a multiple of 16 bytes.
   * \return      true if the offset is in the value range, false if it exceeds the range of 0xFFFFFFF0U.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  bool IsValid() const noexcept { return (offset_value_ <= MaxOffsetValue()) && ((offset_value_ % 16U) == 0U); }

  /*!
   * \brief   Get the offset in bytes prepared for the SOME/IP-TP header.
   * \steady  TRUE
   */
  SomeIpTpOffsetField GetOffsetValue() const noexcept { return offset_value_; }

 private:
  /*!
   * \brief   Get the numeric limit the byte offset can reach as a value, because
   *          TP offset is only 28 bits.
   * \trace   SPEC-4981418
   * \steady  FALSE
   */
  static constexpr SomeIpTpOffsetField MaxOffsetValue() noexcept { return kOffsetLimit; }

  /*!
   * \brief Maximum offset fitting in 28 bits.
   * \trace SPEC-4981418
   */
  static constexpr SomeIpTpOffsetField kOffsetLimit{0xFFFFFFF0U};

  /*!
   * \brief Actual offset value in bytes (not shifted).
   */
  SomeIpTpOffsetField offset_value_;
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_OFFSET_H_

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
/*!        \file
 *         \brief  Class describing the content of a slot enriched by metadata.
 *
 *       \details
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_SLOT_CONTENT_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_SLOT_CONTENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include <type_traits>

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

struct MetaData {
  using TimeStamp = std::chrono::steady_clock::time_point;
  using SequenceCounter = std::uint64_t;

  TimeStamp time_stamp{};
  SequenceCounter sequence_counter{};
};

struct SlotContentHeader {
  MetaData meta_data{};
  std::uintptr_t payload_offset{0U};
  std::size_t payload_size{0U};
};

template <typename SampleType>
struct SlotContent {
  static_assert(std::is_trivially_copyable<SlotContentHeader>::value,
                "The slot content header must be trivially copyable.");
  static_assert(std::is_trivially_copyable<SampleType>::value, "The provided SampleType must be trivially copyable.");

  SlotContentHeader header_data{};
  SampleType sample{};
};

/*!
 * \brief         A helper function to correctly set the payload_offset and payload_size fields.
 * \param[in,out] slot_content  A reference to the slot content whose fields shall be modified.
 * \context       ANY
 * \internal steady TRUE \endinternal
 * \threadsafe    FALSE
 * \reentrant     TRUE
 * \synchronous   TRUE
 *
 * \internal
 * - Compute and set the payload offset.
 * - Set the payload size.
 * \endinternal
 */
template <typename SampleType>
void SetPayloadInfo(SlotContent<SampleType>& slot_content) noexcept {
  // VECTOR NC AutosarC++17_10-A5.2.4: MD_ZEROCOPYBINDING_A5.2.4_reinterpretCastVoidPointerUintPtr
  // VECTOR NC AutosarC++17_10-A5.2.4: MD_ZEROCOPYBINDING_A5.2.4_reinterpretCastVoidPointerUintPtr
  // VECTOR NC AutosarC++17_10-M5.2.9: MD_ZEROCOPYBINDING_M5.2.9_reinterpretCastVoidPointerUintPtr
  // VECTOR NC AutosarC++17_10-M5.2.9: MD_ZEROCOPYBINDING_M5.2.9_reinterpretCastVoidPointerUintPtr
  slot_content.header_data.payload_offset =
      reinterpret_cast<std::uintptr_t>(&slot_content.sample) - reinterpret_cast<std::uintptr_t>(&slot_content);
  slot_content.header_data.payload_size = sizeof(SampleType);
}

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_SLOT_CONTENT_H_

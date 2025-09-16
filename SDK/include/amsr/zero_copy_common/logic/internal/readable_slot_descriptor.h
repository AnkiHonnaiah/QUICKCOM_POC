/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file
 *        \brief  Definition of interface for ReadableSlotDescriptors.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::SlotManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_READABLE_SLOT_DESCRIPTOR_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_READABLE_SLOT_DESCRIPTOR_H_

#include <cstdint>
#include <type_traits>

#include "amsr/core/span.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Interface of a ReadableSlotDescriptor.
 * \details A ReadableSlotDescriptor offers read-only access to one message slot.
 *          This includes message data, the global visibility flag and debug data.
 *          The message data is untyped i.e., a span of bytes.
 *
 * \trace DSGN-ZeroCopyCommon-MessageSlotAbstraction,
 *        DSGN-ZeroCopyCommon-MessageSlotAccess
 * \vprivate Component Private
 */
class ReadableSlotDescriptor {
  // VECTOR NL AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_basicNumericalTypeStaticAssert
  static_assert(std::is_same<std::uint8_t, unsigned char>::value,
                "ReadableSlotDescriptor requires std::uint8_t to be implemented as unsigned char.");

 public:
  /*!
   * \brief Destroy the ReadableSlotDescriptor.
   * \vprivate Component Private
   */
  virtual ~ReadableSlotDescriptor() noexcept = default;

  /*!
   * \brief Get read-only access to the message data referred to by this ReadableSlotDescriptor.
   * \details The returned view and therefore the data access is valid as long as this object instance is valid.
   *          A ReadableSlotDescriptor never changes what data it refers to, i.e. the returned view of this method
   *          refers to the same data throughout the lifetime of a ReadableSlotDescriptor.
   * \return A read-only view of the message data.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE for this object instance and any other SlotDescriptors referencing the same message slot.
   * \reentrant FALSE for this object instance.
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto GetReadableData() const noexcept -> ::amsr::core::Span<std::uint8_t const> = 0;

  /*!
   * \brief Get the current visibility state of the message slot referred to by this ReadableSlotDescriptor.
   * \details This read-operation must synchronize with WritableSlotDescriptor::SetVisibilityFlag() operations
   *          referring to the same message slot (in either this or another process).
   *          In detail, if some actor A sets the slot to be visible and some actor B observes the slot as visible
   *          using this method, actor B must see all writes done by actor A before it set the slot to be visible.
   *          This read-operation to shared memory must be non-blocking.
   * \return True, if the message slot is visible. False otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE for this object instance and any other SlotDescriptors referencing the same message slot.
   * \reentrant FALSE for this object instance.
   * \synchronous TRUE
   *
   * \trace DSGN-ZeroCopyCommon-GlobalVisibilityFlagMemoryOrder
   * \vprivate Component Private
   */
  virtual auto IsSlotVisible() const noexcept -> bool = 0;

 protected:
  /*!
   * \brief Construct new ReadableSlotDescriptor.
   * \vprivate Component Private
   */
  explicit ReadableSlotDescriptor() noexcept = default;

  /*!
   * \brief Create a copy of a ReadableSlotDescriptor.
   * \vprivate Component Private
   */
  ReadableSlotDescriptor(ReadableSlotDescriptor const&) noexcept = default;

  /*!
   * \brief Move a ReadableSlotDescriptor into a new instance.
   * \vprivate Component Private
   */
  ReadableSlotDescriptor(ReadableSlotDescriptor&&) noexcept = default;

  /*!
   * \brief Copy a different ReadableSlotDescriptor into this instance.
   * \vprivate Component Private
   */
  auto operator=(ReadableSlotDescriptor const&) & noexcept -> ReadableSlotDescriptor& = default;

  /*!
   * \brief Move a different ReadableSlotDescriptor into this instance.
   * \vprivate Component Private
   */
  auto operator=(ReadableSlotDescriptor&&) & noexcept -> ReadableSlotDescriptor& = default;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_READABLE_SLOT_DESCRIPTOR_H_

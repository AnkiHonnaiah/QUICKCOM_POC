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
 *        \brief  Definition of interface for WritableSlotDescriptors.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::SlotManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_WRITABLE_SLOT_DESCRIPTOR_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_WRITABLE_SLOT_DESCRIPTOR_H_

#include <cstdint>
#include <type_traits>

#include "amsr/core/span.h"

#include "amsr/zero_copy_common/logic/internal/readable_slot_descriptor.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Interface of a WritableSlotDescriptor.
 * \details A WritableSlotDescriptor offers read and write access to one message slot.
 *          This includes message data, the global visibility flag and debug data.
 *          The message data is untyped i.e., a span of bytes.
 *
 * \trace DSGN-ZeroCopyCommon-MessageSlotAbstraction,
 *        DSGN-ZeroCopyCommon-MessageSlotAccess
 * \vprivate Component Private
 */
class WritableSlotDescriptor : public ReadableSlotDescriptor {
  // VECTOR NL AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_basicNumericalTypeStaticAssert
  static_assert(std::is_same<std::uint8_t, unsigned char>::value,
                "WritableSlotDescriptor requires std::uint8_t to be implemented as unsigned char.");

 public:
  /*!
   * \brief Destroy the WritableSlotDescriptor.
   * \vprivate Component Private
   */
  // VECTOR NC AutosarC++17_10-A10.3.2:MD_ZEROCOPYCOMMON_A10.3.2_override_keyword_needed_false_positive
  virtual ~WritableSlotDescriptor() noexcept = default;

  /*!
   * \brief Reset the memory for the referenced slot into a sane default state.
   * \details This should reset (if required for the implementation) debug data into a sane
   *          default state. It should also clear the global visibility flag of this slot.
   *          This will be called before the slot is used by a server instance.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void ResetSlotMemory() noexcept = 0;

  /*!
   * \brief Get read and write access to the message data referred to by this WritableSlotDescriptor.
   * \details The returned view and therefore the data access is valid as long as this object instance is valid.
   *          A WritableSlotDescriptor never changes what data it refers to, i.e. the returned view of this method
   *          refers to the same data throughout the lifetime of a slot WritableSlotDescriptor.
   * \return A read-writable view of the message data.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto GetWritableData() noexcept -> ::amsr::core::Span<std::uint8_t> = 0;

  /*!
   * \brief Make the message slot referred to by this WritableSlotDescriptor visible.
   * \details This write operation must synchronize with ReadableSlotDescriptor::IsSlotVisible() operations
   *          referring to the same message slot (in either this or another process).
   *          In detail, if some actor A sets the slot to be visible using this method and some actor B observes the
   *          slot as visible, actor B must see all writes done by actor A before it set the slot to be visible.
   *          This write operation to shared memory must be non-blocking and appear atomic.
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
  virtual void SetVisibilityFlag() noexcept = 0;

  /*!
   * \brief Make the message slot referred to by this WritableSlotDescriptor not-visible.
   * \details This write operation to shared memory must be non-blocking and appear atomic.
   * \context ANY
   * \pre -
   *
   * \threadsafe TRUE for this object instance and any other SlotDescriptors referencing the same message slot.
   * \reentrant FALSE for this object instance.
   * \synchronous TRUE
   *
   * \vprivate Component Private
   */
  virtual void ClearVisibilityFlag() noexcept = 0;

  /*!
   * \brief Update debug data for the "Send Message" debug point.
   * \details This method is invoked directly before the server sends the slot to any connected clients.
   *          This write operation to shared memory must be non-blocking.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vprivate Component Private
   */
  virtual void DebugPointSendSlot() noexcept = 0;

  /*!
   * \brief Update debug data for the "get slot" debug point.
   * \details This method is invoked right before the server returns a slot to the user.
   *          This write operation to shared memory must be non-blocking.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vprivate Component Private
   */
  virtual void DebugPointGetSlot() noexcept = 0;

 protected:
  /*!
   * \brief Construct new WritableSlotDescriptor.
   * \vprivate Component Private
   */
  explicit WritableSlotDescriptor() noexcept = default;

  /*!
   * \brief Create a copy of a WritableSlotDescriptor.
   * \vprivate Component Private
   */
  WritableSlotDescriptor(WritableSlotDescriptor const&) noexcept = default;

  /*!
   * \brief Move a WritableSlotDescriptor into a new instance.
   * \vprivate Component Private
   */
  WritableSlotDescriptor(WritableSlotDescriptor&&) noexcept = default;

  /*!
   * \brief Copy a different WritableSlotDescriptor into this instance.
   * \vprivate Component Private
   */
  auto operator=(WritableSlotDescriptor const&) & noexcept -> WritableSlotDescriptor& = default;

  /*!
   * \brief Move a different WritableSlotDescriptor into this instance.
   * \vprivate Component Private
   */
  auto operator=(WritableSlotDescriptor&&) & noexcept -> WritableSlotDescriptor& = default;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_WRITABLE_SLOT_DESCRIPTOR_H_

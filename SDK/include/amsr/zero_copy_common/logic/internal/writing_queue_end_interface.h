/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Declaration of interface WritingQueueEndInterface.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::ZeroCopyQueue
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_WRITING_QUEUE_END_INTERFACE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_WRITING_QUEUE_END_INTERFACE_H_

#include "amsr/core/result.h"

#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/zero_copy_queue_debug_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief An interface for implementations of writable queue ends located in shared memory.
 * \details Allow to push SlotIndex instances to the queue.
 *          Do not allow to peek and pop SlotIndex instances from the queue.
 *          Derived classes must ensure to be nothrow constructible and destructible.
 * \vprivate Component Private
 */
class WritingQueueEndInterface : public ZeroCopyQueueDebugInterface {
 public:
  /*!
   * \brief Destruct the WritingQueueEndInterface.
   * \vprivate Component Private
   */
  ~WritingQueueEndInterface() noexcept override = 0;

  /*!
   * \brief Push a SlotIndex to the writable queue end, if the queue is not full.
   * \details This push operation must synchronize with ReadingQueueEndInterface::Peek() and
   *          ReadingQueueEndInterface::Pop() operations referring to the same queue
   *          (in either this or another process).
   *          In detail, if some actor A pushes a SlotIndex to the queue using this method that some actor B peeks
   *          and/or pops from the queue, actor B must see all writes done by actor A before it can peek and/or pop the
   *          SlotIndex.
   *          All write operations to shared memory must be non-blocking and appear atomic.
   * \param index SlotIndex to push.
   * \return True if the SlotIndex was pushed, false if the queue was full.
   * \error amsr::zero_copy_common::common::ZeroCopyCommonErrc::kQueueError If there is an error when using the queue.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-QueueCommunicationMemoryOrder
   * \vprivate Component Private
   */
  virtual auto Push(SlotHandle::SlotIndex index) noexcept -> amsr::core::Result<bool> = 0;

 protected:
  /*!
   * \brief Construct a WritingQueueEndInterface.
   * \vprivate Component Private
   */
  WritingQueueEndInterface() noexcept = default;

  /*!
   * \brief Construct a WritingQueueEndInterface by copying.
   * \vprivate Component Private
   */
  WritingQueueEndInterface(WritingQueueEndInterface const&) noexcept = default;

  /*!
   * \brief Construct a WritingQueueEndInterface by moving.
   * \vprivate Component Private
   */
  WritingQueueEndInterface(WritingQueueEndInterface&&) noexcept = default;

  /*!
   * \brief Copy another WritingQueueEndInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(WritingQueueEndInterface const&) & noexcept -> WritingQueueEndInterface& = default;

  /*!
   * \brief Move another WritingQueueEndInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(WritingQueueEndInterface&&) & noexcept -> WritingQueueEndInterface& = default;
};

inline WritingQueueEndInterface::~WritingQueueEndInterface() noexcept = default;

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_WRITING_QUEUE_END_INTERFACE_H_

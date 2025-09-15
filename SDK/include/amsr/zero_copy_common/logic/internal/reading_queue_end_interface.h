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
/*!       \file
 *        \brief  Declaration of interface ReadingQueueEndInterface.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::ZeroCopyQueue
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_READING_QUEUE_END_INTERFACE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_READING_QUEUE_END_INTERFACE_H_

#include "amsr/core/optional.h"
#include "amsr/core/result.h"

#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/zero_copy_queue_debug_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief An interface for implementations of readable queue ends located in shared memory.
 * \details Allow to peek and pop SlotIndex instances from the queue.
 *          Do not allow to push SlotIndex instances to the queue.
 *          Derived classes must ensure to be nothrow constructible and destructible.
 * \vprivate Component Private
 */
class ReadingQueueEndInterface : public ZeroCopyQueueDebugInterface {
 public:
  /*!
   * \brief Destruct the ReadingQueueEndInterface.
   * \vprivate Component Private
   */
  ~ReadingQueueEndInterface() noexcept override = 0;

  /*!
   * \brief Peek in the readable queue end to get a SlotIndex, if available.
   * \details This peek operation must synchronize with WritingQueueEndInterface::Push() operations referring to the
   *          same queue (in either this or another process).
   *          In detail, if some actor A peeks a SlotIndex from the queue using this method that some actor B has pushed
   *          to the queue, actor A must see all writes done by actor B before it can peek the SlotIndex.
   *          All write operations to shared memory must be non-blocking and appear atomic.
   * \return Optional containing a SlotIndex if available, empty Optional otherwise.
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
  virtual auto Peek() noexcept -> amsr::core::Result<amsr::core::Optional<SlotHandle::SlotIndex>> = 0;

  /*!
   * \brief Pop a SlotIndex from the readable queue end, if available.
   * \details This pop operation must synchronize with WritingQueueEndInterface::Push() operations referring to the
   *          same queue (in either this or another process).
   *          In detail, if some actor A pops a SlotIndex from the queue using this method that some actor B has pushed
   *          to the queue, actor A must see all writes done by actor B before it can pop the SlotIndex.
   *          All write operations to shared memory must be non-blocking and appear atomic.
   * \return Optional containing a SlotIndex if available, empty Optional otherwise.
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
  virtual auto Pop() noexcept -> amsr::core::Result<amsr::core::Optional<SlotHandle::SlotIndex>> = 0;

 protected:
  /*!
   * \brief Construct a ReadingQueueEndInterface.
   * \vprivate Component Private
   */
  ReadingQueueEndInterface() noexcept = default;

  /*!
   * \brief Construct a ReadingQueueEndInterface by copying.
   * \vprivate Component Private
   */
  ReadingQueueEndInterface(ReadingQueueEndInterface const&) noexcept = default;

  /*!
   * \brief Construct a ReadingQueueEndInterface by moving.
   * \vprivate Component Private
   */
  ReadingQueueEndInterface(ReadingQueueEndInterface&&) noexcept = default;

  /*!
   * \brief Copy another ReadingQueueEndInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ReadingQueueEndInterface const&) & noexcept -> ReadingQueueEndInterface& = default;

  /*!
   * \brief Move another ReadingQueueEndInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ReadingQueueEndInterface&&) & noexcept -> ReadingQueueEndInterface& = default;
};

inline ReadingQueueEndInterface::~ReadingQueueEndInterface() noexcept = default;

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_READING_QUEUE_END_INTERFACE_H_

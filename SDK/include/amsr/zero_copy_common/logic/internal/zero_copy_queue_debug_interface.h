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
 *        \brief  Declaration of interface ZeroCopyQueueDebugInterface.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::ZeroCopyQueue
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_ZERO_COPY_QUEUE_DEBUG_INTERFACE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_ZERO_COPY_QUEUE_DEBUG_INTERFACE_H_

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A debug interface for queue implementations located in shared memory.
 * \details Provide a debug interface for all queues located in shared memory
 *          (ReadingQueueEndInterface and WritingQueueEndInterface).
 *          Derived classes must ensure to be nothrow constructible and destructible.
 * \vprivate Component Private
 */
class ZeroCopyQueueDebugInterface {
 public:
  /*!
   * \brief Destruct the ZeroCopyQueueDebugInterface.
   * \vprivate Component Private
   */
  virtual ~ZeroCopyQueueDebugInterface() noexcept = 0;

 protected:
  /*!
   * \brief Construct a ZeroCopyQueueDebugInterface.
   * \vprivate Component Private
   */
  ZeroCopyQueueDebugInterface() noexcept = default;

  /*!
   * \brief Construct a ZeroCopyQueueDebugInterface by copying.
   * \vprivate Component Private
   */
  ZeroCopyQueueDebugInterface(ZeroCopyQueueDebugInterface const&) noexcept = default;

  /*!
   * \brief Construct a ZeroCopyQueueDebugInterface by moving.
   * \vprivate Component Private
   */
  ZeroCopyQueueDebugInterface(ZeroCopyQueueDebugInterface&&) noexcept = default;

  /*!
   * \brief Copy another ZeroCopyQueueDebugInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ZeroCopyQueueDebugInterface const&) & noexcept -> ZeroCopyQueueDebugInterface& = default;

  /*!
   * \brief Move another ZeroCopyQueueDebugInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ZeroCopyQueueDebugInterface&&) & noexcept -> ZeroCopyQueueDebugInterface& = default;
};

inline ZeroCopyQueueDebugInterface::~ZeroCopyQueueDebugInterface() noexcept = default;

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_ZERO_COPY_QUEUE_DEBUG_INTERFACE_H_

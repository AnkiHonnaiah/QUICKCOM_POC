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
/**        \file  upper_limit_allocator.h
 *        \brief  Memory Buffer Allocator with a configurable upper limit.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_UPPER_LIMIT_ALLOCATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_UPPER_LIMIT_ALLOCATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer_allocator.h"
#include "vac/memory/non_zeroing_allocator.h"

namespace amsr {
namespace someip_daemon_core {

/*!
 * \brief Memory Allocator for Flexible Unique Memory Buffers
 */
class UpperLimitAllocator final : public vac::memory::allocator::MemoryBufferAllocator {
 public:
  /*!
   * \brief Constructor.
   */
  UpperLimitAllocator() noexcept
      : UpperLimitAllocator{std::numeric_limits<vac::memory::allocator::MemoryBuffer::size_type>::max()} {}

  /*!
   * \brief Constructor.
   * \param[in] upper_limit  Maximum memory in bytes the allocator is allowed to allocate
   */
  explicit UpperLimitAllocator(vac::memory::allocator::MemoryBuffer::size_type const upper_limit) noexcept
      : vac::memory::allocator::MemoryBufferAllocator{}, upper_limit_{upper_limit} {}

  /*!
   * \brief Default destructor.
   */
  ~UpperLimitAllocator() noexcept final = default;

  UpperLimitAllocator(UpperLimitAllocator const &) = delete;
  UpperLimitAllocator(UpperLimitAllocator &&) = delete;
  UpperLimitAllocator &operator=(UpperLimitAllocator const &) & = delete;
  UpperLimitAllocator &operator=(UpperLimitAllocator &&) & = delete;

  /*!
   * \brief   Allocate memory.
   * \details Will fail if underlying allocator fails.
   *
   * \param[in]   size The size of the memory to allocate.
   * \return  The allocated memory.
   *
   * \pre              -
   * \threadsafe       TRUE
   *
   * \spec
   *  requires true.
   * \endspec
   */
  ara::core::Result<vac::memory::allocator::MemoryBufferPtr> Allocate(
      typename vac::memory::allocator::MemoryBuffer::size_type size) noexcept final {
    ara::core::Result<vac::memory::allocator::MemoryBufferPtr> result{
        ara::core::Result<vac::memory::allocator::MemoryBufferPtr>::FromError(SomeIpDaemonErrc::error_not_ok)};

    // Try to allocate only if the requested size isn't larger than the configured upper limit
    if (size <= upper_limit_) {
      result = value_.Allocate(size);
    }
    return result;
  }

  /*!
   * \brief  Check if Allocator can be deleted.
   * \return True if Allocator can be destroyed, False otherwise.
   */
  bool CanDestroy() const noexcept final { return value_.CanDestroy(); }

 private:
  /*!
   * \brief Upper size limit in bytes of memory that can be allocated
   */
  vac::memory::allocator::MemoryBuffer::size_type const upper_limit_;

  /*!
   * \brief The actual allocator
   */
  vac::memory::allocator::flexible::FlexibleUniqueMemoryBufferAllocator<vac::memory::NonZeroingAllocator<std::uint8_t>>
      value_{};
};

}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_UPPER_LIMIT_ALLOCATOR_H_

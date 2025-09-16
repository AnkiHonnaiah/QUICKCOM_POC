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
/*!      \file  memory_buffer_allocator_interface.h
 *      \brief  MemoryBufferAllocator interface
 *       \unit  VaCommonLib::MemoryManagement::BufferProvider
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_ALLOCATOR_INTERFACE_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_ALLOCATOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace vac {
namespace memory {
namespace allocator {

/*!
 * \brief Memory buffer allocator interface.
 * \trace CREQ-VaCommonLib-MemoryBuffer
 */
class MemoryBufferAllocator {
 public:
  /*!
   * \brief Default constructor.
   */
  MemoryBufferAllocator() noexcept = default;

  /*!
   * \brief Default MemoryBufferAllocator destructor.
   */
  virtual ~MemoryBufferAllocator() noexcept = default;

  /*!
   * \brief Deleted MemoryBufferAllocator copy constructor.
   */
  MemoryBufferAllocator(MemoryBufferAllocator const &) = delete;

  /*!
   * \brief Deleted MemoryBufferAllocator move constructor.
   */
  MemoryBufferAllocator(MemoryBufferAllocator &&) = delete;

  /*!
   * \brief Deleted MemoryBufferAllocator copy assignment.
   */
  auto operator=(MemoryBufferAllocator const &) & -> MemoryBufferAllocator & = delete;

  /*!
   * \brief Deleted MemoryBufferAllocator copy assignment.
   */
  auto operator=(MemoryBufferAllocator &&) & -> MemoryBufferAllocator & = delete;

  /*!
   * \brief Alias for std::size_t.
   */
  using size_type = std::size_t;

  /*!
   * \brief      Allocate a memory buffer.
   * \return     The Result type of allocated memory.
   * \pre        -
   * \threadsafe TRUE
   * \spec       requires true; \endspec
   */
  virtual auto Allocate(size_type) noexcept -> ara::core::Result<MemoryBufferPtr> = 0;

  /*!
   * \brief  To check if the Memory Buffer Allocator can be deleted.
   * \return True if the Allocator can be destroyed, False otherwise.
   */
  virtual auto CanDestroy() const noexcept -> bool = 0;
};

}  // namespace allocator
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_ALLOCATOR_INTERFACE_H_

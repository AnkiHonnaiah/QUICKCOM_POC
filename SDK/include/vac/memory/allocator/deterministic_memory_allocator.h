/**********************************************************************************************************************
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
/*!        \file  deterministic_memory_allocator.h
 *        \brief  Deterministic Memory Allocation interface
 *         \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_ALLOCATOR_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_ALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"
#include "vac/memory/buffer_provider.h"

namespace vac {
namespace memory {
namespace allocator {
namespace deterministic {

/*!
 * \brief The Interface for Allocation and Deallocation for DeterministicMemory buffers.
 * \trace DSGN-VaCommonLib-DeterministicMemoryBuffer
 */
class DeterministicMemoryAllocator : public MemoryBufferAllocator {
 public:
  /*! \brief An Alias for MemoryBuffer::value_type. */
  using value_type = MemoryBuffer::value_type;
  /*! \brief An alias for vac::memory::BufferProvide<T> with T =  MemoryBuffer::value_type. */
  using BufferProvider = vac::memory::BufferProvider<value_type>;

  /*! \brief Default constructor. */
  DeterministicMemoryAllocator() noexcept = default;
  /*! \brief Default Destructor. */
  ~DeterministicMemoryAllocator() noexcept override = default;
  /*! \brief Default copy constructor deleted. */
  DeterministicMemoryAllocator(DeterministicMemoryAllocator const &) = delete;
  /*! \brief Default move constructor deleted. */
  DeterministicMemoryAllocator(DeterministicMemoryAllocator &&) = delete;
  /*! \brief Default copy assignment operator deleted. */
  DeterministicMemoryAllocator &operator=(DeterministicMemoryAllocator const &) & = delete;
  /*! \brief Default move assignment operator deleted. */
  DeterministicMemoryAllocator &operator=(DeterministicMemoryAllocator &&) & = delete;
  /*!
   * \brief  Released the memory allocated for DeterministicMemory buffers.
   * \param  buffer_provider_ptr Pointer to the buffer to be released.
   * \return void
   */
  virtual void Deallocate(typename BufferProvider::pointer buffer_provider_ptr) noexcept = 0;
};

}  // namespace deterministic
}  // namespace allocator
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_ALLOCATOR_H_

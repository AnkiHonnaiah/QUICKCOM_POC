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
/*!        \file  deterministic_memory_buffer.h
 *        \brief  Deterministic Memory Buffer class
 *         \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_BUFFER_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_BUFFER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/memory/allocator/deterministic_memory_allocator.h"
#include "vac/memory/allocator/memory_buffer_interface.h"
#include "vac/memory/buffer_provider.h"

namespace vac {
namespace memory {
namespace allocator {
namespace deterministic {

/*!
 * \brief The class to create DeterministicMemory buffers.
 * \trace DSGN-VaCommonLib-DeterministicMemoryBuffer
 */
class DeterministicMemoryBuffer final : public MemoryBuffer {
  /*! \brief An alias for vac::memory::BufferProvide<T> with T =  MemoryBuffer::value_type. */
  using BufferProvider = vac::memory::BufferProvider<value_type>;

 public:
  /*! \brief Default constructor deleted. */
  DeterministicMemoryBuffer() = delete;
  /*! \brief Default copy constructor deleted. */
  DeterministicMemoryBuffer(DeterministicMemoryBuffer const&) = delete;
  /*! \brief Default move constructor deleted. */
  DeterministicMemoryBuffer(DeterministicMemoryBuffer&&) = delete;
  /*! \brief Default copy assignment operator deleted. */
  DeterministicMemoryBuffer& operator=(DeterministicMemoryBuffer const&) & = delete;
  /*! \brief Default move assignment operator deleted. */
  DeterministicMemoryBuffer& operator=(DeterministicMemoryBuffer&&) & = delete;

  /*!
   * \brief     Constructor to assign values to all private members.
   * \param[in] deterministic_memory_allocator Reference to deterministic memory allocator.
   * \param[in] buffer_provider_ptr Pointer to the buffer provider.
   * \param[in] number_of_buffer_elements Number of elements inside each buffer.
   */
  DeterministicMemoryBuffer(DeterministicMemoryAllocator& deterministic_memory_allocator,
                            typename BufferProvider::pointer buffer_provider_ptr,
                            typename MemoryBuffer::size_type const number_of_buffer_elements) noexcept
      : MemoryBuffer(),
        deterministic_memory_allocator_{deterministic_memory_allocator},
        buffer_provider_ptr_{buffer_provider_ptr},
        number_of_buffer_elements_{number_of_buffer_elements} {}

  /*!
   * \brief Released memory for Buffers allocated by deterministic memory allocator.
   */
  ~DeterministicMemoryBuffer() noexcept final {                        // VCA_VAC_SAFE_DEALLOCATION
    deterministic_memory_allocator_.Deallocate(buffer_provider_ptr_);  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
  }                                                                    // VCA_VAC_VALID_NONSTATIC_METHOD

  /*!
   * \brief     Reduce this message's size.
   * \details   This function can be used for e.g. reducing the size of the span returned by "GetView"
   *            after having used the buffer as a worst-case-sized preallocated buffer. Calls to this function
   *            will have no effect on memory usage, just an internal size counter will be changed.
   *            This function only allows reducing the current size.
   * \param[in] number_of_buffer_elements The desired new size
   * \threadsafe FALSE
   * \return    The new size after resizing. If the desired size was bigger than the actual one it will return the
   *            actual one.
   */
  typename MemoryBuffer::size_type ReduceSize(
      typename MemoryBuffer::size_type const number_of_buffer_elements) noexcept {
    number_of_buffer_elements_ = std::min(number_of_buffer_elements, number_of_buffer_elements_);
    return number_of_buffer_elements_;
  }

  /*!
   * \brief     Returns a view of allocated memory buffers using provided offset.
   * \param[in] offset The offset at which to start
   * \return    An amsr::core::Span if offset is in range of allocated buffers, empty amsr::core::Span otherwise
   */
  MemoryBuffer::MemoryBufferView GetView(typename MemoryBuffer::size_type offset) const noexcept final {
    MemoryBuffer::MemoryBufferView memory_buffer_view;
    if (offset < number_of_buffer_elements_) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      value_type* const temp_buffer_provider_ptr{static_cast<value_type*>(buffer_provider_ptr_ + offset)};
      memory_buffer_view = amsr::core::MakeSpan(temp_buffer_provider_ptr, (number_of_buffer_elements_ - offset));
    }
    return memory_buffer_view;
  }

 private:
  /*! \brief Deterministic memory allocator. */
  DeterministicMemoryAllocator& deterministic_memory_allocator_;
  /*! \brief Buffer provider pointer. */
  BufferProvider::pointer buffer_provider_ptr_;
  /*! \brief Number of buffer elements. */
  MemoryBuffer::size_type number_of_buffer_elements_;

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__BufferProvider__DeterministicMemoryBufferAllocator, Allocate);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__BufferProvider__DeterministicMemoryBufferAllocator, AllocateDeterministic);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__BufferProvider__DeterministicMemoryBufferAllocator, DeAllocate);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__BufferProvider__DeterministicMemoryBufferAllocator, DeAllocationFailed);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__BufferProvider__DeterministicMemoryBuffer, GetView);
};

}  // namespace deterministic
}  // namespace allocator
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_BUFFER_H_

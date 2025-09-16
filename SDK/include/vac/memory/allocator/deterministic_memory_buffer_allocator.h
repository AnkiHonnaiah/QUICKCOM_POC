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
/*!      \file  deterministic_memory_buffer_allocator.h
 *      \brief  Deterministic Memory Buffer Allocator class
 *       \unit  VaCommonLib::MemoryManagement::BufferProvider
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_BUFFER_ALLOCATOR_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_BUFFER_ALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/abort.h"
#include "amsr/generic/generic_error_domain.h"
#include "amsr/unique_ptr.h"
#include "vac/memory/allocator/deterministic_memory_allocator.h"
#include "vac/memory/allocator/deterministic_memory_buffer.h"

namespace vac {
namespace memory {
namespace allocator {
namespace deterministic {

/*!
 * \brief The class to Allocate and Deallocate DeterministicMemory buffers.
 * \trace DSGN-VaCommonLib-DeterministicMemoryBuffer
 */
class DeterministicMemoryBufferAllocator final : public DeterministicMemoryAllocator {
 public:
  /*!
   * \brief Alias for MemoryBufferAllocator.
   */
  using Base = MemoryBufferAllocator;

  /*!
   * \brief Default constructor deleted..
   */
  DeterministicMemoryBufferAllocator() = delete;
  /*!
   * \brief Default copy constructor deleted.
   */
  DeterministicMemoryBufferAllocator(DeterministicMemoryBufferAllocator const &) = delete;
  /*!
   * \brief Default move constructor deleted.
   */
  DeterministicMemoryBufferAllocator(DeterministicMemoryBufferAllocator &&) = delete;
  /*!
   * \brief Default copy assignment operator deleted.
   */
  DeterministicMemoryBufferAllocator &operator=(DeterministicMemoryBufferAllocator const &) & = delete;
  /*!
   * \brief Default move assignment operator deleted.
   */
  DeterministicMemoryBufferAllocator &operator=(DeterministicMemoryBufferAllocator &&) & = delete;
  /*!
   * \brief Default destructor.
   */
  ~DeterministicMemoryBufferAllocator() noexcept final {  // VCA_VAC_SAFE_DEALLOCATION_RUN_TIME_CHECK
    if (!CanDestroy()) {
      amsr::core::Abort(
          "vac::memory::allocator::deterministic::DeterministicMemoryBufferAllocator::"
          "~DeterministicMemoryBufferAllocator(): Not all allocated memory has been deallocated!");
    }
  }

  /*!
   * \brief     Constructor to reserve memory for Buffer allocation.
   * \param[in] number_of_buffers          Number of buffers to reserve.
   * \param[in] number_of_buffer_elements  Number of elements inside each buffer.
   */
  DeterministicMemoryBufferAllocator(typename Base::size_type const number_of_buffers,
                                     Base::size_type const number_of_buffer_elements) noexcept
      : DeterministicMemoryBufferAllocator(number_of_buffers, number_of_buffer_elements,
                                           ara::core::GetDefaultResource()) {}

  /*!
   * \brief     Constructor to reserve memory for Buffer allocation.
   * \param[in] number_of_buffers          Number of buffers to reserve.
   * \param[in] number_of_buffer_elements  Number of elements inside each buffer.
   * \param[in] memory_resource            The memory resource to use for allocating the DeterministicMemoryBuffer
   *                                       object. Note that the memory resource is used for the MemoryBuffer object,
   *                                       not necessarily for the actual memory.
   */
  DeterministicMemoryBufferAllocator(typename Base::size_type const number_of_buffers,
                                     Base::size_type const number_of_buffer_elements,
                                     ara::core::MemoryResource *memory_resource) noexcept
      : DeterministicMemoryAllocator(), memory_resource_(memory_resource) {
    buffer_provider_.reserve(number_of_buffers, number_of_buffer_elements);
  }

  /*!
   * \brief      Returns a pointer to allocated buffer big enough for number_of_elements.
   * \details    The allocation is done only if the number of buffer elements are less or equal to the reserved buffer
   *             elements. Otherwise return a null pointer.
   * \param[in]  number_of_buffer_elements  Number of elements inside each buffer.
   * \return     Result containing an unique pointer holds MemoryBuffer or an error code.
   * \error      amsr::generic::GenErrc::kRuntimeResourceOutOf, if the DeterministicMemoryBuffer object can't be
   *             allocated.
   * \error      amsr::generic::GenErrc::kRuntimeBufferTooSmall, if the memory of the buffer cannot be allocated.
   * \threadsafe TRUE
   */
  auto Allocate(typename Base::size_type number_of_buffer_elements) noexcept
      -> amsr::core::Result<MemoryBufferPtr> final {
    amsr::core::Result<MemoryBufferPtr> result{amsr::generic::GenErrc::kRuntimeBufferTooSmall};
    amsr::core::Result<amsr::UniquePtr<DeterministicMemoryBuffer>> deterministic_result{
        AllocateDeterministic(number_of_buffer_elements)};
    if (deterministic_result.HasValue()) {
      result.EmplaceValue(MemoryBufferPtr{deterministic_result.Value().release()});
    } else {
      result.EmplaceError(deterministic_result.Error());
    }
    return result;
  }

  /*!
   * \brief      Returns a pointer to an allocated buffer big enough for number_of_elements. The only difference to
   *             "Allocate" is that the result is not downcasted to "MemoryBufferPtr", so additional member functions
   *             on "DeterministicMemoryBuffer" can be accessed.
   * \details    The allocation is done only if number of buffer elements are less or equal to reserved buffer
   *             elements. Otherwise return a null pointer.
   * \param[in]  number_of_buffer_elements  Number of elements inside each buffer.
   * \return     Result containing a unique pointer holding a DeterministicMemoryBuffer or an error code.
   * \error      amsr::generic::GenErrc::kRuntimeResourceOutOf, if the DeterministicMemoryBuffer object can't be
   *             allocated.
   * \error      amsr::generic::GenErrc::kRuntimeBufferTooSmall, if the memory of the buffer cannot be allocated.
   * \threadsafe TRUE
   */
  auto AllocateDeterministic(typename Base::size_type number_of_buffer_elements) noexcept
      -> amsr::core::Result<amsr::UniquePtr<DeterministicMemoryBuffer>> {
    using RetT = amsr::core::Result<amsr::UniquePtr<DeterministicMemoryBuffer>>;
    RetT result{amsr::generic::GenErrc::kRuntimeBufferTooSmall};
    BufferProvider::pointer buffer_provider_ptr{buffer_provider_.allocate(number_of_buffer_elements)};
    if (buffer_provider_ptr != nullptr) {
      result = amsr::AllocateUnique<DeterministicMemoryBuffer>(
          ara::core::PolymorphicAllocator<DeterministicMemoryBuffer>{memory_resource_}, *this, buffer_provider_ptr,
          number_of_buffer_elements);
    }
    return result;
  }

  /*!
   * \brief      Checks if DeterministicMemoryBufferAllocator can be deleted.
   * \return     True if BufferProvider has no allocated memory, False otherwise.
   * \threadsafe FALSE
   */
  auto CanDestroy() const noexcept -> bool final { return !buffer_provider_.HasAllocations(); }

  /*!
   * \brief         Release the memory allocated for DeterministicMemory buffers.
   * \param[in,out] buffer_provider_ptr  Pointer to the buffer to be released.
   * \threadsafe    TRUE
   */
  void Deallocate(typename BufferProvider::pointer buffer_provider_ptr) noexcept final {
    buffer_provider_.deallocate(buffer_provider_ptr);
  }

 private:
  /*!
   * \brief Buffer Provider.
   */
  BufferProvider buffer_provider_{};

  /*!
   * \brief The memory resource to use for allocating a Memory Buffer object.
   */
  ara::core::MemoryResource *memory_resource_;

  /*!
   * \brief declaration for testing purposes
   */
  FRIEND_TEST(UT__BufferProvider__DeterministicMemoryBufferAllocator, DeAllocationFailed);
};

}  // namespace deterministic
}  // namespace allocator
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_DETERMINISTIC_MEMORY_BUFFER_ALLOCATOR_H_

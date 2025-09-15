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
/*!      \file  limited_memory_resource.h
 *       \brief  Provides LimitedMemoryResource which allocates on the heap for the specified number of times.
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_TESTING_LIMITED_MEMORY_RESOURCE_H
#define LIB_VAC_INCLUDE_VAC_TESTING_LIMITED_MEMORY_RESOURCE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>
#include <memory>
#include "amsr/vector.h"
#include "ara/core/memory_resource.h"

namespace vac {
namespace testing {

/*!
 * \brief    A memory resource for testing purposes.
 * \details  Allocates on the heap until the given maximum amount of allocations is exceeded, then returns nullptr.
 * \vprivate
 */
class LimitedMemoryResource : public ara::core::MemoryResource {
 public:
  /*!
   * \brief Allocator type.
   */
  using Alloc = std::allocator<uint8_t>;

  /*!
   * \brief Pointer type.
   */
  using pointer = std::allocator_traits<Alloc>::pointer;

  /*!
   * \brief Default constructor.
   */
  LimitedMemoryResource() = delete;

  /*!
   * \brief     Constructs a new instance of the LimitedMemoryResource class.
   * \details   Performs allocations depending on how many allocations has been performed. Outside the range
   *            [min_allocations_, max_allocations_) allocations fail, inside the range allocations succeed.
   * \param[in] max_allocations  Maximum amount of allocations required before the resource will return nullptr on
   *                             allocation.
   */
  explicit LimitedMemoryResource(std::size_t max_allocations) : LimitedMemoryResource(0, max_allocations) {}

  /*!
   * \brief     Constructs a new instance of the LimitedMemoryResource class.
   * \details   Performs allocations depending on how many allocations has been performed. Outside the range
   *            [min_allocations_, max_allocations_) allocations fail, inside the range allocations succeed.
   * \param[in] max_allocations  Maximum amount of allocations required before the resource will return nullptr on
   *                             allocation.
   * \param[in] min_allocations  Minumum amount of allocations required for the resource to not allocate nullptr.
   */
  explicit LimitedMemoryResource(std::size_t min_allocations, std::size_t max_allocations)
      : ara::core::MemoryResource{},
        allocated_blocks_(),
        allocator_(),
        min_allocations_(min_allocations),
        max_allocations_(max_allocations) {}

  /*!
   * \brief   Destructor.
   * \details Deallocates blocks so that no memory is leaked.
   */
  ~LimitedMemoryResource() noexcept override {  // VCA_VAC_POSSIBLY_FREEING_STATIC_MEMORY
    for (auto& alloc_rec : allocated_blocks_) {
      // VCA_VAC_FUNC_FULFILL_CONTRACT_RUN_TIME_CHECK
      allocator_.deallocate(static_cast<std::uint8_t*>(alloc_rec.ptr), alloc_rec.number_of_bytes);
    }
  };

  /*!
   * \brief  Returns the number of allocated blocks.
   * \return The number of allocated blocks that have not been deallocated, yet.
   */
  auto NumberOfAllocatedBlocks() const noexcept -> std::size_t { return allocated_blocks_.size(); }

  /*!
   * \brief  Checks if no memory is allocated.
   * \return true, if no memory is allocated; false otherwise.
   */
  auto NothingAllocated() const noexcept -> bool { return allocated_blocks_.empty(); }

 protected:
  /*!
   * \brief Default copy constructor.
   */
  LimitedMemoryResource(LimitedMemoryResource const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   */
  LimitedMemoryResource(LimitedMemoryResource&&) noexcept = default;

  /*!
   * \brief Default copy assignment operator.
   */
  auto operator=(LimitedMemoryResource const&) & noexcept -> LimitedMemoryResource& = default;

  /*!
   * \brief Default move assignment operator.
   */
  auto operator=(LimitedMemoryResource&&) & noexcept -> LimitedMemoryResource& = default;

  /*!
   * \brief     Allocate memory on the heap if the amount of already allocated memory does not exceed max_allocations_.
   * \param[in] bytes      The number of bytes to allocate.
   * \param[in] alignment  The alignment of the allocated storage.
   * \return    A pointer to the allocated memory or nullptr if no memory could be allocated.
   * \steady    FALSE
   */
  auto DoAllocate(size_t bytes, size_t alignment) noexcept -> void* override {
    pointer ptr{nullptr};
    if ((alloc_tries >= min_allocations_) && (alloc_tries < max_allocations_)) {
      ptr = allocator_.allocate(bytes);
      // ignore result as it is very unlikely that allocation for a few bytes will fail
      static_cast<void>(allocated_blocks_.EmplaceBack(AllocationRecord{ptr, bytes, alignment}));
    }
    alloc_tries++;
    return ptr;
  }

  /*!
   * \brief     Deallocates memory which was allocated by \a DoAllocate().
   * \param[in] p          A pointer to memory. Must have been allocated using \a DoAllocate().
   * \param[in] bytes      The number of bytes to deallocate.
   * \param[in] alignment  The alignment of the memory to deallocate.
   * \steady    FALSE
   */
  void DoDeallocate(void* const p, size_t bytes, size_t alignment) noexcept override {
    if (p != nullptr) {
      // Check that deallocation args exactly match allocation args.
      auto const i = std::find_if(allocated_blocks_.begin(), allocated_blocks_.end(),
                                  [p, bytes, alignment](AllocationRecord const& r) {
                                    return (r.ptr == p) && (r.number_of_bytes == bytes) && (r.alignment == alignment);
                                  });
      if (i != allocated_blocks_.end()) {
        // VECTOR NL AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
        allocator_.deallocate(static_cast<pointer>(p), bytes);  // VCA_VAC_FUNC_FULFILL_CONTRACT_RUN_TIME_CHECK
        static_cast<void>(allocated_blocks_.erase(i));
      }
    }
  }

  /*!
   * \brief     Checks if \p other equals \p this.
   * \details   They are considered equal if they refer to the same object, that is, if the addresses of the objects are
   *            equal.
   * \param[in] other  The other memory resource.
   * \return    True if \p other equals this object, false otherwise.
   * \steady    TRUE
   */
  auto DoIsEqual(ara::core::MemoryResource const& other) const noexcept -> bool override { return this == &other; }

 private:
  /*!
   * \brief Record holding the results of an allocation.
   */
  struct AllocationRecord {
    /*!
     * \brief Pointer to the allocated memory.
     */
    pointer ptr;

    /*!
     * \brief Number of bytes allocated.
     */
    size_t number_of_bytes;

    /*!
     * \brief Alignment of the allocated memory.
     */
    size_t alignment;
  };

  /*!
   * \brief Records holding the results of allocations.
   */
  amsr::Vector<AllocationRecord> allocated_blocks_;

  /*!
   * \brief The underlying allocator to use.
   */
  Alloc allocator_;

  /*!
   * \brief The minimum number of allocations necessary for the resource to allocate.
   */
  std::size_t min_allocations_{0U};

  /*!
   * \brief The maximum number of allocations possible for this resource.
   */
  std::size_t max_allocations_{0U};

  /*!
   * \brief The amount of allocations not allowed before max_allocations_ are allowed.
   */
  std::size_t alloc_offset{0U};

  /*!
   * \brief The amount of calls to allocate.
   */
  std::size_t alloc_tries{0U};
};

}  // namespace testing
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_TESTING_LIMITED_MEMORY_RESOURCE_H

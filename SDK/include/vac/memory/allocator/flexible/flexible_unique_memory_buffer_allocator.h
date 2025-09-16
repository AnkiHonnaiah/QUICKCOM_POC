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
/*!        \file
 *        \brief  FlexibleUniqueMemoryBufferAllocator is the allocator for FlexibleUniqueMemoryBuffer.
 *         \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"
#include "vac/memory/memory_buffer_allocator.h"

namespace vac {
namespace memory {
namespace allocator {
namespace flexible {

/*!
 * \brief           Allocator for Flexible Unique Memory Buffer
 * \details         Flexible: Is not limited in how many elements it can allocate nor how big each element can be.
 * \tparam          Alloc
 *                  An allocator which must fulfill C++ named requirements: Allocator.
 * \trace           CREQ-VaCommonLib-MemoryBuffer
 */
template <typename Alloc = ara::core::PolymorphicAllocator<std::uint8_t>>
class FlexibleUniqueMemoryBufferAllocator final : public MemoryBufferAllocator {
 public:
  /*!
   * \brief           Default constructor
   * \spec            requires true \endspec
   */
  FlexibleUniqueMemoryBufferAllocator() noexcept
      : FlexibleUniqueMemoryBufferAllocator(ara::core::GetDefaultResource()) {}

  /*!
   * \brief           Constructor
   * \param[in]       memory_resource
   *                  The memory resource to use for allocating the FlexibleUniqueMemoryBuffer object. Note that the
   *                  memory resource is used for the MemoryBuffer object, not necessarily for the actual memory. The
   *                  lifetime of the memory resource must be at least as long as the lifetime of the
   *                  FlexibleUniqueMemoryBufferAllocator.
   * \spec            requires true \endspec
   */
  explicit FlexibleUniqueMemoryBufferAllocator(ara::core::MemoryResource *memory_resource) noexcept
      : MemoryBufferAllocator(), memory_resource_(memory_resource) {}

  /*!
   * \brief           Default destructor
   * \spec            requires true \endspec
   */
  ~FlexibleUniqueMemoryBufferAllocator() noexcept final = default;  // VCA_VAC_SAFE_DEALLOCATION

  /*! \brief          Delete copy constructor */
  FlexibleUniqueMemoryBufferAllocator(FlexibleUniqueMemoryBufferAllocator const &) = delete;

  /*! \brief          Delete move constructor */
  FlexibleUniqueMemoryBufferAllocator(FlexibleUniqueMemoryBufferAllocator &&) = delete;

  /*! \brief          Delete copy assignment */
  auto operator=(FlexibleUniqueMemoryBufferAllocator const &) -> FlexibleUniqueMemoryBufferAllocator & = delete;

  /*! \brief          Delete move assignment */
  auto operator=(FlexibleUniqueMemoryBufferAllocator &&) -> FlexibleUniqueMemoryBufferAllocator & = delete;

  /*! \brief          Alias for container type */
  using Container = typename FlexibleUniqueMemoryBuffer<Alloc>::BufferType;

  // VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_VAC_A15.5.3_exceptionViolatesFunctionsNoexeceptSpec
  // VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_VAC_A15.4.2_exceptionViolatesFunctionsNoexeceptSpec
  /*!
   * \brief           Allocate memory
   * \details         Will fail if underlying allocator fails.
   * \param[in]       size
   *                  The size of the memory to allocate.
   * \return          The allocated memory.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
   *                  if allocation of the returned unique ptr failed.
   * \pre             -
   * \threadsafe      TRUE
   * \spec            requires true. \endspec
   */
  auto Allocate(typename MemoryBuffer::size_type size) noexcept -> amsr::core::Result<MemoryBufferPtr> final {
    // Alias for the type of MemoryBuffer
    using MemoryBufferType = FlexibleUniqueMemoryBuffer<Alloc>;

    // Flexible container. Resized here for performance and determinism.
    // Resizing to larger size later on is allowed.
    Container container{};
    container.resize(size);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

    return amsr::AllocateUnique<MemoryBufferType>(ara::core::PolymorphicAllocator<MemoryBufferType>{memory_resource_},
                                                  std::move(container))
        .Map([](amsr::UniquePtr<MemoryBufferType> &&mbt) noexcept -> MemoryBufferPtr { return std::move(mbt); });
  }

  /*!
   * \brief           Check if Allocator can be deleted
   * \return          True if Allocator can be destroyed, False otherwise.
   */
  auto CanDestroy() const noexcept -> bool final { return true; }

 private:
  /*! \brief          The memory resource to use for allocating a Memory Buffer object */
  ara::core::MemoryResource *memory_resource_;
};

}  // namespace flexible
}  // namespace allocator
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_

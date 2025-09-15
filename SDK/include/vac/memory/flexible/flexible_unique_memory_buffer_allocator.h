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
/**        \file  memory/flexible/flexible_unique_memory_buffer_allocator.h
 *        \brief  Flexible Unique Memory Buffer Allocator
 *         \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *      \details  Allocator for FlexibleUniqueMemoryBuffer
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/vector.h"
#include "ara/core/memory_resource.h"
#include "vac/memory/memory_buffer_allocator.h"

namespace vac {
namespace memory {
namespace flexible {

/*!
 * \brief   Forward declaration.
 * \tparam  IovecType type for io (should contain base_pointer and size, usually osabstraction::io::ConstIOBuffer or
 *          osabstraction::io::MutableIOBuffer is used).
 * \tparam  Alloc allocator used in underlying container. Must fulfill C++ named requirements: Allocator.
 */
template <typename IovecType, typename Alloc = ara::core::PolymorphicAllocator<std::uint8_t>>
class FlexibleUniqueMemoryBuffer;  // VCA_VAC_SAFE_DEALLOCATION

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief  Memory Allocator for Flexible Unique Memory Buffers
 * \tparam IovecType type for io (should contain base_pointer and size, usually osabstraction::io::ConstIOBuffer or
 *         osabstraction::io::MutableIOBuffer is used).
 * \tparam Alloc allocator used in underlying container. Must fulfill C++ named requirements: Allocator.
 */
template <typename IovecType, typename Alloc = std::allocator<std::uint8_t>>
class FlexibleUniqueMemoryBufferAllocator : public MemoryBufferAllocator<IovecType> {
 public:
  /*!
   * \brief Alias for the base class.
   */
  using Base = MemoryBufferAllocator<IovecType>;

  /*!
   * \brief Alias for value type.
   */
  using value_type = std::uint8_t;

  /*!
   * \brief Alias for container type.
   */
  using Container = amsr::core::Vector<value_type, Alloc>;

  /*!
   * \brief Default Constructor
   * \spec
   *  requires true;
   * \endspec
   */
  FlexibleUniqueMemoryBufferAllocator() noexcept = default;

  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~FlexibleUniqueMemoryBufferAllocator() noexcept override = default;  // VCA_VAC_SAFE_DEALLOCATION
  /*!
   * \brief Delete copy constructor.
   */
  FlexibleUniqueMemoryBufferAllocator(FlexibleUniqueMemoryBufferAllocator const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  FlexibleUniqueMemoryBufferAllocator(FlexibleUniqueMemoryBufferAllocator&&) = delete;
  /*!
   * \brief Delete copy assignment.
   * \return
   */
  FlexibleUniqueMemoryBufferAllocator& operator=(FlexibleUniqueMemoryBufferAllocator const&) & = delete;
  /*!
   * \brief Delete move assignment.
   * \return
   */
  FlexibleUniqueMemoryBufferAllocator& operator=(FlexibleUniqueMemoryBufferAllocator&&) & = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_VAC_A15.5.3_exceptionViolatesFunctionsNoexeceptSpec
  // VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_VAC_A15.4.2_exceptionViolatesFunctionsNoexeceptSpec
  /*!
   * \brief     Allocate memory.
   * \details   Will terminate if underlying allocator fails to allocate the given size.
   *
   * \param[in]     size The size of memory to allocate.
   * \return    The allocated memory.
   *
   * \pre              -
   * \threadsafe       TRUE
   *
   * \spec
   *  requires true;
   * \endspec
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstVirtual
  typename Base::MemoryBufferPtr Allocate(typename MemoryBuffer<IovecType>::size_type size) noexcept override {
    /*! \brief Alias for memory buffer type. */
    using MemoryBufferType = FlexibleUniqueMemoryBuffer<IovecType, Alloc>;

    // The container is actually flexible, however we resize here the given size to enhance performance.
    // resize with larger value is allowed later on.
    Container container;

    container.resize(size);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    // Create a unique container
    typename Base::MemoryBufferPtr allocated_memory{std::make_unique<MemoryBufferType>(std::move(container))};
    return allocated_memory;
  }
};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief   Flexible Unique buffer.
 * \details Flexible: The maximum size of the buffer is flexible; i.e. not constrained.
 *          Unique: This class owns its buffer, not shared with any other MemoryBuffer.
 * \tparam  IovecType type for io (should contain base_pointer and size, usually osabstraction::io::ConstIOBuffer or
 *          osabstraction::io::MutableIOBuffer is used).
 * \tparam  Alloc allocator used in underlying container. Must fulfill C++ named requirements: Allocator.
 */
template <typename IovecType, typename Alloc>
class FlexibleUniqueMemoryBuffer final : public MemoryBuffer<IovecType> {  // VCA_VAC_SAFE_DEALLOCATION
 public:
  /*!
   * \brief Alias for the base class
   */
  using Base = MemoryBuffer<IovecType>;

  /*!
   * \brief Alias for MemoryBuffer pointer
   */
  using MemoryBufferPtr = vac::memory::MemoryBufferPtr<IovecType>;

  /*!
   * \brief Alias for container type.
   */
  using BufferType = typename FlexibleUniqueMemoryBufferAllocator<IovecType, Alloc>::Container;

  /*!
   * \brief     Constructor for memory buffer and pre-allocated buffer.
   * \param[in] buffer Pre-allocated buffer using FlexibleUniqueMemoryBufferAllocator.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  explicit FlexibleUniqueMemoryBuffer(BufferType&& buffer) : Base{}, buffer_{std::move(buffer)} {}

  /*!
   * \brief     Obtain a view of the memory buffer.
   * \param     offset The offset at which to start.
   * \pre       -
   * \return    The desired view of the memory buffer.
   * \spec
   *  requires true;
   * \endspec
   */
  typename Base::MemoryBufferView GetView(typename Base::size_type offset) const noexcept final {
    typename Base::MemoryBufferView view{};
    if (offset < buffer_.size()) {
      view.push_back(  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
                       // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_CastRemovesConstQualification
          {const_cast<typename BufferType::value_type*>(&buffer_[offset]), buffer_.size() - offset});
    }
    return view;
  }

  /*!
   * \brief  Obtain the size of the memory buffer.
   * \pre    -
   * \return The size of the buffer.
   * \spec
   *  requires true;
   * \endspec
   */
  typename Base::size_type size() const noexcept final { return buffer_.size(); }

  // VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_VAC_A15.5.3_exceptionViolatesFunctionsNoexeceptSpec
  // VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_VAC_A15.4.2_exceptionViolatesFunctionsNoexeceptSpec
  /*!
   * \brief     Resize the memory buffer.
   * \param     nbytes The size to which to resize.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  void resize(typename Base::size_type nbytes) noexcept final {
    buffer_.resize(nbytes);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief Add data to the end of the buffer.
   * \param value The data to add.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  void push_back(typename Base::value_type const& value) noexcept final {
    buffer_.push_back(value);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

 private:
  /*!
   * \brief The unique buffer.
   */
  BufferType buffer_;
};

}  // namespace flexible
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_

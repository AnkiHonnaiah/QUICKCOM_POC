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
 *        \brief  FlexibleUniqueMemoryBuffer subclass to MemoryBuffer.
 *         \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>
#include <utility>
#include "amsr/core/vector.h"
#include "vac/memory/allocator/memory_buffer_interface.h"
#include "vac/memory/memory_buffer_allocator.h"

namespace vac {
namespace memory {
namespace allocator {
namespace flexible {

/*!
 * \brief           Flexible Unique Memory Buffer
 * \details         Flexible: This memory buffer works with flexible allocators such as
 *                  FlexibleUniqueMemoryBufferAllocator. Unique: This class owns its buffer, not shared with any other
 *                  MemoryBuffer. The memory this buffer contains is not default initialized.
 * \tparam          Alloc
 *                  Allocator used in underlying container. Must fulfill C++ named requirements: Allocator.
 * \trace           CREQ-VaCommonLib-MemoryBuffer
 */
template <typename Alloc = ara::core::PolymorphicAllocator<std::uint8_t>>
class FlexibleUniqueMemoryBuffer final : public MemoryBuffer {  // VCA_VAC_SAFE_DEALLOCATION
 public:
  /*! \brief          Alias for the value type used in the buffer */
  using value_type = std::uint8_t;

  /*! \brief          Alias for the buffer type */
  using BufferType = amsr::core::Vector<value_type, Alloc>;

  /*!
   * \brief           Constructor for MemoryBuffer and pre-allocated buffer
   * \param[in]       buffer
   *                  The buffer to move into the memory buffer.
   * \spec
   * requires true;
   * \endspec
   */
  explicit FlexibleUniqueMemoryBuffer(BufferType &&buffer) : MemoryBuffer{}, buffer_{std::move(buffer)} {}

  /*!
   * \brief           Obtain a view of the memory buffer
   * \param[in]       offset
   *                  The offset at which to start.
   * \return          The desired view of the memory buffer.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  MemoryBufferView GetView(size_type offset) const noexcept final {
    MemoryBufferView view;
    if (offset < buffer_.size()) {
      // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_CastRemovesConstQualification
      view = amsr::core::MakeSpan(const_cast<value_type *>(&buffer_[offset]), buffer_.size() - offset);
    }
    return view;
  }

 private:
  /*! \brief          Friend declaration for testing purposes */
  FRIEND_TEST(UT__BufferProvider__FlexibleUniqueMemoryBuffer, CustomAllocator);
  /*! \brief          The buffer which is allocated to */
  BufferType buffer_;
};

}  // namespace flexible
}  // namespace allocator
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_FLEXIBLE_FLEXIBLE_UNIQUE_MEMORY_BUFFER_H_

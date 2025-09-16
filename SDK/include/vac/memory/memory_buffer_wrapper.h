/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  memory_buffer_wrapper.h
 *        \brief  A wrapper class for soft migration to new implementation of MemoryBuffer.
 *         \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *      \details  This wrapper class make possible to use the new MemoryBuffer with minimal changes
 *                the existing code.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_MEMORY_BUFFER_WRAPPER_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_MEMORY_BUFFER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>

#include "vac/memory/allocator/memory_buffer_interface.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"
#include "vac/memory/memory_buffer.h"

namespace vac {
namespace memory {

/*!
 * \brief   Wrapper class to manage memory buffer created by new allocator::MemoryBuffer.
 * \tparam  IovecType type for io (should contain base_pointer and size, usually osabstraction::io::ConstIOBuffer or
 *          osabstraction::io::MutableIOBuffer is used).
 * \trace   CREQ-VaCommonLib-MemoryBuffer
 */
template <typename IovecType>
class MemoryBufferWrapper final : public MemoryBuffer<IovecType> {
 public:
  /*! \brief An alias for std::size_t. */
  using size_type = std::size_t;
  /*! \brief An alias for std::uint8_t. */
  using value_type = std::uint8_t;
  /*! \brief An alias for vac::memory::MemoryBuffer<IovecType, Alloc>. */
  using MemoryBuffer = vac::memory::MemoryBuffer<IovecType>;
  /*! \brief An alias for amsr::core::Vector<IovecType>. */
  using MemoryBufferView = typename MemoryBuffer::MemoryBufferView;

  /*! \brief Deleted MemoryBufferWrapper constructor. */
  MemoryBufferWrapper() = delete;

  /*! \brief Default MemoryBufferWrapper destructor. */
  ~MemoryBufferWrapper() noexcept final = default;  // VCA_VAC_SAFE_DEALLOCATION

  /*! \brief Deleted MemoryBufferWrapper copy constructor. */
  MemoryBufferWrapper(MemoryBufferWrapper const &) = delete;

  /*! \brief Deleted MemoryBufferWrapper move constructor. */
  MemoryBufferWrapper(MemoryBufferWrapper &&) = delete;

  /*! \brief Deleted MemoryBufferWrapper copy assignment. */
  MemoryBufferWrapper &operator=(MemoryBufferWrapper const &) & = delete;

  /*! \brief Deleted MemoryBufferWrapper copy assignment. */
  MemoryBufferWrapper &operator=(MemoryBufferWrapper &&) & = delete;

  /*!
   * \brief   Single argument Constructor
   * \param   memory_buffer the allocator::MemoryBufferPtr rvalue, must not be nullptr.
   */
  explicit MemoryBufferWrapper(allocator::MemoryBufferPtr &&memory_buffer)
      : MemoryBuffer{}, memory_buffer_{std::move(memory_buffer)}, buffer_size_{0} {
    if (!memory_buffer_) {
      amsr::core::Abort("vac::memory::MemoryBufferWrapper::MemoryBufferWrapper(): memory_buffer must not be nullptr.");
    }
    assert(memory_buffer_.get() != nullptr);           // COV_MSR_INV_STATE_ASSERT
    buffer_size_ = memory_buffer_->GetView(0).size();  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
  }

  /*!
   * \brief   Returns a view of allocated memory buffers using provided offset.
   * \details Calls GetView() of allocator::MemoryBuffer and convert the result to IovecType type.
   * \param   offset The offset at which to start
   * \return  amsr::core::Vector<IovecType> if offset is in range of allocated buffers,
   *          an empty amsr::core::Vector<IovecType> otherwise.
   */
  MemoryBufferView GetView(size_type offset) const final {
    MemoryBufferView io_buffer_view{};
    assert(memory_buffer_.get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
    amsr::core::Span<std::uint8_t> const buffer_view{memory_buffer_->GetView(offset)};
    if (!buffer_view.empty()) {
      // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      io_buffer_view.push_back({static_cast<std::uint8_t *>(buffer_view.data()), buffer_view.size()});
    }
    return io_buffer_view;
  }

  /*!
   * \brief   Returns the size of IovecType obtained by calling GetView().
   * \details size 0 will be returned if this method is called before invoking GetView(offset).
   * \return  size of IovecType if available, 0 otherwise
   */
  size_type size() const noexcept final { return buffer_size_; }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffect
  /*! \brief Override MemoryBuffer::resize(). */
  void resize(size_type) noexcept final {
    amsr::core::Abort("vac::memory::MemoryBufferWrapper: resize() for MemoryBufferWrapper is not supported yet!");
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffect
  /*! \brief Override MemoryBuffer::push_back(). */
  void push_back(value_type const &) noexcept final {
    amsr::core::Abort("vac::memory::MemoryBufferWrapper: push_back() for MemoryBufferWrapper is not supported yet!");
  }

 private:
  /*! \brief allocator::MemoryBufferPtr. */
  allocator::MemoryBufferPtr memory_buffer_;

  /*! \brief IovecType size. */
  size_type buffer_size_;
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_MEMORY_BUFFER_WRAPPER_H_

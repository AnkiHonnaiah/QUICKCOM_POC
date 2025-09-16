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
/**        \file  memory_buffer.h
 *        \brief  Manage memory created by a MemoryBufferAllocator.
 *         \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *      \details  Also implements an Iterator which makes it possible to use this as a replacement for a
 *                amsr::core::Vector.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_MEMORY_BUFFER_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_MEMORY_BUFFER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/span.h"
#include "amsr/core/vector.h"
#include "amsr/unique_ptr.h"

namespace vac {
namespace memory {

/*!
 * \brief   Abstract class to manage a created memory buffer by MemoryBufferAllocator.
 * \details This class must be inherited by the concrete implementation depending on the container type.
 *          Each subclass inheriting from MemoryBuffer must have its own MemoryBufferAllocator subclass that can create
 *          it.
 * \tparam  IovecType type for io (should contain base_pointer and size, usually osabstraction::io::ConstIOBuffer or
 *          osabstraction::io::MutableIOBuffer is used).
 * \trace   CREQ-VaCommonLib-MemoryBuffer
 */
template <typename IovecType>
class MemoryBuffer {
 public:
  /*!
   * \brief Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  MemoryBuffer() noexcept = default;
  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~MemoryBuffer() noexcept = default;
  /*!
   * \brief Delete copy constructor.
   */
  MemoryBuffer(MemoryBuffer const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  MemoryBuffer(MemoryBuffer&&) = delete;
  /*!
   * \brief Delete copy assignment.
   * \return
   */
  MemoryBuffer& operator=(MemoryBuffer const&) & = delete;
  /*!
   * \brief Delete move assignment.
   * \return
   */
  MemoryBuffer& operator=(MemoryBuffer&&) & = delete;

  /*!
   * \brief The value type of a MemoryBuffer is simply a byte.
   */
  using value_type = std::uint8_t;
  /*!
   * \brief The value type of a MemoryBuffer is simply a byte.
   */
  using const_value_type = value_type const;

  /*!
   * \brief Alias for Container View Type.
   */
  using MemoryBufferView = amsr::core::Vector<IovecType>;

  /*!
   * \brief   Iterator for a MemoryBuffer.
   * \details By providing an iterator returned by MemoryBuffer::begin() and MemoryBuffer::end(), it is possible
   *          to use MemoryBuffer as a drop-in replacement for amsr::core::Vector in the serializer (and elsewhere).
   * \tparam  T The type to iterate: "MemoryBuffer" or "MemoryBuffer const".
   * \tparam  V The corresponding value type: value_type or const_value_type.
   */
  template <typename T, typename V>
  class Iterator final : public std::iterator<std::forward_iterator_tag, V> {  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
   public:
    /*! \brief Alias for value type. */
    using value_type = typename std::remove_reference<V>::type;
    /*! \brief Alias for pointer type. */
    using pointer_type = value_type*;
    /*! \brief Alias for reference to value type. */
    using reference_type = typename std::add_lvalue_reference<V>::type;
    /*! \brief Alias for size type. */
    using size_type = typename T::size_type;

    /*!
     * \brief Alias for templated base type because otherwise Doxygen would be confused.
     */
    using IteratorBase = std::iterator<std::forward_iterator_tag, V>;
    /*!
     * \brief     Constructor for an Iterator.
     * \details   Receives a pointer to the concrete MemoryBuffer object to iterate on.
     * \param     memory_buffer The MemoryBuffer to iterate.
     * \param     offset The offset with which to initialize the memory buffer.
     * \pre       -
     * \spec
     *  requires true;
     * \endspec
     */
    Iterator(T const* memory_buffer, size_type offset) noexcept
        : IteratorBase{},
          memory_buffer_{memory_buffer},
          fragments_{memory_buffer_->GetView(0U)},  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
          state_{} {
      ResetState();
      Set(offset);
    }

    /*!
     * \brief     Advance a copy of the iterator by the number of bytes specified.
     * \param     increment The number of bytes by which to increment the iterator copy.
     * \pre       -
     * \return    A copy of this iterator, but incremented.
     * \spec
     *  requires true;
     * \endspec
     */
    auto operator+(size_type increment) const noexcept -> Iterator {
      // VECTOR Next Line AutosarC++17_10-M8.5.2: MD_VAC_M8.5.2_provideExplicitInitializer
      Iterator result{*this};
      result += increment;
      return result;
    }

    // VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_VAC_M5.17.1_noCallRelationToEnsureSemanticEquivalence
    /*!
     * \brief     Advance the iterator by the number of bytes specified.
     * \param     increment The number of bytes by which to increment this iterator.
     * \pre       -
     * \return    Reference to this iterator.
     * \spec
     *  requires true;
     * \endspec
     */
    Iterator& operator+=(size_type increment) & noexcept {
      increment = std::min(increment, (std::numeric_limits<size_type>::max() - state_.offset_));
      Set(state_.offset_ + increment);
      return *this;
    }

    /*!
     * \brief  Advance the iterator by one byte.
     * \pre    -
     * \return The iterator itself.
     * \spec
     *  requires true;
     * \endspec
     */
    Iterator& operator++() noexcept {
      *this += 1U;
      return *this;
    }

    /*!
     * \brief  Perform equality check for two iterators.
     * \param  other The iterator to compare against.
     * \pre    -
     * \return If both iterators are equal, true. Otherwise, false.
     * \spec
     *  requires true;
     * \endspec
     */
    bool operator==(Iterator const& other) const noexcept {
      return ((this->memory_buffer_ == other.memory_buffer_) && (this->state_.offset_ == other.state_.offset_));
    }

    /*!
     * \brief  Perform inequality check for two iterators.
     * \param  other The iterator to compare against.
     * \pre    -
     * \return If both iterators are equal, false. Otherwise, true.
     * \spec
     *  requires true;
     * \endspec
     */
    bool operator!=(Iterator const& other) const noexcept {  // Comment to force line-break. See TAR-22835.
      return !(*this == other);
    }

    /*!
     * \brief  Dereference the iterator.
     * \pre    -
     * \return Reference to the byte value at the iterator's current position.
     * \spec
     *  requires true;
     * \endspec
     */
    reference_type operator*() const noexcept { return *state_.byte_ptr_; }

   private:
    /*!
     * \brief The memory buffer to iterate.
     */
    T const* memory_buffer_;

    /*!
     * \brief Cached fragments as obtained from GetView(0U).
     */
    typename T::MemoryBufferView fragments_;

    /*!
     * \brief Struct to accommodate the iterator state.
     */
    struct IterState {
      /*!
       * \brief The iterator's current offset (logical).
       */
      size_type offset_{0U};
      /*!
       * \brief Index of the current offset's fragment ("physical").
       */
      std::size_t fragment_idx_{0U};
      // VECTOR Next Construct AutosarC++17_10-A4.10.1: MD_VAC_A4.10.1_nullptrLiteral
      /*!
       * \brief Actual pointer within the current offset's fragment ("physical").
       */
      pointer_type byte_ptr_{nullptr};
    };

    /*! \brief The iterator state.
     */
    IterState state_{};

    /*!
     * \brief Set an initial 'sane' state for the iterator.
     * \pre   -
     * \spec
     *  requires true;
     * \endspec
     */
    void ResetState() noexcept {
      state_.offset_ = 0U;
      state_.fragment_idx_ = 0U;
      // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
      state_.byte_ptr_ = (state_.fragment_idx_ < fragments_.size())
                             ? static_cast<pointer_type>(fragments_[state_.fragment_idx_].base_pointer)
                             : nullptr;
    }

    /*!
     * \brief     Set the iterator to a specific offset within the memory buffer.
     * \param     offset The offset to set.
     * \pre       -
     * \spec
     *  requires true;
     * \endspec
     */
    void Set(size_type offset) noexcept {
      std::size_t const buffer_size{memory_buffer_->size()};  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      std::size_t const nr_fragments{fragments_.size()};
      if ((nr_fragments == 0) && (buffer_size > 0)) {
        amsr::core::Abort("MemoryBuffer::Iterator::Set() called with inconsistent MemoryBuffer!");
      }
      // Cap offset at memory_buffer_->size().
      offset = std::min(offset, buffer_size);
      // Set bytes_to_advance to the number of bytes to advance.
      size_type const bytes_to_advance{offset - state_.offset_};
      // Calculate the correct offset into the current fragment.
      // VECTOR Next Construct AutosarC++17_10-M5.0.9: MD_VAC_M5.0.9_explicitConversionChangesSignedness
      // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
      size_type const fragment_offset{
          (state_.byte_ptr_ != nullptr)
              ? static_cast<size_type>(state_.byte_ptr_ -
                                       static_cast<pointer_type>(fragments_[state_.fragment_idx_].base_pointer))
              : 0U};
      // We have to subtract fragment_offset from the number of bytes in the current chunk.
      // We also have to be careful not to access fragments_[state_.fragment_idx_] if
      // state_.fragment_idx_ equals fragments_.size().
      std::size_t const bytes_left_in_fragment{
          (state_.offset_ < buffer_size) ? (fragments_[state_.fragment_idx_].size - fragment_offset) : 0U};

      size_type const bytes_to_advance_in_fragment{
          GetFragmentIdxAndOffset(nr_fragments, bytes_left_in_fragment, bytes_to_advance, fragment_offset)};

      // VECTOR Next Construct AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmeticAllocatorIterator
      // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
      state_.byte_ptr_ = (state_.fragment_idx_ < fragments_.size())
                             ? &(static_cast<pointer_type>(
                                   fragments_[state_.fragment_idx_].base_pointer)[bytes_to_advance_in_fragment])
                             : nullptr;

      // Update the offset.
      state_.offset_ = offset;
    }
    /*!
     * \brief     Helper function that advances the state_.fragment_idx if needed.
     * \details   If bytes_to_advance is larger than what is left in the current fragment the state_.fragment_idx is
     *            updated. This is repeated until we arrive at the correct fragment.
     * \param     nr_fragments Total number of fragments
     * \param     bytes_left_in_fragment Bytes left in current fragment.
     * \param     bytes_to_advance Nr of bytes to advance, could be more than what is left in the current fragment.
     * \param     fragment_offset The offset into the current fragment.
     * \pre       -
     * \return    The offset (in bytes) that should be used for calculating the state_.byte_ptr_.
     * \spec
     *  requires true;
     * \endspec
     */
    size_type GetFragmentIdxAndOffset(size_t nr_fragments, size_t bytes_left_in_fragment, size_type bytes_to_advance,
                                      size_type fragment_offset) noexcept {
      while ((bytes_to_advance > 0U) && (bytes_left_in_fragment <= bytes_to_advance) &&
             (state_.fragment_idx_ < (nr_fragments - 1U))) {
        ++state_.fragment_idx_;
        fragment_offset = 0U;
        bytes_to_advance -= bytes_left_in_fragment;
        // We don't have to subtract fragment_offset from the number of bytes in the current chunk, as this is always
        // zero at this point in time.
        // We also don't have to be careful not to access fragments_[state_.fragment_idx_] if state_.fragment_idx_ ==
        // fragments_.size(), as this loop is executed only so long as state_.fragment_idx_ < (nr_fragments - 1),
        // i.e. state_.fragment_idx_ will still be < nr_fragments after being incremented.
        bytes_left_in_fragment = fragments_[state_.fragment_idx_].size;
      }

      if ((bytes_to_advance > 0U) && (bytes_left_in_fragment <= bytes_to_advance)) {
        // We have to jump over the last fragment!!
        ++state_.fragment_idx_;
      }

      return bytes_to_advance + fragment_offset;
    }
  };

  /*!
   * \brief Type-alias for a memory buffer iterator.
   */
  using iterator = Iterator<MemoryBuffer, value_type>;

  /*!
   * \brief Type-alias for a const memory buffer iterator.
   */
  using const_iterator = Iterator<MemoryBuffer const, const_value_type>;

  /*!
   * \brief Type-alias for the size of this memory type.
   */
  using size_type = std::size_t;

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Obtain an iterator pointing at the logical offset specified.
   * \param     offset The desired logical offset.
   * \pre       -
   * \return    The desired iterator.
   * \spec
   *  requires true;
   * \endspec
   */
  iterator GetIterator(size_type offset) noexcept {
    iterator result{this, offset};
    return result;
  }

  /*!
   * \brief     Obtain an iterator pointing at the logical offset specified.
   * \param     offset The desired logical offset.
   * \pre       -
   * \return    The desired iterator.
   * \spec
   *  requires true;
   * \endspec
   */
  const_iterator GetIterator(size_type offset) const noexcept {
    const_iterator result{this, offset};
    return result;
  }

  /*!
   * \brief  Obtain an iterator pointing at logical offset 0.
   * \pre    -
   * \return The desired iterator.
   * \spec
   *  requires true;
   * \endspec
   */
  iterator begin() noexcept { return GetIterator(0U); }

  /*!
   * \brief  Obtain an iterator pointing at logical offset 0.
   * \pre    -
   * \return The desired iterator.
   * \spec
   *  requires true;
   * \endspec
   */
  const_iterator begin() const noexcept { return GetIterator(0U); }

  /*!
   * \brief  Obtain an iterator pointing just past the last element in the memory buffer.
   * \pre    -
   * \return The desired iterator.
   * \spec
   *  requires true;
   * \endspec
   */
  iterator end() noexcept { return GetIterator(size()); }  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK

  /*!
   * \brief  Obtain an iterator pointing just past the last element in the memory buffer.
   * \pre    -
   * \return The desired iterator.
   * \spec
   *  requires true;
   * \endspec
   */
  const_iterator end() const noexcept { return GetIterator(size()); }  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK

  /*!
   * \brief Type-alias for a  pointer of the value.
   */
  using pointer = value_type*;

  /*!
   * \brief Type-alias for a pointer to an immutable value.
   */
  using const_pointer = const_value_type*;

  /*!
   * \brief     Get view of the buffer starting from the given offset.
   * \param     offset Start offset of the view.
   * \pre       -
   * \return    Memory buffer view starting at the given offset to the end of the buffer.
   *            if offset exceeds the buffer size, an empty view is returned.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual MemoryBufferView GetView(size_type offset) const = 0;

  /*!
   * \brief  Get size of the buffer; i.e. number of elements that are inserted into the memory buffer.
   * \pre    -
   * \return Memory buffer size.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual size_type size() const = 0;

  /*!
   * \brief     Resize the buffer; i.e. insert the given number of elements into the memory buffer.
   * \param     nbytes New memory buffer size (in bytes).
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  virtual void resize(size_type nbytes) = 0;

  /*!
   * \brief STL-like adapter method for back insertion into a given memory buffer instance.
   * \param value The value to store.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  virtual void push_back(value_type const& value) = 0;

  /*!
   * \brief     Copies data to the memory buffer from the given buffer.
   * \details   The number of copied bytes must be lower than the maximum value for size_type.
   * \param     offset The start offset inside the memory buffer where the copying begins from.
   * \param     copy_size Maximum number of bytes to copy.
   * \param     buffer A data buffer to copy from.
   * \pre       -
   * \return    The number of bytes copied.
   *
   * Use void pointer as input parameter to avoid castings by user and to indicate that
   * this function accepts any memory pointer.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual size_type CopyIn(size_type offset, size_type copy_size, void const* buffer) const noexcept {
    // Tracks total number of bytes copied.
    size_type bytes_copied{0U};
    // Check for out-of-bound conditions.
    if ((copy_size > 0U) && (copy_size != std::numeric_limits<size_type>::max()) &&
        (offset < size())) {                              // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      MemoryBufferView const fragments{GetView(offset)};  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      // Create an array view in order to avoid pointer arithmetics.
      // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
      amsr::core::Span<value_type const> in_data{static_cast<const_pointer>(buffer), (copy_size + 1)};
      // Iterate over fragments and copy data chunkwise in.
      for (typename MemoryBufferView::value_type const& frag : fragments) {
        // Cast the destination buffer in order to make the compiler happy.
        // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
        pointer const out_data{static_cast<pointer>(const_cast<void*>(frag.base_pointer))};
        size_type const out_size{frag.size};
        // Truncate to fragment size or number of bytes left to be copied.
        size_type const bytes{std::min(out_size, copy_size)};
        // Copy next data chunk in.
        const_pointer const in_first{in_data.data()};
        const_pointer const in_last{in_data.subspan(bytes).data()};
        static_cast<void>(std::copy(in_first, in_last, out_data));
        // Update counters and move pointers for the next chunk.
        copy_size -= bytes;
        bytes_copied += bytes;
        in_data = in_data.subspan(bytes);
        // No bytes to copy anymore ?
        if (copy_size == 0U) {
          break;
        }
      }
    }  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return bytes_copied;
  }

  /*!
   * \brief     Copies data to the memory buffer from the given memory buffer.
   * \param     offset The start offset inside the memory buffer where the copying begins from.
   * \param     copy_size Maximum number of bytes to copy.
   * \param     buffer A memory buffer to copy from.
   * \param     buffer_offset The start offset inside the given memory buffer where the copying begins from.
   * \pre       -
   * \return    The number of bytes copied.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual size_type CopyIn(size_type offset, size_type copy_size, MemoryBuffer const& buffer,
                           size_type buffer_offset) const noexcept {
    // Tracks total number of bytes copied.
    size_type bytes_copied{0U};
    // Check for out-of-bound conditions.
    if ((offset < size()) && (buffer_offset < buffer.size())) {  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      // Now we are safe to adjust the total size to copy to stay within the limits of both buffers and not exceed the
      // given size of copy_size.
      size_type const this_size_from_offset{size() - offset};  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      size_type const other_size_from_offset{buffer.size() - buffer_offset};
      if ((other_size_from_offset < this_size_from_offset) && (other_size_from_offset < copy_size)) {
        copy_size = other_size_from_offset;
      } else if ((this_size_from_offset < other_size_from_offset) && (this_size_from_offset < copy_size)) {
        copy_size = this_size_from_offset;
      } else {
        // copy_size is already the smallest.
      }

      MemoryBufferView const fragments{GetView(offset)};  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      // Iterate over fragments and copy data chunkwise in.
      for (typename MemoryBufferView::value_type const& frag : fragments)  // COV_LIBVAC_RANGE_FOR_LOOP
      {
        size_type const out_size{frag.size};
        // Truncate to fragment size or number of bytes left to be copied.
        size_type const bytes{std::min(out_size, copy_size)};
        // Copy next data chunk in from the given buffer.
        // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
        bytes_copied += buffer.CopyOut(buffer_offset, bytes, const_cast<void*>(frag.base_pointer));
        // Update counters and move pointers for the next chunk.
        copy_size -= bytes;
        offset += bytes;
        buffer_offset += bytes;
        // No bytes to copy anymore ?
        if (copy_size == 0U) {
          break;
        }
      }
    }  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return bytes_copied;
  }

  /*!
   * \brief     Copies data from the memory buffer to the given buffer.
   * \param     offset The start offset inside the memory buffer where the copying begins from.
   * \param     copy_size Maximum number of bytes to copy.
   * \param     buffer A data buffer to copy into.
   * \pre       -
   * \return    The number of bytes copied.
   * \spec
   *  requires true;
   * \endspec
   *
   * Use void pointer as input parameter to avoid castings by user and to indicate that
   * this function accepts any memory pointer.
   */
  virtual size_type CopyOut(size_type offset, size_type copy_size, void* buffer) const noexcept {
    // Tracks total number of bytes copied.
    size_type bytes_copied{0U};
    // Check for out-of-bound conditions.
    if ((copy_size > 0U) && (offset < size())) {          // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      MemoryBufferView const fragments{GetView(offset)};  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
      // VECTOR Next Construct AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_overFlow
      // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_overFlow
      amsr::core::Span<value_type> const out_view{static_cast<pointer>(buffer), copy_size + 1U};
      size_type out_view_offset{0U};
      // Iterate over fragments and copy data chunkwise out.
      for (typename MemoryBufferView::value_type const& frag : fragments) {
        // Cast start of fragment to byte pointer in order to use pointer arithmetic.
        // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
        const_pointer const in_data{static_cast<const_pointer>(frag.base_pointer)};
        size_type const in_size{frag.size};
        // Truncate to fragment size or number of bytes left to be copied.
        size_type const bytes{std::min(in_size, copy_size)};
        // Copy next data chunk out.
        // VECTOR Next Construct AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_overFlow
        // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_overFlow
        const_pointer const in_last{amsr::core::Span<value_type const>{in_data, copy_size + 1U}.subspan(bytes).data()};
        static_cast<void>(std::copy(in_data, in_last, out_view.subspan(out_view_offset).data()));
        // Update counters and move pointers for the next chunk.
        copy_size -= bytes;
        out_view_offset += bytes;
        bytes_copied += bytes;
        // No bytes to copy anymore ?
        if (copy_size == 0U) {
          break;
        }
      }
    }  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return bytes_copied;
  }
};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief  Alias for MemoryBuffer pointer
 * \tparam IovecType type for io (should contain base_pointer and size, usually osabstraction::io::ConstIOBuffer or
 *         osabstraction::io::MutableIOBuffer is used).
 */
template <typename IovecType>
using MemoryBufferPtr = std::unique_ptr<MemoryBuffer<IovecType>>;

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief   Alias for MemoryBuffer pointer using amsr::UniquePtr
 * \details This is just a short-term solution to allow an easy migration to amsr::UniquePtr and shall be removed
 *          once all components have been migrated.
 * \tparam IovecType type for io (should contain base_pointer and size, usually osabstraction::io::ConstIOBuffer or
 *         osabstraction::io::MutableIOBuffer is used).
 */
template <typename IovecType>
using UniqueMemoryBufferPtr = amsr::UniquePtr<MemoryBuffer<IovecType>>;

}  // namespace memory
}  // namespace vac

// COV_JUSTIFICATION_BEGIN
//   \ID COV_LIBVAC_RANGE_FOR_LOOP
//     \ACCEPT TX
//     \REASON In the range for loop, the condition will always be fulfilled.
// COV_JUSTIFICATION_END

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_MEMORY_BUFFER_H_

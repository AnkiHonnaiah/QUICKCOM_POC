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
 *        \brief  View to a readable and writable memory segment.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_MEMORY_VIEW_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_MEMORY_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/abort.h"
#include "amsr/core/span.h"
#include "amsr/memory/internal/alignment_utilities.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_typed_memory_view.h"
#include "amsr/memory/readable_memory_view.h"

namespace amsr {
namespace memory {

/*!
 * \brief       View to a readable and writable memory segment.
 *
 * \vprivate    Vector product internal API
 *
 * \trace       DSGN-Osab-MemoryView
 */
class ReadWritableMemoryView final {
 public:
  /*!
   * \brief       Creates an empty view that points to no memory segment.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr ReadWritableMemoryView() noexcept : ReadWritableMemoryView(::amsr::core::Span<std::uint8_t>{}) {}

  /*!
   * \brief       Creates a view that points to a readable and writable memory segment.
   *
   * \param[in]   view_to_memory  Span that points to the readable and writable memory segment.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   */
  explicit constexpr ReadWritableMemoryView(::amsr::core::Span<std::uint8_t> view_to_memory) noexcept
      : view_{view_to_memory} {}

  /*!
   * \brief       Returns view to the readable and writable memory segment in form of a span.
   *
   * \return      Span of the memory segment.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr auto GetSpan() const noexcept -> ::amsr::core::Span<std::uint8_t> { return view_; }

  /*!
   * \brief       Creates a sub-view of this readable and writable memory view from an offset with a specified size.
   *
   * \param[in]   offset  Offset (in bytes) into this view where the created view shall be start. Must be in range of
   *                      this view.
   * \param[in]   size    Size (in bytes) that the created view shall have. offset+size must not exceed the size of this
   *                      view.
   *
   * \return      Sub-view of this readable memory view.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   */
  auto GetSubView(Offset offset, SizeBytes size) const noexcept -> ReadWritableMemoryView {
    return ReadWritableMemoryView{view_.subspan(offset.value, size.value)};
  }

  /*!
   * \brief       Creates a sub-view of this readable and writable memory view from an offset to the end of this view.
   *
   * \param[in]   offset  Offset (in bytes) into this view where the created view shall be start. Must be in range of
   *                      this view.
   *
   * \return      Sub-view of this readable memory view.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   */
  auto GetSubView(Offset offset) const noexcept -> ReadWritableMemoryView {
    return ReadWritableMemoryView{view_.subspan(offset.value)};
  }

  /*!
   * \brief       Interpret the memory that this view points to as an instance of a data type.
   *
   * \details     This function asserts that the size of this view and the alignment of this view allow interpreting the
   *              memory segment as such an instance of a data type.
   *
   * \tparam      T      Data type of the array that the memory should be interpreted as.
   *                     Restricted to trivially copyable, not empty type.
   *
   * \param[in]   alignment_requirement  Minimum alignment that an instance of the data type needs to have, defaults to
   *                                     alignof(T). Must be a power of 2. This view must be aligned to at least the
   *                                     specified alignment requirement.
   *
   * \return      Readable and writable typed memory view of the data type.
   *
   * \context     ANY
   *
   * \pre         The size of this view must be at least sizeof(T).
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - Call the other ReinterpretAsTyped overload with a size of 1 and return the result.
   * \endinternal
   */
  template <typename T>
  auto ReinterpretAsTyped(Alignment alignment_requirement = Alignment{alignof(T)}) const noexcept
      -> ReadWritableTypedMemoryView<T> {
    return ReinterpretAsTyped<T>(Size{1}, alignment_requirement);
  }

  /*!
   * \brief       Interpret the memory that this view points to as an array of a data type.
   *
   * \details     This function asserts that the size of this view and the alignment of this view allow interpreting the
   *              memory segment as such a data type array.
   *
   * \tparam      T      Data type of the array that the memory should be interpreted as.
   *                     Restricted to standard-layout, not empty type.
   *
   * \param[in]   count                  Number of elements that the array of the data type is supposed to have.
   * \param[in]   alignment_requirement  Minimum alignment that an instance of the data type needs to have, defaults to
   *                                     alignof(T). Must be a power of 2. This view must be aligned to at least the
   *                                     specified alignment requirement.
   *
   * \return      Readable and writable typed memory view of the data type.
   *
   * \context     ANY
   *
   * \pre         The size of this view must be at least count * sizeof(T).
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - Abort if the size of this view is insufficient for count data type elements.
   *  - Abort if the alignment requirement is not a power of 2.
   *  - Abort if the alignment requirement is larger than the alignment of the memory segment that this view points to.
   *  - reinterpret_cast the pointer to the memory segment into the data type and return a ReadableTypedMemoryView
   *    object of the data type.
   * \endinternal
   */
  template <typename T>
  auto ReinterpretAsTyped(Size count, Alignment alignment_requirement = Alignment{alignof(T)}) const noexcept
      -> ReadWritableTypedMemoryView<T> {
    if (view_.size() < (count.value * sizeof(T))) {
      ::amsr::core::Abort("ReinterpretAsTyped(): Size of memory view is insufficient.");
    }
    internal::AssertAlignmentValid(alignment_requirement);
    if (alignment_requirement.value > GetAlignment().value) {
      ::amsr::core::Abort("ReinterpretAsTyped(): Alignment of memory view is insufficient.");
    }
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_MemoryTypedViewReinterpretCast
    return ReadWritableTypedMemoryView<T>{::amsr::core::Span<T>{reinterpret_cast<T*>(view_.data()), count.value}};
  }

  /*!
   * \brief       Gets size (in bytes) of this readable and writable view.
   *
   * \return      Size (in bytes) of this view.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr auto GetSize() const noexcept -> SizeBytes { return SizeBytes{view_.size()}; }
  /*!
   * \brief       Calculates and returns alignment of the memory segment that this view points to.
   *
   * \return      Alignment of the memory segment that this view points to.
   *
   * \context     ANY
   *
   * \pre         This view must not be empty (constructed with default constructor) but the size of this view is
   *              allowed to be 0.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetAlignment() const noexcept -> Alignment { return internal::CalculateAlignment(view_.data()); }

  /*!
   * \brief       Converts this readable and writable view into a readable view.
   *
   * \return      Converted view.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr auto ToReadableView() const noexcept -> ReadableMemoryView { return ReadableMemoryView{view_}; }

 private:
  /*!
   * \brief   Span that points to the readable and writable memory segment.
   */
  ::amsr::core::Span<std::uint8_t> view_;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_MEMORY_VIEW_H_

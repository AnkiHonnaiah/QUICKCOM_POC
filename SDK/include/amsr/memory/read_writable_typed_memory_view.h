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
 *        \brief  View to a readable and writable array of a data type.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_TYPED_MEMORY_VIEW_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_TYPED_MEMORY_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/span.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/readable_typed_memory_view.h"

namespace amsr {
namespace memory {

/*!
 * \brief       View to a readable array of a data type.
 *
 * \tparam      T   Data type of the array. Restricted to trivially copyable, not empty type.
 *
 * \vprivate    Vector product internal API
 *
 * \trace       DSGN-Osab-MemoryView
 */
template <typename T>
class ReadWritableTypedMemoryView final {
  static_assert(std::is_trivially_copyable<T>::value,
                "T type of ReadWritableTypedMemoryView must be trivially copyable.");
  static_assert(!std::is_empty<T>::value, "T type of ReadWritableTypedMemoryView must not be empty.");

 public:
  /*!
   * \brief       Creates an empty view that points to no data.
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
  constexpr ReadWritableTypedMemoryView() noexcept : ReadWritableTypedMemoryView(::amsr::core::Span<T>{}) {}

  /*!
   * \brief       Creates a view that points to a readable and writable array of a data type.
   *
   * \param[in]   view_to_memory  Span that points to the readable and writable array of the data type.
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
  explicit constexpr ReadWritableTypedMemoryView(::amsr::core::Span<T> view_to_memory) noexcept
      : view_{view_to_memory} {}

  /*!
   * \brief       Returns view to the readable and writable array of the data type.
   *
   * \return      Span of the data type array.
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
  constexpr auto GetSpan() const noexcept -> ::amsr::core::Span<T> { return view_; }

  /*!
   * \brief       Returns reference to the first array element.
   *
   * \return      Reference to the first array element.
   *
   * \context     ANY
   *
   * \pre         The number of elements of the data type array must not be 0.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   */
  constexpr auto GetFirstElement() const noexcept -> T& { return view_[0]; }

  /*!
   * \brief       Gets number of elements in the readable data type array.
   *
   * \return      Number of elements in the readable data type array.
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
  constexpr auto GetSize() const noexcept -> Size { return Size{view_.size()}; }

  /*!
   * \brief       Converts this readable and writable typed view into a readable typed view of the same data type.
   *
   * \return      Converted typed view.
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
  constexpr auto ToReadableView() const noexcept -> ReadableTypedMemoryView<T> {
    return ReadableTypedMemoryView<T>{view_};
  }

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
  auto GetAlignment() const noexcept -> Alignment {
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastTPointerToVoidPointer
    return internal::CalculateAlignment(reinterpret_cast<void*>(view_.data()));
  }

 private:
  /*!
   * \brief   Span that points to the readable and writable array.
   */
  ::amsr::core::Span<T> view_;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READ_WRITABLE_TYPED_MEMORY_VIEW_H_

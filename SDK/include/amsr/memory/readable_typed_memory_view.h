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
 *        \brief  View to a readable array of a data type.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READABLE_TYPED_MEMORY_VIEW_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READABLE_TYPED_MEMORY_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/core/span.h"
#include "amsr/memory/internal/alignment_utilities.h"
#include "amsr/memory/memory_types.h"

namespace amsr {
namespace memory {

/*!
 * \brief       View to a readable array of a data type.
 *
 * \tparam      T   Data type of the array. Restricted to trivially copyable, not empty type.
 *
 * \vprivate    Vector product internal API
 * \trace       DSGN-Osab-MemoryView
 */
template <typename T>
class ReadableTypedMemoryView final {
  static_assert(std::is_trivially_copyable<T>::value, "T type of ReadableTypedMemoryView must be trivially copyable.");
  static_assert(!std::is_empty<T>::value, "T type of ReadableTypedMemoryView must not be empty.");

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
  constexpr ReadableTypedMemoryView() noexcept : ReadableTypedMemoryView(::amsr::core::Span<T const>{}) {}

  /*!
   * \brief       Creates a view that points to a readable array of a data type.
   *
   * \param[in]   view_to_memory  Span that points to the readable array of the data type.
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
  explicit constexpr ReadableTypedMemoryView(::amsr::core::Span<T const> view_to_memory) noexcept
      : view_{view_to_memory} {}

  /*!
   * \brief       Returns view to the readable array of the data type.
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
  constexpr auto GetSpan() const noexcept -> ::amsr::core::Span<T const> { return view_; }

  /*!
   * \brief       Returns const reference to the first array element.
   *
   * \return      Const reference to the first array element.
   *
   * \context     ANY
   *
   * \pre         The size of this view must not be 0.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   */
  constexpr auto GetFirstElement() const noexcept -> T const& { return view_[0]; }

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
    return internal::CalculateAlignment(reinterpret_cast<void const*>(view_.data()));
  }

 private:
  /*!
   * \brief   Span that points to the readable array.
   */
  ::amsr::core::Span<T const> view_;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_READABLE_TYPED_MEMORY_VIEW_H_

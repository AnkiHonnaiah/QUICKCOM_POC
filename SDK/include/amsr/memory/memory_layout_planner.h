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
 *        \brief  Provides MemoryLayoutPlanner class that helps with placing of data structures in memory segments.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_LAYOUT_PLANNER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_LAYOUT_PLANNER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/memory/internal/alignment_utilities.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/utilities.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Utility class that helps with placing of data structures in a memory segment.
 *
 * \details     Calculates offsets into the memory segment where the data structures can be placed so that access to
 *              them will not cause unaligned memory access.
 *
 * \vprivate    Vector product internal API
 *
 * \trace       DSGN-Osab-MemoryLayoutPlanner
 */
class MemoryLayoutPlanner final {
 public:
  /*!
   * \brief       Creates a MemoryLayoutPlanner.
   *
   * \details     Different MemoryLayoutPlanner instances are guaranteed to produce the same memory layout only if they
   *              were created with the same start alignment and if the calls to AddObject() / AddObjectArray() and
   *              AddPadding() were done with the same parameters and in the same order for all MemoryLayoutPlanner
   *              instances.
   *
   * \param[in]   start_alignment  Alignment of the start address of the memory segment in which the data structures are
   *                               supposed to be placed, defaults to the page size. This alignment is also the largest
   *                               alignment that can be used in following calls to AddObject() / AddObjectArray() /
   *                               AddPadding. Alignment must be a power of two.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady          TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  explicit MemoryLayoutPlanner(Alignment start_alignment = Alignment{GetPageSize().value}) noexcept;

  /*!
   * \brief       Places an object into the memory segment.
   *
   * \details     The object will be placed at the lowest possible offset into the memory segment following the last
   *              placed object while taking the alignment requirement into account.
   *
   * \tparam      T                      Type of object to place into the memory segment.
   *                                     Restricted to not empty type.
   *
   * \param[in]   alignment_requirement  Number of bytes that the object shall be aligned to, defaults to alignof(T).
   *                                     Must be a power of two and not larger than the start alignment.
   *
   * \return      Offset into the memory segment where the object was placed and space in the memory segment required
   *              for the object.
   *
   * \context     ANY
   *
   * \pre         The size of the memory layout must not exceed SIZE_MAX-start_alignment with this call.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady          TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - Assert that the passed alignment is a power of 2 and does not exceed the start alignment.
   *  - Call std::align() to get the next aligned address for the specified object with the specified alignment
   *    requirement past the current end address. Abort if std::align() failed.
   *  - Calculate the offset into the memory segment from the returned address.
   *  - Return the offset and the required size for the object.
   * \endinternal
   */
  template <typename T>
  auto AddObject(Alignment alignment_requirement = Alignment{alignof(T)}) noexcept -> std::pair<Offset, SizeBytes> {
    static_assert(!std::is_empty<T>::value, "T type passed to AddObject must not be empty.");

    internal::AssertAlignmentValid(alignment_requirement);
    AssertAlignmentRequirementValid(alignment_requirement);

    void* const next_aligned_address{
        // VCA_OSA_STL_CALL_PASS_PTR_RW
        std::align(alignment_requirement.value, sizeof(T), current_address_, space_.value)};
    if (next_aligned_address == nullptr) {
      ::amsr::core::Abort("MemoryLayoutPlanner: Size and/or alignment of added object is too big.");
    }
    // VECTOR NC AutosarC++17_10-M5.2.9: MD_OSA_M5.2.9_ReinterpretCastVoidPointerUintPtr
    // VECTOR NC AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastVoidPointerUintPtr
    std::pair<Offset, SizeBytes> const result{std::make_pair(
        Offset{reinterpret_cast<std::uintptr_t>(current_address_) - start_address_.value}, SizeBytes{sizeof(T)})};
    // VECTOR NC AutosarC++17_10-M5.2.9: MD_OSA_M5.2.9_ReinterpretCastVoidPointerUintPtr
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastVoidPointerUintPtr
    current_address_ = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(current_address_) + sizeof(T));
    space_.value -= sizeof(T);

    return result;
  }

  /*!
   * \brief       Places an array of objects into the memory segment.
   *
   * \details     The object array will be placed at the lowest possible offset into the memory segment following the
   *              last placed object while taking the alignment requirement into account.
   *
   * \tparam      T                      Type of object for which an array shall be placed into the memory segment.
   *                                     Restricted to not empty type.
   *
   * \param[in]   num_objects            Number of objects in the array. Must not be 0.
   * \param[in]   alignment_requirement  Number of bytes that the object array shall be aligned to, defaults to
   *                                     alignof(T). Note: This only aligns the start of the array, not every array
   *                                     element. Must be a power of two and not larger than the start alignment.
   *
   * \return      Offset into the memory segment where the object array was placed and space in the memory segment
   *              required for the object array.
   *
   * \context     ANY
   *
   * \pre         The size of the memory layout must not exceed SIZE_MAX-start_alignment with this call.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady          TRUE
   */
  /*!
   * \internal
   *  - Assert that num_objects is not 0.
   *  - Assert that the passed alignment is a power of 2 and does not exceed the start alignment.
   *  - Call std::align() to get the next aligned address for the specified object array with the specified alignment
   *    requirement past the current end address. Abort if std::align() failed.
   *  - Calculate the offset into the memory segment from the returned address.
   *  - Return the offset and the required size for the object array.
   * \endinternal
   */
  template <typename T>
  auto AddObjectArray(Size num_objects, Alignment alignment_requirement = Alignment{alignof(T)}) noexcept
      -> std::pair<Offset, SizeBytes> {
    static_assert(!std::is_empty<T>::value, "T type passed to AddObjectArray must not be empty.");

    if (num_objects.value == 0) {
      ::amsr::core::Abort("MemoryLayoutPlanner: Added object array cannot be empty.");
    }
    internal::AssertAlignmentValid(alignment_requirement);
    AssertAlignmentRequirementValid(alignment_requirement);

    // VECTOR NC AutosarC++17_10-A4.7.1: MD_OSA_A4.7.1_MemoryLayoutPlannerSizeWrapAround
    // VECTOR NL AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_MemoryLayoutPlannerSizeWrapAround
    std::size_t const array_size_bytes{num_objects.value * sizeof(T)};

    void* const next_aligned_address{
        // VCA_OSA_STL_CALL_PASS_PTR_RW
        std::align(alignment_requirement.value, array_size_bytes, current_address_, space_.value)};
    if (next_aligned_address == nullptr) {
      ::amsr::core::Abort("MemoryLayoutPlanner: Size and/or alignment of added object is too big.");
    }

    // VECTOR NC AutosarC++17_10-M5.2.9: MD_OSA_M5.2.9_ReinterpretCastVoidPointerUintPtr
    // VECTOR NC AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastVoidPointerUintPtr
    std::pair<Offset, SizeBytes> const result{
        std::make_pair(Offset{reinterpret_cast<std::uintptr_t>(current_address_) - start_address_.value},
                       SizeBytes{array_size_bytes})};
    // VECTOR NC AutosarC++17_10-M5.2.9: MD_OSA_M5.2.9_ReinterpretCastVoidPointerUintPtr
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastVoidPointerUintPtr
    current_address_ = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(current_address_) + array_size_bytes);
    space_.value -= array_size_bytes;

    return result;
  }

  /*!
   * \brief       Adds padding bytes to the memory segment.
   *
   * \param[in]   offset_to_pad_to       Pad to this offset (in bytes) into the memory segment. The offset must not
   *                                     point to memory already used by previously added objects, object arrays or
   *                                     padding.
   *
   * \context     ANY
   *
   * \pre         The size of the memory layout must not exceed SIZE_MAX-start_alignment with this call.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady          TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void AddPadding(Offset offset_to_pad_to) noexcept;

  /*!
   * \brief       Adds padding bytes to the memory segment.
   *
   * \param[in]   alignment_to_pad_to    Number of bytes that the address one byte past the current end of the memory
   *                                     layout shall be aligned to. Must be a power of two and not larger than the
   *                                     start alignment.
   *
   * \return      -
   *
   * \context     ANY
   *
   * \pre         The size of the memory layout must not already be SIZE_MAX-start_alignment before this call and must
   *              not exceed SIZE_MAX-start_alignment with this call.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady          TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto AddPadding(Alignment alignment_to_pad_to) noexcept -> Offset;

  /*!
   * \brief       Returns the size (in bytes) of the currently planned memory layout.
   *
   * \details     The memory segment has to be at least this large to realize the planned memory layout.
   *
   * \return      Size (in bytes) of the currently planned memory layout.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady          TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetRequiredMemorySize() const noexcept -> SizeBytes;

 private:
  /*!
   * \brief       Asserts that a passed alignment requirement is valid.
   *
   * \param[in]   alignment_requirement    Alignment requirement to check. Execution is aborted if this value exceeds
   *                                       the start alignment of the memory segment.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void AssertAlignmentRequirementValid(Alignment alignment_requirement) const noexcept;

  /*!
   * \brief   "Virtual" start address of the planned memory layout.
   *
   * \details This is not the "real" address of the memory segment but the lowest address that is aligned to
   *          start_alignment. This "virtual" address is needed to use std::align(). The memory layout created from this
   *          "virtual" address will be valid for the "real" address of the memory segment as long as all alignment
   *          requirements passed to std::align() do not exceed start_alignment.
   */
  Address const start_address_;

  /*!
   * \brief   "Virtual" current end address the planned memory layout.
   *
   * \details Points to the next unused byte.
   */
  void* current_address_;

  /*!
   * \brief   Remaining virtual address space past current_address_.
   */
  SizeBytes space_;
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_MEMORY_LAYOUT_PLANNER_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_OSAB_TOOLINGISSUE
//   \ACCEPT TX
//   \REASON Either the branch is already covered by a test case but VCast does not show correct coverage measurements,
//           or uncovered switch-label is reported for code parts which do not contain any switch label.
//           This is tracked by TAR-23964.
// COV_JUSTIFICATION_END

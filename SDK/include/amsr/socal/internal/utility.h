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
/*!        \file  utility.h
 *        \brief  Definition of commonly used utility functions.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_UTILITY_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_UTILITY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>

#include "amsr/core/string_view.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Class that provides commonly used utility functions.
 *
 * \unit Socal::Utility
 */
class Utility {
 public:
  /*!
   * \brief Returns the given pointer by performing a nullptr check.
   * \tparam    T    The underlying pointer type.
   * \param[in] ptr  Pointer to the underlying data.
   * \return The given pointer after nullptr check.
   * \steady TRUE
   * \spec requires ptr != nullptr; \endspec
   * \internal
   * - If the given pointer is invalid, terminate the application.
   * - Otherwise, return the given pointer.
   * \endinternal
   */
  template <typename T>
  static T* GetPointerWithNullCheck(T* ptr) noexcept {
    // VECTOR NC AutosarC++17_10-A5.0.2: MD_Socal_AutosarC++17_10-A5.0.2_RightOperandOfLogicalOpInDebugAssert
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOCAL_AutosarC++17_10-M0.1.2_dead_branch_nullptrCheckAlwaysTrue
    // VECTOR NL AutosarC++17_10-M5.3.1: MD_Socal_AutosarC++17_10-M5.3.1_RightOperandOfLogicalOpInDebugAssert
    assert((ptr != nullptr) && "Socal::Utility::GetPointerWithNullCheck: the given pointer must not be null.");
    return ptr;
  }

  /*!
   * \brief Returns a reference by dereferencing the given pointer by performing a nullptr check.
   * \tparam    T    The underlying pointer type that has to be dereferenced.
   * \param[in] ptr  Pointer to the data that has to be dereferenced.
   * \return Reference to the dereferenced data.
   * \steady TRUE
   * \spec requires ptr != nullptr; \endspec
   * \internal
   * - If the given pointer is invalid, terminate the application.
   * - Otherwise, dereference the given pointer and return its reference.
   * \endinternal
   */
  template <typename T>
  static T& GetRef(T* ptr) noexcept {
    return *(GetPointerWithNullCheck(ptr));  // VCA_SOCAL_RETURNED_VALID_REFERENCE
  }
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_UTILITY_H_

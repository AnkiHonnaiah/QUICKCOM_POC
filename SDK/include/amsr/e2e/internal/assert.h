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
 *        \brief  Assert function declaration.
 *
 *      \details  Assert function as a replacement for assert() that always terminates regardless of Debug/Release
 *                build.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_INTERNAL_ASSERT_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_INTERNAL_ASSERT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/abort.h"

namespace amsr {
namespace e2e {
namespace internal {

// VECTOR NC AutosarC++17_10-M0.1.8: MD_E2E_M0.1.8
/*!
 * \brief Replacement for assert() that always aborts if condition is false regardless of whether this is a Debug or
 *        Release build.
 * \param[in] condition condition that must be true otherwise amsr::core::Abort() is called.
 * \param[in] error_text text to be displayed if the assertion fails.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \steady TRUE
 * \unit amsr::e2e::internal::Assert
 */
inline void Assert(bool condition, char const* error_text) noexcept {
  if (!condition) {
    amsr::core::Abort(error_text);
  }
}

}  // namespace internal
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_INTERNAL_ASSERT_H_

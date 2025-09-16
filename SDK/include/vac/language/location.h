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
/*!        \file  location.h
 *        \brief  AMSR_FILE macro.
 *         \unit  VaCommonLib::LanguageSupport::Location
 *
 *      \details  The AMSR_FILE macro returns the raw filename from the standard __FILE__ macro without the path.
 *                Using this in a static_assert will break compilation in GCC5 and GCC6, even though it is done in
 *                compile time.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_LOCATION_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_LOCATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstddef>

namespace vac {
namespace language {
namespace internal {
/*!
 * \brief  Extracts a substring from a zero terminated c-string, which contains all characters after the last slash.
 * \param  str Pointer to the initial character of a zero terminated c-string.
 * \pre    -
 * \return A pointer to the first character after the last slash.
 * \trace  CREQ-VaCommonLib-Location
 * \spec
 *  requires true;
 * \endspec
 */
constexpr char const* SubstrPastLastSlash(char const* str) noexcept {
  char const* substr{str};

  while (*str != 0) {
    // VECTOR Next Line AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    if (*str++ == '/') {
      substr = str;
    }
  }

  return substr;
}

}  // namespace internal
}  // namespace language
}  // namespace vac

// VECTOR Disable AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_macroExtendsAnotherMacro
/*!
 * \brief   AMSR_FILE macro to get the current filename. For details see AMSR-23504.
 * \details This macro should always be evaluated at compile time. Tested with several compilers on godbolt.org.
 */
#ifdef VCAST_ENABLE  // VCAST injects non-constexpr calls into constexpr sections. See TAR-4653.
#define AMSR_FILE __FILE__
// TODO(vsarcnaza): remove this after TAR-6292 is resolved/closed.
#else
#define AMSR_FILE                                                                                     \
  []() {                                                                                              \
    constexpr char const* macro_loc_var_file{vac::language::internal::SubstrPastLastSlash(__FILE__)}; \
    return macro_loc_var_file;                                                                        \
  }()
#endif
// VECTOR Disable AutosarC++17_10-M16.3.2: MD_VAC_M16.3.2_the#And##OperatorsUsed
/*!
 * \brief Converts A to a string literal.
 */
#define STR_HELPER(A) #A
// VECTOR Enable AutosarC++17_10-M16.3.2

/*!
 * \brief Alias for STR_HELPER(A) macro.
 */
#define STR(A) STR_HELPER(A)
/*!
 * \brief AMSR_FILE_LINE macro to get the current filename and line number.
 */
#define AMSR_FILE_LINE                                                             \
  []() {                                                                           \
    constexpr char const* macro_loc_var_file_line{                                 \
        vac::language::internal::SubstrPastLastSlash(__FILE__ ":" STR(__LINE__))}; \
    return macro_loc_var_file_line;                                                \
  }()

// VECTOR Enable AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_macroExtendsAnotherMacro

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_LOCATION_H_
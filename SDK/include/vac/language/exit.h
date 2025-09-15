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
/*!        \file  vac/language/exit.h
 *        \brief  Exit for handling of "Corruptions".
 *         \unit  VaCommonLib::ErrorHandling::Exit
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_EXIT_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_EXIT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <cstdlib>

// VECTOR Disable AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_conditionalCompilationIsOnlyAllowedForIncludeGuards
#if defined(VECTORCAST)
extern "C" {
void VCAST_DUMP_COVERAGE_DATA(void);
}
#endif

namespace vac {
namespace language {

// VECTOR NC AutosarC++17_10-M3.2.4: MD_VAC_M3.2.4_functionWithoutDefinition
/*!
 * \brief   Function for handling of corruptions.
 * \details This is NOT the preferred way to handle corruptions. Returning the exit code to main is better, to not
 *          violate the single-point-of-exit principle. This function directly calls std::_Exit without any additional
 *          cleanup, e.g. no static objects are destroyed and no destructors are called. Functions passed to
 *          std::at_quick_exit() or std::atexit() are not called.
 * \param    exit_code Exit status of the program.
 * \pre      -
 * \spec
 *  requires true;
 * \endspec
 * \trace    DSGN-VaCommonLib-ErrorHandling
 * \vprivate Product private
 */
[[noreturn]] inline void Exit(std::uint8_t exit_code) noexcept {
// VECTOR Disable AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_conditionalCompilationIsOnlyAllowedForIncludeGuards
#if defined(VECTORCAST)
  VCAST_DUMP_COVERAGE_DATA();
#elif defined(_BullseyeCoverage)
  static_cast<void>(cov_write());
#endif
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_VAC_A15.5.2_useOfStdExit
  // VECTOR NL AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  std::_Exit(static_cast<int>(exit_code));
}

}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_EXIT_H_

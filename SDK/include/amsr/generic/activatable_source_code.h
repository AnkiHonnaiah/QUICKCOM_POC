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
/*!        \file  amsr/generic/activatable_source_code.h
 *        \brief  Macros to allow lower-graded code to exist in the same source files as higher-graded code.
 *
 *      \details  https://confluence.vi.vector.int/display/PAW/Concept+Activatable+Source+Code+of+Lower+Quality+Level
 *      \unit     VaCommonLib::ActivatableSourceCode
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_GENERIC_ACTIVATABLE_SOURCE_CODE_H_
#define LIB_VAC_INCLUDE_AMSR_GENERIC_ACTIVATABLE_SOURCE_CODE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>

// VECTOR Disable AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_conditionalCompilationIsOnlyAllowedForIncludeGuards
// >>> Global definitions >>>

// BETA
#ifdef AMSR_LG_BETA_ENABLED
/*! \brief Do nothing in beta code, marks function as deprecated otherwise. */
#define AMSR_LG_BETA
/*! \brief Do nothing in beta code, make function abort otherwise. */
#define AMSR_LG_BETA_VIRTUAL_PLACEHOLDER(function)
#else
/*! \brief Do nothing in beta code, marks function as deprecated otherwise. */
#define AMSR_LG_BETA [[deprecated("Beta code, not to be used in safety context!")]]
// VECTOR Disable AutosarC++17_10-M16.0.6: MD_VAC_M16.0.6_macroNotInParentheses
/*! \brief Do nothing in beta code, make function abort otherwise. */
#define AMSR_LG_BETA_VIRTUAL_PLACEHOLDER(function) \
  function { assert(false); }  // VECTOR SL Compiler-#940: MD_VAC_Compiler-#940_missingReturn
// Beta code, not to be used in safety context!
#endif
// VECTOR Enable AutosarC++17_10-M16.0.6: MD_VAC_M16.0.6_macroNotInParentheses

// QM
#ifdef AMSR_LG_QM_ENABLED
/*! \brief Do nothing in QM code, marks function as deprecated otherwise. */
#define AMSR_LG_QM
/*! \brief Do nothing in QM code, make function abort otherwise. */
#define AMSR_LG_QM_VIRTUAL_PLACEHOLDER(function)
#else
/*! \brief Do nothing in QM code, marks function as deprecated otherwise. */
#define AMSR_LG_QM [[deprecated("QM level code, not to be used in safety context!")]]
// VECTOR Disable AutosarC++17_10-M16.0.6: MD_VAC_M16.0.6_macroNotInParentheses
/*! \brief Do nothing in QM code, make function abort otherwise. */
#define AMSR_LG_QM_VIRTUAL_PLACEHOLDER(function) \
  function { assert(false); }  // VECTOR SL Compiler-#940: MD_VAC_Compiler-#940_missingReturn
// QM level code, not to be used in safety context!
#endif
// VECTOR Enable AutosarC++17_10-M16.0.6: MD_VAC_M16.0.6_macroNotInParentheses

// BETA QM
#if defined AMSR_LG_BETA_ENABLED && defined AMSR_LG_QM_ENABLED  // The single flags plus the combination of both.
#define AMSR_LG_BETAQM_ENABLED
/*! \brief Do nothing in beta QM code, marks function as deprecated otherwise. */
#define AMSR_LG_BETAQM
/*! \brief Do nothing in beta QM code, make function abort otherwise. */
#define AMSR_LG_BETAQM_VIRTUAL_PLACEHOLDER(function)
#else
/*! \brief Do nothing in beta QM code, marks function as deprecated otherwise. */
#define AMSR_LG_BETAQM [[deprecated("Beta QM code, not to be used in safety context!")]]
// VECTOR Disable AutosarC++17_10-M16.0.6: MD_VAC_M16.0.6_macroNotInParentheses
/*! \brief Do nothing in beta QM code, make function abort otherwise. */
#define AMSR_LG_BETAQM_VIRTUAL_PLACEHOLDER(function) \
  function { assert(false); }  // VECTOR SL Compiler-#940: MD_VAC_Compiler-#940_missingReturn
// Beta QM code, not to be used in safety context!
#endif
// VECTOR Enable AutosarC++17_10-M16.0.6: MD_VAC_M16.0.6_macroNotInParentheses

// <<< Global definitions <<<

// >>> Deleters >>>

namespace amsr {
namespace generic {

// BETA
/*!
 * \brief  Deleter for beta code.
 * \tparam T The type to delete.
 *         Limitations for T:
 *         Must be a type which fulfill C++ Named Requirements:  Destructible
 * \trace CREQ-VaCommonLib-ActivatableSourceCode
 * \vprivate Product private
 */
template <typename T>
class LgBetaDeleter final {
 public:
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffect
  /*!
   * \brief   Operator that deletes the given pointer.
   * \details Must only be used in beta code, will terminate through a faulty assert otherwise.
   *          If the given pointer is nullptr, no deletion will take place.
   * \param   t_ptr Pointer, of type T, pointing to the object that should be deleted.
   * \spec
   *   requires true;
   * \endspec
   */
  void operator()(T const* const t_ptr) {
    if (t_ptr != nullptr) {  // COV_LIBVAC_ONLY_POSSIBLE_IF_VCAST_ALLOWS
#ifdef AMSR_LG_BETA_ENABLED
      delete t_ptr;
#else
      // Beta code deleter, not to be used in safety context!
      assert(false);  // COV_LIBVAC_INV_STATE_ASSERT
#endif
    }
  }
};

// QM
/*!
 * \brief  Deleter for QM code.
 * \tparam T The type to delete.
 *         Limitations for T:
 *         Must be a type which fulfill C++ Named Requirements:  Destructible
 * \trace CREQ-VaCommonLib-ActivatableSourceCode
 * \vprivate Product private
 */
template <typename T>
class LgQmDeleter final {
 public:
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffect
  /*!
   * \brief   Operator that deletes the given pointer.
   * \details Must only be used in QM code, will terminate through a faulty assert otherwise.
   *          If the given pointer is nullptr, no deletion will take place.
   * \param   t_ptr Pointer, of type T, pointing to the object that should be deleted.
   * \spec
   *   requires true;
   * \endspec
   */
  void operator()(T const* const t_ptr) {
    if (t_ptr != nullptr) {  // COV_LIBVAC_ONLY_POSSIBLE_IF_VCAST_ALLOWS
#ifdef AMSR_LG_QM_ENABLED
      delete t_ptr;
#else
      // QM level code deleter, not to be used in safety context!
      assert(false);  // COV_LIBVAC_INV_STATE_ASSERT
#endif
    }
  }
};

// BETA QM
/*!
 * \brief  Deleter for beta QM code.
 * \tparam T The type to delete.
 *          Limitations for T:
 *         Must be a type which fulfill C++ Named Requirements:  Destructible
 * \trace CREQ-VaCommonLib-ActivatableSourceCode
 * \vprivate Product private
 */
template <typename T>
class LgBetaQmDeleter final {
 public:
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffect
  /*!
   * \brief   Operator that deletes the given pointer.
   * \details Must only be used in beta QM code, will terminate through a faulty assert otherwise.
   *          If the given pointer is nullptr, no deletion will take place.
   * \param   t_ptr Pointer, of type T, pointing to the object that should be deleted.
   * \spec
   *   requires true;
   * \endspec
   */
  void operator()(T const* const t_ptr) {
    if (t_ptr != nullptr) {  // COV_LIBVAC_ONLY_POSSIBLE_IF_VCAST_ALLOWS
#ifdef AMSR_LG_BETAQM_ENABLED
      delete t_ptr;
#else
      // Beta QM code deleter, not to be used in safety context!
      assert(false);  // COV_LIBVAC_INV_STATE_ASSERT
#endif
    }
  }
};

}  // namespace generic
}  // namespace amsr

// <<< Deleters <<<

// >>> Strings >>>

// VECTOR Disable AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
// VECTOR Disable AutosarC++17_10-M7.3.1: MD_VAC_M7.3.1_symbolGlobalNamespace
#if defined AMSR_LG_BETA_ENABLED && defined AMSR_LG_QM_ENABLED
static constexpr char const* AMSR_LG_USE_ME_TO_KEEP_ME_IN{"AMSR_LG_CONFIG_BETAQM"};
#elif defined AMSR_LG_BETA_ENABLED
static constexpr char const* AMSR_LG_USE_ME_TO_KEEP_ME_IN{"AMSR_LG_CONFIG_BETA"};
#elif defined AMSR_LG_QM_ENABLED
static constexpr char const* AMSR_LG_USE_ME_TO_KEEP_ME_IN{"AMSR_LG_CONFIG_QM"};
#else
// Exists so that it can be used, but empty like for components that make not use of this feature at all.
static constexpr char const* AMSR_LG_USE_ME_TO_KEEP_ME_IN{""};
#endif
// VECTOR Enable AutosarC++17_10-M7.3.1: MD_VAC_M7.3.1_symbolGlobalNamespace
// VECTOR Enable AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally

// <<< Strings <<<
// VECTOR Enable AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_conditionalCompilationIsOnlyAllowedForIncludeGuards

// COV_JUSTIFICATION_BEGIN
//   \ID COV_LIBVAC_ONLY_POSSIBLE_IF_VCAST_ALLOWS
//     \ACCEPT XF
//     \REASON Due to limitation that VCAST_DUMP_COVERAGE_DATA() cannot be placed in production code, this condition
//     can only evaluate as false when VCAST_DUMP_COVERAGE_DATA() can be placed in production code.
//   \ID COV_LIBVAC_INV_STATE_ASSERT
//     \ACCEPT XX
//     \REASON [COV_MSR_INV_STATE] Due to limitations of VCAST, coverage for assert statements can not be registered.
// COV_JUSTIFICATION_END

#endif  // LIB_VAC_INCLUDE_AMSR_GENERIC_ACTIVATABLE_SOURCE_CODE_H_
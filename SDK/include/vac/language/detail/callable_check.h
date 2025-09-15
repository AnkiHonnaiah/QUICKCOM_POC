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
/*!        \file  vac/language/detail/callable_check.h
 *        \brief  Helper functions to check if a callable is a nullptr.
 *         \unit  VaCommonLib::LanguageSupport::CallableCheck
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_CALLABLE_CHECK_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_CALLABLE_CHECK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <type_traits>
#include "vac/language/unique_function.h"

namespace vac {
namespace language {
namespace detail {

/*!
 * \brief     Check if a UniqueFunction callable is nullptr or not.
 * \tparam    Signature The Signature used for UniqueFunction.
 *            Must be of format: Return-type(parameter-list) [const-reference qualifiers]
 * \param     uf The UniqueFunction.
 * \pre       -
 * \return    True if the UniqueFunction contains a nullptr, false otherwise.
 * \spec
 *  requires true;
 * \endspec
 * \trace CREQ-VaCommonLib-CallableCheck
 * \vprivate  Component private
 */
template <typename Signature>
constexpr bool IsCallableNullPtr(vac::language::UniqueFunction<Signature> const& uf) noexcept {
  return !static_cast<bool>(uf);
}

/*!
 * \brief     Check if a function pointer is nullptr or not.
 * \tparam    F Function object. Must fulfill C++ Named Requirements: FunctionObject.
 * \param[in] f The callable.
 * \pre       -
 * \return    True if the callable is nullptr, false otherwise.
 * \spec
 *  requires true;
 * \endspec
 * \trace CREQ-VaCommonLib-CallableCheck
 * \vprivate  Component private
 */
template <typename F, typename std::enable_if_t<std::is_pointer<F>::value, std::int32_t> = 0>
constexpr bool IsCallableNullPtr(F const& f) noexcept {
  return f == nullptr;
}

/*!
 * \brief  Check if a function object is nullptr or not.
 * \tparam F Function object. Must fulfill C++ Named Requirements: FunctionObject.
 * \pre    -
 * \return Always false, because a function object cannot be nullptr.
 * \spec
 *  requires true;
 * \endspec
 * \trace CREQ-VaCommonLib-CallableCheck
 * \vprivate Component private
 */
template <typename F, typename std::enable_if_t<!std::is_pointer<F>::value, std::int32_t> = 0>
constexpr bool IsCallableNullPtr(F const&) noexcept {
  return false;
}

}  // namespace detail
}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_CALLABLE_CHECK_H_

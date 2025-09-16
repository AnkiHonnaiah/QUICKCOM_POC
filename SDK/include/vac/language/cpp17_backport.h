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
/*!        \file  cpp17_backport.h
 *        \brief  Contains useful backports of STL features from C++17 to C++11.
 *         \unit  VaCommonLib::LanguageSupport::Backports::Cpp17Backport
 *         \trace CREQ-VaCommonLib-BackportedCppFunctionalities
 *
 *      \details  The C++17 backported features include also the backported C++14 ones.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_CPP17_BACKPORT_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_CPP17_BACKPORT_H_

#include <cstdint>
#include <type_traits>
#include <utility>
#include "ara/core/utility.h"

namespace vac {
namespace language {

namespace detail {

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
// VECTOR Disable AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_compileTimeResolution
using std::swap;

/*!
 * \brief Helper template in namespace including swap (aka using swap;) to test if a call to swap is well formed.
 */
class swap_test {
 public:
  /*!
   * \brief  Overload for swappable resolution.
   * \tparam T The passed type.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True type.
   */
  template <typename T, typename = decltype(swap(std::declval<T&>(), std::declval<T&>()))>
  static constexpr std::true_type can_swap(std::uint8_t) noexcept {
    return std::true_type();
  }

  // VECTOR Next Construct AutosarC++17_10-M3.2.4: MD_VAC_M3.2.4_functionWithoutDefinition
  /*!
   * \brief  Overload for non-swappable resolution.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return False type.
   */
  template <typename>
  static constexpr std::false_type can_swap(...) noexcept;
};

/*!
 * \brief  Helper template to test if a call to swap is well formed.
 * \tparam T The passed type.
 * \trace  CREQ-VaCommonLib-BackportedCppFunctionalities
 */
template <typename T>
class swappable_impl : public swap_test {
 public:
  /*!
   * \brief Type containing the deduction result.
   */
  using type = decltype(can_swap<T>(0));
};

/*!
 * \brief  Test implementation for swappable.
 * \tparam T The passed type.
 */
template <typename T, bool = (std::is_const<typename std::remove_reference<T>::type>::value) ||
                             (std::is_function<typename std::remove_reference<T>::type>::value)>
struct is_swappable;

/*!
 * \brief  Specialization if the tested type is const.
 * \tparam T The passed type.
 */
template <typename T>
struct is_swappable<T, true> : public std::false_type {};

/*!
 * \brief  Specialization if the tested type is non-const.
 * \tparam T The passed type.
 */
template <typename T>
struct is_swappable<T, false> : swappable_impl<T>::type {};

/*!
 * \brief  Implementation of nothrow swappable.
 * \tparam T The passed type.
 */
template <typename T, bool = is_swappable<T>::value>
struct is_nothrow_swappable;

/*!
 * \brief  Specialization for swappable types.
 * \tparam T The passed type.
 */
template <typename T>
struct is_nothrow_swappable<T, true>
    : std::integral_constant<bool, noexcept(swap(std::declval<T&>(), std::declval<T&>()))> {};

/*!
 * \brief  Specialization for non-swappable types.
 * \tparam T The passed type.
 */
template <typename T>
struct is_nothrow_swappable<T, false> : std::false_type {};

/*!
 * \brief   Wrapper for void_t.
 * \details For some older compilers, unused parameters in alias templates are not guaranteed to ensure SFINAE and can
 *          be ignored.
 * \tparam  Ts The passed type in sequence.
 */
template <typename... Ts>
class make_void {
 public:
  /*! \brief Setting the type to void.
   */
  using type = void;
};
}  // namespace detail

/*!
 * \brief  Is true if swap<T> is possible.
 * \tparam T The passed type.
 */
template <typename T>
struct is_swappable : detail::is_swappable<T> {};

/*!
 * \brief  Is true if swap<T> is noexcept.
 * \tparam T The passed type.
 */
template <typename T>
struct is_nothrow_swappable : detail::is_nothrow_swappable<T> {};

/*! \brief  Utility metafunction that maps a sequence of any types to the type void.
 *  \tparam Ts The passed type in sequence.
 */
template <typename... Ts>
using void_t = typename detail::make_void<Ts...>::type;

/*!
 * \brief Conjunction.
 */
template <typename...>
struct conjunction : std::true_type {};
/*!
 * \brief  Conjunction.
 * \tparam B1 The passed class. B1 must be usable as a base class and define member value that is convertible to bool.
 */
template <typename B1>
struct conjunction<B1> : B1 {};
/*!
 * \brief  Conjunction allows to perform a logical AND on a variadic pack of boolean values.
 * \tparam B1 The passed class. B1 must be usable as a base class and define member value that is convertible to bool.
 * \tparam Bn The passed class in sequence. Every template argument Bi for which Bi::value
 *         is instantiated must be usable as a base class and define member value that is convertible to bool.
 */
template <class B1, class... Bn>
struct conjunction<B1, Bn...> : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {};

/*! \brief Disjunction.
 */
template <typename...>
struct disjunction : std::false_type {};

/*!
 * \brief  Disjunction.
 *  \tparam B1 The passed type. B1 must be usable as a base class and define member value that is convertible to bool.
 */
template <typename B1>
struct disjunction<B1> : B1 {};

/*!
 * \brief Disjunction allows to perform a logical OR on a variadic pack of boolean values.
 * \tparam B1 The passed class. B1 must be usable as a base class and define member value that is convertible to bool.
 * \tparam Bn The passed class in sequence. Every template argument Bi for which Bi::value
 *         is instantiated must be usable as a base class and define member value that is convertible to bool.
 */
template <typename B1, typename... Bn>
struct disjunction<B1, Bn...> : std::conditional<bool(B1::value), B1, disjunction<Bn...>>::type {};

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
// VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_VAC_M7.1.2_parameterAsReferencePointerToConst
// VECTOR Next Construct AutosarC++17_10-A7.5.1: MD_VAC_A7.5.1_ReferenceOrPointerToAParameterPassedByReferenceToConst
/*!
 * \brief     Forms lvalue reference to const type of t.
 * \tparam    T The value type.
 * \param[in] t The non-const value.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    The const-reference form of the value.
 */
template <typename T>
constexpr std::add_const_t<T>& as_const(T& t) noexcept {
  return t;
}

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief  Const rvalue reference overload is deleted to disallow rvalue arguments.
 * \tparam T Value type.
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
void as_const(T const&&) = delete;

namespace detail {
/*!
 * \brief  Default type of is_invocable_r_impl.
 * \tparam Void Void type of the first input.
 * \tparam R The type to check conversion against. R must be a complete type.
 * \tparam F The callable type. F must be a complete type.
 * \tparam Args... The arguments to F. All types in the parameter pack Args shall each be a complete type.
 */
template <typename Void, typename R, typename F, typename... Args>
struct is_invocable_r_impl : std::false_type {};

/*!
 * \brief  Specialization of is_invocable_r_impl when F is invocable.
 * \tparam R The type to check conversion against. R must be a complete type.
 * \tparam F The callable type. F must be a complete type.
 * \tparam Args... The arguments to F. All types in the parameter pack Args shall each be a complete type.
 */
template <typename R, typename F, typename... Args>
struct is_invocable_r_impl<void_t<std::result_of_t<F(Args...)>>, R, F, Args...>
    : disjunction<std::is_void<R>, std::is_convertible<std::result_of_t<F(Args...)>, R>> {};
}  // namespace detail

/*!
 * \brief  Rough implementation of C++17 std::is_invocable_r.
 * \tparam R The type to check conversion against. R must be a complete type.
 * \tparam F The callable type. F must be a complete type.
 * \tparam Args... The arguments to F. All types in the parameter pack Args shall each be a complete type.
 */
template <typename R, typename F, typename... Args>
struct is_invocable_r : detail::is_invocable_r_impl<void, R, F, Args...> {};

// VECTOR Enable AutosarC++17_10-A12.4.1
}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_CPP17_BACKPORT_H_

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
/*!        \file  compile_time.h
 *        \brief  Compile time programming complementing stds type_traits.
 *         \unit  VaCommonLib::LanguageSupport::CompileTimeOperations
 *
 *      \details  Provides structs/constexpr functions for compile time programming.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_COMPILE_TIME_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_COMPILE_TIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

#include "ara/core/array.h"

namespace vac {
namespace language {
namespace compile_time {

/*!
 * \brief Negates the value in X.
 * \tparam X The class which contains a value.
 *         Limitations for X:
 *         Must have static member X::value of type bool
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <class X>
using negate_t = std::integral_constant<bool, !X::value>;

/*!
 * \brief bool sequence.
 * \trace CREQ-VaCommonLib-CompileTimeOperations
 */
template <bool...>
struct bool_sequence {};

/*!
 * \brief  Is true if all values passed evaluate to true.
 * \tparam Xs The bool type in sequence.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <bool... Xs>
using all = std::is_same<bool_sequence<true, Xs...>, bool_sequence<Xs..., true>>;

/*!
 * \brief  Is true if not any values passed evaluate to true.
 * \tparam Xs The bool type in sequence.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <bool... Xs>
using not_any = std::is_same<bool_sequence<false, Xs...>, bool_sequence<Xs..., false>>;

/*!
 * \brief  Is true if any values passed evaluate to true.
 * \tparam Xs The bool type in sequence.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <bool... Xs>
using any = negate_t<not_any<Xs...>>;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief  Is true if not all values passed evaluate to true.
 * \tparam Xs The bool type in sequence.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <bool... Xs>
using not_all = negate_t<all<Xs...>>;

/*!
 * \brief  Is true if the passed value is contained in the passed types.
 * \tparam T The predicate type.
 * \tparam Xs The passed types in sequence.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return True if the predicate type is in the list of types.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <typename T, typename... Xs>
constexpr inline auto any_in() noexcept -> bool {
  return any<std::is_same<T, Xs>::value...>::value;
}

/*!
 * \brief  Get Maximum sizeof for types.
 * \tparam Xs The passed types in sequence.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return The maximum sizeof for the passed types.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <typename... Xs>
constexpr inline auto get_max_sizeof() noexcept -> std::size_t {
  return std::max({static_cast<size_t>(0), sizeof(Xs)...});
}

/*!
 * \brief  Get Maximum alignment for types.
 * \tparam Xs The passed types in sequence.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return The maximum alignment for the passed types.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <typename... Xs>
constexpr inline auto get_max_alignof() noexcept -> std::size_t {
  return std::max({static_cast<size_t>(0), alignof(Xs)...});
}

/*!
 * \brief Tailrecursive implementation of index_from_Type.
 * \tparam N size_t type of input.
 * \tparam T The passed type.
 * \tparam Xs The passed types in sequence.
 */
template <std::size_t N, typename T, typename... Xs>
struct index_from_type_tailrec;
/*!
 * \brief   Tailrecursive implementation of index_from_Type.
 * \tparam  N size_t type of input.
 * \tparam  T The passed type.
 * \tparam  Xs The passed types in sequence.
 * \details Endpoint: If the head of the list is of the wanted type, then return N.
 */
template <std::size_t N, typename T, typename... Xs>
struct index_from_type_tailrec<N, T, T, Xs...> : std::integral_constant<std::size_t, N> {};
/*!
 * \brief   Tailrecursive implementation of index_from_Type.
 * \tparam  N size_t type of input.
 * \tparam  T The passed type.
 * \tparam  X The passed type.
 * \tparam  Xs The passed types in sequence.
 * \details Recursion: The head of the list is NOT of the wanted type, so increment N and recursive with the lists tail.
 */
template <std::size_t N, typename T, typename X, typename... Xs>
struct index_from_type_tailrec<N, T, X, Xs...>
    : std::integral_constant<std::size_t, index_from_type_tailrec<N + 1, T, Xs...>::value> {};
/*!
 * \brief  Calculate the index for T in the list of Xs.
 * \tparam T The passed type.
 * \tparam Xs The passed types in sequence.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <typename T, typename... Xs>
struct index_from_type : std::integral_constant<std::size_t, index_from_type_tailrec<0, T, Xs...>::value> {};

/*!
 * \brief     Gets the index of type X in the list of passed types.
 * \tparam    X The first passed type.
 * \tparam    Xs The passed types in sequence.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    The index of the type which is equal to the passed predicate type.
 * \trace     CREQ-VaCommonLib-CompileTimeOperations
 */
template <typename X, typename... Xs>
constexpr inline auto get_index() noexcept -> std::size_t {
  constexpr ara::core::Array<bool, sizeof...(Xs)> const same{std::is_same<X, Xs>::value...};
  std::size_t res{std::numeric_limits<std::size_t>::max()};
  for (std::size_t i{0}; i < sizeof...(Xs); ++i) {
    if (same[i]) {
      res = i;
      break;
    }
  }
  return res;
}

/*!
 * \brief     Gets the index of a type implicitly convertible to type X in the list of passed types.
 * \tparam    X The first passed type.
 * \tparam    Xs The passed types in sequence.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    The index of the type which is implicitly convertible to the passed predicate type.
 * \trace     CREQ-VaCommonLib-CompileTimeOperations
 */
template <typename X, typename... Xs>
constexpr inline auto get_convertible_index() noexcept -> std::size_t {
  constexpr ara::core::Array<bool, sizeof...(Xs)> const convertible{
      (std::is_same<X, Xs>::value || std::is_convertible<X, Xs>::value)...};
  std::size_t res{std::numeric_limits<std::size_t>::max()};
  for (std::size_t i{0}; i < sizeof...(Xs); ++i) {
    if (convertible[i]) {
      res = i;
      break;
    }
  }
  return res;
}

/*!
 * \brief  Returns the type from the passed index.
 * \tparam N size_t type of input.
 * \tparam Xs The passed types in sequence.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <std::size_t N, typename... Xs>
using get_from_index = typename std::tuple_element<N, std::tuple<Xs...>>::type;

/*!
 * \brief  Checks if any generic parameters are duplicate.
 * \tparam Xs The passed types in sequence.
 * \trace  CREQ-VaCommonLib-CompileTimeOperations
 */
template <typename... Xs>
struct has_duplicate;
/*!
 * \brief   Checks if any generic parameters are duplicate.
 * \details Endpoint of recursion -> no duplicate found.
 * \tparam  X The passed type.
 */
template <typename X>
struct has_duplicate<X> : std::integral_constant<bool, false> {};
/*!
 * \brief   Checks if any generic parameters are duplicate.
 * \details Recursive implementation.
 * \tparam  X The passed type.
 * \tparam  Xs The passed types in sequence.
 */
template <typename X, typename... Xs>
struct has_duplicate<X, Xs...> : std::integral_constant<bool, (any_in<X, Xs...>() || has_duplicate<Xs...>::value)> {};

}  // namespace compile_time
}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_COMPILE_TIME_H_
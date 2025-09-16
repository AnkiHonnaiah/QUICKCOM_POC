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
/**     \file       combine.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_COMBINE_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_COMBINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include <utility>

#include "amsr/log/internal/log_error_domain.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief The value type of the return type of the combined value
 */
template <typename T>
using RetVal = typename std::result_of_t<T()>::value_type;

/*!
 * \brief   Combines two results into one

 * \tparam  Rs  A set of value types
 * \tparam  E   The common error type
 * \tparam  Fn  A result function type
 *
 * \param   res The first tuple-result to combine
 * \param   fn  The second result to combine
 *
 * \return  The combined result
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Fn, typename E, typename... Rs>
auto CombineTuple(Result<std::tuple<Rs...>, E>&& res, Fn&& fn) noexcept -> Result<std::tuple<Rs..., RetVal<Fn>>, E> {
  return std::move(res).AndThen([&fn](std::tuple<Rs...> r1_value) {
    using T2 = typename decltype(fn())::value_type;
    // VCA_LOGAPI_CHECKED_CONTRACT
    return std::forward<Fn>(fn)().Map([&r1_value](T2 r2_value) {
      // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      return std::tuple_cat(std::move(r1_value), std::make_tuple<T2>(std::move(r2_value)));
    });
  });
}

/*!
 * \brief   Combines two results into one
 *
 * \tparam  T1  A result value type.
 *              Must meet the requirements of amsr::core::Result<T1, E>.
 * \tparam  Fn  A result function type.
 * \tparam  E   The common error type
 *              Must meet the requirements of amsr::core::Result<T1, E>.
 *
 * \param   res The first result to combine
 * \param   fn  The second result to combine
 *
 * \return  The combined result
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T1, typename Fn, typename E>
auto Combine(Result<T1, E>&& res, Fn&& fn) noexcept -> Result<std::tuple<T1, RetVal<Fn>>, E> {
  return std::move(res).AndThen([&fn](T1 r1_value) {
    using T2 = typename decltype(fn())::value_type;
    // VCA_LOGAPI_CHECKED_CONTRACT
    return std::forward<Fn>(fn)().Map([&r1_value](T2 r2_value) {
      // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      return std::make_tuple<T1, T2>(std::move(r1_value), std::move(r2_value));
    });
  });
}

// VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1_false_positive_rvalue_overload
/*!
 * \brief   Combines three results into one
 *
 * \tparam  Res   A result value type
 * \tparam  Fn1   A result function type
 * \tparam  Fn2   Another result function type
 * \tparam  E     The common error type
 *
 * \param   res   The first tuple-result to combine
 * \param   fn1   The second result to combine
 * \param   fn2   The third result to combine
 *
 * \return  The combined result
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Res, typename Fn1, typename Fn2>
auto Combine(Res&& res, Fn1&& fn1, Fn2&& fn2) noexcept
    -> Result<std::tuple<typename Res::value_type, RetVal<Fn1>, RetVal<Fn2>>, typename Res::error_type> {
  return CombineTuple(Combine(std::forward<Res>(res), std::forward<Fn1>(fn1)), std::forward<Fn2>(fn2));
}

// VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1_false_positive_rvalue_overload
/*!
 * \brief   Combines four results into one
 *
 * \tparam  Res   A result value type
 * \tparam  Fn1   A result function type
 * \tparam  Fn2   Another result function type
 * \tparam  Fn3   Another result function type
 *
 * \param   res   The first tuple-result to combine
 * \param   fn1   The second result to combine
 * \param   fn2   The third result to combine
 * \param   fn3   The fourth result to combine
 *
 * \return  The combined result
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Res, typename Fn1, typename Fn2, typename Fn3>
auto Combine(Res&& res, Fn1&& fn1, Fn2&& fn2, Fn3&& fn3) noexcept
    -> Result<std::tuple<typename Res::value_type, RetVal<Fn1>, RetVal<Fn2>, RetVal<Fn3>>, typename Res::error_type> {
  return CombineTuple(Combine(std::forward<Res>(res), std::forward<Fn1>(fn1), std::forward<Fn2>(fn2)),
                      std::forward<Fn3>(fn3));
}

// VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1_false_positive_rvalue_overload
/*!
 * \brief   Combines five results into one
 *
 * \tparam  Res   A result value type
 * \tparam  Fn1   A result function type
 * \tparam  Fn2   Another result function type
 * \tparam  Fn3   Another result function type
 * \tparam  Fn4   Another result function type
 *
 * \param   res   The first tuple-result to combine
 * \param   fn1   The second result to combine
 * \param   fn2   The third result to combine
 * \param   fn3   The fourth result to combine
 * \param   fn4   The firth result to combine
 *
 * \return  The combined result
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Res, typename Fn1, typename Fn2, typename Fn3, typename Fn4>
auto Combine(Res&& res, Fn1&& fn1, Fn2&& fn2, Fn3&& fn3, Fn4&& fn4) noexcept
    -> Result<std::tuple<typename Res::value_type, RetVal<Fn1>, RetVal<Fn2>, RetVal<Fn3>, RetVal<Fn4>>,
              typename Res::error_type> {
  return CombineTuple(
      Combine(std::forward<Res>(res), std::forward<Fn1>(fn1), std::forward<Fn2>(fn2), std::forward<Fn3>(fn3)),
      std::forward<Fn4>(fn4));
}

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_COMBINE_H_

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
/**     \file       tuple.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TUPLE_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TUPLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <tuple>
#include <utility>

namespace amsr {
namespace log {
namespace internal {
namespace detail {
// VECTOR NC AutosarC++17_10-A12.8.3: MD_LOG_A12.8.3_false_positive_moved_from
/*!
 * \brief   Internal implementation for MakeFromTuple
 *
 * \tparam T        The type to construct.
 *                  Must be constructible from 0 to I:th elements of the tuple.
 * \tparam  Tuple   The tuple from which to construct.
 *                  Must be std::tuple that contains the types needed to construct T.
 *                  Tuple size shall be larger than 1.
 * \tparam  I       The number of indices
 *
 * \param   t       The values from which to construct from
 * \return  T constructed from the elements of the Tuple.
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, typename Tuple, std::size_t... I>
constexpr auto MakeFromTupleImpl(Tuple&& t, std::index_sequence<I...>) noexcept -> T {
  return T(std::get<I>(std::forward<Tuple>(t))...);
}
}  // namespace detail

/*!
 * \brief   Construct an object of type T using the elements from the tuple
 *
 * \tparam  T       The type to construct
 *                  Must be constructible from all the elements of the tuple.
 * \tparam  Tuple   Must meet the type requirements of Tuple of
 *                  amsr::log::internal::detail::MakeFromTupleImpl<T, Tuple, ... I>(Tuple&&, std::index_sequence<I...>)
 *
 * \param   t       The tuple to construct from
 *
 * \return  The constructed value
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, typename Tuple>
constexpr auto MakeFromTuple(Tuple&& t) noexcept -> T {
  return detail::MakeFromTupleImpl<T>(
      std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TUPLE_H_

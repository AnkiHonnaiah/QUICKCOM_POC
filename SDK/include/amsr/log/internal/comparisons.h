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
/**     \file       comparisons.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_COMPARISONS_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_COMPARISONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace log {
namespace internal {

/*!
 * \brief   Create a comparison function that compares addresses.
 *
 * \tparam  T     The type to compare.
 *
 * \param   cmp   The comparison value.
 *
 * \return  The function that can be used to compare.
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
auto IsSameAs(T const& cmp) noexcept -> decltype(auto) {
  return [&cmp](T const& cand) { return &cand == &cmp; };
}

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_COMPARISONS_H_

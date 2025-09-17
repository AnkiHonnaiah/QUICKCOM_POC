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
/*!        \file  ara/core/array.h
 *        \brief  SWS core type ara::core::Array.
 *         \unit  VaCommonLib::ContainerLibrary::StaticContainers::Array
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_ARRAY_H_
#define LIB_VAC_INCLUDE_ARA_CORE_ARRAY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <utility>

namespace ara {
namespace core {

/*!
 * \brief  Array class template for ara::core.
 * \tparam T Value type.
 * \tparam N std::size_t type.
 * \pre    -
 * \trace  SPEC-7552492
 * \vpublic
 */
template <typename T, std::size_t N>
using Array = std::array<T, N>;

// VECTOR Next Construct AutosarC++17_10-A2.11.4: MD_VAC_A2.11.4_nameOfObjectOrFunctionReused
// VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief         Overloaded std::swap function for ara::core::Array data type.
 * \tparam        T Value type.
 * \tparam        N std::size_t type.
 * \param[in,out] lhs Left hand side Array to swap.
 * \param[in,out] rhs Right hand side Array to swap.
 * \pre
 * \spec
 *   requires true;
 * \endspec     -
 * \trace         SPEC-7552493
 * \vpublic
 */
template <typename T, std::size_t N>
void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>()))) {
  lhs.swap(rhs);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
}

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_ARRAY_H_

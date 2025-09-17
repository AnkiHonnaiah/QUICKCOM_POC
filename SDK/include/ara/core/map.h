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
/*!        \file  ara/core/map.h
 *        \brief  SWS core type ara::core::Map.
 *         \unit  VaCommonLib::ContainerLibrary::DynamicContainers::Map
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_MAP_H_
#define LIB_VAC_INCLUDE_ARA_CORE_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <map>
#include <utility>
#include "ara/core/memory_resource.h"

namespace ara {
namespace core {
/*!
 * \brief Map data type that represents a container of key-value pair with unique keys.
 * \trace SPEC-7552502
 * \vpublic
 */
template <typename K, typename V, typename C = std::less<K>,
          typename Alloc = PolymorphicAllocator<std::pair<K const, V>>>
using Map = std::map<K, V, C, Alloc>;

// VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief         An overloaded std::swap function for ara::core::Map data type.
 * \tparam        K key for (key-value) pair for ara::core::Map data type.
 * \tparam        V Value for (key-value) pair for ara::core::Map data type.
 * \tparam        C Comparer for K (std::less<K>) for (key-value) pair for ara::core::Map data type.
 * \tparam        Alloc ara::core::PolymorphicAllocator for ara::core::Map data type.
 * \param[in,out] lhs Left hand side Map to swap.
 * \param[in,out] rhs Right hand side Map to swap.
 * \pre           -
 * \spec
 *   requires true;
 * \endspec
 * \trace         SPEC-7552503
 * \vpublic
 */
template <typename K, typename V, typename C = std::less<K>,
          typename Alloc = PolymorphicAllocator<std::pair<K const, V>>>
void swap(Map<K, V, C, Alloc>& lhs, Map<K, V, C, Alloc>& rhs) noexcept {
  std::swap(lhs, rhs);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
}

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_MAP_H_

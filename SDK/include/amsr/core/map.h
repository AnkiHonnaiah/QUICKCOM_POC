/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!    \file  amsr/core/map.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_MAP_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/map.h"
#include "ara/core/memory_resource.h"

namespace amsr {
namespace core {

/*!
 * \brief    Alias for ara::core::Map.
 */

template <typename K, typename V, typename C = std::less<K>,
          typename Alloc = ara::core::PolymorphicAllocator<std::pair<K const, V>>>
using Map = ara::core::Map<K, V, C, Alloc>;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief   Alias for ara::core::swap() for Map.
 */
using ara::core::swap;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_MAP_H_

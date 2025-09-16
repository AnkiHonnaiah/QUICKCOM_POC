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
/*!     \file amsr/core/utility.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_UTILITY_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_UTILITY_H_

#include "ara/core/utility.h"

namespace amsr {
namespace core {

/*!
 * \brief Alias for ara::core::in_place_t.
 */
using in_place_t = ara::core::in_place_t;

// VECTOR Next Construct AutosarC++17_10-M17.0.2: MD_VAC_M17.0.2_standardLibraryObjectNameOverride
/*!
 * \brief The singleton instance of in_place_t.
 */
constexpr in_place_t in_place{};

/*!
 * \brief  Alias for ara::core::in_place_type_t.
 */
template <typename T>
using in_place_type_t = ara::core::in_place_type_t<T>;

/*!
 * \brief  Alias for ara::core::in_place_index_t.
 */
template <std::size_t I>
using in_place_index_t = ara::core::in_place_index_t<I>;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::data().
 */
using ara::core::data;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::size().
 */
using ara::core::size;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::empty().
 */
using ara::core::empty;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_UTILITY_H_

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
/*!    \file  amsr/core/array.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_ARRAY_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_ARRAY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/array.h"

namespace amsr {
namespace core {

/*!
 * \brief  Alias for ara::core::Array.
 * \tparam T Value type.
 * \tparam N std::size_t type.
 */
template <typename T, std::size_t N>
using Array = ara::core::Array<T, N>;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::swap() for Array.
 */
using ara::core::swap;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_ARRAY_H_

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
/*!    \file  amsr/core/result.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_RESULT_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_RESULT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "ara/core/result.h"

namespace amsr {
namespace core {

/*!
 * \brief  Alias for ara::core::is_result.
 */
template <typename U>
using is_result = ara::core::is_result<U>;

/*!
 * \brief  Alias for ara::core::Result.
 * \tparam T The type of value.
 * \tparam E The type of error.
 */
template <typename T, typename E = ErrorCode>
using Result = ara::core::Result<T, E>;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::swap() for ara::core::Result.
 */
using ara::core::swap;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_RESULT_H_

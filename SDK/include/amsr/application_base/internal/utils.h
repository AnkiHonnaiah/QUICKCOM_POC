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
/**     \file       utils.h
 *      \brief      Utility.
 *
 *********************************************************************************************************************/
#ifndef LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_UTILS_H_
#define LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/error_code.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"

namespace amsr {
namespace application_base {
namespace internal {

/*!
 * \brief Alias to ara::core::ErrorCode
 */
using ErrorCode = ara::core::ErrorCode;

/*!
 * \brief Alias to ara::core::Result
 */
template <typename T, typename E = ErrorCode>
using Result = ara::core::Result<T, E>;

/*!
 * \brief Alias to ara::core::Optional.
 */
template <typename T>
using Optional = ara::core::Optional<T>;

}  // namespace internal
}  // namespace application_base
}  // namespace amsr
#endif  // LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_UTILS_H_
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
/*!    \file  amsr/core/error_code.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_ERROR_CODE_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_ERROR_CODE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/error_code.h"

namespace amsr {
namespace core {

/*!
 * \brief  Alias for ara::core::ErrorCode.
 * \vpublic
 */
using ErrorCode = ara::core::ErrorCode;
}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_ERROR_CODE_H_

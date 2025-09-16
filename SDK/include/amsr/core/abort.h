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
/*!        \file  amsr/core/abort.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_ABORT_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_ABORT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/abort.h"

namespace amsr {
namespace core {

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::Abort().
 */
using ara::core::Abort;

/*!
 * \brief  Alias for ara::core::AbortHandler().
 */
using AbortHandler = ara::core::AbortHandler;

// VECTOR Next Construct AutosarC++17_10-M7.3.6: MD_VAC_M7.3.6_usingDeclarationInHeaderFile
/*!
 * \brief  Alias for ara::core::SetAbortHandler().
 */
using ara::core::SetAbortHandler;

}  // namespace core
}  // namespace amsr
#endif  // LIB_VAC_INCLUDE_AMSR_CORE_ABORT_H_

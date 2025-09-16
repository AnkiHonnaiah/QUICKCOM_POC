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
/*!    \file  amsr/core/exception.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_EXCEPTION_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_EXCEPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/exception.h"

namespace amsr {
namespace core {

/*!
 * \brief Alias for ara::core::Exception.
 */
using Exception = ara::core::Exception;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_EXCEPTION_H_

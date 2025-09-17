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
/*!     \file   amsr/core/future.h
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_CORE_FUTURE_H_
#define LIB_VAC_INCLUDE_AMSR_CORE_FUTURE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "ara/core/future.h"

namespace amsr {
namespace core {

/*!
 * \brief Alias for ara::core::future_status.
 */
using future_status = ara::core::future_status;

/*!
 * \brief  Alias for ara::core::Future.
 * \tparam T Value type
 * \tparam E Error type
 */
template <typename T, typename E = ErrorCode>
using Future = ara::core::Future<T, E>;

}  // namespace core
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_CORE_FUTURE_H_

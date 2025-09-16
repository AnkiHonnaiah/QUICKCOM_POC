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
/*!        \file
 *        \brief  Provide basic thread functionality.
 *        \unit   amsr::thread::ConfigurableThreads::BaseThread
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_BASE_THREAD_ERROR_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_BASE_THREAD_ERROR_H_

#include <sys/types.h>
#include <utility>

#include "amsr/core/error_code.h"

namespace amsr {
namespace thread {
namespace internal {

/*!
 * \brief Map errors occurring during the static object construction "Create" to their respective error code.
 * \param[in] ret_val The return value from initializing and creating a BaseThread.
 * \return          The mapped error code.
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
// VECTOR Next Construct AutosarC++17_10-A3.9.1:MD_THREAD_A3.9.1
amsr::core::ErrorCode MapToError(int ret_val) noexcept;

}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_BASE_THREAD_ERROR_H_

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
 *        \brief  Provides functionality to set name of thread.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_POSIX_API_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_POSIX_API_H_

#include "amsr/core/result.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace thread {
namespace internal {

/*!
 * \brief Set the name for the given thread.
 * \param[in] thread_handle  Handle of the thread to whom a name should be set.
 * \param[in] thread_name    The name for the thread.
 * \return An empty result if successful.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \error ThreadErrc::kThreadNameOutOfRange       The given name exceeds the defined limit (kMaxSizeThreadName).
 * \error ThreadErrc::kNonPosixError              Setting the name caused a non POSIX specified error. Support data with
 * exact error code is provided.
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \trace               DSGN-VectorThreadLibrary-ThisThread
 * \vprivate Vector component internal API
 */
amsr::core::Result<void> SetNameOfThread(pthread_t thread_handle,
                                         vac::container::CStringView const& thread_name) noexcept;

}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_POSIX_API_H_

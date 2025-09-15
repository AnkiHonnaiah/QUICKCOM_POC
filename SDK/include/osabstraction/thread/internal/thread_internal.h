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
/*!        \file  thread_internal.h
 *        \brief  Provides a function to convert a platform specific error for setting the thread name
 *                to a platform independant error.
 *
 *********************************************************************************************************************/
#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_INTERNAL_THREAD_INTERNAL_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_INTERNAL_THREAD_INTERNAL_H_

#include "amsr/core/error_code.h"

namespace osabstraction {
namespace thread {
namespace internal {

/*!
 * \brief           Gets a platform independent error code for the given error.
 *
 * \param           os_error platform specific error
 *
 * \context         SetNameOfThread()
 *
 * \return          The platform independent error code.
 *
 * \retval osabstraction::OsabErrc::kApiError               The platform specific error indicates that the API was not
 *                                                          used correctly.
 *
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError The platform specific error indicates that the system
 *                                                          environment is not in the expected state. The support data
 *                                                          contains the platform specific error.
 *
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges Lacking privileges to set name.
 *
 * \reentrant         FALSE
 * \synchronous       TRUE
 * \threadsafe        FALSE
 *
 * \vprivate        Vector component internal API
 */
::amsr::core::ErrorCode GetErrorCodeForSetName(std::int32_t os_error) noexcept;

}  // namespace internal
}  // namespace thread
}  // namespace osabstraction

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_THREAD_INTERNAL_THREAD_INTERNAL_H_

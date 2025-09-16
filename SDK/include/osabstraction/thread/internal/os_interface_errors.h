/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Maps the internal errors for the operating system interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_THREAD_INTERNAL_OS_INTERFACE_ERRORS_H_
#define LIB_UTILS_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_THREAD_INTERNAL_OS_INTERFACE_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "osabstraction/internal/errors.h"

namespace osabstraction {
namespace thread {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps pthread_getname_np error number to the osabstraction error.
 *
 * \param[in] os_error  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kSize                     The buffer length is too small to store the thread
 *                                                            name.
 *
 * \error           -
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
::amsr::core::ErrorCode MapGetThreadNameError(osabstraction::internal::OsErrorNumber os_error) noexcept;

}  // namespace os_interface
}  // namespace internal
}  // namespace thread
}  // namespace osabstraction

#endif  // LIB_UTILS_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_THREAD_INTERNAL_OS_INTERFACE_ERRORS_H_

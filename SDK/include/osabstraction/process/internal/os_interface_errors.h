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
 *        \brief  Maps the internal errors for the operating system interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_OS_INTERFACE_ERRORS_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_OS_INTERFACE_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "osabstraction/internal/errors.h"

namespace osabstraction {
namespace process {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps waitpid error number to the osabstraction error.
 *
 * \param[in] os_error  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kDoesNotExist             Calling process does not have child processes.
 * \retval osabstraction::OsabErrc::kBusy                     Function was interrupted by a signal.
 *
 * \error           -
 *
 * \context         osabstraction::process::WaitForChildTermination()
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
auto MapWaitpidError(osabstraction::internal::OsErrorNumber os_error) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps realpath error number to the osabstraction error.
 *
 * \param[in] os_error  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kProcessCreationFailed    Process creation failed.
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 *
 * \error           -
 *
 * \context         osabstraction::process::CreateProcess()
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
auto MapRealpathError(osabstraction::internal::OsErrorNumber os_error) noexcept -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps fork error number to the osabstraction error.
 *
 * \param[in] os_error  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kProcessCreationFailed    Process creation failed.
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no category.
 *
 * \error           -
 *
 * \context         osabstraction::process::CreateProcess()
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
auto MapForkError(osabstraction::internal::OsErrorNumber os_error) noexcept -> ::amsr::core::ErrorCode;

}  // namespace os_interface
}  // namespace internal
}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_OS_INTERFACE_ERRORS_H_

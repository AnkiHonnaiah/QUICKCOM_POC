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
 *  -------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Maps the internal errors for the envvar related operating system interfaces.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENVVAR_INTERNAL_OS_INTERFACE_ERRORS_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENVVAR_INTERNAL_OS_INTERFACE_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "osabstraction/internal/errors.h"

namespace amsr {
namespace envvar {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps setenv error number to the osabstraction error.
 *
 * \param[in] os_error  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected  Unexpected error that is mapped to no category.
 * \retval osabstraction::OsabErrc::kApiError    The envar name is a null pointer, points to an empty
 *                                               string, or points to a string containing an '=' character.
 * \retval osabstraction::OsabErrc::kResource    Insufficient memory was available to add a variable or its
 *                                               value to the environment.
 *
 * \error           -
 *
 * \context         amsr::envvar::SetEnvironmentVariable()
 *
 * \pre             -
 *
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \vprivate        Vector component internal API
 */
auto MapSetEnvironmentVariableError(osabstraction::internal::OsErrorNumber os_error) noexcept
    -> ::amsr::core::ErrorCode;

}  // namespace os_interface
}  // namespace internal
}  // namespace envvar
}  // namespace amsr

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENVVAR_INTERNAL_OS_INTERFACE_ERRORS_H_

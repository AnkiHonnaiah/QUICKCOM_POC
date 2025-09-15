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
 *        \brief  Provides error mappings for POSIX operating system APIs used in memory
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_POSIX_OS_API_ERRORS_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_POSIX_OS_API_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"

namespace amsr {
namespace memory {
namespace internal {
namespace os_interface {

/*!
 * \brief Maps error of fstat call to the osabstraction error domain.
 *
 * \param[in] number  Error number.
 *
 * \retval osabstraction::OsabErrc::kUnexpected              Unexpected error while determining the file size.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError  SHM object size is too big for this process or the  system
 *                                                           does not support the fstat call or a file system I/O error
 *                                                           occurred.
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 *
 * \spec
 *   requires true;
 * \endspec
 */
auto MapGetFileSizeError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

}  // namespace os_interface
}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_MEMORY_INTERNAL_OS_INTERFACE_POSIX_OS_API_ERRORS_H_

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
 *        \brief  ResourceUsage POSIX utilities.
 *        \unit   osabstraction::resource_usage
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_RESOURCEUSAGE_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_POSIX_UTILITIES_H_
#define LIB_RESOURCEUSAGE_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_POSIX_UTILITIES_H_

#include "amsr/core/result.h"
#include "amsr/core/string.h"

#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace resource_usage {
namespace internal {

/*!
 * \brief           Checks that input string is a number.
 *
 * \param[in]       input_string Input string to be checked. It has to represent positive number.
 *
 * \return          True if input string is number. False otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
bool IsNumber(::amsr::core::String const& input_string) noexcept;

/*!
 * \brief           Opens a file and return file descriptor.
 *
 * \param[in]       file  amsr::core::String file name to open.
 *
 * \return          file descriptor
 * \error           osabstraction::OsabErrc::kUnexpected                 Unexpected error
 * \error           osabstraction::OsabErrc::kInsufficientPrivileges     Permissions denied
 * \error           osabstraction::OsabErrc::kDoesNotExist               File does not exist
 * \error           osabstraction::OsabErrc::kInvalidHandle              File path is too long
 * \error           osabstraction::OsabErrc::kResource                   Insufficient memory
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError     Requested operation is invalid or not supported
 *                                                                       or operation failed because of a hardware error
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
::amsr::core::Result<osabstraction::io::FileDescriptor> OpenFileDescriptor(::amsr::core::String const& file) noexcept;

}  // namespace internal
}  // namespace resource_usage
}  // namespace amsr

#endif  // LIB_RESOURCEUSAGE_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_RESOURCE_USAGE_INTERNAL_POSIX_UTILITIES_H_

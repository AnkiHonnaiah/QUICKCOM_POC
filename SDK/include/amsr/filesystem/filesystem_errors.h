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
/*!        \file  filesystem_errors.h
 *        \brief  Provides error mappings for FileSystem errors.
 *        \unit   osabstraction::filesystem_utils
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_FILESYSTEM_ERRORS_H_
#define LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_FILESYSTEM_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"

namespace amsr {
namespace filesystem {

/*!
 * \brief Handles errors returned by closedir.
 *
 * \param[in] number  Error number.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 */
void HandleCloseDirErrors(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps OsError to osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error during determination of the properties.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Not allowed to acess the file system object.
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot read from filesystem:
                                                              - Systemcall could not be performed
                                                              - Path argument exceeds system limit on filepath
                                                              - Reading Error
                                                              - Too many symbolic links or prefixes
 * \retval osabstraction::OsabErrc::DoesNotExist              Path does not exist or is empty string
 * \retval osabstraction::OsabErrc::kResource                 Not enough system resources.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 *
 * \trace DSGN-Osab-Filesystem-FileStatus
 */
auto MapFileStatusErrors(osabstraction::internal::OsErrorNumber number) -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps OsError to osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Permission denied
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot open directory stream
 * \retval osabstraction::OsabErrc::kDoesNotExist             Path does not exist or is empty string
 * \retval osabstraction::OsabErrc::kResource                 Insufficient Resources
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 *
 * \trace DSGN-Osab-Filesystem-DirectoryIterator
 */
auto MapCreateIterErrors(osabstraction::internal::OsErrorNumber number) -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps OsError to osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot read directory stream
 * \retval osabstraction::OsabErrc::kDoesNotExist             Position in directory stream inlvalid
 * \retval osabstraction::OsabErrc::kResource                 Insufficient Resources
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 *
 * \trace DSGN-Osab-Filesystem-DirectoryIterator
 */
auto MapDirectoryIterationErrors(osabstraction::internal::OsErrorNumber number) -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps OsError to osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Permission denied
 * \retval osabstraction::OsabErrc::kAlreadyExists            File system object already exists
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot make directory:
                                                              - A loop of symblic links exists
                                                              - Input exceeds system limit of symbolic links
                                                              - Input resides on read-only filesystem
                                                              - Input exceeds system limit on filepath
 * \retval osabstraction::OsabErrc::kDoesNotExist             Path does not exist or is empty string
 * \retval osabstraction::OsabErrc::kResource                 Insufficient resources
 *
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 *
 * \trace DSGN-Osab-Filesystem-Create
 */
auto MapMkDirErrors(osabstraction::internal::OsErrorNumber number) -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps OsError to osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Permission denied
 * \retval osabstraction::OsabErrc::kAlreadyExists            File system object already exists
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot create file:
                                                              - A loop of symblic links exists
                                                              - Input resides on read-only filesystem
                                                              - Input exceeds system limit on filepath
                                                              - Error while reading from filesystem
 * \retval osabstraction::OsabErrc::kDoesNotExist             Path does not exist or is empty string
 * \retval osabstraction::OsabErrc::kResource                 Insufficient resources
 * \retval osabstraction::OsabErrc::kApiError                 Path specifies directory
 *
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 *
 * \trace DSGN-Osab-Filesystem-Create
 */
auto MapFileCreationErrors(osabstraction::internal::OsErrorNumber number) -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps OsError to osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Permission denied
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot remove:
                                                              - A loop of symblic links exists
                                                              - Input resides on read-only filesystem
                                                              - Input exceeds system limit on filepath
                                                              - Filesystem object busy
                                                              - Directory not empty
 * \retval osabstraction::OsabErrc::kDoesNotExist             Path does not exist or is empty string
 * \retval osabstraction::OsabErrc::kResource                 Insufficient resources
 *
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 *
 * \trace DSGN-Osab-Filesystem-Remove
 */
auto MapRemoveErrors(osabstraction::internal::OsErrorNumber number) -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps OsError to osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Permission denied
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot rename:
                                                              - A loop of symblic links exists
                                                              - Input resides on read-only filesystem
                                                              - Input exceeds system limit on filepath
                                                              - Input exceed system limit of symbolic links
                                                              - Filesystem object busy
                                                              - New directory not empty
                                                              - Old and new are on different filesystem
 * \retval osabstraction::OsabErrc::kDoesNotExist             Path does not exist or is empty string
 * \retval osabstraction::OsabErrc::kApiError                 Old and new are not the same type
 * \retval osabstraction::OsabErrc::kResource                 Insufficient resources
 *
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 *
 * \trace DSGN-Osab-Filesystem-Rename
 */
auto MapRenameErrors(osabstraction::internal::OsErrorNumber number) -> ::amsr::core::ErrorCode;

/*!
 * \brief Maps OsError to osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   Permission denied
 * \retval osabstraction::OsabErrc::kSystemEnvironmentError   Cannot change permissions:
                                                              - A loop of symblic links exists
                                                              - Input resides on read-only filesystem
                                                              - Input exceeds system limit on filepath
 * \retval osabstraction::OsabErrc::kDoesNotExist             Path does not exist or is empty string
 * \retval osabstraction::OsabErrc::kResource                 Insufficient resources
 *
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate Vector component internal API
 *
 * \trace DSGN-Osab-Filesystem-SetPermissions
 */
auto MapChmodErrors(osabstraction::internal::OsErrorNumber number) -> ::amsr::core::ErrorCode;

}  // namespace filesystem
}  // namespace amsr

#endif  // LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_FILESYSTEM_ERRORS_H_

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
/**        \file  filesystem_interface.h
 *        \brief  API functions for filesystem related functionality.
 *         \unit  osabstraction::filesystem_utils
 *   \complexity  filesystem_utils unit is QM, no action required.
 *
 *********************************************************************************************************************/

#ifndef LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_FILESYSTEM_INTERFACE_H_
#define LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_FILESYSTEM_INTERFACE_H_

#include "amsr/core/result.h"
#include "amsr/filesystem/path.h"
#include "amsr/filesystem/types.h"

namespace amsr {
namespace filesystem {

/*!
 * \brief       Get the information of a filesystem object by its path.
 *
 * \param[in]   path  The path to the filesystem object whose information is to be returned.
 *
 * \return      Information about the filesystem object.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Reading from filesystem failed or
 *                                                                operation not supported for this filesystem or
 *                                                                path length exceeds system limit or
 *                                                                too many levels of symbolic links or
 *                                                                [QNX] function temporary unavailable.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string.
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-FileStatus
 */
auto Status(amsr::filesystem::Path const& path) -> ::amsr::core::Result<FileStatus>;

/*!
 * \brief       Get the size of a filesystem object by its path.
 *
 * \param[in]   path  The path to the filesystem object whose size is to be returned.
 *
 * \return      Size of the object in bytes.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Reading from filesystem failed or
 *                                                                operation not supported for this filesystem or
 *                                                                path length exceeds system limit or
 *                                                                too many levels of symbolic links or
 *                                                                [QNX] function temporary unavailable.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-FileStatus
 */
auto FileSize(amsr::filesystem::Path const& path) -> ::amsr::core::Result<uint64_t>;

/*!
 * \brief       Returns whether a filesystem object exists.
 *
 * \param[in]   path  The path to the filesystem object.
 *
 * \return      true if the object exists, false otherwise.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Reading from filesystem failed or
 *                                                                operation not supported for this filesystem or
 *                                                                path length exceeds system limit or
 *                                                                too many levels of symbolic links or
 *                                                                [QNX] function temporary unavailable.
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-FileStatus
 */
auto Exists(amsr::filesystem::Path const& path) -> ::amsr::core::Result<bool>;

/*!
 * \brief       Create file specified in the path. Optionally permissions can be provided to use for the new file.
 *
 * \details     By default the created file has read, write permissions for all permission groups.
 *              The current umask of the process affects the permissions of the created file and may result in fewer
 *              permissions being set. If it is required that all given permissions are set, SetPermissions should be
 *              called afterwards.
 *
 * \param[in]   path   The path of the file to be created.
 * \param[in]   perms  The permissions the file should have.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Reading from filesystem failed or
 *                                                                directory resides in a read-only filesystem or
 *                                                                path length exceeds system limit or
 *                                                                a loop exists in symbolic links.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string.
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 * \error osabstraction::OsabErrc::kApiError                      Path is a directory.
 * \error osabstraction::OsabErrc::kAlreadyExists                 Object already exists.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-Create
 */

auto CreateFile(amsr::filesystem::Path const& path,
                amsr::filesystem::Permission const& perms = kPermOwnerRead | kPermOwnerWrite | kPermGroupRead |
                                                            kPermGroupWrite | kPermOthersRead | kPermOthersWrite)
    -> ::amsr::core::Result<void>;

/*!
 * \brief       Create a directory specified in the path. Optionally permissions can be provided to use for the new
 *              directory.
 *
 * \details     Only the last component of the provided path is considered as the new directory name and created.
 *              If the path contains multiple path components and one of the directories is missing up to
 *              the last it will be considered as an error. If it is not desired use CreateDirectories API.
 *              By default the created directory has read, write, execute permissions for all permission groups.
 *              The current umask of the process affects the permissions of the created file and may result in fewer
 *              permissions being set. If it is required that all given permissions are set, SetPermissions should be
 *              called afterwards.
 *
 * \param[in]   path   The path of the directory to be created.
 * \param[in]   perms  The permissions the directory should have.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Directory resides in a read-only filesystem or
 *                                                                path length exceeds system limit or
 *                                                                a loop exists in symbolic links.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string.
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 * \error osabstraction::OsabErrc::kAlreadyExists                 Object already exists.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-Create
 */
auto CreateDirectory(amsr::filesystem::Path const& path, amsr::filesystem::Permission const& perms = kPermAll)
    -> ::amsr::core::Result<void>;

/*!
 * \brief       Create a directory for each element specified in the path. Optionally permissions can be provided
 *              to use for the new directories.
 *
 * \details     All non-existing pre-directories will also be created.
 *              By default the created directories have read, write, execute permissions for all permission groups.
 *              The current umask of the process affects the permissions of the created file and may result in fewer
 *              permissions being set. If it is required that all given permissions are set, SetPermissions should be
 *              called afterwards.
 *
 * \param[in]   path   The path of the directory to be created.
 * \param[in]   perms  The permissions the directories should have.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Directory resides in a read-only filesystem or
 *                                                                path length exceeds system limit or
 *                                                                a loop exists in symbolic links.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 * \error osabstraction::OsabErrc::kAlreadyExists                 Object already exists
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      FALSE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-Create
 */
auto CreateDirectories(amsr::filesystem::Path const& path, amsr::filesystem::Permission const& perms = kPermAll)
    -> ::amsr::core::Result<void>;

/*!
 * \brief       Remove a filesystem object specified in the path.
 *
 * \param[in]   path   The path of the filesystem object to be removed.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path or
 *                                                                not allowed to remove the filesystem object.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Filesystem object to remove is non-empty directory or
 *                                                                reading from filesystem failed or
 *                                                                object resides in a read-only filesystem or
 *                                                                filesystem object is busy or
 *                                                                path length exceeds system limit or
 *                                                                a loop exists in symbolic links.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string.
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-Remove
 */
auto Remove(amsr::filesystem::Path const& path) -> ::amsr::core::Result<void>;

/*!
 * \brief      Remove a directory specified in the path with its immediate children.
 *
 * \details    If the filesystem object specified in the path is not a directory the behavior is the same as in
 *             Remove() API.
 *
 * \param[in]   path   The path of the filesystem object to be removed.
 *
 * \return      The number of files and directories that were deleted.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path or
 *                                                                not allowed to remove the filesystem object.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Filesystem object to remove is non-empty directory or
 *                                                                reading from filesystem failed or
 *                                                                object resides in a read-only filesystem or
 *                                                                filesystem object is busy or
 *                                                                path length exceeds system limit or
 *                                                                a loop exists in symbolic links.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      FALSE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-Remove
 */
auto RemoveAll(amsr::filesystem::Path const& path) -> ::amsr::core::Result<size_t>;

/*!
 * \brief      Rename a filesystem object. Replaces the target when it exists.
 *
 * \details    If old path is a file, the new path must be either non-existent or existent non-directory.
 *             The existent file will be replaced.
 *             If old path is a directory, the new path must be either non-existent or must be an empty directory.
 *             Empty directory will be replaced.
 *             If the operation fails when new path exists it is left intact.
 *
 * \param[in]   oldp   The path of the filesystem object to be renamed.
 * \param[in]   newp   The new path.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path or
 *                                                                not allowed to change the filesystem object.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Filesystem object to replace is non-empty directory or
 *                                                                reading from filesystem failed or
 *                                                                directory resides in a read-only filesystem or
 *                                                                filesystem object is busy or
 *                                                                path length exceeds system limit or
 *                                                                a loop exists in symbolic links.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string.
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-Rename
 */
auto Rename(amsr::filesystem::Path const& oldp, amsr::filesystem::Path const& newp) -> ::amsr::core::Result<void>;

/*!
 * \brief      Set permissions of filesystem object specified by its path
 *
 * \param[in]   path   The path of the filesystem object.
 * \param[in]   perms  New permissions to set for the object.
 *
 * \error osabstraction::OsabErrc::kUnexpected                    Unexpected error that is mapped to no category.
 * \error osabstraction::OsabErrc::kInsufficientPrivileges        Not allowed to access the given path or
 *                                                                not allowed to modify the permissions.
 * \error osabstraction::OsabErrc::kSystemEnvironmentError        Directory resides in a read-only filesystem or
 *                                                                path length exceeds system limit or
 *                                                                a loop exists in symbolic links.
 * \error osabstraction::OsabErrc::kDoesNotExist                  Path does not exist or is empty string
 * \error osabstraction::OsabErrc::kResource                      Not enough system resources.
 *
 * \pre         -
 *
 * \context     ANY
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-Osab-Filesystem-SetPermissions
 */
auto SetPermissions(amsr::filesystem::Path const& path, amsr::filesystem::Permission const& perms)
    -> ::amsr::core::Result<void>;

}  // namespace filesystem
}  // namespace amsr

#endif  // LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_FILESYSTEM_INTERFACE_H_

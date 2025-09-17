/**********************************************************************************************************************
 *  COPYRIGHT
 *
 -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *
 -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  directory_entry.h
 *        \brief  DirectoryEntry Class
 *        \unit   osabstraction::filesystem_utils
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ENTRY_H_
#define LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/core/result.h"
#include "amsr/filesystem/filesystem_interface.h"
#include "amsr/filesystem/path.h"
#include "amsr/filesystem/types.h"

namespace amsr {
namespace filesystem {
/*!
 * \brief Representation of an entry
 *
 * \vprivate    Vector product internal API
 *
 * \trace       DSGN-Osab-Filesystem-DirectoryIterator
 */
class DirectoryEntry final {
 public:
  /*!
   * \brief Construct a new Entry from a path.
   *
   * \param       path  Path.
   * \param       type  Entry type.
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  explicit DirectoryEntry(Path path, FileType type) noexcept : path_{std::move(path)}, type_{type} {}

  /*!
   * \brief       Get path of the entry
   *
   * \return      The path of the entry
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetPath() const noexcept -> Path;

  /*!
   * \brief       Return if the entry exists
   *
   * \return      true if exists, false otherwise
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto Exists() const noexcept -> bool;

  /*!
   * \brief       Return if the entry is a directory
   *
   * \return      true if directory, false otherwise
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsDirectory() const noexcept -> bool;

  /*!
   * \brief       Return if the entry is other
   *
   * \return      true if not a file and not a directory, false otherwise
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsOther() const noexcept -> bool;

  /*!
   * \brief       Return if the entry is a regular file
   *
   * \return      true if regular file, false otherwise
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsRegularFile() const noexcept -> bool;

  /*!
   * \brief       Return the size of the directory entry.
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetFileSize() const noexcept -> ::amsr::core::Result<uint64_t>;

  /*!
   * \brief Update the cached object information from the filesystem.
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto Refresh() -> ::amsr::core::Result<void>;

  /*!
   * \brief       Get the status of the entry
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
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetStatus() const noexcept -> ::amsr::core::Result<FileStatus>;

 private:
  /*!
   * \brief Path of the entry
   */
  Path path_;

  /*!
   * \brief FileType of the entry
   */
  FileType type_;
};

}  // namespace filesystem
}  // namespace amsr

#endif  // LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ENTRY_H_

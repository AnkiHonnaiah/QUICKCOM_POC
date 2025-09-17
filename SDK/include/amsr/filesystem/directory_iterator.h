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
/*!        \file  directory_iterator.h
 *        \brief  Provides an Iterator for directory entries.
 *        \unit   osabstraction::filesystem_utils
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ITERATOR_H_
#define LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ITERATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <dirent.h>
#include <utility>
#include "amsr/core/result.h"
#include "amsr/filesystem/directory_entry.h"
#include "amsr/filesystem/filesystem_errors.h"
#include "amsr/filesystem/path.h"
#include "amsr/filesystem/types.h"

namespace amsr {
namespace filesystem {

/*!
 * \brief Class to iterate over directory entries.
 *
 * \vprivate    Vector product internal API
 *
 * \trace       DSGN-Osab-Filesystem-DirectoryIterator
 */
class DirectoryIterator final {
 public:
  /*!
   * \brief Constructs the End Iterator.
   *
   * \steady     TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  DirectoryIterator() noexcept = default;

  /*!
   * \brief   Define move constructor.
   *
   * \param           other     Object to move from.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  DirectoryIterator(DirectoryIterator&& other) noexcept
      : dir_ptr_{other.dir_ptr_}, cur_direntry_ptr_{other.cur_direntry_ptr_}, path_{std::move(other.path_)} {
    other.dir_ptr_ = nullptr;
    other.cur_direntry_ptr_ = nullptr;
  }

  /*!
   * \brief   Define move assignment operator.
   *
   * \param           other     Object to move from.
   *
   * \return          Reference to the object.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(DirectoryIterator&& other) & noexcept -> DirectoryIterator& {
    if (dir_ptr_ != nullptr) {
      if (::closedir(dir_ptr_) == -1) {
        HandleCloseDirErrors(osabstraction::internal::GetErrorNumber());
      }
    }

    dir_ptr_ = other.dir_ptr_;
    cur_direntry_ptr_ = other.cur_direntry_ptr_;
    path_ = std::move(other.path_);
    other.dir_ptr_ = nullptr;
    other.cur_direntry_ptr_ = nullptr;
    return *this;
  }

  // Prevent copying
  DirectoryIterator(DirectoryIterator const&) noexcept = delete;

  // Prevent copying
  auto operator=(DirectoryIterator const&) & noexcept -> DirectoryIterator& = delete;

  /*!
   * \brief   Destructor.
   *
   * \param           -
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ~DirectoryIterator() noexcept;

  /*!
   * \brief           Returns an Iterator to an Entry in the root path.
   *
   * \param           root_path Path to the root directory
   *
   * \return          DirectoryIterator.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error
   * \error osabstraction::OsabErrc::kInsufficientPrivileges     Permissions denied.
   * \error osabstraction::OsabErrc::kSystemEnvironment          Operation is not supported or
   *                                                             path length exceeds system limit or
   *                                                             a loop exists in symbolic links.
   * \error osabstraction::OsabErrc::kDoesNotExist               Path does not exist or is an empty string or
   *                                                             path is not a directory.
   * \error osabstraction::OsabErrc::kResource                   Insufficient memory
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  static auto Create(Path const& root_path) noexcept -> ::amsr::core::Result<DirectoryIterator>;

  /*!
   * \brief           Increments the Iterator.
   *
   * \param           -
   *
   * \return          a reference to the iterator.
   *
   * \context         ANY
   *
   * \pre             Must not be the end iterator.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   */
  auto operator++() noexcept -> DirectoryIterator&;

  /*!
   * \brief           Dereferences the iterator to its value
   * \details         The Entry object will be constructed when dereferenced
   *
   * \param           -
   *
   * \return          Entry of the current iteration.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator*() const noexcept -> DirectoryEntry;

  /*!
   * \brief           Compares iterators for equality.
   *
   * \param           rhs Right side of the comparison.
   *
   * \return          true if equal, false otherwise
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator==(DirectoryIterator const& rhs) const noexcept -> bool;

  /*!
   * \brief           Compares iterators for inequality.
   *
   * \param           rhs Right side of the comparison.
   *
   * \return          true if inequal, false otherwise
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator!=(DirectoryIterator const& rhs) const noexcept -> bool;

  /*!
   * \brief           Return Iterator to the first entry.
   *
   * \param           -
   *
   * \return          Iterator to the first entry if successful. The end iterator if creating the iterator fails.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto begin() const noexcept -> DirectoryIterator;

  /*!
   * \brief           Return iterator after the last entry.
   *
   * \param           -
   *
   * \return          The end iterator
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  static auto end() noexcept -> DirectoryIterator;

  /*!
   * \brief           Increments the Iterator
   *
   * \param           -
   *
   * \return          Reference to the iterator or error.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error
   * \error osabstraction::OsabErrc::kDoesNotExist               Entry does not exist
   * \error osabstraction::OsabErrc::kResource                   Insufficient memory
   * \error osabstraction::OsabErrc::kSystemEnvironment          No open directory stream
   *
   * \context         ANY
   *
   * \pre             Must not be the end iterator.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  auto Increment() noexcept -> ::amsr::core::Result<DirectoryIterator&>;

 private:
  /*!
   * \brief Internal c-Tor for Create-Function
   */
  DirectoryIterator(Path path, DIR* dir) noexcept : dir_ptr_{dir}, path_{std::move(path)} {
    static_cast<void>(Increment());  // sets cur_direntry_ptr_
  }

  /*!
   * \brief Directory Stream
   */
  DIR* dir_ptr_{nullptr};

  /*!
   * \brief Current Entry
   */
  dirent* cur_direntry_ptr_{nullptr};

  /*!
   * \brief Path
   */
  Path path_;
};
}  // namespace filesystem
}  // namespace amsr

#endif  // LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ITERATOR_H_

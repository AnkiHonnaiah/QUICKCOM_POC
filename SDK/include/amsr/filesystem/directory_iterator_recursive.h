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
/*!        \file
 *        \brief  Provides an Recursive Iterator for DirectoryEntries.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ITERATOR_RECURSIVE_H_
#define LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ITERATOR_RECURSIVE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/filesystem/directory_iterator.h"
#include "ara/core/vector.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace filesystem {

/*!
 * \brief Type that allows to skip iteration of directories when the priviledges of the iterator are unsifficient.
 */
enum class DirectoryIterationOptions : uint8_t {
  kNone, /*!<  The skip is not allowed, the iterator will fail in case its priviledges are insufficient. */
  kSkipPermissionDenied /*!<  The iterator will skip the directory in case its priviledges are insufficient. */
};

/*!
 * \brief Class for recursive iteration over directory entries.
 */
class RecursiveDirectoryIterator final {
 public:
  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_DelegateConstructor
  /*!
   * \brief           Default constructor.
   *
   * \details         Constructs the end iterator.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  RecursiveDirectoryIterator() noexcept
      : props_(std::make_shared<RecursiveIteratorProps>(RecursiveIteratorProps(DirectoryIterationOptions::kNone))) {}

  /*!
   * \brief           Copy constructor.
   *
   * \param           other     RecursiveDirectoryIterator to copy-construct from.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  RecursiveDirectoryIterator(RecursiveDirectoryIterator const& other) noexcept = default;

  /*!
   * \brief Copy assign C-tor
   *
   * \param           other     RecursiveDirectoryIterator to copy-assign from.
   *
   * \return          Reference to this object.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto operator=(RecursiveDirectoryIterator const& other) & noexcept -> RecursiveDirectoryIterator& = default;

  /*!
   * \brief           Destructor
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  ~RecursiveDirectoryIterator() noexcept = default;

  /*!
   * \brief           Move constructor.
   *
   * \param           other     RecursiveDirectoryIterator to move-construct from.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  RecursiveDirectoryIterator(RecursiveDirectoryIterator&& other) noexcept : props_(std::move(other.props_)) {}

  /*!
   * \brief           Move constructor.
   *
   * \details         Makes moved object invalid.
   *
   * \param           other     RecursiveDirectoryIterator to move-assign from.
   *
   * \return          Reference to this object.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto operator=(RecursiveDirectoryIterator&& other) & noexcept -> RecursiveDirectoryIterator& {
    this->props_ = std::move(other.props_);
    return *this;
  }

  /*!
   * \brief           Creates RecursiveIterator object.
   *
   * \details         By default DirectoryIterationOption kNone is set.
   *
   * \param           root_path   Path to the root directory
   * \param           options     DirectoryIterationOptions
   *
   * \return          RecursiveDirectoryIterator.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges     Permissions denied.
   * \error osabstraction::OsabErrc::kSystemEnvironment          Cannot open dirstream.
   * \error osabstraction::OsabErrc::kDoesNotExist               Argument invalid.
   * \error osabstraction::OsabErrc::kResource                   Insufficient memory.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  static auto Create(Path const& root_path,
                     DirectoryIterationOptions options = DirectoryIterationOptions::kNone) noexcept
      -> ::amsr::core::Result<RecursiveDirectoryIterator>;

  // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_PointerNullAtDereference_false_positive
  /*!
   * \brief           Gets the tree depth on which the RecursiveIterator is currently iterating.
   *
   * \details         The depth is calculated starting from the level (counted as depth 0) of the entries in the folder
   *                  for which the RecursiveIterator has been created. The root folder itself is not counted.
   *
   * \return          Current depth.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto Depth() const noexcept -> size_t { return props_->depth_cnter; }

  // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_PointerNullAtDereference_false_positive
  /*!
   * \brief           Returns whether a new recursion step is pending.
   *
   * \details         A new recursion step is pending when the next iteration step will
   *                  cause to the directory enter.
   *
   * \return          True if a new recursion step pending, false otherwise.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto RecursionPending() const noexcept -> bool { return props_->rec_pend; }

  /*!
   * \brief           Disables a recursion step if the recursion is pending.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  void DisableRecursionPending() noexcept;

  /*!
   * \brief           Removes most recently added directory iterator.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  void Pop() noexcept;

  /*!
   * \brief           Dereferences the current directory iterator.
   *
   * \details         A new DirectoryEntry object is constructed as a result as this call.
   *
   * \return          DirectoryEntry in the currently iterated directory.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto operator*() const noexcept -> DirectoryEntry;

  /*!
   * \brief           Tests the iterators for the equality.
   *
   * \param           rhs Right side of the comparison.
   *
   * \return          True if the iterators are equal, false otherwise.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto operator==(RecursiveDirectoryIterator const& rhs) const noexcept -> bool;

  /*!
   * \brief           Tests the iterators for the inequality.
   *
   * \param           rhs Right side of the comparison.
   *
   * \return          True if the iterators are inequal, false otherwise.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto operator!=(RecursiveDirectoryIterator const& rhs) const noexcept -> bool;

  /*!
   * \brief           Increments the Iterator.
   *
   * \return          A reference to the iterator.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto operator++() noexcept -> RecursiveDirectoryIterator&;

  /*!
   * \brief           Increments the Iterator.
   *
   * \return          A reference to the iterator or error.
   *
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error
   * \error osabstraction::OsabErrc::kDoesNotExist               Entry does not exist
   * \error osabstraction::OsabErrc::kResource                   Insufficient memory
   * \error osabstraction::OsabErrc::kSystemEnvironment          No open directory stream
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto Increment() noexcept -> ::amsr::core::Result<RecursiveDirectoryIterator&>;

 private:
  // VECTOR Next Construct VectorC++-V11.0.3: MD_OSA_V11.0.3_PrivateStructure
  /*!
   * \brief struct for properties of the Recursive Iterator.
   */
  struct RecursiveIteratorProps {
    /*!
     * \brief Options for the iterations.
     */
    DirectoryIterationOptions iter_options;

    /*!
     * \brief Flag to signal if the recursion is pending.
     */
    bool rec_pend{false};

    /*!
     * \brief Stores the current depth in iter_buff.
     */
    size_t depth_cnter{0};

    /*!
     * \brief Buffer to store the directory iterators.
     */
    ara::core::Vector<DirectoryIterator> iter_buff{};

    /*!
     * \brief Stores possible errors when the appending to iter_buff is checked
     *        at end of every iteration.
     */
    ::amsr::core::Result<void> optional_iter_error{};

    /*!
     * \brief Internal RecursiveIteratorProps c-tor.
     */
    explicit RecursiveIteratorProps(DirectoryIterationOptions options) : iter_options{options} {}
  };

  /*!
   * \brief Stores the properties of Recursive Iterator.
   * \details std::shared_ptr allows to avoid move of RecursiveIteratorProps structure, only pointer is moved.
   */
  std::shared_ptr<RecursiveIteratorProps> props_;

  /*!
   * \brief Checks if a new recursion step can be done at end of the iteration.
   *        If yes then the new directory iterator is appended to iter_buff.
   *
   * \return          A reference to the iterator or error.
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
   *
   * \trace DSGN-Osab-Filesystem-RecursiveDirectoryIterator
   */
  auto CheckRecursionAndAppend() -> ::amsr::core::Result<void>;

  // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_PointerNullAtDereference_false_positive
  /*!
   * \brief Internal RecursiveDirectoryIterator c-tor.
   */
  RecursiveDirectoryIterator(DirectoryIterator&& to_add, DirectoryIterationOptions options) noexcept
      : props_(std::make_shared<RecursiveIteratorProps>(RecursiveIteratorProps(options))) {
    props_->iter_buff.push_back(std::move(to_add));
    props_->optional_iter_error = CheckRecursionAndAppend();
  }
};

/*!
 * \brief             Begin for the range based loop support.
 *
 * \details           Returns iter unchanged.
 *
 * \param             iter
 *
 * \return            iter unchanged.
 *
 * \context           ANY
 *
 * \pre               -
 *
 * \reentrant         FALSE
 * \synchronous       TRUE
 * \threadsafe        FALSE
 * \steady            TRUE
 *
 * \vprivate          Vector product internal API
 */
inline auto begin(RecursiveDirectoryIterator iter) -> RecursiveDirectoryIterator { return iter; }

/*!
 * \brief             End for the range based loop support.
 *
 * \details           Returns default construct
 *
 * \return            End iterator.
 *
 * \context           ANY
 *
 * \pre               -
 *
 * \reentrant         FALSE
 * \synchronous       TRUE
 * \threadsafe        FALSE
 * \steady            FALSE
 *
 * \vprivate          Vector product internal API
 */
inline auto end(RecursiveDirectoryIterator const&) -> RecursiveDirectoryIterator { return {}; }

}  // namespace filesystem
}  // namespace amsr

#endif  // LIB_FILESYSTEM_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_DIRECTORY_ITERATOR_RECURSIVE_H_

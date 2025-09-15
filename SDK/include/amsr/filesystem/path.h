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
/**        \file  path.h
 *        \brief  Declares the class Path to refer and manipulate file system paths.
 *        \unit   osabstraction::filesystem_path
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_PATH_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_PATH_H_

#include "amsr/core/string.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace filesystem {

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_Path
/*!
 * \brief Class to refer and manipulate file system paths.
 */
class Path final {
 public:
  /*!
   * \brief Default constructor.
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Path() noexcept = default;

  /*!
   * \brief Copy constructor.
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  Path(Path const&) noexcept = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief Move constructor.
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Path(Path&&) noexcept = default;

  /*!
   * \brief Copy assignment.
   * \return Reference to itself.
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  Path& operator=(Path const&) & noexcept = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief Move assignment.
   * \return Reference to itself.
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Path& operator=(Path&&) & noexcept = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief Destructor.
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ~Path() noexcept = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief Construct a new Path from a String.
   *
   * \param       source  Value to construct Path from.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  explicit Path(::amsr::core::String const& source) noexcept;

  /*!
   * \brief Construct a new Path from a StringView.
   *
   * \param       source  Value to construct Path from.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  explicit Path(::amsr::core::StringView const& source) noexcept;

  /*!
   * \brief Construct a new Path from a temporary String.
   *
   * \param       source  Value to construct Path from.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  explicit Path(::amsr::core::String&& source) noexcept;

  // VECTOR Next Construct VectorC++-V3.9.2: MD_OSA_V3.9.2_PathFunctions_falsepositive
  /*!
   * \brief       Construct a new Path object from a two iterators from which a String can be constructed.
   *
   * \tparam      InputIter  Iterator data type from which a String can be constructed.
   * \param       first  Begin iterator for construction.
   * \param       last   End iterator for construction.
   *
   * \pre         first and last must be valid iterators referring to the same object.
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      FALSE
   *
   * \vprivate    Vector product internal API
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  template <class InputIter>
  Path(InputIter const& first, InputIter const& last) noexcept
      : Path{::amsr::core::String{first, last}} {}  // VCA_OSA_OS_CALL_PASS_PTR_R

  /*!
   * \brief       Append another Path to the current.
   *
   * \details     This method concatenates two Paths and adds a path separator between them.
   *              - If the left side already ends with a separator no additional will be added.
   *              - If the right side is an absolute path, it will replace the left side entirely.
   *
   * \param       rhs  Path to append.
   *
   * \return      Reference to changed Path.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  Path& operator/=(Path const& rhs) & noexcept;

  // VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_OSA_M5.17.1_missing_overload
  /*!
   * \brief       Append another Path to the current.
   *
   * \details     \ref Path::operator/=(Path const&)
   *
   * \tparam      Source  Type to create a Path from.
   *
   * \param       rhs  Path to append.
   *
   * \return      Reference to changed Path.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  template <class Source>
  Path& operator/=(Source const& rhs) & noexcept {
    // VECTOR Next Line AutosarC++17_10-M5.2.12: MD_OSA_M5.2.12_ArrayToPointerDecay
    return *this /= Path{rhs};
  }

  /*!
   * \brief       Create a new Path by appending two Paths together.
   *
   * \details     \ref Path::operator/=(Path const&)
   *
   * \param       rhs  Right side of the two Paths appended together.
   *
   * \return      Newly created Path.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  Path operator/(Path const& rhs) const noexcept;

  // VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_OSA_M5.17.1_missing_overload
  /*!
   * \brief       Create a new Path by appending two Paths together.
   *
   * \details     \ref Path::operator/=(Path const&)
   *
   * \tparam      Source  Type to create a Path from.
   *
   * \param       rhs  Right side of the two Paths appended together.
   *
   * \return      Newly created Path.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  template <class Source>
  Path operator/(Source const& rhs) const noexcept {
    // VECTOR NC AutosarC++17_10-M5.0.16: MD_OSA_M5.0.16_pointer_arithmetic_on_non_array_target_false_positive
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_OSA_M5.2.12_ArrayToPointerDecay
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay)
    return *this / Path{amsr::core::StringView{rhs}};  // VCA_OSA_CALL_BY_PTR_PASS_PTR_R, VCA_OSA_CALL_CTOR
  }

  /*!
   * \brief       Compares two Paths.
   *
   * \details     Two Paths are considered equal if their string representations are equal.
   *
   * \param       rhs  Right side of the two Paths compared.
   *
   * \return      true if the both Paths are equal, false otherwise
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  bool Compare(Path const& rhs) const noexcept;

  /*!
   * \brief       Returns a null-terminated string representation of the Path to be used
   *              with standard library functions.
   *
   * \return      Null-terminated string representation of the Path.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  char const* CStr() const noexcept;

  /*!
   * \brief       Returns whether the Path is empty.
   *
   * \details     The Path is considered empty when its string representation is empty.
   *
   * \return      true if the Path is empty, false otherwise.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  bool IsEmpty() const noexcept;

  /*!
   * \brief       Returns whether the Path is absolute.
   *
   * \details     The Path is considered absolute if it begins with a directory separator character.
   *
   * \return      true if the Path is absolute, false otherwise.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  bool IsAbsolute() const noexcept;

  /*!
   * \brief       Returns a new Path representing the path of the parent directory.
   *
   * \details     If the Path represents root directory or when empty a copy is returned
   *
   * \return      New Path representing the path of the parent directory.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  Path ParentPath() const noexcept;

  /*!
   * \brief       Returns a new Path representing filename component of the Path.
   *
   * \details     The returned filename maybe empty if the Path ends with a directory separator.
   *
   * \return      New Path representing filename component of the Path.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  Path Filename() const noexcept;

  /*!
   * \brief       Retruns a new Path representing a normalized form of the Path.
   *
   * \details     A Path is normalized by applying the following rules:
   *              - If the Path is empty its normalized form is an empty Path.
   *              - Replace multiple directory separators by one.
   *              - Remove "." entry.
   *              - Remove ".." entry if it follows immediately after root.
   *              - Remove ".." entry and an entry preceeding it if the preceeding entry was not "..".
   *              - If after the applying all the rules the resulted Path is empty replace it with ".".
   *
   * \return      New Path representing a normalized form.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  Path Normalize() const noexcept;

  /*!
   * \brief Character representing the path directory separator.
   */
  static constexpr ::amsr::core::String::value_type const kSeparator{'/'};

  /*!
   * \brief       Iterator class to iterate over components of a Path.
   *
   * \details     If the Path is absolute, the first entry will be "/".
   *              If the Path ends with a directory separator the last entry will be an empty Path.
   *              Multiple directory separators will be ignored.
   *              Examples:
   *              "/a/b/c/" will result into the chain "/", "a", "b", "c", ""
   *              "a//b//" will result into chain "a", "b", ""
   */
  class iterator final {
   public:
    iterator() noexcept = default;

    /*!
     * \brief Constructs a Path iterator.
     *
     * \param first  begin iterator for the String representing a Path.
     * \param last   end iterator for the String representing a Path.
     * \param pos    Starting position of the iterator.
     *
     * \pre         All given iterators must belong to the same string and pos must refer to a position within the range
     *              defined by first and last.
     *
     * \context     ANY
     *
     * \reentrant   FALSE
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      FALSE
     *
     * \vprivate    Vector product internal API
     *
     * \trace DSGN-Osab-Filesystem-Path
     */
    iterator(::amsr::core::String::const_iterator const& first, ::amsr::core::String::const_iterator const& last,
             ::amsr::core::String::const_iterator const& pos) noexcept;

    /*!
     * \brief       Pre-increment operator.
     *
     * \return      Reference to itself.
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
     *
     * \trace DSGN-Osab-Filesystem-Path
     */
    iterator& operator++() noexcept;

    /*!
     * \brief       Pre-decrement operator.
     *
     * \return      Reference to itself.
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
     *
     * \trace DSGN-Osab-Filesystem-Path
     */
    iterator& operator--() noexcept;

    /*!
     * \brief       Checks for equality of two iterators.
     *
     * \param       other  Iterator to compare with.
     *
     * \return      true if both iterators are equal, false otherwise.
     *
     * \context     ANY
     *
     * \pre         -
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
     *
     * \trace DSGN-Osab-Filesystem-Path
     */
    bool operator==(iterator const& other) const noexcept;

    /*!
     * \brief       Checks for inequality of two iterators.
     *
     * \param       other  Iterator to compare with.
     *
     * \return      true if both iterators are not equal, false otherwise.
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
     *
     * \trace DSGN-Osab-Filesystem-Path
     */
    bool operator!=(iterator const& other) const noexcept;

    /*!
     * \brief       Dereferences an iterator.
     *
     * \return      New Path to represent a part of the original Path.
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
     *
     * \trace DSGN-Osab-Filesystem-Path
     */
    Path operator*() const noexcept;

   private:
    /*!
     * \brief       Iterates over the String forward until the end of the current Path entry is found.
     *
     * \param       pos  Initial position within the String.
     *
     * \return      String position of the end of the current Path entry.
     *
     * \pre         -
     *
     * \context     ANY
     *
     * \reentrant   FALSE
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     */
    ::amsr::core::String::const_iterator Increment(::amsr::core::String::const_iterator const& pos) const noexcept;

    /*!
     * \brief       Iterates over the String backward until the begin of the previous Path entry is found.
     *
     * \param       pos  Initial position within the String.
     *
     * \return      String position of the begin of the previous Path entry.
     *
     * \pre         -
     *
     * \context     ANY
     *
     * \reentrant   FALSE
     * \synchronous TRUE
     * \threadsafe  FALSE
     * \steady      TRUE
     */
    ::amsr::core::String::const_iterator Decrement(::amsr::core::String::const_iterator const& pos) const noexcept;

    /*!
     * \brief   First position in the String representing the Path.
     */
    ::amsr::core::String::const_iterator first_{};
    /*!
     * \brief   Last position in the String representing the Path.
     */
    ::amsr::core::String::const_iterator last_{};
    /*!
     * \brief   Current position in the String representing the Path related to the active position of the iterator.
     */
    ::amsr::core::String::const_iterator pos_{};
  };

  /*!
   * \brief       Returns an iterator representing the first component of the Path. Part of the iterator implementation.
   *
   * \return      Iterator representing the first component of the Path.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  iterator begin() const noexcept;

  /*!
   * \brief       Returns an iterator representing the next to last component of the Path. Part of the iterator
   *              implementation.
   *
   * \return      Iterator representing the next to last component of the Path.
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
   *
   * \trace DSGN-Osab-Filesystem-Path
   */
  iterator end() const noexcept;

 private:
  /*!
   * \brief       Returns the length of the root part of the Path.
   *
   * \details     The length can be larger than 1 in case the root is represented by more than 1 directory separator.
   *
   * \return      The length of the root part of the Path.
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   */
  std::size_t RootLength() const noexcept;

  /*!
   * \brief       Returns whether the Path has a relative part.
   *
   * \details     Relative part is everything beside the root part of a Path. Empty Path has no relative part as well.
   *
   * \return      true if the Path has a relative part, false otherwise.
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   */
  bool HasRelativePath() const noexcept;

  /*!
   * \brief       Returns whether a single character of a String is a directory separator.
   *
   * \param       symbol  The character to check.
   *
   * \return      true if the character is a directory separator., false otherwise.
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   */
  static bool IsSeparator(::amsr::core::String::value_type symbol) noexcept { return symbol == kSeparator; };

  /*!
   * \brief Internal String representation of the Path.
   */
  ::amsr::core::String data_{};
};

/*!
 * \brief       Checks for equality of two Paths.
 *
 * \param       lhs Left side of the comparison.
 * \param       rhs Right side of the comparison.
 *
 * \return      true if both Paths are equal, false otherwise.
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
bool operator==(Path const& lhs, Path const& rhs) noexcept;

/*!
 * \brief       Checks for inequality of two Paths.
 *
 * \param       lhs Left side of the comparison.
 * \param       rhs Right side of the comparison.
 *
 * \return      true if both Paths are not equal, false otherwise.
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
bool operator!=(Path const& lhs, Path const& rhs) noexcept;

}  // namespace filesystem
}  // namespace amsr

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_FILESYSTEM_PATH_H_

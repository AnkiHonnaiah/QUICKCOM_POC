/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ara/core/string_view.h
 *        \brief  SWS core type ara::core::StringView.
 *         \unit  VaCommonLib::ContainerLibrary::ViewContainers::StringView
 *
 *      \details  Based on ara::core::Span implementation. For \0 terminated string_view, use CStringView.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_STRING_VIEW_H_
#define LIB_VAC_INCLUDE_ARA_CORE_STRING_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <cstring>
#include <ios>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <utility>

#include "ara/core/span.h"

#include "vac/testing/test_adapter.h"

namespace ara {
namespace core {

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief   Basic_string_view describes an object that can refer to a constant contiguous sequence of char-like objects
 *          with the first element of the sequence at position zero.
 * \details Basic_string_view  holds only two members: a const pointer T and a size.
 * \tparam  T The type of character encoding. Only char should be used.
 * \trace   CREQ-VaCommonLib-StringView
 * \vpublic
 */
template <typename T>
class basic_string_view final : private ara::core::Span<T const> {
  /*!
   * \brief Internally used type alias for storage type.
   */
  using span_type = ara::core::Span<T const>;

 public:
  /*!
   * \brief Type of contained element.
   */
  using value_type = T;

  /*!
   * \brief Type of the size field.
   */
  using size_type = std::size_t;

  /*!
   * \brief Type of the differences between indices.
   */
  using difference_type = std::ptrdiff_t;

  /*!
   * \brief Reference type for contained elements.
   */
  using reference = value_type&;

  /*!
   * \brief Const reference type for contained elements.
   */
  using const_reference = value_type const&;

  /*!
   * \brief Pointer type for contained elements.
   */
  using pointer = typename span_type::pointer;

  /*!
   * \brief Const pointer type for contained elements.
   */
  using const_pointer = value_type const*;

  /*!
   * \brief Iterator for the basic_string_view.
   */
  using iterator = typename span_type::iterator;

  /*!
   * \brief Const iterator for the basic_string_view.
   */
  using const_iterator = typename span_type::const_iterator;

  /*!
   * \brief Reverse iterator for the basic_string_view.
   */
  using reverse_iterator = typename span_type::reverse_iterator;

  /*!
   * \brief Const reversed iterator for the basic_string_view.
   */
  using const_reverse_iterator = typename span_type::const_reverse_iterator;

  /*!
   * \brief Basis trait class for char type.
   */
  using traits_type = std::char_traits<T>;

  /*!
   * \brief End of view indicator by the functions that expect a view index.
   */
  static constexpr std::size_t npos{std::size_t(-1)};

  // VECTOR Disable AutosarC++17_10-A12.1.6: MD_VAC_A12.1.6_useInheritingConstructor
  /*!
   * \brief Default constructor for an empty basic_string_view.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  constexpr basic_string_view() noexcept : span_type() {}
  // VECTOR Enable AutosarC++17_10-A12.1.6: MD_VAC_A12.1.6_useInheritingConstructor

  /*!
   * \brief     Constructor with a const pointer and size.
   *            When given a nullptr, the size is reset to 0.
   * \param[in] data_val Pointer to the raw array containing the data.
   * \param[in] data_size Length of data in number of elements.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  constexpr basic_string_view(const_pointer data_val, size_type data_size) noexcept
      : span_type(data_val, ((data_val == nullptr) ? 0 : data_size)) {
    // Inline-If in constructor to support constexpr in C++11.
  }

  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief     Constructor with a const pointer.
   * \param[in] data_val Pointer to the raw array containing the data.
   * \details   Is constexpr starting with C++17.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  constexpr basic_string_view(const_pointer data_val) noexcept
      : basic_string_view(data_val, ((data_val == nullptr) ? 0 : traits_type::length(data_val))) {}

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  // VECTOR Disable AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief   Templated constructor to convert from arbitrary containers.
   *          When given a nullptr, the size is reset to 0.
   * \tparam   ContainerType The type of the container, so ContainerType::data() and ContainerType::size()
   *           can be extracted.
   * \param   t A container providing contiguous data.
   * \details Is constexpr starting with C++14.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename ContainerType,
            typename = typename std::enable_if<
                std::is_same<decltype(std::declval<ContainerType const>().data()), const_pointer>::value>::type>
  constexpr basic_string_view(ContainerType const& t) noexcept : basic_string_view(t.data(), t.size()) {}  // NOLINT
  // VECTOR Enable AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect

  /*!
   * \brief Copy constructor.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  basic_string_view(basic_string_view const&) noexcept = default;

  // VECTOR Disable AutosarC++17_10-A10.2.1: MD_VAC_A10.2.1_redefinitionOfNonVirtualFunction
  /*!
   * \brief  Copy assignment.
   * \spec
   *   requires true;
   * \endspec
   * \return A reference to the assigned-to basic_string_view.
   * \vpublic
   */
  basic_string_view& operator=(basic_string_view const&) & noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Move constructor.
   * \spec
   *   requires true;
   * \endspec
   * \vprivate
   */
  basic_string_view(basic_string_view&&) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief  Move assignment.
   * \spec
   *   requires true;
   * \endspec
   * \return A reference to the assigned-to basic_string_view.
   * \vpublic
   */
  basic_string_view& operator=(basic_string_view&&) & noexcept = default;

  /*! \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~basic_string_view() noexcept = default;
  // VECTOR Enable AutosarC++17_10-A10.2.1: MD_VAC_A10.2.1_redefinitionOfNonVirtualFunction

  /*!
   * \brief Get the size of the string.
   * \vpublic
   */
  using span_type::size;

  /*!
   * \brief Get a pointer to the start of the string.
   * \vpublic
   */
  using span_type::data;

  /*!
   * \brief Checks if the string is empty.
   * \vpublic
   */
  using span_type::empty;

  /*!
   * \brief Get a reference to a single character of the string.
   * \vpublic
   */
  using span_type::operator[];

  /*!
   * \brief Get an iterator to the beginning.
   * \vpublic
   */
  using span_type::begin;

  /*!
   * \brief Get a const iterator to the beginning.
   * \vpublic
   */
  using span_type::cbegin;

  /*!
   * \brief Get a reverse iterator to the beginning of the reversed view.
   * \vpublic
   */
  using span_type::rbegin;

  /*!
   * \brief Get a reverse const iterator to the beginning of the reversed view.
   * \vpublic
   */
  using span_type::crbegin;

  /*!
   * \brief Get an iterator to the end.
   * \vpublic
   */
  using span_type::end;

  /*!
   * \brief Get a const iterator to the end.
   * \vpublic
   */
  using span_type::cend;

  /*!
   * \brief Get a reverse iterator to the end of the reversed view.
   * \vpublic
   */
  using span_type::rend;

  /*!
   * \brief Get a reverse const iterator to the beginning of the reversed view.
   * \vpublic
   */
  using span_type::crend;

  /*!
   * \brief  Get the length of the contained string.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return The size of the string.
   * \vpublic
   */
  constexpr size_type length() const noexcept { return this->size(); }

  /*!
   * \brief   Return the max size of string contained by string_view.
   * \details This formula is based on the current libstdc++ implementations.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  The max size of string contained by string_view
   */
  static constexpr size_type max_size() noexcept {
    return (npos - sizeof(size_type) - sizeof(void*)) / sizeof(value_type) / 4;
  }

  // VECTOR Disable AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  /*!
   * \brief  compare the contents of two string_view objects
   * \param  other the second string view to compare
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return a negative value if this view is less than the other character sequence,
   *         zero if the both character sequences are equal or
   *         a positive value if this view is greater than the other character sequence.
   * \vpublic
   */
  int compare(basic_string_view const& other) const noexcept {
    // #10 Compare the first N chars where N is the minimum of both view's sizes.
    std::size_t const min_size{std::min(this->size(), other.size())};
    int result{traits_type::compare(this->begin(), other.begin(), min_size)};

    // #20 If the first characters are equal.
    if (result == 0) {
      // #30 But both views have different sizes.
      if (this->size() != other.size()) {
        // #40 Then the shorter one is the smaller one.
        result = 1;
        if (this->size() < other.size()) {
          result = -1;
        }
      }
    }
    return result;
  }
  // VECTOR Enable AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  /*!
   * \brief     Compare given string to the string contained by this string_view.
   * \param[in] pos1 Index of first character of subString.
   * \param[in] n1 Number of characters in this string to compare.
   * \param     other String to compare.
   * \return    Integer < 0, 0, or > 0.
   * \pre       pos1 <= size() must be true.
   */
  int compare(size_type pos1, size_type n1, basic_string_view const& other) const noexcept {
    return this->compare(pos1, n1, other, static_cast<size_type>(0), other.size());
  }

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  /*!
   * \brief     Compare a given subString of a String to the subString of the String contained by this string view.
   * \param[in] pos1 Index of first character of the String. pos1 <= size() must be true.
   * \param[in] n1 Number of characters in this string to compare.
   * \param     other subString to compare.
   * \param[in] pos2 Index of first character of the subString. pos2 <= str.size() must be true.
   * \param[in] n2 Number of characters from the subString to compare.
   * \return    Integer < 0, 0, or > 0.
   * \pre       pos1 <= size() must be true.
   * \pre       pos2 <= str.size() must be true.
   */
  int compare(size_type pos1, size_type n1, basic_string_view const& other, size_type pos2,
              size_type n2 = npos) const noexcept {
    basic_string_view const sv_str{basic_string_view{other}.substr(pos2, n2)};
    return basic_string_view{*this}.substr(pos1, n1).compare(sv_str);
  }

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  /*!
   * \brief   Compare a C String to the string contained by this string view.
   * \details Comparing against a nullptr is equivalent to comparing with an empty string.
   * \param   s C String to compare.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  Integer < 0, 0, or > 0.
   */
  int compare(value_type const* s) const noexcept { return this->compare(static_cast<size_type>(0), this->size(), s); }

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  /*!
   * \brief     Compare a C String to the subString of the string contained by this string view.
   * \details   Comparing against a nullptr is equivalent to comparing with an empty string.
   * \param[in] pos Index of first character of subString.
   * \param[in] n1 Number of characters in this string to compare.
   * \param     s C String to compare.
   * \return    Integer < 0, 0, or > 0.
   * \pre       pos <= size() must be true.
   */
  int compare(size_type pos, size_type n1, value_type const* s) const noexcept {
    return basic_string_view{*this}.substr(pos, n1).compare(basic_string_view(s));
  }

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  /*!
   * \brief     Compare the first n characters of a char array to the subString of the string contained by string view.
   * \details   Comparing against a nullptr is equivalent to comparing with an empty string.
   * \param[in] pos Index of first character of this String. pos <= size() must be true.
   * \param[in] n1 Number of characters in this string to compare.
   * \param     s Character array to compare.
   * \param[in] n2 Number of characters from the characater array to compare.
   * \return    Integer < 0, 0, or > 0.
   * \pre       pos <= size() must be true.
   */
  int compare(size_type pos, size_type n1, value_type const* s, size_type n2) const noexcept {
    basic_string_view const sv{s, n2};
    return basic_string_view{*this}.substr(pos, n1).compare(sv);
  }

  /*!
   * \brief     Copy subString into the C String.
   * \param[in] s Dest C String.
   * \param[in] n Number of characters to be copied.
   * \param[in] pos Index of the first character to copy. pos <= size() must be true.
   * \return    Number of characters being copied.
   * \pre       s must not be nullptr.
   * \pre       pos <= size() must be true.
   */
  size_type copy(value_type* s, size_type n, size_type pos = 0) const noexcept {
    size_type effective_length{0};
    if ((s != nullptr)) {
      if (pos <= this->size()) {
        effective_length = std::min(n, this->size() - pos);
        if (effective_length != 0) {
          // VECTOR NC AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmeticAllocatorIterator
          // VECTOR NL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
          this->CopyOptimized(s, span_type::data() + pos, effective_length);
        }
      } else {
        Abort("ara::core::StringView::copy: Position is invalid!");
      }
    } else {
      Abort("ara::core::StringView::copy: C String pointer cannot be nullptr!");
    }
    return effective_length;
  }

  // VECTOR NC AutosarC++17_10-M0.1.2: MD_VAC_M0.1.2_infeasiblePaths
  /*!
   * \brief     Returns a view of the substring.
   * \param[in] pos Position of the first character.
   * \param[in] count Requested length.
   * \pre       this->size() must be smaller than StringView::max_size().
   * \return    View of the substring.
   * \vpublic
   */
  basic_string_view substr(size_type pos = 0, size_type count = npos) const noexcept {
    basic_string_view str{};
    // #10 Unless the start position is greater than the original view's size
    if (this->size() >= pos) {
      // #20 We calculate the number of elements in the new view
      size_type const max_count{this->size() - pos};
      count = std::min(count, max_count);
      // #30 We create a new view of the subset
      str = basic_string_view(std::next(this->data(), static_cast<std::ptrdiff_t>(pos)), count);
    }
    return str;
  }

  /*!
   * \brief  Returns the position of found character.
   * \param  candidate Character to search for.
   * \param  pos Position of the first character.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first character of the found substring, or npos if no such substring is found.
   * \vpublic
   */
  size_type find(value_type candidate, size_type pos = 0) const noexcept {
    return FindChar(this->cend(), candidate, pos);
  }

  /*!
   * \brief  Returns the position of found string.
   * \param  candidate Pointer to a character string to search for.
   * \param  pos Position to start the search at.
   * \param  len Length of substring to search for.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first character of the found substring, or npos if no such substring is found.
   * \vpublic
   */
  size_type find(const_pointer candidate, size_type pos, size_type len) const noexcept {
    size_type ret_value{npos};
    if (candidate != nullptr) {
      ret_value = this->find(basic_string_view(candidate, len), pos);
    }
    return ret_value;
  }

  /*!
   * \brief  Returns the position of found string.
   * \param  candidate Null-terminated sequence of characters to search for.
   * \param  pos Position to start the search at.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first character of the found substring, or npos if no such substring is found.
   * \vpublic
   */
  size_type find(const_pointer candidate, size_type pos = 0) const noexcept {
    size_type ret_value{npos};
    if (candidate != nullptr) {
      ret_value = this->find(basic_string_view(candidate, traits_type::length(candidate)), pos);
    }
    return ret_value;
  }

  /*!
   * \brief   Returns the position of found character.
   * \details The start position cannot be bigger than the maximum value for ptrdiff_t.
   * \param   candidate View to search for.
   * \param   pos Position at which to start the search.
   * \pre     this->size() must be smaller than StringView::max_size().
   * \return  Position of the first character of the found substring, or npos if no such substring is found.
   * \vpublic
   */
  size_type find(basic_string_view candidate, size_type pos = 0) const noexcept {
    size_type ret_value{npos};
    if ((pos <= static_cast<size_type>(std::numeric_limits<std::ptrdiff_t>::max())) &&
        ((pos + candidate.size()) <= this->size())) {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_castMayTruncateValue
      iterator const search_begin{std::next(this->begin(), static_cast<std::ptrdiff_t>(pos))};
      iterator const result{std::search(search_begin, this->end(), candidate.begin(), candidate.end())};
      ret_value = this->IteratorPosToIndex(result, search_begin);
    }
    return ret_value;
  }

  /*!
   * \brief     Finds the last substring equal to the given character sequence.
   * \param[in] candidate View to search for.
   * \param[in] pos Position at which to start the search.
   * \pre       this->size() must be smaller than StringView::max_size().
   * \return    Position of the first character of the found substring or npos if no such substring is found.
   * \internal
   *           - #10 Get a start position that is inside the views bounds.
   *           - #20 Given the view is empty, report a invalid position.
   *             - #30 Given the candidate view is empty, report the last position that is inside the specified range.
   *             - #40 Calculate the start position from the end.
   *             - #50 Search for the substring from right to left.
   *             - #60 Translate the iterator position to the wanted index.
   * \endinternal
   * \vpublic
   */
  size_type rfind(basic_string_view const candidate, size_type const pos = npos) const noexcept {
    size_type ret_value{npos};
    // #10 Get a start position that is inside the views bounds.
    std::size_t const validated_pos{std::min(pos, this->size() - candidate.size())};
    // #20 Given the view is empty, report a invalid position.
    if ((this->empty()) && (!candidate.empty())) {
      ret_value = npos;
    } else if (candidate.empty()) {
      // #30 Given the candidate view is empty, report the last position that is inside the specified range.
      ret_value = validated_pos;
    } else {
      // #40 Otherwise Calculate the start position from the end.
      std::size_t const reversed_start_pos{validated_pos + candidate.size()};
      // #50 Search for the substring from right to left.
      const_reverse_iterator const reverse_begin{
          std::next(this->cbegin(), static_cast<std::ptrdiff_t>(reversed_start_pos))};
      const_reverse_iterator const reverse_end{this->crend()};
      const_reverse_iterator const candidate_reverse_begin{candidate.crbegin()};
      const_reverse_iterator const candidate_reverse_end{candidate.crend()};
      const_reverse_iterator const result{
          std::search(reverse_begin, reverse_end, candidate_reverse_begin, candidate_reverse_end)};
      // #60 Translate the iterator position to the wanted index.
      ret_value = this->ReverseIteratorPosToIndex(result, candidate.size() - 1);
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the last character equal to the candidate.
   * \param  candidate Character to search for.
   * \param  pos Position at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the last character equal to the candidate, or npos if no such substring is found.
   * \vpublic
   */
  size_type rfind(value_type candidate, size_type pos = npos) const noexcept {
    return FindChar(this->crend(), candidate, std::min(pos, this->size()));
  }

  /*!
   * \brief  Finds the last substring equal to the given character sequence.
   * \param  candidate Pointer to a character string to search for.
   * \param  pos Position at which to start the search.
   * \param  count Length of substring to search for.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first character of the found substring or npos if no such substring is found.
   * \vpublic
   */
  size_type rfind(const_pointer candidate, size_type pos, size_type count) const noexcept {
    return this->rfind(basic_string_view(candidate, count), pos);
  }

  /*!
   * \brief  Finds the last substring equal to the given character sequence.
   * \param  candidate Null-terminated sequence of characters to search for.
   * \param  pos Position at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first character of the found substring or npos if no such substring is found.
   * \vpublic
   */
  size_type rfind(const_pointer candidate, size_type pos = npos) const noexcept {
    size_type ret_value{npos};
    if (candidate != nullptr) {
      ret_value = this->rfind(basic_string_view(candidate, traits_type::length(candidate)), pos);
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the first character equal to any of the characters in the given character sequence.
   * \param  candidate View to search for.
   * \param  pos Position at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first occurrence of any character of the substring, or npos if no such character is found.
   * \vpublic
   */
  size_type find_first_of(basic_string_view candidate, size_type pos = 0) const noexcept {
    size_type ret_value{npos};
    if (pos < this->size()) {
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_castMayTruncateValue
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_castMayTruncateValue
      iterator const search_begin{std::next(this->begin(), static_cast<std::ptrdiff_t>(pos))};
      iterator const result{std::find_first_of(search_begin, this->end(), candidate.begin(), candidate.end())};
      ret_value = this->IteratorPosToIndex(result, search_begin);
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the first character equal to any of the characters in the given character sequence.
   * \param  candidate Character to search for.
   * \param  pos Position at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first occurrence of any character of the substring, or npos if no such character is found.
   * \vpublic
   */
  size_type find_first_of(value_type candidate, size_type pos = 0) const noexcept { return find(candidate, pos); }

  /*!
   * \brief  Finds the first character equal to any of the characters in the given character sequence.
   * \param  candidate Pointer to a string of characters to search for.
   * \param  pos Position at which to start the search.
   * \param  count Length of the string of characters to search for.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first occurrence of any character of the substring, or npos if no such character is found.
   * \vpublic
   */
  size_type find_first_of(const_pointer candidate, size_type pos, size_type count) const noexcept {
    size_type ret_value{npos};
    if (candidate != nullptr) {
      ret_value = this->find_first_of(basic_string_view(candidate, count), pos);
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the first character equal to any of the characters in the given character sequence.
   * \param  candidate Null-terminated sequence of characters to search for.
   * \param  pos Position at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first occurrence of any character of the substring, or npos if no such character is found.
   * \vpublic
   */
  size_type find_first_of(const_pointer candidate, size_type pos = 0) const noexcept {
    size_type ret_value{npos};
    if (candidate != nullptr) {
      ret_value = this->find_first_of(basic_string_view(candidate, traits_type::length(candidate)), pos);
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the first character not equal to any of the characters in the given character sequence.
   * \param  candidate View to search for.
   * \param  pos Position at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first occurrence of any character not equal to any character of the substring, or npos if
   *         no such character is found.
   * \vpublic
   */
  size_type find_first_not_of(basic_string_view candidate, size_type pos = 0) const noexcept {
    size_type retval{npos};
    for (; pos < this->size(); ++pos) {
      size_type const p{candidate.find((*this)[pos])};
      if (p == npos) {
        retval = pos;
        break;
      }
    }
    return retval;
  }

  /*!
   * \brief  Finds the first character not equal to the given character.
   * \param  candidate Character to search for.
   * \param  pos Position at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first occurrence not equal to the given character, or npos if no such character is found.
   * \vpublic
   */
  size_type find_first_not_of(value_type candidate, size_type pos = 0) const noexcept {
    return FindNotChar(this->cend(), candidate, pos);
  }

  /*!
   * \brief  Finds the first character not equal to any of the characters in the given character sequence.
   * \param  candidate Pointer to a string of characters to search for.
   * \param  pos Position at which to start the search.
   * \param  count Length of the string of characters to search for.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first occurrence of any character not equal to any character of the substring, or npos if
   *         no such character is found.
   * \vpublic
   */
  size_type find_first_not_of(const_pointer candidate, size_type pos, size_type count) const noexcept {
    size_type ret_value{npos};
    if (candidate != nullptr) {
      ret_value = find_first_not_of(basic_string_view{candidate, count}, pos);
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the first character not equal to any of the characters in the given character sequence.
   * \param  candidate Null-terminated sequence of characters to search for.
   * \param  pos Position at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the first occurrence of any character not equal to any character of the substring, or npos if
   *         no such character is found.
   * \vpublic
   */
  size_type find_first_not_of(const_pointer candidate, size_type pos = 0) const noexcept {
    size_type ret_value{npos};
    if (candidate != nullptr) {
      ret_value = find_first_not_of(basic_string_view{candidate, traits_type::length(candidate)}, pos);
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the last character equal to any of the characters in the given character sequence.
   * \param  candidate Pointer to a string of characters to search for.
   * \param  pos Position in this string_view at which to start the search.
   * \param  count Length of the string of characters to search for.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the last occurrence of any character of the substring, or npos if no such character is found.
   * \vpublic
   */
  size_type find_last_of(const_pointer candidate, size_type pos, size_type count) const noexcept {
    return find_last_of(basic_string_view{candidate, count}, pos);
  }

  /*!
   * \brief  Finds the last character equal to any of the characters in the given character sequence.
   * \param  candidate View to search for.
   * \param  pos Position this string_view at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the last occurrence of any character of the substring, or npos if no such character is found.
   * \vpublic
   */
  size_type find_last_of(basic_string_view candidate, size_type pos = npos) const noexcept {
    size_type ret_value{npos};
    if (this->size_ > 0) {
      size_type const max_pos{this->size_ - 1u};
      pos = std::min(pos, this->size_ - 1u);  // pos must point to a valid character
      size_type self_check_position{0u};
      bool done{false};
      // iterate this data
      // VECTOR Next Line AutosarC++17_10-M5.0.9: MD_VAC_M5.0.9_explicitConversionChangesSignedness
      const_reverse_iterator it{std::next(this->crbegin(), static_cast<difference_type>(max_pos - pos))};
      for (; it != this->crend(); ++it) {
        // iterate input data
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
        for (const_iterator cand_it{candidate.cbegin()}; cand_it != candidate.cend(); ++cand_it) {
          if (*it == *cand_it) {
            ret_value = pos - self_check_position;
            done = true;
            break;
          }
        }
        if (done) {
          break;
        }
        ++self_check_position;
      }
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the last occurrence of a given character.
   * \param  candidate Character to search for.
   * \param  pos Position this string_view at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the last occurrence of the character, or npos if no such character is found.
   * \vpublic
   */
  size_type find_last_of(value_type candidate, size_type pos = npos) const noexcept { return rfind(candidate, pos); }

  /*!
   * \brief  Finds the last character equal to any of the characters in the given character sequence.
   * \param  candidate Null-terminated sequence of characters to search for.
   * \param  pos Position this string_view at which to start the search.
   * \pre    this->size() must be smaller than StringView::max_size().
   * \return Position of the last occurrence of any character of the substring, or npos if no such character is found.
   * \vpublic
   */
  size_type find_last_of(const_pointer candidate, size_type pos = npos) const noexcept {
    return find_last_of(basic_string_view{candidate, traits_type::length(candidate)}, pos);
  }

  // VECTOR NC AutosarC++17_10-M0.1.2: MD_VAC_M0.1.2_infeasiblePaths
  /*!
   * \brief  Finds the last character not equal to any of the characters in the given character sequence.
   * \param  candidate string_view with characters to search for.
   * \param  pos Position to start the search in this string_view.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Position of the last character not in the candidate, or npos if no such character is found.
   * \vpublic
   */
  size_type find_last_not_of(basic_string_view candidate, size_type pos) const noexcept {
    size_type ret_value{npos};
    if (this->size() > 0) {
      size_type search_pos{std::min(pos, this->size() - 1) + 1};
      while (search_pos-- > 0) {
        // VCA_VAC_VALID_ARGUMENT
        if (traits_type::find(candidate.data(), candidate.size(), (*this)[search_pos]) == nullptr) {
          ret_value = search_pos;
          break;
        }
      }
    }
    return ret_value;
  }

  /*!
   * \brief  Finds the last character not equal to the given character.
   * \param  candidate Character to search for.
   * \param  pos Position to start the search in this string_view.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Position of the last character not equal to the candidate, or npos if no such character is found.
   * \vpublic
   */
  size_type find_last_not_of(value_type candidate, size_type pos = npos) const noexcept {
    return FindNotChar(this->crend(), candidate, (pos == npos) ? this->size() : std::min(pos + 1, this->size()));
  }

  /*!
   * \brief  Finds the last character not equal to any of the characters in the given character sequence.
   * \param  candidate Pointer to a string of characters to search for.
   * \param  pos Position to start the search in this string_view.
   * \param  count Length of the string of characters to search for.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Position of the last character not in the candidate, or npos if no such character is found.
   * \vpublic
   */
  size_type find_last_not_of(const_pointer candidate, size_type pos, size_type count) const noexcept {
    return find_last_not_of(basic_string_view{candidate, count}, pos);
  }

  /*!
   * \brief  Finds the last character not equal to any of the characters in the given character sequence.
   * \param  candidate Null-terminated sequence of characters to search for.
   * \param  pos Position to start the search in this string_view.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Position of the last character not in the candidate, or npos if no such character is found.
   * \vpublic
   */
  size_type find_last_not_of(const_pointer candidate, size_type pos = npos) const noexcept {
    return find_last_not_of(basic_string_view{candidate, traits_type::length(candidate)}, pos);
  }

  /*!
   * \brief  Checks if the string_view contains a null terminating symbol.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Returns true if a null character is contained, false if not.
   * \vprivate
   */
  bool ContainsNullCharacter() const noexcept {  // Comment to force line break. See TAR-22835.
    return this->find_last_of('\0') != npos;
  }

  /*!
   * \brief  Checks if the string_view ends with a null terminating symbol.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Returns true if the view is terminated with a null character.
   * \vprivate
   */
  bool IsNullTerminated() const noexcept {
    bool ret_value{false};
    if (((this->data_) != nullptr) && (!this->empty())) {
      const_reference back{this->back()};
      ret_value = back == '\0';
    }
    return ret_value;
  }

  /*!
   * \brief     Moves the end of the view back by n characters.
   * \param[in] n Number of characters to remove from the end of the view.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  void remove_suffix(size_type n) noexcept {
    if (n < this->size_) {
      this->size_ -= n;
    } else {
      this->size_ = 0;
    }
  }

  /*!
   * \brief     Moves the start of the view forward by n characters.
   * \param[in] n Number of characters to remove from the start of the view.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  void remove_prefix(size_type n) noexcept {
    if (this->data_ != nullptr) {
      n = std::min(n, this->size_);
      std::advance(this->data_, n);
      this->size_ -= n;
    }
  }

  /*!
   * \brief Returns the first element.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   * \return The first element.
   * \vpublic
   */
  const_reference front() const noexcept { return at(0); }

  /*!
   * \brief  Returns the last element.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return The last element.
   * \vpublic
   */
  const_reference back() const noexcept { return at(this->size() - 1); }  // VCA_VAC_VALID_RETURN_VALUE

  /*!
   * \brief     Returns the character at specified index.
   * \param[in] i Index of the character
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The character at specified index.
   * \vpublic
   */
  const_reference at(size_type i) const noexcept { return this->operator[](i); }  // VCA_VAC_VALID_RETURN_VALUE

  /*!
   * \brief   Creates an owned string from the string_view.
   * \details Will terminate if the allocator throws an exception.
   * \param   alloc Allocator for string instantiation.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  The created string.
   * \vprivate
   */
  template <typename Allocator = std::allocator<T>>
  auto ToString(Allocator const& alloc = Allocator()) const noexcept -> std::basic_string<T, traits_type, Allocator> {
    return {this->data(), this->size(), alloc};  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief          Swap specialization.
   * \param[in, out] other StringView to be swapped
   * \pre            -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  void swap(basic_string_view& other) noexcept { std::swap(*this, other); }

 private:
  // VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_usingStrongTypes
  /*!
   * \brief     Finds the position in the view for a passed iterator.
   * \param[in] it The iterator to check.
   * \param[in] search_begin The iterator where the search began.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    The position of the iterator.
   */
  size_type IteratorPosToIndex(iterator const it, const_iterator const search_begin) const noexcept {
    size_type ret_value{npos};
    if ((it != this->end()) || (it == search_begin)) {
      difference_type const diff{std::distance(this->begin(), it)};
      assert(diff >= 0);  // COV_MSR_INV_STATE_ASSERT
      ret_value = static_cast<size_type>(diff);
    }
    return ret_value;
  }

  /*!
   * \brief    Finds the position in the view for a passed reverse iterator.
   * \param    reverse_it The reverse_iterator to convert to an index.
   * \param    offset The index is moved by offset.
   * \return   The position of the iterator.
   * \pre      Offset must be smaller than this->size() & this must not be empty.
   * \internal
   *           - #10 Given that the passed iterator points beyond the last element, return the invalid position
   *           - #20 Otherwise return the distance between the iterator position and the start.
   * \endinternal
   */
  size_type ReverseIteratorPosToIndex(const_reverse_iterator reverse_it, std::size_t offset = 0) const noexcept {
    size_type ret_value{npos};
    // #10 Given that the passed iterator points beyond the last element, return the invalid position.
    if (reverse_it != this->crend()) {
      // #20 Otherwise return the distance between the iterator position and the start.
      const_iterator const forward_it{const_reverse_iterator{reverse_it}.base()};
      ret_value = static_cast<size_type>(std::distance(this->cbegin(), forward_it)) - (offset + 1);
    }
    return ret_value;
  }

  /*!
   * \brief  Finds a single character in the StringView.
   * \tparam It Iterator type which determines the direction to search from, forward or reverse.
   * \param  e Iterator pointing to the end of the StringView.
   * \param  ch The character to search for.
   * \param  pos The starting position of the search. Value is always based on the distance from begin().
   * \return The index of ch if found, else npos.
   * \context ANY
   * \pre size() <= max_size()
   * \spec requires true; \endspec
   * \vprivate Component Private
   */
  template <typename It>
  auto FindChar(It const e, value_type const ch, size_type const pos) const noexcept -> size_type {
    size_type ret_value{npos};
    static_assert(static_cast<size_type>(std::numeric_limits<std::ptrdiff_t>::max()) > basic_string_view<T>::max_size(),
                  "ara::core::StringView::max_size() must be smaller than PTRDIFF_MAX.");
    if (pos <= this->size()) {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_castMayTruncateValue
      It const search_begin{std::next(this->cbegin(), static_cast<std::ptrdiff_t>(pos))};
      It const result{std::find(search_begin, e, ch)};
      ret_value = (result == e) ? npos : static_cast<size_type>(std::distance(this->cbegin(), &*result));
    }
    return ret_value;
  }

  /*!
   * \brief  Finds a single character in the StringView that differs from the given character.
   * \tparam It Iterator type which determines the direction to search from, forward or reverse.
   * \param  e Iterator pointing to the end of the StringView.
   * \param  ch The character to exclude in the search.
   * \param  pos The starting position of the search. Value is always based on the distance from begin().
   * \return The index of the first character that is not ch, or npos if no such character exists.
   * \context ANY
   * \pre size() <= max_size()
   * \spec requires true; \endspec
   * \vprivate Component Private
   */
  template <typename It>
  auto FindNotChar(It const e, value_type const ch, size_type const pos) const noexcept -> size_type {
    size_type ret_value{npos};
    static_assert(static_cast<size_type>(std::numeric_limits<std::ptrdiff_t>::max()) > basic_string_view<T>::max_size(),
                  "ara::core::StringView::max_size() must be smaller than PTRDIFF_MAX.");
    if (pos <= this->size()) {
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_castMayTruncateValue
      for (It i{std::next(this->cbegin(), static_cast<std::ptrdiff_t>(pos))}; i != e; ++i) {
        if (*i != ch) {
          ret_value = static_cast<size_type>(std::distance(this->cbegin(), &*i));
          break;
        }
      }
    }
    return ret_value;
  }

  // VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_usingStrongTypes
  /*!
   * \brief     Optimized copy function.
   * \param[in] d Pointer to a character String to copy to.
   * \param     s Pointer to a character String to copy from.
   * \param[in] n Number of characters.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  static void CopyOptimized(value_type* d, value_type const* s, size_type n) noexcept {
    if (n == 1) {
      traits_type::assign(*d, *s);  // VCA_VAC_VALID_ARGUMENT
    } else {
      static_cast<void>(traits_type::copy(d, s, n));
    }
  }
};

namespace internal {

/*!
 *   \brief Identity type alias for basic_string_view
 *  \tparam T The type of character encoding
 * \vprivate
 */
template <typename T>
using BsvIdentity = std::decay_t<T>;

}  // namespace internal

/*!
 * \brief     Checks if the contents of two string views are the same
 * \tparam    T The type of character encoding
 * \param[in] lhs the first string view to compare
 * \param[in] rhs the second string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are the same
 * \vpublic
 */
template <typename T>
constexpr bool operator==(basic_string_view<T> lhs, basic_string_view<T> rhs) noexcept {
  return lhs.compare(rhs) == 0;
}

/*!
 * \brief     Checks if the contents of a string view and a a string view implicitly convertible
 *            object are the same
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param[in] rhs the object to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are the same
 * \vpublic
 */
template <typename T>
constexpr bool operator==(basic_string_view<T> lhs, internal::BsvIdentity<basic_string_view<T>> rhs) noexcept {
  return lhs.compare(rhs) == 0;
}

/*!
 * \brief    Checks if the contents of a string view implicitly convertible object and a
 *            string view are the same
 * \tparam    T The type of character encoding
 * \param[in] lhs the object to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are the same
 * \vpublic
 */
template <typename T>
constexpr bool operator==(internal::BsvIdentity<basic_string_view<T>> lhs, basic_string_view<T> rhs) noexcept {
  return rhs == lhs;
}

/*!
 * \brief     Checks if the contents of a string view and a pointer are the same
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param     rhs the pointer to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are the same
 * \vpublic
 */
template <typename T>
constexpr bool operator==(basic_string_view<T> lhs, T const* rhs) noexcept {
  return lhs.compare(basic_string_view<T>(rhs)) == 0;
}

/*!
 * \brief     Checks if the contents of a string view and a pointer are the same
 * \tparam    T The type of character encoding
 * \param     lhs the pointe' to compare
 * \param[in] rhs the string view to compare
 * \returns   true if both contents are the same
 * \spec
 *   requires true;
 * \endspec
 * \vpublic
 */
template <typename T>
constexpr bool operator==(T const* lhs, basic_string_view<T> rhs) noexcept {
  return rhs == lhs;
}

/*!
 * \brief     Checks if the contents of two string views are not the same
 * \tparam    T The type of character encoding
 * \param[in] lhs the first string view to compare
 * \param[in] rhs the second string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are not the same
 * \vpublic
 */
template <typename T>
constexpr bool operator!=(basic_string_view<T> lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs == rhs);
}

/*!
 * \brief     Checks if the contents of a string view and a string view implicitly convertible
 *            object are not the same
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param[in] rhs the object to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are not the same
 * \vpublic
 */
template <typename T>
constexpr bool operator!=(basic_string_view<T> lhs, internal::BsvIdentity<basic_string_view<T>> rhs) noexcept {
  return !(lhs == rhs);
}

/*!
 * \brief     Checks if the contents of a string view implicitly convertible object and a string view
 *            are not the same
 * \tparam    T The type of character encoding
 * \param[in] lhs the object to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are not the same
 * \vpublic
 */
template <typename T>
constexpr bool operator!=(internal::BsvIdentity<basic_string_view<T>> lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs == rhs);
}

/*!
 * \brief     Checks if the contents of a string view and a pointer are not the same
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param     rhs the pointer to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are not the same
 * \vpublic
 */
template <typename T>
constexpr bool operator!=(basic_string_view<T> lhs, T const* rhs) noexcept {
  return !(lhs == rhs);
}

/*!
 * \brief     Checks if the contents of a string view and a pointer are not the same
 * \tparam    T The type of character encoding
 * \param     lhs the pointer to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if both contents are not the same
 * \vpublic
 */
template <typename T>
constexpr bool operator!=(T const* lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs == rhs);
}

/*!
 * \brief     Checks if the contents of the first string view is smaller than the second string view
 * \tparam    T The type of character encoding
 * \param[in] lhs the first string view to compare
 * \param[in] rhs the second string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of the first string view is smaller than the second string view
 * \vpublic
 */
template <typename T>
constexpr bool operator<(basic_string_view<T> lhs, basic_string_view<T> rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

/*!
 * \brief     Checks if the contents of a string view is smaller than the contents of a string view
 *            implicitly convertible object
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param[in] rhs the object to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a string view is smaller than the contents of an object
 * \vpublic
 */
template <typename T>
constexpr bool operator<(basic_string_view<T> lhs, internal::BsvIdentity<basic_string_view<T>> rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

/*!
 * \brief     Checks if the contents of a string view implicitly convertible object
 *            is smaller than the contents of a string view
 * \tparam    T The type of character encoding
 * \param[in] lhs the object view to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of an object is smaller than the contents of a string view
 * \vpublic
 */
template <typename T>
constexpr bool operator<(internal::BsvIdentity<basic_string_view<T>> lhs, basic_string_view<T> rhs) noexcept {
  return lhs.compare(rhs) < 0;
}

/*!
 * \brief     Checks if the contents of a string view is smaller than the contents of a pointer
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param     rhs the pointer to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a string view is smaller than the contents of an object
 * \vpublic
 */
template <typename T>
constexpr bool operator<(basic_string_view<T> lhs, T const* rhs) noexcept {
  return lhs.compare(basic_string_view<T>(rhs)) < 0;
}

/*!
 * \brief     Checks if the contents of a pointer is smaller than the contents of a string view
 * \tparam    T The type of character encoding
 * \param     lhs the pointer to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a pointer is smaller than the contents of a string view
 * \vpublic
 */
template <typename T>
constexpr bool operator<(T const* lhs, basic_string_view<T> rhs) noexcept {
  return basic_string_view<T>(lhs).compare(rhs) < 0;
}

/*!
 * \brief     Checks if the contents of the first string view is greater than the second string view
 * \tparam    T The type of character encoding
 * \param[in] lhs the first string view to compare
 * \param[in] rhs the second string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of the first string view is greater than the second string view
 * \vpublic
 */
template <typename T>
constexpr bool operator>(basic_string_view<T> lhs, basic_string_view<T> rhs) noexcept {
  return rhs < lhs;
}

/*!
 * \brief     Checks if the contents of a string view is greater than the contents of a string view
 *            implicitly convertible object
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param[in] rhs the object to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a string view is greater than the contents of an object
 * \vpublic
 */
template <typename T>
constexpr bool operator>(basic_string_view<T> lhs, internal::BsvIdentity<basic_string_view<T>> rhs) noexcept {
  return rhs < lhs;
}

/*!
 * \brief     Checks if the contents of a string view implicitly convertible object
 *            is greater than the contents of a string view
 * \tparam    T The type of character encoding
 * \param[in] lhs the object view to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of an object is greater than the contents of a string view
 * \vpublic
 */
template <typename T>
constexpr bool operator>(internal::BsvIdentity<basic_string_view<T>> lhs, basic_string_view<T> rhs) noexcept {
  return rhs < lhs;
}

/*!
 * \brief     Checks if the contents of a string view is greater than the contents of a pointer
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param     rhs the pointer to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a string view is greater than the contents of an object
 * \vpublic
 */
template <typename T>
constexpr bool operator>(basic_string_view<T> lhs, T const* rhs) noexcept {
  return rhs < lhs;
}

/*!
 * \brief     Checks if the contents of a pointer is greater than the contents of a string view
 * \tparam    T The type of character encoding
 * \param     lhs the pointer to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a pointer is greater than the contents of a string view
 * \vpublic
 */
template <typename T>
constexpr bool operator>(T const* lhs, basic_string_view<T> rhs) noexcept {
  return rhs < lhs;
}

/*!
 * \brief     Checks if the contents of the first string view is smaller equal than the second string view
 * \tparam    T The type of character encoding
 * \param[in] lhs the first string view to compare
 * \param[in] rhs the second string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of the first string view is smaller equal than the second string view
 * \vpublic
 */
template <typename T>
constexpr bool operator<=(basic_string_view<T> lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs > rhs);
}

/*!
 * \brief     Checks if the contents of a string view is smaller equal than the contents of a
 *            string view implicitly convertible object
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param[in] rhs the object to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a string view is smaller equal than the contents of an object
 * \vpublic
 */
template <typename T>
constexpr bool operator<=(basic_string_view<T> lhs, internal::BsvIdentity<basic_string_view<T>> rhs) noexcept {
  return !(lhs > rhs);
}

/*!
 * \brief     Checks if the contents of a string view implicitly convertible object
 *            is smaller equal than the contents of a string view
 * \tparam    T The type of character encoding
 * \param[in] lhs the object view to compare
 * \param[in] rhs the string view to compare
 * \returns   true if the contents of an object is smaller equal than the contents of a string view
 * \spec
 *   requires true;
 * \endspec
 * \vpublic
 */
template <typename T>
constexpr bool operator<=(internal::BsvIdentity<basic_string_view<T>> lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs > rhs);
}

/*!
 * \brief     Checks if the contents of a string view is smaller equal than the contents of a pointer
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param     rhs the pointer to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a string view is smaller equal than the contents of an object
 * \vpublic
 */
template <typename T>
constexpr bool operator<=(basic_string_view<T> lhs, T const* rhs) noexcept {
  return !(lhs > rhs);
}

/*!
 * \brief     Checks if the contents of a pointer is smaller equal than the contents of a string view
 * \tparam    T The type of character encoding
 * \param     lhs the pointer to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a pointer is smaller equal than the contents of a string view
 * \vpublic
 */
template <typename T>
constexpr bool operator<=(T const* lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs > rhs);
}

/*!
 * \brief     Checks if the contents of the first string view is greater equal than the second string view
 * \tparam    T The type of character encoding
 * \param[in] lhs the first string view to compare
 * \param[in] rhs the second string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of the first string view is greater equal than the second string view
 * \vpublic
 */
template <typename T>
constexpr bool operator>=(basic_string_view<T> lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs < rhs);
}

/*!
 * \brief     Checks if the contents of a string view is greater equal than the contents of a string view
 *            implicitly convertible object
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param[in] rhs the object to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a string view is greater equal than the contents of an object
 * \vpublic
 */
template <typename T>
constexpr bool operator>=(basic_string_view<T> lhs, internal::BsvIdentity<basic_string_view<T>> rhs) noexcept {
  return !(lhs < rhs);
}

/*!
 * \brief     Checks if the contents of a string view implicitly convertible object is greater equal
 *            than the contents of a string view
 * \tparam    T The type of character encoding
 * \param[in] lhs the object view to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of an object is greater equal than the contents of a string view
 * \vpublic
 */
template <typename T>
constexpr bool operator>=(internal::BsvIdentity<basic_string_view<T>> lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs < rhs);
}

/*!
 * \brief     Checks if the contents of a string view is greater equal than the contents of a pointer
 * \tparam    T The type of character encoding
 * \param[in] lhs the string view to compare
 * \param     rhs the pointer to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a string view is greater equal than the contents of an object
 * \vpublic
 */
template <typename T>
constexpr bool operator>=(basic_string_view<T> lhs, T const* rhs) noexcept {
  return !(lhs < rhs);
}

/*!
 * \brief     Checks if the contents of a pointer is greater equal than the contents of a string view
 * \tparam    T The type of character encoding
 * \param     lhs the pointer to compare
 * \param[in] rhs the string view to compare
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \returns   true if the contents of a pointer is greater equal than the contents of a string view
 * \vpublic
 */
template <typename T>
constexpr bool operator>=(T const* lhs, basic_string_view<T> rhs) noexcept {
  return !(lhs < rhs);
}

template <typename T>
constexpr std::size_t basic_string_view<T>::npos;

// VECTOR Disable AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
/*!
 * \brief StringView is a constant assortment of characters.
 * \vpublic
 */
using StringView = basic_string_view<char>;
// VECTOR Enable AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief Alias for basic_string_view<char16_t>.
 */
using u16string_view = ara::core::basic_string_view<char16_t>;

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*!
 * \brief Alias for basic_string_view<char32_t>.
 */
using u32string_view = ara::core::basic_string_view<char32_t>;

// VECTOR Disable AutosarC++17_10-A13.2.2: MD_VAC_A13.2.2_bitwiseOperatorShallReturnBasicValue
/*!
 * \brief     Enable formatted output for string_view.
 * \tparam    StreamT The stream type to write to. Only std::basic_ostream<Char, Traits<char>> shall be used.
 * \tparam    CharT The type of character encoding. Only char should be used.
 * \param[in] os The output stream to write in.
 * \param     sv The string_view to write.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    The same reference to the output stream.
 * \vpublic
 */
template <typename StreamT, typename CharT,
          typename = std::enable_if_t<std::is_integral<typename StreamT::int_type>::value>>
auto operator<<(StreamT& os, basic_string_view<CharT> const& sv) noexcept
    -> decltype(os.write(sv.data(), std::streamsize())) {
  return os.write(sv.data(), static_cast<std::streamsize>(sv.size()));
}
// VECTOR Enable AutosarC++17_10-A13.2.2: MD_VAC_A13.2.2_bitwiseOperatorShallReturnBasicValue

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_STRING_VIEW_H_

/*!********************************************************************************************************************
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
/*!       \file
 *        \brief  Implementation of a read-only wrapper around a trivially copyable string.
 *        \unit   ZeroCopyCommon::TriviallyCopyableContainer::TriviallyCopyableString
 *
 *********************************************************************************************************************/
#ifndef LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_STRING_READER_H_
#define LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_STRING_READER_H_

#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <type_traits>

#include "amsr/core/abort.h"
#include "amsr/zero_copy_common/container/trivially_copyable_container_error_domain.h"
#include "amsr/zero_copy_common/container/trivially_copyable_string.h"
#include "ara/core/result.h"

namespace amsr {
namespace zero_copy_common {
namespace container {

/*!
 * \brief   A read-only wrapper around a trivially copyable string instance that ensures safe bounds
 * \details A trivially copyable string might be shared between processes of different qualification levels.
 *          This class is constructed as a wrapper around a potentially untrusted instance of a string,
 *          i.e. it is created based on a string instance that might contain corrupted data.
 *          The wrapper allows read-only access to this underlying instance and ensures that no out of bounds
 *          memory access happens. Concretely, the string size is checked so no out of bounds access happens.
 *          WARNING: A wrapper instance is tightly bound to its underlying string. It is the user's responsibility
 *                   to ensure the wrapper is not used after the underlying string has changed/is freed.
 *
 * \trace   DSGN-ZeroCopyCommon-TriviallyCopyableString-ReadOnlyWrapper
 *
 * \tparam  kMaxCapacity Maximum capacity of the string. Must be >= 1.
 * \vpublic
 */
template <std::size_t kMaxCapacity>
// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_containersModeledAfterStandardLibrary
class TriviallyCopyableStringReader final {
  // Having a data size of at least one allows simpler and more defensive implementations of accessor methods.
  static_assert(kMaxCapacity >= 1, "The capacity must be at least one, otherwise no elements can be stored.");

 public:
  /*!
   * \brief Alias for the type of traits in this string.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using traits_type = std::char_traits<char>;

  /*!
   * \brief Typedef for the string size.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using size_type = typename TriviallyCopyableString<kMaxCapacity>::size_type;

  /*!
   * \brief Typedef for the contained element type.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using value_type = typename TriviallyCopyableString<kMaxCapacity>::value_type;

  /*!
   * \brief Typedef for constant references to contained elements.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_reference = typename TriviallyCopyableString<kMaxCapacity>::const_reference;

  /*!
   * \brief Typedef for a constant pointer to contained elements.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_pointer = typename TriviallyCopyableString<kMaxCapacity>::const_pointer;

  /*!
   * \brief Typedef for const iterators.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_iterator = typename TriviallyCopyableString<kMaxCapacity>::const_iterator;

  /*!
   * \brief Typedef for reverse const iterators.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_reverse_iterator = typename TriviallyCopyableString<kMaxCapacity>::const_reverse_iterator;

  /*!
   * \brief Create a safe read-only wrapper around the given string instance.
   * \param[in] string The string to be wrapped. The passed string must outlive the created read-only wrapper.
   * \return Safe wrapper instance around the given string.
   * \error TriviallyCopyableContainerErrc::kContainerCorrupted The given string is corrupted,
   *        i.e. its size() > kMaxCapacity, which could lead to invalid memory accesses.
   * \context ANY
   * \pre -
   * \threadsafe TRUE, if the passed string object instance is used exclusively by this thread.
   * \reentrant TRUE, if different string object instances are used for each call.
   * \synchronous TRUE
   * \vpublic
   */
  static auto FromTriviallyCopyableString(TriviallyCopyableString<kMaxCapacity> const& string)
      -> ara::core::Result<TriviallyCopyableStringReader> {
    using Result = ara::core::Result<TriviallyCopyableStringReader<kMaxCapacity>>;
    Result result{TriviallyCopyableContainerErrc::kDefaultError};

    size_type const size{string.ManagementData()};

    if (size <= kMaxCapacity) {
      amsr::core::Array<char, kMaxCapacity + 1> const& elements{string.ElementData()};
      result.EmplaceValue(TriviallyCopyableStringReader{elements, size});
    } else {
      result.EmplaceError(TriviallyCopyableContainerErrc::kContainerCorrupted);
    }

    return result;
  }

  // Default constructor deleted, since member elements_ must always be valid to avoid 'dangling' wrappers.
  TriviallyCopyableStringReader() noexcept = delete;

  /*!
   * \brief Create a copy of another reader-wrapper.
   * \details This will leave two intact references to the original string.
   *          Be careful that the wrapped string outlives the wrappers instances.
   * \param[in] other The wrapper to be copied.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableStringReader(TriviallyCopyableStringReader const& other) noexcept = default;

  /*!
   * \brief Move the given other string into a new reader instance.
   * \details This will leave two intact references to the original string.
   *          Be careful that the wrapped string outlives the wrappers instances.
   * \param[in] other The wrapper to be copied.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableStringReader(TriviallyCopyableStringReader&& other) noexcept = default;

  // Assignment operators deleted, since a read-wrapper instance shall always belong to the string it originated from.
  // This helps to avoid programming mistakes where lifetimes are violated.
  auto operator=(TriviallyCopyableStringReader const& other) noexcept -> TriviallyCopyableStringReader& = delete;
  auto operator=(TriviallyCopyableStringReader&& other) noexcept -> TriviallyCopyableStringReader& = delete;

  /*!
   * \brief Destruct the reader wrapper.
   */
  ~TriviallyCopyableStringReader() noexcept = default;

  /*!
   * \brief Return a const_iterator to the first element of this string.
   * \return const_iterator to the first element.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto begin() const noexcept -> const_iterator { return &(GetData()[0]); }

  /*!
   * \brief Return a const_iterator to the past-the-end element of the string.
   * \return A const_iterator to the past-the-end element of the string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto end() const noexcept -> const_iterator { return &(GetData()[GetSize()]); }

  /*!
   * \brief Return a const_reverse_iterator to the first element in the reversed string.
   * \return A const_reverse_iterator to the first element in the reversed string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rbegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator{end()}; }

  /*!
   * \brief Return a const_reverse_iterator to the past-the-end element in the reversed string.
   * \return A const_reverse iterator to the past-the-end element in the reversed string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rend() const noexcept -> const_reverse_iterator { return const_reverse_iterator{begin()}; }

  /*!
   * \brief Return a const_iterator to the first element of this string.
   * \return A const_iterator to the first element.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto cbegin() const noexcept -> const_iterator { return begin(); }

  /*!
   * \brief Return a const_iterator to the past-the-end element of this string.
   * \return A const_iterator to the past-the-end element of string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto cend() const noexcept -> const_iterator { return end(); }

  /*!
   * \brief Return a const_reverse_iterator to the first element of the reversed string.
   * \return A const_reverse_iterator to the first element of the reversed string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto crbegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator{end()}; }

  /*!
   * \brief Return a const_reverse_iterator to the past-the-end element of the reversed string.
   * \return A const_reverse_iterator to the past-the-end element of the reversed string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto crend() const noexcept -> const_reverse_iterator { return const_reverse_iterator{begin()}; }

  /*!
   * \brief Return the current number of characters in the string.
   * \return Current size of the string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto size() const noexcept -> size_type { return GetSize(); }

  /*!
   * \brief Return the current number of characters in the string.
   * \return Current size of the string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto length() const noexcept -> size_type { return GetSize(); }

  /*!
   * \brief Return the number of elements the string has allocated memory for in its fixed-size internal buffer.
   * \details As this string never re-allocates memory, this method always returns the maximum capacity
   *          given upon creation.
   * \return The static number of elements this string has allocated memory for, i. e. return kMaxCapacity.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  static constexpr auto capacity() noexcept -> size_type { return kMaxCapacity; }

  /*!
   * \brief Check if the string is empty.
   * \return True if the string is empty, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto empty() const noexcept -> bool { return GetSize() == 0; }

  /*!
   * \brief Return the element at the given position without bounds checking.
   * \details Accessing elements out of bounds (position >= size()) results in undefined behavior.
   * \param[in] position The position of the element to return.
   * \return A const_reference to the requested element.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator[](size_type position) const noexcept -> const_reference { return GetData()[position]; }

  /*!
   * \brief Return the character at the given position with bounds checking.
   * \param[in] position The position of the character to return.
   * \return A const_reference to the requested element.
   * \context ANY
   * \pre position < size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto at(size_type position) const noexcept -> const_reference {
    if (position >= GetSize()) {
      amsr::core::Abort("TriviallyCopyableStringReader: Accessed index is out of bounds!");
    }

    return GetData()[position];  // VCA_ZEROCOPYCOMMON_LNG_02_GETDATA_REFERENCE_VALID
  }

  /*!
   * \brief Return a reference to the first element in the string.
   * \return A const_reference to first element in the string.
   * \context ANY
   * \pre String must not be empty.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto front() const noexcept -> const_reference {
    AssertStringNotEmpty();

    return GetData()[0];  // VCA_ZEROCOPYCOMMON_LNG_02_GETDATA_REFERENCE_VALID
  }

  /*!
   * \brief Return a reference to the last element in the string.
   * \return A const_reference to the last element in the string.
   * \context ANY
   * \pre String must not be empty.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto back() const noexcept -> const_reference {
    AssertStringNotEmpty();

    return GetData()[GetSize() - 1];  // VCA_ZEROCOPYCOMMON_LNG_02_GETDATA_REFERENCE_VALID
  }

  /*!
   * \brief Copy the content of this string into a given C string.
   * \param[in] destination Destination C string.
   * \param[in] length Number of characters to be copied.
   * \param[in] position Index of the first character to copy.
   * \return Number of copied characters.
   * \context ANY
   * \pre destination must not be nullptr.
   * \pre position <= size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto copy(value_type* destination, size_type length, size_type position = 0) const noexcept -> size_type {
    if (destination == nullptr) {
      amsr::core::Abort("TriviallyCopyableStringReader: C string pointer cannot be nullptr!");
    }

    AssertCompliesToSize(position);

    size_type const num_chars_to_copy{std::min(length, size() - position)};
    if (num_chars_to_copy != 0) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
      CopyOptimized(destination, data() + position, num_chars_to_copy);
    }

    return num_chars_to_copy;
  }

  /*!
   * \brief Return a const pointer to the contents of the string.
   * \return Const pointer to the contents of the string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto c_str() const noexcept -> const_pointer { return GetData().data(); }

  /*!
   * \brief Provide access to the contiguous memory region where elements are stored in.
   * \return Const pointer to the contiguous memory region where elements are stored in.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto data() const noexcept -> const_pointer { return GetData().data(); }

  /*!
   * \brief Find the position of a given string in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find(TriviallyCopyableStringReader const& search_string, size_type position = 0) const noexcept -> size_type {
    return find(search_string.data(), position, search_string.size());
  }

  /*!
   * \brief Find the position of a C string in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \param[in] length Number of characters in this string to search for.
   * \return Index of first occurrence, or npos if there are no occurrences or search_string is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find(value_type const* search_string, size_type position, size_type length) const noexcept -> size_type {
    return find(ara::core::StringView{search_string, length}, position);
  }

  /*!
   * \brief Find the position of a C string in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences or search_string is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find(value_type const* search_string, size_type position = 0) const noexcept -> size_type {
    return find(ara::core::StringView{search_string}, position);
  }

  /*!
   * \brief Find the position of a character in this string.
   * \param[in] character Character to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find(value_type character, size_type position = 0) const noexcept -> size_type {
    return find(ara::core::StringView{&character, 1}, position);
  }

  /*!
   * \brief Find the position of a given substring in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find(ara::core::StringView search_string, size_type position = 0) const noexcept -> size_type {
    return ara::core::StringView{*this}.find(search_string, position);
  }

  /*!
   * \brief Find last position of a given string in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rfind(TriviallyCopyableStringReader const& search_string,
             size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return rfind(ara::core::StringView{search_string}, position);
  }

  /*!
   * \brief Find last position of a given C string in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \param[in] length Number of characters in this string to search for.
   * \return Index of last occurrence, or npos if there are no occurrences or search_string is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rfind(value_type const* search_string, size_type position, size_type length) const noexcept -> size_type {
    return rfind(ara::core::StringView{search_string, length}, position);
  }

  /*!
   * \brief Find last position of a given C string in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences or search_string is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rfind(value_type const* search_string,
             size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return rfind(ara::core::StringView{search_string}, position);
  }

  /*!
   * \brief Find last position of a character in this string.
   * \param[in] character Character to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rfind(value_type character, size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept
      -> size_type {
    return rfind(ara::core::StringView{&character, 1}, position);
  }

  /*!
   * \brief Find the last position of a given string in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rfind(ara::core::StringView search_string,
             size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return ara::core::StringView{*this}.rfind(search_string, position);
  }

  /*!
   * \brief Find the first character equal to one of the given characters.
   * \param[in] search_set Set of characters that shall be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_of(TriviallyCopyableStringReader const& search_set, size_type position = 0) const noexcept
      -> size_type {
    return find_first_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the first character equal to one of the given characters.
   * \param[in] search_set Set of characters that shall be matched.
   * \param[in] position Index of character in this string to search from.
   * \param[in] length Number of characters in this string to search for.
   * \return Index of first occurrence, or npos if there are no occurrences or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_of(value_type const* search_set, size_type position, size_type length) const noexcept -> size_type {
    return find_first_of(ara::core::StringView{search_set, length}, position);
  }

  /*!
   * \brief Find the first character equal to one of the given characters.
   * \param[in] search_set Set of characters that shall be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_of(value_type const* search_set, size_type position = 0) const noexcept -> size_type {
    return find_first_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the position of the given character in this string.
   * \param[in] character Character to find.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_of(value_type character, size_type position = 0) const noexcept -> size_type {
    return find_first_of(ara::core::StringView{&character, 1}, position);
  }

  /*!
   * \brief Find the first character equal to one of the given characters.
   * \param[in] search_set Set of characters that shall be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Position of the found character, or npos if not found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_of(ara::core::StringView search_set, size_type position = 0) const noexcept -> size_type {
    return ara::core::StringView{*this}.find_first_of(search_set, position);
  }

  /*!
   * \brief Find the last character equal to one of the given characters.
   * \param[in] search_set Set of characters that shall be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_of(TriviallyCopyableStringReader const& search_set,
                    size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return find_last_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the last character equal to one of the given characters.
   * \param[in] search_set Set of characters that shall be matched.
   * \param[in] position Index of character in this string to search from.
   * \param[in] length Number of characters in this string to search for.
   * \return Index of last occurrence, or npos if there are no occurrences or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_of(value_type const* search_set, size_type position, size_type length) const noexcept -> size_type {
    return find_last_of(ara::core::StringView{search_set, length}, position);
  }

  /*!
   * \brief Find the last character equal to one of the given characters.
   * \param[in] search_set Set of characters that shall be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last occurrence, or npos if there are no occurrences or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_of(value_type const* search_set,
                    size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return find_last_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find last position of a given character in this string.
   * \param[in] character Character to find.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_of(value_type character,
                    size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return find_last_of(ara::core::StringView{&character, 1}, position);
  }

  /*!
   * \brief Find the last character equal to one of the given characters.
   * \param[in] search_set Set of characters that shall be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last occurrence, or npos if there are no occurrences.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_of(ara::core::StringView search_set,
                    size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return ara::core::StringView{*this}.find_last_of(search_set, position);
  }

  /*!
   * \brief Find the first character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first character that does not match, or npos if no such characters were found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_not_of(TriviallyCopyableStringReader const& search_set, size_type position = 0) const noexcept
      -> size_type {
    return find_first_not_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the first character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \param[in] length Number of characters in this string to search for.
   * \return Index of first character that does not match,
   *         or npos if no such characters were found or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_not_of(value_type const* search_set, size_type position, size_type length) const noexcept
      -> size_type {
    return find_first_not_of(ara::core::StringView{search_set, length}, position);
  }

  /*!
   * \brief Find the first character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first character that does not match,
   *         or npos if no such characters were found or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_not_of(value_type const* search_set, size_type position = 0) const noexcept -> size_type {
    return find_first_not_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the first character not equal to the given character.
   * \param[in] character Character that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first character that does not match, or npos if no such characters were found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_not_of(value_type character, size_type position = 0) const noexcept -> size_type {
    return find_first_not_of(ara::core::StringView{&character, 1}, position);
  }

  /*!
   * \brief Find the first character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of first character that does not match, or npos if no such characters were found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_first_not_of(ara::core::StringView search_set, size_type position = 0) const noexcept -> size_type {
    return ara::core::StringView{*this}.find_first_not_of(  // VCA_ZEROCOPYCOMMON_SPC_15_STRING_FUNC_FULFILL_CONTRACT
        search_set, position);
  }

  /*!
   * \brief Find the first character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last character that does not match, or npos if no such characters were found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_not_of(TriviallyCopyableStringReader const& search_set,
                        size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return find_last_not_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the last character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \param[in] length Number of characters in this string to search for.
   * \return Index of last character that does not match,
   *         or npos if no such characters were found or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_not_of(value_type const* search_set, size_type position, size_type length) const noexcept
      -> size_type {
    return find_last_not_of(ara::core::StringView{search_set, length}, position);
  }

  /*!
   * \brief Find the last character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last character that does not match,
   *         or npos if no such characters were found or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_not_of(value_type const* search_set,
                        size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return find_last_not_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the last character not equal to the given character.
   * \param[in] character Character that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last character that does not match, or npos if no such characters were found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_not_of(value_type character,
                        size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return find_last_not_of(ara::core::StringView{&character, 1}, position);
  }

  /*!
   * \brief Find the last character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last character that does not match, or npos if no such characters were found.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_not_of(ara::core::StringView search_set,
                        size_type position = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> size_type {
    return ara::core::StringView{*this}.find_last_not_of(  // VCA_ZEROCOPYCOMMON_SPC_15_STRING_FUNC_FULFILL_CONTRACT
        search_set, position);
  }

  /*!
   * \brief Return a substring of this string.
   * \param[in] position Index of the first character.
   * \param[in] length Length of the substring to return.
   * \return A substring of this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto substr(size_type position = 0, size_type length = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept
      -> TriviallyCopyableString<kMaxCapacity> {
    AssertCompliesToSize(position);
    size_type const effective_length{std::min(length, size() - position)};
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return TriviallyCopyableString<kMaxCapacity>{data() + position, effective_length};
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare a string to this string.
   * \param[in] string String to compare to.
   * \return Negative value if the string is lexicographically before the string parameter, positive value if
   *         the string is after the string parameter, and 0 if they are equal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(TriviallyCopyableStringReader const& string) const noexcept -> int {
    return compare(static_cast<size_type>(0), size(), string);
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare a string to the given substring of this string.
   * \param[in] position Index of character in this string to begin comparison.
   * \param[in] length Number of characters in this string to compare.
   * \param[in] string String to compare to.
   * \return Negative value if the substring is lexicographically before the string parameter, positive value if
   *         the substring is after the string parameter, and 0 if they are equal.
   * \context ANY
   * \pre position <= size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(size_type position, size_type length, TriviallyCopyableStringReader const& string) const noexcept
      -> int {
    return compare(position, length, string, static_cast<size_type>(0), string.size());
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare a substring to the given substring of this string.
   * \param[in] position Index of character in this string to begin comparison.
   * \param[in] length Number of characters in this string to compare.
   * \param[in] substring Substring to compare to.
   * \param[in] position_substring Index of character in the substring to begin comparison.
   * \param[in] length_substring Number of characters in the substring to compare.
   * \return Negative value if the substring is lexicographically before the substring parameter, positive value if
   *         the substring is after the substring parameter, and 0 if they are equal.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre position_substring <= substring.size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(size_type position, size_type length, TriviallyCopyableStringReader const& substring,
               size_type position_substring,
               size_type length_substring = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept -> int {
    AssertCompliesToSize(position);
    AssertCompliesToSizeOf(position_substring, substring);

    amsr::core::StringView const substring_sv{
        amsr::core::StringView{substring}.substr(position_substring, length_substring)};
    return amsr::core::StringView{*this}.substr(position, length).compare(substring_sv);
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare a C string to this string.
   * \details Comparing with a nullptr is equivalent to comparing with an empty string.
   * \param[in] string C string to compare to.
   * \return Negative value if the string is lexicographically before the C string parameter, positive value if
   *         the string is after the C string parameter, and 0 if they are equal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(value_type const* string) const noexcept -> int {
    return compare(static_cast<size_type>(0), size(), string);
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare a C string to the given substring of this string.
   * \details Comparing with a nullptr is equivalent to comparing with an empty string.
   * \param[in] position Index of character in this string to begin comparison.
   * \param[in] length Number of characters in this string to compare.
   * \param[in] string C string to compare to.
   * \return Negative value if the substring is lexicographically before the C string parameter, positive value if
   *         the substring is after the C string parameter, and 0 if they are equal.
   * \context ANY
   * \pre position <= size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(size_type position, size_type length, value_type const* string) const noexcept -> int {
    return compare(position, length, string, (string == nullptr) ? 0 : traits_type::length(string));
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare the given amount of characters of a character array to the given substring of this string.
   * \details Comparing with a nullptr is equivalent to comparing with an empty string.
   * \param[in] position Index of character in this string to start comparison.
   * \param[in] length Number of characters in this string to compare.
   * \param[in] string Character array to compare to.
   * \param[in] length_string Number of characters in the characater array to compare.
   * \return Negative value if the substring is lexicographically before the n characters of the character array
   *         parameter, positive value if the substring is after the n characters of the character array parameter,
   *         and 0 if the substring and the n characters of the character array parameter are equal.
   * \context ANY
   * \pre position <= size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(size_type position, size_type length, value_type const* string, size_type length_string) const noexcept
      -> int {
    AssertCompliesToSize(position);
    return amsr::core::StringView{*this}
        .substr(position, length)
        .compare(amsr::core::StringView{string, length_string});
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare the given StringView to this string.
   * \param[in] string_view StringView to compare to.
   * \return Negative value if the string is lexicographically before the string_view parameter, positive value if the
   *         string is after the string_view parameter, and 0 if they are equal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(ara::core::StringView string_view) const noexcept -> int {
    return ara::core::StringView{*this}.compare(string_view);
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare a StringView to the given substring of this string.
   * \param[in] position Index of character in this string to start comparison.
   * \param[in] length Number of characters in this string to compare.
   * \param[in] string_view StringView to compare to.
   * \return Negative value if the substring is lexicographically before the string_view parameter, positive value if
   *         the substring is after the string_view parameter, and 0 if they are equal.
   * \context ANY
   * \pre position <= size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(size_type position, size_type length, ara::core::StringView string_view) const noexcept -> int {
    return ara::core::StringView{*this}.substr(position, length).compare(string_view);
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare a substring to the given substring of this string.
   * \tparam T Container type that is convertible to a StringView.
   * \param[in] position Index of character in this string to start comparison.
   * \param[in] length Number of characters in this string to compare.
   * \param[in] substring Substring to compare to.
   * \param[in] position_substring Starting position in the substring.
   * \param[in] length_substring Number of characters to read from the substring.
   * \return Negative value if the substring is lexicographically before the substring parameter,
   *         positive value if the substring is after the substring parameter, and 0 if they are equal.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre position_substring <= substring.size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <typename T>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto compare(size_type position, size_type length, T const& substring, size_type position_substring,
               size_type length_substring = TriviallyCopyableString<kMaxCapacity>::npos) const noexcept
      -> std::enable_if_t<(std::is_convertible<T const&, ara::core::StringView>::value) &&
                              (!std::is_convertible<T const&, char const*>::value),
                          int> {
    AssertCompliesToSize(position);
    ara::core::StringView substring_sv{substring};
    AssertCompliesToSizeOf(position_substring, substring_sv);

    substring_sv = substring_sv.substr(position_substring, length_substring);
    return ara::core::StringView{*this}.substr(position, length).compare(substring_sv);
  }

 private:
  /*!
   * \brief Pointer referencing the data of the original string.
   */
  amsr::core::Array<char, kMaxCapacity + 1> const& data_;

  /*!
   * \brief Current size of the read-wrapper (checked at creation).
   */
  size_type const size_;

  /*!
   * \brief Construct a string with the given members.
   * \param[in] elements Char array to wrap with a stringreader instance
   * \param[in] size Size of the string.
   * \context ANY
   * \pre size() <= kMaxCapacity must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  TriviallyCopyableStringReader(amsr::core::Array<char, kMaxCapacity + 1> const& elements, size_type size)
      : data_{elements}, size_{size} {
    //@ assert size <= kMaxCapacity;
  }

  /*!
   * \brief Get the current size of the string.
   * \return Current size of the string.
   * \internal
   * Guarantees the VCA contract GetSize() <= max_size().
   * \endinternal
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetSize() const noexcept -> size_type {
    size_type const result{size_};
    // We know that this holds, as we only set it in the constructor - which is checked by VCA.

    // VCA_ZEROCOPYCOMMON_ANO_01_STRING_READER_SIZE_MEMBER_HOLDS_INVARIANT
    //@ assert result <= kMaxCapacity;
    return result;
  }

  /*!
   * \brief Get the current data reference of the string.
   * \return Reference to the data of the string.
   * \internal
   * Guarantees the VCA contract that the reference is valid.
   * \endinternal
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetData() const noexcept -> amsr::core::Array<char, kMaxCapacity + 1> const& {
    amsr::core::Array<char, kMaxCapacity + 1> const& data_local{data_};

    // VCA_ZEROCOPYCOMMON_ANO_01_STRING_READER_REFERENCES_VALID_ELEMENT_ARRAY
    return data_local;
  }

  /*!
   * \brief Optimized copy function.
   * \param[in] str_to Pointer to a character string to copy to.
   * \param[in] str_from Pointer to a character string to copy from.
   * \param[in] number Number of characters.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static void CopyOptimized(value_type* str_to, value_type const* str_from, size_type number) noexcept {
    static_cast<void>(
        traits_type::copy(str_to, str_from, number));  // VCA_ZEROCOPYCOMMON_SPC_15_STRING_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief Assert that the string is not empty.
   * \details Aborts if the assertion fails.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertStringNotEmpty() const noexcept {
    if (empty()) {
      amsr::core::Abort("TriviallyCopyableStringReader: The string cannot be empty!");
    }
  }

  /*!
   * \brief Assert that the given index is within the bounds of this string.
   * \details Aborts if the assertion fails.
   * \param[in] index Index to check.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertCompliesToSize(size_type const index) const noexcept {
    if (index > size()) {
      amsr::core::Abort("TriviallyCopyableStringReader: Position is invalid!");
    }
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  /*!
   * \brief Assert that the given index is within the bounds of the given string.
   * \details Aborts if the assertion fails.
   * \tparam TString Type of the string to perform size check for.
   * \param[in] index Index to check.
   * \param[in] string String to use for size check.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  template <typename TString>
  static void AssertCompliesToSizeOf(size_type const index, TString const& string) noexcept {
    if (index > string.size()) {
      amsr::core::Abort("TriviallyCopyableStringReader: Position is invalid!");
    }
  }
};

/*!
 * \brief Test equality of two strings.
 * \tparam kMaxCapacityLhs Fixed capacity of the lhs string.
 * \tparam kMaxCapacityRhs Fixed capacity of the rhs string.
 * \param[in] lhs First string.
 * \param[in] rhs Second string.
 * \return True if lhs is equal to rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacityLhs, std::size_t kMaxCapacityRhs>
auto operator==(TriviallyCopyableStringReader<kMaxCapacityLhs> const& lhs,
                TriviallyCopyableStringReader<kMaxCapacityRhs> const& rhs) noexcept -> bool {
  return lhs.compare(rhs) == 0;
}

/*!
 * \brief Test equivalence of a C string and a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs C string.
 * \param[in] rhs String.
 * \return True if lhs is equal to rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator==(char const* lhs, TriviallyCopyableStringReader<kMaxCapacity> const& rhs) noexcept -> bool {
  return rhs.compare(lhs) == 0;
}

/*!
 * \brief Test equality of a string and a C string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs C string.
 * \return True if lhs is equal to rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator==(TriviallyCopyableStringReader<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
  return lhs.compare(rhs) == 0;
}

/*!
 * \brief Test unequality of two string.
 * \tparam kMaxCapacityLhs Fixed capacity of the lhs string.
 * \tparam kMaxCapacityRhs Fixed capacity of the rhs string.
 * \param[in] lhs First string.
 * \param[in] rhs Second string.
 * \return True if both strings are unequal, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacityLhs, std::size_t kMaxCapacityRhs>
auto operator!=(TriviallyCopyableStringReader<kMaxCapacityLhs> const& lhs,
                TriviallyCopyableStringReader<kMaxCapacityRhs> const& rhs) noexcept -> bool {
  return !(lhs == rhs);
}

/*!
 * \brief Test unequality of a C string and a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs C string.
 * \param[in] rhs String.
 * \return True if both strings are unequal, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator!=(char const* lhs, TriviallyCopyableStringReader<kMaxCapacity> const& rhs) noexcept -> bool {
  return !(lhs == rhs);
}

/*!
 * \brief Test unequality of a string and a C string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs C string.
 * \return True if both strings are unequal, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator!=(TriviallyCopyableStringReader<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
  return !(lhs == rhs);
}

/*!
 * \brief Test if one string is less than another string.
 * \tparam kMaxCapacityLhs Fixed capacity of the lhs string.
 * \tparam kMaxCapacityRhs Fixed capacity of the rhs string.
 * \param[in] lhs String.
 * \param[in] rhs String.
 * \return True if lhs < rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacityLhs, std::size_t kMaxCapacityRhs>
auto operator<(TriviallyCopyableStringReader<kMaxCapacityLhs> const& lhs,
               TriviallyCopyableStringReader<kMaxCapacityRhs> const& rhs) noexcept -> bool {
  return lhs.compare(rhs) < 0;
}

/*!
 * \brief Test if a string is less than a C string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs C string.
 * \return True if lhs < rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator<(TriviallyCopyableStringReader<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
  return lhs.compare(rhs) < 0;
}

/*!
 * \brief Test if a C string is less than a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs C string.
 * \param[in] rhs String.
 * \return True if lhs < rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator<(char const* lhs, TriviallyCopyableStringReader<kMaxCapacity> const& rhs) noexcept -> bool {
  return rhs.compare(lhs) > 0;
}

/*!
 * \brief Test if a string is greater than another string.
 * \tparam kMaxCapacityLhs Fixed capacity of the lhs string.
 * \tparam kMaxCapacityRhs Fixed capacity of the rhs string.
 * \param[in] lhs First string.
 * \param[in] rhs Second string.
 * \return True if lhs > rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacityLhs, std::size_t kMaxCapacityRhs>
auto operator>(TriviallyCopyableStringReader<kMaxCapacityLhs> const& lhs,
               TriviallyCopyableStringReader<kMaxCapacityRhs> const& rhs) noexcept -> bool {
  return rhs < lhs;
}

/*!
 * \brief Test if a string is greater than a C string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs C string.
 * \return True if lhs > rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator>(TriviallyCopyableStringReader<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
  return rhs < lhs;
}

/*!
 * \brief Test if a C string is greater than a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs C string.
 * \param[in] rhs String.
 * \return True if lhs > rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator>(char const* lhs, TriviallyCopyableStringReader<kMaxCapacity> const& rhs) noexcept -> bool {
  return rhs < lhs;
}

/*!
 * \brief Test if a string is less than or equal to a string.
 * \tparam kMaxCapacityLhs Fixed capacity of the lhs string.
 * \tparam kMaxCapacityRhs Fixed capacity of the rhs string.
 * \param[in] lhs First string.
 * \param[in] rhs Second string.
 * \return True if lhs <= rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacityLhs, std::size_t kMaxCapacityRhs>
auto operator<=(TriviallyCopyableStringReader<kMaxCapacityLhs> const& lhs,
                TriviallyCopyableStringReader<kMaxCapacityRhs> const& rhs) noexcept -> bool {
  return !(lhs > rhs);
}

/*!
 * \brief Test if a string is less than or equal to a C string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs C string.
 * \return True if lhs <= rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator<=(TriviallyCopyableStringReader<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
  return !(lhs > rhs);
}

/*!
 * \brief Test if a C string is less than or equal to a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs C string.
 * \param[in] rhs String.
 * \return True if lhs <= rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator<=(char const* lhs, TriviallyCopyableStringReader<kMaxCapacity> const& rhs) noexcept -> bool {
  return !(lhs > rhs);
}

/*!
 * \brief Test if a string is greater than or equal to another string.
 * \tparam kMaxCapacityLhs Fixed capacity of the lhs string.
 * \tparam kMaxCapacityRhs Fixed capacity of the rhs string.
 * \param[in] lhs First string.
 * \param[in] rhs Second string.
 * \return True if lhs >= rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacityLhs, std::size_t kMaxCapacityRhs>
auto operator>=(TriviallyCopyableStringReader<kMaxCapacityLhs> const& lhs,
                TriviallyCopyableStringReader<kMaxCapacityRhs> const& rhs) noexcept -> bool {
  return !(lhs < rhs);
}

/*!
 * \brief Test if a string is greater than or equal to a C string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs C string.
 * \return True if lhs >= rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator>=(TriviallyCopyableStringReader<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
  return !(lhs < rhs);
}

/*!
 * \brief Test if a C string is greater than or equal to a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs C string.
 * \param[in] rhs String.
 * \return True if lhs >= rhs, false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator>=(char const* lhs, TriviallyCopyableStringReader<kMaxCapacity> const& rhs) noexcept -> bool {
  return !(lhs < rhs);
}

/*!
 * \brief Output stream operator.
 * \tparam StreamT The stream type to write to.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[out] output_stream The output stream to write to.
 * \param[in] string Reference to a string to write to the given stream.
 * \return The given output stream including the given string.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <typename StreamT, std::size_t kMaxCapacity,
          typename = std::enable_if_t<std::is_integral<typename StreamT::int_type>::value>>
auto operator<<(StreamT& output_stream, TriviallyCopyableStringReader<kMaxCapacity> const& string) noexcept
    -> decltype(output_stream << string.c_str()) {
  return output_stream << string.c_str();  // VCA_ZEROCOPYCOMMON_SPC_15_STRING_FUNC_FULFILL_CONTRACT
}

/*!
 * \brief Create a safe read-only wrapper around the given string.
 * \tparam kMaxCapacity Maximum size of the string.
 * \param string The string being wrapped. The passed string must outlive the created read-only wrapper.
 * \context ANY
 * \return Safe wrapper instance around the given string.
 * \error TriviallyCopyableContainerErrc::kContainerCorrupted The given string is corrupted,
 *        i.e. its size() > kMaxCapacity, which could lead to invalid memory access.
 * \pre -
 * \threadsafe TRUE, if the passed string object instance is used exclusively by this thread.
 * \reentrant TRUE, if different string object instances are used for each call.
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto ReadOnlyWrapper(TriviallyCopyableString<kMaxCapacity> const& string) noexcept
    -> ara::core::Result<TriviallyCopyableStringReader<kMaxCapacity>> {
  return TriviallyCopyableStringReader<kMaxCapacity>::FromTriviallyCopyableString(string);
}

}  // namespace container
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_STRING_READER_H_

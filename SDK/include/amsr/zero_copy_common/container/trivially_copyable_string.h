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
/*!        \file
 *        \brief Implementation of a trivially copyable string that stores characters in contiguous memory.
 *         \unit ZeroCopyCommon::TriviallyCopyableContainer::TriviallyCopyableString
 *   \complexity The high complexity of the class TriviallyCopyableString from the equally named unit is caused by a
 *               high nesting level. The high nesting level is required to not duplicate functionality for different
 *               API signatures.
 *               In summary, from design perspective it is useful to introduce this nesting level as the same behavior
 *               is required on several APIs with different parameters. Some of them introduce additional checks which
 *               cause the violation.
 *               Therefore, the risk is considered low.
 *
 *********************************************************************************************************************/
#ifndef LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_STRING_H_
#define LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_STRING_H_

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/array.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace zero_copy_common {
namespace container {

// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_containersModeledAfterStandardLibrary
/*!
 * \brief A data type that represents sequences of characters.
 * \details Any modifying operation performed on an object of this type must not result in size() > kMaxCapacity.
 * \tparam kMaxCapacity Fixed capacity of this data type.
 * \trace DSGN-ZeroCopyCommon-TriviallyCopyableString
 * \vpublic
 */
template <std::size_t kMaxCapacity>
class TriviallyCopyableString final {
  // We often perform additions of two values of type size_type.
  // To avoid overflows we usually check both parameters to be <= kMaxCapacity.
  // By limiting the kMaxCapacity range, we can ensure that no overflow can occur at any point in our code.
  static_assert((std::numeric_limits<size_t>::max() / 2) >= kMaxCapacity,
                "kMaxCapacity must be small enough to avoid integer overflows.");

 public:
  /*!
   * \brief Alias for the type of traits in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using traits_type = std::char_traits<char>;

  /*!
   * \brief Alias for the type of value in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using value_type = typename traits_type::char_type;

  /*!
   * \brief Alias for the type of parameters that indicate a size or a number of values in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using size_type = std::size_t;

  /*!
   * \brief Alias for the type of parameters that indicate a difference of indexes into this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using difference_type = std::ptrdiff_t;

  /*!
   * \brief Alias for the type of reference in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using reference = value_type&;

  /*!
   * \brief Alias for the type of const reference in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_reference = value_type const&;

  /*!
   * \brief Alias for the type of pointer in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using pointer = value_type*;

  /*!
   * \brief Alias for the type of const pointer in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_pointer = value_type const*;

  /*!
   * \brief Alias for the type of iterator in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using iterator = pointer;

  /*!
   * \brief Alias for the type of const iterator in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_iterator = const_pointer;

  /*!
   * \brief Alias for the type of reverse iterator in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using reverse_iterator = std::reverse_iterator<iterator>;

  /*!
   * \brief Alias for the type of const reverse iterator in this string.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /*!
   * \brief Value returned by various member functions when they fail.
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  static size_type const npos;

  /*!
   * \brief Construct a string from a standard template library string.
   * \param[in] string The standard template library source string to construct the string from.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  explicit TriviallyCopyableString(std::string const& string) noexcept : TriviallyCopyableString(string.c_str()) {}

  /*!
   * \brief Construct a string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableString() noexcept = default;

  /*!
   * \brief Copy-construct a string from another one.
   * \param[in] string Source string to construct the string from.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableString(TriviallyCopyableString const& string) noexcept = default;

  /*!
   * \brief Move-construct a string from another one.
   * \param[in] string Source string which gets moved to the target string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableString(TriviallyCopyableString&& string) noexcept = default;

  /*!
   * \brief Construct a string from copying a substring.
   * \param[in] string Source string to construct the string from.
   * \param[in] position First character index to copy from.
   * \param[in] length Number of characters to copy from.
   * \context ANY
   * \pre position <= string.size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableString(TriviallyCopyableString const& string, size_type position, size_type length = npos) noexcept {
    AssertCompliesToSizeOf(position, string);
    size_type const effective_length{std::min(length, string.size() - position)};
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    const_iterator const start{string.begin() + position};
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    ConstructFromIterator(start, start + effective_length);
  }

  /*!
   * \brief Construct a string from a given pointer which points to a character string.
   * \details If string is nullptr, and length is zero, an empty string will be created.
   * \param[in] string Char array to construct the string from.
   * \param[in] length Number of characters to copy.
   * \context ANY
   * \pre The string must not be nullptr while length is nonzero.
   * \pre The length must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableString(value_type const* string, size_type length) noexcept {
    AssertCompliesToMaxCapacity(length);
    if ((string == nullptr) && (length > 0)) {
      amsr::core::Abort(
          "TriviallyCopyableString::TriviallyCopyableString: Pointer cannot be "
          "nullptr while the number of characters to copy is greater than zero.");
    }

    if (string != nullptr) {
      // VECTOR NC AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_stringPointerArithmetic
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
      ConstructFromIterator(string, string + length);
    }
  }

  // VECTOR NC VectorC++-V12.1.4: MD_ZEROCOPYCOMMON_V12.1.4_stringConstructorShallBeDeclaredExplicit
  /*!
   * \brief Construct a string from a given C string.
   * \details If string is nullptr an empty string will be created.
   * \param[in] string C string to construct the string.
   * \context ANY
   * \pre The size of C string must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableString(value_type const* string) noexcept
      : TriviallyCopyableString{string, ((string == nullptr) ? 0 : traits_type::length(string))} {}

  /*!
   * \brief Construct a string with given number of copies of a given character.
   * \param[in] number Number of characters to copy to the string.
   * \param[in] character Character to fill the string with.
   * \context ANY
   * \pre The number must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableString(size_type number, value_type character) noexcept {
    AssertCompliesToMaxCapacity(number);
    ConstructStrInt(number, character);
  }

  /*!
   * \brief Construct a string from given range.
   * \tparam InputIterator Iterator data type.
   * \param[in] begin Start of the range.
   * \param[in] end End of the range.
   * \context ANY
   * \pre begin and end must be a valid range created from the same object.
   *      The caller is responsible to hold this contract, as validity of iterators cannot be fully checked.
   *      Violating this precondition will result in undefined behavior.
   * \pre The number of elements in the range defined by begin and end must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class InputIterator>
  TriviallyCopyableString(InputIterator begin, InputIterator end) noexcept {
    ConstructFromIterator(begin, end);
  }

  /*!
   * \brief Construct a string from a StringView.
   * \param[in] string_view StringView to construct from.
   * \context ANY
   * \pre The size of the string_view must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  explicit TriviallyCopyableString(ara::core::StringView string_view) noexcept
      : TriviallyCopyableString{string_view.data(), string_view.size()} {}

  /*!
   * \brief Construct a string from a container convertible to a StringView.
   * \tparam T Container type that is convertible to a StringView.
   * \param[in] container Container to construct a string from.
   * \param[in] position Start position for reading the container.
   * \param[in] length Number of characters to read.
   * \context ANY
   * \pre Number must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <typename T, typename = std::enable_if_t<std::is_convertible<T const&, ara::core::StringView>::value>,
            typename = std::enable_if_t<!std::is_convertible<T const&, char const*>::value>>
  TriviallyCopyableString(T const& container, size_type position, size_type length) noexcept
      : TriviallyCopyableString{
            TriviallyCopyableString{ara::core::StringView{container}.data(), ara::core::StringView{container}.size()},
            position, length} {}

  /*!
   * \brief Destructor of the string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  ~TriviallyCopyableString() noexcept = default;

  /*!
   * \brief Assign the contents of the given string to this string.
   * \param[in] string Source string.
   * \return Reference to the modified string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(TriviallyCopyableString const& string) & noexcept -> TriviallyCopyableString& = default;

  /*!
   * \brief Assign the contents of the given string to this string by a move operation.
   * \param[in] string Source string.
   * \return Reference to the modified string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(TriviallyCopyableString&& string) & noexcept -> TriviallyCopyableString& = default;

  /*!
   * \brief Assign the contents of pointer to this string.
   * \param[in] source_pointer The pointer to the char sequence to be copied into this string.
   * \return Reference to the modified string.
   * \context ANY
   * \pre source_pointer must not be nullptr.
   * \pre The char sequence to be copied into this string must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(value_type const* source_pointer) & noexcept -> TriviallyCopyableString& {
    static_cast<void>(assign(source_pointer));
    return *this;
  }

  /*!
   * \brief Assign the content of a character to this string.
   * \param[in] character Source character.
   * \return Reference to the modified string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(value_type character) & noexcept -> TriviallyCopyableString& {
    assign(static_cast<size_type>(1), character);
    return *this;
  }

  /*!
   * \brief Assign the contents of an initializer list to this string.
   * \param[in] initializer_list Source initializer list.
   * \return Reference to the modified string.
   * \context ANY
   * \pre Size of the initializer list must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(std::initializer_list<value_type> initializer_list) & noexcept -> TriviallyCopyableString& {
    assign(initializer_list.begin(), initializer_list.size());
    return *this;
  }

  /*!
   * \brief Assign the contents of a StringView to this string.
   * \param[in] string_view StringView to read from.
   * \return Reference to the modified string.
   * \context ANY
   * \pre Size of the StringView must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(ara::core::StringView const& string_view) & noexcept -> TriviallyCopyableString& {
    static_cast<void>(assign(string_view));
    return *this;
  }

  /*!
   * \brief Assign the contents of a standard template library string into this string.
   * \param[in] string string to read from.
   * \return Reference to the modified string.
   * \context ANY
   * \pre The given string's size must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(std::string const& string) & noexcept -> TriviallyCopyableString& {
    static_cast<void>(assign(string.c_str()));
    return *this;
  }

  /*!
   * \brief Return an iterator to the first element of this string.
   * \return An iterator to the first element of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto begin() noexcept -> iterator { return iterator{DataLocal()}; }

  /*!
   * \brief Return a const_iterator to the first element of this string.
   * \return A const_iterator to the first element of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto begin() const noexcept -> const_iterator { return const_iterator{DataLocal()}; }

  /*!
   * \brief Return an iterator to the last element of this string.
   * \return An iterator to the last element of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto end() noexcept -> iterator {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return iterator{DataLocal() + size()};
  }

  /*!
   * \brief Return a const_iterator to the last element of this string.
   * \return A const_iterator to the last element of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto end() const noexcept -> const_iterator {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return const_iterator{DataLocal() + size()};
  }

  /*!
   * \brief Return a reverse_iterator to the last element of this string.
   * \return A reverse_iterator to the last element of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rbegin() noexcept -> reverse_iterator { return reverse_iterator{end()}; }

  /*!
   * \brief Return a const_reverse_iterator to the last element of this string.
   * \return A const_reverse_iterator to the last element of this string.
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
   * \brief Return a reverse_iterator to the first element of this string.
   * \return A reverse_iterator to the first element of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rend() noexcept -> reverse_iterator { return reverse_iterator{begin()}; }

  /*!
   * \brief Return a const_reverse_iterator to the first element of this string.
   * \return A const_reverse_iterator to the first element of this string.
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
   * \return A const_iterator to the first element of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto cbegin() const noexcept -> const_iterator { return const_iterator{DataLocal()}; }

  /*!
   * \brief Return a const_iterator to the last element of this string.
   * \return A const_iterator to the last element of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto cend() const noexcept -> const_iterator {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return const_iterator{DataLocal() + size()};
  }

  /*!
   * \brief Return a const_reverse_iterator to the last element of this string.
   * \return A const_reverse_iterator to the last element of this string.
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
   * \brief Return a const_reverse_iterator to the first element of this string.
   * \return A const_reverse_iterator to the first element of this string.
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
   * \brief Return the size of this string.
   * \return The size of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto size() const noexcept -> size_type { return size_; };

  /*!
   * \brief Return the size of this string.
   * \return The size of this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto length() const noexcept -> size_type { return size_; }

  /*!
   * \brief Resize the string to the specified number of characters.
   * \param[in] number Number of the characters.
   * \param[in] character Character to insert when the size of this string is increased.
   * \context ANY
   * \pre number <= kMaxCapacity must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void resize(size_type number, value_type character) noexcept {
    AssertCompliesToMaxCapacity(number);
    size_type const current_size{this->size()};
    if (current_size > number) {
      SetLength(number);
    } else if (current_size < number) {
      append(number - current_size, character);
    } else {  // Nothing to be done when current_size == number
    }
  }

  /*!
   * \brief Resize the string to the given length.
   * \param[in] length Length to resize the string to.
   * \context ANY
   * \pre length <= kMaxCapacity must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void resize(size_type length) noexcept { resize(length, value_type{}); }

  /*!
   * \brief Return the total number of characters that this string can hold.
   * \return The total number of characters that this string can hold.
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
   * \brief Return the total number of characters that this string can hold.
   * \return The total number of characters that this string can hold.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  static constexpr auto max_size() noexcept -> size_type { return kMaxCapacity; }

  /*!
   * \brief Erase the contents of the string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void clear() noexcept { static_cast<void>(erase(begin(), end())); }

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
  auto empty() const noexcept -> bool { return size() == 0; }

  /*!
   * \brief Access the specified character in this string without bounds checking.
   * \details Accessing elements out of bounds (position >= size()) results in undefined behavior.
   * \param[in] position The index of the specified character.
   * \return A const_reference to the specified character.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator[](size_type position) const noexcept -> const_reference { return data_[position]; }

  /*!
   * \brief Access the specified character in this string without bounds checking.
   * \details Accessing elements out of bounds (position >= size()) results in undefined behavior.
   * \param[in] position The index of the specified character.
   * \return A reference to the specified character.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator[](size_type position) noexcept -> reference {
    // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_stringUncheckedUserElementAccess
    return data_[position];
  }

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
    if (position >= size()) {
      amsr::core::Abort("TriviallyCopyableString::at: Position is invalid!");
    }
    return operator[](position);  // VCA_ZEROCOPYCOMMON_LNG_02_STRING_VALID_RETURN_VALUE
  }

  /*!
   * \brief Access the specified character in this string.
   * \param[in] position The index of the specified character.
   * \return A reference to the specified character.
   * \context ANY
   * \pre position < size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto at(size_type position) noexcept -> reference {
    if (position >= size()) {
      amsr::core::Abort("TriviallyCopyableString::at: Position is invalid!");
    }
    return operator[](position);  // VCA_ZEROCOPYCOMMON_LNG_02_STRING_VALID_RETURN_VALUE
  }

  /*!
   * \brief Return a const reference to the first character of this string.
   * \return A const reference to the first character of this string.
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
    return operator[](0);
  }

  /*!
   * \brief Return a reference to the first character of this string.
   * \return A reference to the first character of this string.
   * \context ANY
   * \pre String must not be empty.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto front() noexcept -> reference {
    AssertStringNotEmpty();
    return operator[](0);
  }

  /*!
   * \brief Return a const reference to the last character of this string.
   * \return A const reference to the last character of this string.
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
    return operator[](size() - 1);  // VCA_ZEROCOPYCOMMON_LNG_02_STRING_VALID_RETURN_VALUE
  }

  /*!
   * \brief Return a reference to the last character of this string.
   * \return A reference to the last character of this string.
   * \context ANY
   * \pre String must not be empty.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto back() noexcept -> reference {
    AssertStringNotEmpty();
    return operator[](size() - 1);  // VCA_ZEROCOPYCOMMON_LNG_02_STRING_VALID_RETURN_VALUE
  }

  /*!
   * \brief Append a string to this string.
   * \param[in] string String to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator+=(TriviallyCopyableString const& string) & noexcept -> TriviallyCopyableString& {
    return append(string);
  }

  // VECTOR NC AutosarC++17_10-M5.17.1: MD_ZEROCOPYCOMMON_M5.17.1_stringNoOverloadOfBinaryVersion_falsePositive
  /*!
   * \brief Append a string with another capacity to this string.
   * \tparam kMaxCapacityRhs Fixed capacity of the other string.
   * \param[in] string String to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <std::size_t kMaxCapacityRhs>
  auto operator+=(TriviallyCopyableString<kMaxCapacityRhs> const& string) & noexcept ->
      typename std::enable_if<kMaxCapacity != kMaxCapacityRhs, TriviallyCopyableString&>::type {
    return append(string);
  }

  // VECTOR NC AutosarC++17_10-M5.17.1: MD_ZEROCOPYCOMMON_M5.17.1_stringNoCallRelationToBinaryVersion_falsePositive
  /*!
   * \brief Append a C string to this string.
   * \param[in] string C string to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator+=(value_type const* string) & noexcept -> TriviallyCopyableString& { return append(string); }

  /*!
   * \brief Append a character to this string.
   * \param[in] character Character to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator+=(value_type const character) & noexcept -> TriviallyCopyableString& { return append(1, character); }

  /*!
   * \brief Append an initializer list to this string.
   * \param[in] initializer_list Initializer list to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator+=(std::initializer_list<value_type> initializer_list) & noexcept -> TriviallyCopyableString& {
    return append(initializer_list.begin(), initializer_list.size());
  }

  /*!
   * \brief Append a StringView to this string.
   * \param[in] string_view StringView to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator+=(ara::core::StringView const& string_view) & noexcept -> TriviallyCopyableString& {
    return append(string_view);
  }

  /*!
   * \brief Append a string to this string.
   * \param[in] string String to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(TriviallyCopyableString const& string) noexcept -> TriviallyCopyableString& {
    return append(string.data(), string.size());
  }

  /*!
   * \brief Append a substring to this string.
   * \param[in] string String to append.
   * \param[in] position Index of the first character of the string to append.
   * \param[in] length Length of the string to append.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= string.size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(TriviallyCopyableString const& string, size_type position, size_type length = npos) noexcept
      -> TriviallyCopyableString& {
    AssertCompliesToSizeOf(position, string);
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return append(string.data() + position, std::min(length, string.size() - position));
  }

  /*!
   * \brief Append a sub C string to this string.
   * \param[in] string C string to append.
   * \param[in] length Length of the string to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \pre string must not be nullptr when length is greater than zero.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(value_type const* string, size_type length) noexcept -> TriviallyCopyableString& {
    AssertCompliesToMaxCapacity(size() + length);
    if ((string == nullptr) && (length != 0)) {
      amsr::core::Abort("TriviallyCopyableString::append: C string pointer cannot be nullptr!");
    }
    return replace(size(), static_cast<size_type>(0), string, length);
  }

  /*!
   * \brief Append a C string to this string.
   * \param[in] string C string to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(value_type const* string) noexcept -> TriviallyCopyableString& {
    if (string != nullptr) {
      static_cast<void>(append(string, traits_type::length(string)));
    }
    return *this;
  }

  /*!
   * \brief Append a character to this string.
   * \param[in] character Character to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(value_type const character) noexcept -> TriviallyCopyableString& {
    return append(TriviallyCopyableString{static_cast<size_type>(1), character});
  }

  /*!
   * \brief Append characters to this string.
   * \param[in] number Number of characters to append.
   * \param[in] character Character to append given number of times.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(size_type number, value_type const character) noexcept -> TriviallyCopyableString& {
    return append(TriviallyCopyableString{number, character});
  }

  /*!
   * \brief Append a range of characters to this string.
   * \tparam InputIterator Iterator type of the range.
   * \param[in] first Iterator referencing the first character to append.
   * \param[in] last Iterator referencing the last character to append.
   * \return Reference to this string.
   * \context ANY
   * \pre The iterators must have a comparison operator, must be created from the same object and must refer to
   *      a valid range. The caller is responsible to hold this contract, as validity of iterators cannot be
   *      fully checked. Violating this precondition will result in undefined behavior.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class InputIterator>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(InputIterator first, InputIterator last) noexcept -> TriviallyCopyableString& {
    if (first > last) {
      amsr::core::Abort("TriviallyCopyableString::append: Invalid range!");
    }
    return append(first, static_cast<size_type>(std::distance(first, last)));
  }

  /*!
   * \brief Append an initializer list to this string.
   * \param[in] initializer_list Initializer list to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(std::initializer_list<value_type> initializer_list) noexcept -> TriviallyCopyableString& {
    return append(initializer_list.begin(), initializer_list.size());
  }

  /*!
   * \brief Append a StringView to the string.
   * \param[in] string_view StringView to append.
   * \return Reference to this string.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(ara::core::StringView const string_view) noexcept -> TriviallyCopyableString& {
    append(string_view.data(), string_view.size());
    return *this;
  }

  /*!
   * \brief Append a container to the string.
   * \tparam T Container type that is convertible to a StringView.
   * \param[in] container The container to append.
   * \param[in] position Start position for reading the container.
   * \param[in] length Number of characters to read, npos to read until the end.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= container.size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <typename T, typename = std::enable_if_t<std::is_convertible<T const&, ara::core::StringView>::value>,
            typename = std::enable_if_t<!std::is_convertible<T const&, char const*>::value>>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto append(T const& container, size_type position, size_type length = npos) noexcept -> TriviallyCopyableString& {
    ara::core::StringView container_sv{container};
    AssertCompliesToSizeOf(position, container_sv);
    container_sv = container_sv.substr(position, length);
    if (!container_sv.empty()) {
      append(container_sv);
    }
    return *this;
  }

  /*!
   * \brief Append a single character to this string.
   * \param[in] character Character to append.
   * \context ANY
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void push_back(value_type character) noexcept { append(static_cast<size_type>(1), character); }

  /*!
   * \brief Assign the contents of a string into this string.
   * \param[in] string Source string.
   * \return Reference to this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(TriviallyCopyableString const& string) noexcept -> TriviallyCopyableString& {
    return assign(string, static_cast<size_type>(0), npos);
  }

  /*!
   * \brief Move-assign the contents of string into this string.
   * \param[in] string Source string.
   * \return Reference to this string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(TriviallyCopyableString&& string) noexcept -> TriviallyCopyableString& {
    swap(string);
    return *this;
  }

  /*!
   * \brief Assign the contents of a substring into this string.
   * \param[in] string Source string.
   * \param[in] position Index of the first character to assign from string.
   * \param[in] length Number of characters to assign.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= string.size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(TriviallyCopyableString const& string, size_type position, size_type length = npos) noexcept
      -> TriviallyCopyableString& {
    AssertCompliesToSizeOf(position, string);
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return assign(string.data() + position, std::min(length, string.size() - position));
  }

  /*!
   * \brief Assign the contents of a sub C string into this string.
   * \param[in] string Source C string.
   * \param[in] length Number of characters to assign.
   * \return Reference to this string.
   * \context ANY
   * \pre The string must not be nullptr when length not equal zero.
   * \pre length <= kMaxCapacity must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(value_type const* string, size_type length) noexcept -> TriviallyCopyableString& {
    AssertCompliesToMaxCapacity(length);
    return replace(static_cast<size_type>(0), size(), string, length);
  }

  /*!
   * \brief Assign the contents of a C string into this string.
   * \param[in] string Source C string.
   * \return Reference to this string.
   * \context ANY
   * \pre The string must not be nullptr.
   * \pre Size of the C string must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(value_type const* string) noexcept -> TriviallyCopyableString& {
    if (string == nullptr) {
      amsr::core::Abort("TriviallyCopyableString::assign: C string pointer cannot be nullptr!");
    }
    return assign(string, traits_type::length(string));
  }

  /*!
   * \brief Assign the contents of an initializer list into this string.
   * \param[in] initializer_list Source initializer list.
   * \return Reference to this string.
   * \context ANY
   * \pre Size of the initializer list must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(std::initializer_list<value_type> initializer_list) noexcept -> TriviallyCopyableString& {
    return assign(initializer_list.begin(), initializer_list.end());
  }

  /*!
   * \brief Fill the string with copies of the given character.
   * \param[in] number Number of characters to write to the string.
   * \param[in] character Character to assign given number of times.
   * \return Reference to this string.
   * \context ANY
   * \pre number <= kMaxCapacity must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(size_type number, value_type character) noexcept -> TriviallyCopyableString& {
    return assign(TriviallyCopyableString{number, character});
  }

  /*!
   * \brief Assign a range of characters to this string.
   * \tparam InputIterator Iterator type of the range.
   * \param[in] first Iterator referencing the first character to assign.
   * \param[in] last Iterator referencing the last character to assign.
   * \return Reference to this string.
   * \context ANY
   * \pre The input iterators must be created from the same object and must refer to
   *      a valid range. The caller is responsible to hold this contract, as validity of iterators cannot be
   *      fully checked. Violating this precondition will result in undefined behavior.
   * \pre Size of the range must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class InputIterator>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(InputIterator first, InputIterator last) noexcept -> TriviallyCopyableString& {
    return assign(first, static_cast<size_type>(std::distance(first, last)));
  }

  /*!
   * \brief Assign the contents of a StringView to the string.
   * \param[in] string_view StringView to read from.
   * \return Reference to this string.
   * \context ANY
   * \pre Size of the StringView must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(ara::core::StringView string_view) noexcept -> TriviallyCopyableString& {
    assign(string_view.data(), string_view.size());
    return *this;
  }

  /*!
   * \brief Assign the contents of a container to the string.
   * \tparam T Container type that is convertible to a StringView.
   * \param[in] container The container to assign to this string.
   * \param[in] position Start position for reading the container.
   * \param[in] length Number of characters to read, npos to read until the end.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= container.size() must be true.
   * \pre length <= kMaxCapacity must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <typename T, typename = std::enable_if_t<std::is_convertible<T const&, ara::core::StringView>::value>,
            typename = std::enable_if_t<!std::is_convertible<T const&, char const*>::value>>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto assign(T const& container, size_type position, size_type length = npos) noexcept -> TriviallyCopyableString& {
    ara::core::StringView container_sv{container};
    AssertCompliesToSizeOf(position, container_sv);
    container_sv = container_sv.substr(position, length);
    assign(container_sv.data(), container_sv.size());
    return *this;
  }

  /*!
   * \brief Insert the contents of a string into this string.
   * \param[in] position Iterator referencing the insertion position in the string.
   * \param[in] string The string to insert.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(size_type position, TriviallyCopyableString const& string) noexcept -> TriviallyCopyableString& {
    return insert(position, string.data(), string.size());
  }

  /*!
   * \brief Insert the contents of a substring into this string.
   * \tparam kMaxCapacityToInsert Fixed capacity of the string to insert.
   * \param[in] position Iterator referencing the insertion position in the string.
   * \param[in] substring The substring to insert.
   * \param[in] position_substring Index of first character in the substring to insert.
   * \param[in] length_substring Number of characters from the substring to insert.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre position_substring <= substring.size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <size_type kMaxCapacityToInsert>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(size_type position, TriviallyCopyableString<kMaxCapacityToInsert> const& substring,
              size_type position_substring, size_type length_substring = npos) noexcept -> TriviallyCopyableString& {
    AssertCompliesToSize(position);
    AssertCompliesToSizeOf(position_substring, substring);
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return insert(position, substring.data() + position_substring,
                  std::min(length_substring, substring.size() - position_substring));
  }

  /*!
   * \brief Insert the contents of a C string into this string.
   * \param[in] position Iterator referencing the insertion position in the string.
   * \param[in] string The C string to insert.
   * \param[in] length Number of characters to insert.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre string must not be nullptr when length is greater than zero.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(size_type position, value_type const* string, size_type length) noexcept -> TriviallyCopyableString& {
    if ((string == nullptr) && (length > 0)) {
      amsr::core::Abort("TriviallyCopyableString::insert: C string pointer cannot be nullptr!");
    }
    return replace(position, static_cast<size_type>(0), string, length);
  }

  /*!
   * \brief Insert the contents of a C string into this string.
   * \param[in] position Iterator referencing the insertion position in the string.
   * \param[in] string The C string to insert.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(size_type position, value_type const* string) noexcept -> TriviallyCopyableString& {
    return insert(position, string, (string == nullptr) ? 0 : traits_type::length(string));
  }

  /*!
   * \brief Insert consecutive copies of a given character into this string.
   * \param[in] position Iterator referencing the insertion position in the string.
   * \param[in] number Number of characters to insert.
   * \param[in] character Character to insert given number of times.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(size_type position, size_type number, value_type character) noexcept -> TriviallyCopyableString& {
    return insert(position, TriviallyCopyableString{number, character});
  }

  /*!
   * \brief Insert a single character into this string.
   * \param[in] position Const iterator referencing the insertion position in the string.
   * \param[in] character The character to insert.
   * \return Iterator referencing the inserted character.
   * \context ANY
   * \pre The position must be a valid iterator on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(const_iterator position, value_type character) noexcept -> iterator {
    AssertIsValidIterator(position);
    // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
    size_type const pos{static_cast<size_type>(position - begin())};
    replace(position, position, static_cast<size_type>(1), character);
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return iterator{DataLocal() + pos};
  }

  /*!
   * \brief Insert consecutive copies of a given character into this string.
   * \param[in] position Const iterator referencing the insertion position in the string.
   * \param[in] number Number of characters to insert.
   * \param[in] character Character to insert.
   * \return Iterator referencing the first inserted character.
   * \context ANY
   * \pre The position must be a valid iterator on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(const_iterator position, size_type number, value_type character) noexcept -> iterator {
    AssertIsValidIterator(position);
    // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
    size_type const index_position{static_cast<size_type>(position - begin())};
    replace(position, position, number, character);
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return iterator{DataLocal() + index_position};
  }

  /*!
   * \brief Insert a range of characters into this string.
   * \tparam InputIterator Iterator type of the range.
   * \param[in] position Const iterator referencing the insertion position in the string.
   * \param[in] first Start of the range.
   * \param[in] last End of the range.
   * \return Iterator referencing the first inserted character.
   * \context ANY
   * \pre The position must be a valid iterator on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \pre The input iterators must be created from the same object and must refer to
   *      a valid range. The caller is responsible to hold this contract, as validity of iterators
   *      cannot be fully checked. Violating this precondition will result in undefined behavior.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class InputIterator>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(const_iterator position, InputIterator first, InputIterator last) noexcept -> iterator {
    AssertIsValidIterator(position);
    // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
    size_type const pos{static_cast<size_type>(position - begin())};
    replace(position, position, first, last);  // VCA_ZEROCOPYCOMMON_LNG_01_STRING_VALID_ARGUMENT
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return iterator{DataLocal() + pos};
  }

  /*!
   * \brief Insert a range of characters into this string.
   * \param[in] position Const iterator referencing the insertion position in the string.
   * \param[in] initializer_list Initializer list to insert.
   * \return Iterator referencing the first inserted character.
   * \context ANY
   * \pre The position must be a valid iterator on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(const_iterator position, std::initializer_list<value_type> initializer_list) noexcept -> iterator {
    AssertIsValidIterator(position);
    return insert(position, initializer_list.begin(), initializer_list.end());
  }

  /*!
   * \brief Insert the contents of a StringView into this string.
   * \param[in] position Insertion position in the string.
   * \param[in] string_view StringView to insert from.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(size_type position, ara::core::StringView string_view) noexcept -> TriviallyCopyableString& {
    AssertCompliesToSize(position);
    if (!string_view.empty()) {
      insert(position, string_view.data(), string_view.size());
    }
    return *this;
  }

  /*!
   * \brief Insert a substring of a container into this string.
   * \tparam T Container type that is convertible to a StringView.
   * \param[in] position Insertion position in the string.
   * \param[in] insertion The container to insert into this string.
   * \param[in] position_insertion Position to start reading the insertion from.
   * \param[in] length_insertion Number of characters to read from the insertion, npos to read until the end.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre position_insertion <= insertion.size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <typename T, typename = std::enable_if_t<std::is_convertible<T const&, ara::core::StringView>::value>,
            typename = std::enable_if_t<!std::is_convertible<T const&, char const*>::value>>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(size_type position, T const& insertion, size_type position_insertion,
              size_type length_insertion = npos) noexcept -> TriviallyCopyableString& {
    AssertCompliesToSize(position);
    ara::core::StringView const insertion_sv{insertion};
    AssertCompliesToSizeOf(position_insertion, insertion_sv);
    return insert(position, insertion_sv.substr(position_insertion, length_insertion));
  }

  /*!
   * \brief Remove characters from this string.
   * \param[in] position Index of the first character to remove.
   * \param[in] length Number of characters to remove.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto erase(size_type position = 0, size_type length = npos) noexcept -> TriviallyCopyableString& {
    AssertCompliesToSize(position);
    if (length > 0) {
      size_type const num_chars_to_remove{std::min(length, size() - position)};
      size_type const num_trailing_chars{size() - position - num_chars_to_remove};
      if (num_trailing_chars != 0) {
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
        MoveOptimized(DataLocal() + position, data() + position + num_chars_to_remove, num_trailing_chars);
      }
      SetLength(size() - num_chars_to_remove);
    }
    return *this;
  }

  /*!
   * \brief Remove one character from this string.
   * \param[in] position Iterator referencing the character to remove.
   * \return Iterator referencing the same location after removal.
   * \context ANY
   * \pre position must be a valid iterator on this string.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto erase(const_iterator position) noexcept -> iterator {
    AssertIsValidIterator(position);

    iterator return_iterator{};
    if (size() == 0) {
      return_iterator = end();
    } else {
      // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
      size_type const index{static_cast<size_type>(position - begin())};
      size_type const num_chars_to_remove{1};
      size_type const num_trailing_chars{size() - index - num_chars_to_remove};
      if (num_trailing_chars > 0) {
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
        MoveOptimized(DataLocal() + index, data() + index + num_chars_to_remove, num_trailing_chars);
      }
      SetLength(size() - num_chars_to_remove);
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
      return_iterator = iterator{DataLocal() + index};
    }
    return return_iterator;
  }

  /*!
   * \brief Remove a range of characters from this string.
   * \param[in] first Iterator referencing the first character to remove.
   * \param[in] last Iterator referencing the last character to remove.
   * \return Iterator referencing location of the first character after removal.
   * \context ANY
   * \pre first and last must be valid iterators on this string.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto erase(const_iterator first, const_iterator last) noexcept -> iterator {
    AssertIsValidIteratorRange(first, last);

    iterator return_iterator{};
    if (size() == 0) {
      return_iterator = end();
    } else {
      // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
      size_type const pos{static_cast<size_type>(first - begin())};
      if (last == end()) {
        SetLength(pos);
      } else {
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        size_type const distance{static_cast<size_type>(last - first)};
        size_type const num_trailing_chars{size() - pos - distance};
        if (distance != 0) {
          // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
          MoveOptimized(DataLocal() + pos, data() + pos + distance, num_trailing_chars);
        }
        SetLength(size() - distance);
      }
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
      return_iterator = iterator{DataLocal() + pos};
    }
    return return_iterator;
  }

  /*!
   * \brief Remove the last character from this string.
   * \context ANY
   * \pre The string must not be empty when calling this function.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void pop_back() noexcept {
    AssertStringNotEmpty();
    erase(size() - 1);
  }

  /*!
   * \brief Replace the contents of this string from a string.
   * \param[in] position Index of the first character to replace.
   * \param[in] length Number of characters to replace.
   * \param[in] string String to replace from.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(size_type position, size_type length, TriviallyCopyableString const& string) noexcept
      -> TriviallyCopyableString& {
    return replace(position, length, string.data(), string.size());
  }

  /*!
   * \brief Replace the contents of this string from a substring.
   * \param[in] position Index of the first character to replace.
   * \param[in] length Number of characters to replace.
   * \param[in] replacement String to replace from.
   * \param[in] position_replacement Index of the first character of replacement to replace from.
   * \param[in] length_replacement Number of characters which get inserted for the replaced characters.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre position_replacement <= replacement.size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(size_type position, size_type length, TriviallyCopyableString const& replacement,
               size_type position_replacement, size_type length_replacement = npos) noexcept
      -> TriviallyCopyableString& {
    AssertCompliesToSize(position);
    AssertCompliesToSizeOf(position_replacement, replacement);
    size_type const effective_length{std::min(length_replacement, replacement.size() - position_replacement)};
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return replace(position, length, replacement.data() + position_replacement, effective_length);
  }

  /*!
   * \brief Replace the contents of this string from a C string.
   * \param[in] position Index of the first character to replace.
   * \param[in] length Number of characters which get replaced.
   * \param[in] replacement C string to replace from.
   * \param[in] length_replacement Number of characters which get inserted for the replaced characters.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre replacement must not be nullptr when length_replacement is greater than zero.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(size_type position, size_type length, value_type const* replacement,
               size_type length_replacement) noexcept -> TriviallyCopyableString& {
    AssertCompliesToSize(position);
    if ((replacement == nullptr) && (length_replacement != 0)) {
      amsr::core::Abort("TriviallyCopyableString::replace: C string pointer cannot be nullptr!");
    }
    size_type const num_chars_to_replace{std::min(length, size() - position)};
    size_type const tail_length{size() - position - num_chars_to_replace};
    size_type const length_after_replace{(size() + length_replacement) - num_chars_to_replace};

    AssertCompliesToMaxCapacity(length_after_replace);
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    pointer const replacement_start_pointer{DataLocal() + position};
    ReplaceTextBlockByAnother(replacement_start_pointer, num_chars_to_replace, tail_length, replacement,
                              length_replacement);
    SetLength(length_after_replace);
    return *this;
  }

  /*!
   * \brief Replace the contents of this string from a C string.
   * \param[in] position Index of the first character to replace.
   * \param[in] length Number of characters which get replaced.
   * \param[in] string C string to replace from.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(size_type position, size_type length, value_type const* string) noexcept -> TriviallyCopyableString& {
    return replace(position, length, string, (string == nullptr) ? 0 : traits_type::length(string));
  }

  /*!
   * \brief Replace a range with a given number of consecutive copies of a character.
   * \param[in] position Index of the first character to replace.
   * \param[in] num_chars_to_remove Number of characters which get replaced.
   * \param[in] num_chars_to_insert Number of characters which get inserted for the replaced characters.
   * \param[in] character Character to replace from.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(size_type position, size_type num_chars_to_remove, size_type num_chars_to_insert,
               value_type character) noexcept -> TriviallyCopyableString& {
    return replace(position, num_chars_to_remove, TriviallyCopyableString(num_chars_to_insert, character));
  }

  /*!
   * \brief Replace a range of this string with a given string.
   * \param[in] iterator_begin Iterator referencing the begin of the range to replace.
   * \param[in] iterator_end Iterator referencing the end of the range to replace.
   * \param[in] replacement String to replace from.
   * \return Reference to this string.
   * \context ANY
   * \pre iterator_begin and iterator_end must be a valid range on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(const_iterator iterator_begin, const_iterator iterator_end,
               TriviallyCopyableString const& replacement) noexcept -> TriviallyCopyableString& {
    AssertIsValidIteratorRange(iterator_begin, iterator_end);

    return replace(
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(iterator_begin - begin()),
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(iterator_end - iterator_begin), replacement);
  }

  /*!
   * \brief Replace a range of this string with a range of a C string.
   * \param[in] iterator_begin Iterator referencing the begin of the range to replace.
   * \param[in] iterator_end Iterator referencing the end of the range to replace.
   * \param[in] replacement C string to replace from.
   * \param[in] length_replacement Number of characters to insert.
   * \return Reference to this string.
   * \context ANY
   * \pre iterator_begin and iterator_end must be a valid range on this string.
   * \pre replacement must not be nullptr when length_replacement is greater than zero.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(const_iterator iterator_begin, const_iterator iterator_end, value_type const* replacement,
               size_type length_replacement) noexcept -> TriviallyCopyableString& {
    AssertIsValidIteratorRange(iterator_begin, iterator_end);

    return replace(
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(static_cast<size_type>(iterator_begin - begin())),
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(iterator_end - iterator_begin), replacement, length_replacement);
  }

  /*!
   * \brief Replace the contents of this string from a range of characters of a C string.
   * \param[in] iterator_begin Iterator referencing the begin of the range to replace.
   * \param[in] iterator_end Iterator referencing the end of the range to replace.
   * \param[in] replacement C string to replace from.
   * \return Reference to this string.
   * \context ANY
   * \pre iterator_begin and iterator_end must be a valid range on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(const_iterator iterator_begin, const_iterator iterator_end, value_type const* replacement) noexcept
      -> TriviallyCopyableString& {
    AssertIsValidIteratorRange(iterator_begin, iterator_end);

    return replace(
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(static_cast<size_type>(iterator_begin - begin())),
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(iterator_end - iterator_begin), replacement,
        (replacement != nullptr) ? traits_type::length(replacement) : 0);
  }

  /*!
   * \brief Replace the contents of this string from characters.
   * \param[in] iterator_begin Iterator referencing the begin of the range to replace.
   * \param[in] iterator_end Iterator referencing the end of the range to replace.
   * \param[in] number Number of characters to replace.
   * \param[in] character Character to replace from.
   * \return Reference to this string.
   * \context ANY
   * \pre iterator_begin and iterator_end must be a valid range on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(const_iterator iterator_begin, const_iterator iterator_end, size_type number,
               value_type character) noexcept -> TriviallyCopyableString& {
    AssertIsValidIteratorRange(iterator_begin, iterator_end);

    return replace(
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(static_cast<size_type>(iterator_begin - begin())),
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(iterator_end - iterator_begin), TriviallyCopyableString(number, character));
  }

  /*!
   * \brief Replace a given range of this string with a range of characters.
   * \tparam InputIterator Iterator type of the range.
   * \param[in] iterator_begin Iterator referencing the begin of the range to replace.
   * \param[in] iterator_end Iterator referencing the end of the range to replace.
   * \param[in] replacement_iterator_begin Iterator referencing the begin of the range to replace from.
   * \param[in] replacement_iterator_end Iterator referencing the end of the range to replace from.
   * \return Reference to this string.
   * \context ANY
   * \pre iterator_begin and iterator_end must be a valid range on this string.
   * \pre replacement_iterator_begin and replacement_iterator_end must be a valid range created from the same object.
   *      The caller is responsible to hold this contract, as validity of iterators cannot be fully checked.
   *      Violating this precondition will result in undefined behavior.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class InputIterator>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(const_iterator iterator_begin, const_iterator iterator_end, InputIterator replacement_iterator_begin,
               InputIterator replacement_iterator_end) noexcept -> TriviallyCopyableString& {
    AssertIsValidIteratorRange(iterator_begin, iterator_end);

    return replace(
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(static_cast<size_type>(iterator_begin - begin())),
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(iterator_end - iterator_begin),
        TriviallyCopyableString(replacement_iterator_begin, replacement_iterator_end));
  }

  /*!
   * \brief Replace the contents of this string from an initializer list.
   * \param[in] iterator_begin Iterator referencing the begin of the range to replace.
   * \param[in] iterator_end Iterator referencing the end of the range to replace.
   * \param[in] initializer_list Initializer list to replace from.
   * \return Reference to this string.
   * \context ANY
   * \pre iterator_begin and iterator_end must be a valid range on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(const_iterator iterator_begin, const_iterator iterator_end,
               std::initializer_list<value_type> initializer_list) noexcept -> TriviallyCopyableString& {
    AssertIsValidIteratorRange(iterator_begin, iterator_end);

    return replace(
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(static_cast<size_type>(iterator_begin - begin())),
        // VECTOR NL AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
        static_cast<size_type>(iterator_end - iterator_begin), initializer_list.begin(), initializer_list.size());
  }

  /*!
   * \brief Replace part of the string with the contents of a StringView.
   * \param[in] position Position in the string to replace from.
   * \param[in] length Number of characters to replace.
   * \param[in] replacement StringView to read from.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(size_type position, size_type length, ara::core::StringView replacement) noexcept
      -> TriviallyCopyableString& {
    replace(position, length, replacement.data(), replacement.size());
    return *this;
  }

  /*!
   * \brief Replace part of the string with the contents of a container.
   * \tparam T Container type that is convertible to a StringView.
   * \param[in] position Position in the string to replace from.
   * \param[in] length Number of characters to replace.
   * \param[in] replacement Container to read replacement from.
   * \param[in] position_replacement Position to start reading the replacement from.
   * \param[in] length_replacement Number of characters to read from the replacement, npos to read until the end.
   * \return Reference to this string.
   * \context ANY
   * \pre position <= size() must be true.
   * \pre position_replacement <= replacement.size() must be true.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <typename T, typename = std::enable_if_t<std::is_convertible<T const&, ara::core::StringView>::value>,
            typename = std::enable_if_t<!std::is_convertible<T const&, char const*>::value>>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(size_type position, size_type length, T const& replacement, size_type position_replacement,
               size_type length_replacement = npos) noexcept -> TriviallyCopyableString& {
    AssertCompliesToSize(position);
    ara::core::StringView const replacement_sv{replacement};
    AssertCompliesToSizeOf(position_replacement, replacement_sv);
    return replace(position, length, replacement_sv.substr(position_replacement, length_replacement));
  }

  /*!
   * \brief Replace an iterator-bounded part of the string with the contents of a StringView.
   * \param[in] iterator_begin Iterator to the start of the replacement range.
   * \param[in] iterator_end Iterator to the end of the replacement range.
   * \param[in] replacement StringView to read from.
   * \return Reference to this string.
   * \context ANY
   * \pre iterator_begin and iterator_end must be valid iterators on this string.
   * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto replace(const_iterator iterator_begin, const_iterator iterator_end, ara::core::StringView replacement) noexcept
      -> TriviallyCopyableString& {
    AssertIsValidIteratorRange(iterator_begin, iterator_end);

    // VECTOR NC AutosarC++17_10-M5.0.9: MD_ZEROCOPYCOMMON_M5.0.9_stringExplicitConversionChangesSignedness
    replace(static_cast<size_type>(iterator_begin - begin()), static_cast<size_type>(iterator_end - iterator_begin),
            replacement.data(), replacement.size());
    return *this;
  }

  /*!
   * \brief Copy substring into the C string.
   * \param[in] string Destination C string.
   * \param[in] length Number of characters to be copied.
   * \param[in] position Index of the first character to copy.
   * \return Number of characters being copied.
   * \context ANY
   * \pre string must not be nullptr.
   * \pre position <= size() must be true.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto copy(value_type* string, size_type length, size_type position = 0) const noexcept -> size_type {
    if (string == nullptr) {
      amsr::core::Abort("TriviallyCopyableString::copy: C string pointer cannot be nullptr!");
    }
    AssertCompliesToSize(position);
    size_type const num_chars_to_copy{std::min(length, size() - position)};
    if (num_chars_to_copy > 0) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
      CopyOptimized(string, DataLocal() + position, num_chars_to_copy);
    }
    return num_chars_to_copy;
  }

  /*!
   * \brief Swap the contents with a string.
   * \param[in] string String to swap with.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void swap(TriviallyCopyableString& string) noexcept { std::swap(*this, string); }

  /*!
   * \brief Return const pointer to contents.
   * \return Const pointer to contents.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto c_str() const noexcept -> const_pointer { return DataLocal(); }

  /*!
   * \brief Return const pointer to contents.
   * \return Const pointer to contents.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto data() const noexcept -> const_pointer { return DataLocal(); }

  /*!
   * \brief Find position of a substring in this string.
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
  auto find(TriviallyCopyableString const& search_string, size_type position = 0) const noexcept -> size_type {
    return find(search_string.data(), position, search_string.size());
  }

  /*!
   * \brief Find position of a C string in this string.
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
   * \brief Find position of a C string in this string.
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
   * \brief Find position of a character in this string.
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
   * \brief Find the first substring equal to a StringView.
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
   * \brief Find last position of a string in this string.
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
  auto rfind(TriviallyCopyableString const& search_string, size_type position = npos) const noexcept -> size_type {
    return rfind(ara::core::StringView{search_string}, position);
  }

  /*!
   * \brief Find last position of a C string in this string.
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
   * \brief Find last position of a C string in this string.
   * \param[in] search_string String to search for.
   * \param[in] position Index of character in this string to search from.
   * \return Index of last occurrence, or npos if there are no occurrences or search_string is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rfind(value_type const* search_string, size_type position = npos) const noexcept -> size_type {
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
  auto rfind(value_type character, size_type position = npos) const noexcept -> size_type {
    return rfind(ara::core::StringView{&character, 1}, position);
  }

  /*!
   * \brief Find the last substring equal to a StringView.
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
  auto rfind(ara::core::StringView search_string, size_type position = npos) const noexcept -> size_type {
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
  auto find_first_of(TriviallyCopyableString const& search_set, size_type position = 0) const noexcept -> size_type {
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
   * \brief Find the first character equal to one of the given characters.
   * \param[in] character Character that shall be matched.
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
   * \return Index of first occurrence, or npos if there are no occurrences.
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
  auto find_last_of(TriviallyCopyableString const& search_set, size_type position = npos) const noexcept -> size_type {
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
  auto find_last_of(value_type const* search_set, size_type position = npos) const noexcept -> size_type {
    return find_last_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the last character equal to one of the given characters.
   * \param[in] character Character that shall be matched.
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
  auto find_last_of(value_type character, size_type position = npos) const noexcept -> size_type {
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
  auto find_last_of(ara::core::StringView search_set, size_type position = npos) const noexcept -> size_type {
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
  auto find_first_not_of(TriviallyCopyableString const& search_set, size_type position = 0) const noexcept
      -> size_type {
    return find_first_not_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the first character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \param[in] length Number of characters in this string to search for.
   * \return Index of first character that does not match, or npos if no such characters were found
   *         or search_set is nullptr.
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
   * \return Index of first character that does not match, or npos if no such characters were found
   *         or search_set is nullptr.
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
  auto find_last_not_of(TriviallyCopyableString const& search_set, size_type position = npos) const noexcept
      -> size_type {
    return find_last_not_of(ara::core::StringView{search_set}, position);
  }

  /*!
   * \brief Find the last character not equal to any of the given characters.
   * \param[in] search_set Set of characters that shall not be matched.
   * \param[in] position Index of character in this string to search from.
   * \param[in] length Number of characters in this string to search for.
   * \return Index of last character that does not match, or npos if no such characters were found
   *         or search_set is nullptr.
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
   * \return Index of last character that does not match, or npos if no such characters were found
   *         or search_set is nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto find_last_not_of(value_type const* search_set, size_type position = npos) const noexcept -> size_type {
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
  auto find_last_not_of(value_type character, size_type position = npos) const noexcept -> size_type {
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
  auto find_last_not_of(ara::core::StringView search_set, size_type position = npos) const noexcept -> size_type {
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
  auto substr(size_type position = 0, size_type length = npos) const noexcept -> TriviallyCopyableString {
    AssertCompliesToSize(position);
    size_type const effective_length{std::min(length, size() - position)};
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    return TriviallyCopyableString{data() + position, effective_length};
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
  auto compare(TriviallyCopyableString const& string) const noexcept -> int {
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
  auto compare(size_type position, size_type length, TriviallyCopyableString const& string) const noexcept -> int {
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
  auto compare(size_type position, size_type length, TriviallyCopyableString const& substring,
               size_type position_substring, size_type length_substring = npos) const noexcept -> int {
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
    amsr::core::StringView const string_sv{string, length_string};
    return amsr::core::StringView{*this}.substr(position, length).compare(string_sv);
  }

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_ZEROCOPYCOMMON_A3.9.1_stringBaseTypeRequiredBySpecification
  /*!
   * \brief Compare the given StringView to this string.
   * \param[in] string_view StringView to compare to.
   * \return Negative value if the string is lexicographically before the string_view parameter, positive value if the
   *         string is after the string_view parameter, and 0 if they are equal.
   * \context ANY
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
  auto compare(size_type position, size_type length, ara::core::StringView const& string_view) const noexcept -> int {
    AssertCompliesToSize(position);
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
               size_type length_substring = npos) const noexcept
      -> std::enable_if_t<(std::is_convertible<T const&, ara::core::StringView>::value) &&
                              (!std::is_convertible<T const&, char const*>::value),
                          int> {
    AssertCompliesToSize(position);
    ara::core::StringView substring_sv{substring};
    AssertCompliesToSizeOf(position_substring, substring_sv);
    substring_sv = substring_sv.substr(position_substring, length_substring);
    return ara::core::StringView{*this}.substr(position, length).compare(substring_sv);
  }

  /*!
   * \brief Provide access to the contiguous memory region where elements are stored.
   * \details The returned pointer can be interpreted as an array of size kMaxCapacity + 1.
   * \return Reference to the statically sized array where elements are stored in.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto ElementData() const noexcept -> amsr::core::Array<char, kMaxCapacity + 1> const& { return data_; }

  /*!
   * \brief Provide access to the management information of this string (i.e. its size).
   * \return Constant reference to the string's size.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto ManagementData() const noexcept -> size_type const& { return size_; }

 private:
  /*!
   * \brief Replace the first characters given by length with the replacement of given length.
   * \details Everything following the part to replace (i.e. the tail) is moved forth or back so that the
   *          given replacement can be copied into the space between position and tail.
   *          The functionality can be imagined as a delete-insert operation.
   *          The part to be replaced and the replacement may overlap.
   * \param[in] position Pointer to the first character to replace.
   * \param[in] length Number of characters which get replaced.
   * \param[in] tail_length Number of characters which are left after the ones to be replaced.
   * \param[in] replacement C string to replace from.
   * \param[in] length_replacement Number of characters which get inserted for the replaced characters.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void ReplaceTextBlockByAnother(pointer const& position, size_type length, size_type tail_length,
                                 value_type const* replacement, size_type length_replacement) const noexcept {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    pointer const tail_begin_origin{position + length};
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    pointer const tail_begin_new{position + length_replacement};

    if (Disjunct(replacement, length_replacement)) {
      MoveOptimized(tail_begin_new, tail_begin_origin, tail_length);
      CopyOptimized(position, replacement, length_replacement);
    } else {
      // If replacement is inside the area to replace, it needs to be copied beforehand to avoid overwriting it
      TriviallyCopyableString const saved_replacement{replacement, length_replacement};
      MoveOptimized(tail_begin_new, tail_begin_origin, tail_length);
      CopyOptimized(position, saved_replacement.data(), length_replacement);
    }
  }

  /*!
   * \brief Wrapper function to construct a string from a given range.
   * \tparam InputIterator Iterator data type.
   * \param[in] begin Iterator referencing the start of the range.
   * \param[in] end Iterator referencing the end of the range.
   * \context ANY
   * \pre begin and end must be a valid range created from the same object.
   *      The caller is responsible to hold this contract, as validity of iterators cannot be fully checked.
   *      Violating this precondition will result in undefined behavior.
   * \pre The number of elements in the range defined by begin and end must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  template <typename InputIterator>
  void ConstructFromIterator(InputIterator begin, InputIterator end) noexcept {
    ConstructFromIterator(begin, end, typename std::iterator_traits<InputIterator>::iterator_category{});
  }

  /*!
   * \brief Construct a string from a given range if iterator type is input iterator.
   * \tparam InputIterator Iterator data type.
   * \param[in] begin Iterator referencing the start of the range.
   * \param[in] end Iterator referencing the end of the range.
   * \context ANY
   * \pre begin and end must be a valid range created from the same object.
   *      The caller is responsible to hold this contract, as validity of iterators cannot be fully checked.
   *      Violating this precondition will result in undefined behavior.
   * \pre The number of elements in the range defined by begin and end must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  template <typename InputIterator>
  void ConstructFromIterator(InputIterator begin, InputIterator end, std::input_iterator_tag) noexcept {
    size_type len{0};
    while ((begin != end)) {
      if (len >= kMaxCapacity) {
        // The abort message refers to the constructor since this function is only called from it,
        // and it is more helpful to the user.
        amsr::core::Abort(
            "TriviallyCopyableString::TriviallyCopyableString: The number of "
            "characters cannot exceed the maximum!");
      }
      data_[len++] = *begin;
      ++begin;  // VCA_ZEROCOPYCOMMON_SPC_15_STRING_FUNC_FULFILL_CONTRACT
    }
    SetLength(len);
  }

  /*!
   * \brief Construct a string from a given range if iterator type is forward iterator.
   * \tparam InputIterator Iterator data type.
   * \param[in] begin Iterator referencing the start of the range.
   * \param[in] end Iterator referencing the end of the range.
   * \context ANY
   * \pre begin and end must be a valid range created from the same object.
   *      The caller is responsible to hold this contract, as validity of iterators cannot be fully checked.
   *      Violating this precondition will result in undefined behavior.
   * \pre The number of elements in the range defined by begin and end must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  template <typename InputIterator>
  void ConstructFromIterator(InputIterator begin, InputIterator end, std::forward_iterator_tag) noexcept {
    size_type const distance{static_cast<size_type>(std::distance(begin, end))};
    AssertCompliesToMaxCapacity(distance);
    CopyOptimizedItr(DataLocal(), begin, end);
    SetLength(distance);
  }

  /*!
   * \brief Wrapper function to construct a string from a number of copies of a character.
   * \param[in] number Number of characters to copy to the string.
   * \param[in] character Character to construct the string from.
   * \context ANY
   * \pre The number of characters must not exceed kMaxCapacity.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void ConstructStrInt(size_type number, value_type character) noexcept {
    if (number > 0) {
      AssignOptimized(DataLocal(), number, character);
    }
    SetLength(number);
  }

  /*!
   * \brief Optimized copy function.
   * \param[in] str_to Pointer to a character string to copy to.
   * \param[in] str_from Pointer to a character string to copy from.
   * \param[in] number Number of characters to copy.
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
   * \brief Optimized copy function for iterators.
   * \tparam Iterator Iterator data type.
   * \param[in] target Pointer to a character string to copy to.
   * \param[in] iterator_begin Iterator referencing the start of the range.
   * \param[in] iterator_end Iterator referencing the end of the range.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  template <typename Iterator>
  static void CopyOptimizedItr(value_type* target, Iterator iterator_begin, Iterator iterator_end) noexcept {
    while (iterator_begin != iterator_end) {
      traits_type::assign(*target, *iterator_begin);  // VCA_ZEROCOPYCOMMON_LNG_01_STRING_VALID_ARGUMENT
      // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_stringPointerArithmetic
      ++iterator_begin;
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
      ++target;
    }
  }

  /*!
   * \brief Optimized move function.
   * \param[in] character_to Pointer to a character string to move to.
   * \param[in] str_from Pointer to a character string to move from.
   * \param[in] number Number of characters to move.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static void MoveOptimized(value_type* character_to, value_type const* str_from, size_type number) noexcept {
    static_cast<void>(traits_type::move(character_to, str_from, number));
  }

  /*!
   * \brief Optimized assign function.
   * \param[in] target Target to assign given character to.
   * \param[in] number Number of times to assign given character.
   * \param[in] character C string to assign from.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static void AssignOptimized(value_type* target, size_type number, value_type character) noexcept {
    static_cast<void>(traits_type::assign(target, number, character));
  }

  /*!
   * \brief Set the length of this string and ensure that it is zero-terminated.
   * \param[in] length Length.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void SetLength(size_type length) noexcept {
    size_ = length;
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic
    traits_type::assign((DataLocal()[length]),  // VCA_ZEROCOPYCOMMON_LNG_01_STRING_VALID_ARGUMENT
                        value_type{});
  }

  /*!
   * \brief Check if the given C string does not overlap with this string.
   * \param[in] other C string to check if it overlaps with this string.
   * \param[in] length Length of the C string to check.
   * \return Return true if the C string does not overlap with this string, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto Disjunct(value_type const* other, size_type length) const noexcept -> bool {
    bool const other_is_after{other > end()};

    // VECTOR NL AutosarC++17_10-M5.0.15, AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.15_stringPointerArithmetic, MD_ZEROCOPYCOMMON_M5.0.16_stringPointerArithmetic
    bool const other_is_before{(other + length) < begin()};

    return (other_is_after || other_is_before);
  }

  /*!
   * \brief Assert that the given iterator is valid on this string.
   * \details Aborts if the assertion fails.
   * \param[in] it Iterator to check for validity.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertIsValidIterator(const_iterator it) const noexcept {
    // VECTOR NL AutosarC++17_10-M5.0.18: MD_ZEROCOPYCOMMON_M5.0.18_stringPointerComparisonDetectInvalidIterators
    if ((it < begin()) || (it > end())) {
      amsr::core::Abort("TriviallyCopyableString: Position must be a valid iterator on this string!");
    }
  }

  /*!
   * \brief Assert that the given iterators define a valid range on this string.
   * \details Aborts if the assertion fails.
   * \param[in] begin Begin of range to check.
   * \param[in] end End of range to check.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertIsValidIteratorRange(const_iterator begin, const_iterator end) const noexcept {
    AssertIsValidIterator(begin);
    AssertIsValidIterator(end);
    if (begin > end) {
      amsr::core::Abort("TriviallyCopyableString: Iterators must define a valid range on this string!");
    }
  }

  /*!
   * \brief Assert that the given size does not exceed the maximum capacity of this string.
   * \details Aborts if the assertion fails.
   * \param[in] size Size to check.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  static void AssertCompliesToMaxCapacity(size_type const size) noexcept {
    if (size > kMaxCapacity) {
      amsr::core::Abort("TriviallyCopyableString: The number of characters cannot exceed the maximum!");
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
      amsr::core::Abort("TriviallyCopyableString: Position is invalid!");
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
      amsr::core::Abort("TriviallyCopyableString: Position is invalid!");
    }
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
      amsr::core::Abort("TriviallyCopyableString: The string cannot be empty!");
    }
  }

  /*!
   * \brief Return pointer to contents.
   * \return Pointer to contents.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto DataLocal() noexcept -> pointer { return data_.data(); }

  /*!
   * \brief Return const pointer to contents.
   * \return Const pointer to contents.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto DataLocal() const noexcept -> const_pointer { return data_.data(); }

  /*!
   * \brief Size data member of this string.
   */
  size_type size_{};

  /*!
   * \brief Local buffer for string.
   */
  amsr::core::Array<char, kMaxCapacity + 1> data_{};
};

/*!
 * \brief Concatenate two strings.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs First string.
 * \param[in] rhs Second string.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(TriviallyCopyableString<kMaxCapacity> const& lhs,
               TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept -> TriviallyCopyableString<kMaxCapacity> {
  TriviallyCopyableString<kMaxCapacity> str{lhs};
  str += rhs;
  return str;
}

/*!
 * \brief Concatenate two strings with different capacity.
 * \tparam kMaxCapacityLhs Fixed capacity of the lhs string.
 * \tparam kMaxCapacityRhs Fixed capacity of the rhs string.
 * \param[in] lhs First string.
 * \param[in] rhs Second string.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacityLhs.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacityLhs, std::size_t kMaxCapacityRhs>
auto operator+(TriviallyCopyableString<kMaxCapacityLhs> const& lhs,
               TriviallyCopyableString<kMaxCapacityRhs> const& rhs) noexcept ->
    typename std::enable_if<kMaxCapacityLhs != kMaxCapacityRhs, TriviallyCopyableString<kMaxCapacityLhs>>::type {
  TriviallyCopyableString<kMaxCapacityLhs> str{lhs};
  str += rhs;
  return str;
}

// VECTOR NC AutosarC++17_10-M5.17.1: MD_ZEROCOPYCOMMON_M5.17.1_stringNoCallRelationToAssignmentVersion_falsePositive
/*!
 * \brief Concatenate a string with a C string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs C string.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(TriviallyCopyableString<kMaxCapacity> const& lhs,
               typename TriviallyCopyableString<kMaxCapacity>::value_type const* rhs) noexcept
    -> TriviallyCopyableString<kMaxCapacity> {
  TriviallyCopyableString<kMaxCapacity> str{lhs};
  str += rhs;
  return str;
}

/*!
 * \brief Concatenate a string with a character.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs Character.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(TriviallyCopyableString<kMaxCapacity> const& lhs,
               typename TriviallyCopyableString<kMaxCapacity>::value_type const rhs) noexcept
    -> TriviallyCopyableString<kMaxCapacity> {
  TriviallyCopyableString<kMaxCapacity> str{lhs};
  str += rhs;
  return str;
}

/*!
 * \brief Concatenate a string with an initializer list.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs Initializer list.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(TriviallyCopyableString<kMaxCapacity> const& lhs,
               std::initializer_list<typename TriviallyCopyableString<kMaxCapacity>::value_type> const& rhs) noexcept
    -> TriviallyCopyableString<kMaxCapacity> {
  TriviallyCopyableString<kMaxCapacity> str{lhs};
  str += rhs;
  return str;
}

/*!
 * \brief Concatenate a string with a StringView.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs String.
 * \param[in] rhs StringView.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(TriviallyCopyableString<kMaxCapacity> const& lhs, ara::core::StringView const& rhs) noexcept
    -> TriviallyCopyableString<kMaxCapacity> {
  TriviallyCopyableString<kMaxCapacity> str{lhs};
  str += rhs;
  return str;
}

// VECTOR NC AutosarC++17_10-M5.17.1: MD_ZEROCOPYCOMMON_M5.17.1_stringNoOverloadOfAssignmentVersion_falsePositive
/*!
 * \brief Concatenate a C string and a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs C string.
 * \param[in] rhs String.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(char const* lhs, TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept
    -> TriviallyCopyableString<kMaxCapacity> {
  TriviallyCopyableString<kMaxCapacity> str{lhs, std::char_traits<char>::length(lhs)};
  str += rhs;
  return str;
}

// VECTOR NC AutosarC++17_10-M5.17.1: MD_ZEROCOPYCOMMON_M5.17.1_stringNoOverloadOfAssignmentVersion_falsePositive
/*!
 * \brief Concatenate a C string and a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs C string.
 * \param[in] rhs String.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(char const* lhs, TriviallyCopyableString<kMaxCapacity>&& rhs) noexcept
    -> TriviallyCopyableString<kMaxCapacity> {
  return std::move(rhs.insert(0, lhs));
}

// VECTOR NC AutosarC++17_10-M5.17.1: MD_ZEROCOPYCOMMON_M5.17.1_stringNoOverloadOfAssignmentVersion_falsePositive
/*!
 * \brief Concatenate a character and a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs Character.
 * \param[in] rhs String.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(char lhs, TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept
    -> TriviallyCopyableString<kMaxCapacity> {
  TriviallyCopyableString<kMaxCapacity> str{1, lhs};
  str += rhs;
  return str;
}

// VECTOR NC AutosarC++17_10-M5.17.1: MD_ZEROCOPYCOMMON_M5.17.1_stringNoOverloadOfAssignmentVersion_falsePositive
/*!
 * \brief Concatenate a character and a string.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs Character.
 * \param[in] rhs String.
 * \return New string with the value of lhs + rhs.
 * \context ANY
 * \pre This operation must not result in the size of the string exceeding kMaxCapacity.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
auto operator+(char lhs, TriviallyCopyableString<kMaxCapacity>&& rhs) noexcept
    -> TriviallyCopyableString<kMaxCapacity> {
  return std::move(rhs.insert(typename TriviallyCopyableString<kMaxCapacity>::size_type(0), 1, lhs));
}

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
auto operator==(TriviallyCopyableString<kMaxCapacityLhs> const& lhs,
                TriviallyCopyableString<kMaxCapacityRhs> const& rhs) noexcept -> bool {
  return lhs.compare(rhs) == 0;
}

/*!
 * \brief Test equality of a C string and a string.
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
auto operator==(char const* lhs, TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept -> bool {
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
auto operator==(TriviallyCopyableString<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
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
auto operator!=(TriviallyCopyableString<kMaxCapacityLhs> const& lhs,
                TriviallyCopyableString<kMaxCapacityRhs> const& rhs) noexcept -> bool {
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
auto operator!=(char const* lhs, TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept -> bool {
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
auto operator!=(TriviallyCopyableString<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
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
auto operator<(TriviallyCopyableString<kMaxCapacityLhs> const& lhs,
               TriviallyCopyableString<kMaxCapacityRhs> const& rhs) noexcept -> bool {
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
auto operator<(TriviallyCopyableString<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
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
auto operator<(char const* lhs, TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept -> bool {
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
auto operator>(TriviallyCopyableString<kMaxCapacityLhs> const& lhs,
               TriviallyCopyableString<kMaxCapacityRhs> const& rhs) noexcept -> bool {
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
auto operator>(TriviallyCopyableString<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
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
auto operator>(char const* lhs, TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept -> bool {
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
auto operator<=(TriviallyCopyableString<kMaxCapacityLhs> const& lhs,
                TriviallyCopyableString<kMaxCapacityRhs> const& rhs) noexcept -> bool {
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
auto operator<=(TriviallyCopyableString<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
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
auto operator<=(char const* lhs, TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept -> bool {
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
auto operator>=(TriviallyCopyableString<kMaxCapacityLhs> const& lhs,
                TriviallyCopyableString<kMaxCapacityRhs> const& rhs) noexcept -> bool {
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
auto operator>=(TriviallyCopyableString<kMaxCapacity> const& lhs, char const* rhs) noexcept -> bool {
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
auto operator>=(char const* lhs, TriviallyCopyableString<kMaxCapacity> const& rhs) noexcept -> bool {
  return !(lhs < rhs);
}

/*!
 * \brief Swap two strings.
 * \details kMaxCapacity of lhs and rhs must be the same.
 * \tparam kMaxCapacity Fixed capacity of the string.
 * \param[in] lhs Reference to the first string.
 * \param[in] rhs Reference to the second string.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <std::size_t kMaxCapacity>
// NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
void swap(TriviallyCopyableString<kMaxCapacity>& lhs, TriviallyCopyableString<kMaxCapacity>& rhs) noexcept {
  lhs.swap(rhs);
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
auto operator<<(StreamT& output_stream, TriviallyCopyableString<kMaxCapacity> const& string) noexcept
    -> decltype(output_stream << string.c_str()) {
  return output_stream << string.c_str();  // VCA_ZEROCOPYCOMMON_SPC_15_STRING_FUNC_FULFILL_CONTRACT
}

/*!
 * \brief Value returned by various member functions when they fail.
 */
template <std::size_t kMaxCapacity>
typename TriviallyCopyableString<kMaxCapacity>::size_type const TriviallyCopyableString<kMaxCapacity>::npos{
    std::numeric_limits<TriviallyCopyableString<kMaxCapacity>::size_type>::max()};

}  // namespace container
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_STRING_H_

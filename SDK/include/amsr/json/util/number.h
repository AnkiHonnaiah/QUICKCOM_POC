/**********************************************************************************************************************
 *  COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 * -----------------------------------------------------------------------------------------------------------------*/
/**       \file     number.h
 *        \brief    A collection of utility functions for JSON numbers.
 *
 *        \details  Provides string to number conversion functions.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_NUMBER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_NUMBER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

#include "amsr/core/result.h"
#include "vac/language/byte.h"
#include "vac/language/compile_time.h"

#include "amsr/json/util/json_error_domain.h"
#include "amsr/json/util/types.h"

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief Checks if type is an integer.
 * \tparam T Type to check.
 *
 * \details Does not apply for bool.
 */
template <typename T>
using IsInt =
    typename vac::language::compile_time::all<!std::is_same<T, bool>::value, std::is_integral<T>::value>::type;

/*!
 * \brief Typedef for longlong.
 *
 * \details Return type of std::strtoll and transformed to fixed-size type later.
 */
using SignedLL = long long int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_JSON_base_type

/*!
 * \brief Typedef for unsigned longlong.
 *
 * \details Return type of std::strtoull and transformed to fixed-size type later.
 */
using UnsignedLL = unsigned long long int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_JSON_base_type

namespace util {
/*!
 * \brief Resets the value of errno to zero.
 * \spec
 *   requires true;
 * \endspec
 */
inline void ResetErrno() noexcept {
  // VECTOR NC AutosarC++17_10-M19.3.1: MD_JSON_Errno
  // VCA_VAJSON_EXTERNAL_CALL
  errno = 0;
}

// VECTOR NC AutosarC++17_10-A3.9.1: MD_JSON_base_type
/*!
 * \brief Returns the value of errno.
 * \return The value of errno.
 * \spec
 *   requires true;
 * \endspec
 */
inline auto GetErrno() noexcept -> int {
  // VECTOR NL AutosarC++17_10-M19.3.1: MD_JSON_Errno
  return errno;
}

/*!
 * \brief A parser for JSON numbers.
 * \pre This class assumes that all leading whitespace has been stripped.
 *
 * \details -
 */
class NumberParser {
 public:
  /*!
   * \brief Constructs a NumberParser.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  NumberParser() noexcept = default;

  /*!
   * \brief Gets a pointer beyond the last parsed character.
   * \return Pointer beyond the last parsed character.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto End() const noexcept -> char const* { return end_; }

  /*!
   * \brief Parses a StringView as a long long.
   * \param view to parse.
   * \return The parsed number.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - Reset errno.
   * - Convert the view to a long long.
   * \endinternal
   */
  auto LongLong(StringView view) noexcept -> SignedLL {
    ResetErrno();
    // VCA_VAJSON_EXTERNAL_CALL
    return std::strtoll(view.data(), &this->end_, 0);
  }

  /*!
   * \brief Parses a StringView as an unsigned long long.
   * \param view to parse. Must be pointing to a mutable buffer.
   * \return The parsed number.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - If the view starts with a dash:
   *   - Skip the parsing.
   * - Otherwise:
   *   - Reset errno.
   *   - Convert the view to an unsigned long long.
   * \endinternal
   */
  auto UnsignedLongLong(StringView view) noexcept -> UnsignedLL {
    UnsignedLL result{0};
    // strtoull accepts negative values!!
    // VCA_VAJSON_WITHIN_SPEC
    if (view[0] == '-') {
      // Set end_ so that it will be reported as 'parsed unsuccessfully'
      // VECTOR NL AutosarC++17_10-A5.2.3: MD_JSON_AutosarC++17_10-A5.2.3_const_cast
      this->end_ = const_cast<char*>(view.data());  // NOLINT(cppcoreguidelines-pro-type-const-cast)
    } else {
      ResetErrno();
      // VCA_VAJSON_EXTERNAL_CALL
      result = std::strtoull(view.data(), &this->end_, 0);
    }
    return result;
  }

  /*!
   * \brief Parses a StringView as a double.
   * \param view to parse.
   * \return The parsed number.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - Reset errno.
   * - Convert the view to a double.
   * \endinternal
   */
  auto Double(StringView view) noexcept -> double {
    ResetErrno();
    // VCA_VAJSON_EXTERNAL_CALL
    return std::strtod(view.data(), &this->end_);
  }

 private:
  /*!
   * \brief Pointer beyond the last parsed character.
   *
   * \details Set by character conversion functions. Null if the parse did not work.
   */
  char* end_{nullptr};
};
}  // namespace util
}  // namespace internal

/*!
 * \brief A representation of a parsed JSON number.
 *
 * \details -
 *
 * \vpublic
 *
 * \trace DSGN-JSON-Reader-Number-Formatting
 */
class JsonNumber final {
 public:
  /*!
   * \brief Creates a new number.
   * \param view to parse.
   * \return A Result containing the number or the error that occurred.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the number contains invalid characters}
   *
   * \vprivate Component private.
   *
   * \context ANY
   * \pre All whitespace must have been stripped.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If the input is not empty and a valid JSON number:
   *   - Return the number.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  static auto New(StringView view) noexcept -> Result<JsonNumber> {
    return Result<JsonNumber>{JsonNumber{view}}
        .Filter([](JsonNumber const& num) { return not num.view_.empty(); },  // VCA_VAJSON_EXTERNAL_CALL
                Err(JsonErrc::kInvalidJson, "JsonNumber::New: View must not be empty."))
        .Filter([](JsonNumber const& num) { return num.Validate(); },
                Err(JsonErrc::kInvalidJson, "JsonNumber::New: Number contains invalid characters."));
  };

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Tries to convert the number to a type.
   * \tparam T Type to convert to.
   * \return The Result containing the converted number if the conversion was successful, or an error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, Could not convert number}
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Convert the number to type T.
   * - If the conversion was successful:
   *   - Return a Result containing the number.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename T>
  auto TryAs() const noexcept -> amsr::core::Result<T> {
    return MakeResult(this->As<T>(), JsonErrc::kInvalidJson, "Could not convert number.");
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Tries to convert the number to a bool.
   * \return The Optional containing the bool if the conversion was successful, or empty.
   *
   * \details Maps "1" to true and "0" to false.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - If the number is a single '1' or '0' character:
   *   - Convert to true or false respectively.
   *   - Return a Result containing the bool.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Bool, std::enable_if_t<std::is_same<Bool, bool>::value>* = nullptr>
  auto As() const noexcept -> Optional<bool> {
    Optional<bool> opt;
    if (this->GetNumberOfChars() == 1) {
      // VCA_VAJSON_WITHIN_SPEC
      switch (this->view_[0]) {
        case '1':
          opt.emplace(true);
          break;
        case '0':
          opt.emplace(false);
          break;
        default:
          // in case something different from 0 or 1 is included, there will be no conversion.
          break;
      }
    }
    return opt;
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Tries to convert the number to a signed integer.
   * \tparam Integer Type of signed integer to convert to.
   * \return The Optional containing the signed integer if the conversion was successful, or empty.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Convert the number to a signed number.
   * - If the conversion was successful:
   *   - Return a Result containing the number.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Integer,
            std::enable_if_t<internal::IsInt<Integer>::value && std::is_signed<Integer>::value>* = nullptr>
  auto As() const noexcept -> Optional<Integer> {
    internal::util::NumberParser parser{};
    internal::SignedLL const result{parser.LongLong(this->view_)};

    return this->ExtractIfSuccessful<Integer>(result, parser.End());
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Tries to convert the number to an unsigned integer.
   * \tparam Integer Type of unsigned integer to convert to.
   * \return The Optional containing the unsigned integer if the conversion was successful, or empty.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Convert the number to an unsigned number.
   * - If the conversion was successful:
   *   - Return a Result containing the number.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Integer,
            std::enable_if_t<internal::IsInt<Integer>::value && (!std::is_signed<Integer>::value)>* = nullptr>
  auto As() const noexcept -> Optional<Integer> {
    internal::util::NumberParser parser{};
    internal::UnsignedLL const result{parser.UnsignedLongLong(this->view_)};

    return this->ExtractIfSuccessful<Integer>(result, parser.End());
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Tries to convert the number to a floating point type.
   * \tparam Float Floating point type to convert to.
   * \return The Optional containing the floating point number if the conversion was successful, or empty.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Convert the number to the desired floating point type.
   * - If the conversion was successful:
   *   - Return a Result containing the number.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   */
  template <typename Float, std::enable_if_t<std::is_floating_point<Float>::value>* = nullptr>
  auto As() const noexcept -> Optional<Float> {
    internal::util::NumberParser parser{};
    double const conversion_result{parser.Double(this->view_)};

    return this->ExtractIfSuccessful<Float>(conversion_result, parser.End());
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Tries to convert the number to a vac::language::byte.
   * \return The Optional containing the byte if the conversion was successful, or empty.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - Convert the number to a vac::language::byte.
   * - If the conversion was successful:
   *   - Return a Result containing the number.
   * - Otherwise:
   *   - Return an error.
   * \endinternal
   *
   * \vprivate
   */
  template <typename Byte, std::enable_if_t<std::is_same<vac::language::byte, Byte>::value>* = nullptr>
  auto As() const noexcept -> Optional<Byte> {
    Optional<Byte> opt;

    Optional<std::uint8_t> conversion_result{this->As<std::uint8_t>()};
    if (conversion_result.has_value()) {
      opt.emplace(static_cast<Byte>(*conversion_result));
    }

    return opt;
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Converts the number to a JsonNumber.
   * \return The Optional containing the JsonNumber.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - Return the number as a Number type.
   * \endinternal
   *
   * \vprivate
   */
  template <typename Num, std::enable_if_t<std::is_same<JsonNumber, Num>::value>* = nullptr>
  auto As() const noexcept -> Optional<Num> {
    return Optional<Num>{JsonNumber(this->view_)};
  }

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_JSON_gtest_noexcept_violation
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_JSON_gtest_noexcept_violation
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Converts the number using a user defined parser.
   * \tparam Fn Type of function parser.
   * \param parser function to convert the string view into a number.
   * \return The parsed number.
   *
   * \details Parser function must take the string representation of the number as amsr::core::StringView and return
   * the desired number type.
   *
   * \context ANY
   * \pre Parser does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Convert the number using the given function.
   * \endinternal
   */
  template <typename Fn>
  auto Convert(Fn&& parser) const noexcept -> decltype(parser(std::declval<StringView>())) {
    // VCA_VAJSON_WITHIN_SPEC
    return parser(this->view_);
  }

 private:
  /*!
   * \brief Constructs a Number from a StringView.
   * \param view to parse.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  explicit constexpr JsonNumber(StringView view) noexcept : view_{view} {}

  /*!
   * \brief Gets the number of characters.
   * \return The number of characters.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetNumberOfChars() const noexcept -> std::size_t { return this->view_.size(); }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Casts the passed number to a smaller number type if still valid.
   * \tparam TargetType Type to cast to.
   * \tparam SourceType Type to cast from.
   * \param number to cast.
   * \return The Optional containing the number if the cast was successful, or empty.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - If the number does not exceed the numeric limits of the target type:
   *   - Cast the number to the target type.
   *   - Return an Optional containing the number.
   * - Otherwise:
   *   - Return an empty Optional.
   * \endinternal
   */
  template <typename TargetType, typename SourceType>
  static auto Cast(SourceType number) noexcept -> Optional<TargetType> {
    Optional<TargetType> opt;
    // VECTOR NC Compiler-#186: MD_JSON_PlatformVariant
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_JSON_AutosarC++17_10-M0.1.2_dead_code
    if ((std::numeric_limits<TargetType>::max() >= number) && (std::numeric_limits<TargetType>::lowest() <= number)) {
      opt.emplace(TargetType(number));
    }
    return opt;
  }

  /*!
   * \brief Checks if the parse has been successful.
   * \param end The end-pointer that is used to verify the parse.
   * \return True if the number was successfully parsed, otherwise false.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - If no ERANGE error occurred and the end-pointer has been set to past-the-last character and the last character is
   *   no '.' character:
   *   - Return true.
   * - Otherwise:
   *   - Return false.
   * \endinternal
   */
  auto ParseSuccessful(char const* end) const noexcept -> bool {
    // std::strtod uncomplainingly parses input that ends with a period, although that is invalid JSON!
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_JSON_AutosarC++17_10-M0.1.2_dead_code
    return ((internal::util::GetErrno() != ERANGE) && (end == this->view_.cend()) &&
            // VCA_VAJSON_WITHIN_SPEC
            (this->view_.back() != '.'));
  }

  // VECTOR NC VectorC++-V3.9.2: MD_JSON_VectorC++-V3.9.2_arithmetic_types
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Converts the passed number to a smaller number type if still valid.
   * \tparam TargetType Type to convert to.
   * \tparam SourceType Type to convert from.
   * \param num The source value.
   * \param end The end iterator of the buffer.
   * \return The Optional containing the number if the conversion was successful, or empty.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - If the number has been successfully parsed and casted to the target type:
   *   - Return an Optional containing the number.
   * - Otherwise:
   *   - Return an empty Optional.
   * \endinternal
   */
  template <typename TargetType, typename SourceType>
  auto ExtractIfSuccessful(SourceType num, char const* end) const noexcept -> Optional<TargetType> {
    Optional<TargetType> opt;

    if (this->ParseSuccessful(end)) {
      opt = JsonNumber::Cast<TargetType>(num);
    }

    return opt;
  }

  /*!
   * \brief Validates the contained number.
   * \return True if the number is valid, otherwise false.
   *
   * \context ANY
   * \pre -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - If the number has multiple digits:
   *   - Check that if it has a leading zero, it is a valid hex or float number.
   * - Otherwise:
   *   - Check that it is a valid digit.
   * \endinternal
   */
  auto Validate() const noexcept -> bool {
    std::size_t const size{this->view_.size()};
    // VCA_VAJSON_WITHIN_SPEC
    std::size_t const first_digit_pos{(this->view_[0] == '-') ? std::size_t{1} : std::size_t{0}};

    bool const is_multiple_digits{size > (first_digit_pos + 1)};
    bool is_valid{false};
    if (is_multiple_digits) {
      constexpr static CStringView kValidLetters{".eExX"_sv};

      // If it has a leading '0' character, it must be followed by a valid letter character in order to be a valid hex
      // or float number.
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_JSON_AutosarC++17_10-M0.3.1_OOB
      // VCA_VAJSON_WITHIN_SPEC
      is_valid = (this->view_[first_digit_pos] != '0') ||
                 std::any_of(kValidLetters.cbegin(), kValidLetters.cend(), [this, first_digit_pos](char ch) {
                   // VECTOR NC AutosarC++17_10-M0.3.1: MD_JSON_AutosarC++17_10-M0.3.1_OOB
                   // VCA_VAJSON_LAMBDA_CAPTURE
                   return this->view_[first_digit_pos + 1] == ch;
                 });
    } else {
      is_valid =
          (size == (first_digit_pos + 1)) &&
          // std::isdigit requires argument to be representable as unsigned char. On systems where char is signed
          // the value must be positive. A negative value of char is no valid digit anyway.
          // VECTOR NL AutosarC++17_10-M4.5.3, AutosarC++17_10-M0.3.1: MD_JSON_AutosarC++17_10-M4.5.3_char_comparison, MD_JSON_AutosarC++17_10-M0.3.1_OOB
          (this->view_[first_digit_pos] >= 0) &&  // VCA_VAJSON_EXTERNAL_CALL
          // VECTOR NL VectorC++-V21.8.7, AutosarC++17_10-M0.3.1: MD_JSON_VectorC++-V21.8.7_char_handling, MD_JSON_AutosarC++17_10-M0.3.1_OOB
          (std::isdigit(this->view_[first_digit_pos]) != 0);  // VCA_VAJSON_EXTERNAL_CALL
    }

    return is_valid;
  }

  /*!
   * \brief Stored view onto the buffer.
   */
  StringView view_;
};
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_NUMBER_H_

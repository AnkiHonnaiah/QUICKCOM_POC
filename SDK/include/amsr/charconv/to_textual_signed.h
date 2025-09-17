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
/*!        \file
 *        \brief  Converts a signed integer value to its textual representation.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_SIGNED_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_SIGNED_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <bitset>
#include <iterator>
#include <limits>
#include <sstream>
#include <type_traits>

#include "amsr/charconv/charconv_error_domain.h"
#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/charconv/internal/str_to_types.h"
#include "amsr/charconv/internal/to_textual_template_class.h"
#include "amsr/charconv/internal/utility.h"
#include "amsr/charconv/type_helper.h"
#include "amsr/charconv_common/internal/buffer_pointer.h"
#include "amsr/charconv_common/internal/common_utility.h"
#include "amsr/charconv_common/internal/to_textual_buffer.h"
#include "amsr/core/abort.h"
#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace charconv {

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a signed integer value to its textual representation.
 * \tparam T Signed integer type.
 * \unit amsr::charconv::ToTextualSigned
 * \vprivate Product Private
 */
template <typename T>
class ToTextual<T, typename std::enable_if<is_signed_and_not_char<T>::value>::type> final {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer Memory segment to save textual representation.
   * \param[in] value_in Signed integer value to be converted.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ToTextual(amsr::core::Span<char> buffer, T value_in) noexcept
      : buffer_{buffer}, value_{value_in} {}  // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_CONTAINER

  /*!
   * \brief Move constructs instance of class.
   * \param[in] to_textual Instance of ToTextual
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ToTextual(ToTextual&& to_textual) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] to_textual Instance of ToTextual
   * \return Moved ToTextual
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ToTextual& operator=(ToTextual&& to_textual) & noexcept = default;

  // ToTextual is not copy constructible because it would break exclusive management of the resource.
  ToTextual(ToTextual const& to_textual) = delete;

  // ToTextual is not copy assignable because it would break exclusive management of the resource.
  ToTextual& operator=(ToTextual const& to_textual) & = delete;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~ToTextual() noexcept = default;

  /*!
   * \brief Converts a signed integer value to its textual representation in the given base which is written
   * successively to buffer.
   * \param[in] base Given base.
   * \param[in] fmt Formatting parameters.
   * \return Textual representation of value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kValueTooLarge In case the value is larger than the specified range.
   * \reentrant TRUE
   * \trace DSGN-CharacterConversion-ToTextual
   * \trace SPEC-9717269, SPEC-9718138, SPEC-11451591, SPEC-17702354, SPEC-17702355
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  // SPEC-9717269 (decimal), SPEC-9718138 (hexadecimal), SPEC-11451591 (binary)
  // SPEC-17702354 (valid result), SPEC-17702355 (kValueTooLarge)
  amsr::core::Result<amsr::core::Span<char>> Convert(NumberBase const base = NumberBase::kDecimal,
                                                     ExtendedFormat const fmt = extended_format_default) noexcept {
    return WriteSignedInteger(base, fmt);
  }

 private:
  /*!
   * \brief Converts the contents of the bitset to an unsigned long integer.
   * \tparam N Size of bitset. Limited by design to 8, 16, 32 and 64 as these are the bit sizes of the different
   * integer types.
   * \details Replaces to_ulong() function as it might throw an exception of type std::overflow_error.
   * \param[in] bit_array Bitset.
   * \return Integer representation of the data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - Convert bitset to number.
   * \endinternal
   */
  template <std::size_t N>
  static std::uint64_t ToULong(std::bitset<N> const& bit_array) noexcept {
    static_assert(((N == (1 * kBitsInByte)) || (N == (2 * kBitsInByte)) ||
                   ((N == (4 * kBitsInByte)) || (N == (8 * kBitsInByte)))),
                  "Bitset is not of size 8, 16, 32 or 64.");  // guarantee that there is no out-of-bounds access and no
                                                              // wrap-around when factor is shifted
    std::uint64_t value{0};
    std::uint64_t factor{1};
    for (std::size_t i{0}; i <= (N - 2); i++) {
      if (bit_array[i]) {
        value += factor;
      }
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_no_wrap_around
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_no_wrap_around
      factor = factor << 1;  // multiply by 2
    }
    if (bit_array[N - 1]) {
      value += factor;
    }

    return value;
  }

  /*!
   * \brief Converts the contents of the bitset to an unsigned long integer.
   * \tparam N Size of bitset.
   * \details Replaces to_string() function as it might throw an exception of type std::out_of_range.
   * \param[in] bit_array Bitset.
   * \return String representation of the data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - For each bit in bitset (starting from highest bit):
   *   - Add value to stream.
   * \endinternal
   */
  template <std::size_t N>
  static amsr::core::String ToString(std::bitset<N> bit_array) noexcept {
    amsr::core::String binary_string{""};
    amsr::core::StringView const location{amsr::core::StringView{"ToTextualSigned::ToString"}};
    // VCA_CHARCONV_ASSERT_STRING_IS_LARGE_ENOUGH_FUNC
    internal::AssertStringIsLargeEnough(binary_string, amsr::charconv::Convert<std::uint64_t>(N), location);

    for (std::size_t i{bit_array.size()}; i > 0; --i) {
      if (bit_array[i - 1] == 0) {
        binary_string.append("0");  // VCA_CHARCONV_STRING_APPEND
      } else {
        binary_string.append("1");  // VCA_CHARCONV_STRING_APPEND
      }
    }

    return binary_string;
  }

  /*!
   * \brief Converts a negative integer value to two's complement hexadecimal string.
   * \param[in] value Integer value.
   * \param[in] fmt Formatting parameters for hexadecimal output.
   * \return Two's complement hexadecimal string.
   * \context ANY
   * \pre Value < 0.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 If value is minimum representable value:
   *     - Assemble string manually.
   * #20 Otherwise:
   *     #30 Get two's complement numerical value from binary two's complement.
   *     #40 Convert two's complement numerical value to hexadecimal.
   *     #50 Get string representation.
   * \endinternal
   */
  static amsr::core::String ToTwosComplementHexadecimalValue(
      T const value, ExtendedFormat const fmt = extended_format_default) noexcept {
    amsr::core::String result_string{""};
    amsr::core::StringView const msg1{amsr::core::StringView{
        "ToTextualSigned::ToTwosComplementHexadecimalValue: Internal error. Precondition violated (value < 0)."}};
    // VCA_CHARCONV_ASSERT_FUNC
    internal::Assert(value < 0, msg1);

    if (value == std::numeric_limits<T>::min()) {  // #10
      // This case has to be handled separately. There is no valid value -value_copy, so value can't be just
      // converted to a positive value and then printed.
      // VECTOR NL AutosarC++17_10-A8.5.2: MD_CharacterConversion_A8.5.2_braced_initialization
      amsr::core::String const zeros((sizeof(T) * 2) - 1, '0');
      amsr::core::StringView const msg2{amsr::core::StringView{
          "ToTextualSigned::ToTwosComplementHexadecimalValue: Internal error. Size insufficient."}};
      // VCA_CHARCONV_ASSERT_STRING_IS_LARGE_ENOUGH_FUNC
      internal::AssertStringIsLargeEnough(result_string, amsr::charconv::Convert<std::uint64_t>(zeros.size() + 1),
                                          msg2);
      result_string.append(amsr::core::StringView{"8"}).append(zeros);  // VCA_CHARCONV_STRING_APPEND
      // VCA_CHARCONV_BASICSTRING_DESTRUCTOR
    } else {  // #20
      // VECTOR NC AutosarC++17_10-M5.0.3: MD_CharacterConversion_M5.0.3_change_of_signedness_calc
      T const value_copy{amsr::charconv::ConvertOrAbort<T>(-value)};

      // #30
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_cast_may_truncate_value
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_cast_may_truncate_value
      std::uint64_t numerical_value_one_complement{ToULong(
          (std::bitset<sizeof(T) * kBitsInByte>(amsr::charconv::ConvertOrAbort<std::uint64_t>(value_copy))).flip())};
      std::uint64_t const numerical_value_twos_complement{ToULong(std::bitset<sizeof(T) * kBitsInByte>(
          amsr::charconv::Convert<std::uint64_t>(++numerical_value_one_complement)))};

      // #40
      std::stringstream result{};  // VCA_CHARCONV_STRINGSTREAM_CONSTRUCTOR
      if (fmt.abc_case == LetterCase::kUpperCase) {
        result << std::uppercase;  // VCA_CHARCONV_STRINGSTREAM_INSERTION_OPERATOR_CONTRACT
      }
      result << std::hex << numerical_value_twos_complement;  // VCA_CHARCONV_STRINGSTREAM_INSERTION_OPERATOR_CONTRACT

      // #50
      result_string = internal::ToString(result);  // VCA_CHARCONV_TOSTRING_FUNC
    }                                              // VCA_CHARCONV_BASICSTRINGSTREAM_DESTRUCTOR
    return result_string;
  }

  /*!
   * \brief Converts a value of type T to a binary two's complement textual representation.
   * \param[in] value The value of type T to be converted.
   * \return The binary two's complement textual representation of the value.
   * \context ANY
   * \pre value < 0
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 If value is minimum representable value:
   *     - Assemble string manually.
   * #20 Otherwise:
   *     - Calculate one's complement of negative numerical value.
   *     - Calculate two's complement by adding 1 to one's complement.
   *     - Convert to string.
   * \endinternal
   */
  static amsr::core::String ToTwosComplementBinaryVector(T value) noexcept {
    amsr::core::String result_string{""};
    amsr::core::StringView const msg1{amsr::core::StringView{
        "ToTextualSigned::ToTwosComplementBinaryVector: Internal error. Precondition violated (value < 0)."}};
    // VCA_CHARCONV_ASSERT_FUNC
    internal::Assert(value < 0, msg1);

    if (value == std::numeric_limits<T>::min()) {  // #10
      // This case has to be handled separately. There is no valid value -value_copy, so value can't be just
      // converted to a positive value and then printed.
      // VECTOR NL AutosarC++17_10-A8.5.2: MD_CharacterConversion_A8.5.2_braced_initialization
      amsr::core::String const zeros((sizeof(T) * 2) - 1, '0');
      amsr::core::StringView const msg2{
          amsr::core::StringView{"ToTextualSigned::ToTwosComplementBinaryVector: Internal error. Size insufficient."}};
      // VCA_CHARCONV_ASSERT_STRING_IS_LARGE_ENOUGH_FUNC
      internal::AssertStringIsLargeEnough(result_string, amsr::charconv::Convert<std::uint64_t>(zeros.size() + 1),
                                          msg2);
      result_string.append("1").append(zeros);  // VCA_CHARCONV_BASICSTRING_DESTRUCTOR
      // VCA_CHARCONV_BASICSTRING_DESTRUCTOR
    } else {  // #20
      // VECTOR NC AutosarC++17_10-M5.0.3: MD_CharacterConversion_M5.0.3_change_of_signedness_calc
      T const value_copy{amsr::charconv::ConvertOrAbort<T>(-value)};

      // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_cast_may_truncate_value
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_cast_may_truncate_value
      std::uint64_t numerical_value_one_complement{amsr::charconv::Convert<std::uint64_t>(ToULong(
          (std::bitset<sizeof(T) * kBitsInByte>(amsr::charconv::ConvertOrAbort<std::uint64_t>(value_copy))).flip()))};
      std::bitset<sizeof(T) * kBitsInByte> const& numerical_value_twos_complement{std::bitset<sizeof(T) * kBitsInByte>(
          amsr::charconv::Convert<std::uint64_t>(++numerical_value_one_complement))};

      // VCA_CHARCONV_BASICSTRING_ASSIGNMENT_OPERATOR_CONTRACT
      result_string = ToString(numerical_value_twos_complement);
    }

    return result_string;
  }

  // VECTOR NC Metric-HIS.PATH: MD_CharacterConversion_Metric-HIS.PATH
  /*!
   * \brief Adds optional fill characters and copies the content of array to buffer.
   * \param[in] array Textual representation of value, in reverse order.
   * \param[in] number_of_digits Required number of digits of value.
   * \param[in] fmt Formatting parameters.
   * \return Textual representation of value, in correct order.
   * \context ANY
   * \pre \c number_of_digits is at least 1 and not greater than kNbDigitsOfLimitMaxUnsignedLLDecimalNumber.
   * \threadsafe FALSE
   * \error CharconvErrc::kValueTooLarge In case the value is larger than the specified range.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 If specified fmt.width is bigger than number of digits, update final length.
   * #20 If memory segment is large enough for data
   *     #21 If final length is bigger than number of digits, add fill characters.
   *     #22 For negative values, add minus sign.
   *     #23 Flip and copy array content to memory segment.
   * \endinternal
   */
  amsr::core::Result<amsr::core::Span<char>> ShiftFillFlipAndCopyToMemorySegment(
      amsr::core::Array<char, internal::kNbDigitsOfLimitMaxUnsignedLLDecimalNumber> array, std::size_t number_of_digits,
      ExtendedFormat fmt) {
    amsr::core::StringView const msg1{
        amsr::core::StringView{"ToTextualSigned::ShiftFillFlipAndCopyToMemorySegment: number of digits too large."}};
    std::uint8_t const max_length{internal::kNbDigitsOfLimitMaxUnsignedLLDecimalNumber};
    // VCA_CHARCONV_ASSERT_FUNC
    internal::Assert(number_of_digits <= max_length, msg1);
    amsr::core::StringView const msg2{
        amsr::core::StringView{"ToTextualSigned::ShiftFillFlipAndCopyToMemorySegment: number of digits too small."}};
    // VCA_CHARCONV_ASSERT_FUNC
    internal::Assert(number_of_digits >= 1, msg2);
    amsr::core::Result<amsr::core::Span<char>> result{CharconvErrc::kValueTooLarge};
    std::size_t final_length{number_of_digits};
    if (fmt.width > number_of_digits) {  // #10
      final_length = fmt.width;
    }

    // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_POINTER
    if (buffer_.GetSize() >= final_length) {  // #20
      buffer_.Reset();                        // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_POINTER
      if (final_length > number_of_digits) {  // #21
        std::size_t fill_width{final_length - number_of_digits};
        while (fill_width != 0) {
          buffer_.Add(fmt.fillchar);  // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_POINTER
          fill_width--;
        }
      }

      if (array[0] == '-') {  // #22
        buffer_.Add('-');     // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_POINTER
      }

      // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_cast_may_truncate_value_precondition
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_cast_may_truncate_value_precondition
      // VECTOR NC AutosarC++17_10-M5.0.3: MD_CharacterConversion_M5.0.3_change_of_signedness_calc
      std::uint8_t const max_index{amsr::charconv::ConvertOrAbort<std::uint8_t>(number_of_digits - 1U)};
      for (std::size_t i{0}; i <= max_index; i++) {  // #23
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CharacterConversion_M5.0.16_out_of_bounds_1
        char const ch{array[max_index - i]};
        if (ch == '-') {
          break;
        }
        buffer_.Add(ch);  // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_POINTER
      }

      result.EmplaceValue(buffer_.GetBuffer());  // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_POINTER
    }

    return result;
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CharacterConversion_A15.5.3_span
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CharacterConversion_A15.4.2_span
  /*!
   * \brief Converts a signed integer value to its textual representation in the given base which is written
   * successively to buffer.
   * \param[in] base Given base.
   * \param[in] fmt Formatting parameters.
   * \return Textual representation of value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kValueTooLarge In case the value is larger than the specified range.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 If base is decimal:
   *    #11 If value is negative,
   *         #111 Calculate its absolute value.
   *         #112 And store minus sign.
   *     #12 Retrieve digits from value as character.
   *     #13 Store them in array (in reversed order).
   *     #14 Apply formatting, flip and copy array content to memory segment.
   * #20 If base is hexadecimal:
   *     #21 If value is smaller than 0:
   *         - Calculate two's complement and write it to stream.
   *     #22 Else:
   *         - Set format properties of stream and write value in hexadecimal base to stream.
   *     #23 Copy stream content to memory segment.
   * #30 Otherwise (binary base)
   *     #31 If value is smaller than 0:
   *         - Calculate two's complement and write it to stream.
   *     #32 Else:
   *         - Set format properties of stream and write value in binary base.
   *     #33 Copy stream content to memory segment.
   * \endinternal
   */
  amsr::core::Result<amsr::core::Span<char>> WriteSignedInteger(NumberBase const base,
                                                                ExtendedFormat const fmt) noexcept {
    amsr::core::Result<amsr::core::Span<char>> result{CharconvErrc::kValueTooLarge};

    if (base == NumberBase::kDecimal) {  // #10
      T const number{value_};
      amsr::core::Array<char, internal::kNbDigitsOfLimitMinSignedLLDecimalNumber + 1> array{};
      std::size_t index{0};
      if (number == 0) {
        array[index] = '0';
        index++;
      } else {
        std::uint64_t absolute_value{0};
        if (number < 0) {  // #11
          if (number == std::numeric_limits<T>::min()) {
            absolute_value = amsr::charconv::ConvertOrAbort<std::uint64_t>(std::numeric_limits<T>::max()) + 1;
          } else {
            // VECTOR NC AutosarC++17_10-M5.0.9: MD_CharacterConversion_M5.0.9
            absolute_value = amsr::charconv::ConvertOrAbort<std::uint64_t>(
                amsr::charconv::ConvertOrAbort<std::int64_t>(number) * -1);  // #111
          }
          array[index] = '-';  // #112
          index++;
        } else {
          absolute_value = amsr::charconv::ConvertOrAbort<std::uint64_t>(number);
        }

        while (absolute_value != 0) {
          // #12
          std::uint64_t const digit{absolute_value % 10};
          absolute_value = absolute_value / 10;
          char const digit_as_char{internal::DigitToAscii(digit)};
          // VECTOR NC AutosarC++17_10-M5.0.16: MD_CharacterConversion_M5.0.16_out_of_bounds_2
          // VCA_CHARCONV_ARRAY_ACCESS
          array[index] = digit_as_char;  // #13
          // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_no_wrap_around
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_no_wrap_around
          index++;
        }
      }
      result = ShiftFillFlipAndCopyToMemorySegment(array, index, fmt);  // #14
    } else if (base == NumberBase::kHex) {                              // #20
      std::stringstream stream;                                         // VCA_CHARCONV_STRINGSTREAM_CONSTRUCTOR
      internal::SetStreamFillCharAndWidth(fmt, stream);                 // VCA_CHARCONV_SETSTREAMFILLCHAR_FUNC
      if (value_ < 0) {                                                 // #21
        amsr::core::String const hex_value_str{ToTwosComplementHexadecimalValue(value_, fmt)};
        stream << hex_value_str;  // VCA_CHARCONV_VALID_ARGUMENT
        // VCA_CHARCONV_BASICSTRING_DESTRUCTOR
      } else {                                 // #22
        internal::SetStreamCase(fmt, stream);  // VCA_CHARCONV_SETSTREAMCASE_FUNC
        std::int64_t const val{amsr::charconv::Convert<std::int64_t>(value_)};
        stream << std::hex << val;  // VCA_CHARCONV_STRINGSTREAM_INSERTION_OPERATOR_CONTRACT
      }
      // #23
      amsr::core::String const literal{internal::ToString(stream)};  // VCA_CHARCONV_TOSTRING_FUNC
      result =
          buffer_.DoSafeMemcpy(amsr::core::StringView{literal});  // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_WRITE
                                                                  // VCA_CHARCONV_BASICSTRING_DESTRUCTOR
    } else {                                                      // #30
      std::stringstream stream;                                   // VCA_CHARCONV_STRINGSTREAM_CONSTRUCTOR
      internal::SetStreamFillCharAndWidth(fmt, stream);           // VCA_CHARCONV_SETSTREAMFILLCHAR_FUNC
      if (value_ < 0) {                                           // #31
        amsr::core::String const& binary_value_str{ToTwosComplementBinaryVector(value_)};
        stream << binary_value_str;  // VCA_CHARCONV_VALID_ARGUMENT
        // VCA_CHARCONV_BASICSTRING_DESTRUCTOR
      } else {  // #32
        std::bitset<sizeof(T) * kBitsInByte> const bitset{
            std::bitset<sizeof(T) * kBitsInByte>(amsr::charconv::ConvertOrAbort<std::uint64_t>(value_))};
        amsr::core::String const value_binary{bitset.to_string()};  // VCA_CHARCONV_BITSET_TO_STRING
        stream << internal::SkipLeadingZeros(value_binary);         // VCA_CHARCONV_SKIPLEADINGZEROS_FUNC
      }
      // #33
      amsr::core::String const literal{internal::ToString(stream)};  // VCA_CHARCONV_TOSTRING_FUNC
      result =
          buffer_.DoSafeMemcpy(amsr::core::StringView{literal});  // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_WRITE
    }                                                             // VCA_CHARCONV_BASICSTRING_DESTRUCTOR

    return result;
  }

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ToTextualSigned__Test, ULong__Sint8);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ToTextualSigned__Test, ULong__Sint16);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ToTextualSigned__Test, ULong__Sint32);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ToTextualSigned__Test, ULong__Sint64);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename S>
  FRIEND_TEST(UT__ToTextualSigned__Typed, ToTwosComplementHexadecimalValue__PreconditionViolated);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename S>
  FRIEND_TEST(UT__ToTextualSigned__Typed, ToTwosComplementBinaryVector__PreconditionViolated);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename S>
  FRIEND_TEST(UT__ToTextualSigned__Typed, ShiftFillFlipAndCopyToMemorySegment__Precondition1);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename S>
  FRIEND_TEST(UT__ToTextualSigned__Typed, ShiftFillFlipAndCopyToMemorySegment__Precondition2);

  /*! buffer to save textual representation */
  internal::ToTextualBuffer buffer_;

  /*! value to be converted */
  T value_;
};

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_SIGNED_H_

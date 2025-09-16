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
 *        \brief  Parses next characters as floating-point number.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_PARSE_FLOATING_POINT_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_PARSE_FLOATING_POINT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/charconv/charconv_error_domain.h"
#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/charconv/internal/utility.h"
#include "amsr/charconv/type_helper.h"
#include "amsr/charconv_common/internal/buffer.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Parses next characters as floating-point number.
 * \unit amsr::charconv::internal::ParseFloatingPoint
 * \vprivate Component Private
 */
class ParseFloatingPoint : public Buffer {
 public:
  // VECTOR NC AutosarC++17_10-A12.1.6: MD_CharacterConversion_A12.1.6_inheriting_constructors
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer_in Memory segment with textual representation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  explicit ParseFloatingPoint(amsr::core::StringView buffer_in) noexcept : Buffer{buffer_in} {}

 protected:
  /*!
   * \brief Move constructs instance of class.
   * \param[in] fp_to_parse Instance of ParseFloatingPoint
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  ParseFloatingPoint(ParseFloatingPoint&& fp_to_parse) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] fp_to_parse Instance of ParseFloatingPoint
   * \return Moved ParseFloatingPoint
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  ParseFloatingPoint& operator=(ParseFloatingPoint&& fp_to_parse) & noexcept = default;

  // ParseFloatingPoint is not copy constructible because it would break exclusive management of the resource.
  ParseFloatingPoint(ParseFloatingPoint const& fp_to_parse) = delete;

  // ParseFloatingPoint is not copy assignable because it would break exclusive management of the resource.
  ParseFloatingPoint& operator=(ParseFloatingPoint const& fp_to_parse) & = delete;

 public:
  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  ~ParseFloatingPoint() noexcept override = default;  // VCA_CHARCONV_DESTRUCTOR_MOLE_1298

  // VECTOR NC AutosarC++17_10-A8.2.1: MD_CharacterConversion_A8.2.1_trailing_type_syntax
  /*!
   * \brief Converts a character sequence to a floating point value which is stored in \c value.
   * \tparam T Data type of value: float or double.
   * \param[in] fmt Format of value, default is general, other formats are not supported.
   * \return Converted value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case no conversion can be performed or buffer is empty.
   * \error CharconvErrc::kResultOutOfRange In case the converted value falls out of range of the corresponding return
   * type.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 skip leading whitespace
   * #20 If no characters to parse: reset current buffer
   * #25 Otherwise:
   *     #30 If plus sign OR stand-alone minus sign: reset current buffer
   *     #40 Else if floating point format is valid
   *         #50 if char is a digit OR char after minus is a digit
   *             #60 convert region to null-terminated string
   *             #62 convert string to value
   *             #64 if value is out of bounds
   *                 #70 return out of range error
   *             #66 otherwise
   *                 #80 set return value
   *         #90 Otherwise:
   *             #95 call ParseNanAndInf
   *     #100 Otherwise do nothing / return error.
   * \endinternal
   */
  template <typename T>
  amsr::core::Result<T> ReadFloatingPoint(FPFormat const fmt) noexcept {
    amsr::core::Result<T> result{amsr::core::Result<T>::FromError(CharconvErrc::kInvalidArgument)};
    // #10
    SkipLeadingWhitespace();

    if (!AreCharsLeft()) {  // #20
      ResetCurrentBuffer();
    } else {  // #25
      char const next_char{PeekChar()};
      bool const only_one_char_left{GetNumberOfCharsLeft() == 1};
      if ((next_char == '+') || ((next_char == '-') && only_one_char_left)) {  // #30
        ResetCurrentBuffer();
      } else if (IsFloatingPointFormatCorrect(fmt)) {  // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER
        // #50
        char char_after_minus{' '};
        if (next_char == '-') {
          char_after_minus = GetCurrentBufferView(1, 1)[0];
        }
        bool const is_next_char_digit{IsDigit(static_cast<std::uint8_t>(next_char))};
        bool const is_char_after_minus_digit{IsDigit(static_cast<std::uint8_t>(char_after_minus))};
        if (is_next_char_digit || ((next_char == '-') && is_char_after_minus_digit)) {
          // #60 convert region to null-terminated string
          amsr::core::String buffer2{MakeCString()};

          // #62
          ConstBufferPointer const buffer_first{&buffer2[0]};
          BufferPointer buffer_last{};
          // VECTOR NC AutosarC++17_10-M19.3.1: MD_CharacterConversion_M19.3.1_errno
          // VCA_CHARCONV_ERRNO_NOT_NULL
          errno = 0;  // SEI CERT C Coding Standard ERR30-C
          T const converted_value{ConvertStringTo<T>(buffer_first, buffer_last)};

          // Advance last pointer by end of match.
          // VCA_CHARCONV_STDDISTANCE_FUNC
          std::ptrdiff_t const length{StdDistance(buffer_first, ConstBufferPointer{buffer_last.ptr})};
          Advance(amsr::charconv::ConvertOrAbort<std::size_t>(length));

          if (IsValueOutOfRange(converted_value)) {  // #64
            // #70
            result.EmplaceError(CharconvErrc::kResultOutOfRange);
          } else {  // #66
            // #80
            result.EmplaceValue(converted_value);
          }
          // VCA_CHARCONV_BASICSTRING_DESTRUCTOR
        } else {  // #90
          // #95
          result = ParseNanAndInf<T>();
        }
      } else {  // #100
        // intentionally left empty: floating point format is incorrect, return error
      }
    }
    return result;
  }

 private:
  /*!
   * \brief Checks if character sequence adheres to the specified floating-point format \c fmt.
   * \param[in] fmt Floating-point format.
   * \return \c true, if character sequence adheres to \c fmt, \c false if \c fmt is \c FPFormat::kScientific and the
   * buffer does not have an exponent or the format is FPFormat::kFixed and the literal has an exponent.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \trace SPEC-9718158, SPEC-9718159
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  // SPEC-9718158 (scientific format), SPEC-9718159 (fixed format)
  bool IsFloatingPointFormatCorrect(FPFormat const fmt) const noexcept;

  /*!
   * \brief Checks if buffer contains a valid exponent, i.e. [e|E][+|-]\\d{2+}
   * \return \c true, if yes \c false, if not
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  bool HasValidExponent() const noexcept;

  /*!
   * \brief Copies all characters from the string until (and excluding) the first invalid character and attaches a
   * null-termination at the end.
   * \return The character sequence until (and excluding) the first invalid character, with null-termination (CString).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  amsr::core::String MakeCString() const noexcept;

  /*!
   * \brief Converts string to floating point number of type float.
   * \tparam F Type of the converted floating point value.
   * \param[in] first Start of memory block.
   * \param[out] last End of memory block (excluded).
   * \return Returns the converted value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * return converted value
   * \endinternal
   */
  template <typename F,
            std::enable_if_t<std::is_floating_point<F>::value && std::is_same<float, F>::value, std::int32_t> = 0>
  static F ConvertStringTo(ConstBufferPointer first, BufferPointer& last) noexcept {
    float const converted_value{strtof(first.ptr, &last.ptr)};  // VCA_CHARCONV_STRTOF_FUNC
    return converted_value;
  }

  /*!
   * \brief Converts string to floating point number of type double.
   * \tparam F Type of the converted floating point value.
   * \param[in] first Start of memory block.
   * \param[out] last End of memory block (excluded).
   * \return Returns the converted value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * return converted value
   * \endinternal
   */
  template <typename F,
            std::enable_if_t<std::is_floating_point<F>::value && std::is_same<double, F>::value, std::int32_t> = 0>
  static F ConvertStringTo(ConstBufferPointer first, BufferPointer& last) noexcept {
    double const converted_value{strtod(first.ptr, &last.ptr)};  // VCA_CHARCONV_STRTOD_FUNC
    return converted_value;
  }

  /*!
   * \brief Checks if value returned by strtof is out of range for float.
   * \param[in] value Value to be checked.
   * \return \c true, if value is out of range, \c false otherwise.
   * \context ANY
   * \pre errno has to be set to 0 before strtof is called.
   * \threadsafe FALSE
   * \error CharconvErrc::kResultOutOfRange in case value is out of range.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  static bool IsValueOutOfRange(float value) noexcept;

  /*!
   * \brief Checks if value returned by strtod is out of range for double.
   * \param[in] value Value to be checked.
   * \return \c true, if value is out of range, \c false otherwise.
   * \context ANY
   * \pre errno has to be set to 0 before strtof is called.
   * \threadsafe FALSE
   * \error CharconvErrc::kResultOutOfRange in case value is out of range.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  static bool IsValueOutOfRange(double value) noexcept;

  // VECTOR NC AutosarC++17_10-A8.2.1: MD_CharacterConversion_A8.2.1_trailing_type_syntax
  /*!
   * \brief Parses character sequences for infinity or nan and updates current buffer.
   * \tparam T Data type of value.
   * \return Infinity or quiet nan.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case no conversion could be performed.
   * \reentrant TRUE
   * \trace SPEC-9718147, SPEC-9718146
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Parse leading minus sign
   * #20 If current buffer contains a character sequence of infinity type:
   *     - Return infinity value
   * #30 Else if current buffer contains a character sequence of nan type:
   *     - Return quiet nan
   * #40 Else reset current buffer.
   * \endinternal
   */
  // SPEC-9718147 (nan), SPEC-9718146 (infinity)
  template <typename T>
  amsr::core::Result<T> ParseNanAndInf() noexcept {
    amsr::core::Result<T> result{amsr::core::Result<T>::FromError(CharconvErrc::kInvalidArgument)};
    // #10
    bool const is_negative{ParseLeadingMinusSign()};

    if (ParseInfinity()) {  // #20
      if (is_negative) {
        result.EmplaceValue(-std::numeric_limits<T>::infinity());
      } else {
        result.EmplaceValue(std::numeric_limits<T>::infinity());
      }
    } else if (ParseNan()) {  // #30
      result.EmplaceValue(std::numeric_limits<T>::quiet_NaN());
    } else {  // #40
      ResetCurrentBuffer();
    }

    return result;
  }

  /*!
   * \brief Checks if current buffer contains a valid infinity string and updates buffer if this is the case.
   * \return \c true, if beginning of buffer matches inf or infinity (case does not matter), \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \trace SPEC-9718146
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  // SPEC-9718146 (infinity)
  bool ParseInfinity() noexcept;

  /*!
   * \brief Checks if current buffer contains a valid nan string and updates buffer if this is the case.
   * \return \c true, if beginning of buffer matches nan or nan(.*) where .* is a sequence of digits, letters or
   * underscores (case does not matter), \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \trace SPEC-9718147
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  // SPEC-9718147 (nan)
  bool ParseNan() noexcept;

  /*!
   * \brief Converts all letters in the character sequence to lowercase.
   * \param[in] original Original character sequence.
   * \return The character sequence in lowercase.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  static amsr::core::String ConvertToLowerCase(amsr::core::StringView original) noexcept;

  /*!
   * \brief Parses char_sequence of nan(char_sequence) if possible.
   * \context ANY
   * \pre buffer contains 'nan' (parsed) followed by '(' (not parsed)
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  void ParseNanExpressionInParentheses() noexcept;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  template <typename T>
  /*! Friend test declaration. */
  FRIEND_TEST(UT__ParseFloatingPoint, ReadFloatingPoint__MakeCString);
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  template <typename T>
  /*! Friend test declaration. */
  FRIEND_TEST(UT__ParseFloatingPoint, MoveConstructor);
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  template <typename T>
  /*! Friend test declaration. */
  FRIEND_TEST(UT__ParseFloatingPoint, MoveAssignment);
};

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_PARSE_FLOATING_POINT_H_

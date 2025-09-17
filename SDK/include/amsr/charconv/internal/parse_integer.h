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
 *        \brief  Parses next characters as integer.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_PARSE_INTEGER_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_PARSE_INTEGER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cmath>
#include <limits>

#include "amsr/charconv/charconv_error_domain.h"
#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/charconv/internal/str_to_types.h"
#include "amsr/charconv/internal/utility.h"
#include "amsr/charconv/type_helper.h"
#include "amsr/charconv_common/internal/buffer.h"
#include "amsr/charconv_common/internal/common_utility.h"
#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Parses next characters as integer.
 * \unit amsr::charconv::internal::ParseInteger
 * \vprivate Component Private
 */
class ParseInteger : public Buffer {
 public:
  // VECTOR NC AutosarC++17_10-A12.1.6: MD_CharacterConversion_A12.1.6_inheriting_constructors
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer_in Memory segment with textual representation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  explicit ParseInteger(amsr::core::StringView buffer_in) noexcept : Buffer(buffer_in) {}

 protected:
  /*!
   * \brief Move constructs instance of class.
   * \param[in] int_to_parse Instance of ParseInteger
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  ParseInteger(ParseInteger&& int_to_parse) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] int_to_parse Instance of ParseInteger
   * \return Moved ParseInteger
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  ParseInteger& operator=(ParseInteger&& int_to_parse) & noexcept = default;

  // ParseInteger is not copy constructible because it would break exclusive management of the resource.
  ParseInteger(ParseInteger const& int_to_parse) = delete;

  // ParseInteger is not copy assignable because it would break exclusive management of the resource.
  ParseInteger& operator=(ParseInteger const& int_to_parse) & = delete;

 public:
  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  ~ParseInteger() noexcept override = default;  // VCA_CHARCONV_DESTRUCTOR_MOLE_1298

 private:
  /*!
   * \brief Converts base enum to decimal number.
   * \tparam T Integer type.
   * \param[in] base Base as enum.
   * \return Base as decimal value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  template <typename T>
  static T GetBaseAsNumber(NumberBase base) noexcept {
    T base_as_int{10};  // NumberBase kDecimal
    if (base == NumberBase::kDecimal) {
      base_as_int = 10;
    } else if (base == NumberBase::kHex) {
      base_as_int = 16;
    } else {
      amsr::core::Abort(amsr::core::StringView{"ParseInteger::GetAsBaseNumber: Invalid base."}.data());
    }

    return base_as_int;
  }

  /*!
   * \brief Checks if character is a valid digit.
   * \param[in] ch Character.
   * \param[in] base Numerical base.
   * \return \c true if character is valid, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  static bool IsValidChar(char ch, NumberBase base) {
    bool const is_digit{IsDigit(static_cast<std::uint8_t>(ch))};
    bool const is_x_digit{IsXDigit(static_cast<std::uint8_t>(ch))};
    return ((base == NumberBase::kDecimal) && is_digit) || ((base == NumberBase::kHex) && is_x_digit);
  }

 public:
  // VECTOR NC AutosarC++17_10-A8.2.1: MD_CharacterConversion_A8.2.1_trailing_type_syntax
  // VECTOR NC Metric-HIS.PATH: MD_CharacterConversion_Metric-HIS.PATH
  /*!
   * \brief Reads a number.
   * \tparam T Type of value.
   * \param[in] base Number base.
   * \param[in] is_negative Flag for the sign of the value.
   * \return The converted value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case no conversion can be performed or current_buffer is empty.
   * \error CharconvErrc::kResultOutOfRange In case the converted value falls out of range of the corresponding return
   * type.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Iterate over valid digits.
   *     #20 Convert current character to numerical value.
   *     #30 If value is negative:
   *         #40 If next value is larger than lower bound of \c T:
   *             - Calculate next value and increase digit count.
   *         #45 Otherwise set out-of-range flag.
   *     #50 Else:
   *         #60 If next value is smaller than upper bound of \c T:
   *             - Calculate next value and increase digit count.
   *         #65 Otherwise set out-of-range flag.
   * #70 If result is out of range: set out of range error.
   * #80 If result is negative zero: reset buffer and set invalid argument error.
   * #90 Otherwise: return converted value.
   * \endinternal
   */
  template <typename T, std::enable_if_t<is_signed_and_not_char<T>::value, std::int32_t> = 0>
  amsr::core::Result<T> ReadNumber(NumberBase base, bool is_negative) noexcept {
    amsr::core::Result<T> result{amsr::core::Result<T>::FromError(CharconvErrc::kInvalidArgument)};
    std::uint32_t digit_count{0};
    bool is_out_of_range{false};
    T value{static_cast<T>(0)};

    // #10
    while (AreCharsLeft()) {
      T const factor{(is_negative) ? static_cast<T>(-1) : static_cast<T>(1)};
      T const base_as_int{GetBaseAsNumber<T>(base)};
      char const peek_ch{PeekChar()};
      if (!IsValidChar(peek_ch, base)) {
        break;
      }

      // #20
      char const ch{GetChar()};
      std::uint8_t const digit{AsciiToInt(ch, base)};
      T const digit_as_type_t{amsr::charconv::ConvertOrAbort<T>(digit)};
      if (is_negative) {  // #30
        bool const inside_lower_bound{value >= ((std::numeric_limits<T>::lowest() + digit_as_type_t) / base_as_int)};
        if (inside_lower_bound) {  // #40
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_integer_out_of_range
          // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_integer_out_of_range
          // VECTOR NC AutosarC++17_10-M5.0.3: MD_CharacterConversion_M5.0.3_change_of_signedness_calc
          value = amsr::charconv::ConvertOrAbort<T>((value * base_as_int) + (factor * digit_as_type_t));
          digit_count++;
        } else {  // #45
          is_out_of_range = true;
        }
      } else {  // #50
        bool const inside_upper_bound{value <= ((std::numeric_limits<T>::max() - digit_as_type_t) / base_as_int)};
        if (inside_upper_bound) {  // #60
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_integer_out_of_range
          // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_integer_out_of_range
          // VECTOR NC AutosarC++17_10-M5.0.3: MD_CharacterConversion_M5.0.3_change_of_signedness_calc
          value = amsr::charconv::ConvertOrAbort<T>((value * base_as_int) + (factor * digit_as_type_t));
          digit_count++;
        } else {  // #65
          is_out_of_range = true;
        }
      }
    }

    if (is_out_of_range) {  // #70
      result.EmplaceError(CharconvErrc::kResultOutOfRange);
    } else if ((value == 0) && (is_negative)) {  // #80
      ResetCurrentBuffer();
      result.EmplaceError(CharconvErrc::kInvalidArgument);
    } else {  // #90
      result.EmplaceValue(value);
    }

    return result;
  }

  // VECTOR NC AutosarC++17_10-A8.2.1: MD_CharacterConversion_A8.2.1_trailing_type_syntax
  // VECTOR NC Metric-HIS.PATH: MD_CharacterConversion_Metric-HIS.PATH
  /*!
   * \brief Reads a number.
   * \tparam T Type of value.
   * \param[in] base Number base.
   * \param[in] is_negative Flag for the sign of the value.
   * \return The converted value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case no conversion can be performed or current_buffer is empty.
   * \error CharconvErrc::kResultOutOfRange In case the converted value falls out of range of the corresponding return
   * type.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #05 If value is not negative:
   *     #10 Iterate over valid digits.
   *         #20 Convert current character to numerical value.
   *         #30 If next value is smaller than upper bound of \c T:
   *             - Calculate next value and increase digit count.
   *         #35 Otherwise set out-of-range flag.
   *     #40 If result is out of range: set out of range error.
   *     #50 Otherwise: return converted value.
   * \endinternal
   */
  template <typename T, std::enable_if_t<is_unsigned_and_not_char<T>::value, std::int32_t> = 0>
  amsr::core::Result<T> ReadNumber(NumberBase base, bool is_negative) noexcept {
    amsr::core::Result<T> result{amsr::core::Result<T>::FromError(CharconvErrc::kInvalidArgument)};
    if (!is_negative) {  // #05
      std::uint32_t digit_count{0};
      bool is_out_of_range{false};
      T value{static_cast<T>(0)};

      // #10
      while (AreCharsLeft()) {
        T const base_as_int{GetBaseAsNumber<T>(base)};
        char const peek_ch{PeekChar()};
        if (!IsValidChar(peek_ch, base)) {
          break;
        }

        // #20
        char const ch{GetChar()};
        std::uint8_t const digit{AsciiToInt(ch, base)};

        T const digit_as_type_t{amsr::charconv::Convert<T>(digit)};
        bool const inside_upper_bound{value <= ((std::numeric_limits<T>::max() - digit_as_type_t) / base_as_int)};
        if (inside_upper_bound) {  // #30
          // #40
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_integer_out_of_range
          // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_integer_out_of_range
          // VECTOR NC AutosarC++17_10-M5.0.3: MD_CharacterConversion_M5.0.3_change_of_signedness_calc
          // VECTOR NC AutosarC++17_10-M5.0.4: MD_CharacterConversion_M5.0.4_change_of_signedness_calc
          value = amsr::charconv::ConvertOrAbort<T>((value * base_as_int) + digit_as_type_t);
          digit_count++;
        } else {  // #35
          is_out_of_range = true;
        }
      }

      if (is_out_of_range) {  // #40
        result.EmplaceError(CharconvErrc::kResultOutOfRange);
      } else {  // #50
        result.EmplaceValue(value);
      }
    }
    return result;
  }

 private:
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename T>
  FRIEND_TEST(UT__ParseInteger__Typed, MoveConstructor);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CharacterConversion_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename T>
  FRIEND_TEST(UT__ParseInteger__Typed, MoveAssignment);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ParseInteger__Test, IsValidChar);
};

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_PARSE_INTEGER_H_

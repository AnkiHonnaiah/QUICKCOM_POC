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
 *        \brief  Converts textual representation of a signed integer to its numerical value.
 *
 *********************************************************************************************************************/
#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_SIGNED_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_SIGNED_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <type_traits>

#include "amsr/charconv/internal/parse_integer.h"
#include "amsr/charconv/internal/str_to_types.h"
#include "amsr/charconv/internal/to_number_template_class.h"
#include "amsr/charconv_common/internal/common_utility.h"
#include "amsr/charconv_common/internal/const_buffer_pointer.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a representation of a signed integer to a numerical value.
 * \tparam T Signed integer type.
 * \unit amsr::charconv::ToNumberSigned
 * \vprivate Product Private
 */
template <typename T>
class ToNumber<T, typename std::enable_if<is_signed_and_not_char<T>::value>::type>
    : public charconv::internal::ParseInteger {
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
  explicit ToNumber(amsr::core::StringView buffer_in) noexcept : ParseInteger{buffer_in} {};

  /*!
   * \brief Move constructs instance of class.
   * \param[in] to_number Instance of ToNumber
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  ToNumber(ToNumber&& to_number) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] to_number Instance of ToNumber
   * \return Moved ToNumber
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  ToNumber& operator=(ToNumber&& to_number) & noexcept = default;

  // ToNumber is not copy constructible because it would break exclusive management of the resource.
  ToNumber(ToNumber const& to_number) = delete;

  // ToNumber is not copy assignable because it would break exclusive management of the resource.
  ToNumber& operator=(ToNumber const& to_number) & = delete;

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
  ~ToNumber() noexcept override = default;  // VCA_CHARCONV_DESTRUCTOR_MOLE_1298

  /*!
   * \brief Converts a character sequence to a signed integer value.
   * \param[in] base Numerical base.
   * \return Converted value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case conversion is impossible or buffer size is 0.
   * \error CharconvErrc::kResultOutOfRange In case the converted value falls out of range of the corresponding return
   * type.
   * \reentrant TRUE
   * \trace DSGN-CharacterConversion-ToNumber
   * \trace SPEC-9718154, SPEC-9718156, SPEC-9718151, SPEC-9718152, SPEC-9718155, SPEC-17702356, SPEC-17702358
   * \trace SPEC-17702357
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   *
   * \internal
   * #10 Skip leading whitespace.
   * #20 If buffer is not empty AND first char is not plus sign AND not stand-alone minus sign:
   *     #30 Convert to value.
   * #40 Else: reset current buffer.
   * \endinternal
   */
  //  SPEC-9718154 (decimal), SPEC-9718156 (hexadecimal), SPEC-9718151 (leading whitespaces discarded),
  //  SPEC-9718152 (plus sign not valid), SPEC-9718155 (minus sign valid for signed integers)
  //  SPEC-17702356 (valid result), SPEC-17702358 (kResultOutOfRange), SPEC-17702357 (kInvalidArgument)
  amsr::core::Result<T> Convert(NumberBase const base = NumberBase::kDecimal) noexcept {  //
    amsr::core::Result<T> result{amsr::core::Result<T>::FromError(CharconvErrc::kInvalidArgument)};
    bool conversion_attempted{false};

    // #10
    this->SkipLeadingWhitespace();

    if (AreCharsLeft()) {  // #20
      char const next_char{PeekChar()};
      bool const only_one_char_left{GetNumberOfCharsLeft() == 1};
      if ((next_char != '+') && (!((next_char == '-') && only_one_char_left))) {  // #30
        result = ConvertToSigned(base);
        conversion_attempted = true;
      }
    }

    if (!conversion_attempted) {  // #40
      ResetCurrentBuffer();
    }

    return result;
  }  // COV_CONVERT_FUNCTION

 private:
  /*!
   * \brief Converts a character array to a signed integer value.
   * \param[in] base Numerical base.
   * \return Converted value.
   * \context ANY
   * \pre No leading whitespace, at least one character left
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case no conversion can be performed or buffer is empty.
   * \error CharconvErrc::kResultOutOfRange In case the converted value falls out of range of the corresponding return
   * type.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Parse leading minus sign.
   * #20 If base is decimal and if first character is digit: read number.
   * #30 If base is hexadecimal and first character is hex digit: read number.
   * #40 Otherwise, reset current buffer.
   * \endinternal
   */
  amsr::core::Result<T> ConvertToSigned(NumberBase base) noexcept {
    amsr::core::Result<T> result_value{amsr::core::Result<T>::FromError(CharconvErrc::kInvalidArgument)};

    // #10
    bool const is_negative{ParseLeadingMinusSign()};
    char const ch{PeekChar()};
    bool const is_digit{internal::IsDigit(static_cast<std::uint8_t>(ch))};
    bool const is_x_digit{internal::IsXDigit(static_cast<std::uint8_t>(ch))};

    if ((base == NumberBase::kDecimal) && is_digit) {
      result_value = ReadNumber<T>(base, is_negative);
    } else if ((base == NumberBase::kHex) && is_x_digit) {
      // #30
      result_value = ReadNumber<T>(NumberBase::kHex, is_negative);
    } else {  // #40
      ResetCurrentBuffer();
    }

    return result_value;
  }
};

}  // namespace charconv
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_CONVERT_FUNCTION
//   \ACCEPT XX
//   \REASON It is covered by multiple tests, e.g. by TYPED_TEST(UT__ToNumberSigned, Convert__Success__Decimal).
// COV_JUSTIFICATION_END

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_SIGNED_H_

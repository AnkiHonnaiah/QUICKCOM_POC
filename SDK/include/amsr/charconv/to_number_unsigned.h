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
 *        \brief  Converts textual representation of an unsigned integer to its numerical value.
 *
 *********************************************************************************************************************/
#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_UNSIGNED_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_UNSIGNED_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <type_traits>

#include "amsr/charconv/internal/parse_integer.h"
#include "amsr/charconv/internal/str_to_types.h"
#include "amsr/charconv/internal/to_number_template_class.h"
#include "amsr/charconv_common/internal/common_utility.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a textual representation of an unsigned integer to a numerical value.
 * \tparam T Unsigned integer type.
 * \unit amsr::charconv::ToNumberUnsigned
 * \vprivate Product Private
 */
template <typename T>
class ToNumber<T, typename std::enable_if<is_unsigned_and_not_char<T>::value>::type>
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
   * \brief Converts a character sequence to an unsigned integer value.
   * \param[in] base Numerical base.
   * \return Converted value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case no conversion can be performed or current_buffer is empty.
   * \error CharconvErrc::kResultOutOfRange In case the converted value falls out of range of the corresponding return
   * type.
   * \reentrant TRUE
   * \trace DSGN-CharacterConversion-ToNumber
   * \trace SPEC-9718154, SPEC-9718156, SPEC-9718151, SPEC-9718152, SPEC-9718153, SPEC-17702356, SPEC-17702358
   * \trace SPEC-17702357
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   *
   * \internal
   * #10 Skip leading whitespace.
   * #20 If buffer is not empty AND first char is not plus sign AND first char is not minus sign:
   *     #30 Convert to value.
   * #40 Else: reset current buffer.
   * \endinternal
   */
  //  SPEC-9718154 (decimal), SPEC-9718156 (hexadecimal)
  //  SPEC-9718151 (leading whitespaces discarded), SPEC-9718152 (plus sign not valid), SPEC-9718153 (minus sign
  //  * not valid for unsigned integers)
  //  SPEC-17702356 (valid result), SPEC-17702358 (kResultOutOfRange), SPEC-17702357 (kInvalidArgument)
  amsr::core::Result<T> Convert(NumberBase const base = NumberBase::kDecimal) noexcept {
    amsr::core::Result<T> result{amsr::core::Result<T>::FromError(CharconvErrc::kInvalidArgument)};
    bool conversion_attempted{false};

    // #10
    this->SkipLeadingWhitespace();

    if (AreCharsLeft()) {  // #20
      char const ch{PeekChar()};
      if ((ch != '+') && (!(ch == '-'))) {
        // #30
        result = ConvertToUnsigned(base);
        conversion_attempted = true;
      }
    }

    if (!conversion_attempted) {  // #40
      ResetCurrentBuffer();
    }

    return result;
  }

 private:
  /*!
   * \brief Converts a character array to an unsigned integer value.
   * \param[in] base Numerical base.
   * \return Converted value.
   * \context ANY
   * \pre A valid unsigned number without leading whitespace
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case no conversion can be performed or current_buffer is empty.
   * \error CharconvErrc::kResultOutOfRange In case the converted value falls out of range of the corresponding return
   * type.
   * \reentrant TRUE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 If base is decimal and first character is a digit: read number.
   * #20 If base is hexadecimal and first character is a hex digit: read number.
   * #30 Otherwise, reset current buffer.
   * \endinternal
   */
  amsr::core::Result<T> ConvertToUnsigned(NumberBase base) noexcept {
    amsr::core::Result<T> result_value{amsr::core::Result<T>::FromError(CharconvErrc::kInvalidArgument)};
    char const ch{PeekChar()};
    bool const is_digit{internal::IsDigit(static_cast<std::uint8_t>(ch))};
    bool const is_x_digit{internal::IsXDigit(static_cast<std::uint8_t>(ch))};

    if ((base == NumberBase::kDecimal) && is_digit) {  // #10
      result_value = ReadNumber<T>(NumberBase::kDecimal, false);
    } else if ((base == NumberBase::kHex) && is_x_digit) {  // #20
      result_value = ReadNumber<T>(NumberBase::kHex, false);
    } else {  // #30
      ResetCurrentBuffer();
    }

    return result_value;
  }
};

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_UNSIGNED_H_

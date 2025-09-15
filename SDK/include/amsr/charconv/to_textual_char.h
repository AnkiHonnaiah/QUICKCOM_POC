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
 *        \brief  Converts a char value to its textual representation.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_CHAR_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_CHAR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/charconv/internal/to_textual_template_class.h"
#include "amsr/charconv/to_textual_signed.h"
#include "amsr/charconv/to_textual_unsigned.h"
#include "amsr/charconv/type_helper.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a char value to its textual representation.
 * \tparam T Char type (if signed).
 * \unit amsr::charconv::ToTextualChar
 * \complexity This unit is basically just a wrapper to avoid questionable operations on a char data type. Therefore,
 * it is acceptable that the number of inter-unit calls is higher than the allowed maximum.
 * \vprivate Product Private
 */
template <typename T>
class ToTextual<T, typename std::enable_if<is_char_and_signed<T>::value>::type> final {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer Memory segment to save textual representation.
   * \param[in] value_in Signed char value to be converted.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ToTextual(amsr::core::Span<char> buffer, T value_in) noexcept
      : to_textual_{buffer, static_cast<std::int8_t>(value_in)} {}  // COV_CHARCONV_CHAR_TO_TEXTUAL_FUNCTIONS

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
   * \brief Converts a char value to its textual representation in the given base which is written
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
   *
   * \internal
   * - Convert char as std::int8_t.
   * \endinternal
   */
  // SPEC-9717269 (decimal), SPEC-9718138 (hexadecimal), SPEC-11451591 (binary)
  // SPEC-17702354 (valid result), SPEC-17702355 (kValueTooLarge)
  amsr::core::Result<amsr::core::Span<char>> Convert(NumberBase const base = NumberBase::kDecimal,
                                                     ExtendedFormat const fmt = extended_format_default) noexcept {
    return to_textual_.Convert(base, fmt);
  }  // COV_CHARCONV_CHAR_TO_TEXTUAL_FUNCTIONS

 private:
  /*! Conversion information of char. */
  ToTextual<std::int8_t> to_textual_;
};

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a char value to its textual representation.
 * \tparam T Char type (if unsigned).
 * \unit amsr::charconv::ToTextualChar
 * \vprivate Product Private
 */
template <typename T>
class ToTextual<T, typename std::enable_if<is_char_and_unsigned<T>::value>::type> final {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer Memory segment to save textual representation.
   * \param[in] value_in Unsigned char value to be converted.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ToTextual(amsr::core::Span<char> buffer, T value_in) noexcept
      : to_textual_{buffer, static_cast<std::uint8_t>(value_in)} {}  // COV_CHARCONV_CHAR_TO_TEXTUAL_FUNCTIONS

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
   * \brief Converts a char value to its textual representation in the given base which is written
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
   *
   * \internal
   * - Convert char as std::uint8_t.
   * \endinternal
   */
  // SPEC-9717269 (decimal), SPEC-9718138 (hexadecimal), SPEC-11451591 (binary)
  // SPEC-17702354 (valid result), SPEC-17702355 (kValueTooLarge)
  amsr::core::Result<amsr::core::Span<char>> Convert(NumberBase const base = NumberBase::kDecimal,
                                                     ExtendedFormat const fmt = extended_format_default) noexcept {
    return to_textual_.Convert(base, fmt);
  }  // COV_CHARCONV_CHAR_TO_TEXTUAL_FUNCTIONS

 private:
  /*! Conversion information of char. */
  ToTextual<std::uint8_t> to_textual_;
};

}  // namespace charconv
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_CHARCONV_CHAR_TO_TEXTUAL_FUNCTIONS
//   \ACCEPT XX
//   \REASON Depending on whether char is signed or unsigned in the current environment, one of the overloads will not
//           be executed by the unit tests, e.g. TEST_F(UT__ToTextualChar, Convert__Success).
// COV_JUSTIFICATION_END

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_CHAR_H_

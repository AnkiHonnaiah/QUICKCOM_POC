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
 *        \brief  Converts textual representation of a char to its numerical value.
 *
 *********************************************************************************************************************/
#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_CHAR_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_CHAR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <type_traits>

#include "amsr/charconv/internal/to_number_template_class.h"
#include "amsr/charconv/to_number_signed.h"
#include "amsr/charconv/to_number_unsigned.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a representation of a char value to a numerical value.
 * \tparam T Char type (if signed).
 * \unit amsr::charconv::ToNumberChar
 * \vprivate Product Private
 */
template <typename T>
class ToNumber<T, typename std::enable_if<is_char_and_signed<T>::value>::type> final {
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
  explicit ToNumber(amsr::core::StringView buffer_in) noexcept
      : buffer_{buffer_in} {};  // COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS

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
  ~ToNumber() noexcept = default;  // VCA_CHARCONV_DESTRUCTOR_MOLE_1298

  /*!
   * \brief Converts a character sequence to a char value.
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
   * - Convert char as std::int8_t.
   * - If successful: return converted value as char.
   * - Otherwise: return error.
   * \endinternal
   */
  //  SPEC-9718154 (decimal), SPEC-9718156 (hexadecimal), SPEC-9718151 (leading whitespaces discarded),
  //  SPEC-9718152 (plus sign not valid), SPEC-9718155 (minus sign valid for signed integers)
  //  SPEC-17702356 (valid result), SPEC-17702358 (kResultOutOfRange), SPEC-17702357 (kInvalidArgument)
  amsr::core::Result<T> Convert(NumberBase const base = NumberBase::kDecimal) const noexcept {
    ToNumber<std::int8_t> to_number{buffer_};
    return to_number.Convert(base)
        .Map([](std::int8_t value) { return static_cast<char>(value); })  // COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS
        .MapError([](ErrorCode errc) { return errc; });                   // COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS
  }                                                                       // COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS

 private:
  /*! Memory segment with textual representation. */
  amsr::core::StringView buffer_;
};

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a representation of a char value to a numerical value.
 * \tparam T Char type (if unsigned).
 * \unit amsr::charconv::ToNumberChar
 * \vprivate Product Private
 */
template <typename T>
class ToNumber<T, typename std::enable_if<is_char_and_unsigned<T>::value>::type> final {
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
  explicit ToNumber(amsr::core::StringView buffer_in) noexcept
      : buffer_{buffer_in} {};  // COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS

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
  ~ToNumber() noexcept = default;  // VCA_CHARCONV_DESTRUCTOR_MOLE_1298

  /*!
   * \brief Converts a character sequence to a char value.
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
   * - Convert char as std::uint8_t.
   * - If successful: return converted value as char.
   * - Otherwise: return error.
   * \endinternal
   */
  //  SPEC-9718154 (decimal), SPEC-9718156 (hexadecimal), SPEC-9718151 (leading whitespaces discarded),
  //  SPEC-9718152 (plus sign not valid), SPEC-9718155 (minus sign valid for signed integers)
  //  SPEC-17702356 (valid result), SPEC-17702358 (kResultOutOfRange), SPEC-17702357 (kInvalidArgument)
  amsr::core::Result<T> Convert(NumberBase const base = NumberBase::kDecimal) const noexcept {
    ToNumber<std::uint8_t> to_number{buffer_};
    return to_number.Convert(base)
        .Map([](std::uint8_t value) { return static_cast<char>(value); })  // COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS
        .MapError([](ErrorCode errc) { return errc; });                    // COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS
  }                                                                        // COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS

 private:
  /*! Memory segment with textual representation. */
  amsr::core::StringView buffer_;
};

// COV_JUSTIFICATION_BEGIN
// \ID COV_CHARCONV_CHAR_TO_NUMBER_FUNCTIONS
//   \ACCEPT XX
//   \REASON Depending on whether char is signed or unsigned in the current environment, one of the overloads will not
//           be executed by the unit tests, e.g. TEST_F(UT__ToNumberChar, Convert__Success).
// COV_JUSTIFICATION_END

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_CHAR_H_

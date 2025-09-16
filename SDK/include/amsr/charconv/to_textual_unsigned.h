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
 *        \brief  Converts an unsigned integer value to its textual representation.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_UNSIGNED_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_UNSIGNED_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <bitset>
#include <sstream>
#include <type_traits>

#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/charconv/internal/to_textual_template_class.h"
#include "amsr/charconv/internal/utility.h"
#include "amsr/charconv/type_helper.h"
#include "amsr/charconv_common/internal/to_textual_buffer.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts an unsigned integer value to its textual representation.
 * \tparam T Unsigned integer type.
 * \unit amsr::charconv::ToTextualUnsigned
 * \vprivate Product Private
 */
template <typename T>
class ToTextual<T, typename std::enable_if<is_unsigned_and_not_char<T>::value>::type> final {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer Memory segment to save textual representation.
   * \param[in] value_in Unsigned integer value to be converted.
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
   * \brief Converts an unsigned integer value to its textual representation in the given base which is written
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
    return WriteUnsignedInteger(base, fmt);
  }

 private:
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CharacterConversion_A15.5.3_span
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CharacterConversion_A15.4.2_span
  /*!
   * \brief Converts an unsigned integer value to its textual representation in the given base which is written
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
   * #10 Set format properties of stream and write value
   * #20 Copy stream contents to buffer.
   * \endinternal
   */
  amsr::core::Result<amsr::core::Span<char>> WriteUnsignedInteger(NumberBase const base,
                                                                  ExtendedFormat const fmt) noexcept {
    // #10
    std::stringstream stream;                          // VCA_CHARCONV_STRINGSTREAM_CONSTRUCTOR
    internal::SetStreamFillCharAndWidth(fmt, stream);  // VCA_CHARCONV_SETSTREAMFILLCHAR_FUNC
    std::uint64_t const val{amsr::charconv::Convert<std::uint64_t>(value_)};

    if (base == NumberBase::kHex) {
      internal::SetStreamCase(fmt, stream);  // VCA_CHARCONV_SETSTREAMCASE_FUNC
      stream << std::hex << val;             // VCA_CHARCONV_STRINGSTREAM_INSERTION_OPERATOR_CONTRACT
    } else if (base == NumberBase::kBinary) {
      amsr::core::String const value_binary{
          (std::bitset<sizeof(T) * kBitsInByte>(value_)).to_string()};  // VCA_CHARCONV_BITSET_TO_STRING
      stream << internal::SkipLeadingZeros(value_binary);               // VCA_CHARCONV_SKIPLEADINGZEROS_FUNC
    } else {
      stream << std::dec << val;  // VCA_CHARCONV_STRINGSTREAM_INSERTION_OPERATOR_CONTRACT
    }

    // #20
    amsr::core::String const literal{internal::ToString(stream)};  // VCA_CHARCONV_TOSTRING_FUNC
    // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_WRITE
    return buffer_.DoSafeMemcpy(amsr::core::StringView{literal});
  }  // VCA_CHARCONV_BASICSTRING_DESTRUCTOR, VCA_CHARCONV_BASICSTRINGSTREAM_DESTRUCTOR

  /*! buffer to save textual representation */
  internal::ToTextualBuffer buffer_;

  /*! value to be converted */
  T value_;
};

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_UNSIGNED_H_

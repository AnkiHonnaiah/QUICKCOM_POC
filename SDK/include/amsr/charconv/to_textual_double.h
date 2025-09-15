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
 *        \brief  Converts a double value to its textual representation.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_DOUBLE_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_DOUBLE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iomanip>
#include <limits>
#include <sstream>
#include <type_traits>

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
 * \brief Converts a double value to its textual representation.
 * \unit amsr::charconv::ToTextualDouble
 * \vprivate Product Private
 */
template <>
class ToTextual<double> final {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer Memory segment to save textual representation.
   * \param[in] value_in Double value to be converted.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ToTextual(amsr::core::Span<char> buffer, double value_in) noexcept
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
   * \brief Converts a double value to its textual representation and writes it to buffer.
   * \param[in] fmt Formatting parameters for floating point output.
   * \return Textual representation of value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kValueTooLarge In case the value is larger than the specified range.
   * \reentrant TRUE
   * \trace DSGN-CharacterConversion-ToTextual
   * \trace SPEC-9717268, SPEC-9717267, SPEC-9717266, SPEC-9717270, SPEC-9717271, SPEC-17702354, SPEC-17702355
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  // SPEC-9717268 (fixed format), SPEC-9717267 (scientific format), SPEC-9717266 (general format)
  // SPEC-9717270 (infinity), SPEC-9717271 (nan), SPEC-17702354 (valid result), SPEC-17702355 (kValueTooLarge)
  amsr::core::Result<amsr::core::Span<char>> Convert(FPFormat const fmt = fp_format_default) noexcept {
    return WriteFloatingPoint(fmt);
  }

 private:
  /*!
   * \brief Converts a double value to its textual representation and writes it to buffer.
   * \param[in] fmt Formatting parameters for floating point output.
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
   * #10 Set format properties of stream and write value.
   * #20 If buffer is large enough, copy stream contents to memory segment.
   * \endinternal
   */
  amsr::core::Result<amsr::core::Span<char>> WriteFloatingPoint(FPFormat const fmt) noexcept {
    // #10
    std::stringstream stream{};                                              // VCA_CHARCONV_STRINGSTREAM_CONSTRUCTOR
    stream << std::setprecision(std::numeric_limits<double>::digits10 + 1);  // VCA_CHARCONV_SETPRECISION_FUNC
    if (fmt == amsr::charconv::FPFormat::kFixed) {
      stream << std::fixed << value_;  // VCA_CHARCONV_STRINGSTREAM_INSERTION_OPERATOR_CONTRACT
    } else if (fmt == amsr::charconv::FPFormat::kScientific) {
      stream << std::scientific << value_;    // VCA_CHARCONV_STRINGSTREAM_INSERTION_OPERATOR_CONTRACT
    } else {                                  // fmt is FPFormat::kGeneral
      stream << std::defaultfloat << value_;  // VCA_CHARCONV_STRINGSTREAM_INSERTION_OPERATOR_CONTRACT
    }

    amsr::core::String const literal{internal::ToString(stream)};  // VCA_CHARCONV_TOSTRING_FUNC
    // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_NO_WRITE
    return buffer_.DoSafeMemcpy(amsr::core::StringView{literal});  // #20
  }                                                                // VCA_CHARCONV_BASICSTRINGSTREAM_DESTRUCTOR

  /*! buffer to save textual representation */
  internal::ToTextualBuffer buffer_;

  /*! value to be converted */
  double value_;
};

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_DOUBLE_H_

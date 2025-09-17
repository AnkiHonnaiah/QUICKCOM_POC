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
 *        \brief  Converts a boolean value to a textual representation.
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_BOOL_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_BOOL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/charconv/internal/to_textual_template_class.h"
#include "amsr/charconv/type_helper.h"
#include "amsr/charconv_common/internal/boolean_literals.h"
#include "amsr/charconv_common/internal/common_utility.h"
#include "amsr/charconv_common/internal/to_textual_buffer.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a boolean value to a textual representation.
 * \unit amsr::charconv::ToTextualBool
 * \vprivate Product Private
 */
template <>
class ToTextual<bool> final {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer_in Memory segment to save textual representation.
   * \param[in] value_in Boolean value to be converted.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ToTextual(amsr::core::Span<char> buffer_in, bool value_in) noexcept
      : buffer_{buffer_in}, value_{value_in} {}  // VCA_CHARCONV_UNDEFINED_FUNC_THIS_POINTER_CONTAINER

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
   * \brief Converts a boolean value to its textual representation which is written successively to buffer.
   * \return Textual representation of value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kValueTooLarge In case the value is larger than the specified range.
   * \reentrant TRUE
   * \trace DSGN-CharacterConversion-ToTextual
   * \trace SPEC-9718139, SPEC-17702354, SPEC-17702355
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  // SPEC-9718139 (bool), SPEC-17702354 (valid result), SPEC-17702355 (kValueTooLarge)
  amsr::core::Result<amsr::core::Span<char>> Convert() noexcept;

 private:
  /*!
   * \brief Converts a boolean value to its textual representation which is written successively to buffer.
   * \param[in] literal String view of boolean value.
   * \return Textual representation of value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kValueTooLarge In case the value is larger than the specified range.
   * \reentrant TRUE
   * \trace DSGN-CharacterConversion-ToTextual
   * \trace SPEC-9718139, SPEC-17702354, SPEC-17702355
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char>> WriteBool(amsr::core::StringView literal) noexcept;

  /*! buffer to save textual representation */
  internal::ToTextualBuffer buffer_;

  /*! value to be converted */
  bool value_;
};

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_TO_TEXTUAL_BOOL_H_

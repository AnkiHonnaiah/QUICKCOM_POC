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
 *        \brief  Converts textual representation of a float to its numerical value.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_FLOAT_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_FLOAT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>

#include "amsr/charconv/internal/parse_floating_point.h"
#include "amsr/charconv/internal/to_number_template_class.h"
#include "amsr/charconv/type_helper.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {

// VECTOR NC AutosarC++17_10-M14.7.3: MD_CharacterConversion_M14.7.3_specialization_in_different_file
/*!
 * \brief Converts a textual representation of a float to a numerical value.
 * \unit amsr::charconv::ToNumberFloat
 * \vprivate Product Private
 */
template <>
class ToNumber<float> : public internal::ParseFloatingPoint {
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
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit ToNumber(amsr::core::StringView buffer_in) noexcept : ParseFloatingPoint{buffer_in} {};

  /*!
   * \brief Move constructs instance of class.
   * \param[in] to_number Instance of ToNumber
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
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
   * \vprivate Product Private
   * \steady UNKNOWN
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
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~ToNumber() noexcept override = default;  // VCA_CHARCONV_DESTRUCTOR_MOLE_1298

  /*!
   * \brief Converts a character sequence to a float value.
   * \param[in] fmt Formatting parameters for floating point output.
   * \return Converted value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error CharconvErrc::kInvalidArgument In case conversion is impossible or buffer size is 0.
   * \error CharconvErrc::kResultOutOfRange In case the converted value falls out of range of the corresponding return
   * type.
   * \reentrant TRUE
   * \trace DSGN-CharacterConversion-ToNumber
   * \trace SPEC-9718151, SPEC-9718152, SPEC-17702356, SPEC-17702358, SPEC-17702357
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  // SPEC-9718151 (leading whitespaces discarded), SPEC-9718152 (plus sign not valid)
  // SPEC-17702356 (valid result), SPEC-17702358 (kResultOutOfRange), SPEC-17702357 (kInvalidArgument)
  amsr::core::Result<float> Convert(FPFormat const fmt) noexcept;
};

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_TO_NUMBER_FLOAT_H_

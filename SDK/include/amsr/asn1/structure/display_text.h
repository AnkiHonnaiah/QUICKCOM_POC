/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  amsr/asn1/structure/display_text.h
 *        \brief  Representation of parsed Display Text.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DISPLAY_TEXT_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DISPLAY_TEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/encoder/encode_util.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/variant.h"

namespace amsr {
namespace asn1 {
namespace structure {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Type Alias
 */
using DisplayText =
    amsr::core::Variant<structure::Ia5String, structure::VisibleString, structure::BMPString, structure::Utf8String>;

/*!
 * \brief Encoding utils for display text.
 */
class DisplayTextUtils {
 public:
  /*!
   * \brief Encodes DisplayText.
   * \param[in] display_text The DisplayText to encode.
   * \return The DER-encoded display text.
   */
  static auto EncodeDisplayText(DisplayText const& display_text) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{
        amsr::core::Result<ara::core::Vector<std::uint8_t>>::FromError(
            Asn1Errc::kConstraintCheckFail,
            "Tried to encode a DisplayString that is not between 1 and 200 characters long."_sv)};

    if ((display_text.get_if<structure::Ia5String>() != nullptr) &&
        ((*display_text.get_if<structure::Ia5String>()).size() >= 1) &&
        ((*display_text.get_if<structure::Ia5String>()).size() <= 200)) {
      result = amsr::asn1::encoder::EncoderUtility::EncodeIa5String(Asn1Parser::Asn1Class::kUniversal,
                                                                    *display_text.get_if<structure::Ia5String>());

    } else if ((display_text.get_if<structure::VisibleString>() != nullptr) &&
               ((*display_text.get_if<structure::VisibleString>()).size() >= 1) &&
               ((*display_text.get_if<structure::VisibleString>()).size() <= 200)) {
      result = amsr::asn1::encoder::EncoderUtility::EncodeVisibleString(
          Asn1Parser::Asn1Class::kUniversal, *display_text.get_if<structure::VisibleString>());

    } else if ((display_text.get_if<structure::BMPString>() != nullptr) &&
               ((*display_text.get_if<structure::BMPString>()).size() >= 1) &&
               ((*display_text.get_if<structure::BMPString>()).size() <= 200)) {
      result = amsr::asn1::encoder::EncoderUtility::EncodeBMPString(Asn1Parser::Asn1Class::kUniversal,
                                                                    *display_text.get_if<structure::BMPString>());

    } else if ((display_text.get_if<structure::Utf8String>() != nullptr) &&
               ((*display_text.get_if<structure::Utf8String>()).size() >= 1) &&
               ((*display_text.get_if<structure::Utf8String>()).size() <= 200)) {
      result = amsr::asn1::encoder::EncoderUtility::EncodeUtf8String(Asn1Parser::Asn1Class::kUniversal, false,
                                                                     *display_text.get_if<structure::Utf8String>());
    } else {
      // result initialized with kConstraintCheckFail error
    }

    return result;
  }
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_DISPLAY_TEXT_H_

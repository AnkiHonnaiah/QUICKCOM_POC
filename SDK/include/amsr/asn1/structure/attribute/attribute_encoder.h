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
/*!        \file  attribute_encoder.h
 *        \brief
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATTRIBUTE_ATTRIBUTE_ENCODER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATTRIBUTE_ATTRIBUTE_ENCODER_H_

#include "amsr/asn1/asn1_error_domain.h"
#include "amsr/asn1/encoder/encode_util.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/asn1/structure/directory_string.h"
#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"
#include "constants.h"

namespace amsr {
namespace asn1 {
namespace structure {
namespace attribute {
/*!
 * \brief Utility for encoding attributes
 * \details \c ATTRIBUTE representations (<tt>SingleAttribute</tt>, <tt>AttributeSet</tt>) can contain values of a
 *      variety of known and an infinite amount of unknown types. This makes encoding potentially complex, which is why
 *      this utility exists. It tries to handle both single-valued and set-valued attributes while keeping the actual
 *      encoding routines minimal. Extending this class <b>should</b> only require expanding the type check and the
 *      value encoding.
 */
class AttributeEncoder {
 public:
  /*!
   * \brief Encode an attribute based on its members
   * \tparam Ts The possible element types
   * \param type The type of the attribute
   * \param value The (variant) value of the attribute
   * \return A result containing a DER-encoded representation of the SingleAttribute or AttributeSet.
   */
  template <typename... Ts>
  static auto Encode(Oid const& type, amsr::core::Variant<Ts...> const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    ara::core::Vector<uint8_t> encoded_data{};

    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{amsr::asn1::encoder::EncoderUtility::EncodeOid(type)};

    if (result) {
      static_cast<void>(encoded_data.insert(encoded_data.end(), result->begin(), result->end()));
      result = EncodeAttributeValue(type, value);
    }

    if (result) {
      static_cast<void>(encoded_data.insert(encoded_data.end(), result->begin(), result->end()));
      ara::core::Vector<std::uint8_t> const header{amsr::asn1::encoder::EncoderUtility::EncodeHeader(
          Asn1Parser::Asn1Class::kUniversal, true,
          static_cast<std::uint64_t>(Asn1Parser::UniversalElementTag::kSequence), encoded_data.size())};
      static_cast<void>(encoded_data.insert(encoded_data.begin(), header.begin(), header.end()));
      result.EmplaceValue(encoded_data);
    }

    return result;
  }

  /*!
   * \brief Encodes the value portion of an attribute.
   * \tparam Ts The possible element types. They can be vectors.
   * \param type The type of the attribute
   * \param value The value of the attribute
   * \return A result containing a DER-encoded representation of the value of the SingleAttribute or AttributeSet.
   */
  template <typename... Ts>
  static auto EncodeAttributeValue(Oid const& type, amsr::core::Variant<Ts...> const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{Asn1Errc::kRuntimeFault};
    if (MapsToDirectoryString(type)) {
      result = EncodeIfOfType<DirectoryString>(value);
    } else if (MapsToPrintableString(type)) {
      result = EncodeIfOfType<PrintableString>(value);
    } else if (MapsToIa5String(type)) {
      result = EncodeIfOfType<Ia5String>(value);
    } else {
      result = EncodeIfOfType<RawData>(value);
    }
    return result;
  }

 private:
  /*!
   * \brief Checks if a given value is of an assumed type and encodes it if so. If not, returns an error.
   * \tparam T The assumed element type
   * \param value The variant possibly containing a value of type \c T
   * \return A result containing a DER-encoded representation of the value or an error if encoding failed or the variant
   *    does not contain a value of the assumed type.
   */
  template <typename T, typename... Ts>
  static auto EncodeIfOfType(amsr::core::Variant<Ts...> const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{
        amsr::core::Result<ara::core::Vector<std::uint8_t>>::FromError(
            Asn1Errc::kRuntimeFault, "A SingleAttribute's value's physical type did not match its OID."_sv)};
    if (T const* const ptr{value.template get_if<T>()}) {
      result = EncodeTypedValue(*ptr);
    }
    return result;
  }

  /*!
   * \brief Checks if a given value is of an assumed vector type and encodes it if so. If not, returns an error.
   * \tparam T The assumed element type of the vector. That is, \c value is assumed to contain a <tt>Vector\<T\></tt>.
   * \param value The variant possibly containing a value of type <tt>Vector\<T\></tt>.
   * \return A result containing a DER-encoded representation of the value or an error if encoding failed or the variant
   *    does not contain a value of the assumed type.
   */
  template <typename T, typename... Ts>
  static auto EncodeIfOfType(amsr::core::Variant<ara::core::Vector<Ts>...> const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{
        amsr::core::Result<ara::core::Vector<std::uint8_t>>::FromError(
            Asn1Errc::kRuntimeFault, "An AttributeSet's value's physical type did not match its OID."_sv)};
    if (ara::core::Vector<T> const* const ptr{value.template get_if<ara::core::Vector<T>>()}) {
      result = EncodeTypedValue(*ptr);
    }
    return result;
  }

  /*!
   * \brief Encodes a single value or value element (for vectors), the type of which has already been checked.
   * \param value The DirectoryString to encode
   * \return A result containing a DER-encoded representation of the single value.
   */
  static auto EncodeTypedValue(DirectoryString const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{
        amsr::core::Result<ara::core::Vector<std::uint8_t>>::FromError(
            Asn1Errc::kInvalidContent,
            "A DirectoryString cannot be encoded because it is of an invalid physical type."_sv)};

    if (T61String const* const t61_string{value.get_if<T61String>()}) {
      result = encoder::EncoderUtility::EncodeT61String(Asn1Parser::Asn1Class::kUniversal, *t61_string);
    } else if (UniversalString const* const uni_string{value.get_if<UniversalString>()}) {
      result = encoder::EncoderUtility::EncodeUniversalString(Asn1Parser::Asn1Class::kUniversal, *uni_string);
    } else if (BMPString const* const bmp_string{value.get_if<BMPString>()}) {
      result = encoder::EncoderUtility::EncodeBMPString(Asn1Parser::Asn1Class::kUniversal, *bmp_string);
    } else if (PrintableString const* const pri_string{value.get_if<PrintableString>()}) {
      result = encoder::EncoderUtility::EncodePrintableString(Asn1Parser::Asn1Class::kUniversal, *pri_string);
    } else if (Utf8String const* const utf_string{value.get_if<Utf8String>()}) {
      result = encoder::EncoderUtility::EncodeUtf8String(Asn1Parser::Asn1Class::kUniversal, false, *utf_string);
    } else {
      // Error in default result.
    }
    return result;
  }
  /*!
   * \brief Encodes a single value or value element (for vectors), the type of which has already been checked.
   * \param value The PrintableString to encode
   * \return A result containing a DER-encoded representation of the single value.
   */
  static auto EncodeTypedValue(PrintableString const& value) -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    return encoder::EncoderUtility::EncodePrintableString(Asn1Parser::Asn1Class::kUniversal, value);
  }
  /*!
   * \brief Encodes a single value or value element (for vectors), the type of which has already been checked.
   * \param value The Ia5String to encode
   * \return A result containing a DER-encoded representation of the single value.
   */
  static auto EncodeTypedValue(Ia5String const& value) -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    return encoder::EncoderUtility::EncodeIa5String(Asn1Parser::Asn1Class::kUniversal, value);
  }
  /*!
   * \brief Encodes a single value or value element (for vectors), the type of which has already been checked.
   * \param value The RawData to encode
   * \return A result containing a DER-encoded representation of the single value.
   */
  static auto EncodeTypedValue(RawData const& value) -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    return value.Data();
  }

  /*!
   * \brief Encodes a vector of single elements.
   * \tparam T The element type in the vector
   * \param value The Vector to encode
   * \return A result containing a DER-encoded representation of the vector value.
   */
  template <typename T>
  static auto EncodeTypedValue(ara::core::Vector<T> const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{
        amsr::core::Result<ara::core::Vector<std::uint8_t>>::FromError(Asn1Errc::kRuntimeFault,
                                                                       "Tried to encode an empty AttributeSet."_sv)};
    ara::core::Vector<std::uint8_t> encoded_data{};

    // NOTE: Empty sets are not allowed.
    for (typename ara::core::Vector<T>::const_reference element : value) {
      result = EncodeTypedValue(element);
      if (result) {
        static_cast<void>(encoded_data.insert(encoded_data.end(), result->begin(), result->end()));
      } else {
        break;
      }
    }

    if (result) {
      ara::core::Vector<std::uint8_t> const header{amsr::asn1::encoder::EncoderUtility::EncodeHeader(
          Asn1Parser::Asn1Class::kUniversal, true, static_cast<std::uint64_t>(Asn1Parser::UniversalElementTag::kSet),
          encoded_data.size())};
      static_cast<void>(encoded_data.insert(encoded_data.begin(), header.begin(), header.end()));
      result.EmplaceValue(encoded_data);
    }
    return result;
  }
};
}  // namespace attribute
}  // namespace structure
}  // namespace asn1
}  // namespace amsr
#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATTRIBUTE_ATTRIBUTE_ENCODER_H_

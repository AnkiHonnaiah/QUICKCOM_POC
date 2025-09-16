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
/*!        \file  amsr/asn1/encoder/encode_util.h
 *        \brief  Utility class to provide encoder functions.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ENCODER_ENCODE_UTIL_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ENCODER_ENCODE_UTIL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "ara/core/vector.h"

#include "amsr/asn1/asn1_parser.h"
#include "amsr/asn1/structure/asn1_primitive_types.h"

namespace amsr {
namespace asn1 {
namespace encoder {

/*!
 * \brief Utility to encode primitive types.
 */
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.One_encoder_must_support_all_asn1_types
class EncoderUtility {
 public:
  /*!
   * \brief Encodes the identifier and length.
   * \details Encodes the identifier and length octets.
   * \param[in] element_class element class.
   * \param[in] constructed true if constructed.
   * \param[in] tag element tag.
   * \param[in] content_size content size in bytes.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeHeader(Asn1Parser::Asn1Class element_class, bool constructed, std::uint64_t tag,
                           size_t content_size) noexcept -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes the GeneralType.
   * \details Encodes the GeneralType.
   * \param[in] element_class element class.
   * \param[in] constructed true if constructed..
   * \param[in] tag element tag.
   * \param[in] exclude_header encode content only.
   * \param[in] value GeneralType value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeGeneralType(Asn1Parser::Asn1Class element_class, bool constructed, std::uint64_t tag,
                                bool exclude_header, structure::GeneralType<std::uint8_t> const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes the String.
   * \details Encodes the String.
   * \param[in] element_class element class.
   * \param[in] constructed true if constructed..
   * \param[in] tag element tag.
   * \param[in] exclude_header encode content only.
   * \param[in] value String value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeString(Asn1Parser::Asn1Class element_class, bool constructed, std::uint64_t tag,
                           bool exclude_header, amsr::core::String const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes the NumericalType.
   * \details Encodes the NumericalType.
   * \param[in] element_class element class.
   * \param[in] tag element tag.
   * \param[in] exclude_header encode content only.
   * \param[in] value NumericalType value to be encoded.
   * \return encoded data.
   * \error Asn1Errc::kInvalidContent If the numerical value is empty.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeNumericalType(Asn1Parser::Asn1Class element_class, std::uint64_t tag, bool exclude_header,
                                  structure::NumericalType<std::uint8_t> const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
  /*!
   * \brief Encodes boolean data.
   * \details Encodes the input boolean data.
   * \param[in] element_class element class.
   * \param[in] value boolean value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeBoolean(Asn1Parser::Asn1Class element_class, structure::Boolean value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes Null data.
   * \details Encodes the input Null data.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeNull() noexcept -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes integer data.
   * \details Encodes the input integer data.
   * \param[in] element_class element class.
   * \param[in] value Integer value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \error Asn1Errc::kInvalidContent If the input integer is empty.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeInteger(Asn1Parser::Asn1Class element_class, structure::Integer const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
  /*!
   * \brief Encodes Enumerated data.
   * \details Encodes the input Enumerated data.
   * \param[in] element_class element class.
   * \param[in] value Enumerated value to be encoded.
   * \return encoded data.
   * \error Asn1Errc::kInvalidContent If the input value is empty.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeEnumerated(Asn1Parser::Asn1Class element_class, structure::Enumerated const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Encodes bitstring data.
   * \details Encodes the input bitstring data.
   * \param[in] element_class element class.
   * \param[in] constructed true iff constructed.
   * \param[in] tag element tag.
   * \param[in] exclude_header encode content only.
   * \param[in] value bitstring value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kInvalidUnusedBits If the number of unused bits is more than 7.
   * \vprivate Product Private
   */
  static auto EncodeBitString(Asn1Parser::Asn1Class element_class, bool constructed, std::uint64_t tag,
                              bool exclude_header, structure::BitString const& value) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
  /*!
   * \brief Encodes octet string data.
   * \details Encodes the input octet string data.
   * \param[in] element_class element class.
   * \param[in] constructed true iff constructed.
   * \param[in] value octet string value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeOctetString(Asn1Parser::Asn1Class element_class, bool constructed,
                                structure::OctetString const& value) noexcept -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes utf8 string data.
   * \details Encodes the input utf8 string data.
   * \param[in] element_class element class.
   * \param[in] constructed true iff constructed.
   * \param[in] value utf8 string value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeUtf8String(Asn1Parser::Asn1Class element_class, bool constructed,
                               structure::Utf8String const& value) noexcept -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes Utc time data.
   * \details Encodes the input Utc time data.
   * \param[in] element_class element class.
   * \param[in] value Utc time value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeUtcTime(Asn1Parser::Asn1Class element_class, structure::UtcTime const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes Generalized time data.
   * \details Encodes the input Generalized time data.
   * \param[in] element_class element class.
   * \param[in] value Generalized time value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeGeneralizedTime(Asn1Parser::Asn1Class element_class,
                                    structure::GeneralizedTime const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes ObjectDescriptor data.
   * \details Encodes the input ObjectDescriptor data.
   * \param[in] element_class element class.
   * \param[in] value ObjectDescriptor value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeObjectDescriptor(Asn1Parser::Asn1Class element_class,
                                     structure::ObjectDescriptor const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes Time data.
   * \details Encodes the input Time data.
   * \param[in] element_class element class.
   * \param[in] value Time value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeTime(Asn1Parser::Asn1Class element_class, structure::Time const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes NumericString data.
   * \details Encodes the input NumericString data.
   * \param[in] element_class element class.
   * \param[in] value NumericString value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeNumericString(Asn1Parser::Asn1Class element_class, structure::NumericString const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes PrintableString data.
   * \details Encodes the input PrintableString data.
   * \param[in] element_class element class.
   * \param[in] value PrintableString value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodePrintableString(Asn1Parser::Asn1Class element_class,
                                    structure::PrintableString const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes T61String data.
   * \details Encodes the input T61String data.
   * \param[in] element_class element class.
   * \param[in] value T61String value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeT61String(Asn1Parser::Asn1Class element_class, structure::T61String const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes VideotexString data.
   * \details Encodes the input VideotexString data.
   * \param[in] element_class element class.
   * \param[in] value VideotexString value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeVideotexString(Asn1Parser::Asn1Class element_class, structure::VideotexString const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes Ia5String data.
   * \details Encodes the input Ia5String data.
   * \param[in] element_class element class.
   * \param[in] value Ia5String value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeIa5String(Asn1Parser::Asn1Class element_class, structure::Ia5String const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes GraphicString data.
   * \details Encodes the input GraphicString data.
   * \param[in] element_class element class.
   * \param[in] value GraphicString value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeGraphicString(Asn1Parser::Asn1Class element_class, structure::GraphicString const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes VisibleString data.
   * \details Encodes the input VisibleString data.
   * \param[in] element_class element class.
   * \param[in] value VisibleString value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeVisibleString(Asn1Parser::Asn1Class element_class, structure::VisibleString const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes GeneralString data.
   * \details Encodes the input GeneralString data.
   * \param[in] element_class element class.
   * \param[in] value GeneralString value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeGeneralString(Asn1Parser::Asn1Class element_class, structure::GeneralString const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes UniversalString data.
   * \details Encodes the input UniversalString data.
   * \param[in] element_class element class.
   * \param[in] value UniversalString value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeUniversalString(Asn1Parser::Asn1Class element_class,
                                    structure::UniversalString const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes BMPString data.
   * \details Encodes the input BMPString data.
   * \param[in] element_class element class.
   * \param[in] value BMPString value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeBMPString(Asn1Parser::Asn1Class element_class, structure::BMPString const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes Date data.
   * \details Encodes the input Date data.
   * \param[in] element_class element class.
   * \param[in] value Date value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeDate(Asn1Parser::Asn1Class element_class, structure::Date const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes TimeOfDay data.
   * \details Encodes the input TimeOfDay data.
   * \param[in] element_class element class.
   * \param[in] value TimeOfDay value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeTimeOfDay(Asn1Parser::Asn1Class element_class, structure::TimeOfDay const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes DateTime data.
   * \details Encodes the input DateTime data.
   * \param[in] element_class element class.
   * \param[in] value DateTime value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeDateTime(Asn1Parser::Asn1Class element_class, structure::DateTime const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes Duration data.
   * \details Encodes the input Duration data.
   * \param[in] element_class element class.
   * \param[in] value Duration value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeDuration(Asn1Parser::Asn1Class element_class, structure::Duration const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes OIDInternationalized data.
   * \details Encodes the input OIDInternationalized data.
   * \param[in] element_class element class.
   * \param[in] value OIDInternationalized value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeOIDInternationalized(Asn1Parser::Asn1Class element_class,
                                         structure::OIDInternationalized const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes RelativeOIDInternationalized data.
   * \details Encodes the input RelativeOIDInternationalized data.
   * \param[in] element_class element class.
   * \param[in] value RelativeOIDInternationalized value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeRelativeOIDInternationalized(Asn1Parser::Asn1Class element_class,
                                                 structure::RelativeOIDInternationalized const& value) noexcept
      -> ara::core::Vector<std::uint8_t>;

  /*!
   * \brief Encodes Oid data with a universal tag.
   * \details Encodes the input Oid data.
   * \param[in] value Oid value to be encoded.
   * \return encoded data.
   * \error Asn1Errc::kInvalidContent If the OID is empty
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeOid(structure::Oid const& value) -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Encodes Oid data.
   * \details Encodes the input Oid data.
   * \param[in] element_class element class.
   * \param[in] tag type tag to apply.
   * \param[in] value Oid value to be encoded.
   * \return encoded data.
   * \error Asn1Errc::kInvalidContent If the OID is empty
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeOid(Asn1Parser::Asn1Class element_class, std::uint64_t tag, structure::Oid const& value)
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
  /*!
   * \brief Encodes RelativeOid data with a universal tag.
   * \details Encodes the input RelativeOid data.
   * \param[in] value RelativeOid value to be encoded.
   * \return encoded data.
   * \error Asn1Errc::kInvalidContent If the OID is empty
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeRelativeOid(structure::RelativeOid const& value)
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
  /*!
   * \brief Encodes Real data.
   * \details Encodes the input Real data.
   * \param[in] element_class element class.
   * \param[in] tag element tag.
   * \param[in] value Real value to be encoded.
   * \param[in] base base valid values 2, 10.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \error Asn1Errc::kUnsupportedBase  If base given is not 2 or 10.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeReal(Asn1Parser::Asn1Class element_class, std::uint64_t tag, structure::Real const& value,
                         std::uint8_t base) -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
  /*!
   * \brief Encodes an integer in a special role (like an OID arc number or a tag > 30) in a special 7-bit form.
   * \details OIDs consist of multiple numbers (e.g. 1.2.250.1.16.9). The DER encoding of these numbers follows special
   *   rules. The first two arcs are encoded in a special way. This encoding is not the subject of this function.
   *   Starting from the third number, integers are encoded in chunks of 7 bits with the most significant bit used
   *   as metadata. If its value is 1, the octet is a part of a multi-byte integer. For instance, 250 is encoded as
   *   <tt>1000 0001 0111 1010</tt> which symbolizes 1 * 128 + 122. Type tags greater than 30 use the same logic.
   *   This enables long integers to be encoded without a length field or terminator octet.
   * \param[in] input The integer to encode.
   * \return The DER-encoded, big-endian value of \c input if it were part of an OBJECT IDENTIFIER.
   * \see https://www.oss.com/asn1/resources/books-whitepapers-pubs/asn1-books.html, sections 18.2.8 and figure 18.3
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeIntWithImplicitLength(std::uint64_t input) noexcept -> ara::core::Vector<std::uint8_t>;

 private:
  /*!
   * \brief Encodes Oid data.
   * \details Encodes the input Oid data.
   * \param[in] element_class element class.
   * \param[in] tag element tag.
   * \param[in] relative true if the oid is relative.
   * \param[in] value Oid value to be encoded.
   * \return encoded data.
   * \error Asn1Errc::kInvalidContent If the OID is empty
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeOid(Asn1Parser::Asn1Class element_class, std::uint64_t tag, bool relative,
                        amsr::core::String const& value) -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
  /*!
   * \brief Encodes Real data in Decimal NR3 format.
   * \details Encodes the input Real data in Decimal NR3 format.
   * \param[in] value Real value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeDecimalNR3(structure::Real const& value) -> ara::core::Vector<std::uint8_t>;
  /*!
   * \brief Encodes Real data in Base 2 binary format.
   * \details Encodes the input Real data in Base 2 binary format.
   * \param[in] value Real value to be encoded.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeBase2Binary(structure::Real const& value) noexcept -> ara::core::Vector<std::uint8_t>;
};
}  // namespace encoder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ENCODER_ENCODE_UTIL_H_

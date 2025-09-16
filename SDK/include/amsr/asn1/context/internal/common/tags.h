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
/*!        \file  amsr/asn1/context/internal/common/tags.h
 *        \brief  ASN.1 tags for use in contexts
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TAGS_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TAGS_H_

// TODO: If this tag format is used, obsolete Asn1Parser's tags. Tags are a concern of ASN.1 context, not the parser.
#include <cstdint>

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief The four tag classes of ASN.1 in their DER two-bit representation.
 */
enum class Class : std::uint8_t {
  kUniversal = 0b00000000,
  kApplication = 0b01000000,
  kContextSpecific = 0b10000000,
  kPrivate = 0b11000000
};

/*!
 * \brief The unified format for the part of tags reserved for type identification, called "tags" herein.
 * \details Type tags have no length restriction in ASN.1, but tag numbers larger than 26 have not been encountered
 *      in RFC 5912. This RFC lists all PKIX-related ASN.1 modules, so technically, even the remaining 5 bits of
 *      a one-octet tag will usually suffice.
 */
using Tag = std::int64_t;

/*!
 * \brief The list of universal and internally used tag values.
 * \details ASN.1 defines universal tags from 0 onwards, however there might be untagged contexts. VaAsn1 introduces
 *      internal tags with negative sign for that reason.
 */
enum class UniversalTag : Tag {
  kUndefined = -1,              /*!< Internally used for tagless CHOICEs, not an ASN.1 standard. */
  kReservedER,                  /*!< Reserved for use by the encoding rules */
  kBoolean,                     /*!< Boolean type. */
  kInteger,                     /*!< Integer type. */
  kBitString,                   /*!< Bit string type. */
  kOctetString,                 /*!< Octet string type. */
  kNull,                        /*!< Null type. */
  kObjectIdentifier,            /*!< Object type. */
  kObjectDescriptor,            /*!< Object descriptor type. */
  kExternal,                    /*!< External type and Instance-of type. */
  kReal,                        /*!< Real type. */
  kEnumerated,                  /*!< Enumerated type. */
  kEmbeddedPDV,                 /*!< Embedded-pdv type. */
  kUTF8String,                  /*!< UTF8 string type. */
  kRelativeOID,                 /*!< Relative object identifier type. */
  kTime,                        /*!< The Time Type. */
  kReservedFuture,              /*!< Reserved for future editions. */
  kSequence,                    /*!< Sequence and Sequence-of type. */
  kSet,                         /*!< Set and Set-of types. */
  kNumericString,               /*!< Numeric string type. */
  kPrintableString,             /*!< Printable string type. */
  kT61String,                   /*!< T61 String type. */
  kVideotexString,              /*!< Videotex String type. */
  kIA5String,                   /*!< IA5String type. */
  kUTCTime,                     /*!< UTC time type. */
  kGeneralizedTime,             /*!< Generalized time type. */
  kGraphicString,               /*!< Graphic String type. */
  kVisibleString,               /*!< Visible String type/ISO64 String type. */
  kGeneralString,               /*!< General String type. */
  kUniversalString,             /*!< Universal String type. */
  kCharacterString,             /*!< Character String type. */
  kBMPString,                   /*!< BMP String type. */
  kDate,                        /*!< Date type. */
  kTimeOfDay,                   /*!< TIME-OF-DAY type. */
  kDateTime,                    /*!< DATE-TIME type. */
  kDuration,                    /*!< Duration type. */
  kOIDInternationalized,        /*!< OID internationalized resource identifier type. */
  kRelativeOIDInternationalized /*!< Relative OID internationalized resource identifier type. */
};
}  // namespace context
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_TAGS_H_
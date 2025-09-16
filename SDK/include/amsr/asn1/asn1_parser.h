/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   amsr/asn1/asn1_parser.h
 *        \brief  ASN.1 parser interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_builder.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace asn1 {

/*!
 * \brief ASN.1 parser interface.
 */
class Asn1Parser {
 public:
  /*!
   * \brief Type definition for pointer to Asn1Parser.
   */
  using Uptr = std::unique_ptr<Asn1Parser>;

  /*!
   * \brief Constructor instance of class.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate Product Private
   */
  Asn1Parser() = default;

  /*!
   * \brief Explicitly deleted constructor.
   * \vprivate Product Private
   */
  Asn1Parser(Asn1Parser const& /*other*/) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Asn1Parser(Asn1Parser&& /*other*/) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Asn1Parser& operator=(Asn1Parser& /*other*/) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Asn1Parser& operator=(Asn1Parser&& /*other*/) = delete;

  /*!
   * \brief Destructs instance of class.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate Product Private
   */
  virtual ~Asn1Parser() = default;

  /*!
   * \brief Enumeration of all encodings.
   * \vprivate Product Private
   */
  enum class EncodingType : std::uint8_t {
    kBER = 0, /*!< Basic Encoding Rules. */
    kPER = 1, /*!< Packed Encoding Rules. */
    kCER = 2, /*!< Canonical Encoding Rules. */
    kDER = 3  /*!< Distinguished Encoding Rules. */
  };

  /*!
   * \brief Enumeration of all element types.
   * \vprivate Product Private
   */
  enum class Asn1ElementType : std::uint8_t {
    kPrimitive = 0,  /*!< Primitive Type. */
    kConstructed = 1 /*!< Constructed Type. */
  };

  /*!
   * \brief Enumeration of all classes.
   * \vprivate Product Private
   */
  enum class Asn1Class : std::uint8_t {
    kUniversal = 0,       /*!< Universal element class. */
    kApplication = 1,     /*!< Application element class. */
    kContextSpecific = 2, /*!< ContextSpecific element class. */
    kPrivate = 3          /*!< Private element class. */
  };

  /*!
   * \brief Enumeration of all tags.
   * \vprivate Product Private
   */
  enum class UniversalElementTag : std::uint8_t {
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

  /*!
   * \brief Structure with objects' information.
   * \vprivate Product Private
   */
  struct ObjectInfo {
    Asn1Class element_class{};      /*!< Represents class type. */
    bool is_constructed{};          /*!< Represents constructed type. */
    std::uint64_t element_tag{};    /*!< Element tag number. */
    std::uint64_t element_length{}; /*!< Length of the content. */
    std::size_t header_size{};      /*!< Number of parsed bytes in current element */
  };

  /*!
   * \brief Creates instance of ASN.1 Parser.
   * \details Creates a instance of ASN.1 parser, which will delegate based on encoding type.
   * \return Unique pointer to ASN.1 parser implementation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static Uptr CreateAsn1Parser() noexcept;

  /*!
   * \brief Parses ASN.1 structure.
   * \details For each parsed ASN.1 element, the method calls the corresponding callback.
   *          If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] asn1_structure ASN.1 structure to be parsed.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \param[in] encoding The encoding of the ASN.1 structure.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error kRuntimeFault  If parsing fails or calling one of the callback fails.
   * \vprivate Product Private
   */
  virtual amsr::core::Result<void> Parse(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                         Asn1Builder& asn1_builder, EncodingType encoding) const noexcept = 0;
};

}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_H_

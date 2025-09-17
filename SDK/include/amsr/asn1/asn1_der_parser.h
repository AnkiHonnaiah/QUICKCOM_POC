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
/*!       \file   asn1_der_parser.h
 *        \brief  DER encoded, ASN.1 parser interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_DER_PARSER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_DER_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_base_parser.h"
#include "amsr/core/string.h"
#include "amsr/iostream/stateful_output_stream.h"

namespace amsr {
namespace asn1 {

/*!
 * \brief ASN.1 parser interface.
 * \trace DSGN-VaAsn1-Parser
 * \vprivate Product Private
 */
// VECTOR NC Metric-OO.WMC.One: MD_VAASN1_Metric-OO.WMC.GetObject_will_be_refactored
class Asn1DERParser : public Asn1BaseParser {
 public:
  /*!
   * \brief Constructor of class.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate Product Private
   */
  Asn1DERParser() = default;

  /*!
   * \brief Iterator for structure
   */
  using CIterator = amsr::core::Span<std::uint8_t const>::const_iterator;

  /*!
   * \brief Parses ASN.1 structure.
   * \details For each parsed ASN.1 element, the method calls the corresponding callback.
   *          If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] asn1_structure ASN.1 structure encoded in DER to be parsed.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput If end of structure reached before completing an element.
   * \error Asn1Errc::kUnknownTag If tag number is unknown for \c Universal class.
   * \error Asn1Errc::kUnsupportedTag If tag number is not supported by this implementation.
   * \error Asn1Errc::kInvalidContent If content is not encoding properly as per \c DER.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Result<void> Parse(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                 Asn1Builder& asn1_builder) const noexcept override;

  /*!
   * \brief For a byte string starting with a DER-encoded ASN.1 object, returns that object.
   * \details The passed string may contain surplus data after the first ASN.1 object. That data is not returned.
   *    Moreover, in an array of multiple ASN.1 objects, only the first one is returned.
   * \param[in] asn1_structure A string starting with an ASN.1 structure encoded in DER.
   * \return A subspan from 0 to (length of first parsed ASN.1 object) of \c asn1_structure
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput If an object header exists but its length exceeds that of \c asn1_structure
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto FirstObject(amsr::core::Span<std::uint8_t const> const& asn1_structure) noexcept
      -> amsr::core::Result<amsr::core::Span<std::uint8_t const>>;

  /*!
   * \brief For a byte string starting with a DER-encoded ASN.1 object, returns that object's value without header.
   * \details An ASN.1 object can contain multiple nested headers. This only strips the very first header, effectively
   *    turning an OCTET STRING containing a SEQUENCE into said SEQUENCE.
   * \param[in] asn1_structure A string starting with an ASN.1 structure encoded in DER.
   * \return A subspan of \c asn1_structure including the \c V part of the TLV tuple.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput If an object header exists but its length exceeds that of \c asn1_structure
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto WithoutHeader(amsr::core::Span<std::uint8_t const> const& asn1_structure) noexcept
      -> amsr::core::Result<amsr::core::Span<std::uint8_t const>>;

  /*!
   * \brief Converts a DER-encoded byte vector representing the value (V) of an OID to dot notation, e.g. \c 2.999
   * \param[in] oid_value_der Value, without type (T) or length (L), of the OID to convert
   * \param[in] is_relative If true, does not treat the first octet as top-level arcs (use with relative OIDs)
   * \return A result containing the string representation
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kInvalidContent If content is not encoding properly as per DER
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-PrimitiveTypes
   * \vprivate Product Private
   */
  static auto OidToDotNotationString(amsr::core::Span<std::uint8_t const> const& oid_value_der,
                                     bool is_relative = false) noexcept -> amsr::core::Result<amsr::core::String>;

  /*!
   * \brief Parses class, primitive/constructed, tag and Length of current element.
   * \details Parses class, primitive/constructed, tag and Length of current element.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \return Returns result containing element information
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput If end of structure reached before completing an element.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetObject(amsr::core::Span<std::uint8_t const> const& asn1_structure) noexcept
      -> amsr::core::Result<Asn1Parser::ObjectInfo>;

 private:
  /*!
   * \brief Parsing states for GetObject
   */
  enum class States : std::uint8_t {
    kInit,   /*!< Init */
    kTag,    /*!< Parse tag number (greater than 30) */
    kLength, /*!< Parse length */
    kError,  /*!< Error occurred */
    kDone    /*!< Done */
  };

  /*!
   * \brief Status of GetObject state machine
   */
  struct GetObjectState {
    States state{States::kInit};   /*!< State */
    Asn1Parser::ObjectInfo info{}; /*!< Generating object */
    std::size_t parsed{0};         /*!< Count of parsed octets */
    bool has_error{true};          /*!< Error flag */
  };

  /*!
   * \brief Handles the \c kInit state in \c GetObject
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] go_state The input status of \c GetObject
   * \return An internally used helper struct representing the state for \c GetObject
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetObjectHandleInit(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                  GetObjectState const& go_state) noexcept -> GetObjectState;

  /*!
   * \brief Handles the \c kTag state in \c GetObject
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] go_state The input status of \c GetObject
   * \return An internally used helper struct representing the state for \c GetObject
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetObjectHandleTag(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                 GetObjectState const& go_state) noexcept -> GetObjectState;

  /*!
   * \brief Handles the \c kLength state in \c GetObject
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] go_state The input status of \c GetObject
   * \return An internally used helper struct representing the state for \c GetObject
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GetObjectHandleLength(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                    GetObjectState const& go_state) noexcept -> GetObjectState;

  /*!
   * \brief Parses ASN.1 structure with DER encoding.
   * \details For each parsed ASN.1 class, the method calls the corresponding method.
   *          If any call to one of the method fails, the parsing stops and fails.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput If end of structure reached before completing an element.
   * \error Asn1Errc::kUnknownTag If tag number is unknown for \c Universal class.
   * \error Asn1Errc::kUnsupportedTag If tag number is not supported by this implementation.
   * \error Asn1Errc::kInvalidContent If content is not encoding properly as per \c DER.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto ParseDER(amsr::core::Span<std::uint8_t const> const& asn1_structure, Asn1Builder& asn1_builder) const noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Adds two non-negative numbers, as long as overflow does not occur.
   * \param[in] lhs Left-hand-side operand of the addition
   * \param[in] rhs Right-hand-side operand of the addition
   * \return The sum of \c lhs and \c rhs
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kUnsupportedNumeric If an overflow occurs.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto AddOverflowSafe(std::size_t lhs, std::size_t rhs) noexcept -> amsr::core::Result<std::size_t>;

  /*!
   * \brief Parses elements of universal class.
   * \details For each primitive types of universal class calls respective builder APIs.
   *          For each constructed this method calls internal parser functions.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] info Element information such as class, primitive, tag and length.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kUnknownTag If tag number is unknown for \c Universal class.
   * \error Asn1Errc::kUnsupportedTag If tag number is not supported by this implementation.
   * \error Asn1Errc::kInvalidContent If content is not encoding properly as per \c DER.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-ASN1Classes
   * \vprivate Product Private
   */
  amsr::core::Result<void> ParseUniversalType(amsr::core::Span<std::uint8_t const> const asn1_structure,
                                              Asn1Parser::ObjectInfo const& info,
                                              Asn1Builder& asn1_builder) const noexcept;

  /*!
   * \brief Parses primitive types of universal class.
   * \details For each primitive types of universal class calls respective builder APIs.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] info Element information such as class, primitive, tag and length.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kUnknownTag If tag number is unknown for \c Universal class.
   * \error Asn1Errc::kUnsupportedTag If tag number is not supported by this implementation.
   * \error Asn1Errc::kInvalidContent If content is not encoding properly as per \c DER.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-PrimitiveTypes
   * \vprivate Product Private
   */
  static amsr::core::Result<void> ParsePrimitiveType(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                                     Asn1Parser::ObjectInfo const& info,
                                                     Asn1Builder& asn1_builder) noexcept;

  /*!
   * \brief Parses Object identifier primitive type.
   * \details Decodes object identifier from given \c asn1_structure and calls involves builder API
   *          with object identifier.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \param[in] is_relative Parse as relative OID.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kInvalidContent If content is not encoding properly as per \c DER encoding rule.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-PrimitiveTypes
   * \vprivate Product Private
   */
  static amsr::core::Result<void> ParseOid(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                           Asn1Builder& asn1_builder, bool const is_relative = false) noexcept;

  /*!
   * \brief Parses real type.
   * \details Decodes real number from given \c asn1_structure and calls builder API with real value.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kInvalidContent If content is not encoding properly as per \c DER encoding rule.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-PrimitiveTypes
   * \vprivate Product Private
   */
  static amsr::core::Result<void> ParseReal(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                            Asn1Builder& asn1_builder) noexcept;

  /*!
   * \brief Parses elements of constructed type of universal class.
   * \details For each type of constructed type calls respective builder APIs and parses child elements of those types.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] info Element information such as class, primitive, tag and length.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kUnknownTag If tag number is unknown for Universal class.
   * \error Asn1Errc::kUnsupportedTag If tag number is not supported by this implementation.
   * \error Asn1Errc::kInvalidContent If content is not encoding properly as per DER.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-ConstructedTypes
   * \vprivate Product Private
   */
  amsr::core::Result<void> ParseConstructedType(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                                Asn1Parser::ObjectInfo const& info,
                                                Asn1Builder& asn1_builder) const noexcept;

  /*!
   * \brief Parses elements of application class.
   * \details Checks type is constructed or primitive, calling corresponding builder APIs and parsing child elements.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] info Element information such as class, primitive, tag and length.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput If end of structure reached before completing an element.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-ASN1Classes
   * \vprivate Product Private
   */
  amsr::core::Result<void> ParseApplicationType(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                                Asn1Parser::ObjectInfo const& info,
                                                Asn1Builder& asn1_builder) const noexcept;

  /*!
   * \brief Parses elements of Context Specific class.
   * \details Call respective APIs of builder based on primitive or constructed type.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] info Element information such as class, primitive, tag and length.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput If end of structure reached before completing an element.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-ASN1Classes
   * \vprivate Product Private
   */
  amsr::core::Result<void> ParseContextSpecificType(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                                    Asn1Parser::ObjectInfo const& info,
                                                    Asn1Builder& asn1_builder) const noexcept;

  /*!
   * \brief Parses elements of Private class.
   * \details Checks type is constructed or primitive, calling corresponding builder APIs and parsing child elements.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \param[in] info Element information such as class, primitive, tag and length.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \return void result
   * \context The encoding of the ASN.1 structure. This should be \c DER
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput} If end of structure reached before completing an element.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace DSGN-VaAsn1-ASN1Classes
   * \vprivate Product Private
   */
  amsr::core::Result<void> ParsePrivateType(amsr::core::Span<std::uint8_t const> const& asn1_structure,
                                            Asn1Parser::ObjectInfo const& info,
                                            Asn1Builder& asn1_builder) const noexcept;
  /*!
   * \brief Write the first septet-encoded number of an OID.
   * \param[in] parsed_value The output previously generated by \c GetNumber
   * \param[out] sos The stream to write to
   * \param[in] is_relative Whether the OID is relative
   * \return void (writes to the passed stream)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto HandleOidFirstNumber(std::uint64_t parsed_value, amsr::stream::StatefulOutputStream& sos,
                                   bool is_relative) noexcept -> void;

  /*!
   * \brief Parses number in asn.1 structure.
   * \details Parses number in sequence of octets, with bit 8 representing end of sequence.
   * \param[in] asn1_structure Asn.1 data to be parsed.
   * \return A pair <tt>(n, p)</tt> with \c n being the parsed septet-based number and \c p being the count
   *        of parsed octets.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error Asn1Errc::kIncompleteInput If end of structure reached before completing an element.
   * \error Asn1Errc::kUnsupportedNumeric If number is bigger than 64 bit.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static amsr::core::Result<std::pair<std::uint64_t, std::size_t>> GetNumber(
      amsr::core::Span<std::uint8_t const> const& asn1_structure) noexcept;
};

}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_DER_PARSER_H_

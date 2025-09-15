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
/*!        \file  amsr/asn1/structure/general_name.h
 *        \brief  Representation of parsed General Name.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_NAME_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_NAME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/encoder/encode_util.h"
#include "amsr/asn1/structure/dns_name.h"
#include "amsr/asn1/structure/edi_party_name.h"
#include "amsr/asn1/structure/name.h"
#include "amsr/asn1/structure/oraddress.h"
#include "amsr/asn1/structure/other_name.h"
#include "amsr/asn1/structure/uniform_resource_identifier.h"
#include "amsr/core/variant.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief Type alias of GeneralName structure
 */

using GeneralName = amsr::core::Variant<structure::OtherName, structure::OctetString, structure::ORAddress,
                                        structure::Name, structure::EDIPartyName, structure::Ia5String,
                                        structure::DNSName, structure::UniformResourceIdentifier, structure::Oid>;

/*!
 * \brief Type alias of GeneralNames structure
 */

using GeneralNames = ara::core::Vector<GeneralName>;

/*!
 * \brief Utility to encode GeneralName
 */
class GeneralNameUtils {
 public:
  /*!
   * \brief Encodes GeneralName structure.
   * \param[in] general_name GeneralName structure variant data.
   * \return encoded data if the structure has value or else return error.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kIncompleteInput  Content data is not as per specification.
   * \error Asn1Errc::kConstraintCheckFail  If the value constraint checks fails.
   * \vprivate Product Private
   */
  // VECTOR NC Metric-HIS.VG: MD_VAASN1_Metric-HIS.VG_Encoding_to_be_replaced_with_implicit_utility
  static auto EncodeGeneralName(structure::GeneralName const& general_name) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    ara::core::Vector<std::uint8_t> encoded_general_name{};
    ara::core::Vector<std::uint8_t> encoded_data{};
    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{
        amsr::core::Result<ara::core::Vector<std::uint8_t>>::FromError(
            Asn1Errc::kIncompleteInput, "Tried to encode a GeneralName, but did not recognize its physical type."_sv)};

    if (general_name.get_if<structure::OtherName>() != nullptr) {
      structure::OtherName const& other_name_data{*general_name.get_if<structure::OtherName>()};
      result = other_name_data.EncodeContextSpecific(0);
      if (result.HasValue()) {
        static_cast<void>(
            encoded_general_name.insert(encoded_general_name.end(), result.Value().begin(), result.Value().end()));
      }
    } else if (general_name.get_if<structure::Ia5String>() != nullptr) {
      result = encoder::EncoderUtility::EncodeString(
          Asn1Parser::Asn1Class::kContextSpecific, false, 1, false,
          static_cast<amsr::core::String>(*general_name.get_if<structure::Ia5String>()));
      static_cast<void>(encoded_general_name.insert(encoded_general_name.end(), result->begin(), result->end()));
    } else if (general_name.get_if<structure::DNSName>() != nullptr) {
      result = encoder::EncoderUtility::EncodeString(
          Asn1Parser::Asn1Class::kContextSpecific, false, 2, false,
          static_cast<amsr::core::String>(*general_name.get_if<structure::DNSName>()));
      static_cast<void>(encoded_general_name.insert(encoded_general_name.end(), result->begin(), result->end()));
    } else if (general_name.get_if<structure::ORAddress>() != nullptr) {
      structure::ORAddress const& or_address{*general_name.get_if<ORAddress>()};
      result = or_address.Encode(true);
      if (result) {
        ara::core::Vector<std::uint8_t> const& header_or_address_data{
            ::amsr::asn1::encoder::EncoderUtility::EncodeHeader(Asn1Parser::Asn1Class::kContextSpecific, true, 3,
                                                                result.Value().size())};
        static_cast<void>(encoded_general_name.insert(encoded_general_name.end(), header_or_address_data.begin(),
                                                      header_or_address_data.end()));
        static_cast<void>(
            encoded_general_name.insert(encoded_general_name.end(), result.Value().begin(), result.Value().end()));
      }
    } else if (structure::Name const* const name{general_name.get_if<structure::Name>()}) {
      if (structure::RDNSequence const* const rdn_sequence{name->get_if<structure::RDNSequence>()}) {
        if (!rdn_sequence->empty()) {
          result = NameEncodingUtils::EncodeName(*rdn_sequence);
        } else {
          result.EmplaceError(Asn1Errc::kIncompleteInput,
                              "A GeneralName is of type 'Name', but contains an empty RDNSequence.");
        }
      }
      if (result) {
        ara::core::Vector<std::uint8_t> const& encoded_header_data{::amsr::asn1::encoder::EncoderUtility::EncodeHeader(
            Asn1Parser::Asn1Class::kContextSpecific, true, 4, result.Value().size())};
        static_cast<void>(
            encoded_general_name.insert(encoded_data.end(), encoded_header_data.begin(), encoded_header_data.end()));
        static_cast<void>(
            encoded_general_name.insert(encoded_general_name.end(), result.Value().begin(), result.Value().end()));
      }
    } else if (general_name.get_if<structure::OctetString>() != nullptr) {
      result = encoder::EncoderUtility::EncodeGeneralType(Asn1Parser::Asn1Class::kContextSpecific, false, 7, false,
                                                          *general_name.get_if<structure::OctetString>());
      static_cast<void>(encoded_general_name.insert(encoded_general_name.end(), result->begin(), result->end()));
    } else if (general_name.get_if<structure::EDIPartyName>() != nullptr) {
      result = (*general_name.get_if<structure::EDIPartyName>()).Encode(true);
      if (result) {
        ara::core::Vector<std::uint8_t> const& encoded_edi_party_header_data{
            ::amsr::asn1::encoder::EncoderUtility::EncodeHeader(Asn1Parser::Asn1Class::kContextSpecific, true, 5,
                                                                result.Value().size())};
        static_cast<void>(encoded_general_name.insert(encoded_general_name.end(), encoded_edi_party_header_data.begin(),
                                                      encoded_edi_party_header_data.end()));
        static_cast<void>(
            encoded_general_name.insert(encoded_general_name.end(), result.Value().begin(), result.Value().end()));
      }
    } else if (general_name.get_if<structure::Oid>() != nullptr) {
      result = encoder::EncoderUtility::EncodeOid(Asn1Parser::Asn1Class::kContextSpecific, 8,
                                                  *general_name.get_if<structure::Oid>());
      if (result) {
        static_cast<void>(encoded_general_name.insert(encoded_general_name.end(), result->begin(), result->end()));
      }
    } else if (general_name.get_if<structure::UniformResourceIdentifier>() != nullptr) {
      result = encoder::EncoderUtility::EncodeString(
          Asn1Parser::Asn1Class::kContextSpecific, false, 6, false,
          static_cast<amsr::core::String>(*general_name.get_if<structure::UniformResourceIdentifier>()));
      if (result) {
        static_cast<void>(encoded_general_name.insert(encoded_general_name.end(), result->begin(), result->end()));
      }
    } else {
      // result initialized with kIncompleteInput error
    }

    if (result) {
      result.EmplaceValue(encoded_general_name);
    }

    return result;
  }
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_GENERAL_NAME_H_

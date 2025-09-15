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
/*!        \file  amsr/asn1/structure/name.h
 *        \brief  Representation of parsed Name.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NAME_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NAME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <set>

#include "amsr/core/variant.h"
#include "ara/core/vector.h"

#include "amsr/asn1/encoder/encode_util.h"
#include "attribute.h"

namespace amsr {
namespace asn1 {
namespace structure {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Type alias
 */
using RelativeDistinguishedName = ara::core::Vector<SingleAttribute>;
/*!
 * \brief Type alias
 */
using RDNSequence = ara::core::Vector<RelativeDistinguishedName>;
/*!
 * \brief Type alias
 */
using Name = amsr::core::Variant<RDNSequence>;

/*!
 * \brief Utility to encode Name
 */
class NameEncodingUtils {
 public:
  /*!
   * \brief Encodes Name structure.
   * \param[in] rdn RDNSequence structure data.
   * \return encoded data if the structure has value or else return error.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error Asn1Errc::kConstraintCheckFail If the parameter constraint is not satisfied.
   * \vprivate Product Private
   */
  static auto EncodeName(RDNSequence const& rdn) noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    ara::core::Vector<std::uint8_t> encoded_data{};
    amsr::core::Result<ara::core::Vector<std::uint8_t>> result{
        amsr::core::Result<ara::core::Vector<std::uint8_t>>::FromError(
            Asn1Errc::kRuntimeFault, "Unexpected failure during Encoding of a Name value."_sv)};

    ara::core::Vector<std::uint8_t> encoded_rdn_seq{};

    if (rdn.empty()) {
      // Allow empty RDNSequence
      result.EmplaceValue();
    } else {
      for (structure::RelativeDistinguishedName const& rel_dis_name : rdn) {
        result.EmplaceError(Asn1Errc::kConstraintCheckFail,
                            "Tried to encode a Name containing an empty RelativeDistinguishedName.");
        ara::core::Vector<std::uint8_t> encoded_rdns{};
        for (SingleAttribute const& attribute_type_value : rel_dis_name) {
          result = attribute_type_value.Encode();
          if (result.HasValue()) {
            static_cast<void>(encoded_rdns.insert(encoded_rdns.end(), result.Value().begin(), result.Value().end()));
          } else {
            break;
          }
        }
        if (result.HasValue()) {
          ara::core::Vector<std::uint8_t> const& encode_set_header{encoder::EncoderUtility::EncodeHeader(
              Asn1Parser::Asn1Class::kUniversal, true, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kSet),
              encoded_rdns.size())};

          static_cast<void>(
              encoded_rdn_seq.insert(encoded_rdn_seq.end(), encode_set_header.begin(), encode_set_header.end()));
          static_cast<void>(encoded_rdn_seq.insert(encoded_rdn_seq.end(), encoded_rdns.begin(), encoded_rdns.end()));
        } else {
          break;
        }
      }
    }

    if (result.HasValue()) {
      ara::core::Vector<std::uint8_t> const& encode_rdn_seq_header{encoder::EncoderUtility::EncodeHeader(
          Asn1Parser::Asn1Class::kUniversal, true, static_cast<uint64_t>(Asn1Parser::UniversalElementTag::kSequence),
          encoded_rdn_seq.size())};
      ara::core::Vector<std::uint8_t> encoded_name{};
      static_cast<void>(
          encoded_name.insert(encoded_name.end(), encode_rdn_seq_header.begin(), encode_rdn_seq_header.end()));
      static_cast<void>(encoded_name.insert(encoded_name.end(), encoded_rdn_seq.begin(), encoded_rdn_seq.end()));

      static_cast<void>(encoded_data.insert(encoded_data.end(), encoded_name.begin(), encoded_name.end()));
      result.EmplaceValue(encoded_data);
    }
    return result;
  }

  /*!
   * \brief Encodes Name structure.
   * \param[in] name Name structure data.
   * \return encoded data if the structure has value or else return error.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto EncodeName(Name const& name) noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>> {
    amsr::core::Result<ara::core::Vector<std::uint8_t>> retval{
        amsr::core::Result<ara::core::Vector<std::uint8_t>>::FromError(
            Asn1Errc::kRuntimeFault,
            "Internal error while trying to encode a Name: Contained data is of unknown physical type."_sv)};
    if (structure::RDNSequence const* const rdn_seq{name.get_if<RDNSequence>()}) {
      retval = EncodeName(*rdn_seq);
    }

    return retval;
  }
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_NAME_H_

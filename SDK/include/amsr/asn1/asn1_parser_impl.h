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
/*!       \file   amsr/asn1/asn1_parser_impl.h
 *        \brief  ASN.1 parser interface implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_IMPL_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_base_parser.h"
#include "amsr/asn1/asn1_parser.h"

namespace amsr {
namespace asn1 {

/*!
 * \brief Internal implementation of ASN.1 parser.
 */
class Asn1ParserImpl : public Asn1Parser {
 public:
  /*!
   * \brief Parses ASN.1 structure.
   * \param[in] asn1_structure ASN.1 structure encoded in DER to be parsed.
   * \param[in] asn1_builder ASN.1 builder callback interface.
   * \param[in] encoding The encoding of the ASN.1 structure.
   * \return void result.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \copydoc Asn1Parser::Parse
   * \vprivate Product Private
   */
  amsr::core::Result<void> Parse(amsr::core::Span<std::uint8_t const> const& asn1_structure, Asn1Builder& asn1_builder,
                                 EncodingType encoding) const noexcept override;
};

}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_PARSER_IMPL_H_

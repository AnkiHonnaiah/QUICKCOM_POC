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
/*!       \file   amsr/asn1/asn1_base_parser.h
 *        \brief  ASN.1 decoder interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_BASE_PARSER_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_BASE_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_parser.h"

namespace amsr {
namespace asn1 {

/*!
 * \brief Internal decoder for ASN.1 structure.
 */
class Asn1BaseParser {
 public:
  /*!
   * \brief Type definition for pointer to Asn1BaseParser.
   */
  using Uptr = std::unique_ptr<Asn1BaseParser>;

  /*!
   * \brief Constructor for ASN.1 decoder interface.
   * \vprivate Product Private
   */
  Asn1BaseParser() = default;

  /*!
   * \brief Explicitly deleted constructor.
   * \vprivate Product Private
   */
  Asn1BaseParser(Asn1BaseParser const& /*other*/) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \vprivate Product Private
   */
  Asn1BaseParser(Asn1BaseParser&& /*other*/) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Asn1BaseParser& operator=(Asn1BaseParser& /*other*/) = delete;

  /*!
   * \brief Explicitly deleted constructor.
   * \return -
   * \vprivate Product Private
   */
  Asn1BaseParser& operator=(Asn1BaseParser&& /*other*/) = delete;

  /*!
   * \brief Destructor.
   * \vprivate Product Private
   */
  virtual ~Asn1BaseParser() = default;

  /*!
   * \brief Parses ASN.1 structure.
   * \details For each parsed ASN.1 element, the method calls the corresponding callback.
   *          If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] asn1_structure ASN.1 structure to be parsed.
   * \param[in] asn1_builder ASN.1 builder callback interface.
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
                                         Asn1Builder& asn1_builder) const noexcept = 0;
};

}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_ASN1_BASE_PARSER_H_

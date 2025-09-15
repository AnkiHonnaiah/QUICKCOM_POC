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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!      \file   crypto/x509customextensionparser/asn1_parser.h
 *       \brief  ANS.1 Parser Interface implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_ASN1_PARSER_H_
#define LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_ASN1_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/crypto/common/asn1_callback_interface.h"
#include "amsr/crypto/common/asn1_parser.h"
#include "crypto/common/util/logger.h"

namespace crypto {
namespace x509customextensionparser {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::operator""_sv;

/*!
 * \brief ASN.1 Parser
 * \vpublic
 */
class Asn1Parser final : public amsr::crypto::common::Asn1Parser {
 public:
  /*!
   * \brief Constructs instance of class.
   * \vprivate Component Private
   */
  Asn1Parser() = default;

  /*!
   * \brief Destructs instance of class.
   * \vprivate Component Private
   */
  ~Asn1Parser() noexcept final = default;

  /*!
   * \brief Asn1Parser is not copy constructable.
   * \vprivate Component Private
   */
  Asn1Parser(Asn1Parser const& /*other*/) = delete;

  /*!
   * \brief Asn1Parser is not move constructable.
   * \vprivate Component Private
   */
  Asn1Parser(Asn1Parser&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Asn1Parser is not copy assignable.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(Asn1Parser const& /*other*/) & noexcept -> Asn1Parser& = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Asn1Parser is not move assignable.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(Asn1Parser&& /*other*/) & noexcept -> Asn1Parser& = delete;

  /*!
   * \brief Parse DER encoded ASN.1 structure
   * \details For each parsed ASN.1 element, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] der_encoded_asn1_structure DER encoded ASN.1 structure to be parsed.
   * \param[in] asn1_callback_interface Callback interface.
   * \pre asn1_callback_interface != nullptr
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kRuntimeFault If parsing fails or calling one of the callback fails.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Asn1Parse(ara::crypto::ReadOnlyMemRegion der_encoded_asn1_structure,
                 std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> amsr::core::Result<void> final;

  /*!
   * \brief Parse DER encoded ASN.1 structure
   * \details For each parsed ASN.1 element, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] der_encoded_asn1_structure DER encoded ASN.1 structure to be parsed.
   * \param[in] asn1_callback_interface Callback interface.
   * \context ANY
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kRuntimeFault If parsing fails or calling one of the callback fails.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Asn1Parse(ara::crypto::ReadOnlyMemRegion der_encoded_asn1_structure,
                 amsr::crypto::common::Asn1CallbackInterface& asn1_callback_interface) const noexcept
      -> amsr::core::Result<void> final;
};

}  // namespace x509customextensionparser
}  // namespace crypto

#endif  // LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_ASN1_PARSER_H_

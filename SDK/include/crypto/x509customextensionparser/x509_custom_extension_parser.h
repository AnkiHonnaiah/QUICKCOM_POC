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
/*!      \file   crypto/x509customextensionparser/x509_custom_extension_parser.h
 *       \brief  X.509 Custom Extension Parser Interface implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_X509_CUSTOM_EXTENSION_PARSER_H_
#define LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_X509_CUSTOM_EXTENSION_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/attribute.h"
#include "amsr/asn1/structure/extension.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/crypto/common/asn1_callback_interface.h"
#include "amsr/crypto/x509/x509_custom_extension_parser.h"
#include "ara/crypto/x509/attribute_certificate.h"
#include "ara/crypto/x509/certificate.h"

namespace crypto {
namespace x509customextensionparser {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief X.509 Custom Extension Parser
 * \vpublic
 */
class X509CustomExtensionParser final : public amsr::crypto::x509::X509CustomExtensionParser {
 public:
  /*!
   * \brief Constructs instance of class.
   * \vprivate Component Private
   */
  X509CustomExtensionParser() noexcept = default;

  /*!
   * \brief Destructs instance of class.
   * \vprivate Component Private
   */
  ~X509CustomExtensionParser() noexcept final = default;

  /*!
   * \brief X509CustomExtensionParser is not copy constructable.
   * \vprivate Component Private
   */
  X509CustomExtensionParser(X509CustomExtensionParser const& /*other*/) = delete;

  /*!
   * \brief X509CustomExtensionParser is not move constructable.
   * \vprivate Component Private
   */
  X509CustomExtensionParser(X509CustomExtensionParser&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief X509CustomExtensionParser is not copy assignable.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(X509CustomExtensionParser const& /*other*/) & noexcept -> X509CustomExtensionParser& = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief X509CustomExtensionParser is not move assignable.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(X509CustomExtensionParser&& /*other*/) & noexcept -> X509CustomExtensionParser& = delete;

  /*!
   * \copydoc amsr::crypto::x509::X509CustomExtensionParser::ParseCustomCertExtensionByOid(amsr::crypto::common::Asn1CallbackInterface::Oid,ara::crypto::x509::Certificate const&,std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface>) const noexcept
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_CustomExtensionParsing
   * \vprivate Component Private
   */
  auto ParseCustomCertExtensionByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid, ara::crypto::x509::Certificate const& cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> amsr::core::Result<bool> final;

  /*!
   * \copydoc amsr::crypto::x509::X509CustomExtensionParser::ParseCustomCertExtensionByOid(amsr::crypto::common::Asn1CallbackInterface::Oid,ara::crypto::ReadOnlyMemRegion,std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface>) const noexcept
   * \trace CREQ-Crypto-X509Provider_X509_Certificate_CustomExtensionParsing
   * \vprivate Component Private
   */
  auto ParseCustomCertExtensionByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid, ara::crypto::ReadOnlyMemRegion der_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> amsr::core::Result<bool> final;

  /*!
   * \copydoc amsr::crypto::x509::X509CustomExtensionParser::ParseCustomAttributeCertExtensionByOid()
   * \trace CREQ-Crypto-X509Provider_X509_AttributeCertificate_CustomExtensionParsing
   * \vprivate Component Private
   */
  auto ParseCustomAttributeCertExtensionByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid,
      ara::crypto::x509::AttributeCertificate const& attribute_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> amsr::core::Result<bool> final;

  /*!
   * \copydoc amsr::crypto::x509::X509CustomExtensionParser::ParseCustomAttributeCertExtensionByOid(amsr::crypto::common::Asn1CallbackInterface::Oid,ara::crypto::ReadOnlyMemRegion,std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface>) const noexcept
   * \trace CREQ-Crypto-X509Provider_X509_AttributeCertificate_CustomExtensionParsing
   * \vprivate Component Private
   */
  auto ParseCustomAttributeCertExtensionByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid, ara::crypto::ReadOnlyMemRegion der_attribute_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> amsr::core::Result<bool> final;

  /*!
   * \copydoc amsr::crypto::x509::X509CustomExtensionParser::ParseCustomAttributeByOid(amsr::crypto::common::Asn1CallbackInterface::Oid,ara::crypto::ReadOnlyMemRegion,std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface>) const noexcept
   * \trace CREQ-Crypto-X509Provider_X509_AttributeCertificate_AttributeParsing
   * \vprivate Component Private
   */
  auto ParseCustomAttributeByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid, ara::crypto::ReadOnlyMemRegion der_attribute_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> amsr::core::Result<void> final;

  /*!
   * \copydoc amsr::crypto::x509::X509CustomExtensionParser::ParseCustomAttributeByOid()
   * \trace CREQ-Crypto-X509Provider_X509_AttributeCertificate_AttributeParsing
   * \vprivate Component Private
   */
  auto ParseCustomAttributeByOid(amsr::crypto::common::Asn1CallbackInterface::Oid oid,
                                 ara::crypto::x509::AttributeCertificate const& attribute_cert,
                                 std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface)
      const noexcept -> amsr::core::Result<void> final;

 private:
  /*!
   * \brief Find the extension with given \c oid in \c extension_info and return its value field content.
   * \details Extension value fields are always OCTET STRINGs containing DER-encoded values.
   * \details This method returns the inner value \b without the OCTET STRING header.
   * \param[in] oid OID string identifying the extension to look for
   * \param[in] extensions Extensions structure parsed from the certificate
   * \return The encoded extension value content
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kUnexpectedValue If the certificate doesn't have an extension with the provided OID.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static auto FindExtension(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid,
      amsr::core::Optional<ara::core::Vector<amsr::asn1::structure::Extension>> const& extensions) noexcept
      -> amsr::core::Result<std::pair<bool, ara::core::Vector<std::uint8_t>>>;

  /*!
   * \brief Encode given extension.
   * \param[in] extension Extension to encode
   * \return The encoded extension value content
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kUnexpectedValue If encoding failed.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static auto EncodeExtension(amsr::asn1::structure::Extension const& extension) noexcept
      -> amsr::core::Result<std::pair<bool, ara::core::Vector<std::uint8_t>>>;
  /*!
   * \brief Find the attribute with given \c oid in \c in the attributes vector
   * \param[in] oid OID string identifying the extension to look for
   * \param[in] attributes Vector of attributes
   * \return The encoded attribute
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kUnexpectedValue If the certificate doesn't have an extension with the provided OID.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  static auto FindAttribute(amsr::crypto::common::Asn1CallbackInterface::Oid oid,
                            ara::core::Vector<amsr::asn1::structure::AttributeSet> const& attributes) noexcept
      -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;
};

}  // namespace x509customextensionparser
}  // namespace crypto

#endif  // LIB_LIBX509CUSTOMEXTENSIONPARSER_INCLUDE_CRYPTO_X509CUSTOMEXTENSIONPARSER_X509_CUSTOM_EXTENSION_PARSER_H_

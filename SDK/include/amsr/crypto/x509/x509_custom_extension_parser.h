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
/*!       \file  amsr/crypto/x509/x509_custom_extension_parser.h
 *        \brief  X.509 Custom Extension Parser Interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_AMSR_CRYPTO_X509_X509_CUSTOM_EXTENSION_PARSER_H_
#define LIB_LIBARA_INCLUDE_AMSR_CRYPTO_X509_X509_CUSTOM_EXTENSION_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/crypto/common/asn1_callback_interface.h"

#include "ara/core/result.h"
#include "ara/crypto/x509/attribute_certificate.h"
#include "ara/crypto/x509/certificate.h"

namespace amsr {
namespace crypto {
namespace x509 {

/*!
 * \brief X.509 Custom Extension Parser
 * \vpublic
 */
class X509CustomExtensionParser {
 public:
  /*!
   * \brief Default constructor.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate Component Private
   */
  X509CustomExtensionParser() = default;

  /*!
   * \brief Destructs instance of class.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vpublic
   */
  virtual ~X509CustomExtensionParser() noexcept = default;

  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<X509CustomExtensionParser>;

  /*!
   * \brief Parse the extension value of a custom X.509 extension with the specified Oid
   * \details For each parsed ASN.1 element of the extnValue, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] oid Extension object identifier.
   * \param[in] cert Certificate object to be parsed.
   * \param[in] asn1_callback_interface Custom extensions parser that implements the callbacks.
   * \return Criticality of the extension.
   * \error SecurityErrorDomain::kRuntimeFault If parsing the extension fails or calling one of the callback fails.
   * \error SecurityErrorDomain::kUnexpectedValue If the certificate doesn't have an extension with the provided Oid.
   * \vpublic
   */
  virtual auto ParseCustomCertExtensionByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid, ara::crypto::x509::Certificate const& cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> ara::core::Result<bool> = 0;

  /*!
   * \brief Parse the extension value of a custom X.509 extension with the specified Oid
   * \details For each parsed ASN.1 element of the extnValue, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] oid Extension object identifier.
   * \param[in] der_cert DER encoded certificate.
   * \param[in] asn1_callback_interface Custom extensions parser that implements the callbacks.
   * \return Criticality of the extension.
   * \error SecurityErrorDomain::kRuntimeFault If parsing the extension fails or calling one of the callback fails.
   * \error SecurityErrorDomain::kUnexpectedValue If the certificate doesn't have an extension with the provided Oid.
   * \vpublic
   */
  virtual auto ParseCustomCertExtensionByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid, ara::crypto::ReadOnlyMemRegion der_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> ara::core::Result<bool> = 0;

  /*!
   * \brief Parse the extension value of a custom X.509 extension with the specified Oid
   * \details For each parsed ASN.1 element of the extnValue, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] oid Extension object identifier.
   * \param[in] attribute_cert AttributeCertificate object to be parsed.
   * \param[in] asn1_callback_interface Custom extensions parser that implements the callbacks.
   * \return Criticality of the extension.
   * \error SecurityErrorDomain::kRuntimeFault If parsing the extension fails or calling one of the callback fails.
   * \error SecurityErrorDomain::kUnexpectedValue If the certificate doesn't have an extension with the provided Oid.
   * \vpublic
   */
  virtual auto ParseCustomAttributeCertExtensionByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid,
      ara::crypto::x509::AttributeCertificate const& attribute_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> ara::core::Result<bool> = 0;

  /*!
   * \brief Parse the extension value of a custom X.509 extension with the specified Oid
   * \details For each parsed ASN.1 element of the extnValue, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] oid Extension object identifier.
   * \param[in] der_attribute_cert DER encoded AttributeCertificate object to be parsed.
   * \param[in] asn1_callback_interface Custom extensions parser that implements the callbacks.
   * \return Criticality of the extension.
   * \error SecurityErrorDomain::kRuntimeFault If parsing the extension fails or calling one of the callback fails.
   * \error SecurityErrorDomain::kUnexpectedValue If the certificate doesn't have an extension with the provided Oid.
   * \vpublic
   */
  virtual auto ParseCustomAttributeCertExtensionByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid, ara::crypto::ReadOnlyMemRegion der_attribute_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> ara::core::Result<bool> = 0;

  /*!
   * \brief Parse the attribute value of a X.509 AttributeCertificate with the specified Oid
   * \details For each parsed ASN.1 element of the Attribute Set, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] oid AttributeType (object identifier).
   * \param[in] attribute_cert AttributeCertificate object to be parsed.
   * \param[in] asn1_callback_interface Custom extensions parser that implements the callbacks.
   * \error SecurityErrorDomain::kRuntimeFault If parsing the extension fails or calling one of the callback fails.
   * \error SecurityErrorDomain::kUnexpectedValue If the certificate doesn't have an extension with the provided Oid.
   * \vpublic
   */
  virtual auto ParseCustomAttributeByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid,
      ara::crypto::x509::AttributeCertificate const& attribute_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> amsr::core::Result<void> = 0;

  /*!
   * \brief Parse the attribute value of a X.509 AttributeCertificate with the specified Oid
   * \details For each parsed ASN.1 element of the Attribute Set, the method calls
   *          the corresponding callback. If any call to one of the callbacks fails, the parsing stops and fails.
   * \param[in] oid AttributeType (object identifier).
   * \param[in] der_attribute_cert DER encoded AttributeCertificate object to be parsed.
   * \param[in] asn1_callback_interface Custom extensions parser that implements the callbacks.
   * \error SecurityErrorDomain::kRuntimeFault If parsing the extension fails or calling one of the callback fails.
   * \error SecurityErrorDomain::kUnexpectedValue If the certificate doesn't have an extension with the provided Oid.
   * \vpublic
   */
  virtual auto ParseCustomAttributeByOid(
      amsr::crypto::common::Asn1CallbackInterface::Oid oid, ara::crypto::ReadOnlyMemRegion der_attribute_cert,
      std::unique_ptr<amsr::crypto::common::Asn1CallbackInterface> asn1_callback_interface) const noexcept
      -> amsr::core::Result<void> = 0;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  X509CustomExtensionParser(X509CustomExtensionParser const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  X509CustomExtensionParser(X509CustomExtensionParser&& /*other*/) noexcept = default;

  /*!
   * \brief Default move assignment constructor
   * \vprivate Component Private
   */
  auto operator=(X509CustomExtensionParser&& /*other*/) & noexcept -> X509CustomExtensionParser& = default;

  /*!
   * \brief Default copy assignment constructor
   * \vprivate Component Private
   */
  auto operator=(X509CustomExtensionParser const& /*other*/) & -> X509CustomExtensionParser& = default;
};

}  // namespace x509
}  // namespace crypto
}  // namespace amsr

#endif  // LIB_LIBARA_INCLUDE_AMSR_CRYPTO_X509_X509_CUSTOM_EXTENSION_PARSER_H_

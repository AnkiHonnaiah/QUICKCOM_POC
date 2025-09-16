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
/*!        \file  ara/crypto/x509/basic_cert_info.h
 *        \brief  Basic Certificate Information interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_BASIC_CERT_INFO_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_BASIC_CERT_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/cryp/x509_public_key_info.h"
#include "ara/crypto/x509/x509_dn.h"

namespace ara {
namespace crypto {
namespace x509 {

/*!
 * \brief Basic Certificate Information interface.
 * \vpublic
 */
class BasicCertInfo : public ara::crypto::Serializable {
 public:
  /*!
   * \brief X.509 v3 Key Constraints type definition.
   * \vpublic
   */
  using KeyConstraints = std::uint32_t;

  /*!
   * \brief No key constraints.
   * \vpublic
   */
  static KeyConstraints const kConstrNone{0};

  /*!
   * \brief The key can be used for digital signature production.
   * \vpublic
   */
  static KeyConstraints const kConstrDigitalSignature{0x08000};

  /*!
   * \brief The key can be used in cases requiring the "non-repudiation" guarantee.
   * \vpublic
   */
  static KeyConstraints const kConstrNonRepudiation{0x04000};

  /*!
   * \brief The key can be used for key encipherment.
   * \vpublic
   */
  static KeyConstraints const kConstrKeyEncipherment{0x02000};

  /*!
   * \brief The key can be used for data encipherment.
   * \vpublic
   */
  static KeyConstraints const kConstrDataEncipherment{0x01000};

  /*!
   * \brief The key can be used for a key agreement protocol execution.
   * \vpublic
   */
  static KeyConstraints const kConstrKeyAgreement{0x00800};

  /*!
   * \brief The key can be used for certificates signing.
   * \vpublic
   */
  static KeyConstraints const kConstrKeyCertSign{0x00400};

  /*!
   * \brief The key can be used for Certificates Revocation Lists (CRL) signing.
   * \vpublic
   */
  static KeyConstraints const kConstrCrlSign{0x00200};

  /*!
   * \brief The encipherment key can be used for enciphering only.
   * \vpublic
   */
  static KeyConstraints const kConstrEncipherOnly{0x00100};

  /*!
   * \brief The encipherment key can be used for deciphering only.
   * \vpublic
   */
  static KeyConstraints const kConstrDecipherOnly{0x00080};

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Load the subject public key information object to realm of specified crypto provider.
   * \details If (crypto_provider == nullptr) then X509PublicKeyInfo object
              will be loaded in realm of the Stack-default Crypto Provider
   * \param[in] crypto_provider shared pointer of a target Crypto Provider, where the public key will be used.
   * \return constant reference of the subject public key interface
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::crypto::cryp::X509PublicKeyInfo const& SubjectPubKey(
      ara::crypto::cryp::CryptoProvider::Sptr crypto_provider = nullptr) const noexcept = 0;

  /*!
   * \brief Get the subject DN.
   * \return subject DN.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual X509DN const& SubjectDn() const noexcept = 0;

  /*!
   * \brief Find out whether this is a CA request.
   * \return true if it is a CA request and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsCa() const noexcept = 0;

  /*!
   * \brief Get the constraint on the path length defined in the Basic Constraints extension.
   * \details A return value equal to std::numeric_limits<std::uint32_t>::max() means no path limit was specified in the
   *          certificate.
   * \return certification path length limit.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint32_t GetPathLimit() const noexcept = 0;

  /*!
   * \brief Get the key constraints for the key associated with this PKCS\#10 object.
   * \return key constraints.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual KeyConstraints GetConstraints() const noexcept = 0;

  /*!
   * \brief Get signature algorithm identifier used to sign this object.
   * \return Signature algorithm identifier.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual auto GetSignatureAlgorithm() const noexcept -> CryptoAlgId = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_BASIC_CERT_INFO_H_

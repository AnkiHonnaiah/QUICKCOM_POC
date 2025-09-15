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
/*!        \file  crypto/x509provider/x509/certificate.h
 *        \brief  Extended certificate interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/x509_certificate.h"
#include "amsr/core/string.h"
#include "ara/crypto/x509/certificate.h"

#include "crypto/x509provider/x509/x509_dn.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief           Extended Certificate interface.
 */
class Certificate : public ara::crypto::x509::Certificate {
 public:
  /*!
   * \brief Structure representing a certificate extension.
   */
  struct Extension {
    amsr::core::String oid_string{""}; /*!< Extension OID string */
    bool critical{false};              /*!< Criticality flag */
  };

  /*!
   * \brief         Unique smart pointer of the interface.
   */
  using Uptr = std::unique_ptr<Certificate, ara::crypto::CustomDeleter>;

  /*!
   * \brief         Unique smart pointer of the interface.
   */
  using Uptrc = std::unique_ptr<Certificate const, ara::crypto::CustomDeleter>;

  /*!
   * \brief         Clone this instance.
   * \return        A unique pointer to the clone of this instance
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto Clone() const noexcept -> Uptr = 0;

  /*!
   * \brief         Sets the certificate status.
   * \param[in]     status The status to be set.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \trace         CREQ-Crypto-X509Provider_X509_Trust_Management
   * \vprivate      Component Private
   */
  virtual void SetStatus(Status status) const noexcept = 0;

  /*!
   * \brief         Get the serial number of the certificate.
   * \return        The serial number
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto GetSerialNumber() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;

  /*!
   * \brief         Get the authority key identifier of the certificate.
   * \details       Unlike AuthorityKeyId() the value is not DER encoded.
   * \return        The authority key identifier
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto GetAuthorityKeyId() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;

  /*!
   * \brief         Get the subject key identifier of the certificate.
   * \details       Unlike SubjectKeyId() the value is not DER encoded.
   * \return        The subject key identifier
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto GetSubjectKeyId() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;

  /*!
   * \brief         Get the extensions of the certificate.
   * \return        The the extensions contained in the certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto GetExtensions() const noexcept -> amsr::core::Span<Extension const> = 0;

  /*!
   * \brief         Determine whether this certificate is self-issued.
   * \return        True if this is self-issued, false otherwise
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto IsSelfIssued() const noexcept -> bool = 0;

  /*!
   * \brief         Determine whether this certificate is self-signed.
   * \return        True if this is self-signed, false otherwise
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto IsSelfSigned() const noexcept -> bool = 0;

  /*!
   * \brief         Get the certificate's issuer distinguished name (concrete type).
   * \return        The issuer DN of this certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto ConcreteIssuerDn() const -> X509DN const& = 0;

  /*!
   * \brief         Get the certificate's subject distinguished name (concrete type).
   * \return        The subject DN af this certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto ConcreteSubjectDn() const noexcept -> X509DN const& = 0;

  /*!
   * \brief         Get the DER encoded binary representation of this certificate.
   * \return        The DER encoded binary representation of this certificate
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto Encode() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_CERTIFICATE_H_
